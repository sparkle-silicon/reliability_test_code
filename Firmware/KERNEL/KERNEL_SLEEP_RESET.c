/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-08 20:32:10
 * @Description: This file applys for chip soft reset and chip sleep interface
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
#include "KERNEL_SLEEP_RESET.H"
#include "CUSTOM_POWER.H"
#include "KERNEL_TIMER.H"
int8_t KBS_KSDC1R_CONTEXT = 0;
int8_t KBS_KSDC2R_CONTEXT = 0;
int8_t KBS_KSDC3R_CONTEXT = 0;
int32_t SYSCTL_PIO0_CFG_CONTEXT = 0;
int32_t SYSCTL_PIO1_CFG_CONTEXT = 0;
int32_t SYSCTL_PIO2_CFG_CONTEXT = 0;
int32_t SYSCTL_PIO3_CFG_CONTEXT = 0;
int32_t SYSCTL_PIO4_CFG_CONTEXT = 0;
int32_t SYSCTL_PIO5_CFG_CONTEXT = 0;
int32_t SYSCTL_MODEN0_CONTEXT = 0;
int32_t SYSCTL_MODEN1_CONTEXT = 0;
int32_t SYSCTL_PIO0_IECFG_CONTEXT = 0;
int32_t SYSCTL_PIO1_IECFG_CONTEXT = 0;
int32_t SYSCTL_PIO2_IECFG_CONTEXT = 0;
int32_t SYSCTL_PIO3_IECFG_CONTEXT = 0;
int32_t SYSCTL_SWITCH_PLL_CONTEXT = 0;
int32_t SYSCTL_CLKDIV_OSC96M_CONTEXT = 0;
int32_t SYSCTL_PAD_PECI_CONTEXT = 0;
int32_t SYSCTL_PMUCSR_CONTEXT = 0;
int32_t SYSCTL_RET_CTRL_CONTEXT = 0;
int32_t ADC_CTRL_CONTEXT = 0;
int32_t SYSCTL_PMU_CFG_CONTEXT = 0;

/* ----------------------------------------------------------------------------
 * FUNCTION:   Module_SoftReset
 *
 * you need set corresponding bit to enable Module reset
 *
 * -----------------------------Module Reset bit map---------------------------
 * Moudle_RESET_REG0(0x30428)  1:reset enable  0:reset disable
 *   bit31:PS2_0 bit30:ROMP bit29:KBC bit28:PMC5 bit27:PMC4 bit26:PMC3 bit25:PMC2 bit24:PMC1
 *   bit23:SHM bit22:SWUC bit21:BRAM bit20:GPIO bit19:SMB0 bit18:SMB1 bit17:SMB2 bit16:SMB3
 *   bit15:WDT bit14:PPRORT bit13:HOST bit12:ADC bit11:PWM bit10:UARTA_BAUD bit9:UARTB_BAUD bit8:UART0_BAUD
 *   bit7:UART1_BAUD bit6:UART2_BAUD bit5:UART3_BAUD bit4:CAN0 bit3:CAN1 bit2:CAN2 bit1:CAN3 bit0:SPIM
 * ----------------------------------------------------------------------------
 * Moudle_RESET_REG1(0x3042c)  1:reset enable  0:reset disable
 *   bit18:LPC RESET ENABLE 1:yes 0:no     bit 16:CHIP RESET
 *   bit15:APB bit14:SYSCTL bit13:SM2 bit12:SM3 bit11:EFUSE bit10:PNP bit9:LDN bit8:SPIF
 *   bit7:H2E bit6:ICTL bit5:TIMER0 bit4:TIMER1 bit3:TIMER2 bit2:TIMER3 bit1:KBS bit0:PS2_1
 * ------------------------------------------------------------------------- */
void Module_SoftReset(int reg_idx, int bit_no)
{
	/*--------------this is a module reset example-------------*/
	// Moudle_RESET_REG0 |= 0x3ff066e1;
	// Moudle_RESET_REG0 = 0x0;
	// Moudle_RESET_REG1 = 0x0; 
	if(reg_idx == 0)
	{
		Moudle_RESET_REG0 |= (1 << bit_no);
		Moudle_RESET_REG0 = 0x0;
	}
	else
	{
		Moudle_RESET_REG1 |= (1 << bit_no);
		Moudle_RESET_REG1 = 0x0;
	}

}
/* ----------------------------------------------------------------------------
 * FUNCTION:   CPU_Sleep
 *
 * This is CPU Sleep Interface,If you call this function,cpu will sleep and
 * it will wake up when interrupt occurs.
 *
 * ------------------------------------------------------------------------- */
void CPU_Sleep(void)
{
	asm volatile("wfi");
}

void ALIGNED(4) OPTIMIZE(0) CPU_SLP_RES(void)
{
	volatile int i = 0;
	//flash power down
	MAILBOX_SELF_CMD = MAILBOX_CMD_FLASH_ENTERLOWPOWER; // 命令字
	MAILBOX_SET_IRQ(MAILBOX_Control_IRQ_NUMBER);   // 触发子系统中断
	MAILBOX_WAIT_IRQ(MAILBOX_CMD_FLASH_ENTERLOWPOWER, MAILBOX_Control_IRQ_NUMBER);
	MAILBOX_CLEAR_IRQ(MAILBOX_Control_IRQ_NUMBER); // 清除中断状态
	if(MAILBOX_OTHER_INFO1)
	{
		MAILBOX_SELF_CMD = MAILBOX_CMD_FLASH_ENTERLOWPOWER; // 命令字
		MAILBOX_SET_IRQ(MAILBOX_Control_IRQ_NUMBER);   // 触发子系统中断
		MAILBOX_WAIT_IRQ(MAILBOX_CMD_FLASH_ENTERLOWPOWER, MAILBOX_Control_IRQ_NUMBER);
		MAILBOX_CLEAR_IRQ(MAILBOX_Control_IRQ_NUMBER); // 清除中断状态
	}
	//mem retn
	SYSCTL_RET_CTRL &= ~(BIT(1) | BIT(2) | BIT(4) | BIT(5) | BIT(6) | BIT(7) | BIT(10) | BIT(11));//3μA
	//flash pin
	SYSCTL_PIO1_IECFG = 0x0;//外部spif pin
	SYSCTL_PIO3_IECFG &= 0x00ffffff;//内部spif pin
	//SYSCTL_PIO3_CFG = 0x0;
	//dvdd
	SYSCTL_DVDD_EN |= ((BIT(1) | BIT(2) | BIT(4) | BIT(5) | BIT(6) | BIT(7) | BIT(11)) << 12);//iso enable
	SYSCTL_DVDD_EN &= ~(BIT(1) | BIT(2) | BIT(4) | BIT(5) | BIT(6) | BIT(7) | BIT(11));
	//mode en
	SYSCTL_MODEN1 &= ~MAILBOX_EN;
	SYSCTL_MODEN1 &= ~(CRYPTO_EN | SPIFE_EN | SPIFI_EN | CACHE_EN);
#ifdef SUPPORT_DEEPSLEEP
//vcore->1.0V
	SYSCTL_PMU_CFG &= (~(0xff << 10));
	SYSCTL_PMU_CFG |= 22 << 10;
#endif
#ifdef SUPPORT_SLEEP
//switch osc to 3m
	SYSCTL_CLKDIV_OSC96M = 0x3F;
#endif
//cpu sleep
	asm volatile("wfi");
	nop; nop; nop; nop; nop; nop;
	SYSCTL_PMU_CFG = SYSCTL_PMU_CFG_CONTEXT;
	//restore
	SYSCTL_DVDD_EN |= (BIT(1) | BIT(2) | BIT(4) | BIT(5) | BIT(6) | BIT(7) | BIT(11));
	REG8(0xbffff) = 0;
	nop; nop; nop; nop; nop; nop;
	SYSCTL_DVDD_EN &= ~((BIT(1) | BIT(2) | BIT(4) | BIT(5) | BIT(6) | BIT(7) | BIT(11)) << 12);//iso enable
	SYSCTL_PIO0_CFG = SYSCTL_PIO0_CFG_CONTEXT;
	SYSCTL_PIO1_CFG = SYSCTL_PIO1_CFG_CONTEXT;
	SYSCTL_PIO2_CFG = SYSCTL_PIO2_CFG_CONTEXT;
	SYSCTL_PIO3_CFG = SYSCTL_PIO3_CFG_CONTEXT;
	SYSCTL_PIO4_CFG = SYSCTL_PIO4_CFG_CONTEXT;
	SYSCTL_PIO5_CFG = SYSCTL_PIO5_CFG_CONTEXT;
	SYSCTL_MODEN0 = SYSCTL_MODEN0_CONTEXT;
	SYSCTL_MODEN1 = SYSCTL_MODEN1_CONTEXT;
	SYSCTL_PIO0_IECFG = SYSCTL_PIO0_IECFG_CONTEXT;
	SYSCTL_PIO1_IECFG = SYSCTL_PIO1_IECFG_CONTEXT;
	SYSCTL_PIO2_IECFG = SYSCTL_PIO2_IECFG_CONTEXT;
	SYSCTL_PIO3_IECFG = SYSCTL_PIO3_IECFG_CONTEXT;
	SYSCTL_CLKDIV_OSC96M = SYSCTL_CLKDIV_OSC96M_CONTEXT;
	SYSCTL_SWITCH_PLL = SYSCTL_SWITCH_PLL_CONTEXT;
	SYSCTL_PAD_PECI = SYSCTL_PAD_PECI_CONTEXT;
	SYSCTL_PMUCSR = SYSCTL_PMUCSR_CONTEXT;
	SYSCTL_RET_CTRL = SYSCTL_RET_CTRL_CONTEXT;
	ADC_CTRL = ADC_CTRL_CONTEXT;
#if SUPPORT_KBS_WAKEUP
	KBS_KSDC1R = KBS_KSDC1R_CONTEXT;
	KBS_KSDC2R = KBS_KSDC2R_CONTEXT;
	KBS_KSDC3R = KBS_KSDC3R_CONTEXT;
#endif 
	for(i = 0; i < 1000; i++)
	{
		nop; nop; nop;
	}
	//flash exit power down
	MAILBOX_SELF_CMD = MAILBOX_CMD_FLASH_EXITLOWPOWER; // 命令字
	MAILBOX_SET_IRQ(MAILBOX_Control_IRQ_NUMBER);   // 触发子系统中断
	MAILBOX_WAIT_IRQ(MAILBOX_CMD_FLASH_EXITLOWPOWER, MAILBOX_Control_IRQ_NUMBER);
	MAILBOX_CLEAR_IRQ(MAILBOX_Control_IRQ_NUMBER); // 清除中断状态
	if(MAILBOX_OTHER_INFO1)
	{
		MAILBOX_SELF_CMD = MAILBOX_CMD_FLASH_EXITLOWPOWER; // 命令字
		MAILBOX_SET_IRQ(MAILBOX_Control_IRQ_NUMBER);   // 触发子系统中断
		MAILBOX_WAIT_IRQ(MAILBOX_CMD_FLASH_EXITLOWPOWER, MAILBOX_Control_IRQ_NUMBER);
		MAILBOX_CLEAR_IRQ(MAILBOX_Control_IRQ_NUMBER); // 清除中断状态
	}
	for(i = 0; i < 10000; i++)
	{
		nop; nop; nop;
	}
}
/* ----------------------------------------------------------------------------
 * FUNCTION:   Save_Context
 *
 * Before entering low power mode, save the necessary CPU status
 *
 *
 * ------------------------------------------------------------------------- */
void Save_Context(void)
{
#if SUPPORT_KBS_WAKEUP
	KBS_KSDC1R_CONTEXT = KBS_KSDC1R;
	KBS_KSDC2R_CONTEXT = KBS_KSDC2R;
	KBS_KSDC3R_CONTEXT = KBS_KSDC3R;
#endif 
	SYSCTL_PIO0_CFG_CONTEXT = SYSCTL_PIO0_CFG;
	SYSCTL_PIO1_CFG_CONTEXT = SYSCTL_PIO1_CFG;
	SYSCTL_PIO2_CFG_CONTEXT = SYSCTL_PIO2_CFG;
	SYSCTL_PIO3_CFG_CONTEXT = SYSCTL_PIO3_CFG;
	SYSCTL_PIO4_CFG_CONTEXT = SYSCTL_PIO4_CFG;
	SYSCTL_PIO5_CFG_CONTEXT = SYSCTL_PIO5_CFG;
	SYSCTL_MODEN0_CONTEXT = SYSCTL_MODEN0;
	SYSCTL_MODEN1_CONTEXT = SYSCTL_MODEN1;
	SYSCTL_PIO0_IECFG_CONTEXT = SYSCTL_PIO0_IECFG;
	SYSCTL_PIO1_IECFG_CONTEXT = SYSCTL_PIO1_IECFG;
	SYSCTL_PIO2_IECFG_CONTEXT = SYSCTL_PIO2_IECFG;
	SYSCTL_PIO3_IECFG_CONTEXT = SYSCTL_PIO3_IECFG;
	SYSCTL_SWITCH_PLL_CONTEXT = SYSCTL_SWITCH_PLL;
	SYSCTL_CLKDIV_OSC96M_CONTEXT = SYSCTL_CLKDIV_OSC96M;//降频3M
	SYSCTL_PAD_PECI_CONTEXT = SYSCTL_PAD_PECI;
	SYSCTL_PMUCSR_CONTEXT = SYSCTL_PMUCSR;
	SYSCTL_RET_CTRL_CONTEXT = SYSCTL_RET_CTRL;
	ADC_CTRL_CONTEXT = ADC_CTRL;
	SYSCTL_PMU_CFG_CONTEXT = SYSCTL_PMU_CFG;
}
/* ----------------------------------------------------------------------------
 * FUNCTION:   Restore_Context
 *
 * Before exiting low power mode, restore the necessary CPU status
 *
 *
 * ------------------------------------------------------------------------- */
void Restore_Context(void)
{
	SYSCTL_PIO0_CFG = SYSCTL_PIO0_CFG_CONTEXT;
	SYSCTL_PIO1_CFG = SYSCTL_PIO1_CFG_CONTEXT;
	SYSCTL_PIO2_CFG = SYSCTL_PIO2_CFG_CONTEXT;
	SYSCTL_PIO3_CFG = SYSCTL_PIO3_CFG_CONTEXT;
	SYSCTL_PIO4_CFG = SYSCTL_PIO4_CFG_CONTEXT;
	SYSCTL_PIO5_CFG = SYSCTL_PIO5_CFG_CONTEXT;
	SYSCTL_MODEN0 = SYSCTL_MODEN0_CONTEXT;
	SYSCTL_MODEN1 = SYSCTL_MODEN1_CONTEXT;
	SYSCTL_PIO0_IECFG = SYSCTL_PIO0_IECFG_CONTEXT;
	SYSCTL_PIO1_IECFG = SYSCTL_PIO1_IECFG_CONTEXT;
	SYSCTL_PIO2_IECFG = SYSCTL_PIO2_IECFG_CONTEXT;
	SYSCTL_PIO3_IECFG = SYSCTL_PIO3_IECFG_CONTEXT;
	SYSCTL_CLKDIV_OSC96M = SYSCTL_CLKDIV_OSC96M_CONTEXT;
	SYSCTL_SWITCH_PLL = SYSCTL_SWITCH_PLL_CONTEXT;
	SYSCTL_PAD_PECI = SYSCTL_PAD_PECI_CONTEXT;
	SYSCTL_PMUCSR = SYSCTL_PMUCSR_CONTEXT;
	SYSCTL_RET_CTRL = SYSCTL_RET_CTRL_CONTEXT;
	ADC_CTRL = ADC_CTRL_CONTEXT;
	SYSCTL_PMU_CFG = SYSCTL_PMU_CFG_CONTEXT;
}
void CPU_Timer_Set()
{
	// stop timer
	write_csr(0xBDB, 0x1);
	// enable cpu timer interrupt
	int32_t val = read_csr(0xBD1);
	write_csr(0xBD1, val | 0x2);
	// clear timer
	write_csr(0xBDA, 0x0);
	// set timer
	write_csr(0xBD9, 32768 / 2);
	// start timer
	write_csr(0xBDB, 0x0);
}

void Enter_LowPower_Mode(void)
{
	printf("enter lowpower mode\n");
	//1.置位标志位，保护现场
	Low_Power_Flag = 1;
	Save_Context();
	//2.关闭模块功能，减少功耗
#ifdef SUPPORT_SLEEP
	CHIP_Sleep();
#endif
#ifdef SUPPORT_DEEPSLEEP
	CHIP_Deep_Sleep();
#endif
#ifdef SUPPORT_HIBERNATION
	CHIP_Hibernation();
#endif
#if SUPPORT_PWRSW_WAKEUP
	GPIO_Input_EN(GPIOA, 11, ENABLE);
	PWRSW_Config(0, 0);
#endif
#if SUPPORT_GPIO_WAKEUP
	GPIO_Input_EN(GPIOA, 3, ENABLE);
	GPIO_Config(GPIOA, 3, 2, 0, 1, 0);
#endif
#if SUPPORT_KBS_WAKEUP
	if((SYSCTL_PIO3_UDCFG & 0xff0000) != 0xff0000)//kbs in pull up
	{
		SYSCTL_PIO3_UDCFG |= 0x00ff0000;
		for(volatile int i = 0; i < 100000; i++)
		{
			nop; nop; nop;
		}
	}
#endif
	//开启wfi模式，降低CPU运行
	Disable_Interrupt_Main_Switch();                  // Disable All Interrupt
	Smf_Ptr = Load_Smfi_To_Dram(CPU_SLP_RES, 0x600);
	if(Smf_Ptr != 0)
		(*Smf_Ptr)(); // Do Function at malloc address
	Enable_Interrupt_Main_Switch();
	free(Smf_Ptr);  // 释放通过 malloc 分配的内存空间
	Smf_Ptr = NULL; // 将指针设置为 NULL，以避免悬空指针问题
}
void Exit_LowPower_Mode(void)
{
	if(Low_Power_Flag)
	{
		//*(volatile uint32_t *)(0x2780) = 0x0;
		Low_Power_Flag = 0;
		Restore_Context();
		printf("exit lowpower mode\n");
		printf("trap test\n"); printf("trap test\n"); printf("trap test\n");
	}
}
void CHIP_Sleep(void)
{
	dprint("Sleep mode\n");
	VDWORD TEMP_MODEN0 = 0;
	VDWORD TEMP_MODEN1 = 0;
#if SUPPORT_GPIO_WAKEUP
	TEMP_MODEN0 |= GPIO_EN;
#endif
#if SUPPORT_PWRSW_WAKEUP
	SYSCTL_PWRSWCSR &= ~0x1;
#endif
#if SUPPORT_KBS_WAKEUP
	TEMP_MODEN0 |= KBS_EN;
#endif
	TEMP_MODEN0 |= MAILBOX_EN;
	TEMP_MODEN1 = DRAM_EN | SYSCTL_EN | SPIFE_EN | APB_EN | GPIODB_EN | CACHE_EN | IVT_EN | ROM_EN | CRYPTO_EN | SPIFI_EN;
	//mode en
	SYSCTL_MODEN0 = TEMP_MODEN0;
	SYSCTL_MODEN1 = TEMP_MODEN1;
	//peci pad off
	SYSCTL_PAD_PECI |= BIT1;
	//Enable WFI Mode
	SYSCTL_PMUCSR &= 0xFFFFeFFF;
	SYSCTL_PMUCSR |= BIT(20);
	//[2]:main_clk_sel  [3]:enable deepsleep [0]:swtich pll
	SYSCTL_SWITCH_PLL = (0x1 << 1) | (0x1 << 2) | (0x1 << 5);
	//adc power down
	ADC_CTRL = 0x0;
	//PMU_CFG
	SYSCTL_PMU_CFG &= 0xFFFFFC7F;//PMU_CFG
	//ie off
	SYSCTL_PIO0_IECFG = 0x0;
	//SYSCTL_PIO1_IECFG = 0x00f00000;//外部spif pin
	SYSCTL_PIO2_IECFG = 0x0;
	//switch osc to 3m
	SYSCTL_CLKDIV_OSC96M = 0x3F;
	//io mux
#if SUPPORT_KBS_WAKEUP
	SYSCTL_PIO5_CFG &= 0xfffc0000;
#if (KBD_8_n_SWITCH == 16)
	SYSCTL_PIO1_CFG = 0;
#elif (KBD_8_n_SWITCH == 17)
	SYSCTL_PIO1_CFG &= 0b11 << 6;
#elif (KBD_8_n_SWITCH == 18)
	SYSCTL_PIO1_CFG &= (0b11 << 6 | 0b11 << 10);
#endif
	SYSCTL_PIO3_IECFG = 0xffff0000;
#else
	SYSCTL_PIO5_CFG = 0;
	SYSCTL_PIO1_CFG = 0;
	SYSCTL_PIO3_IECFG = 0xff000000;//内部flash pin
#endif
	SYSCTL_PIO0_CFG = 0;
	SYSCTL_PIO2_CFG = 0;
	SYSCTL_PIO3_CFG &= 0xff00;//外部flash pin
	SYSCTL_PIO4_CFG = 0;
}

void CHIP_Deep_Sleep(void)
{
	dprint("Deep_Sleep mode\n");
	VDWORD TEMP_MODEN0 = 0;
	VDWORD TEMP_MODEN1 = 0;
#if SUPPORT_GPIO_WAKEUP
	TEMP_MODEN0 |= GPIO_EN;
#endif
#if SUPPORT_PWRSW_WAKEUP
	SYSCTL_PWRSWCSR &= ~0x1;
#endif
#if SUPPORT_KBS_WAKEUP
	TEMP_MODEN0 |= KBS_EN;
	KBS_KSDC3R = 0;
	KBS_KSDC2R = (KBS_KSDC1R & (~0xf));//1切换为0的延迟时间 24μs
	KBS_KSDC1R = (KBS_KSDC1R & (~0x7)) | 0x1; // round 2
#endif
	TEMP_MODEN0 |= MAILBOX_EN;
	TEMP_MODEN1 = DRAM_EN | SYSCTL_EN | SPIFE_EN | APB_EN | GPIODB_EN | CACHE_EN | IVT_EN | ROM_EN | CRYPTO_EN | SPIFI_EN;
	//mode en
	SYSCTL_MODEN0 = TEMP_MODEN0;
	SYSCTL_MODEN1 = TEMP_MODEN1;
	//mask gpio interrupt
	// GPIO0_INTMASK0=0xff;GPIO0_INTMASK1=0xff;GPIO0_INTMASK2=0xff;GPIO0_INTMASK3=0xff;
	// GPIO1_INTMASK0=0xff;GPIO1_INTMASK1=0xff;GPIO1_INTMASK2=0xff;GPIO1_INTMASK3=0xff;
	// GPIO2_INTMASK0=0xff;GPIO2_INTMASK1=0xff;
	// GPIO3_INTMASK0=0xff;GPIO3_INTMASK1=0xff;GPIO3_INTMASK2=0xff;
	//PECI pad off
	SYSCTL_PAD_PECI |= BIT1;
	//Enable WFI Mode
	SYSCTL_PMUCSR &= 0xFFFFeFFF;
	SYSCTL_PMUCSR |= BIT(20);//Enable WFI Mode deepsleep flag
	//mem retn
	//SYSCTL_RET_CTRL=0;
	//PMU_CFG
	SYSCTL_PMU_CFG &= 0xFFFFFC7F;//PMU_CFG
	//ie off
	SYSCTL_PIO0_IECFG = 0x0;
	//SYSCTL_PIO1_IECFG = 0x00f00000;//外部spif pin
	SYSCTL_PIO2_IECFG = 0x0;
	//adc power down
	ADC_CTRL = 0x0;//ADC low power config bit0 close ldo and bit1 close comp
	//[3]:enable deepsleep [0]:swtich pll
	SYSCTL_SWITCH_PLL = (0x1 << 5) | (0x1 << 3) | (0x1 << 1);
	//io mux
#if SUPPORT_KBS_WAKEUP
	SYSCTL_PIO5_CFG &= 0xfffc0000;
#if (KBD_8_n_SWITCH == 16)
	SYSCTL_PIO1_CFG = 0;
#elif (KBD_8_n_SWITCH == 17)
	SYSCTL_PIO1_CFG &= 0b11 << 6;
#elif (KBD_8_n_SWITCH == 18)
	SYSCTL_PIO1_CFG &= (0b11 << 6 | 0b11 << 10);
#endif
	SYSCTL_PIO3_IECFG &= 0xffff0000;
#else
	SYSCTL_PIO5_CFG = 0;
	SYSCTL_PIO1_CFG = 0;
	SYSCTL_PIO3_IECFG = 0xff000000;//内部flash pin
#endif
	SYSCTL_PIO0_CFG = 0x0;
	SYSCTL_PIO2_CFG = 0;
	SYSCTL_PIO3_CFG &= 0xff00;
	SYSCTL_PIO4_CFG = 0;
}

#if 0
void CHIP_Deep_Sleep2(void)
{
	GPIO_Input_EN(GPIOA, 11, ENABLE);
	GPIO_Config(GPIOA, 11, 2, 0, 1, 0);
	// *(volatile uint32_t *)(0x3050c) = 0x0;
	// SYSCTL_RESERVER = BIT(28);
	*(volatile uint8_t *)(0x2780) = 0x1 << 4;
	SYSCTL_DVDD_EN = 0xff6109;
	*(volatile uint32_t *)(0x30510) = 0x1eff;
	SYSCTL_PMUCSR |= BIT(20) | BIT0;//Enable WFI Mode deepsleep2 flag
	// SYSCTL_PIO1_CFG = 0x0;
	// SYSCTL_RST1 |= 0x1<<16;
	SYSCTL_PIO0_IECFG = 0xffffffff;
	SYSCTL_PIO1_IECFG = 0xffffffff;
	SYSCTL_PIO2_IECFG = 0xffffffff;
	SYSCTL_PIO3_IECFG = 0xffffffff;
	ADC_PM = 0b11;//ADC low power config bit0 close ldo and bit1 close comp
	SYSCTL_CLKDIV_PECI = 0x0;
	SYSCTL_SWITCH_PLL = (0x1 << 5) | (0x1 << 4) | (0x1 << 3) | (0x0 << 2) | (0x1 << 1);//sleep mode=1,dlsp=1,disable_osc80m = 1,main_clk_sel = 0
	printf("SYSCTL_SWITCH_PLL:0x%x\n", SYSCTL_SWITCH_PLL);
	SYSCTL_MODEN0 = 0x1fffffff;
	SYSCTL_MODEN1 = 0x1ffffff;
	GPIO0_DEBOUNCE0 = 0x0;
}
#endif
void CHIP_Hibernation(void)
{
	dprint("Hibernation mode\n");
	if(REG8(0x2780) & 0x1)
	{
		REG8(0x2780) = 0;
		for(volatile int i = 0; i < 100000; i++)
		{
			nop;
		}
	}
	REG8(0x2780) = 0x1 | Hibernation_Wakeup_GPIO;
}

