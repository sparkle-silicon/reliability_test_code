/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
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
#include "KERNEL_GPIO.H"

#define PS2_CLOCK  (HIGH_CHIP_CLOCK / (SYSCTL_CLKDIV_PS2+1)) 
 //-----------------------------------------------------------------------------
 // PS2 TIMEOUT
 //-----------------------------------------------------------------------------
#define PS2_WaitTime 3000
#define DeviceResetCmdITTime 100 // 1000 ms
#define KBCmdBusyInactiveTime 10 // 90 ~ 100 ms
#define InactiveTime 6           // 50 ~ 60 ms
#define CheckDeviceTimeOut 4     // 30 ~ 40 ms
#define SkipScanAUXTime 150      // Skip KBD_SCAN aux devices for keyboard or mouse driver init. (10ms timer base)
#define PS2_OnOffTimeOutCUNT 5
#define KBD_TIMEOUT 1000       /* Timeout in ms for keyboard command acknowledge */
#define KBC_TIMEOUT 250        /* Timeout in ms for sending to keyboard controller */
#define IdentifyTimeOut 10   // IdentifyTimeOut*10 ms
 //-----------------------------------------------------------------------------
 // PS2 Channel
 //-----------------------------------------------------------------------------
#define PS2M_CHANNEL 0
#define PS2K_CHANNEL 1
#define PS2_0_CHANNEL PS2M_CHANNEL
#define PS2_1_CHANNEL PS2K_CHANNEL
 //-----------------------------------------------------------------------------
 // PS2 CMD WORD
 //-----------------------------------------------------------------------------
#define CCMD_MOUSE_DISABLE 0xA7 /* Disable mouse interface */
#define CCMD_MOUSE_ENABLE 0xA8  /* Enable mouse interface */
#define CCMD_KBD_DISABLE 0xAD   /* Disable KBD interface */
#define CCMD_KBD_ENABLE 0xAE    /* Enable KBD interface */
#define CCMD_READ 0x20          /* Read command */
#define CCMD_WRITE 0x60         /* Write command */
#define CCMD_SELFTEST_AA 0xAA      /* Self-test command */
#define CCMD_SELFTEST_AB 0xAB      /* Self-test command */
#define CCMD_RESET 0xFF         /* Reset command */
//#define AUX_INTS_OFF (KBD_MODE_KCC | KBD_MODE_DISABLE_MOUSE | KBD_MODE_SYS | KBD_MODE_KBD_INT)
#define AUX_INTS_ON 0x26
#define KBD_INTS_ON 0x37
#define AUX_ENABLE_DEV 0xF4 /* Enable aux device */
#define KBD_CMD_ENABLE 0xF4 /* Enable scanning */
//-----------------------------------------------------------------------------
// PS2 STATUS
//-----------------------------------------------------------------------------
#define KBD_STAT_GTO 0x40      /* General receive/xmit timeout */
#define KBD_STAT_PERR 0x80     /* Parity error */
#define PS2_STR_OBF 0x2
#define PS2_STR_IBF 0x1
//-----------------------------------------------------------------------------
// PS2 Interface Debug
//-----------------------------------------------------------------------------
#define PS2_Pack_Debug 0
#define PS2_KBCMD 0 // Enable Debug keyboard command to keyboard and ACK bytes
#define PS2_MSCMD 0 // Enable Debug keyboard command to TP or moues and ACK bytes
#define PS2_AUXTypeDebug 0
//-----------------------------------------------------------------------------
// PS2 CR Register define
//-----------------------------------------------------------------------------
#define PS2_CR_MS_EN            HEX20
#define PS2_CR_KB_EN            HEX10
#define PS2_CR_MS_IRQ           HEX02
#define PS2_CR_KB_IRQ           HEX01
#define PSW_CR_DISEN            HEX00
/* KBD_MODE_KCC  0x40   Scan code conversion to PC format */
/* KBD_MODE_SYS  0x04   The system flag */
/* KBD_MODE_MOUSE_INT 0x02 Mouse data generate */
/* KBD_MODE_KBD_INT 0x01   kbd data generate  */
/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
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
extern void Handle_Kbd(BYTE channel);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#endif
