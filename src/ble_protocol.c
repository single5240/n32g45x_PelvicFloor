#include "ble_protocol.h"

#include <string.h>

#define BLE_FRAME_HEADER_1             0x5AU
#define BLE_FRAME_HEADER_2             0xA5U
#define BLE_MAX_DATA_LENGTH            32U
#define BLE_MAX_FRAME_LENGTH           (6U + BLE_MAX_DATA_LENGTH)
#define BLE_RESPONSE_COMMAND           0xE0U
#define BLE_COMMAND_STOP_ALL            0x08U
#define BLE_COMMAND_UI_ACTION           0x10U
#define BLE_COMMAND_GET_STATUS          0x71U
#define BLE_COMMAND_LINK_KEEPALIVE      0x90U
#define BLE_RX_FRAME_TIMEOUT_MS         200U
#define BLE_HEARTBEAT_TIMEOUT_MS        3000U
#define BLE_TX_BUFFER_SIZE              512U

#if ((BLE_TX_BUFFER_SIZE & (BLE_TX_BUFFER_SIZE - 1U)) != 0U) || \
    (BLE_TX_BUFFER_SIZE > 65536U)
#error "BLE_TX_BUFFER_SIZE must be a power of two within uint16_t index range"
#endif

typedef BleProtocolResult_t (*BleCommandHandler_t)(const uint8_t *data,
	                                                uint8_t length,
	                                                uint32_t now_ms,
	                                                uint8_t *response,
	                                                uint8_t *response_length);

typedef struct
{
	uint8_t command;
	uint8_t request_length;
	BleCommandHandler_t handler;
} BleCommandEntry_t;

typedef struct
{
	uint8_t frame[BLE_MAX_FRAME_LENGTH];
	uint8_t frame_length;
	uint8_t expected_length;
	uint32_t last_byte_ms;
	uint8_t tx_buffer[BLE_TX_BUFFER_SIZE];
	volatile uint16_t tx_read_index;
	volatile uint16_t tx_write_index;
	uint32_t last_heartbeat_ms;
	uint32_t last_link_activity_ms;
	uint8_t heartbeat_received;
	uint8_t link_active;
	uint8_t remote_danger_active;
	uint8_t ui_action_pending;
	uint8_t defer_response;
	BleProtocolCallbacks_t callbacks;
	BleProtocolStats_t stats;
} BleProtocolContext_t;

static BleProtocolContext_t s_ble;

static BleProtocolResult_t BleCommand_StopAll(const uint8_t *data,
	                                           uint8_t length,
	                                           uint32_t now_ms,
	                                           uint8_t *response,
	                                           uint8_t *response_length);
static BleProtocolResult_t BleCommand_SetStrength(const uint8_t *data,
                                                 uint8_t length,
                                                 uint32_t now_ms,
                                                 uint8_t *response,
                                                 uint8_t *response_length);
static BleProtocolResult_t BleCommand_UiAction(const uint8_t *data,
	                                            uint8_t length,
	                                            uint32_t now_ms,
	                                            uint8_t *response,
	                                            uint8_t *response_length);
static BleProtocolResult_t BleCommand_GetStatus(const uint8_t *data,
	                                             uint8_t length,
	                                             uint32_t now_ms,
	                                             uint8_t *response,
	                                             uint8_t *response_length);
static BleProtocolResult_t BleCommand_Keepalive(const uint8_t *data,
	                                             uint8_t length,
	                                             uint32_t now_ms,
	                                             uint8_t *response,
	                                             uint8_t *response_length);

static const BleCommandEntry_t s_command_table[] =
{
	{BLE_COMMAND_SET_STRENGTH,   1U, BleCommand_SetStrength},
	{BLE_COMMAND_STOP_ALL,       0U, BleCommand_StopAll},
	{BLE_COMMAND_UI_ACTION,      1U, BleCommand_UiAction},
	{BLE_COMMAND_GET_STATUS,     0U, BleCommand_GetStatus},
	{BLE_COMMAND_LINK_KEEPALIVE, 0U, BleCommand_Keepalive}
};

static const uint8_t s_crc8_table[256] =
{
	0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D,
	0x70,0x77,0x7E,0x79,0x6C,0x6B,0x62,0x65,0x48,0x4F,0x46,0x41,0x54,0x53,0x5A,0x5D,
	0xE0,0xE7,0xEE,0xE9,0xFC,0xFB,0xF2,0xF5,0xD8,0xDF,0xD6,0xD1,0xC4,0xC3,0xCA,0xCD,
	0x90,0x97,0x9E,0x99,0x8C,0x8B,0x82,0x85,0xA8,0xAF,0xA6,0xA1,0xB4,0xB3,0xBA,0xBD,
	0xC7,0xC0,0xC9,0xCE,0xDB,0xDC,0xD5,0xD2,0xFF,0xF8,0xF1,0xF6,0xE3,0xE4,0xED,0xEA,
	0xB7,0xB0,0xB9,0xBE,0xAB,0xAC,0xA5,0xA2,0x8F,0x88,0x81,0x86,0x93,0x94,0x9D,0x9A,
	0x27,0x20,0x29,0x2E,0x3B,0x3C,0x35,0x32,0x1F,0x18,0x11,0x16,0x03,0x04,0x0D,0x0A,
	0x57,0x50,0x59,0x5E,0x4B,0x4C,0x45,0x42,0x6F,0x68,0x61,0x66,0x73,0x74,0x7D,0x7A,
	0x89,0x8E,0x87,0x80,0x95,0x92,0x9B,0x9C,0xB1,0xB6,0xBF,0xB8,0xAD,0xAA,0xA3,0xA4,
	0xF9,0xFE,0xF7,0xF0,0xE5,0xE2,0xEB,0xEC,0xC1,0xC6,0xCF,0xC8,0xDD,0xDA,0xD3,0xD4,
	0x69,0x6E,0x67,0x60,0x75,0x72,0x7B,0x7C,0x51,0x56,0x5F,0x58,0x4D,0x4A,0x43,0x44,
	0x19,0x1E,0x17,0x10,0x05,0x02,0x0B,0x0C,0x21,0x26,0x2F,0x28,0x3D,0x3A,0x33,0x34,
	0x4E,0x49,0x40,0x47,0x52,0x55,0x5C,0x5B,0x76,0x71,0x78,0x7F,0x6A,0x6D,0x64,0x63,
	0x3E,0x39,0x30,0x37,0x22,0x25,0x2C,0x2B,0x06,0x01,0x08,0x0F,0x1A,0x1D,0x14,0x13,
	0xAE,0xA9,0xA0,0xA7,0xB2,0xB5,0xBC,0xBB,0x96,0x91,0x98,0x9F,0x8A,0x8D,0x84,0x83,
	0xDE,0xD9,0xD0,0xD7,0xC2,0xC5,0xCC,0xCB,0xE6,0xE1,0xE8,0xEF,0xFA,0xFD,0xF4,0xF3
};

static uint8_t BleProtocol_Crc8(const uint8_t *data, uint8_t length)
{
	uint8_t crc = 0U;
	uint8_t index;

	for (index = 0U; index < length; index++)
	{
		crc = s_crc8_table[crc ^ data[index]];
	}
	return crc;
}

static uint16_t BleProtocol_TxFree(void)
{
	return (uint16_t)((s_ble.tx_read_index - s_ble.tx_write_index - 1U) &
	                  (BLE_TX_BUFFER_SIZE - 1U));
}

static void BleProtocol_QueueResponse(uint8_t request_command,
	                                  BleProtocolResult_t result,
	                                  const uint8_t *response,
	                                  uint8_t response_length)
{
	uint8_t frame[BLE_MAX_FRAME_LENGTH];
	uint8_t data_length = (uint8_t)(2U + response_length);
	uint8_t frame_length = (uint8_t)(6U + data_length);
	uint8_t index;

	if ((response_length > (BLE_MAX_DATA_LENGTH - 2U)) ||
	    (BleProtocol_TxFree() < frame_length))
	{
		s_ble.stats.tx_dropped_frames++;
		return;
	}

	frame[0] = BLE_FRAME_HEADER_1;
	frame[1] = BLE_FRAME_HEADER_2;
	frame[2] = BLE_PROTOCOL_VERSION;
	frame[3] = data_length;
	frame[4] = BLE_RESPONSE_COMMAND;
	frame[5] = request_command;
	frame[6] = (uint8_t)result;
	for (index = 0U; index < response_length; index++)
	{
		frame[7U + index] = response[index];
	}
	frame[frame_length - 1U] = BleProtocol_Crc8(frame,
	                                          (uint8_t)(frame_length - 1U));

	for (index = 0U; index < frame_length; index++)
	{
		s_ble.tx_buffer[s_ble.tx_write_index] = frame[index];
		s_ble.tx_write_index = (uint16_t)((s_ble.tx_write_index + 1U) &
		                                       (BLE_TX_BUFFER_SIZE - 1U));
	}
	s_ble.stats.tx_frames++;
}

static void BleProtocol_QueueNotification(uint8_t command,
	                                       const uint8_t *data,
	                                       uint8_t data_length)
{
	uint8_t frame[BLE_MAX_FRAME_LENGTH];
	uint8_t frame_length = (uint8_t)(6U + data_length);
	uint8_t index;

	if ((data_length > BLE_MAX_DATA_LENGTH) ||
	    (BleProtocol_TxFree() < frame_length))
	{
		s_ble.stats.tx_dropped_frames++;
		return;
	}

	frame[0] = BLE_FRAME_HEADER_1;
	frame[1] = BLE_FRAME_HEADER_2;
	frame[2] = BLE_PROTOCOL_VERSION;
	frame[3] = data_length;
	frame[4] = command;
	for (index = 0U; index < data_length; index++)
	{
		frame[5U + index] = data[index];
	}
	frame[frame_length - 1U] = BleProtocol_Crc8(frame,
	                                          (uint8_t)(frame_length - 1U));

	for (index = 0U; index < frame_length; index++)
	{
		s_ble.tx_buffer[s_ble.tx_write_index] = frame[index];
		s_ble.tx_write_index = (uint16_t)((s_ble.tx_write_index + 1U) &
		                                       (BLE_TX_BUFFER_SIZE - 1U));
	}
	s_ble.stats.tx_frames++;
}

static void BleProtocol_Dispatch(const uint8_t *frame, uint32_t now_ms)
{
	uint8_t command = frame[4];
	uint8_t length = frame[3];
	uint8_t response[BLE_PROTOCOL_STATUS_LENGTH];
	uint8_t response_length = 0U;
	uint8_t index;
	BleProtocolResult_t result = BLE_RESULT_UNSUPPORTED;
	s_ble.defer_response = 0U;

	if (s_ble.callbacks.on_command != 0)
	{
		s_ble.callbacks.on_command(command, &frame[5], length, now_ms);
	}

	for (index = 0U;
	     index < (uint8_t)(sizeof(s_command_table) / sizeof(s_command_table[0]));
	     index++)
	{
		if (s_command_table[index].command == command)
		{
			if (length != s_command_table[index].request_length)
			{
				s_ble.stats.command_length_errors++;
				result = BLE_RESULT_BAD_LENGTH;
			}
			else
			{
				result = s_command_table[index].handler(&frame[5], length,
				                                                now_ms, response,
				                                                &response_length);
			}
			break;
		}
	}
	if (result == BLE_RESULT_UNSUPPORTED)
	{
		s_ble.stats.unsupported_commands++;
	}

	if (s_ble.defer_response == 0U)
	{
		BleProtocol_QueueResponse(command, result, response, response_length);
	}
}

static void BleProtocol_ProcessFrame(uint32_t now_ms)
{
	uint8_t crc;

	if (s_ble.frame[2] != BLE_PROTOCOL_VERSION)
	{
		s_ble.stats.version_errors++;
		return;
	}

	crc = BleProtocol_Crc8(s_ble.frame,
	                       (uint8_t)(s_ble.expected_length - 1U));
	if (crc != s_ble.frame[s_ble.expected_length - 1U])
	{
		s_ble.stats.crc_errors++;
		return;
	}
	s_ble.stats.valid_frames++;
	s_ble.last_link_activity_ms = now_ms;
	if (s_ble.link_active == 0U)
	{
		s_ble.link_active = 1U;
		if (s_ble.callbacks.link_state != 0)
		{
			s_ble.callbacks.link_state(1U);
		}
	}

	BleProtocol_Dispatch(s_ble.frame, now_ms);
}

void BleProtocol_Init(const BleProtocolCallbacks_t *callbacks)
{
	memset(&s_ble, 0, sizeof(s_ble));
	if (callbacks != 0)
	{
		s_ble.callbacks = *callbacks;
	}
}

void BleProtocol_Reset(void)
{
	if ((s_ble.remote_danger_active != 0U) &&
	    (s_ble.callbacks.remote_danger_timeout != 0))
	{
		s_ble.callbacks.remote_danger_timeout();
	}
	if ((s_ble.link_active != 0U) && (s_ble.callbacks.link_state != 0))
	{
		s_ble.callbacks.link_state(0U);
	}
	s_ble.frame_length = 0U;
	s_ble.expected_length = 0U;
	s_ble.tx_read_index = 0U;
	s_ble.tx_write_index = 0U;
	s_ble.last_heartbeat_ms = 0U;
	s_ble.heartbeat_received = 0U;
	s_ble.last_link_activity_ms = 0U;
	s_ble.link_active = 0U;
	s_ble.remote_danger_active = 0U;
	s_ble.ui_action_pending = 0U;
	s_ble.defer_response = 0U;
}

void BleProtocol_InputByte(uint8_t data, uint32_t now_ms)
{
	if (s_ble.frame_length == 0U)
	{
		if (data != BLE_FRAME_HEADER_1)
		{
			s_ble.stats.sync_errors++;
			return;
		}
		s_ble.frame[0] = data;
		s_ble.frame_length = 1U;
	}
	else if (s_ble.frame_length == 1U)
	{
		if (data == BLE_FRAME_HEADER_2)
		{
			s_ble.frame[1] = data;
			s_ble.frame_length = 2U;
		}
		else if (data != BLE_FRAME_HEADER_1)
		{
			s_ble.stats.sync_errors++;
			s_ble.frame_length = 0U;
		}
	}
	else
	{
		s_ble.frame[s_ble.frame_length++] = data;
		if (s_ble.frame_length == 4U)
		{
			if (s_ble.frame[3] > BLE_MAX_DATA_LENGTH)
			{
				s_ble.stats.length_errors++;
				s_ble.frame_length = 0U;
				s_ble.expected_length = 0U;
				return;
			}
			s_ble.expected_length = (uint8_t)(6U + s_ble.frame[3]);
		}
		if ((s_ble.expected_length != 0U) &&
		    (s_ble.frame_length == s_ble.expected_length))
		{
			BleProtocol_ProcessFrame(now_ms);
			s_ble.frame_length = 0U;
			s_ble.expected_length = 0U;
		}
	}
	s_ble.last_byte_ms = now_ms;
}

void BleProtocol_Task(uint32_t now_ms)
{
	if ((s_ble.frame_length != 0U) &&
	    ((uint32_t)(now_ms - s_ble.last_byte_ms) >= BLE_RX_FRAME_TIMEOUT_MS))
	{
		s_ble.stats.frame_timeouts++;
		s_ble.frame_length = 0U;
		s_ble.expected_length = 0U;
	}
	if ((s_ble.link_active != 0U) &&
	    ((uint32_t)(now_ms - s_ble.last_link_activity_ms) >=
	     BLE_HEARTBEAT_TIMEOUT_MS))
	{
		s_ble.link_active = 0U;
		if (s_ble.callbacks.link_state != 0)
		{
			s_ble.callbacks.link_state(0U);
		}
	}
}

void BleProtocol_CompleteUiAction(uint32_t now_ms)
{
	uint8_t response[BLE_PROTOCOL_STATUS_LENGTH];

	if (s_ble.ui_action_pending == 0U)
	{
		return;
	}
	if (s_ble.callbacks.get_status == 0)
	{
		BleProtocol_QueueResponse(BLE_COMMAND_UI_ACTION,
		                          BLE_RESULT_INTERNAL_ERROR, 0, 0U);
	}
	else
	{
		s_ble.callbacks.get_status(now_ms, response);
		BleProtocol_QueueResponse(BLE_COMMAND_UI_ACTION, BLE_RESULT_OK,
		                          response, BLE_PROTOCOL_STATUS_LENGTH);
	}
	s_ble.ui_action_pending = 0U;
}

void BleProtocol_NotifyStatus(uint32_t now_ms)
{
	uint8_t status[BLE_PROTOCOL_STATUS_LENGTH];

	if (s_ble.callbacks.get_status == 0)
	{
		return;
	}

	s_ble.callbacks.get_status(now_ms, status);
	BleProtocol_QueueNotification(BLE_COMMAND_STATUS_NOTIFY, status,
	                              BLE_PROTOCOL_STATUS_LENGTH);
}

void BleProtocol_NotifyTherapyStart(uint8_t channel, uint8_t profile)
{
	uint8_t data[2];

	data[0] = channel;
	data[1] = profile;
	BleProtocol_QueueNotification(BLE_COMMAND_THERAPY_START_NOTIFY,
	                              data, (uint8_t)sizeof(data));
}

void BleProtocol_NotifyTherapyEnd(uint16_t duration_seconds)
{
	uint8_t data[2];

	data[0] = (uint8_t)(duration_seconds & 0xFFU);
	data[1] = (uint8_t)(duration_seconds >> 8U);
	BleProtocol_QueueNotification(BLE_COMMAND_THERAPY_END_NOTIFY,
	                              data, (uint8_t)sizeof(data));
}

void BleProtocol_SetRemoteDangerActive(uint8_t active)
{
	s_ble.remote_danger_active = (active != 0U) ? 1U : 0U;
}

uint8_t BleProtocol_ReadTxByte(uint8_t *data)
{
	if ((data == 0) || (s_ble.tx_read_index == s_ble.tx_write_index))
	{
		return 0U;
	}

	*data = s_ble.tx_buffer[s_ble.tx_read_index];
	s_ble.tx_read_index = (uint16_t)((s_ble.tx_read_index + 1U) &
	                                 (BLE_TX_BUFFER_SIZE - 1U));
	return 1U;
}

uint8_t BleProtocol_HasTxData(void)
{
	return (s_ble.tx_read_index != s_ble.tx_write_index) ? 1U : 0U;
}

void BleProtocol_GetStats(BleProtocolStats_t *stats)
{
	if (stats != 0)
	{
		*stats = s_ble.stats;
	}
}

uint8_t BleProtocol_IsHeartbeatValid(uint32_t now_ms)
{
	if (s_ble.heartbeat_received == 0U)
	{
		return 0U;
	}
	return ((uint32_t)(now_ms - s_ble.last_heartbeat_ms) <
	        BLE_HEARTBEAT_TIMEOUT_MS) ? 1U : 0U;
}

static BleProtocolResult_t BleCommand_StopAll(const uint8_t *data,
	                                           uint8_t length,
	                                           uint32_t now_ms,
	                                           uint8_t *response,
	                                           uint8_t *response_length)
{
	(void)data;
	(void)length;
	if ((s_ble.callbacks.stop_all == 0) ||
	    (s_ble.callbacks.get_status == 0))
	{
		return BLE_RESULT_INTERNAL_ERROR;
	}

	s_ble.callbacks.stop_all();
	s_ble.callbacks.get_status(now_ms, response);
	*response_length = BLE_PROTOCOL_STATUS_LENGTH;
	return BLE_RESULT_OK;
}

static BleProtocolResult_t BleCommand_SetStrength(const uint8_t *data,
                                                 uint8_t length,
                                                 uint32_t now_ms,
                                                 uint8_t *response,
                                                 uint8_t *response_length)
{
	BleProtocolResult_t result;

	(void)length;
	if ((s_ble.callbacks.set_strength == 0) ||
	    (s_ble.callbacks.get_status == 0))
	{
		return BLE_RESULT_INTERNAL_ERROR;
	}

	result = s_ble.callbacks.set_strength(data[0]);
	if (result == BLE_RESULT_OK)
	{
		s_ble.callbacks.get_status(now_ms, response);
		*response_length = BLE_PROTOCOL_STATUS_LENGTH;
	}
	return result;
}

static BleProtocolResult_t BleCommand_UiAction(const uint8_t *data,
	                                            uint8_t length,
	                                            uint32_t now_ms,
	                                            uint8_t *response,
	                                            uint8_t *response_length)
{
	BleProtocolResult_t result;

	(void)length;
	(void)now_ms;
	(void)response;
	(void)response_length;

	if ((data[0] == 0U) || (data[0] > 7U))
	{
		return BLE_RESULT_BAD_PARAMETER;
	}
	if (s_ble.ui_action_pending != 0U)
	{
		return BLE_RESULT_BUSY;
	}
	if (s_ble.callbacks.ui_action == 0)
	{
		return BLE_RESULT_INTERNAL_ERROR;
	}

	result = s_ble.callbacks.ui_action(data[0]);
	if (result == BLE_RESULT_OK)
	{
		s_ble.ui_action_pending = 1U;
		s_ble.defer_response = 1U;
	}
	return result;
}

static BleProtocolResult_t BleCommand_GetStatus(const uint8_t *data,
	                                             uint8_t length,
	                                             uint32_t now_ms,
	                                             uint8_t *response,
	                                             uint8_t *response_length)
{
	(void)data;
	(void)length;
	if (s_ble.callbacks.get_status == 0)
	{
		return BLE_RESULT_INTERNAL_ERROR;
	}

	s_ble.callbacks.get_status(now_ms, response);
	*response_length = BLE_PROTOCOL_STATUS_LENGTH;
	return BLE_RESULT_OK;
}

static BleProtocolResult_t BleCommand_Keepalive(const uint8_t *data,
	                                             uint8_t length,
	                                             uint32_t now_ms,
	                                             uint8_t *response,
	                                             uint8_t *response_length)
{
	(void)data;
	(void)length;
	(void)response;
	(void)response_length;
	s_ble.last_heartbeat_ms = now_ms;
	s_ble.heartbeat_received = 1U;
	return BLE_RESULT_OK;
}
