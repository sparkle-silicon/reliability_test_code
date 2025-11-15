/*
 * @Author: Iversu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2025-10-20 20:33:27
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
void BRAM_SIOConfig(void)
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
    *((volatile uint8_t *)(BRAM_BASE_ADDR + i)) = 0xff;
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
  for(i = 0; i <= 96; i++)
  {
    BRAM_ReadBuff[i] = *((volatile uint8_t *)(BRAM_BASE_ADDR + i));
    dprint("bd[%d]:%#x\n", i, BRAM_ReadBuff[i]);
  }
}
/**
 * @brief BRAM配置函数,BRAM映射到例如0x270端口，如果主机对0x270端口填数会被当做一个index
 * 如果index_0<index<index_1,0x271端口写将会触发intr1_36中断，0x271端口读将会触发intr1_37中断，
 * 可设置mask屏蔽intr1_36和intr1_37中断。并会将270 index，271端口的操作写到bram对应index的位置。
 *
 * @param index_0 索引0
 * @param index_1 索引1
 * @param mask 中断屏蔽位
 *
 * @return 无
 *
 */
void BRAM_Config(uint8_t index_0, uint8_t index_1, uint8_t mask)
{
    REG32(0x3051C)&=~0xff;
    REG32(0x3051C)|=index_0;
    REG32(0x3051C)&=(~0xff)<<8;
    REG32(0x3051C)|=(index_1<<8);
    if(mask)
        REG32(0x3051C)|=0x30000;
    else
        REG32(0x3051C)&=~0x30000;
}
