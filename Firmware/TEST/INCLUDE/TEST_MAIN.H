/*
 * @Author: Maple
 * @LastEditors: Maple
 * @LastEditTime: 2023-04-02 21:37:04
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
#ifndef _TEST_MAIN_H_
#define _TEST_MAIN_H_
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "TEST_CONFIG.H"
#include "KERNEL_MEMORY.H"
#include "KERNEL_MAIN.H"
extern char * read_instructions_msg;
extern const char * printf_instructions_msg;
extern void Device_init(void);
extern const FUNCT_PTR_V_V service_table[];
#endif