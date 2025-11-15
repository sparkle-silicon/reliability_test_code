/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2023-11-03 12:01:25
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
#ifndef KERNEL_PORT6064_H
#define KERNEL_PORT6064_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "AE_FUNC.H"
#include "KERNEL_HOST.H"
#include "KERNEL_PS2.H"
#include "CUSTOM_KBC6064.H"

#define KBC_CTL_OBFKIE                          HEX01
#define KBC_CTL_OBFMIE                          HEX02
#define KBC_CTL_OBEIE                           HEX04
#define KBC_CTL_IBFIE                           HEX08
#define KBC_CTL_PMC12IE                         HEX10
#define KBC_STA_OBF                             HEX01
#define KBC_STA_IBF                             HEX02
#define KBC_STA_GPF                             HEX04
#define KBC_STA_A2                              HEX08
#define KBC_STA_KL                              HEX10 //May be "keyboard lock" (more likely unused on modern systems)
#define KBC_STA_SAOBF                           HEX20 //May be "receive time-out" or "second PS/2 port output buffer full"
#define KBC_STA_TOE                             HEX40 //Timeout error
#define KBC_STA_PE                              HEX80 //Parity error
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define KBC_Int_Enable(type)	    (KBC_CTL |= (0x1<<(type)))
#define KBC_Int_Disable(type)	    (KBC_CTL &= ~(0x1<<(type)))
#define KBC_Int_Enable_Read(type)	((KBC_CTL & (0x1<<(type)))!=0)
#define KBC_IBF_INTERRUPT_OFFSET 13
/*-----------------------------------------------------------------------------
 * Wait Host Delay Parameter
 *---------------------------------------------------------------------------*/
#define WaitKBDataDelay         0x200
#define WaitECDataDelay         0x200
#define WaitPM2DataDelay        0x200
#define WaitKBCIdle             30000
 /*-----------------------------------------------------------------------------
  * Command Parameter
  *---------------------------------------------------------------------------*/
#define _CMD_BYTE1              0x02
#define _CMD_BYTE2              0x03
#define _CMD_BYTE3              0x04
#define _CMD_BYTE4              0x05

  //KBC调试打印
#if KBC_DEBUG 
#define kbc_dprint dprint
#else 
#define kbc_dprint(fmt, args...)
#endif
  /*-----------------------------------------------------------------------------
   * External Calling Prototype
   *---------------------------------------------------------------------------*/
extern void Service_PCI(void);
extern void KB_Cmd_Handle(BYTE data_byte);
extern void Write_Debug_Data_To_Sram(uint8_t Data);
extern void Write_KCCB(BYTE NewKCCB);
extern BYTE Set_Port60_Data_Handle(void);
extern BYTE Set_Port60_Multi_Data_Handle(void);
extern BYTE Loop_Wait_Get_Port60_Data(void);
extern void Service_PCI_Main(void);
extern void A20GATE_ON(void);
extern void Command_A7(void);
extern void Command_A8(void);
extern void Command_AD(void);
extern void Command_AE(void);
extern void KBC_Variable_Clear(void);
/*-----------------------------------------------------------------------------
 * Internal Process Prototype
 *---------------------------------------------------------------------------*/

#endif
