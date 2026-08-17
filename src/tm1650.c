/**
  ******************************************************************************
  * @author  全威智能科技
  * @version V1.0
  * @date    2021-04-27
  * @brief   TM1650配置C文件
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "tm1650.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/


/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
 
///TM1650通讯SPI函数
void write_tm1650(uint8_t temp)
{
	static uint8_t i;
  for (i = 0; i < 8; i++)
	{
		if ((temp & 0x80) == 0x80)
		{
			LED_DAT_ON;
		} 
		else
		{
			LED_DAT_OFF;
		}
//		Delay10us(1);
		LED_CLK_ON;
		Delay10us(1);
		LED_CLK_OFF;
		temp = temp << 1;
  }
}

//void write_tm16501(uint8_t temp)
//{
//	static uint8_t i;
//  for (i = 0; i < 8; i++)
//	{
//		if ((temp & 0x80) == 0x80)
//		{
//				LED_DAT1_ON;
//		} else {
//				LED_DAT1_OFF;
//		}
////		Delay10us(1);
//		LED_CLK1_ON;
//		Delay10us(1);
//		LED_CLK1_OFF;
//		temp = temp << 1;
//  }
//}

void StartPrg(void)
{
	LED_DAT_OFF;
	Delay1us(1);
	LED_CLK_OFF;
	Delay1us(1);
}

void StopPrg(void)
{
	Delay1us(1);
	LED_CLK_ON;
	Delay1us(1);
	LED_DAT_ON;
}

void AckPrg(void)
{
	Delay1us(1);
	LED_DAT_OFF;
	LED_CLK_ON;
	Delay1us(1);
	LED_CLK_OFF;
	Delay1us(1);
}

//void StartPrg1(void)
//{
//	LED_DAT1_OFF;
//	Delay1us(1);
//	LED_CLK1_OFF;
//	Delay1us(1);
//}

//void StopPrg1(void)
//{
//	Delay1us(1);
//	LED_CLK1_ON;
//	Delay1us(1);
//	LED_DAT1_ON;
//}

//void AckPrg1(void)
//{
//	Delay1us(1);
//	LED_DAT1_OFF;
//	LED_CLK1_ON;
//	Delay1us(1);
//	LED_CLK1_OFF;
//	Delay1us(1);
//}

void Init_tm1650(void)
{
	StartPrg();
	write_tm1650(TM1650COM);
	AckPrg();
	write_tm1650(TM1650SYS);
	AckPrg();
	StopPrg();
}

//void Init_tm16501(void)
//{
//	StartPrg1();
//	write_tm16501(TM1650COM);
//	AckPrg1();
//	write_tm16501(TM1650SYS);
//	AckPrg1();
//	StopPrg1();
//}

void Display_Tm1650(uint8_t a,uint8_t b,uint8_t c,uint8_t d)
{
	StartPrg();
	write_tm1650(TM1650WAY1);
	AckPrg();
	write_tm1650(a);
	AckPrg();
	StopPrg();
	StartPrg();
	write_tm1650(TM1650WAY2);
	AckPrg();
	write_tm1650(b);
	AckPrg();
	StopPrg();
	StartPrg();
	write_tm1650(TM1650WAY3);
	AckPrg();
	write_tm1650(c);
	AckPrg();
	StopPrg();
	StartPrg();
	write_tm1650(TM1650WAY4);
	AckPrg();
	write_tm1650(d);
	AckPrg();
	StopPrg();
}

//void Display_Tm16501(uint8_t a,uint8_t b,uint16_t c,uint8_t d)
//{
//	StartPrg1();
//	write_tm16501(TM1650WAY1);
//	AckPrg1();
//	write_tm16501(a);
//	AckPrg1();
//	StopPrg1();
//	StartPrg1();
//	write_tm16501(TM1650WAY2);
//	AckPrg1();
//	write_tm16501(b);
//	AckPrg1();
//	StopPrg1();
//	StartPrg1();
//	write_tm16501(TM1650WAY3);
//	AckPrg1();
//	write_tm16501(c);
//	AckPrg1();
//	StopPrg1();
//	StartPrg1();
//	write_tm16501(TM1650WAY4);
//	AckPrg1();
//	write_tm16501(d);
//	AckPrg1();
//	StopPrg1();
//}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


