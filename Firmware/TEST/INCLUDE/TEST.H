/*
 * @Author: Maple
 * @LastEditors: Maple
 * @LastEditTime: 2023-04-02 21:37:22
 * @Description: 该文件没有实际作用，仅仅用来测试
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
#ifndef _TEST_H_
#define _TEST_H_
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
#include "TEST_CONFIG.H"
#define IBF HEX00
#define OBF HEX01
/*函数声明*/
void SCI_test(void);
void SMI_test(void);
void SWUC_Test(void);
void SYSCTL_PORINT_Test(void);
#endif