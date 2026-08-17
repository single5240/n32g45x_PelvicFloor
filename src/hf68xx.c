/**
  ******************************************************************************
  * @author  全威智能科技
  * @version V1.0
  * @date    2021-04-09
  * @brief   HF68XX配置C文件
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "hf68xx.h"

//#include "HC32L130J8TA.h"
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
 
void SglSound(uint8_t ss)///单语音函数,通过输入脉冲个数SS来识别播放相对应的语音
{
	static uint8_t s;
	
//	while (STU_SOUND_BUSY);///等待上一段语音播放完毕
	SOUND_RST_ON;
	Delay10us(10);
	SOUND_RST_OFF;
	Delay1ms(3);
	for (s = 0; s < ss; s++)
	{
			SOUND_DATA_ON;
			Delay10us(10);
			SOUND_DATA_OFF;
			Delay10us(10);
	}
}
 
void MultSounds(uint8_t ss1, uint8_t ss2, uint8_t ss3, uint8_t ss4, uint8_t ss5, uint8_t ss6)///拼接语音函数
{
	static uint8_t s1,s2,s31,s32,s41,s42,s43,s51,s52,s6;
	
	SOUND_RST_ON;///播出第一段语音
	Delay10us(10);
	SOUND_RST_OFF;
	Delay1ms(5);
	for (s1 = 0; s1 < ss1; s1++)
	{
			SOUND_DATA_ON;
			Delay10us(10);
			SOUND_DATA_OFF;
			Delay10us(10);
	}
	Delay1ms(1);
	while (STU_SOUND_BUSY);
	
	if (ss2 != 0)
	{///第二段语音
			SOUND_RST_ON;
			Delay10us(10);
			SOUND_RST_OFF;
			Delay1ms(5);
			for (s2 = 0; s2 < ss2; s2++)
			{
					SOUND_DATA_ON;
					Delay10us(10);
					SOUND_DATA_OFF;
					Delay10us(10);
			}
			Delay1ms(1);
			while (STU_SOUND_BUSY);
	}
	if (ss3 != 0)//几百
	{
			SOUND_RST_ON;
			Delay10us(10);
			SOUND_RST_OFF;
			Delay1ms(5);
			for (s31 = 0; s31 < ss3; s31++)
			{
					SOUND_DATA_ON;
					Delay10us(10);
					SOUND_DATA_OFF;
					Delay10us(10);
			}
			Delay1ms(1);
			while (STU_SOUND_BUSY);
			SOUND_RST_ON;
			Delay10us(10);
			SOUND_RST_OFF;
			Delay1ms(5);
			for (s32 = 0; s32 < 39; s32++)
			{
					SOUND_DATA_ON;
					Delay10us(10);
					SOUND_DATA_OFF;
					Delay10us(10);
			}
			Delay1ms(1);
			while (STU_SOUND_BUSY);
	}
	if (ss4 == 1)
	{
			SOUND_RST_ON;
			Delay10us(10);
			SOUND_RST_OFF;;
			Delay1ms(5);
			for (s41 = 0; s41 < 28; s41++)
			{
					SOUND_DATA_ON;
					Delay10us(10);
					SOUND_DATA_OFF;
					Delay10us(10);
			}
			Delay1ms(1);
			while (STU_SOUND_BUSY);
	}
	else if (ss4 != 0)//几十
	{
			SOUND_RST_ON;
			Delay10us(10);
			SOUND_RST_OFF;
			Delay1ms(5);
			for (s42 = 0; s42 < ss4; s42++)
			{
					SOUND_DATA_ON;
					Delay10us(10);
					SOUND_DATA_OFF;
					Delay10us(10);
			}
			Delay1ms(1);
			while (STU_SOUND_BUSY);
			SOUND_RST_ON;
			Delay10us(10);
			SOUND_RST_OFF;
			Delay1ms(5);
			for (s43 = 0; s43 < 38; s43++)
			{
					SOUND_DATA_ON;
					Delay10us(10);
					SOUND_DATA_OFF;
					Delay10us(10);
			}
			Delay1ms(1);
			while (STU_SOUND_BUSY);
	}
	if (ss5 == 1)//使用次数为零，提醒刷卡
	{
			SOUND_RST_ON;
			Delay10us(10);
			SOUND_RST_OFF;
			Delay1ms(5);
			for (s51 = 0; s51 < 28; s51++)
			{
					SOUND_DATA_ON;
					Delay10us(10);
					SOUND_DATA_OFF;
					Delay10us(10);
			}
			Delay1ms(1);
			while (STU_SOUND_BUSY);
	} 
	else if (ss5 != 0)//几
	{
			SOUND_RST_ON;
			Delay10us(10);
			SOUND_RST_OFF;
			Delay1ms(5);
			for (s52 = 0; s52 < ss5; s52++)
			{
					SOUND_DATA_ON;
					Delay10us(10);
					SOUND_DATA_OFF;
					Delay10us(10);
			}
			Delay1ms(1);
			while (STU_SOUND_BUSY);
	}
	if (ss6 != 0)//次
	{
			SOUND_RST_ON;
			Delay10us(10);
			SOUND_RST_OFF;
			Delay1ms(5);
			for (s6 = 0; s6 < 27; s6++)
			{
					SOUND_DATA_ON;
					Delay10us(10);
					SOUND_DATA_OFF;
					Delay10us(10);
			}
	}
}


/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


