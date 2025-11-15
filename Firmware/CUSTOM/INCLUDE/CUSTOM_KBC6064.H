/*
 * @Author: Iversu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2023-09-18 13:57:50
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
#ifndef CUSTOM_KBC6064_H
#define CUSTOM_KBC6064_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
#include "KERNEL_PORT6064.H"
#include "KERNEL_MAIN.H"
/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern void Command_80(void);
extern void Command_81(void);
extern void Command_82(void);
extern void Command_83(void);
extern void Command_84(void);
extern void Command_85(void);
extern void Command_86(void);
extern void Command_87(void);
extern void Command_88(void);
extern void Command_89(void);
extern void Command_8A(void);
extern void Command_8B(void);
extern void Command_8C(void);
extern void Command_8D(void);
extern void Command_8E(void);
extern void Command_8F(void);
extern void Command_90(void);
extern void Command_91(void);
extern void Command_92(void);
extern void Command_93(void);
extern void Command_94(void);
extern void Command_95(void);
extern void Command_96(void);
extern void Command_97(void);
extern void Command_98(void);
extern void Command_99(void);
extern void Command_9A(void);
extern void Command_9B(void);
extern void Command_9C(void);
extern void Command_9D(void);
extern void Command_9E(void);
extern void Command_9F(void);
extern void Command_A0(void);
extern void Command_A2(void);
extern void Command_A3(void);
extern void Command_AF(void);
extern void Command_B0(void);
extern void Command_B1(void);
extern void Command_B2(void);
extern void Command_B3(void);
extern void Command_B4(void);
extern void Command_B5(void);
extern void Command_B6(void);
extern void Command_B7(void);
extern void Command_B8(void);
extern void Command_B9(void);
extern void Command_BA(void);
extern void Command_BB(void);
extern void Command_BC(void);
extern void Command_BD(void);
extern void Command_BE(void);
extern void Command_BF(void);
extern void Command_C0(void);
extern void Command_C1(void);
extern void Command_C2(void);
extern void Command_C3(void);
extern void Command_C4(void);
extern void Command_C5(void);
extern void Command_C6(void);
extern void Command_C7(void);
extern void Command_C8(void);
extern void Command_C9(void);
extern void Command_CA(void);
extern void Command_CB(void);
extern void Command_CC(void);
extern void Command_CD(void);
extern void Command_CE(void);
extern void Command_CF(void);
extern void Command_D5(void);
extern void Command_D6(void);
extern void Command_D7(void);
extern void Command_D8(void);
extern void Command_D9(void);
extern void Command_DA(void);
extern void Command_DB(void);
extern void Command_DC(void);
extern void Command_DD(void);
extern void Command_DE(void);
extern void Command_DF(void);
#if SUPPORTED_KBC_EX
extern void Command_E1(void);
extern void Command_E2(void);
extern void Command_E3(void);
extern void Command_E4(void);
extern void Command_E5(void);
extern void Command_E6(void);
extern void Command_E7(void);
extern void Command_E8(void);
extern void Command_E9(void);
extern void Command_EA(void);
extern void Command_EB(void);
extern void Command_EC(void);
extern void Command_ED(void);
extern void Command_EE(void);
extern void Command_EF(void);
#endif
//-----------------------------------------------------------------------------
#endif
