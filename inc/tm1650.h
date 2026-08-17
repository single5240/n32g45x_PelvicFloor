/**
  ******************************************************************************
  * @author  全威智能科技
  * @version V1.0
  * @date    2021-04-27
  * @brief   TM1650配置H文件
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#ifndef __TM1650_H__
#define __TM1650_H__


/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/

#include "init.h"

#define TM1650COM			0x48////TM1650系统命令
#define TM1650SYS 		0x01////系统参数，BIT7-BIT4,亮度设置,1-7:1-7级亮度，0：默认8级亮度.BIT3:段模式，0:默认8段，1:7段。BIT2:工作模式，0：正常，1：待机，BIT1:默认0，BIT0:显示开关，0：关，1：开
#define TM1650WAY1		0x68///左起第1位数码管地址
#define TM1650WAY2		0x6a///左起第2位数码管地址
#define TM1650WAY3		0x6c///左起第3位数码管地址
#define TM1650WAY4		0x6e///左起第4位数码管地址

#define LED_CLK_ON 				GPIO_WriteBit(LED_CLK_PORT,LED_CLK_PIN,Bit_SET)
#define LED_CLK_OFF				GPIO_WriteBit(LED_CLK_PORT,LED_CLK_PIN,Bit_RESET)
#define LED_DAT_ON				GPIO_WriteBit(LED_DAT_PORT,LED_DAT_PIN,Bit_SET)
#define LED_DAT_OFF				GPIO_WriteBit(LED_DAT_PORT,LED_DAT_PIN,Bit_RESET)

//#define LED_CLK1_ON				GPIO_WriteBit(LED_CLK1_PORT,LED_CLK1_PIN,Bit_SET)
//#define LED_CLK1_OFF			GPIO_WriteBit(LED_CLK1_PORT,LED_CLK1_PIN,Bit_RESET)
//#define LED_DAT1_ON				GPIO_WriteBit(LED_DAT1_PORT,LED_DAT1_PIN,Bit_SET)
//#define LED_DAT1_OFF			GPIO_WriteBit(LED_DAT1_PORT,LED_DAT1_PIN,Bit_RESET)

void write_tm1650(uint8_t temp);
//void write_tm16501(uint8_t temp);
void Init_tm1650(void);
//void Init_tm16501(void);
void Display_Tm1650(uint8_t a,uint8_t b,uint8_t c,uint8_t d);
//void Display_Tm16501(uint8_t a,uint8_t b,uint16_t c,uint8_t d);
void StartPrg(void);
//void StartPrg1(void);
void StopPrg(void);
//void StopPrg1(void);
void AckPrg(void);
//void AckPrg1(void);
	


#endif /* __TM1650_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
