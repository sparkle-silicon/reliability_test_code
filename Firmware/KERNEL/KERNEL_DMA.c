/*
 * @Author: Iversu
 * @LastEditors: coolwangzi@126.com
 * @LastEditTime: 2025-09-15 15:04:46
 * @Description: This is DMA configuration interface file
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

#include "AE_INCLUDE.H"
#include "KERNEL_DMA.H"

 /**
  * @brief dma初始化
  * @param  none
  *
  * @return none
  */
void DMA_Init(DMA_InitTypeDef* DMA_Init_Struct)
{
    uint32_t  rdata = 0;
    uint32_t  wdata = 0;
    uint8_t src_per = 0;
    uint8_t dest_per = 0;
    //SYSCTL_DMA_SEL
    if (DMA_Init_Struct->DMA_Trans_Type != 0x0)
    {
        SYSCTL_DMA_SEL = (DMA_Init_Struct->DMA_Periph_Type) & 0xFFFF;
    }
    //CTL0_L
    wdata = (DMA_Init_Struct->DMA_Dest_Width << 1) | (DMA_Init_Struct->DMA_Src_Width << 4) |
        (DMA_Init_Struct->DMA_Dest_Inc << 7) | (DMA_Init_Struct->DMA_Src_Inc << 9) |
        (DMA_Init_Struct->DMA_Dest_Msize << 11) | (DMA_Init_Struct->DMA_Src_Msize << 14) |
        (DMA_Init_Struct->DMA_Trans_Type << 20);
    DMA_CTL0_L = wdata;
    //CTL0_H
    rdata = DMA_CTL0_H;
    rdata &= ~(0xfff);
    DMA_CTL0_H = (rdata | (DMA_DONE_EN << 12) | DMA_Init_Struct->DMA_Block_Ts);
    //CFG_L
    wdata = DMA_Init_Struct->DMA_Priority << 5;
    rdata = DMA_CFG0_L;
    rdata &= ~((0x1 << 11) | (0x1 << 10));      //源和目标设备都设置为硬件握手
    DMA_CFG0_L = (rdata | wdata);
    //CFG_H
    rdata = DMA_CFG0_H;
    rdata &= ~(0xff << 7);

    if (DMA_Init_Struct->DMA_Periph_Type & (0xff << 24))//tx
    {
        dest_per = (DMA_Init_Struct->DMA_Periph_Type >> 24) & 0xf;
    }
    else
    {
        src_per = (DMA_Init_Struct->DMA_Periph_Type >> 16) & 0xf;
    }
    rdata |= (dest_per << 11) | (src_per << 7);
    DMA_CFG0_H = rdata;
    //SAR0
    DMA_SAR0 = DMA_Init_Struct->DMA_Src_Addr;
    //DAR0
    DMA_DAR0 = DMA_Init_Struct->DMA_Dest_Addr;
    //ENABLE DMA and DMA_CHANNEL
    DMA_DmaCfgReg = 0x1;
    if (DMA_Init_Struct->DMA_Periph_Type != SPIM_DMA_RX)
    {
        wdata = (0x1 | (0x1 << 8));
        DMA_ChEnReg = wdata;
    }
}
/**
 * @brief dma传输完成检测(需要确认)
 * @param  none
 *
 * @return done return 1,not done return 0
 */
char DMA_Transfer_Done(void)
{
    uint8_t ret = 0;
    ret = DMA_CTL0_H & 0x1f;
    if (ret != 0)
        return 0;
    else
        return 1;

}
