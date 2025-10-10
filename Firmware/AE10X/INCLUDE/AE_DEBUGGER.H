/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-10 16:07:44
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
#define PMC_KBC_SendCnt 256
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
#define RECOARD_KBC_PMC_DATA 0xCC		//      	0xCC--Recoard KBC/PMC data
#define STOP_RECOARD_KBC_PMC_DATA 0xEE	//      	0xEE--Stop recording KBC/PMC data
#define HANDSHAKE_REQUEST 0x99			// 		0x99--Handshake request
#define DISCONNECTION_REQUEST 0x66		// 		0x66--no used DEBUGGER FUNCTION

#define DEBUGGER_BASE_ADDR  UART1_BASE_ADDR
// extern volatile uint32_t DEBUGGER_FIFO;
#define DEBUGGER_REG_ADDR(offset) REG_ADDR(DEBUGGER_BASE_ADDR, offset)
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


/*****************************************************************************
 *Debugger Sequence flag definition
 ****************************************************************************/
#define D_PowerOn 			0x01
#define D_HibernateWakeup 	0x02
#define D_SleepWakeup 		0x03
#define D_Sleep 			0x04
#define D_Hibernate			0x05
#define D_Shutdown 			0x06
#define D_Reboot			0x07
/*****************************************************************************
 *Debugger Macro definition
 ****************************************************************************/
////!!!!!!!!!!
#if ENABLE_DEBUGGER_SUPPORT&&(DEBUGGER_OUTPUT_SWITCH==1)
// attention! iic_debugger must use the smbus0
//  the "DEBUGGER_I2C_CHANNEL" is just switch the channel mapping instead of initialization
#define DEBUGGER_I2C_CHANNEL I2C_CHANNEL_0 // select I2C channel mapping of smbus0
#define DEBUGGER_I2C_SPEED I2C0_SPEED // select I2C channel mapping of smbus0
#define DEBUGGER_I2C_ADDRESS 0x6A // select I2C channel mapping of smbus0
#endif
////!!!!!!!!!!
#if ENABLE_DEBUGGER_SUPPORT
/* Debugger Slave or Master channel */
#if (DEBUG_UART_SWITCH == 0)
#define DEBUGGER_UART UART0_BASE_ADDR
#elif (DEBUG_UART_SWITCH == 1)
#define DEBUGGER_UART UART1_BASE_ADDR
// #elif (DEBUG_UART_SWITCH == 2)
// #define DEBUGGER_UART UART2_BASE_ADDR
// #elif (DEBUG_UART_SWITCH == 3)
// #define DEBUGGER_UART UART3_BASE_ADDR
#elif (DEBUG_UART_SWITCH == 4)
#define DEBUGGER_UART UARTA_BASE_ADDR
#elif (DEBUG_UART_SWITCH == 5)
#define DEBUGGER_UART UARTB_BASE_ADDR
#else
#define DEBUGGER_UART NULL
#endif
#if ENABLE_DEBUGGER_SUPPORT // Macros that enable the entire debugger function
#if (DEBUG_UART_SWITCH == PRINTF_UART_SWITCH || DEBUG_UART_SWITCH == COMMAND_UART_SWITCH)
#if (DEBUG_UART_SWITCH < 6 && DEBUG_UART_SWITCH >= 0)
#warning "If you think that the debuger and the output are on the same serial port, it will probably make an error"
#endif
#endif
#endif
#define DEBUGGER_BUFF_SIZE 256
/*uart buffer size*/
#define UART_BUFFER_SIZE 64
#define KBD_BUFF_SIZE DEBUGGER_BUFF_SIZE
struct KBD_Event{
	BYTE data[KBD_BUFF_SIZE];
	uint16_t head;
	uint16_t tail;
};
//*****************************************************************************
// extern variable
//*****************************************************************************
extern BYTE Debugger_KBC_PMC_Record(BYTE direction, BYTE channel, BYTE value);
extern BYTE Debugger_KBD_Record(BYTE value);
extern BYTE Debuger_Cmd_Data; // Debugger CMD data
extern WORD Intr_num[];
extern char Debugger_Data[];
extern char Debugger_Cmd[];
extern short buf[];
extern char KBC_PMC_Event[];

extern void Deubgger_Cmd_Parsing(BYTE data);
extern void Debugger_Cmd_IRQ(BYTE debuge_data);
extern void Debugger_I2c_Req(WORD baseaddr);
extern void Debugger_I2c_Send(WORD baseaddr);
extern void DEBUGGER_DATA(void); // Debugger Function read
extern void DEBUGGER_Change(void);
extern void KBC_PMC_DataPending(char *KBC_PMC_PendingData);
extern void Debugger_Send_KBD(void);
extern void DEBUGGER_Int(void);
extern void DEBUGER_putchar(char ch);

char Debugger_Handshake(BYTE data);
#endif
extern WORD Intr_num[];
#endif
