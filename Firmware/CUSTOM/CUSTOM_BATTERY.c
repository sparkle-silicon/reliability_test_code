/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-12-21 18:45:54
 * @Description: Platform battery & battery charger control code
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
#include "CUSTOM_BATTERY.H"
#include "CUSTOM_GPIO.H"
#include "KERNEL_MEMORY.H"
#include "CUSTOM_POWER.H"

/*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/
BYTE BAT_CtrlStep;
BYTE BAT_ID_Step;
BYTE BAT_10MsTCunt;
BYTE BAT_SNumber;
VWORD ByteTemp01;
VWORD ByteTemp02;
BYTE BAT_ID_TimeOut;
BYTE BAT_FailCause;
BYTE BAT_FirstDataIndex;
BYTE BAT_PollingIndex;
BYTE Charger_TimeOut;
BYTE WakeUpChrCunt_Min;
VWORD PreChrCunt_Min;
VWORD NormalChrCunt_Min;
VWORD WakeUpChrCunt;
VWORD PreChrCunt;
VWORD NormalChrCunt;
BYTE BAT_OTCnt;//Over-temperature count
BYTE BAT_OVCnt;//Over-voltage count
VWORD CHARGE_CURRENT_BK;
VWORD CHARGE_VOLTAGE_BK;
VWORD INPUT_CURRENT;
VWORD CHARGE_CURRENT;
VWORD CHARGE_VOLTAGE;
VWORD NearFullChrCunt;
VWORD BAT_CC, BAT_CV;
VWORD BAT_RSOC, BAT_VOLT, BAT_RCAP;
VWORD BAT_ALARM, BAT_FCAP, BAT_TEMP, BAT_ACURRENT;
VWORD BAT_DCAP, BAT_DVOL, BAT_SN, BAT_MODE;
short BAT_CURRENT;
//----------------------------------------------------------------------------
// Battery Device Name Table
//----------------------------------------------------------------------------
char Devicename[] = { 0x0d,0x4e,0x42,0x30,0x31,0x36,0x31,0x2d,'\0' };
const BatteryDeviceName BatDevNameTable[] =
{
	{0x00, "Li-poly"},
	{0x01, "Li-ion"},
	{0x02, Devicename},
};

//----------------------------------------------------------------------------
// Battery Data Table
//----------------------------------------------------------------------------
const BatteryData BAT_PollingDataTable[] =
{
	/*发给电池的命令    电池数据暂存的16位全局变量  电池数据存入ecspace地址*/
	{BATCmd_RSOC,		&BAT_RSOC,				{&BATTERY_RSOC,NULL}						},
	{BATCmd_current, 	(VWORD *)&BAT_CURRENT,			{&BATTERY_CURRENT_L,&BATTERY_CURRENT_H}	},
	{BATCmd_volt, 		&BAT_VOLT,				{&BATTERY_VOL_L,&BATTERY_VOL_H}			},
	{BATCmd_RMcap, 		&BAT_RCAP,				{&BATTERY_RMCAP_L,&BATTERY_RMCAP_H}		},
	{BATCmd_BatStatus, 	&BAT_ALARM,			{NULL,NULL}									},
	{BATCmd_FCcap, 		&BAT_FCAP,				{&BATTERY_FCAP_L,&BATTERY_FCAP_H}			},
	{BATCmd_temp, 		&BAT_TEMP,				{&BATTERY_TEMP_L,&BATTERY_TEMP_H}			},
	{BATCmd_CC, 		&BAT_CC,				{NULL,NULL}									},
	{BATCmd_CV, 		&BAT_CV,				{NULL,NULL}									},
	{BATCmd_AVcurrent, 	&BAT_ACURRENT,			{&BATTERY_ACURRENT_L,&BATTERY_ACURRENT_H}	},
	{BATCmd_DCap, 		&BAT_DCAP,				{&BATTERY_DCAP_L,&BATTERY_DCAP_H}			},
	{BATCmd_DVolt, 		&BAT_DVOL,				{&BATTERY_DVOL_L,&BATTERY_DVOL_H}			},
	{BATCmd_SerialNo, 	&BAT_SN,				{&BATTERY_SN_L,&BATTERY_SN_H}				},
	{BATCmd_mode, 		&BAT_MODE,				{NULL,NULL}									},
};


void BAT_Var_Clear(void)
{
	BATTERY_ACURRENT_L = 0;
	BATTERY_ACURRENT_H = 0;
	BAT_ALARM = 0;
	BAT_CC = 0;
	BAT_CV = 0;
	BATTERY_CURRENT_L = 0;
	BATTERY_CURRENT_H = 0;
	BATTERY_TEMP_L = 0;
	BATTERY_TEMP_H = 0;
	BATTERY_RSOC = 0;
	BATTERY_DCAP_L = 0;
	BATTERY_DCAP_H = 0;
	BATTERY_FCAP_L = 0;
	BATTERY_FCAP_H = 0;
	BATTERY_RMCAP_L = 0;
	BATTERY_RMCAP_H = 0;
	BATTERY_DVOL_L = 0;
	BATTERY_DVOL_H = 0;
	BATTERY_VOL_L = 0;
	BATTERY_VOL_H = 0;
	BATTERY_SN_L = 0;
	BATTERY_SN_H = 0;
	Bat_Info_Cnt = 0;
	BAT_FirstDataIndex = 0x00;
	BATTERY_STATUS2 = 0x00;
	BATTERY_STATUS1 &= (~bat_valid);
	CTRL_FLAG1 &= (~bat_in);
	BATTERY_STATUS3 = 0x00;
	BAT_CtrlStep = 0x00;
	BAT_ID_Step = 0x00;
	BAT_10MsTCunt = 0x00;
	BAT_SNumber = 0x00;
	BAT_ID_TimeOut = 0x00;
	BAT_PollingIndex = 0x00;
	Charger_TimeOut = 0x00;
	WakeUpChrCunt_Min = 0x00;
	PreChrCunt_Min = 0x00;
	NormalChrCunt_Min = 0x00;
	WakeUpChrCunt = 0x00;
	PreChrCunt = 0x00;
	NormalChrCunt = 0x00;
	BAT_OTCnt = 0x00;
	BAT_OVCnt = 0x00;
	CHARGE_CURRENT_BK = 0x00;
	CHARGE_VOLTAGE_BK = 0x00;
	PF_CHARGE_FULL_LED_OFF();
}

void BatteryTARSetup(void)
{
	I2c_Master_Set_Tar(BAT_ADDR, I2C_REGADDR_7BIT, BAT_I2C_CHANNEL);
}

void ChargerTARSetup(void)
{
	I2c_Master_Set_Tar(CHARGER_ADDR, I2C_REGADDR_7BIT, BAT_I2C_CHANNEL);
}

void BAT_ChargePinControl(BYTE enable)
{
	if(enable)
	{
		CE_OFF();
	}
	else
	{
		CE_ON();
	}
}

void BatteryWrite(BYTE addr, WORD value)
{
	BYTE data[2] = { 0 };
	data[0] = (value & 0xFF);
	data[1] = ((value >> 8) & 0xFF);

	I2cM_Write_Block(BAT_ADDR, data, 2, addr, BAT_I2C_CHANNEL);
}

void ChargerWrite(BYTE addr, WORD value)
{
	BYTE data[2] = { 0 };
	data[0] = (value & 0xFF);
	data[1] = ((value >> 8) & 0xFF);

	I2cM_Write_Block(CHARGER_ADDR, data, 2, addr, BAT_I2C_CHANNEL);
}

VWORD BatteryRead(BYTE addr)
{
	BYTE i = 2;
	word_read temp;
	BatteryTARSetup();
	while(I2c_Check_TFE(BAT_I2C_CHANNEL));
	I2c_Writeb(addr, I2C_DATA_CMD_OFFSET, BAT_I2C_CHANNEL);
	I2c_Writeb(I2C_WRITE, I2C_DATA_CMD_RWDIR, BAT_I2C_CHANNEL);
	I2c_Writeb(I2C_READ, I2C_DATA_CMD_RWDIR, BAT_I2C_CHANNEL);
	I2c_Writeb(I2C_READ | I2C_STOP, I2C_DATA_CMD_RWDIR, BAT_I2C_CHANNEL);
	while((i--)&&(0 == (I2c_Check_RFNE(BAT_I2C_CHANNEL))))
	{
		temp.data0[1 - i] = I2c_Readb(I2C_DATA_CMD_OFFSET, BAT_I2C_CHANNEL);
	}
	return temp.data;
}

VWORD ChargerRead(BYTE addr)
{
	BYTE i = 2;
	word_read temp;
	ChargerTARSetup();
	while(I2c_Check_TFE(CHG_I2C_CHANNEL));
	I2c_Writeb(addr, I2C_DATA_CMD_OFFSET, CHG_I2C_CHANNEL);
	I2c_Writeb(I2C_WRITE, I2C_DATA_CMD_RWDIR, CHG_I2C_CHANNEL);
	I2c_Writeb(I2C_READ, I2C_DATA_CMD_RWDIR, CHG_I2C_CHANNEL);
	I2c_Writeb(I2C_READ | I2C_STOP, I2C_DATA_CMD_RWDIR, CHG_I2C_CHANNEL);
	while((i--) && (0 == I2c_Check_RFNE(CHG_I2C_CHANNEL)))
	{
		temp.data0[1 - i] = I2c_Readb(I2C_DATA_CMD_OFFSET, CHG_I2C_CHANNEL);
	}
	return temp.data;
}

void Battery_Static_Mode_Judge(void)
{
	if((SystemIsS5) && IS_MASK_CLEAR(CTRL_FLAG1, bat_in))
	{
		noac_1s_cnt++;
		if(noac_1s_cnt >= 180)
		{
			noac_1s_cnt = 0;
			Enter_LowPower_Mode();
		}
	}
	else
	{
		noac_1s_cnt = 0;
	}
}

void SetBATIDTimeOutCNT(void)
{
	BAT_ID_TimeOut = BatIDTimeOut;
}

void SetBATCtrlStep(BYTE ctrlstep)
{
	BAT_CtrlStep = ctrlstep;
	Charger_TimeOut = 0x00;
	switch(BAT_CtrlStep)
	{
		case BAT_Step_ID:
			SetBATIDTimeOutCNT();
			BAT_ID_Step = BATID_Step_GetDV;
			break;
		case BAT_Step_WC:
			BATTERY_STATUS1 &= ~(bat_InCharge + bat_Dischg + bat_Full);
			BATTERY_STATUS2 = 0x00;
			WakeUpChrCunt = 0x0000;
			WakeUpChrCunt_Min = 0x00;
			CLEAR_MASK(BATTERY_STATUS1, bat_valid);
			PF_CHARGE_FULL_LED_OFF();
			SET_MASK(BATTERY_STATUS2, bat_WC);
			break;
		case BAT_Step_PC:
			BATTERY_STATUS1 &= ~(bat_InCharge + bat_Dischg + bat_Full);
			BATTERY_STATUS2 = 0x00;
			PreChrCunt = 0x0000;
			PreChrCunt_Min = 0x0000;
			PF_CHARGE_FULL_LED_OFF();
			SET_MASK(BATTERY_STATUS2, bat_PC);
			break;
		case BAT_Step_NC:
			BATTERY_STATUS1 &= ~(bat_InCharge + bat_Dischg + bat_Full);
			BATTERY_STATUS2 = 0x00;
			NormalChrCunt = 0x0000;
			NormalChrCunt_Min = 0x0000;
			NearFullChrCunt = 0;
			CHARGE_CURRENT_BK = 0x00;
			CHARGE_VOLTAGE_BK = 0x00;
			PF_CHARGE_FULL_LED_OFF();
			SET_MASK(BATTERY_STATUS2, bat_NC);
			break;
		case BAT_Step_DC:
			BATTERY_STATUS1 &= ~(bat_InCharge + bat_Dischg + bat_Full);
			BATTERY_STATUS2 = 0x00;
			PF_CHARGE_FULL_LED_OFF();
			SET_MASK(BATTERY_STATUS2, bat_DC);
			SET_MASK(BATTERY_STATUS1, bat_Dischg);
			break;
		case BAT_Step_FullyChg:
			BATTERY_STATUS1 &= ~(bat_InCharge + bat_Dischg + bat_Full);
			SET_MASK(BATTERY_STATUS1, bat_Full);
			BATTERY_STATUS2 = 0x00;
			break;
	}
}

void BAT1_SetFail(BYTE failcause)
{
	BAT_FailCause = failcause;
	SetBATCtrlStep(BAT_Step_SetFail);
}

void PollingBATInfo(void)
{
	if(BAT_PollingIndex < (sizeof(BAT_PollingDataTable) / sizeof(BatteryData)))
	{
		if((BAT_PollingDataTable[BAT_PollingIndex].Var) != NULL)
		{
			*(BAT_PollingDataTable[BAT_PollingIndex].Var) = BatteryRead(BAT_PollingDataTable[BAT_PollingIndex].Cmd);
		}
		if((BAT_PollingDataTable[BAT_PollingIndex].Bat_Ecsapce.Var_L) != NULL)
		{
			*(BAT_PollingDataTable[BAT_PollingIndex].Bat_Ecsapce.Var_L) = (VBYTE)((*(BAT_PollingDataTable[BAT_PollingIndex].Var)) & 0xff);
		}
		else
		{
			//处理空指针的情况
		}
		if((BAT_PollingDataTable[BAT_PollingIndex].Bat_Ecsapce.Var_H) != NULL)
		{
			*(BAT_PollingDataTable[BAT_PollingIndex].Bat_Ecsapce.Var_H) = (VBYTE)(((*(BAT_PollingDataTable[BAT_PollingIndex].Var)) >> 8) & 0xff);
		}
		else
		{
			//处理空指针的情况
		}
		BAT_PollingIndex++;
		if(BAT_PollingIndex >= (sizeof(BAT_PollingDataTable) / sizeof(BatteryData)))
		{
			BAT_PollingIndex = 0;
			//可做特殊处理例如：BATTERY1_TEMP_H = (VBYTE)(((BAT_TEMP - 2730) >> 8) & 0xFF);
		}
	}
}

void CheckBatteryProtection(void)
{
	WORD voltage;
	if(IS_MASK_SET(BAT_ALARM, OverTemp_alarm))
	{
		if(++BAT_OTCnt > BatOTTimeOut)
		{
			BAT1_SetFail(Bat_OverTemperature);
		}
	}
	else
	{
		BAT_OTCnt = 0x00;
	}
	voltage = BATTERY_VOL_L;
	if(voltage > BATOverVoltage)
	{
		if(++BAT_OVCnt > BatOVTimeOut)
		{
			BAT1_SetFail(Bat_OverVoltage);
		}
	}
	else
	{
		BAT_OVCnt = 0x00;
	}
}

void BATForceStep(void)
{
	if(IS_MASK_SET(BATTERY_STATUS3, bat_ForceChg))
	{
		if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
		{
			if(IS_MASK_SET(BATTERY_STATUS1, bat_Dischg))
			{
				SetBATCtrlStep(BAT_Step_PC);
			}
		}
	}
	else if(IS_MASK_SET(BATTERY_STATUS3, bat_ForceDischg))
	{
		if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
		{
			if(IS_MASK_SET(BATTERY_STATUS1, bat_InCharge))
			{
				SetBATCtrlStep(BAT_Step_DC);
			}
		}
	}
}

BYTE DisableCharger(void)
{
	BYTE result;
	result = 0x00;
	if(SmartCharger_Support)
	{
		CHARGE_CURRENT = 00;
		CHARGE_VOLTAGE = 00;
		INPUT_CURRENT = 00;
		if(SetSmartCharger() != 0x00)//set charger fail
		{
			Charger_TimeOut++;
			if(Charger_TimeOut > ChargerSettingTimeOut)
			{
				SetBATCtrlStep(BAT_Step_ChargerFail);
			}
			result = 0x00;
		}
		else//set charger success
		{
			if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
			{
				if(IS_MASK_SET(BATTERY_STATUS3, bat_ForceDischg))
				{
					BAT_ChargePinControl(0);
				}
				else
				{
					BAT_ChargePinControl(1);
				}
			}
			else
			{
				BAT_ChargePinControl(0);
				SET_MASK(BATTERY_STATUS2, bat_DC_OK);
				SET_MASK(BATTERY_STATUS1, bat_Dischg);
			}
			Charger_TimeOut = 0x00;
			result = 0x01;
		}
	}
	else
	{
		if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
		{
			if(IS_MASK_SET(BATTERY_STATUS3, bat_ForceDischg))
			{
				BAT_ChargePinControl(0);
			}
			else
			{
				BAT_ChargePinControl(1);
			}
		}
		else
		{
			BAT_ChargePinControl(0);
			SET_MASK(BATTERY_STATUS2, bat_DC_OK);
			SET_MASK(BATTERY_STATUS1, bat_Dischg);
		}
		result = 0x01;
	}
	return (result);
}

void BAT_FullChargeCheck(void)
{
	if((BAT_CtrlStep == BAT_Step_PC) || (BAT_CtrlStep == BAT_Step_NC))
	{
		if(IS_MASK_SET(BAT_ALARM, FullyChg))//FullyChg_alarm
		{
			if(DisableCharger() == 0x01)
			{
				SetBATCtrlStep(BAT_Step_FullyChg);
				PF_CHARGE_FULL_LED_ON();
				PF_CHARGE_LED_OFF();
			}
		}
		else if(BAT_RSOC >= 95)
		{
			if(BAT_CC == 0)//The charging current required for the battery
			{
				if(DisableCharger() == 0x01)
				{
					SetBATCtrlStep(BAT_Step_FullyChg);
					PF_CHARGE_FULL_LED_ON();
					PF_CHARGE_LED_OFF();
				}
			}
			else if(BAT_CURRENT < 0x64)//measuring current
			{
				NearFullChrCunt++;
				if(NearFullChrCunt == 6000)
				{
					NearFullChrCunt = 0;
					if(DisableCharger() == 0x01)
					{
						SetBATCtrlStep(BAT_Step_FullyChg);
						PF_CHARGE_FULL_LED_ON();
						PF_CHARGE_LED_OFF();
					}
				}
			}
		}
		else
		{
			NearFullChrCunt = 0;
		}
	}
}

void BAT_FullDischargeCheck(void)
{
	if(BAT_CtrlStep == BAT_Step_DC)
	{
		if(IS_MASK_SET(BAT_ALARM, FullyDsg))
		{
			SetBATCtrlStep(BAT_Step_FullyDisChg);
		}
	}
}

void BATIdentifyNextStep(void)
{
	BAT_ID_Step++;
	if(BAT_ID_Step == BATID_Step_FirstData)
	{
		BAT_FirstDataIndex = 0x00;
	}
	SetBATIDTimeOutCNT();
}

BYTE SetSmartCharger(void)
{
	ChargerWrite(CHG_InputCurrent, (INPUT_CURRENT));
	ChargerWrite(CHG_ChargeVoltage, (CHARGE_VOLTAGE));
	ChargerWrite(CHG_ChargeCurrent, (CHARGE_CURRENT));
	return 0;
}

void BATID_GetDesignVoltage(void)
{
	WORD DVTemp;
	ByteTemp01 = BatteryRead(BATCmd_DVolt);
	if(ByteTemp01)
	{
		DVTemp = ByteTemp01;
		if(DVTemp < 8400)
		{
			BAT_SNumber = 0x02;
		}
		else if(DVTemp < 12600)
		{
			BAT_SNumber = 0x03;
		}
		else
		{
			BAT_SNumber = 0x04;
		}
		BATIdentifyNextStep();//next ID step
		ByteTemp01 = 0x00;
		ByteTemp02 = 0x00;
	}
	else
	{
		BAT_ID_TimeOut--;
		if(BAT_ID_TimeOut == 0x00)
		{
			SetBATCtrlStep(BAT_Step_WC);
		}
	}
}

void BATID_CheckBatAuth(void)
{
	BATIdentifyNextStep();
}

void BATID_CheckDeviceName(void)
{
#ifdef	BAT_DEV_NAME_SUPPORT
	BYTE index = 0;
	BYTE Temp_Name[20] = { 1 };
	BYTE i = 0;
	for(index = 0; index < (sizeof(BatDevNameTable) / sizeof(BatteryDeviceName)); index++)
	{
		BYTE sLength = strlen(BatDevNameTable[index].DeviceName);
		BatteryTARSetup();
		I2c_Master_Read_Block(Temp_Name, sLength, BATCmd_Dname, BAT_I2C_CHANNEL);
		for(i = 0; i < sLength; i++)
		{
			if(Temp_Name[i] != BatDevNameTable[index].DeviceName[i])
			{
				break;
			}
		}
		if(i == sLength)//match device name
		{
			BATIdentifyNextStep();         // next step                  
			return;
		}
	}
	if(Temp_Name[0] == 0)
	{
		BAT_ID_TimeOut--;
		if(BAT_ID_TimeOut == 0x00)           // time out
		{
			dprint("Please check SMBUS settings and connections\n");
			SetBATCtrlStep(BAT_Step_WC);       // Go to wake up charge
		}
	}
	else
	{
		BAT1_SetFail(Bat_NoSupport); 			// no support
	}
#else
	BATIdentifyNextStep();         // next step   
#endif
}

void BATID_GetAllInfo(void)
{
	for(BAT_FirstDataIndex = 0; BAT_FirstDataIndex < (sizeof(BAT_PollingDataTable) / sizeof(BatteryData)); BAT_FirstDataIndex++)
	{
		if((BAT_PollingDataTable[BAT_FirstDataIndex].Var) != NULL)
		{
			*(BAT_PollingDataTable[BAT_FirstDataIndex].Var) = BatteryRead(BAT_PollingDataTable[BAT_FirstDataIndex].Cmd);
		}
		if((BAT_PollingDataTable[BAT_FirstDataIndex].Bat_Ecsapce.Var_L) != NULL)
		{
			if((BAT_PollingDataTable[BAT_FirstDataIndex].Var) != NULL)
				*(BAT_PollingDataTable[BAT_FirstDataIndex].Bat_Ecsapce.Var_L) = (VBYTE)((*(BAT_PollingDataTable[BAT_FirstDataIndex].Var)) & 0xff);
			else
				*(BAT_PollingDataTable[BAT_FirstDataIndex].Bat_Ecsapce.Var_L) = (VBYTE)(BatteryRead(BAT_PollingDataTable[BAT_FirstDataIndex].Cmd) & 0xff);
		}
		else
		{
			//处理空指针的情况
		}
		if((BAT_PollingDataTable[BAT_FirstDataIndex].Bat_Ecsapce.Var_H) != NULL)
		{
			if((BAT_PollingDataTable[BAT_FirstDataIndex].Var) != NULL)
				*(BAT_PollingDataTable[BAT_FirstDataIndex].Bat_Ecsapce.Var_H) = (VBYTE)(((*(BAT_PollingDataTable[BAT_FirstDataIndex].Var)) >> 8) & 0xff);
			else
				*(BAT_PollingDataTable[BAT_FirstDataIndex].Bat_Ecsapce.Var_H) = (VBYTE)(((BatteryRead(BAT_PollingDataTable[BAT_FirstDataIndex].Cmd)) >> 8) & 0xff);
		}
		else
		{
			//处理空指针的情况
		}
	}
	//可做特殊处理例如：BATTERY1_TEMP_H = (VBYTE)(((BAT_TEMP - 2730) >> 8) & 0xFF);
	BATIdentifyNextStep();
}

void BATID_InitOK(void)
{
	SET_MASK(BATTERY_STATUS1, bat_valid);
	if(IS_MASK_SET(CTRL_FLAG1, adapter_in))//adapter_in
	{
		SetBATCtrlStep(BAT_Step_PC);//PreCharge
	}
	else
	{
		SetBATCtrlStep(BAT_Step_DC);//Discharge
	}
}

void IdentifyBAT(void)
{
	switch(BAT_ID_Step)
	{
		case BATID_Step_GetDV:
			BATID_GetDesignVoltage();//Get Design Voltage
			break;
		case BATID_Step_Auth:
			BATID_CheckBatAuth();//Reserved for checking battery authentication 
			break;
		case BATID_Step_MFName:
			BATID_CheckDeviceName();//Reserved for checking device name
			break;
		case BATID_Step_FirstData:
			BATID_GetAllInfo();
			break;
		case BATID_Step_InitOK:
			BATID_InitOK();
			break;
		default:
			break;
	}
}

void BAT_SetWakeUpCharge(void)
{
	if(SmartCharger_Support)
	{
		CHARGE_CURRENT = BAT_CURR_WUCHG;
		CHARGE_VOLTAGE = BAT_VOLT_WUCHG;
		INPUT_CURRENT = IN_CURRENT;
		if(SetSmartCharger() != 0x00)//set charger fail
		{
			Charger_TimeOut++;
			if(Charger_TimeOut > ChargerSettingTimeOut)
			{
				SetBATCtrlStep(BAT_Step_ChargerFail);
			}
		}
		else
		{
			BAT_ChargePinControl(1);
			SET_MASK(BATTERY_STATUS2, bat_WC_OK);
			SET_MASK(BATTERY_STATUS1, bat_InCharge);
			Charger_TimeOut = 0x00;
		}
	}
	else
	{
		BAT_ChargePinControl(1);
		SET_MASK(BATTERY_STATUS2, bat_WC_OK);
		SET_MASK(BATTERY_STATUS1, bat_InCharge);
	}
}

void BATWakeUpCharge(void)
{
	if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
	{
		if(IS_MASK_CLEAR(BATTERY_STATUS2, bat_WC_OK))
		{
			BAT_SetWakeUpCharge();
		}
		else										//check if it is possible to stop wake-up charging
		{
			WakeUpChrCunt++;
			if((WakeUpChrCunt % 6000) == 0)//1min
			{
				BAT_SetWakeUpCharge();
				WakeUpChrCunt = 0x0000;
				WakeUpChrCunt_Min++;
				if(WakeUpChrCunt_Min > BatWCTimeOut)
				{
					BAT1_SetFail(Bat_WakeUpChargeFail);
				}
			}
			else if((WakeUpChrCunt % 600) == 0)//6s
			{
				BAT_RCAP = BatteryRead(BATCmd_RMcap);
				if(BAT_RCAP != 0)//If SMBUS communication is successful, there is no need to perform wake-up charging
				{
					SetBATCtrlStep(BAT_Step_ID);
				}
			}
		}
	}
	else
	{
		PF_CHARGE_LED_OFF();
		BAT1_SetFail(Bat_Weak);
	}
}

void BAT_SetPreCharge(void)
{
	if(SmartCharger_Support)
	{
		PF_CHARGE_LED_ON();
		CHARGE_CURRENT = BAT_CURR_PRECHG;
		CHARGE_VOLTAGE = BAT_VOLT_PRECHG;
		INPUT_CURRENT = IN_CURRENT;
		if(SetSmartCharger() != 0x00)
		{
			Charger_TimeOut++;
			if(Charger_TimeOut > ChargerSettingTimeOut)
			{
				SetBATCtrlStep(BAT_Step_ChargerFail);
			}
		}
		else
		{
			BAT_ChargePinControl(1);
			SET_MASK(BATTERY_STATUS2, bat_PC_OK);
			SET_MASK(BATTERY_STATUS1, bat_InCharge);
			Charger_TimeOut = 0x00;
		}
	}
	else
	{
		BAT_ChargePinControl(1);
		SET_MASK(BATTERY_STATUS2, bat_PC_OK);
		SET_MASK(BATTERY_STATUS1, bat_InCharge);
	}
}

void BAT_PreCharge(void)
{
	WORD cctemp;
	if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
	{
		if(IS_MASK_CLEAR(BATTERY_STATUS2, bat_PC_OK))//Perform pre charging
		{
			PreChrCunt = 0x00;
			BAT_SetPreCharge();
		}
		else//Check if it is possible to stop pre charging
		{
			PreChrCunt++;
			if((PreChrCunt % 6000) == 0)//1min
			{
				BAT_SetPreCharge();
				PreChrCunt = 0x0000;
				PreChrCunt_Min++;
				if(PreChrCunt_Min > BatPCTimeOut)
				{
					BAT1_SetFail(Bat_PreChargeFail);
				}
			}
			else if((PreChrCunt % 600) == 0)
			{
				cctemp = BAT_CC;
				if(cctemp > Bat1_PC2NC_Cap)
				{
					SetBATCtrlStep(BAT_Step_NC);
				}
			}
		}
	}
	else
	{
		SetBATCtrlStep(BAT_Step_DC);
	}
}

void BAT_SetNormalCharge(void)
{
	WORD CC_temp;
	if(SmartCharger_Support)
	{
		PF_CHARGE_LED_ON();
		CC_temp = BAT_CC;
		CC_temp *= SC_Current_Sense;
		CHARGE_CURRENT = CC_temp;
		CHARGE_VOLTAGE = BAT_CV;
		INPUT_CURRENT = IN_CURRENT;
		if(SetSmartCharger() != 0x00)
		{
			Charger_TimeOut++;
			if(Charger_TimeOut > ChargerSettingTimeOut)
			{
				SetBATCtrlStep(BAT_Step_ChargerFail);
			}
		}
		else
		{
			BAT_ChargePinControl(1);
			SET_MASK(BATTERY_STATUS2, bat_NC_OK);
			SET_MASK(BATTERY_STATUS1, bat_InCharge);
			Charger_TimeOut = 0x00;
		}
	}
	else
	{
		BAT_ChargePinControl(1);
		SET_MASK(BATTERY_STATUS2, bat_NC_OK);
		SET_MASK(BATTERY_STATUS1, bat_InCharge);
	}
}

void BAT_NormalCharge(void)
{
	if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
	{
		if(IS_MASK_CLEAR(BATTERY_STATUS2, bat_NC_OK))
		{
			CHARGE_CURRENT_BK = BAT_CC;
			NormalChrCunt = 0x0000;
			BAT_SetNormalCharge();
		}
		else
		{
			NormalChrCunt++;
			if(CHARGE_CURRENT_BK != BAT_CC)//Update charging current, etc
			{
				CHARGE_CURRENT_BK = BAT_CC;
				BAT_SetNormalCharge();
			}
			else
			{
				if((NormalChrCunt % 6000) == 0)//1min
				{
					NormalChrCunt_Min++;
					if(NormalChrCunt_Min > BatNCTimeOut)//20h
					{
						BAT1_SetFail(Bat_NormalChargeFail);
					}
					BAT_SetNormalCharge();
					NormalChrCunt = 0x0000;
				}
			}
		}
	}
	else
	{
		SetBATCtrlStep(BAT_Step_DC);
	}
}

void BATDisCharge(void)
{
	if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
	{
		if(IS_MASK_SET(BATTERY_STATUS3, bat_ForceDischg))
		{
			if(IS_MASK_CLEAR(BATTERY_STATUS2, bat_DC_OK))
			{
				DisableCharger();
			}
		}
		else
		{
			SetBATCtrlStep(BAT_Step_PC);
		}
	}
	else
	{
		if(IS_MASK_CLEAR(BATTERY_STATUS2, bat_DC_OK))
		{
			DisableCharger();
		}
	}
}

void BATFullyCharged(void)
{
	if(IS_MASK_CLEAR(CTRL_FLAG1, adapter_in))
	{
		SetBATCtrlStep(BAT_Step_DC);
	}
}

void BATFullyDisharged(void)
{
	if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
	{
		SetBATCtrlStep(BAT_Step_WC);
	}
	else
	{
		SetBATCtrlStep(BAT_Step_FullyDisChg);
	}
}

void BATSetFail(void)
{
	if(DisableCharger() == 0x01)
	{
		BAT_Var_Clear();
		SetBATCtrlStep(BAT_Step_Fail);
	}
}

void BATForceDischarge(void)
{}

void BATForceCharge(void)
{}

void BATAutoLearning(void)
{}

//Call once every 10 ms 
void BatChgControlCenter(void)
{
	if(IS_MASK_CLEAR(CTRL_FLAG1, bat_in))//Battery not connected
	{
		PF_CHARGE_LED_OFF();
		PF_CHARGE_FULL_LED_OFF();
		return;
	}
	if(IS_MASK_SET(BATTERY_STATUS1, bat_valid))//Battery valid
	{
		BAT_10MsTCunt++;
		if((BAT_10MsTCunt > 100))//1 Sec Timer
		{
			BAT_10MsTCunt = 0;
			PollingBATInfo();		//Polling Battery Data
			CheckBatteryProtection();		//Battery Protection
		}
		BATForceStep();			//Determine whether forced charging and discharging are necessary
		if(IS_MASK_SET(CTRL_FLAG1, adapter_in))//Adapter in
		{
			BAT_FullChargeCheck();//Check if battery is fully charged
		}
		else
		{
			BAT_FullDischargeCheck();//Check if battery is fully discharged
		}
	}
	switch(BAT_CtrlStep)
	{
		case BAT_Step_ID:
			IdentifyBAT();
			break;
		case BAT_Step_WC:
			BATWakeUpCharge();
			break;
		case BAT_Step_PC:
			BAT_PreCharge();
			break;
		case BAT_Step_NC:
			BAT_NormalCharge();
			break;
		case BAT_Step_DC:
			BATDisCharge();
			break;
		case BAT_Step_FullyChg:
			BATFullyCharged();
			break;
		case BAT_Step_FullyDisChg:
			BATFullyDisharged();
			break;
		case BAT_Step_SetFail:
			BATSetFail();
			break;
		case BAT_Step_Fail:
			break;
		case BAT_Step_ForceDC:
			BATForceDischarge();
			break;
		case BAT_Step_ForceC:
			BATForceCharge();
			break;
		case BAT_Step_AutoLN:
			BATAutoLearning();
			break;
		case BAT_Step_ChargerFail:
			break;
		case BAT_Step_Out:
			break;
		default:
			break;
	}
}

//*****************************************************************************
//  Shutdown：
//		此函数是电池进入Shutdown模式（ship mode模式）的一个实例，电池型号是BQ40Z50，不同型号电池可能实现方式不同，具体以电池本身使用手册为准。
//
//  parameter :
//		i2c_addr : slave addr
//		Date : write data buffer
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//		none
//*****************************************************************************
void Shutdown(BYTE i2c_addr, WORD i2c_channel)
{
	BYTE Command = 0x00;
	WORD Date = 0x0010;
	BYTE TxDate[2];
	TxDate[0] = Date & 0xff;    // 小端发送，高位数据先发送，低位数据后发送
	TxDate[1] = (Date >> 8) & 0xff;
	I2cM_Write_Word(i2c_addr, TxDate, Command, i2c_channel);// 连续往0x00小端按字两次写入0x0010则会进入ShutDown模式
	I2cM_Write_Word(i2c_addr, TxDate, Command, i2c_channel);

}
