/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
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
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
#include "KERNEL_PWM.H"
/*风扇扫描level等级结构???*/
typedef struct thermalstruct
{
	BYTE  	CSTEP;    //风扇调速step,对应风扇的转速调节系数
	s8  	CFanOn;		//温度上限
	s8  	CFanOff;	//温度下限
	BYTE 	CDCR;      //初始占空比
}thermal;
#define FAN0_CTR 246//245/246/255
#define FAN0_MAX_DCR (FAN0_CTR)
#define FAN0_MIN_DCR (0)
#define TEMP_SETPOINT  20
#define TEMP_MAX  65
#define _Kp  8 // 比例系数的倒数
#define _Ki  10 // 积分系数的倒数
#define _Kd  2  // 微分系数的倒数
// #define FAN1_CTR_MAX_DCR 246

extern s8  ReadCPUTemp(void);
extern s8  ReadSysTemp(void);
BYTE FAN_LEV(BYTE channel);
BYTE FAN_Dynamic(BYTE channel);//动态算法
BYTE FAN_PID(BYTE channel);//PID算法
BYTE FAN_Init(BYTE channel, BYTE clk, BYTE ctr);
#endif
