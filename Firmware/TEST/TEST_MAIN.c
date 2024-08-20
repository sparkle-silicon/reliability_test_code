/*
 * @Author: Maple
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-02-07 12:41:14
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
int test_loop(void)
{
	dprint("Enter test_service \n");
	test_service();
}
int main()
{
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
