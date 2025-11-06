/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-21 17:37:47
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
#ifndef KERNEL_PS2_H
#define KERNEL_PS2_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"

#define PS2_CLOCK  (HIGH_CHIP_CLOCK / (SYSCTL_CLKDIV_PS2+1)) 
#define PS2_WaitTime 3000
//-----------------------------------------------------------------------------
// PS2 CMD WORD
//-----------------------------------------------------------------------------
#define PS2M_CHANNEL 0
#define PS2K_CHANNEL 1
#define PS2_0_CHANNEL PS2M_CHANNEL
#define PS2_1_CHANNEL PS2K_CHANNEL

#define CCMD_MOUSE_DISABLE 0xA7 /* Disable mouse interface */
#define CCMD_MOUSE_ENABLE 0xA8  /* Enable mouse interface */
#define CCMD_KBD_DISABLE 0xAD   /* Disable KBD interface */
#define CCMD_KBD_ENABLE 0xAE    /* Enable KBD interface */
#define CCMD_READ 0x20          /* Read command */
#define CCMD_WRITE 0x60         /* Write command */
#define CCMD_SELFTEST_AA 0xAA      /* Self-test command */
#define CCMD_SELFTEST_AB 0xAB      /* Self-test command */
#define CCMD_RESET 0xFF         /* Reset command */

#define KBD_STAT_GTO 0x40      /* General receive/xmit timeout */
#define KBD_STAT_PERR 0x80     /* Parity error */
#define KBD_TIMEOUT 1000       /* Timeout in ms for keyboard command acknowledge */
#define KBC_TIMEOUT 250        /* Timeout in ms for sending to keyboard controller */
#define AUX_ENABLE_DEV 0xF4 /* Enable aux device */
#define KBD_CMD_ENABLE 0xF4 /* Enable scanning */
#define PS2_STR_OBF 0x2
#define PS2_STR_IBF 0x1
#define PS2_IRQ_MS_EN (1 << 1)
#define PS2_IRQ_MS_DIS (0 << 1)
//-----------------------------------------------------------------------------
// PS2 Interface Debug
//-----------------------------------------------------------------------------
#define PS2_Pack_Debug 0
#define PS2_KBCMD 0 // Enable Debug keyboard command to keyboard and ACK bytes
#define PS2_MSCMD 0 // Enable Debug keyboard command to TP or moues and ACK bytes
#define PS2_AUXTypeDebug 0
//**************************************
#define PS2_Pack_Debug_Index 0x77
#define PS2_AUX_PlugIn 0x88
#define PS2_AUX_StatsuUpdate 0x99
#define PS2_AUXType_MS 0xAA
#define PS2_AUXType_KB 0xAB
#define PS2_KB_Strange 0xFF
#define PS2_AUXType_F2_TimeOut 0xA0
#define FF_ACK_FA_TimeOut 0xA1
#define FF_ACK_AA_TimeOut 0xA2
#define FF_ACK_00_TimeOut 0xA3
#define FF_ACK_00_TimeDone 0xA4
//**************************************
#define PS2_Port0_CLK_CtrlReg GPCRF0
#define PS2_Port0_Data_CtrlReg GPCRF1
#define PS2_PORT0_CLK_CtrlReg GPCRF2
#define PS2_PORT0_Data_CtrlReg GPCRF3
#define ScanAUXStep_ScanPort 0xF0
#define ScanAUXStep_ScanPort0 0xF0
#define ScanAUXStep_ScanPort1 0xF1
#define ScanAUXStep_WaitPort 0xE0
#define ScanAUXStep_WaitPort0 0xE0
#define ScanAUXStep_WaitPort1 0xE1
#define ScanAUXStep_Skip 0xD0
#define ScanAUXStep_Skip0 0xD0
#define ScanAUXStep_Skip1 0xD1
#define ScanAUXStep_Init 0xC0
#define ScanAUXStep_Init0 0xC0
#define ScanAUXStep_Init1 0xC1
#define ScanAUXStep_InitFail 0xC8
#define ScanAUXStep_InitFail0 0xC8
#define ScanAUXStep_InitFail1 0xC9
#define ScanAUXStep_CheckType 0xB0
#define ScanAUXStep_CheckType0 0xB0
#define ScanAUXStep_CheckType1 0xB1
#define ScanAUXStep_UpdateKB 0xA0
#define ScanAUXStep_UpdateKB0 0xA0
#define ScanAUXStep_UpdateKB1 0xA1
#define ScanAUXStep_CheckOK 0x80
#define ScanAUXStep_CheckOK1 0x80
#define ScanAUXStep_CheckOK2 0x81
#define DeviceResetCmdITTime 100 // 1000 ms
#define KBCmdBusyInactiveTime 10 // 90 ~ 100 ms
#define InactiveTime 6           // 50 ~ 60 ms
#define CheckDeviceTimeOut 4     // 30 ~ 40 ms
#define SkipScanAUXTime 150      // Skip KBD_SCAN aux devices for keyboard or mouse driver init. (10ms timer base)
#define PS2_OnOffTimeOutCUNT 5
#define PS2_ReceiveMode 0x17
#define PS2_InhibitMode 0x11
#define MouseDataPending 0x33
#define MouseIRQPending 0x44
#define AUX_RECONNECT1 0xaa
#define AUX_RECONNECT2 0x00
#define AUX_INTS_OFF (KBD_MODE_KCC | KBD_MODE_DISABLE_MOUSE | KBD_MODE_SYS | KBD_MODE_KBD_INT)
#define AUX_INTS_ON (0x26)
#define KBD_INTS_ON (0x37)
#define MAX_RETRIES 60
#define IRQC_INT_DEVICE_PS2_CH0 6
/************PS2 Register define***************/
#define PS2_CR_MS_EN            HEX20
#define PS2_CR_KB_EN            HEX10
#define PS2_CR_MS_IRQ           HEX02
#define PS2_CR_KB_IRQ           HEX01
#define PSW_CR_DISEN            HEX00
/* KBD_MODE_KCC  0x40   Scan code conversion to PC format */
/* KBD_MODE_SYS  0x04   The system flag */
/* KBD_MODE_MOUSE_INT 0x02 Mouse data generate */
/* KBD_MODE_KBD_INT 0x01   kbd data generate  */
extern VBYTE PS2_PortN_Data[];
extern VBYTE *KBDataPending;
extern VBYTE *MSDataPending;
extern BYTE Handle_Kbd_Event(BYTE channel);
extern BYTE Handle_Mouse_Event(BYTE channel);
extern void Handle_Mouse(BYTE channel);
extern BYTE PS2_DevScan(void);

extern void MS_Data_Suspend(BYTE nPending);  // zb 11.8 add
extern void Send_Cmd_To_PS2_Mouse(BYTE PortNum);
extern void Send_Data_To_PS2(BYTE PortNum, BYTE PortData);
extern void Open_Aux(BYTE channel);
extern void Service_Send_PS2(void);
extern void InitAndIdentifyPS2(void);
extern int Send_Aux_Data_To_Host(BYTE auxdata);
extern int AE10x_PS2_Init(void);
extern char Disable_Aux_Data_Reporting(BYTE channel);
extern void PS2_PortN_Init(uint8_t channel);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#endif
