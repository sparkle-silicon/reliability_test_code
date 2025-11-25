/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2024-01-03 09:56:22
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
#ifndef __CUSTOM_FAN_H
#define __CUSTOM_FAN_H
#include "AE_GLOBAL.H"
#include "KERNEL_PWM.H"

/*-----------------------------------------------------------------------------
 * Fan Level Structure
 *---------------------------------------------------------------------------*/
typedef struct thermalstruct
{
	BYTE  	CSTEP;    //风扇调速step,对应风扇的转速调节系数
	s8  	CFanOn;		//温度上限
	s8  	CFanOff;	//温度下限
	BYTE 	CDCR;      //初始占空比
}thermal;
/*-----------------------------------------------------------------------------
 * Fan Control 
 *---------------------------------------------------------------------------*/
#define FAN0_CTR 246//245/246/255
#define FAN0_MAX_DCR (FAN0_CTR)
#define FAN0_MIN_DCR (0)
#define FAN_Dynamic_Adj   0 //动态调节
#define FAN_LEV_Adj   0 //等级调节
#define FAN_PID_Adj   0 // PID调节
#define TEMP_SETPOINT  45 //动态算法平衡点
#define TEMP_SETPOINT2  48 //PID稳定点
#define TEMP_LEV 20 //等级算法
#define TEMP_MAX  75
#define _Kp  0.5 // 比例系数倒数
#define _Ki  0.33 // 积分系数倒数
#define _Kd  10  // 微分系数
// #define FAN1_CTR_MAX_DCR 246
/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
extern s8  ReadCPUTemp(void);
extern s8  ReadSysTemp(void);
extern BYTE FAN_LEV(BYTE channel);
extern BYTE FAN_Dynamic(BYTE channel);//动态算法
extern BYTE FAN_PID(BYTE channel);//PID算法
extern BYTE FAN_Init(BYTE channel, BYTE clk, BYTE ctr);
#endif
