/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2023-11-03 16:40:12
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
#ifndef KERNEL_SHAREMEMORY_H
#define KERNEL_SHAREMEMORY_H
#include "AE_GLOBAL.H"
#include "KERNEL_PNP.H"
//-----------------------------------------------------------------------------
// Share Memory Define
//-----------------------------------------------------------------------------
#define SMF_SR_IR_HOST      HEX01
#define SMF_SR_IR_EC        HEX02
#define SMF_SR_IE_HOST      HEX04
#define SMF_SR_IE_EC        HEX08
#define SMS_CTL_HSIE        HEX80
#define SMS_CTL_HSEMW       HEX40
#define SMS_WUCTL_SMSW0E    HEX01
#define SMS_WUCTL_SMSW1E    HEX02
#define SMS_WUCTL_SMSW2E    HEX04
#define SMS_WUCTL_SMSW3E    HEX08
#define SMS_WxAAS_WPE       0xC0
#define SMS_WxAAS_LWPE      0x80
#define SMS_WxAAS_HWPE      0x40
#define SMS_WxAAS_NWPE      0x00
#define SMS_WxAAS_RPE       0x30
#define SMS_WxAAS_LRPE      0x20
#define SMS_WxAAS_HRPE      0x10
#define SMS_WxAAS_NRPE      0x00
#define SMS_WxAAS_2048SIZE  0x07
#define SMS_WxAAS_1024SIZE  0x06
#define SMS_WxAAS_512SIZE   0x05
#define SMS_WxAAS_256SIZE   0x04
#define SMS_WxAAS_128SIZE   0x03
#define SMS_WxAAS_64SIZE    0x02
#define SMS_WxAAS_32SIZE    0x01
#define SMS_WxAAS_16SIZE    0x00
#define SMFSR_EC_IE         HEX08
#define SMFSR_EC_IR         HEX02
/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
extern void SMS_Init(VBYTE windows, VBYTE baseaddr, VBYTE aas);
extern void SMF_Init(void);
extern void SHAREMEM_PNP(void);
extern void SRAM_WRITE(VDWORD offset, VBYTE data);
extern BYTE SRAM_READ(VDWORD offset);
extern void SHAREMEMORY_INIT(void);
extern void SMSEC_Int_Enable(BYTE type);
extern void SMSEC_Int_Disable(BYTE type);
extern void SMSHOST_Int_Enable(BYTE type);
extern void SMSHOST_Int_Disable(BYTE type);
#endif