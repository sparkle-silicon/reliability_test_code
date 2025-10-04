/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-04 16:24:38
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
#ifndef CUSTOM_INIT_H
#define CUSTOM_INIT_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "AE_INIT.H"
#include "KERNEL_MEMORY.H"
#include "CUSTOM_FAN.H"
#include "AE_FLASHINFO.H"
#define PWRSW_RSTOEN 0x100
#define PWRSW_DBTIMEL_64ms 0x40
#define PWRSW_DBTIMEL_96ms 0x80
#define PWRSW_DBTIMEL_1000ms 0xC0
#define PWRSW_INT_WDT 0x20
#define PWRSW_RSTMODE 0x10
#define PWRSW_WDTIME_12000ms 0x0E
#define PWRSW_WDTIME_10000ms 0x0C
#define PWRSW_WDTIME_8000ms 0x0A
#define PWRSW_WDTIME_7000ms 0x08
#define PWRSW_WDTIME_6000ms 0x06
#define PWRSW_WDTIME_5000ms 0x04
#define PWRSW_WDTIME_4000ms 0x02
#define PWRSW_WDTIME_1000ms 0x0
#define PWRSW_EN 0x1
extern FUNCT_PTR_V_V Get_DoubleBoot_ADDR(void);
extern void Default_Config();
extern void Device_init(void);
#define ADDR_OR_ID(addr, id) ((addr) | (((id)&0xfff) << 20))
#endif
