/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-07-02 16:25:05
 * @Description: This file is used for INTC interrupt
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
#include "KERNEL_INTC.H"
#include "KERNEL_MAIN.H"
#include "KERNEL_I2C.H"
#include "KERNEL_CEC.H"
#include "AE_DEBUGGER.H"
#include "AE_COMMAND.H"
#include "CUSTOM_POWER.H"
#define TEST_INTC 0 //测试二级中断代码
#define INTC_MODE 0 //1:mask 0:disable
extern unsigned char iicFeedback, iic_flag, iic_int_flag;
extern char Uart_buffer[UART_BUFFER_SIZE]; // An array of data transferred by the debugger
extern BYTE RPMC_OOB_TempArr[80];
/************weak声明************/
void intr0_gpio_a0(void) __weak;
void intr0_gpio_a1(void) __weak;
void intr0_gpio_a2(void) __weak;
void intr0_gpio_a3(void) __weak;
void intr0_gpio_a4(void) __weak;
void intr0_gpio_a5(void) __weak;
void intr0_gpio_a6(void) __weak;
void intr0_gpio_a7(void) __weak;
void intr0_gpio_a8(void) __weak;
void intr0_gpio_a9(void) __weak;
void intr0_gpio_a10(void) __weak;
void intr0_gpio_a11(void) __weak;
void intr0_gpio_a12(void) __weak;
void intr0_gpio_a13(void) __weak;
void intr0_gpio_a14(void) __weak;
void intr0_gpio_a15(void) __weak;
void intr0_gpio_a16(void) __weak;
void intr0_gpio_a17(void) __weak;
void intr0_gpio_a18(void) __weak;
void intr0_gpio_a19(void) __weak;
void intr0_gpio_a20(void) __weak;
void intr0_gpio_a21(void) __weak;
void intr0_gpio_a22(void) __weak;
void intr0_gpio_a23(void) __weak;
void intr0_gpio_a24(void) __weak;
void intr0_gpio_a25(void) __weak;
void intr0_gpio_a26(void) __weak;
void intr0_gpio_a27(void) __weak;
void intr0_gpio_a28(void) __weak;
void intr0_gpio_a29(void) __weak;
void intr0_gpio_a30(void) __weak;
void intr0_gpio_a31(void) __weak;
void intr0_gpio_b0(void) __weak;
void intr0_gpio_b1(void) __weak;
void intr0_gpio_b2(void) __weak;
void intr0_gpio_b3(void) __weak;
void intr0_gpio_b4(void) __weak;
void intr0_gpio_b5(void) __weak;
void intr0_gpio_b6(void) __weak;
void intr0_gpio_b7(void) __weak;
void intr0_gpio_b8(void) __weak;
void intr0_gpio_b9(void) __weak;
void intr0_gpio_b10(void) __weak;
void intr0_gpio_b11(void) __weak;
void intr0_gpio_b12(void) __weak;
void intr0_gpio_b13(void) __weak;
void intr0_gpio_b14(void) __weak;
void intr0_gpio_b15(void) __weak;
void intr0_gpio_b16(void) __weak;
void intr0_gpio_b17(void) __weak;
void intr0_gpio_b18(void) __weak;
void intr0_gpio_b19(void) __weak;
void intr0_gpio_b20(void) __weak;
void intr0_gpio_b21(void) __weak;
void intr0_gpio_b22(void) __weak;
void intr0_gpio_b23(void) __weak;
void intr0_gpio_b24(void) __weak;
void intr0_gpio_b25(void) __weak;
void intr0_gpio_b26(void) __weak;
#if (defined(AE103))
void intr0_gpio_b27(void) __weak;
void intr0_gpio_b28(void) __weak;
void intr0_gpio_b29(void) __weak;
void intr0_gpio_b30(void) __weak;
void intr0_gpio_b31(void) __weak;
#endif
void intr1_gpio_c0(void) __weak;  // 0
void intr1_gpio_c1(void) __weak;  // 1
void intr1_gpio_c2(void) __weak;  // 2
void intr1_gpio_c3(void) __weak;  // 3
void intr1_gpio_c4(void) __weak;  // 4
void intr1_gpio_c5(void) __weak;  // 5
void intr1_gpio_c6(void) __weak;  // 6
void intr1_gpio_c7(void) __weak;  // 7
void intr1_gpio_c8(void) __weak;  // 8
void intr1_gpio_c9(void) __weak;  // 9
void intr1_gpio_c10(void) __weak; // 10
void intr1_gpio_c11(void) __weak; // 11
void intr1_gpio_c12(void) __weak; // 12
void intr1_gpio_c13(void) __weak; // 13
#if (defined(AE103))
void intr1_gpio_c14(void) __weak; // 14
void intr1_gpio_c15(void) __weak; // 15
#endif
// void intr1_gpio_e0(void) __weak; // 14 16
// void intr1_gpio_e1(void) __weak; // 15 17
// void intr1_gpio_e2(void) __weak; // 16 18
// void intr1_gpio_e3(void) __weak; // 17 19
// void intr1_gpio_e4(void) __weak; // 18 20
// void intr1_gpio_e5(void) __weak; // 19 21
// void intr1_gpio_e6(void) __weak; // 20 22
// void intr1_gpio_e7(void) __weak; // 21 23
// void intr1_gpio_e8(void) __weak;  // 22
// void intr1_gpio_e9(void) __weak;  // 23
// void intr1_gpio_e10(void) __weak; // 24
// void intr1_gpio_e11(void) __weak; // 25
// void intr1_gpio_e12(void) __weak; // 26
// void intr1_gpio_e13(void) __weak; // 27
// void intr1_gpio_e14(void) __weak; // 28
// void intr1_gpio_e15(void) __weak; // 29

#if defined(AE103)
#if (GLE01 == 1)
void intr1_gpio_e16(void) __weak; // 30
void intr1_gpio_e17(void) __weak; // 31
void intr1_gpio_e18(void) __weak; // 32
void intr1_gpio_e19(void) __weak; // 33
void intr1_gpio_e20(void) __weak; // 34
void intr1_gpio_e21(void) __weak; // 35
void intr1_gpio_e22(void) __weak; // 36
void intr1_gpio_e23(void) __weak; // 37
void intr1_dma(void) __weak;	  // 24
void intr1_cec0(void) __weak;	  // 25
void intr1_cec1(void) __weak;	  // 26
#else
void intr1_sm2(void) __weak; // 24
void intr1_sm3(void) __weak; // 25
#endif
void intr1_smbus4(void) __weak;		 // 27
void intr1_smbus5(void) __weak;		 // 28
void intr1_owi(void) __weak;		 // 29
void intr1_null30(void) __weak;		 // 30
void intr1_peci(void) __weak;		 // 31
void intr1_i3c0(void) __weak;		 // 32
void intr1_i3c1(void) __weak;		 // 33
void intr1_i3c2(void) __weak;		 // 34
void intr1_i3c3(void) __weak;		 // 35
void intr1_null36(void) __weak;		 // 36
void intr1_null37(void) __weak;		 // 37
void intr1_pmc2_ibf_ec(void) __weak; // 38
void intr1_pmc2_obe_ec(void) __weak; // 39
#endif
void intr1_pmc3_ibf_ec(void) __weak; // 38 40
void intr1_pmc3_obe_ec(void) __weak; // 39 41
void intr1_pmc4_ibf_ec(void) __weak; // 40 42
void intr1_pmc4_obe_ec(void) __weak; // 41 43
void intr1_pmc5_ibf_ec(void) __weak; // 42 44
void intr1_pmc5_obe_ec(void) __weak; // 43 45
void intr1_ps2_1(void) __weak;		 // 44 46
#if defined(AE101)
void intr1_null45(void) __weak; // 45 47
void intr1_null46(void) __weak; // 46 48
void intr1_null47(void) __weak; // 47 49
#elif (defined(AE102) || defined(AE103))
void intr1_uart1(void) __weak;	// 45 47
void intr1_null48(void) __weak; // 46 48
void intr1_null49(void) __weak; // 47 49
#endif
void intr1_spim(void) __weak;	// 48 50
void intr1_smbus0(void) __weak; // 49 51
void intr1_smbus1(void) __weak; // 50 52
void intr1_smbus2(void) __weak; // 51 53
void intr1_smbus3(void) __weak; // 52 54
#if defined(AE101)
void intr1_null53(void) __weak; // 53 55
void intr1_null54(void) __weak; // 54 56
void intr1_null55(void) __weak; // 55 57
void intr1_null56(void) __weak; // 56 58
void intr1_null57(void) __weak; // 57 59
#elif (defined(AE102) || defined(AE103))
void intr1_smbus6(void) __weak; // 53 55
void intr1_smbus7(void) __weak; // 54 56
void intr1_smbus8(void) __weak; // 55 57
void intr1_null58(void) __weak; // 56 58
void intr1_null59(void) __weak; // 57 59
#endif
void intr1_por_int(void) __weak; // 58 60
#if ((defined(AE103)) && (GLE01 == 1))
void intr1_mailbox(void) __weak; // 59 61
#else
void intr1_null61(void) __weak; // 59 61
#endif
#if (defined(AE103))
void intr1_espi(void) __weak;	// 62
void intr1_null63(void) __weak; // 63
#endif
/************weak声明************/
void disable_intc(int index, int num)
{
	set_ictl_reg(index, INTC_ICTL_INTMASK_L, num);
}
void enable_intc(int index, int num)
{
	clear_ictl_reg(index, INTC_ICTL_INTMASK_L, num);
}
void intr0_gpio_a0(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[32]++;
#endif
	if(GPIO0_INTSTATUS0 & (0x1 << 0)) // if int happened
	{
		// BRAM_EC_Read();
		// BRAM_EC_Write();
		GPIO0_EOI0 |= (0x1 << 0); // clear int
	}
#if SUPPORT_GPIO_WAKEUP
	Exit_LowPower_Mode();
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a0 mask\n");
	ICTL0_INTMASK0 |= 0x1 << 0;
#else
	printf("intr0_gpio_a0 disable\n");
	ICTL0_INTEN0 &= ~(0x1 << 0);
#endif
#endif
}
void intr0_gpio_a1(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[33]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a1 mask\n");
	ICTL0_INTMASK0 |= 0x1 << 1;
#else
	printf("intr0_gpio_a1 disable\n");
	ICTL0_INTEN0 &= ~(0x1 << 1);
#endif
#endif
}
void intr0_gpio_a2(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[34]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a2 mask\n");
	ICTL0_INTMASK0 |= 0x1 << 2;
#else
	printf("intr0_gpio_a2 disable\n");
	ICTL0_INTEN0 &= ~(0x1 << 2);
#endif
#endif
}
void intr0_gpio_a3(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[35]++;
#endif
	GPIO0_EOI0 |= (0x1 << 3); // clear int
#if SUPPORT_GPIO_WAKEUP
	Exit_LowPower_Mode();
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a3 mask\n");
	ICTL0_INTMASK0 |= 0x1 << 3;
#else
	printf("intr0_gpio_a3 disable\n");
	ICTL0_INTEN0 &= ~(0x1 << 3);
#endif
#endif
}
void intr0_gpio_a4(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[36]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a4 mask\n");
	ICTL0_INTMASK0 |= 0x1 << 4;
#else
	printf("intr0_gpio_a4 disable\n");
	ICTL0_INTEN0 &= ~(0x1 << 4);
#endif
#endif
}
void intr0_gpio_a5(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[37]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a5 mask\n");
	ICTL0_INTMASK0 |= 0x1 << 5;
#else
	printf("intr0_gpio_a5 disable\n");
	ICTL0_INTEN0 &= ~(0x1 << 5);
#endif
#endif
}
void intr0_gpio_a6(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[38]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a6 mask\n");
	ICTL0_INTMASK0 |= 0x1 << 6;
#else
	printf("intr0_gpio_a6 disable\n");
	ICTL0_INTEN0 &= ~(0x1 << 6);
#endif
#endif
}
void intr0_gpio_a7(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[39]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a7 mask\n");
	ICTL0_INTMASK0 |= 0x1 << 7;
#else
	printf("intr0_gpio_a7 disable\n");
	ICTL0_INTEN0 &= ~(0x1 << 7);
#endif
#endif
}
void intr0_gpio_a8(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[40]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a8 mask\n");
	ICTL0_INTMASK1 |= 0x1 << 0;
#else
	printf("intr0_gpio_a8 disable\n");
	ICTL0_INTEN1 &= ~(0x1 << 0);
#endif
#endif
}
void intr0_gpio_a9(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[41]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a9 mask\n");
	ICTL0_INTMASK1 |= 0x1 << 1;
#else
	printf("intr0_gpio_a9 disable\n");
	ICTL0_INTEN1 &= ~(0x1 << 1);
#endif
#endif
}
void intr0_gpio_a10(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[42]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a10 mask\n");
	ICTL0_INTMASK1 |= 0x1 << 2;
#else
	printf("intr0_gpio_a10 disable\n");
	ICTL0_INTEN1 &= ~(0x1 << 2);
#endif
#endif
}
void intr0_gpio_a11(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[43]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a11 mask\n");
	ICTL0_INTMASK1 |= 0x1 << 3;
#else
	printf("intr0_gpio_a11 disable\n");
	ICTL0_INTEN1 &= ~(0x1 << 3);
#endif
#endif
}
void intr0_gpio_a12(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[44]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a12 mask\n");
	ICTL0_INTMASK1 |= 0x1 << 4;
#else
	printf("intr0_gpio_a12 disable\n");
	ICTL0_INTEN1 &= ~(0x1 << 4);
#endif
#endif
}
void intr0_gpio_a13(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[45]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a13 mask\n");
	ICTL0_INTMASK1 |= 0x1 << 5;
#else
	printf("intr0_gpio_a13 disable\n");
	ICTL0_INTEN1 &= ~(0x1 << 5);
#endif
#endif
}
void intr0_gpio_a14(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[46]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a14 mask\n");
	ICTL0_INTMASK1 |= 0x1 << 6;
#else
	printf("intr0_gpio_a14 disable\n");
	ICTL0_INTEN1 &= ~(0x1 << 6);
#endif
#endif
}
void intr0_gpio_a15(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[47]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a15 mask\n");
	ICTL0_INTMASK1 |= 0x1 << 7;
#else
	printf("intr0_gpio_a15 disable\n");
	ICTL0_INTEN1 &= ~(0x1 << 7);
#endif
#endif
}
void intr0_gpio_a16(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[48]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a16 mask\n");
	ICTL0_INTMASK2 |= 0x1 << 0;
#else
	printf("intr0_gpio_a16 disable\n");
	ICTL0_INTEN2 &= ~(0x1 << 0);
#endif
#endif
}
void intr0_gpio_a17(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[49]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a17 mask\n");
	ICTL0_INTMASK2 |= 0x1 << 1;
#else
	printf("intr0_gpio_a17 disable\n");
	ICTL0_INTEN2 &= ~(0x1 << 1);
#endif
#endif
}
void intr0_gpio_a18(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[50]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a18 mask\n");
	ICTL0_INTMASK2 |= 0x1 << 2;
#else
	printf("intr0_gpio_a18 disable\n");
	ICTL0_INTEN2 &= ~(0x1 << 2);
#endif
#endif
}
void intr0_gpio_a19(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[51]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a19 mask\n");
	ICTL0_INTMASK2 |= 0x1 << 3;
#else
	printf("intr0_gpio_a19 disable\n");
	ICTL0_INTEN2 &= ~(0x1 << 3);
#endif
#endif
}
void intr0_gpio_a20(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[52]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a20 mask\n");
	ICTL0_INTMASK2 |= 0x1 << 4;
#else
	printf("intr0_gpio_a20 disable\n");
	ICTL0_INTEN2 &= ~(0x1 << 4);
#endif
#endif
}
void intr0_gpio_a21(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[53]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a21 mask\n");
	ICTL0_INTMASK2 |= 0x1 << 5;
#else
	printf("intr0_gpio_a21 disable\n");
	ICTL0_INTEN2 &= ~(0x1 << 5);
#endif
#endif
}
void intr0_gpio_a22(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[54]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a22 mask\n");
	ICTL0_INTMASK2 |= 0x1 << 6;
#else
	printf("intr0_gpio_a22 disable\n");
	ICTL0_INTEN2 &= ~(0x1 << 6);
#endif
#endif
}
void intr0_gpio_a23(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[55]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a23 mask\n");
	ICTL0_INTMASK2 |= 0x1 << 7;
#else
	printf("intr0_gpio_a23 disable\n");
	ICTL0_INTEN2 &= ~(0x1 << 7);
#endif
#endif
}
void intr0_gpio_a24(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[56]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a24 mask\n");
	ICTL0_INTMASK3 |= 0x1 << 0;
#else
	printf("intr0_gpio_a24 disable\n");
	ICTL0_INTEN3 &= ~(0x1 << 0);
#endif
#endif
}
void intr0_gpio_a25(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[57]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a25 mask\n");
	ICTL0_INTMASK3 |= 0x1 << 1;
#else
	printf("intr0_gpio_a25 disable\n");
	ICTL0_INTEN3 &= ~(0x1 << 1);
#endif
#endif
}
void intr0_gpio_a26(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[58]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a26 mask\n");
	ICTL0_INTMASK3 |= 0x1 << 2;
#else
	printf("intr0_gpio_a26 disable\n");
	ICTL0_INTEN3 &= ~(0x1 << 2);
#endif
#endif
}
void intr0_gpio_a27(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[59]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a27 mask\n");
	ICTL0_INTMASK3 |= 0x1 << 3;
#else
	printf("intr0_gpio_a27 disable\n");
	ICTL0_INTEN3 &= ~(0x1 << 3);
#endif
#endif
}
void intr0_gpio_a28(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[60]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a28 mask\n");
	ICTL0_INTMASK3 |= 0x1 << 4;
#else
	printf("intr0_gpio_a28 disable\n");
	ICTL0_INTEN3 &= ~(0x1 << 4);
#endif
#endif
}
void intr0_gpio_a29(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[61]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a29 mask\n");
	ICTL0_INTMASK3 |= 0x1 << 5;
#else
	printf("intr0_gpio_a29 disable\n");
	ICTL0_INTEN3 &= ~(0x1 << 5);
#endif
#endif
}
void intr0_gpio_a30(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[62]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a30 mask\n");
	ICTL0_INTMASK3 |= 0x1 << 6;
#else
	printf("intr0_gpio_a30 disable\n");
	ICTL0_INTEN3 &= ~(0x1 << 6);
#endif
#endif
}
void intr0_gpio_a31(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[63]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_a31 mask\n");
	ICTL0_INTMASK3 |= 0x1 << 7;
#else
	printf("intr0_gpio_a31 disable\n");
	ICTL0_INTEN3 &= ~(0x1 << 7);
#endif
#endif
}
void intr0_gpio_b0(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[64]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b0 mask\n");
	ICTL0_INTMASK4 |= 0x1 << 0;
#else
	printf("intr0_gpio_b0 disable\n");
	ICTL0_INTEN4 &= ~(0x1 << 0);
#endif
#endif
}
void intr0_gpio_b1(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[65]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b1 mask\n");
	ICTL0_INTMASK4 |= 0x1 << 1;
#else
	printf("intr0_gpio_b1 disable\n");
	ICTL0_INTEN4 &= ~(0x1 << 1);
#endif
#endif
}
void intr0_gpio_b2(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[66]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b2 mask\n");
	ICTL0_INTMASK4 |= 0x1 << 2;
#else
	printf("intr0_gpio_b2 disable\n");
	ICTL0_INTEN4 &= ~(0x1 << 2);
#endif
#endif
}
void intr0_gpio_b3(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[67]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b3 mask\n");
	ICTL0_INTMASK4 |= 0x1 << 3;
#else
	printf("intr0_gpio_b3 disable\n");
	ICTL0_INTEN4 &= ~(0x1 << 3);
#endif
#endif
}
void intr0_gpio_b4(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[68]++;
#endif
	GPIO1_EOI0 |= 0x1 << 4; // 清除中断
#if SUPPORT_GPIO_WAKEUP
	Exit_LowPower_Mode();
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b4 mask\n");
	ICTL0_INTMASK4 |= 0x1 << 4;
#else
	printf("intr0_gpio_b4 disable\n");
	ICTL0_INTEN4 &= ~(0x1 << 4);
#endif
#endif
}
void intr0_gpio_b5(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[69]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b5 mask\n");
	ICTL0_INTMASK4 |= 0x1 << 5;
#else
	printf("intr0_gpio_b5 disable\n");
	ICTL0_INTEN4 &= ~(0x1 << 5);
#endif
#endif
}
void intr0_gpio_b6(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[70]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b6 mask\n");
	ICTL0_INTMASK4 |= 0x1 << 6;
#else
	printf("intr0_gpio_b6 disable\n");
	ICTL0_INTEN4 &= ~(0x1 << 6);
#endif
#endif
}
void intr0_gpio_b7(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[71]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b7 mask\n");
	ICTL0_INTMASK4 |= 0x1 << 7;
#else
	printf("intr0_gpio_b7 disable\n");
	ICTL0_INTEN4 &= ~(0x1 << 7);
#endif
#endif
}
void intr0_gpio_b8(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[72]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b8 mask\n");
	ICTL0_INTMASK5 |= 0x1 << 0;
#else
	printf("intr0_gpio_b8 disable\n");
	ICTL0_INTEN5 &= ~(0x1 << 0);
#endif
#endif
}
void intr0_gpio_b9(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[73]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b9 mask\n");
	ICTL0_INTMASK5 |= 0x1 << 1;
#else
	printf("intr0_gpio_b9 disable\n");
	ICTL0_INTEN5 &= ~(0x1 << 1);
#endif
#endif
}
void intr0_gpio_b10(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[74]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b10 mask\n");
	ICTL0_INTMASK5 |= 0x1 << 2;
#else
	printf("intr0_gpio_b10 disable\n");
	ICTL0_INTEN5 &= ~(0x1 << 2);
#endif
#endif
}
void intr0_gpio_b11(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[75]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b11 mask\n");
	ICTL0_INTMASK5 |= 0x1 << 3;
#else
	printf("intr0_gpio_b11 disable\n");
	ICTL0_INTEN5 &= ~(0x1 << 3);
#endif
#endif
}
void intr0_gpio_b12(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[76]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b12 mask\n");
	ICTL0_INTMASK5 |= 0x1 << 4;
#else
	printf("intr0_gpio_b12 disable\n");
	ICTL0_INTEN5 &= ~(0x1 << 4);
#endif
#endif
}
void intr0_gpio_b13(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[77]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b13 mask\n");
	ICTL0_INTMASK5 |= 0x1 << 5;
#else
	printf("intr0_gpio_b13 disable\n");
	ICTL0_INTEN5 &= ~(0x1 << 5);
#endif
#endif
}
void intr0_gpio_b14(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[78]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b14 mask\n");
	ICTL0_INTMASK5 |= 0x1 << 6;
#else
	printf("intr0_gpio_b14 disable\n");
	ICTL0_INTEN5 &= ~(0x1 << 6);
#endif
#endif
}
void intr0_gpio_b15(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[79]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b15 mask\n");
	ICTL0_INTMASK5 |= 0x1 << 7;
#else
	printf("intr0_gpio_b15 disable\n");
	ICTL0_INTEN5 &= ~(0x1 << 7);
#endif
#endif
}
void intr0_gpio_b16(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[80]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b16 mask\n");
	ICTL0_INTMASK6 |= 0x1 << 0;
#else
	printf("intr0_gpio_b16 disable\n");
	ICTL0_INTEN6 &= ~(0x1 << 0);
#endif
#endif
}
void intr0_gpio_b17(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[81]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b17 mask\n");
	ICTL0_INTMASK6 |= 0x1 << 1;
#else
	printf("intr0_gpio_b17 disable\n");
	ICTL0_INTEN6 &= ~(0x1 << 1);
#endif
#endif
}
void intr0_gpio_b18(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[82]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b18 mask\n");
	ICTL0_INTMASK6 |= 0x1 << 2;
#else
	printf("intr0_gpio_b18 disable\n");
	ICTL0_INTEN6 &= ~(0x1 << 2);
#endif
#endif
}
void intr0_gpio_b19(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[83]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b19 mask\n");
	ICTL0_INTMASK6 |= 0x1 << 3;
#else
	printf("intr0_gpio_b19 disable\n");
	ICTL0_INTEN6 &= ~(0x1 << 3);
#endif
#endif
}
void intr0_gpio_b20(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[84]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b20 mask\n");
	ICTL0_INTMASK6 |= 0x1 << 4;
#else
	printf("intr0_gpio_b20 disable\n");
	ICTL0_INTEN6 &= ~(0x1 << 4);
#endif
#endif
}
void intr0_gpio_b21(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[85]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b21 mask\n");
	ICTL0_INTMASK6 |= 0x1 << 5;
#else
	printf("intr0_gpio_b21 disable\n");
	ICTL0_INTEN6 &= ~(0x1 << 5);
#endif
#endif
}
void intr0_gpio_b22(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[86]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b22 mask\n");
	ICTL0_INTMASK6 |= 0x1 << 6;
#else
	printf("intr0_gpio_b22 disable\n");
	ICTL0_INTEN6 &= ~(0x1 << 6);
#endif
#endif
}
void intr0_gpio_b23(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[87]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b23 mask\n");
	ICTL0_INTMASK6 |= 0x1 << 7;
#else
	printf("intr0_gpio_b23 disable\n");
	ICTL0_INTEN6 &= ~(0x1 << 7);
#endif
#endif
}
void intr0_gpio_b24(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[88]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b24 mask\n");
	ICTL0_INTMASK7 |= 0x1 << 0;
#else
	printf("intr0_gpio_b24 disable\n");
	ICTL0_INTEN7 &= ~(0x1 << 0);
#endif
#endif
}
void intr0_gpio_b25(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[89]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b25 mask\n");
	ICTL0_INTMASK7 |= 0x1 << 1;
#else
	printf("intr0_gpio_b25 disable\n");
	ICTL0_INTEN7 &= ~(0x1 << 1);
#endif
#endif
}
void intr0_gpio_b26(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[90]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b26 mask\n");
	ICTL0_INTMASK7 |= 0x1 << 2;
#else
	printf("intr0_gpio_b26 disable\n");
	ICTL0_INTEN7 &= ~(0x1 << 2);
#endif
#endif
}
#ifdef AE103
void intr0_gpio_b27(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[91]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b27 mask\n");
	ICTL0_INTMASK7 |= 0x1 << 3;
#else
	printf("intr0_gpio_b27 disable\n");
	ICTL0_INTEN7 &= ~(0x1 << 3);
#endif
#endif
}
void intr0_gpio_b28(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[92]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b28 mask\n");
	ICTL0_INTMASK7 |= 0x1 << 4;
#else
	printf("intr0_gpio_b28 disable\n");
	ICTL0_INTEN7 &= ~(0x1 << 4);
#endif
#endif
}
void intr0_gpio_b29(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[93]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b29 mask\n");
	ICTL0_INTMASK7 |= 0x1 << 5;
#else
	printf("intr0_gpio_b29 disable\n");
	ICTL0_INTEN7 &= ~(0x1 << 5);
#endif
#endif
}
void intr0_gpio_b30(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[94]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b30 mask\n");
	ICTL0_INTMASK7 |= 0x1 << 6;
#else
	printf("intr0_gpio_b30 disable\n");
	ICTL0_INTEN7 &= ~(0x1 << 6);
#endif
#endif
}
void intr0_gpio_b31(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[95]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr0_gpio_b31 mask\n");
	ICTL0_INTMASK7 |= 0x1 << 7;
#else
	printf("intr0_gpio_b31 disable\n");
	ICTL0_INTEN7 &= ~(0x1 << 7);
#endif
#endif
}
#endif
void intr1_gpio_c0(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[91]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[96]++;
#endif
#endif

#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_c0 mask\n");
	ICTL1_INTMASK0 |= 0x1 << 0;
#else
	printf("intr1_gpio_c0 disable\n");
	ICTL1_INTEN0 &= ~(0x1 << 0);
#endif
#endif
}
void intr1_gpio_c1(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[92]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_c1 mask\n");
	ICTL1_INTMASK0 |= 0x1 << 1;
#else
	printf("intr1_gpio_c1 disable\n");
	ICTL1_INTEN0 &= ~(0x1 << 1);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[97]++;
#endif
#endif
}
void intr1_gpio_c2(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[93]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_c2 mask\n");
	ICTL1_INTMASK0 |= 0x1 << 2;
#else
	printf("intr1_gpio_c2 disable\n");
	ICTL1_INTEN0 &= ~(0x1 << 2);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[98]++;
#endif
#endif
}
void intr1_gpio_c3(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[94]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_c3 mask\n");
	ICTL1_INTMASK0 |= 0x1 << 3;
#else
	printf("intr1_gpio_c3 disable\n");
	ICTL1_INTEN0 &= ~(0x1 << 3);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[99]++;
#endif
#endif
}
void intr1_gpio_c4(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[95]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_c4 mask\n");
	ICTL1_INTMASK0 |= 0x1 << 4;
#else
	printf("intr1_gpio_c4 disable\n");
	ICTL1_INTEN0 &= ~(0x1 << 4);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[100]++;
#endif
#endif
}
void intr1_gpio_c5(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[96]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_c5 mask\n");
	ICTL1_INTMASK0 |= 0x1 << 5;
#else
	printf("intr1_gpio_c5 disable\n");
	ICTL1_INTEN0 &= ~(0x1 << 5);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[101]++;
#endif
#endif
}
void intr1_gpio_c6(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[97]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_c6 mask\n");
	ICTL1_INTMASK0 |= 0x1 << 6;
#else
	printf("intr1_gpio_c6 disable\n");
	ICTL1_INTEN0 &= ~(0x1 << 6);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[102]++;
#endif
#endif
}
void intr1_gpio_c7(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[98]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_c7 mask\n");
	ICTL1_INTMASK0 |= 0x1 << 7;
#else
	printf("intr1_gpio_c7 disable\n");
	ICTL1_INTEN0 &= ~(0x1 << 7);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[103]++;
#endif
#endif
}
void intr1_gpio_c8(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[99]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_c8 mask\n");
	ICTL1_INTMASK1 |= 0x1 << 0;
#else
	printf("intr1_gpio_c8 disable\n");
	ICTL1_INTEN1 &= ~(0x1 << 0);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[104]++;
#endif
#endif
}
void intr1_gpio_c9(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[100]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_c9 mask\n");
	ICTL1_INTMASK1 |= 0x1 << 1;
#else
	printf("intr1_gpio_c9 disable\n");
	ICTL1_INTEN1 &= ~(0x1 << 1);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[105]++;
#endif
#endif
}
void intr1_gpio_c10(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[101]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_c10 mask\n");
	ICTL1_INTMASK1 |= 0x1 << 2;
#else
	printf("intr1_gpio_c10 disable\n");
	ICTL1_INTEN1 &= ~(0x1 << 2);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[106]++;
#endif
#endif
}
void intr1_gpio_c11(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[102]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_c11 mask\n");
	ICTL1_INTMASK1 |= 0x1 << 3;
#else
	printf("intr1_gpio_c11 disable\n");
	ICTL1_INTEN1 &= ~(0x1 << 3);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[107]++;
#endif
#endif
}
void intr1_gpio_c12(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[103]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_c12 mask\n");
	ICTL1_INTMASK1 |= 0x1 << 4;
#else
	printf("intr1_gpio_c12 disable\n");
	ICTL1_INTEN1 &= ~(0x1 << 4);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[108]++;
#endif
#endif
}
void intr1_gpio_c13(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[104]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_c13 mask\n");
	ICTL1_INTMASK1 |= 0x1 << 5;
#else
	printf("intr1_gpio_c13 disable\n");
	ICTL1_INTEN1 &= ~(0x1 << 5);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[109]++;
#endif
#endif
}
#if (defined(AE103))
void intr1_gpio_c14(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[110]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_c14 mask\n");
	ICTL1_INTMASK1 |= 0x1 << 6;
#else
	printf("intr1_gpio_c14 disable\n");
	ICTL1_INTEN1 &= ~(0x1 << 6);
#endif
#endif
}
void intr1_gpio_c15(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[111]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_c15 mask\n");
	ICTL1_INTMASK1 |= 0x1 << 7;
#else
	printf("intr1_gpio_c15 disable\n");
	ICTL1_INTEN1 &= ~(0x1 << 7);
#endif
#endif
}
#endif
// void intr1_gpio_e0(void)
// {
// #if (defined(AE101) || defined(AE102))
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[105]++;
// #endif
// #endif
// #ifdef AE103
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[112]++;
// #endif
// #endif
// }
// void intr1_gpio_e1(void)
// {
// #if (defined(AE101) || defined(AE102))
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[106]++;
// #endif
// #endif
// #ifdef AE103
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[113]++;
// #endif
// #endif
// }
// void intr1_gpio_e2(void)
// {
// #if (defined(AE101) || defined(AE102))
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[107]++;
// #endif
// #endif
// #ifdef AE103
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[114]++;
// #endif
// #endif
// }
// void intr1_gpio_e3(void)
// {
// #if (defined(AE101) || defined(AE102))
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[108]++;
// #endif
// #endif
// #ifdef AE103
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[115]++;
// #endif
// #endif
// }
// void intr1_gpio_e4(void)
// {
// #if (defined(AE101) || defined(AE102))
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[109]++;
// #endif
// #endif
// #ifdef AE103
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[116]++;
// #endif
// #endif
// }
// void intr1_gpio_e5(void)
// {
// #if (defined(AE101) || defined(AE102))
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[110]++;
// #endif
// #endif
// #ifdef AE103
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[117]++;
// #endif
// #endif
// }
// void intr1_gpio_e6(void)
// {
// #if (defined(AE101) || defined(AE102))
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[111]++;
// #endif
// #endif
// #ifdef AE103
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[118]++;
// #endif
// #endif
// }
// void intr1_gpio_e7(void)
// {
// #if (defined(AE101) || defined(AE102))
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[112]++;
// #endif
// #endif
// #ifdef AE103
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[119]++;
// #endif
// #endif
// }
// void intr1_gpio_e8(void)
// {
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[113]++;
// #endif
// }
// void intr1_gpio_e9(void)
// {
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[114]++;
// #endif
// }
// void intr1_gpio_e10(void)
// {
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[115]++;
// #endif
// }
// void intr1_gpio_e11(void)
// {
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[116]++;
// #endif
// }
// void intr1_gpio_e12(void)
// {
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[117]++;
// #endif
// }
// void intr1_gpio_e13(void)
// {
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[118]++;
// #endif
// }
// void intr1_gpio_e14(void)
// {
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[119]++;
// #endif
// }
// void intr1_gpio_e15(void)
// {
// #if ENABLE_DEBUGGER_SUPPORT
// 	Intr_num[120]++;
// #endif
// }
void intr1_gpio_e16(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[121]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_e16 mask\n");
	ICTL1_INTMASK2 |= 0x1 << 0;
#else
	printf("intr1_gpio_e16 disable\n");
	ICTL1_INTEN2 &= ~(0x1 << 0);
#endif
#endif
}
void intr1_gpio_e17(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[122]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_e17 mask\n");
	ICTL1_INTMASK2 |= 0x1 << 1;
#else
	printf("intr1_gpio_e17 disable\n");
	ICTL1_INTEN2 &= ~(0x1 << 1);
#endif
#endif
}
void intr1_gpio_e18(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[123]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_e18 mask\n");
	ICTL1_INTMASK2 |= 0x1 << 2;
#else
	printf("intr1_gpio_e18 disable\n");
	ICTL1_INTEN2 &= ~(0x1 << 2);
#endif
#endif
}
void intr1_gpio_e19(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[124]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_e19 mask\n");
	ICTL1_INTMASK2 |= 0x1 << 3;
#else
	printf("intr1_gpio_e19 disable\n");
	ICTL1_INTEN2 &= ~(0x1 << 3);
#endif
#endif
}
void intr1_gpio_e20(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[125]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_e20 mask\n");
	ICTL1_INTMASK2 |= 0x1 << 4;
#else
	printf("intr1_gpio_e20 disable\n");
	ICTL1_INTEN2 &= ~(0x1 << 4);
#endif
#endif
}
void intr1_gpio_e21(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[126]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_e21 mask\n");
	ICTL1_INTMASK2 |= 0x1 << 5;
#else
	printf("intr1_gpio_e21 disable\n");
	ICTL1_INTEN2 &= ~(0x1 << 5);
#endif
#endif
}
void intr1_gpio_e22(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[127]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_e22 mask\n");
	ICTL1_INTMASK2 |= 0x1 << 6;
#else
	printf("intr1_gpio_e22 disable\n");
	ICTL1_INTEN2 &= ~(0x1 << 6);
#endif
#endif
}
void intr1_gpio_e23(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[128]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_gpio_e23 mask\n");
	ICTL1_INTMASK2 |= 0x1 << 7;
#else
	printf("intr1_gpio_e23 disable\n");
	ICTL1_INTEN2 &= ~(0x1 << 7);
#endif
#endif
}

void intr1_dma(void) // 24
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[120]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_dma mask\n");
	ICTL1_INTMASK3 |= 0x1 << 0;
#else
	printf("intr1_dma disable\n");
	ICTL1_INTEN3 &= ~(0x1 << 0);
#endif
#endif
}

void intr1_cec0(void) // 25
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[121]++;
#endif

#if TEST_INTC
#if INTC_MODE
	printf("intr1_cec0 mask\n");
	ICTL1_INTMASK3 |= 0x1 << 1;
#else
	printf("intr1_cec0 disable\n");
	ICTL1_INTEN3 &= ~(0x1 << 1);
#endif
#endif

	DWORD stat, int_stat;
	stat = CEC_Read(CEC0_SR_OFFSET);
	if((stat & 0xF0) != 0)
	{
		irqprint("CEC statu : %#x\n", stat);
		irqprint("statu erro ! \n");
		CEC_Write(0, CEC0_CTRL_OFFSET); // disable the CEC
	}
	int_stat = CEC_Read(CEC0_ISR_OFFSET);
#if CEC_mode_select // initiator
	if(int_stat & CEC_intStatue_sbis)
	{
	}
	if(int_stat & CEC_intStatue_sfis)
	{
	}
#else // follower
	if(int_stat & CEC_intStatue_rhis)
	{
	}
	if(int_stat & CEC_intStatue_rbis)
	{
	}
	if(int_stat & CEC_intStatue_rfis)
	{
	}
#endif
	CEC_Write(int_stat, CEC0_ISR_OFFSET); // clear the CEC_INT
}

void intr1_cec1(void) // 26
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[122]++;
#endif

#if TEST_INTC
#if INTC_MODE
	printf("intr1_cec1 mask\n");
	ICTL1_INTMASK3 |= 0x1 << 2;
#else
	printf("intr1_cec1 disable\n");
	ICTL1_INTEN3 &= ~(0x1 << 2);
#endif
#endif

	DWORD stat, int_stat;
	stat = CEC_Read(CEC1_SR_OFFSET);
	if((stat & 0xF0) != 0)
	{
		irqprint("CEC statu : %#x\n", stat);
		irqprint("statu erro ! \n");
		CEC_Write(0, CEC1_CTRL_OFFSET); // disable the CEC
	}
	int_stat = CEC_Read(CEC1_ISR_OFFSET);
#if CEC_mode_select // initiator
	if(int_stat & CEC_intStatue_sbis)
	{
	}
	if(int_stat & CEC_intStatue_sfis)
	{
	}
#else // follower
	if(int_stat & CEC_intStatue_rhis)
	{
	}
	if(int_stat & CEC_intStatue_rbis)
	{
	}
	if(int_stat & CEC_intStatue_rfis)
	{
	}
#endif
	CEC_Write(int_stat, CEC1_ISR_OFFSET); // clear the CEC_INT
}

void intr1_smbus4(void) // 27
{
#if TEST_INTC
#if INTC_MODE
	printf("intr1_smbus4 mask\n");
	ICTL1_INTMASK3 |= 0x1 << 3;
#else
	printf("intr1_smbus4 disable\n");
	ICTL1_INTEN3 &= ~(0x1 << 3);
#endif
#endif

	BYTE abrt_source_l = I2c_Readb(I2C_TX_ABRT_SOURCE_OFFSET, I2C_CHANNEL_4);
	BYTE abrt_source_h = I2c_Readb((I2C_TX_ABRT_SOURCE_OFFSET + 1), I2C_CHANNEL_4);
	BYTE raw_int = I2c_Readb(I2C_RAW_INTR_STAT_OFFSET, I2C_CHANNEL_4);
	if(raw_int & I2C_INTR_TX_ABRT)
	{
		dprint("smbus4 tx_abrt! addr is 0x%X data is 0x%X\n", SMB_Temp_Addr, SMB_Temp_Data);
		dprint("tx_abrt_source:0x%x  \n", (WORD)(abrt_source_l | (abrt_source_h << 8)));
	}
	I2c_Readb(I2C_CLR_TX_ABRT_OFFSET, I2C_CHANNEL_4); // 清除中断
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[123]++;
	DWORD ic_stat, int_stat;
	ic_stat = I2c_Readb(I2C_STATUS_OFFSET, I2C_CHANNEL_4);
	int_stat = i2c_dw_read_clear_intrbits(I2C_CHANNEL_4); // clear interrupts
	irqprint("int stat is %d \n", int_stat);
	if(int_stat & I2C_INTR_TX_ABRT)
	{
		irqprint("iic send erro !!! \n");
		i2c_dw_tx_abrt(I2C_CHANNEL_4);
	}
	if(int_stat & I2C_INTR_RX_FULL) // start req judge (set flags)
	{
	#if DEBUGGER_I2C_CHANNEL == 4
		Debugger_Cmd_IRQ(I2c_Slave_Read_Byte(DEBUGGER_I2C_CHANNEL));
	#endif
	}
	if(int_stat & I2C_INTR_TX_EMPTY) // sFIFO almost empty
	{
	#if DEBUGGER_I2C_CHANNEL == 4
		Debugger_I2c_Send(DEBUGGER_I2C_CHANNEL);
		assert_print("iicFeedback %#x", iicFeedback);
		F_Service_Debugger_Send = 1;
	#endif
	}
	if(int_stat & I2C_INTR_RD_REQ) // read request only once
	{
	#if DEBUGGER_I2C_CHANNEL == 4
		F_Service_Debugger_Rrq = 1;
		I2C4_INTR_MASK &= ~0x20; // 屏蔽RD_REQ中断
	#endif
	}
	if(!(ic_stat & I2C_SLV_ACTIVITY))
	{
		// irqprint("the iic of slave is dead! \n");
		// I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
	}
	if(!(ic_stat & I2C_MST_ACTIVITY))
	{
		// irqprint("the iic of master is dead! \n");
		// I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
	}
#endif
}
void intr1_smbus5(void) // 28
{
#if TEST_INTC
#if INTC_MODE
	printf("intr1_smbus5 mask\n");
	ICTL1_INTMASK3 |= 0x1 << 4;
#else
	printf("intr1_smbus5 disable\n");
	ICTL1_INTEN3 &= ~(0x1 << 4);
#endif
#endif
	BYTE abrt_source_l = I2c_Readb(I2C_TX_ABRT_SOURCE_OFFSET, I2C_CHANNEL_5);
	BYTE abrt_source_h = I2c_Readb((I2C_TX_ABRT_SOURCE_OFFSET + 1), I2C_CHANNEL_5);
	BYTE raw_int = I2c_Readb(I2C_RAW_INTR_STAT_OFFSET, I2C_CHANNEL_5);
	if(raw_int & I2C_INTR_TX_ABRT)
	{
		dprint("smbus5 tx_abrt! addr is 0x%X data is 0x%X\n", SMB_Temp_Addr, SMB_Temp_Data);
		dprint("tx_abrt_source:0x%x  \n", (WORD)(abrt_source_l | (abrt_source_h << 8)));
	}
	I2c_Readb(I2C_CLR_TX_ABRT_OFFSET, I2C_CHANNEL_5); // 清除中断
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[124]++;

	DWORD ic_stat, int_stat;
	ic_stat = I2c_Readb(I2C_STATUS_OFFSET, I2C_CHANNEL_5);
	int_stat = i2c_dw_read_clear_intrbits(I2C_CHANNEL_5); // clear interrupts
	if(int_stat & I2C_INTR_TX_ABRT)
	{
		irqprint("iic send erro !!! \n");
		i2c_dw_tx_abrt(I2C_CHANNEL_5);
	}
	if(int_stat & I2C_INTR_RX_FULL) // start req judge (set flags)
	{
	#if DEBUGGER_I2C_CHANNEL == 5
		Debugger_Cmd_IRQ(I2c_Slave_Read_Byte(DEBUGGER_I2C_CHANNEL));
	#endif
	}
	if(int_stat & I2C_INTR_TX_EMPTY) // sFIFO almost empty
	{
	#if DEBUGGER_I2C_CHANNEL == 5
		Debugger_I2c_Send(DEBUGGER_I2C_CHANNEL);
		assert_print("iicFeedback %#x", iicFeedback);
		F_Service_Debugger_Send = 1;
	#endif
	}
	if(int_stat & I2C_INTR_RD_REQ) // read request only once
	{
	#if DEBUGGER_I2C_CHANNEL == 5
		F_Service_Debugger_Rrq = 1;
		I2C5_INTR_MASK &= ~0x20; // 屏蔽RD_REQ中断
	#endif
	}
	if(!(ic_stat & I2C_SLV_ACTIVITY))
	{
		// irqprint("the iic of slave is dead! \n");
		// I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
	}
	if(!(ic_stat & I2C_MST_ACTIVITY))
	{
		// irqprint("the iic of master is dead! \n");
		// I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
	}
#endif
}

void intr1_owi(void) // 29
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[125]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_owi mask\n");
	ICTL1_INTMASK3 |= 0x1 << 5;
#else
	printf("intr1_owi disable\n");
	ICTL1_INTEN3 &= ~(0x1 << 5);
#endif
#endif
}

void intr1_null30(void) // 30
{}

void intr1_peci(void) // 31
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[127]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_peci mask\n");
	ICTL1_INTMASK3 |= 0x1 << 7;
#else
	printf("intr1_peci disable\n");
	ICTL1_INTEN3 &= ~(0x1 << 7);
#endif
#endif
}
void intr1_i3c0(void) // 32
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[128]++;
#endif

#if TEST_INTC
#if INTC_MODE
	printf("intr1_i3c0 mask\n");
	ICTL1_INTMASK4 |= 0x1 << 0;
#else
	printf("intr1_i3c0 disable\n");
	ICTL1_INTEN4 &= ~(0x1 << 0);
#endif
#endif

	irqprint("null 32\n");
}
void intr1_i3c1(void) // 33
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[129]++;
#endif

#if TEST_INTC
#if INTC_MODE
	printf("intr1_i3c1 mask\n");
	ICTL1_INTMASK4 |= 0x1 << 1;
#else
	printf("intr1_i3c1 disable\n");
	ICTL1_INTEN4 &= ~(0x1 << 1);
#endif
#endif

	irqprint("null 33\n");
}
void intr1_i3c2(void) // 34
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[130]++;
#endif

#if TEST_INTC
#if INTC_MODE
	printf("intr1_i3c2 mask\n");
	ICTL1_INTMASK4 |= 0x1 << 2;
#else
	printf("intr1_i3c2 disable\n");
	ICTL1_INTEN4 &= ~(0x1 << 2);
#endif
#endif

	irqprint("null 34\n");
}
void intr1_i3c3(void) // 35
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[131]++;
#endif

#if TEST_INTC
#if INTC_MODE
	printf("intr1_i3c3 mask\n");
	ICTL1_INTMASK4 |= 0x1 << 3;
#else
	printf("intr1_i3c3 disable\n");
	ICTL1_INTEN4 &= ~(0x1 << 3);
#endif
#endif

	irqprint("null 35\n");
}
void intr1_null36(void) // 36
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[132]++;
#endif
	irqprint("null 36\n");
}
void intr1_null37(void) // 37
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[133]++;
#endif
	irqprint("null 37\n");
}
void intr1_pmc2_ibf_ec(void) // 38
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[134]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_pmc2_ibf_ec mask\n");
	ICTL1_INTMASK4 |= 0x1 << 6;
#else
	printf("intr1_pmc2_ibf_ec disable\n");
	ICTL1_INTEN4 &= ~(0x1 << 6);
#endif
#endif
	PMC2_CTL &= ~IBF_INT_ENABLE; // 清除中断，读走以后再打开
	F_Service_PCI3 = 1;
}
void intr1_pmc2_obe_ec(void) // 39
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[135]++;
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_pmc2_obe_ec mask\n");
	ICTL1_INTMASK4 |= 0x1 << 7;
#else
	printf("intr1_pmc2_obe_ec disable\n");
	ICTL1_INTEN4 &= ~(0x1 << 7);
#endif
#endif
}

void intr1_pmc3_ibf_ec(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[129]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_pmc3_ibf_ec mask\n");
	ICTL1_INTMASK5 |= 0x1 << 0;
#else
	printf("intr1_pmc3_ibf_ec disable\n");
	ICTL1_INTEN5 &= ~(0x1 << 0);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[136]++;
#endif
#endif
	PMC3_CTL &= ~IBF_INT_ENABLE;
	F_Service_PCI4 = 1;
}
void intr1_pmc3_obe_ec(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[130]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_pmc3_obe_ec mask\n");
	ICTL1_INTMASK5 |= 0x1 << 1;
#else
	printf("intr1_pmc3_obe_ec disable\n");
	ICTL1_INTEN5 &= ~(0x1 << 1);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[137]++;
#endif
#endif
}
void intr1_pmc4_ibf_ec(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[131]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_pmc4_ibf_ec mask\n");
	ICTL1_INTMASK5 |= 0x1 << 2;
#else
	printf("intr1_pmc4_ibf_ec disable\n");
	ICTL1_INTEN5 &= ~(0x1 << 2);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[138]++;
#endif
#endif
	PMC4_CTL &= ~IBF_INT_ENABLE;
	F_Service_PCI5 = 1;
}
void intr1_pmc4_obe_ec(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[132]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_pmc4_ibf_ec mask\n");
	ICTL1_INTMASK5 |= 0x1 << 3;
#else
	printf("intr1_pmc4_ibf_ec disable\n");
	ICTL1_INTEN5 &= ~(0x1 << 3);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[139]++;
#endif
#endif
}
void intr1_pmc5_ibf_ec(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[133]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_pmc5_ibf_ec mask\n");
	ICTL1_INTMASK5 |= 0x1 << 4;
#else
	printf("intr1_pmc5_ibf_ec disable\n");
	ICTL1_INTEN5 &= ~(0x1 << 4);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[140]++;
#endif
#endif
	PMC5_CTL &= ~IBF_INT_ENABLE;
	F_Service_PCI6 = 1;
}
void intr1_pmc5_obe_ec(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[134]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_pmc5_obe_ec mask\n");
	ICTL1_INTMASK5 |= 0x1 << 5;
#else
	printf("intr1_pmc5_obe_ec disable\n");
	ICTL1_INTEN5 &= ~(0x1 << 5);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[141]++;
#endif
#endif
}
void intr1_ps2_1(void) // 46
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[135]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_ps2_1 mask\n");
	ICTL1_INTMASK5 |= 0x1 << 6;
#else
	printf("intr1_ps2_1 disable\n");
	ICTL1_INTEN5 &= ~(0x1 << 6);
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[142]++;
#endif
#endif
	if(MS_Main_CHN == 2)
		Handle_Mouse(MS_Main_CHN - 1);
	else if(KB_Main_CHN == 2)
		Handle_Kbd(KB_Main_CHN - 1);
}
#if defined(AE101)
void intr1_null45(void) // 45 47
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[136]++;
#endif
	irqprint("null 45\n");
}
void intr1_null46(void) // 46 48
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[137]++;
#endif
	irqprint("null 46\n");
}
void intr1_null47(void) // 47 49
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[138]++;
#endif
	irqprint("null 47\n");
}
#elif (defined(AE102) || defined(AE103))
void intr1_uart1(void)
{
#if TEST_INTC
#if INTC_MODE
	printf("intr1_uart1 mask\n");
	ICTL1_INTMASK5 |= 0x1 << 7;
#else
	printf("intr1_uart1 disable\n");
	ICTL1_INTEN5 &= ~(0x1 << 7);
#endif
#endif

#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[136]++;
#endif
#endif
	register u8 iir = (UART1_IIR & 0xf);
	if(iir == UART_IIR_RLS) // 奇偶校验、溢出、格式错误、失效中断
	{
		register u8 lsr = UART1_LSR;
		if(lsr & (UART_LSR_BI | UART_LSR_FE | UART_LSR_PE)) // 打断传输,格式错误，奇偶校验，
		{
			UART1_RX;					 // 读出异常值
			irqprint("Receive error\n"); // 报错
		#if (defined(AE101) || defined(AE102))
			SYSCTL_PIO2_UDCFG |= BIT3;
		#endif
		#ifdef AE103
			SYSCTL_PIO1_UDCFG |= BIT3;
		#endif
		} // 接收错误
	}
#if (ENABLE_COMMAND_SUPPORT && COMMAND_UART_SWITCH == 1)
	if(F_Service_CMD == 1)
	{
		char temp = UART1_RX;
	#if (!IRQC_DEBUG)
		UNUSED_VAR(temp);
	#endif
		irqprint("erro of CMD_RUN:%#x,%c\n", temp, temp);
		return;
	}
	CMD_UART_BUFF[CMD_UART_CNT] = UART1_RX;
	if(CMD_UART_BUFF[CMD_UART_CNT] == '\n' || CMD_UART_CNT == CMD_BUFF_MAX)
	{
		CMD_UART_BUFF[CMD_UART_CNT] = '\0';
		if(CMD_UART_BUFF[CMD_UART_CNT - 1] == ' ')
		{
			CMD_UART_CNT--;
			CMD_UART_BUFF[CMD_UART_CNT] = '\0';
		}
		F_Service_CMD = 1;
	}
	else if(CMD_UART_BUFF[CMD_UART_CNT] == '\r')
		CMD_UART_BUFF[CMD_UART_CNT] = '\0';
	else
		CMD_UART_CNT++;
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[143]++;
#endif
#endif
#if ENABLE_DEBUGGER_SUPPORT
#if DEBUGGER_OUTPUT_SWITCH == 0
#if DEBUG_UART_SWITCH == 1
	char temp = UART1_RX;
	Uart_buffer[Uart_Rx_index++] = temp;
	Uart_Rx_index %= UART_BUFFER_SIZE;
	Debugger_Cmd_IRQ(temp);
#endif
#endif
#endif
}
void intr1_null48(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[144]++;
#endif
}
void intr1_null49(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[145]++;
#endif
}
#endif
void intr1_spim(void)
{
#if TEST_INTC
#if INTC_MODE
	printf("intr1_spim mask\n");
	ICTL1_INTMASK6 |= 0x1 << 2;
#else
	printf("intr1_spim disable\n");
	ICTL1_INTEN6 &= ~(0x1 << 2);
#endif
#endif
	uint8_t data = 0;
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[139]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[146]++;
#endif
#endif
	if((SPIM_ISR & 0X10)) // fifo接收满中断
	{
		if(SPI_Read_Start == 2) // 读命令完成准备读出
		{
			while(!(SPIM_SR & 0x4))
				;
			data = SPIM_DA0;
			if(data == 0xff)
			{
				// irqprint("data is %#x \n",data);
			}
			else
			{
				// irqprint("data is %#x \n", data);
				SPIM_DA0 = 0xff;
			}
		}
	}
	if(SPIM_ISR & 0X8) // fifo接收溢出中断
	{
		SPIM_ISR |= 0x8; // 清除中断
	}
	if(SPIM_ISR & 0x2) // 发送fifo满中断
	{
	}
	if((SPIM_ISR & 0x1) && (!(SPIM_IMSR & 0x1))) // 发送fifo空中断
	{
		SPI_Block_Erase(0x0, 0);					 // 擦除
		SPI_Page_Program(0, 256, SPI_Write_Buff, 0); // 写入
		SPI_IRQ_Config(txeim, DISABLE);
		SPI_Read_Start = 1; // 写入完成判断可以读
	}
}
void intr1_smbus0(void)
{
#if TEST_INTC
#if INTC_MODE
	printf("intr1_smbus0 mask\n");
	ICTL1_INTMASK6 |= 0x1 << 3;
#else
	printf("intr1_smbus0 disable\n");
	ICTL1_INTEN6 &= ~(0x1 << 3);
#endif
#endif
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[140]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[147]++;
#endif
#endif
	BYTE abrt_source_l = I2c_Readb(I2C_TX_ABRT_SOURCE_OFFSET, I2C_CHANNEL_0);
	BYTE abrt_source_h = I2c_Readb((I2C_TX_ABRT_SOURCE_OFFSET + 1), I2C_CHANNEL_0);
	BYTE raw_int = I2c_Readb(I2C_RAW_INTR_STAT_OFFSET, I2C_CHANNEL_0);
	if(raw_int & I2C_INTR_TX_ABRT)
	{
		dprint("smbus0 tx_abrt! addr is 0x%X data is 0x%X\n", SMB_Temp_Addr, SMB_Temp_Data);
		dprint("tx_abrt_source:0x%x  \n", (WORD)(abrt_source_l | (abrt_source_h << 8)));
	}
	I2c_Readb(I2C_CLR_TX_ABRT_OFFSET, I2C_CHANNEL_0); // 清除中断
#if 0
	if(I2c_Readb(I2C_RAW_INTR_STAT_OFFSET, I2C_CHANNEL_0) & 0x04)
	{
		printf("smbus0 read data:%#x\n", I2c_Slave_Read_Byte(0));
	}
	if(I2c_Readb(I2C_RAW_INTR_STAT_OFFSET, I2C_CHANNEL_0) & 0x20)
	{
		// I2c_Slave_Write_Byte(0xfe, 0);
		// I2c_Slave_Write_Byte(0xdd, 0);
		// printf("send ec\n");
		// printf("send ed\n");
		assert_print("iicFeedback %#x", iicFeedback);
		Debugger_I2c_Req(DEBUGGER_I2C_CHANNEL);
		assert_print("iicFeedback %#x", iicFeedback);
	}
#endif
#if ENABLE_DEBUGGER_SUPPORT
	DWORD int_stat, ic_stat;
	// BYTE mask = I2c_Readb(I2C_INTR_MASK_OFFSET,baseaddr);
	// BYTE raw_int = I2c_Readb(I2C_RAW_INTR_STAT_OFFSET, I2C_CHANNEL_0);
	ic_stat = I2c_Readb(I2C_STATUS_OFFSET, I2C_CHANNEL_0);
	int_stat = i2c_dw_read_clear_intrbits(I2C_CHANNEL_0); // clear interrupts
#if 1
	if(int_stat & I2C_INTR_RX_FULL) // start req judge (set flags)
	{
	#if DEBUGGER_I2C_CHANNEL == 0
		Debugger_Cmd_IRQ(I2c_Slave_Read_Byte(DEBUGGER_I2C_CHANNEL));
	#endif
	}
	if(int_stat & I2C_INTR_TX_EMPTY) // sFIFO almost empty
	{
	#if DEBUGGER_I2C_CHANNEL == 0
		Debugger_I2c_Send(DEBUGGER_I2C_CHANNEL);
		assert_print("iicFeedback %#x", iicFeedback);
		F_Service_Debugger_Send = 1;
	#endif
	}
	if(int_stat & I2C_INTR_RD_REQ) // read request only once
	{
	#if DEBUGGER_I2C_CHANNEL == 0
			// assert_print("iicFeedback %#x", iicFeedback);
			// Debugger_I2c_Req(DEBUGGER_I2C_CHANNEL);
			// assert_print("iicFeedback %#x", iicFeedback);
		F_Service_Debugger_Rrq = 1;
		I2C0_INTR_MASK &= ~0x20; // 屏蔽RD_REQ中断
	#endif
	}
	if(!(ic_stat & I2C_SLV_ACTIVITY))
	{
		// irqprint("the iic of slave is dead! \n");
		// I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
	}
	if(!(ic_stat & I2C_MST_ACTIVITY))
	{
		// irqprint("the iic of master is dead! \n");
		// I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
	}
	if(int_stat & I2C_INTR_TX_ABRT)
	{
		// irqprint("iic send erro !!! \n");
		// i2c_dw_tx_abrt(I2C_CHANNEL_0);
	}
#endif
#endif
}
void intr1_smbus1(void)
{
#if TEST_INTC
#if INTC_MODE
	printf("intr1_smbus1 mask\n");
	ICTL1_INTMASK6 |= 0x1 << 4;
#else
	printf("intr1_smbus1 disable\n");
	ICTL1_INTEN6 &= ~(0x1 << 4);
#endif
#endif
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[141]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[148]++;
#endif
#endif
	BYTE abrt_source_l = I2c_Readb(I2C_TX_ABRT_SOURCE_OFFSET, I2C_CHANNEL_1);
	BYTE abrt_source_h = I2c_Readb((I2C_TX_ABRT_SOURCE_OFFSET + 1), I2C_CHANNEL_1);
	BYTE raw_int = I2c_Readb(I2C_RAW_INTR_STAT_OFFSET, I2C_CHANNEL_1);
	if(raw_int & I2C_INTR_TX_ABRT)
	{
		dprint("smbus1 tx_abrt! addr is 0x%x data is 0x%x\n", SMB_Temp_Addr, SMB_Temp_Data);
		dprint("tx_abrt_source:0x%x  \n", (WORD)(abrt_source_l | (abrt_source_h << 8)));
	}
	I2c_Readb(I2C_CLR_TX_ABRT_OFFSET, I2C_CHANNEL_1); // 清除中断
#if ENABLE_DEBUGGER_SUPPORT
	DWORD ic_stat, int_stat;
	ic_stat = I2c_Readb(I2C_STATUS_OFFSET, I2C_CHANNEL_1);
	int_stat = i2c_dw_read_clear_intrbits(I2C_CHANNEL_1); // clear interrupts
	if(int_stat & I2C_INTR_TX_ABRT)
	{
		irqprint("iic action erro !!! \n");
		i2c_dw_tx_abrt(I2C_CHANNEL_1);
		I2c_Writeb(0, I2C_INTR_MASK_OFFSET, I2C_CHANNEL_1);
	}
	if(int_stat & I2C_INTR_RX_FULL) // start req judge (set flags)
	{
	#if DEBUGGER_I2C_CHANNEL == 1
		Debugger_Cmd_IRQ(I2c_Slave_Read_Byte(DEBUGGER_I2C_CHANNEL));
	#endif
	}
	if(int_stat & I2C_INTR_TX_EMPTY) // sFIFO almost empty
	{
	#if DEBUGGER_I2C_CHANNEL == 1
		Debugger_I2c_Send(DEBUGGER_I2C_CHANNEL);
		assert_print("iicFeedback %#x", iicFeedback);
		F_Service_Debugger_Send = 1;
	#endif
	}
	if(int_stat & I2C_INTR_RD_REQ) // read request only once
	{
	#if DEBUGGER_I2C_CHANNEL == 1
		F_Service_Debugger_Rrq = 1;
		I2C1_INTR_MASK &= ~0x20; // 屏蔽RD_REQ中断
	#endif
	}
	if(!(ic_stat & I2C_SLV_ACTIVITY))
	{
		// irqprint("the iic of slave is dead! \n");
		// I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
	}
	if(!(ic_stat & I2C_MST_ACTIVITY))
	{
		// irqprint("the iic of master is dead! \n");
		// I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
	}
#endif
}
void intr1_smbus2(void)
{
#if TEST_INTC
#if INTC_MODE
	printf("intr1_smbus2 mask\n");
	ICTL1_INTMASK6 |= 0x1 << 5;
#else
	printf("intr1_smbus2 disable\n");
	ICTL1_INTEN6 &= ~(0x1 << 5);
#endif
#endif
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[142]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[149]++;
#endif
#endif
	BYTE abrt_source_l = I2c_Readb(I2C_TX_ABRT_SOURCE_OFFSET, I2C_CHANNEL_2);
	BYTE abrt_source_h = I2c_Readb((I2C_TX_ABRT_SOURCE_OFFSET + 1), I2C_CHANNEL_2);
	BYTE raw_int = I2c_Readb(I2C_RAW_INTR_STAT_OFFSET, I2C_CHANNEL_2);
	if(raw_int & I2C_INTR_TX_ABRT)
	{
		dprint("smbus2 tx_abrt! addr is 0x%X data is 0x%X\n", SMB_Temp_Addr, SMB_Temp_Data);
		dprint("tx_abrt_source:0x%x  \n", (WORD)(abrt_source_l | (abrt_source_h << 8)));
	}
	I2c_Readb(I2C_CLR_TX_ABRT_OFFSET, I2C_CHANNEL_2); // 清除中断
#if ENABLE_DEBUGGER_SUPPORT
	DWORD ic_stat, int_stat;
	ic_stat = I2c_Readb(I2C_STATUS_OFFSET, I2C_CHANNEL_2);
	int_stat = i2c_dw_read_clear_intrbits(I2C_CHANNEL_2); // clear interrupts
	if(int_stat & I2C_INTR_TX_ABRT)
	{
		irqprint("iic action erro !!! \n");
		i2c_dw_tx_abrt(I2C_CHANNEL_2);
	}
	if(int_stat & I2C_INTR_RX_FULL) // start req judge (set flags)
	{
	#if DEBUGGER_I2C_CHANNEL == 2
		Debugger_Cmd_IRQ(I2c_Slave_Read_Byte(DEBUGGER_I2C_CHANNEL));
	#endif
	}
	if(int_stat & I2C_INTR_TX_EMPTY) // sFIFO almost empty
	{
	#if DEBUGGER_I2C_CHANNEL == 2
		Debugger_I2c_Send(DEBUGGER_I2C_CHANNEL);
		assert_print("iicFeedback %#x", iicFeedback);
		F_Service_Debugger_Send = 1;
	#endif
	}
	if(int_stat & I2C_INTR_RD_REQ) // read request only once
	{
	#if DEBUGGER_I2C_CHANNEL == 2
		F_Service_Debugger_Rrq = 1;
		I2C2_INTR_MASK &= ~0x20; // 屏蔽RD_REQ中断
	#endif
	}
	if(!(ic_stat & I2C_SLV_ACTIVITY))
	{
		// irqprint("the iic of slave is dead! \n");
		// I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
	}
	if(!(ic_stat & I2C_MST_ACTIVITY))
	{
		// irqprint("the iic of master is dead! \n");
		// I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
	}
#endif
}
void intr1_smbus3(void)
{
#if TEST_INTC
#if INTC_MODE
	printf("intr1_smbus3 mask\n");
	ICTL1_INTMASK6 |= 0x1 << 6;
#else
	printf("intr1_smbus3 disable\n");
	ICTL1_INTEN6 &= ~(0x1 << 6);
#endif
#endif
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[143]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[150]++;
#endif
#endif
	BYTE abrt_source_l = I2c_Readb(I2C_TX_ABRT_SOURCE_OFFSET, I2C_CHANNEL_3);
	BYTE abrt_source_h = I2c_Readb((I2C_TX_ABRT_SOURCE_OFFSET + 1), I2C_CHANNEL_3);
	BYTE raw_int = I2c_Readb(I2C_RAW_INTR_STAT_OFFSET, I2C_CHANNEL_3);
	if(raw_int & I2C_INTR_TX_ABRT)
	{
		dprint("smbus3 tx_abrt! addr is 0x%X data is 0x%X\n", SMB_Temp_Addr, SMB_Temp_Data);
		dprint("tx_abrt_source:0x%x  \n", (WORD)(abrt_source_l | (abrt_source_h << 8)));
	}
	I2c_Readb(I2C_CLR_TX_ABRT_OFFSET, I2C_CHANNEL_3); // 清除中断
#if ENABLE_DEBUGGER_SUPPORT
	DWORD ic_stat, int_stat;
	ic_stat = I2c_Readb(I2C_STATUS_OFFSET, I2C_CHANNEL_3);
	int_stat = i2c_dw_read_clear_intrbits(I2C_CHANNEL_3); // clear interrupts
	if(int_stat & I2C_INTR_TX_ABRT)
	{
		irqprint("iic action erro !!! \n");
		i2c_dw_tx_abrt(I2C_CHANNEL_3);
	}
	if(int_stat & I2C_INTR_RX_FULL) // start req judge (set flags)
	{
	#if DEBUGGER_I2C_CHANNEL == 3
		Debugger_Cmd_IRQ(I2c_Slave_Read_Byte(DEBUGGER_I2C_CHANNEL));
	#endif
	}
	if(int_stat & I2C_INTR_TX_EMPTY) // sFIFO almost empty
	{
	#if DEBUGGER_I2C_CHANNEL == 3
		Debugger_I2c_Send(DEBUGGER_I2C_CHANNEL);
		assert_print("iicFeedback %#x", iicFeedback);
		F_Service_Debugger_Send = 1;
	#endif
	}
	if(int_stat & I2C_INTR_RD_REQ) // read request only once
	{
	#if DEBUGGER_I2C_CHANNEL == 3
		F_Service_Debugger_Rrq = 1;
		I2C2_INTR_MASK &= ~0x20; // 屏蔽RD_REQ中断
	#endif
	}
	if(!(ic_stat & I2C_SLV_ACTIVITY))
	{
		// irqprint("the iic of slave is dead! \n");
		// I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
	}
	if(!(ic_stat & I2C_MST_ACTIVITY))
	{
		// irqprint("the iic of master is dead! \n");
		// I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
	}
#endif
}
#if defined(AE101)
void intr1_null53(void) // 53 55
{}
void intr1_null54(void) // 54 56
{}
void intr1_null55(void) // 55 57
{}
void intr1_null56(void) // 56 58
{}
void intr1_null57(void) // 57 59
{}
#elif (defined(AE102) || defined(AE103))
void intr1_smbus6(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[144]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[151]++;
#endif
#endif

#if TEST_INTC
#if INTC_MODE
	printf("intr1_smbus6 mask\n");
	ICTL1_INTMASK6 |= 0x1 << 7;
#else
	printf("intr1_smbus6 disable\n");
	ICTL1_INTEN6 &= ~(0x1 << 7);
#endif
#endif
}
void intr1_smbus7(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[145]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[152]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_smbus7 mask\n");
	ICTL1_INTMASK7 |= 0x1 << 0;
#else
	printf("intr1_smbus7 disable\n");
	ICTL1_INTEN7 &= ~(0x1 << 0);
#endif
#endif
}
void intr1_smbus8(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[146]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[153]++;
#endif
#endif
#if TEST_INTC
#if INTC_MODE
	printf("intr1_smbus8 mask\n");
	ICTL1_INTMASK7 |= 0x1 << 1;
#else
	printf("intr1_smbus8 disable\n");
	ICTL1_INTEN7 &= ~(0x1 << 1);
#endif
#endif
}
void intr1_null58(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[147]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[154]++;
#endif
#endif
}
void intr1_null59(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[148]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[155]++;
#endif
#endif
}
#endif
void intr1_por_int(void)
{
#if TEST_INTC
#if INTC_MODE
	printf("intr1_por_int mask\n");
	ICTL1_INTMASK7 |= 0x1 << 4;
#else
	printf("intr1_por_int disable\n");
	ICTL1_INTEN7 &= ~(0x1 << 4);
#endif
#endif
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[149]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[156]++;
#endif
#endif
}
#if ((defined(AE103)) && (GLE01 == 1))
void intr1_mailbox(void)
{
#if TEST_INTC
#if INTC_MODE
	printf("i3c_master_int mask\n");
	ICTL1_INTMASK7 |= 0x1 << 5;
#else
	printf("i3c_master_int disable\n");
	ICTL1_INTEN7 &= ~(0x1 << 5);
#endif
#endif
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[150]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[157]++;
#endif
#endif
	// printf("c2e int\n");
	Mailbox_Int_Store = C2EINT;
	C2E_CMD = C2EINFO0;
	do
	{
		C2EINT = C2EINT;
		nop;
	}
	while(C2EINT); // 清除中断
	F_Service_Mailbox = 1;
}
#else
void intr1_null61(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[150]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[157]++;
#endif
#endif
}
#endif
#ifdef AE103
void intr1_espi(void)
{
#if TEST_INTC
#if INTC_MODE
	printf("intr1_espi mask\n");
	ICTL1_INTMASK7 |= 0x1 << 6;
#else
	printf("intr1_espi disable\n");
	ICTL1_INTEN7 &= ~(0x1 << 6);
#endif
#endif
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[158]++;
#endif

#ifdef ENABLE_eSPI_INT_QUICK_CTRL
	/* Quick PWRGD_LO Control */
	if(IS_eSPI_SLP_S3_LO() && IS_eSPI_SLP_S3_VALID())
	{
		// ALL_SYS_PWRGD_LO();
		// VCCST_PWRGD_OUT_EC_LO();
	}
#endif
	eSPI_BUF_VWCTRL1 |= VWCTRL1;
#if SUPPORT_HOOK_WARMBOOT
	/* Process Warmboot Tag only */
	if(VWCTRL1 & F_VWIDX3_UPDATED)
	{
		if(VWIDX3 & F_IDX3_PLTRST_VALID)
		{
			if(VWIDX3 & F_IDX3_PLTRST)
			{
				/* VWire PLTRST Status - Hi */
				if(eSPI_PLTRST_TAG == F_PLTRST_HI_TO_LO)
				{
					/* 3: Detected Lo to Hi */
					eSPI_PLTRST_TAG = F_PLTRST_LO_TO_HI;
				}
				else
				{
					/* 1: Detected Hi */
					eSPI_PLTRST_TAG = F_PLTRST_HI_LEVEL;
				}
			}
			else
			{
				/* VWire PLTRST Status - Lo */
				if(eSPI_PLTRST_TAG == F_PLTRST_HI_LEVEL)
				{
					/* 2: Detected Hi To Lo */
					eSPI_PLTRST_TAG = F_PLTRST_HI_TO_LO;
				}
			}
		}
	}
#endif
	VWCTRL1 = 0xFF;
	/*OOB-ERPMC Interrupt*/
	if(REG32(0x330C0) && eRPMCSTS)
	{
		TaskParams params;
		REG32(0x330C0) |= eRPMCSTS;
		RPMC_OOB_TempArr[14] = 0xAA;//set default
		if(eSPI_OOBReceive(RPMC_OOB_TempArr))
		{
			if(RPMC_OOB_TempArr[2] == 0) // receive message length is 0, means no message
			{
				//异常处理
				return;
			}
		}
		switch(RPMC_OOB_TempArr[14]) // cmd type
		{
			case 0x0:                            // WriteRootKey
				if(RPMC_OOB_TempArr[2] == 0x48) // WriteRootKey message1
				{
					memcpy(&eRPMC_WriteRootKey_m1, RPMC_OOB_TempArr, sizeof(eRPMC_WriteRootKey_m1));
				}
				else if(RPMC_OOB_TempArr[2] == 0x0B) // WriteRootKey message2
				{
					memcpy(&eRPMC_WriteRootKey_m2, RPMC_OOB_TempArr, sizeof(eRPMC_WriteRootKey_m2));
					/*mailbox WriteRootKey trigger*/
					task_head = Add_Task((TaskFunction)Mailbox_WriteRootKey_Trigger, params, &task_head);
				}
				else//payload length error
				{
					eRPMC_WriteRootKey_data.Extended_Status = 0x04;
					eSPI_OOBSend((BYTE *)&eRPMC_WriteRootKey_data);
				}
				break;
			case 0x1:                            // UpdateHMACKey
				if(RPMC_OOB_TempArr[2] == 0x32) // UpdateHMACKey message
				{
					memcpy(&eRPMC_UpdateHMACKey, RPMC_OOB_TempArr, sizeof(eRPMC_UpdateHMACKey));
					task_head = Add_Task((TaskFunction)Mailbox_UpdateHMACKey_Trigger, params, &task_head);
				}
				else//payload length error
				{
					eRPMC_UpdateHMACKey_data.Extended_Status = 0x04;
					eSPI_OOBSend((BYTE *)&eRPMC_UpdateHMACKey_data);
				}
				break;
			case 0x2:                            // IncrementCounter
				if(RPMC_OOB_TempArr[2] == 0x32) // IncrementCounter message
				{
					memcpy(&eRPMC_IncrementCounter, RPMC_OOB_TempArr, sizeof(eRPMC_IncrementCounter));
					task_head = Add_Task((TaskFunction)Mailbox_IncrementCounter_Trigger, params, &task_head);
				}
				else//payload length error
				{
					eRPMC_IncrementCounter_data.Extended_Status = 0x04;
					eSPI_OOBSend((BYTE *)&eRPMC_IncrementCounter_data);
				}
				break;
			case 0x3:                            // RequestCounter
				if(RPMC_OOB_TempArr[2] == 0x3A) // RequestCounter message
				{
					memcpy(&eRPMC_RequestCounter, RPMC_OOB_TempArr, sizeof(eRPMC_RequestCounter));
					task_head = Add_Task((TaskFunction)Mailbox_RequestCounter_Trigger, params, &task_head);
				}
				else//payload length error
				{
					eRPMC_RequestCounter_data.Extended_Status = 0x04;
					eSPI_OOBSend((BYTE *)&eRPMC_RequestCounter_data);
				}
				break;
			default:
				break;
		}
		if(RPMC_OOB_TempArr[13] == 0x9F)// ReadParameters
		{
			if(RPMC_OOB_TempArr[2] == 0x0B)
			{
				memcpy(&eRPMC_ReadParameters, RPMC_OOB_TempArr, sizeof(eRPMC_ReadParameters));
				task_head = Add_Task((TaskFunction)Mailbox_ReadParameter_Trigger, params, &task_head);
			}
			else//payload length error
			{
				eRPMC_ReadParameters_data.Extended_Status = 0x04;
				eSPI_OOBSend((BYTE *)&eRPMC_ReadParameters_data);
			}
		}
	}
}
void intr1_null63(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[159]++;
#endif
}
#endif
/* Temporarily debug interrupt service used by AE102 due to macro issue  */
const __weak FUNCT_PTR_V_V intr0_service[] =
{
	intr0_gpio_a0,	// 0
	intr0_gpio_a1,	// 1
	intr0_gpio_a2,	// 2
	intr0_gpio_a3,	// 3
	intr0_gpio_a4,	// 4
	intr0_gpio_a5,	// 5
	intr0_gpio_a6,	// 6
	intr0_gpio_a7,	// 7
	intr0_gpio_a8,	// 8
	intr0_gpio_a9,	// 9
	intr0_gpio_a10, // 10
	intr0_gpio_a11, // 11
	intr0_gpio_a12, // 12
	intr0_gpio_a13, // 13
	intr0_gpio_a14, // 14
	intr0_gpio_a15, // 15
	intr0_gpio_a16, // 16
	intr0_gpio_a17, // 17
	intr0_gpio_a18, // 18
	intr0_gpio_a19, // 19
	intr0_gpio_a20, // 20
	intr0_gpio_a21, // 21
	intr0_gpio_a22, // 22
	intr0_gpio_a23, // 23
	intr0_gpio_a24, // 24
	intr0_gpio_a25, // 25
	intr0_gpio_a26, // 26
	intr0_gpio_a27, // 27
	intr0_gpio_a28, // 28
	intr0_gpio_a29, // 29
	intr0_gpio_a30, // 30
	intr0_gpio_a31, // 31
	intr0_gpio_b0,	// 32
	intr0_gpio_b1,	// 33
	intr0_gpio_b2,	// 34
	intr0_gpio_b3,	// 35
	intr0_gpio_b4,	// 36
	intr0_gpio_b5,	// 37
	intr0_gpio_b6,	// 38
	intr0_gpio_b7,	// 39
	intr0_gpio_b8,	// 40
	intr0_gpio_b9,	// 41
	intr0_gpio_b10, // 42
	intr0_gpio_b11, // 43
	intr0_gpio_b12, // 44
	intr0_gpio_b13, // 45
	intr0_gpio_b14, // 46
	intr0_gpio_b15, // 47
	intr0_gpio_b16, // 48
	intr0_gpio_b17, // 49
	intr0_gpio_b18, // 50
	intr0_gpio_b19, // 51
	intr0_gpio_b20, // 52
	intr0_gpio_b21, // 53
	intr0_gpio_b22, // 54
	intr0_gpio_b23, // 55
	intr0_gpio_b24, // 56
	intr0_gpio_b25, // 57
	intr0_gpio_b26, // 58
	intr0_gpio_b27, // 59
	intr0_gpio_b28, // 60
	intr0_gpio_b29, // 61
	intr0_gpio_b30, // 62
	intr0_gpio_b31, // 63
};
const __weak FUNCT_PTR_V_V intr1_service[] =
{
	intr1_gpio_c0,	// 0
	intr1_gpio_c1,	// 1
	intr1_gpio_c2,	// 2
	intr1_gpio_c3,	// 3
	intr1_gpio_c4,	// 4
	intr1_gpio_c5,	// 5
	intr1_gpio_c6,	// 6
	intr1_gpio_c7,	// 7
	intr1_gpio_c8,	// 8
	intr1_gpio_c9,	// 9
	intr1_gpio_c10, // 10
	intr1_gpio_c11, // 11
	intr1_gpio_c12, // 12
	intr1_gpio_c13, // 13
	intr1_gpio_c14, // 14
	intr1_gpio_c15, // 15
	intr1_gpio_e16, // 16
	intr1_gpio_e17, // 17
	intr1_gpio_e18, // 18
	intr1_gpio_e19, // 19
	intr1_gpio_e20, // 20
	intr1_gpio_e21, // 21
	intr1_gpio_e22, // 22
	intr1_gpio_e23, // 23
#if (GLE01 == 1)
		intr1_dma,	// 24
		intr1_cec0, // 25
#else
		intr1_sm2, // 24
		intr1_sm3, // 25
#endif
		intr1_cec1,		   // 26
		intr1_smbus4,	   // 27
		intr1_smbus5,	   // 28
		intr1_owi,		   // 29
		intr1_null30,	   // 30
		intr1_peci,		   // 31
		intr1_i3c0,		   // 32
		intr1_i3c1,		   // 33
		intr1_i3c2,		   // 34
		intr1_i3c3,		   // 35
		intr1_null36,	   // 36
		intr1_null37,	   // 37
		intr1_pmc2_ibf_ec, // 38
		intr1_pmc2_obe_ec, // 39
		intr1_pmc3_ibf_ec, // 40
		intr1_pmc3_obe_ec, // 41
		intr1_pmc4_ibf_ec, // 42
		intr1_pmc4_obe_ec, // 43
		intr1_pmc5_ibf_ec, // 44
		intr1_pmc5_obe_ec, // 45
		intr1_ps2_1,	   // 46
		intr1_uart1,	   // 47
		intr1_null48,	   // 48
		intr1_null49,	   // 49
		intr1_spim,		   // 50
		intr1_smbus0,	   // 51
		intr1_smbus1,	   // 52
		intr1_smbus2,	   // 53
		intr1_smbus3,	   // 54
		intr1_smbus6,	   // 55
		intr1_smbus7,	   // 56
		intr1_smbus8,	   // 57
		intr1_null58,	   // 58
		intr1_null59,	   // 59
		intr1_por_int,	   // 60
#if ((defined(AE103)) && (GLE01 == 1))
		intr1_mailbox, // 61
#else
		intr1_null61, // 61
#endif
		intr1_espi,	  // 62
		intr1_null63, // 63
};
/*debugger interrupts funtions*/
// enable set
BYTE Int_Control0_Enable(BYTE int_num)
{
	BYTE offset = int_num / 8;		  // 偏移
	BYTE mask = 0x1 << (int_num % 8); // 掩码
	if(offset <= (ICTL0_INTEN7_OFFSET - ICTL0_INTEN0_OFFSET))
	{
		INTC0_REG(ICTL0_INTEN0_OFFSET + offset) |= mask;
	}
	else
	{
		assert_print("error");
		return -1;
	}
	return 0;
}
BYTE Int_Control1_Enable(BYTE int_num)
{
	BYTE offset = int_num / 8;		  // 偏移
	BYTE mask = 0x1 << (int_num % 8); // 掩码
	if(offset <= (ICTL1_INTEN7_OFFSET - ICTL1_INTEN0_OFFSET))
	{
		INTC1_REG(ICTL1_INTEN0_OFFSET + offset) |= mask;
	}
	else
	{
		assert_print("error");
		return -1;
	}
	return 0;
}
// disable set
BYTE Int_Control0_Disable(BYTE int_num)
{
	BYTE offset = int_num / 8;		  // 偏移
	BYTE mask = 0x1 << (int_num % 8); // 掩码
	if(offset <= (ICTL0_INTEN7_OFFSET - ICTL0_INTEN0_OFFSET))
	{
		INTC0_REG(ICTL0_INTEN0_OFFSET + offset) &= ~mask;
	}
	else
	{
		assert_print("error");
		return -1;
	}
	return 0;
}
BYTE Int_Control1_Disable(BYTE int_num)
{
	BYTE offset = int_num / 8;		  // 偏移
	BYTE mask = 0x1 << (int_num % 8); // 掩码
	if(offset <= (ICTL1_INTEN7_OFFSET - ICTL1_INTEN0_OFFSET))
	{
		INTC1_REG(ICTL1_INTEN0_OFFSET + offset) &= ~mask;
	}
	else
	{
		assert_print("error");
		return -1;
	}
	return 0;
}
// mask set
BYTE Int_Control0_Mask(BYTE int_num)
{
	BYTE offset = int_num / 8;		  // 偏移
	BYTE mask = 0x1 << (int_num % 8); // 掩码
	if(offset <= (ICTL0_INTMASK7_OFFSET - ICTL0_INTMASK0_OFFSET))
	{
		INTC0_REG(ICTL0_INTMASK0_OFFSET + offset) |= mask;
	}
	else
	{
		assert_print("error");
		return -1;
	}
	return 0;
}
BYTE Int_Control1_Mask(BYTE int_num)
{
	BYTE offset = int_num / 8;		  // 偏移
	BYTE mask = 0x1 << (int_num % 8); // 掩码
	if(offset <= (ICTL1_INTMASK7_OFFSET - ICTL1_INTMASK0_OFFSET))
	{
		INTC1_REG(ICTL1_INTMASK0_OFFSET + offset) |= mask;
	}
	else
	{
		assert_print("error");
		return -1;
	}
	return 0;
}
// unmask set
BYTE Int_Control0_Unmask(BYTE int_num)
{
	BYTE offset = int_num / 8;		  // 偏移
	BYTE mask = 0x1 << (int_num % 8); // 掩码
	if(offset <= (ICTL0_INTMASK7_OFFSET - ICTL0_INTMASK0_OFFSET))
	{
		INTC0_REG(ICTL0_INTMASK0_OFFSET + offset) &= ~mask;
	}
	else
	{
		assert_print("error");
		return -1;
	}
	return 0;
}
BYTE Int_Control1_Unmask(BYTE int_num)
{
	BYTE offset = int_num / 8;		  // 偏移
	BYTE mask = 0x1 << (int_num % 8); // 掩码
	if(offset <= (ICTL1_INTMASK7_OFFSET - ICTL1_INTMASK0_OFFSET))
	{
		INTC1_REG(ICTL1_INTMASK0_OFFSET + offset) &= ~mask;
	}
	else
	{
		assert_print("error");
		return -1;
	}
	return 0;
}
// status read
BYTE Int_Control0_Status(BYTE int_num)
{
	BYTE offset = int_num / 8;		  // 偏移
	BYTE mask = 0x1 << (int_num % 8); // 掩码
	if(offset <= (ICTL0_MASKSTATUS7_OFFSET - ICTL0_MASKSTATUS0_OFFSET))
	{
		return (INTC0_REG(ICTL0_MASKSTATUS0_OFFSET + offset) & mask) != 0;
	}
	else
	{
		assert_print("error");
		return -1;
	}
}
BYTE Int_Control1_Status(BYTE int_num)
{
	BYTE offset = int_num / 8;		  // 偏移
	BYTE mask = 0x1 << (int_num % 8); // 掩码
	if(offset <= (ICTL1_MASKSTATUS7_OFFSET - ICTL1_MASKSTATUS0_OFFSET))
	{
		return (INTC1_REG(ICTL1_MASKSTATUS0_OFFSET + offset) & mask) != 0;
	}
	else
	{
		assert_print("error");
		return -1;
	}
}
BYTE Int_Control0_Enable_Read(BYTE int_num)
{
	BYTE offset = int_num / 8;		  // 偏移
	BYTE mask = 0x1 << (int_num % 8); // 掩码
	if(offset <= (ICTL0_INTEN7_OFFSET - ICTL0_INTEN0_OFFSET))
	{
		return (INTC0_REG(ICTL0_INTEN0_OFFSET + offset) & mask) != 0;
	}
	else
	{
		assert_print("error");
		return -1;
	}
}
BYTE Int_Control1_Enable_Read(BYTE int_num)
{
	BYTE offset = int_num / 8;		  // 偏移
	BYTE mask = 0x1 << (int_num % 8); // 掩码
	if(offset <= (ICTL1_INTEN7_OFFSET - ICTL1_INTEN0_OFFSET))
	{
		return (INTC1_REG(ICTL1_INTEN0_OFFSET + offset) & mask) != 0;
	}
	else
	{
		assert_print("error");
		return -1;
	}
}
BYTE Int_Control0_Mask_Read(BYTE int_num)
{
	BYTE offset = int_num / 8;		  // 偏移
	BYTE mask = 0x1 << (int_num % 8); // 掩码
	if(offset <= (ICTL0_INTMASK7_OFFSET - ICTL0_INTMASK0_OFFSET))
	{
		return (INTC0_REG(ICTL0_INTMASK0_OFFSET + offset) & mask) != 0;
	}
	else
	{
		assert_print("error");
		return -1;
	}
}
BYTE Int_Control1_Mask_Read(BYTE int_num)
{
	BYTE offset = int_num / 8;		  // 偏移
	BYTE mask = 0x1 << (int_num % 8); // 掩码
	if(offset <= (ICTL1_INTMASK7_OFFSET - ICTL1_INTMASK0_OFFSET))
	{
		return (INTC1_REG(ICTL1_INTMASK0_OFFSET + offset) & mask) != 0;
	}
	else
	{
		assert_print("error");
		return -1;
	}
}
void intc_init()
{
#if ICTL_MODULE_EN
	intc_MoudleClock_EN;
	// enable irq interrupt
	ICTL0_INTEN0 = 0xff;
	ICTL0_INTEN1 = 0xff;
	ICTL0_INTEN2 = 0xff;
	ICTL0_INTEN3 = 0xff;
	ICTL0_INTEN4 = 0xff;
	ICTL0_INTEN5 = 0xff;
	ICTL0_INTEN6 = 0xff;
	ICTL0_INTEN7 = 0x07;
	ICTL1_INTEN0 = 0xff;
	ICTL1_INTEN1 = 0xff;
	ICTL1_INTEN2 = 0xff;
	ICTL1_INTEN3 = 0xff;
	ICTL1_INTEN4 = 0xff;
	ICTL1_INTEN5 = 0xff;
	ICTL1_INTEN6 = 0xff;
	ICTL1_INTEN7 = 0xff;
	// mask all irq interrupt
	ICTL0_INTMASK0 = 0x00;
	ICTL0_INTMASK1 = 0x00;
	ICTL0_INTMASK2 = 0x00;
	ICTL0_INTMASK3 = 0x00;
	ICTL0_INTMASK4 = 0x00;
	ICTL0_INTMASK5 = 0x00;
	ICTL0_INTMASK6 = 0x00;
	ICTL0_INTMASK7 = 0xf8;
	ICTL1_INTMASK0 = 0x00;
	ICTL1_INTMASK1 = 0x00;
	ICTL1_INTMASK2 = 0x00;
	ICTL1_INTMASK3 = 0x00;
	ICTL1_INTMASK4 = 0x00;
	ICTL1_INTMASK5 = 0x00;
	ICTL1_INTMASK6 = 0x00;
	ICTL1_INTMASK7 = 0x00;
#endif
}
