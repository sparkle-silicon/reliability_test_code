/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-12 15:44:45
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
#ifndef __KERNEL_SPIF_H_
#define __KERNEL_SPIF_H_
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "AE_MALLOC.H"
#include "AE_LIBC.H"
#include "KERNEL_MEMORY.H"
SPIF_MODE READ_FLASH_QUAD_ENABLE(const uint32_t looplimit);
SPIF_MODE SELECT_DUAL_QUAD(const SPIF_MODE mode, const uint32_t looplimit);
void SELECT_FLASH(SPIF_INT_EXT selectflsh, SPIF_MODE mode, SPIF_PIN WP, SPIF_PIN HOLD, const uint32_t looplimit);
void *SPIF_Read_FLASH(void *flashaddr, void *memaddr, size_t size, const uint32_t looplimit);
void *SPIF_Wirte_FLASH(void *flashaddr, void *memaddr, size_t size, const uint32_t looplimit);
uint32_t SPIF_Erase_FLASH(uint8_t command, void *flash_addr, const uint32_t looplimit);
uint32_t SPIF_Read_ID(const uint32_t looplimit);
uint32_t READ_FLASH_INFO(const uint32_t looplimit);
void Flash_Update_Function(const uint32_t looplimit);
void ALIGNED(4) Mailbox_EFLASH_Mirror_Trigger(BYTE mode, DWORD fw_size, DWORD start_addr);
#endif
