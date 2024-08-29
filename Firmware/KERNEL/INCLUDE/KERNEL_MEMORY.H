/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-03-06 11:37:17
 * @Description: Memory Definition for extern all code base reference
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
//-------------------------------------------------
// Common Header Include
//-------------------------------------------------
#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
// DRAM BASE ADDRESS DEFINITION
#define MEMORY_BASE0_ADDR DRAM_BASE_ADDR
#define MEMORY_BASE1_ADDR 0x20100UL
#define MEMORY_BASE2_ADDR 0x20600UL
#define MEMORY_BASE3_ADDR 0x20700UL
#define MEMORY_SIZE 0x800UL
/* Please set the maximum value of the new variable.
 * All address must be less than mem set in the link script*/
#define MEMORY_256SIZE DEC256
#define MEMORY_128SIZE DEC128
#define MEMORY_064SIZE DEC64
#define MEMORY_032SIZE DEC32
#define MEMORY_016SIZE DEC16
#define MEMORY_008SIZE HEX08
/*************************************************
 * MEMORY MAP (2K RAM RANGE FOR DEFINITION)
 **************************************************
 *
 *  0x0000-0x00FF : GLOBAL  VARIABLES
 *
 *  0x0100-0x18FF : KERNEL & CHIP LEVEL VATIABLES
 *
 *  0x1900-0x19FF : EC SPACE VARIABLES
 *
 *************************************************/
/****************************************************************
--------------------- MEMORY SECTION 0 --------------------------
------------------- GLOBAL TEMP VARIABLES -----------------------
****************************************************************/
#define Global_Var_BASE MEMORY_BASE0_ADDR
/*Service Flag*/
#define Service_BASE Global_Var_BASE // 0x20000
// Hi-Level Service
#define F_Service_WakeUp (*(VBYTEP)(Service_BASE + 0)) // CPUTIMER_WAKEUP
#define F_Service_PUTC (*(VBYTEP)(Service_BASE + 1))   // Output Character State Variable
#define F_Service_CMD (*(VBYTEP)(Service_BASE + 2))    // UART command support function
#define F_Service_PCI (*(VBYTEP)(Service_BASE + 3))    // KBC相关
// #define     F_Service_Send        (*(VBYTEP)(Service_BASE + 4))
// #define     F_Service_Send_PS2    (*(VBYTEP)(Service_BASE + 5))
#define F_Service_PCI2 (*(VBYTEP)(Service_BASE + 6))    // 62/66 Host Command/Data service
#define F_Service_MS_1 (*(VBYTEP)(Service_BASE + 7))    // 1ms标志，用于添加后续周期服务函数
#define F_Service_KBS (*(VBYTEP)(Service_BASE + 8))     // Service the local keyboard in response to a key press.
#define F_Service_Mailbox (*(VBYTEP)(Service_BASE + 9)) // Mailbox service flag
// Lo-Level Service
#define F_Service_PCI3 (*(VBYTEP)(Service_BASE + 10))      // PMC2 PORT cmd/data process
#define F_Service_PCI4 (*(VBYTEP)(Service_BASE + 11))      // PMC3 PORT cmd/data process
#define F_Service_LPCRST (*(VBYTEP)(Service_BASE + 14))    // Meaning unclear
#define F_Service_PCI5 (*(VBYTEP)(Service_BASE + 15))      // PMC4 PORT cmd/data process
#define F_Service_PCI6 (*(VBYTEP)(Service_BASE + 16))      // PMC5 PORT cmd/data process
#define F_Service_Reserved2 (*(VBYTEP)(Service_BASE + 17)) // Meaning unclear
// debuger service
#define F_Service_Debugger (*(VBYTEP)(Service_BASE + 18))      // debuger service variable
#define F_Service_Debugger_Cnt (*(VBYTEP)(Service_BASE + 19))  // debuger service variable
#define F_Service_Debugger_Cmd (*(VBYTEP)(Service_BASE + 20))  // debuger service variable
#define F_Service_KBL (*(VBYTEP)(Service_BASE + 21))           // debuger service variable
#define F_Service_Debugger_Send (*(VBYTEP)(Service_BASE + 22)) // debuger service variable
#define F_Service_Debugger_Rrq (*(VBYTEP)(Service_BASE + 23))  // debuger service variable
#define F_Service_Reserved3 (*(VBYTEP)(Service_BASE + 24))     // Meaning unclear
#define F_Service_PS2_Handler (*(VBYTEP)(Service_BASE + 25))
// no set
// #define     F_Service_UNLOCK
// #define     F_Service_CIR
// #define     F_Service_OEM_SRVC_3
// #define     F_Service_OEM_SRVC_1
// #define     F_Service_OEM_SRVC_2
/*Global for Debug_Temp variable*/
#define Global_TEMP_BASE (Service_BASE + MEMORY_032SIZE) // 0x20020
// ordinary function temp variable
#define _R1 (*(VBYTEP)(Global_TEMP_BASE + 0)) // main loop service
#define _R2 (*(VBYTEP)(Global_TEMP_BASE + 1)) // global register like i
#define _R3 (*(VBYTEP)(Global_TEMP_BASE + 2)) // global register like j
#define _R4 (*(VBYTEP)(Global_TEMP_BASE + 3)) // kbd xlate KBS_Xlate_Code2
#define _R5 (*(VBYTEP)(Global_TEMP_BASE + 4)) // global register general
#define _R6 (*(VBYTEP)(Global_TEMP_BASE + 5)) // global register general
#define _R7 (*(VBYTEP)(Global_TEMP_BASE + 6)) // global register general
#define _R8 (*(VBYTEP)(Global_TEMP_BASE + 7)) // global register general
// interrupt function temp variable
#define _I1 (*(VBYTEP)(Global_TEMP_BASE + 8))  // interrupt register like i
#define _I2 (*(VBYTEP)(Global_TEMP_BASE + 9))  // interrupt register like j
#define _I3 (*(VBYTEP)(Global_TEMP_BASE + 10)) // interrupt register general
#define _I4 (*(VBYTEP)(Global_TEMP_BASE + 11)) // interrupt register general
// function temp count variable
#define _C1 (*(VWORDP)(Global_TEMP_BASE + 12)) // global general count register 16bit
#define _C2 (*(VWORDP)(Global_TEMP_BASE + 14)) // interrupt general count register 16bit
/*Code Version*/
#define Version_BASE (Global_TEMP_BASE + MEMORY_016SIZE) // 0x20030
#define CORE_Version (*(VBYTEP)(Version_BASE + 0))       // Code version
// #define     Oem_BBKVer              (*(VBYTEP)(Version_BASE + 1))
#define Oem_CodeVer (*(VBYTEP)(Version_BASE + 2)) // Handle KBC command A2
// #define     Oem_ModelVer            (*(VBYTEP)(Version_BASE + 3))
#define Oem_SubVer (*(VBYTEP)(Version_BASE + 4)) // Handle command C5
// #define     Oem_PCBVer              (*(VBYTEP)(Version_BASE + 5))
#define Oem_TestVer (*(VBYTEP)(Version_BASE + 6)) // Handle command 9F - Oem Code Version
// #define     KeyboardMatrixID        (*(VBYTEP)(Version_BASE + 7))
/*Time count*/
#define OEMHook_MemBase (Version_BASE + MEMORY_008SIZE)     // 0x20038
#define timer_1ms_count (*(VBYTEP)(OEMHook_MemBase + 0))    // Periodic timing flag
#define timer_5ms_count (*(VBYTEP)(OEMHook_MemBase + 1))    // Periodic timing flag
#define timer_50ms_count (*(VBYTEP)(OEMHook_MemBase + 2))   // Periodic timing flag
#define timer_100ms_count (*(VBYTEP)(OEMHook_MemBase + 3))  // Periodic timing flag
#define timer_1s_count (*(VBYTEP)(OEMHook_MemBase + 4))     // Periodic timing flag
#define timer_1min_count (*(VBYTEP)(OEMHook_MemBase + 5))   // Periodic timing flag
#define timer_1hours_count (*(VWORDP)(OEMHook_MemBase + 6)) // Periodic timing flag
/*GPIO Debounce*/
#define Debounce_MemBase (OEMHook_MemBase + MEMORY_008SIZE) // 0x20040
#define DEBOUNCE_FLAG0 (*(VBYTEP)(Debounce_MemBase + 0))
#define DEBOUNCE_CONT0 (*(VBYTEP)(Debounce_MemBase + 1)) // Meaning unclear
#define DEBOUNCE_CONT1 (*(VBYTEP)(Debounce_MemBase + 2)) // Meaning unclear
#define DEBOUNCE_CONT2 (*(VBYTEP)(Debounce_MemBase + 3)) // Meaning unclear
// #define     DEBOUNCE_FLAG1          (*(VBYTEP)(Debounce_MemBase + 4))
#define DEBOUNCE_CONT3 (*(VBYTEP)(Debounce_MemBase + 5)) // Meaning unclear
#define DEBOUNCE_CONT4 (*(VBYTEP)(Debounce_MemBase + 6)) // Meaning unclear
#define DEBOUNCE_CONT5 (*(VBYTEP)(Debounce_MemBase + 7)) // Meaning unclear
// #define     DEBOUNCE_FLAG2          (*(VBYTEP)(Debounce_MemBase + 8))
#define DEBOUNCE_CONT6 (*(VBYTEP)(Debounce_MemBase + 9))  // Meaning unclear
#define DEBOUNCE_CONT7 (*(VBYTEP)(Debounce_MemBase + 10)) // Meaning unclear
#define DEBOUNCE_CONT8 (*(VBYTEP)(Debounce_MemBase + 11)) // Meaning unclear
// #define     DEBOUNCE_FLAG3         (*(VBYTEP)(Debounce_MemBase + 12))
#define DEBOUNCE_CONT9 (*(VBYTEP)(Debounce_MemBase + 13)) // Meaning unclear
// #define     DEBOUNCE_CONT10         (*(VBYTEP)(Debounce_MemBase + 14))
// #define     DEBOUNCE_CONT11          (*(VBYTEP)(Debounce_MemBase + 15))
/*LED*/
#define Led_Data_BASE (Debounce_MemBase + MEMORY_016SIZE) // 0x20050
#define Led_Data_SCROLL (*(VBYTEP)(Led_Data_BASE + 0))    // Check ScrollLock LED Status
#define Led_Data_NUM (*(VBYTEP)(Led_Data_BASE + 1))       // Check NumLock LED Status
#define Led_Data_CAPS (*(VBYTEP)(Led_Data_BASE + 2))      // Check CapsLock LED Status
// #define     Led_Data_OVL
// #define     Led_Data_LED4
// #define     Led_Data_LED5
// #define     Led_Data_LED6
// #define     Led_Data_LED7
// #define     Led_Data_REG
// #define     Led_Data_GREAN
// #define     Led_Data_BLUE
// #define     Led_Data_Alpha
// #define     Led_Data_RGB
#define CPU_RUN_BASE (Led_Data_BASE + MEMORY_032SIZE) // 0x20070
#define CPU_FREQ (*(VDWORDP)(CPU_RUN_BASE + 0))
#define DEFAULT_BASE_ADDR1 (CPU_RUN_BASE + MEMORY_016SIZE)               // 0x20080
#define DEFAULT_BASE_ADDR1_SIZE (MEMORY_BASE1_ADDR - DEFAULT_BASE_ADDR1) // 64
/****************************************************************
--------------------- MEMORY SECTION 1 --------------------------
-----------------KERNEL & CHIP LEVEL VATIABLES-------------------
****************************************************************/
/*HOST interface*/
// Host flag
#define Host_Flag_BASE MEMORY_BASE1_ADDR // 0x20100
#define Host_Flag_INTR_KEY (*(VBYTEP)(Host_Flag_BASE + 0))
#define Host_Flag_INTR_AUX (*(VBYTEP)(Host_Flag_BASE + 1))
#define Host_Flag_SYS_FLAG (*(VBYTEP)(Host_Flag_BASE + 2))
#define Host_Flag_DISAB_KEY (*(VBYTEP)(Host_Flag_BASE + 3))
#define Host_Flag_DISAB_AUX (*(VBYTEP)(Host_Flag_BASE + 4))
#define Host_Flag_XLATE_PC (*(VBYTEP)(Host_Flag_BASE + 5))
#define Host_Flag (*(VBYTEP)(Host_Flag_BASE + 6))
#define FastA20 (*(VBYTEP)(Host_Flag_BASE + 7))
#define MS_Report_Flag (*(VBYTEP)(Host_Flag_BASE + 8))
#define EnableTP (*(VBYTEP)(Host_Flag_BASE + 9))
#define Gen_Info_BREAK_SCAN (*(VBYTEP)(Host_Flag_BASE + 10))
#define AUX2ByteCommand (*(VBYTEP)(Host_Flag_BASE + 11))
#define KB_Scan_Flag (*(VBYTEP)(Host_Flag_BASE + 12))
#define MinuteCnt (*(VBYTEP)(Host_Flag_BASE + 13))
#define Debug_Num (*(VWORDP)(Host_Flag_BASE + 14))
/*ACPI Interface*/
#define ACPI_BASE (Host_Flag_BASE + MEMORY_016SIZE) // 0x20110
#define ACPI_FLAG (*(uByte *)(ACPI_BASE + 0x0))
/*--------------------------------------------------------------------*/
#define ACPI_STATE_BYTE ACPI_FLAG.byte   // ACPI_STATE
#define ACPI_STATE_S0 ACPI_FLAG.bit.bit0 // ACPI_STATE
#define ACPI_STATE_S1 ACPI_FLAG.bit.bit1 // ACPI_STATE
#define ACPI_STATE_S2 ACPI_FLAG.bit.bit2 // ACPI_STATE
#define ACPI_STATE_S3 ACPI_FLAG.bit.bit3 // ACPI_STATE
#define ACPI_STATE_S4 ACPI_FLAG.bit.bit4 // ACPI_STATE
#define ACPI_STATE_S5 ACPI_FLAG.bit.bit5 // ACPI_STATE
/*--------------------------------------------------------------------*/
#define CORE_OSFLAG (*(uByte *)(ACPI_BASE + 0x1))
/*--------------------------------------------------------------------*/
#define WAIT_SCI_CENTER CORE_OSFLAG.bit.bit6
#define OS_ACPI_Mode CORE_OSFLAG.bit.bit7
/*--------------------------------------------------------------------*/
// #define     CORE_CONFIG1            (*(uByte*)(ACPI_BASE + 0x2))
// #define     CORE_CONFIG2            (*(uByte*)(ACPI_BASE + 0x3))
/*--------------------------------------------------------------------*/
// #define     Flag                    (*(uWord*)(ACPI_BASE + 0x4))
#define ShutDnCause (*(VBYTEP)(ACPI_BASE + 0x6)) // setup shutdown case
/*KBC Interface*/
#define Pass_BuffBase (ACPI_BASE + MEMORY_008SIZE) // 0x20118
#define Pass_Buff (*(VBYTEP)(Pass_BuffBase + 0x0))
#define Tmp_Load Pass_Buff
#define Pass_Buff_Idx (*(VBYTEP)(Pass_BuffBase + 0x1))
#define Pass_On (*(VBYTEP)(Pass_BuffBase + 0x2))
#define Pass_Off (*(VBYTEP)(Pass_BuffBase + 0x3))
#define Pass_Make1 (*(VBYTEP)(Pass_BuffBase + 0x4))
#define Pass_Make2 (*(VBYTEP)(Pass_BuffBase + 0x5))
// #define     Pass_RV1                (*(VBYTEP)(Pass_BuffBase + 0x6))
// #define     Pass_RV2                (*(VBYTEP)(Pass_BuffBase + 0x7))
#define KBC_MemBase (Pass_BuffBase + MEMORY_008SIZE) // 0x20120
#define KB_Command (*(VBYTEP)(KBC_MemBase + 0x0))
#define Emulate8042Port (*(VBYTEP)(KBC_MemBase + 0x1))
#define KBHICmd (*(VBYTEP)(KBC_MemBase + 0x2))
#define KBHIStep (*(VBYTEP)(KBC_MemBase + 0x3))
#define KBHIData (*(VBYTEP)(KBC_MemBase + 0x4))
#define KBPendingRXCount (*(VBYTEP)(KBC_MemBase + 0x5))
#define KBPendingTXCount (*(VBYTEP)(KBC_MemBase + 0x6))
#define KB_Main_CHN (*(VBYTEP)(KBC_MemBase + 0x7))
#define MS_MemBase (KBC_MemBase + MEMORY_008SIZE) // 0x20128
#define MSPendingRXCount (*(VBYTEP)(MS_MemBase + 0x0))
#define MSPendingTXCount (*(VBYTEP)(MS_MemBase + 0x1))
#define MSCmdAck (*(VBYTEP)(MS_MemBase + 0x2))
#define MS_Resolution (*(VBYTEP)(MS_MemBase + 0x3))
#define MS_Sampling_Rate (*(VBYTEP)(MS_MemBase + 0x4))
#define MS_ID_Count (*(VBYTEP)(MS_MemBase + 0x5))
#define MS_Main_CHN (*(VBYTEP)(MS_MemBase + 0x6))
/*PMC Interface*/
#define PM_BASE (MS_MemBase + MEMORY_008SIZE) // 0x20130
#define PM1Cmd (*(VBYTEP)(PM_BASE + 0x0))
#define PM1Step (*(VBYTEP)(PM_BASE + 0x1))
#define PM1StepSP (*(VBYTEP)(PM_BASE + 0x2))
#define PM1Data (*(VBYTEP)(PM_BASE + 0x3))
#define PM1Data1 (*(VBYTEP)(PM_BASE + 0x4))
#define PM1Data2 (*(VBYTEP)(PM_BASE + 0x5))
#define PM2Cmd (*(VBYTEP)(PM_BASE + 0x6))
#define PM2Step (*(VBYTEP)(PM_BASE + 0x7))
#define PM2Data (*(VBYTEP)(PM_BASE + 0x8))
#define PM3Cmd (*(VBYTEP)(PM_BASE + 0x9))
#define PM3Step (*(VBYTEP)(PM_BASE + 0xA))
#define PM3Data (*(VBYTEP)(PM_BASE + 0xB))
#define PM4Cmd (*(VBYTEP)(PM_BASE + 0xC))
#define PM4Step (*(VBYTEP)(PM_BASE + 0xD))
#define PM4Data (*(VBYTEP)(PM_BASE + 0xE))
#define PM5Cmd (*(VBYTEP)(PM_BASE + 0xF))
#define PM5Step (*(VBYTEP)(PM_BASE + 0x10))
#define PM5Data (*(VBYTEP)(PM_BASE + 0x11))
#define PM4UnProcessCnt (*(VBYTEP)(PM_BASE + 0x12))
#define PM5UnProcessCnt (*(VBYTEP)(PM_BASE + 0x13))
#define ECU_Cmd (*(VBYTEP)(PM_BASE + 0x14))
#define ECU_Send (*(VBYTEP)(PM_BASE + 0x15))
#define PageWrite_Flag (*(VBYTEP)(PM_BASE + 0x16))
/*SCI Event*/
#define SCI_MemBase (PM_BASE + MEMORY_032SIZE) // 0x20150
#define SCI_Event_In_Index (*(VBYTEP)(SCI_MemBase + 0x0))
#define SCI_Event_Out_Index (*(VBYTEP)(SCI_MemBase + 0x1))
#define SCI_StepTimer (*(VBYTEP)(SCI_MemBase + 0x2))
#define SCI_LastQueryEvent (*(VBYTEP)(SCI_MemBase + 0x3))
#define SCI_QueryEvent (*(VBYTEP)(SCI_MemBase + 0x4))
#define PD_SCI_Event_In_Index (*(VBYTEP)(SCI_MemBase + 0x5))
#define PD_SCI_Event_Out_Index (*(VBYTEP)(SCI_MemBase + 0x6))
/*SMI Event*/
#define SMI_MemBase (SCI_MemBase + MEMORY_008SIZE) // 0x20158
#define SMI_Event_In_Index (*(VBYTEP)(SMI_MemBase + 0x0))
#define SMI_Event_Out_Index (*(VBYTEP)(SMI_MemBase + 0x1))
#define SMI_StepTimer (*(VBYTEP)(SMI_MemBase + 0x2))
// #define     SMI_LastQueryEvent      (*(VBYTEP)(SMI_MemBase + 0x3))
#define SMI_QueryEvent (*(VBYTEP)(SMI_MemBase + 0x4))
#define SCI_Response_Flag (*(VBYTEP)(SMI_MemBase + 0x5))
#define SCI_Count (*(VBYTEP)(SMI_MemBase + 0x6))
/*KBS Interface*/
#define KBS_MemBase (SMI_MemBase + MEMORY_008SIZE)          // 0x20160
#define KBD_SCAN_STATE (*(ukey_state *)(KBS_MemBase + 0x0)) // size 1/2byte
#define KBD_RV1 (*(VBYTEP)(KBS_MemBase + 0x1))              // 保护用间隔
#define KBS_INFO (*(uKEY *)(KBS_MemBase + 0x2))             // size 1/2byte
#define KBD_RV2 (*(VBYTEP)(KBS_MemBase + 0x3))              // 保护用间隔
#define KBD_SCAN (*(sscan *)(KBS_MemBase + 0x4))            // size 3/4byte
#define KBD_RV3 (*(VBYTEP)(KBS_MemBase + 0x7))              // 保护用间隔
#define KBS_REPEAT (*(VBYTEP)(KBS_MemBase + 0x8))
#define KBS_DELAY (*(VBYTEP)(KBS_MemBase + 0x9))
#define KBS_COUNT (*(VBYTEP)(KBS_MemBase + 0xA))
#define FnStickKey (*(VBYTEP)(KBS_MemBase + 0xB))
#define NumLockKey (*(VBYTEP)(KBS_MemBase + 0xC)) // debug add
#define Keyboard_CodeSet (*(VBYTEP)(KBS_MemBase + 0xD))
#define Keyboard_Typematic (*(VBYTEP)(KBS_MemBase + 0xE))
#define Restore_Typematic (*(VBYTEP)(KBS_MemBase + 0xF))
#define Fn_BUNDLED_BASE ((KBS_MemBase + MEMORY_016SIZE)) // 0x20170
#define Fn_BUNDLED_FLAG ((VBYTEP)(Fn_BUNDLED_BASE))      // 定义一个16字节的数组
// SCANNER
#define Scanner_State_BASE (Fn_BUNDLED_BASE + MEMORY_016SIZE) // 0x20180
#define Scanner_State (*(VBYTEP)(Scanner_State_BASE + 0))
#define Scanner_State_NUM_LOCK (*(VBYTEP)(Scanner_State_BASE + 1))
#define Scanner_State_LEFT (*(VBYTEP)(Scanner_State_BASE + 2))
#define Scanner_State_RIGHT (*(VBYTEP)(Scanner_State_BASE + 3))
#define Scanner_State_ALT (*(VBYTEP)(Scanner_State_BASE + 4))
#define Scanner_State_CONTROL (*(VBYTEP)(Scanner_State_BASE + 5))
#define Scanner_State_OVERLAY (*(VBYTEP)(Scanner_State_BASE + 6))
#define Scanner_State_FN (*(VBYTEP)(Scanner_State_BASE + 7))
/*PS2 Interface*/
#define PS2_MemBase (Scanner_State_BASE + MEMORY_008SIZE) // 0x20188
#define MS_Driver_Type (*(VBYTEP)(PS2_MemBase + 0x0))
#define MS_Frame_Cnt (*(VBYTEP)(PS2_MemBase + 0x1))
#define TP_ACK_CUNT (*(VBYTEP)(PS2_MemBase + 0x5))
#define MS_AutoReset_ACK_CNT (*(VBYTEP)(PS2_MemBase + 0x6))
#define MS_Data_Cnt (*(VBYTEP)(PS2_MemBase + 0x7))
#define PS2_PORT0_FLAG (*(VBYTEP)(PS2_MemBase + 0x8))
/*--------------------------------------------------------------------*/
#define PORT0_DEVICE_IS_MOUSE BIT0
#define PORT0_DEVICE_IS_KEYBOARD BIT1
/*--------------------------------------------------------------------*/
#define PS2_PORT1_FLAG (*(VBYTEP)(PS2_MemBase + 0x9))
/*--------------------------------------------------------------------*/
#define PORT1_DEVICE_IS_MOUSE BIT0
#define PORT1_DEVICE_IS_KEYBOARD BIT1
/*--------------------------------------------------------------------*/
#define acknowledge (*(VBYTEP)(PS2_MemBase + 0xB))
#define resend (*(VBYTEP)(PS2_MemBase + 0xC))
// #define 	ps2_port0_reply_expected (*(VBYTEP)(PS2_MemBase + 0xD))
// #define 	ps2_port1_reply_expected (*(VBYTEP)(PS2_MemBase + 0xE))
#define ps2_portn_reply_expected (*(VBYTEP)(PS2_MemBase + 0xF))
#define TP_ACK_Timeout (*(VWORDP)(PS2_MemBase + 0x10))
// KB Data Peding Buff
#define KBData_Buff_MemBase (PS2_MemBase + MEMORY_032SIZE)
// KB Data Peding Buff
#define MSData_Buff_MemBase (KBData_Buff_MemBase + MEMORY_008SIZE)
/*INTC*/
#define INTC_MemBase (MSData_Buff_MemBase + MEMORY_008SIZE) // 0x20198
#define LPC_MON_CNT (*(VBYTEP)(INTC_MemBase + 0x0))         // LPC存储偏移计数
#define LPC_SOF (*(VBYTEP)(INTC_MemBase + 0x1))             // 接收起始标志
#define LPC_EOF (*(VBYTEP)(INTC_MemBase + 0x2))             // 接收结束标志
#define LongTime_Flag (*(VBYTEP)(INTC_MemBase + 0x3))       // 调试器内容
/*PWM&TACH*/
// PWM
#define PWM_Membase ((INTC_MemBase + MEMORY_008SIZE)) // 0x201A0
// #define     PWM_CTR                 (*(VBYTEP)(PWM_Membase+0x0))
// #define     PWM_DCR                 (*(VBYTEP)(PWM_Membase+0x1))
// #define     TACH0_Data              (*(VWORDP)(PWM_Membase+0x2))
#define TACH0_Speed (*(VWORDP)(PWM_Membase + 0x4)) // Fan speed variable
#define TACH1_Speed (*(VWORDP)(PWM_Membase + 0x6)) // Fan speed variable
#define TACH2_Speed (*(VWORDP)(PWM_Membase + 0x8)) // Fan speed variable
#define TACH3_Speed (*(VWORDP)(PWM_Membase + 0xA)) // Fan speed variable
/*UART Module*/
// UART0_BAUD
#define UART0_Membase ((PWM_Membase + MEMORY_016SIZE)) // 0x201B0
#define update_mode (*(VBYTEP)(UART0_Membase + 0x0))   // 固件更新模式
#define update_flag (*(VBYTEP)(UART0_Membase + 0x1))   // 固件更新标志
#define PRINT_SERVICE_CNT (*(VWORDP)(UART0_Membase + 0x2))
#define print_cnt (*(VWORDP)(UART0_Membase + 0x4))
#define uart_updata_flag (*(VBYTEP)(UART0_Membase + 0x6))
#define CMD_UART_CNT (*(VBYTEP)(UART0_Membase + 0x7))
#define print_number (*(VDWORDP)(UART0_Membase + 0x8))
#define g_uart_base (*(VDWORDP)(UART0_Membase + 0xC))
// UART1_BAUD
#define UART1_Membase ((UART0_Membase + MEMORY_016SIZE)) // 0x201C0
// #define     UART1_Status            (*(VBYTEP)(UART1_Membase + 0x0))
// #define     UART1_ReadData          (*(VBYTEP)(UART1_Membase + 0x1))
// #define     UART1_ReceiveFlag       (*(VBYTEP)(UART1_Membase + 0x2))
// #define     UART1_WriteData         (*(VBYTEP)(UART1_Membase + 0x3))
// UART2_BAUD
#define UART2_Membase ((UART1_Membase + MEMORY_008SIZE)) // 0x201C8
// #define     UART2_Status            (*(VBYTEP)(UART2_Membase + 0x0))
// #define     UART2_ReadData          (*(VBYTEP)(UART2_Membase + 0x1))
// #define     UART2_ReceiveFlag       (*(VBYTEP)(UART2_Membase + 0x2))
// #define     UART2_WriteData         (*(VBYTEP)(UART2_Membase + 0x3))
// UART3_BAUD
#define UART3_Membase ((UART2_Membase + MEMORY_008SIZE)) // 0x201D0
// #define     UART3_Status            (*(VBYTEP)(UART3_Membase + 0x0))
// #define     UART3_ReadData          (*(VBYTEP)(UART3_Membase + 0x1))
// #define     UART3_ReceiveFlag       (*(VBYTEP)(UART3_Membase + 0x2))
// #define     UART3_WriteData         (*(VBYTEP)(UART3_Membase + 0x3))
// UARTA_BAUD
#define UARTA_Membase ((UART3_Membase + MEMORY_008SIZE)) // 0x201D8
// #define     UARTA_Status            (*(VBYTEP)(UARTA_Membase + 0x0))
// #define     UARTA_WriteData         (*(VBYTEP)(UARTA_Membase + 0x1))
// #define     UARTA_ReceiveFlag       (*(VBYTEP)(UARTA_Membase + 0x2))
// #define     UARTA_ReadData          (*(VBYTEP)(UARTA_Membase + 0x3))
// #define     UARTA_WriteFlag         (*(VBYTEP)(UARTA_Membase + 0x4))
// UARTB_BAUD
#define UARTB_Membase ((UARTA_Membase + MEMORY_008SIZE)) // 0x201E0
// #define     UARTB_Status            (*(VBYTEP)(UARTB_Membase + 0x0))
// #define     UARTB_WriteData         (*(VBYTEP)(UARTB_Membase + 0x1))
// #define     UARTB_ReceiveFlag       (*(VBYTEP)(UARTB_Membase + 0x2))
// #define     UARTB_ReadData          (*(VBYTEP)(UARTB_Membase + 0x3))
// #define     UARTB_WriteFlag         (*(VBYTEP)(UARTB_Membase + 0x4))
// CAN0
#define CAN0_Membase ((UARTB_Membase + MEMORY_008SIZE)) // 0x201E8
#define CAN0_ReadFlag (*(VBYTEP)(CAN0_Membase + 0x0))   // 接收完毕标志
// #define     CAN0_WriteFlag
// #define     CAN0_Status
// #define     CAN0_Int_Status
// CAN1
#define CAN1_Membase ((CAN0_Membase + MEMORY_008SIZE)) // 0x201F0
#define CAN1_ReadFlag (*(VBYTEP)(CAN1_Membase + 0x0))  // 接收完毕标志
// #define     CAN1_WriteFlag          (*(VBYTEP)(CAN1_Membase + 0x1))
// #define     CAN1_Status             (*(VBYTEP)(CAN1_Membase + 0x2))
// #define     CAN1_Int_Status         (*(VBYTEP)(CAN1_Membase + 0x3))
// CAN2
#define CAN2_Membase ((CAN1_Membase + MEMORY_008SIZE)) // 0x201F8
#define CAN2_ReadFlag (*(VBYTEP)(CAN2_Membase + 0x0))  // 接收完毕标志
// #define     CAN2_WriteFlag          (*(VBYTEP)(CAN2_Membase + 0x1))
// #define     CAN2_Status             (*(VBYTEP)(CAN2_Membase + 0x2))
// #define     CAN2_Int_Status         (*(VBYTEP)(CAN2_Membase + 0x3))
// CAN3
#define CAN3_Membase ((CAN2_Membase + MEMORY_008SIZE)) // 0x20200
#define CAN3_ReadFlag (*(VBYTEP)(CAN3_Membase + 0x0))  // 接收完毕标志
// #define     CAN3_WriteFlag          (*(VBYTEP)(CAN3_Membase + 0x1))
// #define     CAN3_Status             (*(VBYTEP)(CAN3_Membase + 0x2))
// #define     CAN3_Int_Status         (*(VBYTEP)(CAN3_Membase + 0x3))
/*WDT Module*/
#define WDT_Membase ((CAN3_Membase + MEMORY_008SIZE)) // 0x20208
/*ADC Interface*/
#define ADC_MemBase (WDT_Membase + MEMORY_008SIZE) // 0x20210
#define ADC_Data0 (*(short *)((ADC_MemBase) + 0x0))
#define ADC_Data1 (*(short *)((ADC_MemBase) + 0x2))
#define ADC_Data2 (*(short *)((ADC_MemBase) + 0x4))
#define ADC_Data3 (*(short *)((ADC_MemBase) + 0x6))
#define ADC_Data4 (*(short *)((ADC_MemBase) + 0x8))
#define ADC_Data5 (*(short *)((ADC_MemBase) + 0xa))
#define ADC_Data6 (*(short *)((ADC_MemBase) + 0xc))
#define ADC_Data7 (*(short *)((ADC_MemBase) + 0xe))
/*I2C&SMB Interface*/
#define SMB_MemBase (ADC_MemBase + MEMORY_016SIZE)     // 0x20218
#define SMB_Check_Timer (*(VBYTEP)(SMB_MemBase + 0x0)) // Meaning unclear
#define SMB_Temp_Addr (*(VBYTEP)(SMB_MemBase + 0x1))
#define SMB_Temp_Data (*(VBYTEP)(SMB_MemBase + 0x2))
#define SMB_PEC_Value (*(VBYTEP)(SMB_MemBase + 0x3))
#define SMB_SlaveAddress (*(VWORDP)(SMB_MemBase + 0x4))
/*SPIM interface*/
#define SPI_MemBase (SMB_MemBase + MEMORY_008SIZE)   // 0x20220
#define SPI_FLAHS_ID (*(VDWORDP)(SPI_MemBase + 0x0)) // 存放flashid
#define SPI_Timeout (*(VDWORDP)(SPI_MemBase + 0x4))  // 超时跳出
// #define     SPI_ReadDATA            (*(VWORDP)(SPI_MemBase + 0x8))
#define SPI_WriteDATA (*(VWORDP)(SPI_MemBase + 0xa))  // 存放准备往里写数据
#define SPI_Read_Start (*(VBYTEP)(SPI_MemBase + 0xc)) // 判断读过程情况
#define FLASH_SR (*(VBYTEP)(SPI_MemBase + 0xd))       // flash状态
/*BRAM Module*/
#define BRAM_MemBase (SPI_MemBase + MEMORY_016SIZE) // 0x20230
#define BRAM_ReadBuff ((VBYTEP)(BRAM_MemBase + 0))  // 定义一个48字节的数组
// #define     BRAM_ReadBuff_FIFO0      (*(VBYTEP)(BRAM_MemBase+0))
// #define     BRAM_ReadBuff_FIFO1      (*(VBYTEP)(BRAM_MemBase+1))
// #define     BRAM_ReadBuff_FIFO2      (*(VBYTEP)(BRAM_MemBase+2))
// #define     BRAM_ReadBuff_FIFO3      (*(VBYTEP)(BRAM_MemBase+3))
// #define     BRAM_ReadBuff_FIFO4      (*(VBYTEP)(BRAM_MemBase+4))
// #define     BRAM_ReadBuff_FIFO5      (*(VBYTEP)(BRAM_MemBase+5))
// #define     BRAM_ReadBuff_FIFO6      (*(VBYTEP)(BRAM_MemBase+6))
// #define     BRAM_ReadBuff_FIFO7      (*(VBYTEP)(BRAM_MemBase+7))
// #define     BRAM_ReadBuff_FIFO8      (*(VBYTEP)(BRAM_MemBase+8))
// #define     BRAM_ReadBuff_FIFO9      (*(VBYTEP)(BRAM_MemBase+9))
// #define     BRAM_ReadBuff_FIFO10     (*(VBYTEP)(BRAM_MemBase+10))
// #define     BRAM_ReadBuff_FIFO11     (*(VBYTEP)(BRAM_MemBase+11))
// #define     BRAM_ReadBuff_FIFO12     (*(VBYTEP)(BRAM_MemBase+12))
// #define     BRAM_ReadBuff_FIFO13     (*(VBYTEP)(BRAM_MemBase+13))
// #define     BRAM_ReadBuff_FIFO14     (*(VBYTEP)(BRAM_MemBase+14))
// #define     BRAM_ReadBuff_FIFO15     (*(VBYTEP)(BRAM_MemBase+15))
// #define     BRAM_ReadBuff_FIFO16     (*(VBYTEP)(BRAM_MemBase+16))
// #define     BRAM_ReadBuff_FIFO17     (*(VBYTEP)(BRAM_MemBase+17))
// #define     BRAM_ReadBuff_FIFO18     (*(VBYTEP)(BRAM_MemBase+18))
// #define     BRAM_ReadBuff_FIFO19     (*(VBYTEP)(BRAM_MemBase+19))
// #define     BRAM_ReadBuff_FIFO20     (*(VBYTEP)(BRAM_MemBase+20))
// #define     BRAM_ReadBuff_FIFO21     (*(VBYTEP)(BRAM_MemBase+21))
// #define     BRAM_ReadBuff_FIFO22     (*(VBYTEP)(BRAM_MemBase+22))
// #define     BRAM_ReadBuff_FIFO23     (*(VBYTEP)(BRAM_MemBase+23))
// #define     BRAM_ReadBuff_FIFO24     (*(VBYTEP)(BRAM_MemBase+24))
// #define     BRAM_ReadBuff_FIFO25     (*(VBYTEP)(BRAM_MemBase+25))
// #define     BRAM_ReadBuff_FIFO26     (*(VBYTEP)(BRAM_MemBase+26))
// #define     BRAM_ReadBuff_FIFO27     (*(VBYTEP)(BRAM_MemBase+27))
// #define     BRAM_ReadBuff_FIFO28     (*(VBYTEP)(BRAM_MemBase+28))
// #define     BRAM_ReadBuff_FIFO29     (*(VBYTEP)(BRAM_MemBase+29))
// #define     BRAM_ReadBuff_FIFO30     (*(VBYTEP)(BRAM_MemBase+30))
// #define     BRAM_ReadBuff_FIFO31     (*(VBYTEP)(BRAM_MemBase+31))
// #define     BRAM_ReadBuff_FIFO32     (*(VBYTEP)(BRAM_MemBase+32))
// #define     BRAM_ReadBuff_FIFO33     (*(VBYTEP)(BRAM_MemBase+33))
// #define     BRAM_ReadBuff_FIFO34     (*(VBYTEP)(BRAM_MemBase+34))
// #define     BRAM_ReadBuff_FIFO35     (*(VBYTEP)(BRAM_MemBase+35))
// #define     BRAM_ReadBuff_FIFO36     (*(VBYTEP)(BRAM_MemBase+36))
// #define     BRAM_ReadBuff_FIFO37     (*(VBYTEP)(BRAM_MemBase+37))
// #define     BRAM_ReadBuff_FIFO38     (*(VBYTEP)(BRAM_MemBase+38))
// #define     BRAM_ReadBuff_FIFO39     (*(VBYTEP)(BRAM_MemBase+39))
// #define     BRAM_ReadBuff_FIFO40     (*(VBYTEP)(BRAM_MemBase+40))
// #define     BRAM_ReadBuff_FIFO41     (*(VBYTEP)(BRAM_MemBase+41))
// #define     BRAM_ReadBuff_FIFO42     (*(VBYTEP)(BRAM_MemBase+42))
// #define     BRAM_ReadBuff_FIFO43     (*(VBYTEP)(BRAM_MemBase+43))
// #define     BRAM_ReadBuff_FIFO44     (*(VBYTEP)(BRAM_MemBase+44))
// #define     BRAM_ReadBuff_FIFO45     (*(VBYTEP)(BRAM_MemBase+45))
// #define     BRAM_ReadBuff_FIFO46     (*(VBYTEP)(BRAM_MemBase+46))
// #define     BRAM_ReadBuff_FIFO47     (*(VBYTEP)(BRAM_MemBase+47))
// #define     BRAM_ReadBuff_FIFO48     (*(VBYTEP)(BRAM_MemBase+48))
/*SPIF Interface*/
#define SPIF_MemBase (BRAM_MemBase + MEMORY_064SIZE) // 0x20270
/*ShareMemory*/
#define SM_MemBase (SPIF_MemBase + MEMORY_032SIZE) // 0x20290
/*SWUC Module*/
#define SWUC_MemBase (SM_MemBase + MEMORY_032SIZE) // 0x202B0
/*P80 Module*/
#define P80_MemBase (SWUC_MemBase + MEMORY_016SIZE) // 0x202C0
/*Power switch*/
#define PSW_MemBase (P80_MemBase + MEMORY_008SIZE) // 0x202C8
/*PPORT Module*/
#define PPORT_MemBase (PSW_MemBase + MEMORY_008SIZE) // 0x202D0
// POWER
#define POWER_MemBase (PPORT_MemBase + MEMORY_008SIZE) // 0x202D8
// #define     POWER_MemBase           (BRAM_MemBase + MEMORY_064SIZE)//0x20270
#define DelayDoPowerSeq (*(VDWORDP)(POWER_MemBase + 0x0))
#define System_PowerState (*(VBYTEP)(POWER_MemBase + 0x4))
#define F_PST_BUSY BIT(7)
// #define     CtrlPowStep             (*(VBYTEP)(POWER_MemBase + 0x5))
// #define     PowSeqWait              (*(VBYTEP)(POWER_MemBase + 0x6))
#define AutoColdBootTimer (*(VBYTEP)(POWER_MemBase + 0x7))
// #define     ColdBootTimer           (*(VBYTEP)(POWER_MemBase + 0x8))
// #define     LockSwitchPowerOn       (*(VBYTEP)(POWER_MemBase + 0x9))
//  #define     PowSwOverTimer          (*(VBYTEP)(POWER_MemBase + 0xA))
//  #define     DeepSleepCountDown      (*(VBYTEP)(POWER_MemBase + 0xB))
//  #define     DEBUG_RAM1              (*(VBYTEP)(POWER_MemBase + 0xC))
#define RSTStatus (*(VBYTEP)(POWER_MemBase + 0xD))
// #define     power_flag              (*(VBYTEP)(POWER_MemBase + 0xE))
#define S5S0_flag (*(VBYTEP)(POWER_MemBase + 0xF))
#define PowerSequence_Step (*(VBYTEP)(POWER_MemBase + 0x10))
#define PowerStatusBK (*(VBYTEP)(POWER_MemBase + 0x11))
#define PowerState_Monitor_Index (*(VBYTEP)(POWER_MemBase + 0x12))
// #define     DeepSleepCunt           (*(VBYTEP)(POWER_MemBase + 0x13))
#define HWPG_TIMER (*(VWORDP)(POWER_MemBase + 0x14))
#define ECAutoPowerOn (*(VBYTEP)(POWER_MemBase + 0x16))
#define ECWDTRSTNow (*(VBYTEP)(POWER_MemBase + 0x17))
#define SBSWReleaseCunt (*(VBYTEP)(POWER_MemBase + 0x18))
#define GPCRA0 (*(VBYTEP)(POWER_MemBase + 0x19))
#define PowSeq_Record_Flag (*(VBYTEP)(POWER_MemBase + 0x1A))
#define PSW_COUNTER (*(VWORDP)(POWER_MemBase + 0x1b))
#define PowerSequence_WaitTime (*(VWORDP)(POWER_MemBase + 0x1d))
#define PowerSequence_Delay (*(VWORDP)(POWER_MemBase + 0x2b))
// #define     AlwOnFlag	            (*(VBYTEP)(POWER_MemBase + 0x2c))
#define Auto_On (*(VBYTEP)(POWER_MemBase + 0x2d))
#define Low_Power_Flag (*(VBYTEP)(POWER_MemBase + 0x2e))
// BATTERY
#define BATTERY_MemBase (POWER_MemBase + MEMORY_064SIZE) // 0x20318
#define AC_IN (*(VBYTEP)(BATTERY_MemBase + 0x0))
#define BATTERY_IN (*(VBYTEP)(BATTERY_MemBase + 0x1))
#define ChargerStep (*(VBYTEP)(BATTERY_MemBase + 0x2))
#define PD_IN (*(VBYTEP)(BATTERY_MemBase + 0x3))
#define AC_Plugin_Once_Flag (*(VBYTEP)(BATTERY_MemBase + 0x4))
#define PD_Plugin_Once_Flag (*(VBYTEP)(BATTERY_MemBase + 0x5))
#define BT_Plugin_Once_Flag (*(VBYTEP)(BATTERY_MemBase + 0x6))
#define Charger_CtrlStep (*(VBYTEP)(BATTERY_MemBase + 0x7))
#define BatteryDesignVoltage (*(VDWORDP)(BATTERY_MemBase + 0x8))
// #define     CurrentNow              (*(VDWORDP)(BATTERY_MemBase + 0xC))
// #define     TemperatureNow          (*(VDWORDP)(BATTERY_MemBase + 0x10))
#define BatteryInfoRdy (*(VBYTEP)(BATTERY_MemBase + 0x14))
#define PD_INT (*(VBYTEP)(BATTERY_MemBase + 0x15))
#define ChargerCount (*(VBYTEP)(BATTERY_MemBase + 0x16))
#define Bat_Info_Cnt (*(VBYTEP)(BATTERY_MemBase + 0x1A))
#define noac_1s_cnt (*(VBYTEP)(BATTERY_MemBase + 0x1B))
// DEBUG
/* Flags*/
#define DEBUG_MemBase (BATTERY_MemBase + MEMORY_032SIZE) // 0x20338
#define Req_flag (*(VBYTEP)(DEBUG_MemBase + 0x0))
#define Slave_flag (*(VBYTEP)(DEBUG_MemBase + 0x1))
#define ResetChipFlag (*(VBYTEP)(DEBUG_MemBase + 0x2))
#define Num_flag (*(VBYTEP)(DEBUG_MemBase + 0x3))
#define KBC_PMC_Flag (*(VBYTEP)(DEBUG_MemBase + 0x4))
#define Key_Flag (*(VBYTEP)(DEBUG_MemBase + 0x5))
#define Buf_flag (*(VBYTEP)(DEBUG_MemBase + 0x6))
/* Index */
#define Buf_num (*(VBYTEP)(DEBUG_MemBase + 0x7))
#define KBC_PMC_Event_Num (*(VWORDP)(DEBUG_MemBase + 0x8))
#define Event_Num (*(VBYTEP)(DEBUG_MemBase + 0xA))
#define int_buf_index (*(VBYTEP)(DEBUG_MemBase + 0xB))
#define PowSeq_Record_Cnt (*(VBYTEP)(DEBUG_MemBase + 0xC))
#define Buf_Num (*(VBYTEP)(DEBUG_MemBase + 0xD))            // 调试器内容
#define Buf_Flag (*(VBYTEP)(DEBUG_MemBase + 0xE))           // 调试器内容
#define Debug_PRINTF_LEVEL (*(VBYTEP)(DEBUG_MemBase + 0xF)) // 调试器内容
#define Debug_Temp (*(VDWORDP)(DEBUG_MemBase + 0x10))       // 调试器内容
#define Debug_Timeout_Count (*(VWORDP)(DEBUG_MemBase + 0x14))

#ifdef AE103

#define eSPI_MemBase (DEBUG_MemBase + MEMORY_064SIZE) // 0x20358
#define eSPI_BUF_VWCTRL1 (*(VBYTEP)(eSPI_MemBase + 0x0))
#define eSPI_PLTRST_TAG (*(VBYTEP)(eSPI_MemBase + 0x1))
#define xOOB_PacketMaxLength (*(VBYTEP)(eSPI_MemBase + 0x2))
#define xOOB_PacketLength (*(VBYTEP)(eSPI_MemBase + 0x3))
#define xOOB_Timeout (*(VBYTEP)(eSPI_MemBase + 0x4))
#define xOOB_Failed (*(VBYTEP)(eSPI_MemBase + 0x5))
#define xOOB_FailedCounter (*(VBYTEP)(eSPI_MemBase + 0x6))
#define xOOB_DataCounter (*(VBYTEP)(eSPI_MemBase + 0x7))
#define xOOB_Scan (*(VBYTEP)(eSPI_MemBase + 0x8))
#define xOOB_GetPCH_Temper (*(VBYTEP)(eSPI_MemBase + 0x9))
#define xOOB_GetPCH_RTC_Timer (*(VBYTEP)(eSPI_MemBase + 0xA))
#define xOOB_GetPCH_Temper_Timer (*(VBYTEP)(eSPI_MemBase + 0xB))
#define xOOB_PeciGetCpuT_Timer (*(VBYTEP)(eSPI_MemBase + 0xC))
#define xOOB_PCH_Temperature (*(VBYTEP)(eSPI_MemBase + 0xD))
#define xTIME_SYNC (*(VBYTEP)(eSPI_MemBase + 0xE))
#define xTIME_HH (*(VBYTEP)(eSPI_MemBase + 0xF))
#define xTIME_MM (*(VBYTEP)(eSPI_MemBase + 0x10))
#define xTIME_SS (*(VBYTEP)(eSPI_MemBase + 0x11))
#define xOOB_PECI_CPU_T (*(VBYTEP)(eSPI_MemBase + 0x12))
#define xOOB_PECI_Tj_max (*(VBYTEP)(eSPI_MemBase + 0x13))
#define xOOB_KEY_CRASHLOG (*(VBYTEP)(eSPI_MemBase + 0x14))
#define KEY_FCD_LOCK BIT1
#define KEY_FLD_LOCK BIT2
#define xOOB_PauseTimer (*(VBYTEP)(eSPI_MemBase + 0x15))
#define xOOB_GET_FLASH_ADR3 (*(VBYTEP)(eSPI_MemBase + 0x16))
#define xOOB_GET_FLASH_ADR2 (*(VBYTEP)(eSPI_MemBase + 0x17))
#define xOOB_GET_FLASH_ADR1 (*(VBYTEP)(eSPI_MemBase + 0x18))
#define xOOB_GET_FLASH_ADR0 (*(VBYTEP)(eSPI_MemBase + 0x19))
#define xOOB_GET_CRASHLOG (*(VBYTEP)(eSPI_MemBase + 0x1A))
#define xOOB_GET_PCH_TMPR (*(VBYTEP)(eSPI_MemBase + 0x1B))
#define xOOB_GET_RTC_DATA (*(VBYTEP)(eSPI_MemBase + 0x1C))
#define xOOB_CrashLogAddrH (*(VBYTEP)(eSPI_MemBase + 0x1D))
#define xOOB_CrashLogAddrM (*(VBYTEP)(eSPI_MemBase + 0x1E))
#define xOOB_Delay (*(VBYTEP)(eSPI_MemBase + 0x1F))
#define xOOB_CrashCpuIndex (*(VBYTEP)(eSPI_MemBase + 0x20))
#define xOOB_CrashCpuIndexH (*(VBYTEP)(eSPI_MemBase + 0x21))
#define xOOB_GetCrashLogLock (*(VBYTEP)(eSPI_MemBase + 0x22))
#define xOOB_RemindGetCrashLogLED (*(VBYTEP)(eSPI_MemBase + 0x23))

#define wOOB_CrashCpuDataLines (*(VWORDP)(eSPI_MemBase + 0x24))
#define wOOB_CrashCpuDataBytes (*(VWORDP)(eSPI_MemBase + 0x26))
#define wOOB_CrashPchDataSizes (*(VWORDP)(eSPI_MemBase + 0x28))
#define wOOB_CrashLogDataIndex (*(VWORDP)(eSPI_MemBase + 0x2A))

#define PECI_MemBase (eSPI_MemBase + MEMORY_032SIZE) // 0x20398
#define PECI_FLAG (*(VBYTEP)(PECI_MemBase + 0x0))
#define F_PECI_STOP BIT(7)
#define F_PECI_BUSY BIT(6)
#define F_PECI_UPDATED BIT(5)
#define F_PECI_INIT5 BIT(4)
#define F_PECI_INIT4 BIT(3)
#define F_PECI_INIT3 BIT(2)
#define F_PECI_INIT2 BIT(1)
#define F_PECI_INIT BIT(0)
#define PECI_CMD (*(VBYTEP)(PECI_MemBase + 0x1))
#define PECI_CompletionCode (*(VBYTEP)(PECI_MemBase + 0x2))
#define PECI_TIMER (*(VBYTEP)(PECI_MemBase + 0x3))
#define PECI_ERRCNT (*(VBYTEP)(PECI_MemBase + 0x4))
#define PECI_ERRSTS (*(VBYTEP)(PECI_MemBase + 0x5))
#define PECI_OVTCT (*(VBYTEP)(PECI_MemBase + 0x6))

#define PECI_CPU_T (*(VBYTEP)(PECI_MemBase + 0x8))
#define PECI_CNT (*(VBYTEP)(PECI_MemBase + 0x9))
#define PECI_DIR (*(VBYTEP)(PECI_MemBase + 0xA))
#define PECI_DELAY (*(VWORDP)(PECI_MemBase + 0xB))
#define PECI_MEMADR (*(VWORDP)(PECI_MemBase + 0xD))

#define PECI_RdPkgCfg_Idx16_CC (*(VBYTEP)(PECI_MemBase + 0xE))
#define PECI_PowerLimit1 (*(VBYTEP)(PECI_MemBase + 0xF))
#define PECI_PowerLimit1T (*(VBYTEP)(PECI_MemBase + 0x10))
#define PECI_PowerLimit2 (*(VBYTEP)(PECI_MemBase + 0x11))
#define PECI_PowerLimit3 (*(VBYTEP)(PECI_MemBase + 0x12))
#define PECI_PowerLimit4 (*(VBYTEP)(PECI_MemBase + 0x13))

#define PECI_PowerUnit (*(VBYTEP)(PECI_MemBase + 0x14))
#define PECI_EnergyUnit (*(VBYTEP)(PECI_MemBase + 0x15))
#define PECI_TimeUnit (*(VBYTEP)(PECI_MemBase + 0x16))
#define PECI_UnitPower (*(VWORDP)(PECI_MemBase + 0x17))
#define PECI_UnitTime (*(VWORDP)(PECI_MemBase + 0x19))
#define PECI_UnitEnergy (*(VWORDP)(PECI_MemBase + 0x1B))

#define xOOB_PECI_TEST (*(VBYTEP)(PECI_MemBase + 0x1D))
#define xOOB_PECI_PLx_Index (*(VBYTEP)(PECI_MemBase + 0x1E))
#define xOOB_PECI_PLx_Data0 (*(VBYTEP)(PECI_MemBase + 0x1F))
#define xOOB_PECI_PLx_Data1 (*(VBYTEP)(PECI_MemBase + 0x20))

#define Mailbox_MemBase (PECI_MemBase + MEMORY_064SIZE) // 0x203B8
#define Mailbox_Int_Store (*(VBYTEP)(Mailbox_MemBase + 0x00))
#define update_test_flag (*(VBYTEP)(Mailbox_MemBase + 0x01))
#define ROM_COPY_CNT (*(VDWORDP)(Mailbox_MemBase + 0x04))
#define Remaining_size (*(VDWORDP)(Mailbox_MemBase + 0x08))
#define eFlash_Forbid_Flag (*(VBYTEP)(Mailbox_MemBase + 0x0C))

#define DEFAULT_BASE_ADDR2 (Mailbox_MemBase + MEMORY_016SIZE) // 0x203C0
#define DEFAULT_BASE_SIZE 0x2A8

#endif // end #if AE103
/*--------------------------------------ALL DEFINITION END----------------------------------------------*/
#include "CUSTOM_MEMORY.H"
#endif
