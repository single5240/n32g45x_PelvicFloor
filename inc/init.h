/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
**
 * ****************************************************************************/

/**
 * @file init.h
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#ifndef __INIT_H__
#define __INIT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g45x.h"
#include "main.h"
#include "string.h"
#include "stdbool.h"
#include "stdio.h"

/* BATEN 高电平使能，低电平关闭。 */
#define BATEN_ON 							GPIO_WriteBit(BATEN_PORT,BATEN_PIN,Bit_SET)
#define BATEN_OFF 						GPIO_WriteBit(BATEN_PORT,BATEN_PIN,Bit_RESET)

#define TM1621C_CS_ON 				GPIO_WriteBit(TM1621B_CS_PORT,TM1621B_CS_PIN,Bit_SET)
#define TM1621C_CS_OFF 				GPIO_WriteBit(TM1621B_CS_PORT,TM1621B_CS_PIN,Bit_RESET)

#define TM1621C_CLK_ON 				GPIO_WriteBit(TM1621B_CLK_PORT,TM1621B_CLK_PIN,Bit_SET)
#define TM1621C_CLK_OFF 			GPIO_WriteBit(TM1621B_CLK_PORT,TM1621B_CLK_PIN,Bit_RESET)

#define TM1621C_DATA_ON 			GPIO_WriteBit(TM1621B_DATA_PORT,TM1621B_DATA_PIN,Bit_SET)
#define TM1621C_DATA_OFF 			GPIO_WriteBit(TM1621B_DATA_PORT,TM1621B_DATA_PIN,Bit_RESET)

#define VEN_ON 								GPIO_WriteBit(VEN_PORT,VEN_PIN,Bit_SET)
#define VEN_OFF 							GPIO_WriteBit(VEN_PORT,VEN_PIN,Bit_RESET)

#define BLEN_ON 							GPIO_WriteBit(BLEN_PORT,BLEN_PIN,Bit_SET)
#define BLEN_OFF 							GPIO_WriteBit(BLEN_PORT,BLEN_PIN,Bit_RESET)

#define BLEEN_ON              GPIO_WriteBit(BLEEN_PORT,BLEEN_PIN,Bit_RESET)
#define BLEEN_OFF             GPIO_WriteBit(BLEEN_PORT,BLEEN_PIN,Bit_SET)

#define SWEN_ON 							GPIO_WriteBit(SWEN_PORT,SWEN_PIN,Bit_SET)
#define SWEN_OFF 							GPIO_WriteBit(SWEN_PORT,SWEN_PIN,Bit_RESET)

#define MOTOR_PWM_FREQUENCY_HZ       10000U
#define MOTOR_PWM_PERIOD_COUNTS      12800U
#define MOTOR_PWM_DUTY_HIGH_VOLTAGE_PERCENT   43U
#define MOTOR_PWM_DUTY_LOW_VOLTAGE_PERCENT    47U
#define MOTOR_PWM_HIGH_VOLTAGE_MV           4200U
#define MOTOR_PWM_LOW_VOLTAGE_MV            3500U

#if (MOTOR_PWM_DUTY_HIGH_VOLTAGE_PERCENT == 0U) || \
    (MOTOR_PWM_DUTY_HIGH_VOLTAGE_PERCENT > 100U) || \
    (MOTOR_PWM_DUTY_LOW_VOLTAGE_PERCENT == 0U) || \
    (MOTOR_PWM_DUTY_LOW_VOLTAGE_PERCENT > 100U)
#error "Motor PWM duty must be within 1..100"
#endif

#if (MOTOR_PWM_LOW_VOLTAGE_MV >= MOTOR_PWM_HIGH_VOLTAGE_MV)
#error "Motor PWM compensation voltage range is invalid"
#endif

#define READ_PB_MAIN					GPIO_ReadInputDataBit(PB_MAIN_PORT,PB_MAIN_PIN)
#define READ_PB_SS						GPIO_ReadInputDataBit(PB_SS_PORT,PB_SS_PIN)
#define READ_PB_FUN						GPIO_ReadInputDataBit(PB_FUN_PORT,PB_FUN_PIN)
#define READ_PB_PWRM					GPIO_ReadInputDataBit(PB_PWRM_PORT,PB_PWRM_PIN)
#define READ_PB_PWRP					GPIO_ReadInputDataBit(PB_PWRP_PORT,PB_PWRP_PIN)
#define READ_STDBY						GPIO_ReadInputDataBit(STDBY_PORT,STDBY_PIN)
#define READ_CHARG						GPIO_ReadInputDataBit(CHARG_PORT,CHARG_PIN)
#define READ_BLESTA                    GPIO_ReadInputDataBit(BLESTA_PORT,BLESTA_PIN)

extern uint16_t TIM2_PrescalerValue;

void RCC_Configuration(void);
void GPIO_Configuration(void);
void TIM1_Configuration(void);
void TIM2_Configuration(void);
void TIM3_Configuration(void);
void TIM4_Configuration(void);
void TIM8_Configuration(void);
void TIM6_Configuration(void);
void USART2_Configuration(void);
uint8_t IWDG_Configuration(void);
uint8_t Clock_RestoreAfterStop0(void);
void NVIC_Configuration(void);
void DAC_ChannelConfig(void);
//void E1countExtiInit(void);
//void E2countExtiInit(void);
void LowPowerWakeExtiInit(void);
uint8_t ADC1_Initial(void);
uint8_t ADC2_Initial(void);
uint8_t ADC_DisableSafe(ADC_Module* ADCx);
uint8_t ADC_GetData(ADC_Module* ADCx, uint8_t ADC_Channel, uint16_t *data);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */



