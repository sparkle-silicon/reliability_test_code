/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-10-22 18:10:32
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
#ifndef KERNEL_HOST_H
#define KERNEL_HOST_H
#include "AE_GLOBAL.H"
#include "KERNEL_PORT6064.H"

/*-----------------------------------------------------------------------------
 * KBC_STA Flag
 *---------------------------------------------------------------------------*/
#define KBC_OBF                   0x00 //Output Buffer Full
#define KBC_IBF                   0x01 //Input Buffer Full
#define KBC_GPF                   0x02 //System Flag
#define KBC_CMD                   0x03 //Command or Data
#define KBC_KL                    0x04 //May be "keyboard lock" (more likely unused on modern systems)
#define KBC_SAOBF                 0x05 //May be "receive time-out" or "second PS/2 port output buffer full"
#define KBC_TOE                   0x06 //Timeout error
#define KBC_PE                    0x07 //Parity error
/*-----------------------------------------------------------------------------
 * KBC_CTL Flag
 *---------------------------------------------------------------------------*/
#define KBC_OBFKIE                0x00 // When this bit is set to 1, generates keyboard SERIRQ (IRQ1) when the KBC output buffer is full
#define KBC_OBFMIE                0x01 // When this bit is set to 1, generates mouse SERIRQ (IRQ12) when the KBC output buffer is full
#define KBC_OBEIE                 0x02 // Data output register empty interrupt enable
#define KBC_IBFIE                 0x03 // Data input register full interrupt enable
/*-----------------------------------------------------------------------------
 * PMC Flag
 *---------------------------------------------------------------------------*/
#define PM1_HOST_INT_ENABLE 0x10 // P143
#define IBF_INT_ENABLE 0x1       // P157
#define OBE_INT_ENABLE 0x2
#define OBF_KEYBOARD_INT_ENABLE 0x1 // P143
#define OBF_MOUSE_INT_ENABLE 0x2    // P143
/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
extern void Service_Send(void);
extern BYTE Send_KB_Data_To_Host(BYTE nKBData);
extern void KBC_Data_Suspend(BYTE nPending);
extern BYTE Release_KBC_Data_Suspend(void);
extern void Transmit_Data_To_Host(BYTE data_byte);
extern void Mouse_Data_To_Host(BYTE data_byte);
extern void KBC_Init(void);
extern void PMC_Init(void);
/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/
 //-----------------------------------------------------------------------------

#endif
