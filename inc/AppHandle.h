#ifndef _APPHANDLE_H
#define _APPHANDLE_H
#include "main.h"

void Bt_Send(uint8_t cmd, uint8_t data);
uint8_t BLE_Checksum(uint8_t *data,uint8_t size);


void Clear_Array(uint8_t arr[], int size);
void App_Event(uint8_t cmd,uint8_t ctr);
void Send_UART_Frame(USART_Module* USARTx, uint8_t *frame, uint16_t size) ;

void Send_BeatPack(void);

#endif
