/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:20:07
 * @Description:
 *
 *
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 以下是中文及英文版权同步声明，编码为UTF-8。
 * Copyright has legal effects and violations will be prosecuted.
 * 版权具有法律效力，违反必究。
 *
 * Copyright ©2021-2025 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2025龙晶石半导体科技（苏州）有限公司
 */
#ifndef __CUSTOM_EVENT_H
#define __CUSTOM_EVENT_H
#include "AE_GLOBAL.H"

//---------------------------------------------------------------------------------
//Debounce Time Setting
//---------------------------------------------------------------------------------
#define T_AC_DEBOUNCE    	    5       // 50 ms (timebase: 10 ms)
#define T_PSW_DEBOUNCE   	    5    	// 50 ms (timebase: 10 ms)
#define T_BAT_DEBOUNCE   	    15      // 150 ms (timebase: 10 ms)
#define T_PSWOFF         	    500 	// 5 secs (timebase: 10ms) EC 5sec override turn off system power
#define T_EXTEVT_DEBOUNCE 	    10      // 100 ms (timebase: 10 ms)

struct sEvent{
	VBYTE *REG_Evt;
	BYTE	Flag_Evt;
	VBYTE *Cunter_Evt;
	BYTE	Time_Evt;
	FUNCT_PTR_V_V	press_evt;
	FUNCT_PTR_V_V	release_evt;
	FUNCT_PTR_V_V	nochange_evt;
};

//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------
extern void Event_Center(BYTE Count_id);

#endif