/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-07-22 10:42:22
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

void gpio_init(void)
{
#if GPIO_MODULE_EN
	gpio_MoudleClock_EN;
	gpiodb_MoudleClock_EN;
	// 1.default config pin iomux and pull up
	// 2.default config gpio  Voltage Signal and IO Direction
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
	for(cnt = 0; cnt < 6; cnt++)
	{
		if(flag & BIT(cnt))
		{
			printf("Actual baud rate of the serial port %X == %d\n", ((cnt < 4) ? cnt : ((cnt == 5) ? 0xB : 0xA)), baud[cnt]);
		}
	}
	printf("Uart init done.\n");
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
	printf("I2c_channel_init done.\n");
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
	printf("SPI init done.\n");
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
	printf("PWM init done\n");
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
	printf("TACH init done\n");
#endif
}
void kbc_pmc_init(void)
{
	pmckbc_MoudleClock_EN;
#if (PMC_MODULE_EN)
	pmc1_init();
	pmc2_init();
	printf("PMC init done.\n");
#endif
}
void pport_init(void)
{
#if (PPORT_MODULE_EN)
	pport_MoudleClock_EN;
	sysctl_iomux_pport();
	// pport reg init function
	printf("Pport init done.\n");
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
	printf("CAN init done.\n");
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
	printf("cec init done.\n");
#endif
}
void owi_init(void)
{
#if (OWI_MODULE_EN)
	SYSCTL_CLKDIV_OWI = 0x0; // 时钟1分频,分频数为0base
	owi_MoudleClock_EN;
	sysctl_iomux_owi();
	OWI_Init(128, 30, 20, 10);
	printf("owi init done.\n");
#endif
}
void rtc_init(void)
{
#if (RTC_MODULE_EN)
	rtc_MoudleClock_EN;
	RTC_Init(0, 1, LOW_CHIP_CLOCK);
	printf("rtc init done.\n");
#endif
}
void peci_init(void)
{
#if (PECI_MODULE_EN)
	SYSCTL_CLKDIV_PECI = 1;
	peci_MoudleClock_EN;
	sysctl_iomux_peci();
	Init_PECI();
	printf("peci init done.\n");
#endif
}
void espi_init(void)
{
#if (ESPI_MODULE_EN)
	espi_MoudleClock_EN;
	sysctl_iomux_espi();
	ESPI_Init();
	printf("espi init done.\n");
#endif
}
void adc_init(void)
{
#if ADC_MODULE_EN
	adc_MoudleClock_EN;
#if (ADC0_EN_Init)
	sysctl_iomux_adc0();
	ADC_Init(ADC_CHANNEL0, 1, 0, 0);
#endif
#if (ADC1_EN_Init)
	sysctl_iomux_adc1();
	ADC_Init(ADC_CHANNEL1, 1, 0, 0);
#endif
#if (ADC2_EN_Init)
	sysctl_iomux_adc2();
	ADC_Init(ADC_CHANNEL2, 1, 0, 0);
#endif
#if (ADC3_EN_Init)
	sysctl_iomux_adc3();
	ADC_Init(ADC_CHANNEL3, 1, 0, 0);
#endif
#if (ADC4_EN_Init)
	sysctl_iomux_adc4();
	ADC_Init(ADC_CHANNEL4, 1, 0, 0);
#endif
#if (ADC5_EN_Init)
	sysctl_iomux_adc5();
	ADC_Init(ADC_CHANNEL5, 1, 0, 0);
#endif
#if (ADC6_EN_Init)
	sysctl_iomux_adc6();
	ADC_Init(ADC_CHANNEL6, 1, 0, 0);
#endif
#if (ADC7_EN_Init)
	sysctl_iomux_adc7();
	ADC_Init(ADC_CHANNEL7, 1, 0, 0);
#endif
	printf("ADC init done \n");
#endif
}

// void Module_init(void)
// {

// 	// 2. Initialize APB CLOCK BUS ENABLE
// 	apb_MoudleClock_EN;

// 	// 4.Initialize The Serial Port
// 	uart_init();

// 	// 5.Initialize The SMBUS
// 	smbus_init();

// 	printf("End init \n");
// 	return;
// }
