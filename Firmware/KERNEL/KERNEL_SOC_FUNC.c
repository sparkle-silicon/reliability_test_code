/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-07-17 14:28:14
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
#include "KERNEL_SOC_FUNC.H"
#include "AE_FUNC.H"
#include "AE_CONFIG.H"
#include "CUSTOM_BATTERY.H"
#include "KERNEL_MAILBOX.H"

void ps2_0_frequency(void)
{
	/*init ps2 0 freq */
	PS2_PORT0_CPSR = (((HIGHT_CHIP_CLOCK * 5) / 1000 + 500) / 1000);		 // 5us
	PS2_PORT0_DVR = (((HIGHT_CHIP_CLOCK / PS2_PORT0_CPSR) / 1000 + 5) / 10); // 100us
}
void ps2_1_frequency(void)
{
	/*init ps2 1 freq */
	PS2_PORT1_CPSR = (((HIGHT_CHIP_CLOCK * 5) / 1000 + 500) / 1000);		 // 5us
	PS2_PORT1_DVR = (((HIGHT_CHIP_CLOCK / PS2_PORT0_CPSR) / 1000 + 5) / 10); // 100us
}

void pmc1_init(void)
{
	PMC1_IE = 0x3f;
#if !(LPC_WAY_OPTION_SWITCH)
	PMC1_CTL |= IBF_INT_ENABLE;
#endif
}
void pmc2_init(void)
{
	PMC2_IE = 0x3f;
#if !(LPC_WAY_OPTION_SWITCH)
	PMC2_CTL |= IBF_INT_ENABLE;
#endif
}
void pmc3_init(void)
{
	PMC3_IE = 0x3f;
#if !(LPC_WAY_OPTION_SWITCH)
	PMC3_CTL |= IBF_INT_ENABLE;
#endif
}
void pmc4_init(void)
{
	PMC4_IE = 0x3f;
#if !(LPC_WAY_OPTION_SWITCH)
	PMC4_CTL |= IBF_INT_ENABLE;
#endif
}
void pmc5_init(void)
{
	PMC5_IE = 0x3f;
#if !(LPC_WAY_OPTION_SWITCH)
	PMC5_CTL |= IBF_INT_ENABLE;
#endif
}
void kbc_init(void)
{
	SET_BIT(KBC_CTL, KBC_IBFIE); // set KBC_IBF enable
}
void i2c0_pull_up(void)
{
	GPIO_Pullup_Config(GPIOA, 11);
	GPIO_Pullup_Config(GPIOA, 12);
}
void i2c1_pull_up(void)
{
	GPIO_Pullup_Config(GPIOA, 17);
	GPIO_Pullup_Config(GPIOA, 18);
}
void i2c2_pull_up(void)
{
	GPIO_Pullup_Config(GPIOB, 14);
	GPIO_Pullup_Config(GPIOB, 15);
}
void i2c3_pull_up(void)
{
#if (defined(TEST101) || defined(AE101) || defined(AE102))
	GPIO_Pullup_Config(GPIOB, 21);
	GPIO_Pullup_Config(GPIOB, 22);
#elif (defined(AE103))
	GPIO_Pullup_Config(GPIOB, 25);
	GPIO_Pullup_Config(GPIOB, 26);
#endif
}
void i2c4_pull_up(void)
{
#if (defined(AE103))
	GPIO_Pullup_Config(GPIOA, 4);
	GPIO_Pullup_Config(GPIOA, 5);
#endif
}
void i2c5_pull_up(void)
{
#if (defined(AE103))
	GPIO_Pullup_Config(GPIOB, 0);
	GPIO_Pullup_Config(GPIOB, 7);
#endif
}

void tach0_pull_up(void)
{
	GPIO_Pullup_Config(GPIOA, 30);
}
void tach1_pull_up(void)
{
	GPIO_Pullup_Config(GPIOA, 31);
}
void tach2_pull_up(void)
{
	GPIO_Pullup_Config(GPIOC, 10);
}
void tach3_pull_up(void)
{
	GPIO_Pullup_Config(GPIOC, 11);
}
void ps2_0_pull_up(void)
{
	GPIO_Pullup_Config(GPIOB, 8); // gpd6,d7 pull up
	GPIO_Pullup_Config(GPIOB, 9);
}
void ps2_1_pull_up(void)
{
	GPIO_Pullup_Config(GPIOB, 12);
	GPIO_Pullup_Config(GPIOB, 13);
}
void cec_pull_up(void)
{
	GPIO_Pullup_Config(GPIOB, 8); // gpd6,d7 pull up
}
void kbs_pull_up(void)
{
	for (register unsigned char i = 8; i <= 31; i++)
		GPIO_Pullup_Config(GPIOE, i); // gpe all pull up
#if (KBD_8_n_SWITCH == 17 || KBD_8_n_SWITCH == 18)
	GPIO_Pullup_Config(GPIOA, 19); // C3 kso 16
#endif
#if (KBD_8_n_SWITCH == 18)
	GPIO_Pullup_Config(GPIOA, 21); // C3 kso 17
#endif
}
void gpio_init(void)
{
#if GPIO_MODULE_EN
	gpio_MoudleClock_EN;
	gpiodb_MoudleClock_EN;
	// 1.default config pin iomux and pull up
	pin_DefaultConfig();
	// 2.default config gpio  Voltage Signal and IO Direction
	gpio_DefaultConfig();
#endif
}
void uart_init(void)
{
#if UART_MODULE_EN
	int cnt;
	int flag = 0;
	int baud[6];
#ifdef UART0_BAUD
	uart0_MoudleClock_EN;
	sysctl_iomux_uart0();
	flag |= BIT0;
	baud[0] = serial_init(UART0_CHANNEL, UART0_BAUD);
#endif
#if (defined(AE102) || defined(AE103))
#ifdef UART1_BAUD
	uart1_MoudleClock_EN;
	sysctl_iomux_uart1();
	flag |= BIT1;
	baud[1] = serial_init(UART1_CHANNEL, UART1_BAUD);
#endif
// #ifdef UART2_BAUD
// 	uart2_MoudleClock_EN;
// 	sysctl_iomux_uart2();
// 	flag |= BIT2;
// 	baud[2] = serial_init(UART2_CHANNEL, UART2_BAUD);
// #endif
// #ifdef UART3_BAUD
// 	uart3_MoudleClock_EN;
// 	sysctl_iomux_uart3();
// 	flag |= BIT3;
// 	baud[3] = serial_init(UART3_CHANNEL, UART3_BAUD);
// #endif
#endif
#ifdef UARTA_BAUD
	uarta_MoudleClock_EN;
	sysctl_iomux_uarta();
	flag |= BIT4;
	baud[4] = serial_init(UARTA_CHANNEL, UARTA_BAUD);
#endif
#ifdef UARTB_BAUD
	SMBUS3_UARTB_SEL;
	uartb_MoudleClock_EN;
	sysctl_iomux_uartb();
	flag |= BIT5;
	baud[5] = serial_init(UARTB_CHANNEL, UARTB_BAUD);
#endif
	for (cnt = 0; cnt < 6; cnt++)
	{
		if (flag & BIT(cnt))
		{
			dprint("Actual baud rate of the serial port %X == %d\n", ((cnt < 4) ? cnt : ((cnt == 5) ? 0xB : 0xA)), baud[cnt]);
		}
	}
	dprint("Uart init done.\n");
#endif
}
void smbus_init(void)
{
#if I2C_MODULE_EN
#if I2C0_EN_Init
	smbus0_MoudleClock_EN;
	sysctl_iomux_i2c0();
	i2c0_pull_up();
#if (DEBUGGER_OUTPUT_SWITCH == 1)
#if (DEBUGGER_I2C_CHANNEL == I2C_CHANNEL_0)
	I2c_Channel_Init(I2C_CHANNEL_0, I2C0_SPEED, I2C_SLAVE_ROLE, 0x6A, 1);
	I2C0_INTR_MASK |= I2C_INTR_RX_FULL;
#endif
#else
	I2c_Channel_Init(I2C_CHANNEL_0, I2C0_SPEED, I2C_MASTER_ROLE, 0x6A, 1);
#endif
#endif
#if I2C1_EN_Init
	smbus1_MoudleClock_EN;
	sysctl_iomux_i2c1();
	i2c1_pull_up();
#if (DEBUGGER_OUTPUT_SWITCH == 1)
#if (DEBUGGER_I2C_CHANNEL == I2C_CHANNEL_1)
	I2c_Channel_Init(I2C_CHANNEL_1, I2C1_SPEED, I2C_SLAVE_ROLE, 0x4c, 1);
	I2C1_INTR_MASK |= I2C_INTR_RX_FULL;
#endif
#else
	I2c_Channel_Init(I2C_CHANNEL_1, I2C1_SPEED, I2C_MASTER_ROLE, 0x4C, 1);
#endif
#endif
#if I2C2_EN_Init
	smbus2_MoudleClock_EN;
	sysctl_iomux_i2c2();
	i2c2_pull_up();
#if (DEBUGGER_OUTPUT_SWITCH == 1)
#if (DEBUGGER_I2C_CHANNEL == I2C_CHANNEL_2)
	I2c_Channel_Init(I2C_CHANNEL_2, I2C2_SPEED, I2C_SLAVE_ROLE, 0x00, 1);
	I2C2_INTR_MASK |= I2C_INTR_RX_FULL;
#endif
#else
	I2c_Channel_Init(I2C_CHANNEL_2, I2C2_SPEED, I2C_NO_ROLE, 0x00, 1);
#endif
#endif
#if I2C3_EN_Init
	smbus3_MoudleClock_EN;
	sysctl_iomux_i2c3();
	i2c3_pull_up();
#if (DEBUGGER_OUTPUT_SWITCH == 1)
#if (DEBUGGER_I2C_CHANNEL == I2C_CHANNEL_3)
	I2c_Channel_Init(I2C_CHANNEL_3, I2C3_SPEED, I2C_SLAVE_ROLE, 0x00, 1);
	I2C3_INTR_MASK |= I2C_INTR_RX_FULL;
#endif
#else
	I2c_Channel_Init(I2C_CHANNEL_3, I2C3_SPEED, I2C_NO_ROLE, 0x00, 1);
#endif
#endif
#ifdef AE103
#if I2C4_EN_Init
	smbus4_MoudleClock_EN;
	sysctl_iomux_i2c4();
	i2c4_pull_up();
#if (DEBUGGER_OUTPUT_SWITCH == 1)
#if (DEBUGGER_I2C_CHANNEL == I2C_CHANNEL_4)
	I2c_Channel_Init(I2C_CHANNEL_4, I2C4_SPEED, I2C_SLAVE_ROLE, 0x00, 1);
	I2C4_INTR_MASK |= I2C_INTR_RX_FULL;
#endif
#else
	I2c_Channel_Init(I2C_CHANNEL_4, I2C4_SPEED, I2C_NO_ROLE, 0x00, 1);
#endif
#endif
#if I2C5_EN_Init
	smbus5_MoudleClock_EN;
	sysctl_iomux_i2c5();
	i2c5_pull_up();
#if (DEBUGGER_OUTPUT_SWITCH == 1)
#if (DEBUGGER_I2C_CHANNEL == I2C_CHANNEL_5)
	I2c_Channel_Init(I2C_CHANNEL_5, I2C5_SPEED, I2C_SLAVE_ROLE, 0x00, 1);
	I2C5_INTR_MASK |= I2C_INTR_RX_FULL;
#endif
#else
	I2c_Channel_Init(I2C_CHANNEL_5, I2C5_SPEED, I2C_NO_ROLE, 0x00, 1);
#endif
#endif
#endif
	dprint("I2c_channel_init done.\n");
#endif
}
void spim_init(void)
{
#if SPI_MODULE_EN
	spim_MoudleClock_EN;
	sysctl_iomux_spim();
#if SPIM_CS_EN
	/*By default, a slave device does not multiplexing the chip select signal and utilizes hardware
	circuitry to manage the chip select signal. For two or more devices, the default
	configuration involves the use of two chip select signals for control. If there are specific
	requirements, it is possible to disable the reuse of unnecessary chip select signals.*/
	sysctl_iomux_spim_cs();
#endif
	SPI_Init(0, SPIM_CPOL_LOW, SPIM_CPHA_FE, SPIM_LSB, 0x7, 1);
	dprint("SPI init done.\n");
#endif
}
void pwm_tach_init(void)
{

#if (TACH_MODULE_EN | PWM_MODULE_EN)
	pwm_MoudleClock_EN;
#endif
#if (PWM_MODULE_EN)
	PWM_CLOCK_Init(); // defined PWM_CLKn_PRESCALE PWM_CTRnm PWM_TCLK_PRESCALE config
#if (PWM0_EN_Init)
	sysctl_iomux_pwm0();
	PWM_Init_channel(PWM_CHANNEL0, PWM_LOW, PWM_CLK0, PWM_CTR0, 98, 0);
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
	sysctl_iomux_tach0();
	tach0_pull_up();
	TACH_Init_Channel(TACH_CHANNEL0, 1, 0);
#endif
#if (TACH1_EN)
	sysctl_iomux_tach1();
	tach1_pull_up();
	TACH_Init_Channel(TACH_CHANNEL1, 1, 0);
#endif
#if (TACH2_EN)
	sysctl_iomux_tach2();
	tach2_pull_up();
	TACH_Init_Channel(TACH_CHANNEL2, 1, 0);
#endif
#if (TACH3_EN)
	sysctl_iomux_tach3();
	tach3_pull_up();
	TACH_Init_Channel(TACH_CHANNEL3, 1, 0);
#endif
	dprint("TACH init done\n");
#endif
}
void kbc_pmc_init(void)
{
#if (KBC_MODULE_EN)
	pmckbc_MoudleClock_EN;
	kbc_init();
	dprint("KBC init done.\n");
#endif
#if (PMC_MODULE_EN)
	/* If you want to use GPIO to control the chip select signal,
	 you can comment out the following two lines
	 (CUSTOM_GPIO.H is configured for GPIO by default).*/
#if 0 // 用GPIO模拟SCI
	sysctl_iomux_sci();
	sysctl_iomux_smi();
#endif
	pmc1_init();
	pmc2_init();
	pmc3_init();
	pmc4_init();
	pmc5_init();
	dprint("PMC init done.\n");
#endif
}
void kbs_init(void)
{
#if (KBS_MODULE_EN)
	kbs_MoudleClock_EN;
	sysctl_iomux_kbs(); // KBD_8_n_SWITCH
	kbs_pull_up();
	KayBoardScan_config();
	dprint("Kbd init done.\n");
#endif
}
void ps2_init(void)
{
#if (PS2_MODULE_EN)
#if PS2_0_EN_Init
	ps2_0_MoudleClock_EN;
	ps2_0_frequency();
	//sysctl_iomux_ps2_0();
#if 0 // 上拉配置
	ps2_0_pull_up();
#endif
#endif
#if PS2_1_EN_Init
	ps2_1_MoudleClock_EN;
	ps2_1_frequency();
	//sysctl_iomux_ps2_1();
#if 0 // 上拉配置
	ps2_1_pull_up();
#endif
#endif
	dprint("Ps2 init done.\n");
#endif
}
void pport_init(void)
{
#if (PPORT_MODULE_EN)
	pport_MoudleClock_EN;
	sysctl_iomux_pport();
	// pport reg init function
	dprint("Pport init done.\n");
#endif
}
void can_init(void)
{
#if (CAN_MODULE_EN)
#if CAN0_EN_Init
	can0_MoudleClock_EN;
	sysctl_iomux_can0();
	// can0 reg init function
#endif
#if CAN1_EN_Init
	can1_MoudleClock_EN;
	sysctl_iomux_can1();
	// can0 reg init function
#endif
#if CAN2_EN_Init
	can2_MoudleClock_EN;
	sysctl_iomux_can2();
	// can0 reg init function
#endif
#if CAN3_EN_Init
	can3_MoudleClock_EN;
	sysctl_iomux_can3();
	// can0 reg init function
#endif
	dprint("CAN init done.\n");
#endif
}
void cec_init(void)
{
#if (CEC_MODULE_EN)
	cec_MoudleClock_EN;
	sysctl_iomux_cec();
	cec_pull_up();
#if CEC_mode_select
	CEC_initiator_init();
#else
	CEC_follower_init();
#endif
	dprint("cec init done.\n");
#endif
}
void owi_init(void)
{
#if (OWI_MODULE_EN)
	SYSCTL_CLKDIV_OWI = 0x0; // 时钟1分频,分频数为0base
	owi_MoudleClock_EN;
	sysctl_iomux_owi();
	OWI_Init(128, 30, 20, 10);
	dprint("owi init done.\n");
#endif
}
void rtc_init(void)
{
#if (RTC_MODULE_EN)
	rtc_MoudleClock_EN;
	RTC_Init(0, 1, LOW_CHIP_CLOCK);
	dprint("rtc init done.\n");
#endif
}
void peci_init(void)
{
#if (PECI_MODULE_EN)
	SYSCTL_CLKDIV_PECI = 1;
	peci_MoudleClock_EN;
	sysctl_iomux_peci();
	Init_PECI();
	dprint("peci init done.\n");
#endif
}
void espi_init(void)
{
#if (ESPI_MODULE_EN)
	espi_MoudleClock_EN;
	sysctl_iomux_espi();
	ESPI_Init();
	dprint("espi init done.\n");
#endif
}
void adc_init(void)
{
#if ADC_MODULE_EN
	adc_MoudleClock_EN;
#if (ADC0_EN_Init)
	sysctl_iomux_adc0();
	ADC_SWTriggerInit(ADC_CHANNEL0, 1, 0, 0);
#endif
#if (ADC1_EN_Init)
	sysctl_iomux_adc1();
	ADC_SWTriggerInit(ADC_CHANNEL1, 1, 0, 0);
#endif
#if (ADC2_EN_Init)
	sysctl_iomux_adc2();
	ADC_SWTriggerInit(ADC_CHANNEL2, 1, 0, 0);
#endif
#if (ADC3_EN_Init)
	sysctl_iomux_adc3();
	ADC_SWTriggerInit(ADC_CHANNEL3, 1, 0, 0);
#endif
#if (ADC4_EN_Init)
	sysctl_iomux_adc4();
	ADC_SWTriggerInit(ADC_CHANNEL4, 1, 0, 0);
#endif
#if (ADC5_EN_Init)
	sysctl_iomux_adc5();
	ADC_SWTriggerInit(ADC_CHANNEL5, 1, 0, 0);
#endif
#if (ADC6_EN_Init)
	sysctl_iomux_adc6();
	ADC_SWTriggerInit(ADC_CHANNEL6, 1, 0, 0);
#endif
#if (ADC7_EN_Init)
	sysctl_iomux_adc7();
	ADC_SWTriggerInit(ADC_CHANNEL7, 1, 0, 0);
#endif
	dprint("ADC init done \n");
#endif
}
void time_init(void)
{
#if TIMER_MODULE_EN
	// timer0/1 时钟分频设置 32.768k/(2<<division)
	SYSCTL_CLKDIV_TMR0 = TIMER0_Division;
	timer0_MoudleClock_EN;
	TIMER_Init(TIMER0, 1, 0x0, 0x1); // delay~=65us
	SYSCTL_CLKDIV_TMR1 = TIMER1_Division;
	timer1_MoudleClock_EN;			 // ms delay
	TIMER_Init(TIMER1, 1, 0x0, 0x1); // delay~=65us
	// timer2/3 时钟分频设置 24.576Mhz/(2<<division)
	SYSCTL_CLKDIV_TMR2 = TIMER2_Division;
	timer2_MoudleClock_EN;			 // 1ms service
	TIMER_Init(TIMER2, 1, 0x0, 0x1); // delay~= 0.083us
	SYSCTL_CLKDIV_TMR3 = TIMER3_Division;
	timer3_MoudleClock_EN;			 // us delay
	TIMER_Init(TIMER3, 1, 0x0, 0x1); // delay~=0.083us

	while ((TIMER_TRIS & 0xf) != 0xf)
		;
	TIMER_TEOI; // clear all interrupt
	dprint("Timer init done\n");
#endif
#if WDT_MODULE_EN
	/*wdt*/
	wdt_MoudleClock_EN;
	WDT_Init(0x1, 0xa); // FIXME
	dprint("watchdong init done.\n");
#endif
// 开启定时器对应功能
#if TIMER_MODULE_EN
	TIMER_Init(TIMER2, TIMER2_1ms, 0x1, 0x0); // 1ms service计时函数
#endif
// 复位看门狗
#if WDT_MODULE_EN
	WDT_FeedDog();
#endif
}
void __weak SECTION(".init.module") Module_init(void)
{

	// 1.Obtain The CPU Running Frequency
	get_cpu_freq();

	// 2. Initialize APB CLOCK BUS ENABLE
	apb_MoudleClock_EN;

	// 3.Initialize The GPIO
	// gpio_init();

	// 4.Initialize The Serial Port
	uart_init();

	// 5.Initialize The SMBUS
	smbus_init();
	// 6.Initialize The SPI
	spim_init();
	// 7.Initialize The PWM and The TACH
	pwm_tach_init();
	// 8.Initialize The KBC and The PMC
	kbc_pmc_init();
	// 9.Initialize  The KBS
	kbs_init();
#if (defined(AE102))
	// 11.Initialize The PPort
	pport_init();
	// 12.Initialize The CAN
	can_init();
#endif
#if (defined(AE103) && 0)
	// 13.Initialize The CEC
	cec_init();
	// 14.Initialize The OWI
	owi_init();
	// 15.Initialize The OWI
	rtc_init();
	// 16.Initialize The PECI
	peci_init();
	// 17.Initialize The eSPI
	espi_init();
#endif
	// 17.Initialize The ADC
	// adc_init();
	// 18. Initialize The timer and The watch dog
	time_init();

	// 19.Initialize The Mailboxs
#if (GLE01 == 1)
	mailbox_init();

	SPIF_Init();
	sysctl_iomux_lpc();
#endif

	dprint("End init \n");
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