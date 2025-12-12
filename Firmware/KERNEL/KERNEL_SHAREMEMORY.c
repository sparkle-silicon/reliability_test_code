/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2024-04-23 16:32:18
 * @Description:
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
#include "KERNEL_SHAREMEMORY.H"
#include "KERNEL_TIMER.H"
#include "KERNEL_MEMORY.H"
void SMS_Init(VBYTE windows, VBYTE baseaddr, VBYTE aas)
{
#if !(SRAM_CLOCK_EN)
  dprint("SRAM_CLOCK_EN is not ready!");
  return;
#endif
  if (windows == 0)
  {
    SMS_WUCTL |= SMS_WUCTL_SMSW0E;//enable window
    SMS_W0BA = baseaddr; //config sram window base addr
    SMS_W0AAS = aas;//config sram window size & protection 
  }
  else if (windows == 1)
  {
    SMS_WUCTL |= SMS_WUCTL_SMSW1E;//enable window
    SMS_W1BA = baseaddr; //config sram window base addr
    SMS_W1AAS = aas;//config sram window size & protection 
  }
  else if (windows == 2)
  {
    SMS_WUCTL |= SMS_WUCTL_SMSW2E;//enable window
    SMS_W2BA = baseaddr; //config sram window base addr
    SMS_W2AAS = aas;//config sram window size & protection 
  }
  else if (windows == 3)
  {
    SMS_WUCTL |= SMS_WUCTL_SMSW3E;//enable window
    SMS_W3BA = baseaddr; //config sram window base addr
    SMS_W3AAS = aas;//config sram window size & protection 
  }
  SMS_ECSIE |= 0x0f;//config EC Semaphore interrupt enable
  SMS_ECSA &= 0x0;//config EC Semaphore address
}
void SMF_Init(void)
{
  SMF_SR |= SMF_SR_IE_HOST | SMF_SR_IE_EC;
}
void SHAREMEM_PNP(void)
{
  //Enable SMFI
  SYSCTL_HDEVEN |= HOST_SMFI_EN; // host SMFI logic device pnp enable
  SYSCTL_SPCTL0 |= PNPCNST_SETEN; //enable pnp const register//PNPKEY, PNPPORT, VENDORID, CHIPVER 的设置使
  Config_PNP_Access_Request();
  //Config SMFI
  Config_PNP_Write(0x23, 0x01, 0x01);
  Config_PNP_Write(0x07, 0x0f, 0x0f);
  //config firmware cycle 
  Config_PNP_Write(0xf0, 0x0f, 0xfe);
  Config_PNP_Write(0xf1, 0x0f, 0x00);
  Config_PNP_Write(0xf2, 0x0f, 0x00);
  Config_PNP_Write(0xf4, 0x0f, 0xf0);           //config FWID 0xf
  Config_PNP_Write(0xf5, 0x0f, 0x00);
  Config_PNP_Write(0xf6, 0x0f, 0x00);
  //Config SRAM offset map
  Config_PNP_Write(0x60, 0x0f, 0x00);
  Config_PNP_Write(0x61, 0x0f, 0x00);
  //HOST Semaphore Config
  dprint("HOST Semaphore Config over\n");
  //Config_PNP_Write(0xf9, 0x0f, 0xff); //config HOST Semaphore interrupt enable
  Config_PNP_Write(0xfa, 0x0f, 0x0);  //config HOST Semaphore address
  Config_PNP_Write(0x30, 0x0f, 0x01);
  Config_PNP_Access_Release();
}
void SRAM_WRITE(VDWORD offset, VBYTE data)
{
  REG8(SRAM_BASE_ADDR + offset) = data;

}
BYTE SRAM_READ(VDWORD offset)
{
  return REG8(SRAM_BASE_ADDR + offset);
}
void SHAREMEMORY_INIT(void)
{
#if !(SHM_CLOCK_EN)
  dprint("SHM_CLOCK_EN is not ready!");
  return;
#endif
  sram_ModuleClock_EN;
  SMS_Init(0, 0x00, SMS_WxAAS_1024SIZE | SMS_WxAAS_NWPE | SMS_WxAAS_NRPE);
  SMS_Init(1, 0x40, SMS_WxAAS_1024SIZE | SMS_WxAAS_NWPE | SMS_WxAAS_NRPE);
  SMS_Init(2, 0x80, SMS_WxAAS_1024SIZE | SMS_WxAAS_NWPE | SMS_WxAAS_NRPE);
  SMS_Init(3, 0xC0, SMS_WxAAS_1024SIZE | SMS_WxAAS_NWPE | SMS_WxAAS_NRPE);
  SMF_Init();
}
void SMSEC_Int_Enable(BYTE type)
{
  SMF_SR |= (0x1 << type);
}
void SMSEC_Int_Disable(BYTE type)
{
  SMF_SR &= ~(0x1 << type);
}
void SMSHOST_Int_Enable(BYTE type)
{
  SMS_HSSTA |= (0x1 << type);
}
void SMSHOST_Int_Disable(BYTE type)
{
  SMS_HSSTA &= ~(0x1 << type);
}