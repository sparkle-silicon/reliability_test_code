/*
 * @Author: Iversu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2024-03-07 15:18:41
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
//-----------------------------------------------------------------------------
// Include Definition
//-----------------------------------------------------------------------------
#ifndef CUSTOM_MEMORY_H
#define CUSTOM_MEMORY_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
/****************************************************************
--------------------- MEMORY SECTION 3 --------------------------
-------------------- EC SPACE DEFINITION ------------------------
****************************************************************/
#define ECSPACE_BASE_ADDR MEMORY_BASE2_ADDR
#if SUPPORT_ANX7447
//for anx7447
#define xEC_MainVersion REG8(ECSPACE_BASE_ADDR + 0x00) 
#define xEC_SubVersion REG8(ECSPACE_BASE_ADDR + 0x01) 
#define xEC_KBCVersion REG8(ECSPACE_BASE_ADDR + 0x02) 
#define xEC_TestVersion REG8(ECSPACE_BASE_ADDR + 0x03) 
#define xEC_PDPort0MainVersion REG8(ECSPACE_BASE_ADDR + 0x04) 
#define xEC_PDPort0SubVersion REG8(ECSPACE_BASE_ADDR + 0x05) 
#define xEC_PDPort1MainVersion REG8(ECSPACE_BASE_ADDR + 0x06) 
#define xEC_PDPort1SubVersion REG8(ECSPACE_BASE_ADDR + 0x07) 
// for mode and version
#define EC_VERSION_MINOR xEC_MainVersion  // Version L
#define EC_VERSION_SENIOR xEC_SubVersion // Version H
#define EC_RESET_MODE REG8(ECSPACE_BASE_ADDR + 0x08)
#define EC_S3_TIMEOUT_L REG8(ECSPACE_BASE_ADDR + 0x09)
#define EC_S3_TIMEOUT_H REG8(ECSPACE_BASE_ADDR + 0x0A)

// for acpi smb
#define ACPI_SMB_PROTOCOL REG8(ECSPACE_BASE_ADDR + 0x0B)
#define ACPI_SMB_STATUS REG8(ECSPACE_BASE_ADDR + 0x0C)
#define ACPI_SMB_ADDRESS REG8(ECSPACE_BASE_ADDR + 0x0D)
#define ACPI_SMB_COMMAND REG8(ECSPACE_BASE_ADDR + 0x0E)
#define ACPI_SMB_DATA REG8(ECSPACE_BASE_ADDR + 0x0F)
#else
// for mode and version
#define EC_VERSION_MINOR REG8(ECSPACE_BASE_ADDR + 0x00)  // Version L
#define EC_VERSION_SENIOR REG8(ECSPACE_BASE_ADDR + 0x01) // Version H
#define EC_RESET_MODE REG8(ECSPACE_BASE_ADDR + 0x02)
#define EC_S3_TIMEOUT_L REG8(ECSPACE_BASE_ADDR + 0x03)
#define EC_S3_TIMEOUT_H REG8(ECSPACE_BASE_ADDR + 0x04)
// for acpi smb
#define ACPI_SMB_PROTOCOL REG8(ECSPACE_BASE_ADDR + 0x05)
#define ACPI_SMB_STATUS REG8(ECSPACE_BASE_ADDR + 0x06)
#define ACPI_SMB_ADDRESS REG8(ECSPACE_BASE_ADDR + 0x07)
#define ACPI_SMB_COMMAND REG8(ECSPACE_BASE_ADDR + 0x08)
#define ACPI_SMB_DATA REG8(ECSPACE_BASE_ADDR + 0x09)
// #define     ACPI_SMB_BCNT                       REG8(ECSPACE_BASE_ADDR+0x0A)

#endif
// for battery
// for battery
#define BATTERY_RSOC REG8(ECSPACE_BASE_ADDR + 0x10)   // Battery  battery_remain_percent
#define BATTERY_FCAP_L REG8(ECSPACE_BASE_ADDR + 0x11) // Battery  battery_charge
#define BATTERY_FCAP_H REG8(ECSPACE_BASE_ADDR + 0x12)
#define BATTERY_RMCAP_L REG8(ECSPACE_BASE_ADDR + 0x13) // Battery  battery_remain
#define BATTERY_RMCAP_H REG8(ECSPACE_BASE_ADDR + 0x14)
#define BATTERY_TEMP_L REG8(ECSPACE_BASE_ADDR + 0x15) // Battery  battery_temp
#define BATTERY_TEMP_H REG8(ECSPACE_BASE_ADDR + 0x16)
#define BATTERY_VOL_L REG8(ECSPACE_BASE_ADDR + 0x17) // Battery  battery_voltage
#define BATTERY_VOL_H REG8(ECSPACE_BASE_ADDR + 0x18)
#define BATTERY_CURRENT_L REG8(ECSPACE_BASE_ADDR + 0x19) // Battery  battery_current
#define BATTERY_CURRENT_H REG8(ECSPACE_BASE_ADDR + 0x1A)
#define BATTERY_ACURRENT_L REG8(ECSPACE_BASE_ADDR + 0x1B) // Battery  AverageCurrent
#define BATTERY_ACURRENT_H REG8(ECSPACE_BASE_ADDR + 0x1C)
#define BATTERY_DCAP_L REG8(ECSPACE_BASE_ADDR + 0x1D) // Battery  battery_design_charge
#define BATTERY_DCAP_H REG8(ECSPACE_BASE_ADDR + 0x1E)
#define BATTERY_DVOL_L REG8(ECSPACE_BASE_ADDR + 0x1F) // Battery  battery_design_voltage
#define BATTERY_DVOL_H REG8(ECSPACE_BASE_ADDR + 0x20)
#define BATTERY_STATUS REG8(ECSPACE_BASE_ADDR + 0x21) // Battery   battery_status
#define BATTERY_SN_L REG8(ECSPACE_BASE_ADDR + 0x22)   // Battery SerialNumber
#define BATTERY_SN_H REG8(ECSPACE_BASE_ADDR + 0x23)
#define BATTERY_CCURRENT_L REG8(ECSPACE_BASE_ADDR + 0x24) // Battery  charge current L
#define BATTERY_CCURRENT_H REG8(ECSPACE_BASE_ADDR + 0x25) // Battery  charge current H
#define BATTERY_CVOLTAGE_L REG8(ECSPACE_BASE_ADDR + 0x26) // Battery  charge voltage L
#define BATTERY_CVOLTAGE_H REG8(ECSPACE_BASE_ADDR + 0x27) // Battery  charge voltage H
#define BATTERY_MODE_L REG8(ECSPACE_BASE_ADDR + 0x28)     // Battery  mode L
#define BATTERY_MODE_H REG8(ECSPACE_BASE_ADDR + 0x29)     // Battery  mode H
#define BATTERY_ALARM REG8(ECSPACE_BASE_ADDR + 0x2A)      // Battery  alarm
#define BATTERY_STATUS1 REG8(ECSPACE_BASE_ADDR + 0x2B)      // Battery  status
#define BATTERY_STATUS2 REG8(ECSPACE_BASE_ADDR + 0x2C)      // Battery  status
#define BATTERY_STATUS3 REG8(ECSPACE_BASE_ADDR + 0x2D)      // Battery  status
// for bright
#define BRIGHT_LEVEL_CTRL REG8(ECSPACE_BASE_ADDR + 0x30) // Brightness  level
#define BRIGHT_LEVEL REG8(ECSPACE_BASE_ADDR + 0x31)      // Brightness current level
// for temperature
#define DIMM0_TEMP DATA8(ECSPACE_BASE_ADDR + 0x38)    // DRAM 0 temperature
#define DIMM1_TEMP DATA8(ECSPACE_BASE_ADDR + 0x39)    // DRAM 1 temperature
#define SYSTEM_TEMP DATA8(ECSPACE_BASE_ADDR + 0x3A)   // system Temperature
#define CPU_TEMP DATA8(ECSPACE_BASE_ADDR + 0x3B)      // CPU Temperature
#define GPU_TEMP DATA8(ECSPACE_BASE_ADDR + 0x3C)      // GPU Temperature
#define PECI_CPU_TEMP DATA8(ECSPACE_BASE_ADDR + 0x3D) // Use PECI interface to read CPU temperature
// for fan
//  #define     EC_CPU_FAN_DRIVE_LEVEL		        REG8(ECSPACE_BASE_ADDR+0x40)
//  #define     EC_GPU_FAN_DRIVE_LEVEL		        REG8(ECSPACE_BASE_ADDR+0x41)
//  #define     EC_CPU_FAN_RPM_L				    REG8(ECSPACE_BASE_ADDR+0x42)
//  #define     EC_CPU_FAN_RPM_H				    REG8(ECSPACE_BASE_ADDR+0x43)
//  #define     EC_GPU_FAN_RPM_L				    REG8(ECSPACE_BASE_ADDR+0x44)
//  #define     EC_GPU_FAN_RPM_H				    REG8(ECSPACE_BASE_ADDR+0x45)
// for control flag
#define CRISIS_KEY_STATUS REG8(ECSPACE_BASE_ADDR + 0x48) // Crisis key flag
#define CTRL_FLAG1 REG8(ECSPACE_BASE_ADDR + 0x49)        // EC power control  flag 1
                                                         //---------------------------------------------------------------------------------
#define adapter_in BIT0                                  // 1 AC adapter exist
#define power_on BIT1                                    // 1 system power on
#define enter_S3 BIT2                                    // 1 host enter suspend to RAM mode
#define pwrbtn_press BIT3                                // 1 waiting for PSW released
#define enter_S4 BIT4                                    // 1 host enter suspend to HD mode
#define system_on BIT5                                   // 1 system on (SWPG issued)
#define bat_in BIT6                                      // 1 battery in
#define rvcc_on BIT7                                     // 1 RVCC turnned on
//---------------------------------------------------------------------------------
#define CTRL_FLAG2 REG8(ECSPACE_BASE_ADDR + 0x50) // EC power control flag 2
                                                  //---------------------------------------------------------------------------------
#define SLEEP_S3_ON BIT0                          // 1:S3 power status
#define SLEEP_S4_ON BIT1                          // 1:S4 power status
#define SLEEP_S5_ON BIT2                          // 1:S5 power status
//---------------------------------------------------------------------------------
#define CTRL_FLAG3 REG8(ECSPACE_BASE_ADDR + 0x58)   // EC power control flag 3
#define SYSTEM_MISC1 REG8(ECSPACE_BASE_ADDR + 0x59) // System Misc flag 1
                                                    //---------------------------------------------------------------------------------
                                                    // #define     T_sensor0       BIT(0x00)
                                                    // #define     T_sensor1       BIT(0x01)
#define ACPI_OS BIT(2)
// #define     T_sensor2       BIT(0x03)
// #define     EC_bank0        BIT(0x04)
// #define     EC_bank1        BIT(0x05)
// #define     EC_bank2        BIT(0x06)
// #define     EC_bank3        BIT(0x07)
//---------------------------------------------------------------------------------
#define SYSTEM_MISC2 REG8(ECSPACE_BASE_ADDR + 0x60) // System Misc flag 2
#define SYSTEM_MISC3 REG8(ECSPACE_BASE_ADDR + 0x61) // System Misc flag 3
#define EVENT_FLAG1 REG8(ECSPACE_BASE_ADDR + 0x62)  // Event status flag 1
                                                    //---------------------------------------------------------------------------------
#define DummyFlag BIT(0x00)
//---------------------------------------------------------------------------------
#define EVENT_FLAG2 REG8(ECSPACE_BASE_ADDR + 0x68) // Event status flag 2
#define EVENT_FLAG3 REG8(ECSPACE_BASE_ADDR + 0x69) // Event status flag 3

#if SUPPORT_ANX7447
#define xUCSI_DS_VERSION REG8(ECSPACE_BASE_ADDR + 0xC0) 
#define xUCSI_DS_RESERVED REG8(ECSPACE_BASE_ADDR + 0xC2) 
#define xUCSI_DS_CCI0 REG8(ECSPACE_BASE_ADDR + 0xC4) 
#define xUCSI_DS_CCI1 REG8(ECSPACE_BASE_ADDR + 0xC5) 
#define xUCSI_DS_CCI2 REG8(ECSPACE_BASE_ADDR + 0xC6) 
#define xUCSI_DS_CCI3 REG8(ECSPACE_BASE_ADDR + 0xC7) 
#define xUCSI_DS_CTL0 REG8(ECSPACE_BASE_ADDR + 0xC8) 
#define xUCSI_COMMAND xUCSI_DS_CTL0
#define xUCSI_DS_CTL1 REG8(ECSPACE_BASE_ADDR + 0xC9) 
#define xUCSI_DS_CTL2 REG8(ECSPACE_BASE_ADDR + 0xCA) 
#define xUCSI_DS_CTL3 REG8(ECSPACE_BASE_ADDR + 0xCB) 
#define xUCSI_DS_CTL4 REG8(ECSPACE_BASE_ADDR + 0xCC) 
#define xUCSI_DS_CTL5 REG8(ECSPACE_BASE_ADDR + 0xCD) 
#define xUCSI_DS_CTL6 REG8(ECSPACE_BASE_ADDR + 0xCE) 
#define xUCSI_DS_CTL7 REG8(ECSPACE_BASE_ADDR + 0xCF) 
#define xUCSI_DS_MGI (REG8(ECSPACE_BASE_ADDR + 0xD0))
#define xUCSI_DS_MGO (REG8(ECSPACE_BASE_ADDR + 0xE0))
//ERR_CMD_UNSUCCESS_DUE_TO_DEAD_BATTERY
// #define xEC_Bt1RSOC BATTERY_RSOC

#else
#define xUCSI_DS_VERSION REG8(ECSPACE_BASE_ADDR + 0x70) 
#define xUCSI_DS_RESERVED REG8(ECSPACE_BASE_ADDR + 0x72) 
#define xUCSI_DS_CCI0 REG8(ECSPACE_BASE_ADDR + 0x74)
#define xUCSI_DS_CCI1 REG8(ECSPACE_BASE_ADDR + 0x75)
#define xUCSI_DS_CCI2 REG8(ECSPACE_BASE_ADDR + 0x76)
#define xUCSI_DS_CCI3 REG8(ECSPACE_BASE_ADDR + 0x77)
#define xUCSI_DS_CTL0 REG8(ECSPACE_BASE_ADDR + 0x78)
#define xUCSI_COMMAND xUCSI_DS_CTL0
#define xUCSI_DS_CTL1 REG8(ECSPACE_BASE_ADDR + 0x79)
#define xUCSI_DS_CTL2 REG8(ECSPACE_BASE_ADDR + 0x7A)
#define xUCSI_DS_CTL3 REG8(ECSPACE_BASE_ADDR + 0x7B)
#define xUCSI_DS_CTL4 REG8(ECSPACE_BASE_ADDR + 0x7C)
#define xUCSI_DS_CTL5 REG8(ECSPACE_BASE_ADDR + 0x7D)
#define xUCSI_DS_CTL6 REG8(ECSPACE_BASE_ADDR + 0x7E)
#define xUCSI_DS_CTL7 REG8(ECSPACE_BASE_ADDR + 0x7F)
#define xUCSI_DS_MGI REG8(ECSPACE_BASE_ADDR + 0x80) 
#define xUCSI_DS_MGO REG8(ECSPACE_BASE_ADDR + 0x90)

#define xEC_MainVersion REG8(ECSPACE_BASE_ADDR + 0xA0) 
#define xEC_SubVersion REG8(ECSPACE_BASE_ADDR + 0xA1) 
#define xEC_KBCVersion REG8(ECSPACE_BASE_ADDR + 0xA2) 
#define xEC_TestVersion REG8(ECSPACE_BASE_ADDR + 0xA3) 
#define xEC_PDPort0MainVersion REG8(ECSPACE_BASE_ADDR + 0xA4)
#define xEC_PDPort0SubVersion REG8(ECSPACE_BASE_ADDR + 0xA5)
#define xEC_PDPort1MainVersion REG8(ECSPACE_BASE_ADDR + 0xA6)
#define xEC_PDPort1SubVersion REG8(ECSPACE_BASE_ADDR + 0xA7)

#define xEC_TSR1 REG8(ECSPACE_BASE_ADDR + 0xA8) 
#define xEC_TSR1H REG8(ECSPACE_BASE_ADDR + 0xA9) 
#define xEC_TESTS_ON REG8(ECSPACE_BASE_ADDR + 0xAA) 
#define xEC_TESTS_OFF REG8(ECSPACE_BASE_ADDR + 0xAB) 

#endif
#define DBG_Info_Base(offset) *(VBYTEP)(MEMORY_BASE3_ADDR +offset) // 32BYTE reserverd for debug information
#endif
