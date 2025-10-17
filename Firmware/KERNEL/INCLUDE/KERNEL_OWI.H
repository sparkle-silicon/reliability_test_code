/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-01-12 17:26:26
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
#ifndef KERNEL_OWI_H
#define  KERNEL_OWI_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
extern void OWI_Init(u8 data_len, u8 out_clk, u8 T0H, u8 T1H, u8 clkdiv);
extern void OWI_GO(u16 reset_us);
extern void OWI_Set_Data(u8 led_num_start, u8 led_num_over, u32 data);
extern void OWI_Clear_Data(u8 led_num_start, u8 led_num_over);
extern void OWI_SendData(u32 data);
#define OWI_GENERAL_CTRL_MSB 0x1000000
#define OWI_GENERAL_CTRL_LSB 0x0

#define OWI_GENERAL_CTRL_INTSR 0x800000
#define OWI_GENERAL_CTRL_CYCLE_NUM_CLK_DO(x) ((x&0xff)<<15)
#define OWI_GENERAL_CTRL_Length(x) ((x&0x7f)<<3)
#define OWI_GENERAL_CTRL_GO 0b100
#define OWI_GENERAL_CTRL_SEL_RZ 0b00

#endif 
