/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-07-08 09:53:28
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
#if TEST
#include "HARDWARE_CAN.H"
#endif
//  The Debugger Function Uart Entrance
#define READ_REQUEST 0x55			   //		0x55--read request
#define WRITE_REQUEST 0xFF			   //		0xff--write request
#define GET_DEBUG_TEMP 0x88			   //		0x88--Cancel set to slave so can get Debug_Temp
#define SLAVE_READ_DATA 0xDD		   //		0xDD--Set to slave so can read data
#define KBD_READ_REQUEST 0xAA		   //		0xAA--KBD read request
#define CANCEL_KBD_READ_REQUEST 0xBB   //		0xBB--cancel KBD read request
#define RESET_CHIP_USE_WATCHDOG 0xE0   //		0xE0--Reset chip (use watchdog)
#define SWITCH_KEYBOARD_CODE 0xB0	   //		0xB0--Switching keyboard Codes
#define FIRMWARE_UPDATE 0x75		   //		0x75--Firmware update
#define INTERRUPT_FUNCTION 0x11		   //		0x11--Interrupt function block
#define RECOARD_KBC_PMC_DATA 0xCC	   //      0xCC--Recoard KBC/PMC data
#define STOP_RECOARD_KBC_PMC_DATA 0xEE //      0xEE--Stop recording KBC/PMC data
/*****************************************************************************
 *Debugger Macro definition
 ****************************************************************************/
#define DEBUGGER_OUTPUT_SWITCH 0				// 0: uart  1: i2c  2: LCP
#define DEBUGGER_UART_CHANNEL DEBUG_UART_SWITCH // select UART channel as debugger (0/1/2/3/4/5/-1)
////!!!!!!!!!!
// attention! iic_debugger must use the smbus0
//  the "DEBUGGER_I2C_CHANNEL" is just switch the channel mapping instead of initialization
#define DEBUGGER_I2C_CHANNEL I2C_CHANNEL_0 // select I2C channel mapping of smbus0
////!!!!!!!!!!
#if ENABLE_DEBUGGER_SUPPORT
/* Debugger Slave or Master channel */
#if (DEBUGGER_UART_CHANNEL == 0)
#define DEBUGGER_UART0_CHANNEL 1 // debugger uart channel
#define DEBUGGER_UART UART0_BASE_ADDR
#define DEBUGGER_UART_REG UART0_REG
#elif (DEBUGGER_UART_CHANNEL == 1)
#define DEBUGGER_UART1_CHANNEL 1
#define DEBUGGER_UART UART1_BASE_ADDR
#define DEBUGGER_UART_REG UART1_REG
// #elif (DEBUGGER_UART_CHANNEL == 2)
// #define DEBUGGER_UART2_CHANNEL 1
// #define DEBUGGER_UART UART2_BASE_ADDR
// #define DEBUGGER_UART_REG UART2_REG
// #elif (DEBUGGER_UART_CHANNEL == 3)
// #define DEBUGGER_UART3_CHANNEL 1
// #define DEBUGGER_UART UART3_BASE_ADDR
// #define DEBUGGER_UART_REG UART3_REG
#elif (DEBUGGER_UART_CHANNEL == 4)
#define DEBUGGER_UARTA_CHANNEL 1
#define DEBUGGER_UART UARTA_BASE_ADDR
#define DEBUGGER_UART_REG UARTA_REG
#elif (DEBUGGER_UART_CHANNEL == 5)
#define DEBUGGER_UARTB_CHANNEL 1
#define DEBUGGER_UART UARTB_BASE_ADDR
#define DEBUGGER_UART_REG UARTB_REG
#else
#define DEBUGGER_UART NULL
#define DEBUGGER_UART_REG(offset) 0
#endif
#if ENABLE_DEBUGGER_SUPPORT // Macros that enable the entire debugger function
#if (DEBUG_UART_SWITCH == PRINTF_UART_SWITCH || DEBUG_UART_SWITCH == COMMAND_UART_SWITCH)
#if (DEBUG_UART_SWITCH < 6 && DEBUG_UART_SWITCH >= 0)
#warning "If you think that the debuger and the output are on the same serial port, it will probably make an error"
#endif
#endif
#endif
#define KBD_BUFF_SIZE 256
struct KBD_Event
{
	BYTE data[KBD_BUFF_SIZE];
	uint16_t head;
	uint16_t tail;
};
//*****************************************************************************
// extern variable
//*****************************************************************************
extern char Debugger_Data[];
extern char Debugger_Cmd[];
extern u8 buf[];
extern char KBC_PMC_Event[];
extern WORD Intr_num[];
extern void Deubgger_Cmd_Parsing(BYTE data);
extern void Debugger_Cmd_IRQ(BYTE debuge_data);
extern void Debugger_I2c_Req(WORD baseaddr);
extern void Debugger_I2c_Send(WORD baseaddr);
extern void DEBUGGER_DATA(void); // Debugger Function read
extern void DEBUGGER_Change(void);
extern void KBC_PMC_DataPending(char *KBC_PMC_PendingData);
extern BYTE Debugger_KBC_PMC_Record(BYTE direction, BYTE channel, BYTE value);
extern BYTE Debugger_KBD_Record(BYTE value);
extern void Debugger_Send_KBD(void);
extern void DEBUGGER_Int(void);
extern void DEBUGER_putchar(char ch);
extern BYTE Debuger_Cmd_Data; // Debugger CMD data
#endif
extern WORD Intr_num[];
#endif
