/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-06-04 17:55:20
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
int32_t SYSCTL_MODEN0_CONTEXT = 0;
int32_t SYSCTL_MODEN1_CONTEXT = 0;
int32_t SYSCTL_PIO0_IECFG_CONTEXT = 0;
int32_t SYSCTL_PIO1_IECFG_CONTEXT = 0;
int32_t SYSCTL_PIO2_IECFG_CONTEXT = 0;
int32_t SYSCTL_PIO3_IECFG_CONTEXT = 0;
int32_t SYSCTL_CLKDIV_I3C_CONTEXT = 0;
int32_t SYSCTL_CLKDIV_PECI_CONTEXT = 0;
int32_t SYSCTL_RESERVER_CONTEXT = 0;
int32_t SYSCTL_DVDD_EN_CONTEXT = 0;
int32_t SYSCTL_SWITCH_PLL_CONTEXT = 0;
int32_t SYSCTL_CLKDIV_OSC96M_CONTEXT = 0;
int32_t RET_CTRL = 0;
// int32_t SYSCTL_DVDD_EN_CONTEXT = 0;
// int32_t SYSCTL_SWITCH_PLL_CONTEXT = 0;
// int32_t SYSCTL_CLKDIV_OSC96M_CONTEXT = 0;

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
/* ----------------------------------------------------------------------------
 * FUNCTION:   Save_Context
 *
 * Before entering low power mode, save the necessary CPU status
 *
 *
 * ------------------------------------------------------------------------- */
void Save_Context(void)
{
	SYSCTL_MODEN0_CONTEXT = SYSCTL_MODEN0;
	SYSCTL_MODEN1_CONTEXT = SYSCTL_MODEN1;
	SYSCTL_PIO0_IECFG_CONTEXT = SYSCTL_PIO0_IECFG;
	SYSCTL_PIO1_IECFG_CONTEXT = SYSCTL_PIO1_IECFG;
	SYSCTL_PIO2_IECFG_CONTEXT = SYSCTL_PIO2_IECFG;
	SYSCTL_PIO3_IECFG_CONTEXT = SYSCTL_PIO3_IECFG;
	SYSCTL_CLKDIV_I3C_CONTEXT = SYSCTL_CLKDIV_I3C;
	SYSCTL_CLKDIV_PECI_CONTEXT = SYSCTL_CLKDIV_PECI;
	SYSCTL_DVDD_EN_CONTEXT = SYSCTL_DVDD_EN;
	SYSCTL_SWITCH_PLL_CONTEXT = SYSCTL_SWITCH_PLL;
	SYSCTL_CLKDIV_OSC96M_CONTEXT = SYSCTL_CLKDIV_OSC96M;//降频3M
	RET_CTRL = *(volatile uint32_t *)(0x30510);
// 	SYSCTL_DVDD_EN_CONTEXT = SYSCTL_DVDD_EN;
// 	SYSCTL_SWITCH_PLL_CONTEXT = SYSCTL_SWITCH_PLL;
// 	SYSCTL_CLKDIV_OSC96M_CONTEXT = SYSCTL_CLKDIV_OSC96M;//降频3M
	// printf("BE:MODE0:%x,MODE1:%x\n",SYSCTL_MODEN0,SYSCTL_MODEN1);
	// printf("BE:IE0:%x,IE1:%x,IE2:%x,IE3:%x,IE4:%x,IE5:%x\n",SYSCTL_PIO0_IECFG,SYSCTL_PIO1_IECFG,SYSCTL_PIO2_IECFG,SYSCTL_PIO3_IECFG,SYSCTL_CLKDIV_I3C,SYSCTL_CLKDIV_PECI);
	// GPIO_Config(GPIOA, 0, 1, 0, 1, 1);
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
	SYSCTL_MODEN0 = SYSCTL_MODEN0_CONTEXT;
	SYSCTL_MODEN1 = SYSCTL_MODEN1_CONTEXT;
	SYSCTL_PIO0_IECFG = SYSCTL_PIO0_IECFG_CONTEXT;
	SYSCTL_PIO1_IECFG = SYSCTL_PIO1_IECFG_CONTEXT;
	SYSCTL_PIO2_IECFG = SYSCTL_PIO2_IECFG_CONTEXT;
	SYSCTL_PIO3_IECFG = SYSCTL_PIO3_IECFG_CONTEXT;
	SYSCTL_CLKDIV_I3C = SYSCTL_CLKDIV_I3C_CONTEXT;
	SYSCTL_CLKDIV_PECI = SYSCTL_CLKDIV_PECI_CONTEXT;
	SYSCTL_DVDD_EN = SYSCTL_DVDD_EN_CONTEXT;
	SYSCTL_CLKDIV_OSC96M = SYSCTL_CLKDIV_OSC96M_CONTEXT;
	SYSCTL_SWITCH_PLL = SYSCTL_SWITCH_PLL_CONTEXT;
	*(volatile uint32_t *)(0x30510) = RET_CTRL;
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
//deepsleep2测试环境,已废弃
#if 0
void Low_Power_Set(void)
{
	GPIO_Input_EN(GPIOA, 11, ENABLE);
	GPIO_Config(GPIOA, 11, 2, 0, 1, 0);
	*(volatile uint32_t *)(0x3050c) = 0x0;
	*(volatile uint32_t *)(0x30510) = 0x0;
	SYSCTL_RESERVER = BIT(28);
	// SYSCTL_DVDD_EN |= BIT4 | BIT5 | BIT6;
	// SYSCTL_DVDD_EN &= ~(BIT0 | BIT1 | BIT2);
	SYSCTL_DVDD_EN = 0xffffff;
	// SYSCTL_SWITCH_PLL &= ~BIT2;//main_clk_sel = 0
	SYSCTL_PMUCSR |= BIT(20) | BIT0;//Enable WFI Mode deepsleep2 flag
	SYSCTL_PIO1_CFG = 0x0;
	SYSCTL_RST0 = 0x0;
	SYSCTL_PIO0_IECFG = 0xffffffff;
	// SYSCTL_PIO0_IECFG = 0x7008008;//PIOA3输入使能
//printf("SYSCTL_PIO0_IECFG:0x%x\n", SYSCTL_PIO0_IECFG);
	// SYSCTL_PIO1_IECFG = 0x8;
	SYSCTL_PIO1_IECFG = 0xffffffff;
	//printf("SYSCTL_PIO1_IECFG:0x%x\n", SYSCTL_PIO1_IECFG);
	// SYSCTL_PIO2_IECFG = 0x0;
	SYSCTL_PIO2_IECFG = 0xffffffff;
	//printf("SYSCTL_PIO2_IECFG:0x%x\n", SYSCTL_PIO2_IECFG);
	// SYSCTL_PIO3_IECFG = 0x3000000;
	SYSCTL_PIO3_IECFG = 0xffffffff;
	//printf("SYSCTL_PIO3_IECFG:0x%x\n", SYSCTL_PIO3_IECFG);
	ADC_PM = 0b11;//ADC low power config bit0 close ldo and bit1 close comp
	//printf("SYSCTL_SWITCH_PLL:0x%x\n", SYSCTL_SWITCH_PLL);
	////mode 0 uart0 run gpio run 
	//SYSCTL_CLKDIV_SPIS = 0x0;
	SYSCTL_CLKDIV_PECI = 0x0;
	SYSCTL_SWITCH_PLL = (0x1 << 3) | (0x1 << 4) | (0x1 << 1) | (0x0 << 2) | (0x1 << 5);//sleep mode=1,dlsp=1,disable_osc80m = 1,main_clk_sel = 0
	printf("SYSCTL_SWITCH_PLL:0x%x\n", SYSCTL_SWITCH_PLL);
	// SYSCTL_MODEN0 = UART0_EN | GPIO_EN | PWM_EN| UART1_EN;
	// SYSCTL_MODEN0 = 0x00302180;//BRAM GPIO mailebox uart0 uart1
	// SYSCTL_MODEN0 = 0x10100100;
	SYSCTL_MODEN0 = 0x1fffffff;
	// SYSCTL_MODEN1 = DRAM_EN | SYSCTL_EN | SPIF_EN | APB_EN | GPIODB_EN | CACHE_EN | 0x00800000 | ICTL_EN | 0x00040000;
	// SYSCTL_MODEN1 = 0x19cffff;//crypto_cpu IRAM ROM | 0x100000
	SYSCTL_MODEN1 = 0x1ffffff;
	// SYSCTL_MODEN1 = 0x10c0e00;
	GPIO0_DEBOUNCE0 = 0x0;
// #if SUPPORT_GPIO_WAKEUP	
// 	GPIO_Config(GPIOA, 0, 2, 0, 1, 0);
// #endif
	//sysctl_iomux_pwm1();
	//PWM_Init_channel(PWM_CHANNEL1, PWM_HIGH, PWM_CLK0, PWM_CTR0, 50, 0);
	// asm volatile("wfi");
}
#endif
void Enter_LowPower_Mode(void)
{
	printf("enter lowpower mode\n");
#if SUPPORT_PWRSW_WAKEUP
	PWRSW_Config(0, 0);
#endif
#if SUPPORT_GPIO_WAKEUP
	GPIO_Config(GPIOB, 4, 2, 0, 1, 1);
#endif
	//1.置位标志位，保护现场
	Low_Power_Flag = 1;
	Save_Context();
	//2.关闭模块功能，减少功耗
	// CHIP_Sleep();
	// CHIP_Deep_Sleep();
	CHIP_Hibernation();
	//开启wfi模式，降低CPU运行
	CPU_Sleep();
}
void Exit_LowPower_Mode(void)
{
	if(Low_Power_Flag)
	{
		*(volatile uint32_t *)(0x2780) = 0x0;
		Low_Power_Flag = 0;
		Restore_Context();
		printf("exit lowpower mode\n");
	}
}
void CHIP_Sleep(void)
{
	printf("Sleep mode\n");
	GPIO_Input_EN(GPIOA, 3, ENABLE);
	GPIO_Config(GPIOA, 3, 2, 0, 1, 0);
	SYSCTL_RESERVER = BIT(28);
	//[2]:main_clk_sel  [3]:enable deepsleep [0]:swtich pll
	SYSCTL_SWITCH_PLL = (0x1 << 1) | (0x1 << 2) | (0x1 << 5);
	*(volatile uint32_t *)(0x30510) = 0x8ff;
	SYSCTL_DVDD_EN |= 0b10001111011 << 13;//iso enable 
	SYSCTL_DVDD_EN = 0xf6709;//poweroff
	ADC_PM = 0x3;//ADC low power config bit0 close ldo and bit1 close comp
	SYSCTL_PMUCSR = 0xFFFFFC7F;	//PMU_CFG

	SYSCTL_PIO0_IECFG = 0xffffffff;//关闭io输入脚，未适配
	SYSCTL_PIO1_IECFG = 0xffffffff;
	SYSCTL_PIO2_IECFG = 0xffffffff;
	SYSCTL_PIO3_IECFG = 0xffffffff;
	SYSCTL_CLKDIV_PECI = 0x0;
	SYSCTL_CLKDIV_OSC96M = 0x1f;//如果main_slc没设置为1,则主频降为3M，设置为1则降为32k

	SYSCTL_MODEN0 = 0x1fffffff;//关闭模块（未适配）
	SYSCTL_MODEN1 = 0x1ffffff;//关闭模块（未适配）
	GPIO0_DEBOUNCE0 = 0x0;
}

void CHIP_Deep_Sleep(void)
{
	printf("Deep_Sleep mode\n");
	GPIO_Input_EN(GPIOA, 3, ENABLE);
	GPIO_Config(GPIOA, 3, 2, 0, 1, 0);
	sysctl_iomux_config(GPIOA, 26, 0);//防止触发LPC_RST中断
	// SYSCTL_RESERVER = BIT(28);
	SYSCTL_PMUCSR &= 0xFFFFeFFF;
	SYSCTL_PMUCSR |= BIT(20);//Enable WFI Mode deepsleep flag

	*(volatile uint32_t *)(0x30510) = 0xfff;
	*(volatile uint32_t *)(0x30510) &= ~(BIT(6) | BIT(7));

	SYSCTL_DVDD_EN |= 0b10001111011 << 13;//iso enable
	SYSCTL_DVDD_EN = 0x8f6709;

	// SYSCTL_PIO1_CFG = 0x0;
	SYSCTL_PIO0_IECFG = 0x7008008;
	SYSCTL_PIO1_IECFG = 0x8;
	SYSCTL_PIO2_IECFG = 0x0;
	SYSCTL_PIO3_IECFG = 0xFF000000;
	ADC_PM = 0b11;//ADC low power config bit0 close ldo and bit1 close comp
	SYSCTL_CLKDIV_PECI = 0x0;
	SYSCTL_SWITCH_PLL = (0x1 << 5) | (0x1 << 3) | (0x1 << 1);//bit5  , bit4:dslp2 mode = 1 main_clk_sel = 0 ,disable_osc80m = 1,sleep mode=1,dlsp=1,,
	SYSCTL_MODEN0 = UART0_EN | GPIO_EN | PWM_EN | (0x1 << 13);
	SYSCTL_MODEN1 = DRAM_EN | SYSCTL_EN | SPIF_EN | APB_EN | GPIODB_EN | CACHE_EN | IRAM_EN | ICTL_EN | IVT_EN | ROM_EN | (0x1 << 23) | (0x1 << 24);
	GPIO0_DEBOUNCE0 = 0x0;
}

//废弃
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
	printf("Hibernation mode\n");
	GPIO_Input_EN(GPIOA, 11, ENABLE);
	GPIO_Config(GPIOA, 11, 2, 0, 1, 0);
	*(volatile uint8_t *)(0x2780) = 0x2;
	// printf("0x2780:%x\n", *(volatile uint8_t *)0x2780);
	nop;
	*(volatile uint8_t *)(0x2780) = 0x3 | 0x1 << 4;
	// printf("0x2780_1:%x\n", *(volatile uint8_t *)0x2780);
}

