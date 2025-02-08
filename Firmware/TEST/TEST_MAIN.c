/*
 * @Author: Maple
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-02-08 16:09:35
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
#include "AE_INCLUDE.H"
#include "TEST_MAIN.H"
#include "TEST_IRQ.H"
#include "TEST_INTC.H"
#include "TEST_AUTOMATIC.H"
#include "KERNEL_INCLUDE.H"
#if TEST

// void BBLED_TEST(void)//在32k的情况下运行的数据
// {
// 	uint8_t pwm_led_mode = 0;
// 	{//进入低功耗模式(具体可配置未知，目的只是为了输出pwm led)
// 		SYSCTL_SWITCH_PLL = 0;//0x304D4=sleep mode,32K clock,32.768K clock
// 		SYSCTL_PMUCSR |= BIT(20);//0x30498 = enable WFI
// 	}
// //配置TIMER2定时器中断2sec唤醒一次
// 	SYSCTL_CLKDIV_TMR2 = TIMER2_Division;//配置TMR2分频=0
// 	timer2_MoudleClock_EN;//enable TMR2 mode
// 	TIMER_Init(TIMER2, (LOW_CHIP_CLOCK / (2 << TIMER2_Division)/*clock=*/ * 2/*sec*/), 0x1/*loop*/, 0x0/**/);
// 	//配置PWM波输出
// 	pwm_MoudleClock_EN;
// 	PWM_CTRL |= PRESCALE_4;//4分频32000/4=8000
// 		/* pwm_scale */
// 	PWM_SCALER0 = 0;//8000/1=8000
// 	PWM_SCALER1 = 0;//8000/1=8000
// 	PWM_SCALER2 = 0;//8000/1=8000
// 	PWM_SCALER3 = 0;//8000/1=8000
// 	PWM_CTR0_1 = 0x3f3f;//CTR=64-1
// 	PWM_CTR2_3 = 0x3f3f;//CTR=64-1
// 	sysctl_iomux_pwm0();

// 	// printf("wfi\n");//uart 输出，可屏蔽
// 	PWM_Init_channel(PWM_CHANNEL0, PWM_LOW, PWM_CLK0, PWM_CTR0, pwm_led_mode, 1);//一次步进1，步进64次
// 	while(1)//被TIMER2中断唤醒后继续跑，重新进入低功耗
// 	{
// 		// printf("pwm_led_mode = %#x\n", pwm_led_mode);//uart 输出，可屏蔽
// 		// while(!(PRINTF_LSR & UART_LSR_TEMP));//uart 输出，可屏蔽
// 		PWM_ReInit_channel(PWM_CHANNEL0, pwm_led_mode, 1);
// 		asm volatile("wfi");//进入低功耗模式
// 		pwm_led_mode = (~pwm_led_mode) & 0x3f;//0/0x3f
// 	}
// 	// printf("exit wfi\n");//uart 输出，可屏蔽
// //退出低功耗模式(恢复现场)
// 	// PWM_CLOCK_Init();
// 	// sysctl_iomux_pwm0();
// 	// PWM_Init_channel(PWM_CHANNEL0, PWM_LOW, PWM_CLK0, PWM_CTR0, 98, 0);

// 	// TIMER_Init(TIMER2, TIMER2_1ms, 0x1, 0x0); // 1ms service计时函数
// 	pwm_led_mode = 0;
// 	return;
// }

extern lpc_mon lpc_stat;
extern BYTE Write_buff[256];
extern BYTE Read_buff[256];
//----------------------------------------------------------------------------
// FUNCTION: Service_PCI
// KBC PORT service function
//----------------------------------------------------------------------------
void Service_PCI(void)
{
#if (Service_PCI_START == 1)
#if LPC_WAY_OPTION_SWITCH
	if(Is_FLAG_CLEAR(KBC_STA, KBC_STA_IBF))
		return;
#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
	AUTO_TEST_PORT(0);
#endif
#else
	if(F_Service_PCI == 1)
	{
		F_Service_PCI = 0;
		if(Is_FLAG_CLEAR(KBC_STA, KBC_STA_IBF))
			return;
	#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
		AUTO_TEST_PORT(0);
	#endif
	}
#endif
#endif
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Service_PCI2
 * 62/66 Host Command/Data service
 * ------------------------------------------------------------------------- */
void Service_PCI2(void)
{
#if (Service_PCI2_START == 1)
#if LPC_WAY_OPTION_SWITCH
	if(Is_FLAG_CLEAR(PMC1_STR, IBF1))
		return;
#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
	AUTO_TEST_PORT(1);
#endif
#else
	if(F_Service_PCI2 == 1)
	{
		F_Service_PCI2 = 0;
		if(Is_FLAG_CLEAR(PMC1_STR, IBF1))
			return;
	#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
		AUTO_TEST_PORT(1);
	#endif
	}
#endif
#endif
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Service_PCI3
 * PMC2 PORT cmd/data process
 * ------------------------------------------------------------------------- */
void Service_PCI3(void)
{
#if (Service_PCI3_START == 1)
#if LPC_WAY_OPTION_SWITCH
	if(Is_FLAG_CLEAR(PMC2_STR, IBF2))
		return;
#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
	AUTO_TEST_PORT(2);
#endif
#else
	if(F_Service_PCI3 == 1)
	{
		F_Service_PCI3 = 0;
		if(Is_FLAG_CLEAR(PMC2_STR, IBF2))
			return;
	#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
		AUTO_TEST_PORT(2);
	#endif
	}
#endif
#endif
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Service_PCI4
 * PMC3 PORT cmd/data process
 * ------------------------------------------------------------------------- */
void Service_PCI4(void)
{
#if (Service_PCI4_START == 1)
#if LPC_WAY_OPTION_SWITCH
	if(Is_FLAG_CLEAR(PMC3_STR, IBF3))
		return;
#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
	AUTO_TEST_PORT(3);
#endif
#else
	if(F_Service_PCI4 == 1)
	{
		F_Service_PCI4 = 0;
		if(Is_FLAG_CLEAR(PMC3_STR, IBF3))
			return;
	#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
		AUTO_TEST_PORT(3);
	#endif
	}
#endif
#endif
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Service_PCI5
 * PMC4 PORT cmd/data process
 * ------------------------------------------------------------------------- */
void Service_PCI5(void)
{
#if (Service_PCI5_START == 1)
#if LPC_WAY_OPTION_SWITCH
	if(Is_FLAG_CLEAR(PMC4_STR, IBF4))
		return;
#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
	AUTO_TEST_PORT(4);
#endif
#else
	if(F_Service_PCI5 == 1)
	{
		F_Service_PCI5 = 0;
		if(Is_FLAG_CLEAR(PMC4_STR, IBF4))
			return;
	#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
		AUTO_TEST_PORT(4);
	#endif
	}
#endif
#endif
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Service_PCI6
 * PMC5 PORT cmd/data process
 * ------------------------------------------------------------------------- */
void Service_PCI6(void)
{
#if (Service_PCI6_START == 1)
#if LPC_WAY_OPTION_SWITCH
	if(Is_FLAG_CLEAR(PMC5_STR, IBF5))
		return;
#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
	AUTO_TEST_PORT(5);
#endif
#else
	if(F_Service_PCI6 == 1)
	{
		F_Service_PCI6 = 0;
		if(Is_FLAG_CLEAR(PMC5_STR, IBF5))
			return;
	#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
		AUTO_TEST_PORT(5);
	#endif
	}
#endif
#endif
}
//----------------------------------------------------------------------------
// FUNCTION: test_service
// polling service table
//----------------------------------------------------------------------------
extern short array_count;
void test_service(void)
{
	while(1)
	{
		if(_R1 >= array_count)
			_R1 = 0;
		(service_table[_R1])();
		_R1++;
	#if LPC_MODULE_EN
		if(LPC_EOF == 1)
		{
			dprint("Cycle Type:%s\n", lpc_stat.cyctpe_dir);
			dprint("Addr:%#x\n", lpc_stat.addr);
			dprint("Data:%#x\n", lpc_stat.data);
			LPC_EOF = 0;
		}
	#endif
	}
}
//----------------------------------------------------------------------------
// FUNCTION: main loop
// enter main loop
//----------------------------------------------------------------------------
extern BYTE RPMC_OOB_TempArr[80];
// 将一个字符转换为对应的十六进制值
unsigned char hexCharToValue(char c)
{
	if(c >= '0' && c <= '9')
	{
		return c - '0';
	}
	else if(c >= 'a' && c <= 'f')
	{
		return c - 'a' + 10;
	}
	else if(c >= 'A' && c <= 'F')
	{
		return c - 'A' + 10;
	}
	else
	{
		return 0; // 如果不是合法的十六进制字符，返回0
	}
}

// 将十六进制字符串转换为字节数组
void hexStringToByteArray(const char *hexString, unsigned char *byteArray, int byteArraySize)
{
	for(int i = 0; i < byteArraySize; i++)
	{
// 每两个字符组成一个字节
		byteArray[i] = (hexCharToValue(hexString[i * 2]) << 4) | hexCharToValue(hexString[i * 2 + 1]);
	}
}
int test_loop(void)
{
	dprint("Enter test_service \n");
	test_service();
}
int main()
{
	// RPMC_OOB_TempArr[13]=0x9B;
	// RPMC_OOB_TempArr[14]=0x00;
	// RPMC_OOB_TempArr[15]=0x00;
	// RPMC_OOB_TempArr[16]=0x00;
	// const char hexString0[] = "1f1e1d1c1b1a191817161514131211100f0e0d0c0b0a09080706050403020100";
	// const char hexString1[] = "23d673afd5435e132ec2cb402cf422fe7d37bc4abdcfd3b71142";
	// const char hexString2[] = "67b3";
	// const char HMAC[] = "5ecff72deae107a1c89fd55a5f97efb92b2bc18bd3a18c7806902b38346080fa";
	// const char KData[] = "04030201";
	// const char ISign[] = "254232171ed05a5ecfce9df415c7e9b26fa365a076d9efe20a7d1d33fc3c5d21";
	// const char CData[] = "00003ffe";
	// const char RSign[] = "62beacc46a26dcfad2f162cfa08182d7124df94b80ffda9cd08fee3f7c581d0d";
	// const char Tag[] = "010000000000000000000000";
// 循环遍历十六进制字符串，并将每两位转换为数字
	// hexStringToByteArray(hexString0, &RPMC_OOB_TempArr[17], 32);
	// hexStringToByteArray(hexString1, &RPMC_OOB_TempArr[49], 26);
	// memcpy(&eRPMC_WriteRootKey_m1, RPMC_OOB_TempArr, sizeof(eRPMC_WriteRootKey_m1));
	// hexStringToByteArray(hexString2, &RPMC_OOB_TempArr[12], 2);
	// memcpy(&eRPMC_WriteRootKey_m2, RPMC_OOB_TempArr, sizeof(eRPMC_WriteRootKey_m2));
	// /*mailbox WriteRootKey trigger*/
	// task_head=Add_Task((TaskFunction)Mailbox_WriteRootKey_Trigger,Params,&task_head);

	// RPMC_OOB_TempArr[13]=0x9B;
	// RPMC_OOB_TempArr[14]=0x01;
	// hexStringToByteArray(HMAC, &RPMC_OOB_TempArr[21], 32);
	// hexStringToByteArray(KData, &RPMC_OOB_TempArr[17], 4);
	// memcpy(&eRPMC_UpdateHMACKey, RPMC_OOB_TempArr, sizeof(eRPMC_UpdateHMACKey));
	// /*mailbox UpdateHMACKey trigger*/
	// task_head=Add_Task((TaskFunction)Mailbox_UpdateHMACKey_Trigger,Params,&task_head);

	// RPMC_OOB_TempArr[13]=0x9B;
	// RPMC_OOB_TempArr[14]=0x03;
	// hexStringToByteArray(RSign, &RPMC_OOB_TempArr[29], 32);
	// hexStringToByteArray(Tag, &RPMC_OOB_TempArr[17], 12);
	// memcpy(&eRPMC_RequestCounter, RPMC_OOB_TempArr, sizeof(eRPMC_RequestCounter));
	// task_head=Add_Task((TaskFunction)Mailbox_RequestCounter_Trigger,Params,&task_head);

	// RPMC_OOB_TempArr[13]=0x9B;
	// RPMC_OOB_TempArr[14]=0x02;
	// hexStringToByteArray(ISign, &RPMC_OOB_TempArr[21], 32);
	// hexStringToByteArray(CData, &RPMC_OOB_TempArr[17], 4);
	// memcpy(&eRPMC_IncrementCounter, RPMC_OOB_TempArr, sizeof(eRPMC_IncrementCounter));
	// task_head=Add_Task((TaskFunction)Mailbox_IncrementCounter_Trigger,Params,&task_head);

	unsigned long tmp;
	int ret = 0;
	// dprint ("%s",printf_instructions_msg);
	// Init_GPIO();
	// OEM_INIT_ACPI();
	dprint("this is test_%s\n", __FUNCTION__);
#if SWUC_TEST
	SWUC_Test();
#endif
#if SPI_DEBUG
	// SPI_Init();
	sysctl_iomux_config(GPIOA, 6, 2);  // spim_sck
	sysctl_iomux_config(GPIOA, 19, 2); // spim_mosi
	sysctl_iomux_config(GPIOA, 21, 2); // spim_miso
	sysctl_iomux_config(GPIOB, 16, 2); // csn1
	sysctl_iomux_config(GPIOB, 18, 2); // csn0
	SPI_Read_ID0();
	SPI_Flash_Test();
#endif
#if SCI_Test
	GPIOA_REG(GPIO_INPUT_VAL) |= ((0x0) << 0);
	GPIOA_REG(GPIO_INPUT_EN) |= ((0x1) << 0);
	GPIOA_REG(GPIO_LOW_IE) &= (~(0x1) << 0);
	SCI_test();
#endif
#if SMI_Test
	GPIOA_REG(GPIO_INPUT_VAL) |= ((0x0) << 0);
	GPIOA_REG(GPIO_INPUT_EN) |= ((0x1) << 0);
	GPIOA_REG(GPIO_LOW_IE) &= (~(0x1) << 0);
	SMI_test();
#endif
#ifdef AE103
#if SMx_TEST
	SM_Test_main();
#endif
#endif
#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
	KBC_CTL = 0x8;
	PMC1_CTL = 0x1;
	PMC2_CTL = 0x1;
	uart0_MoudleClock_EN;
	sysctl_iomux_uart0();
	serial_init(0, 115200);
	uart1_MoudleClock_EN;
	sysctl_iomux_uart1();
	serial_init(1, 115200);
	uart2_MoudleClock_EN;
	sysctl_iomux_uart2();
	serial_init(2, 115200);
	uart3_MoudleClock_EN;
	sysctl_iomux_uart3();
	serial_init(3, 115200);
#endif
	dprint("This is external flash\n");
	Specific_Mem_init(); // memory init
	test_loop();
}
#endif
