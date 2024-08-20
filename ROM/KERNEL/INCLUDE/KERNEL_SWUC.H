/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:28:55
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
#ifndef __KERNEL_SWUC_H_
#define __KERNEL_SWUC_H_
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_PNP.H"
/**事件中断宏**/
#define RI1_EC_IRQ   HEX01
#define RI2_EC_IRQ   HEX02
#define RING_EC_IRQ  HEX08
#define SIRQ_EC_IRQ  HEX40
#define MIRQ_EC_IRQ  HEX80
#define DISABLE      HEX00
#define ENABLE       HEX01
/**传统模式或ACPI模式唤醒中断宏**/
#define RTC_IRQ      HEX01
#define ACPI_S1      HEX02
#define ACPI_S2      HEX04
#define ACPI_S3      HEX08
#define ACPI_S4      HEX10
#define ACPI_S5      HEX20
#define PWRBT        HEX40
#define PWRSLY       HEX80

#define SWUC_Int_Status(type)	((SWUSTA & (0x1<<type)) != 0)
void SWUC_Int_Enable(BYTE  type);
void SWUC_Int_Disable(BYTE  type);
BYTE  SWUC_Int_Enable_Read(BYTE  type);
void SWUC_PNP_Config(void);
void SWUC_EventIRQ_Config(BYTE  irq_type,BYTE  sw);
void SWUC_RI_Config(void);
void SWUC_RING_Config(void);
void SWUC_SW_WakeUp_Config(uint8_t hsecm_ec);
void SWUC_ACPIIRQ_Config(BYTE  irq_type,BYTE  sw);
void SWUC_PWUREQ_Config(void);
void SWUC_SMI_Config(void);
void SWUC_KBRST_Config(void);
void SWUC_G20_Config(void);
/*****测试代码*****/
void SWUC_UART_Test(void);
void SWUC_RI1_RI2_Test(void);
void SWUC_Ring_Test(void);
void SWUC_Soft_Test(void);
void SWUC_LegacyMode_Test(void);
void SWUC_ACPI_Init(void);
void SWUC_PWUREQ_Init(void);
void SWUC_SMI_Init(void);
void SWUC_KBRST_Init(uint8_t mode);
void SWUC_GA20_Init(uint8_t mode);
#endif