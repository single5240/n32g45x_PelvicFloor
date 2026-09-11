#include "ble_protocol.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint8_t s_stop_count;
static uint8_t s_action_count;
static BleProtocolResult_t s_action_result;
static uint8_t s_link_state;
static uint8_t s_danger_timeout_count;

static uint8_t Crc8(const uint8_t *data, size_t length)
{
	uint8_t crc = 0U;
	size_t index;
	uint8_t bit;

	for (index = 0U; index < length; index++)
	{
		crc ^= data[index];
		for (bit = 0U; bit < 8U; bit++)
		{
			crc = ((crc & 0x80U) != 0U) ?
			      (uint8_t)((crc << 1U) ^ 0x07U) : (uint8_t)(crc << 1U);
		}
	}
	return crc;
}

static void StopAll(void)
{
	s_stop_count++;
}

static BleProtocolResult_t UiAction(uint8_t action)
{
	(void)action;
	s_action_count++;
	return s_action_result;
}

static void LinkState(uint8_t connected)
{
	s_link_state = connected;
}

static void RemoteDangerTimeout(void)
{
	s_danger_timeout_count++;
}

static void GetStatus(uint32_t now_ms,
	                  uint8_t status[BLE_PROTOCOL_STATUS_LENGTH])
{
	uint8_t index;

	for (index = 0U; index < BLE_PROTOCOL_STATUS_LENGTH; index++)
	{
		status[index] = index;
	}
	status[12] = (BleProtocol_IsHeartbeatValid(now_ms) != 0U) ? 0x04U : 0U;
}

static size_t ReadResponse(uint8_t *response, size_t capacity)
{
	size_t length = 0U;
	uint8_t data;

	while ((length < capacity) && (BleProtocol_ReadTxByte(&data) != 0U))
	{
		response[length++] = data;
	}
	return length;
}

static void SendFrame(uint8_t command, const uint8_t *data, uint8_t length,
	                  uint32_t now_ms)
{
	uint8_t frame[38U];
	uint8_t index;
	uint8_t frame_length = (uint8_t)(6U + length);

	frame[0] = 0x5AU;
	frame[1] = 0xA5U;
	frame[2] = BLE_PROTOCOL_VERSION;
	frame[3] = length;
	frame[4] = command;
	for (index = 0U; index < length; index++)
	{
		frame[5U + index] = data[index];
	}
	frame[frame_length - 1U] = Crc8(frame, frame_length - 1U);
	for (index = 0U; index < frame_length; index++)
	{
		BleProtocol_InputByte(frame[index], now_ms);
	}
}

static void AssertResponse(uint8_t request_command, uint8_t result,
	                       uint8_t response_data_length)
{
	uint8_t response[38U];
	size_t length = ReadResponse(response, sizeof(response));

	assert(length == (size_t)(8U + response_data_length));
	assert(response[0] == 0x5AU);
	assert(response[1] == 0xA5U);
	assert(response[2] == BLE_PROTOCOL_VERSION);
	assert(response[3] == (uint8_t)(2U + response_data_length));
	assert(response[4] == 0xE0U);
	assert(response[5] == request_command);
	assert(response[6] == result);
	assert(response[length - 1U] == Crc8(response, length - 1U));
}

static void AssertStatusNotification(void)
{
	uint8_t response[38U];
	size_t length = ReadResponse(response, sizeof(response));
	uint8_t index;

	assert(length == (size_t)(6U + BLE_PROTOCOL_STATUS_LENGTH));
	assert(response[0] == 0x5AU);
	assert(response[1] == 0xA5U);
	assert(response[2] == BLE_PROTOCOL_VERSION);
	assert(response[3] == BLE_PROTOCOL_STATUS_LENGTH);
	assert(response[4] == BLE_COMMAND_STATUS_NOTIFY);
	for (index = 0U; index < BLE_PROTOCOL_STATUS_LENGTH; index++)
	{
		assert(response[5U + index] == ((index == 12U) ? 0U : index));
	}
	assert(response[length - 1U] == Crc8(response, length - 1U));
}

int main(void)
{
	BleProtocolCallbacks_t callbacks;
	uint8_t bad_crc[] = {0x5AU, 0xA5U, 0x11U, 0x00U, 0x71U, 0x00U};
	uint8_t action;
	uint8_t byte;
	uint8_t index;
	BleProtocolStats_t stats;

	callbacks.stop_all = StopAll;
	callbacks.ui_action = UiAction;
	callbacks.link_state = LinkState;
	callbacks.remote_danger_timeout = RemoteDangerTimeout;
	callbacks.get_status = GetStatus;
	callbacks.on_command = 0;
	BleProtocol_Init(&callbacks);
	assert(BleProtocol_HasTxData() == 0U);
	assert(Crc8((const uint8_t *)"\x5A\xA5\x11\x00\x71", 5U) == 0x0FU);

	BleProtocol_InputByte(0x00U, 10U);
	BleProtocol_InputByte(0x5AU, 10U);
	SendFrame(0x71U, 0, 0U, 10U);
	assert(BleProtocol_HasTxData() != 0U);
	AssertResponse(0x71U, BLE_RESULT_OK, BLE_PROTOCOL_STATUS_LENGTH);
	assert(BleProtocol_HasTxData() == 0U);
	assert(s_link_state == 1U);
	BleProtocol_NotifyStatus(50U);
	AssertStatusNotification();

	SendFrame(0x90U, 0, 0U, 100U);
	AssertResponse(0x90U, BLE_RESULT_OK, 0U);
	assert(BleProtocol_IsHeartbeatValid(3099U) != 0U);
	assert(BleProtocol_IsHeartbeatValid(3100U) == 0U);

	SendFrame(0x08U, 0, 0U, 200U);
	AssertResponse(0x08U, BLE_RESULT_OK, BLE_PROTOCOL_STATUS_LENGTH);
	assert(s_stop_count == 1U);

	action = 6U;
	s_action_result = BLE_RESULT_SAFETY_LOCK;
	SendFrame(0x10U, &action, 1U, 300U);
	AssertResponse(0x10U, BLE_RESULT_SAFETY_LOCK, 0U);
	assert(s_action_count == 1U);
	action = 3U;
	s_action_result = BLE_RESULT_OK;
	SendFrame(0x10U, &action, 1U, 301U);
	assert(BleProtocol_ReadTxByte(&byte) == 0U);
	BleProtocol_CompleteUiAction(302U);
	AssertResponse(0x10U, BLE_RESULT_OK, BLE_PROTOCOL_STATUS_LENGTH);
	action = 8U;
	SendFrame(0x10U, &action, 1U, 300U);
	AssertResponse(0x10U, BLE_RESULT_BAD_PARAMETER, 0U);

	SendFrame(0x44U, 0, 0U, 400U);
	AssertResponse(0x44U, BLE_RESULT_UNSUPPORTED, 0U);
	SendFrame(0x71U, &action, 1U, 500U);
	AssertResponse(0x71U, BLE_RESULT_BAD_LENGTH, 0U);

	for (index = 0U; index < (uint8_t)sizeof(bad_crc); index++)
	{
		BleProtocol_InputByte(bad_crc[index], 600U);
	}
	assert(BleProtocol_ReadTxByte(&byte) == 0U);

	BleProtocol_InputByte(0x5AU, 700U);
	BleProtocol_InputByte(0xA5U, 700U);
	BleProtocol_Task(900U);
	SendFrame(0x71U, 0, 0U, 901U);
	AssertResponse(0x71U, BLE_RESULT_OK, BLE_PROTOCOL_STATUS_LENGTH);

	BleProtocol_Reset();
	assert(s_link_state == 0U);
	SendFrame(0x90U, 0, 0U, 1000U);
	AssertResponse(0x90U, BLE_RESULT_OK, 0U);
	BleProtocol_SetRemoteDangerActive(1U);
	BleProtocol_Task(4000U);
	assert(s_danger_timeout_count == 0U);
	assert(s_link_state == 0U);
	BleProtocol_Reset();
	assert(s_danger_timeout_count == 1U);
	BleProtocol_GetStats(&stats);
	assert(stats.valid_frames != 0U);
	assert(stats.crc_errors == 1U);
	assert(stats.frame_timeouts == 1U);
	assert(stats.command_length_errors == 1U);
	assert(stats.unsupported_commands == 1U);
	assert(stats.tx_dropped_frames == 0U);

	puts("ble_protocol_test: PASS");
	return 0;
}
