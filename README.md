# 基于 STM32 和 FreeRTOS 的智能环境监测终端

> STM32F103C8T6 + FreeRTOS 多任务温湿度监测系统：实时采集温湿度，OLED 显示、串口打印、超限报警，各任务独立运行互不阻塞。

## ✨ 功能特性

- **温湿度采集**：DHT22 单总线驱动，每 2 秒采集一次，带 8 位校验和
- **OLED 显示**：SSD1306（I2C）实时显示温湿度（整数 + 1 位小数）
- **串口打印**：UART 输出温湿度数据 + 系统运行日志（Tick / 剩余堆内存）
- **超限报警**：湿度 ≥65%，或温度 ≥40℃ / ≤10℃ 时 LED 闪烁报警
- **多任务架构**：6 个 FreeRTOS 任务独立运行，通过队列 / 信号量 / 互斥锁协作

## 🧰 硬件清单

| 模块 | 型号 | 说明 |
|------|------|------|
| 主控 | STM32F103C8T6（Blue Pill） | 72MHz |
| 温湿度传感器 | DHT22 / AM2302 | 单总线 |
| 显示屏 | OLED SSD1306 | 0.96 寸，I2C |
| USB 转串口 | CH340 | 串口调试 |
| LED  | — | 报警 |

**接线表：**

| 外设 | 引脚 | 说明 |
|------|------|------|
| DHT22 DATA | PB12 | 开漏输出 + 上拉 |
| OLED SCL / SDA | PB6 / PB7 | I2C1，100kHz |
| LED | PA1 | 超限闪烁 |


## 🏗️ 系统架构

### 任务分配

| 任务 | 优先级 | 栈（字） | 功能 | 通信方式 |
|------|--------|---------|------|---------|
| dht22_read_task | 3 | 512 | DHT22 采集 | 队列发数据、信号量报警 |
| oled_display_task | 1 | 128 | OLED 显示 | 队列收数据 |
| usart1_humitur_task | 1 | 128 | 串口打印温湿度 | 队列收数据 + 互斥锁 |
| usart1_log_task | 1 | 128 | 串口系统日志 | 互斥锁 |
| led_task | 1 | 128 | 超限 LED 报警 | 二值信号量 |



数据流
--------

| 谁发给谁 | 用什么 |
|----------|--------|
| 采集任务 → OLED / 串口 | 消息队列（传数据） |
| 采集任务 → LED 报警 | 二值信号量（传通知） |
| 两个串口任务 → UART | 互斥锁（防打架） |


- **队列**传数据（温湿度值）
- **二值信号量**传通知（超限了）
- **互斥锁**保护共享资源（printf / 串口）

## 📁 目录结构



```plaintext
humitureproject/
├── BSP/    # 底层驱动（与 RTOS 无关）
│   ├── DHT22.c / DHT22.h    # DHT22 单总线驱动
│   ├── OLED.c / OLED.h    # SSD1306 驱动（HAL 硬件 I2C）
│   ├── OLED_Font.h    # 8x16 字库
│   └── key.c / key.h    # 按键扫描
├── APP/    # 应用任务（RTOS 相关）
│   ├── DHT22_task.c    # 采集任务
│   ├── oled_task.c    # 显示任务
│   ├── usart_task.c    # 串口任务（数据 + 日志）
│   ├── outlimit_task.c    # 报警任务
│   └── key_task.c    # 按键任务
└── Core/    # STM32CubeIDE 生成（HAL）
    ├── Inc/
    │   └── main.h    # 数据结构 + 全局句柄
    ├── Src/
    │   ├── main.c    # 任务创建 + printf 重定向
    │   └── stm32f1xx_hal_timebase_tim.c    # HAL 时基切到 TIM4
    └── Middlewares/
        └── FreeRTOS/    # FreeRTOS 内核（手动移植）
            └── ...（具体文件省略）
```





## 🔑 技术亮点

1. **FreeRTOS 手动移植**：正确配置 SVC / PendSV / SysTick 三个中断，实现任务调度
2. **HAL 时基冲突解决**：将 HAL 时基从 SysTick 切换到 TIM4，把 SysTick 让给 FreeRTOS
3. **DWT 微秒延时 + 临界区**：用 DWT 硬件计数器实现微秒级精准延时，配合临界区保护，解决 RTOS 环境下 SysTick 中断打断 DHT22 单总线时序导致的数据错乱
4. **三种 IPC 各司其职**：队列传数据、二值信号量传通知、互斥锁保护串口，解决多任务并发下的数据撕裂和串口乱码
5. **printf 重定向 + 互斥锁**：重定向 `printf` 到 UART，多个串口任务通过互斥锁串行访问，避免打印交叉错乱

## 🚀 快速开始

1. 用 **STM32CubeIDE** 打开工程
2. 编译烧录到 STM32F103C8T6
3. 打开串口助手，即可看到温湿度数据和系统日志
4. 观察 OLED 屏幕实时显示，用手捏住 DHT22 升温测试报警功能


## 🔧 环境

- STM32CubeIDE（GCC，arm-none-eabi）
- FreeRTOS（heap_4，总堆 4096 字节，Tick 100Hz）
- HAL 库
