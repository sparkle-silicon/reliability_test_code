/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-10-22 17:42:44
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
#include "AE_INIT.H"
#include "AE_FLASHINFO.H"
#include "CUSTOM_FAN.H"
/* or */
#define ADDR_OR_ID(addr, id) ((addr) | (((id)&0xfff) << 20))
/*DoubleBoot Address*/
#define DoubleBoot_DEF_ADDR 0xA0084
/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
extern FUNCT_PTR_V_V Get_DoubleBoot_ADDR(void);
extern void Default_Config();
extern void Device_init(void);
#endif
