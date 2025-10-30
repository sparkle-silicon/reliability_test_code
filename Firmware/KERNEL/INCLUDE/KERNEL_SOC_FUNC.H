/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-30 23:23:04
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
#ifndef KERNEL_SOC_FUNC_H
#define KERNEL_SOC_FUNC_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "AE_FUNC.H"
#include "KERNEL_INCLUDE.H"
#include "CUSTOM_INCLUDE.H"

// Module init function
extern void gpio_init(void);
extern void mailbox_init(void);
extern void uart_init(void);
extern void smbus_init(void);
extern void i3c_init(void);
extern void spi_init(void);
extern void pwm_tach_init(void);
extern void host_init(void);
extern void kbs_init(void);
extern void ps2_init(void);
extern void cec_init(void);
extern void adc_init(void);
extern void time_init(void);
//Init all modules
extern void Module_init(void);
//Exit function
extern void exit(int __status);
#endif
