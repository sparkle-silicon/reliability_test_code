/*
 * @Author: Iversu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2023-11-03 15:19:58
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
#ifndef KERNEL_WATCHDOG_H
#define KERNEL_WATCHDOG_H
#include "AE_GLOBAL.H"

#define WDT_ISR  0x01
#define Wdt_Int_Status ((WDT_STAT & (0x1 << 0)) != 0)
#define Wdt_Int_Status ((WDT_STAT & (0x1 << 0)) != 0)
#define WDT_CRR_CRR 0x76;
#define WDT_FLAG 1 //看门狗喂狗or清中断 1 or 0
/************* WDT Register define************/
#define WDT_CR_RPL_2        HEX00
#define WDT_CR_RPL_4        HEX04
#define WDT_CR_RPL_8        HEX08
#define WDT_CR_RPL_16       HEX0C
#define WDT_CR_RPL_32       HEX10
#define WDT_CR_RPL_64       HEX14
#define WDT_CR_RPL_128      HEX18
#define WDT_CR_RPL_256      HEX1C
#define WDT_CR_RMOD_RESET   HEX00
#define WDT_CR_RMOD_INTR    HEX02
#define WDT_CR_EN           HEX01
#define WDT_CR_DISEN        HEX00

extern void WDT_Clear_IRQ(void);
void WDT_FeedDog(void);
void WDT_Init(BYTE mode, WORD count);
#endif
