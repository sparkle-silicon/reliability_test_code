/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:26:30
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
#ifndef __KERNEL_BRAM_H_
#define __KERNEL_BRAM_H_
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
#include "KERNEL_PNP.H"
#define sms_write(addr,value)	((*((volatile uint8_t *)(addr))) = (value))
#define sms_read(addr)		(*((volatile uint8_t *)(addr)))
#define HBRAM_EN 0x00040000
extern void BRAM_Config(void);
extern void BRAM_EC_Read(void);
extern void BRAM_EC_Write(void);
#endif