/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:26:30
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
#ifndef __KERNEL_BRAM_H_
#define __KERNEL_BRAM_H_
#include "AE_GLOBAL.H"
#include "KERNEL_PNP.H"

/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
extern void BRAM_Config(uint8_t index_0, uint8_t index_1, uint8_t mask);
extern void BRAM_Read(void);
extern void BRAM_Write(void);
#endif