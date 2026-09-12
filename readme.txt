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

系统没有 RTOS。`SysTick_Handler()` 每 1 ms 只递增时基；主循环执行 `App_RunOnce()`，只有完整执行一次 10 ms 输入、通讯、事件、状态切换和控制链后才喂独立看门狗，空闲时通过 `__WFI()` 等待中断。这样主循环阻塞、系统时基停止或周期业务无法完成时都不会继续喂狗。SysTick、USART2 与治疗定时器处于同一抢占级，不会相互抢占治疗 ISR；同时挂起时 SysTick 和 USART2 的响应顺序高于治疗定时器，降低时基及蓝牙接收饥饿风险。IWDG 按 40 kHz LSI、32 分频和重装值 2499 配置，标称超时约 2 s。启动日志会输出 IWDG、BOR、POR、外部复位和低功耗复位标志，用于区分软件未喂狗与疑似供电复位。

为定位蓝牙控制期间的偶发 IWDG 复位，固件使用 BKP DAT1～DAT41 保存低开销运行快照。主循环只记录当前任务阶段，SysTick 每 100 ms 保存治疗/USART2 中断计数和串口错误计数；HardFault、MemManage、BusFault、UsageFault 和断言会额外保存 CFSR、HFSR、MMFAR 和 BFAR，ARM Compiler 5 构建还会保存异常栈中的 PC、LR、xPSR 和 EXC_RETURN。IWDG 复位后启动日志以 `diag prev`、`diag irq`、`diag fault` 输出上次快照。该诊断不在治疗 ISR 内打印日志，也不改变治疗定时器配置。

TIM1/TIM8 仅在对应治疗通道强度非零时启动计数器及 UPDATE/CC3 中断。通道归零、模式切换、关机或故障关断时立即关闭对应定时器中断并清除外设和 NVIC 挂起标志；再次从零档启动时复位计数器后重新使能，避免关机和未使用通道持续产生无效高频中断。

SysTick、USART2 及治疗定时器 ISR 在退出前执行 `__DSB()`，确保外设和 SRAM 写入完成后再进行异常返回。该处理同时作为 Cortex-M4 r0p0/r0p1 勘误 838869 的软件规避措施；启动日志输出 CPUID，供目标芯片内核版本核对。
对于已确认的 Cortex-M4 r0p0/r0p1，固件还会在 MPU 关闭时设置 `ACTLR.DISDEFWBUF`，全局关闭默认写缓冲，作为勘误 838869 的强化规避。启动日志中的 `actlr` 和 `err838869` 用于确认规避是否实际生效；该措施可能轻微增加 SRAM/外设写入延迟。

| 周期 | 主要工作 |
| --- | --- |
| 每轮主循环 | 蓝牙 TX 队列发送；所有协作任务返回后喂 IWDG |
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
- 桥臂换向采用“当前桥臂关闭 → 全关断死区 → 另一桥臂开启”。TIM1/TIM8 的 CC3 比较中断仅用于结束死区。
- 每个通道的两侧桥臂必须互斥，禁止同时导通。

### 压力与电池

- ADC1 用于电池电压及 PA6 外部参考采样；ADC2 用于 PA2 压力传感器采样。
- BATEN（PB2）在开机、工作和充电电池会话期间持续有效，关机或故障安全状态关闭，避免周期性通断干扰模拟前端。
- 压力换算值、过压阈值和阀门有效电平尚待硬件确认；未确认前不得以显示数值作为医疗或安全依据。

### 本机按键

| 按键 | 治疗页 | 压力页 |
| --- | --- | --- |
| 电源短按 | 循环设置 10/20/30 min | 同左 |
| 电源长按 | 开机或关机 | 开机或关机 |
| 功能短按 | 切换至压力页，并停止治疗输出 | 切换至治疗页，并停止压力输出 |
| 启动/停止短按 | 循环 P1→P2→P3，并清零强度 | 启动或停止充气 |
| 启动/停止长按 | 切换当前调节通道 | 请求放气 |
| 加/减短按 | 调整选中通道强度（0～60） | 无强度调节 |

### 蓝牙

- USART2：115200、8N1、无硬件流控，PB4/PB5 使用第三重映射。
- 协议说明见 `docs/蓝牙通讯协议V1.1.2.md`。
- 远程关机、治疗和压力 UI 动作当前已开放，但治疗/压力危险动作仍要求状态合法、未处于充电互锁且 PB7 物理连接有效；断链会走统一停止路径。
- 远程开机不支持：关机状态下蓝牙模块已关闭。

## 5. 重要宏定义

以下宏位于 `inc/main.h`，修改前必须完成相应的示波器或台架验证。

| 宏 | 当前值 | 作用与注意事项 |
| --- | ---: | --- |
| `TREATMENT_DAC_OUTPUT_ENABLE` | `1` | 允许 TIM6/DAC 治疗幅值链路；DAC=0 是否等于高压安全归零需实测。 |
| `TREATMENT_BRIDGE_PWM_OUTPUT_ENABLE` | `1` | 允许 TIM1/TIM8 桥臂 PWM 输出；会实际驱动治疗桥臂。 |
| `TREATMENT_BRIDGE_DEADTIME_US` | `50` | 换向全关断死区，基于当前 128 MHz 时钟和定时器预分频 7；不可在未测关断时间前缩短。 |
| `TREATMENT_DAC_FIXED_VALUE_TEST_ENABLE` | `0` | `1` 时强制两路 DAC 输出固定码值，仅限联调。 |
| `TREATMENT_DAC_FIXED_VALUE` | `2000` | 固定 DAC 联调码值；受上限 3800 编译检查保护。 |
| `BLE_REMOTE_POWER_OFF_CONTROL_ENABLE` | `1` | 允许蓝牙 `POWER_LONG` 请求关机。 |
| `BLE_REMOTE_TREATMENT_CONTROL_ENABLE` | `1` | 允许蓝牙治疗危险动作进入状态机。 |
| `BLE_REMOTE_PRESSURE_CONTROL_ENABLE` | `1` | 允许蓝牙压力危险动作进入状态机。 |
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
- `docs/蓝牙通讯协议V1.1.2.md`
- `AGENTS.md`
