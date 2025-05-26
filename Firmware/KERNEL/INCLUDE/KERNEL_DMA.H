#ifndef __KERNEL_DMA_H
#define __KERNEL_DMA_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
#include "KERNEL_PNP.H"

extern void DMA_INIT(BYTE TT_FC, BYTE BLK_TS, BYTE SRC_PER, BYTE DEST_PER, DWORD SAR0, DWORD DAR0, BYTE DmaCfgReg);

//Interrupt Enable Bit
#define DMA_INT_DIS HEX00
#define DMA_INT_EN HEX01

//Destination Transfer Width
#define DMA_DST_TR_WIDTH_8bits HEX00  
#define DMA_DST_TR_WIDTH_16bits HEX01
#define DMA_DST_TR_WIDTH_32bits HEX02  
#define DMA_DST_TR_WIDTH_64bits HEX03
#define DMA_DST_TR_WIDTH_128bits HEX04  
#define DMA_DST_TR_WIDTH_256bits HEX05              //0x5,0x6,0x7 : 256bits
//Source Transfer Width
#define DMA_SRC_TR_WIDTH_8bits HEX00  
#define DMA_SRC_TR_WIDTH_16bits HEX01
#define DMA_SRC_TR_WIDTH_32bits HEX02  
#define DMA_SRC_TR_WIDTH_64bits HEX03
#define DMA_SRC_TR_WIDTH_128bits HEX04  
#define DMA_SRC_TR_WIDTH_256bits HEX05             //0x5,0x6,0x7 : 256bits
//Destination Address Increment
#define DMA_DINC_INC HEX00                        //Increments the destination address
#define DMA_DINC_DEC HEX01                        //Decrements the destination address
#define DMA_DINC_No_chagne HEX02                  //0x2,0x3 : No change in the destination address
//Source Address Increment
#define DMA_SINC_INC HEX00                        //Increments the source address
#define DMA_SINC_DEC HEX01                        //Decrements the source address
#define DMA_SINC_No_chagne HEX02                        //0x2,0x3 : No change in the source address
//Destination Burst Transaction Length
#define DMA_DEST_MSIZE_1 HEX00                    //Number of data items to be transferred is 1
#define DMA_DEST_MSIZE_4 HEX01
#define DMA_DEST_MSIZE_8 HEX02
#define DMA_DEST_MSIZE_16 HEX03
#define DMA_DEST_MSIZE_32 HEX04
#define DMA_DEST_MSIZE_64 HEX05
#define DMA_DEST_MSIZE_128 HEX06
#define DMA_DEST_MSIZE_256 HEX07
//Source Burst Transaction Length
#define DMA_SRC_MSIZE_1 HEX00                    //Number of data items to be transferred is 1
#define DMA_SRC_MSIZE_4 HEX01
#define DMA_SRC_MSIZE_8 HEX02
#define DMA_SRC_MSIZE_16 HEX03
#define DMA_SRC_MSIZE_32 HEX04
#define DMA_SRC_MSIZE_64 HEX05
#define DMA_SRC_MSIZE_128 HEX06
#define DMA_SRC_MSIZE_256 HEX07
//Source gather enable
#define DMA_SRC_GATHER_DIS HEX00                    //Source gather is disabled
#define DMA_SRC_GATHER_EN HEX01                     //Source gather is enabled
//Destination scatter enable
#define DMA_DST_SCATTER_DISEN HEX00                 //Destination Scatter is disabled
#define DMA_DST_SCATTER_EN HEX01                    //Destination Scatter is enabled
//Transfer Type and Flow Control
#define DMA_TT_FC_MtoM_DMA HEX00                    //Transfer type is Memory to Memory and Flow Controller is DW_ahb_dmac
#define DMA_TT_FC_MtoP_DMA HEX01                    //Transfer type is Memory to Peripheral and Flow Controller is DW_ahb_dmac
#define DMA_TT_FC_PtoM_DMA HEX02                    //Transfer type is Peripheral to Memory and Flow Controller is DW_ahb_dmac
#define DMA_TT_FC_PtoP_DMA HEX03                    //Transfer type is Peripheral to Peripheral and Flow Controller is DW_ahb_dmac
#define DMA_TT_FC_PtoM_P HEX04                      //Transfer type is Peripheral to Memory and Flow Controller is Peripheral
#define DMA_TT_FC_PtoP_SP HEX05                     //Transfer type is Peripheral to Peripheral and Flow Controller is Source Peripheral
#define DMA_TT_FC_MtoP_P HEX06                      //Transfer type is Memory to Peripheral and Flow Controller is Peripheral
#define DMA_TT_FC_PtoP_DP HEX07                     //Transfer type is Peripheral to Peripheral and Flow Controller is Destination Peripheral

#define DMA_LLP_DST_DIS HEX00                       //Block chaining using LinkedList is disabled on the Destination side
#define DMA_LLP_DST_EN HEX01                        //Block chaining using LinkedList is enabled on the Destination side

#define DMA_LLP_SRC_DIS HEX00                       //Block chaining using LinkedList is disabled on the Source side
#define DMA_LLP_SRC_EN HEX01                        //Block chaining using LinkedList is enabled on the Source side

//Done bit
#define DMA_DONE_DIS HEX00                          //DONE bit is deasserted the enb of block transfer
#define DMA_DONE_EN HEX01                           //SET the DONE bit at the end of block transfer

//Channel Priority
#define DMA_CH_PRIOR_0 HEX00                        //Channel priority is 0
#define DMA_CH_PRIOR_1 HEX01
#define DMA_CH_PRIOR_2 HEX02
#define DMA_CH_PRIOR_3 HEX03
#define DMA_CH_PRIOR_4 HEX04
#define DMA_CH_PRIOR_5 HEX05
#define DMA_CH_PRIOR_6 HEX06
#define DMA_CH_PRIOR_7 HEX07

//Channel Suspend
#define DMA_NOT_SUSPENDED HEX00                     //DMA transfer from the source is not suspended
#define DMA_SUSPENDED HEX01                         //Suspend DMA transfer from the source
//Destination Software or Hardware Handshaking Select
#define DMA_DST_HARDWARE_HS HEX00
#define DMA_DST_SOFTWARE_HS HEX01
//Source Software or Hardware Handshaking Select
#define DMA_SRC_HARDWARE_HS HEX00
#define DMA_SRC_SOFTWARE_HS HEX01

//DW_ahb_dmac Enable bit
#define DMA_DIS HEX00
#define DMA_EN HEX01

//Channel Enable
#define DMA_CH_DIS HEX00
#define DMA_CH_EN HEX01

#define DMA_CH_DIS_WE HEX00
#define DMA_CH_EN_WE HEX01

#endif

