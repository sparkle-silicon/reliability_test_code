/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-02-23 16:50:43
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
#ifndef CUSTOM_POWER_H
#define CUSTOM_POWER_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_PORT6064.H"
#include "KERNEL_PNP.H"
#include "KERNEL_GPIO.H"
//-----------------------------------------------------------------------------
// Define
//-----------------------------------------------------------------------------
#define SYSTEM_S0 0x10
#define SYSTEM_S3 0x33
#define SYSTEM_S4 0x44
#define SYSTEM_S5 0x55
#define SYSTEM_S5_S0 0x50
#define SYSTEM_S4_S0 0x40
#define SYSTEM_S3_S0 0x30
#define SYSTEM_S0_S3 0x03
#define SYSTEM_S0_S4 0x04
#define SYSTEM_S0_S5 0x05
#define SYSTEM_REBOOT 0xAA
#define SYSTEM_EC_WDTRST 0xFF
//-----------------------------------------------------------------------------
// Shutdown cause
//-----------------------------------------------------------------------------
#define SC_S0SLPOff 0x01		// System shutdown by S3/S4/S5 Off
#define SC_S3S4SLPOff 0x02		// System shutdown by S3/S4/S5 fail Off
#define SC_4SEC 0x03			// System shutdown by power button 4 sec overwrite
#define SC_CPUHOT 0x04			// System shutdown by CPU too hot to shutdown
#define SC_HWPGSx_S0Off 0x05	// System shutdown by HWPG Fail in Sx_S0 power sequence
#define SC_HWPS0Off 0x06		// System shutdown by HWPG Fail in S0
#define SC_PowerOnWatchDog 0x07 // System shutdown by Power on WDT
#define SC_EC_0x0000 0xEC	// EC code reset
#define SC_ExtWatchDog 0xFF // external watch-dog
#define SC_IntWatchDog 0xFE // internal watch-dog
#define SC_ECColdBoot 0xFC	// EC VSTBY or WRST reset
//-----------------------------------------------------------------------------
// System status define
//-----------------------------------------------------------------------------
#define SystemIsS0 System_PowerState == SYSTEM_S0
#define SystemIsS3 System_PowerState == SYSTEM_S3
#define SystemIsS4 System_PowerState == SYSTEM_S4
#define SystemIsS5 System_PowerState == SYSTEM_S5
#define SystemNotS0 System_PowerState != SYSTEM_S0
#define SystemNotS3 System_PowerState != SYSTEM_S3
#define SystemNotS4 System_PowerState != SYSTEM_S4
#define SystemNotS5 System_PowerState != SYSTEM_S5
//-----------------------------------------------------------------------------
// Power On Sequence Time-Out Condition
//-----------------------------------------------------------------------------
#define T_HWPG 500 // 500ms (timebase: 1ms)
//-----------------------------------------------------------------------------
// 时序检测输入状态等待时间(ms)
//-----------------------------------------------------------------------------
#define Wait_Time 500
#define Wait_until_Correct 0
//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------
extern void PowerState_Monitor(void);
extern void Custom_S3_S0_Trigger(void);
extern void Custom_S4_S0_Trigger(void);
extern void Custom_S5_S0_Trigger(void);
extern void Custom_S5S0_Sequence(void);
extern void Custom_S4S0_Sequence(void);
extern void Custom_S3S0_Sequence(void);
extern void HWPG_Monitor(void);
extern void Sys_PowerState_Control(void);
extern void Custom_S0_S3_Trigger(BYTE causecode);
extern void Custom_S0_S4_Trigger(BYTE causecode);
extern void Custom_S0_S5_Trigger(BYTE causecode);
extern void Custom_S0S3_Sequence(void);
extern void Custom_S0S4_Sequence(void);
extern void Custom_S0S5_Sequence(void);
extern void ResetSource_Monitor(void);
extern BYTE S5_S0_Variable(void);
extern BYTE S4_S0_Variable(void);
extern BYTE S3_S0_Variable(void);
extern BYTE S0_S5_Variable(void);
extern BYTE S0_S4_Variable(void);
extern BYTE S0_S3_Variable(void);
extern void Sleep_S3_Monitor(void);
extern void Sleep_S4_Monitor(void);
extern void Sleep_S5_Monitor(void);
extern void SB_PowerButton_Enable(void);
extern void SB_PowerButton_Monitor(void);
extern void PWRSW_Config(BYTE timeout, BYTE mode);
extern void PWRSW_Rstoutputen(void);
extern void Copy_Power_Sequence_Record_To_Sram(VBYTE Record_Flag);
extern void AutoON_Check_AfterUpdate(void);
extern void CHIPRESET_Check_AfterUpdate(void);
extern BYTE Set_AutoON_AfterUpdate(void);
extern BYTE PF_CHARGE_LED_ON(void);
extern BYTE PF_CHARGE_LED_OFF(void);
extern BYTE PF_CHARGE_FULL_LED_ON(void);
extern BYTE PF_CHARGE_FULL_LED_OFF(void);
extern void PowerChange_Var_Clear(void);
extern void Hook_EC_ACK_eSPI_Reset(void);
typedef struct PowerSEQ
{
	FUNCT_PTR_B_V func; // Do step function
	WORD delay;			// delay time (ms)
	BYTE checkstatus;	// step function is check pin status (EX: slp_s3 HWPG ...)
} sPowerSEQ;
typedef struct PowSeq_Pin_Record
{
	BYTE pin_group : 3;
	BYTE pin_location : 5;
	BYTE pin_level : 1;
	WORD pin_delay;
} sPowSeq_Pin_Record;
#endif
