/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-11-09 20:55:26
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
#ifndef __KERNEL_SWUC_H_
#define __KERNEL_SWUC_H_
#include "AE_GLOBAL.H"
#include "KERNEL_PNP.H"
/*-----------------------------------------------------------------------------
 * Event IRQ Type
 *---------------------------------------------------------------------------*/
#define RI1_EC_IRQ   HEX01
#define RI2_EC_IRQ   HEX02
#define RING_EC_IRQ  HEX08
#define SIRQ_EC_IRQ  HEX40
#define MIRQ_EC_IRQ  HEX80
/*-----------------------------------------------------------------------------
 * ACPI Wakeup IRQ Type
 *---------------------------------------------------------------------------*/
#define RTC_IRQ      HEX01
#define ACPI_S1      HEX02
#define ACPI_S2      HEX04
#define ACPI_S3      HEX08
#define ACPI_S4      HEX10
#define ACPI_S5      HEX20
#define PWRBT        HEX40
#define PWRSLY       HEX80
/*-----------------------------------------------------------------------------
 * SWUC Interrupt Status
 *---------------------------------------------------------------------------*/
#define SWUC_Int_Status(type)	((SWUSTA & (0x1<<type)) != 0)
/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
void SWUC_Int_Enable(BYTE  type);
void SWUC_Int_Disable(BYTE  type);
BYTE  SWUC_Int_Enable_Read(BYTE  type);
void SWUC_PNP_Config(void);
void SWUC_EventIRQ_Config(BYTE  irq_type, BYTE  sw);
void SWUC_SW_WakeUp_Config(uint8_t hsecm_ec);
void SWUC_ACPIIRQ_Config(BYTE  irq_type, BYTE  sw);
void SWUC_ACPI_Init(void);
void SWUC_PWUREQ_Init(void);
void SWUC_SMI_Init(void);
void SWUC_KBRST_Init(uint8_t mode);
void SWUC_GA20_Init(uint8_t mode);
extern void SWUC_Init(uint8_t ga20, uint8_t ga20mode, uint8_t kbrst, uint8_t smi, uint8_t pwreq, uint8_t acpi);
#endif