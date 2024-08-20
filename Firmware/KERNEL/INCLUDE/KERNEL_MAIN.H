/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-10-09 15:09:08
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
#ifndef KERNEL_MAIN_H
#define KERNEL_MAIN_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
extern VBYTE *Tmp_XPntr;
extern VBYTE *Tmp_XPntr1;
extern void Handle_Mouse(BYTE channel);
extern void Service_PCI3_Main(void);
extern void Service_PCI4_Main(void);
extern void Service_PCI5_Main(void);
extern void Service_PCI6_Main(void);
extern void KBC_PMC_DataPending(char *KBC_PMC_PendingData);
extern void GetKBC_PMC_PendingData(void);
extern void Specific_Mem_init(void);
//-----------------------------------------------------------------------------
#endif
