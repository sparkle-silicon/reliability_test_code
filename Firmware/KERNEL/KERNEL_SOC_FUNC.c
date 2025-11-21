/*
 * @Author: Iversu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2025-11-08 16:09:53
 * @Description: This is about the  national crypto algorithm implementation
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
#include "KERNEL_INCLUDE.H"
#include "CUSTOM_INCLUDE.H"

void gpio_init(void)
{
#if GPIO_MODULE_EN
	gpio_ModuleClock_EN;
	SYSCTL_CLKDIV_GPIODB = GPIODB_CLOCK_DIVISION; // set clock division to 0
	gpiodb_ModuleClock_EN;
	// 1.default config pin iomux and pull up
	pin_DefaultConfig();
	// 2.default config gpio  Voltage Signal and IO Direction
	gpio_DefaultConfig();
#endif
}
void mailbox_init(void)
{
#if MAILBOX_MODULE_EN
	mailbox_ModuleClock_EN;
	Mailbox_Init();
#endif
}
void uart_init(void)
{
#if UART_MODULE_EN
	SYSCTL_CLKDIV_UART = UART_CLOCK_DIVISION;
	int flag = 0;
	int baud[4];
#ifdef UART0_BAUD
	uart0_ModuleClock_EN;
	sysctl_iomux_uart0();
	flag |= BIT0;
	baud[0] = serial_init(UART0_CHANNEL, UART0_BAUD);
#endif
#ifdef UART1_BAUD
	uart1_ModuleClock_EN;
	sysctl_iomux_uart1(UART1_TX_SEL, UART1_RX_SEL);
	flag |= BIT1;
	baud[1] = serial_init(UART1_CHANNEL, UART1_BAUD);
#endif
#ifdef UARTA_BAUD
	uarta_ModuleClock_EN;
	sysctl_iomux_uarta(UARTA_TX_SEL, UARTA_RX_SEL);
	flag |= BIT2;
	baud[2] = serial_init(UARTA_CHANNEL, UARTA_BAUD);
#endif
#ifdef UARTB_BAUD
	uartb_ModuleClock_EN;
	sysctl_iomux_uartb();
	flag |= BIT3;
	baud[3] = serial_init(UARTB_CHANNEL, UARTB_BAUD);
#endif
	for (int cnt = 0; cnt < 4; cnt++)
	{
		if (flag & BIT(cnt))
		{
			dprint("Actual baud rate of the serial port %X == %d.\n", ((cnt < 2) ? cnt : (cnt + 8)), baud[cnt]);
		}
	}
	dprint("UART init done.\n");
#endif
}
void smbus_init(void)
{
#if I2C_MODULE_EN
	SYSCTL_CLKDIV_SMB = SMUBUS_CLOCK_DIVISION;
#if I2C0_EN_Init||((DEBUGGER_OUTPUT_SWITCH == 1)&&(DEBUGGER_I2C_CHANNEL==I2C_CHANNEL_0))
	smbus0_ModuleClock_EN;
	sysctl_iomux_i2c0(I2C0_CLK_SEL, I2C0_DAT_SEL);
	i2c0_pull_up(I2C0_CLK_SEL, I2C0_DAT_SEL);
	I2c_Channel_Init(I2C_CHANNEL_0, I2C0_SPEED, I2C0_DEFAULT_ROLE, I2C0_DEFAULT_ADDR, 1);
#endif
#if I2C1_EN_Init||((DEBUGGER_OUTPUT_SWITCH == 1)&&(DEBUGGER_I2C_CHANNEL==I2C_CHANNEL_1))
	smbus1_ModuleClock_EN;
	sysctl_iomux_i2c1();
	i2c1_pull_up();
	I2c_Channel_Init(I2C_CHANNEL_1, I2C1_SPEED, I2C1_DEFAULT_ROLE, I2C1_DEFAULT_ADDR, 1);
#endif
#if I2C2_EN_Init||((DEBUGGER_OUTPUT_SWITCH == 1)&&(DEBUGGER_I2C_CHANNEL==I2C_CHANNEL_2))
	smbus2_ModuleClock_EN;
	sysctl_iomux_i2c2(I2C2_CLK_SEL);
	i2c2_pull_up(I2C2_CLK_SEL);
	I2c_Channel_Init(I2C_CHANNEL_2, I2C2_SPEED, I2C2_DEFAULT_ROLE, I2C2_DEFAULT_ADDR, 1);
#endif
#if I2C3_EN_Init||((DEBUGGER_OUTPUT_SWITCH == 1)&&(DEBUGGER_I2C_CHANNEL==I2C_CHANNEL_3))
	smbus3_ModuleClock_EN;
	sysctl_iomux_i2c3();
	i2c3_pull_up();
	I2c_Channel_Init(I2C_CHANNEL_3, I2C3_SPEED, I2C3_DEFAULT_ROLE, I2C3_DEFAULT_ADDR, 1);
#endif
#if I2C4_EN_Init||((DEBUGGER_OUTPUT_SWITCH == 1)&&(DEBUGGER_I2C_CHANNEL==I2C_CHANNEL_4))
	smbus4_ModuleClock_EN;
	sysctl_iomux_i2c4();
	i2c4_pull_up();
	I2c_Channel_Init(I2C_CHANNEL_4, I2C4_SPEED, I2C4_DEFAULT_ROLE, I2C4_DEFAULT_ADDR, 1);
#endif
#if I2C5_EN_Init||((DEBUGGER_OUTPUT_SWITCH == 1)&&(DEBUGGER_I2C_CHANNEL==I2C_CHANNEL_5))
	smbus5_ModuleClock_EN;
	sysctl_iomux_i2c5();
	i2c5_pull_up();
	I2c_Channel_Init(I2C_CHANNEL_5, I2C5_SPEED, I2C5_DEFAULT_ROLE, I2C5_DEFAULT_ADDR, 1);
#endif
#if I2C6_EN_Init||((DEBUGGER_OUTPUT_SWITCH == 1)&&(DEBUGGER_I2C_CHANNEL==I2C_CHANNEL_6))
	smbus6_ModuleClock_EN;
	sysctl_iomux_i2c6();
	i2c6_pull_up();
	I2c_Channel_Init(I2C_CHANNEL_6, I2C6_SPEED, I2C6_DEFAULT_ROLE, I2C6_DEFAULT_ADDR, 1);
#endif
#if I2C7_EN_Init||((DEBUGGER_OUTPUT_SWITCH == 1)&&(DEBUGGER_I2C_CHANNEL==I2C_CHANNEL_7))
	smbus7_ModuleClock_EN;
	sysctl_iomux_i2c7();
	i2c7_pull_up();
	I2c_Channel_Init(I2C_CHANNEL_7, I2C7_SPEED, I2C7_DEFAULT_ROLE, I2C7_DEFAULT_ADDR, 1);
#endif
#if I2C8_EN_Init||((DEBUGGER_OUTPUT_SWITCH == 1)&&(DEBUGGER_I2C_CHANNEL==I2C_CHANNEL_8))
	smbus8_ModuleClock_EN;
	sysctl_iomux_i2c8();
	i2c8_pull_up();
	I2c_Channel_Init(I2C_CHANNEL_8, I2C8_SPEED, I2C8_DEFAULT_ROLE, I2C8_DEFAULT_ADDR, 1);
#endif
#if (DEBUGGER_OUTPUT_SWITCH == 1)//初始化调试器//因为是调试手段,所以尽早初始化(UART因为不需要考虑地址,只需要波特率因此相对简单)
	I2c_Channel_Init(DEBUGGER_I2C_CHANNEL, DEBUGGER_I2C_SPEED, I2C_SLAVE_ROLE, DEBUGGER_I2C_ADDRESS, 1);
	SMBUSn_INTR_MASK0(DEBUGGER_I2C_CHANNEL) |= I2C_INTR_RX_FULL;
#endif
	dprint("SMBUS init done.\n");
#endif
}
void i3c_init(void)
{
#if I3C_MODULE_EN
	// SYSCTL_CLKDIV_I3C = I3C_CLOCK_DIVISION;
	/****************** slave init(only I3C2 and I3C3) ******************/
#if I3C2_EN_Init
	i3c2_ModuleClock_EN;
	sysctl_iomux_slave0();
	I3C_Slave_Init(I3C_SLAVE0_STATIC_ADDR, I3C_SLAVE0_DEFAULT_IDPARTNO, I3C_SLAVE0_DEFAULT_DCR, I3C_SLAVE0_DEFAULT_BCR, I3C_SLAVE0);
#endif
#if I2C3_EN_Init
	i3c3_ModuleClock_EN;
	sysctl_iomux_slave1();
	I3C_Slave_Init(I3C_SLAVE1_STATIC_ADDR, I3C_SLAVE1_DEFAULT_IDPARTNO, I3C_SLAVE1_DEFAULT_DCR, I3C_SLAVE1_DEFAULT_BCR, I3C_SLAVE1);
#endif
	dprint("I3C slave init done.\n");
	/****************** master init(only I3C0 and I3C1) ******************/
#if I3C0_EN_Init
	i3c0_ModuleClock_EN;
	sysctl_iomux_master0();
	i3c0_pull_up();
	I3C_WAIT_SDA_PU(I3C_MASTER0);	//需要等SCL/SDA都拉高后才能进行初始化，否则会误触发IBI中断
	if (I3C_MASTER0_MODE == I3C_MASTER_I3C_MODE)
	{
		I3C_SDR_Master_Init(SDR_DEFAULT_SPEED, I3C_MASTER0);
		I3C_MASTER_ENTDAA(I3C_MASTER0_DEFAULT_DCT, I3C_MASTER0_DEFAULT_DYNAMICADDR, I3C_MASTER0);//specify a dynamic addr
		dprint("i3c master0 as i3c mode init\n");
	}
	else if (I3C_MASTER0_MODE == I3C_MASTER_I2C_MODE)
	{
		I3C_Legacy_Master_Init(I3C_LEGACY_SPEED, I3C_MASTER0);
		dprint("i3c master0 as i2c mode init\n");
	}
#endif
#if I3C1_EN_Init
	i3c1_ModuleClock_EN;
	sysctl_iomux_master1();
	i3c1_pull_up();
	I3C_WAIT_SDA_PU(I3C_MASTER1);	//需要等SCL/SDA都拉高后才能进行初始化，否则会误触发IBI中断
	if (I3C_MASTER1_MODE == I3C_MASTER_I3C_MODE)
	{
		I3C_SDR_Master_Init(SDR_DEFAULT_SPEED, I3C_MASTER1);
		I3C_MASTER_ENTDAA(I3C_MASTER1_DEFAULT_DCT, I3C_MASTER1_DEFAULT_DYNAMICADDR, I3C_MASTER1);//specify a dynamic addr
		dprint("i3c master1 as i3c mode init\n");
	}
	else if (I3C_MASTER1_MODE == I3C_MASTER_I2C_MODE)
	{
		I3C_Legacy_Master_Init(I3C_LEGACY_SPEED, I3C_MASTER1);
		dprint("i3c master1 as i2c mode init\n");
	}
#endif
	dprint("I3C master init done.\n");
	dprint("I3C init done.\n");
#endif
}
void spi_init(void)
{
#if SPI_MODULE_EN
#if SPIM_EN_Init
	SYSCTL_CLKDIV_SPIM = SPIM_CLOCK_DIVISION;
	spim_ModuleClock_EN;
	sysctl_iomux_spim(SPIM_MOSI_SEL, SPIM_MISO_SEL, SPIM_QE_SEL);
#if SPIM_CS_EN
	/*By default, a slave device does not multiplexing the chip select signal and utilizes hardware
	circuitry to manage the chip select signal. For two or more devices, the default
	configuration involves the use of two chip select signals for control. If there are specific
	requirements, it is possible to disable the reuse of unnecessary chip select signals.*/
	sysctl_iomux_spim_cs(SPIM_CS0_SEL, SPIM_CS1_SEL);
#endif
	SPIM_Init(SPIM_MODE_STANDARD, SPIM_DEFAULT_CPHA, SPIM_DEFAULT_CPOL, SPIM_DEFAULT_DLY, SPIM_DEFAULT_DSSP, SPIM_DEFAULT_LSB, SPIM_DEFAULT_CPSRR, SPIM_DEFAULT_SAMDLY);
#endif
	dprint("SPI Master init done.\n");
#if SPIFE_EN_Init
	spif_ModuleClock_EN;
	if (SYSCTL_PIO_CFG & BIT1)//使用外部FLASH
	{
		SPIFI_Init();//内部SPIF无法控制,因此主要是内部引脚开关,内部FLASH运行状态之类的控制
		dprint("INTERNAL FLASH init done.\n");
	}
	else//使用内部FLASH才能初始化
	{
		sysctl_iomux_spif(SPIFE_CSN_SEL, SPIFE_QE_SEL, SPIFE_WP_SEL);
	}
	SPIFE_Init();//初始化外部FLASH的一些细节,注意,如果使用外部FLASH可能会和cache冲突
	dprint("EXTERNAL FLASH init done.\n");
#endif
	dprint("SPI init done.\n");
#endif

}
void pwm_tach_init(void)
{
#if (TACH_MODULE_EN | PWM_MODULE_EN)
	pwm_ModuleClock_EN;
#endif
#if (PWM_MODULE_EN)
	PWM_CLOCK_Init();
#if (PWM0_EN_Init)
	sysctl_iomux_pwm0();
	PWM_Init_channel(PWM_CHANNEL0, PWM_LOW, PWM_CLK0, PWM_CTR0, 5, 0);
#endif
#if (PWM1_EN_Init)
	sysctl_iomux_pwm1();
	PWM_Init_channel(PWM_CHANNEL1, PWM_HIGH, PWM_CLK0, PWM_CTR0, 5, 0);
#endif
#if (PWM2_EN_Init)
	sysctl_iomux_pwm2();
	PWM_Init_channel(PWM_CHANNEL2, PWM_HIGH, PWM_CLK1, PWM_CTR1, 5, 0);
#endif
#if (PWM3_EN_Init)
	sysctl_iomux_pwm3();
	PWM_Init_channel(PWM_CHANNEL3, PWM_HIGH, PWM_CLK1, PWM_CTR1, 5, 0);
#endif
#if (PWM4_EN_Init)
	sysctl_iomux_pwm4();
	PWM_Init_channel(PWM_CHANNEL4, PWM_LOW, PWM_CLK2, PWM_CTR2, 5, 0);
#endif
#if (PWM5_EN_Init)
	sysctl_iomux_pwm5();
	PWM_Init_channel(PWM_CHANNEL5, PWM_LOW, PWM_CLK2, PWM_CTR2, 5, 0);
#endif
#if (PWM6_EN_Init)
	sysctl_iomux_pwm6();
	PWM_Init_channel(PWM_CHANNEL6, PWM_LOW, PWM_CLK3, PWM_CTR3, 5, 0);
#endif
#if (PWM7_EN_Init)
	sysctl_iomux_pwm7();
	PWM_Init_channel(PWM_CHANNEL7, PWM_LOW, PWM_CLK3, PWM_CTR3, 5, 0);
#endif
	dprint("PWM init done\n");
#endif
#if (TACH_MODULE_EN)
#if (TACH0_EN)
	sysctl_iomux_tach0(TACH0_PIN_SEL);
	tach0_pull_up(TACH0_PIN_SEL);
	TACH_Init_Channel(TACH_CHANNEL0, 1, 0);
#endif
#if (TACH1_EN)
	sysctl_iomux_tach1(TACH1_PIN_SEL);
	tach1_pull_up(TACH1_PIN_SEL);
	TACH_Init_Channel(TACH_CHANNEL1, 1, 0);
#endif
#if (TACH2_EN)
	sysctl_iomux_tach2();
	tach2_pull_up();
	TACH_Init_Channel(TACH_CHANNEL2, 1, 0);
#endif
#if (TACH3_EN)
	sysctl_iomux_tach3(TACH3_PIN_SEL);
	tach3_pull_up(TACH3_PIN_SEL);
	TACH_Init_Channel(TACH_CHANNEL3, 1, 0);
#endif
	dprint("TACH init done\n");
#endif
}
void host_init(void)
{
#if HOST_MODULE_EN
#if ESPI_EN_Init
	espi_ModuleClock_EN;
	//Switch espi
	sysctl_iomux_host(1, HOST_IRQ_PIN_SEL, HOST_RST_PIN_SEL, 0, 0);
	ESPI_Init();
	dprint("ESPI init done.\n");
#elif LPC_EN_Init
	// LPC_ModuleClock_EN;
		//Switch lpc
	sysctl_iomux_host(0, HOST_IRQ_PIN_SEL, HOST_RST_PIN_SEL, LPC_CLKRUN_PIN_SEL, LPC_PD_PIN_SEL);
	//lpc init
	dprint("LPC init done.\n");
#else//按照默認選擇初始化
	sysctl_iomux_host(1, 0, 0, 0, 0);
	dprint("Default host iomux init done.\n");
#endif
#if SWUC_EN_Init
	swuc_ModuleClock_EN;
	sysctl_iomux_swuc(SWUC_GA20_ENABLE, SWUC_KRST_ENABLE, SWUC_PWUREQ_ENABLE, HOST_SMI_PLTRST_SWITCH);
	SWUC_Init(SWUC_GA20_ENABLE, SWUC_GA20_MODE, SWUC_KRST_ENABLE, HOST_SMI_PLTRST_SWITCH, SWUC_PWUREQ_ENABLE, SWUC_ACPI_ENABLE);
	dprint("SWUC init done.\n");
#endif
#if HOST_P80_En_Init
	sysctl_iomux_l80();
	L80_Init();
#endif
#if (KBC_MODULE_EN||PMC_MODULE_EN)
	pmckbc_ModuleClock_EN;
#endif
#if (KBC_MODULE_EN)
	kbc_init();
	dprint("KBC init done.\n");
#endif
#if (PMC_MODULE_EN)
	/* If you want to use GPIO to control the chip select signal,
	 you can comment out the following two lines
	 (CUSTOM_GPIO.H is configured for GPIO by default).*/
	sysctl_iomux_sci(HOST_SCI_PIN_SEL);
	pmc_init();
	dprint("PMC init done.\n");
#endif
#if (PECI_MODULE_EN)
	SYSCTL_CLKDIV_PECI = PECI_CLOCK_DIVISION;
	peci_ModuleClock_EN;
	sysctl_iomux_peci();
	PECI_Init();
	dprint("PECI init done.\n");
#endif
#endif
}
void kbs_init(void)
{
#if (KBS_MODULE_EN)
	kbs_ModuleClock_EN;
	sysctl_iomux_kbs(KBD_8_n_SWITCH); // KBD_8_n_SWITCH
	kbs_pull_up(KBD_8_n_SWITCH);
	KBS_Init();
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_KBS_PRESS, en, 1, 0);
	dprint("KBS init done.\n");
#endif
}
void ps2_init(void)
{
#if (PS2_MODULE_EN)
	SYSCTL_CLKDIV_PS2 = PS2_CLOCK_DIVISION;
#if PS2_0_EN_Init
	ps2_0_ModuleClock_EN;
	sysctl_iomux_ps2_0(PS2_0_CLK_SEL, PS2_0_DAT_SEL);
	ps2_0_pull_up(PS2_0_CLK_SEL, PS2_0_DAT_SEL);
	PS2_PortN_Init(PS2_0_CHANNEL);//仅初始化通道,Device初始化应该由实际情况实现
#endif
#if PS2_1_EN_Init
	ps2_1_ModuleClock_EN;
	sysctl_iomux_ps2_1(PS2_1_CLK_SEL, PS2_1_DAT_SEL);
	ps2_1_pull_up(PS2_1_CLK_SEL, PS2_1_DAT_SEL);
	PS2_PortN_Init(PS2_1_CHANNEL);//仅初始化通道,Device初始化应该由实际情况实现
#endif
	dprint("PS2 init done.\n");
#endif
}
void cec_init(void)
{
#if (CEC_MODULE_EN)
	SYSCTL_CLKDIV_CEC = CEC_CLOCK_DIVISION; // 24M clock
	cec_ModuleClock_EN;
#if CEC0_EN_Init
	sysctl_iomux_cec0(CEC0_PIN_SEL);
	cec0_pull_up(CEC0_PIN_SEL);
	CEC_Init(CEC0_CHANNEL, CEC0_MODE_SWITCH);
#endif
#if CEC1_EN_Init
	sysctl_iomux_cec1();
	cec1_pull_up();
	CEC_Init(CEC1_CHANNEL, CEC1_MODE_SWITCH);
#endif
	dprint("CEC init done.\n");
#endif
}
void adc_init(void)
{
#if ADC_MODULE_EN
	SYSCTL_CLKDIV_OSC96M_ADC = ADC_CLOCK_DIVISION;
	adc_ModuleClock_EN;
#if (ADC0_EN_Init)
	sysctl_iomux_adc0();
	ADC_SW_Sample_Init_Single(ADC_CHANNEL0, ADC_DATA_CHANNEL0, ADC_SINGLEENDED);
#endif
#if (ADC1_EN_Init)
	sysctl_iomux_adc1();
	ADC_SW_Sample_Init_Single(ADC_CHANNEL1, ADC_DATA_CHANNEL1, ADC_SINGLEENDED);
#endif
#if (ADC2_EN_Init)
	sysctl_iomux_adc2();
	ADC_SW_Sample_Init_Single(ADC_CHANNEL2, ADC_DATA_CHANNEL2, ADC_SINGLEENDED);
#endif
#if (ADC3_EN_Init)
	sysctl_iomux_adc3();
	ADC_SW_Sample_Init_Single(ADC_CHANNEL3, ADC_DATA_CHANNEL3, ADC_SINGLEENDED);
#endif
#if (ADC4_EN_Init)
	sysctl_iomux_adc4();
	ADC_SW_Sample_Init_Single(ADC_CHANNEL4, ADC_DATA_CHANNEL4, ADC_SINGLEENDED);
#endif
#if (ADC5_EN_Init)
	sysctl_iomux_adc5();
	ADC_SW_Sample_Init_Single(ADC_CHANNEL5, ADC_DATA_CHANNEL5, ADC_SINGLEENDED);
#endif
#if (ADC6_EN_Init)
	sysctl_iomux_adc6();
	ADC_SW_Sample_Init_Single(ADC_CHANNEL6, ADC_DATA_CHANNEL6, ADC_SINGLEENDED);
#endif
#if (ADC7_EN_Init)
	sysctl_iomux_adc7();
	ADC_SW_Sample_Init_Single(ADC_CHANNEL7, ADC_DATA_CHANNEL7, ADC_SINGLEENDED);
#endif
	dprint("ADC init done \n");
#endif
}
void time_init(void)
{
#if TIMER_MODULE_EN
	SYSCTL_CLKDIV_TMR0 = TIMER0_CLOCK_DIVISION;
	timer0_ModuleClock_EN;
	TIMER_Init(TIMER0, 1, 0x0, 0x1); // 24Mdelay~= 0.083us,96Mdelay~=0.041us

	SYSCTL_CLKDIV_TMR1 = TIMER1_CLOCK_DIVISION;
	timer1_ModuleClock_EN;			 // ms delay
	TIMER_Init(TIMER1, 1, 0x0, 0x1); // 24Mdelay~= 0.083us,96Mdelay~=0.041us

	SYSCTL_CLKDIV_TMR2 = TIMER2_CLOCK_DIVISION;
	timer2_ModuleClock_EN;			 // 1ms service
	TIMER_Init(TIMER2, 1, 0x0, 0x1); // 24Mdelay~= 0.083us,96Mdelay~=0.041us

	SYSCTL_CLKDIV_TMR3 = TIMER3_CLOCK_DIVISION;
	timer3_ModuleClock_EN;			 // us delay
	TIMER_Init(TIMER3, 1, 0x0, 0x1); // 24Mdelay~= 0.083us,96Mdelay~=0.041us

	while ((TIMER_TRIS & 0xf) != 0xf)
		;
	TIMER_TEOI; // clear all interrupt
	dprint("TIMER init done\n");
#endif

#if (RTC_MODULE_EN)
	rtc_ModuleClock_EN;
	RTC_Init(0, 1, LOW_CHIP_CLOCK);
	dprint("RTC init done.\n");
#endif
#if WDT_MODULE_EN
	/*wdt*/
	wdt_ModuleClock_EN;
	WDT_Init(0x1, 0xa); // FIXME
	dprint("WDT init done.\n");
#endif
}

void __weak SECTION(".init.module") Module_init(void)
{
	// 1.Initialize The GPIO
	gpio_init();
	// 2.Initialize The Mailbox	(crypto cpu and mailbox)
	mailbox_init();
	// 3.Switch Default SMBUS3 or UARTB 
	DEFAULT_SMBUS3_UARTB_SEL;
	// 4.Initialize The Serial Port
	uart_init();
	// 5.Initialize The SMBUS
	smbus_init();
	// 6.Initialize The I3C
	i3c_init();
	// 7.Initialize The SPI (SPIF and SPIM)
	spi_init();
	// 8.Initialize The PWM and The TACH
	pwm_tach_init();
	// 9.Initialize The Host(KBC, PMC, LPC or ESPI, SWUC)
	host_init();
	// 10.Initialize  The KBS
	kbs_init();
	// 11.Initialize  The PS2 Channel
	ps2_init();
	// 12.Initialize The CEC
	cec_init();
	// 13.Initialize The ADC
	adc_init();
	// 14 Initialize The timer and The watch dog and The RTC
	time_init();
	dprint("Module init End.\n");
	return;
}
#if defined(USER_AE10X_LIBC_A)
void exit(int __status)
{
	_exit(__status);
}
NORETURN USED void _exit(int __status)
{
	dprint("exit status doc %d\n", __status);
	while (1)
		;
}
#endif