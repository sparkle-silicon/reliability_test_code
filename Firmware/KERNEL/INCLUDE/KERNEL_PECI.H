/*
 * @Author: Iversu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2025-10-18 21:06:17
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

#ifndef KERNEL_PECI_H
#define KERNEL_PECI_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"  
#include "AE_CONFIG.H"
#include "AE_PRINTF.H"
#include "AE_MALLOC.H"

#define PECI_CLOCK  (HIGH_CHIP_CLOCK / (SYSCTL_CLKDIV_PECI+1)) 

 /*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/
#define PECI_CLKDIV_1 1
#define PECI_CLKDIV_2 2
#define PECI_CLKDIV_4 4
#define PECI_CLKDIV_8 8

#define PMU_EN_IBIAS 0x00000100UL
#define PMU_EN_BUF 0x00000200UL
#define PECI_RESERVED_REG 0x00000001UL

#define VTTlev_110V 0b00    //peci寄存器控制方式设置peci pad电压为1.10v
#define VTTlev_105V 0b01    //peci寄存器控制方式设置peci pad电压为1.05v
#define VTTlev_180V 0b10    //peci寄存器控制方式设置peci pad电压为1.80v
#define VTTlev_125V 0b11    //peci寄存器控制方式设置peci pad电压为1.25v

#define SYSCTL_PECI_VTT_080V 0b0000 //sysctl寄存器控制方式设置peci pad电压为0.8v
#define SYSCTL_PECI_VTT_085V 0b0001
#define SYSCTL_PECI_VTT_090V 0b0010
#define SYSCTL_PECI_VTT_095V 0b0011
#define SYSCTL_PECI_VTT_100V 0b0100
#define SYSCTL_PECI_VTT_105V 0b0101
#define SYSCTL_PECI_VTT_110V 0b0110
#define SYSCTL_PECI_VTT_115V 0b0111
#define SYSCTL_PECI_VTT_120V 0b1000
#define SYSCTL_PECI_VTT_125V 0b1001
#define SYSCTL_PECI_VTT_130V 0b1010
#define SYSCTL_PECI_VTT_135V 0b1011

#define PADSEL_SYSCTL 1   //选择sysctl寄存器设置pad电压
#define PADSEL_PECI 0     //选择peci寄存器设置pad电压

#define TRANSFER_RATE_1_8MHz 0b000  //传输速率
#define TRANSFER_RATE_1_0MHz 0b001
#define TRANSFER_RATE_500KHz 0b010
#define TRANSFER_RATE_250KHz 0b011
#define TRANSFER_RATE_1_5MHz 0b100

#define PECI_HOST_ENABLE 0x08   //主机使能

#define PECI_VTT_SEL 0x00000080UL

 /*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/
#define SUPPORT_PECI_SUBROUTINE TRUE
#define SUPPORT_PECI_POWER_CTRL TRUE
#define SUPPORT_SOFTWARE_AWFCS FALSE

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
#define PECI_STAR_HOBY BIT0
#define PECI_STAR_FINISH BIT1
#define PECI_STAR_RD_FCS_ERR BIT2
#define PECI_STAR_WR_FCS_ERR BIT3
#define PECI_STAR_LSB_FIRST BIT4
#define PECI_STAR_EXTERR BIT5
#define PECI_STAR_BUSERR BIT6
#define PECI_STAR_RCV_ERRCODE BIT7

//*****************************************************************************
// Ctrl Reg BIT
//*****************************************************************************
#define PECI_CTRL1R_START BIT0
#define PECI_CTRL1R_AWFCS_EN BIT1
#define PECI_CTRL1R_CONCTRL BIT2
#define PECI_CTRL1R_PECIHEN BIT3
#define PECI_CTRL1R_FCSERR_ABT BIT4
#define PECI_CTRL1R_FIFOCLR BIT5
#define PECI_CTRL1R_AWFCS_FRC_CTRL BIT6
#define PECI_CTRL1R_HHRAE BIT7
//*****************************************************************************
// Global variable (memory address allocation follow project )
//*****************************************************************************
#if PECI_Softwave_AWFCS
extern u8 PECI_CRC8;
#endif

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
// Selection
//*****************************************************************************
#define PECI_Softwave_AWFCS         1       // 1 : softwave calculate AW_FCS
                                            // 0 : H.W. calculate AW_FCS
#define PECI_HostID                 0x01    // Host ID = PECI_HostID << 1 (bit7 ~ bit0)
#define PECI_TIMEOUT                30000
//*****************************************************************************
// Star Reg BIT
//*****************************************************************************
#define PECI_STAR_HOBY            BIT0
#define PECI_STAR_FINISH          BIT1
#define PECI_STAR_RD_FCS_ERR      BIT2
#define PECI_STAR_WR_FCS_ERR      BIT3
#define PECI_STAR_LSB_FIRST       BIT4
#define PECI_STAR_EXTERR          BIT5
#define PECI_STAR_BUSERR          BIT6
#define PECI_STAR_RCV_ERRCODE     BIT7

//*****************************************************************************
// Ctrl Reg BIT
//*****************************************************************************
#define PECI_CTRL1R_START           BIT0
#define PECI_CTRL1R_AWFCS_EN        BIT1
#define PECI_CTRL1R_CONCTRL         BIT2
#define PECI_CTRL1R_PECIHEN         BIT3
#define PECI_CTRL1R_FCSERR_ABT      BIT4
#define PECI_CTRL1R_FIFOCLR         BIT5
#define PECI_CTRL1R_AWFCS_FRC_CTRL  BIT6
#define PECI_CTRL1R_HHRAE           BIT7
//*****************************************************************************
// Global variable (memory address allocation follow project )
//*****************************************************************************
#if PECI_Softwave_AWFCS
extern u8 PECI_CRC8;
#endif

//*****************************************************************************
// Define PECI command
//*****************************************************************************
#define PECI_CMD_GetDIB             0xF7
#define PECI_CMD_Pin                0x00
#define PECI_CMD_GetTemp            0x01
#define PECI_CMD_RdPkgConfig        0xA1  
#define PECI_CMD_WrPkgConfig        0xA5  
#define PECI_CMD_RdIAMSR            0xB1
#define PECI_CMD_WrIAMSR            0xB5
#define PECI_CMD_RdPCIConfig        0x61
#define PECI_CMD_WrPCIConfig        0x65
#define PECI_CMD_RdPCIConfigLocal   0xE1
#define PECI_CMD_WrPCIConfigLocal   0xE5

//*****************************************************************************
// Define completion code
//*****************************************************************************
#define PECI_CC_Valid               0x40
#define PECI_CC_TimeOut             0x80
#define PECI_CC_Ignored             0x90

//*****************************************************************************
// Function prototype
//*****************************************************************************
extern void PECI_HostEnable(void);
extern void PECI_HostDisable(void);
extern BYTE PECI_CheckHostBusy(void);
extern BYTE PECI_CheckHostFinish(void);
extern void ResetPECIStatus(void);
extern void PECI_HostControl(BYTE control);
extern BYTE PECI_Ping(BYTE addr, BYTE ReadLen, BYTE WriteLen);
extern BYTE PECI_GetDIB(BYTE addr, BYTE* ReadData, BYTE ReadLen, BYTE WriteLen);
extern BYTE PECI_GetTemp(BYTE addr, BYTE* ReadData, BYTE Domain, BYTE ReadLen, BYTE WriteLen);
extern BYTE PECI_RdPkgConfig(BYTE addr, BYTE* ReadData, BYTE Domain, BYTE Retry, BYTE Index, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen);
extern BYTE PECI_WrPkgConfig(BYTE addr, BYTE* WriteData, BYTE Domain, BYTE Retry, BYTE Index, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen);
extern BYTE PECI_RdIAMSR(BYTE addr, BYTE* ReadData, BYTE Domain, BYTE Retry, BYTE ProcessorID, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen);
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

#endif