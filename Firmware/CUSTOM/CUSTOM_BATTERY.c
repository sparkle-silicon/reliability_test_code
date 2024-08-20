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
#include "CUSTOM_MAIN.H"
#include "KERNEL_MEMORY.H"
#include "CUSTOM_POWER.H"

/*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/
VWORD ChargerDataRead;
VWORD ChargerData12;
VWORD ChargerData14;
VWORD ChargerData15;
VWORD ChargerData20;
VWORD ChargerData21;
VWORD ChargerData22;
VWORD ChargerData30;
VWORD ChargerData33;
VWORD ChargerData3D;
BYTE BAT1_CtrlStep;
BYTE BAT1_ID_Step;
BYTE BAT1_1SecTimer;
BYTE BAT1_2SecTimer;
BYTE BAT1_S_Number;
VWORD ByteTemp01;
VWORD ByteTemp02;
BYTE BAT1_ID_TimeOut_CNT;
BYTE BAT1_Number;
BYTE BAT1_FailCause;
BYTE BAT1_FirstDataIndex;
BYTE BAT1_PollingDataIndex;
BYTE Charger_TimeOut;
BYTE WakeUpChrCunt_Min;
VWORD PreChrCunt_Min;
VWORD NormalChrCunt_Min;
VWORD WakeUpChrCunt;
VWORD PreChrCunt;
VWORD NormalChrCunt;
BYTE BAT1_OTCunt;
BYTE BAT1_OVCunt;
VWORD CHARGE_CURRENT_BK;
VWORD CHARGE_VOLTAGE_BK;
VWORD CHARGE_OPTIONS;
VWORD INPUT_CURRENT_LO;
VWORD CHARGE_CURRENT_LO;
VWORD CHARGE_VOLTAGE_LO;
BYTE BT1_STATUS2;
BYTE BT1_STATUS3;
BYTE Charger_Mode;
VWORD TimerCount;
BYTE BAT1_ALARM2;
VWORD BAT1_CC_L, BAT1_CV_L;
VWORD BAT1_RSOC, BAT1_CURRENT, BAT1_VOLT, BAT1_RCAP;
VWORD BAT1_ALARM1, BAT1_FCAP, BAT1_TEMP, BAT1_ACURRENT;
VWORD BAT1_DCAP, BAT1_DVOL, BAT1_SN, BAT1_mode_L;

const BatteryData BAT1_PollingDataTable[] =
{
	/*发给电池的命令    电池数据暂存的16位全局变量  电池数据存入ecspace地址*/
	{BATCmd_RSOC,		&BAT1_RSOC,				{&BATTERY1_RSOC,NULL}						},
	{BATCmd_current, 	&BAT1_CURRENT,			{&BATTERY1_CURRENT_L,&BATTERY1_CURRENT_H}	},
	{BATCmd_volt, 		&BAT1_VOLT,				{&BATTERY1_VOL_L,&BATTERY1_VOL_H}			},
	{BATCmd_RMcap, 		&BAT1_RCAP,				{&BATTERY1_RMCAP_L,&BATTERY1_RMCAP_H}		},
	{BATCmd_BatStatus, 	&BAT1_ALARM1,			{NULL,NULL}									},
	{BATCmd_FCcap, 		&BAT1_FCAP,				{&BATTERY1_FCAP_L,&BATTERY1_FCAP_H}			},
	{BATCmd_temp, 		&BAT1_TEMP,				{&BATTERY1_TEMP_L,&BATTERY1_TEMP_H}			},
	{BATCmd_CC, 		&BAT1_CC_L,				{NULL,NULL}									},
	{BATCmd_CV, 		&BAT1_CV_L,				{NULL,NULL}									},
	{BATCmd_AVcurrent, 	&BAT1_ACURRENT,			{&BATTERY1_ACURRENT_L,&BATTERY1_ACURRENT_H}	},
	{BATCmd_DCap, 		&BAT1_DCAP,				{&BATTERY1_DCAP_L,&BATTERY1_DCAP_H}			},
	{BATCmd_DVolt, 		&BAT1_DVOL,				{&BATTERY1_DVOL_L,&BATTERY1_DVOL_H}			},
	{BATCmd_SerialNo, 	&BAT1_SN,				{&BATTERY1_SN_L,&BATTERY1_SN_H}				},
	{BATCmd_mode, 		&BAT1_mode_L,			{NULL,NULL}									},
};


void BAT1_PlugOutClrVariables(void)
{
	BATTERY1_ACURRENT_L = 0;
	BATTERY1_ACURRENT_H = 0;
	BAT1_ALARM1 = 0;
	BAT1_CC_L = 0;
	BAT1_CV_L = 0;
	BATTERY1_CURRENT_L = 0;
	BATTERY1_CURRENT_H = 0;
	BATTERY1_TEMP_L = 0;
	BATTERY1_TEMP_H = 0;
	BATTERY1_RSOC = 0;
	BATTERY1_DCAP_L = 0;
	BATTERY1_DCAP_H = 0;
	BATTERY1_FCAP_L = 0;
	BATTERY1_FCAP_H = 0;
	BATTERY1_RMCAP_L = 0;
	BATTERY1_RMCAP_H = 0;
	BATTERY1_DVOL_L = 0;
	BATTERY1_DVOL_H = 0;
	BATTERY1_VOL_L = 0;
	BATTERY1_VOL_H = 0;
	BATTERY1_SN_L = 0;
	BATTERY1_SN_H = 0;
	Bat_Info_Cnt = 0;
	BAT1_FirstDataIndex = 0x00;
	BT1_STATUS2 = 0x00;
	BT1_STATUS1 &= (~bat_valid);
	CTRL_FLAG1 &= (~bat_in);
	BT1_STATUS3 = 0x00;
	BAT1_CtrlStep = 0x00;
	BAT1_ID_Step = 0x00;
	BAT1_1SecTimer = 0x00;
	BAT1_S_Number = 0x00;
	BAT1_ID_TimeOut_CNT = 0x00;
	BAT1_PollingDataIndex = 0x00;
	Charger_TimeOut = 0x00;
	WakeUpChrCunt_Min = 0x00;
	PreChrCunt_Min = 0x00;
	NormalChrCunt_Min = 0x00;
	WakeUpChrCunt = 0x00;
	PreChrCunt = 0x00;
	NormalChrCunt = 0x00;
	BAT1_OTCunt = 0x00;
	BAT1_OVCunt = 0x00;
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

void Hook_BAT1_ChargePinON(BYTE enable)
{
	if(enable)
	{
		ChargerWrite(SC85_ChargeOption0, 0xE70E & 0xFFFF);
		PF_CHARGE_LED_ON();
	}
	else
	{
		ChargerWrite(SC85_ChargeOption0, 0xE70F & 0xFFFF);
		PF_CHARGE_LED_OFF();
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

VWORD BatteryRead(WORD addr)
{
	BYTE i = 2;
	word_read temp;
	BatteryTARSetup();
	while(I2c_Check_TFE(BAT_I2C_CHANNEL))
	{
	}
	I2c_Writeb(addr, I2C_DATA_CMD_OFFSET, BAT_I2C_CHANNEL);
	I2c_Writeb(I2C_WRITE, I2C_DATA_CMD_RWDIR, BAT_I2C_CHANNEL);
	I2c_Writeb(I2C_READ, I2C_DATA_CMD_RWDIR, BAT_I2C_CHANNEL);
	I2c_Writeb(I2C_READ | I2C_STOP, I2C_DATA_CMD_RWDIR, BAT_I2C_CHANNEL);
	while((0 == I2c_Check_RFNE(BAT_I2C_CHANNEL)) && (i--))
	{
		temp.data0[1 - i] = I2c_Readb(I2C_DATA_CMD_OFFSET, BAT_I2C_CHANNEL);
	}
	if(i)
	{
	}
	return temp.data;

}

VWORD ChargerRead(WORD addr)
{
	BYTE i = 2;
	word_read temp;
	ChargerTARSetup();
	while(I2c_Check_TFE(CHG_I2C_CHANNEL))
	{
	}
	I2c_Writeb(addr, I2C_DATA_CMD_OFFSET, CHG_I2C_CHANNEL);
	I2c_Writeb(I2C_WRITE, I2C_DATA_CMD_RWDIR, CHG_I2C_CHANNEL);
	I2c_Writeb(I2C_READ, I2C_DATA_CMD_RWDIR, CHG_I2C_CHANNEL);
	I2c_Writeb(I2C_READ | I2C_STOP, I2C_DATA_CMD_RWDIR, CHG_I2C_CHANNEL);
	while((0 == I2c_Check_RFNE(CHG_I2C_CHANNEL)) && (i--))
	{
		temp.data0[1 - i] = I2c_Readb(I2C_DATA_CMD_OFFSET, CHG_I2C_CHANNEL);
	}
	if(i)
	{
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

void SetBAT1IDTimeOutCNT(void)
{
	BAT1_ID_TimeOut_CNT = BatIDTimeOut;
}

void SetBAT1CtrlStep(BYTE ctrlstep)
{
	BAT1_CtrlStep = ctrlstep;
	Charger_TimeOut = 0x00;
	switch(BAT1_CtrlStep)
	{
		case BAT_Step_ID:
			SetBAT1IDTimeOutCNT();
			BAT1_ID_Step = BATID_Step_GetDV;
			break;
		case BAT_Step_WC:
			BT1_STATUS1 &= ~(bat_InCharge + bat_Dischg + bat_Full);
			BT1_STATUS2 = 0x00;
			WakeUpChrCunt = 0x0000;
			WakeUpChrCunt_Min = 0x00;
			CLEAR_MASK(BT1_STATUS1, bat_valid);
			PF_CHARGE_FULL_LED_OFF();
			SET_MASK(BT1_STATUS2, bat_WC);
			break;
		case BAT_Step_PC:
			BT1_STATUS1 &= ~(bat_InCharge + bat_Dischg + bat_Full);
			BT1_STATUS2 = 0x00;
			PreChrCunt = 0x0000;
			PreChrCunt_Min = 0x0000;
			PF_CHARGE_FULL_LED_OFF();
			SET_MASK(BT1_STATUS2, bat_PC);
			break;
		case BAT_Step_NC:
			BT1_STATUS1 &= ~(bat_InCharge + bat_Dischg + bat_Full);
			BT1_STATUS2 = 0x00;
			NormalChrCunt = 0x0000;
			NormalChrCunt_Min = 0x0000;
			CHARGE_CURRENT_BK = 0x00;
			CHARGE_VOLTAGE_BK = 0x00;
			PF_CHARGE_FULL_LED_OFF();
			SET_MASK(BT1_STATUS2, bat_NC);
			break;
		case BAT_Step_DC:
			BT1_STATUS1 &= ~(bat_InCharge + bat_Dischg + bat_Full);
			BT1_STATUS2 = 0x00;
			PF_CHARGE_FULL_LED_OFF();
			SET_MASK(BT1_STATUS2, bat_DC);
			SET_MASK(BT1_STATUS1, bat_Dischg);
			break;
		case BAT_Step_FullyChg:
			BT1_STATUS1 &= ~(bat_InCharge + bat_Dischg + bat_Full);
			SET_MASK(BT1_STATUS1, bat_Full);
			BT1_STATUS2 = 0x00;
			TimerCount = 0;
			break;
	}
}

void BAT1_SetFail(BYTE failcause)
{
	BAT1_FailCause = failcause;
	SetBAT1CtrlStep(BAT_Step_SetFail);
}

void PollingBAT1Data(void)
{
	if(BAT1_PollingDataIndex < (sizeof(BAT1_PollingDataTable) / sizeof(BatteryData)))
	{
		if((BAT1_PollingDataTable[BAT1_PollingDataIndex].Var) != NULL)
		{
			*(BAT1_PollingDataTable[BAT1_PollingDataIndex].Var) = BatteryRead(BAT1_PollingDataTable[BAT1_PollingDataIndex].Cmd);
		}
		if((BAT1_PollingDataTable[BAT1_PollingDataIndex].Bat_Ecsapce.Var_L) != NULL)
		{
			*(BAT1_PollingDataTable[BAT1_PollingDataIndex].Bat_Ecsapce.Var_L) = (VBYTE)((*(BAT1_PollingDataTable[BAT1_PollingDataIndex].Var)) & 0xff);
		}
		else
		{
			//处理空指针的情况
		}
		if((BAT1_PollingDataTable[BAT1_PollingDataIndex].Bat_Ecsapce.Var_H) != NULL)
		{
			*(BAT1_PollingDataTable[BAT1_PollingDataIndex].Bat_Ecsapce.Var_H) = (VBYTE)(((*(BAT1_PollingDataTable[BAT1_PollingDataIndex].Var)) >> 8) & 0xff);
		}
		else
		{
			//处理空指针的情况
		}
		BAT1_PollingDataIndex++;
		if(BAT1_PollingDataIndex >= (sizeof(BAT1_PollingDataTable) / sizeof(BatteryData)))
		{
			BAT1_PollingDataIndex = 0;
			//可做特殊处理例如：BATTERY1_TEMP_H = (VBYTE)(((BAT1_TEMP - 2730) >> 8) & 0xFF);
		}
	}
}

void BAT1Protection(void)
{
	WORD voltage;
	BAT1_ALARM2 = (BYTE)((BAT1_ALARM1 >> 8) & 0x00FF);
	if(IS_MASK_SET(BAT1_ALARM2, OverTemp_alarm))
	{
		if(++BAT1_OTCunt > BatOTTimeOut)
		{
			BAT1_SetFail(Bat_OverTemperature);
		}
	}
	else
	{
		BAT1_OTCunt = 0x00;
	}
	voltage = BATTERY1_VOL_L;
	if(voltage > BATOverVoltage)
	{
		if(++BAT1_OVCunt > BatOVTimeOut)
		{
			BAT1_SetFail(Bat_OverVoltage);
		}
	}
	else
	{
		BAT1_OVCunt = 0x00;
	}
}

void BAT1ForceStep(void)
{
	if(IS_MASK_SET(BT1_STATUS3, bat_ForceChg))
	{
		if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
		{
			if(IS_MASK_SET(BT1_STATUS1, bat_Dischg))
			{
				SetBAT1CtrlStep(BAT_Step_PC);
			}
		}
	}
	else if(IS_MASK_SET(BT1_STATUS3, bat_ForceDischg))
	{
		if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
		{
			if(IS_MASK_SET(BT1_STATUS1, bat_InCharge))
			{
				SetBAT1CtrlStep(BAT_Step_DC);
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
		CHARGE_CURRENT_LO = 00;
		CHARGE_VOLTAGE_LO = 00;
		INPUT_CURRENT_LO = IN_CURRENT;
		if(SetSmartCharger() != 0x00)
		{
			Charger_TimeOut++;
			if(Charger_TimeOut > ChargerSettingTimeOut)
			{
				SetBAT1CtrlStep(BAT_Step_ChargerFail);
			}
			result = 0x00;
		}
		else
		{
			if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
			{
				if(IS_MASK_SET(BT1_STATUS3, bat_ForceDischg))
				{
					Hook_BAT1_ChargePinON(0);
				}
				else
				{
					Hook_BAT1_ChargePinON(1);
				}
			}
			else
			{
				Hook_BAT1_ChargePinON(0);
				SET_MASK(BT1_STATUS2, bat_DC_OK);
				SET_MASK(BT1_STATUS1, bat_Dischg);
			}
			Charger_TimeOut = 0x00;
			result = 0x01;
		}
	}
	else
	{
		if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
		{
			if(IS_MASK_SET(BT1_STATUS3, bat_ForceDischg))
			{
				Hook_BAT1_ChargePinON(0);
			}
			else
			{
				Hook_BAT1_ChargePinON(1);
			}
		}
		else
		{
			Hook_BAT1_ChargePinON(0);
			SET_MASK(BT1_STATUS2, bat_DC_OK);
			SET_MASK(BT1_STATUS1, bat_Dischg);
		}
		result = 0x01;
	}
	return (result);
}

void BAT1_CheckFullyCharged(void)
{
	if((BAT1_CtrlStep == BAT_Step_PC) || (BAT1_CtrlStep == BAT_Step_NC))
	{
		if(IS_MASK_SET(BAT1_ALARM1, FullyChg))
		{
			if(DisableCharger() == 0x01)
			{
				SetBAT1CtrlStep(BAT_Step_FullyChg);
				PF_CHARGE_FULL_LED_ON();
				PF_CHARGE_LED_OFF();
			}
		}
		else if(BAT1_RSOC >= 95)
		{
			if(BAT1_CC_L == 0)
			{
				if(DisableCharger() == 0x01)
				{
					SetBAT1CtrlStep(BAT_Step_FullyChg);
					SET_MASK(BT1_STATUS1, bat_Full);
					PF_CHARGE_FULL_LED_ON();
					PF_CHARGE_LED_OFF();
				}
			}
			else if(BAT1_CURRENT < 0x64)
			{
				TimerCount++;
				if(TimerCount == 6000)
				{
					TimerCount = 0;
					if(DisableCharger() == 0x01)
					{
						SetBAT1CtrlStep(BAT_Step_FullyChg);
						PF_CHARGE_FULL_LED_ON();
						PF_CHARGE_LED_OFF();
					}
				}
			}
		}
		else
		{
			TimerCount = 0;
		}
	}
}

void BAT1_CheckFullyDisCharged(void)
{
	if(BAT1_CtrlStep == BAT_Step_DC)
	{
		if(IS_MASK_SET(BAT1_ALARM1, FullyDsg))
		{
			SetBAT1CtrlStep(BAT_Step_FullyDisChg);
		}
	}
}

void BAT1IdentifyNextStep(void)
{
	BAT1_ID_Step++;
	if(BAT1_ID_Step == BATID_Step_FirstData)
	{
		BAT1_FirstDataIndex = 0x00;
	}
	SetBAT1IDTimeOutCNT();
}

BYTE SetSmartCharger(void)
{
	WORD mode = 0;
	mode &= ~(BIT_BAT_MODE_IN_CTRL | BIT_BAT_MODE_PRIMARY);
	mode |= BIT_BAT_MODE_DIS_ALARM | BIT_BAT_MODE_DIS_CHG;
	BatteryWrite(_CMD_BatteryMode, mode);
	ChargerWrite(SC85_VINREG, 0x2400);
	ChargerWrite(SC85_InputCurrent, IN_CURRENT);
	ChargerWrite(SC85_ChargeVoltage, CHARGE_VOLTAGE_LO);
	ChargerWrite(SC85_ChargeCurrent, CHARGE_CURRENT_LO);
	return 0;
}

uint8_t I2C1_charge_init(void)
{
	CHARGE_OPTIONS = 0xE70E;
	ChargerWrite(SC85_ChargeOption0, CHARGE_OPTIONS & 0xFFFF);
	CHARGE_OPTIONS = 0x0237;
	ChargerWrite(SC85_ChargeOption2, CHARGE_OPTIONS & 0xFFFF);
	CHARGE_OPTIONS = 0x7F00;
	ChargerWrite(SC85_InputCurrent, CHARGE_OPTIONS & 0xFFFF);
	CHARGE_OPTIONS = 0x4A64;
	ChargerWrite(SC85_Prochot_Option0, CHARGE_OPTIONS & 0xFFFF);
	return 1;
}

void BAT1ID_GetDesignVoltage(void)
{
	short int ID;
	ID = ChargerRead(SC85_DeviceID);
	if(ID != 0x68)
	{
		return;
	}
	WORD DVTemp;
	ByteTemp01 = BatteryRead(BATCmd_DVolt);
	if(ByteTemp01)
	{
		DVTemp = ByteTemp01;
		BATTERY1_DVOL_L = ByteTemp01;
		if(DVTemp < 8400)
		{
			BAT1_S_Number = 0x02;
		}
		else if(DVTemp < 12600)
		{
			BAT1_S_Number = 0x03;
		}
		else
		{
			BAT1_S_Number = 0x04;
		}
		BAT1IdentifyNextStep();
		ByteTemp01 = 0x00;
		ByteTemp02 = 0x00;
	}
	else
	{
		BAT1_ID_TimeOut_CNT--;
		if(BAT1_ID_TimeOut_CNT == 0x00)
		{
			SetBAT1CtrlStep(BAT_Step_WC);
		}
	}
}

void BAT1ID_CheckBatAuth(void)
{
	BAT1IdentifyNextStep();
}

void BAT1ID_CheckDeviceName(void)
{
	BAT1IdentifyNextStep();
}

void BAT1ID_PollingFirstData(void)
{
	for(BAT1_FirstDataIndex = 0; BAT1_FirstDataIndex < (sizeof(BAT1_PollingDataTable) / sizeof(BatteryData)); BAT1_FirstDataIndex++)
	{
		if((BAT1_PollingDataTable[BAT1_PollingDataIndex].Var) != NULL)
		{
			*(BAT1_PollingDataTable[BAT1_PollingDataIndex].Var) = BatteryRead(BAT1_PollingDataTable[BAT1_PollingDataIndex].Cmd);
		}
		if((BAT1_PollingDataTable[BAT1_PollingDataIndex].Bat_Ecsapce.Var_L) != NULL)
		{
			if((BAT1_PollingDataTable[BAT1_PollingDataIndex].Var) != NULL)
				*(BAT1_PollingDataTable[BAT1_PollingDataIndex].Bat_Ecsapce.Var_L) = (VBYTE)((*(BAT1_PollingDataTable[BAT1_PollingDataIndex].Var)) & 0xff);
			else
				*(BAT1_PollingDataTable[BAT1_PollingDataIndex].Bat_Ecsapce.Var_L) = (VBYTE)(BatteryRead(BAT1_PollingDataTable[BAT1_PollingDataIndex].Cmd) & 0xff);
		}
		else
		{
			//处理空指针的情况
		}
		if((BAT1_PollingDataTable[BAT1_PollingDataIndex].Bat_Ecsapce.Var_H) != NULL)
		{
			if((BAT1_PollingDataTable[BAT1_PollingDataIndex].Var) != NULL)
				*(BAT1_PollingDataTable[BAT1_PollingDataIndex].Bat_Ecsapce.Var_H) = (VBYTE)(((*(BAT1_PollingDataTable[BAT1_PollingDataIndex].Var)) >> 8) & 0xff);
			else
				*(BAT1_PollingDataTable[BAT1_PollingDataIndex].Bat_Ecsapce.Var_H) = (VBYTE)(((BatteryRead(BAT1_PollingDataTable[BAT1_PollingDataIndex].Cmd)) >> 8) & 0xff);
		}
		else
		{
			//处理空指针的情况
		}
	}
	//可做特殊处理例如：BATTERY1_TEMP_H = (VBYTE)(((BAT1_TEMP - 2730) >> 8) & 0xFF);
	BAT1IdentifyNextStep();
}

void BAT1ID_InitOK(void)
{
	SET_MASK(BT1_STATUS1, bat_valid);
	if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
	{
		SetBAT1CtrlStep(BAT_Step_PC);
	}
	else
	{
		SetBAT1CtrlStep(BAT_Step_DC);
	}
}

void IdentifyBAT1(void)
{
	switch(BAT1_ID_Step)
	{
		case BATID_Step_GetDV:
			BAT1ID_GetDesignVoltage();
			break;
		case BATID_Step_Auth:
			BAT1ID_CheckBatAuth();
			break;
		case BATID_Step_MFName:
			BAT1ID_CheckDeviceName();
			break;
		case BATID_Step_FirstData:
			BAT1ID_PollingFirstData();
			break;
		case BATID_Step_InitOK:
			BAT1ID_InitOK();
			break;
		default:
			break;
	}
}

void BAT1_SETWakeUpCharge(void)
{
	if(SmartCharger_Support)
	{
		CHARGE_CURRENT_LO = BAT_CURR_WUCHG;
		CHARGE_VOLTAGE_LO = BAT_VOLT_WUCHG;
		INPUT_CURRENT_LO = IN_CURRENT;
		if(SetSmartCharger() != 0x00)
		{
			Charger_TimeOut++;
			if(Charger_TimeOut > ChargerSettingTimeOut)
			{
				SetBAT1CtrlStep(BAT_Step_ChargerFail);
			}
		}
		else
		{
			Hook_BAT1_ChargePinON(1);
			SET_MASK(BT1_STATUS2, bat_WC_OK);
			SET_MASK(BT1_STATUS1, bat_InCharge);
			Charger_TimeOut = 0x00;
		}
	}
	else
	{
		Hook_BAT1_ChargePinON(1);
		SET_MASK(BT1_STATUS2, bat_WC_OK);
		SET_MASK(BT1_STATUS1, bat_InCharge);
	}
}

void BAT1WakeUpCharge(void)
{
	if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
	{
		if(IS_MASK_CLEAR(BT1_STATUS2, bat_WC_OK))
		{
			BAT1_SETWakeUpCharge();
		}
		else
		{
			WakeUpChrCunt++;
			if((WakeUpChrCunt % 6000) == 0)
			{
				BAT1_SETWakeUpCharge();
				WakeUpChrCunt = 0x0000;
				WakeUpChrCunt_Min++;
				if(WakeUpChrCunt_Min > BatWCTimeOut)
				{
					BAT1_SetFail(Bat_WakeUpChargeFail);
				}
			}
			else if((WakeUpChrCunt % 600) == 0)
			{
				BAT1_RSOC = BatteryRead(BATCmd_RMcap);
				if(BAT1_RSOC == 0)
				{
					SetBAT1CtrlStep(BAT_Step_ID);
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

void BAT1_SETPreCharge(void)
{
	if(SmartCharger_Support)
	{
		PF_CHARGE_LED_ON();
		CHARGE_CURRENT_LO = BAT_CURR_WUCHG;
		CHARGE_VOLTAGE_LO = BAT_VOLT_PRECHG;
		INPUT_CURRENT_LO = IN_CURRENT;
		if(SetSmartCharger() != 0x00)
		{
			Charger_TimeOut++;
			if(Charger_TimeOut > ChargerSettingTimeOut)
			{
				SetBAT1CtrlStep(BAT_Step_ChargerFail);
			}
		}
		else
		{
			Hook_BAT1_ChargePinON(1);
			SET_MASK(BT1_STATUS2, bat_PC_OK);
			SET_MASK(BT1_STATUS1, bat_InCharge);
			Charger_TimeOut = 0x00;
		}
	}
	else
	{
		Hook_BAT1_ChargePinON(1);
		SET_MASK(BT1_STATUS2, bat_PC_OK);
		SET_MASK(BT1_STATUS1, bat_InCharge);
	}
}

void BAT1_PreCharge(void)
{
	WORD cctemp;
	if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
	{
		if(IS_MASK_CLEAR(BT1_STATUS2, bat_PC_OK))
		{
			PreChrCunt = 0x00;
			BAT1_SETPreCharge();
		}
		else
		{
			PreChrCunt++;
			if((PreChrCunt % 6000) == 0)
			{
				BAT1_SETPreCharge();
				PreChrCunt = 0x0000;
				PreChrCunt_Min++;
				if(PreChrCunt_Min > BatPCTimeOut)
				{
					BAT1_SetFail(Bat_PreChargeFail);
				}
			}
			else if((PreChrCunt % 600) == 0)
			{
				cctemp = BAT1_CC_L;
				if(cctemp > Bat1_PC2NC_Cap)
				{
					SetBAT1CtrlStep(BAT_Step_NC);
				}
			}
		}
	}
	else
	{
		SetBAT1CtrlStep(BAT_Step_DC);
	}
}

void BAT1_SETNormalCharge(void)
{
	WORD temp;
	if(SmartCharger_Support)
	{
		PF_CHARGE_LED_ON();
		if(BAT1_CC_L > BAT_CURR_NMCHG)
		{
			temp = BAT_CURR_NMCHG;
			CHARGE_CURRENT_BK = BAT_CURR_NMCHG;
		}
		else
		{
			temp = BAT1_CC_L;
			CHARGE_CURRENT_BK = temp;
		}
		temp *= SC_Current_Sense;
		CHARGE_CURRENT_LO = temp;
		CHARGE_VOLTAGE_LO = 16800;
		INPUT_CURRENT_LO = IN_CURRENT;
		if(SetSmartCharger() != 0x00)
		{
			Charger_TimeOut++;
			if(Charger_TimeOut > ChargerSettingTimeOut)
			{
				SetBAT1CtrlStep(BAT_Step_ChargerFail);
			}
		}
		else
		{
			Hook_BAT1_ChargePinON(1);
			SET_MASK(BT1_STATUS2, bat_NC_OK);
			SET_MASK(BT1_STATUS1, bat_InCharge);
			Charger_TimeOut = 0x00;
		}
	}
	else
	{
		Hook_BAT1_ChargePinON(1);
		SET_MASK(BT1_STATUS2, bat_NC_OK);
		SET_MASK(BT1_STATUS1, bat_InCharge);
	}
}

void BAT1_NormalCharge(void)
{
	if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
	{
		if(IS_MASK_CLEAR(BT1_STATUS2, bat_NC_OK))
		{
			CHARGE_CURRENT_BK = BAT1_CC_L;
			NormalChrCunt = 0x0000;
			BAT1_SETNormalCharge();
		}
		else
		{
			NormalChrCunt++;
			if(BAT1_CC_L > BAT_CURR_NMCHG)
			{
				BAT1_CC_L = BAT_CURR_NMCHG;
			}
			if(CHARGE_CURRENT_BK != BAT1_CC_L)
			{
				CHARGE_CURRENT_BK = BAT1_CC_L;
				BAT1_SETNormalCharge();
			}
			else
			{
				if((NormalChrCunt % 6000) == 0)
				{
					NormalChrCunt_Min++;
					if(NormalChrCunt_Min > BatNCTimeOut)
					{
						BAT1_SetFail(Bat_NormalChargeFail);
					}
					BAT1_SETNormalCharge();
					NormalChrCunt = 0x0000;
				}
			}
		}
	}
	else
	{
		SetBAT1CtrlStep(BAT_Step_DC);
	}
}

void BAT1DisCharge(void)
{
	if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
	{
		if(IS_MASK_SET(BT1_STATUS3, bat_ForceDischg))
		{
			if(IS_MASK_CLEAR(BT1_STATUS2, bat_DC_OK))
			{
				DisableCharger();
			}
		}
		else
		{
			SetBAT1CtrlStep(BAT_Step_PC);
		}
	}
	else
	{
		if(IS_MASK_CLEAR(BT1_STATUS2, bat_DC_OK))
		{
			DisableCharger();
		}
	}
}

void BAT1FullyCharged(void)
{
	if(IS_MASK_CLEAR(CTRL_FLAG1, adapter_in))
	{
		SetBAT1CtrlStep(BAT_Step_DC);
	}
}

void BAT1FullyDisharged(void)
{
	if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
	{
		SetBAT1CtrlStep(BAT_Step_WC);
	}
	else
	{
		SetBAT1CtrlStep(BAT_Step_FullyDisChg);
	}
}

void BAT1SetFail(void)
{
	if(DisableCharger() == 0x01)
	{
		BAT1_PlugOutClrVariables();
		SetBAT1CtrlStep(BAT_Step_Fail);
	}
}

void BAT1ForceDischarge(void)
{}

void BAT1ForceCharge(void)
{}

void BAT1AutoLearning(void)
{}

void Battey1ControlCenter(void)
{
	if(IS_MASK_CLEAR(CTRL_FLAG1, adapter_in))
	{
		PF_CHARGE_LED_OFF();
		PF_CHARGE_FULL_LED_OFF();
	}
	if(IS_MASK_CLEAR(CTRL_FLAG1, bat_in))
	{
		PF_CHARGE_LED_OFF();
		PF_CHARGE_FULL_LED_OFF();
		return;
	}
	if(IS_MASK_SET(BT1_STATUS1, bat_valid))
	{
		BAT1_1SecTimer++;
		if(((BAT1_1SecTimer % 4) == 0))
		{
			ChargerData20 = ChargerRead(SC85_Status);
			if((ChargerData20 & 0x0010) != 0x0000)
			{
				ChargerData20 = (ChargerData20 & 0xFFEF);
				ChargerWrite(SC85_Status, ChargerData20 & 0xFFFF);
			}
		}
		// if (BAT1_1SecTimer > 100)	// sparkle debug lost key when bat in 23.11.22
		// {							// sparkle debug lost key when bat in 23.11.22
		BAT1_1SecTimer = 0x00;
		ChargerData21 = ChargerRead(SC85_Prochot_Status);
		if((ChargerData21 & 0x0080) != 0x0000)
		{
			ChargerData21 = (ChargerData21 & 0xF77F);
			ChargerWrite(SC85_Prochot_Status, ChargerData21 & 0xFFFF);
			Hook_BAT1_ChargePinON(0);
		}
		PollingBAT1Data();
		BAT1Protection();
		// }							// sparkle debug lost key when bat in 23.11.22
		BAT1ForceStep();
		if(IS_MASK_SET(CTRL_FLAG1, adapter_in))
		{
			BAT1_CheckFullyCharged();
		}
		else
		{
			BAT1_CheckFullyDisCharged();
		}
	}
	switch(BAT1_CtrlStep)
	{
		case BAT_Step_ID:
			IdentifyBAT1();
			break;
		case BAT_Step_WC:
			BAT1WakeUpCharge();
			break;
		case BAT_Step_PC:
			BAT1_PreCharge();
			break;
		case BAT_Step_NC:
			BAT1_NormalCharge();
			break;
		case BAT_Step_DC:
			BAT1DisCharge();
			break;
		case BAT_Step_FullyChg:
			BAT1FullyCharged();
			break;
		case BAT_Step_FullyDisChg:
			BAT1FullyDisharged();
			break;
		case BAT_Step_SetFail:
			BAT1SetFail();
			break;
		case BAT_Step_Fail:
			break;
		case BAT_Step_ForceDC:
			BAT1ForceDischarge();
			break;
		case BAT_Step_ForceC:
			BAT1ForceCharge();
			break;
		case BAT_Step_AutoLN:
			BAT1AutoLearning();
			break;
		case BAT_Step_ChargerFail:
			break;
		case BAT_Step_Out:
			break;
		default:
			break;
	}
}
