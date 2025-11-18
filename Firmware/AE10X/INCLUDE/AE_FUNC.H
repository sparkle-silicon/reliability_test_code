/*
 * @Author: Linyu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2025-11-15 18:05:06
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
#ifndef AE_FUNC_H
#define AE_FUNC_H
#include "AE_GLOBAL.H"
 /*************时钟模块控制宏*************/
#define SPIM_CLOCK_EN 1
#define CAN3_CLOCK_EN 1
#define CAN2_CLOCK_EN 1
#define CAN1_CLOCK_EN 1
#define CAN0_CLOCK_EN 1
#define UART3_CLOCK_EN 1
#define UART2_CLOCK_EN 1
#define UART1_CLOCK_EN 1
#define UART0_CLOCK_EN 1
#define UARTB_CLOCK_EN 1
#define UARTA_CLOCK_EN 1
#define PWM_CLOCK_EN 1
#define ADC_CLOCK_EN 1
#define PPORT_CLOCK_EN 1
#define WDT_CLOCK_EN 1
#define SMBUS8_CLOCK_EN 1
#define SMBUS7_CLOCK_EN 1
#define SMBUS6_CLOCK_EN 1
#define SMBUS5_CLOCK_EN 1
#define SMBUS4_CLOCK_EN 1
#define SMBUS3_CLOCK_EN 1
#define SMBUS2_CLOCK_EN 1
#define SMBUS1_CLOCK_EN 1
#define SMBUS0_CLOCK_EN 1
#define GPIO_CLOCK_EN 1
#define BRAM_CLOCK_EN 1
#define SWUC_CLOCK_EN 1
#define SHM_CLOCK_EN 1
#define PMCKBC_CLOCK_EN 1
#define PS2_0_CLOCK_EN 1
#define PS2_1_CLOCK_EN 1
#define CEC_CLOCK_EN 1
#define KBS_CLOCK_EN 1
#define RTC_CLOCK_EN 1
#define PECI_CLOCK_EN 1
#define ESPI_CLOCK_EN 1
#define TIMER3_CLOCK_EN 1
#define TIMER2_CLOCK_EN 1
#define TIMER1_CLOCK_EN 1
#define TIMER0_CLOCK_EN 1
#define ICTL_CLOCK_EN 1 // 中断控制器模块使能控制
#define H2E_CLOCK_EN 1  // H2E模块使能控制
#define SPIFE_CLOCK_EN 1
#define SRAM_CLOCK_EN 1
#define GPIODB_CLOCK_EN 1 // GPIO去抖使能
#define SYSCTL_CLOCK_EN 1
#define DRAM_CLOCK_EN 1
#define APB_CLOCK_EN 1 // E-P总线使能控制
#define I3C0_CLOCK_EN 1
#define I3C1_CLOCK_EN 1
#define I3C2_CLOCK_EN 1
#define I3C3_CLOCK_EN 1
#define MAILBOX_CLOCK_EN 1
/*************时钟模块控制宏*************/
// SYSCTL MOUDLE ENABLE BIT
// SYSCTL_MODE0
#define TMR1_EN         0x80000000
#define TMR2_EN         0x40000000
#define TMR3_EN         0x20000000
#define KBS_EN          0x10000000
#define PS2_1_EN        0x08000000
#define PS2_0_EN        0x04000000
#define ROMP_EN         0x02000000
#define PMCKBC_EN       0x01000000
#define SHM_EN          0x00800000
#define SWUC_EN         0x00400000
#define BRAM_EN         0x00200000
#define GPIO_EN         0x00100000
#define SMB0_EN         0x00080000
#define SMB1_EN         0x00040000
#define SMB2_EN         0x00020000
#define SMB3_EN         0x00010000
#define WDT_EN          0x00008000
#define I3C3_EN         0x00004000
#define MAILBOX_EN      0x00002000
#define ADC_EN          0x00001000
#define PWM_EN          0x00000800
#define UARTA_EN        0x00000400
#define UARTB_EN        0x00000200
#define UART0_EN        0x00000100
#define UART1_EN        0x00000080
#define I3C2_EN         0x00000040
#define I3C1_EN         0x00000020
#define I3C0_EN         0x00000010
#define SMB8_EN         0x00000008
#define SMB7_EN         0x00000004
#define SMB6_EN         0x00000002
#define SPIM_EN         0x00000001
// SYSCTL_MODE1
#define CRYPTO_EN       0x01000000
#define IRAM_EN         0x00800000
#define PECI_EN         0x00400000
#define RTC_EN          0x00200000
#define ROM_EN          0x00100000
#define CACHE_EN        0x00080000
#define IVT_EN          0x00040000
// #define RESERVD_EN          0x00020000
#define CEC_EN          0x00010000
#define SMB4_EN         0x00008000
#define SMB5_EN         0x00004000
#define IRAM1_EN        0x00002000
#define ESPI_EN         0x00001000
#define APB_EN          0x00000800
#define DRAM_EN         0x00000400
#define SYSCTL_EN       0x00000200
// #define RESERVD_EN   0x00000100
#define SPIFI_EN        0x00000080
#define DMA_EN          0x00000040
#define GPIODB_EN       0x00000020
#define SRAM_EN         0x00000010
#define SPIFE_EN        0x00000008
#define H2E_EN          0x00000004
#define ICTL_EN         0x00000002
#define TMR0_EN         0x00000001
//-----------------------------------------------------------------------------
// SYSCTL_RST0 寄存器位定义
//-----------------------------------------------------------------------------
#define PS2M_RST      (1UL << 31) // PS2M模块复位控制
// #define ROMP_RST       (1UL << 30) // ROMP模块复位控制
#define KBC_RST       (1UL << 29) // KBC模块复位控制
#define PMC5_RST      (1UL << 28) // PMC5模块复位控制
#define PMC4_RST      (1UL << 27) // PMC4模块复位控制
#define PMC3_RST      (1UL << 26) // PMC3模块复位控制
#define PMC2_RST      (1UL << 25) // PMC2模块复位控制
#define PMC1_RST      (1UL << 24) // PMC1模块复位控制
#define SHM_RST       (1UL << 23) // SHMEM模块复位控制
#define SWUC_RST      (1UL << 22) // SWUC模块复位控制
#define BRAM_RST      (1UL << 21) // BRAM模块复位控制
#define GPIO_RST      (1UL << 20) // GPIO模块复位控制
#define SMB0_RST      (1UL << 19) // SMBUS0模块复位控制
#define SMB1_RST      (1UL << 18) // SMBUS1模块复位控制
#define SMB2_RST      (1UL << 17) // SMBUS2模块复位控制
#define SMB3_RST      (1UL << 16) // SMBUS3模块复位控制
#define WDT_RST       (1UL << 15) // WDT模块复位控制
#define I3C3_RST      (1UL << 14) // I3C3模块复位控制
#define HOST_RST      (1UL << 13) // HOST模块复位控制
#define ADC_ANA_RST   (1UL << 12) // ADC模块模拟电路复位控制
#define PWM_RST       (1UL << 11) // PWM模块复位控制
#define UARTA_RST     (1UL << 10) // UARTA模块复位控制
#define UARTB_RST     (1UL << 9)  // UARTB模块复位控制
#define UART0_RST     (1UL << 8)  // UART0模块复位控制
#define UART1_RST     (1UL << 7)  // UART1模块复位控制
#define I3C2_RST      (1UL << 6)  // I3C2模块复位控制
#define I3C1_RST      (1UL << 5)  // I3C1模块复位控制
#define I3C0_RST      (1UL << 4)  // I3C0模块复位控制
#define SMB8_RST      (1UL << 3)  // SMB8模块复位控制
#define SMB7_RST      (1UL << 2)  // SMB7模块复位控制
#define SMB6_RST      (1UL << 1)  // SMB6模块复位控制
#define SPIM_RST      (1UL << 0)  // SPIM模块复位控制
//-----------------------------------------------------------------------------
// SYSCTL_RST1 寄存器位定义
//-----------------------------------------------------------------------------
#define ADC_CLK_DIG_RST   (1UL << 30)     // ADC 数字逻辑时钟复位控制
#define PECI_RST          (1UL << 29)     // PECI模块复位控制
#define RTC_RST           (1UL << 28)     // RTC模块复位控制
#define SMB4_RST          (1UL << 26)     // SMBUS4模块复位控制
#define SMB5_RST          (1UL << 25)     // SMBUS5模块复位控制
#define CEC_RST           (1UL << 23)     // CEC模块复位控制
#define ADC_DIG_RST       (1UL << 22)     // ADC模块数字逻辑复位控制
#define ESPI_RST           (1UL << 21)     // ESPI模块复位控制
#define CRYPTO_CPU_RST     (1UL << 17)     // CRYPTO CPU模块复位控制
#define CHIP_RST           (1UL << 16)     // 全芯片的复位控制
#define APB_RST            (1UL << 15)     // APB总线复位控制
#define SYSCTL_RST         (1UL << 14)     // SYSCTL模块复位控制
#define MAILBOX_RST        (1UL << 13)     // PNP模块软复位控制
#define SPIFI_RST          (1UL << 12)     // SPIF EMB模块软复位控制
#define DMA_RST            (1UL << 11)     // DMA模块软复位控制
#define PNP_RST            (1UL << 10)     // PNP模块软复位控制
#define LDN_RST            (1UL << 9)      // 逻辑设备的软复位控制
#define SPIFE_RST          (1UL << 8)     // SPIF模块复位控制
#define H2E_RST            (1UL << 7)      // H2E模块复位控制
#define ICTL_RST           (1UL << 6)      // 中断控制模块复位控制
#define TMR0_RST           (1UL << 5)      // TIMER0模块复位控制
#define TMR1_RST           (1UL << 4)      // TIMER1模块复位控制
#define TMR2_RST           (1UL << 3)      // TIMER2模块复位控制
#define TMR3_RST           (1UL << 2)      // TIMER3模块复位控制
#define KBS_RST            (1UL << 1)      // KBS模块复位控制
#define PS2K_RST           (1UL << 0)      // PS2K模块复位控制
extern void enable_mcycle_minstret();
extern void disable_mcycle_minstret();
extern void core_wfe();
extern void pmp_open_all_space();
extern void switch_m2u_mode();
uint32_t NOINLINE measure_cpu_freq(size_t n);
uint32_t get_timer_value();
uint32_t get_timer_freq();
uint32_t mtime_lo(void);
uint32_t get_instret_value();
uint32_t get_cycle_value();
uint32_t get_cpu_freq();
extern void Disable_Interrupt_Main_Switch(void);
extern void Enable_Interrupt_Main_Switch(void);
void Default_Config();
#if SPIM_CLOCK_EN
#define spim_ModuleClock_EN SYSCTL_MODEN0 |= SPIM_EN;
#else
#define spim_ModuleClock_EN SYSCTL_MODEN0 &= (~SPIM_EN);
#endif
#if UART1_CLOCK_EN
#define uart1_ModuleClock_EN SYSCTL_MODEN0 |= UART1_EN;
#else
#define uart1_ModuleClock_EN SYSCTL_MODEN0 &= (~UART1_EN);
#endif
#if UART0_CLOCK_EN
#define uart0_ModuleClock_EN SYSCTL_MODEN0 |= UART0_EN;
#else
#define uart0_ModuleClock_EN SYSCTL_MODEN0 &= (~UART0_EN);
#endif
#if UARTB_CLOCK_EN
#define uartb_ModuleClock_EN SYSCTL_MODEN0 |= UARTB_EN;
#else
#define uartb_ModuleClock_EN SYSCTL_MODEN0 &= (~UARTB_EN);
#endif
#if UARTA_CLOCK_EN
#define uarta_ModuleClock_EN SYSCTL_MODEN0 |= UARTA_EN;
#else
#define uarta_ModuleClock_EN SYSCTL_MODEN0 &= (~UARTA_EN);
#endif
#if PWM_CLOCK_EN
#define pwm_ModuleClock_EN SYSCTL_MODEN0 |= PWM_EN;
#else
#define pwm_ModuleClock_EN SYSCTL_MODEN0 &= (~PWM_EN);
#endif
#if ADC_CLOCK_EN
#define adc_ModuleClock_EN SYSCTL_MODEN0 |= ADC_EN;
#else
#define adc_ModuleClock_EN SYSCTL_MODEN0 &= (~ADC_EN);
#endif
#if MAILBOX_CLOCK_EN
#define mailbox_ModuleClock_EN SYSCTL_MODEN0 |= MAILBOX_EN;
#else
#define mailbox_ModuleClock_EN SYSCTL_MODEN0 &= (~MAILBOX_EN);
#endif
#if WDT_CLOCK_EN
#define wdt_ModuleClock_EN SYSCTL_MODEN0 |= WDT_EN;
#else
#define wdt_ModuleClock_EN SYSCTL_MODEN0 &= (~WDT_EN);
#endif
#if SMBUS8_CLOCK_EN
#define smbus8_ModuleClock_EN SYSCTL_MODEN0 |= SMB8_EN;
#else
#define smbus8_ModuleClock_EN SYSCTL_MODEN0 &= (~SMB8_EN);
#endif
#if SMBUS7_CLOCK_EN
#define smbus7_ModuleClock_EN SYSCTL_MODEN0 |= SMB7_EN;
#else
#define smbus7_ModuleClock_EN SYSCTL_MODEN0 &= (~SMB7_EN);
#endif
#if SMBUS6_CLOCK_EN
#define smbus6_ModuleClock_EN SYSCTL_MODEN0 |= SMB6_EN;
#else
#define smbus6_ModuleClock_EN SYSCTL_MODEN0 &= (~SMB6_EN);
#endif
#if SMBUS5_CLOCK_EN
#define smbus5_ModuleClock_EN SYSCTL_MODEN1 |= SMB5_EN;
#else
#define smbus5_ModuleClock_EN SYSCTL_MODEN1 &= (~SMB5_EN);
#endif
#if SMBUS4_CLOCK_EN
#define smbus4_ModuleClock_EN SYSCTL_MODEN1 |= SMB4_EN;
#else
#define smbus4_ModuleClock_EN SYSCTL_MODEN1 &= (~SMB4_EN);
#endif
#if SMBUS3_CLOCK_EN
#define smbus3_ModuleClock_EN SYSCTL_MODEN0 |= SMB3_EN;
#else
#define smbus3_ModuleClock_EN SYSCTL_MODEN0 &= (~SMB3_EN);
#endif
#if SMBUS2_CLOCK_EN
#define smbus2_ModuleClock_EN SYSCTL_MODEN0 |= SMB2_EN;
#else
#define smbus2_ModuleClock_EN SYSCTL_MODEN0 &= (~SMB2_EN);
#endif
#if SMBUS1_CLOCK_EN
#define smbus1_ModuleClock_EN SYSCTL_MODEN0 |= SMB1_EN;
#else
#define smbus1_ModuleClock_EN SYSCTL_MODEN0 &= (~SMB1_EN);
#endif
#if SMBUS0_CLOCK_EN
#define smbus0_ModuleClock_EN SYSCTL_MODEN0 |= SMB0_EN;
#else
#define smbus0_ModuleClock_EN SYSCTL_MODEN0 &= (~SMB0_EN);
#endif
#if GPIO_CLOCK_EN
#define gpio_ModuleClock_EN SYSCTL_MODEN0 |= GPIO_EN;
#else
#define gpio_ModuleClock_EN SYSCTL_MODEN0 &= (~GPIO_EN);
#endif
#if BRAM_CLOCK_EN
#define bram_ModuleClock_EN SYSCTL_MODEN0 |= BRAM_EN;
#else
#define bram_ModuleClock_EN SYSCTL_MODEN0 &= (~BRAM_EN);
#endif
#if SWUC_CLOCK_EN
#define swuc_ModuleClock_EN SYSCTL_MODEN0 |= SWUC_EN;
#else
#define swuc_ModuleClock_EN SYSCTL_MODEN0 &= (~SWUC_EN);
#endif
#if SHM_CLOCK_EN
#define shm_ModuleClock_EN SYSCTL_MODEN0 |= SHM_EN;
#else
#define shm_ModuleClock_EN SYSCTL_MODEN0 &= (~SHM_EN);
#endif
#if PMCKBC_CLOCK_EN
#define pmckbc_ModuleClock_EN SYSCTL_MODEN0 |= PMCKBC_EN;
#else
#define pmckbc_ModuleClock_EN SYSCTL_MODEN0 &= (~PMCKBC_EN);
#endif
#if PS2_0_CLOCK_EN
#define ps2_0_ModuleClock_EN SYSCTL_MODEN0 |= PS2_0_EN;
#else
#define ps2_0_ModuleClock_EN SYSCTL_MODEN0 &= (~PS2_0_EN);
#endif
#if PS2_1_CLOCK_EN
#define ps2_1_ModuleClock_EN SYSCTL_MODEN0 |= PS2_1_EN;
#else
#define ps2_1_ModuleClock_EN SYSCTL_MODEN0 &= (~PS2_1_EN);
#endif
#if KBS_CLOCK_EN
#define kbs_ModuleClock_EN SYSCTL_MODEN0 |= KBS_EN;
#else
#define kbs_ModuleClock_EN SYSCTL_MODEN0 &= (~KBS_EN);
#endif
#if TIMER3_CLOCK_EN
#define timer3_ModuleClock_EN SYSCTL_MODEN0 |= TMR3_EN;
#else
#define timer3_ModuleClock_EN SYSCTL_MODEN0 &= (~TMR3_EN);
#endif
#if TIMER2_CLOCK_EN
#define timer2_ModuleClock_EN SYSCTL_MODEN0 |= TMR2_EN;
#else
#define timer2_ModuleClock_EN SYSCTL_MODEN0 &= (~TMR2_EN);
#endif
#if TIMER1_CLOCK_EN
#define timer1_ModuleClock_EN SYSCTL_MODEN0 |= TMR1_EN;
#else
#define timer1_ModuleClock_EN SYSCTL_MODEN0 &= (~TMR1_EN);
#endif
#if I3C0_CLOCK_EN
#define i3c0_ModuleClock_EN SYSCTL_MODEN0 |= I3C0_EN;
#else
#define i3c0_ModuleClock_EN SYSCTL_MODEN0 &= (~I3C0_EN);
#endif
#if I3C1_CLOCK_EN
#define i3c1_ModuleClock_EN SYSCTL_MODEN0 |= I3C1_EN;
#else
#define i3c1_ModuleClock_EN SYSCTL_MODEN0 &= (~I3C1_EN);
#endif
#if I3C2_CLOCK_EN
#define i3c2_ModuleClock_EN SYSCTL_MODEN0 |= I3C2_EN;
#else
#define i3c2_ModuleClock_EN SYSCTL_MODEN0 &= (~I3C2_EN);
#endif
#if I3C3_CLOCK_EN
#define i3c3_ModuleClock_EN SYSCTL_MODEN0 |= I3C3_EN;
#else
#define i3c3_ModuleClock_EN SYSCTL_MODEN0 &= (~I3C3_EN);
#endif
#if TIMER0_CLOCK_EN
#define timer0_ModuleClock_EN SYSCTL_MODEN1 |= TMR0_EN;
#else
#define timer0_ModuleClock_EN SYSCTL_MODEN1 &= (~TMR0_EN);
#endif
#if ICTL_CLOCK_EN
#define intc_ModuleClock_EN SYSCTL_MODEN1 |= ICTL_EN;
#else
#define intc_ModuleClock_EN SYSCTL_MODEN1 &= (~ICTL_EN);
#endif
#if H2E_CLOCK_EN
#define h2e_ModuleClock_EN SYSCTL_MODEN1 |= H2E_EN;
#else
#define h2e_ModuleClock_EN SYSCTL_MODEN1 &= (~H2E_EN);
#endif
#if SPIFE_CLOCK_EN
#define spif_ModuleClock_EN SYSCTL_MODEN1 |= SPIFE_EN;
#else
#define spif_ModuleClock_EN SYSCTL_MODEN1 &= (~SPIFE_EN);
#endif
#if SRAM_CLOCK_EN
#define sram_ModuleClock_EN SYSCTL_MODEN1 |= SRAM_EN;
#else
#define sram_ModuleClock_EN SYSCTL_MODEN1 &= (~SRAM_EN);
#endif
#if GPIODB_CLOCK_EN
#define gpiodb_ModuleClock_EN SYSCTL_MODEN1 |= GPIODB_EN;
#else
#define gpiodb_ModuleClock_EN SYSCTL_MODEN1 &= (~GPIODB_EN);
#endif
#if SYSCTL_CLOCK_EN
#define sysctl_ModuleClock_EN SYSCTL_MODEN1 |= SYSCTL_EN;
#else
#define sysctl_ModuleClock_EN SYSCTL_MODEN1 &= (~SYSCTL_EN);
#endif
#if DRAM_CLOCK_EN
#define dram_ModuleClock_EN SYSCTL_MODEN1 |= DRAM_EN;
#else
#define dram_ModuleClock_EN SYSCTL_MODEN1 &= (~DRAM_EN);
#endif
#if APB_CLOCK_EN
#define apb_ModuleClock_EN SYSCTL_MODEN1 |= APB_EN;
#else
#define apb_ModuleClock_EN SYSCTL_MODEN1 &= (~APB_EN);
#endif
#if CEC_CLOCK_EN
#define cec_ModuleClock_EN SYSCTL_MODEN1 |= CEC_EN;
#else
#define cec_ModuleClock_EN SYSCTL_MODEN1 &= (~CEC_EN);
#endif
#if RTC_CLOCK_EN
#define rtc_ModuleClock_EN SYSCTL_MODEN1 |= RTC_EN;
#else
#define rtc_ModuleClock_EN SYSCTL_MODEN1 &= (~RTC_EN);
#endif
#if PECI_CLOCK_EN
#define peci_ModuleClock_EN SYSCTL_MODEN1 |= PECI_EN;
#else
#define peci_ModuleClock_EN SYSCTL_MODEN1 &= (~PECI_EN);
#endif
#if ESPI_CLOCK_EN
#define espi_ModuleClock_EN SYSCTL_MODEN1 |= ESPI_EN;
#else
#define espi_ModuleClock_EN SYSCTL_MODEN1 &= (~ESPI_EN);
#endif
#endif
