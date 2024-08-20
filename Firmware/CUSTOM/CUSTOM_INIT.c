/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-07-29 10:06:31
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
#include "CUSTOM_INIT.H"
#include "CUSTOM_FAN.H"
#include "AE_ANX7447_UCSI.H"

/****************************************************************************
* SPKAE10X Init FLOW :
* 1. DoubleBoot (Custom Configuration Double Boot Addr，Function Is Get_DoubleBoot_ADDR() In File CUSTOM_INIT.c)
* 2. Default  Config(Custom Configuration All ,Function Is Default_Config() In File CUSTOM_INIT.c)
* 3. Modlue Init (Custom Configuration All ,Function Is Module_init() In File KERNEL_SOC_FUNC.c)
* 4. Interrupt Configuration (Custom Configuration All , Function Is Irqc_init() In File KERNEL_IRQ.c)
* 5. return start and goto main() function
****************************************************************************/
/*
 * @brief 提供初始化 boot的id（boot id），boot跳转地址
 * @id boot0 id: = NULL(不跳转,默认)
 * @id boot1 id: = 0xff0(直接跳转)
 * @id boot2 id: = 0xff1(看门狗复位则跳转)
 * @id boot3 id: = 0xff2(PWRSW 超时复位)
 * @return (id<<20)|(addr)
 */
FUNCT_PTR_V_V Get_DoubleBoot_ADDR(void)
{
	register FUNCT_PTR_V_V db_ptr = NULL;
#ifdef DUBLE_FIRMWARE1
	db_ptr = (FUNCT_PTR_V_V)ADDR_OR_ID(DoubleBoot_DEF_ADDR, 0xFF1); // 0xff1,ff3,ff5
#elif defined(DUBLE_FIRMWARE2)
	db_ptr = (FUNCT_PTR_V_V)NULL;
#else
	db_ptr = (FUNCT_PTR_V_V)NULL;
#endif
	return db_ptr;
}
/*
 * @brief 如果开启启动低功耗，可通过此函数配置模块时钟默认开启状态
 * @id boot0 id: = 0xff1
 * @return (id<<20)|(addr)
 */
void Default_ModuleClock_LowPower()
{
	SYSCTL_MODEN0 &= ~(SPIM_EN); // SPIM
#if (defined(AE102) || defined(AE103))
	SYSCTL_MODEN0 &= ~(CAN0_EN | CAN1_EN | CAN2_EN | CAN3_EN); // CAN0-3
	SYSCTL_MODEN0 &= ~(UART1_EN | UART2_EN | UART3_EN);		   // UART1-3
	SYSCTL_MODEN0 &= ~(PP_EN);								   // PPORT
#endif
	SYSCTL_MODEN0 &= ~(UARTA_EN | UARTB_EN | UART0_EN); // UART0AB
	SYSCTL_MODEN0 &= ~(PWM_EN);							// PWM
	SYSCTL_MODEN0 &= ~(ADC_EN);							// ADC
	SYSCTL_MODEN0 &= ~(WDT_EN);							// WDT
#if (defined(AE103))
	SYSCTL_MODEN1 &= ~(CEC_EN | SM4_EN); // CEC
#endif
	SYSCTL_MODEN0 &= ~(SMB0_EN | SMB1_EN | SMB2_EN | SMB3_EN); // SMBUS3=0
	SYSCTL_MODEN0 &= ~(GPIO_EN);// GPIO
	SYSCTL_MODEN0 &= ~(BRAM_EN);					 // BRAM
	SYSCTL_MODEN0 &= ~(SWUC_EN);					 // SWUC
	SYSCTL_MODEN0 &= ~(SHM_EN);						 // SHM
	SYSCTL_MODEN0 &= ~(PMCKBC_EN);					 // PMCKBC
	SYSCTL_MODEN0 &= ~(PS2_0_EN | PS2_1_EN);		 // PS2_0
	SYSCTL_MODEN0 &= ~(KBS_EN);						 // KBS
	SYSCTL_MODEN0 &= ~(TMR1_EN | TMR2_EN | TMR3_EN); // TIMER3-1
	SYSCTL_MODEN1 &= ~(TMR0_EN); // TIMER0
	SYSCTL_MODEN1 &= ~(ICTL_EN);// ICTL is necessary
	// SYSCTL_MODEN1 &= ~(H2E_EN);// H2E is necessary
	// SYSCTL_MODEN1&=~(SPIF_EN);// SPIF is necessary
	SYSCTL_MODEN1 &= ~(SRAM_EN);// SRAM
	SYSCTL_MODEN1 &= ~(GPIODB_EN);// GPIODB
#if defined(AE103)
	// SYSCTL_MODEN1 &= ~(EFUSE_EN);
	// SYSCTL_MODEN1 &= ~(SM2_EN | SM3_EN);
#endif
	// SYSCTL_MODEN1&=~(SYSCTL_EN);// SYSCTL is necessary
	// SYSCTL_MODEN1&=~(DRAM_EN);// DRAM is necessary
	// SYSCTL_MODEN1&=~(APB_EN);// PB is necessary
#if defined(AE103)
	//SYSCTL_MODEN1 &= ~(ESPI_EN);
	// SYSCTL_MODEN1 &= ~(TRNG_EN);
	SYSCTL_MODEN1 &= ~(SMB4_EN | SMB5_EN);
	SYSCTL_MODEN1 &= ~(CEC_EN);
	SYSCTL_MODEN1 &= ~(OWI_EN);
	// SYSCTL_MODEN1 &= ~(CACHE_EN);
	// SYSCTL_MODEN1 &= ~(LPCMON_EN);
	// SYSCTL_MODEN1 &= ~(ROM_EN);
	// SYSCTL_MODEN1 &= ~(RTC_EN);
	// SYSCTL_MODEN1 &= ~(PECI_EN);
#endif

}
/*
 * @brief 如果开启启动低功耗，可通过以下两个函数配置引脚默认状态
 * @function1 ：Default_PinIO_Set
 * @function2 ：Default_GPIO_LowPower
 */
void Default_PinIO_Set(int val, int GPIO, int idx, int lens)
{
	for(register int i = 0; i <= lens; i++)
	{
		GPIO_Input_EN(GPIO, idx + i, (!((val & BIT(i)) >> i)));
	}
}
void Default_GPIO_LowPower()
{
	Default_PinIO_Set(PinA0_7_InOut, GPIOA, 0, 7);
	Default_PinIO_Set(PinA8_15_InOut, GPIOA, 8, 7);
	Default_PinIO_Set(PinA16_23_InOut, GPIOA, 16, 7);
	Default_PinIO_Set(PinA24_31_InOut, GPIOA, 24, 7);
	Default_PinIO_Set(PinB0_7_InOut, GPIOB, 0, 7);
	Default_PinIO_Set(PinB8_15_InOut, GPIOB, 8, 7);
	Default_PinIO_Set(PinB16_23_InOut, GPIOB, 16, 7);
#if (defined(AE101)||defined(AE102))
	Default_PinIO_Set(PinB24_26_InOut, GPIOB, 24, 2);
#elif defined(AE103)
	Default_PinIO_Set(PinB24_31_InOut, GPIOB, 24, 7);
#endif
	Default_PinIO_Set(PinC0_7_InOut, GPIOC, 0, 7);
#if (defined(AE101)||defined(AE102))
	Default_PinIO_Set(PinC8_13_InOut, GPIOC, 8, 5);
	Default_PinIO_Set(PinD0_6_InOut, GPIOD, 0, 6);
#elif defined(AE103)
	Default_PinIO_Set(PinC8_15_InOut, GPIOC, 8, 7);
	Default_PinIO_Set(PinD0_7_InOut, GPIOD, 0, 7);
	Default_PinIO_Set(PinD8_InOut, GPIOD, 8, 1);
#endif

	Default_PinIO_Set(PinE0_7_InOut, GPIOE, 0, 7);
	Default_PinIO_Set(PinE8_15_InOut, GPIOE, 8, 7);
	Default_PinIO_Set(PinE16_23_InOut, GPIOE, 16, 7);
}
/*
 * @brief 配置PLL默认时钟
 */
void Default_Freq(void)
{
#if defined(AE103)
#if (OSC_CLOCK_SWITCH == 1)
	SYSCTL_SWITCH_PLL &= ~(BIT0 | BIT1);						  // 切换外部并关闭内部osc192M（低功耗考虑）
	// SYSCTL_RESERVER = 0x40800000 | ((CHIP_CLOCK_SWITCH - 1) << 16) | PLL_ENF2 | PLL_POSTDIV2; // PLL分频
#else
	// SYSCTL_SWITCH_PLL |= (BIT0 | BIT1);			  // 切换内部时钟
	SYSCTL_CLKDIV_OSC80M = (CHIP_CLOCK_SWITCH - 1); // 配置内部时钟分频
#endif
#else
	// SYSCTL_RESERVER = 0x40800000 | ((CHIP_CLOCK_SWITCH - 1) << 16) | PLL_ENF2 | PLL_POSTDIV2;
#endif
	nop;
	nop;
	nop;
}
/*
 * @brief 如果启动低功耗，可通过此函数配置
 */
void SECTION(".init.Default") Default_Config()
{
	// 默认频率配置
	Default_Freq();
	// 低功耗模式配置
#if SUPPORT_LOWPOWER
	Default_ModuleClock_LowPower();
	Default_GPIO_LowPower();
	SYSCTL_PWRSWCSR = PWRSW_EN | PWRSW_WDTIME_1000ms | PWRSW_INT_WDT | PWRSW_DBTIMEL_64ms; // 使能PWR超时计时，1000ms，有wdt中断，
#endif
}
//----------------------------------------------------------------------------
// FUNCTION: Device_init
// edevice enable
//----------------------------------------------------------------------------
void Device_init(void)
{
	// device PD CHIP Init
#if SUPPORT_ANX7447
	u8 ret = ucsi_init();
	if(ret != UCSI_COMMAND_SUCC)
	{
		assert_print("ucsi_init failed.\n");
		// return UCSI_COMMAND_FAIL;
	}
#endif
#if (PWM_MODULE_EN)
#if (SUPPORT_FAN1&&(FAN1_PWM_CHANNEL_SWITCH<=7))
	FAN_Init(FAN1_PWM_CHANNEL_SWITCH, PWM_CLK0, PWM_CTR0);
#endif
#if SUPPORT_FAN2&&(FAN1_PWM_CHANNEL_SWITCH<=7)
	FAN_Init(FAN2_PWM_CHANNEL_SWITCH, PWM_CLK0, PWM_CTR0);
#endif
#endif
#if SUPPORT_LD_PNP_DEVBOARD
	LogicalDevice_PNP_Config();
#if SUPPORT_SHAREMEM_PNP
	ShareMem_PNP_Config();
#endif
#endif
} // 初始化配置
