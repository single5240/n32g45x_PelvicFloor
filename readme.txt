# QW-316 主控固件

面向 HV1.01 硬件的手持盆底治疗仪主控固件。目标 MCU 为 Nations N32G455CCL7（Cortex-M4），采用裸机协作式架构，覆盖治疗双通道输出、压力检测、按键/LCD、充电与电池、蓝牙通讯等功能。

> 治疗高压、DAC 幅值、压力换算、气泵和电磁阀真值表仍需以原理图和台架实测为准。本仓库中的联调开关及参数不代表量产验收结论。

## 1. 编译与下载环境

| 项目 | 当前配置 |
| --- | --- |
| 主工程 | `MDK-ARM/Main.uvprojx` |
| 默认 Target | `N32G45x` |
| MCU | `N32G455CCL7` |
| 工具链 | Keil MDK-ARM，ARM Compiler 5（当前验证环境：V5.06 update 6） |
| 备用工程 | `EWARM/ADC_SingleRead.ewp`，仅保留，不作为默认维护入口 |
| 下载/调试 | SWD；工程关闭 JTAG、保留 SWD |

### 编译步骤

1. 使用 Keil 打开 `MDK-ARM/Main.uvprojx`。
2. 选择 `N32G45x` target，执行 **Rebuild**。
3. 编译生成物位于 `MDK-ARM/Objects/`，均已被 `.gitignore` 忽略，不提交到仓库。

命令行验证示例（本机安装 Keil 后）：

```powershell
C:\Keil_v5\UV4\UV4.exe -b MDK-ARM\Main.uvprojx -j0
```

Keil 工程元数据中的 `CLOCK(12000000)` 仅供 IDE 使用；实际运行时钟由 `firmware/CMSIS/device/system_n32g45x.c` 决定，当前为内部 8 MHz HSI 经 PLL 得到 128 MHz，`SystemCoreClock` 为 128 MHz。

## 2. 工程结构

| 路径 | 职责 |
| --- | --- |
| `src/main.c` | 应用状态机、周期调度、按键/UI、电池、压力、蓝牙业务与统一安全关断 |
| `src/n32g45x_it.c` | SysTick、USART2、TIM1/TIM8 治疗波形中断 |
| `src/init.c` | 时钟、GPIO、ADC、DAC、定时器、USART2、NVIC 初始化 |
| `inc/` | 应用、板级引脚、外设声明和联调宏定义 |
| `firmware/` | N32 CMSIS 与标准外设库 |
| `docs/` | 状态机实现与蓝牙协议说明 |
| `MDK-ARM/` | Keil 工程文件 |
| `middlewares/`、`compoennts/` | 第三方组件与 RTT 日志；`compoennts` 为历史目录名，请勿改名 |

## 3. 系统架构

系统没有 RTOS。`SysTick_Handler()` 每 1 ms 只递增时基；主循环执行 `App_RunOnce()`，只有完整执行一次 10 ms 输入、通讯、事件、状态切换和控制链后才喂独立看门狗。普通空闲处固定使用 `Delay1ms(1U)` 硬延时，不恢复普通 `__WFI()`；该延时会增加 STOP0 前等待阶段的电流。软件启动型 IWDG 在 `POWER_OFF` 阶段保持未启动，进入充电或工作状态前才启动；其余配置仍为 40 kHz LSI、32 分频和重装值 2499，标称超时约 2 s。启动日志会输出 IWDG、BOR、POR、外部复位和低功耗复位标志，用于区分软件未喂狗与疑似供电复位。

无充电器时每次物理上电默认进入 `POWER_OFF`，保留 10 s 下载和恢复窗口，期间电源键或 PB10/PB11 充电状态边沿会重新开始计时。窗口结束后通过 PB15/EXTI15、PB10/EXTI10、PB11/EXTI11 唤醒的 STOP0 降低功耗；唤醒后先恢复 HSI-PLL 产生的 128 MHz 时钟，再重建 GPIO、TIM1/TIM8、TIM4 和 USART2，最后恢复 1 ms SysTick 与其他 NVIC 中断。正常工作后 IWDG 已无法停止，因此再次关机时先立即关闭危险输出，继续运行蜂鸣器任务 2 s，再用 BKP DAT42 标记并受控复位；该标记仅在软件复位原因同时成立时有效，复位后不重复等待 10 s，直接进入 STOP0。STOP0 要求选项字节为软件 IWDG 且 `nRST_STOP=1`，条件不满足时仅记录错误并保持普通关机等待，不在运行时改写选项字节。进入 STOP0 前会关闭并复位 USART2、ADC1/ADC2、DAC、TIM1/TIM3/TIM4/TIM6/TIM8 及其非必要时钟，通过 SYS DIS 关闭 TM1621B，拉低 BATEN、拉高 BLEEN，保持治疗桥、蜂鸣器、气泵、阀门和背光的安全电平；PB4/PB5、PB7 及非唤醒按键切换为模拟输入，PB10/PB11/PB15、GPIOB/AFIO、PWR/BKP 和 SWD 保留用于唤醒与恢复。开机路径通过 `Treatment_DacReprepareForOperation()` 重建两路 DAC 与 TIM6。低功耗睡眠模式由 `APP_STOP2_ENABLE` 选择：0 为 STOP0（默认、已验证），1 为 STOP2（更深睡眠，唤醒/充电路径需先做台架验证再确定为量产默认）。

为定位蓝牙控制期间的偶发 IWDG 复位，固件使用 BKP DAT1～DAT41 保存低开销运行快照。主循环只记录当前任务阶段，SysTick 每 100 ms 保存治疗/USART2 中断计数、TIM1/TIM8 无有效 CC3 标志的异常入口计数和串口错误计数；HardFault、MemManage、BusFault、UsageFault 和断言会额外保存 CFSR、HFSR、MMFAR 和 BFAR，ARM Compiler 5 构建还会保存异常栈中的 PC、LR、xPSR 和 EXC_RETURN。启动文件另有独立 `.fault_snapshot` NOINIT SRAM 区：故障时保存 MSP/PSP、完整基础异常帧、基础帧前后各 8 字、ICSR/SHCSR/CCR、MSP 距栈底距离及栈哨兵余量；软件或 IWDG 复位后以 `fault retained` RTT 日志输出，读取后不清除。IWDG 复位后启动日志以 `diag prev`、`diag irq`、`diag fault` 输出上次 BKP 快照，其中 `spur=TIM1/TIM8` 为饱和到 63 的异常入口计数。该诊断不在治疗 ISR 内打印日志，也不改变治疗定时器配置。

治疗脉冲保留向下计数 PWM 方案。TIM1/TIM8 仅在对应治疗通道强度非零时启动计数器及 CC3 比较中断，不开启 UPDATE 和 CC4 中断。CC3 在下一次计数器重装前 200 us、即当前相位的无效输出窗口内预选下一桥臂；重装后由 PWM 硬件同步开始脉冲。每相周期为 625 us，目标有效脉宽为 300 us，其余时间两桥臂无有效输出。CH2 因使用 TIM8_CH1N/CH2N，继续按互补输出采用等效 PWM 模式。通道归零、模式切换、关机或故障关断时立即关闭对应定时器中断并清除外设和 NVIC 挂起标志。

TIM1/TIM8 的包络斜坡统一使用 3200 点 Q16 只读系数表，短斜坡通过移位索引复用该表；CC3 比较中断内不执行包络整数除法，0～60 档强度基值也直接查表。该优化约增加 6.4 KB Flash、不增加运行时 RAM，相对原整数计算的最大量化差异为 1 个 DAC 计算码。

SysTick、USART2 及治疗定时器 ISR 在退出前执行 `__DSB()`，确保外设和 SRAM 写入完成后再进行异常返回。该处理同时作为 Cortex-M4 r0p0/r0p1 勘误 838869 的软件规避措施；启动日志输出 CPUID，供目标芯片内核版本核对。
对于已确认的 Cortex-M4 r0p0/r0p1，固件还会在 MPU 关闭时设置 `ACTLR.DISDEFWBUF`，全局关闭默认写缓冲，作为勘误 838869 的强化规避。启动日志中的 `actlr` 和 `err838869` 用于确认规避是否实际生效；该措施可能轻微增加 SRAM/外设写入延迟。

| 周期 | 主要工作 |
| --- | --- |
| 每轮主循环 | 蓝牙 TX（AT 命令及协议队列）逐字节等待 TXDE 发送，单次等待超时退出；所有协作任务返回后喂 IWDG |
| 10 ms | 按键/充电检测、蓝牙解析、事件队列、状态切换、治疗/压力控制、远程动作应答 |
| 50 ms | LCD 刷新、闪烁 |
| 100 ms | 电池 ADC、压力 ADC 采样协调 |
| 1000 ms | 治疗倒计时、充电动画 |

输入均转换为 `AppEvent_t`，由应用状态机集中处理；蓝牙 `UI_ACTION` 与实体按键复用同一事件路径，不能直接改写硬件寄存器。

主要状态：`POWER_OFF`、`BOOTING`、`READY`、`THERAPY`、`PRESSURE`、`CHARGING`、`FAULT`。治疗、压力、关机、充电和故障路径均应汇聚到 `Treatment_StopOutputs()`、`Pressure_StopOutputs()` 或 `Board_EnterSafeState()`。

## 4. 功能使用

### 治疗

- 治疗页面支持 P1～P3、双通道选择与 0～60 档强度。
- CH1 使用 TIM1，幅值链路为 PA5/DAC 通道 2；CH2 使用 TIM8，幅值链路为 PA4/DAC 通道 1。
- 桥臂换向仅使用 TIM1/TIM8 的 CC3 比较中断：在重装前 200 us 预选下一桥臂，UPDATE 只作为硬件计数重装事件，不产生 ISR。
- 每个通道的两侧桥臂必须互斥，禁止同时导通。

### 压力与电池

- ADC1 用于电池电压及 PA6 外部参考采样；ADC2 用于 PA2 压力传感器采样。
- 电池电压先经 3:1 一阶低通；蓝牙上报百分比再使用 2% 死区和连续 3 次同值确认，抑制负载瞬态造成的小程序电量跳变。非充电仅允许百分比下降，充电仅允许百分比上升；首次有效采样立即上报。
- 压力流程为“空闲→预充气→正式测试→结果保持→放气复位”：首次有效采样达到 5 mmHg 后开始固定 10 s 测试，测试期间气泵以 10 kHz PWM 继续充气；电池电压 4.20 V 至 3.50 V 时占空比由 40% 线性补偿至 60%，压力值闪烁并显示实时值。
- 预充气最长 60 s；110 mmHg 为最高优先级停泵和终止测试上限。仅完整运行 10 s 的测试保留最大压力并主动上报；提前停止、放气、ADC 异常或超压均丢弃本轮数据。
- BATEN（PB2）在开机、工作和充电电池会话期间持续有效，关机或故障安全状态关闭，避免周期性通断干扰模拟前端。
- 压力换算值、过压阈值和阀门有效电平尚待硬件确认；未确认前不得以显示数值作为医疗或安全依据。

### 本机按键

| 按键 | 治疗页 | 压力页 |
| --- | --- | --- |
| 电源短按 | 循环设置 10/20/30 min | 同左 |
| 电源长按 | 开机或关机 | 开机或关机 |
| 功能短按 | 切换至压力页，并停止治疗输出 | 切换至治疗页，并停止压力输出 |
| 启动/停止短按 | 循环 P1→P2→P3，并清零强度 | 空闲时启动充气；充气中停止充气；测试中手动结束；其他阶段禁止充气 |
| 启动/停止长按 | 切换当前调节通道 | 请求放气 |
| 加/减短按 | 调整选中通道强度（0～60） | 无强度调节 |

### 蓝牙

- USART2：115200、8N1、无硬件流控，PB4/PB5 使用第三重映射。
- 协议说明见 `docs/蓝牙通讯协议V1.1.6.md`。
- 远程关机、治疗和压力 UI 动作当前已开放，但治疗/压力危险动作仍要求状态合法、未处于充电互锁且 PB7 物理连接有效；断链会走统一停止路径。
- 不再发送 `THERAPY_START_NOTIFY(0x93)`。`THERAPY_END_NOTIFY(0x94)` 发送结束类型、会话内实际输出通道、脉冲模式、时长和 CH1/CH2 结束强度；计时结束固定上报 600/1200/1800 s，主动结束均发送，时长按累计有效输出的完整秒换算。
- `UI_ACTION=08` 仅在正式压力测试阶段停止测试，并将未满 10 s 的本轮结果判为无效；仅正式测试阶段的状态快照 `flags.bit6=1`，完整运行 10 s 且 BLE 仍连接时发送 `PRESSURE_RESULT_NOTIFY(0x92)`。
- 远程开机不支持：关机状态下蓝牙模块已关闭。

## 5. 重要宏定义

以下宏位于 `inc/main.h`，修改前必须完成相应的示波器或台架验证。

| 宏 | 当前值 | 作用与注意事项 |
| --- | ---: | --- |
| `TREATMENT_DAC_OUTPUT_ENABLE` | `1` | 台架联调：启用 TIM6/DAC 治疗幅值链路；量产前仍需完成负载幅值验证。 |
| `APP_DIAGNOSTICS_ENABLE` | `0` | 卡死定位诊断关闭；诊断构建时置 1，并配合 DAC/桥臂置 0 保留内部时序中断负载。 |
| `TREATMENT_BRIDGE_PWM_OUTPUT_ENABLE` | `1` | 台架联调：启用 TIM1/TIM8 桥臂 PWM 物理输出。 |
| `TREATMENT_PULSE_FREQUENCY_HZ` | `800` | 完整双相脉冲频率；每相槽为 625 us。 |
| `TREATMENT_PULSE_WIDTH_US` | `300` | 单相桥臂有效脉宽，由定时器 PWM 比较值硬件关断。 |
| `TREATMENT_DAC_FIXED_VALUE_TEST_ENABLE` | `0` | `1` 时强制两路 DAC 输出固定码值，仅限联调。 |
| `TREATMENT_DAC_FIXED_VALUE` | `2000` | 固定 DAC 联调码值；受上限 3800 编译检查保护。 |
| `MOTOR_PWM_DUTY_HIGH_VOLTAGE_PERCENT` / `MOTOR_PWM_DUTY_LOW_VOLTAGE_PERCENT` | `40` / `60` | 气泵 TIM4_CH4 在高/低电压端的 PWM 占空比。 |
| `MOTOR_PWM_HIGH_VOLTAGE_MV` / `MOTOR_PWM_LOW_VOLTAGE_MV` | `4200` / `3500` | 气泵占空比线性补偿的高/低电压端点；区间外钳位。 |
| `PRESSURE_MAX_MMHG` | `110` | 最大压力保护上限；预充气或测试中达到即停泵、终止测试并丢弃数据。 |
| `PRESSURE_TEST_SETPOINT_MMHG` | `5` | 首次有效采样达到该值时开始固定时长测试，气泵继续运行。 |
| `PRESSURE_INFLATE_TIMEOUT_S` | `60` | 达到测试起点前的最长连续预充气时间。 |
| `PRESSURE_TEST_DURATION_S` | `10` | 正常有效测试的固定时长。 |
| `BLE_REMOTE_POWER_OFF_CONTROL_ENABLE` | `1` | 允许蓝牙 `POWER_LONG` 请求关机。 |
| `BLE_REMOTE_TREATMENT_CONTROL_ENABLE` | `1` | 允许蓝牙治疗危险动作进入状态机。 |
| `BLE_REMOTE_PRESSURE_CONTROL_ENABLE` | `1` | 允许蓝牙压力危险动作进入状态机。 |
| `APP_STOP0_ENABLE` | `1` | 允许无充电器的 `POWER_OFF` 状态进入 STOP0；须配合正确选项字节。 |
| `APP_STOP0_ENTRY_DELAY_MS` | `10000` | 上电或唤醒后进入 STOP0 前保留的下载、恢复和输入检测窗口。 |
| `APP_POWER_OFF_BEEP_DELAY_MS` | `2000` | 工作状态关机后保留蜂鸣器任务运行的时间，结束后受控复位并进入 STOP0。 |
| `APP_CORTEX_M4_838869_WORKAROUND_ENABLE` | `1` | 对 Cortex-M4 r0p0/r0p1 启用 838869 全局写缓冲规避；可能轻微增加写入延迟。 |
| `BUZZER_OUTPUT_ENABLE` | `1` | 启用蜂鸣器提示音；TIM3 使用 PB1 输出 PWM。 |

上述蓝牙宏只控制编译门禁，不取消运行时安全检查。治疗或压力输出前仍会检查充电状态、PB7 连接状态及当前应用状态。

## 6. 修改与验证要求

1. 修改治疗、DAC、PWM、定时器、GPIO、电源、压力或充电逻辑前，先核对原理图和 N32G455 数据手册。
2. ISR 中只做确定性、短时操作；不得加入 `Delay`、阻塞 I/O、动态内存或轮询等待。
3. `Pwr1/Pwr2` 被主循环和 ISR 共享，必须保持 `volatile`；新增多字节共享数据时需考虑原子性与临界区。
4. 每次提交至少执行 `git diff --check` 和 Keil Rebuild；涉及输出控制还必须在目标板验证上电安全态、启停、模式切换、断链、充电互锁和异常关断。
5. 编译通过不等于治疗输出、负载幅值、压力保护或电气安全已验证。

详细实现请阅读：

- `docs/状态机原理与业务实现.md`
- `docs/蓝牙通讯协议V1.1.6.md`
- `AGENTS.md`
