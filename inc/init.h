/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
**
 * ****************************************************************************/

/**
 * @file init.h
<<<<<<< .mine
 * @author Nations
=======
 * @author Nations
>>>>>>> .r78072
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

#define READ_PB_MAIN					GPIO_ReadInputDataBit(PB_MAIN_PORT,PB_MAIN_PIN)
#define READ_PB_SS						GPIO_ReadInputDataBit(PB_SS_PORT,PB_SS_PIN)
#define READ_PB_FUN						GPIO_ReadInputDataBit(PB_FUN_PORT,PB_FUN_PIN)
#define READ_PB_PWRM					GPIO_ReadInputDataBit(PB_PWRM_PORT,PB_PWRM_PIN)
#define READ_PB_PWRP					GPIO_ReadInputDataBit(PB_PWRP_PORT,PB_PWRP_PIN)
#define READ_STDBY						GPIO_ReadInputDataBit(STDBY_PORT,STDBY_PIN)
#define READ_CHARG						GPIO_ReadInputDataBit(CHARG_PORT,CHARG_PIN)

extern uint16_t TIM2_PrescalerValue;
extern uint16_t TIM5_PrescalerValue;

void RCC_Configuration(void);
void GPIO_Configuration(void);
void TIM1_Configuration(void);
void TIM2_Configuration(void);
void TIM3_Configuration(void);
void TIM4_Configuration(void);
void TIM5_Configuration(void);
void TIM8_Configuration(void);
void TIM6_Configuration(void);
void USART2_Configuration(void);
void NVIC_Configuration(void);
void DAC_ChannelConfig(void);
//void E1countExtiInit(void);
//void E2countExtiInit(void);
void ChargExtiInit(void);
void PBExtiInit(void);
uint8_t ADC_Initial(ADC_Module* ADCx);
uint8_t ADC_DisableSafe(ADC_Module* ADCx);
uint16_t ADC_GetData(ADC_Module* ADCx, uint8_t ADC_Channel);


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



