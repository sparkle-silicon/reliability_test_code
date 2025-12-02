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
 /*-----------------------------------------------------------------------------
  * Low Power Mode Define
  *---------------------------------------------------------------------------*/
#define SUPPORT_DEEPSLEEP
  /*-----------------------------------------------------------------------------
   * Wakeup Source Define
   *---------------------------------------------------------------------------*/
#define SUPPORT_GPIO_WAKEUP 1     // GPIO中断唤醒
#define SUPPORT_RTC_WAKEUP 0      // RTC定时器唤醒
#define SUPPORT_PWRSW_WAKEUP 0    // PWRSW唤醒
#define SUPPORT_KBS_WAKEUP 0      // 键盘按键唤醒
#define Hibernation_Wakeup_GPIO Hibernation_Wakeup_GPIOA11
   /*-----------------------------------------------------------------------------
    * Hibernation_Wakeup pin define
    *---------------------------------------------------------------------------*/
#define Hibernation_Wakeup_GPIOA11 0x2
#define Hibernation_Wakeup_GPIOB4  0x4
#define Hibernation_Wakeup_GPIOB17 0x8
    /*-----------------------------------------------------------------------------
     * External Calling Prototype
     *---------------------------------------------------------------------------*/
extern void Module_SoftReset(int, int);
extern void Enter_LowPower_Mode(void);
extern void Exit_LowPower_Mode(void);
extern void CHIP_Sleep(void);
extern void CHIP_Deep_Sleep(void);
extern void CHIP_Hibernation(void);
extern void Service_LowPower(void);
extern void Reset_ALG_TRNG_AfterWakeup(void);
#endif