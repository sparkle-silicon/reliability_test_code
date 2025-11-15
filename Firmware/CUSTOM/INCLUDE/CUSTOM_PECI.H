/*
 * @Author: Iversu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2025-09-12 15:05:21
 * @Description: This is about the  national crypto algorithm implementation
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
#ifndef CUSTOM_PECI_H
#define CUSTOM_PECI_H
#include "KERNEL_PECI.H"

 //*****************************************************************************
 // Function prototype
 //*****************************************************************************
extern void Service_PECI(void);
extern void Service_PECI_Data(void);
extern void Service_PECI_Command(void);
extern void PECI_Init(void);
extern void PECI_Set_Pad(u16 VTTlev, u8 PADsel);
extern void Calc_CPU_Temperature(void);
extern BYTE PECI_ReadDIB_TEST(void);
extern BYTE PECI_PING_TEST(void);
extern BYTE PECI_ReadTemp_TEST(void);
extern BYTE PECI_RdPkgConfig_TEST(void);
// extern void PECI_Init(u16 VTTlev, u8 PADsel);
// extern void peci_test(void);
#endif