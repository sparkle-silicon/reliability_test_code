/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-11-03 15:19:07
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
#ifndef KERNEL_MAILBOX_H
#define KERNEL_MAILBOX_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"

extern void mailbox_init(void);
void Mailbox_Control();
void Mailbox_Firmware();
void Mailbox_Efuse();
void Mailbox_eRPMC();
void Mailbox_SecretKey();
void Mailbox_Crypto();
void Mailbox_SecureStorage();
extern void Mailbox_C2E_Service(void);
extern void Mailbox_Read_FLASHID_Trigger(void);
extern void Mailbox_Read_EFUSE_Trigger(void);
extern void Mailbox_FW_Extension_Trigger(void);
extern void Mailbox_APB2_Source_Alloc_Trigger(void);
extern void Mailbox_Test(void);
extern void Mailbox_eRPMC_Trigger(void);
extern void Mailbox_WriteRootKey_Trigger(void);
extern void Mailbox_UpdateHMACKey_Trigger(void);
extern void Mailbox_IncrementCounter_Trigger(void);
extern void Mailbox_RequestCounter_Trigger(void);
#endif