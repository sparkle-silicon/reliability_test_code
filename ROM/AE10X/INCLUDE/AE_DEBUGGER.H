/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-16 16:17:21
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
#ifndef AE_DEBUGGER_H
#define AE_DEBUGGER_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "AE_UPDATE.H"
#include "KERNEL_INCLUDE.H"
#define DEBUGGER_CMD_READ_REQUEST 0x55  //		0x55--read request
#define DEBUGGER_CMD_WRITE_REQUEST 0xFF //		0xff--write request
#define DEBUGGER_CMD_RESET_CHIP 0xE0    //		0xE0--Reset chip 
#define DEBUGGER_CMD_HANDSHAKE 0x99     //		0x99--Handshake Protocol
#define DISCONNECTION_REQUEST 0x66      // 		0x66--no used DEBUGGER FUNCTION使用退出
#define DEBUGGER_CMD_UPDATE 0x75        //		0xff--'u' update firmware

// #define DEBUGGER_BASE_ADDR  UART1_BASE_ADDR
extern volatile uint32_t DEBUGGER_FIFO;
#define DEBUGGER_REG_ADDR(offset) REG_ADDR(DEBUGGER_FIFO, offset)
#define DEBUGGER_REG(offset) REG8(DEBUGGER_REG_ADDR(offset))
#define DEBUGGER_RX DEBUGGER_REG(UART_RBR_OFFSET)
#define DEBUGGER_TX DEBUGGER_REG(UART_THR_OFFSET)
#define DEBUGGER_DLL DEBUGGER_REG(UART_DLL_OFFSET)
#define DEBUGGER_DLH DEBUGGER_REG(UART_DLH_OFFSET)
#define DEBUGGER_IER DEBUGGER_REG(UART_IER_OFFSET)
#define DEBUGGER_IIR DEBUGGER_REG(UART_IIR_OFFSET)
#define DEBUGGER_FCR DEBUGGER_REG(UART_FCR_OFFSET)
#define DEBUGGER_LCR DEBUGGER_REG(UART_LCR_OFFSET)
#define DEBUGGER_MICR DEBUGGER_REG(UART_MICR_OFFSET)
#define DEBUGGER_LSR DEBUGGER_REG(UART_LSR_OFFSET)
#define DEBUGGER_MSR DEBUGGER_REG(UART_MSR_OFFSET)
#define DEBUGGER_USR DEBUGGER_REG(UART_USR_OFFSET)

//*****************************************************************************
// extern variable
//*****************************************************************************
extern int32_t Deubgger_Uart_Find(const uint32_t looplimit);
extern uint32_t Deubgger_Cmd_Parsing(const unsigned char *cmd);
int32_t GET_DEBUGGER_CMD(void);
#endif


