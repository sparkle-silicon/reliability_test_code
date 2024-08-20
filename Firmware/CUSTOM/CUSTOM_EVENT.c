/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-11-03 11:16:42
 * @Description: Event support
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
#include "CUSTOM_EVENT.H"
#include "CUSTOM_GPIO.H"
#include "KERNEL_MEMORY.H"
#include "CUSTOM_POWER.H"
//-----------------------------------------------------------------------------
// no change function
//-----------------------------------------------------------------------------
void NullEvent(void)
{}
//-----------------------------------------------------------------------------
// Adapter in function
//-----------------------------------------------------------------------------
void AdapterIn(void)
{}
//-----------------------------------------------------------------------------
// Adapter out function
//-----------------------------------------------------------------------------
void AdapterOut(void)
{}
//-----------------------------------------------------------------------------
// power button was pressed
//-----------------------------------------------------------------------------
void PSWPressed(void)
{
	PSW_COUNTER = T_PSWOFF;
	switch(System_PowerState)
	{
		case SYSTEM_S5:
			Custom_S5_S0_Trigger();
			break;
		case SYSTEM_S4:
			Custom_S4_S0_Trigger();
			break;
		case SYSTEM_S3:
			Custom_S3_S0_Trigger();
			break;
		case SYSTEM_S0:
			dprint("SYSTEM_S0\n");
			// ECQEvent();
			break;
		default:
			break;
	}
}
//-----------------------------------------------------------------------------
// power button was released
//-----------------------------------------------------------------------------
void PSWReleased(void)
{
	printf("power button released\n");
}
//-----------------------------------------------------------------------------
// power button was overrided
//-----------------------------------------------------------------------------
void PSWOverrided(void)
{
	if(IS_MASK_SET(CTRL_FLAG1, pwrbtn_press)) // power button was pressed
	{
		if(PSW_COUNTER != 0) // pwrbutton press 4 second
		{
			PSW_COUNTER -= 1;	  // PSW_COUNTER count down
			if(PSW_COUNTER == 0) // time out
			{
				// Oem_TriggerS0S5(SC_4SEC);
			}
		}
	}
}
//-----------------------------------------------------------------------------
// battery 1 in function
//-----------------------------------------------------------------------------
void Battery1In(void)
{}
//-----------------------------------------------------------------------------
// battery 1 out function
//-----------------------------------------------------------------------------
void Battery1Out(void)
{}
// ----------------------------------------------------------------------------
// Device insert/remove debounce routine.
// input:   Count_id
// ----------------------------------------------------------------------------
const struct sEvent Event_Table[] =
{
	{&CTRL_FLAG1, pwrbtn_press, &DEBOUNCE_CONT0, T_PSW_DEBOUNCE, PSWPressed, PSWReleased, PSWOverrided},
	{&CTRL_FLAG1, adapter_in, &DEBOUNCE_CONT1, T_AC_DEBOUNCE, AdapterIn, AdapterOut, NullEvent},
	{&CTRL_FLAG1, bat_in, &DEBOUNCE_CONT2, T_BAT_DEBOUNCE, Battery1In, Battery1Out, NullEvent},
	{&EVENT_FLAG1, DummyFlag, &DEBOUNCE_CONT3, T_EXTEVT_DEBOUNCE, NullEvent, NullEvent, NullEvent},
	{&EVENT_FLAG1, DummyFlag, &DEBOUNCE_CONT4, T_EXTEVT_DEBOUNCE, NullEvent, NullEvent, NullEvent},
	{&EVENT_FLAG1, DummyFlag, &DEBOUNCE_CONT5, T_EXTEVT_DEBOUNCE, NullEvent, NullEvent, NullEvent},
	{&EVENT_FLAG1, DummyFlag, &DEBOUNCE_CONT6, T_EXTEVT_DEBOUNCE, NullEvent, NullEvent, NullEvent},
	{&EVENT_FLAG1, DummyFlag, &DEBOUNCE_CONT7, T_EXTEVT_DEBOUNCE, NullEvent, NullEvent, NullEvent},
	{&EVENT_FLAG1, DummyFlag, &DEBOUNCE_CONT8, T_EXTEVT_DEBOUNCE, NullEvent, NullEvent, NullEvent},
	{&EVENT_FLAG1, DummyFlag, &DEBOUNCE_CONT9, T_EXTEVT_DEBOUNCE, NullEvent, NullEvent, NullEvent},
};
// ----------------------------------------------------------------------------
// Event_Center function
//
// Note :
//	If want to call the function of status not change, please set nochange_func = 1
// ----------------------------------------------------------------------------
void Event_Center(BYTE Count_id)
{
	BYTE new_state, old_state;
	BYTE record_flag;
	switch(Count_id)
	{
		case 0:
			new_state = Read_PWR_SW();
			break;
		case 1:
			// new_state = Read_BC_ACOK();
			new_state = 0x00;
			break;
		case 2:
			// new_state = Read_BATT_PRS_1();
			new_state = 0x00;
			break;
		case 3:
			new_state = 0x00;
			break;
		case 4:
			new_state = 0x00;
			break;
		case 5:
			new_state = 0x00;
			break;
		case 6:
			new_state = 0x00;
			break;
		case 7:
			new_state = 0x00;
			break;
		case 8:
			new_state = 0x00;
			break;
		case 9:
			new_state = 0x00;
			break;
		default:
			new_state = 0x00;
			break;
	}
	Tmp_XPntr = Event_Table[Count_id].REG_Evt;
	Tmp_XPntr1 = Event_Table[Count_id].Cunter_Evt;
	if((*Tmp_XPntr1) != 0)
	{
		DEBOUNCE_FLAG0 = 1;
	}
	else
	{
		DEBOUNCE_FLAG0 = 0;
	}
	record_flag = Event_Table[Count_id].Flag_Evt;
	old_state = ((*Tmp_XPntr & record_flag) != 0x00);
	if(new_state != old_state)
	{
		if((*Tmp_XPntr1) == 0x00)
		{
			(*Tmp_XPntr1) = Event_Table[Count_id].Time_Evt;
		}
		else
		{
			(*Tmp_XPntr1)--;
			if((*Tmp_XPntr1) == 0)
			{
				if(new_state)
				{
					(Event_Table[Count_id].press_evt)();
					*Tmp_XPntr |= record_flag;
				}
				else
				{
					(Event_Table[Count_id].release_evt)();
					*Tmp_XPntr &= (~record_flag);
				}
			}
		}
	}
	else
	{
		if((*Tmp_XPntr1) != 0)
		{
			if(DEBOUNCE_FLAG0 == 0)
				(*Tmp_XPntr1) = Event_Table[Count_id].Time_Evt;
		}
		(Event_Table[Count_id].nochange_evt)();
	}
}
