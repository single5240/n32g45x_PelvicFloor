#ifndef __SPI_NFC_H__
#define __SPI_NFC_H__


#include "main.h"

//SPI 接口配置

#define SET_SPI_CS      GPIO_WriteBit(RFID_NSS_PORT,RFID_NSS_PIN,Bit_SET);//NSS
#define CLR_SPI_CS      GPIO_WriteBit(RFID_NSS_PORT,RFID_NSS_PIN,Bit_RESET);
#define SET_SPI_CK  	  GPIO_WriteBit(RFID_SCK_PORT,RFID_SCK_PIN,Bit_SET);//CLK，SPI时钟
#define CLR_SPI_CK  	  GPIO_WriteBit(RFID_SCK_PORT,RFID_SCK_PIN,Bit_RESET);
#define SET_SPI_MOSI  	GPIO_WriteBit(RFID_MOSI_PORT,RFID_MOSI_PIN,Bit_SET);//MOSI，从设备数据输入
#define CLR_SPI_MOSI  	GPIO_WriteBit(RFID_MOSI_PORT,RFID_MOSI_PIN,Bit_RESET);
#define STU_SPI_MISO  	GPIO_ReadInputDataBit(RFID_MISO_PORT,RFID_MISO_PIN)			//MISO，从设备数据输出
#define SET_NFC_RST  	  GPIO_WriteBit(RFID_NRST_PORT,RFID_NRST_PIN,Bit_SET);//复位,低电平有效
#define CLR_NFC_RST  	  GPIO_WriteBit(RFID_NRST_PORT,RFID_NRST_PIN,Bit_RESET);

#define IRQ  GPIO_ReadInputDataBit(RFID_IRQ_PORT,RFID_IRQ_PIN)

#endif


