/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-09-18 13:57:34
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
#ifndef CUSTOM_PMC1_H
#define CUSTOM_PMC1_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
#include "KERNEL_ACPI.H"
#include "KERNEL_PORT6064.H"
#include "KERNEL_MAIN.H"
#define OBF1 BIT(0x00)
#define IBF1 BIT(0x01)
#define Busy BIT(0x02)
#define C_D1 BIT(0x03)
#define BURST BIT(0x04)
#define SCIEVT BIT(0x05)
#define SMIEVT BIT(0x06)
/*--------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern BYTE OEM_Get_Port62_Data(void);
/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/
void EC_Cmd_0X(void);
void EC_Cmd_1X(void);
void EC_Cmd_2X(void);
void EC_Cmd_3X(void);
void EC_Cmd_4X(void);
void EC_Cmd_5X(void);
void EC_Cmd_6X(void);
void EC_Cmd_7X(void);
void EC_Cmd_9X(void);
void EC_Cmd_AX(void);
void EC_Cmd_BX(void);
void EC_Cmd_CX(void);
void EC_Cmd_DX(void);
void EC_Cmd_EX(void);
void EC_Cmd_FX(void);
void EC6266_Cmd_85(void);
void EC6266_Cmd_86(void);
void EC6266_Cmd_87(void);
void EC6266_Cmd_88(void);
void EC6266_Cmd_89(void);
void EC6266_Cmd_8A(void);
void EC6266_Cmd_8B(void);
void EC6266_Cmd_8C(void);
void EC6266_Cmd_8D(void);
void EC6266_Cmd_8E(void);
void EC6266_Cmd_8F(void);
void EC62_Data_Step0(void);
void EC62_Data_Step4(void);
void EC62_Data_Step5(void);
void EC62_Data_Step6(void);
void EC62_Data_Step7(void);
void EC6266_Cmd_90(void);
void EC6266_Cmd_91(void);
void EC6266_Cmd_92(void);
void EC6266_Cmd_93(void);
void EC6266_Cmd_94(void);
void EC6266_Cmd_95(void);
void EC6266_Cmd_96(void);
void EC6266_Cmd_97(void);
void EC6266_Cmd_98(void);
void EC6266_Cmd_99(void);
void EC6266_Cmd_9A(void);
void EC6266_Cmd_9B(void);
void EC6266_Cmd_9C(void);
void EC6266_Cmd_9D(void);
void EC6266_Cmd_9E(void);
void EC6266_Cmd_9F(void);
void EC6266_Cmd_A0(void);
void EC6266_Cmd_A1(void);
void EC6266_Cmd_A2(void);
void EC6266_Cmd_A3(void);
void EC6266_Cmd_A4(void);
void EC6266_Cmd_A5(void);
void EC6266_Cmd_A6(void);
void EC6266_Cmd_A7(void);
void EC6266_Cmd_A8(void);
void EC6266_Cmd_A9(void);
void EC6266_Cmd_AA(void);
void EC6266_Cmd_AB(void);
void EC6266_Cmd_AC(void);
void EC6266_Cmd_AD(void);
void EC6266_Cmd_AE(void);
void EC6266_Cmd_AF(void);
void EC6266_Cmd_B0(void);
void EC6266_Cmd_B1(void);
void EC6266_Cmd_B2(void);
void EC6266_Cmd_B3(void);
void EC6266_Cmd_B4(void);
void EC6266_Cmd_B5(void);
void EC6266_Cmd_B6(void);
void EC6266_Cmd_B7(void);
void EC6266_Cmd_B8(void);
void EC6266_Cmd_B9(void);
void EC6266_Cmd_BA(void);
void EC6266_Cmd_BB(void);
void EC6266_Cmd_BC(void);
void EC6266_Cmd_BD(void);
void EC6266_Cmd_BE(void);
void EC6266_Cmd_BF(void);
void EC6266_Cmd_C0(void);
void EC6266_Cmd_C1(void);
void EC6266_Cmd_C2(void);
void EC6266_Cmd_C3(void);
void EC6266_Cmd_C4(void);
void EC6266_Cmd_C5(void);
void EC6266_Cmd_C6(void);
void EC6266_Cmd_C7(void);
void EC6266_Cmd_C8(void);
void EC6266_Cmd_C9(void);
void EC6266_Cmd_CA(void);
void EC6266_Cmd_CB(void);
void EC6266_Cmd_CC(void);
void EC6266_Cmd_CD(void);
void EC6266_Cmd_CE(void);
void EC6266_Cmd_CF(void);
void EC6266_Cmd_D0(void);
void EC6266_Cmd_D1(void);
void EC6266_Cmd_D2(void);
void EC6266_Cmd_D3(void);
void EC6266_Cmd_D4(void);
void EC6266_Cmd_D5(void);
void EC6266_Cmd_D6(void);
void EC6266_Cmd_D7(void);
void EC6266_Cmd_D8(void);
void EC6266_Cmd_D9(void);
void EC6266_Cmd_DA(void);
void EC6266_Cmd_DB(void);
void EC6266_Cmd_DC(void);
void EC6266_Cmd_DD(void);
void EC6266_Cmd_DE(void);
void EC6266_Cmd_DF(void);
void EC6266_Cmd_E0(void);
void EC6266_Cmd_E1(void);
void EC6266_Cmd_E2(void);
void EC6266_Cmd_E3(void);
void EC6266_Cmd_E4(void);
void EC6266_Cmd_E5(void);
void EC6266_Cmd_E6(void);
void EC6266_Cmd_E7(void);
void EC6266_Cmd_E8(void);
void EC6266_Cmd_E9(void);
void EC6266_Cmd_EA(void);
void EC6266_Cmd_EB(void);
void EC6266_Cmd_EC(void);
void EC6266_Cmd_ED(void);
void EC6266_Cmd_EE(void);
void EC6266_Cmd_EF(void);
void EC6266_Cmd_F0(void);
void EC6266_Cmd_F1(void);
void EC6266_Cmd_F2(void);
void EC6266_Cmd_F3(void);
void EC6266_Cmd_F4(void);
void EC6266_Cmd_F5(void);
void EC6266_Cmd_F6(void);
void EC6266_Cmd_F7(void);
void EC6266_Cmd_F8(void);
void EC6266_Cmd_F9(void);
void EC6266_Cmd_FA(void);
void EC6266_Cmd_FB(void);
void EC6266_Cmd_FC(void);
void EC6266_Cmd_FD(void);
void EC6266_Cmd_FE(void);
void EC6266_Cmd_FF(void);
//-----------------------------------------------------------------------------
#include "AE_DEBUGGER.H"
#endif
