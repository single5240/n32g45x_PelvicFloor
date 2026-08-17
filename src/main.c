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
int main(void)
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
