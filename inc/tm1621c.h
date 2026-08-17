/**
  ******************************************************************************
  * @author  全威智能科技
  * @version V1.2
  * @date    2021-01-19
  * @brief   init配置H文件
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#ifndef __TM1621D_H__
#define __TM1621D_H__

#include "init.h"

#define WRITECOM			0xa0
#define SETCOM				0x80

void LCD_init(void);
void Allon_LCD(void);
void Alloff_LCD(void);
void comm_LCD(uint8_t data1, uint8_t data2); 
void write_LCD(uint8_t ws,uint8_t add, uint8_t data);

#endif

