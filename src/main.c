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

#define DBG_TAG "SYS"
#define DBG_LVL DBG_INFO
#include "log_printf.h"
#include "SEGGER_RTT.h"

// LCD数字数组
const uint8_t NUM1[] = {0xaf, 0x06, 0x6d, 0x4f, 0xc6, 0xcb, 0xeb, 0x0e, 0xef, 0xcf, 0x00};
const uint8_t NUM2[] = {0x5f, 0x06, 0x3d, 0x2f, 0x66, 0x6b, 0x7b, 0x0e, 0x7f, 0x6f, 0x00};

uint8_t AIRIN_Logo = 0x10; /// 充气图标
uint8_t CH1_Logo = 0x10;   ////1通道图标
uint8_t FORM_Logo = 0x10;  ////模式图标
uint8_t CH2_Logo = 0x10;   ////2通道图标

uint8_t PRESS_Logo = 0x80;	/// 压力值图�?
uint8_t AIROUT_Logo = 0x80; /// 放气图标
uint8_t MMHG_Logo = 0x80;	/// mmHg图标
uint8_t TIM_Logo = 0x80;	/// 时间图标

uint8_t Bat_Value0 = 0x10; ////电池电量外框图标
uint8_t Bat_Value1 = 0x80; ////电池电量一格图�?
uint8_t Bat_Value2 = 0x40; ////电池电量二格图标
uint8_t Bat_Value3 = 0x20; ////电池电量三格图标




uint16_t TIM2_PrescalerValue = 0;


uint8_t Set_Minute = 30; ////开机默�?30分钟
uint8_t Minute = 30;	 ////开机默�?30分钟
uint8_t Second = 0;
uint8_t Ch_Flag = 0;  ////通道标志�?0:A通道�?1：B通道
uint8_t WorkType = 0; /////0：治疗模式，1：检测模�?
uint8_t Formula = 0;  ////处方0,1,2�?
uint8_t Ico_Formula = 0;
uint8_t Pwr1 = 0; /////0-60档强�?
uint8_t Pwr2 = 0;
volatile uint8_t TreatmentPwmEnableCh1 = 0U;
volatile uint8_t TreatmentPwmEnableCh2 = 0U;
uint16_t Press_Value = 0; /// 压力�?
uint16_t Buzz_cnt = 0;	  /// 蜂鸣器时�?
uint8_t Flash_Flag = 0;	  ////闪烁标志�?
uint8_t Charg_Flag = 0;	  ////充电中标志位
uint8_t Chargok_Flag = 1; ////充满电标志位


uint8_t Mode_Change = 0;

uint16_t Bl_Cnt = 0; ////背光点亮计数

uint16_t Moto_Count = 0;
uint16_t MotoFirst_Flag = 1;
uint16_t MotoRun_Cnt = 0;
uint16_t MotoStop_Cnt = 0;
uint8_t Moto_StateFlag = 0;

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

/*============================================================================
 * 新主程序框架
 *
 * 设计原则�?
 * 1. 中断只提供系统节拍或完成高实时性的波形输出，不在中断里处理 UI 业务�?
 * 2. 按键和蓝牙命令统一转换为事件，再由状态机决定能否执行�?
 * 3. ADC、LCD、充电检测等任务按固定周期运行，禁止在主循环中无限阻塞�?
 * 4. 所有状态切换都通过 App_RequestState()，便于集中执行进�?/退出动作�?
 *============================================================================*/

typedef enum
{
	APP_STATE_POWER_OFF = 0, /* 关机等待：只保留按键和充电检�? */
	APP_STATE_BOOTING,       /* 开机初始化：按顺序启动工作外设 */
	APP_STATE_READY,         /* 已开机但尚未输出 */
	APP_STATE_THERAPY,       /* 治疗模式 */
	APP_STATE_PRESSURE,      /* 压力检测/充放气模式 */
	APP_STATE_CHARGING,      /* 关机充电动画; 可以一边充电一边工作 */
	APP_STATE_FAULT          /* 故障模式：立即关闭所有危险输出 */
} AppState_t;

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
#define POWER_ON_HOLD_MS           2000U
#define POWER_OFF_HOLD_MS          2000U
#define START_LONG_HOLD_MS         2000U
#define PRESSURE_START_LONG_HOLD_MS 3000U
#define UI_BLINK_PERIOD_MS         500U
#define UI_MAX_POWER               60U
#define UI_BEEP_ON_MS              60U
#define UI_BEEP_GAP_MS             80U
#define PRESSURE_INFLATE_TIMEOUT_MS 3000U
#define PRESSURE_DEFLATE_TIME_MS    3000U

/* 电池检测参数：PA3 �? 1/2 电池分压，PA6 为外�? 2.5 V 参考�? */
#define BATTERY_ADC_SAMPLE_COUNT          8U
#define BATTERY_ADC_USED_SAMPLE_COUNT     6U
#define BATTERY_REFERENCE_MV              2500U
#define BATTERY_DIVIDER_GAIN              2U
#define BATTERY_SAMPLE_IDLE_TICKS         8U
#define BATTERY_PRESSURE_IDLE_TICKS       48U
#define BATTERY_ADC_RETRY_TICKS           10U
#define BATTERY_VALID_MIN_MV              2500U
#define BATTERY_VALID_MAX_MV              5000U
#define BATTERY_FULL_MV                   4200U
#define BATTERY_LEVEL_3_MV                4100U
#define BATTERY_LEVEL_2_MV                3900U
#define BATTERY_LEVEL_1_MV                3700U
#define BATTERY_LOW_ENTER_MV              3500U
#define BATTERY_LOW_EXIT_MV               3600U
#define BATTERY_LOW_CONFIRM_COUNT         3U

/* Pressure sensor: PA2 / ADC2 channel 11. Temporary nominal calibration. */
#define PRESSURE_ADC_SAMPLE_PERIOD_MS      200U
#define PRESSURE_ADC_AVERAGE_PERIOD_MS     5000U
#define PRESSURE_ADC_REFERENCE_MV          3300U
#define PRESSURE_SENSOR_FULL_SCALE_KPA     40U
#define PRESSURE_SENSOR_FULL_SCALE_UV      75000U
#define PRESSURE_AMPLIFIER_GAIN            47U
#define PRESSURE_MMHG_PER_KPA_X1000        7501U
#define PRESSURE_ADC_ZERO_CODE             347U
#define PRESSURE_ADC_ZERO_DEADBAND         12U
#define PRESSURE_ADC_BATTERY_PAUSE_TICKS   2U

/* 联调压力安全阈值可调整，但不得超过固定的 150 mmHg 硬上限。 */
#define PRESSURE_SAFE_LIMIT_MMHG            100U
#define PRESSURE_HARD_LIMIT_MMHG            150U

#if (PRESSURE_SAFE_LIMIT_MMHG == 0U) || \
    (PRESSURE_SAFE_LIMIT_MMHG > PRESSURE_HARD_LIMIT_MMHG)
#error "PRESSURE_SAFE_LIMIT_MMHG must be within 1..150 mmHg"
#endif

typedef struct
{
	AppState_t state;        /* 当前系统状�? */
	AppState_t next_state;   /* 请求切换到的状�? */
	uint8_t ui_dirty;        /* �? 1 时才重新刷新 LCD */
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
	uint8_t selected_channel;       /* 0：通道 1�?1：通道 2 */
	uint8_t formula;                /* 处方编号�?0�?2，LCD 显示 1�?3 */
	uint8_t power_ch1;
	uint8_t power_ch2;
	uint8_t set_minutes;
	uint8_t remaining_minutes;
	uint8_t remaining_seconds;
	uint8_t blink_on;
	uint16_t blink_elapsed_ms;
	uint8_t battery_level;          /* 0�?3 �? */
	uint8_t battery_low;            /* 低电量时整个电池图标闪烁 */
	uint8_t charger_connected;
	uint8_t charger_full;
	uint8_t charge_frame;
	uint8_t ble_connected;          /* 未连接常亮，连接后闪�? */
	uint16_t pressure_value;        /* 已换算的显示值，硬限制为 0～150 mmHg */
	uint8_t pressure_result_blink;  /* 测量结果持续闪烁，直到再次按�? */
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

typedef struct
{
	uint8_t session_active;
	uint8_t adc_initialized;
	uint8_t measurement_pending;
	uint8_t next_sample_ticks;
	uint8_t retry_ticks;
	uint8_t valid;
	uint8_t level;
	uint8_t percent;
	uint8_t low_battery;
	uint8_t low_confirm_count;
	uint8_t recover_confirm_count;
	uint8_t error_reported;
	uint16_t voltage_mv;
} BatteryContext_t;

/* SysTick 中断�? 1 ms 增加一次。中断和主循环共享，因此必须使用 volatile�? */
static volatile uint32_t s_system_tick_ms = 0U;
static AppContext_t s_app;
static AppScheduler_t s_scheduler;
static KeyFilter_t s_keys[KEY_ID_COUNT];
static AppEventQueue_t s_event_queue;
static UiModel_t s_ui;
static BatteryContext_t s_battery;
static uint8_t s_pressure_adc_initialized;
static uint32_t s_pressure_adc_last_sample_ms;
static uint32_t s_pressure_adc_window_start_ms;
static uint32_t s_pressure_adc_sample_sum;
static uint16_t s_pressure_adc_sample_count;
static uint8_t s_pressure_adc_sampling_active;
static uint8_t s_pressure_adc_pause_ticks;
static PressureAction_t s_pressure_output_action;

static uint8_t s_charger_raw;
static uint8_t s_charger_stable;
static uint8_t s_charger_debounce_count;

static void Board_Init(void);
static void Board_EnterSafeState(void);
static void Treatment_StopOutputs(void);
static void Pressure_StopOutputs(void);
static void Pressure_ApplyOutputs(void);
static void App_Init(void);
static void App_RunOnce(void);
static void App_RequestState(AppState_t next_state);
static void App_ApplyStateTransition(void);
static void App_StateEnter(AppState_t state);
static void App_StateExit(AppState_t state);
static const char *App_StateName(AppState_t state);
static const char *App_EventName(AppEvent_t event);
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
static uint8_t Ui_GetBatteryDisplayLevel(void);
static uint8_t Ui_GetDisplayMinutes(void);
static void Ui_CycleTreatmentTime(void);
static void Ui_Countdown1s(void);
static void Ui_Beep(uint8_t count);
static void Ui_BuzzerTask10ms(void);

static void Battery_InitModel(void);
static void Battery_StartSession(void);
static void Battery_Stop(void);
static uint8_t Battery_ReadAveragedAdc(uint16_t *battery_adc,
                                      uint16_t *reference_adc);
static uint16_t Battery_CalculateVoltageMv(uint16_t battery_adc,
                                          uint16_t reference_adc);
static uint8_t Battery_CalculateLevel(uint16_t voltage_mv);
static uint8_t Battery_CalculatePercent(uint16_t voltage_mv);
static void Battery_UpdateLowState(uint16_t voltage_mv);
static void Battery_ProcessMeasurement(uint16_t battery_adc,
                                       uint16_t reference_adc);
static uint8_t Battery_Task100ms(void);
static uint16_t Pressure_CalculateMmHg(uint16_t adc_value,
                                       uint16_t *adc_input_mv,
                                       uint16_t *sensor_output_uv);
static uint8_t Pressure_ReadFilteredAdc(uint16_t *adc_value);
static void Pressure_UpdateLiveValue(uint16_t adc_value);
static void PressureAdc_Task100ms(void);

/* 后续蓝牙、ADC 和压力算法通过这些接口更新 UI，不直接操作段码�? */
void AppUi_SetBleConnected(uint8_t connected);
void AppUi_SetBattery(uint8_t level, uint8_t low_battery);
void AppUi_SetPressureResult(uint16_t value);
void AppUi_InflationCompleted(void);
uint8_t AppBattery_IsValid(void);
uint16_t AppBattery_GetVoltageMv(void);
uint8_t AppBattery_GetPercent(void);

static void Input_Task10ms(void);
static void Communication_Task10ms(void);
static void AppEvent_Task10ms(void);
static void Control_Task10ms(void);
static void Ui_Task50ms(void);
static void Sensor_Task100ms(void);
static void Power_Task1000ms(void);

/*
 * 1 ms 系统节拍入口�?
 * SysTick_Handler() 只调用此函数，不要在中断里增加业务处理�?
 */
void App_Tick1msISR(void)
{
	s_system_tick_ms++;
}

/* 板级基础初始化：这里只初始化所有状态都会使用的资源�? */
static void Board_Init(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	SystemCoreClockUpdate();
	DAC_ChannelConfig();
	DAC_SetCh1Data(DAC_ALIGN_R_12BIT, 0U);
	DAC_SetCh2Data(DAC_ALIGN_R_12BIT, 0U);
	TIM6_Configuration();
	TIM1_Configuration();
	TIM8_Configuration();
	TIM4_Configuration();
	NVIC_Configuration();
	SEGGER_RTT_Init();
	LOG_I("t=%u system init, core=%u Hz", s_system_tick_ms, SystemCoreClock);

	/* GPIO 初始化完成后，第一时间把执行器置于安全状态�? */
	Board_EnterSafeState();

	/* 使用独立�? SysTick 作为应用�? 1 ms 节拍�? */
	if (SysTick_Config(SystemCoreClock / 1000U) != 0U)
	{
		/* 节拍初始化失败时保持安全状态，不继续启动业务�? */
		LOG_E("t=%u SysTick init failed", s_system_tick_ms);
		while (1)
		{
		}
	}
}

/* 将当前能够直接控制的输出置于安全电平�? */
static void Board_EnterSafeState(void)
{
	Treatment_StopOutputs();
	Pressure_StopOutputs();
	Battery_Stop();

	BLEN_OFF;
	VEN_OFF;
	BATEN_OFF;
	BLEEN_OFF;
	SWEN_OFF;

	/* 蜂鸣器属于交互提示，由蜂鸣任务负责停止，不与危险输出混在一起�? */
}

static void Treatment_StopOutputs(void)
{
	/* UI 目标值和遗留波形中断读取的档位同时归零�? */
	s_ui.power_ch1 = 0U;
	s_ui.power_ch2 = 0U;
	Pwr1 = 0U;
	Pwr2 = 0U;
	TreatmentPwmEnableCh1 = 0U;
	TreatmentPwmEnableCh2 = 0U;

	/* Keep the hardware 300 us compare value ready for the next treatment. */
	TIM_SetCmp1(TIM1, 4800U);
	TIM_SetCmp2(TIM1, 4800U);
	TIM_SetCmp1(TIM8, 4800U);
	TIM_SetCmp2(TIM8, 4800U);
	TIM_EnableCapCmpCh(TIM1, TIM_CH_1, TIM_CAP_CMP_DISABLE);
	TIM_EnableCapCmpCh(TIM1, TIM_CH_2, TIM_CAP_CMP_DISABLE);
	TIM_EnableCapCmpChN(TIM1, TIM_CH_1, TIM_CAP_CMP_N_DISABLE);
	TIM_EnableCapCmpChN(TIM1, TIM_CH_2, TIM_CAP_CMP_N_DISABLE);
	TIM_EnableCapCmpCh(TIM8, TIM_CH_1, TIM_CAP_CMP_DISABLE);
	TIM_EnableCapCmpCh(TIM8, TIM_CH_2, TIM_CAP_CMP_DISABLE);
	TIM_EnableCapCmpChN(TIM8, TIM_CH_1, TIM_CAP_CMP_N_DISABLE);
	TIM_EnableCapCmpChN(TIM8, TIM_CH_2, TIM_CAP_CMP_N_DISABLE);
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
	s_pressure_output_action = PRESSURE_ACTION_IDLE;
	BATEN_OFF;
	s_pressure_adc_sampling_active = 0U;
	s_pressure_adc_pause_ticks = 0U;
	if (s_pressure_adc_initialized != 0U)
	{
		if (ADC_DisableSafe(ADC2) == 0U)
		{
			ADC_DeInit(ADC2);
		}
		s_pressure_adc_initialized = 0U;
	}
}

static void Pressure_ApplyOutputs(void)
{
	PressureAction_t requested_action = PRESSURE_ACTION_IDLE;

	if ((s_app.state == APP_STATE_PRESSURE) &&
	    (s_ui.charger_connected == 0U))
	{
		requested_action = s_ui.pressure_action;
	}

	if (requested_action == s_pressure_output_action)
	{
		return;
	}

	/* Any action transition first returns both actuators to the inactive state. */
	TIM_SetCmp4(TIM4, 0U);
	TIM_EnableCapCmpCh(TIM4, TIM_CH_4, TIM_CAP_CMP_DISABLE);
	SWEN_OFF;

	if (requested_action == PRESSURE_ACTION_INFLATING)
	{
		TIM_SetCmp4(TIM4, MOTOR_PWM_COMPARE_COUNTS);
		TIM_GenerateEvent(TIM4, TIM_EVT_SRC_UPDATE);
		TIM_SetCnt(TIM4, 0U);
		TIM_EnableCapCmpCh(TIM4, TIM_CH_4, TIM_CAP_CMP_ENABLE);
	}
	else if (requested_action == PRESSURE_ACTION_DEFLATING)
	{
		SWEN_ON;
	}

	s_pressure_output_action = requested_action;
}

/* 初始化应用状态，不在这里执行耗时或阻塞操作 */
static void App_Init(void)
{
	s_app.state = APP_STATE_POWER_OFF;
	s_app.next_state = APP_STATE_POWER_OFF;
	s_app.ui_dirty = 1U;

	s_scheduler.last_10ms = s_system_tick_ms;
	s_scheduler.last_50ms = s_system_tick_ms;
	s_scheduler.last_100ms = s_system_tick_ms;
	s_scheduler.last_1000ms = s_system_tick_ms;

	Ui_InitModel();
	Battery_InitModel();
	Key_Init();

	s_event_queue.read_index = 0U;
	s_event_queue.write_index = 0U;

	s_charger_stable = ((READ_STDBY == Bit_RESET) ||
	                    (READ_CHARG == Bit_RESET)) ? 1U : 0U;
	s_charger_raw = s_charger_stable;
	s_charger_debounce_count = 0U;
	s_ui.charger_connected = s_charger_stable;
	s_ui.charger_full = (READ_STDBY == Bit_RESET) ? 1U : 0U;
	LOG_I("t=%u app init, charger=%s charg_n=%u standby_n=%u full=%u",
	      s_system_tick_ms,
	      (s_ui.charger_connected != 0U) ?
	      ((s_ui.charger_full != 0U) ? "FULL" : "CHARGING") :
	      "DISCONNECTED",
	      (uint8_t)READ_CHARG, (uint8_t)READ_STDBY, s_ui.charger_full);

	/* 上电时优先识别充电状态 */
	if (s_ui.charger_connected != 0U)
	{
		App_RequestState(APP_STATE_CHARGING);
		App_ApplyStateTransition();
	}
}

/* 主循环的一次调度：每个任务必须快速返回，禁止在任务内部长时间 Delay�? */
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

	/* 处理周期任务产生的状态切换请求�? */
	App_ApplyStateTransition();
}

static const char *App_StateName(AppState_t state)
{
	switch (state)
	{
		case APP_STATE_POWER_OFF: return "POWER_OFF";
		case APP_STATE_BOOTING:   return "BOOTING";
		case APP_STATE_READY:     return "READY";
		case APP_STATE_THERAPY:   return "THERAPY";
		case APP_STATE_PRESSURE:  return "PRESSURE";
		case APP_STATE_CHARGING:  return "CHARGING";
		case APP_STATE_FAULT:     return "FAULT";
		default:                  return "UNKNOWN";
	}
}

static const char *App_EventName(AppEvent_t event)
{
	switch (event)
	{
		case APP_EVENT_POWER_SHORT:          return "POWER_SHORT";
		case APP_EVENT_POWER_LONG:           return "POWER_LONG";
		case APP_EVENT_FUNCTION_SHORT:       return "FUNCTION_SHORT";
		case APP_EVENT_START_SHORT:          return "START_SHORT";
		case APP_EVENT_START_LONG:           return "START_LONG";
		case APP_EVENT_PLUS_SHORT:           return "PLUS_SHORT";
		case APP_EVENT_MINUS_SHORT:          return "MINUS_SHORT";
		case APP_EVENT_CHARGER_CONNECTED:    return "CHARGER_CONNECTED";
		case APP_EVENT_CHARGER_DISCONNECTED: return "CHARGER_DISCONNECTED";
		default:                             return "NONE";
	}
}

/* 请求状态切换，不允许业务代码直接修改当前状态�? */
static void App_RequestState(AppState_t next_state)
{
	s_app.next_state = next_state;
}

/* 集中执行状态退出和进入动作，防止外设只开不关�? */
static void App_ApplyStateTransition(void)
{
	if (s_app.state == s_app.next_state)
	{
		return;
	}

	LOG_I("t=%u state %s -> %s", s_system_tick_ms,
	      App_StateName(s_app.state), App_StateName(s_app.next_state));
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
			Battery_StartSession();
			Ui_InitHardware();
			s_ui.beep_remaining = 0U;
			s_ui.beep_on_ms = 0U;
			s_ui.beep_gap_ms = 0U;
			TIM_EnableCapCmpCh(TIM3, TIM_CH_4, TIM_CAP_CMP_DISABLE);
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
			 * 当前阶段只启动UI。ADC、治疗和气泵外设继续保持关闭
			 * 等相应模块完成后再逐项加入这里
			 */
			/* POWER_OFF and CHARGING are already safe source states. Keep BLEN
			 * stable here so charge-to-work startup does not pulse the backlight off. */
			Ui_InitModel();
			if (s_battery.session_active == 0U)
			{
				Battery_StartSession();
			}
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
			Ui_InitHardware();
			s_ui.pressure_action = PRESSURE_ACTION_IDLE;
			s_app.ui_dirty = 1U;
			break;

		case APP_STATE_PRESSURE:
			Ui_InitHardware();
			s_ui.power_ch1 = 0U;
			s_ui.power_ch2 = 0U;
			s_app.ui_dirty = 1U;
			break;

		default:
			App_RequestState(APP_STATE_FAULT);
			break;
	}
}

/* 使用无符号减法保证系统节拍溢出后仍能正确判断周期�? */
static uint8_t Scheduler_IsDue(uint32_t *last_tick, uint32_t period_ms)
{
	uint32_t now = s_system_tick_ms;

	if ((uint32_t)(now - *last_tick) < period_ms)
	{
		return 0U;
	}

	/* 正常情况下累加周期；阻塞过久时直接追到当前时间，避免连续补跑�? */
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
			/* 短按统一在稳定释放时产生，长按释放时不再补发短按�? */
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
		if ((s_app.state == APP_STATE_POWER_OFF) ||
		    (s_app.state == APP_STATE_CHARGING))
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
		long_threshold_ms = (s_app.state == APP_STATE_PRESSURE) ?
		                    PRESSURE_START_LONG_HOLD_MS :
		                    START_LONG_HOLD_MS;
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
	uint8_t standby_n = (uint8_t)READ_STDBY;
	uint8_t charg_n = (uint8_t)READ_CHARG;
	uint8_t connected = ((standby_n == (uint8_t)Bit_RESET) ||
	                     (charg_n == (uint8_t)Bit_RESET)) ? 1U : 0U;
	uint8_t full = (standby_n == (uint8_t)Bit_RESET) ? 1U : 0U;

	/* Keep a low-rate raw-pin trace so Type-C/charger hardware can be diagnosed. */
	// if ((s_system_tick_ms % 5000U) < KEY_SCAN_PERIOD_MS)
	// {
	// 	LOG_I("t=%u charger pins charg_n=%u standby_n=%u connected=%u full=%u",
	// 	      s_system_tick_ms, charg_n, standby_n, connected, full);
	// }

	if (full != s_ui.charger_full)
	{
		s_ui.charger_full = full;
		s_app.ui_dirty = 1U;
		/* 拔出时两个低有效信号会同时释放，交给插拔去抖日志报告�? */
		if ((connected != 0U) && (s_charger_stable != 0U))
		{
			LOG_I("t=%u charger status=%s charg_n=%u standby_n=%u",
			      s_system_tick_ms, (full != 0U) ? "FULL" : "CHARGING",
			      charg_n, standby_n);
		}
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
			LOG_I("t=%u charger status=%s charg_n=%u standby_n=%u full=%u",
			      s_system_tick_ms,
			      (connected != 0U) ?
			      ((full != 0U) ? "FULL" : "CHARGING") :
			      "DISCONNECTED",
			      charg_n, standby_n, full);
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
		/* 队列满时保留已有事件，避免覆盖尚未处理的关机事件�? */
		LOG_W("t=%u event queue full, drop=%s", s_system_tick_ms,
		      App_EventName(event));
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
	LOG_I("t=%u event=%s state=%s", s_system_tick_ms,
	      App_EventName(event), App_StateName(s_app.state));

	/* Charger events update power presence without interrupting active work. */
	if (event == APP_EVENT_CHARGER_CONNECTED)
	{
		if (s_app.state == APP_STATE_PRESSURE)
		{
			Pressure_StopOutputs();
		}
		/* Charger presence is parallel to normal operation. Only enter the
		 * charging-only display when the product is currently powered off. */
		if (s_app.state == APP_STATE_POWER_OFF)
		{
			App_RequestState(APP_STATE_CHARGING);
		}
		s_app.ui_dirty = 1U;
		return;
	}

	if (event == APP_EVENT_CHARGER_DISCONNECTED)
	{
		if (s_app.state == APP_STATE_CHARGING)
		{
			App_RequestState(APP_STATE_POWER_OFF);
		}
		s_app.ui_dirty = 1U;
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

	if (s_app.state == APP_STATE_CHARGING)
	{
		if (event == APP_EVENT_POWER_LONG)
		{
			App_RequestState(APP_STATE_BOOTING);
		}
		return;
	}

	if ((s_app.state == APP_STATE_BOOTING) ||
	    (s_app.state == APP_STATE_FAULT))
	{
		return;
	}

	/* 工作状态下记录有效操作；背光本身跟�? UI 生命周期�? */
	Ui_RecordActivity();
	if (s_ui.pressure_result_blink != 0U)
	{
		/* 需求要求压力结果持续闪烁，直到用户再次按下任意按键�? */
		s_ui.pressure_result_blink = 0U;
		s_app.ui_dirty = 1U;
	}

	switch (event)
	{
		case APP_EVENT_POWER_LONG:
			Ui_Beep(3U);
			App_RequestState((s_ui.charger_connected != 0U) ?
			                 APP_STATE_CHARGING : APP_STATE_POWER_OFF);
			break;

		case APP_EVENT_POWER_SHORT:
			Ui_Beep(1U);
			Ui_CycleTreatmentTime();
			LOG_I("t=%u treatment time=%u min", s_system_tick_ms,
			      s_ui.set_minutes);
			s_app.ui_dirty = 1U;
			break;

		case APP_EVENT_FUNCTION_SHORT:
			Ui_Beep(1U);
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
			if (s_app.state == APP_STATE_THERAPY)
			{
				s_ui.formula = (uint8_t)((s_ui.formula + 1U) % 3U);
				s_ui.power_ch1 = 0U;
				s_ui.power_ch2 = 0U;
				LOG_I("t=%u formula=P%u, power reset", s_system_tick_ms,
				      (uint8_t)(s_ui.formula + 1U));
			}
			else if ((s_app.state == APP_STATE_PRESSURE) &&
			         (s_ui.charger_connected == 0U))
			{
				/* 短按启动/停止充气，联调阶段由固定超时兜底。 */
				if (s_ui.pressure_value >= PRESSURE_SAFE_LIMIT_MMHG)
				{
					LOG_W("t=%u inflation blocked, pressure=%u mmHg limit=%u mmHg",
					      s_system_tick_ms, s_ui.pressure_value,
					      PRESSURE_SAFE_LIMIT_MMHG);
				}
				else if (s_ui.pressure_action == PRESSURE_ACTION_INFLATING)
				{
					s_ui.pressure_action = PRESSURE_ACTION_IDLE;
					s_ui.pressure_action_ms = 0U;
				}
				else
				{
					s_ui.pressure_action = PRESSURE_ACTION_INFLATING;
					s_ui.pressure_action_ms = PRESSURE_INFLATE_TIMEOUT_MS;
				}
				if (s_ui.pressure_value < PRESSURE_SAFE_LIMIT_MMHG)
				{
					LOG_I("t=%u pressure action=%s", s_system_tick_ms,
					      (s_ui.pressure_action == PRESSURE_ACTION_INFLATING) ?
					      "INFLATING" : "IDLE");
				}
			}
			s_app.ui_dirty = 1U;
			break;

		case APP_EVENT_START_LONG:
			Ui_Beep(1U);
			if (s_app.state == APP_STATE_THERAPY)
			{
				s_ui.selected_channel = (uint8_t)!s_ui.selected_channel;
				LOG_I("t=%u selected channel=%u", s_system_tick_ms,
				      (uint8_t)(s_ui.selected_channel + 1U));
			}
			else if ((s_app.state == APP_STATE_PRESSURE) &&
			         (s_ui.charger_connected == 0U))
			{
				s_ui.pressure_action = PRESSURE_ACTION_DEFLATING;
				s_ui.pressure_action_ms = PRESSURE_DEFLATE_TIME_MS;
				LOG_I("t=%u pressure action=DEFLATING", s_system_tick_ms);
			}
			s_app.ui_dirty = 1U;
			break;

		case APP_EVENT_PLUS_SHORT:
			if (s_app.state == APP_STATE_THERAPY)
			{
				Ui_Beep(1U);
				/* 疗程结束后保持显�? 0，直到用户再次增加强度开始新疗程�? */
				if ((s_ui.power_ch1 == 0U) &&
				    (s_ui.power_ch2 == 0U) &&
				    (s_ui.remaining_minutes == 0U) &&
				    (s_ui.remaining_seconds == 0U))
				{
					s_ui.remaining_minutes = s_ui.set_minutes;
				}
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
				LOG_I("t=%u power ch1=%u ch2=%u", s_system_tick_ms,
				      s_ui.power_ch1, s_ui.power_ch2);
				s_app.ui_dirty = 1U;
			}
			break;

		case APP_EVENT_MINUS_SHORT:
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
				LOG_I("t=%u power ch1=%u ch2=%u", s_system_tick_ms,
				      s_ui.power_ch1, s_ui.power_ch2);
				s_app.ui_dirty = 1U;
			}
			break;

		default:
			break;
	}
}

static void Ui_InitModel(void)
{
	/* LCD/蜂鸣器初始化标志保留，其他字段恢复默认交互状态�? */
	s_ui.selected_channel = 0U;
	s_ui.formula = 0U;
	s_ui.power_ch1 = 0U;
	s_ui.power_ch2 = 0U;
	s_ui.set_minutes = 30U;
	s_ui.remaining_minutes = 30U;
	s_ui.remaining_seconds = 0U;
	s_ui.blink_on = 1U;
	s_ui.blink_elapsed_ms = 0U;
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

	/* UI 开启期间背光常亮，不再使用无操作倒计时单独关闭背光�? */
	BLEN_ON;
	LOG_I("t=%u ui hardware lcd=%u blen_out=%u blen_pin=%u",
	      s_system_tick_ms, s_ui.lcd_initialized,
	      GPIO_ReadOutputDataBit(BLEN_PORT, BLEN_PIN),
	      GPIO_ReadInputDataBit(BLEN_PORT, BLEN_PIN));
}

static void Ui_Shutdown(void)
{
	BLEN_OFF;

	if (s_ui.lcd_initialized != 0U)
	{
		Alloff_LCD();
	}
}

static void Ui_RecordActivity(void)
{
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

static uint8_t Ui_GetBatteryDisplayLevel(void)
{
	if (s_ui.charger_connected != 0U)
	{
		return (s_ui.charger_full != 0U) ? 3U : s_ui.charge_frame;
	}

	return s_ui.battery_level;
}

/*
 * LCD 只显示整数分钟，因此对剩余时间向上取整：
 * 9:59-9:01 显示 10，只有到 9:00 才显示 9:00
 */
static uint8_t Ui_GetDisplayMinutes(void)
{
	uint8_t display_minutes = s_ui.remaining_minutes;

	if ((s_ui.remaining_seconds != 0U) && (display_minutes < 99U))
	{
		display_minutes++;
	}

	return display_minutes;
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
	uint8_t display_minutes = Ui_GetDisplayMinutes();
	uint8_t channel1_icon = 0x10U;
	uint8_t channel2_icon = 0x10U;
	uint8_t formula_icon = 0x10U;
	uint8_t formula_value = (uint8_t)(s_ui.formula + 1U);
	uint8_t ble_icon = 0x80U;
	uint8_t battery_segments = Ui_GetBatterySegments(Ui_GetBatteryDisplayLevel());

	if (s_ui.blink_on == 0U)
	{
		/* P 图标与处方数字作为一个整体闪烁 */
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

	}

	/* 蓝牙未连接时常亮，连接成功后随统一闪烁节拍闪烁�? */
	if ((s_ui.ble_connected != 0U) && (s_ui.blink_on == 0U))
	{
		ble_icon = 0U;
	}

	if ((s_ui.charger_connected == 0U) &&
	    (s_ui.battery_low != 0U) && (s_ui.blink_on == 0U))
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
	write_LCD(1U, 16U, NUM2[display_minutes / 10U] | ble_icon);
	write_LCD(1U, 18U, NUM2[display_minutes % 10U] | 0x80U);
	write_LCD(1U, 20U, battery_segments);
}

static void Ui_RenderPressure(void)
{
	uint16_t pressure = s_ui.pressure_value;
	uint8_t display_minutes = Ui_GetDisplayMinutes();
	uint8_t air_in_icon = 0U;
	uint8_t air_out_icon = 0U;
	uint8_t pressure_icon = 0x80U;
	uint8_t unit_icon = 0x80U;
	uint8_t ble_icon = 0x80U;
	uint8_t battery_segments = Ui_GetBatterySegments(Ui_GetBatteryDisplayLevel());
	uint8_t pressure_hundreds;
	uint8_t pressure_tens;
	uint8_t pressure_ones;

	if (pressure > PRESSURE_HARD_LIMIT_MMHG)
	{
		pressure = PRESSURE_HARD_LIMIT_MMHG;
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

	if (((s_ui.pressure_result_blink != 0U) ||
	     (pressure >= PRESSURE_SAFE_LIMIT_MMHG)) &&
	    (s_ui.blink_on == 0U))
	{
		pressure_hundreds = 10U;
		pressure_tens = 10U;
		pressure_ones = 10U;
		pressure_icon = 0U;
		unit_icon = 0U;
	}

	if ((s_ui.ble_connected != 0U) && (s_ui.blink_on == 0U))
	{
		ble_icon = 0U;
	}

	if ((s_ui.charger_connected == 0U) &&
	    (s_ui.battery_low != 0U) && (s_ui.blink_on == 0U))
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
	write_LCD(1U, 16U, NUM2[display_minutes / 10U] | ble_icon);
	write_LCD(1U, 18U, NUM2[display_minutes % 10U] | 0x80U);
	write_LCD(1U, 20U, battery_segments);
}

static void Ui_RenderCharging(void)
{
	uint8_t level = Ui_GetBatteryDisplayLevel();

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
			/* 当前 LCD 无字母字模，故障界面暂时清屏并保留背光 */
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
	uint8_t new_state = (connected != 0U) ? 1U : 0U;

	if (new_state != s_ui.ble_connected)
	{
		LOG_I("t=%u BLE connected=%u", s_system_tick_ms, new_state);
	}
	s_ui.ble_connected = new_state;
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
	s_ui.pressure_value = (value > PRESSURE_HARD_LIMIT_MMHG) ?
	                      PRESSURE_HARD_LIMIT_MMHG : value;
	s_ui.pressure_action = PRESSURE_ACTION_IDLE;
	s_ui.pressure_action_ms = 0U;
	Pressure_ApplyOutputs();
	s_ui.pressure_result_blink = 1U;
	s_app.ui_dirty = 1U;
	LOG_I("t=%u pressure result=%u mmHg", s_system_tick_ms,
	      s_ui.pressure_value);
}

void AppUi_InflationCompleted(void)
{
	if (s_ui.pressure_action == PRESSURE_ACTION_INFLATING)
	{
		s_ui.pressure_action = PRESSURE_ACTION_IDLE;
		s_ui.pressure_action_ms = 0U;
		s_app.ui_dirty = 1U;
		LOG_I("t=%u inflation completed", s_system_tick_ms);
	}
}

static void Battery_InitModel(void)
{
	s_battery.session_active = 0U;
	s_battery.adc_initialized = 0U;
	s_battery.measurement_pending = 0U;
	s_battery.next_sample_ticks = 0U;
	s_battery.retry_ticks = 0U;
	s_battery.valid = 0U;
	s_battery.level = 0U;
	s_battery.percent = 0U;
	s_battery.low_battery = 0U;
	s_battery.low_confirm_count = 0U;
	s_battery.recover_confirm_count = 0U;
	s_battery.error_reported = 0U;
	s_battery.voltage_mv = 0U;
}

static void Battery_StartSession(void)
{
	/* 每次开机重新建立滤波初值，避免沿用上一次关机前的数据�? */
	Battery_Stop();
	Battery_InitModel();
	s_battery.session_active = 1U;
	AppUi_SetBattery(0U, 0U);
}

static void Battery_Stop(void)
{
	BATEN_OFF;
	s_battery.session_active = 0U;
	s_battery.measurement_pending = 0U;
	s_battery.next_sample_ticks = 0U;

	if (s_battery.adc_initialized != 0U)
	{
		if (ADC_DisableSafe(ADC1) == 0U)
		{
			ADC_DeInit(ADC1);
		}
		s_battery.adc_initialized = 0U;
	}
}

/*
 * 连续读取 8 组电�?/参考通道，分别去掉一个最大值和一个最小值，
 * 对剩�? 6 组求平均。这样既能抑制偶发尖峰，又不会让任务长期阻塞�?
 */
static uint8_t Battery_ReadAveragedAdc(uint16_t *battery_adc,
                                      uint16_t *reference_adc)
{
	uint8_t index;
	uint16_t battery_sample;
	uint16_t reference_sample;
	uint16_t battery_min = 0xFFFFU;
	uint16_t battery_max = 0U;
	uint16_t reference_min = 0xFFFFU;
	uint16_t reference_max = 0U;
	uint32_t battery_sum = 0U;
	uint32_t reference_sum = 0U;

	/*
	 * PA3 and PA6 have different source voltages/impedances. Do not alternate
	 * them for every retained sample: after selecting each channel, discard one
	 * conversion so the ADC sampling capacitor can settle to the new input.
	 */
	if (ADC_GetData(ADC1, ADC1_Channel_04_PA3, &battery_sample) == 0U)
	{
		return 0U;
	}

	for (index = 0U; index < BATTERY_ADC_SAMPLE_COUNT; index++)
	{
		if (ADC_GetData(ADC1, ADC1_Channel_04_PA3,
		                &battery_sample) == 0U)
		{
			return 0U;
		}

		battery_sum += battery_sample;

		if (battery_sample < battery_min)
		{
			battery_min = battery_sample;
		}
		if (battery_sample > battery_max)
		{
			battery_max = battery_sample;
		}
	}

	if (ADC_GetData(ADC1, ADC1_Channel_03_PA6, &reference_sample) == 0U)
	{
		return 0U;
	}

	for (index = 0U; index < BATTERY_ADC_SAMPLE_COUNT; index++)
	{
		if (ADC_GetData(ADC1, ADC1_Channel_03_PA6,
		                &reference_sample) == 0U)
		{
			return 0U;
		}

		reference_sum += reference_sample;

		if (reference_sample < reference_min)
		{
			reference_min = reference_sample;
		}
		if (reference_sample > reference_max)
		{
			reference_max = reference_sample;
		}
	}

	battery_sum -= (uint32_t)battery_min + battery_max;
	reference_sum -= (uint32_t)reference_min + reference_max;

	*battery_adc = (uint16_t)((battery_sum +
	                           (BATTERY_ADC_USED_SAMPLE_COUNT / 2U)) /
	                          BATTERY_ADC_USED_SAMPLE_COUNT);
	*reference_adc = (uint16_t)((reference_sum +
	                             (BATTERY_ADC_USED_SAMPLE_COUNT / 2U)) /
	                            BATTERY_ADC_USED_SAMPLE_COUNT);
	return 1U;
}

/*
 * PA6 �? 2.5 V 精密参考和 PA3 �? 1/2 电池分压使用同一�? ADC 参考电源：
 *
 *   ADC_BATT / ADC_REF = (Vbat / 2) / 2500 mV
 *   Vbat(mV) = ADC_BATT * 2500 * 2 / ADC_REF
 *
 * 比值法会约�? ADC 满量程和 VDDA，因�? VDDA 波动不会直接带入结果�?
 */
static uint16_t Battery_CalculateVoltageMv(uint16_t battery_adc,
                                          uint16_t reference_adc)
{
	uint32_t numerator;

	if (reference_adc == 0U)
	{
		return 0U;
	}

	numerator = (uint32_t)battery_adc * BATTERY_REFERENCE_MV *
	            BATTERY_DIVIDER_GAIN;
	return (uint16_t)((numerator + (reference_adc / 2U)) / reference_adc);
}

static uint8_t Battery_CalculateLevel(uint16_t voltage_mv)
{
	if (voltage_mv >= BATTERY_LEVEL_3_MV)
	{
		return 3U;
	}
	if (voltage_mv >= BATTERY_LEVEL_2_MV)
	{
		return 2U;
	}
	if (voltage_mv >= BATTERY_LEVEL_1_MV)
	{
		return 1U;
	}
	return 0U;
}

/* 沿用旧程序的 3.5�?4.2 V 线性百分比，仅供通信/调试使用�? */
static uint8_t Battery_CalculatePercent(uint16_t voltage_mv)
{
	uint32_t percent;

	if (voltage_mv <= BATTERY_LOW_ENTER_MV)
	{
		return 0U;
	}
	if (voltage_mv >= BATTERY_FULL_MV)
	{
		return 100U;
	}

	percent = ((uint32_t)(voltage_mv - BATTERY_LOW_ENTER_MV) * 100U + 350U) /
	          (BATTERY_FULL_MV - BATTERY_LOW_ENTER_MV);
	return (uint8_t)percent;
}

static void Battery_UpdateLowState(uint16_t voltage_mv)
{
	if (voltage_mv <= BATTERY_LOW_ENTER_MV)
	{
		s_battery.recover_confirm_count = 0U;
		if (s_battery.low_confirm_count < BATTERY_LOW_CONFIRM_COUNT)
		{
			s_battery.low_confirm_count++;
		}
		if (s_battery.low_confirm_count >= BATTERY_LOW_CONFIRM_COUNT)
		{
			s_battery.low_battery = 1U;
		}
	}
	else if (voltage_mv >= BATTERY_LOW_EXIT_MV)
	{
		s_battery.low_confirm_count = 0U;
		if (s_battery.recover_confirm_count < BATTERY_LOW_CONFIRM_COUNT)
		{
			s_battery.recover_confirm_count++;
		}
		if (s_battery.recover_confirm_count >= BATTERY_LOW_CONFIRM_COUNT)
		{
			s_battery.low_battery = 0U;
		}
	}
	else
	{
		/* 3.5�?3.6 V 为回差区，保持当前低电状态�? */
		s_battery.low_confirm_count = 0U;
		s_battery.recover_confirm_count = 0U;
	}
}

static void Battery_ProcessMeasurement(uint16_t battery_adc,
                                       uint16_t reference_adc)
{
	uint8_t previous_valid = s_battery.valid;
	uint8_t previous_level = s_battery.level;
	uint8_t previous_low = s_battery.low_battery;
	uint16_t measured_mv = Battery_CalculateVoltageMv(battery_adc,
	                                                   reference_adc);

	if ((measured_mv < BATTERY_VALID_MIN_MV) ||
	    (measured_mv > BATTERY_VALID_MAX_MV))
	{
		if (s_battery.error_reported == 0U)
		{
			LOG_W("t=%u battery value invalid, bat_adc=%u ref_adc=%u mv=%u",
			      s_system_tick_ms, battery_adc, reference_adc, measured_mv);
			s_battery.error_reported = 1U;
		}
		return;
	}

	if (s_battery.valid == 0U)
	{
		s_battery.voltage_mv = measured_mv;
		s_battery.valid = 1U;
	}
	else
	{
		/* 一阶低通：新值占 1/4，旧值占 3/4，降低负载脉冲造成的跳动�? */
		s_battery.voltage_mv = (uint16_t)(((uint32_t)s_battery.voltage_mv * 3U +
		                                        measured_mv + 2U) / 4U);
	}

	s_battery.level = Battery_CalculateLevel(s_battery.voltage_mv);
	s_battery.percent = Battery_CalculatePercent(s_battery.voltage_mv);
	Battery_UpdateLowState(s_battery.voltage_mv);
	AppUi_SetBattery(s_battery.level, s_battery.low_battery);

	if (s_battery.error_reported != 0U)
	{
		LOG_I("t=%u battery sampling recovered", s_system_tick_ms);
		s_battery.error_reported = 0U;
	}
	if (previous_valid == 0U)
	{
		LOG_I("t=%u battery ready, bat_adc=%u ref_adc=%u mv=%u percent=%u level=%u",
		      s_system_tick_ms, battery_adc, reference_adc,
		      s_battery.voltage_mv, s_battery.percent, s_battery.level);
	}
	else if (previous_level != s_battery.level)
	{
		LOG_I("t=%u battery level=%u mv=%u percent=%u", s_system_tick_ms,
		      s_battery.level, s_battery.voltage_mv, s_battery.percent);
	}
	if (previous_low != s_battery.low_battery)
	{
		if (s_battery.low_battery != 0U)
		{
			LOG_W("t=%u low battery, mv=%u", s_system_tick_ms,
			      s_battery.voltage_mv);
		}
		else
		{
			LOG_I("t=%u low battery cleared, mv=%u", s_system_tick_ms,
			      s_battery.voltage_mv);
		}
	}
#if 0 /* 周期性电池日志：联调其他功能时暂时屏蔽。 */
	if (s_app.state == APP_STATE_PRESSURE)
	{
		LOG_I("t=%u battery pressure sample, bat_adc=%u ref_adc=%u mv=%u percent=%u",
		      s_system_tick_ms, battery_adc, reference_adc,
		      s_battery.voltage_mv, s_battery.percent);
	}
	else
	{
		LOG_D("t=%u battery sample, bat_adc=%u ref_adc=%u mv=%u percent=%u",
		      s_system_tick_ms, battery_adc, reference_adc,
		      s_battery.voltage_mv, s_battery.percent);
	}
#endif
}

/* Returns 1 only when ADC1 performed a battery/reference conversion. */
static uint8_t Battery_Task100ms(void)
{
	uint16_t battery_adc;
	uint16_t reference_adc;
	uint8_t pressure_mode = (s_app.state == APP_STATE_PRESSURE) ? 1U : 0U;
	uint8_t working_state = ((s_app.state == APP_STATE_CHARGING) ||
	                         (s_app.state == APP_STATE_READY) ||
	                         (s_app.state == APP_STATE_THERAPY) ||
	                         (s_app.state == APP_STATE_PRESSURE)) ? 1U : 0U;

	if (working_state == 0U)
	{
		if (s_battery.measurement_pending != 0U)
		{
			BATEN_OFF;
			s_battery.measurement_pending = 0U;
		}
		return 0U;
	}

	if (s_battery.retry_ticks != 0U)
	{
		s_battery.retry_ticks--;
		return 0U;
	}

	if (s_battery.adc_initialized == 0U)
	{
		if (ADC1_Initial() == 0U)
		{
			if (s_battery.error_reported == 0U)
			{
				LOG_E("t=%u battery ADC init failed", s_system_tick_ms);
				s_battery.error_reported = 1U;
			}
			s_battery.retry_ticks = BATTERY_ADC_RETRY_TICKS;
			return 0U;
		}
		s_battery.adc_initialized = 1U;
		LOG_I("t=%u battery ADC initialized", s_system_tick_ms);
	}

	if (s_battery.measurement_pending != 0U)
	{
		/* BATEN 已保持一�? 100 ms 任务周期，分压节点已经稳定�? */
		if (Battery_ReadAveragedAdc(&battery_adc, &reference_adc) != 0U)
		{
			Battery_ProcessMeasurement(battery_adc, reference_adc);
		}
		else
		{
			/* 转换异常时重新初始化 ADC，但保留上一笔有效显示�? */
			if (s_battery.error_reported == 0U)
			{
				LOG_E("t=%u battery ADC conversion failed", s_system_tick_ms);
				s_battery.error_reported = 1U;
			}
			if (ADC_DisableSafe(ADC1) == 0U)
			{
				LOG_E("t=%u battery ADC power-down timeout", s_system_tick_ms);
			}
			/* A conversion timeout can leave ADC state stale; reset before retry. */
			ADC_DeInit(ADC1);
			s_battery.adc_initialized = 0U;
			s_battery.retry_ticks = BATTERY_ADC_RETRY_TICKS;
		}

		if (pressure_mode == 0U)
		{
			BATEN_OFF;
		}
		else
		{
			BATEN_ON;
		}
		s_battery.measurement_pending = 0U;
		s_battery.next_sample_ticks = (pressure_mode != 0U) ?
		                              BATTERY_PRESSURE_IDLE_TICKS :
		                              BATTERY_SAMPLE_IDLE_TICKS;
		return 1U;
	}

	if (s_battery.next_sample_ticks != 0U)
	{
		s_battery.next_sample_ticks--;
		return 0U;
	}

	/* 只在采样前打开分压检测电路，下一�? 100 ms 任务再读取�? */
	BATEN_ON;
	s_battery.measurement_pending = 1U;
	return 0U;
}

/*
 * Temporary pressure conversion for the 40 kPa sensor:
 * 75 mV full-scale sensor output, 47x amplifier gain and 3.3 V ADC reference.
 * The isolated PA2 samples currently indicate a zero code near 0. Production
 * calibration must replace this provisional zero-point and sensitivity.
 */
static uint16_t Pressure_CalculateMmHg(uint16_t adc_value,
                                       uint16_t *adc_input_mv,
                                       uint16_t *sensor_output_uv)
{
	uint32_t input_mv;
	uint32_t signal_mv;
	uint32_t sensor_uv;
	uint32_t pressure_kpa_x100;
	uint32_t pressure_mmhg;

	input_mv = ((uint32_t)adc_value * PRESSURE_ADC_REFERENCE_MV +
	            (4095U / 2U)) / 4095U;
	if (adc_input_mv != NULL)
	{
		*adc_input_mv = (uint16_t)input_mv;
	}
	if (adc_value <= (PRESSURE_ADC_ZERO_CODE + PRESSURE_ADC_ZERO_DEADBAND))
	{
		if (sensor_output_uv != NULL)
		{
			*sensor_output_uv = 0U;
		}
		return 0U;
	}

	signal_mv = (((uint32_t)(adc_value - PRESSURE_ADC_ZERO_CODE) *
	              PRESSURE_ADC_REFERENCE_MV) + (4095U / 2U)) / 4095U;
	sensor_uv = (signal_mv * 1000U + (PRESSURE_AMPLIFIER_GAIN / 2U)) /
	            PRESSURE_AMPLIFIER_GAIN;
	pressure_kpa_x100 = (sensor_uv * PRESSURE_SENSOR_FULL_SCALE_KPA * 100U +
	                      (PRESSURE_SENSOR_FULL_SCALE_UV / 2U)) /
	                     PRESSURE_SENSOR_FULL_SCALE_UV;
	pressure_mmhg = (pressure_kpa_x100 * PRESSURE_MMHG_PER_KPA_X1000 + 50000U) /
	                 100000U;

	if (sensor_output_uv != NULL)
	{
		*sensor_output_uv = (uint16_t)sensor_uv;
	}
	if (pressure_mmhg > PRESSURE_HARD_LIMIT_MMHG)
	{
		return PRESSURE_HARD_LIMIT_MMHG;
	}

	return (uint16_t)pressure_mmhg;
}

/*
 * Pressure uses ADC2 exclusively. Discard the first result after selecting
 * PA2, then use the median of three retained conversions so one disturbed
 * conversion cannot become a displayed pressure.
 */
static uint8_t Pressure_ReadFilteredAdc(uint16_t *adc_value)
{
	uint16_t sample_a;
	uint16_t sample_b;
	uint16_t sample_c;
	uint16_t temporary;

	if (adc_value == NULL)
	{
		return 0U;
	}

	if (ADC_GetData(ADC2, ADC2_Channel_11_PA2, &temporary) == 0U)
	{
		return 0U;
	}
	if (ADC_GetData(ADC2, ADC2_Channel_11_PA2, &sample_a) == 0U)
	{
		return 0U;
	}
	if (ADC_GetData(ADC2, ADC2_Channel_11_PA2, &sample_b) == 0U)
	{
		return 0U;
	}
	if (ADC_GetData(ADC2, ADC2_Channel_11_PA2, &sample_c) == 0U)
	{
		return 0U;
	}

	if (sample_a > sample_b)
	{
		temporary = sample_a;
		sample_a = sample_b;
		sample_b = temporary;
	}
	if (sample_b > sample_c)
	{
		temporary = sample_b;
		sample_b = sample_c;
		sample_c = temporary;
	}
	if (sample_a > sample_b)
	{
		sample_b = sample_a;
	}

	*adc_value = sample_b;
	return 1U;
}

/* This is the live display path; it must not depend on debug logging. */
static void Pressure_UpdateLiveValue(uint16_t adc_value)
{
	uint16_t pressure_mmhg = Pressure_CalculateMmHg(adc_value, NULL, NULL);
	uint8_t inflation_stopped = 0U;

	if ((pressure_mmhg >= PRESSURE_SAFE_LIMIT_MMHG) &&
	    (s_ui.pressure_action == PRESSURE_ACTION_INFLATING))
	{
		s_ui.pressure_action = PRESSURE_ACTION_IDLE;
		s_ui.pressure_action_ms = 0U;
		Pressure_ApplyOutputs();
		inflation_stopped = 1U;
		LOG_W("t=%u inflation stopped, pressure=%u mmHg limit=%u mmHg",
		      s_system_tick_ms, pressure_mmhg, PRESSURE_SAFE_LIMIT_MMHG);
	}

	if (pressure_mmhg == s_ui.pressure_value)
	{
		if (inflation_stopped != 0U)
		{
			s_app.ui_dirty = 1U;
		}
		return;
	}

	s_ui.pressure_value = pressure_mmhg;
	s_ui.pressure_result_blink = 0U;
	s_app.ui_dirty = 1U;
	LOG_I("t=%u pressure live adc=%u pressure=%u mmHg", s_system_tick_ms,
	      adc_value, pressure_mmhg);
}

/*
 * ADC2 is dedicated to pressure sampling; ADC1 remains owned by the battery.
 */
static void PressureAdc_Task100ms(void)
{
	uint32_t now = s_system_tick_ms;
	uint16_t raw_value;

	if (s_app.state != APP_STATE_PRESSURE)
	{
		s_pressure_adc_sampling_active = 0U;
		s_pressure_adc_pause_ticks = 0U;
		s_pressure_adc_last_sample_ms = now;
		s_pressure_adc_window_start_ms = now;
		s_pressure_adc_sample_sum = 0U;
		s_pressure_adc_sample_count = 0U;
		return;
	}

	/*
	 * The pressure front end follows BATEN on the target board. Keep it enabled
	 * for the complete pressure session; periodic battery pulses modulate PA2.
	 */
	BATEN_ON;

	if (s_pressure_adc_pause_ticks != 0U)
	{
		s_pressure_adc_pause_ticks--;
		s_pressure_adc_last_sample_ms = now;
		return;
	}

	if (s_pressure_adc_initialized == 0U)
	{
		if (ADC2_Initial() == 0U)
		{
			LOG_E("t=%u pressure ADC2 init failed", now);
			return;
		}
		s_pressure_adc_initialized = 1U;
		LOG_I("t=%u pressure ADC2 initialized", now);
	}

	if (s_pressure_adc_sampling_active == 0U)
	{
		s_pressure_adc_sampling_active = 1U;
		s_pressure_adc_last_sample_ms = now;
		s_pressure_adc_window_start_ms = now;
		s_pressure_adc_sample_sum = 0U;
		s_pressure_adc_sample_count = 0U;
		return;
	}

	if ((uint32_t)(now - s_pressure_adc_last_sample_ms) <
	    PRESSURE_ADC_SAMPLE_PERIOD_MS)
	{
		return;
	}

	if ((uint32_t)(now - s_pressure_adc_last_sample_ms) >
	    (PRESSURE_ADC_SAMPLE_PERIOD_MS * 4U))
	{
		s_pressure_adc_last_sample_ms = now;
	}
	else
	{
		s_pressure_adc_last_sample_ms += PRESSURE_ADC_SAMPLE_PERIOD_MS;
	}

	if (Pressure_ReadFilteredAdc(&raw_value) == 0U)
	{
		if (ADC_DisableSafe(ADC2) == 0U)
		{
			ADC_DeInit(ADC2);
		}
		s_pressure_adc_initialized = 0U;
		return;
	}
	Pressure_UpdateLiveValue(raw_value);
	s_pressure_adc_sample_sum += raw_value;
	s_pressure_adc_sample_count++;

	if ((uint32_t)(now - s_pressure_adc_window_start_ms) <
	    PRESSURE_ADC_AVERAGE_PERIOD_MS)
	{
		return;
	}

#if 0 /* 周期性压力统计日志：联调其他功能时暂时屏蔽。 */
	{
		uint16_t average_value;
		uint16_t adc_input_mv;
		uint16_t sensor_output_uv;
		uint16_t pressure_mmhg;

		average_value = (uint16_t)((s_pressure_adc_sample_sum +
		                            (s_pressure_adc_sample_count / 2U)) /
		                           s_pressure_adc_sample_count);
		pressure_mmhg = Pressure_CalculateMmHg(average_value, &adc_input_mv,
		                                       &sensor_output_uv);
		LOG_I("t=%u pressure adc=%u adc_mv=%u sensor_uv=%u pressure=%u mmHg samples=%u",
		      now, average_value, adc_input_mv, sensor_output_uv, pressure_mmhg,
		      s_pressure_adc_sample_count);
	}
#endif

	s_pressure_adc_window_start_ms = now;
	s_pressure_adc_sample_sum = 0U;
	s_pressure_adc_sample_count = 0U;
}

uint8_t AppBattery_IsValid(void)
{
	return s_battery.valid;
}

uint16_t AppBattery_GetVoltageMv(void)
{
	return s_battery.voltage_mv;
}

uint8_t AppBattery_GetPercent(void)
{
	return s_battery.percent;
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
		LOG_W("t=%u therapy stopped: zero remaining time", s_system_tick_ms);
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
		LOG_I("t=%u therapy completed, time=0 power=0/0", s_system_tick_ms);
		Ui_RecordActivity();
		s_app.ui_dirty = 1U;
		return;
	}

	/* 向上取整后的显示值只在秒数减到0 时变化 */
	if (s_ui.remaining_seconds == 0U)
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
	/* TODO：解析蓝牙接收缓存，并把合法命令转换为与按键相同的事件�? */
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
	static uint8_t previous_pwr1;
	static uint8_t previous_pwr2;

	Ui_BuzzerTask10ms();

	/* PWM-only treatment: each nonzero UI level enables its own channel.
	 * The countdown and state checks are the common safety interlock. */
	TreatmentPwmEnableCh1 = ((s_app.state == APP_STATE_THERAPY) &&
	                        ((s_ui.remaining_minutes != 0U) ||
	                         (s_ui.remaining_seconds != 0U)) &&
	                        (s_ui.power_ch1 != 0U)) ? 1U : 0U;
	TreatmentPwmEnableCh2 = ((s_app.state == APP_STATE_THERAPY) &&
	                        ((s_ui.remaining_minutes != 0U) ||
	                         (s_ui.remaining_seconds != 0U)) &&
	                        (s_ui.power_ch2 != 0U)) ? 1U : 0U;

	if ((previous_pwr1 == 0U) && (TreatmentPwmEnableCh1 != 0U))
	{
		LOG_I("t=%u treatment ch1 pulse start", s_system_tick_ms);
	}
	else if ((previous_pwr1 != 0U) && (TreatmentPwmEnableCh1 == 0U))
	{
		LOG_I("t=%u treatment ch1 pulse stop", s_system_tick_ms);
	}

	if ((previous_pwr2 == 0U) && (TreatmentPwmEnableCh2 != 0U))
	{
		LOG_I("t=%u treatment ch2 pulse start", s_system_tick_ms);
	}
	else if ((previous_pwr2 != 0U) && (TreatmentPwmEnableCh2 == 0U))
	{
		LOG_I("t=%u treatment ch2 pulse stop", s_system_tick_ms);
	}

	previous_pwr1 = TreatmentPwmEnableCh1;
	previous_pwr2 = TreatmentPwmEnableCh2;

	if ((s_app.state == APP_STATE_PRESSURE) &&
	    (s_ui.pressure_action != PRESSURE_ACTION_IDLE) &&
	    (s_ui.pressure_action_ms != 0U))
	{
		if (s_ui.pressure_action_ms <= KEY_SCAN_PERIOD_MS)
		{
			s_ui.pressure_action_ms = 0U;
			s_ui.pressure_action = PRESSURE_ACTION_IDLE;
			s_app.ui_dirty = 1U;
			LOG_I("t=%u pressure action timeout", s_system_tick_ms);
		}
		else
		{
			s_ui.pressure_action_ms -= KEY_SCAN_PERIOD_MS;
		}
	}

	Pressure_ApplyOutputs();
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
	if ((Battery_Task100ms() != 0U) &&
	    (s_app.state == APP_STATE_PRESSURE))
	{
		s_pressure_adc_pause_ticks = PRESSURE_ADC_BATTERY_PAUSE_TICKS;
	}
	PressureAdc_Task100ms();
}

static void Power_Task1000ms(void)
{
	if (s_ui.charger_connected != 0U)
	{
		if (s_ui.charger_full == 0U)
		{
			s_ui.charge_frame = (uint8_t)((s_ui.charge_frame + 1U) % 4U);
			s_app.ui_dirty = 1U;
		}
	}

	Ui_Countdown1s();

	/* TODO：低电量、自动关机和蓝牙心跳在对应模块完成后接入 */
}

int main(void)
{
	Board_Init();
	App_Init();
	LOG_I("t=%u main loop started", s_system_tick_ms);

	while (1)
	{
		App_RunOnce();

		/* 等待下一次中断，避免空转占满 CPU */
		__WFI();
	}
}
