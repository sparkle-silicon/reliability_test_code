/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-11-09 20:55:34
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
#ifndef __KERNEL_ADC_H
#define __KERNEL_ADC_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"

#define ADC_CHANNEL0 0
#define ADC_CHANNEL1 1
#define ADC_CHANNEL2 2
#define ADC_CHANNEL3 3
#define ADC_CHANNEL4 4
#define ADC_CHANNEL5 5
#define ADC_CHANNEL6 6
#define ADC_CHANNEL7 7
#define ADC_CHANNEL8 8
#define ADC_CHANNEL9 9
#define ADC_CHANNEL10 10
#define ADC_CHANNEL11 11

#define ADC_CLOCK (CHIP_CLOCK_INT_HIGH / ((SYSCTL_CLKDIV_OSC96M_ADC&0x3F)+((SYSCTL_CLKDIV_OSC96M_ADC>>6)&0x3F)))

#define ADC_DATA_CHANNEL0 0
#define ADC_DATA_CHANNEL1 1
#define ADC_DATA_CHANNEL2 2
#define ADC_DATA_CHANNEL3 3
#define ADC_DATA_CHANNEL4 4
#define ADC_DATA_CHANNEL5 5
#define ADC_DATA_CHANNEL6 6
#define ADC_DATA_CHANNEL7 7

#define ADC_SINGLEENDED 0
#define ADC_FULLYDIFFEENTIAL 1

 /**************************ADC Register define**************************/
#define ADC_CTRL_TRIGGER_SOFT       HEX00
#define ADC_CTRL_TRIGGER_HARD       HEX80
#define ADC_CTRL_TRIGGER_CONTINUE   0x100
#define ADC_CTRL_TRIGGER_RESERVE    0x180
#define ADC_CTRL_FORMAT_SOURCE      HEX40
#define ADC_CTRL_FORMAT_COMPLE      HEX00
#define ADC_CTRL_SCALE_EN           HEX20
#define ADC_CTRL_SCALE_DISEN        HEX00
#define ADC_CTRL_MODE_SIGLE         HEX10
#define ADC_CTRL_MODE_DIFF          HEX00
#define ADC_CTRL_FIFO_THR_0         HEX00
#define ADC_CTRL_FIFO_THR_1         HEX01
#define ADC_CTRL_FIFO_THR_2         HEX02
#define ADC_CTRL_FIFO_THR_3         HEX03
#define ADC_CTRL_FIFO_THR_4         HEX04
#define ADC_PM_COMP_PD_EN           HEX00
#define ADC_PM_COMP_PD_DISEN        HEX02
#define ADC_PM_LDO_PD_EN            HEX00
#define ADC_PM_LDO_PD_DISEN         HEX01
#define ADC_TRIGGER_START           HEX01
#define ADC_INTSTAT_UV_CLEAR        HEX04
#define ADC_INTSTAT_OV_CLEAR        HEX02
#define ADC_INTSTAT_AF_CLEAR        HEX01


#define ADC_Int_Mask(type)			(ADC_INTMASK |= (0x1<<type))
#define ADC_Int_Unmask(type)		(ADC_INTMASK &= ~(0x1<<type))
#define ADC_Int_Mask_Read(type)		((ADC_INTMASK & (0x1<<type)) != 0)
#define ADC_Int_Status(type)	((ADC_INTSTAT & (0x1<<type)) != 0)

/*****中断屏蔽宏*******/
#define  ADCData0_intrMask        HEX01
#define  ADCData1_intrMask        HEX02
#define  ADCData2_intrMask        HEX04
#define  ADCData3_intrMask        HEX08
#define  ADCData4_intrMask        HEX10
#define  ADCData5_intrMask        HEX20
#define  ADCData6_intrMask        HEX40
#define  ADCData7_intrMask        HEX80

#define  ADC_Cmp0fail_intrMask    0x100
#define  ADC_Cmp1fail_intrMask    0x200
#define  ADC_Cmp2fail_intrMask    0x400
#define  ADC_AllData_intrMask     0x7FF
/******采样方式宏*******/
#define  SINGLE_ENDED             0
#define  DIFFERENTIAL             1
#define  SW_SAMPLE                0
#define  HW_SAMPLE                1
#define  CONT_SAMPLE              2 
#define  AUTO_SAMPLE              3

/******ADCFormat*******/
#define OriginCode 0 //原码
#define CompCode   1 //补码

//*****************************************************************************
// Function prototype
//*****************************************************************************

void ADC_TriggerMode_Config(uint8_t mode);
void ADC_SampleMode_Config(uint8_t mode);
void ADC_Channelx_Config(uint8_t channelx, uint8_t sw);
void ADC_IRQ_Config(uint16_t irq_type, uint8_t sw, uint8_t comparator_on);
void ADC_Clear_IRQ(uint8_t irq_type);
uint8_t CheckADCDataValidity(uint8_t channelx);
void ADC_Timercount_Set(uint32_t timercount);
void ADC_SW_Sample_Init_Single(uint8_t channelx, uint8_t ADC_Databuffer_channelx, uint8_t mode);
void ADC_SW_Sample_Init_Multi(uint8_t mode, uint8_t Data_Select, uint8_t Chanal_8_mode);
void ADC_HW_Sample_Init_Single(uint8_t channelx, u_int8_t ADC_Databuffer_channelx, uint8_t mode, uint32_t timercount);
void ADC_HW_Sample_Init_Multi(uint8_t mode, uint32_t timercount, uint8_t Data_Select, uint8_t Chanal_8_mode);
void ADC_Auto_Sample_Init_Single(uint8_t channelx, uint8_t ADC_Databuffer_channelx, char cmp_channelx, uint8_t cmp_rule, uint16_t threshold, uint32_t timercount, uint8_t mode);
void ADC_Cont_Sample_Init_Single(uint8_t channelx, u_int8_t ADC_Databuffer_channelx, uint8_t mode);
void ADC_Cont_Sample_Init_Multi(uint8_t mode, uint8_t Data_Select, uint8_t Chanal_8_mode);
short ADC_ReadData(uint8_t ADC_Databuffer_channelx);
void ADC_Cnt(uint32_t timercount);
#endif
