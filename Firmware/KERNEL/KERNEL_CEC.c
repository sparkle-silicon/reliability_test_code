/*
 * @Author: Yangxiaolong
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-17 21:56:55
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
#include "KERNEL_CEC.H"

 //*****************************************************************************
 //
 //  CEC initiator Initialization function
 //  parameter :
 //              none
 //  return :
 //              none
 //******************************************************************************
void CEC_initiator_init(BYTE channel)
{
    // enable the interrupt
    CECn_IER(channel) = CEC_Int_Send_Frame | CEC_Int_Send_Block | CEC_Int_erro;
    CECn_CPSR(channel) = (HIGHT_CHIP_CLOCK / 10000) - 1;
    CECn_CTRL(channel) = CEC_Ctr_Clean_tFIFO | CEC_Ctr_sdb | CEC_Ctr_en; // enable the broadcast frame send
}
//*****************************************************************************
//
//  CEC follower Initialization function
//  parameter :
//              none
//  return :
//              none
//******************************************************************************
void CEC_follower_init(BYTE channel)
{
    // enable the interrup
    CECn_IER(channel) = CEC_Int_Receive_Head | CEC_Int_Receive_Block | CEC_Int_Receive_Frame | CEC_Int_erro;
    CECn_CPSR(channel) = (HIGHT_CHIP_CLOCK / 10000) - 1;
    CECn_ADDR(channel) = CEC_Initiator_address;                                        //"Initiator_address" as device_addr while follower
    CECn_CTRL(channel) = CEC_Ctr_Clean_rFIFO | CEC_Ctr_eba | CEC_Ctr_rad | CEC_Ctr_en; // receive data no matter address
}
//*****************************************************************************
//
//  CEC  frame sending that include one "head block" and lots of "data blocks"
//  parameter :
//              none
//  return :
//              none
//******************************************************************************
void CEC_Frame_send(BYTE channel, char *data, BYTE len, BYTE broadcast)
{

    if(len > 15)
    {
        dprint("the frame length exceeded !!\n");
        return;
    }
    if(broadcast) // head
    {
        CECn_CTRL(channel) |= CEC_Ctr_sdb;
        // CECn_DA(channel) = (CEC_Initiator_address << 4) | CEC_Destination_address;
    }
    else
    {
        CECn_CTRL(channel) &= ~CEC_Ctr_sdb;
        // CECn_DA(channel) = (CEC_Initiator_address << 4) | CEC_Destination_address;
    }

    for(int i = 0; i < len - 1; i++)
    {
        CECn_DA(channel) = data[i];
    }
    CECn_DA(channel) = data[len - 1] | CEC_send_EOM;
    CECn_CTRL(channel) |= CEC_Ctr_send; // Send fifo
}
void CEC_Init(uint8_t channel, uint8_t mode)
{
    if(mode)
    {
        CEC_initiator_init(channel);
    }
    else
    {
        CEC_follower_init(channel);
    }
}