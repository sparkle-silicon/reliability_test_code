/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-10-30 23:23:04
 * @Description:
 *
 *
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 以下是中文及英文版权同步声明，编码为UTF-8。
 * Copyright has legal effects and violations will be prosecuted.
 * 版权具有法律效力，违反必究。
 *
 * Copyright ©2021-2025 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2025龙晶石半导体科技（苏州）有限公司
 */
#ifndef KERNEL_SOC_FUNC_H
#define KERNEL_SOC_FUNC_H
#include "AE_GLOBAL.H"

/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
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
extern void Module_init(void);
extern void exit(int __status);
#endif
