/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-18 20:58:39
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
#ifndef KERNEL_TIMER_H
#define KERNEL_TIMER_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"

#define TIMER0 0x0
#define TIMER1 0x1
#define TIMER2 0x2
#define TIMER3 0x3
/************TIMER0常见定时定义************/
#define TIMER0_CLOCK (HIGHT_CHIP_CLOCK / ((SYSCTL_CLKDIV_TMR0+1)<<1))
#define TIMER0_1ms (TIMER0_CLOCK/1000)
#define TIMER0_100us (TIMER0_1ms / 10)
#define TIMER0_50us (TIMER0_1ms / 20)
#define TIMER0_20us (TIMER0_1ms / 50)
#define TIMER0_10us (TIMER0_1ms / 100)
#define TIMER0_5us (TIMER0_1ms / 200)
#define TIMER0_2us (TIMER0_1ms / 500)
#define TIMER0_1us (TIMER0_1ms / 1000)
/************TIMER1常见定时定义************/
#define TIMER1_CLOCK (HIGHT_CHIP_CLOCK / ((SYSCTL_CLKDIV_TMR1+1)<<1))
#define TIMER1_1ms (TIMER1_CLOCK/1000)
#define TIMER1_100us (TIMER1_1ms / 10)
#define TIMER1_50us (TIMER1_1ms / 20)
#define TIMER1_20us (TIMER1_1ms / 50)
#define TIMER1_10us (TIMER1_1ms / 100)
#define TIMER1_5us (TIMER1_1ms / 200)
#define TIMER1_2us (TIMER1_1ms / 500)
#define TIMER1_1us (TIMER1_1ms / 1000)
/************TIMER2常见定时定义************/
// 由于计数器只有16位而timer2频率有24.576Mhz所以延时最长大概4ms左右
#define TIMER2_CLOCK (HIGHT_CHIP_CLOCK / ((SYSCTL_CLKDIV_TMR2+1)<<1))
#define TIMER2_1ms (TIMER2_CLOCK / 1000)
#define TIMER2_100us (TIMER2_1ms / 10)
#define TIMER2_50us (TIMER2_1ms / 20)
#define TIMER2_20us (TIMER2_1ms / 50)
#define TIMER2_10us (TIMER2_1ms / 100)
#define TIMER2_5us (TIMER2_1ms / 200)
#define TIMER2_2us (TIMER2_1ms / 500)
#define TIMER2_1us (TIMER2_1ms / 1000)
/************TIMER3常见定时定义************/
// 由于计数器只有16位而timer2频率有24.576Mhz所以延时最长大概4ms左右
#define TIMER3_CLOCK (HIGHT_CHIP_CLOCK / ((SYSCTL_CLKDIV_TMR3+1)<<1))
#define TIMER3_1ms (TIMER3_CLOCK / 1000)
#define TIMER3_100us (TIMER3_1ms / 10)
#define TIMER3_50us (TIMER3_1ms / 20)
#define TIMER3_20us (TIMER3_1ms / 50)
#define TIMER3_10us (TIMER3_1ms / 100)
#define TIMER3_5us (TIMER3_1ms / 200)
#define TIMER3_2us (TIMER3_1ms / 500)
#define TIMER3_1us (TIMER3_1ms / 1000)
/************TIMER Register define************/
#define TIMER_MASK_EN       HEX04
#define TIMER_MODE_SIGLE    HEX02
#define TIMER_EN            HEX01
#define TIMER_INT           HEX01

extern int TIMER_Disable(u8 index);
extern int TIMER_Init(u8 index, u32 load_count, u8 loop_enable, u8 int_mask);
extern void InternalWDTNow(void);
extern BYTE Timer_Int_Enable(BYTE channel);
extern BYTE Timer_Int_Disable(BYTE channel);
extern BYTE Timer_Int_Enable_Read(BYTE channel);
extern BYTE Timer_Int_Status(BYTE channel);
extern void Timer_Int_Clear(BYTE channel);
extern void udelay(DWORD delay);
extern void vDelayXms(WORD bMS);
extern void vDelayXus(DWORD bUS);
#endif
