/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:30:51
 * @Description: This file is used for port80 configuration
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
#include "KERNEL_PORT80.H"
#include "KERNEL_TIMER.H"
//-----------------------------------------------------------------------------
// Port 80 with BRAM Output, PNP config
//-----------------------------------------------------------------------------
void Port80_Bram_PNP_Config(void)
{
#if P80_TO_BRAM_SWITCH
	PNP_LD_EN |= 0x00240000; //enable BRAM and Port80 logic device
	PNP_CTRL0 |= (0x02000000);
	PNP_CTRL0 &= (~0x01000000);
	//Set Bram & Port80 Config
	Config_PNP_Write(0x23, 0x01, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x07, 0x10, 0x10);
	vDelayXms(5);
	Config_PNP_Write(0x60, 0x10, 0x00);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x10, 0x70);
	vDelayXms(5);
	Config_PNP_Write(0xf0, 0x10, 0x00);     //Disable Host access Bram 0x10-0x1F 0x20-0x2F
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x10, 0x01);
	vDelayXms(5);
#else
//pnp config
	PNP_LD_EN |= 0x00200000; //enable Port80 logic device
	//Port80 latch to GPIO
	PNP_CTRL0 |= 0x01000000;
	//select GPE0 & GPE7 to hlat & llat
	sysctl_iomux_config(GPIOB, 0, 1); //select hlat
	sysctl_iomux_config(GPIOB, 7, 1); //select llat
#endif
}
