/*
 * @Author: Maple
 * @LastEditors: Maple
 * @LastEditTime: 2023-04-02 21:37:13
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
#ifndef _TEST_TIMEEVENT_
#define _TEST_TIMEEVENT_
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
#include "TEST_CONFIG.H"
#if TEST
void Hook_1msEvent(BYTE EventId);
void Hook_5msEvent(void);
void Hook_10msEvent(void);
void Hook_10msEventA(void);
void Hook_10msEventB(void);
void Hook_50msEventA(void);
void Hook_50msEventB(void);
void Hook_50msEventC(void);
void Hook_100msEventA(void);
void Hook_100msEventB(void);
void Hook_100msEventC(void);
void Hook_500msEventA(void);
void Hook_500msEventB(void);
void Hook_500msEventC(void);
void Hook_1secEventA(void);
void Hook_1secEventB(void);
void Hook_1secEventC(void);
void Hook_1minEvent(void);
#endif
#endif