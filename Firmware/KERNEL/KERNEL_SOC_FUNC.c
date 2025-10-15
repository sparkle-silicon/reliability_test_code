/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-10 18:28:38
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
#include "KERNEL_I3C.H"

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
	int flag = 0;
	int baud[4];
#ifdef UART0_BAUD
	uart0_MoudleClock_EN;
	sysctl_iomux_uart0();
	flag |= BIT0;
	baud[0] = serial_init(UART0_CHANNEL, UART0_BAUD);
#endif
#ifdef UART1_BAUD
	uart1_MoudleClock_EN;
	sysctl_iomux_uart1(UART1_TX_SEL, UART1_RX_SEL);
	flag |= BIT1;
	baud[1] = serial_init(UART1_CHANNEL, UART1_BAUD);
#endif
#ifdef UARTA_BAUD
	uarta_MoudleClock_EN;
	sysctl_iomux_uarta(UARTA_TX_SEL, UARTA_RX_SEL);
	flag |= BIT2;
	baud[2] = serial_init(UARTA_CHANNEL, UARTA_BAUD);
#endif
#ifdef UARTB_BAUD
	uartb_MoudleClock_EN;
	sysctl_iomux_uartb();
	flag |= BIT3;
	baud[3] = serial_init(UARTB_CHANNEL, UARTB_BAUD);
#endif
	for(int cnt = 0; cnt < 4; cnt++)
	{
		if(flag & BIT(cnt))
		{
			dprint("Actual baud rate of the serial port %X == %d.\n", ((cnt < 2) ? cnt : (cnt + 8)), baud[cnt]);
		}
	}
	dprint("Uart init done.\n");
#endif
}
void smbus_init(void)
{
#if I2C_MODULE_EN
#if I2C0_EN_Init||((DEBUGGER_OUTPUT_SWITCH == 1)&&(DEBUGGER_I2C_CHANNEL==I2C_CHANNEL_0))
	smbus0_MoudleClock_EN;
	sysctl_iomux_i2c0(I2C0_CLK_SEL, I2C0_DAT_SEL);
	i2c0_pull_up(I2C0_CLK_SEL, I2C0_DAT_SEL);
	I2c_Channel_Init(I2C_CHANNEL_0, I2C0_SPEED, I2C0_DEFAULT_ROLE, I2C0_DEFAULT_ADDR, 1);
#endif
#if I2C1_EN_Init||((DEBUGGER_OUTPUT_SWITCH == 1)&&(DEBUGGER_I2C_CHANNEL==I2C_CHANNEL_1))
	smbus1_MoudleClock_EN;
	sysctl_iomux_i2c1();
	i2c1_pull_up();
	I2c_Channel_Init(I2C_CHANNEL_1, I2C1_SPEED, I2C1_DEFAULT_ROLE, I2C1_DEFAULT_ADDR, 1);
#endif
#if I2C2_EN_Init||((DEBUGGER_OUTPUT_SWITCH == 1)&&(DEBUGGER_I2C_CHANNEL==I2C_CHANNEL_2))
	smbus2_MoudleClock_EN;
	sysctl_iomux_i2c2(I2C2_CLK_SEL);
	i2c2_pull_up(I2C2_CLK_SEL);
	I2c_Channel_Init(I2C_CHANNEL_2, I2C2_SPEED, I2C2_DEFAULT_ROLE, I2C2_DEFAULT_ADDR, 1);
#endif
#if I2C3_EN_Init||((DEBUGGER_OUTPUT_SWITCH == 1)&&(DEBUGGER_I2C_CHANNEL==I2C_CHANNEL_3))
	smbus3_MoudleClock_EN;
	sysctl_iomux_i2c3();
	i2c3_pull_up();
	I2c_Channel_Init(I2C_CHANNEL_3, I2C3_SPEED, I2C3_DEFAULT_ROLE, I2C3_DEFAULT_ADDR, 1);
#endif
#if I2C4_EN_Init||((DEBUGGER_OUTPUT_SWITCH == 1)&&(DEBUGGER_I2C_CHANNEL==I2C_CHANNEL_4))
	smbus4_MoudleClock_EN;
	sysctl_iomux_i2c4();
	i2c4_pull_up();
	I2c_Channel_Init(I2C_CHANNEL_4, I2C4_SPEED, I2C4_DEFAULT_ROLE, I2C4_DEFAULT_ADDR, 1);
#endif
#if I2C5_EN_Init||((DEBUGGER_OUTPUT_SWITCH == 1)&&(DEBUGGER_I2C_CHANNEL==I2C_CHANNEL_5))
	smbus5_MoudleClock_EN;
	sysctl_iomux_i2c5();
	i2c5_pull_up();
	I2c_Channel_Init(I2C_CHANNEL_5, I2C5_SPEED, I2C5_DEFAULT_ROLE, I2C5_DEFAULT_ADDR, 1);
#endif
#if I2C6_EN_Init||((DEBUGGER_OUTPUT_SWITCH == 1)&&(DEBUGGER_I2C_CHANNEL==I2C_CHANNEL_6))
	smbus6_MoudleClock_EN;
	sysctl_iomux_i2c6();
	i2c6_pull_up();
	I2c_Channel_Init(I2C_CHANNEL_6, I2C6_SPEED, I2C6_DEFAULT_ROLE, I2C6_DEFAULT_ADDR, 1);
#endif
#if I2C7_EN_Init||((DEBUGGER_OUTPUT_SWITCH == 1)&&(DEBUGGER_I2C_CHANNEL==I2C_CHANNEL_7))
	smbus7_MoudleClock_EN;
	sysctl_iomux_i2c7();
	i2c7_pull_up();
	I2c_Channel_Init(I2C_CHANNEL_7, I2C7_SPEED, I2C7_DEFAULT_ROLE, I2C7_DEFAULT_ADDR, 1);
#endif
#if I2C8_EN_Init||((DEBUGGER_OUTPUT_SWITCH == 1)&&(DEBUGGER_I2C_CHANNEL==I2C_CHANNEL_8))
	smbus8_MoudleClock_EN;
	sysctl_iomux_i2c8();
	i2c8_pull_up();
	I2c_Channel_Init(I2C_CHANNEL_8, I2C8_SPEED, I2C8_DEFAULT_ROLE, I2C8_DEFAULT_ADDR, 1);
#endif
#if (DEBUGGER_OUTPUT_SWITCH == 1)//初始化调试器//因为是调试手段,所以尽早初始化(UART因为不需要考虑地址,只需要波特率因此相对简单)
	I2c_Channel_Init(DEBUGGER_I2C_CHANNEL, DEBUGGER_I2C_SPEED, I2C_SLAVE_ROLE, DEBUGGER_I2C_ADDRESS, 1);
	SMBUSn_INTR_MASK0(DEBUGGER_I2C_CHANNEL) |= I2C_INTR_RX_FULL;
#endif
	dprint("I2c_channel_init done.\n");
#endif
}
void i3c_init(void)
{
#if I3C_MODULE_EN
	/****************** master init ******************/
#if I3C0_EN_Init
	i3c0_MoudleClock_EN;
	sysctl_iomux_master0();
	i3c0_pull_up();
	I3C_Master_Init(I3C_MASTER0_DEFAULT_ROLE, I3C_MASTER0_SPEED, I3C_MASTER0_DEFAULT_ADDR, I3C_MASTER0_DEFAULT_DCT, I3C_MASTER0_DEFAULT_DYNAMICADDR, I3C_MASTER0);
#endif

#if I3C1_EN_Init
	i3c1_MoudleClock_EN;
	sysctl_iomux_master1();
	i3c1_pull_up();
	I3C_Master_Init(I3C_MASTER1_DEFAULT_ROLE, I3C_MASTER1_SPEED, I3C_MASTER1_DEFAULT_ADDR, I3C_MASTER1_DEFAULT_DCT, I3C_MASTER1_DEFAULT_DYNAMICADDR, I3C_MASTER1);
#endif
	dprint("i3c master init done.\n");

	/****************** slave init ******************/
#if I3C2_EN_Init
	i3c2_MoudleClock_EN;
	sysctl_iomux_slave0();
	i3c2_pull_up();
	I3C_Slave_Init(I3C_SLAVE0_DEFAULT_ADDR, I3C_SLAVE0_DEFAULT_IDPARTNO, I3C_SLAVE0_DEFAULT_DCR, I3C_SLAVE0_DEFAULT_BCR, I3C_SLAVE0);
#endif
#if I2C3_EN_Init
	i3c3_MoudleClock_EN;
	sysctl_iomux_slave1();
	i3c3_pull_up();
	I3C_Slave_Init(I3C_SLAVE1_DEFAULT_ADDR, I3C_SLAVE1_DEFAULT_IDPARTNO, I3C_SLAVE1_DEFAULT_DCR, I3C_SLAVE1_DEFAULT_BCR, I3C_SLAVE1);
#endif
	dprint("i3c slave init done.\n");


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
	SPI_Init(0, SPIM_CPOL_LOW, SPIM_CPHA_FE, SPIM_MSB, 0x7, 1);
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
	// PWM_CTR0_1 = 0x104;//0x163;
	// PWM_CTR2_3 = 0x101;
	// PWM_DCR0_1 = 0x101;
	// PWM_DCR2_3 = 0x101;
	// PWM_DCR4_5 = 0x101;
	// PWM_DCR6_7 = 0x101;
	// PWM_CTRL = 0xff01;
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
void ps2_init(void)
{
#if (PS2_MODULE_EN)
#if PS2_0_EN_Init
	ps2_0_MoudleClock_EN;
	ps2_0_frequency();
	sysctl_iomux_ps2_0(PS2_0_CLK_SEL, PS2_0_DAT_SEL);
	ps2_0_pull_up(PS2_0_CLK_SEL, PS2_0_DAT_SEL);
#endif
#if PS2_1_EN_Init
	ps2_1_MoudleClock_EN;
	ps2_1_frequency();
	sysctl_iomux_ps2_1(PS2_1_CLK_SEL, PS2_1_DAT_SEL);
	ps2_1_pull_up(PS2_1_CLK_SEL, PS2_1_DAT_SEL);
#endif
	dprint("Ps2 init done.\n");
#endif
}

void cec_init(void)
{
#if (CEC_MODULE_EN)
	cec_MoudleClock_EN;

#if CEC0_EN_Init
	sysctl_iomux_cec0(0);
	cec0_pull_up(0);
#if CEC0_mode_select
	CEC_initiator_init(0);
#else
	CEC_follower_init(0);
#endif
#endif

#if CEC1_EN_Init
	sysctl_iomux_cec1();
	cec1_pull_up();
#if CEC1_mode_select
	CEC_initiator_init(1);
#else
	CEC_follower_init(1);
#endif
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
	ADC_SW_Sample_Init_Single(ADC_CHANNEL0, Databuffer_channel0, SingleEnded);
#endif
#if (ADC1_EN_Init)
	sysctl_iomux_adc1();
	ADC_SW_Sample_Init_Single(ADC_CHANNEL1, Databuffer_channel1, SingleEnded);
#endif
#if (ADC2_EN_Init)
	sysctl_iomux_adc2();
	ADC_SW_Sample_Init_Single(ADC_CHANNEL2, Databuffer_channel2, SingleEnded);
#endif
#if (ADC3_EN_Init)
	sysctl_iomux_adc3();
	ADC_SW_Sample_Init_Single(ADC_CHANNEL3, Databuffer_channel3, SingleEnded);
#endif
#if (ADC4_EN_Init)
	sysctl_iomux_adc4();
	ADC_SW_Sample_Init_Single(ADC_CHANNEL4, Databuffer_channel4, SingleEnded);
#endif
#if (ADC5_EN_Init)
	sysctl_iomux_adc5();
	ADC_SW_Sample_Init_Single(ADC_CHANNEL5, Databuffer_channel5, SingleEnded);
#endif
#if (ADC6_EN_Init)
	sysctl_iomux_adc6();
	ADC_SW_Sample_Init_Single(ADC_CHANNEL6, Databuffer_channel6, SingleEnded);
#endif
#if (ADC7_EN_Init)
	sysctl_iomux_adc7();
	ADC_SW_Sample_Init_Single(ADC_CHANNEL7, Databuffer_channel7, SingleEnded);
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

	while((TIMER_TRIS & 0xf) != 0xf)
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
	// 1.Initialize The GPIO
	gpio_init();
	// 2.Switch Default SMBUS3 or UARTB 
	DEFAULT_SMBUS3_UARTB_SEL;
	// 3.Initialize The Serial Port
	uart_init();
	// 4.Initialize The SMBUS
	smbus_init();
	// 6.Initialize The I3C
	i3c_init();
	// 7.Initialize The SPI
	spim_init();
	// 8.Initialize The PWM and The TACH
	pwm_tach_init();
	// 9.Initialize The KBC and The PMC
	kbc_pmc_init();
	// 10.Initialize  The KBS
	kbs_init();
	ps2_init();
#if ( 0)
	// 11.Initialize The CEC
	cec_init();
	// 12.Initialize The OWI
	owi_init();
	// 13.Initialize The OWI
	rtc_init();
	// 14.Initialize The PECI
	peci_init();
	// 15.Initialize The eSPI
	espi_init();
#endif
	// 16.Initialize The ADC
	adc_init();
	// 17. Initialize The timer and The watch dog
	time_init();
	// 17.Initialize The Mailbox
	mailbox_init();
	// 18.Initialize The SPIF
	SPIF_Init();
	// 19.Initialize The LPC
	sysctl_iomux_lpc();

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
	while(1)
		;
}
#endif