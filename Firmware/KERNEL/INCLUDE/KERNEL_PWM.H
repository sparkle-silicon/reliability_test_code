/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-02-08 16:08:18
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
#ifndef __KERNEL_PWM_H
#define __KERNEL_PWM_H
#include "AE_GLOBAL.H"
#include "KERNEL_GPIO.H"
/*-----------------------------------------------------------------------------
 * Define Config
 *---------------------------------------------------------------------------*/
#define PWM_TCLK_PRESCALE 1//0:480 prescale/1:4 prescale
#if ((PWM_TCLK_PRESCALE==4)||(PWM_TCLK_PRESCALE==1))
#define PWM_TCLK_FREQ ((HIGH_CHIP_CLOCK)/4)
#else
#define PWM_TCLK_FREQ ((HIGH_CHIP_CLOCK)/480)
#endif
#define PWM_CLK0_PRESCALE 1//(PWM_SCALER0=PWM_CLK0_PRESCALE-1)
#define PWM_CLK1_PRESCALE 2//(PWM_SCALER1=PWM_CLK0_PRESCALE-1)
#define PWM_CLK2_PRESCALE 24//(PWM_SCALER2=PWM_CLK0_PRESCALE-1)
#define PWM_CLK3_PRESCALE 48//(PWM_SCALER3=PWM_CLK0_PRESCALE-1)
#define PWM_CLK0_FREQ (PWM_TCLK_FREQ/PWM_CLK0_PRESCALE)//
#define PWM_CLK1_FREQ (PWM_TCLK_FREQ/PWM_CLK1_PRESCALE)//
#define PWM_CLK2_FREQ (PWM_TCLK_FREQ/PWM_CLK2_PRESCALE)//
#define PWM_CLK3_FREQ (PWM_TCLK_FREQ/PWM_CLK3_PRESCALE)//
#define PWM_CTR0_REG (100)
#define PWM_CTR1_REG (200)
#define PWM_CTR2_REG (246)
#define PWM_CTR3_REG (255)
#define PWM_CTR01 ((PWM_CTR0_REG&0xff)|((PWM_CTR1_REG&0xff)<<8))
#define PWM_CTR23 ((PWM_CTR2_REG&0xff)|((PWM_CTR3_REG&0xff)<<8))
#define PWM_CLK0 0x0 
#define PWM_CLK1 0x1 
#define PWM_CLK2 0x2 
#define PWM_CLK3 0x3 
#define PWM_CTR0 0x0 
#define PWM_CTR1 0x1 
#define PWM_CTR2 0x2 
#define PWM_CTR3 0x3 
#define PWM_HIGH 0x0 
#define PWM_LOW 0x1 
#define PWM_CHANNEL0 0
#define PWM_CHANNEL1 1
#define PWM_CHANNEL2 2
#define PWM_CHANNEL3 3
#define PWM_CHANNEL4 4
#define PWM_CHANNEL5 5
#define PWM_CHANNEL6 6
#define PWM_CHANNEL7 7
#define TACH_CHANNEL0 0
#define TACH_CHANNEL1 1
#define TACH_CHANNEL2 2
#define TACH_CHANNEL3 3
/*-----------------------------------------------------------------------------
 * Parameter Define
 *---------------------------------------------------------------------------*/
#define PRESCALE_4 		0x1
#define PRESCALE_480 	0xfffe
#define CLK_MODE  4
#define TACH0_MODE 8
#define TACH1_MODE 10
#define TACH2_MODE 12
#define TACH3_MODE 14
#define TACH_MODE0  0x0
#define TACH_MODE1  0x1
#define TACH_MODE2  0x2
#define TACH_MODE3  0x3
#define Tach_Int_Enable(channel)	(TACH_INT |= (0x1 << channel))
#define Tach_Int_Disable(channel)	(TACH_INT &= ~(0x1 << channel))
#define Tach_Int_Enable_Read(channel)		((TACH_INT & (0x1 << channel)) != 0)

#define TACH_CLK_PRESCALE 480
#define TACH_CLK (HIGH_CHIP_CLOCK/TACH_CLK_PRESCALE)
#define TACH_MINUTE 60
#define TACH_HOUR 3600
#define TACH_DAY 86400
#define TACH_RPM(tach_cycle) ((TACH_MINUTE*(TACH_CLK))/(tach_cycle))
/*-----------------------------------------------------------------------------
 * Pwm Registers Struct
 *---------------------------------------------------------------------------*/
typedef struct pwm_str{
	BYTE   pwm_no;
	BYTE   prescale;
	BYTE   pwm_polarity;
	BYTE   pwm_softmode;
	WORD pwm_scale;
	BYTE    pwm_ctr;
	BYTE   pwm_clk_sel;
	BYTE   pwm_ctr_sel;
	BYTE    pwm_dcdr;
	BYTE   pwm_step;
} spwm_str;
typedef struct tach_str{
	BYTE   tach_no;
	BYTE   tach_mode;
	BYTE   tach_intr;
} stach_str;
/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
extern BYTE  PWM_Enable(BYTE  pwm_no);
extern BYTE  PWM_Disable(BYTE  pwm_no);
extern void  PWM_CLOCK_Init(void);
extern BYTE  PWM_Init_channel(BYTE channel, BYTE polarity, BYTE clk, BYTE ctr, BYTE dcr, BYTE step);
extern BYTE  PWM_ReInit_channel(BYTE channel, BYTE dcr, BYTE step);
extern BYTE  PWM_Get(spwm_str *pwm_ptr);
extern BYTE  TACH_Enable(BYTE  tach_no);
extern BYTE  TACH_Disable(BYTE  tach_no);
extern BYTE  TACH_Int_Enable(BYTE  tach_no);
extern BYTE  TACH_Int_Disable(BYTE  tach_no);
extern BYTE  TACH_Set(stach_str *tach_ptr);
extern BYTE  TACH_Init_Channel(BYTE channel, BYTE mode, BYTE tach_int);
extern BYTE  TACH_Clear_IRQ(BYTE  tach_no);
extern WORD TACH_Get(BYTE  tach_no);
WORD TACH_Get_Polling(BYTE  tach_no);
#endif
