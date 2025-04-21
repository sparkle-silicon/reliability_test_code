/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-06-18 14:44:43
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
// #if defined(AE103)
// int32_t SYSCTL_DVDD_EN_CONTEXT = 0;
// int32_t SYSCTL_SWITCH_PLL_CONTEXT = 0;
// int32_t SYSCTL_CLKDIV_OSC96M_CONTEXT = 0;
// #endif

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
// #if defined(AE103)
// 	SYSCTL_DVDD_EN_CONTEXT = SYSCTL_DVDD_EN;
// 	SYSCTL_SWITCH_PLL_CONTEXT = SYSCTL_SWITCH_PLL;
// 	SYSCTL_CLKDIV_OSC96M_CONTEXT = SYSCTL_CLKDIV_OSC96M;//降频3M
// #endif
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
	SYSCTL_PMUCSR |= BIT(20)|BIT0;//Enable WFI Mode deepsleep2 flag
	SYSCTL_PIO1_CFG = 0x0;
	SYSCTL_RST0 = 0x0;
	SYSCTL_PIO0_IECFG = 0xffffffff;
	// SYSCTL_PIO0_IECFG = 0x7008008;//PIOA3输入使能
//printf("SYSCTL_PIO0_IECFG:0x%x\n", SYSCTL_PIO0_IECFG);
	// SYSCTL_PIO1_IECFG = 0x8;
	SYSCTL_PIO1_IECFG  = 0xffffffff;
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
	SYSCTL_SWITCH_PLL = (0x1<<3) |(0x1<<4)|(0x1<<1)|(0x0<<2)|(0x1<<5);//sleep mode=1,dlsp=1,disable_osc80m = 1,main_clk_sel = 0
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
	// GLE_Sleep();
	// GLE_Deep_Sleep();
	GLE_Hibernation();
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
void GLE_Sleep(void)
{
	printf("GLE_Sleep mode\n");
	GPIO_Input_EN(GPIOA, 3, ENABLE);
	GPIO_Config(GPIOA, 3, 2, 0, 1, 0);
	SYSCTL_RESERVER = BIT(28);
	//[2]:main_clk_sel  [3]:enable deepsleep [0]:swtich pll
	SYSCTL_SWITCH_PLL = (0x1<<1)|(0x1<<2)|(0x1<<5);
	*(volatile uint32_t *)(0x30510) = 0x8ff;
	SYSCTL_DVDD_EN|=0b10001111011<<13;//iso enable 
	SYSCTL_DVDD_EN = 0xf6709;//poweroff
	ADC_PM = 0x3;//ADC low power config bit0 close ldo and bit1 close comp
	SYSCTL_PMUCSR = 0xFFFFFC7F;	//PMU_CFG

	SYSCTL_PIO0_IECFG = 0xffffffff;//关闭io输入脚，未适配
	SYSCTL_PIO1_IECFG  = 0xffffffff;
	SYSCTL_PIO2_IECFG = 0xffffffff;
	SYSCTL_PIO3_IECFG = 0xffffffff;
	SYSCTL_CLKDIV_PECI = 0x0;
	SYSCTL_CLKDIV_OSC96M = 0x1f;//如果main_slc没设置为1,则主频降为3M，设置为1则降为32k

	SYSCTL_MODEN0 = 0x1fffffff;//关闭模块（未适配）
	SYSCTL_MODEN1 = 0x1ffffff;//关闭模块（未适配）
	GPIO0_DEBOUNCE0 = 0x0;
}

void GLE_Deep_Sleep(void)
{
	printf("GLE_Deep_Sleep mode\n");
    GPIO_Input_EN(GPIOA, 3, ENABLE);
	GPIO_Config(GPIOA, 3, 2, 0, 1, 0);
	// SYSCTL_RESERVER = BIT(28);
	SYSCTL_PMUCSR = 0xFFFFeFFF;	//PMU_CFG

	SYSCTL_MODEN0 = 0x1fffffff;
	SYSCTL_MODEN1 = 0x1ffffff;

	*(volatile uint32_t *)(0x30510) = 0x8ff;

	SYSCTL_DVDD_EN = 0b10001111011<<13;//iso enable
	SYSCTL_DVDD_EN = 0x8f6709;

	SYSCTL_PMUCSR = 0xFFFFFC7F ;//Enable WFI Mode deepsleep flag
	// SYSCTL_PIO1_CFG = 0x0;
	SYSCTL_PIO0_IECFG = 0xffffffff;
	SYSCTL_PIO1_IECFG = 0xffffffff;
	SYSCTL_PIO2_IECFG = 0xffffffff;
	SYSCTL_PIO3_IECFG = 0xffffffff;
	ADC_PM = 0b11;//ADC low power config bit0 close ldo and bit1 close comp
	SYSCTL_CLKDIV_PECI = 0x0;
	SYSCTL_SWITCH_PLL = (0x1<<5)|(0x1<<3)|(0x0<<2)|(0x1<<1);//bit5  , bit4:dslp2 mode = 1 main_clk_sel = 0 ,disable_osc80m = 1,sleep mode=1,dlsp=1,,

	GPIO0_DEBOUNCE0 = 0x0;
}

//废弃
#if 0
void GLE_Deep_Sleep2(void)
{
	GPIO_Input_EN(GPIOA, 11, ENABLE);
	GPIO_Config(GPIOA, 11, 2, 0, 1, 0);
	// *(volatile uint32_t *)(0x3050c) = 0x0;
	// SYSCTL_RESERVER = BIT(28);
	*(volatile uint8_t *)(0x2780) =	0x1<<4;
	SYSCTL_DVDD_EN = 0xff6109;
	*(volatile uint32_t *)(0x30510) = 0x1eff;
	SYSCTL_PMUCSR |= BIT(20)|BIT0;//Enable WFI Mode deepsleep2 flag
	// SYSCTL_PIO1_CFG = 0x0;
	// SYSCTL_RST1 |= 0x1<<16;
	SYSCTL_PIO0_IECFG = 0xffffffff;
	SYSCTL_PIO1_IECFG = 0xffffffff;
	SYSCTL_PIO2_IECFG = 0xffffffff;
	SYSCTL_PIO3_IECFG = 0xffffffff;
	ADC_PM = 0b11;//ADC low power config bit0 close ldo and bit1 close comp
	SYSCTL_CLKDIV_PECI = 0x0;
	SYSCTL_SWITCH_PLL = (0x1<<5) |(0x1<<4)|(0x1<<3)|(0x0<<2)|(0x1<<1);//sleep mode=1,dlsp=1,disable_osc80m = 1,main_clk_sel = 0
	printf("SYSCTL_SWITCH_PLL:0x%x\n", SYSCTL_SWITCH_PLL);
	SYSCTL_MODEN0 = 0x1fffffff;
	SYSCTL_MODEN1 =	0x1ffffff;
	GPIO0_DEBOUNCE0 = 0x0;
}
#endif
void GLE_Hibernation(void)
{
	printf("GLE_Hibernation mode\n");
	GPIO_Input_EN(GPIOA, 11, ENABLE);
	GPIO_Config(GPIOA, 11, 2, 0, 1, 0);
	*(volatile uint8_t *)(0x2780) = 0x2;
	// printf("0x2780:%x\n", *(volatile uint8_t *)0x2780);
	nop;
	*(volatile uint8_t *)(0x2780) = 0x3|0x1<<4;
	// printf("0x2780_1:%x\n", *(volatile uint8_t *)0x2780);
}

#ifdef AE103
#define GPIOB4_NOSET 0
void AE103_Sleep(void)
{
	
// 	//sleep mode config
// 	GPIO_Input_EN(GPIOA, 3, ENABLE);
// 	SYSCTL_SWITCH_PLL &= ~(BIT2);//main_clk_sel = 0  32k
// 	//SYSCTL_SWITCH_PLL |= BIT2;//main_clk_sel = 1
// 	SYSCTL_DVDD_EN |= BIT4 | BIT5 | BIT6;
// 	SYSCTL_DVDD_EN &= ~(BIT0 | BIT1 | BIT2);
// 	//SYSCTL_SWITCH_PLL |= BIT1;//enable osc80m
// 	SYSCTL_SWITCH_PLL &= ~(BIT1 | BIT3); // disable osc80m 并设置 sleep mode
// 	//printf("SYSCTL_SWITCH_PLL:%x\n", SYSCTL_SWITCH_PLL);
// 	SYSCTL_CLKDIV_SPIS = 0x0;
// 	SYSCTL_CLKDIV_PECI = 0x0;
// 	SYSCTL_PMUCSR |= BIT(20);//Enable WFI Mode
// 	SYSCTL_PIO0_IECFG = 0x7008008;
// 	//printf("SYSCTL_PIO0_IECFG:0x%x\n", SYSCTL_PIO0_IECFG);
// 	SYSCTL_PIO1_IECFG = 0x8;
// 	//GPIO_Input_EN(GPIOA, 3, ENABLE);
// 	//printf("SYSCTL_PIO1_IECFG:0x%x\n", SYSCTL_PIO1_IECFG);
// 	SYSCTL_PIO2_IECFG = 0x0;
// 	//printf("SYSCTL_PIO2_IECFG:0x%x\n", SYSCTL_PIO2_IECFG);
// 	SYSCTL_PIO3_IECFG = 0x3000000;
// 	//printf("SYSCTL_PIO3_IECFG:0x%x\n", SYSCTL_PIO3_IECFG);
// 	ADC_PM = 0b11;//ADC low power config bit0 close ldo and bit1 close comp
// //#if GPIOB4_NOSET//NOUP
// 	//mode 0 no run 
// 	//SYSCTL_MODEN0 = 0;
// 	//modle 1 spif(bit3)run  cache(19)run 
// 	//SYSCTL_MODEN1 = SPIF_EN | CACHE_EN;
// //#else//GPIOB4_NOSET
// 	////mode 0 uart0 run gpio run 
// 	SYSCTL_MODEN0 = UART0_EN | GPIO_EN | PWM_EN;
// 		//modle 1 spif(bit3)run  cache(19)run 
// 	SYSCTL_MODEN1 = DRAM_EN | SYSCTL_EN | SPIF_EN | APB_EN | GPIODB_EN | CACHE_EN | IRAM_EN | ICTL_EN | IVT_EN;
// 	GPIO0_DEBOUNCE0 = 0x0;
// 	//printf("SYSCTL_MODEN1:0x%x\n", SYSCTL_MODEN1);
// // SYSCTL_MODEN0 = 0xffffffff;
// // SYSCTL_MODEN1 = 0xffffffff;
// //#endif
// // #if SUPPORT_GPIO_WAKEUP	
// // 	GPIO_Config(GPIOA, 0, 2, 0, 1, 0);
// // #endif
// 	//printf("SYSCTL_SWITCH_PLL:0x%x\n", SYSCTL_SWITCH_PLL);
// 	//sysctl_iomux_pwm1();
// 	//PWM_Init_channel(PWM_CHANNEL1, PWM_HIGH, PWM_CLK0, PWM_CTR0, 50, 0);
// 	//asm volatile("wfi");
}
void AE103_Deep_Sleep(void)
{

// 	SYSCTL_PIO0_IECFG = 0x0;
// #if 0//NOUP
// 	if(SYSCTL_PIO_CFG & BIT1)
// 		SYSCTL_PIO1_IECFG = 0x00f00000;//only open spif
// 	else
// 		SYSCTL_PIO1_IECFG = 0x0;
// #else//GPIOB4_NOSET
// 	if(SYSCTL_PIO_CFG & BIT1)
// 		SYSCTL_PIO1_IECFG = 0x00f00000 | BIT4;// open spif
// 	else
// 		SYSCTL_PIO1_IECFG = BIT4;
// 	ICTL0_INTMASK4 |= BIT4;
// 	GPIO1_INTMASK0 |= BIT4;//先屏蔽中断
// 	GPIO1_DDR0 &= (~BIT4);//配置位输入
// 	GPIO1_INTEN0 |= BIT4;//使能中断
// 	GPIO1_INTTYPE_LEVEL0 |= BIT4;//设置为边沿触发
// 	GPIO1_INT_POLARITY0 &= ~BIT4;//设置为下降沿触发
// 	GPIO1_INTMASK0 &= ~BIT4;//取消中断屏蔽
// 	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_INTR0, en, 1, 0);
// 	ICTL0_INTEN4 |= BIT4;
// 	set_csr(mstatus, MSTATUS_MIE);
// 	ICTL0_INTMASK4 &= ~BIT4;
// #endif

// 	if(SYSCTL_PIO_CFG & BIT1)
// 		SYSCTL_PIO1_IECFG = 0x00f00000;//only open spif
// 	else
// 		SYSCTL_PIO1_IECFG = 0x0;//only open spif
// 	SYSCTL_PIO2_IECFG = 0x0;
// 	SYSCTL_PIO3_IECFG = 0x0;
// 	SYSCTL_CLKDIV_I3C = 0x0;
// 	SYSCTL_CLKDIV_PECI = 0x0;
// 	SYSCTL_PMUCSR |= BIT(20);//Enable WFI Mode
// 	//SET Enable DeepSleep and Switch PLL
// 	//BIT0:1:SWITCH Internel OSC32k/196M(0:switch externel OSC32k/pll)
// 	//BIT1:0:Disable Internel OSC192M(1:ENable Internel OSC192M)
// 	//BIT2:1:Sleep Main Freq Set High Clock(0:Sleep Main Freq Set Low Clock)
// 	//BIT3:0:Sleep No close High Clock(1:wait main freq set low clock(bit2=0),close high clock)
// 	SYSCTL_SWITCH_PLL = BIT3 | BIT0;
// 	ADC_PM = 0b11;//ADC low power config bit0 close ldo and bit1 close comp 
// 	SYSCTL_DVDD_EN |= BIT4 | BIT5 | BIT6;
// 	SYSCTL_DVDD_EN &= ~(BIT0 | BIT1 | BIT2);
// #if GPIOB4_NOSET//NOUP
// 	//mode 0 no run 
// 	SYSCTL_MODEN0 = 0;
// 	//modle 1 spif(bit3)run  cache(19)run 
// 	SYSCTL_MODEN1 = SPIF_EN | CACHE_EN;
// #else//GPIOB4_NOSET
// 	////mode 0 uart0 run gpio run 
// 	SYSCTL_MODEN0 = UART0_EN | GPIO_EN;
// 	//modle 1 spif(bit3)run  cache(19)run 
// 	SYSCTL_MODEN1 = SPIF_EN | CACHE_EN | ICTL_EN;
// #endif
// 	asm volatile("wfi");
}
#define LOW_TRIM_DVAL 0
#define High_TRIM_DVAL 0
#define Delay_RTC_Timer 10
void Test_OSC32K(void)
{
	const u32 mask = 0x3ff;
	u8 offset = 0;
	u32 trim;
	trim = (REG32(0x304D8) >> offset) & mask;
	trim += LOW_TRIM_DVAL;
	REG32(0x304D8) = (REG32(0x304D8) & (~(mask << offset))) | ((trim & mask) << offset);//LOW
	offset = 13;
	trim = (REG32(0x304D8) >> offset) & mask;
	trim += High_TRIM_DVAL;
	REG32(0x304D8) = (REG32(0x304D8) & (~(mask << offset))) | ((trim & mask) << offset);//LOW//HIGH
	//open pwm clk and reset pwm mode
	SYSCTL_MODEN0 |= PWM_EN;
	SYSCTL_RST0 |= PWM_EN;
	nop;
	SYSCTL_RST0 &= (~PWM_EN);
	nop;
	//set PWM[7:0] IO MUX pin multiplexing to GPA[7:0]
	sysctl_iomux_pwm0();
	sysctl_iomux_pwm1();
	sysctl_iomux_pwm2();
	sysctl_iomux_pwm3();
	sysctl_iomux_pwm4();
	sysctl_iomux_pwm5();
	sysctl_iomux_pwm6();
	sysctl_iomux_pwm7();
	//SYSCTL PMU CONFIG STATUS REGISTER Set  Enable CPU WFI 
	//WFI:Wait For Interrupt
	//WFE:Wait For Event(close)
	SYSCTL_PMUCSR |= BIT(20);
	//Control PWM 8 Divider
	PWM_CTR0_1 = 0x101;
	PWM_CTR2_3 = 0x101;
	PWM_DCR0_1 = 0x101;
	PWM_DCR2_3 = 0x101;
	PWM_DCR4_5 = 0x101;
	PWM_DCR6_7 = 0x101;
	PWM_CTRL = 0xff01;
	//delay 800us run 24/8M PWM(3M)
	vDelayXus(200);
	vDelayXus(200);
	vDelayXus(200);
	vDelayXus(200);
	//RTC WAKEUP
	RTC_CCR0 &= ~RTC_CCR_EN;
	Set_RTC_MatchVal(Delay_RTC_Timer + Get_RTC_CountVal());
	RTC_CCR0 |= RTC_CCR_EN | RTC_CCR_IEN | RTC_CCR_WEN | RTC_CCR_PSCLREN;
	//CPSR set 1 sec
	Set_RTC_PreScaler(LOW_CHIP_CLOCK);
	//SET Enable DeepSleep and Switch PLL
	//BIT0:1:SWITCH Internel OSC32k/196M(0:switch externel OSC32k/pll)
	//BIT1:0:Disable Internel OSC192M(1:ENable Internel OSC192M)
	//BIT2:1:Sleep Main Freq Set High Clock(0:Sleep Main Freq Set Low Clock)
	//BIT3:0:Sleep No close High Clock(1:wait main freq set low clock(bit2=0),close high clock)
	SYSCTL_SWITCH_PLL = BIT3 | BIT0;
	//mode 0 pwm(bit11)run
	SYSCTL_MODEN0 = PWM_EN;
	//modle 1 spif(bit3)run apb(11)run cache(19)run 
	SYSCTL_MODEN1 = SPIF_EN | APB_EN | CACHE_EN | RTC_EN;
	CPU_Sleep();
}
#endif