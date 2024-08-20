/*
 * @Author: dejavuwdh
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-06 10:48:08
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
#ifndef AE_ROMPATCH_H
#define AE_ROMPATCH_H
#include "AE_GLOBAL.H"
#include "AE_INCLUDE.H"
#if PRINTF_UART_SWITCH == 0
#define UART_BAUD UART0_BAUD
#elif PRINTF_UART_SWITCH == 1
#define UART_BAUD UART1_BAUD
#elif PRINTF_UART_SWITCH == 2
#define UART_BAUD UART2_BAUD
#elif PRINTF_UART_SWITCH == 3
#define UART_BAUD UART3_BAUD
#elif PRINTF_UART_SWITCH == 4
#define UART_BAUD UARTA_BAUD
#elif PRINTF_UART_SWITCH == 5
#define UART_BAUD UARTB_BAUD
#endif





void OPTIMIZE0 USED rom_patch_config_func(void);
void OPTIMIZE0 USED rom_patch_test_func_flash(void);
extern spatch_typdef patch_array[];
#define X1_RA (0x1)
#define JAL_RD(rd) ((rd & 0x1f) << 7)
#define JAL_OPCODE (0x6f)
#define JAL_ADDR_OFFSET(pc_addr, jp_addr) (jp_addr - pc_addr)
#define JAL_IMM(imm) (((imm & 0x7fe) << 20) | ((imm & 0x800) << 9) | (imm & 0xff000) | ((imm & 0x100000) << 11))
#define JAL_CODE(rd, pc_addr, jp_addr) (JAL_OPCODE | JAL_RD(rd) | JAL_IMM(JAL_ADDR_OFFSET(pc_addr, jp_addr)))
// JAL_ADDR_OFFSET(pc_addr,jp_addr)))
#define JAL_RA_CODE(pc_addr, jp_addr) (0xef | JAL_IMM(JAL_ADDR_OFFSET(pc_addr, jp_addr)))
// JAL_ADDR_OFFSET(pc_addr,jp_addr)))
#define JAL_CODE_BYTE0(rd) ((JAL_OPCODE & 0x7f) | (JAL_RD(rd) & 0x80)) & 0xff
#define JAL_CODE_BYTE1(rd, imm) (((imm & 0xf000) | (JAL_RD(rd) & 0xf00)) >> 8) & 0xff
#define JAL_CODE_BYTE2(imm) (((imm & 0xf0000) >> 16) | ((imm & 0x800) >> 7) | ((imm & 0xe) << 4)) & 0xff
#define JAL_CODE_BYTE3(imm) ((((imm & 0x7f0) >> 4) | ((imm & 0x100000) >> 13))) & 0xff
#endif