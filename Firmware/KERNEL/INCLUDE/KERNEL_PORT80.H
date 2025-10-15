/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:28:03
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
#ifndef KERNEL_PORT80_H
#define KERNEL_PORT80_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_PNP.H"
#define P80LR_EN BIT(24)
#define P80L_EN  BIT(25)
//函数声明
void Port80_Bram_PNP_Config(void);
void Port80_Config(uint16_t addr0, uint8_t addr1, uint32_t offset, uint8_t rgn_en);
char Get_Port80_Bufferdata(uint32_t *p_data);
extern void L80_Init();
#endif