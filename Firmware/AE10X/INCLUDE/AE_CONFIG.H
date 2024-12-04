/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-08 16:55:09
 * @Description:
 *
 *
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 以下是中文及英文版权同步声明，编码为UTF-8。
 * Copyright has legal effects and violations will be prosecuted.
 * 版权具有法律效力，违反必究。
 *
 * Copyright ©2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2023龙晶石半导体科技（苏州）有限公司
 */
#ifndef AE_CONFIG_H
#define AE_CONFIG_H
/****************************************************************************
 * Include Headfile
 *****************************************************************************/
#include "AE_GLOBAL.H"
#include "AE_REG.H"
/****************************************************************************
 * 平台选择
 *****************************************************************************/
#define SCI_POLLING_CONTROL 0 // 由于龙芯平台SCI有效时间为15ms，如果为龙芯平台需要把该宏打开

// GLE01主系统相关控制宏
#define GLE01 1                      // GLE01打开关闭
#define FLASH_TO_IRAM0 1             // 打开crypto代码从flash搬运到IRAM0功能
#define MAILBOX_SMS_UPDATE_SUPPORT 1 // 打开4K SMS方式验证更新
#define secure_area 0x80000          // 不能更新的安全区域 512K

/****************************************************************************
 *  Functional support
 *****************************************************************************/
#define SUPPORT_LOWPOWER 0    // 低功耗
#define SUPPORT_GPIO_WAKEUP 0 // GPIO中断唤醒
#ifdef AE103
#define SUPPORT_RTC_WAKEUP 0 // RTC定时器唤醒
#endif
#define SUPPORT_CPUTIMER_WAKEUP 0 // CPU定时器唤醒
#define SUPPORT_PWRSW_WAKEUP 0    // 长按GPIOB4唤醒
#define SUPPORT_KBS_WAKEUP 0      // 键盘按键唤醒
#define SUPPORT_KBS_REPEAT 0      // Enabled KBS REPEAT FUNCTION
#define SUPPORT_MATRIX_UTILITY 0  // Enabled PMC Matrix Utility Support

// ACPI Function Setting
#define SUPPORT_ACPI_SMB_EC 0     // Enable ACPI SMBus via EC
#define SUPPORT_ACPI_SMI 0        // Use SMI in ACPI Mode(No recommend)
#define SUPPORT_QEvent_Pending 0  // QEvent Pending support
#define SUPPORT_I2C_TEMPERATURE 0 // I2C temp get

// eSPI Function Support
#define ITE_eSPI_LOW_FREQUENCY FALSE // TRUE: For Bring Up, LA Analyzer
#define SUPPORT_INTERFACE_eSPI TRUE
#define ITE_eSPI_SAFS_MODE FALSE
#define eSPI_RESET_FOR_DSW_PWROK TRUE     // TRUE: eSPI with None-DeepSx
#define SUPPORT_EN_VW_ACK_BOOT_LOAD FALSE // TRUE: Boot Ack after VW enable
#define SUPPORT_OOB_CRASHLOG TRUE
#define SUPPORT_OOB_CRASHLOG_KEY TRUE
#define _OOB_CRASHLOG_LOCK_TIMER 8
#define SUPPORT_HOOK_WARMBOOT TRUE
#define ENABLE_eSPI_INT_QUICK_CTRL

// Enabled KB Interrupt when system Sx
#define ENABLE_S5_KEYBOARD_INTR 0 // Enabled KB Interrupt when system S5
#define ENABLE_S3_KEYBOARD_INTR 0 // Enabled KB Interrupt when system S3

// debuger Function Setting
// EC FIRMWARE UPDATA
#define SUPPORT_FIRMWARE_UPDATE 0 // firmware update support
#define SUPPORT_SHAREMEM_PNP 1    // SHM PNP support
// UART
#define SUPPORT_REAL_OR_DELAY_PRINTF 0 // 0:实时输出；1：延时输出（延时输出可切换为实时输出）
#define ENABLE_COMMAND_SUPPORT 1       // CMD Enable
#define ENABLE_DEBUGGER_SUPPORT 1      // Debugger Enable
#define SUPPORT_BREATHING_LED 0        // LED
// port80 &&pnp
#define SUPPORT_LD_PNP 1           // 1:open logicdevice pnp  0:close pnp
#define SUPPORT_LD_PNP_DEVBOARD 0  // 1:open logicdevice pnp when devboard  0:close pnp
#define SUPPORT_8042DEBUG_OUTPUT 0 // 输出8042交互命令数据流到SRAM，必要时可以将这段内存打印出来观察
#define SUPPORT_MEM_MALLOC 0       // 0 run mem_malloc 1 not run mem_malloc (if user it,can't user heap)

// driver
#define SUPPORT_ANX7447 0 // ANX7447 PD CHIP
//battery charge
#define SUPPORT_BATTERY_CHARGE 0
// PS2 device scan
#define SUPPROT_PS2DEV_SCAN 0
// 风扇控制
#define SUPPORT_FAN1 0 // 风扇控制
#define SUPPORT_FAN2 0

/****************************************************************************
 * Service Enable
 *****************************************************************************/
#define Service_PCI_START 1
#define Service_Mailbox_START 1
#define Service_Send_START 1
#define Service_Send_PS2_START 1
#define Service_PCI2_START 1
#define Service_MS_1_START 1
#define Service_PCI3_START 1
#define Service_PCI4_START 1
#define Service_PCI5_START 1
#define Service_PCI6_START 1
#define Service_Reserved2_START 0
#define Service_Reserved3_START 0
/****************************************************************************
 * Module Enable
 *****************************************************************************/
#define GPIO_MODULE_EN 1
#define IRQC_MODULE_EN 1
#define ICTL_MODULE_EN 1
#define I2C_MODULE_EN 1
#define PS2_MODULE_EN 1
#define KBS_MODULE_EN 1
#define SPI_MODULE_EN 0
#define WDT_MODULE_EN 0
#define PWM_MODULE_EN 1
#define TACH_MODULE_EN 0
#define TIMER_MODULE_EN 1
#define PMC_MODULE_EN 1
#define KBC_MODULE_EN 1
#define ADC_MODULE_EN 1
#define UART_MODULE_EN 1
// reserved
#if (defined(AE103) || defined(AE102))
#define PPORT_MODULE_EN 0
#define CAN_MODULE_EN 0
#endif
#if (PPORT_MODULE_EN & KBS_MODULE_EN)
#warning "You want use KBS ,but PPORT Enable, They reuse the same pin"
#endif
#if (defined(AE103))
#define CEC_MODULE_EN 0
#define OWI_MODULE_EN 0
#define RTC_MODULE_EN 0
#define PECI_MODULE_EN 0
#define ESPI_MODULE_EN 0

#endif
/****************************************************************************
 * Switch
 *****************************************************************************/
/* ACPI EC SCI Interrupt Response Timing ( 8-16uSec).*/
#define ACPI_SCI_Response 0

#define KBD_8_n_SWITCH 18 // n:8*n
//#define KBD_SN1211        // kbd demo code if nos's run sn1211 please undef
#if (KBD_8_n_SWITCH == 16)
#define KBD_8_16
#elif (KBD_8_n_SWITCH == 17)
#define KBD_8_17
#if (SPI_MODULE_EN)
#warning "You want use KBSOUT[16] ,but SPIM Enable, They reuse the same pin"
#endif
#elif (KBD_8_n_SWITCH == 18)
#define KBD_8_18
#if (SPI_MODULE_EN)
#warning "You want use KBSOUT[16:17] ,but SPIM Enable, They reuse the same pin"
#endif
#endif
#define LPC_WAY_OPTION_SWITCH 1 // 1:polling way   0:interrupt way
#define KBC_PNP_ONLY_SWITCH 0   // 0:all logicdevice 1:only open keyboard&mouse
#define FAN1_PWM_CHANNEL_SWITCH 4
#define FAN2_PWM_CHANNEL_SWITCH 0

#define CHIP_CLOCK_INT_HIGH 96000000
#define CHIP_CLOCK_INT_LOW  32000
#define CHIP_CLOCKFREQ_DEFAULT 4     // 1(96) 2(48) 3(32) 4（24）5（19.2）分频数（时钟）                                                                              
#define LOW_CHIP_CLOCK CHIP_CLOCK_INT_LOW
#define HIGHT_CHIP_CLOCK (CHIP_CLOCK_INT_HIGH / CHIP_CLOCK_SWITCH) // 96M/CHIP_CLOCK_SWITCH
#define TIMER_FREQ LOW_CHIP_CLOCK * 2

#define ADC_CLOCK_SWITCH 7
#define ADC_CLOCK ((LOW_CHIP_CLOCK * 3000) / ((ADC_CLOCK_SWITCH) ? (ADC_CLOCK_SWITCH * 2) : 1)) // 96M/CHIP_CLOCK_SWITCH
#define PLL_POSTDIV2 ((ADC_CLOCK_SWITCH & 0x1f) << 21)
// uart 功能选择 suggestions COMMAND_UART_SWITCH == PRINTF_UART_SWITCH != DEBUG_UART_SWITCH
#if (defined(AE102) || defined(AE103))
#if ENABLE_DEBUGGER_SUPPORT
#define DEBUG_UART_SWITCH 1
#else
#undef DEBUG_UART_SWITCH
#endif
#define PRINTF_UART_SWITCH 0                 // 串口引脚选择 uarta对应数值2 uartb对应数值3
#define COMMAND_UART_SWITCH PRINTF_UART_SWITCH // It's aobut receive uart, comand send is printf.
#else
#if ENABLE_DEBUGGER_SUPPORT
#define DEBUG_UART_SWITCH 4
#else
#undef DEBUG_UART_SWITCH
#endif

#define PRINTF_UART_SWITCH 0                   // 串口引脚选择 uarta对应数值2 uartb对应数值3
#define COMMAND_UART_SWITCH PRINTF_UART_SWITCH // It's aobut receive uart, comand send is printf.
#endif
/****************************************************************************
 * Module SWITCH Enable
 *****************************************************************************/
// uart0~3时间精度:8/freq
/*       各分频推荐最大波特率：

 *    主频        最小波特率     最大波特率        推荐范围        推荐波特率
 * (3.072M):        5.86        384000          <=38400         9600/19200
 * (3M)             5.722       375000          <=37500         9600/14400
 * (6.144M)         11.72       768000          <=76800         19200/38400
 * (6M)             11.444      750000          <=75000         14400/38400
 * (12.288M)        23.44       1536000         <=153600        38400/57600
 * (12M)            22.889      1500000         <=150000        38400/56000
 * (24.576M)        46.88       3072000         <=307200        57600/128000
 * (24M)            45.777      3000000         <=300000       56000/115200
 * (49.152M)        93.75       6144000         <=614400       128000/256000
 * (48M):           91.55       6000000         <=600000       115200/256000
 *注：频率都在理想范围内计算，实际存在误差，波特率精度应在±1%~±5%区间
 */
// uartAB时间精度:16/freq
/*       各分频推荐最大波特率：

 *    主频        最小波特率     最大波特率        推荐范围        推荐波特率
 * (3.072M):        2.93        192000          <=19200           9600
 * (3M)             5.722       187500          <=18750           9600
 * (6.144M)         11.72       384000          <=38400           19200
 * (6M)             11.444      375000          <=37500           14400
 * (12.288M)        23.44       768000          <=76800        38400/57600
 * (12M)            22.889      750000          <=75000        38400/56000
 * (24.576M)        46.88       1536000         <=153600       57600/128000
 * (24M)            45.777      1500000         <=150000       56000/115200
 * (49.152M)        93.75       3072000         <=307200       128000/256000
 * (48M):           91.55       3000000         <=300000       115200/256000
 *注：频率都在理想范围内计算，实际存在误差，波特率精度应在±1%~±5%区间
 */
#if (COMMAND_UART_SWITCH == 0 || DEBUG_UART_SWITCH == 0 || PRINTF_UART_SWITCH == 0)
#define UART0_BAUD 115200
#endif
#if (defined(AE102) || defined(AE103))
#if (COMMAND_UART_SWITCH == 1 || DEBUG_UART_SWITCH == 1 || PRINTF_UART_SWITCH == 1)
#define UART1_BAUD 115200
#endif
// #if (COMMAND_UART_SWITCH == 2 || DEBUG_UART_SWITCH == 2 || PRINTF_UART_SWITCH == 2)
// #define UART2_BAUD 115200
// #endif
// #if (COMMAND_UART_SWITCH == 3 || DEBUG_UART_SWITCH == 3 || PRINTF_UART_SWITCH == 3)
// #define UART3_BAUD 115200
// #endif
#endif
#if (COMMAND_UART_SWITCH == 2 || DEBUG_UART_SWITCH == 2 || PRINTF_UART_SWITCH == 2)
#define UARTA_BAUD 115200
#endif
#if (COMMAND_UART_SWITCH == 3 || DEBUG_UART_SWITCH == 3 || PRINTF_UART_SWITCH == 3)
#define UARTB_BAUD 115200
#endif

// I2C 初始化
#define I2C0_EN_Init 1 & SMBUS0_CLOCK_EN
#define I2C0_SPEED 100000
#define I2C1_EN_Init 0 & SMBUS1_CLOCK_EN
#define I2C1_SPEED 100000
#define I2C2_EN_Init 0 & SMBUS2_CLOCK_EN
#define I2C2_SPEED 100000
#define I2C3_EN_Init 1 & SMBUS3_CLOCK_EN
#define I2C3_SPEED 100000
#if (defined(AE103))
#define I2C4_EN_Init 0 & SMBUS4_CLOCK_EN
#define I2C4_SPEED 100000
#define I2C5_EN_Init 0 & SMBUS5_CLOCK_EN
#define I2C5_SPEED 100000
/*yujiang add*/
#define I2C6_EN_Init 0 & SMBUS6_CLOCK_EN
#define I2C6_SPEED 100000
#define I2C7_EN_Init 0 & SMBUS7_CLOCK_EN
#define I2C7_SPEED 100000
#define I2C8_EN_Init 0 & SMBUS8_CLOCK_EN
#define I2C8_SPEED 100000
/*yujiang add*/
#endif
#if SPI_MODULE_EN
#define SPIM_CS_EN 1
#endif
// PWM TACH 初始化
#define PWM0_EN_Init 0 & PWM_CLOCK_EN
#define PWM1_EN_Init 0 & PWM_CLOCK_EN
#define PWM2_EN_Init 0 & PWM_CLOCK_EN
#define PWM3_EN_Init 0 & PWM_CLOCK_EN
#define PWM4_EN_Init 0 & PWM_CLOCK_EN
#define PWM5_EN_Init 0 & PWM_CLOCK_EN
#define PWM6_EN_Init 0 & PWM_CLOCK_EN
#define PWM7_EN_Init 0 & PWM_CLOCK_EN
#define TACH0_EN 1 & PWM_CLOCK_EN
#define TACH1_EN 0 & PWM_CLOCK_EN
#define TACH2_EN 0 & PWM_CLOCK_EN
#define TACH3_EN 0 & PWM_CLOCK_EN
// PS2初始化
#define PS2_0_EN_Init 1 & PS2_0_CLOCK_EN
#define PS2_1_EN_Init 1 & PS2_1_CLOCK_EN
// ADC初始化
#define ADC0_EN_Init 0 & ADC_CLOCK_EN
#define ADC1_EN_Init 0 & ADC_CLOCK_EN
#define ADC2_EN_Init 0 & ADC_CLOCK_EN
#define ADC3_EN_Init 0 & ADC_CLOCK_EN
#define ADC4_EN_Init 0 & ADC_CLOCK_EN
#define ADC5_EN_Init 0 & ADC_CLOCK_EN
#define ADC6_EN_Init 0 & ADC_CLOCK_EN
#define ADC7_EN_Init 0 & ADC_CLOCK_EN
#define ADC7_EN_Init 0 & ADC_CLOCK_EN
// CAN初始化
#if (defined(AE103) || defined(AE102))
#define CAN0_EN_Init 0 & CAN0_CLOCK_EN
#define CAN1_EN_Init 0 & CAN1_CLOCK_EN
#define CAN2_EN_Init 0 & CAN2_CLOCK_EN
#define CAN3_EN_Init 0 & CAN3_CLOCK_EN
#endif
#if (CEC_MODULE_EN & PS2_0_EN)
#warning "You want use KBSOUT[16] ,but SPIM Enable, They reuse the same pin"
#endif
#if (SPI_MODULE_EN & PWM6_EN_Init)
#warning "You want use PWM6 ,but SPIM SCK Enable, They reuse the same pin"
#endif
#if (CAN1_EN_Init & PWM6_EN_Init)
#warning "You want use PWM4 or PWM5 ,but CAN1 TXRX Enable, They reuse the same pin"
#endif
#if (PECI_MODULE_EN & I2C2_EN_Init)
#warning "You want use PECI ,but I2C2 Enable, They reuse the same pin"
#endif

/****************************************************************************
 * Debug enable
 *****************************************************************************/
#define DEBUG 1                                       // 0:close printf   1:open printf
#define KBS_DEBUG 0 & DEBUG                           // 0:close printf   1:open printf
#define KBC_DEBUG 0 & DEBUG                           // 0:close printf   1:open printf
#define I2C_DEBUG 0 & DEBUG                           // 0:close printf   1:open printf
#define IRQC_DEBUG 0 & DEBUG                          // 0:close printf   1:open printf
#define ANX7447_UCSI_DEBUG 1 & DEBUG &SUPPORT_ANX7447 // 0:close printf   1:open printf

/****************************************************************************
 * Test enable
 *****************************************************************************/
// TEST为1是TEST_MAIN.c测试代码 0为KERNEL_MAIN.c下的主体代码
#define TEST 0

#endif /* AE_CONFIG_H */
