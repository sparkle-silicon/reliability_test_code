/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-18 20:56:06
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
#ifndef KERNEL_UART_H
#define KERNEL_UART_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
#define UART0_CHANNEL 0x0
#define UART1_CHANNEL 0x1
// #define UART2_CHANNEL 0x2
// #define UART3_CHANNEL 0x3
#define UARTA_CHANNEL 0x2//0xA
#define UARTB_CHANNEL 0x3//0xB
// bit index
// Interrupt enable
#define UART_IER_PTIME BIT7
#define UART_IER_EDSSI BIT3
#define UART_IER_RLSIE BIT2
#define UART_IER_THREIE BIT1
#define UART_IER_RDAIE BIT0
// Interrupt status
#define UART_IIR_RLS HEX06
#define UART_IIR_RDA HEX04
#define UART_IIR_TIMEOUT HEX0C
#define UART_IIR_THRE HEX02
#define UART_IIR_MS HEX00
#define UART_IIR_BUSY HEX07
// FIFO control
#define UART_FCR_RFTL_14BYTE (BIT7 | BIT6)
#define UART_FCR_RFTL_8BYTE BIT7
#define UART_FCR_RFTL_4BYTE BIT6
#define UART_FCR_RFTL_1BYTE (~UART_FCR_RFTL_14BYTE)
#define UART_FCR_TFTL_14BYTE (BIT5 | BIT4)
#define UART_FCR_TFTL_8BYTE BIT5
#define UART_FCR_TFTL_4BYTE BIT4
#define UART_FCR_TFTL_1BYTE (~UART_FCR_TFTL_14BYTE)
#define UART_FCR_TFR BIT2
#define UART_FCR_RFR BIT1
#define UART_FCR_FIFOEN BIT0
#define UART_LCR_DLAB BIT7
#define UART_LCR_DLS_8bit (BIT1 | BIT0)
#define UART_LSR_RFE BIT7
#define UART_LSR_TEMP BIT6
#define UART_LSR_THRE BIT5
#define UART_LSR_BI BIT4
#define UART_LSR_FE BIT3
#define UART_LSR_PE BIT2
#define UART_LSR_OE BIT1
#define UART_LSR_DR BIT0
#define UART_LCR_DATA8BIT HEX03
/* TXCTRL register */
#define UART_TXEN 0x1
#define UART_TXWM(x) (((x) & 0xffff) << 16)
/* RXCTRL register */
#define UART_RXEN 0x1
#define UART_RXWM(x) (((x) & 0xffff) << 16)
/* IP register */
#define UART_IP_TXWM 0x1
#define UART_IP_RXWM 0x2

#define UART_CLOCK  (HIGH_CHIP_CLOCK / (SYSCTL_CLKDIV_UART+1)) // UART clock

extern BYTE Uart_Int_Enable(BYTE channel, BYTE type);
extern BYTE Uart_Int_Disable(BYTE channel, BYTE type);
extern BYTE Uart_Int_Enable_Read(BYTE channel, BYTE type);
extern BYTE Uart_Int_Status(BYTE channel, BYTE type);
extern int serial_init(DWORD uart_num, DWORD baudrate);
int serial_deinit(DWORD uart_num);/*取消串口初始化（再次使用要重新init）*/
int serial2gpio(DWORD uart_num);/*串口切换为GPIO，并关闭串口模块*/
int gpio2serial(DWORD uart_num);/*GPIO切换为串口，并使能串口模块*/
extern void Service_PUTC(void);
#endif