/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-20 16:46:47
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
#ifndef AE_INIT_H
#define AE_INIT_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "AE_FUNC.H"
#if TEST
#include "TEST_SOC_FUNC.H"
#else
#include "KERNEL_SOC_FUNC.H"
#endif
#include "CUSTOM_INIT.H"
#define DoubleBoot_DEF_ADDR 0xA0084

#define AE_UPDATE_NO_INIT_FUNCTION {if(SYSCTL_CFG & BIT3) {\
set_csr(mstatus, MSTATUS_MIE); return; \
}};
extern void exit(int __status);
#endif