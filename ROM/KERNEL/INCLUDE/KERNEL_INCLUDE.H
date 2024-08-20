/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-07-19 14:23:23
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
#ifndef KERNEL_INCLUDE_H
#define KERNEL_INCLUDE_H
#include "AE_GLOBAL.H"//全局变量
#include "AE_CONFIG.H"
//-----------------------------------------------------------------------------
#include "KERNEL_MAIN.H"
//-----------------------------------------------------------------------------
#include "KERNEL_SOC_FUNC.H"
#include "KERNEL_MEMORY.H"
#include "KERNEL_INIT.H"
#include "KERNEL_SPIF.H"
#include "KERNEL_SLEEP_RESET.H"
#include "KERNEL_I2C.H"
#include "KERNEL_WATCHDOG.H"
#include "KERNEL_PWM.H"
#include "KERNEL_GPIO.H"
#include "KERNEL_UART.H"
#ifdef AE103
#include "KERNEL_SMx.H"
#endif
//-----------------------------------------------------------------------------
#endif
