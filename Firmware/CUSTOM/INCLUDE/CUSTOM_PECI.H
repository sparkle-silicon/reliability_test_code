/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-02-07 16:56:51
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
#ifndef CUSTOM_PECI_H
#define CUSTOM_PECI_H
#include "KERNEL_PECI.H"

#ifndef SUPPORT_PECI
#define SUPPORT_PECI TRUE
#endif

/*-----------------------------------------------------------------------------
 * Parameter & Label Process Definition
 *---------------------------------------------------------------------------*/
#define _PECI_HostID 0x01
#define PECI_TIMEOUT 30000
//-----------------------------------------------------------------------------
// Define CPU address
//-----------------------------------------------------------------------------
#define _PECI_CPU_ADDR 0x30
#define _PECI_CPU_ADDR_TEST 0x0C

//-----------------------------------------------------------------------------
// Define PECI command
//-----------------------------------------------------------------------------
#define _PECI_CMD_GetDIB 0xF7
#define _PECI_CMD_Ping 0x00
#define _PECI_CMD_GetTemp 0x01
#define _PECI_CMD_RdPkgConfig 0xA1
#define _PECI_CMD_WrPkgConfig 0xA5
#define _PECI_CMD_RdIAMSR 0xB1
#define _PECI_CMD_WrIAMSR 0xB5
#define _PECI_CMD_RdPCIConfig 0x61
#define _PECI_CMD_WrPCIConfig 0x65
#define _PECI_CMD_RdPCIConfigLocal 0xE1
#define _PECI_CMD_WrPCIConfigLocal 0xE5
/* Define completion code */
#define _PECI_CC_Valid 0x40
#define _PECI_CC_TimeOut 0x80
#define _PECI_CC_Ignored 0x90

// ----------------------------------------------------------------------------
// PECI3.0 domain parameter definition
// ----------------------------------------------------------------------------
#define _PECI_Domain_0 0x00
#define _PECI_Domain_1 0x01

// ----------------------------------------------------------------------------
// The index definition of PECI3.0 read/write Pkg command
// ----------------------------------------------------------------------------
#define _PECI_Index_IDENTIFIER 0x00 // Package identifier read
#define _PECI_Index_PTR 0x02        // Package temperature read
#define _PECI_Index_TTR 0x10        // Read max. Allowed Processor temper.
#define _PECI_Index_DRCTR 0x16      // DRAM rank channel temperature read
#define _PECI_Index_PLANE 0x19      // Per Plane Power Limit
#define _PECI_Index_PPL1 0x1A       // Power limit 1
#define _PECI_Index_PPL2 0x1B       // Power limit 2
#define _PECI_Index_PPSU 0x1C       // Power SKU unit
#define _PECI_Index_PPSL 0x1D       // Package power and time limit info.
#define _PECI_Index_PPSH 0x1E       // Package power and time limit info.
#define _PECI_Index_PPL3 0x39       // Power limit 3
#define _PECI_Index_PPL4 0x3C       // Power limit 4

//*****************************************************************************
// Star Reg BIT
//*****************************************************************************
#define PECI_HOSTAR_HOBY BIT0
#define PECI_HOSTAR_FINISH BIT1
#define PECI_HOSTAR_RD_FCS_ERR BIT2
#define PECI_HOSTAR_WR_FCS_ERR BIT3
#define PECI_HOSTAR_LSB_FIRST BIT4
#define PECI_HOSTAR_EXTERR BIT5
#define PECI_HOSTAR_BUSERR BIT6
#define PECI_HOSTAR_RCV_ERRCODE BIT7

//*****************************************************************************
// Ctrl Reg BIT
//*****************************************************************************
#define PECI_HOCTLR_START BIT0
#define PECI_HOCTLR_AWFCS_EN BIT1
#define PECI_HOCTLR_CONCTRL BIT2
#define PECI_HOCTLR_PECIHEN BIT3
#define PECI_HOCTLR_FCSERR_ABT BIT4
#define PECI_HOCTLR_FIFOCLR BIT5
#define PECI_HOCTLR_AWFCS_FRC_CTRL BIT6
#define PECI_HOCTLR_HHRAE BIT7
//*****************************************************************************
// Global variable (memory address allocation follow project )
//*****************************************************************************
#if PECI_Softwave_AWFCS
extern u8 PECI_CRC8;
#endif

//*****************************************************************************
// Define PECI command
//*****************************************************************************
#define PECI_CMD_GetDIB 0xF7
#define PECI_CMD_Pin 0x00
#define PECI_CMD_GetTemp 0x01
#define PECI_CMD_RdPkgConfig 0xA1
#define PECI_CMD_WrPkgConfig 0xA5
#define PECI_CMD_RdIAMSR 0xB1
#define PECI_CMD_WrIAMSR 0xB5
#define PECI_CMD_RdPCIConfig 0x61
#define PECI_CMD_WrPCIConfig 0x65
#define PECI_CMD_RdPCIConfigLocal 0xE1
#define PECI_CMD_WrPCIConfigLocal 0xE5

//*****************************************************************************
// Define completion code
//*****************************************************************************
#define PECI_CC_Valid 0x40
#define PECI_CC_TimeOut 0x80
#define PECI_CC_Ignored 0x90

//*****************************************************************************
// Variable prototype
//*****************************************************************************
extern BYTE PECI_Get_DIB[];
extern BYTE PECI_Get_DIB_TEST[];
extern BYTE PECI_Get_Temp[];
extern BYTE PECI_RdPkgCfg_Idx16[];
#define Processor_Tj_max PECI_RdPkgCfg_Idx16[2]
#define Processor_T_control PECI_RdPkgCfg_Idx16[1]
extern BYTE PECI_ReadBuffer[];
extern BYTE PECI_WriteBuffer[];

//*****************************************************************************
// Function prototype
//*****************************************************************************
extern BYTE PECI_ReadDIB(void);
extern BYTE PECI_ReadTemp(void);
extern BYTE PECI_ReadPowerUnit(void);
extern BYTE PECI_ReadPowerLimit1(void);
extern BYTE PECI_ReadPowerLimit2(void);
extern BYTE PECI_WritePowerLimit1(BYTE SettingWatts, BYTE SettingTimer);
extern BYTE PECI_WritePowerLimit2(BYTE SettingWatts);
extern BYTE PECI_ReadPowerLimit3(void);
extern BYTE PECI_WritePowerLimit3(BYTE SettingWatts);
extern BYTE PECI_ReadPowerLimit4(void);
extern BYTE PECI_WritePowerLimit4(BYTE SettingWatts);
extern void Calc_CPU_Temperature(void);
extern void peci_test(void);
extern void Service_PECI(void);
extern void Init_PECI(void);
extern void PECI_Set_Pad(u16 VTTlev, u8 PADsel);
extern BYTE PECI_ReadDIB_TEST(void);
extern BYTE PECI_PING_TEST(void);
extern BYTE PECI_ReadTemp_TEST(void);
extern BYTE PECI_RdPkgConfig_TEST(void);
#endif