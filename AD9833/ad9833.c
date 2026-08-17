/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
 * @file delay.c
<<<<<<< .mine
 * @author Nations 
=======
 * @author Nations
>>>>>>> .r78072
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "ad9833.h"
#include "init.h"
#include "main.h"

/** @addtogroup 
 * @{
 */

/**************************************
*   函 数 名: AD9833_Delay
*   功能说明: ad9833延迟
*   形    参: 无
*   返 回 值: 无
*************************************/
static void ad9833_Delay(void)
{
	uint16_t i;
	for(i=0;i<1;i++);
}

void ad9833_delay(uint16_t time)
{
	uint16_t i;
	for(i=0;i<time;i++);

}
/**************************************
*   函 数 名: Write_ad9833
*   功能说明: ad9833写入16位数据
*   形    参: txdata：待写入的16位数据
*   返 回 值: 无
*************************************/
void Write_ad9833(uint16_t d)
{
	uint16_t i,dt;
	
	SCLK_ON;
	SCS_ON;
	ad9833_Delay();
	SCS_OFF;
	
	dt=d;
	for(i=0;i<16;i++)
	{
		if(dt&0x8000)
			SDATA_ON;
		else
			SDATA_OFF;
		ad9833_Delay();
		SCLK_OFF;
		ad9833_Delay();
		SCLK_ON;
		dt=dt<<1;
	}
	SCS_ON;
	SCLK_OFF;
}

/*
*********************************************************************************************************
*	函 数 名: AD9833_WaveSeting
*	功能说明: 向SPI总线发送16个bit数据
*	形    参: 1.Freq: 频率值, 0.1 hz - 12Mhz
			  2.Freq_SFR: 0 或 1
			  3.WaveMode: TRI_WAVE(三角波),SIN_WAVE(正弦波),SQU_WAVE(方波)
			  4.Phase : 波形的初相位
*	返 回 值: 无
*********************************************************************************************************
*/ 
void AD9833_WaveSeting(double Freq,uint16_t Freq_SFR,uint16_t WaveMode,uint16_t Phase)
{
		uint16_t frequence_LSB,frequence_MSB,Phs_data;
		double   frequence_mid,frequence_DATA;
		uint32_t frequence_hex;

		/*********************************计算频率的16进制值***********************************/
		frequence_mid=268435456/25;//适合25M晶振
		//如果时钟频率不为25MHZ，修改该处的频率值，单位MHz ，AD9833最大支持25MHz
		frequence_DATA=Freq;
		frequence_DATA=frequence_DATA/1000000;
		frequence_DATA=frequence_DATA*frequence_mid;
		frequence_hex=frequence_DATA;  //这个frequence_hex的值是32位的一个很大的数字，需要拆分成两个14位进行处理；
		frequence_LSB=frequence_hex; //frequence_hex低16位送给frequence_LSB
		frequence_LSB=frequence_LSB&0x3fff;//去除最高两位，16位数换去掉高位后变成了14位
		frequence_MSB=frequence_hex>>14; //frequence_hex高16位送给frequence_HSB
		frequence_MSB=frequence_MSB&0x3fff;//去除最高两位，16位数换去掉高位后变成了14位

		Phs_data=Phase|0xC000;	//相位值
		Write_ad9833(0x0100); //复位AD9833,即RESET位为1
		Write_ad9833(0x2100); //选择数据一次写入，B28位和RESET位为1

		if(Freq_SFR==0)				  //把数据设置到设置频率寄存器0
		{
		 	frequence_LSB=frequence_LSB|0x4000;
		 	frequence_MSB=frequence_MSB|0x4000;
			 //使用频率寄存器0输出波形
			Write_ad9833(frequence_LSB); //L14，选择频率寄存器0的低14位数据输入
			Write_ad9833(frequence_MSB); //H14 频率寄存器的高14位数据输入
			Write_ad9833(Phs_data);	//设置相位
			//AD9833_Write(0x2000); /**设置FSELECT位为0，芯片进入工作状态,频率寄存器0输出波形**/
	    }
		if(Freq_SFR==1)				//把数据设置到设置频率寄存器1
		{
			 frequence_LSB=frequence_LSB|0x8000;
			 frequence_MSB=frequence_MSB|0x8000;
			//使用频率寄存器1输出波形
			Write_ad9833(frequence_LSB); //L14，选择频率寄存器1的低14位输入
			Write_ad9833(frequence_MSB); //H14 频率寄存器1为
			Write_ad9833(Phs_data);	//设置相位
			//AD9833_Write(0x2800); /**设置FSELECT位为0，设置FSELECT位为1，即使用频率寄存器1的值，芯片进入工作状态,频率寄存器1输出波形**/
		}

		if(WaveMode==TRI_WAVE) //输出三角波波形
		 	Write_ad9833(0x2002); 
		if(WaveMode==SQU_WAVE)	//输出方波波形
			Write_ad9833(0x2028); 
		if(WaveMode==SIN_WAVE)	//输出正弦波形
			Write_ad9833(0x2000); 
}




