/*
 * @Author: Yangxiaolong
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-02-07 11:43:27
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
#ifndef __KERNEL_CEC_H_
#define __KERNEL_CEC_H_
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#define CEC0_CHANNEL 0
#define CEC1_CHANNEL 1
#define CEC_Destination_address              0x0F//option. and the "F" mean broadcast
#define CEC_Initiator_address                0x0F//as device address while CEC used to follower 
 //the bit of cec interrupt_enable_register
#define CEC_Int_Receive_Head                 0x01
#define CEC_Int_Receive_Block                0x02
#define CEC_Int_Receive_Frame                0x04
#define CEC_Int_Send_Block                   0x08
#define CEC_Int_Send_Frame                   0x10
#define CEC_Int_erro                         0x20
 //the bit of cec sr_register
#define CEC_SR_Busy                          0x01
#define CEC_SR_Rbc                           0x02
#define CEC_SR_Reom                          0x04
#define CEC_SR_Sendackst                     0x08
#define CEC_SR_Sackerr                       0x10
#define CEC_SR_Lineerr                       0x20
#define CEC_SR_Biterr                        0x10
#define CEC_SR_Errder                        0x20
//the bit of cec control_register
#define CEC_Ctr_en                           0x01//CEC enable 
#define CEC_Ctr_send                         0x02//Send FIFOs after this bit written "1"
#define CEC_Ctr_sdb                          0x04//"1": send the broadcast frame; "0": send the datas frame 
#define CEC_Ctr_dsa                          0x08//disable all the ACK
#define CEC_Ctr_eba                          0x10//enable the "broadcast ACK"
#define CEC_Ctr_rad                          0x20//reveive all frame no matter the address is correct or not
#define CEC_Ctr_Clean_rFIFO                  0x40//"1": clear the receive FIFOs 
#define CEC_Ctr_Clean_tFIFO                  0x80//"1": clear the send FIFOs 
//the bit of cec interrup statue
#define CEC_intStatue_rhis                   0x01//receive header int
#define CEC_intStatue_fbis                   0x02//receive block int
#define CEC_intStatue_rfis                   0x04//receive frame int
#define CEC_intStatue_sbis                   0x08//send block int
#define CEC_intStatue_sfis                   0x10//send frame int
#define CEC_intStatue_erris                  0x20//action erro int
//the bit of EOM in block
#define CEC_send_EOM                         0x100
// void CEC_Write(WORD value, BYTE offset);
// WORD CEC_Read(BYTE offset);
extern void CEC_initiator_init(BYTE channel);
extern void CEC_follower_init(BYTE channel);
extern void CEC_Frame_send(BYTE channel, char *data, BYTE len, BYTE broadcast);
#endif