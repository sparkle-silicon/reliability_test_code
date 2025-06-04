/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-06-04 17:56:42
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
#ifndef KERNEL_SOFT_RESET_H
#define KERNEL_SOFT_RESET_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include <KERNEL_RTC.H>
/*--------------------Soft reset module flag-----------------*/
#define ICTL_RST_REG_POS 1
#define ICTL_RST_BIT_POS 6
#define SYSCTL_RST_REG_POS 1
#define SYSCTL_RST_BIT_POS 14
/*--------------------------------------------------------------------------*/
/*--------------------Soft reset Register Address definition-----------------*/
#define Moudle_RESET_REG0_OFFSET        HEX28
#define Moudle_RESET_REG1_OFFSET        HEX2C
#define SRESET_REG_ADDR(offset)         REG_ADDR(SYSCTL_BASE_ADDR,offset)
#define Moudle_RESET_REG0_ADDR          SRESET_REG_ADDR(Moudle_RESET_REG0_OFFSET)
#define Moudle_RESET_REG1_ADDR          SRESET_REG_ADDR(Moudle_RESET_REG1_OFFSET)
#define Moudle_RESET_REG0               REG32(Moudle_RESET_REG0_ADDR)
#define Moudle_RESET_REG1               REG32(Moudle_RESET_REG1_ADDR)
/*--------------------------------------------------------------------------*/
extern void Module_SoftReset(int, int);
extern void CPU_Sleep(void);
extern void Save_Context(void);
extern void Restore_Context(void);
extern void Low_Power_Set(void);
extern void Enter_LowPower_Mode(void);
extern void Exit_LowPower_Mode(void);

extern void CHIP_Hibernation(void);
extern void Test_OSC32K(void);
#endif