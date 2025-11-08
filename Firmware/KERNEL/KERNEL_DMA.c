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

void Moudle_DMA_Enable(uint32_t Module)
{
    
    switch (Module)
    {
    case I3C0_DMA_RX://固定为MASTER模式
        I3C_WriteREG_DWORD((I3C_ReadREG_DWORD(DEVICE_CTRL_OFFSET,0)|DEVICE_CTRL_DMA_ENABLE), DEVICE_CTRL_OFFSET, 0);
        break;
    case I3C0_DMA_TX://固定为MASTER模式
        I3C_WriteREG_DWORD((I3C_ReadREG_DWORD(DEVICE_CTRL_OFFSET,0)|DEVICE_CTRL_DMA_ENABLE), DEVICE_CTRL_OFFSET, 0);
        break;
    case I3C1_DMA_RX://固定为MASTER模式
        I3C_WriteREG_DWORD((I3C_ReadREG_DWORD(DEVICE_CTRL_OFFSET,0)|DEVICE_CTRL_DMA_ENABLE), DEVICE_CTRL_OFFSET, 1);
        break;
    case I3C1_DMA_TX://固定为MASTER模式
        I3C_WriteREG_DWORD((I3C_ReadREG_DWORD(DEVICE_CTRL_OFFSET,0)|DEVICE_CTRL_DMA_ENABLE), DEVICE_CTRL_OFFSET, 1);
        break;
    case I3C2_DMA_RX://固定为SLAVE模式
        I3C_WriteREG_DWORD(0x2, DMACTRL_OFFSET, 2);   //DMA使能
        break;
    case I3C2_DMA_TX://固定为SLAVE模式
    {
        uint32_t status=I3C_ReadREG_DWORD(STATUS_OFFSET,2);
        uint32_t errwarn=I3C_ReadREG_DWORD(ERRWARN_OFFSET,2);
        if(status&0x500)
        {
            I3C_WriteREG_DWORD((I3C_ReadREG_DWORD(STATUS_OFFSET,2)|(status&0x500)), STATUS_OFFSET, 2);
        }
        if(errwarn&0x14)
        {
            I3C_WriteREG_DWORD((I3C_ReadREG_DWORD(ERRWARN_OFFSET,2)|(errwarn&0x14)), ERRWARN_OFFSET, 2);
        }
        I3C_WriteREG_DWORD(0x4, DMACTRL_OFFSET, 2);   //DMA使能
        break;
    }
    case I3C3_DMA_RX://固定为SLAVE模式
        I3C_WriteREG_DWORD(0x2, DMACTRL_OFFSET, 3);   //DMA使能
        break;
    case I3C3_DMA_TX://固定为SLAVE模式
    {
        uint32_t status=I3C_ReadREG_DWORD(STATUS_OFFSET,3);
        uint32_t errwarn=I3C_ReadREG_DWORD(ERRWARN_OFFSET,3);
        if(status&0x500)
        {
            I3C_WriteREG_DWORD((I3C_ReadREG_DWORD(STATUS_OFFSET,3)|(status&0x500)), STATUS_OFFSET, 3);
        }
        if(errwarn&0x14)
        {
            I3C_WriteREG_DWORD((I3C_ReadREG_DWORD(ERRWARN_OFFSET,3)|(errwarn&0x14)), ERRWARN_OFFSET, 3);
        }
        I3C_WriteREG_DWORD(0x4, DMACTRL_OFFSET, 3);   //DMA使能
        break;
    }
    
    case SUBUS0_DMA_RX:
        I2c_Write_Short(0x1, SMBUS_DMA_CR_OFFSET, 0); 
        break;
    case SUBUS0_DMA_TX:
        I2c_Write_Short(0x2, SMBUS_DMA_CR_OFFSET, 0);
        break;
    case SUBUS1_DMA_RX:
        I2c_Write_Short(0x1, SMBUS_DMA_CR_OFFSET, 1);
        break;
    case SUBUS1_DMA_TX:
        I2c_Write_Short(0x2, SMBUS_DMA_CR_OFFSET, 1);
        break;
    case SUBUS2_DMA_RX:
        I2c_Write_Short(0x1, SMBUS_DMA_CR_OFFSET, 2);
        break;
    case SUBUS2_DMA_TX:
        I2c_Write_Short(0x2, SMBUS_DMA_CR_OFFSET, 2);
        break;
    case SUBUS3_DMA_RX:
        I2c_Write_Short(0x1, SMBUS_DMA_CR_OFFSET, 3);
        break;
    case SUBUS3_DMA_TX:
        I2c_Write_Short(0x2, SMBUS_DMA_CR_OFFSET, 3);
        break;
    case SUBUS4_DMA_RX:
        I2c_Write_Short(0x1, SMBUS_DMA_CR_OFFSET, 4);
        break;
    case SUBUS4_DMA_TX:
        I2c_Write_Short(0x2, SMBUS_DMA_CR_OFFSET, 4);
        break;
    case SUBUS5_DMA_RX:
        I2c_Write_Short(0x1, SMBUS_DMA_CR_OFFSET, 5);
        break;
    case SUBUS5_DMA_TX:
        I2c_Write_Short(0x2, SMBUS_DMA_CR_OFFSET, 5);
        break;
    case SUBUS6_DMA_RX:
        I2c_Write_Short(0x1, SMBUS_DMA_CR_OFFSET, 6);
        break;
    case SUBUS6_DMA_TX:
        I2c_Write_Short(0x2, SMBUS_DMA_CR_OFFSET, 6);
        break;
    default:
        break;
    }
}


void DMA_Init(DMA_InitTypeDef*DMA_Init_Struct)
{
    uint32_t  rdata = 0;
    uint32_t  wdata = 0;
    uint8_t src_per = 0;
    uint8_t dest_per = 0;
    //SYSCTL_DMA_SEL
    if (DMA_Init_Struct->DMA_Trans_Type != 0x0)
    {
        SYSCTL_DMA_SEL=(DMA_Init_Struct->DMA_Periph_Type)&0xFFFF;
    }
    Moudle_DMA_Enable(DMA_Init_Struct->DMA_Periph_Type);
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
        dest_per=(DMA_Init_Struct->DMA_Periph_Type>>24)&0xf;
        //printf("dest_per:%d\n",dest_per);
    }
    else
    {
        src_per=(DMA_Init_Struct->DMA_Periph_Type>>16)&0xf;
        //printf("src_per:%d\n",src_per);
    }
    rdata |= (dest_per << 11) | (src_per << 7);
    DMA_CFG0_H = rdata;
    //SAR0
    DMA_SAR0 = DMA_Init_Struct->DMA_Src_Addr;
    //DAR0
    DMA_DAR0 = DMA_Init_Struct->DMA_Dest_Addr;
    //ENABLE DMA and DMA_CHANNEL
    // DMA_DmaCfgReg = 0x1;
    // if(DMA_Init_Struct->DMA_Periph_Type!=SPIM_DMA_RX)
    // {
    //     wdata = (0x1 | (0x1<<8));
    //     DMA_ChEnReg  = wdata;
    //     printf("DMA_Ch EN\n");
    // }
    // printf("DMA_ChEnReg:0x%x\n",DMA_ChEnReg);
}
void DMA_Channal_EN(uint8_t DMA_Channal)
{
    DMA_DmaCfgReg = 0x1;
    DMA_ChEnReg  = ((0x1<<DMA_Channal)|((0x1<<DMA_Channal)<<8));
}
/**
 * @brief dma传输完成检测(需要确认)
 * @param  none
 *
 * @return done return 1,not done return 0
 */
char DMA_Transfer_Done(uint8_t DMA_Channal)
{
    uint8_t ret=0;
    ret=DMA_ChEnReg&0x7;
    
    if(ret&(0x1<<DMA_Channal))
        return 0;
    else
        return 1;

}
