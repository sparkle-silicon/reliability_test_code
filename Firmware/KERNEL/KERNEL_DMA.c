#include "AE_INCLUDE.H"
#include "KERNEL_DMA.H"




/*
    注意：
    dst_tr_width，src_tr_width的宽度大小只能配置为32位
    blk_ts最大为0x1f,即只能搬运31趟
    tt_fc只能配置为0x0,0x1,0x2这三种情况
*/

//*****************************************************************************
//  DMA init
//
//  parameter :
//      TT_FC:                Transfer Type and Flow Control
//      BLK_TS :              Block Transfer Size
//      SRC_PER:              Source Hardware Interface
//      DEST_PER:             Destination hardware interface
//      SAR0:                 DCurrent Source Address of DMA_CHANNELS_0 transfer
//      DAR0:                 Current Destination address of DMA_CHANNELS_0 transfer
//      DmaCfgReg:            This register is used to enable the DW_ahb_dmac
//  return :
//      none
//*****************************************************************************
void DMA_INIT(BYTE TT_FC, BYTE BLK_TS, BYTE SRC_PER, BYTE DEST_PER, DWORD SAR0, DWORD DAR0, BYTE DmaCfgReg)
{
    uint32_t  rdata;
    //CTL0
    DMA_CTL0_low = (DMA_LLP_SRC_DIS << 28) | (DMA_LLP_DST_DIS << 27) | (TT_FC << 20) | 
    (DMA_SRC_GATHER_DIS << 18) | (DMA_SRC_GATHER_DIS << 17)| (DMA_SRC_MSIZE_1 << 14) | (DMA_DEST_MSIZE_1 << 11) 
    | (DMA_SINC_INC << 9) | (DMA_DINC_No_chagne << 7) | (DMA_SRC_TR_WIDTH_32bits << 4) | (DMA_DST_TR_WIDTH_32bits << 1)
    | DMA_INT_DIS;
    printf("DMA_CTL0_low:%x\n", DMA_CTL0_low);

    rdata = DMA_CTL0_high;
    rdata &= ~(0xf);
    DMA_CTL0_high = rdata | (DMA_DONE_EN << 12) | BLK_TS;
    printf("DMA_CTL0_high:%x\n", DMA_CTL0_high);
    //CFG0
    rdata = DMA_CFG0_low;
    rdata &= ~((0x1 << 11) | (0x1 << 10));      //源和目标设备都设置为硬件握手
    DMA_CFG0_low = rdata | (DMA_SRC_HARDWARE_HS << 11) | (DMA_DST_HARDWARE_HS << 10) | (DMA_NOT_SUSPENDED << 8) | (DMA_CH_PRIOR_0 << 5);
    printf("DMA_CFG0_low:%x\n", DMA_CFG0_low);

    //当外设为I3C0-3或者SMBUS3-6需要配置SYSCTL_DMA_SEL
    if(TT_FC == 0x1)           //Memory to Peripheral
    {
        if((DAR0 >= 0x38014 && DAR0 <= 0x38A40) || ((DAR0 >= 0x4310 && DAR0 <= 0x4710)))   
            SYSCTL_DMA_SEL = (0x1 << (int)DEST_PER);
        printf("Memory to Peripheral\n");
    }
    else if(TT_FC == 0x2)      //Peripheral to Memory
    {
        if((SAR0 >= 0x38014 && SAR0 <= 0x38A40) || ((SAR0 >= 0x4310 && SAR0 <= 0x4710)))
            SYSCTL_DMA_SEL = (0x1 << (int)SRC_PER);
        printf("Peripheral to Memory\n");
    }
    rdata = DMA_CFG0_high;
    DMA_CFG0_high = (rdata | (DEST_PER << 11) | (SRC_PER << 7));
    printf("DMA_CFG0_high:%x\n", DMA_CFG0_high);

    //SAR0
    DMA_SAR0 = SAR0;
    printf("DMA_SAR0:%x\n", DMA_SAR0);
    //DAR0
    DMA_DAR0 = DAR0;
    printf("DMA_DAR0:%x\n", DMA_DAR0);
    //enable DMA and DMA_CHANNEL
    DMA_DmaCfgReg = DmaCfgReg;                       //1:enable DMA,0:disable DMA
    printf("DMA_DmaCfgReg:%x\n", DMA_DmaCfgReg);
    DMA_ChEnReg = (0x1 | (0x1 << 8));                //1:enable channel_0 , 0:disable channel_0
    printf("DMA_ChEnReg:%x\n", DMA_ChEnReg);
}


