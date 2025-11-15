/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2024-01-25 17:02:33
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
#ifndef _KERNEL_RTC_H
#define _KERNEL_RTC_H

#include "AE_GLOBAL.H" //全局变量
#include "AE_REG.H"    //寄存器
#include "AE_CONFIG.H"

 // //如果开了预分频计数器，则会预分频跑到
#define RTC_CCR_IEN BIT0//中断使能
#define RTC_CCR_MASK BIT1//中断屏蔽
#define RTC_CCR_EN BIT2//计数器使能
#define RTC_CCR_WEN BIT3//计数器循环使能
#define RTC_CCR_PSCLREN BIT4//预分频使能
extern u32 Get_RTC_CountVal(void);
extern void Set_RTC_MatchVal(u32 count);
extern void Set_RTC_LoadVal(u32 count);
extern void Set_RTC_PreScaler(u32 count);
extern u32 Get_RTC_PrescalerCountVal(void);
extern u32 Get_RTC_Status(void);
extern void RTC_Deep_Sleep_Test(u32 sleep_time);
extern void RTC_Init(u32 loadcnt, u32 match, u32 prescaler);
#endif