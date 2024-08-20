/*
 * @Author: Yangxiaolong
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-02-07 12:35:26
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
//  CEC Write in two byte
// !!!the cec only supported 16-bit write
//  parameter :
//              value: the value written to CEC_register
//              offset: CEC_register_offset
//  return :
//      none
//
//*****************************************************************************
void CEC_Write(WORD value, BYTE offset)
{
    CEC_REG(offset) = value;
    return;
}
//*****************************************************************************
//
//  CEC read in two byte
// !!!the cec only supported 16-bit read
//  parameter :
//              offset: CEC_register_offset
//  return :
//              none
//
//*****************************************************************************
WORD CEC_Read(BYTE offset)
{
    return CEC_REG(offset);
}
//*****************************************************************************
//
//  CEC initiator Initialization function
//  parameter :
//              none
//  return :
//              none
//******************************************************************************
void CEC_initiator_init(void)
{
   // enable the interrup
    CEC_IER = CEC_Int_Send_Frame | CEC_Int_Send_Block | CEC_Int_erro;
    CEC_CPSR = (HIGHT_CHIP_CLOCK / 10000) - 1;
    CEC_CTRL = CEC_Ctr_Clean_tFIFO | CEC_Ctr_sdb | CEC_Ctr_en;// enable the broadcast frame send
}
//*****************************************************************************
//
//  CEC follower Initialization function
//  parameter :
//              none
//  return :
//              none
//******************************************************************************
void CEC_follower_init(void)
{
    // enable the interrup
    CEC_IER = CEC_Int_Receive_Head | CEC_Int_Receive_Block | CEC_Int_Receive_Frame | CEC_Int_erro;
    CEC_CPSR = (HIGHT_CHIP_CLOCK / 10000) - 1;
    CEC_ADDR = CEC_Initiator_address;             //"Initiator_address" as device_addr while follower
    CEC_CTRL = CEC_Ctr_Clean_rFIFO | CEC_Ctr_eba | CEC_Ctr_rad | CEC_Ctr_en; // receive data no matter address
}
//*****************************************************************************
//
//  CEC  frame sending that include one "head block" and lots of "data blocks"
//  parameter :
//              none
//  return :
//              none
//******************************************************************************
void CEC_Frame_send(char *data, BYTE len, BYTE broadcast)
{

    if(len > 15)
    {
        dprint("the frame too long !! \n");
        return;
    }
    if(broadcast)//head
    {
        CEC_CTRL |= CEC_Ctr_sdb;
        CEC_DA = (CEC_Initiator_address << 4) | CEC_Destination_address;
    }
    else
    {
        CEC_CTRL &= ~CEC_Ctr_sdb;
        CEC_DA = (CEC_Initiator_address << 4) | CEC_Destination_address;
    }

    for(int i = 0; i < len - 1; i++)
    {
        CEC_DA = data[i];
    }
    CEC_DA = data[len - 1] | CEC_send_EOM;
    CEC_CTRL |= CEC_Ctr_send; // Send fifo
    return;
}
