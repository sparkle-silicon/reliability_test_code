/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:26:53
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
#ifndef KERNEL_INIT_H
#define KERNEL_INIT_H
#include "AE_GLOBAL.H"

/*-----------------------------------------------------------------------------
 * Data Parameter definition
 *---------------------------------------------------------------------------*/
#if 1
#define EXT_CB2_INIT        0x22
#define EXT_CB3_INIT        0x01
#define RAM_INIT_WAKE1      0x00
#define RAM_INIT_WAKE2      0x00
#define RAM_INIT_WAKE3      0x40
/*-----------------------------------------------------------------------------
 * Local Data Structure Prototype
 *---------------------------------------------------------------------------*/
struct REG_INIT_DEF
{
    VBYTE   *address;
    BYTE    initdata;
};
/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Internal Process Prototype
 *---------------------------------------------------------------------------*/
#endif
//-----------------------------------------------------------------------------
#endif
