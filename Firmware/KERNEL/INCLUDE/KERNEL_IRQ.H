/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-11-06 13:51:50
 * @Description: 中断号越大优先级越高
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
#ifndef KERNEL_IRQ_H_
#define KERNEL_IRQ_H_
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_INCLUDE.H"
#define IRQC_INT_DEVICE_CPUS 0
#define IRQC_INT_DEVICE_CPUT 1
#define IRQC_INT_DEVICE_CPUR 2
#define IRQC_INT_DEVICE_SWUC 3
#define IRQC_INT_DEVICE_LPCRST 4
#define IRQC_INT_DEVICE_PWRSW 5
#define IRQC_INT_DEVICE_PS2_0 6
#define IRQC_INT_DEVICE_KBS_SDV 7
#define IRQC_INT_DEVICE_KBS_PRESS 8
#define IRQC_INT_DEVICE_TACH0 9
#define IRQC_INT_DEVICE_TACH1 10
#define IRQC_INT_DEVICE_TACH2 11
#define IRQC_INT_DEVICE_TACH3 12
#define IRQC_INT_DEVICE_KBC_IBF 13
#define IRQC_INT_DEVICE_KBC_OBE 14
#define IRQC_INT_DEVICE_PMC1_IBF 15
#define IRQC_INT_DEVICE_PMC1_OBE 16
#if (defined(TEST101) || defined(AE101) || defined(AE102))
#define IRQC_INT_DEVICE_PMC2_IBF 17
#define IRQC_INT_DEVICE_PMC2_OBE 18
#elif defined(AE103)
#define IRQC_INT_DEVICE_WU42 17
#define IRQC_INT_DEVICE_RTC 18
#endif
#define IRQC_INT_DEVICE_WDT 19
#define IRQC_INT_DEVICE_ADC 20
#define IRQC_INT_DEVICE_UART0 21
#define IRQC_INT_DEVICE_UARTA 22
#define IRQC_INT_DEVICE_UARTB 23
#define IRQC_INT_DEVICE_SM_EC 24
#define IRQC_INT_DEVICE_SM_HOST 25
#define IRQC_INT_DEVICE_TIMER0 26
#define IRQC_INT_DEVICE_TIMER1 27
#define IRQC_INT_DEVICE_TIMER2 28
#define IRQC_INT_DEVICE_TIMER3 29
#define IRQC_INT_DEVICE_INTR0 30
#define IRQC_INT_DEVICE_INTR1 31
#if IRQC_DEBUG
#define irqprint(fmt, args...) dprint(fmt, ##args)
#else
#define irqprint(fmt, args...)
#endif
extern void cpu_irq_en(void);
extern void Irqc_init();
extern void irqc_enable_interrupt(uint32_t source);
extern void irqc_disable_interrupt(uint32_t source);
extern void irqc_set_pending(uint32_t source);
extern void irqc_clear_pending(uint32_t source);
extern void irqc_set_level_trig(uint32_t source);
extern void irqc_set_posedge_trig(uint32_t source);
extern void irqc_set_negedge_trig(uint32_t source);
void enable_irq(uint32_t source, uint32_t lvl, uint32_t edge);
void disable_irq(uint32_t source, uint32_t lvl, uint32_t edge);
#define CSR_IRQC_CONFIG(IRQC, en_or_dis, lvl, edge) en_or_dis##able_irq(IRQC, lvl, edge); // lvl为0是边沿触发，1是电平触发
extern DWORD kernel_trap(uintptr_t mcause, uintptr_t sp);
extern FUNCT_PTR_V_V Fla_If_Ptr, Smf_Ptr, GLE01_RomCode_Ptr, IVT_Ptr;
#include "AE_DEBUGGER.H"
#endif
