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
 * @file main.c
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "main.h"

// LCD数字数组
const uint8_t NUM1[] = {0xaf, 0x06, 0x6d, 0x4f, 0xc6, 0xcb, 0xeb, 0x0e, 0xef, 0xcf, 0x00};
const uint8_t NUM2[] = {0x5f, 0x06, 0x3d, 0x2f, 0x66, 0x6b, 0x7b, 0x0e, 0x7f, 0x6f, 0x00};

uint8_t BEL_SendData[6] = {0};
uint8_t BEL_SendBeat[13] = {0};
uint8_t BEL_ReadData[6] = {0};

uint8_t BEL_Flag = 0;
uint8_t BEL_Beat_Flag = 0;
uint16_t BEL_Count;

uint8_t AIRIN_Logo = 0x10; /// 充气图标
uint8_t CH1_Logo = 0x10;   ////1通道图标
uint8_t FORM_Logo = 0x10;  ////模式图标
uint8_t CH2_Logo = 0x10;   ////2通道图标

uint8_t PRESS_Logo = 0x80;	/// 压力值图标
uint8_t AIROUT_Logo = 0x80; /// 放气图标
uint8_t MMHG_Logo = 0x80;	/// mmHg图标
uint8_t BLE_Logo = 0x80;	////蓝牙图标
uint8_t TIM_Logo = 0x80;	/// 时间图标

uint8_t Bat_Value0 = 0x10; ////电池电量外框图标
uint8_t Bat_Value1 = 0x80; ////电池电量一格图标
uint8_t Bat_Value2 = 0x40; ////电池电量二格图标
uint8_t Bat_Value3 = 0x20; ////电池电量三格图标

uint8_t Press_Value1 = 0;	////压力个位数图标
uint8_t Press_Value10 = 0;	////压力十位数图标
uint8_t Press_Value100 = 0; ////压力百位数图标

uint8_t Pwr1_1 = 0;	 ////1通道强度个位数图标
uint8_t Pwr1_10 = 0; ////1通道强度十位数图标

uint8_t Pwr2_1 = 0;	 ////2通道强度个位数图标
uint8_t Pwr2_10 = 0; ////2通道强度十位数图标

uint16_t TIM2_PrescalerValue = 0;
uint16_t TIM3_PrescalerValue = 0;
uint16_t TIM4_PrescalerValue = 0;
uint16_t TIM5_PrescalerValue = 0;

uint8_t Key_PB_MAIN = 0;
uint8_t Key_Down_PB_MAIN = 0;
uint8_t Key_Flag_PB_MAIN = 0;
uint8_t Key_Up_PB_MAIN = 0;
uint8_t Key_LongFlag_PB_MAIN = 0;
uint16_t Key_DownTime_PB_MAIN = 0;
uint16_t Key_UpTime_PB_MAIN = 0;

uint8_t Key_PB_FUN = 0;
uint8_t Key_Down_PB_FUN = 0;
uint8_t Key_Flag_PB_FUN = 0;
uint8_t Key_Up_PB_FUN = 0;
uint8_t Key_LongFlag_PB_FUN = 0;
uint16_t Key_DownTime_PB_FUN = 0;
uint16_t Key_UpTime_PB_FUN = 0;

uint8_t Key_PB_SS = 0;
uint8_t Key_Down_PB_SS = 0;
uint8_t Key_Flag_PB_SS = 0;
uint8_t Key_Up_PB_SS = 0;
uint8_t Key_LongFlag_PB_SS = 0;
uint16_t Key_DownTime_PB_SS = 0;
uint16_t Key_UpTime_PB_SS = 0;

uint8_t Key_PB_PWRM = 0;
uint8_t Key_Down_PB_PWRM = 0;
uint8_t Key_Flag_PB_PWRM = 0;
uint8_t Key_Up_PB_PWRM = 0;
uint8_t Key_LongFlag_PB_PWRM = 0;
uint16_t Key_DownTime_PB_PWRM = 0;
uint16_t Key_UpTime_PB_PWRM = 0;

uint8_t Key_PB_PWRP = 0;
uint8_t Key_Down_PB_PWRP = 0;
uint8_t Key_Flag_PB_PWRP = 0;
uint8_t Key_Up_PB_PWRP = 0;
uint8_t Key_LongFlag_PB_PWRP = 0;
uint16_t Key_DownTime_PB_PWRP = 0;
uint16_t Key_UpTime_PB_PWRP = 0;

uint8_t Tx2_Data[20];

uint8_t Time_SetFlag = 1;
uint8_t Set_Minute = 30; ////开机默认30分钟
uint8_t Minute = 30;	 ////开机默认30分钟
uint8_t Second = 0;
uint8_t Pwr_Flag = 0; ////上电标志位
// uint8_t Start_Flag=0;////启动标志位
// uint8_t Pause_Flag=0;////停止标志位
uint8_t Ch_Flag = 0;  ////通道标志位0:A通道，1：B通道
uint8_t WorkType = 0; /////0：治疗模式，1：检测模式
uint8_t Formula = 0;  ////处方0,1,2，
uint8_t Ico_Formula = 0;
uint8_t Pwr1 = 0; /////0-60档强度
uint8_t Pwr2 = 0;
uint8_t Adc_Flag = 0;
uint8_t Answer1_Flag = 0;
uint8_t LowBat_Flag = 0; ////低电量标志位
uint8_t LowBat_Temp = 0;
uint16_t Press_Value = 0; /// 压力值
uint16_t Buzz_cnt = 0;	  /// 蜂鸣器时长
uint16_t Sleep_cnt = 0;	  /// 休眠时长
uint16_t Charg_Count = 0; ////充电计数
uint8_t Flash_Flag = 0;	  ////闪烁标志位
uint8_t Charg_Flag = 0;	  ////充电中标志位
uint8_t Chargok_Flag = 1; ////充满电标志位

uint16_t Tim1_Count = 0;
uint16_t Tim8_Count = 0;

uint8_t Tim_Cnt = 0;

uint8_t Mode_Change = 0;

uint16_t Bl_Cnt = 0; ////背光点亮计数

uint16_t Moto_Count = 0;
uint16_t MotoFirst_Flag = 1;
uint16_t MotoRun_Cnt = 0;
uint16_t MotoStop_Cnt = 0;
uint16_t Pressure_DAQ = 0;
uint16_t Pre_Send = 0;
uint8_t Pressure_Flag = 0;
uint16_t Ico_Pressure = 0;
uint8_t Moto_StateFlag = 0;
uint8_t BLE_Pressure = 0;

extern uint8_t Wave_SelectA;
extern uint8_t Wave_SelectB;

extern uint16_t E1_Power;
extern uint16_t E2_Power;

extern uint32_t E1_Step;
extern uint32_t E2_Step;

extern uint8_t Key_SelectA;
extern uint8_t Key_SelectB;
extern uint8_t Time_Flag;
// uint32_t E1_Length=0;
// uint16_t Set1_Flag=0;
// uint16_t Set1_Count=0;
// uint16_t E1_Period=0;
// uint16_t E1_Prescaler=0;
// uint16_t E1_Pulse=0;
// uint16_t Set1_Threshold=0;
uint8_t Bat_Value = 0;
uint16_t i = 0;
uint16_t Adc_Bat = 0;	 /////电池电量ADC值
uint16_t Adc_Ref = 0;	 /////参考电压ADC值
float Adc_Bat_Value = 0; ////实际电池电压

uint16_t Pressure_Test = 0;
uint16_t Start_PressureADC = 160;
uint8_t MOTORUN = 0;

#define FLASH_PAGE_SIZE ((uint16_t)0x800)
#define FLASH_WRITE_Freq_ADDR ((uint32_t)0x08010000)
#define FLASH_SN_Code_ADDR ((uint32_t)0x08011000)
#define FLASH_Set_Freq_Flag_ADDR ((uint32_t)0x08012000)
#define FLASH_WRITE_END_ADDR ((uint32_t)0x08018000)

// 保存
void FLASH_Write(uint32_t Write_Addr, uint32_t Write_Data, uint8_t len)
{
	FLASH_Unlock();
	/* Erase */
	if (FLASH_COMPL != FLASH_EraseOnePage(Write_Addr))
	{
	}
	/* Program */
	for (uint32_t Counter_Num = 0; Counter_Num < FLASH_PAGE_SIZE; Counter_Num += len)
	{
		if (FLASH_COMPL != FLASH_ProgramWord(Write_Addr + Counter_Num, Write_Data))
		{
		}
	}
	FLASH_Lock();
}

// 读取
uint32_t FLASH_Read(uint32_t Read_Addr, uint8_t len)
{
	uint32_t Read_Data = 0;
	/* Check */
	for (uint32_t Counter_Num = 0; Counter_Num < FLASH_PAGE_SIZE; Counter_Num += 4)
	{
		Read_Data = (*(__IO uint32_t *)(Read_Addr + Counter_Num));
	}
	return Read_Data;
}

void DisplayPrg(void)
{
	if (Pwr_Flag)
	{
		if (WorkType) //////检测模式
		{
			/////显示压力测试界面
			//			AIRIN_Logo=0x10;
			//			AIROUT_Logo=0x80;
			//			PRESS_Logo=0x80;
			//			MMHG_Logo=0x80;
			if (Press_Value < Start_PressureADC)
			{
				Press_Value = Start_PressureADC;
			}
			BLE_Pressure = (Press_Value - Start_PressureADC) / 10;

			Press_Value1 = ((Press_Value - Start_PressureADC) / 10) % 10;
			Press_Value10 = ((Press_Value - Start_PressureADC) / 10) / 10 % 10;
			Press_Value100 = ((Press_Value - Start_PressureADC) / 10) / 100;
			//////消隐治疗模式界面
			CH1_Logo = 0;
			CH2_Logo = 0;
			FORM_Logo = 0;
			Pwr1_1 = 10;
			Pwr1_10 = 10;
			Pwr2_1 = 10;
			Pwr2_10 = 10;
			//			Ico_Formula=9;
			//			write_LCD(1,4, NUM1[10]|FORM_Logo);
		}
		else //////治疗模式
		{
			/////显示压力测试界面
			AIRIN_Logo = 0;
			AIROUT_Logo = 0;
			PRESS_Logo = 0;
			MMHG_Logo = 0;
			Press_Value1 = 10;
			Press_Value10 = 10;
			Press_Value100 = 10;
			//////消隐治疗模式界面
			//			CH1_Logo=0x10;
			//			CH2_Logo=0x10;

			Pwr1_1 = Pwr1 % 10;
			Pwr1_10 = Pwr1 / 10;
			Pwr2_1 = Pwr2 % 10;
			Pwr2_10 = Pwr2 / 10;
		}
		write_LCD(1, 0, NUM1[Pwr1_10] | AIRIN_Logo);
		write_LCD(1, 2, NUM1[Pwr1_1] | CH1_Logo);
		write_LCD(1, 4, NUM1[Ico_Formula + 1] | FORM_Logo);
		write_LCD(1, 6, NUM1[Pwr2_10]);
		write_LCD(1, 8, NUM1[Pwr2_1] | CH2_Logo);
		write_LCD(1, 10, NUM2[Press_Value100] | PRESS_Logo);
		write_LCD(1, 12, NUM2[Press_Value10] | AIROUT_Logo);
		write_LCD(1, 14, NUM2[Press_Value1] | MMHG_Logo);
		write_LCD(1, 16, NUM2[Minute / 10] | BLE_Logo);
		write_LCD(1, 18, NUM2[Minute % 10] | TIM_Logo);
		write_LCD(1, 20, (Bat_Value0 | Bat_Value1 | Bat_Value2 | Bat_Value3));
	}
	else if (Charg_Flag || Chargok_Flag) ////充电中或充满电显示内容
	{
		write_LCD(1, 0, 0);
		write_LCD(1, 2, 0);
		write_LCD(1, 4, 0);
		write_LCD(1, 6, 0);
		write_LCD(1, 8, 0);
		write_LCD(1, 10, 0);
		write_LCD(1, 12, 0);
		write_LCD(1, 14, 0);
		write_LCD(1, 16, 0);
		write_LCD(1, 18, 0);
		write_LCD(1, 20, (Bat_Value0 | Bat_Value1 | Bat_Value2 | Bat_Value3));
	}
}
/// 按键
void KeyScan(void)
{
	if (Bit_SET == READ_PB_MAIN) ////电源
	{
		if (Key_Down_PB_MAIN == 0)
		{
			Key_Up_PB_MAIN = 0;
			Key_Down_PB_MAIN = 1;
			Key_DownTime_PB_MAIN = 0;
			Key_Flag_PB_MAIN = 0;
			Key_LongFlag_PB_MAIN = 0;
			//				Key_PB_MAIN=1;
		}
		else
		{
			if (Key_DownTime_PB_MAIN > 2000 && Key_LongFlag_PB_MAIN == 0) ////长按2S
			{
				Key_LongFlag_PB_MAIN = 1;
				Key_PB_MAIN = 1;
			}
			if (Key_DownTime_PB_MAIN > 30 && Key_Flag_PB_MAIN == 0) /// 短按30MS
			{
				Key_Flag_PB_MAIN = 1;
				//				Key_PB_MAIN=1;
			}
		}
	}
	else
	{
		if (Key_Up_PB_MAIN == 0)
		{
			Key_Up_PB_MAIN = 1;
			Key_UpTime_PB_MAIN = 0;
		}
		else
		{
			if (Key_UpTime_PB_MAIN > 30) // 松开时间大于30MS
			{
				Key_Down_PB_MAIN = 0;
				if (Key_Flag_PB_MAIN && Key_LongFlag_PB_MAIN == 0)
				{
					Key_Flag_PB_MAIN = 0;
					Key_PB_MAIN = 1;
				}
			}
		}
	}
	if (Bit_SET == READ_PB_FUN) ////功能
	{
		if (Key_Down_PB_FUN == 0)
		{
			Key_Up_PB_FUN = 0;
			Key_Down_PB_FUN = 1;
			Key_DownTime_PB_FUN = 0;
			Key_Flag_PB_FUN = 0;
			Key_LongFlag_PB_FUN = 0;
			//				Key_PB_FUN=1;
		}
		else
		{
			if (Key_DownTime_PB_FUN > 30 && Key_Flag_PB_FUN == 0) ////短按
			{
				Key_Flag_PB_FUN = 1;
				Key_PB_FUN = 1;
			}
		}
	}
	else
	{
		if (Key_Up_PB_FUN == 0)
		{
			Key_Up_PB_FUN = 1;
			Key_UpTime_PB_FUN = 0;
		}
		else
		{
			if (Key_UpTime_PB_FUN > 30) // 松开时间大于30MS
			{
				Key_Down_PB_FUN = 0;
			}
		}
	}
	if (Bit_SET == READ_PB_PWRM) ////减
	{
		if (Key_Down_PB_PWRM == 0)
		{
			Key_Up_PB_PWRM = 0;
			Key_Down_PB_PWRM = 1;
			Key_DownTime_PB_PWRM = 0;
			Key_Flag_PB_PWRM = 0;
			Key_LongFlag_PB_PWRM = 0;
			//				Key_PB_PWRM=1;
		}
		else
		{
			if (Key_DownTime_PB_PWRM > 30 && Key_Flag_PB_PWRM == 0) ////短按
			{
				Key_Flag_PB_PWRM = 1;
				Key_PB_PWRM = 1;
			}
		}
	}
	else
	{
		if (Key_Up_PB_PWRM == 0)
		{
			Key_Up_PB_PWRM = 1;
			Key_UpTime_PB_PWRM = 0;
		}
		else
		{
			if (Key_UpTime_PB_PWRM > 30) // 松开时间大于30MS
			{
				Key_Down_PB_PWRM = 0;
			}
		}
	}
	if (Bit_SET == READ_PB_PWRP) ////加
	{
		if (Key_Down_PB_PWRP == 0)
		{
			Key_Up_PB_PWRP = 0;
			Key_Down_PB_PWRP = 1;
			Key_DownTime_PB_PWRP = 0;
			Key_Flag_PB_PWRP = 0;
			Key_LongFlag_PB_PWRP = 0;
			//				Key_PB_PWRP=1;
		}
		else
		{
			if (Key_DownTime_PB_PWRP > 30 && Key_Flag_PB_PWRP == 0) ////短按
			{
				Key_Flag_PB_PWRP = 1;
				Key_PB_PWRP = 1;
			}
		}
	}
	else
	{
		if (Key_Up_PB_PWRP == 0)
		{
			Key_Up_PB_PWRP = 1;
			Key_UpTime_PB_PWRP = 0;
		}
		else
		{
			if (Key_UpTime_PB_PWRP > 30) // 松开时间大于30MS
			{
				Key_Down_PB_PWRP = 0;
			}
		}
	}
	if (Bit_SET == READ_PB_SS) ////启停
	{
		if (Key_Down_PB_SS == 0)
		{
			Key_Up_PB_SS = 0;
			Key_Down_PB_SS = 1;
			Key_DownTime_PB_SS = 0;
			Key_Flag_PB_SS = 0;
			Key_LongFlag_PB_SS = 0;
			//				Key_PB_SS=1;
		}
		else
		{
			if (Key_DownTime_PB_SS > 2000 && Key_LongFlag_PB_SS == 0) ////长按2S
			{
				Key_LongFlag_PB_SS = 1;
				Key_PB_SS = 1;
			}
			if (Key_DownTime_PB_SS > 30 && Key_Flag_PB_SS == 0) ////短按
			{
				Key_Flag_PB_SS = 1;
				//				Key_PB_SS=1;
			}
		}
	}
	else
	{
		if (Key_Up_PB_SS == 0)
		{
			Key_Up_PB_SS = 1;
			Key_UpTime_PB_SS = 0;
		}
		else
		{
			if (Key_UpTime_PB_SS > 30) // 松开时间大于30MS
			{
				Key_Down_PB_SS = 0;
				if (Key_Flag_PB_SS && Key_LongFlag_PB_SS == 0)
				{
					Key_Flag_PB_SS = 0;
					Key_PB_SS = 1;
				}
			}
		}
	}
}

void KeyEvent(void)
{
	if (Key_PB_MAIN && Key_LongFlag_PB_MAIN) ////长按电源按键关机
	{
		Key_PB_MAIN = 0;
		if (Pwr_Flag)
		{
			Pwr_Flag = 0;

			//			Buzz_cnt=500;
			//			Sleep_cnt=0;
			//			BLEN_OFF;////关闭背光
			//			HOTEN1_OFF;////关闭加热
			//			HOTEN2_OFF;
			//			VEN_OFF;////关闭升压
			//			BATEN_OFF;////关闭电池检测
			//			Set_Minute1=30;
			//			Set_Minute2=30;
			//			Minute1=30;
			//			Second1=0;
			//			Minute2=30;
			//			Second2=0;
			//			Pwr1=0;
			//			Pwr2=0;
			//			Alarm1_Flag=0;
			//			Alarm2_Flag=0;
			//			TIM_EnableCapCmpCh(TIM1, TIM_CH_1, TIM_CAP_CMP_DISABLE);
			//			TIM_EnableCapCmpCh(TIM1, TIM_CH_2, TIM_CAP_CMP_DISABLE);
			//			TIM_EnableCapCmpCh(TIM8, TIM_CH_1, TIM_CAP_CMP_N_DISABLE);
			//			TIM_EnableCapCmpCh(TIM8, TIM_CH_2, TIM_CAP_CMP_N_DISABLE);
			//			DAC_SetCh1Data(DAC_ALIGN_R_12BIT, 0);
			//			DAC_SetCh2Data(DAC_ALIGN_R_12BIT, 0);
		}
	}
	else if (Key_PB_MAIN) ////短按电源按键切换时间
	{
		Key_PB_MAIN = 0;
		if (Pwr_Flag)
		{
			Buzz_cnt = 30; /// 按键音
			Sleep_cnt = 0; /// 清零休眠计数
			Bl_Cnt = 60;   //////背光点亮
			Time_Flag = 1;
			if (Set_Minute == 30) /// 30分钟就跳到10分钟
				Set_Minute = 10;
			else if (Set_Minute % 10 == 0) /// 10分钟或20分钟就加10分钟
				Set_Minute = Set_Minute + 10;
			else ////否则加为整10分钟
				Set_Minute = Set_Minute / 10 + 10;
			Minute = Set_Minute;
			Second = 0;
		}
	}
	if (Key_LongFlag_PB_SS && Key_PB_SS) ////长按启停键
	{
		Key_PB_SS = 0;
		if (Pwr_Flag)
		{
			Buzz_cnt = 30; /// 按键音
			Sleep_cnt = 0; /// 清零休眠计
			Bl_Cnt = 60;   ////背光点亮
			if (WorkType)  /////检测模式
			{

				SWEN_ON;			/// 开启电磁阀
				MotoFirst_Flag = 1; // 重置初次充气标志位
				Key_SelectA = 2;
				MotoStop_Cnt = 2000; // 电磁阀开启2S
			}
			else /////治疗模式
			{
				Ch_Flag = !Ch_Flag; ////通道切换
			}
		}
	}
	else if (Key_PB_SS) ////短按启停键
	{
		Key_PB_SS = 0;
		Time_Flag = 0;
		if (Pwr_Flag)
		{
			Buzz_cnt = 30; /// 按键音
			Sleep_cnt = 0; /// 清零休眠计
			Bl_Cnt = 60;   ////背光点亮
			if (WorkType)  /////检测模式
			{
				Time_SetFlag = 0;
				Key_SelectA = 1;
				SWEN_OFF;		 /////关闭电磁阀
				MotoRun_Cnt = 1; // 启动马达
			}
			else /////治疗模式
			{
				if (Formula == 2) ////除非切换
				{
					Formula = 0;
				}
				else
				{
					Formula++;
				}
				Pwr1 = 0;
				Pwr2 = 0;
				Wave_SelectA = Formula;
				Wave_SelectB = Formula;
			}
		}
	}
	if (Key_PB_FUN) ////短按功能键
	{
		Key_PB_FUN = 0;
		Time_Flag = 0;
		if (Pwr_Flag)
		{
			Buzz_cnt = 30; /// 按键音
			Sleep_cnt = 0; /// 清零休眠计
			Bl_Cnt = 60;   ////背光点亮
			if ((Pwr1 == 0) && (Pwr2 == 0) && (Mode_Change == 0))
			{
				WorkType = !WorkType;
			}
		}
	}
	if (Key_PB_PWRP) ////加
	{
		Key_PB_PWRP = 0;
		Time_Flag = 0;
		if (Pwr_Flag)
		{
			Buzz_cnt = 30; /// 按键音
			Sleep_cnt = 0; /// 清零休眠计
			Bl_Cnt = 60;   ////背光点亮
			if (WorkType == 0)
			{
				Time_SetFlag = 0;
				if (Ch_Flag) ////通道2
				{
					if (Pwr2 == 60) ////强度最大60档
						Pwr2 = 60;
					else
						Pwr2++;
				}
				else ////通道1
				{
					if (Pwr1 == 60)
						Pwr1 = 60;
					else
						Pwr1++;
				}
			}
		}
	}
	if (Key_PB_PWRM) ////减
	{
		Key_PB_PWRM = 0;
		Time_Flag = 0;
		if (Pwr_Flag)
		{
			Buzz_cnt = 30; /// 按键音
			Sleep_cnt = 0; /// 清零休眠计
			Bl_Cnt = 60;   ////背光点亮
			if (WorkType == 0)
			{
				if (Ch_Flag) ////通道2
				{
					if (Pwr2 == 0) ////强度最小0档
					{
						Pwr2 = 0;
					}
					else
						Pwr2--;
				}
				else ////通道1
				{
					if (Pwr1 == 0)
						Pwr1 = 0;
					else
						Pwr1--;
				}
			}
		}
	}
}
/// 串口发送数据
void SendPrg(void)
{
	if (Answer1_Flag)
	{
		for (i = 0; i < (Tx2_Data[2] + 5); i++) ////数据长度+5，头两位，CMD一位，数据长度一位，校验一位，
		{
			USART_SendData(USART2, Tx2_Data[i]); // 发送数据
			while (USART_GetFlagStatus(USART2, USART_FLAG_TXDE) == RESET)
				;
		}
		Answer1_Flag = 0;
	}
}

/**
 * @brief   Main program
 */
int mainold(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	TIM5_Configuration();	////定时器
	USART2_Configuration(); ////串口
	NVIC_Configuration();	////中断
	Delay1ms(100);
	if (Bit_SET == READ_PB_MAIN)
	{
		Delay1ms(1000);
		if (Bit_SET == READ_PB_MAIN) /////长按主按键
		{
			Pwr_Flag = 1;			////上电标志
			USART2_Configuration(); ////上位机接口
									//				TIM2_Configuration();////DAC
			VEN_ON;					////开启压力传感器电源
			TIM4_Configuration();	////MOTO
			TIM1_Configuration();	////电刺激
			TIM2_Configuration();
			TIM6_Configuration();
			TIM8_Configuration();
			DAC_ChannelConfig();
			TIM_Enable(TIM6, ENABLE); ////启动DAC时钟
			BLEEN_ON;

			Delay1ms(10);
			TIM_EnableCapCmpCh(TIM1, TIM_CH_1, TIM_CAP_CMP_DISABLE); /// 关闭通道输出
			TIM_EnableCapCmpCh(TIM1, TIM_CH_2, TIM_CAP_CMP_DISABLE);
			TIM_EnableCapCmpChN(TIM1, TIM_CH_1, TIM_CAP_CMP_N_DISABLE);
			TIM_EnableCapCmpChN(TIM1, TIM_CH_2, TIM_CAP_CMP_N_DISABLE);
			ADC_Initial(ADC1); /// 初始化ADC检测
			ADC_Initial(ADC2); /// 初始化ADC检测
			BATEN_ON;		   /// 使能电量检测
			Delay1ms(100);

			SWEN_OFF;	   ////关闭电磁阀
			Bl_Cnt = 60;   ////背光点亮
			TM1621C_CS_ON; ////预设电平
			TM1621C_CLK_ON;
			TM1621C_DATA_ON;
			Delay1ms(100);
			LCD_init(); /// 屏幕初始化
			Delay1ms(100);
			Allon_LCD();		  /// 点亮屏幕
			TIM3_Configuration(); ////BUZZ
			Delay1ms(50);
			TIM_EnableCapCmpCh(TIM3, TIM_CH_4, TIM_CAP_CMP_DISABLE); ////关闭蜂鸣器
			Delay1ms(50);
			TIM_EnableCapCmpCh(TIM3, TIM_CH_4, TIM_CAP_CMP_ENABLE); ////开启蜂鸣器
			Delay1ms(50);
			TIM_EnableCapCmpCh(TIM3, TIM_CH_4, TIM_CAP_CMP_DISABLE); ////关闭蜂鸣器
			Delay1ms(50);
			TIM_EnableCapCmpCh(TIM3, TIM_CH_4, TIM_CAP_CMP_ENABLE); ////开启蜂鸣器
			Delay1ms(50);
			TIM_EnableCapCmpCh(TIM3, TIM_CH_4, TIM_CAP_CMP_DISABLE); ////关闭蜂鸣器
			Delay1ms(100);
			Tx2_Data[0] = 0xaa; /// 一位数据
			Tx2_Data[1] = 0x55; /// 表示发送的是通讯握手
			Delay1ms(200);
			Alloff_LCD(); /// 屏幕熄灭
			AdcPrg();	  ////每次开机检测一次电池电量
			Delay1ms(300);

			//				Delay1ms(1000);
			//				Start_PressureADC=ADC_GetData(ADC1,ADC1_Channel_11_PA2);////获得压力的ADC值
		}
	}
	while (1)
	{
		KeyScan();
		KeyEvent();
		DisplayPrg();
		ChargPrg(); ////充电状态可能会关闭并复位ADC
		//			SendPrg();
		//			if((Pwr1==0)&&(Pwr2==0))
		//				VEN_OFF;
		//			else
		//				VEN_ON;
		//			AdcPrg();
		if (Pwr_Flag)
		{
			Pressure_Test = ADC_GetData(ADC1, ADC1_Channel_11_PA2); // 测试语句
			Pressure_DAQ = 0;										// 清空上一组数据
			for (uint8_t i = 0; i < 8; i++)							// 简易滤波算法：取均值
			{
				Pressure_DAQ = Pressure_DAQ + ADC_GetData(ADC1, ADC1_Channel_11_PA2); ////获得压力的ADC值
			}
			Press_Value = Pressure_DAQ / 8;
			if (Pre_Send < Press_Value)
			{
				Pre_Send = Press_Value;
			}
			if (Press_Value > 1360) // 气压上限为120mmHg,转换为AD读数为2000；
			{
				MotoStop_Cnt = 2000; // 电磁阀开启两秒放气
			}
		}

		if (BEL_Beat_Flag)
		{
			Send_BeatPack();
			BEL_Beat_Flag = 0;
		}
		if (BEL_Flag)
		{
			App_Event(BEL_ReadData[3], BEL_ReadData[4]);
			BEL_Flag = 0;
		}
	}
}
/// 充电状态灯及电量控制
void ChargPrg(void)
{
	if (Bit_RESET == READ_STDBY) /// 如果充满电
	{
		if (Pwr_Flag) /// 充电中要停止工作
		{
			Pwr_Flag = 0;
			Pwr1 = 0;
			Pwr2 = 0;
			Bl_Cnt = 60;		  ////背光点亮
			TIM_SetCmp1(TIM1, 0); /// 电刺激脉宽清零
			TIM_SetCmp2(TIM1, 0);
			TIM_SetCmp4(TIM3, 0);									 ////蜂鸣器脉宽清零
			TIM_SetCmp4(TIM4, 0);									 ////MOTO清零
																	 //			TIM_SetCmp1(TIM2,0);////DAC2清零
																	 //			TIM_SetCmp2(TIM2,0);////DAC1清零
			TIM_EnableCapCmpCh(TIM1, TIM_CH_1, TIM_CAP_CMP_DISABLE); /// 关闭通道输出
			TIM_EnableCapCmpCh(TIM1, TIM_CH_2, TIM_CAP_CMP_DISABLE);
			TIM_EnableCapCmpChN(TIM1, TIM_CH_1, TIM_CAP_CMP_N_DISABLE);
			TIM_EnableCapCmpChN(TIM1, TIM_CH_2, TIM_CAP_CMP_N_DISABLE);
			TIM_EnableCapCmpCh(TIM3, TIM_CH_4, TIM_CAP_CMP_DISABLE); ////关闭蜂鸣器
			TIM_DeInit(TIM1);										 ////重置电刺激定时器
			TIM_DeInit(TIM2);										 ////重置DAC定时器
			TIM_DeInit(TIM3);										 ////重置蜂鸣器定时器
			TIM_DeInit(TIM4);										 ////重置MOTO定时器
			VEN_OFF;												 /// 关闭电刺激电源
			SWEN_OFF;												 /// 关闭电磁阀
			ADC_DeInit(ADC1);										 /// 重置ADC1
			ADC_DeInit(ADC2);										 /// 重置ADC2
			BATEN_OFF;												 ////关闭电量检测使能
		}
		Charg_Flag = 0;
		Chargok_Flag = 1;
	}
	else if (Bit_RESET == READ_CHARG) /// 如果充电中
	{
		if (Pwr_Flag) /// 充电中要停止工作
		{
			Pwr_Flag = 0;
			Pwr1 = 0;
			Pwr2 = 0;
			Bl_Cnt = 60;		  ////背光点亮
			TIM_SetCmp1(TIM1, 0); /// 电刺激脉宽清零
			TIM_SetCmp2(TIM1, 0);
			TIM_SetCmp4(TIM3, 0);									 ////蜂鸣器脉宽清零
			TIM_SetCmp4(TIM4, 0);									 ////MOTO清零
			TIM_SetCmp1(TIM2, 0);									 ////DAC2清零
			TIM_SetCmp2(TIM2, 0);									 ////DAC1清零
			TIM_EnableCapCmpCh(TIM1, TIM_CH_1, TIM_CAP_CMP_DISABLE); /// 关闭通道输出
			TIM_EnableCapCmpCh(TIM1, TIM_CH_2, TIM_CAP_CMP_DISABLE);
			TIM_EnableCapCmpChN(TIM1, TIM_CH_1, TIM_CAP_CMP_N_DISABLE);
			TIM_EnableCapCmpChN(TIM1, TIM_CH_2, TIM_CAP_CMP_N_DISABLE);
			TIM_EnableCapCmpCh(TIM3, TIM_CH_4, TIM_CAP_CMP_DISABLE); ////关闭蜂鸣器
			TIM_DeInit(TIM1);										 ////重置电刺激定时器
			TIM_DeInit(TIM2);										 ////重置DAC定时器
			TIM_DeInit(TIM3);										 ////重置蜂鸣器定时器
			TIM_DeInit(TIM4);										 ////重置MOTO定时器
			VEN_OFF;												 /// 关闭电刺激电源
			SWEN_OFF;												 /// 关闭电磁阀
			ADC_DeInit(ADC1);										 /// 重置ADC1
			ADC_DeInit(ADC2);										 /// 重置ADC2
			BATEN_OFF;												 ////关闭电量检测使能
		}
		Charg_Flag = 1;
		Chargok_Flag = 0;
	}
	else
	{
		Charg_Flag = 0;
		Chargok_Flag = 0;
		if (Pwr_Flag == 0) ////如果当前状态时休眠就进入休眠
		{
			Charg_Count++; /// 防止充电中和充满电标志脚切换过程中有时隙，造成直接进入休眠状态而造成充满电绿灯不亮
			Delay1ms(10);
			if (Charg_Count >= 100)
			{
				Charg_Count = 0;
				if ((Bit_SET == READ_STDBY) && (Bit_SET == READ_CHARG)) /// 未充电状态下就进入休眠
				{
					/// 关闭ADC模块及中断

					Delay1ms(100);
					Pwr1 = 0;
					Pwr2 = 0;
					Pwr_Flag = 0;
					Bl_Cnt = 0;

					TIM_SetCmp1(TIM1, 0); /// 电刺激脉宽清零
					TIM_SetCmp2(TIM1, 0);
					TIM_SetCmp4(TIM4, 0);									 ////蜂鸣器脉宽清零
					TIM_EnableCapCmpCh(TIM1, TIM_CH_1, TIM_CAP_CMP_DISABLE); /// 关闭通道输出
					TIM_EnableCapCmpCh(TIM1, TIM_CH_2, TIM_CAP_CMP_DISABLE);
					TIM_EnableCapCmpChN(TIM1, TIM_CH_1, TIM_CAP_CMP_N_DISABLE);
					TIM_EnableCapCmpChN(TIM1, TIM_CH_2, TIM_CAP_CMP_N_DISABLE);
					TIM_EnableCapCmpCh(TIM4, TIM_CH_4, TIM_CAP_CMP_DISABLE); ////关闭蜂鸣器
					TIM_DeInit(TIM1);										 ////重置电刺激定时器
					TIM_DeInit(TIM4);										 ////重置蜂鸣器定时器
					VEN_OFF;												 /// 关闭电刺激电源
					SWEN_OFF;												 /// 关闭电磁阀
					ADC_DeInit(ADC1);										 /// 重置ADC1
					BATEN_OFF;												 ////关闭电量检测使能

					TIM_DeInit(TIM5); ////重置TIM5
					BLEN_OFF;
					Alloff_LCD(); /// 屏幕熄灭

					PBExtiInit();	 /// 使能按键中断
					ChargExtiInit(); ////使能充电中断

					//						Charg_Count=0;
					EXTI_ClrITPendBit(EXTI_LINE10); /// 清除CHARG唤醒中断
					EXTI_ClrITPendBit(EXTI_LINE15); /// 清除PB唤醒中断
					PWR_BackupAccessEnable(ENABLE);
					RCC_EnableBackupReset(DISABLE);
					Delay1ms(100);
					PWR_EnterSTOP2Mode(PWR_STOPENTRY_WFI); ////休眠
					NVIC_SystemReset();					   /// 重启
				}
			}
		}
		else if (Adc_Flag && Pwr_Flag)
		{
			AdcPrg();
			Adc_Flag = 0;
			if (LowBat_Flag) /// 如果电池亏电，就进入休眠
			{
				if (LowBat_Temp != LowBat_Flag)
				{
					LowBat_Temp = LowBat_Flag;
					if (LowBat_Temp)
						Pwr_Flag = 0;
				}
			}
		}
	}
}
void AdcPrg(void)
{
	//	Press_Value=ADC_GetData(ADC1,ADC1_Channel_11_PA2);////获得压力的ADC值
	Adc_Bat = ADC_GetData(ADC1, ADC1_Channel_04_PA3); ////获得电池ADC值
	Adc_Ref = ADC_GetData(ADC1, ADC1_Channel_03_PA6); ////获得参考电压2.5V的ADC值
	if (Adc_Ref == 0U)
	{
		return;
	}
	Adc_Bat_Value = Adc_Bat * 2.5f * 2 / Adc_Ref;
	if (Adc_Bat_Value > 4.2f) /// 满电4.2V，电量图标显示满格
	{
		LowBat_Flag = 0;
		Bat_Value0 = 0x10;
		Bat_Value1 = 0x80;
		Bat_Value2 = 0x40;
		Bat_Value3 = 0x20;
	}
	else if (Adc_Bat_Value > 3.9f) ////3.7V电量显示两格
	{
		LowBat_Flag = 0;
		Bat_Value0 = 0x10;
		Bat_Value1 = 0x80;
		Bat_Value2 = 0x40;
		Bat_Value3 = 0;
	}
	else if (Adc_Bat_Value > 3.7f) ////3.6V电量显示一格
	{
		LowBat_Flag = 0;
		Bat_Value0 = 0x10;
		Bat_Value1 = 0x80;
		Bat_Value2 = 0;
		Bat_Value3 = 0;
	}
	else if (Adc_Bat_Value > 3.5f) ////电量显示外框
	{
		Bat_Value0 = 0x10;
		Bat_Value1 = 0;
		Bat_Value2 = 0;
		Bat_Value3 = 0;
		LowBat_Flag = 0;
	}
	//	else///电池电量低于3.5V
	//	{
	//		LowBat_Flag=1;
	//	}

	Bat_Value = (Adc_Bat_Value - 3.5) * 1000 / 7;
	if (Bat_Value > 100)
	{
		Bat_Value = 100;
	}
}

#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param file pointer to the source file name
 * @param line assert_param error line source number
 */
void assert_failed(const uint8_t *expr, const uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}

#endif

/**
 * @}
 */

/**
 * @}
 */

// 重写

/*============================================================================
 * 新主程序框架
 *
 * 设计原则：
 * 1. 中断只提供系统节拍或完成高实时性的波形输出，不在中断里处理 UI 业务。
 * 2. 按键和蓝牙命令统一转换为事件，再由状态机决定能否执行。
 * 3. ADC、LCD、充电检测等任务按固定周期运行，禁止在主循环中无限阻塞。
 * 4. 所有状态切换都通过 App_RequestState()，便于集中执行进入/退出动作。
 *============================================================================*/

typedef enum
{
	APP_STATE_POWER_OFF = 0, /* 关机等待：只保留按键和充电检测 */
	APP_STATE_BOOTING,       /* 开机初始化：按顺序启动工作外设 */
	APP_STATE_READY,         /* 已开机但尚未输出 */
	APP_STATE_THERAPY,       /* 治疗模式 */
	APP_STATE_PRESSURE,      /* 压力检测/充放气模式 */
	APP_STATE_CHARGING,      /* 充电模式：禁止治疗和气泵输出 */
	APP_STATE_FAULT          /* 故障模式：立即关闭所有危险输出 */
} AppState_t;

typedef enum
{
	APP_MODE_THERAPY = 0,
	APP_MODE_PRESSURE
} AppWorkMode_t;

typedef enum
{
	APP_EVENT_NONE = 0,
	APP_EVENT_POWER_SHORT,
	APP_EVENT_POWER_LONG,
	APP_EVENT_FUNCTION_SHORT,
	APP_EVENT_START_SHORT,
	APP_EVENT_START_LONG,
	APP_EVENT_PLUS_SHORT,
	APP_EVENT_MINUS_SHORT,
	APP_EVENT_CHARGER_CONNECTED,
	APP_EVENT_CHARGER_DISCONNECTED
} AppEvent_t;

typedef enum
{
	KEY_ID_POWER = 0,
	KEY_ID_FUNCTION,
	KEY_ID_START,
	KEY_ID_PLUS,
	KEY_ID_MINUS,
	KEY_ID_COUNT
} KeyId_t;

typedef enum
{
	PRESSURE_ACTION_IDLE = 0,
	PRESSURE_ACTION_INFLATING,
	PRESSURE_ACTION_DEFLATING
} PressureAction_t;

#define APP_EVENT_QUEUE_SIZE       16U
#define KEY_SCAN_PERIOD_MS         10U
#define KEY_DEBOUNCE_COUNT         3U
#define POWER_ON_HOLD_MS           3000U
#define POWER_OFF_HOLD_MS          3000U
#define START_LONG_HOLD_MS         3000U
#define UI_BACKLIGHT_SECONDS       60U
#define UI_BLINK_PERIOD_MS         500U
#define UI_MAX_POWER               60U
#define UI_BEEP_ON_MS              60U
#define UI_BEEP_GAP_MS             80U

typedef struct
{
	AppState_t state;        /* 当前系统状态 */
	AppState_t next_state;   /* 请求切换到的状态 */
	AppWorkMode_t work_mode; /* 用户选择的工作模式 */
	uint8_t ui_dirty;        /* 为 1 时才重新刷新 LCD */
	uint8_t fault_code;      /* 0 表示无故障，其他值后续统一定义 */
} AppContext_t;

typedef struct
{
	uint8_t raw_sample;
	uint8_t stable_pressed;
	uint8_t debounce_count;
	uint8_t long_reported;
	uint16_t hold_ms;
} KeyFilter_t;

typedef struct
{
	AppEvent_t data[APP_EVENT_QUEUE_SIZE];
	uint8_t read_index;
	uint8_t write_index;
} AppEventQueue_t;

typedef struct
{
	uint8_t lcd_initialized;
	uint8_t selected_channel;       /* 0：通道 1，1：通道 2 */
	uint8_t formula;                /* 处方编号：0～2，LCD 显示 1～3 */
	uint8_t power_ch1;
	uint8_t power_ch2;
	uint8_t set_minutes;
	uint8_t remaining_minutes;
	uint8_t remaining_seconds;
	uint8_t time_selected;
	uint8_t blink_on;
	uint16_t blink_elapsed_ms;
	uint8_t backlight_seconds;
	uint8_t battery_level;          /* 0～3 格 */
	uint8_t battery_low;            /* 低电量时整个电池图标闪烁 */
	uint8_t charger_connected;
	uint8_t charger_full;
	uint8_t charge_frame;
	uint8_t ble_connected;          /* 未连接常亮，连接后闪烁 */
	uint16_t pressure_value;        /* 已换算的显示值，范围限制为 0～999 */
	uint8_t pressure_result_blink;  /* 测量结果持续闪烁，直到再次按键 */
	PressureAction_t pressure_action;
	uint16_t pressure_action_ms;
	uint8_t buzzer_initialized;
	uint8_t beep_remaining;
	uint16_t beep_on_ms;
	uint16_t beep_gap_ms;
} UiModel_t;

typedef struct
{
	uint32_t last_10ms;
	uint32_t last_50ms;
	uint32_t last_100ms;
	uint32_t last_1000ms;
} AppScheduler_t;

/* SysTick 中断每 1 ms 增加一次。中断和主循环共享，因此必须使用 volatile。 */
static volatile uint32_t s_system_tick_ms = 0U;
static AppContext_t s_app;
static AppScheduler_t s_scheduler;
static KeyFilter_t s_keys[KEY_ID_COUNT];
static AppEventQueue_t s_event_queue;
static UiModel_t s_ui;

static uint8_t s_charger_raw;
static uint8_t s_charger_stable;
static uint8_t s_charger_debounce_count;

static void Board_Init(void);
static void Board_EnterSafeState(void);
static void Treatment_StopOutputs(void);
static void Pressure_StopOutputs(void);
static void App_Init(void);
static void App_RunOnce(void);
static void App_RequestState(AppState_t next_state);
static void App_ApplyStateTransition(void);
static void App_StateEnter(AppState_t state);
static void App_StateExit(AppState_t state);
static uint8_t Scheduler_IsDue(uint32_t *last_tick, uint32_t period_ms);

static uint8_t Key_ReadPressed(KeyId_t key_id);
static void Key_Init(void);
static void Key_Update(KeyId_t key_id);
static void Key_PushShortEvent(KeyId_t key_id);
static void Key_PushLongEvent(KeyId_t key_id);
static void Charger_Update(void);
static uint8_t EventQueue_Push(AppEvent_t event);
static uint8_t EventQueue_Pop(AppEvent_t *event);
static void App_HandleEvent(AppEvent_t event);

static void Ui_InitModel(void);
static void Ui_InitHardware(void);
static void Ui_Shutdown(void);
static void Ui_RecordActivity(void);
static void Ui_Render(void);
static void Ui_RenderTherapy(void);
static void Ui_RenderPressure(void);
static void Ui_RenderCharging(void);
static void Ui_ClearDisplay(void);
static uint8_t Ui_GetBatterySegments(uint8_t level);
static void Ui_CycleTreatmentTime(void);
static void Ui_Countdown1s(void);
static void Ui_Beep(uint8_t count);
static void Ui_BuzzerTask10ms(void);

/* 后续蓝牙、ADC 和压力算法通过这些接口更新 UI，不直接操作段码。 */
void AppUi_SetBleConnected(uint8_t connected);
void AppUi_SetBattery(uint8_t level, uint8_t low_battery);
void AppUi_SetPressureResult(uint16_t value);
void AppUi_InflationCompleted(void);

static void Input_Task10ms(void);
static void Communication_Task10ms(void);
static void AppEvent_Task10ms(void);
static void Control_Task10ms(void);
static void Ui_Task50ms(void);
static void Sensor_Task100ms(void);
static void Power_Task1000ms(void);

/*
 * 1 ms 系统节拍入口。
 * SysTick_Handler() 只调用此函数，不要在中断里增加业务处理。
 */
void App_Tick1msISR(void)
{
	s_system_tick_ms++;
}

/* 板级基础初始化：这里只初始化所有状态都会使用的资源。 */
static void Board_Init(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	SystemCoreClockUpdate();

	/* GPIO 初始化完成后，第一时间把执行器置于安全状态。 */
	Board_EnterSafeState();

	/* 使用独立的 SysTick 作为应用层 1 ms 节拍。 */
	if (SysTick_Config(SystemCoreClock / 1000U) != 0U)
	{
		/* 节拍初始化失败时保持安全状态，不继续启动业务。 */
		while (1)
		{
		}
	}
}

/* 将当前能够直接控制的输出置于安全电平。 */
static void Board_EnterSafeState(void)
{
	Treatment_StopOutputs();
	Pressure_StopOutputs();

	BLEN_OFF;
	VEN_OFF;
	BATEN_OFF;
	BLEEN_OFF;
	SWEN_OFF;

	/* 蜂鸣器属于交互提示，由蜂鸣任务负责停止，不与危险输出混在一起。 */
}

static void Treatment_StopOutputs(void)
{
	/* UI 目标值和遗留波形中断读取的档位同时归零。 */
	s_ui.power_ch1 = 0U;
	s_ui.power_ch2 = 0U;
	Pwr1 = 0U;
	Pwr2 = 0U;

	TIM_SetCmp1(TIM1, 0U);
	TIM_SetCmp2(TIM1, 0U);
	TIM_EnableCapCmpCh(TIM1, TIM_CH_1, TIM_CAP_CMP_DISABLE);
	TIM_EnableCapCmpCh(TIM1, TIM_CH_2, TIM_CAP_CMP_DISABLE);
	TIM_EnableCapCmpChN(TIM1, TIM_CH_1, TIM_CAP_CMP_N_DISABLE);
	TIM_EnableCapCmpChN(TIM1, TIM_CH_2, TIM_CAP_CMP_N_DISABLE);
	TIM_EnableCapCmpCh(TIM8, TIM_CH_1, TIM_CAP_CMP_DISABLE);
	TIM_EnableCapCmpCh(TIM8, TIM_CH_2, TIM_CAP_CMP_DISABLE);
	DAC_SetCh1Data(DAC_ALIGN_R_12BIT, 0U);
	DAC_SetCh2Data(DAC_ALIGN_R_12BIT, 0U);
}

static void Pressure_StopOutputs(void)
{
	s_ui.pressure_action = PRESSURE_ACTION_IDLE;
	s_ui.pressure_action_ms = 0U;
	TIM_SetCmp4(TIM4, 0U);
	TIM_EnableCapCmpCh(TIM4, TIM_CH_4, TIM_CAP_CMP_DISABLE);
	SWEN_OFF;
}

/* 初始化应用状态，不在这里执行耗时或阻塞操作。 */
static void App_Init(void)
{
	s_app.state = APP_STATE_POWER_OFF;
	s_app.next_state = APP_STATE_POWER_OFF;
	s_app.work_mode = APP_MODE_THERAPY;
	s_app.ui_dirty = 1U;
	s_app.fault_code = 0U;

	s_scheduler.last_10ms = s_system_tick_ms;
	s_scheduler.last_50ms = s_system_tick_ms;
	s_scheduler.last_100ms = s_system_tick_ms;
	s_scheduler.last_1000ms = s_system_tick_ms;

	Ui_InitModel();
	Key_Init();

	s_event_queue.read_index = 0U;
	s_event_queue.write_index = 0U;

	s_charger_stable = ((READ_STDBY == Bit_RESET) ||
	                    (READ_CHARG == Bit_RESET)) ? 1U : 0U;
	s_charger_raw = s_charger_stable;
	s_charger_debounce_count = 0U;
	s_ui.charger_connected = s_charger_stable;
	s_ui.charger_full = (READ_STDBY == Bit_RESET) ? 1U : 0U;

	/* 上电时优先识别充电状态，避免在接入充电器时启动治疗输出。 */
	if (s_ui.charger_connected != 0U)
	{
		App_RequestState(APP_STATE_CHARGING);
		App_ApplyStateTransition();
	}
}

/* 主循环的一次调度：每个任务必须快速返回，禁止在任务内部长时间 Delay。 */
static void App_RunOnce(void)
{
	if (Scheduler_IsDue(&s_scheduler.last_10ms, 10U))
	{
		Input_Task10ms();
		Communication_Task10ms();
		AppEvent_Task10ms();
		App_ApplyStateTransition();
		Control_Task10ms();
	}

	if (Scheduler_IsDue(&s_scheduler.last_50ms, 50U))
	{
		Ui_Task50ms();
	}

	if (Scheduler_IsDue(&s_scheduler.last_100ms, 100U))
	{
		Sensor_Task100ms();
	}

	if (Scheduler_IsDue(&s_scheduler.last_1000ms, 1000U))
	{
		Power_Task1000ms();
	}

	/* 处理周期任务产生的状态切换请求。 */
	App_ApplyStateTransition();
}

/* 请求状态切换，不允许业务代码直接修改当前状态。 */
static void App_RequestState(AppState_t next_state)
{
	s_app.next_state = next_state;
}

/* 集中执行状态退出和进入动作，防止外设只开不关。 */
static void App_ApplyStateTransition(void)
{
	if (s_app.state == s_app.next_state)
	{
		return;
	}

	App_StateExit(s_app.state);
	s_app.state = s_app.next_state;
	App_StateEnter(s_app.state);
	s_app.ui_dirty = 1U;
}

static void App_StateExit(AppState_t state)
{
	switch (state)
	{
		case APP_STATE_THERAPY:
			Treatment_StopOutputs();
			break;

		case APP_STATE_PRESSURE:
			Pressure_StopOutputs();
			break;

		default:
			break;
	}
}

static void App_StateEnter(AppState_t state)
{
	switch (state)
	{
		case APP_STATE_POWER_OFF:
			Board_EnterSafeState();
			s_ui.power_ch1 = 0U;
			s_ui.power_ch2 = 0U;
			s_ui.pressure_action = PRESSURE_ACTION_IDLE;
			Ui_Shutdown();
			break;

		case APP_STATE_CHARGING:
			Board_EnterSafeState();
			Ui_InitHardware();
			s_ui.beep_remaining = 0U;
			s_ui.beep_on_ms = 0U;
			s_ui.beep_gap_ms = 0U;
			TIM_EnableCapCmpCh(TIM3, TIM_CH_4, TIM_CAP_CMP_DISABLE);
			BLEN_OFF;
			s_ui.backlight_seconds = 0U;
			s_app.ui_dirty = 1U;
			break;

		case APP_STATE_FAULT:
			Board_EnterSafeState();
			Ui_InitHardware();
			Ui_RecordActivity();
			s_app.ui_dirty = 1U;
			break;

		case APP_STATE_BOOTING:
			/*
			 * 当前阶段只启动 UI。ADC、治疗和气泵外设继续保持关闭，
			 * 等相应模块完成后再逐项加入这里。
			 */
			Board_EnterSafeState();
			Ui_InitModel();
			Ui_InitHardware();
			Ui_RecordActivity();
			Ui_Beep(3U);
			App_RequestState(APP_STATE_THERAPY);
			break;

		case APP_STATE_READY:
			Ui_InitHardware();
			s_app.ui_dirty = 1U;
			break;

		case APP_STATE_THERAPY:
			s_app.work_mode = APP_MODE_THERAPY;
			s_ui.pressure_action = PRESSURE_ACTION_IDLE;
			s_app.ui_dirty = 1U;
			break;

		case APP_STATE_PRESSURE:
			s_app.work_mode = APP_MODE_PRESSURE;
			s_ui.power_ch1 = 0U;
			s_ui.power_ch2 = 0U;
			s_app.ui_dirty = 1U;
			break;

		default:
			App_RequestState(APP_STATE_FAULT);
			break;
	}
}

/* 使用无符号减法保证系统节拍溢出后仍能正确判断周期。 */
static uint8_t Scheduler_IsDue(uint32_t *last_tick, uint32_t period_ms)
{
	uint32_t now = s_system_tick_ms;

	if ((uint32_t)(now - *last_tick) < period_ms)
	{
		return 0U;
	}

	/* 正常情况下累加周期；阻塞过久时直接追到当前时间，避免连续补跑。 */
	if ((uint32_t)(now - *last_tick) > (period_ms * 4U))
	{
		*last_tick = now;
	}
	else
	{
		*last_tick += period_ms;
	}

	return 1U;
}

static uint8_t Key_ReadPressed(KeyId_t key_id)
{
	switch (key_id)
	{
		case KEY_ID_POWER:
			return (READ_PB_MAIN == Bit_SET) ? 1U : 0U;

		case KEY_ID_FUNCTION:
			return (READ_PB_FUN == Bit_SET) ? 1U : 0U;

		case KEY_ID_START:
			return (READ_PB_SS == Bit_SET) ? 1U : 0U;

		case KEY_ID_PLUS:
			return (READ_PB_PWRP == Bit_SET) ? 1U : 0U;

		case KEY_ID_MINUS:
			return (READ_PB_PWRM == Bit_SET) ? 1U : 0U;

		default:
			return 0U;
	}
}

static void Key_Init(void)
{
	uint8_t index;

	for (index = 0U; index < (uint8_t)KEY_ID_COUNT; index++)
	{
		uint8_t pressed = Key_ReadPressed((KeyId_t)index);

		s_keys[index].raw_sample = pressed;
		s_keys[index].stable_pressed = pressed;
		s_keys[index].debounce_count = KEY_DEBOUNCE_COUNT;
		s_keys[index].long_reported = 0U;
		s_keys[index].hold_ms = 0U;
	}
}

static void Key_Update(KeyId_t key_id)
{
	KeyFilter_t *key = &s_keys[key_id];
	uint8_t raw_pressed = Key_ReadPressed(key_id);
	uint16_t long_threshold_ms = 0U;

	if (raw_pressed != key->raw_sample)
	{
		key->raw_sample = raw_pressed;
		key->debounce_count = 1U;
		return;
	}

	if (key->debounce_count < KEY_DEBOUNCE_COUNT)
	{
		key->debounce_count++;
		if (key->debounce_count < KEY_DEBOUNCE_COUNT)
		{
			return;
		}

		if (key->stable_pressed != raw_pressed)
		{
			/* 短按统一在稳定释放时产生，长按释放时不再补发短按。 */
			if ((key->stable_pressed != 0U) &&
			    (raw_pressed == 0U) &&
			    (key->long_reported == 0U))
			{
				Key_PushShortEvent(key_id);
			}

			key->stable_pressed = raw_pressed;
			key->hold_ms = 0U;
			key->long_reported = 0U;
		}
	}

	if (key->stable_pressed == 0U)
	{
		return;
	}

	if (key->hold_ms <= (uint16_t)(0xFFFFU - KEY_SCAN_PERIOD_MS))
	{
		key->hold_ms += KEY_SCAN_PERIOD_MS;
	}

	if (key_id == KEY_ID_POWER)
	{
		if (s_app.state == APP_STATE_POWER_OFF)
		{
			long_threshold_ms = POWER_ON_HOLD_MS;
		}
		else if ((s_app.state == APP_STATE_READY) ||
		         (s_app.state == APP_STATE_THERAPY) ||
		         (s_app.state == APP_STATE_PRESSURE))
		{
			long_threshold_ms = POWER_OFF_HOLD_MS;
		}
	}
	else if (key_id == KEY_ID_START)
	{
		long_threshold_ms = START_LONG_HOLD_MS;
	}

	if ((long_threshold_ms != 0U) &&
	    (key->hold_ms >= long_threshold_ms) &&
	    (key->long_reported == 0U))
	{
		key->long_reported = 1U;
		Key_PushLongEvent(key_id);
	}
}

static void Key_PushShortEvent(KeyId_t key_id)
{
	AppEvent_t event = APP_EVENT_NONE;

	switch (key_id)
	{
		case KEY_ID_POWER:
			event = APP_EVENT_POWER_SHORT;
			break;

		case KEY_ID_FUNCTION:
			event = APP_EVENT_FUNCTION_SHORT;
			break;

		case KEY_ID_START:
			event = APP_EVENT_START_SHORT;
			break;

		case KEY_ID_PLUS:
			event = APP_EVENT_PLUS_SHORT;
			break;

		case KEY_ID_MINUS:
			event = APP_EVENT_MINUS_SHORT;
			break;

		default:
			break;
	}

	if (event != APP_EVENT_NONE)
	{
		(void)EventQueue_Push(event);
	}
}

static void Key_PushLongEvent(KeyId_t key_id)
{
	if (key_id == KEY_ID_POWER)
	{
		(void)EventQueue_Push(APP_EVENT_POWER_LONG);
	}
	else if (key_id == KEY_ID_START)
	{
		(void)EventQueue_Push(APP_EVENT_START_LONG);
	}
}

static void Charger_Update(void)
{
	uint8_t connected = ((READ_STDBY == Bit_RESET) ||
	                     (READ_CHARG == Bit_RESET)) ? 1U : 0U;
	uint8_t full = (READ_STDBY == Bit_RESET) ? 1U : 0U;

	if (full != s_ui.charger_full)
	{
		s_ui.charger_full = full;
		s_app.ui_dirty = 1U;
	}

	if (connected != s_charger_raw)
	{
		s_charger_raw = connected;
		s_charger_debounce_count = 1U;
		return;
	}

	if (s_charger_debounce_count < KEY_DEBOUNCE_COUNT)
	{
		s_charger_debounce_count++;
		if ((s_charger_debounce_count >= KEY_DEBOUNCE_COUNT) &&
		    (s_charger_stable != connected))
		{
			s_charger_stable = connected;
			s_ui.charger_connected = connected;
			(void)EventQueue_Push((connected != 0U) ?
			                      APP_EVENT_CHARGER_CONNECTED :
			                      APP_EVENT_CHARGER_DISCONNECTED);
		}
	}
}

static uint8_t EventQueue_Push(AppEvent_t event)
{
	uint8_t next_index = (uint8_t)((s_event_queue.write_index + 1U) %
	                               APP_EVENT_QUEUE_SIZE);

	if (next_index == s_event_queue.read_index)
	{
		/* 队列满时保留已有事件，避免覆盖尚未处理的关机事件。 */
		return 0U;
	}

	s_event_queue.data[s_event_queue.write_index] = event;
	s_event_queue.write_index = next_index;
	return 1U;
}

static uint8_t EventQueue_Pop(AppEvent_t *event)
{
	if (s_event_queue.read_index == s_event_queue.write_index)
	{
		return 0U;
	}

	*event = s_event_queue.data[s_event_queue.read_index];
	s_event_queue.read_index = (uint8_t)((s_event_queue.read_index + 1U) %
	                                    APP_EVENT_QUEUE_SIZE);
	return 1U;
}

static void App_HandleEvent(AppEvent_t event)
{
	/* 充电状态优先级最高，接入充电器后立即退出所有工作状态。 */
	if (event == APP_EVENT_CHARGER_CONNECTED)
	{
		App_RequestState(APP_STATE_CHARGING);
		return;
	}

	if (event == APP_EVENT_CHARGER_DISCONNECTED)
	{
		if (s_app.state == APP_STATE_CHARGING)
		{
			App_RequestState(APP_STATE_POWER_OFF);
		}
		return;
	}

	if (s_app.state == APP_STATE_POWER_OFF)
	{
		if (event == APP_EVENT_POWER_LONG)
		{
			App_RequestState(APP_STATE_BOOTING);
		}
		return;
	}

	if ((s_app.state == APP_STATE_CHARGING) ||
	    (s_app.state == APP_STATE_BOOTING) ||
	    (s_app.state == APP_STATE_FAULT))
	{
		return;
	}

	/* 工作状态下按任意有效按键都重新点亮背光。 */
	Ui_RecordActivity();
	if (s_ui.pressure_result_blink != 0U)
	{
		/* 需求要求压力结果持续闪烁，直到用户再次按下任意按键。 */
		s_ui.pressure_result_blink = 0U;
		s_app.ui_dirty = 1U;
	}

	switch (event)
	{
		case APP_EVENT_POWER_LONG:
			Ui_Beep(3U);
			App_RequestState(APP_STATE_POWER_OFF);
			break;

		case APP_EVENT_POWER_SHORT:
			Ui_Beep(1U);
			Ui_CycleTreatmentTime();
			s_ui.time_selected = 1U;
			s_app.ui_dirty = 1U;
			break;

		case APP_EVENT_FUNCTION_SHORT:
			Ui_Beep(1U);
			s_ui.time_selected = 0U;
			if (s_app.state == APP_STATE_THERAPY)
			{
				App_RequestState(APP_STATE_PRESSURE);
			}
			else if (s_app.state == APP_STATE_PRESSURE)
			{
				App_RequestState(APP_STATE_THERAPY);
			}
			break;

		case APP_EVENT_START_SHORT:
			Ui_Beep(1U);
			s_ui.time_selected = 0U;
			if (s_app.state == APP_STATE_THERAPY)
			{
				s_ui.formula = (uint8_t)((s_ui.formula + 1U) % 3U);
				s_ui.power_ch1 = 0U;
				s_ui.power_ch2 = 0U;
			}
			else if (s_app.state == APP_STATE_PRESSURE)
			{
				/* 轻按为充气启动/停止；真实完成条件由压力控制模块通知。 */
				if (s_ui.pressure_action == PRESSURE_ACTION_INFLATING)
				{
					s_ui.pressure_action = PRESSURE_ACTION_IDLE;
				}
				else
				{
					s_ui.pressure_action = PRESSURE_ACTION_INFLATING;
				}
				s_ui.pressure_action_ms = 0U;
			}
			s_app.ui_dirty = 1U;
			break;

		case APP_EVENT_START_LONG:
			Ui_Beep(1U);
			s_ui.time_selected = 0U;
			if (s_app.state == APP_STATE_THERAPY)
			{
				s_ui.selected_channel = (uint8_t)!s_ui.selected_channel;
			}
			else if (s_app.state == APP_STATE_PRESSURE)
			{
				s_ui.pressure_action = PRESSURE_ACTION_DEFLATING;
				s_ui.pressure_action_ms = 2000U;
			}
			s_app.ui_dirty = 1U;
			break;

		case APP_EVENT_PLUS_SHORT:
			s_ui.time_selected = 0U;
			if (s_app.state == APP_STATE_THERAPY)
			{
				Ui_Beep(1U);
				uint8_t *power = (s_ui.selected_channel == 0U) ?
				                 &s_ui.power_ch1 : &s_ui.power_ch2;
				if (*power < UI_MAX_POWER - 10)
				{
					(*power)+=10;
				}
				else
				{
					(*power) = UI_MAX_POWER;
				}
				s_app.ui_dirty = 1U;
			}
			break;

		case APP_EVENT_MINUS_SHORT:
			s_ui.time_selected = 0U;
			if (s_app.state == APP_STATE_THERAPY)
			{
				Ui_Beep(1U);
				uint8_t *power = (s_ui.selected_channel == 0U) ?
				                 &s_ui.power_ch1 : &s_ui.power_ch2;
				if (*power > 10U)
				{
					(*power)-=10;
				}
				else
				{
					(*power) = 0;
				}
				s_app.ui_dirty = 1U;
			}
			break;

		default:
			break;
	}
}

static void Ui_InitModel(void)
{
	/* LCD/蜂鸣器初始化标志保留，其他字段恢复默认交互状态。 */
	s_ui.selected_channel = 0U;
	s_ui.formula = 0U;
	s_ui.power_ch1 = 0U;
	s_ui.power_ch2 = 0U;
	s_ui.set_minutes = 30U;
	s_ui.remaining_minutes = 30U;
	s_ui.remaining_seconds = 0U;
	s_ui.time_selected = 0U;
	s_ui.blink_on = 1U;
	s_ui.blink_elapsed_ms = 0U;
	s_ui.backlight_seconds = 0U;
	s_ui.battery_level = 0U;
	s_ui.battery_low = 0U;
	s_ui.charge_frame = 0U;
	s_ui.ble_connected = 0U;
	s_ui.pressure_value = 0U;
	s_ui.pressure_result_blink = 0U;
	s_ui.pressure_action = PRESSURE_ACTION_IDLE;
	s_ui.pressure_action_ms = 0U;
	s_ui.beep_remaining = 0U;
	s_ui.beep_on_ms = 0U;
	s_ui.beep_gap_ms = 0U;
}

static void Ui_InitHardware(void)
{
	if (s_ui.lcd_initialized == 0U)
	{
		TM1621C_CS_ON;
		TM1621C_CLK_ON;
		TM1621C_DATA_ON;
		LCD_init();
		Alloff_LCD();
		s_ui.lcd_initialized = 1U;
	}

	if (s_ui.buzzer_initialized == 0U)
	{
		TIM3_Configuration();
		TIM_EnableCapCmpCh(TIM3, TIM_CH_4, TIM_CAP_CMP_DISABLE);
		s_ui.buzzer_initialized = 1U;
	}
}

static void Ui_Shutdown(void)
{
	BLEN_OFF;
	s_ui.backlight_seconds = 0U;

	if (s_ui.lcd_initialized != 0U)
	{
		Alloff_LCD();
	}
}

static void Ui_RecordActivity(void)
{
	s_ui.backlight_seconds = UI_BACKLIGHT_SECONDS;
	if (s_ui.lcd_initialized != 0U)
	{
		BLEN_ON;
	}
}

static uint8_t Ui_GetBatterySegments(uint8_t level)
{
	uint8_t segments = 0x10U;

	if (level >= 1U)
	{
		segments |= 0x80U;
	}
	if (level >= 2U)
	{
		segments |= 0x40U;
	}
	if (level >= 3U)
	{
		segments |= 0x20U;
	}

	return segments;
}

static void Ui_ClearDisplay(void)
{
	uint8_t address;

	for (address = 0U; address <= 20U; address += 2U)
	{
		write_LCD(1U, address, 0U);
	}
}

static void Ui_RenderTherapy(void)
{
	uint8_t channel1_icon = 0x10U;
	uint8_t channel2_icon = 0x10U;
	uint8_t formula_icon = 0x10U;
	uint8_t formula_value = (uint8_t)(s_ui.formula + 1U);
	uint8_t time_icon = 0x80U;
	uint8_t ble_icon = 0x80U;
	uint8_t battery_segments = Ui_GetBatterySegments(s_ui.battery_level);

	if (s_ui.blink_on == 0U)
	{
		/* P 图标与处方数字作为一个整体闪烁。 */
		formula_icon = 0U;
		formula_value = 10U;
		if (s_ui.selected_channel == 0U)
		{
			channel1_icon = 0U;
		}
		else
		{
			channel2_icon = 0U;
		}

		if (s_ui.time_selected != 0U)
		{
			time_icon = 0U;
		}
	}

	/* 蓝牙未连接时常亮，连接成功后随统一闪烁节拍闪烁。 */
	if ((s_ui.ble_connected != 0U) && (s_ui.blink_on == 0U))
	{
		ble_icon = 0U;
	}

	if ((s_ui.battery_low != 0U) && (s_ui.blink_on == 0U))
	{
		battery_segments = 0U;
	}

	write_LCD(1U, 0U, NUM1[s_ui.power_ch1 / 10U]);
	write_LCD(1U, 2U, NUM1[s_ui.power_ch1 % 10U] | channel1_icon);
	write_LCD(1U, 4U, NUM1[formula_value] | formula_icon);
	write_LCD(1U, 6U, NUM1[s_ui.power_ch2 / 10U]);
	write_LCD(1U, 8U, NUM1[s_ui.power_ch2 % 10U] | channel2_icon);
	write_LCD(1U, 10U, NUM2[10]);
	write_LCD(1U, 12U, NUM2[10]);
	write_LCD(1U, 14U, NUM2[10]);
	write_LCD(1U, 16U, NUM2[s_ui.remaining_minutes / 10U] | ble_icon);
	write_LCD(1U, 18U, NUM2[s_ui.remaining_minutes % 10U] | time_icon);
	write_LCD(1U, 20U, battery_segments);
}

static void Ui_RenderPressure(void)
{
	uint16_t pressure = s_ui.pressure_value;
	uint8_t air_in_icon = 0U;
	uint8_t air_out_icon = 0U;
	uint8_t pressure_icon = 0x80U;
	uint8_t unit_icon = 0x80U;
	uint8_t time_icon = 0x80U;
	uint8_t ble_icon = 0x80U;
	uint8_t battery_segments = Ui_GetBatterySegments(s_ui.battery_level);
	uint8_t pressure_hundreds;
	uint8_t pressure_tens;
	uint8_t pressure_ones;

	if (pressure > 999U)
	{
		pressure = 999U;
	}

	if (s_ui.pressure_action == PRESSURE_ACTION_INFLATING)
	{
		air_in_icon = (s_ui.blink_on != 0U) ? 0x10U : 0U;
	}
	else if (s_ui.pressure_action == PRESSURE_ACTION_DEFLATING)
	{
		air_out_icon = (s_ui.blink_on != 0U) ? 0x80U : 0U;
	}

	pressure_hundreds = (uint8_t)((pressure / 100U) % 10U);
	pressure_tens = (uint8_t)((pressure / 10U) % 10U);
	pressure_ones = (uint8_t)(pressure % 10U);

	if ((s_ui.pressure_result_blink != 0U) && (s_ui.blink_on == 0U))
	{
		pressure_hundreds = 10U;
		pressure_tens = 10U;
		pressure_ones = 10U;
		pressure_icon = 0U;
		unit_icon = 0U;
	}

	if ((s_ui.time_selected != 0U) && (s_ui.blink_on == 0U))
	{
		time_icon = 0U;
	}

	if ((s_ui.ble_connected != 0U) && (s_ui.blink_on == 0U))
	{
		ble_icon = 0U;
	}

	if ((s_ui.battery_low != 0U) && (s_ui.blink_on == 0U))
	{
		battery_segments = 0U;
	}

	write_LCD(1U, 0U, NUM1[10] | air_in_icon);
	write_LCD(1U, 2U, NUM1[10]);
	write_LCD(1U, 4U, NUM1[10]);
	write_LCD(1U, 6U, NUM1[10]);
	write_LCD(1U, 8U, NUM1[10]);
	write_LCD(1U, 10U, NUM2[pressure_hundreds] | pressure_icon);
	write_LCD(1U, 12U, NUM2[pressure_tens] | air_out_icon);
	write_LCD(1U, 14U, NUM2[pressure_ones] | unit_icon);
	write_LCD(1U, 16U, NUM2[s_ui.remaining_minutes / 10U] | ble_icon);
	write_LCD(1U, 18U, NUM2[s_ui.remaining_minutes % 10U] | time_icon);
	write_LCD(1U, 20U, battery_segments);
}

static void Ui_RenderCharging(void)
{
	uint8_t level = s_ui.charger_full ? 3U : s_ui.charge_frame;

	Ui_ClearDisplay();
	write_LCD(1U, 20U, Ui_GetBatterySegments(level));
}

static void Ui_Render(void)
{
	if (s_ui.lcd_initialized == 0U)
	{
		return;
	}

	switch (s_app.state)
	{
		case APP_STATE_THERAPY:
			Ui_RenderTherapy();
			break;

		case APP_STATE_PRESSURE:
			Ui_RenderPressure();
			break;

		case APP_STATE_CHARGING:
			Ui_RenderCharging();
			break;

		case APP_STATE_FAULT:
			/* 当前 LCD 无字母字模，故障界面暂时清屏并保留背光。 */
			Ui_ClearDisplay();
			break;

		default:
			break;
	}
}

static void Ui_CycleTreatmentTime(void)
{
	if (s_ui.set_minutes >= 30U)
	{
		s_ui.set_minutes = 10U;
	}
	else
	{
		s_ui.set_minutes += 10U;
	}

	s_ui.remaining_minutes = s_ui.set_minutes;
	s_ui.remaining_seconds = 0U;
}

void AppUi_SetBleConnected(uint8_t connected)
{
	s_ui.ble_connected = (connected != 0U) ? 1U : 0U;
	s_app.ui_dirty = 1U;
}

void AppUi_SetBattery(uint8_t level, uint8_t low_battery)
{
	s_ui.battery_level = (level > 3U) ? 3U : level;
	s_ui.battery_low = (low_battery != 0U) ? 1U : 0U;
	s_app.ui_dirty = 1U;
}

void AppUi_SetPressureResult(uint16_t value)
{
	s_ui.pressure_value = (value > 999U) ? 999U : value;
	s_ui.pressure_action = PRESSURE_ACTION_IDLE;
	s_ui.pressure_action_ms = 0U;
	s_ui.pressure_result_blink = 1U;
	s_app.ui_dirty = 1U;
}

void AppUi_InflationCompleted(void)
{
	if (s_ui.pressure_action == PRESSURE_ACTION_INFLATING)
	{
		s_ui.pressure_action = PRESSURE_ACTION_IDLE;
		s_ui.pressure_action_ms = 0U;
		s_app.ui_dirty = 1U;
	}
}

static void Ui_Beep(uint8_t count)
{
	if ((s_ui.buzzer_initialized == 0U) || (count == 0U))
	{
		return;
	}

	s_ui.beep_remaining = count;
	s_ui.beep_on_ms = UI_BEEP_ON_MS;
	s_ui.beep_gap_ms = 0U;
	TIM_EnableCapCmpCh(TIM3, TIM_CH_4, TIM_CAP_CMP_ENABLE);
}

static void Ui_BuzzerTask10ms(void)
{
	if (s_ui.buzzer_initialized == 0U)
	{
		return;
	}

	if (s_ui.beep_on_ms != 0U)
	{
		if (s_ui.beep_on_ms <= KEY_SCAN_PERIOD_MS)
		{
			s_ui.beep_on_ms = 0U;
			TIM_EnableCapCmpCh(TIM3, TIM_CH_4, TIM_CAP_CMP_DISABLE);

			if (s_ui.beep_remaining > 0U)
			{
				s_ui.beep_remaining--;
			}
			if (s_ui.beep_remaining > 0U)
			{
				s_ui.beep_gap_ms = UI_BEEP_GAP_MS;
			}
		}
		else
		{
			s_ui.beep_on_ms -= KEY_SCAN_PERIOD_MS;
		}
		return;
	}

	if (s_ui.beep_gap_ms != 0U)
	{
		if (s_ui.beep_gap_ms <= KEY_SCAN_PERIOD_MS)
		{
			s_ui.beep_gap_ms = 0U;
			if (s_ui.beep_remaining > 0U)
			{
				s_ui.beep_on_ms = UI_BEEP_ON_MS;
				TIM_EnableCapCmpCh(TIM3, TIM_CH_4, TIM_CAP_CMP_ENABLE);
			}
		}
		else
		{
			s_ui.beep_gap_ms -= KEY_SCAN_PERIOD_MS;
		}
	}
}

static void Ui_Countdown1s(void)
{
	if ((s_app.state != APP_STATE_THERAPY) ||
	    ((s_ui.power_ch1 == 0U) && (s_ui.power_ch2 == 0U)))
	{
		return;
	}

	if ((s_ui.remaining_minutes == 0U) &&
	    (s_ui.remaining_seconds == 0U))
	{
		Treatment_StopOutputs();
		s_ui.remaining_minutes = s_ui.set_minutes;
		Ui_RecordActivity();
		s_app.ui_dirty = 1U;
		return;
	}

	if (s_ui.remaining_seconds == 0U)
	{
		s_ui.remaining_minutes--;
		s_ui.remaining_seconds = 59U;
	}
	else
	{
		s_ui.remaining_seconds--;
	}

	if ((s_ui.remaining_minutes == 0U) &&
	    (s_ui.remaining_seconds == 0U))
	{
		Treatment_StopOutputs();
		s_ui.remaining_minutes = s_ui.set_minutes;
		Ui_RecordActivity();
		s_app.ui_dirty = 1U;
		return;
	}

	/* LCD 只显示分钟，但分钟变化时仍需刷新。 */
	if (s_ui.remaining_seconds == 59U)
	{
		s_app.ui_dirty = 1U;
	}
}

static void Input_Task10ms(void)
{
	uint8_t index;

	for (index = 0U; index < (uint8_t)KEY_ID_COUNT; index++)
	{
		Key_Update((KeyId_t)index);
	}

	Charger_Update();
}

static void Communication_Task10ms(void)
{
	/* TODO：解析蓝牙接收缓存，并把合法命令转换为与按键相同的事件。 */
}

static void AppEvent_Task10ms(void)
{
	AppEvent_t event;
	uint8_t handled_count = 0U;

	while ((handled_count < APP_EVENT_QUEUE_SIZE) &&
	       (EventQueue_Pop(&event) != 0U))
	{
		App_HandleEvent(event);
		handled_count++;
	}
}

static void Control_Task10ms(void)
{
	Ui_BuzzerTask10ms();

	/*
	 * UI 联调阶段不驱动气泵。充气保持到再次轻按或控制模块通知完成；
	 * 放气图标保持 2 秒，后续由气阀控制模块替代这段临时时序。
	 */
	if ((s_app.state == APP_STATE_PRESSURE) &&
	    (s_ui.pressure_action != PRESSURE_ACTION_IDLE) &&
	    (s_ui.pressure_action_ms != 0U))
	{
		if (s_ui.pressure_action_ms <= KEY_SCAN_PERIOD_MS)
		{
			s_ui.pressure_action_ms = 0U;
			s_ui.pressure_action = PRESSURE_ACTION_IDLE;
			s_app.ui_dirty = 1U;
		}
		else
		{
			s_ui.pressure_action_ms -= KEY_SCAN_PERIOD_MS;
		}
	}
}

static void Ui_Task50ms(void)
{
	if ((s_app.state == APP_STATE_THERAPY) ||
	    (s_app.state == APP_STATE_PRESSURE))
	{
		s_ui.blink_elapsed_ms += 50U;
		if (s_ui.blink_elapsed_ms >= UI_BLINK_PERIOD_MS)
		{
			s_ui.blink_elapsed_ms = 0U;
			s_ui.blink_on = (uint8_t)!s_ui.blink_on;
			s_app.ui_dirty = 1U;
		}
	}

	if (s_app.ui_dirty == 0U)
	{
		return;
	}

	Ui_Render();
	s_app.ui_dirty = 0U;
}

static void Sensor_Task100ms(void)
{
	/* TODO：仅在 ADC 已初始化且当前状态需要时采集压力和电池数据。 */
}

static void Power_Task1000ms(void)
{
	if ((s_app.state == APP_STATE_THERAPY) ||
	    (s_app.state == APP_STATE_PRESSURE) ||
	    (s_app.state == APP_STATE_READY))
	{
		if (s_ui.backlight_seconds > 0U)
		{
			s_ui.backlight_seconds--;
			BLEN_ON;
		}
		else
		{
			BLEN_OFF;
		}
	}

	if (s_app.state == APP_STATE_CHARGING)
	{
		if (s_ui.charger_full == 0U)
		{
			s_ui.charge_frame = (uint8_t)((s_ui.charge_frame + 1U) % 4U);
			s_app.ui_dirty = 1U;
		}
	}

	Ui_Countdown1s();

	/* TODO：低电量、自动关机和蓝牙心跳在对应模块完成后接入。 */
}

int main(void)
{
	Board_Init();
	App_Init();

	while (1)
	{
		App_RunOnce();

		/* 等待下一次中断，避免空转占满 CPU。 */
		__WFI();
	}
}
