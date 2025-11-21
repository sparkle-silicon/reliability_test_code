/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-10-21 16:35:46
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
#include "KERNEL_RTC.H"
/*low power mode define*/
#define SUPPORT_DEEPSLEEP
/*wakeup define*/
#define SUPPORT_GPIO_WAKEUP 1     // GPIO中断唤醒
#define SUPPORT_RTC_WAKEUP 0      // RTC定时器唤醒
#define SUPPORT_PWRSW_WAKEUP 0    // PWRSW唤醒
#define SUPPORT_KBS_WAKEUP 0      // 键盘按键唤醒
#define Hibernation_Wakeup_GPIO Hibernation_Wakeup_GPIOA11
/*Hibernation_Wakeup pin define*/
#define Hibernation_Wakeup_GPIOA11 0x2
#define Hibernation_Wakeup_GPIOB4  0x4
#define Hibernation_Wakeup_GPIOB17 0x8

/*--------------------Soft reset Register Address definition-----------------*/
#define Module_RESET_REG0_OFFSET        HEX28
#define Module_RESET_REG1_OFFSET        HEX2C
#define SRESET_REG_ADDR(offset)         REG_ADDR(SYSCTL_BASE_ADDR,offset)
#define Module_RESET_REG0_ADDR          SRESET_REG_ADDR(Module_RESET_REG0_OFFSET)
#define Module_RESET_REG1_ADDR          SRESET_REG_ADDR(Module_RESET_REG1_OFFSET)
#define Module_RESET_REG0               REG32(Module_RESET_REG0_ADDR)
#define Module_RESET_REG1               REG32(Module_RESET_REG1_ADDR)
/*--------------------------------------------------------------------------*/
extern void Module_SoftReset(int, int);
extern void Low_Power_Set(void);
extern void Enter_LowPower_Mode(void);
extern void Exit_LowPower_Mode(void);
extern void CHIP_Sleep(void);
extern void CHIP_Deep_Sleep(void);
extern void CHIP_Hibernation(void);
extern void Service_LowPower(void);
#endif