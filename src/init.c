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
 * @file init.c
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "init.h"
#include "delay.h"

/** @addtogroup
 * @{
 */

/**
 * @brief  Configures the different system clocks.
 */
void RCC_Configuration(void)
{
//		RCC_ConfigPclk1(RCC_HCLK_DIV4);
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR,ENABLE);
    /* DAC Periph clock enable */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_DAC, ENABLE);
    /* GPIO Periph clock enable */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOC, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD, ENABLE);

    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO, ENABLE);
    /* Treatment PWM timer */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_TIM1, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_TIM8, ENABLE);
    /* TIM2 clock enable */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM2, ENABLE);
    /* TIM3 clock enable */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM3, ENABLE);
//    /* TIM4 clock enable */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM4, ENABLE);
    /* TIM6 Periph clock enable */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM6, ENABLE);
    /* Enable USARTy and USARTz Clock */
//    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_UART5, ENABLE);
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_USART2, ENABLE);
//    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_USART3, ENABLE);
    /* Enable ADC1, ADC2, ADC3 and ADC4 clocks */
    RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_ADC1 | RCC_AHB_PERIPH_ADC2 | RCC_AHB_PERIPH_ADC3 | RCC_AHB_PERIPH_ADC4,
                           ENABLE);
    /* RCC_ADCHCLK_DIV16*/
    ADC_ConfigClk(ADC_CTRL3_CKMOD_AHB,RCC_ADCHCLK_DIV16);

    /*
     * N32G45x ADC also needs a dedicated 1 MHz timing clock. SystemInit()
     * resets CFG2 to HSI/1, which is 8 MHz, so divide it explicitly.
     */
    RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSI, RCC_ADC1MCLK_DIV8);

}
/**
 * @brief  Configures the different GPIO ports.
 */
void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;

    GPIO_InitStruct(&GPIO_InitStructure);
    /* GPIOA Periph clock enable */
	
		GPIO_InitStructure.Pin       = SWEN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.Pin       = BATT_PIN|REF_PIN|ADCM_PIN|DAC1_PIN|DAC2_PIN;////
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.Pin       = IN1L_PIN|IN1R_PIN|IN2L_PIN;/* TIM1_CH1, CH2, CH1N */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.Pin       = PB_PWRM_PIN|PB_SS_PIN;//////下拉
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.Pin       = BATEN_PIN|BLEEN_PIN;////
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.Pin       = STDBY_PIN|CHARG_PIN;////
    /* CHARG/STDBY are active-low status inputs; keep their idle level defined. */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.Pin       = PB_MAIN_PIN|PB_PWRP_PIN|PB_FUN_PIN;////下拉
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
		
		/* Treatment PWM output: TIM8_CH2N. */
		GPIO_InitStructure.Pin       = IN2R_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

		/* Keep the existing peripheral functions for buzzer and motor PWM. */
		GPIO_InitStructure.Pin       = BUZZ_PIN|MOTOEN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.Pin       = TM1621B_CS_PIN|TM1621B_CLK_PIN|TM1621B_DATA_PIN;/////
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(GPIOC, &GPIO_InitStructure);
		
		GPIO_InitStructure.Pin       = VEN_PIN|BLEN_PIN;/////
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(GPIOD, &GPIO_InitStructure);
		
		
    /* Configure USARTy Rx as input floating */
    GPIO_InitStructure.Pin       = RX_PIN;////USART2_RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
    /* Configure USARTy Tx as alternate function push-pull */
    GPIO_InitStructure.Pin        = TX_PIN;////USART2_TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

		
		GPIO_ConfigPinRemap(GPIO_RMP_SW_JTAG_SW_ENABLE,ENABLE);////PB4
    GPIO_ConfigPinRemap(GPIO_PART1_RMP_TIM1,ENABLE);////PA7,PB0
		GPIO_ConfigPinRemap(GPIO_RMP3_USART2,ENABLE);////PB4,PB5
		GPIO_ConfigPinRemap(GPIO_RMP_PD01,ENABLE);////PD0/PD1
}
/**
 * @brief  Configures tim1 clocks.
 */
void TIM1_Configuration(void)
{
    TIM_TimeBaseInitType TIM_TimeBaseStructure;
    OCInitType TIM_OCInitStructure;
    /* Time base configuration */
	TIM_TimeBaseStructure.Period    = 9999;/* 16 MHz / 10000 = 1600 Hz; alternating L/R gives 800 Hz biphasic groups */
    TIM_TimeBaseStructure.Prescaler = 7;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_DOWN;
    TIM_TimeBaseStructure.RepetCnt  = 0;

    TIM_InitTimeBase(TIM1, &TIM_TimeBaseStructure);
    TIM_ConfigArPreload(TIM1, ENABLE);

    /* PWM1 Mode configuration: Channel1 *////1L
    TIM_OCInitStructure.OcMode       = TIM_OCMODE_PWM2;/* 300 us pulse at the end of each 625 us slot. */
    TIM_OCInitStructure.OutputState  = TIM_OUTPUT_STATE_ENABLE;///输出使能
    TIM_OCInitStructure.OutputNState = TIM_OUTPUT_NSTATE_DISABLE;
    TIM_OCInitStructure.Pulse        = 4800;/* 300 us at 16 MHz */
    TIM_OCInitStructure.OcPolarity   = TIM_OC_POLARITY_HIGH;///优先�?
    TIM_OCInitStructure.OcNPolarity  = TIM_OCN_POLARITY_HIGH;
    TIM_OCInitStructure.OcIdleState  = TIM_OC_IDLE_STATE_RESET;
    TIM_OCInitStructure.OcNIdleState = TIM_OC_IDLE_STATE_RESET;

    TIM_InitOc1(TIM1, &TIM_OCInitStructure);

    /* PWM1 Mode configuration: Channel2 *////1R
    TIM_OCInitStructure.OcMode       = TIM_OCMODE_PWM2;
    TIM_OCInitStructure.OutputState  = TIM_OUTPUT_STATE_ENABLE;///输出使能
    TIM_OCInitStructure.OutputNState = TIM_OUTPUT_NSTATE_DISABLE;
    TIM_OCInitStructure.Pulse        = 4800;/* 300 us at 16 MHz */
    TIM_OCInitStructure.OcPolarity   = TIM_OC_POLARITY_HIGH;///优先�?
    TIM_OCInitStructure.OcNPolarity  = TIM_OCN_POLARITY_HIGH;
    TIM_OCInitStructure.OcIdleState  = TIM_OC_IDLE_STATE_RESET;
    TIM_OCInitStructure.OcNIdleState = TIM_OC_IDLE_STATE_RESET;

    TIM_InitOc2(TIM1, &TIM_OCInitStructure);

    /* TIM1 enable update irq */
    
	TIM_EnableCtrlPwmOutputs(TIM1, ENABLE);
    /* TIM1 enable counter */
    
	TIM_ConfigInt(TIM1, TIM_INT_UPDATE, ENABLE);
	TIM_EnableCapCmpCh(TIM1, TIM_CH_1, TIM_CAP_CMP_DISABLE);
	TIM_EnableCapCmpCh(TIM1, TIM_CH_2, TIM_CAP_CMP_DISABLE);
	TIM_Enable(TIM1, ENABLE);
    
}
/**
 * @brief  Configures tim8 clocks.
 */
#if 0 /* Legacy TIM8 treatment output; replaced by TIM1 CH1/CH1N and CH2/CH2N. */
void TIM8_Configuration(void)
{
    TIM_TimeBaseInitType TIM_TimeBaseStructure;
    OCInitType TIM_OCInitStructure;
    /* Time base configuration */
    TIM_TimeBaseStructure.Period    = 7999;////way2:2kHZ
    TIM_TimeBaseStructure.Prescaler = 7;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_DOWN;
    TIM_TimeBaseStructure.RepetCnt  = 0;

    TIM_InitTimeBase(TIM8, &TIM_TimeBaseStructure);

    /* PWM1 Mode configuration: Channel1 *////2R
    TIM_OCInitStructure.OcMode       = TIM_OCMODE_PWM1;///波形设置
    TIM_OCInitStructure.OutputState  = TIM_OUTPUT_STATE_DISABLE;///输出使能
    TIM_OCInitStructure.OutputNState = TIM_OUTPUT_NSTATE_ENABLE;
    TIM_OCInitStructure.Pulse        = 7800;///占空�?=Pulse/Period
    TIM_OCInitStructure.OcPolarity   = TIM_OC_POLARITY_HIGH;///优先�?
    TIM_OCInitStructure.OcNPolarity  = TIM_OCN_POLARITY_HIGH;
    TIM_OCInitStructure.OcIdleState  = TIM_OC_IDLE_STATE_RESET;
    TIM_OCInitStructure.OcNIdleState = TIM_OC_IDLE_STATE_RESET;

    TIM_InitOc1(TIM8, &TIM_OCInitStructure);

    /* PWM1 Mode configuration: Channel2 *////2L
    TIM_OCInitStructure.OcMode       = TIM_OCMODE_PWM1;///波形设置
//    TIM_OCInitStructure.OutputState  = TIM_OUTPUT_STATE_DISABLE;///输出使能
//    TIM_OCInitStructure.OutputNState = TIM_OUTPUT_NSTATE_ENABLE;
    TIM_OCInitStructure.Pulse        = 7800;///占空�?=Pulse/Period
//    TIM_OCInitStructure.OcPolarity   = TIM_OC_POLARITY_HIGH;///优先�?
//    TIM_OCInitStructure.OcNPolarity  = TIM_OCN_POLARITY_HIGH;
//    TIM_OCInitStructure.OcIdleState  = TIM_OC_IDLE_STATE_RESET;
//    TIM_OCInitStructure.OcNIdleState = TIM_OC_IDLE_STATE_RESET;

    TIM_InitOc2(TIM8, &TIM_OCInitStructure);

    /* TIM8 enable update irq */
    TIM_ConfigInt(TIM8, TIM_INT_UPDATE, ENABLE);

    /* TIM8 enable counter */
    TIM_Enable(TIM8, ENABLE);
    TIM_EnableCtrlPwmOutputs(TIM8, ENABLE);
}
#endif

/* TIM8 schedules the second treatment bridge (IN2L/IN2R). */
void TIM8_Configuration(void)
{
    TIM_TimeBaseInitType time_base;
    OCInitType oc_init;

    time_base.Period    = 9999U;
    time_base.Prescaler = 7U;
    time_base.ClkDiv    = 0U;
    time_base.CntMode   = TIM_CNT_MODE_DOWN;
    time_base.RepetCnt  = 0U;
    TIM_InitTimeBase(TIM8, &time_base);
    TIM_ConfigArPreload(TIM8, ENABLE);

    oc_init.OcMode       = TIM_OCMODE_PWM2;
    oc_init.OutputState  = TIM_OUTPUT_STATE_DISABLE;
    oc_init.OutputNState = TIM_OUTPUT_NSTATE_ENABLE;
    oc_init.Pulse        = 4800U;
    oc_init.OcPolarity   = TIM_OC_POLARITY_HIGH;
    oc_init.OcNPolarity  = TIM_OCN_POLARITY_HIGH;
    oc_init.OcIdleState  = TIM_OC_IDLE_STATE_RESET;
    oc_init.OcNIdleState = TIM_OC_IDLE_STATE_RESET;
    TIM_InitOc1(TIM8, &oc_init);
    TIM_InitOc2(TIM8, &oc_init);

    TIM_EnableCtrlPwmOutputs(TIM8, ENABLE);
    TIM_ConfigInt(TIM8, TIM_INT_UPDATE, ENABLE);
    TIM_EnableCapCmpChN(TIM8, TIM_CH_1, TIM_CAP_CMP_N_DISABLE);
    TIM_EnableCapCmpChN(TIM8, TIM_CH_2, TIM_CAP_CMP_N_DISABLE);
    TIM_Enable(TIM8, ENABLE);
}

//void TIM3_Configuration(void)////通用定时�?
//{
//    TIM_TimeBaseInitType TIM_TimeBaseStructure;


//    /* Time base configuration */
//    TIM_TimeBaseStructure.Period    = 7999;////(7+1)*(7999+1)/64=1ms
//    TIM_TimeBaseStructure.Prescaler = 1;
//    TIM_TimeBaseStructure.ClkDiv    = 0;
//    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;

//    TIM_InitTimeBase(TIM3, &TIM_TimeBaseStructure);
//    /* Prescaler configuration */

//    /* TIM2 enable update irq */
//    TIM_ConfigInt(TIM3, TIM_INT_UPDATE, ENABLE);

//    /* TIM2 enable counter */
//    TIM_Enable(TIM3, ENABLE);
//}
//void TIM2_Configuration(void)////DAC
//{
//    TIM_TimeBaseInitType TIM_TimeBaseStructure;
//    OCInitType TIM_OCInitStructure;
//    /* Compute the prescaler value */

//    /* Time base configuration */
//    TIM_TimeBaseStructure.Period    = 2560;////
//    TIM_TimeBaseStructure.Prescaler = 0;
//    TIM_TimeBaseStructure.ClkDiv    = 0;
//    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;

//    TIM_InitTimeBase(TIM2, &TIM_TimeBaseStructure);
//		
//		TIM_OCInitStructure.OcMode      = TIM_OCMODE_PWM1;
//    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
//    TIM_OCInitStructure.Pulse       = 0;
//    TIM_OCInitStructure.OcPolarity  = TIM_OC_POLARITY_HIGH;
//		
//		TIM_InitOc1(TIM2, &TIM_OCInitStructure);

//    TIM_ConfigOc1Preload(TIM2, TIM_OC_PRE_LOAD_ENABLE);////R
//	
//		TIM_OCInitStructure.OcMode      = TIM_OCMODE_PWM1;
//    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
//    TIM_OCInitStructure.Pulse       = 0;
//    TIM_OCInitStructure.OcPolarity  = TIM_OC_POLARITY_HIGH;
//		
//		TIM_InitOc2(TIM2, &TIM_OCInitStructure);

//    TIM_ConfigOc2Preload(TIM2, TIM_OC_PRE_LOAD_ENABLE);////R

//    TIM_ConfigArPreload(TIM2, ENABLE);

//    /* TIM3 enable counter */
//    TIM_Enable(TIM2, ENABLE);
//}
void TIM3_Configuration(void)////BUZZ
{
    TIM_TimeBaseInitType TIM_TimeBaseStructure;
    OCInitType TIM_OCInitStructure;
    /* Compute the prescaler value */

    /* Time base configuration */
    TIM_TimeBaseStructure.Period    = 5925;////2.7khz
    TIM_TimeBaseStructure.Prescaler = 3;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;

    TIM_InitTimeBase(TIM3, &TIM_TimeBaseStructure);
		
		TIM_OCInitStructure.OcMode      = TIM_OCMODE_PWM1;
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse       = 2963;
    TIM_OCInitStructure.OcPolarity  = TIM_OC_POLARITY_HIGH;
		
		TIM_InitOc4(TIM3, &TIM_OCInitStructure);

    TIM_ConfigOc4Preload(TIM3, TIM_OC_PRE_LOAD_ENABLE);////

    TIM_ConfigArPreload(TIM3, ENABLE);

    /* TIM3 enable counter */
    TIM_Enable(TIM3, ENABLE);
}
/**
 * @brief  Configures tim4 clocks.
 */
void TIM2_Configuration(void)////通用定时�?
{
    TIM_TimeBaseInitType TIM_TimeBaseStructure;

    TIM2_PrescalerValue = 7; //(uint16_t) (SystemCoreClock / 12000000) - 1;

    /* Time base configuration */
    TIM_TimeBaseStructure.Period    = 7999;////(7+1)*(7999+1)/64=1ms
    TIM_TimeBaseStructure.Prescaler = 0;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;

    TIM_InitTimeBase(TIM2, &TIM_TimeBaseStructure);
    /* Prescaler configuration */
    TIM_ConfigPrescaler(TIM2, TIM2_PrescalerValue, TIM_PSC_RELOAD_MODE_IMMEDIATE);

    /* TIM2 enable update irq */
    TIM_ConfigInt(TIM2, TIM_INT_UPDATE, ENABLE);

    /* TIM2 enable counter */
    TIM_Enable(TIM2, ENABLE);
}



void TIM4_Configuration(void)////MOTO
{
    TIM_TimeBaseInitType TIM_TimeBaseStructure;
    OCInitType TIM_OCInitStructure;
    /* Compute the prescaler value */

    /* Time base configuration */
    TIM_TimeBaseStructure.Period    = 7999;////100hz
    TIM_TimeBaseStructure.Prescaler = 799;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;

    TIM_InitTimeBase(TIM4, &TIM_TimeBaseStructure);
		
		TIM_OCInitStructure.OcMode      = TIM_OCMODE_PWM1;
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse       = 0;
    TIM_OCInitStructure.OcPolarity  = TIM_OC_POLARITY_HIGH;
		
		TIM_InitOc4(TIM4, &TIM_OCInitStructure);

    TIM_ConfigOc4Preload(TIM4, TIM_OC_PRE_LOAD_ENABLE);////R

    TIM_ConfigArPreload(TIM4, ENABLE);

    /* TIM3 enable counter */
    TIM_Enable(TIM4, ENABLE);
}
/**
 * @brief  TIM6 Init.
 */
void TIM6_Configuration(void)////DAC 时钟
{
    TIM_TimeBaseInitType TIM_TimeBaseStructure;

    /* TIM6 configuration */
    TIM_InitTimBaseStruct(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.Period    = 639;
    TIM_TimeBaseStructure.Prescaler = 0;
    TIM_TimeBaseStructure.ClkDiv    = 0x0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_InitTimeBase(TIM6, &TIM_TimeBaseStructure);

    /* TIM6 TRGO selection */
    TIM_SelectOutputTrig(TIM6, TIM_TRGO_SRC_UPDATE);
}

void USART2_Configuration(void)////上位�?
{
    USART_InitType USART_InitStructure;
    /* USARTy and USARTz configuration ------------------------------------------------------*/
    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.BaudRate            = 115200;
    USART_InitStructure.WordLength          = USART_WL_8B;
    USART_InitStructure.StopBits            = USART_STPB_1;
    USART_InitStructure.Parity              = USART_PE_NO;///检验模�?
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;

    /* Configure USARTy */
    USART_Init(USART2, &USART_InitStructure);
    /* Enable USARTz Receive interrupts */
    USART_ConfigInt(USART2, USART_INT_RXDNE, ENABLE);////开启接收中�?
//    USART_ConfigInt(USART1, USART_INT_TXDE, ENABLE);///开启发送中�?
    /* Enable the USARTy */
    USART_Enable(USART2, ENABLE);
}
//void USART3_Configuration(void)////上位�?
//{
//    USART_InitType USART_InitStructure;
//    /* USARTy and USARTz configuration ------------------------------------------------------*/
//    USART_StructInit(&USART_InitStructure);
//    USART_InitStructure.BaudRate            = 115200;
//    USART_InitStructure.WordLength          = USART_WL_8B;
//    USART_InitStructure.StopBits            = USART_STPB_1;
//    USART_InitStructure.Parity              = USART_PE_NO;///检验模�?
//    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
//    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;

//    /* Configure USARTy */
//    USART_Init(USART3, &USART_InitStructure);
//    /* Enable USARTz Receive interrupts */
//    USART_ConfigInt(USART3, USART_INT_RXDNE, ENABLE);////开启接收中�?
//    //		USART_ConfigInt(USART3, USART_INT_TXDE, ENABLE);///开启发送中�?
//    /* Enable the USARTy */
//    USART_Enable(USART3, ENABLE);
//}
//void USART5_Configuration(void)////串口
//{
//    USART_InitType USART_InitStructure;
//    /* USARTy and USARTz configuration ------------------------------------------------------*/
//    USART_StructInit(&USART_InitStructure);
//    USART_InitStructure.BaudRate            = 115200;
//    USART_InitStructure.WordLength          = USART_WL_8B;
//    USART_InitStructure.StopBits            = USART_STPB_1;
//    USART_InitStructure.Parity              = USART_PE_NO;///检验模�?
//    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
//    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;

//    /* Configure USARTy */
//    USART_Init(UART5, &USART_InitStructure);
//    /* Enable USARTz Receive interrupts */
//    USART_ConfigInt(UART5, USART_INT_RXDNE, ENABLE);////开启接收中�?
//    //		USART_ConfigInt(UART4, USART_INT_TXDE, ENABLE);///开启发送中�?
//    /* Enable the USARTy */
//    USART_Enable(UART5, ENABLE);
//}
/**
 * @brief  Configure the nested vectored interrupt controller.
 */
void NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    /* Enable the TIM1 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM8_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Enable the TIM2 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM2_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Enable the USART2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel            = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  Configures the different GPIO ports.
 */
void DAC_ChannelConfig(void)
{
    DAC_InitType DAC_InitStructure;

    /* DAC channel1 Configuration */
    DAC_InitStructure.Trigger          = DAC_TRG_T6_TRGO;////T6定时器触�?
    DAC_InitStructure.WaveGen          = DAC_WAVEGEN_NOISE;
    DAC_InitStructure.LfsrUnMaskTriAmp = DAC_UNMASK_LFSRBIT0;
    DAC_InitStructure.BufferOutput     = DAC_BUFFOUTPUT_ENABLE;///输出使能
    DAC_Init(DAC_CHANNEL_1,&DAC_InitStructure);
    DAC_Init(DAC_CHANNEL_2,&DAC_InitStructure);

    /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is
       automatically connected to the DAC converter. */
    DAC_Enable(DAC_CHANNEL_1, ENABLE);
    DAC_Enable(DAC_CHANNEL_2, ENABLE);
    /* Set DAC Channel1 DHR12L register */
    DAC_SetCh1Data(DAC_ALIGN_R_12BIT, 4090);
    DAC_SetCh2Data(DAC_ALIGN_R_12BIT, 4090);//900
}

//void E1countExtiInit(void)
//{
//    EXTI_InitType EXTI_InitStructure;
//    NVIC_InitType NVIC_InitStructure;

//    /* Check the parameters */
//    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

//    /*Configure key EXTI Line to key input Pin*/
//    GPIO_ConfigEXTILine(GPIOB_PORT_SOURCE, GPIO_PIN_SOURCE1);

//    /*Configure key EXTI line*/
//    EXTI_InitStructure.EXTI_Line    = EXTI_LINE1;
//    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // EXTI_Trigger_Falling;
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//    EXTI_InitPeripheral(&EXTI_InitStructure);

//    /*Set key input interrupt priority*/
//    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI1_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x0F;
//    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//}
//void E2countExtiInit(void)
//{
//    EXTI_InitType EXTI_InitStructure;
//    NVIC_InitType NVIC_InitStructure;

//    /* Check the parameters */
//    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

//    /*Configure key EXTI Line to key input Pin*/
//    GPIO_ConfigEXTILine(GPIOB_PORT_SOURCE, GPIO_PIN_SOURCE2);

//    /*Configure key EXTI line*/
//    EXTI_InitStructure.EXTI_Line    = EXTI_LINE2;
//    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // EXTI_Trigger_Falling;
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//    EXTI_InitPeripheral(&EXTI_InitStructure);

//    /*Set key input interrupt priority*/
//    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI2_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x0F;
//    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//}
void ChargExtiInit(void)
{
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    /*Configure key EXTI Line to key input Pin*/
    GPIO_ConfigEXTILine(GPIOB_PORT_SOURCE, GPIO_PIN_SOURCE10);

    /*Configure key EXTI line*/
    EXTI_InitStructure.EXTI_Line    = EXTI_LINE10;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitPeripheral(&EXTI_InitStructure);

    /*Set key input interrupt priority*/
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void PBExtiInit(void)
{
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    /*Configure key EXTI Line to key input Pin*/
    GPIO_ConfigEXTILine(GPIOB_PORT_SOURCE, GPIO_PIN_SOURCE15);

    /*Configure key EXTI line*/
    EXTI_InitStructure.EXTI_Line    = EXTI_LINE15;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitPeripheral(&EXTI_InitStructure);

    /*Set key input interrupt priority*/
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
uint8_t ADC_Initial(ADC_Module* ADCx)
{
    ADC_InitType ADC_InitStructure;
    uint32_t timeout = 100000U;
    /* ADC configuration ------------------------------------------------------*/
    ADC_InitStructure.WorkMode       = ADC_WORKMODE_INDEPENDENT;
    ADC_InitStructure.MultiChEn      = DISABLE;
    ADC_InitStructure.ContinueConvEn = DISABLE;
    ADC_InitStructure.ExtTrigSelect  = ADC_EXT_TRIGCONV_NONE;
    ADC_InitStructure.DatAlign       = ADC_DAT_ALIGN_R;
    ADC_InitStructure.ChsNumber      = 1;
    ADC_Init(ADCx, &ADC_InitStructure);


    /* Enable ADC */
    ADC_Enable(ADCx, ENABLE);
    /*Check ADC Ready*/
    while (ADC_GetFlagStatusNew(ADCx, ADC_FLAG_RDY) == RESET)
    {
        if (--timeout == 0U)
        {
            (void)ADC_DisableSafe(ADCx);
            return 0U;
        }
    }
    /* Official requirement: wait at least 8 us after ADC enable. */
    Delay10us(1U);
    /* Start ADC calibration */
    ADC_StartCalibration(ADCx);
    /* Check the end of ADC calibration */
    timeout = 100000U;
    while (ADC_GetCalibrationStatus(ADCx))
    {
        if (--timeout == 0U)
        {
            (void)ADC_DisableSafe(ADCx);
            return 0U;
        }
    }
    /* Official requirement: wait at least 8 us after calibration. */
    Delay10us(1U);

    return 1U;
}
uint8_t ADC_DisableSafe(ADC_Module* ADCx)
{
    uint32_t timeout = 100000U;

    ADC_Enable(ADCx, DISABLE);
    while (ADC_GetFlagStatusNew(ADCx, ADC_FLAG_PD_RDY) == RESET)
    {
        if (--timeout == 0U)
        {
            return 0U;
        }
    }

    return 1U;
}
uint8_t ADC_GetData(ADC_Module* ADCx, uint8_t ADC_Channel, uint16_t *data)
{
    uint32_t timeout = 100000U;

    if (data == NULL)
    {
        return 0U;
    }
    *data = 0U;

    ADC_ClearFlag(ADCx, ADC_FLAG_ENDC);
    ADC_ClearFlag(ADCx, ADC_FLAG_STR);
    ADC_ConfigRegularChannel(ADCx, ADC_Channel, 1, ADC_SAMP_TIME_239CYCLES5);
    /* Start ADC Software Conversion */
    ADC_EnableSoftwareStartConv(ADCx, ENABLE);
    while (ADC_GetFlagStatus(ADCx, ADC_FLAG_ENDC) == RESET) {
        if (--timeout == 0U) {
            return 0U;
        }
    }
    /*
     * N32G455 erratum: after ENDC, wait at least two ADC clocks before
     * reading DAT. ADC clock is AHB/16 here, so 1 us provides margin.
     */
    Delay1us(1U);
    *data = ADC_GetDat(ADCx);
    ADC_ClearFlag(ADCx, ADC_FLAG_ENDC);
    ADC_ClearFlag(ADCx, ADC_FLAG_STR);
    return 1U;
}


