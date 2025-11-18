/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2023-12-07 10:46:23
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
#ifndef CUSTOM_SCANTEBS_H
#define CUSTOM_SCANTEBS_H
#include "AE_GLOBAL.H"
#include "KERNEL_XLATE.H"

extern const BYTE  Cr_KBD_Tables[][8];
extern void Check_KB_Wake_S3(BYTE  KSO, BYTE  KSI);
extern void Check_Send_Key(BYTE  table_entry, BYTE  event);
extern BYTE  Skip_Send_Key(void);
extern  void Process_Sskey3(BYTE  sskey2, BYTE  event);
extern BYTE  Scratch_Get_Index_Comb_BIT6(void);
extern BYTE  Scratch_Get_Index_Comb_BIT7(void);
extern void Scratch_Fn_Key_Break(void);
extern void Scratch_Fn_Key_Make(void);
extern void Scratch_NUMLK_Key_Break(void);
extern void Scratch_NUMLK_Key_Make(void);
extern VBYTE* Tmp_code_pointer;
#endif