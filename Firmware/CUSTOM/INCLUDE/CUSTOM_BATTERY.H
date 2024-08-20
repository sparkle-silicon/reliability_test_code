/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:20:02
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
#ifndef CUSTOM_BATTERY_H
#define CUSTOM_BATTERY_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
#include "KERNEL_ACPI.H"
#include "KERNEL_I2C.H"
#include "KERNEL_TIMER.H"
/*-----------------------------------------------------------------------------
 * Parameter Definition
 *---------------------------------------------------------------------------*/
#define SmartCharger_Support	TRUE   // True : using charger IC for battery charge control
#define BAT1FuncSupport         TRUE   // True : support battery 1 management function
#define BAT_I2C_CHANNEL		1		   //选择电池SMBUS通道
#define CHG_I2C_CHANNEL		1		   //选择充电SMBUS通道
#define BAT_ADDR			0x0B
#define CHARGER_ADDR		0x09
#define DisablePower 0
#define PDPowerMode	1
#define ACPowerMode	2
//BAT Cmd
#define BATCmd_access           0x00  	// manufacture access
#define BATCmd_RCalarm          0x01 	// remaining capacity alarm
#define BATCmd_RTalarm          0x02   	// remaining time alarm
#define BATCmd_mode             0x03  	// battery mode
#define BATCmd_AtRate           0x04   	// AtRate
#define BATCmd_AtRateFull       0x05  	// AtRate time to full
#define BATCmd_AtRateEmpty      0x06  	// AtRate time to empty
#define BATCmd_AtRateOK         0x07   	// AtRate OK
#define BATCmd_temp             0x08   	// temperature
#define BATCmd_volt             0x09   	// voltage
#define BATCmd_current          0x0A   	// current
#define BATCmd_AVcurrent        0x0B   	// average current
#define BATCmd_MaxError         0x0C  	// maximum error
#define BATCmd_RSOC             0x0D  	// relative state of charge
#define BATCmd_ASOC             0x0E   	// absolute state of charge
#define BATCmd_RMcap            0x0F   	// remaining capacity
#define BATCmd_FCcap            0x10   	// full charge capacity
#define BATCmd_RunTimeEmpty     0x11 	// run time to empty
#define BATCmd_AVTimeEmpty      0x12  	// average time to empty
#define BATCmd_AVTimeFull       0x13   	// average time to full
#define BATCmd_CC               0x14  	// charging current
#define BATCmd_CV               0x15   	// charging voltage
#define BATCmd_BatStatus        0x16   	// battery status
#define BATCmd_CycleCount       0x17   	// cycle count
#define BATCmd_DCap             0x18   	// design capacity
#define BATCmd_DVolt            0x19   	// design voltage
#define BATCmd_Spec             0x1A 	// spec information
#define BATCmd_Date             0x1B   	// manufacture date
#define BATCmd_SerialNo         0x1C   	// serial number
#define BATCmd_Mname            0x20 	// manufacturer name
#define BATCmd_Dname            0x21  	// device name
#define BATCmd_Dchem            0x22  	// device chemistry
#define BATCmd_Mdata            0x23 	// manufacture data
#define InputCurrentSet         0x7F00	//3456mA
#define BAT_VOLT_WUCHG         8400//0x4000	//16384mV
#define BAT_CURR_WUCHG		   384//0x0180	//384mA
#define BAT_VOLT_PRECHG        4200*4//0x4000
#define BAT_CURR_PRECHG        384//0x0400	//1024mA
#define BAT_VOLT_NMCHG         4200*4//0x4000
//BAT1_CC_L = 0x0300;	 // 768mA
//BAT1_CC_L = 0x0200;	 // 512mA
//BAT1_CC_L = 0x03C0;	 // 960mA
//BAT1_CC_L = 0x0400;	 // 1024mA
//BAT1_CC_L = 0x0800;	 // 2048mA
//BAT1_CC_L = 0x0BB8;	 // 3000mA
//BAT1_CC_L = 0x1388;	 // 5000mA
//#define BAT_CURR_NMCHG         0x0E00//3500mA
#define BAT_CURR_NMCHG         0x0800//2000mA
#define Bat1_PC2NC_Cap         256    
#define I_20W   			    (1024/2)
#define I_25W                   (1280/2)
#define I_30W				    (1536/2)
#define I_45W   			    (2048/2)
#define I_65W   			    0x7F00
#define I_90W   			    (4096/2)
#define I_130W  			    (6144/2)
#define I_150W  			    (7168/2)
#define IN_CURRENT	            I_65W
#define Charge_Cutoff_Current   0x80 //128mA
#define SC_Current_Sense 		1
#define Step_ID	            	0x01    // Identify main battery
#define Step_WC			    	0x02    // Battery wake up charge
#define Step_PC		        	0x03    // Battery pre-charge
#define Step_NC		        	0x04    // Battery normal charge
#define Step_DC			    	0x05    // Battery discharge
#define BTAlarmTemperature		0x1450		//52.00 Temp Alarm Discharge
#define MFID					0x004d//0x0040
//charger cmd
#define SC85_ChargeOption0		0x12	// 0:Enable charge; 1:inhibit charge
#define SC85_ChargeCurrent		0x14    // Set charge current
#define SC85_ChargeVoltage		0x15    // Set charge voltage
#define SC85_Status				0x20    // Charge Status reg
#define SC85_Prochot_Status		0x21    // Prochot Status reg
#define SC85_InputCurrent		0x3F    // Set input current	
#define SC85_ChargeOption2		0x31	// 0:Enable charge; 1:inhibit charge
#define SC85_Prochot_Option0    0x33	// Prochot Option0() Register
#define SC85_VINREG				0x3D    // Set input voltage 3.2V- 19.520V ,do not need set,this is follow the input ac 
#define SC85_VsysMin			0x3E    // Set 	Vsys min,do not need set,this is follow the Cell_PINS
#define SC85_MFID				0xFE	// Get Manufacturer ID
#define SC85_DeviceID			0xFF	// Get device ID
/*-----------------------------------------------------------------------------
 * Variable Declarations
 *---------------------------------------------------------------------------*/
extern void Power_Switch(void);
extern void GetBatteryINFO(void);
extern void ChargerCenter(void);
extern void BAT1_PlugOutClrVariables(void);
/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Reference Smart Battery Data Specification
 * The following definition summarizes the Smart Battery command set.
 *---------------------------------------------------------------------------*/
#define _SMB_BatteryAddr            0x16
#define _CMD_ManufacturerAccess     0x00
#define _CMD_RemainingCapacityAlarm 0x01
#define _CMD_RemainingTimeAlarm     0x02
#define _CMD_BatteryMode            0x03
#define _CMD_AtRate                 0x04
#define _CMD_AtRateTimeToFull       0x05
#define _CMD_AtRateTimeToEmpty      0x06
#define _CMD_AtRateOK               0x07
#define _CMD_Temperature            0x08
#define _CMD_Voltage                0x09
#define _CMD_Current                0x0A
#define _CMD_AverageCurrent         0x0B
#define _CMD_MaxError               0x0C
#define _CMD_RelativeStateOfCharge  0x0D
#define _CMD_AbsoluteStateOfCharge  0x0E
#define _CMD_RemainingCapacity      0x0F
#define _CMD_FullChargeCapacity     0x10
#define _CMD_RunTimeToEmpty         0x11
#define _CMD_AverageTimeToEmpty     0x12
#define _CMD_AverageTimeToFull      0x13
#define _CMD_ChargingCurrent        0x14
#define _CMD_ChargingVoltage        0x15
#define _CMD_BatteryStatus          0x16
#define _CMD_CycleCount             0x17
#define _CMD_DesignCapacity         0x18
#define _CMD_DesignVoltage          0x19
#define _CMD_SpecificationInfo      0x1A
#define _CMD_ManufactureDate        0x1B
#define _CMD_SerialNumber           0x1C
#define _CMD_ManufacturerName       0x20
#define _CMD_DeviceName             0x21
#define _CMD_DeviceChemistry        0x22
#define _CMD_ManufacturerData       0x23
#define _CMD_OptionalMfgFunction5   0x2F
#define _CMD_OptionalMfgFunction4   0x3C
#define _CMD_OptionalMfgFunction3   0x3D
#define _CMD_OptionalMfgFunction2   0x3E
#define _CMD_OptionalMfgFunction1   0x3F
#define _CMD_ChargerInputCurrent    0x3F
#define A_XBYTE  *(VBYTE *)
#define A_XWORD  *(VWORD *)
//-------------------------------------------------------------------------------
#define BIT_BAT_MODE_IN_CTRL   BIT(8)    // Internal charge control enabled
#define BIT_BAT_MODE_PRIMARY   BIT(9)    // Battery in its primary role
#define BIT_BAT_MODE_DIS_ALARM BIT(13)   // Disable alarm warning broadcast
#define BIT_BAT_MODE_DIS_CHG   BIT(14)   // Disable charge vol & cur broadcast
//-------------------------------------------------------------------------------
// Charge and discharge control step define
//-------------------------------------------------------------------------------
#define BAT_Step_ID	            0x01    // Identify main battery
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define BATID_Step_GetDV        0x01    // First time to get battery design voltage and checking SMBus communication is OK
#define BATID_Step_Auth         0x02    // Battery Authentication if necessary
#define BATID_Step_MFName       0x03    // Get manufacturer name and checking battery is supported
#define BATID_Step_FirstData    0x04    // Polling first data
#define BATID_Step_InitOK       0x05    // Init_OK,prepare Discharge/Pre-Charge. (always at last step)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define BAT_Step_WC			    0x02    // Battery wake up charge
#define BAT_Step_PC		        0x03    // Battery pre-charge
#define BAT_Step_NC		        0x04    // Battery normal charge
#define BAT_Step_DC			    0x05    // Battery discharge
#define BAT_Step_FullyChg       0x10    // Battery fully charged
#define BAT_Step_FullyDisChg    0x11    // Battery fully discharged
#define BAT_Step_SetFail        0x20    // Battery will fail
#define BAT_Step_Fail		    0x21    // Battery abnormal
#define BAT_Step_ForceDC        0x30    // Battery force discharge
#define BAT_Step_ForceC         0x31    // Battery force charge
#define BAT_Step_AutoLN         0x32    // Battery auot learning
#define BAT_Step_ChargerFail    0x40    // Smart charger abnormal
#define BAT_Step_Out		    0x50    // Battery plug out
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//end bat step
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//extern BYTE    BT1_STATUS2;            //(ECRAM+0x61)  Battery 1 status2 flag
#define bat_InCharge_Verse  BIT0	    // 1: battery 1 in any charge phase
#define bat_valid           BIT1   	    // 1: Battery 1 battery idetify OK
#define bat_Dead       		BIT4   	    // 1: Battery 1 fully discharged
#define bat_Full            BIT5   	    // 1: Battery 1 fully charged
#define bat_Dischg   		BIT6   	    // 1: battery 1 discharging
#define bat_InCharge   		BIT7    	    // 1: battery 1 in any charge phase
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern BYTE    BT1_STATUS2;            //(ECRAM+0x61)  Battery 1 status2 flag
#define bat_DC         		BIT0	    // 1: Set Battery 1 discharge
#define bat_WC              BIT1   	    // 1: Set Battery 1 wake up charge
#define bat_PC         		BIT2	    // 1: Set Battery 1 pre-charge
#define bat_NC              BIT3   	    // 1: Set Battery 1 normal charge
#define bat_DC_OK           BIT4	    // 1: Battery 1 in discharge
#define bat_WC_OK           BIT5   	    // 1: Battery 1 in wake up charge
#define bat_PC_OK           BIT6	    // 1: Battery 1 in pre-charge
#define bat_NC_OK           BIT7   	    // 1: Battery 1 in normal charge
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern BYTE    BT1_STATUS3;            //(ECRAM+0x74)  Battery 1
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define bat_ForceChg    BIT0            // Battery 1 in force charge mode   
#define bat_ForceDischg BIT1            // Battery 1 in force discharge mode 
#define bat_AL          BIT2            // Battery 1 in auto learning
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern BYTE    BAT1_ALARM2;            //(ECRAM+0x73)  Battery 1
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define RemTime_alarm	BIT0            // alarmremaining time
#define RemCap_alarm  	BIT1            // alarmremaining capacity
//#define  				BIT2        	//
#define TermDsg_alarm 	BIT3            // alarmterminate discharge
#define OverTemp_alarm 	BIT4            // alarmover temperature
//#define  				BIT5        	//
#define TermChg_alarm 	BIT6            // alarmterminate charge
#define OverChg_alarm 	BIT7            // alarmover charge
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define BATOverVoltage		    18100
#define ChargerSettingTimeOut   3       // re-try counter of setting of smart charger
#define BatIDTimeOut            5       // battery 1 identify re-try counter
#define BatWCTimeOut            60      // timer base is 1min (1hr)
#define BatPCTimeOut            120     // timer base is 1min (2hr)
#define BatNCTimeOut            1200     // timer base is 1min (20hr)
#define BatOVTimeOut            5       // Main battery over voltage 5sec
#define BatOTTimeOut            5       // Main battery over temperature
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Battery Fail cause
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define Bat_WakeUpChargeFail    0x01
#define Bat_PreChargeFail       0x02
#define Bat_NormalChargeFail    0x03
#define Bat_Weak                0x04
#define Bat_OverVoltage         0x05
#define Bat_OverTemperature     0x06
#define Bat_NoSupport           0x07
#define Bat_AuthFail            0x08
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SMBerror0    	BIT0            // error code bit0
#define SMBerror1    	BIT1            // error code bit1
#define SMBerror2      	BIT2            // error code bit2
#define SMBerror3      	BIT3            // error code bit3
#define FullyDsg    	BIT4            // statusfully discharged
#define FullyChg     	BIT5            // statusfully charged
#define Dsg         	BIT6            // statusdischarging
#define Initialized  	BIT7            // statusgauge initialized
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef struct Bat_ecsapce
{
	VBYTE *Var_L;
	VBYTE *Var_H;
}Battery_Ecsapce;

typedef struct BatData
{
	VBYTE Cmd;
	VWORD *Var;
	Battery_Ecsapce Bat_Ecsapce;
}BatteryData;
typedef union _word_read
{
	VWORD data;
	BYTE  data0[2];
}word_read;

extern BYTE BAT1_FailCause;
extern BYTE BAT1_CtrlStep;
extern BYTE Bat_Read_NWait_Flag;
extern void Battery_Static_Mode_Judge(void);
extern uint8_t I2C1_charge_init(void);
extern void Battey1ControlCenter(void);
extern void PollingBAT1Data(void);
extern BYTE CheckEnterDeepSleep(void);
extern void SetBAT1CtrlStep(BYTE ctrlstep);
extern void ChargerWrite(BYTE addr, WORD value);
extern BYTE SetSmartCharger(void);
//-----------------------------------------------------------------------------
#endif
