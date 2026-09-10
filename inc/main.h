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
 * @file main.h
 * @author Nations
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g45x.h"
#include "init.h"
#include "delay.h"
#include "tm1621c.h"

#define TM1621B_CS_PORT							GPIOC
#define TM1621B_CS_PIN 							GPIO_PIN_13

#define TM1621B_CLK_PORT						GPIOC
#define TM1621B_CLK_PIN 						GPIO_PIN_14

#define TM1621B_DATA_PORT						GPIOC
#define TM1621B_DATA_PIN 						GPIO_PIN_15

#define BLEN_PORT										GPIOD
#define BLEN_PIN 										GPIO_PIN_0

#define VEN_PORT										GPIOD
#define VEN_PIN 										GPIO_PIN_1

#define DAC2_PORT 									GPIOA
#define DAC2_PIN 										GPIO_PIN_4

#define DAC1_PORT 									GPIOA
#define DAC1_PIN 										GPIO_PIN_5

#define ADCM_PORT 									GPIOA
#define ADCM_PIN 										GPIO_PIN_2

#define BATT_PORT 									GPIOA
#define BATT_PIN 										GPIO_PIN_3

#define REF_PORT 										GPIOA
#define REF_PIN 										GPIO_PIN_6

#define IN2L_PORT										GPIOA
#define IN2L_PIN 										GPIO_PIN_7

#define IN2R_PORT										GPIOB
#define IN2R_PIN 										GPIO_PIN_0

#define BUZZ_PORT 									GPIOB
#define BUZZ_PIN 										GPIO_PIN_1

#define BATEN_PORT 									GPIOB
#define BATEN_PIN 									GPIO_PIN_2

#define STDBY_PORT									GPIOB
#define STDBY_PIN 									GPIO_PIN_11

#define CHARG_PORT									GPIOB
#define CHARG_PIN 									GPIO_PIN_10

#define PB_FUN_PORT 								GPIOB
#define PB_FUN_PIN 									GPIO_PIN_13

#define PB_PWRP_PORT 								GPIOB
#define PB_PWRP_PIN 								GPIO_PIN_14

#define PB_MAIN_PORT 								GPIOB
#define PB_MAIN_PIN 								GPIO_PIN_15

#define IN1L_PORT										GPIOA
#define IN1L_PIN 										GPIO_PIN_8

#define IN1R_PORT										GPIOA
#define IN1R_PIN 										GPIO_PIN_9

#define PB_SS_PORT 									GPIOA
#define PB_SS_PIN 									GPIO_PIN_10

#define PB_PWRM_PORT 								GPIOA
#define PB_PWRM_PIN 								GPIO_PIN_11

#define SWEN_PORT 									GPIOA
#define SWEN_PIN 										GPIO_PIN_12

#define TX_PORT											GPIOB
#define TX_PIN 											GPIO_PIN_4

#define RX_PORT											GPIOB
#define RX_PIN 											GPIO_PIN_5

#define BLEEN_PORT                  GPIOB
#define BLEEN_PIN                   GPIO_PIN_6

#define BLESTA_PORT                 GPIOB
#define BLESTA_PIN                  GPIO_PIN_7

#define MOTOEN_PORT									GPIOB
#define MOTOEN_PIN 									GPIO_PIN_9











#define CONST_TIMES				68
#define Open_Ch  300

#define TREATMENT_PULSE_MODE_COUNT  3U
#define TREATMENT_CHANNEL_1         0U
#define TREATMENT_CHANNEL_2         1U
/* 台架联调：启用双通道 DAC 包络输出；量产前仍需完成负载幅值验证。 */
#define TREATMENT_DAC_OUTPUT_ENABLE 1U
/* Remote dangerous actions stay disabled until the corresponding hardware
 * calibration and bench verification have been completed. */
#define BLE_REMOTE_TREATMENT_CONTROL_ENABLE 0U
#define BLE_REMOTE_PRESSURE_CONTROL_ENABLE  0U

#define APP_BLE_USART_ERROR_OVERRUN          0x01U
#define APP_BLE_USART_ERROR_FRAME            0x02U
#define APP_BLE_USART_ERROR_NOISE            0x04U
#define APP_BLE_USART_ERROR_PARITY           0x08U

#if (BLE_REMOTE_TREATMENT_CONTROL_ENABLE > 1U) || \
    (BLE_REMOTE_PRESSURE_CONTROL_ENABLE > 1U)
#error "BLE remote control gates must be 0 or 1"
#endif

void TreatmentPulse_SetMode(uint8_t mode);
void TreatmentPulse_PrepareChannel(uint8_t channel, uint8_t mode);
void App_BleRxByteISR(uint8_t data);
void App_BleTxReadyISR(void);
void App_BleUsartErrorISR(uint8_t error_flags);



typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
} Status;

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
