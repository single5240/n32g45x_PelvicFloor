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
 * @file n32g45x_it.c
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32g45x_it.h"





uint8_t Rx2_Data[20];

//extern uint8_t Tx2_Data[20];
uint8_t Buff_Size=0;
extern uint8_t  BEL_SendData[10];
extern uint8_t  BEL_ReadData[6];
extern uint8_t  BEL_Flag;
extern uint8_t  BEL_Beat_Flag;
extern uint16_t BEL_Count;


uint16_t Receive4_Time=0;

uint16_t Receive3_Time=0;


uint16_t ChA_DACValue=4090;
uint16_t ChB_DACValue=4090;


uint8_t Receive2_Count=0;
uint8_t Rx2_Count=0;
uint8_t Receive2_Time=0;
uint8_t Rx2_CheckSum=0;
uint8_t Rx2_CheckSum16=0;

extern uint8_t Key_Down_PB_MAIN;
extern uint8_t Key_Up_PB_MAIN;
extern uint16_t Key_DownTime_PB_MAIN;
extern uint16_t Key_UpTime_PB_MAIN;

extern uint8_t Key_Down_PB_FUN;
extern uint8_t Key_Up_PB_FUN;
extern uint16_t Key_DownTime_PB_FUN;
extern uint16_t Key_UpTime_PB_FUN;

extern uint8_t Key_Down_PB_PWRM;
extern uint8_t Key_Up_PB_PWRM;
extern uint16_t Key_DownTime_PB_PWRM;
extern uint16_t Key_UpTime_PB_PWRM;

extern uint8_t Key_Down_PB_PWRP;
extern uint8_t Key_Up_PB_PWRP;
extern uint16_t Key_DownTime_PB_PWRP;
extern uint16_t Key_UpTime_PB_PWRP;

extern uint8_t Key_Down_PB_SS;
extern uint8_t Key_Up_PB_SS;
extern uint16_t Key_DownTime_PB_SS;
extern uint16_t Key_UpTime_PB_SS;

extern uint16_t Buzz_cnt;
extern uint16_t Sleep_cnt;/// 休眠时长

extern uint16_t Tim1_Count;
extern uint16_t Tim8_Count;

extern uint8_t Time_SetFlag;
extern uint8_t Set_Minute;////开机默认30分钟
extern uint8_t Minute;////开机默认30分钟
extern uint8_t Second;
extern uint8_t Pwr_Flag;////上电标志位
extern uint8_t Ch_Flag;////通道标志位0:1通道，1：2通道
extern uint8_t Formula;/////处方0，1,2
extern uint8_t WorkType;////0:治疗模式，1：检测模式
extern uint8_t Pwr1;/////0-60档强度
extern uint8_t Pwr2;

extern uint8_t Adc_Flag;
extern uint8_t Flash_Flag;

extern uint8_t Charg_Flag;
extern uint8_t Chargok_Flag;

extern uint8_t AIRIN_Logo;///充气图标
extern uint8_t CH1_Logo;////1通道图标
extern uint8_t FORM_Logo;////模式图标
extern uint8_t CH2_Logo;////2通道图标

extern uint8_t PRESS_Logo;///压力值图标
extern uint8_t AIROUT_Logo;///放气图标
extern uint8_t MMHG_Logo;///mmHg图标
extern uint8_t BLE_Logo;////蓝牙图标
extern uint8_t TIM_Logo;///时间图标

extern uint8_t Press_Value1;////压力个位数图标
extern uint8_t Press_Value10;////压力十位数图标
extern uint8_t Press_Value100;////压力百位数图标


extern uint8_t Pwr1_1;////1通道强度个位数图标
extern uint8_t Pwr1_10;////1通道强度十位数图标

extern uint8_t Pwr2_1;////2通道强度个位数图标
extern uint8_t Pwr2_10;////2通道强度十位数图标

extern uint8_t Bat_Value0;////电池电量显示值
extern uint8_t Bat_Value1;////电池电量显示值
extern uint8_t Bat_Value2;////电池电量显示值
extern uint8_t Bat_Value3;////电池电量显示值

extern uint16_t Bl_Cnt;

extern uint8_t Mode_Change;

//uint16_t E1_UnTouch_Count=0;
//uint16_t E2_UnTouch_Count=0;
/********************************************************/
uint8_t Wave_SelectA=0;
uint8_t Wave_SelectB=0;

uint8_t LenWave_SelectA1=0;
uint8_t LenWave_SelectA2=0;
uint8_t TriWave_SelectA1=0;
uint8_t TriWave_SelectA2=0;
uint8_t TraWave_SelectA1=0;
uint8_t TraWave_SelectA2=0;
uint8_t IdxWave_SelectA=1;


uint8_t LenWave_CountA=60;
uint8_t TriWave_CountA=15;
uint8_t TraWave_CountA=10;

uint8_t LenWave_CountB=60;
uint8_t TriWave_CountB=15;
uint8_t TraWave_CountB=10;

uint8_t LenWave_SelectB1=1;
uint8_t LenWave_SelectB2=1;
uint8_t TriWave_SelectB1=0;
uint8_t TriWave_SelectB2=0;
uint8_t TraWave_SelectB1=1;
uint8_t TraWave_SelectB2=1;
uint8_t IdxWave_SelectB=1;

uint32_t 	E1_Step=0;
uint32_t 	E2_Step=0;

uint16_t  WAVE_RANGE;
uint16_t  wave_range;
uint16_t	Wave_StepA;

uint16_t	Start_PowerA;
uint16_t	Start_PowerB;

uint16_t	Pwr2_ADCValue;
uint16_t 	CHSIZE=500;

uint16_t	SinWaveA_ADCValue;
uint16_t	SinWaveB_ADCValue;

uint16_t	Pwr1_ADCValue;
uint32_t 	E1_Length=0;

uint16_t 	E1_ProfileA=400;
uint16_t 	E1_ProfileB=400;

uint16_t 	E1_Keep_Power=0;
uint16_t 	E2_Keep_Power=0;
uint16_t 	E1_Power=0;
uint16_t 	E2_Power=0;

uint8_t		Key_SelectA=3;
uint8_t		Key_SelectB=0;
uint8_t		Time_Flag=0;
uint8_t		Ico_Flag=0;
uint16_t	Ico_Cnt=0;


uint16_t ChangeData_A1[17];
uint16_t ChangeData_A2[8];

uint16_t ChangeData_B1[17];
uint16_t ChangeData_B2[8];

extern uint16_t Moto_Count;
extern uint16_t MotoFirst_Flag;//首次充气标志位
extern uint16_t MotoRun_Cnt;//电机启动标志位
extern uint16_t MotoStop_Cnt;//电磁阀开启标志位
extern uint16_t Press_Value;//压力值
extern uint16_t Start_PressureADC;
extern uint8_t	Ico_Formula;
extern uint8_t  Moto_StateFlag;

extern uint16_t RunA_NumS;
extern uint16_t RunA_NumM;
extern uint16_t RunA_NumL;
extern uint16_t RunA_NumX;
extern uint16_t RunA_i;
extern uint16_t RunA_j;
extern uint16_t RunA_m;
extern uint16_t RunA_n;
extern uint16_t RunWaveA_Count;

extern uint16_t RunB_NumS;
extern uint16_t RunB_NumM;
extern uint16_t RunB_NumL;
extern uint16_t RunB_NumX;
extern uint16_t RunB_i;
extern uint16_t RunB_j;
extern uint16_t RunB_m;
extern uint16_t RunB_n;
extern uint16_t RunWaveB_Count;

extern uint16_t Wave_ChangeA;
extern uint16_t Wave_ChangeB;

extern uint16_t RunModeA_Count[8][4];
extern uint16_t	RunModeA_Hz[14][3];
extern uint16_t	RunModeA_Cycle[16][2];
extern uint16_t	RunModeA_Step[14][3];

extern uint16_t RunModeB_Count[8][4];
extern uint16_t	RunModeB_Hz[14][3];
extern uint16_t	RunModeB_Cycle[16][2];
extern uint16_t	RunModeB_Step[14][3];

/****************************************************************************/

/** @addtogroup N32G45X_StdPeriph_Template
 * @{
 */

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
 * @brief  This function handles Memory Manage exception.
 */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
 * @brief  This function handles Bus Fault exception.
 */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
 * @brief  This function handles Usage Fault exception.
 */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
 * @brief  This function handles SVCall exception.
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles Debug Monitor exception.
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 N32G45X Peripherals Interrupt Handlers                     */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_n32g45x.s).                                                 */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 */
/*void PPP_IRQHandler(void)
{
}*/
uint8_t RAED_DATA;
uint8_t checksum = 0;
void USART2_IRQHandler(void)
{
    if (USART_GetIntStatus(USART2, USART_INT_RXDNE) != RESET) 
    {
        /*防止数据溢出*/
        if(Receive2_Count >= MAX_BUFFER_SIZE) {
            Receive2_Count = 0; // 溢出时重置
        }

        /*读取数据*/
        BEL_ReadData[Receive2_Count++] = USART_ReceiveData(USART2);
        Receive2_Time=0;
        if(BEL_ReadData[0]!=0x5a)
        {
            Receive2_Count=0;
        }      
        /*检查帧头*/
        if(Receive2_Count >= 6 && BEL_ReadData[0]==0x5a && BEL_ReadData[1]==0xa5) 
        {

								checksum = BLE_Checksum(BEL_ReadData, Receive2_Count );
                /*校验帧数据*/
                if(BEL_ReadData[Receive2_Count-1] == checksum) 
                {
                    BEL_Flag = 1;
                }
                Receive2_Count = 0; /*处理完成重置缓冲区*/

        }
    }
}
/**
 * @brief  This function handles USARTy global interrupt request.
 */

uint16_t Set_Value=55;
void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetIntStatus(TIM1, TIM_INT_UPDATE) != RESET)
    {
        TIM_ClrIntPendingBit(TIM1, TIM_INT_UPDATE);  //250us
        if(Pwr1)////
        {	
						E1_Step++;
						switch(Wave_SelectA)
						{
							case 0:/*长周期梯形波***************************************************************************************/
										E1_Power=Pwr1*Set_Value;
										switch(TraWave_SelectA1)
										{
											case 0:	/*上升段100ms*/											
												if(E1_Step<200)
												{
													Pwr1_ADCValue=(E1_Power*E1_Step)/200;
												}else
												{
													TraWave_SelectA1=1;
												}
												break;
											case 1:/*维持段200ms*/
												if(E1_Step<600)
												{
													Pwr1_ADCValue=E1_Power;
												}else
												{
													TraWave_SelectA1=2;
												}												
												break;
											case 2:/*下降段100ms*/
												if(E1_Step<800)
												{
													Pwr1_ADCValue=(E1_Power*(800-E1_Step))/200;
												}else
												{
													TraWave_SelectA1=3;
												}
												break;
											case 3:/*空闲段400ms*/
												if(E1_Step<1600)
												{
													Pwr1_ADCValue=0;
												}else
												{
													TraWave_SelectA1=4;
												}												
												break;
											case 4:/*一轮周期结束重置参数*/
												E1_Step=0;
												TraWave_SelectA1=0;
												Pwr1_ADCValue=0;
												TraWave_CountA--;
												if(TraWave_CountA==0)
												{
													TraWave_CountA=20;
													Wave_SelectA=3;
												}
												break;
											default:
												break;
										}
									break;
							case 1:/*长周期棱形波***************************************************************************************/
										E1_Power=Pwr1*Set_Value;
										switch(LenWave_SelectA1)
										{
											case 0:/*上升段400ms*/
												if(E1_Step<800)
												{
													Pwr1_ADCValue=(E1_Power*E1_Step)/800;
												}else
												{
													LenWave_SelectA1=1;
												}
												break;
											case 1:/*下降段400ms*/
												if(E1_Step<1600)
												{
													Pwr1_ADCValue=(E1_Power*(1600-E1_Step))/800;
												}else
												{ 
													LenWave_SelectA1=2;
												}												
												break;
											case 2:/*空闲段500ms*/
												if(E1_Step<2600)
												{
													Pwr1_ADCValue=0;
												}else
												{
													LenWave_SelectA1=3;
												}
												break;	
											case 3:/*一轮周期结束重置参数*/
												E1_Step=0;
												LenWave_SelectA1=0;
												Pwr1_ADCValue=0;
												LenWave_CountA--;
												if(LenWave_CountA==0)
												{
													LenWave_CountA=30;
													Wave_SelectA=4;
												}
												break;
											default:
												break;
										}
									break;
							case 2:/*长周期三角波***************************************************************************************/
										E1_Power=Pwr1*Set_Value;
										switch(TriWave_SelectA1)
										{
											case 0:/*上升段1600ms*/
												if(E1_Step<3200)
												{
													Pwr1_ADCValue=(E1_Power*E1_Step)/3200;
												}
												else
												{
													TriWave_SelectA1=1;
												}
												break;
											case 1:/*空闲段2000ms*/
												if(E1_Step<7200)
												{
													Pwr1_ADCValue=0;
												}
												else
												{
													TriWave_SelectA1=2;
												}
												break;
											case 2:/*一轮周期结束重置参数*/
												E1_Step=0;
												TriWave_SelectA1=0;
												TriWave_CountA--;
												if(TriWave_CountA==0)
												{
													TriWave_CountA=30;
													Wave_SelectA=5;
												}																								
												break;
											default:
												break;
										}
									break;
							case 3:/*短周期梯形波***************************************************************************************/
										E1_Power=Pwr1*Set_Value;
										switch(TraWave_SelectA2)
										{
											case 0:/*上升段50ms*/												
												if(E1_Step<100)
												{
													Pwr1_ADCValue=(E1_Power*E1_Step)/100;
												}else
												{
													TraWave_SelectA2=1;
												}
												break;
											case 1:/*维持段200ms*/
												if(E1_Step<500)
												{
													Pwr1_ADCValue=E1_Power;
												}else
												{
													TraWave_SelectA2=2;
												}												
												break;
											case 2:/*下降段50ms*/
												if(E1_Step<600)
												{
													Pwr1_ADCValue=(E1_Power*(600-E1_Step))/100;
												}else
												{
													TraWave_SelectA2=3;
												}
												break;
											case 3:/*空闲段300ms*/
												if(E1_Step<1200)
												{
													Pwr1_ADCValue=0;
												}else
												{
													TraWave_SelectA2=4;
												}												
												break;
											case 4:/*一轮周期结束重置参数*/
												E1_Step=0;
												TraWave_SelectA2=0;
												Pwr1_ADCValue=0;
												TraWave_CountA--;
												if(TraWave_CountA==0)
												{
													TraWave_CountA=10;
													Wave_SelectA=0;
												}
												break;
											default:
												break;
										}
									break;
								case 4:/*短周期棱形波***************************************************************************************/
										E1_Power=Pwr1*Set_Value;
										switch(LenWave_SelectA2)
										{
											case 0:/*上升段200ms*/
												if(E1_Step<400)
												{
													Pwr1_ADCValue=(E1_Power*E1_Step)/400;
												}else
												{
													LenWave_SelectA2=1;
												}
												break;
											case 1:/*下降段200ms*/
												if(E1_Step<800)
												{
													Pwr1_ADCValue=(E1_Power*(800-E1_Step))/400;
												}else
												{
													LenWave_SelectA2=2;
												}												
												break;
											case 2:/*空闲段500ms*/
												if(E1_Step<1800)
												{
													Pwr1_ADCValue=0;
													Pwr2_ADCValue=0;
												}else
												{
													LenWave_SelectA2=3;
												}
												break;	
											case 3:/*一轮周期结束重置参数*/
												E1_Step=0;
												LenWave_SelectA2=0;
												Pwr1_ADCValue=0;
												LenWave_CountA--;
												if(LenWave_CountA==0)
												{
													LenWave_CountA=60;
													Wave_SelectA=1;												
												}
												break;
											default:
												break;
										}
									break;
								case 5:/*短周期三角波***************************************************************************************/
										E1_Power=Pwr1*Set_Value;
										switch(TriWave_SelectA2)
										{
											case 0:/*上升段800ms*/
												if(E1_Step<1600)
												{
													Pwr1_ADCValue=(E1_Power*E1_Step)/1600;
												}
												else
												{
													TriWave_SelectA2=1;
												}
												break;
											case 1:/*空闲段1000ms*/
												if(E1_Step<3600)
												{
													Pwr1_ADCValue=0;
												}
												else
												{
													TriWave_SelectA2=2;
												}
												break;
											case 2:/*一轮周期结束重置参数*/
												E1_Step=0;
												TriWave_SelectA2=0;
												TriWave_CountA--;
												if(TriWave_CountA==0)
												{
													TriWave_CountA=15;
													Wave_SelectA=2;
												}												
												break;
											default:
												break;
										}
								break;	
							default :
									break;

						}
            ChA_DACValue = 3400 - Pwr1_ADCValue;
            if(ChA_DACValue<10)
            {
              ChA_DACValue = 300;
            }
            DAC_SetCh2Data(DAC_ALIGN_R_12BIT, ChA_DACValue);
						Tim1_Count++;
						if(Tim1_Count==1)
						{
								TIM_EnableCapCmpCh(TIM1, TIM_CH_1, TIM_CAP_CMP_ENABLE);
								TIM_EnableCapCmpCh(TIM1, TIM_CH_2, TIM_CAP_CMP_DISABLE);
						}
						else if(Tim1_Count==2)
						{
								Tim1_Count=0;
								TIM_EnableCapCmpCh(TIM1, TIM_CH_1, TIM_CAP_CMP_DISABLE);
								TIM_EnableCapCmpCh(TIM1, TIM_CH_2, TIM_CAP_CMP_ENABLE);
						}
				}
				else
				{
						
						TIM_EnableCapCmpCh(TIM1, TIM_CH_1, TIM_CAP_CMP_DISABLE);
						TIM_EnableCapCmpCh(TIM1, TIM_CH_2, TIM_CAP_CMP_DISABLE);
				}
		}
}



void TIM8_UP_IRQHandler(void)
{
    if (TIM_GetIntStatus(TIM8, TIM_INT_UPDATE) != RESET)
    {
        TIM_ClrIntPendingBit(TIM8, TIM_INT_UPDATE);  //250us
        if(Pwr2)////
        {	
						E2_Step++;
						switch(Wave_SelectB)
						{
							case 0:/*长周期梯形波***************************************************************************************/
										E2_Power=Pwr2*Set_Value;
										switch(TraWave_SelectB1)
										{
											case 0:	/*上升段100ms*/											
												if(E2_Step<200)
												{
													Pwr2_ADCValue=(E2_Power*E2_Step)/200;
												}else
												{
													TraWave_SelectB1=1;
												}
												break;
											case 1:/*维持段200ms*/
												if(E2_Step<600)
												{
													Pwr2_ADCValue=E2_Power;
												}else
												{
													TraWave_SelectB1=2;
												}												
												break;
											case 2:/*下降段100ms*/
												if(E2_Step<800)
												{
													Pwr2_ADCValue=(E2_Power*(800-E2_Step))/200;
												}else
												{
													TraWave_SelectB1=3;
												}
												break;
											case 3:/*空闲段400ms*/
												if(E2_Step<1600)
												{
													Pwr2_ADCValue=0;
												}else
												{
													TraWave_SelectB1=4;
												}												
												break;
											case 4:/*一轮周期结束重置参数*/
												E2_Step=0;
												TraWave_SelectB1=0;
												Pwr2_ADCValue=0;
												TraWave_CountB--;
												if(TraWave_CountB==0)
												{
													TraWave_CountB=20;
													Wave_SelectB=3;
												}
												break;
											default:
												break;
										}
									break;
							case 1:/*长周期棱形波***************************************************************************************/
										E2_Power=Pwr2*Set_Value;
										switch(LenWave_SelectB1)
										{
											case 0:/*上升段400ms*/
												if(E2_Step<800)
												{
													Pwr2_ADCValue=(E2_Power*E2_Step)/800;
												}else
												{
													LenWave_SelectB1=1;
												}
												break;
											case 1:/*下降段400ms*/
												if(E2_Step<1600)
												{
													Pwr2_ADCValue=(E2_Power*(1600-E2_Step))/800;
												}else
												{ 
													LenWave_SelectB1=2;
												}												
												break;
											case 2:/*空闲段500ms*/
												if(E2_Step<2600)
												{
													Pwr2_ADCValue=0;
												}else
												{
													LenWave_SelectB1=3;
												}
												break;	
											case 3:/*一轮周期结束重置参数*/
												E2_Step=0;
												LenWave_SelectB1=0;
												Pwr2_ADCValue=0;
												LenWave_CountB--;
												if(LenWave_CountB==0)
												{
													LenWave_CountB=30;
													Wave_SelectB=4;
												}
												break;
											default:
												break;
										}
									break;
							case 2:/*长周期三角波***************************************************************************************/
										E2_Power=Pwr2*Set_Value;
										switch(TriWave_SelectB1)
										{
											case 0:/*上升段1600ms*/
												if(E2_Step<3200)
												{
													Pwr2_ADCValue=(E2_Power*E2_Step)/3200;
												}
												else
												{
													TriWave_SelectB1=1;
												}
												break;
											case 1:/*空闲段2000ms*/
												if(E2_Step<7200)
												{
													Pwr2_ADCValue=0;
												}
												else
												{
													TriWave_SelectB1=2;
												}
												break;
											case 2:/*一轮周期结束重置参数*/
												E2_Step=0;
												TriWave_SelectB1=0;
												TriWave_CountB--;
												if(TriWave_CountB==0)
												{
													TriWave_CountB=30;
													Wave_SelectB=5;
												}																								
												break;
											default:
												break;
										}
									break;
							case 3:/*短周期梯形波***************************************************************************************/
										E2_Power=Pwr2*Set_Value;
										switch(TraWave_SelectB2)
										{
											case 0:/*上升段50ms*/												
												if(E2_Step<100)
												{
													Pwr2_ADCValue=(E2_Power*E2_Step)/100;
												}else
												{
													TraWave_SelectB2=1;
												}
												break;
											case 1:/*维持段200ms*/
												if(E2_Step<500)
												{
													Pwr2_ADCValue=E2_Power;
												}else
												{
													TraWave_SelectB2=2;
												}												
												break;
											case 2:/*下降段50ms*/
												if(E2_Step<600)
												{
													Pwr2_ADCValue=(E2_Power*(600-E2_Step))/100;
												}else
												{
													TraWave_SelectB2=3;
												}
												break;
											case 3:/*空闲段300ms*/
												if(E2_Step<1200)
												{
													Pwr2_ADCValue=0;
												}else
												{
													TraWave_SelectB2=4;
												}												
												break;
											case 4:/*一轮周期结束重置参数*/
												E2_Step=0;
												TraWave_SelectB2=0;
												Pwr2_ADCValue=0;
												TraWave_CountB--;
												if(TraWave_CountB==0)
												{
													TraWave_CountB=10;
													Wave_SelectB=0;
												}
												break;
											default:
												break;
										}
									break;
								case 4:/*短周期棱形波***************************************************************************************/
										E2_Power=Pwr2*Set_Value;
										switch(LenWave_SelectB2)
										{
											case 0:/*上升段200ms*/
												if(E2_Step<400)
												{
													Pwr2_ADCValue=(E2_Power*E1_Step)/400;
												}else
												{
													LenWave_SelectB2=1;
												}
												break;
											case 1:/*下降段200ms*/
												if(E2_Step<800)
												{
													Pwr2_ADCValue=(E2_Power*(800-E2_Step))/400;
												}else
												{
													LenWave_SelectB2=2;
												}												
												break;
											case 2:/*空闲段500ms*/
												if(E2_Step<1800)
												{
													Pwr2_ADCValue=0;
												}else
												{
													LenWave_SelectB2=3;
												}
												break;	
											case 3:/*一轮周期结束重置参数*/
												E2_Step=0;
												LenWave_SelectB2=0;
												Pwr2_ADCValue=0;
												LenWave_CountB--;
												if(LenWave_CountB==0)
												{
													LenWave_CountB=60;
													Wave_SelectB=1;												
												}
												break;
											default:
												break;
										}
									break;
								case 5:/*短周期三角波***************************************************************************************/
										E2_Power=Pwr2*Set_Value;
										switch(TriWave_SelectB2)
										{
											case 0:/*上升段800ms*/
												if(E2_Step<1600)
												{
													Pwr2_ADCValue=(E2_Power*E2_Step)/1600;
												}
												else
												{
													TriWave_SelectB2=1;
												}
												break;
											case 1:/*空闲段1000ms*/
												if(E1_Step<3600)
												{
													Pwr2_ADCValue=0;
												}
												else
												{
													TriWave_SelectB2=2;
												}
												break;
											case 2:/*一轮周期结束重置参数*/
												E2_Step=0;
												TriWave_SelectB2=0;
												TriWave_CountB--;
												if(TriWave_CountB==0)
												{
													TriWave_CountB=15;
													Wave_SelectB=2;
												}												
												break;
											default:
												break;
										}
								break;	
							default :
									break;

						}
            ChB_DACValue = 3400 - Pwr2_ADCValue;
            if(ChB_DACValue<10)
            {
              ChB_DACValue = 300;
            }
            DAC_SetCh1Data(DAC_ALIGN_R_12BIT, ChB_DACValue);

						Tim8_Count++;
						if(Tim8_Count==1)
						{
								TIM_EnableCapCmpCh(TIM8, TIM_CH_1, TIM_CAP_CMP_ENABLE);
								TIM_EnableCapCmpCh(TIM8, TIM_CH_2, TIM_CAP_CMP_DISABLE);
						}
						else if(Tim8_Count==2)
						{
								Tim8_Count=0;
								TIM_EnableCapCmpCh(TIM8, TIM_CH_1, TIM_CAP_CMP_DISABLE);
								TIM_EnableCapCmpCh(TIM8, TIM_CH_2, TIM_CAP_CMP_ENABLE);
						}
				}
				else
				{
						
						TIM_EnableCapCmpCh(TIM8, TIM_CH_1, TIM_CAP_CMP_DISABLE);
						TIM_EnableCapCmpCh(TIM8, TIM_CH_2, TIM_CAP_CMP_DISABLE);
				}
		}
}

/**
 * @brief  This function handles TIM3 global interrupt request.
 */
void TIM2_IRQHandler(void)///作为通用定时器使用
{
    
    if (TIM_GetIntStatus(TIM2, TIM_INT_UPDATE) != RESET)
    {
        TIM_ClrIntPendingBit(TIM2, TIM_INT_UPDATE);

        if(Receive2_Time<11)
            Receive2_Time++;///10ms
        if(Receive2_Time==10)///超过20MS还没有收到下个串口信号
        {
            memset(BEL_ReadData,0,Receive2_Count);
            Receive2_Count=0;
        }


		}
}
		/**
 * @brief  This function handles TIM3 global interrupt request.
 */
void TIM5_IRQHandler(void)///作为通用定时器使用
{
		static uint16_t hh,kk,jj;
    if (TIM_GetIntStatus(TIM5, TIM_INT_UPDATE) != RESET)
    {
			TIM_ClrIntPendingBit(TIM5, TIM_INT_UPDATE);

        if(Receive2_Time<11)
            Receive2_Time++;///10ms
        if(Receive2_Time==10)///超过10MS还没有收到下个串口信号
        {
            memset(Rx2_Data,0,Receive2_Count);
            Receive2_Count=0;
        }
				if (Key_Down_PB_MAIN== 1)
				{
						Key_DownTime_PB_MAIN++;
						if (Key_DownTime_PB_MAIN >= 30000)
						{
								Key_DownTime_PB_MAIN = 30000;
						}
				}
				if (Key_Up_PB_MAIN== 1)
				{
						Key_UpTime_PB_MAIN++;
						if (Key_UpTime_PB_MAIN >= 30000)
						{
								Key_UpTime_PB_MAIN = 30000;
						}
				}
				if (Key_Down_PB_FUN== 1)
				{
						Key_DownTime_PB_FUN++;
						if (Key_DownTime_PB_FUN >= 30000)
						{
								Key_DownTime_PB_FUN = 30000;
						}
				}
				if (Key_Up_PB_FUN== 1)
				{
						Key_UpTime_PB_FUN++;
						if (Key_UpTime_PB_FUN >= 30000)
						{
								Key_UpTime_PB_FUN = 30000;
						}
				}
				if (Key_Down_PB_PWRM== 1)
				{
						Key_DownTime_PB_PWRM++;
						if (Key_DownTime_PB_PWRM >= 30000)
						{
								Key_DownTime_PB_PWRM = 30000;
						}
				}
				if (Key_Up_PB_PWRM== 1)
				{
						Key_UpTime_PB_PWRM++;
						if (Key_UpTime_PB_PWRM >= 30000)
						{
								Key_UpTime_PB_PWRM = 30000;
						}
				}
				if (Key_Down_PB_PWRP== 1)
				{
						Key_DownTime_PB_PWRP++;
						if (Key_DownTime_PB_PWRP >= 30000)
						{
								Key_DownTime_PB_PWRP = 30000;
						}
				}
				if (Key_Up_PB_PWRP== 1)
				{
						Key_UpTime_PB_PWRP++;
						if (Key_UpTime_PB_PWRP >= 30000)
						{
								Key_UpTime_PB_PWRP = 30000;
						}
				}
				if (Key_Down_PB_SS== 1)
				{
						Key_DownTime_PB_SS++;
						if (Key_DownTime_PB_SS >= 30000)
						{
								Key_DownTime_PB_SS = 30000;
						}
				}
				if (Key_Up_PB_SS== 1)
				{
						Key_UpTime_PB_SS++;
						if (Key_UpTime_PB_SS>= 30000)
						{
								Key_UpTime_PB_SS = 30000;
						}
				}
				Ico_Cnt++;
				if(Ico_Cnt>500)
				{
					Ico_Cnt=0;
					Ico_Flag=!Ico_Flag;
					if(WorkType)
					{
						Ico_Formula=9;	
						switch(Key_SelectA)
						{
							case 1:								
								AIROUT_Logo=0x80;
								PRESS_Logo=0x80;
								MMHG_Logo=0x80;	
                Moto_StateFlag=0x01;
								if(Ico_Flag)
									AIRIN_Logo=0x10;
								else
									AIRIN_Logo=0;
								break;
							case 2:
		            Moto_StateFlag=0x02;
								AIRIN_Logo=0x10;
								PRESS_Logo=0x80;
								MMHG_Logo=0x80;	
								if(Ico_Flag)
									AIROUT_Logo=0x80;
								else
									AIROUT_Logo=0;
								break;
							case 3:
								Moto_StateFlag=0x00;
								AIRIN_Logo=0x10;
								AIROUT_Logo=0x80;
								PRESS_Logo=0x80;
								MMHG_Logo=0x80;	
								break;
						}
					}
					else
					{
						if(Ico_Flag)
						{
							FORM_Logo=0x10;
							Ico_Formula=Formula;
						}
						else
						{
							FORM_Logo=0;
							Ico_Formula=9;							
						}
						if(Ch_Flag)
						{
							CH1_Logo=0x10;							
							if(Ico_Flag)
								CH2_Logo=0x10;
							else
								CH2_Logo=0;
						}
						else
						{
							CH2_Logo=0x10;
							if(Ico_Flag)
								CH1_Logo=0x10;
							else
								CH1_Logo=0;
						}
					}
					if(Time_Flag)
					{
						if(Ico_Flag)
							TIM_Logo=0x80;
						else
							TIM_Logo=0;
					}
					else
					{
							TIM_Logo=0x80;
					}


				}
				jj++;
				if(jj>=300)
				{
					jj=0;
					Adc_Flag=1;///检测一次电池电量
					Flash_Flag=!Flash_Flag;////图标闪烁

				}
				if(Buzz_cnt)////蜂鸣器响
				{
					TIM_EnableCapCmpCh(TIM3, TIM_CH_4, TIM_CAP_CMP_ENABLE);////打开蜂鸣器
					Buzz_cnt--;
					if(Buzz_cnt==0)
						TIM_EnableCapCmpCh(TIM3, TIM_CH_4, TIM_CAP_CMP_DISABLE);////关闭蜂鸣器
				}
				if(MotoStop_Cnt)/*开启电磁阀操作*/
				{					
					MotoStop_Cnt--;
					SWEN_ON;
					if(MotoStop_Cnt==0)/*放气完成后关闭电磁阀，如果常开会发热*/
					{            
						SWEN_OFF;
						Key_SelectA=3;
            Mode_Change=0;
					}
				}
				
				if(MotoRun_Cnt)/*电机启动*/
				{
          Mode_Change = 1;
					Moto_Count++;
					if(Moto_Count<300)
					{
						TIM_SetCmp4(TIM4,800);/*当前PWM占空比为10%*/
					}
					else
					{
						if(Moto_Count>499)
						{
								Moto_Count=0;
							if(MotoFirst_Flag)/*第一次充气时需连续充气值70mmHg左右才停止*/
							{
								if(Press_Value>700)/*到达设定压力后停止充气*/
								{
									MotoRun_Cnt=0;
									Key_SelectA=3;
									MotoFirst_Flag=0;
								}
							}
							else
							{
								MotoRun_Cnt=0;/*后续根据按键进行充气每次约10mmHg*/
                Key_SelectA=3;
							}
						}
						TIM_SetCmp4(TIM4,0);						
					}
				}		
				hh++;
				if(hh>=1000)////1S
				{
					hh=0;
					
					if(Bl_Cnt)////背光点亮计数
					{
						Bl_Cnt--;
						BLEN_ON;
					}
					else
						BLEN_OFF;
					if(Charg_Flag)
					{
						kk++;
						if(kk>=4)
							kk=0;
						switch(kk)
						{
							case 0:
								Bat_Value0=0x10;////电池电量显示空
								Bat_Value1=0;//
								Bat_Value2=0;///
								Bat_Value3=0;///
								break;
							case 1:
								Bat_Value0=0x10;////电池电量显示一格
								Bat_Value1=0x80;//
								Bat_Value2=0;///
								Bat_Value3=0;///
								break;
							case 2:
								Bat_Value0=0x10;////电池电量显示两格
								Bat_Value1=0x80;//
								Bat_Value2=0x40;///
								Bat_Value3=0;///
								break;
							case 3:
								Bat_Value0=0x10;////电池电量显示满格
								Bat_Value1=0x80;//
								Bat_Value2=0x40;///
								Bat_Value3=0x20;///
								break;
							default:
								break;
						}
					}
					else if(Chargok_Flag)
					{
						Bat_Value0=0x10;////电池电量显示满电
						Bat_Value1=0x80;//
						Bat_Value2=0x40;///
						Bat_Value3=0x20;///
					}
					if(Pwr1||Pwr2)////电刺激强度不为零就开始倒计时
					{
            if(Second==0)
            {
              if(Minute==0)
              {
								Pwr1=0;
								Pwr2=0;
								Minute=Set_Minute;
								Bl_Cnt=60;
              }
              else 
              {
								Minute--;	
								Second = 59;
              }
            }
						else	 
						{
              Second--;
						}  
					}
//					if((Pwr1==0)&&(Pwr2==0))////如果两通道都没有工作
//					{
//						Sleep_cnt++;
//						if(Sleep_cnt>300)///5分钟待机就休眠
//							Pwr_Flag=0;
//					}
				}
				BEL_Count++;
				if(BEL_Count>1000)
				{
					BEL_Count=0;
					BEL_Beat_Flag=1;
				}
    }
}
/**
 * @brief  External lines KEY_INPUT_EXTI_LINE interrupt.
 */
/////
//void EXTI1_IRQHandler(void)
//{
//    if (RESET != EXTI_GetITStatus(EXTI_LINE1)) ////A通道电刺激脱落检测
//    {
//        EXTI_ClrITPendBit(EXTI_LINE1);
//        if(Pwr1>3)
//					E1_UnTouch_Count++;
//    }
//}
//void EXTI2_IRQHandler(void)
//{
//		if (RESET != EXTI_GetITStatus(EXTI_LINE2)) ////B通道电刺激脱落检测
//    {
//        EXTI_ClrITPendBit(EXTI_LINE2);
//				if(Pwr2>3)
//					E2_UnTouch_Count++;
//    }
//}
void EXTI15_10_IRQHandler(void)
{
    if (RESET != EXTI_GetITStatus(EXTI_LINE10)) ////CHARG唤醒
    {
        EXTI_ClrITPendBit(EXTI_LINE10);
    }
		if (RESET != EXTI_GetITStatus(EXTI_LINE15)) ////PB唤醒
    {
        EXTI_ClrITPendBit(EXTI_LINE15);
    }
}


/**
 * @}
 */
