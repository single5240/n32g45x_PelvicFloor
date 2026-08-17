/**
  ******************************************************************************
  * @author  全威智能科技
  * @version V3.0
  * @date    2020-01-09
  * @brief   LTC6903配置C文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	
  * 淘宝	:	
  * 阿里巴巴:	
  ******************************************************************************
  */


#include "ltc6903.h"
//#include "main.h"



//------------------------------------------------/
//函数功能:通过SPI写读一字节                    /
//输入参数:oct: 频率范围参数，不同的频率范围有不同的OCT值，                              /
//					DAC：频率值
///					CNF:0=两个都有输出，相位差180度，
//------------------------------------------------/
void SPI_WR_Byte(uint8_t oct,uint16_t dac,uint8_t cnf)
{
	static uint8_t i,j;
	uint16_t odc;
	uint8_t hd,ld;

	odc=(uint16_t)((oct<<12)|(dac<<2)|cnf);
	hd=(uint8_t)(odc>>8);
	ld=(uint8_t)(odc&0x0ff);
	SEN_OFF;
	Delay1ms(1);
	SCK_OFF;
	Delay1ms(1);
	for(i=0;i<8;i++)
	{
		if(hd&0x80)
			SDI_ON;
		else
			SDI_OFF;
		hd<<=1;
		Delay1us(10);
		SCK_ON;
		Delay1us(10);
		SCK_OFF;
	}
	for(j=0;j<8;j++)
	{
		if(ld&0x80)
			SDI_ON;
		else
			SDI_OFF;
		ld<<=1;
		Delay1us(10);
		SCK_ON;
		Delay1us(10);
		SCK_OFF;
	}
	Delay1ms(1);
	SEN_ON;
}

