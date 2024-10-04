/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:29:28
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
#include "KERNEL_BRAM.H"
#include "KERNEL_TIMER.H"
/**
 * @brief 此函数用于配置BRAM逻辑设备，并初始化其相关参数。
 *
 * @param 无
 *
 * @return 无
 */
void BRAM_Config(void)
{
#if !(BRAM_CLOCK_EN)
  return;
#endif
// Enable BRAM logic device
#if (SYSCTL_CLOCK_EN)
  SYSCTL_HDEVEN |= HBRAM_EN;      //host侧BRAM使能
#endif
  // Enable BRAM
  Config_PNP_Write(0x23, 0x01, 0x01);
  Config_PNP_Write(0x07, 0x10, 0x10);
  Config_PNP_Write(0x60, 0x10, 0x00);
  Config_PNP_Write(0x61, 0x10, 0x60);
  Config_PNP_Write(0x30, 0x10, 0x01);
}
/**
 * @brief 在EC端进行BRAM地址写操作
 *
 * @param 无
 *
 * @return 无
 *
 * @note 此函数用于在EC端进行BRAM地址写操作。
 */
void BRAM_EC_Write(void)
{
  int i;
  for(i = 0; i < 48; i++)
  {
    *((volatile uint8_t *)(BRAM_BASE_ADDR + i)) = 0xec;
    vDelayXms(1);
  }
  dprint("bram data is writen!\n");
}
/**
 * @brief 在EC端进行BRAM地址读操作
 *
 * @param 无
 *
 * @return 无
 *
 * @note 此函数用于在EC端进行BRAM地址读操作。
 */
void BRAM_EC_Read(void)
{
  uint8_t i;
  for(i = 0; i <= 0x2F; i++)
  {
    BRAM_ReadBuff[i] = *((volatile uint8_t *)(BRAM_BASE_ADDR + i));
    dprint("bram data is %#x\n", BRAM_ReadBuff[i]);
  }
  dprint("bram data read end!\n");
}
