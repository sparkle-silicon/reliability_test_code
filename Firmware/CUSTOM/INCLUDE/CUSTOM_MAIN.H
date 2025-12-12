/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:20:43
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
#ifndef CUSTOM_MAIN_H
#define CUSTOM_MAIN_H
#include "AE_GLOBAL.H"

 /*-----------------------------------------------------------------------------
  * External Calling Process
  *---------------------------------------------------------------------------*/
extern void Service_HOST_RST(void);
/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/
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
//-----------------------------------------------------------------------------
#endif
