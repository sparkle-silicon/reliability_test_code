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
#define P80_TO_BRAM_SWITCH 1
//-----------------------------------------------------------------------------
// Port 80 with BRAM Output, PNP config
//-----------------------------------------------------------------------------
void Port80_Bram_PNP_Config(void)
{
#if P80_TO_BRAM_SWITCH
	PNP_LD_EN |= HOST_BRAM_EN | HOST_P80_EN; //enable BRAM and Port80 logic device
	PNP_CTRL0 |= P80L_EN;
	PNP_CTRL0 &= (~P80LR_EN);
	//Set Bram & Port80 Config
	Config_PNP_Write(0x23, 0x01, 0x01);
	Config_PNP_Write(0x07, 0x10, 0x10);
	Config_PNP_Write(0x60, 0x10, 0x00);
	Config_PNP_Write(0x61, 0x10, 0x70);
	Config_PNP_Write(0xf0, 0x10, 0x00);     //Disable Host access Bram 0x10-0x1F 0x20-0x2F
	Config_PNP_Write(0x30, 0x10, 0x01);
#else
//pnp config
	PNP_LD_EN |= HOST_P80_EN; //enable Port80 logic device
	//Port80 latch to GPIO
	PNP_CTRL0 |= P80LR_EN;
	//select GPE0 & GPE7 to hlat & llat
	sysctl_iomux_config(GPIOB, 0, 1); //select hlat
	sysctl_iomux_config(GPIOB, 7, 1); //select llat
#endif
}

void Port80_Config(uint16_t addr0,uint8_t addr1,uint32_t offset,uint8_t rgn_en)
{
	PNP_LD_EN |= HOST_P80_EN; //enable Port80 logic device
	P80_Idx=0;Current_P80_Idx=0;Last_P80_Idx=0;Total_P80_Idx=0;
	offset = offset>>6;//base:sram+0ffset<<6
	REG32(0x304e8) &= ~0xffffff;
	REG32(0x304e8) |= (addr0) | (addr1 << 16) | BIT(24) | BIT(25);
	REG32(0x304e8) &= ~BIT(26);
	REG32(0x304e8) |=rgn_en << 26;
	SYSCTL_ESPI_P80_CFG &= ~0xf;
	SYSCTL_ESPI_P80_CFG|=(offset&0xf);
	SYSCTL_ESPI_P80_CFG &=~BIT(6);//interrupt enable
}

char Get_Port80_Bufferdata(uint32_t *p_data)
{
	uint8_t pbuf_idx=0;
	if((P80_Idx==0) && (Total_P80_Idx==0))
		return 1;
	if(Total_P80_Idx>=16)
	{
		pbuf_idx=P80_Idx*4;
		for(int i=0;i<16;i++)
		{
			if(pbuf_idx==0)
				pbuf_idx=64;
			pbuf_idx-=4;
			p_data[i]=(REG8(SRAM_BASE_ADDR+((SYSCTL_ESPI_P80_CFG&0x0f)<<6)+pbuf_idx))|
				(REG8(SRAM_BASE_ADDR+((SYSCTL_ESPI_P80_CFG&0x0f)<<6)+pbuf_idx+1)<<8)|
				(REG8(SRAM_BASE_ADDR+((SYSCTL_ESPI_P80_CFG&0x0f)<<6)+pbuf_idx+2)<<16);
		}
	}
	else
	{
		pbuf_idx=P80_Idx*4;
		for(int i=0;i<Total_P80_Idx;i++)
		{
			if(pbuf_idx==0)
				pbuf_idx=64;
			pbuf_idx-=4;
			p_data[i]=(REG8(SRAM_BASE_ADDR+((SYSCTL_ESPI_P80_CFG&0x0f)<<6)+pbuf_idx))|
				(REG8(SRAM_BASE_ADDR+((SYSCTL_ESPI_P80_CFG&0x0f)<<6)+pbuf_idx+1)<<8)|
				(REG8(SRAM_BASE_ADDR+((SYSCTL_ESPI_P80_CFG&0x0f)<<6)+pbuf_idx+2)<<16);
		}
	}
	return 0;
}