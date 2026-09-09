#ifndef __BLE_PROTOCOL_H__
#define __BLE_PROTOCOL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define BLE_PROTOCOL_VERSION          0x11U
#define BLE_PROTOCOL_STATUS_LENGTH    15U

typedef enum
{
	BLE_RESULT_OK = 0x00U,
	BLE_RESULT_BAD_LENGTH = 0x01U,
	BLE_RESULT_BAD_PARAMETER = 0x02U,
	BLE_RESULT_UNSUPPORTED = 0x03U,
	BLE_RESULT_STATE_CONFLICT = 0x04U,
	BLE_RESULT_CHARGING_LOCK = 0x05U,
	BLE_RESULT_SAFETY_LOCK = 0x06U,
	BLE_RESULT_BUSY = 0x08U,
	BLE_RESULT_INTERNAL_ERROR = 0x09U
} BleProtocolResult_t;

typedef struct
{
	void (*stop_all)(void);
	BleProtocolResult_t (*ui_action)(uint8_t action);
	void (*link_state)(uint8_t connected);
	void (*remote_danger_timeout)(void);
	void (*get_status)(uint32_t now_ms,
	                   uint8_t status[BLE_PROTOCOL_STATUS_LENGTH]);
} BleProtocolCallbacks_t;

void BleProtocol_Init(const BleProtocolCallbacks_t *callbacks);
void BleProtocol_Reset(void);
void BleProtocol_InputByte(uint8_t data, uint32_t now_ms);
void BleProtocol_Task(uint32_t now_ms);
void BleProtocol_CompleteUiAction(uint32_t now_ms);
void BleProtocol_SetRemoteDangerActive(uint8_t active);
uint8_t BleProtocol_ReadTxByte(uint8_t *data);
uint8_t BleProtocol_IsHeartbeatValid(uint32_t now_ms);

#ifdef __cplusplus
}
#endif

#endif
