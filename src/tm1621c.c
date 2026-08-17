/**
  ******************************************************************************
  * @author  全威智能科技
  * @version V1.2
  * @date    2020-01-19
  * @brief   INIT配置C文件
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "tm1621c.h"

void LCD_init(void)
{
		comm_LCD(0x80, 0x40); //100命令,关闭LCD偏置发生器
    comm_LCD(0x85, 0x20); //100命令,设置BIAS 1/3
    comm_LCD(0x80, 0x20); //100命令,开启系统振荡器
    comm_LCD(0x80, 0x60); //100命令,开启LCD偏置发生器
    //    write_LCD(0x80, 0x60); //101命令,点亮所有LCD图标
    //    write_LCD(0x80, 0x60); //101命令,清除所有LCD图标
    Delay1us(10);
}
void Allon_LCD(void) 
{
  write_LCD(1,0, 0xff);
	write_LCD(1,2, 0xff);
	write_LCD(1,4, 0xff);
	write_LCD(1,6, 0xff);
	write_LCD(1,8, 0xff);
	write_LCD(1,10, 0xff);
	write_LCD(1,12, 0xff);
	write_LCD(1,14, 0xff);
	write_LCD(1,16, 0xff);
	write_LCD(1,18, 0xff);
	write_LCD(1,20, 0xff);
}
void Alloff_LCD(void) 
{
	write_LCD(1,0, 0);
	write_LCD(1,2, 0);
	write_LCD(1,4, 0);
	write_LCD(1,6, 0);
	write_LCD(1,8, 0);
	write_LCD(1,10, 0);
	write_LCD(1,12, 0);
	write_LCD(1,14, 0);
	write_LCD(1,16, 0);
	write_LCD(1,18, 0);
	write_LCD(1,20, 0);
}
void comm_LCD(uint8_t data1, uint8_t data2)  //初始化LCD数据函数
{ 
		uint8_t i,j;
    TM1621C_CS_OFF;
    Delay1us(10);
		for (i = 0; i < 8; i++) 
	  {
        if ((data1 & 0x80) == 0x80) {
            TM1621C_DATA_ON;
        } else {
            TM1621C_DATA_OFF;
        }
        Delay1us(10);
        TM1621C_CLK_OFF;
        Delay1us(10);
        TM1621C_CLK_ON;
        Delay1us(10);
        data1 <<= 1;
    }
    for (j = 0; j < 8; j++) 
		{
        if ((data2 & 0x80) == 0x80) {
            TM1621C_DATA_ON;
        } else {
            TM1621C_DATA_OFF;
        }
        Delay1us(10);
        TM1621C_CLK_OFF;
        Delay1us(10);
        TM1621C_CLK_ON;
        Delay1us(10);
        data2 <<= 1;
    }
    Delay1us(10);
    TM1621C_CS_ON;
    Delay1us(10);
}
void write_LCD(uint8_t ws,uint8_t add, uint8_t data)  //写LCD数据函数
{ 
		uint8_t i,j;
		uint8_t wc;
		uint8_t addr=add<<2;
		if(ws)////写数据
			wc=WRITECOM;
		else////设置LCD
			wc=SETCOM;
//    TM1621C_CS_ON;
//    TM1621C_CLK_ON;
//    TM1621C_DATA_ON
    Delay1us(10);
    TM1621C_CS_OFF;
    Delay1us(10);
		for (i = 0; i < 3; i++) 
	  {
        if ((wc & 0x80) == 0x80) {
            TM1621C_DATA_ON;
        } else {
            TM1621C_DATA_OFF;
        }
        Delay1us(10);
        TM1621C_CLK_OFF;
        Delay1us(10);
        TM1621C_CLK_ON;
        Delay1us(10);
        wc <<= 1;
    }
    for (i = 0; i < 6; i++) 
	  {
        if ((addr & 0x80) == 0x80) {
            TM1621C_DATA_ON;
        } else {
            TM1621C_DATA_OFF;
        }
        Delay1us(10);
        TM1621C_CLK_OFF;
        Delay1us(10);
        TM1621C_CLK_ON;
        Delay1us(10);
        addr <<= 1;
    }
    for (j = 0; j < 8; j++) 
		{
        if ((data & 0x80) == 0x80) {
            TM1621C_DATA_ON;
        } else {
            TM1621C_DATA_OFF;
        }
        Delay1us(10);
        TM1621C_CLK_OFF;
        Delay1us(10);
        TM1621C_CLK_ON;
        Delay1us(10);
        data <<= 1;
    }
    Delay1us(10);
    TM1621C_CS_ON;
    Delay1us(10);
}



