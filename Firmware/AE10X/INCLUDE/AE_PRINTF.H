/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-06-19 19:10:34
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
#ifndef AE_PRINTF_H
#define AE_PRINTF_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
#include "KERNEL_UART.H"
#include "AE_SOFT_FP.H"

#if SUPPORT_REAL_OR_DELAY_PRINTF
extern int printr(const char *fmt, ...);//实时输出
extern int printd(const char *fmt, ...);//延时输出
#define REAL_PRINT_ON F_Service_PUTC = 1;//开启实时输出
#define DELAY_PRINT_ON F_Service_PUTC = 0;//开启延时输出
#else
#define printr printf
#define printd printf
#define REAL_PRINT_ON 
#define DELAY_PRINT_ON 
#endif
/**********************define**************************************************************/
// #define DEBUG
#if DEBUG
#define dprint(fmt, args...) printf(fmt, ##args)//调试输出
#define assert_print(fmt, args...) printf("%s:%d,%s:" fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##args)//调试代码信息输出
#else
#define dprint(fmt, args...)
#define assert_print(fmt, args...)
#endif
#define Moudle_dbg(Mod_Dbg_En,fmt, args...) {if(Mod_Dbg_En)printf(fmt, ##args);}//模块输出
#define Level_dbg(level,fmt, args...) {  if((level) >= Debug_PRINTF_LEVEL)printf(fmt, ##args);}//等级输出
#define Mod_lev_dbg(Mod_Dbg_En,level,fmt, args...) {  if((Mod_Dbg_En)&&((level) >= Debug_PRINTF_LEVEL))printf(fmt, ##args);}//模块等级输出

// #define Mode
#define MODE_SYSCTL Mode.SYSCTL
#define MODE_PNP Mode.PNP
#define MODE_SPIF Mode.SPIF
#define MODE_PMC Mode.PMC
#define MODE_KBC Mode.KBC
#define MODE_SM Mode.SM
#define MODE_SWUC Mode.SWUC
#define MODE_BRAM Mode.BRAM
#define MODE_INTC Mode.INTC
#define MODE_TIMER0 Mode.TIMER&BIT0
#define MODE_TIMER1 Mode.TIMER&BIT1
#define MODE_TIMER2 Mode.TIMER&BIT2
#define MODE_TIMER3 Mode.TIMER&BIT3
#define MODE_KBS Mode.KBS
#define MODE_PS20 Mode.PS2&BIT0
#define MODE_PS21 Mode.PS2&BIT1
#define MODE_GPIO0 Mode.GPIO&BIT0
#define MODE_GPIO1 Mode.GPIO&BIT1
#define MODE_GPIO2 Mode.GPIO&BIT2
#define MODE_GPIO3 Mode.GPIO&BIT3
#define MODE_GPIO4 Mode.GPIO&BIT4
#define MODE_I2C0 Mode.I2C&BIT0
#define MODE_I2C1 Mode.I2C&BIT1
#define MODE_I2C2 Mode.I2C&BIT2
#define MODE_I2C3 Mode.I2C&BIT3
#define MODE_I2C4 Mode.I2C&BIT4
#define MODE_I2C5 Mode.I2C&BIT5
#define MODE_WDT Mode.WDT
#define MODE_PPORT Mode.PPORT
#define MODE_ADC Mode.ADC
#define MODE_PWM0 Mode.PWM&BIT0
#define MODE_PWM1 Mode.PWM&BIT1
#define MODE_PWM2 Mode.PWM&BIT2
#define MODE_PWM3 Mode.PWM&BIT3
#define MODE_PWM4 Mode.PWM&BIT4
#define MODE_PWM5 Mode.PWM&BIT5
#define MODE_PWM6 Mode.PWM&BIT6
#define MODE_PWM7 Mode.PWM&BIT7
#define MODE_UART0 Mode.UART&BIT0
#define MODE_UART1 Mode.UART&BIT1
#define MODE_UART2 Mode.UART&BIT2
#define MODE_UART3 Mode.UART&BIT3
#define MODE_UART4 Mode.UART&BIT4
#define MODE_UART5 Mode.UART&BIT5
#define MODE_UART6 Mode.UART&BIT6
#define MODE_CAN0 Mode.CAN&BIT0
#define MODE_CAN1 Mode.CAN&BIT1
#define MODE_CAN2 Mode.CAN&BIT2
#define MODE_CAN3 Mode.CAN&BIT3
#define MODE_SPIM Mode.SPIM
#define MODE_P80 Mode.P80

#define PRINT_MAX_SIZE 0x400
extern VBYTE print_buff[];
#define ZEROPAD	BIT0		/* pad with zero */
#define SIGN	BIT1		/* unsigned/signed long */
#define PLUS	BIT2		/* show plus */
#define SPACE	BIT3		/* space if plus */
#define LEFT	BIT4		/* left justified */
#define SMALL	BIT5		/* Must be 32 == 0x20 */
#define SPECIAL	BIT6		/* 0x */
#define PRINTF_FLAG_RESERVER   BIT7        /* */
#define FLOAT_8   BIT(8)       /*float8 */
#define HALF_FLOAT   BIT(9)       /*float16 */
#define SIGLE_FLOAT   BIT(10)       /*float */
#define DOUBLE_FLOAT   BIT(11)       /*double */
#define QUAD_FLOAT   BIT(12)       /*float128 */
#define EXP_FLOAT   BIT(13)       /*scientific notation %e */
#define AUTO_FMT_FLOAT   BIT(14)       /*auto format specifier %g*/
#define PRINTF_FLAG_RESERVER2  BIT(15)        /* */

#define is_digit(c) ((c) >= '0' && (c) <= '9')
#if (PRINTF_UART_SWITCH == 0)
#define PRINTF_LSR UART0_LSR
#define PRINTF_LCR UART0_LCR
#define PRINTF_IER UART0_IER
#define PRINTF_IIR UART0_IIR
#define PRINTF_TX UART0_TX
#define PRINTF_RX UART0_RX
#elif (PRINTF_UART_SWITCH == 1)
#define PRINTF_LSR UART1_LSR
#define PRINTF_LCR UART1_LCR
#define PRINTF_IER UART1_IER
#define PRINTF_IIR UART1_IIR
#define PRINTF_TX UART1_TX
#define PRINTF_RX UART1_RX
#elif (PRINTF_UART_SWITCH == 2)
#define PRINTF_LSR UART2_LSR
#define PRINTF_LCR UART2_LCR
#define PRINTF_IER UART2_IER
#define PRINTF_IIR UART2_IIR
#define PRINTF_TX UART2_TX
#define PRINTF_RX UART2_RX
#elif (PRINTF_UART_SWITCH == 3)
#define PRINTF_LSR UART3_LSR
#define PRINTF_LCR UART3_LCR
#define PRINTF_IER UART3_IER
#define PRINTF_IIR UART3_IIR
#define PRINTF_TX UART3_TX
#define PRINTF_RX UART3_RX
#elif (PRINTF_UART_SWITCH == 4)
#define PRINTF_LSR UARTA_LSR
#define PRINTF_LCR UARTA_LCR
#define PRINTF_IER UARTA_IER
#define PRINTF_IIR UARTA_IIR
#define PRINTF_TX UARTA_TX
#define PRINTF_RX UARTA_RX
#elif (PRINTF_UART_SWITCH == 5)
#define PRINTF_LSR UARTB_LSR
#define PRINTF_LCR UARTB_LCR
#define PRINTF_IER UARTB_IER
#define PRINTF_IIR UARTB_IIR
#define PRINTF_TX UARTB_TX
#define PRINTF_RX UARTB_RX
#else
#define PRINTF_LSR UARTA_LSR
#define PRINTF_LCR UARTA_LCR
#define PRINTF_IER UARTA_IER
#define PRINTF_IIR UARTA_IIR
#define PRINTF_TX UARTB_TX
#define PRINTF_RX UARTA_RX
#endif
#define __ismask(x) (_ctype[(BYTE)(x)]) //_ctype[(int)(BYTE )(x)&0x07 != 0
#undef isalnum
#define isalnum(c) ((__ismask(c) & (HEX01 | HEX02 | HEX04))) // 检查所传的字符是否是字母和数字
#undef isalpha
#define isalpha(c) ((__ismask(c) & (HEX01 | HEX02))) // 检查所传的字符是否是字母
#undef iscntrl
#define iscntrl(c) (__ismask(c) & HEX20) // 检查所传的字符是否是控制字符
#undef isdigit
#define isdigit(c) (__ismask(c) & HEX04) // 检查所传的字符是否是十进制数字
#undef isgraph
#define isgraph(c) (__ismask(c) & (HEX10 | HEX01 | HEX02 | HEX04)) // 检查所传的字符是否有图形表示法
#undef islower
#define islower(c) ((__ismask(c) & (HEX01 | HEX02)) == HEX02) // 检查所传的字符是否是小写字母
#undef isprint
#define isprint(c) (__ismask(c) & (HEX10 | HEX01 | HEX02 | HEX04 | HEX80)) // 检查所传的字符是否是可打印的
#undef ispunct
#define ispunct(c) (__ismask(c) & HEX10) // 检查所传的字符是否是标点符号字符
#undef isspace
#define isspace(c) (__ismask(c) & HEX08) // 检查所传的字符是否是空白字符
#undef isupper
#define isupper(c) ((__ismask(c) & (HEX01 | HEX02)) == HEX01) // 检查所传的字符是否是大写字母
#undef isxdigit
#define isxdigit(c) (__ismask(c) & (HEX40 | HEX04)) // 检查所传的字符是否是十六进制数字
#undef tolower
#define tolower(c) (isupper(c) ? ((c) - 'A' + 'a') : (c)) // 大写转小写
#undef toupper
#define toupper(c) (islower(c) ? ((c) - 'a' + 'A') : (c)) // 小写转大写

#endif