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
 * Copyright ©2021-2025 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2025龙晶石半导体科技（苏州）有限公司
 */
#include "KERNEL_PORT80.H"
#include "KERNEL_TIMER.H"
#include "KERNEL_MEMORY.H"
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
	// //select GPE0 & GPE7 to hlat & llat
	// sysctl_iomux_l80();
#endif
}

/**
 * @brief 配置捕获某个端口或者某个端口范围的数据存到SRAM+offset处，其中主机一帧数据存
 * 存到sram为四字节信息，总共fifo长度为16*4=64字节，其中BYTE0为数据，BYTE1/2为端口，BYTE3为空，例如：BYTE0=0x12,
 * BYTE1=0x80,BYTE2=0x00,BYTE3=0x00,则表示捕获到端口:0x0080,数据:0x12。
 *
 * @param addr0 端口地址0捕获使能
 * @param addr1 端口地址1捕获使能
 * @param offset 放到SRAM的偏移，例如：offset=0x100，则存到SRAM的地址为0x31000+0x100=0x31100
 * @param rgn_en 端口范围使能  addr0<IO<addr1的端口捕获使能
*/
void Port80_Config(uint16_t addr0, uint8_t addr1, uint32_t offset, uint8_t rgn_en)
{
	PNP_CTRL0 |= P80L_EN;
	PNP_LD_EN |= HOST_P80_EN; //enable Port80 logic device
	P80_Idx = 0; Current_P80_Idx = 0; Last_P80_Idx = 0; Total_P80_Idx = 0;
	offset = offset >> 6;//base:sram+0ffset<<6
	SYSCTL_P80_ADDR_CFG &= ~0xffffff;
	SYSCTL_P80_ADDR_CFG |= (addr0) | (addr1 << 16) | BIT(24) | BIT(25);//bit24,25为同时使能addr0/1
	SYSCTL_P80_ADDR_CFG &= ~BIT(26);
	SYSCTL_P80_ADDR_CFG |= rgn_en << 26;//bit26为端口范围使能
	SYSCTL_ESPI_P80_CFG &= ~0x1f;
	SYSCTL_ESPI_P80_CFG |= (offset & 0x1f);//配置存到sram的偏移sram+0ffset<<6
	SYSCTL_ESPI_P80_CFG &= ~BIT(6);//interrupt enable
}

char Get_Port80_Bufferdata(uint32_t *p_data)
{
	uint8_t pbuf_idx = 0;
	if ((P80_Idx == 0) && (Total_P80_Idx == 0))
		return 1;
	if (Total_P80_Idx >= 16)
	{
		pbuf_idx = P80_Idx * 4;
		for (int i = 0; i < 16; i++)
		{
			if (pbuf_idx == 0)
				pbuf_idx = 64;
			pbuf_idx -= 4;
			p_data[i] = (REG8(SRAM_BASE_ADDR + ((SYSCTL_ESPI_P80_CFG & 0x1f) << 6) + pbuf_idx)) |
				(REG8(SRAM_BASE_ADDR + ((SYSCTL_ESPI_P80_CFG & 0x1f) << 6) + pbuf_idx + 1) << 8) |
				(REG8(SRAM_BASE_ADDR + ((SYSCTL_ESPI_P80_CFG & 0x1f) << 6) + pbuf_idx + 2) << 16);
		}
	}
	else
	{
		pbuf_idx = P80_Idx * 4;
		for (int i = 0; i < Total_P80_Idx; i++)
		{
			if (pbuf_idx == 0)
				pbuf_idx = 64;
			pbuf_idx -= 4;
			p_data[i] = (REG8(SRAM_BASE_ADDR + ((SYSCTL_ESPI_P80_CFG & 0x1f) << 6) + pbuf_idx)) |
				(REG8(SRAM_BASE_ADDR + ((SYSCTL_ESPI_P80_CFG & 0x1f) << 6) + pbuf_idx + 1) << 8) |
				(REG8(SRAM_BASE_ADDR + ((SYSCTL_ESPI_P80_CFG & 0x1f) << 6) + pbuf_idx + 2) << 16);
		}
	}
	return 0;
}