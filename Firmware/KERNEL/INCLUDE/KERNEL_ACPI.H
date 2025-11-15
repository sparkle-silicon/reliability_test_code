/*
 * @Author: Iversu
 * @LastEditors: daweslinyu 
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
#ifndef KERNEL_ACPI_H
#define KERNEL_ACPI_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
#define T_Burst_Loop 250

/*-----------------------------------------------------------------------------
 * CUSTOM SCI/SMI Event Code Define
 *---------------------------------------------------------------------------*/
#define _SCIEVT_NULL 0x00    // Can't use
#define _SCIEVT_01H 0x01     // Reserved
#define _SCIEVT_02H 0x02     // Reserved
#define _SCIEVT_03H 0x03     // Reserved
#define _SCIEVT_04H 0x04     // Reserved
#define _SCIEVT_05H 0x05     // Reserved
#define _SCIEVT_BATTERY 0x06 // Battery status changed
#define _SCIEVT_ADAPTER 0x07 // Adapter status changed
#define _SCIEVT_08H 0x08     // Reserved
#define _SCIEVT_09H 0x09     // Reserved
#define _SCIEVT_FnF1 0x0A    // Hotkey Fn+ F1 Event
#define _SCIEVT_FnF2 0x0B    // Hotkey Fn+ F2 Event
#define _SCIEVT_FnF3 0x0C    // Hotkey Fn+ F3 Event
#define _SCIEVT_FnF4 0x0D    // Hotkey Fn+ F4 Event
#define _SCIEVT_FnF5 0x0E    // Hotkey Fn+ F5 Event
#define _SCIEVT_FnF6 0x0F    // Hotkey Fn+ F6 Event
#define _SCIEVT_FnF7 0x10    // Hotkey Fn+ F7 Event
#define _SCIEVT_FnF8 0x11    // Hotkey Fn+ F8 Event
#define _SCIEVT_FnF9 0x12    // Hotkey Fn+ F9 Event
#define _SCIEVT_FnF10 0x13   // Hotkey Fn+F10 Event
#define _SCIEVT_FnF11 0x14   // Hotkey Fn+F11 Event
#define _SCIEVT_FnF12 0x15   // Hotkey Fn+F12 Event
#define _SCIEVT_SLEEP 0x31
#define _SCIEVT_LCD 0x32

/*-----------------------------------------------------------------------------
 * 标志位
 *---------------------------------------------------------------------------*/
#define PMC_OBF                 0
#define PMC_IBF                 1
#define PMC_GPF                 2
#define PMC_CMD                 3
#define BURST_Mode              4
#define SCI_EVENT               5
#define SMI_EVENT               6


#define PMC1_Int_Enable(type) (PMC1_CTL |= (0x1 << (type)))
#define PMC2_Int_Enable(type) (PMC2_CTL |= (0x1 << (type)))
#define PMC3_Int_Enable(type) (PMC3_CTL |= (0x1 << (type)))
#define PMC4_Int_Enable(type) (PMC4_CTL |= (0x1 << (type)))
#define PMC5_Int_Enable(type) (PMC5_CTL |= (0x1 << (type)))
#define PMC1_Int_Disable(type) (PMC1_CTL &= ~(0x1 << (type)))
#define PMC2_Int_Disable(type) (PMC2_CTL &= ~(0x1 << (type)))
#define PMC3_Int_Disable(type) (PMC3_CTL &= ~(0x1 << (type)))
#define PMC4_Int_Disable(type) (PMC4_CTL &= ~(0x1 << (type)))
#define PMC5_Int_Disable(type) (PMC5_CTL &= ~(0x1 << (type)))
#define PMC1_Int_Enable_Read(type) ((PMC1_CTL & (0x1 << (type))) != 0)
#define PMC2_Int_Enable_Read(type) ((PMC2_CTL & (0x1 << (type))) != 0)
#define PMC3_Int_Enable_Read(type) ((PMC3_CTL & (0x1 << (type))) != 0)
#define PMC4_Int_Enable_Read(type) ((PMC4_CTL & (0x1 << (type))) != 0)
#define PMC5_Int_Enable_Read(type) ((PMC5_CTL & (0x1 << (type))) != 0)
/*-----------------------------------------------------------------------------
 * Code Parameter
 *---------------------------------------------------------------------------*/
#define _PM1_STEP_1 1
#define _PM1_STEP_2 2
#define _PM1_STEP_3 3
#define _PM1_STEP_4 4
#define _PM1_STEP_5 5
#define _PM1_STEP_6 6
#define _PM1_STEP_7 7
#define QeventSCI 0x01
#define SCIMode_Normal 0x00 // For CUSTOM SCI event
#define SCIMode_Pending 0x01
#define SCIMode_Force 0x02
#define SCIMode_ACPI_SMBus 0x03
/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
#define EVENT_BUFFER_SIZE 9
#define PENDING_BUFFER 8
extern VBYTE SCI_Event_Buffer[10];
extern VBYTE SMI_Event_Buffer[10];
extern VBYTE PD_SCI_Event_Buffer[10];
extern void Service_PCI2(void);
extern void Service_PCI2_Main(void);
extern void EC6266_Cmd_80(void);
extern void EC6266_Cmd_81(void);
extern void EC6266_Cmd_82(void);
extern void EC6266_Cmd_83(void);
extern void EC6266_Cmd_84(void);
extern void EC_Cmd_8X(void);
extern void Service_Event_Center(void);
void SCI_Response(BYTE sci_count);
extern void SCI_Interrupt(void);
extern void SMI_Interrupt(void);
extern void Loop_Delay(BYTE delay);
extern BYTE Read_SCI_Query_Value(void);
extern BYTE Write_SCI_Query_Value(BYTE NewSCI);
extern BYTE Read_SMI_Query_Value(void);
extern BYTE Write_SMI_Query_Value(BYTE NewSMI);
extern void Clear_Event_Buffer(void);
extern int CheckBurstMode(void); // zb 10.26 add
extern void vSetTotalBurstTime(void);
extern void ACPI_SMB_Via_EC(void);
/*-----------------------------------------------------------------------------
 * Internal Calling Prototype
 *---------------------------------------------------------------------------*/
extern void Gen_EC_QEvent(BYTE sci_number, BYTE sci_mode);
extern void EC_QEvent_Suspend(BYTE sci_number);
extern void Release_EC_QEvent_Suspend(void);
extern void Write_Ext_RAMSpace(void);
extern void Read_Ext_RAMSpace(void);
extern BYTE Read_Map_ECSPACE_BASE_ADDR(BYTE MapIndex);
extern void Write_Map_ECSPACE_BASE_ADDR(BYTE MapIndex, BYTE data1);
extern void SCI_Send(void);
//-----------------------------------------------------------------------------
#include "CUSTOM_PMC1.H"
#include "CUSTOM_PMC2.H"
#include "CUSTOM_PMC3.H"
#include "CUSTOM_PMC4.H"
#include "CUSTOM_PMC5.H"
#include "CUSTOM_GPIO.H"
#endif
