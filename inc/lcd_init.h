#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "main.h"

#define USE_HORIZONTAL 0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 320

#else
#define LCD_W 320
#define LCD_H 240
#endif



//-----------------LCD端口定义---------------- 

//#define CS_ON 										GPIO_WriteBit(CS_PORT,CS_PIN,Bit_SET)
//#define CS_OFF 										GPIO_WriteBit(CS_PORT,CS_PIN,Bit_RESET)

//#define RES_ON 										GPIO_WriteBit(RES_PORT,RES_PIN,Bit_SET)
//#define RES_OFF 									GPIO_WriteBit(RES_PORT,RES_PIN,Bit_RESET)

//#define DC_ON 										GPIO_WriteBit(DC_PORT,DC_PIN,Bit_SET)
//#define DC_OFF 										GPIO_WriteBit(DC_PORT,DC_PIN,Bit_RESET)

//#define SDA_ON 										GPIO_WriteBit(SDA_PORT,SDA_PIN,Bit_SET)
//#define SDA_OFF 									GPIO_WriteBit(SDA_PORT,SDA_PIN,Bit_RESET)

//#define SCL_ON 										GPIO_WriteBit(SCL_PORT,SCL_PIN,Bit_SET)
//#define SCL_OFF 									GPIO_WriteBit(SCL_PORT,SCL_PIN,Bit_RESET)

//#define BLEN_ON 									GPIO_WriteBit(BLEN_PORT,BLEN_PIN,Bit_SET)
//#define BLEN_OFF 									GPIO_WriteBit(BLEN_PORT,BLEN_PIN,Bit_RESET)

void LCD_GPIO_Init(void);//初始化GPIO
void LCD_WriteByte(u8 dat);//模拟SPI时序
void LCD_WR_Dat8(u8 dat);//写入一个字节
void LCD_WR_Dat16(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif




