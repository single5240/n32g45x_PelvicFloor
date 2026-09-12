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

#define APP_DIAG_FAULT_HARDFAULT   1U
#define APP_DIAG_FAULT_MEMMANAGE   2U
#define APP_DIAG_FAULT_BUSFAULT    3U
#define APP_DIAG_FAULT_USAGEFAULT  4U
#define APP_DIAG_FAULT_ASSERT      5U

void App_DiagnosticsRecordFaultISR(uint16_t fault_type);
void App_DiagnosticsRecordFaultContextISR(uint16_t fault_type,
                                          const uint32_t *stack_frame,
                                          uint32_t exc_return);

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
/* 台架联调：置 1 后启用桥臂 PWM；死区未验收前默认保持关闭。 */
#define TREATMENT_BRIDGE_PWM_OUTPUT_ENABLE 1U
/* 联调值基于当前 128 MHz 定时器时钟和预分频 7，量产值待实测确认。 */
#define TREATMENT_BRIDGE_DEADTIME_US        50U
#define TREATMENT_TIMER_TICKS_PER_US        16U
#define TREATMENT_TIMER_RELOAD_VALUE        7999U
#define TREATMENT_BRIDGE_DEADTIME_COMPARE  \
	(TREATMENT_TIMER_RELOAD_VALUE -          \
	 (TREATMENT_BRIDGE_DEADTIME_US * TREATMENT_TIMER_TICKS_PER_US))
/* 台架联调：置 1 后两路 DAC 使用固定码值；量产构建必须保持关闭。 */
#define TREATMENT_DAC_FIXED_VALUE_TEST_ENABLE 0U
#define TREATMENT_DAC_FIXED_VALUE             2000U

#if (TREATMENT_DAC_FIXED_VALUE_TEST_ENABLE > 1U)
#error "TREATMENT_DAC_FIXED_VALUE_TEST_ENABLE must be 0 or 1"
#endif

#if (TREATMENT_BRIDGE_PWM_OUTPUT_ENABLE > 1U)
#error "TREATMENT_BRIDGE_PWM_OUTPUT_ENABLE must be 0 or 1"
#endif

#if (TREATMENT_BRIDGE_DEADTIME_US < 13U) || \
    (TREATMENT_BRIDGE_DEADTIME_US >= 500U)
#error "TREATMENT_BRIDGE_DEADTIME_US must be within 13..499 us"
#endif

#if (TREATMENT_DAC_FIXED_VALUE > 3800U)
#error "TREATMENT_DAC_FIXED_VALUE must not exceed 3800"
#endif

/* Remote actions use the local event/state-machine path. Dangerous output
 * actions still require the runtime charging and physical-link checks. */
#define BLE_REMOTE_POWER_OFF_CONTROL_ENABLE 1U
#define BLE_REMOTE_TREATMENT_CONTROL_ENABLE 1U
#define BLE_REMOTE_PRESSURE_CONTROL_ENABLE  1U

/* With the nominal 40 kHz LSI and /32 prescaler, 2499 gives about 2 s. */
#define APP_IWDG_ENABLE                       1U
#define APP_IWDG_RELOAD_VALUE                 2499U
/* Diagnostic gate: keep the buzzer timer and PB1 output disabled when 0. */
#define BUZZER_OUTPUT_ENABLE                  1U
/* ARM Cortex-M4 r0p0/r0p1 erratum 838869: disable the default write buffer. */
#define APP_CORTEX_M4_838869_WORKAROUND_ENABLE 1U

#define APP_BLE_USART_ERROR_OVERRUN          0x01U
#define APP_BLE_USART_ERROR_FRAME            0x02U
#define APP_BLE_USART_ERROR_NOISE            0x04U
#define APP_BLE_USART_ERROR_PARITY           0x08U

#if (BLE_REMOTE_POWER_OFF_CONTROL_ENABLE > 1U) || \
    (BLE_REMOTE_TREATMENT_CONTROL_ENABLE > 1U) || \
    (BLE_REMOTE_PRESSURE_CONTROL_ENABLE > 1U)
#error "BLE remote control gates must be 0 or 1"
#endif

#if (APP_IWDG_ENABLE > 1U)
#error "APP_IWDG_ENABLE must be 0 or 1"
#endif

#if (BUZZER_OUTPUT_ENABLE > 1U)
#error "BUZZER_OUTPUT_ENABLE must be 0 or 1"
#endif

#if (APP_CORTEX_M4_838869_WORKAROUND_ENABLE > 1U)
#error "APP_CORTEX_M4_838869_WORKAROUND_ENABLE must be 0 or 1"
#endif

#if (APP_IWDG_RELOAD_VALUE > 0x0FFFU)
#error "APP_IWDG_RELOAD_VALUE must fit the 12-bit IWDG reload register"
#endif

void TreatmentPulse_SetMode(uint8_t mode);
void TreatmentPulse_PrepareChannel(uint8_t channel, uint8_t mode);
void TreatmentPulse_SetChannelEnabled(uint8_t channel, uint8_t enabled);
void App_FaultSafeShutdownISR(void);
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
