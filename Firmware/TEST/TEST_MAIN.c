/*
 * @Author: Maple
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-18 18:26:40
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
 // 		SYSCTL_SLEEPCFG = 0;//0x304D4=sleep mode,32K clock,32.768K clock
 // 		SYSCTL_PMUCSR |= BIT(20);//0x30498 = enable WFI
 // 	}
 // //配置TIMER2定时器中断2sec唤醒一次
 // 	SYSCTL_CLKDIV_TMR2 = TIMER2_CLOCK_DIVISION;//配置TMR2分频=0
 // 	timer2_ModuleClock_EN;//enable TMR2 mode
 // 	TIMER_Init(TIMER2, (LOW_CHIP_CLOCK / (2 << TIMER2_CLOCK_DIVISION)/*clock=*/ * 2/*sec*/), 0x1/*loop*/, 0x0/**/);
 // 	//配置PWM波输出
 // 	pwm_ModuleClock_EN;
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
	if (Is_FLAG_CLEAR(KBC_STA, KBC_STA_IBF))
		return;
#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
	AUTO_TEST_PORT(0);
#endif
#else
	if (F_Service_PCI == 1)
	{
		F_Service_PCI = 0;
		if (Is_FLAG_CLEAR(KBC_STA, KBC_STA_IBF))
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
	if (Is_FLAG_CLEAR(PMC1_STR, IBF1))
		return;
#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
	AUTO_TEST_PORT(1);
#endif
#else
	if (F_Service_PCI2 == 1)
	{
		F_Service_PCI2 = 0;
		if (Is_FLAG_CLEAR(PMC1_STR, IBF1))
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
	if (Is_FLAG_CLEAR(PMC2_STR, IBF2))
		return;
#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
	AUTO_TEST_PORT(2);
#endif
#else
	if (F_Service_PCI3 == 1)
	{
		F_Service_PCI3 = 0;
		if (Is_FLAG_CLEAR(PMC2_STR, IBF2))
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
	if (Is_FLAG_CLEAR(PMC3_STR, IBF3))
		return;
#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
	AUTO_TEST_PORT(3);
#endif
#else
	if (F_Service_PCI4 == 1)
	{
		F_Service_PCI4 = 0;
		if (Is_FLAG_CLEAR(PMC3_STR, IBF3))
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
	if (Is_FLAG_CLEAR(PMC4_STR, IBF4))
		return;
#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
	AUTO_TEST_PORT(4);
#endif
#else
	if (F_Service_PCI5 == 1)
	{
		F_Service_PCI5 = 0;
		if (Is_FLAG_CLEAR(PMC4_STR, IBF4))
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
	if (Is_FLAG_CLEAR(PMC5_STR, IBF5))
		return;
#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
	AUTO_TEST_PORT(5);
#endif
#else
	if (F_Service_PCI6 == 1)
	{
		F_Service_PCI6 = 0;
		if (Is_FLAG_CLEAR(PMC5_STR, IBF5))
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
	while (1)
	{
		if (_R1 >= array_count)
			_R1 = 0;
		(service_table[_R1])();
		_R1++;
#if LPC_MODULE_EN
		if (LPC_EOF == 1)
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
	if (c >= '0' && c <= '9')
	{
		return c - '0';
	}
	else if (c >= 'a' && c <= 'f')
	{
		return c - 'a' + 10;
	}
	else if (c >= 'A' && c <= 'F')
	{
		return c - 'A' + 10;
	}
	else
	{
		return 0; // 如果不是合法的十六进制字符，返回0
	}
}

// 将十六进制字符串转换为字节数组
void hexStringToByteArray(const char* hexString, unsigned char* byteArray, int byteArraySize)
{
	for (int i = 0; i < byteArraySize; i++)
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
	// SPIM_Init();
	sysctl_iomux_config(GPIOA, 6, 2);  // spim_sck
	sysctl_iomux_config(GPIOA, 19, 2); // spim_mosi
	sysctl_iomux_config(GPIOA, 21, 2); // spim_miso
	sysctl_iomux_config(GPIOB, 16, 2); // csn1
	sysctl_iomux_config(GPIOB, 18, 2); // csn0
	SPI_Read_ID0();
	SPI_Flash_Test();
#endif
#if SCI_Test
	GPIO0_REG(GPIO_INPUT_VAL) |= ((0x0) << 0);
	GPIO0_REG(GPIO_INPUT_EN) |= ((0x1) << 0);
	GPIO0_REG(GPIO_LOW_IE) &= (~(0x1) << 0);
	SCI_test();
#endif
#if SMI_Test
	GPIO0_REG(GPIO_INPUT_VAL) |= ((0x0) << 0);
	GPIO0_REG(GPIO_INPUT_EN) |= ((0x1) << 0);
	GPIO0_REG(GPIO_LOW_IE) &= (~(0x1) << 0);
	SMI_test();
#endif

#if LPC_PRESSURE_TEST && (defined(AE102) || defined(AE103))
	KBC_CTL = 0x8;
	PMC1_CTL = 0x1;
	PMC2_CTL = 0x1;
	uart0_ModuleClock_EN;
	sysctl_iomux_uart0();
	serial_init(0, 115200);
	uart1_ModuleClock_EN;
	sysctl_iomux_uart1();
	serial_init(1, 115200);
	uart2_ModuleClock_EN;
	sysctl_iomux_uart2();
	serial_init(2, 115200);
	uart3_ModuleClock_EN;
	sysctl_iomux_uart3();
	serial_init(3, 115200);
#endif

	// 定义指向起始地址的volatile指针（防止编译器优化写操作）
	volatile unsigned char* base_addr = (volatile unsigned char*)0x29000;
	// 定义起始地址的volatile指针（防止编译器优化，确保实际读取内存）
	volatile  unsigned char* base_addr0 = (volatile  unsigned char*)0x2a000;
	// 计算地址范围大小：0x2a000 - 0x29000 = 0x1000（共4096个字节）
	const uint32_t range_size = 0x2b000 - 0x2a000;

	// 循环写入0到0xff（共256个值，对应0x29000到0x290ff）
	for (uint16_t i = 0; i < 256; i++) {
		// base_addr[i] 等价于访问地址 0x29000 + i
		base_addr[i] = i;
		printf("0x%x : %x\n", 0x29000 + i, base_addr[i]);
	}

	for (uint32_t i = 0; i < range_size; i++) {
		//清零
		*(base_addr0 + i) = 0;
	}
	printf("dma test start\n");
	DMA_InitStruct.DMA_Dest_Width = DMA_DST_TR_WIDTH_8bits;
	DMA_InitStruct.DMA_Src_Width = DMA_SRC_TR_WIDTH_8bits;
	DMA_InitStruct.DMA_Dest_Inc = DMA_DINC_INC;
	DMA_InitStruct.DMA_Src_Inc = DMA_SINC_INC;
	DMA_InitStruct.DMA_Dest_Msize = DMA_DEST_MSIZE_4;
	DMA_InitStruct.DMA_Src_Msize = DMA_SRC_MSIZE_4;
	DMA_InitStruct.DMA_Trans_Type = DMA_TT_FC_MtoM_DMA;
	DMA_InitStruct.DMA_Block_Ts = 0x2;
	DMA_InitStruct.DMA_Priority = 0;
	DMA_InitStruct.DMA_Dest_Addr = 0x2a000;
	DMA_InitStruct.DMA_Src_Addr = 0x29000;
	DMA_Init(&DMA_InitStruct);
	printf("dma init ok\n");
	vDelayXms(10);

	// 遍历每个地址并打印
	for (uint32_t i = 0; i < range_size; i++) {
		// 每16个字节换行，并在行首打印当前起始地址
		if (i % 16 == 0) {
			// 地址格式：0x29000 这样的8位十六进制（前导0补齐）
			printf("\n0x%08x:  ", (uint32_t)(base_addr0 + i));
		}
		// 打印当前字节值（两位十六进制，前导0补齐）
		printf("%02x ", base_addr0[i]);
	}
	printf("\n");  // 最后补一个换行

	//iram -uartb tx
	// sysctl_iomux_uartb();
	// serial_init(UARTB_CHANNEL, 115200);
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_8bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_No_chagne;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_INC;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_MtoP_DMA;
	// DMA_InitStruct.DMA_Periph_Type=UARTB_DMA_TX;
	// DMA_InitStruct.DMA_Block_Ts=0x2;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=&UARTB_TX;
	// DMA_InitStruct.DMA_Src_Addr=0x29000;
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(10);

	//iram -uart0 tx uart0 rx iram
	// REG32(0x29000)=0xffffff41;//A
	// REG32(0x29004)=0xffffff42;//B
	// REG32(0x29008)=0xffffff43;//C
	// sysctl_iomux_uart0();
	// serial_init(UART0_CHANNEL, 115200);
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_No_chagne;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_INC;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_MtoP_DMA;
	// DMA_InitStruct.DMA_Periph_Type=UART0_DMA_TX;
	// DMA_InitStruct.DMA_Block_Ts=0x3;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=&UART0_TX;
	// DMA_InitStruct.DMA_Src_Addr=0x29000;
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(10);

	// UART0_IER=0;
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_INC;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_No_chagne;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_PtoM_DMA;
	// DMA_InitStruct.DMA_Periph_Type=UART0_DMA_RX;
	// DMA_InitStruct.DMA_Block_Ts=0x1f;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=0x2a000;
	// DMA_InitStruct.DMA_Src_Addr=&UART0_TX;
	// DMA_Init(&DMA_InitStruct);

	//iram -uart1 tx uart1 rx -> iram
	REG32(0x29000) = 0xffffff41;//A
	REG32(0x29004) = 0xffffff42;//B
	REG32(0x29008) = 0xffffff43;//C
	sysctl_iomux_uart1(0, 0);
	serial_init(UART1_CHANNEL, 115200);
	DMA_InitStruct.DMA_Dest_Width = DMA_DST_TR_WIDTH_32bits;
	DMA_InitStruct.DMA_Src_Width = DMA_SRC_TR_WIDTH_32bits;
	DMA_InitStruct.DMA_Dest_Inc = DMA_DINC_No_chagne;
	DMA_InitStruct.DMA_Src_Inc = DMA_SINC_INC;
	DMA_InitStruct.DMA_Dest_Msize = DMA_DEST_MSIZE_32;
	DMA_InitStruct.DMA_Src_Msize = DMA_DEST_MSIZE_32;
	DMA_InitStruct.DMA_Trans_Type = DMA_TT_FC_MtoP_DMA;
	DMA_InitStruct.DMA_Periph_Type = UART1_DMA_TX;
	DMA_InitStruct.DMA_Block_Ts = 0x3;
	DMA_InitStruct.DMA_Priority = 0;
	DMA_InitStruct.DMA_Dest_Addr = &UART1_TX;
	DMA_InitStruct.DMA_Src_Addr = 0x29000;
	DMA_Init(&DMA_InitStruct);
	vDelayXms(10);

	UART1_IER = 0;
	DMA_InitStruct.DMA_Dest_Width = DMA_DST_TR_WIDTH_32bits;
	DMA_InitStruct.DMA_Src_Width = DMA_SRC_TR_WIDTH_32bits;
	DMA_InitStruct.DMA_Dest_Inc = DMA_DINC_INC;
	DMA_InitStruct.DMA_Src_Inc = DMA_SINC_No_chagne;
	DMA_InitStruct.DMA_Dest_Msize = DMA_DEST_MSIZE_32;
	DMA_InitStruct.DMA_Src_Msize = DMA_DEST_MSIZE_32;
	DMA_InitStruct.DMA_Trans_Type = DMA_TT_FC_PtoM_DMA;
	DMA_InitStruct.DMA_Periph_Type = UART1_DMA_RX;
	DMA_InitStruct.DMA_Block_Ts = 0x1f;
	DMA_InitStruct.DMA_Priority = 0;
	DMA_InitStruct.DMA_Dest_Addr = 0x2a000;
	DMA_InitStruct.DMA_Src_Addr = &UART1_TX;
	DMA_Init(&DMA_InitStruct);

	//iram -uarta tx
	// REG32(0x29000)=0xffffff41;//A
	// REG32(0x29004)=0xffffff42;//B
	// REG32(0x29008)=0xffffff43;//C
	// sysctl_iomux_uarta(0,0);
	// serial_init(UARTA_CHANNEL, 115200);
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_No_chagne;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_INC;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_MtoP_DMA;
	// DMA_InitStruct.DMA_Periph_Type=UARTA_DMA_TX;
	// DMA_InitStruct.DMA_Block_Ts=0x2;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=&UARTA_TX;
	// DMA_InitStruct.DMA_Src_Addr=0x29000;
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(10);

	//iram -> smbus0 tx  smbus0 rx  -> iram 
	// #define SMBUS I2C_CHANNEL_0						//改
	// smbus0_ModuleClock_EN;
	// REG32(0x29000)=0x00000001;//cmd:0x00(w)      data:0x01
	// REG32(0x29004)=0x00000300;//cmd:0x03(stp/r)  data:0x00
	// REG32(0x29008)=0xffffff10;//C

	// I2c_Write_Short(0x2, I2C_DMA_CR_OFFSET, SMBUS);       //主机使能DMA
	// sysctl_iomux_i2c0(I2C0_CLK_SEL, I2C0_DAT_SEL);
	// I2c_Channel_Init(SMBUS, I2C0_SPEED, I2C_MASTER_ROLE, 0x4c, 1);

	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_No_chagne;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_INC;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_MtoP_DMA;
	// DMA_InitStruct.DMA_Periph_Type=SUBUS0_DMA_TX;          //改
	// DMA_InitStruct.DMA_Block_Ts=0x2;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=0x4010;				// 改
	// DMA_InitStruct.DMA_Src_Addr=0x29000;
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(10);

	// I2c_Write_Short(0x1, I2C_DMA_CR_OFFSET, SMBUS);       //主机使能DMA
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_INC;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_No_chagne;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_PtoM_DMA;
	// DMA_InitStruct.DMA_Periph_Type=SUBUS0_DMA_RX; 	//改
	// DMA_InitStruct.DMA_Block_Ts=0x1;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=0x2a000;	
	// DMA_InitStruct.DMA_Src_Addr=0x4010;			//改
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(10);
	// printf("temp cmd0:0x%x\n",REG32(0x2a000));
	// printf("temp cmd1:0x%x\n",REG32(0x2a004));
	// printf("temp cmd10:0x%x\n",REG32(0x2a008));

	//iram -> smbus1 tx  smbus1 rx  -> iram 
	// #define SMBUS I2C_CHANNEL_1						//改
	// smbus0_ModuleClock_EN;
	// REG32(0x29000)=0x00000001;//cmd:0x00(w)      data:0x01
	// REG32(0x29004)=0x00000300;//cmd:0x03(stp/r)  data:0x00
	// REG32(0x29008)=0xffffff10;//C

	// I2c_Write_Short(0x2, I2C_DMA_CR_OFFSET, SMBUS);       //主机使能DMA
	// sysctl_iomux_i2c1();					//改
	// I2c_Channel_Init(SMBUS, I2C0_SPEED, I2C_MASTER_ROLE, 0x4c, 1);

	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_No_chagne;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_INC;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_MtoP_DMA;
	// DMA_InitStruct.DMA_Periph_Type=SUBUS1_DMA_TX;          //改
	// DMA_InitStruct.DMA_Block_Ts=0x2;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=0x4110;				// 改
	// DMA_InitStruct.DMA_Src_Addr=0x29000;
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(10);

	// I2c_Write_Short(0x1, I2C_DMA_CR_OFFSET, SMBUS);       //主机使能DMA
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_INC;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_No_chagne;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_PtoM_DMA;
	// DMA_InitStruct.DMA_Periph_Type=SUBUS1_DMA_RX; 	//改
	// DMA_InitStruct.DMA_Block_Ts=0x1;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=0x2a000;	
	// DMA_InitStruct.DMA_Src_Addr=0x4110;			//改
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(10);
	// printf("temp cmd0:0x%x\n",REG32(0x2a000));
	// printf("temp cmd1:0x%x\n",REG32(0x2a004));
	// printf("temp cmd10:0x%x\n",REG32(0x2a008));


	//iram -> smbus2 tx  smbus2 rx  -> iram 
	// #define SMBUS I2C_CHANNEL_2						//改
	// smbus0_ModuleClock_EN;
	// REG32(0x29000)=0x00000001;//cmd:0x00(w)      data:0x01
	// REG32(0x29004)=0x00000300;//cmd:0x03(stp/r)  data:0x00
	// REG32(0x29008)=0xffffff10;//C

	// I2c_Write_Short(0x2, I2C_DMA_CR_OFFSET, SMBUS);       //主机使能DMA
	// sysctl_iomux_i2c2(1);					//改
	// I2c_Channel_Init(SMBUS, I2C0_SPEED, I2C_MASTER_ROLE, 0x4c, 1);

	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_No_chagne;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_INC;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_MtoP_DMA;
	// DMA_InitStruct.DMA_Periph_Type=SUBUS2_DMA_TX;          //改
	// DMA_InitStruct.DMA_Block_Ts=0x2;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=0x4210;				// 改
	// DMA_InitStruct.DMA_Src_Addr=0x29000;
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(10);

	// I2c_Write_Short(0x1, I2C_DMA_CR_OFFSET, SMBUS);       //主机使能DMA
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_INC;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_No_chagne;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_PtoM_DMA;
	// DMA_InitStruct.DMA_Periph_Type=SUBUS2_DMA_RX; 	//改
	// DMA_InitStruct.DMA_Block_Ts=0x1;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=0x2a000;	
	// DMA_InitStruct.DMA_Src_Addr=0x4210;			//改
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(10);
	// printf("temp cmd0:0x%x\n",REG32(0x2a000));
	// printf("temp cmd1:0x%x\n",REG32(0x2a004));
	// printf("temp cmd10:0x%x\n",REG32(0x2a008));

	//iram -> smbus3 tx  smbus3 rx  -> iram 
	// #define SMBUS I2C_CHANNEL_3						//改
	// smbus0_ModuleClock_EN;
	// REG32(0x29000)=0x00000001;//cmd:0x00(w)      data:0x01
	// REG32(0x29004)=0x00000300;//cmd:0x03(stp/r)  data:0x00
	// REG32(0x29008)=0xffffff10;//C

	// I2c_Write_Short(0x2, I2C_DMA_CR_OFFSET, SMBUS);       //主机使能DMA
	// sysctl_iomux_i2c3();					//改
	// I2c_Channel_Init(SMBUS, I2C0_SPEED, I2C_MASTER_ROLE, 0x4c, 1);

	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_No_chagne;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_INC;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_MtoP_DMA;
	// DMA_InitStruct.DMA_Periph_Type=SUBUS3_DMA_TX;          //改
	// DMA_InitStruct.DMA_Block_Ts=0x2;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=0x4310;				// 改
	// DMA_InitStruct.DMA_Src_Addr=0x29000;
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(10);

	// I2c_Write_Short(0x1, I2C_DMA_CR_OFFSET, SMBUS);       //主机使能DMA
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_INC;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_No_chagne;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_PtoM_DMA;
	// DMA_InitStruct.DMA_Periph_Type=SUBUS3_DMA_RX; 	//改
	// DMA_InitStruct.DMA_Block_Ts=0x1;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=0x2a000;	
	// DMA_InitStruct.DMA_Src_Addr=0x4310;			//改
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(10);
	// printf("temp cmd0:0x%x\n",REG32(0x2a000));
	// printf("temp cmd1:0x%x\n",REG32(0x2a004));
	// printf("temp cmd10:0x%x\n",REG32(0x2a008));

	//iram -> smbus4 tx  smbus4 rx  -> iram 
	// #define SMBUS I2C_CHANNEL_4						//改
	// smbus0_ModuleClock_EN;
	// REG32(0x29000)=0x00000001;//cmd:0x00(w)      data:0x01
	// REG32(0x29004)=0x00000300;//cmd:0x03(stp/r)  data:0x00
	// REG32(0x29008)=0xffffff10;//C

	// I2c_Write_Short(0x2, I2C_DMA_CR_OFFSET, SMBUS);       //主机使能DMA
	// sysctl_iomux_i2c4();					//改
	// I2c_Channel_Init(SMBUS, I2C0_SPEED, I2C_MASTER_ROLE, 0x4c, 1);
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_No_chagne;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_INC;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_MtoP_DMA;
	// DMA_InitStruct.DMA_Periph_Type=SUBUS4_DMA_TX;          //改
	// DMA_InitStruct.DMA_Block_Ts=0x2;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=0x4510;				// 改
	// DMA_InitStruct.DMA_Src_Addr=0x29000;
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(10);

	// I2c_Write_Short(0x1, I2C_DMA_CR_OFFSET, SMBUS);       //主机使能DMA
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_INC;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_No_chagne;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_PtoM_DMA;
	// DMA_InitStruct.DMA_Periph_Type=SUBUS4_DMA_RX; 	//改
	// DMA_InitStruct.DMA_Block_Ts=0x1;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=0x2a000;	
	// DMA_InitStruct.DMA_Src_Addr=0x4510;			//改
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(10);
	// printf("temp cmd0:0x%x\n",REG32(0x2a000));
	// printf("temp cmd1:0x%x\n",REG32(0x2a004));
	// printf("temp cmd10:0x%x\n",REG32(0x2a008));

	//iram -> smbus5 tx  smbus5 rx  -> iram 
	// #define SMBUS I2C_CHANNEL_5						//改
	// smbus0_ModuleClock_EN;
	// REG32(0x29000)=0x00000001;//cmd:0x00(w)      data:0x01
	// REG32(0x29004)=0x00000300;//cmd:0x03(stp/r)  data:0x00
	// REG32(0x29008)=0xffffff10;//C

	// I2c_Write_Short(0x2, I2C_DMA_CR_OFFSET, SMBUS);       //主机使能DMA
	// sysctl_iomux_i2c5();					//改
	// I2c_Channel_Init(SMBUS, I2C0_SPEED, I2C_MASTER_ROLE, 0x4c, 1);

	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_No_chagne;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_INC;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_MtoP_DMA;
	// DMA_InitStruct.DMA_Periph_Type=SUBUS5_DMA_TX;          //改
	// DMA_InitStruct.DMA_Block_Ts=0x2;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=0x4610;				// 改
	// DMA_InitStruct.DMA_Src_Addr=0x29000;
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(10);

	// I2c_Write_Short(0x1, I2C_DMA_CR_OFFSET, SMBUS);       //主机使能DMA
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_INC;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_No_chagne;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_PtoM_DMA;
	// DMA_InitStruct.DMA_Periph_Type=SUBUS5_DMA_RX; 	//改
	// DMA_InitStruct.DMA_Block_Ts=0x1;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=0x2a000;	
	// DMA_InitStruct.DMA_Src_Addr=0x4610;			//改
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(10);
	// printf("temp cmd0:0x%x\n",REG32(0x2a000));
	// printf("temp cmd1:0x%x\n",REG32(0x2a004));
	// printf("temp cmd10:0x%x\n",REG32(0x2a008));

	//iram -> smbus6 tx  smbus6 rx  -> iram 
	// #define SMBUS I2C_CHANNEL_6						//改
	// smbus0_ModuleClock_EN;
	// REG32(0x29000)=0x00000001;//cmd:0x00(w)      data:0x01
	// REG32(0x29004)=0x00000300;//cmd:0x03(stp/r)  data:0x00
	// REG32(0x29008)=0xffffff10;//C

	// I2c_Write_Short(0x2, I2C_DMA_CR_OFFSET, SMBUS);       //主机使能DMA
	// sysctl_iomux_i2c6();					//改
	// I2c_Channel_Init(SMBUS, I2C0_SPEED, I2C_MASTER_ROLE, 0x4c, 1);
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_No_chagne;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_INC;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_MtoP_DMA;
	// DMA_InitStruct.DMA_Periph_Type=SUBUS6_DMA_TX;          //改
	// DMA_InitStruct.DMA_Block_Ts=0x2;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=0x4710;				// 改
	// DMA_InitStruct.DMA_Src_Addr=0x29000;
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(10);

	// I2c_Write_Short(0x1, I2C_DMA_CR_OFFSET, SMBUS);       //主机使能DMA
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_INC;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_No_chagne;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_PtoM_DMA;
	// DMA_InitStruct.DMA_Periph_Type=SUBUS6_DMA_RX; 	//改
	// DMA_InitStruct.DMA_Block_Ts=0x1;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=0x2a000;	
	// DMA_InitStruct.DMA_Src_Addr=0x4710;			//改
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(10);
	// printf("temp cmd0:0x%x\n",REG32(0x2a000));
	// printf("temp cmd1:0x%x\n",REG32(0x2a004));
	// printf("temp cmd10:0x%x\n",REG32(0x2a008));


	//uartb rx - iram
	// sysctl_iomux_uartb();
	// serial_init(UARTB_CHANNEL, 115200);
	// UARTB_IER=0;
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_INC;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_No_chagne;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_PtoM_DMA;
	// DMA_InitStruct.DMA_Periph_Type=UARTB_DMA_RX;
	// DMA_InitStruct.DMA_Block_Ts=0x1f;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=0x2a000;
	// DMA_InitStruct.DMA_Src_Addr=0x5400;
	// DMA_Init(&DMA_InitStruct);

	//iram -> uarta -tx uarta rx - iram
	// sysctl_iomux_uarta(0,0);
	// serial_init(UARTA_CHANNEL, 115200);
	// UARTA_IER=0;
	// REG32(0x29000)=0xffffff41;//A
	// REG32(0x29004)=0xffffff42;//B
	// REG32(0x29008)=0xffffff43;//C
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_No_chagne;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_INC;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_MtoP_DMA;
	// DMA_InitStruct.DMA_Periph_Type=UARTA_DMA_TX;
	// DMA_InitStruct.DMA_Block_Ts=0x3;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=&UARTA_TX;
	// DMA_InitStruct.DMA_Src_Addr=0x29000;
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(10);

	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_INC;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_No_chagne;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_PtoM_DMA;
	// DMA_InitStruct.DMA_Periph_Type=UARTA_DMA_RX;
	// DMA_InitStruct.DMA_Block_Ts=0x1f;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=0x2a000;
	// DMA_InitStruct.DMA_Src_Addr=&UARTA_TX;
	// DMA_Init(&DMA_InitStruct);

	//uart0 -rx uarta tx 没通
	// sysctl_iomux_uarta(0,0);
	// serial_init(UARTA_CHANNEL, 115200);
	// sysctl_iomux_uartb();
	// serial_init(UARTB_CHANNEL, 115200);
	// UART0_IER=0;
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_8bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_8bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_No_chagne;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_No_chagne;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_PtoP_DMA;
	// DMA_InitStruct.DMA_Periph_Type=UART0_DMA_RX|UARTB_DMA_TX;
	// DMA_InitStruct.DMA_Block_Ts=0x1;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=&UARTB_TX;
	// DMA_InitStruct.DMA_Src_Addr=&UART0_RX;
	// DMA_Init(&DMA_InitStruct);

	//iram -> spim tx spim rx -> iram 
	// printf("\n");
	// printf("iram->spim tx test\n");
	// VDWORD* source_parm_ptr = (VDWORDP)(0x28000);
	// volatile uint8_t * read_buff = (volatile uint8_t *)0x29500;
	// for (int i = 0; i < 40; i++)
	// {
	// 	*(source_parm_ptr + i) = 0xaa4055ff + ((0x0 + i) << 16);
	// 	printf("%x:%x\n", (source_parm_ptr + i), *(source_parm_ptr + i));
	// }
	// sysctl_iomux_spim();
	// sysctl_iomux_spim_cs();
	// SPI_Init(0, SPIM_CPOL_LOW, SPIM_CPHA_FE, SPIM_MSB, 0x7, 1);
	// SPIM_CTRL&=~(0x3<<6);
	// SPIM_CTRL|=1<<7;
	// //擦除FLASH
	// SPI_Block_Erase(0x0, 0);
	// SPI_Send_Cmd_Addr(read_buff, 0, 256, 0);
	// for (int i = 0; i < 30; i++)
	// {
	// 	dprint("Erased data[%d]:0x%x\n",i,read_buff[i]);
	// }

	// //写
	// SPI_Flash_CS_Low(0);
	// SPI_Send_Byte(0x6);//写使能
	// SPI_Flash_CS_High(0);

	// SPI_Flash_CS_Low(0);      //这里拉低片选，等DMA搬运完之后还要把片选拉高，结束传输
	// SPI_Send_Byte(0x02);           //页编程指令
	// SPI_Send_Byte(0);
	// SPI_Send_Byte(0);
	// SPI_Send_Byte(0);           //只发送写使能和地址，数据通过DMA搬运
	// //send data
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_No_chagne;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_INC;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_1;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_1;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_MtoP_DMA;
	// DMA_InitStruct.DMA_Periph_Type=SPIM_DMA_TX;
	// DMA_InitStruct.DMA_Block_Ts=0x1f;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=&SPIM_DA;
	// DMA_InitStruct.DMA_Src_Addr=(VDWORD)source_parm_ptr;
	// DMA_Init(&DMA_InitStruct);
	// vDelayXms(20);
	// SPI_Flash_CS_High(0);

	// SPIM_CTRL&=~(0x3<<6);
	// vDelayXms(10);
	// SPI_Send_Cmd_Addr(read_buff, 0, 256, 0);
	// vDelayXms(1);
	// for (int i = 0; i < 64; i++)
	// {
	// 	dprint("dmatransfered data[%d]:0x%x\n",i,read_buff[i]);
	// }
	// printf("\n");
	// printf("spim rx->iram test\n");
	// SPI_Flash_CS_Low(0);
	// SPI_Send_Byte(CMD_READ_DATA); // 读命令
	// SPI_Send_Byte((0 & 0xFF0000) >> 16);//set addr:0
	// SPI_Send_Byte((0 & 0xFF00) >> 8);
	// SPI_Send_Byte(0 & 0xFF);
	// SPIM_CTRL&=~(0x3<<6);
	// SPIM_CTRL|=1<<6;		      //SPIM DMA接收使能
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_INC;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_No_chagne;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_PtoM_DMA;
	// DMA_InitStruct.DMA_Periph_Type=SPIM_DMA_RX;
	// DMA_InitStruct.DMA_Block_Ts=0x1f;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=0x2a000;
	// DMA_InitStruct.DMA_Src_Addr=&SPIM_DA;
	// DMA_Init(&DMA_InitStruct);
	// #if 0
	// vDelayXms(1);
	// for(int j=0;j<31;j++)
	// {
	// 	SPI_Timeout = 100000;
	// 	while (!((SPIM_SR)&SPI_TFE)) // 等待发送fifo空
	// 	{
	// 		SPI_Timeout--;
	// 		if (SPI_Timeout == 0)
	// 		{
	// 		dprint("spi等待发送超时\n");
	// 		return 0;
	// 		}
	// 	}
	// 	SPIM_DA = 0xff; // 发送数据
	// 	SPI_Timeout = 1000000;
	// 	while (!((SPIM_SR)&SPI_RFNE)) // 等待接收fifo不空
	// 	{
	// 		SPI_Timeout--;
	// 		if (SPI_Timeout == 0)
	// 		{
	// 		dprint("spi等待接收超时\n");
	// 		return 0;
	// 		}
	// 	}
	// }
	// vDelayXms(20);   //防止片选过早拉高，导致最后一个数读不出来,正向
	// SPI_Flash_CS_High(0);
	// //DMA通道使能
	// DMA_ChEnReg = (0x1 | 0x1 << 8);
	// vDelayXms(20);
	// for (int i = 0; i < 30; i++)
	// {
	// 	dprint("Read data[%d]:0x%x\n",i,REG32(0x2a000+i*4));
	// }
	// #else
	// #define ADDRess 0x0
	// #define CS_sel 0
	// SPIM_CTRL = 0x6754;            //dma_wr_en:0,dma_rd_en=1   //反向是SPIM搬给IRAM，DMA是接收方，所以要打开接收FIFO使能
	// SPIM_CPSR = 0x1;                                                // 时钟分频
	// SPIM_IMSR = 0x1f;
	// //SPIM往FLASH中提取数据
	// unsigned char ad[3];
	// ad[0] = (ADDRess >> 16) & 0xff;
	// ad[1] = (ADDRess >> 8) & 0xff;
	// ad[2] = (ADDRess) & 0xff;
	// SPI_Flash_CS_Low(CS_sel);
	// SPI_Send_Byte(0x03);    //读命令指令
	// SPI_Send_Byte(ad[0]);
	// SPI_Send_Byte(ad[1]);
	// SPI_Send_Byte(ad[2]);
	// //把FLASH中读取到的数据存放在SPIM的数据寄存器上，不提走，让DMA提走传给IRAM
	// for (int i = 0; i < 40; i++)
	// {
	// 	while ((SPIM_SR & 0x1) == 0x0);//wait tx_fifo not full
	// 	// 写入 SPI 数据寄存器
	// 	REG16(0x6012) = 0xff;
	// 	// 等待 RX FIFO 非空
	// 	while ((SPIM_SR & 0x4) == 0x0);//wait rx_fifo not empty
	// }
	// vDelayXms(1);   //防止片选过早拉高，导致最后一个数读不出来,正向
	// SPI_Flash_CS_High(CS_sel);
	// vDelayXms(1);//反向
	// //DMA通道使能
	// DMA_ChEnReg = (0x1 | 0x1 << 8);
	// vDelayXms(1);
	// for (int i = 0; i < 30; i++)
	// {
	// 	dprint("Read data[%d]:0x%x\n",i,REG32(0x2a000+i*4));
	// }
	// #endif

	// SPIM_CTRL&=~(0x3<<6);
	// SPI_Send_Cmd_Addr(read_buff, 0, 256, 0);
	// vDelayXms(1);
	// for (int i = 0; i < 256; i++)
	// {
	// 	dprint("end flash data[%d]:0x%x\n",i,read_buff[i]);
	// }

	//iram -> i3c tx
	// DMA_InitStruct.DMA_Dest_Width=DMA_DST_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Src_Width=DMA_SRC_TR_WIDTH_32bits;
	// DMA_InitStruct.DMA_Dest_Inc=DMA_DINC_No_chagne;
	// DMA_InitStruct.DMA_Src_Inc=DMA_SINC_INC;
	// DMA_InitStruct.DMA_Dest_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Src_Msize=DMA_DEST_MSIZE_32;
	// DMA_InitStruct.DMA_Trans_Type=DMA_TT_FC_MtoP_DMA;
	// DMA_InitStruct.DMA_Periph_Type=I3C0_DMA_TX;          //改
	// DMA_InitStruct.DMA_Block_Ts=0x2;
	// DMA_InitStruct.DMA_Priority=0;
	// DMA_InitStruct.DMA_Dest_Addr=&TX_DATA_PORT_0;		// 改
	// DMA_InitStruct.DMA_Src_Addr=0x29000;
	// DMA_Init(&DMA_InitStruct);

	dprint("This is external flash\n");
	Specific_Mem_init(); // memory init
	test_loop();
}
#endif

// #ifdef AE103
// #define GPIOB4_NOSET 0
// void AE103_Sleep(void)
// {

// // 	//sleep mode config
// // 	GPIO_Input_EN(GPIOA, 3, ENABLE);
// // 	SYSCTL_SLEEPCFG &= ~(BIT2);//main_clk_sel = 0  32k
// // 	//SYSCTL_SLEEPCFG |= BIT2;//main_clk_sel = 1
// // 	SYSCTL_DVDD_EN |= BIT4 | BIT5 | BIT6;
// // 	SYSCTL_DVDD_EN &= ~(BIT0 | BIT1 | BIT2);
// // 	//SYSCTL_SLEEPCFG |= BIT1;//enable osc80m
// // 	SYSCTL_SLEEPCFG &= ~(BIT1 | BIT3); // disable osc80m 并设置 sleep mode
// // 	//printf("SYSCTL_SLEEPCFG:%x\n", SYSCTL_SLEEPCFG);
// // 	SYSCTL_CLKDIV_SPIS = 0x0;
// // 	SYSCTL_CLKDIV_PECI = 0x0;
// // 	SYSCTL_PMUCSR |= BIT(20);//Enable WFI Mode
// // 	SYSCTL_PIOA_IECFG = 0x7008008;
// // 	//printf("SYSCTL_PIOA_IECFG:0x%x\n", SYSCTL_PIOA_IECFG);
// // 	SYSCTL_PIOB_IECFG = 0x8;
// // 	//GPIO_Input_EN(GPIOA, 3, ENABLE);
// // 	//printf("SYSCTL_PIOB_IECFG:0x%x\n", SYSCTL_PIOB_IECFG);
// // 	SYSCTL_PIOCD_IECFG = 0x0;
// // 	//printf("SYSCTL_PIOCD_IECFG:0x%x\n", SYSCTL_PIOCD_IECFG);
// // 	SYSCTL_PIOE_IECFG = 0x3000000;
// // 	//printf("SYSCTL_PIOE_IECFG:0x%x\n", SYSCTL_PIOE_IECFG);
// // 	ADC_PM = 0b11;//ADC low power config bit0 close ldo and bit1 close comp
// // //#if GPIOB4_NOSET//NOUP
// // 	//mode 0 no run 
// // 	//SYSCTL_MODEN0 = 0;
// // 	//modle 1 spif(bit3)run  cache(19)run 
// // 	//SYSCTL_MODEN1 = SPIFE_EN | CACHE_EN;
// // //#else//GPIOB4_NOSET
// // 	////mode 0 uart0 run gpio run 
// // 	SYSCTL_MODEN0 = UART0_EN | GPIO_EN | PWM_EN;
// // 		//modle 1 spif(bit3)run  cache(19)run 
// // 	SYSCTL_MODEN1 = DRAM_EN | SYSCTL_EN | SPIFE_EN | APB_EN | GPIODB_EN | CACHE_EN | IRAM_EN | ICTL_EN | IVT_EN;
// // 	GPIO0_DEBOUNCE0 = 0x0;
// // 	//printf("SYSCTL_MODEN1:0x%x\n", SYSCTL_MODEN1);
// // // SYSCTL_MODEN0 = 0xffffffff;
// // // SYSCTL_MODEN1 = 0xffffffff;
// // //#endif
// // // #if SUPPORT_GPIO_WAKEUP	
// // // 	GPIO_Config(GPIOA, 0, 2, 0, 1, 0);
// // // #endif
// // 	//printf("SYSCTL_SLEEPCFG:0x%x\n", SYSCTL_SLEEPCFG);
// // 	//sysctl_iomux_pwm1();
// // 	//PWM_Init_channel(PWM_CHANNEL1, PWM_HIGH, PWM_CLK0, PWM_CTR0, 50, 0);
// // 	//asm volatile("wfi");
// }
// void AE103_Deep_Sleep(void)
// {

// // 	SYSCTL_PIOA_IECFG = 0x0;
// // #if 0//NOUP
// // 	if(SYSCTL_PIO_CFG & BIT1)
// // 		SYSCTL_PIOB_IECFG = 0x00f00000;//only open spif
// // 	else
// // 		SYSCTL_PIOB_IECFG = 0x0;
// // #else//GPIOB4_NOSET
// // 	if(SYSCTL_PIO_CFG & BIT1)
// // 		SYSCTL_PIOB_IECFG = 0x00f00000 | BIT4;// open spif
// // 	else
// // 		SYSCTL_PIOB_IECFG = BIT4;
// // 	ICTL0_INTMASK4 |= BIT4;
// // 	GPIO1_INTMASK0 |= BIT4;//先屏蔽中断
// // 	GPIO1_DDR0 &= (~BIT4);//配置位输入
// // 	GPIO1_INTEN0 |= BIT4;//使能中断
// // 	GPIO1_INTTYPE_LEVEL0 |= BIT4;//设置为边沿触发
// // 	GPIO1_INT_POLARITY0 &= ~BIT4;//设置为下降沿触发
// // 	GPIO1_INTMASK0 &= ~BIT4;//取消中断屏蔽
// // 	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_INTR0, en, 1, 0);
// // 	ICTL0_INTEN4 |= BIT4;
// // 	set_csr(mstatus, MSTATUS_MIE);
// // 	ICTL0_INTMASK4 &= ~BIT4;
// // #endif

// // 	if(SYSCTL_PIO_CFG & BIT1)
// // 		SYSCTL_PIOB_IECFG = 0x00f00000;//only open spif
// // 	else
// // 		SYSCTL_PIOB_IECFG = 0x0;//only open spif
// // 	SYSCTL_PIOCD_IECFG = 0x0;
// // 	SYSCTL_PIOE_IECFG = 0x0;
// // 	SYSCTL_CLKDIV_I3C = 0x0;
// // 	SYSCTL_CLKDIV_PECI = 0x0;
// // 	SYSCTL_PMUCSR |= BIT(20);//Enable WFI Mode
// // 	//SET Enable DeepSleep and Switch PLL
// // 	//BIT0:1:SWITCH Internel OSC32k/196M(0:switch externel OSC32k/pll)
// // 	//BIT1:0:Disable Internel OSC192M(1:ENable Internel OSC192M)
// // 	//BIT2:1:Sleep Main Freq Set High Clock(0:Sleep Main Freq Set Low Clock)
// // 	//BIT3:0:Sleep No close High Clock(1:wait main freq set low clock(bit2=0),close high clock)
// // 	SYSCTL_SLEEPCFG = BIT3 | BIT0;
// // 	ADC_PM = 0b11;//ADC low power config bit0 close ldo and bit1 close comp 
// // 	SYSCTL_DVDD_EN |= BIT4 | BIT5 | BIT6;
// // 	SYSCTL_DVDD_EN &= ~(BIT0 | BIT1 | BIT2);
// // #if GPIOB4_NOSET//NOUP
// // 	//mode 0 no run 
// // 	SYSCTL_MODEN0 = 0;
// // 	//modle 1 spif(bit3)run  cache(19)run 
// // 	SYSCTL_MODEN1 = SPIFE_EN | CACHE_EN;
// // #else//GPIOB4_NOSET
// // 	////mode 0 uart0 run gpio run 
// // 	SYSCTL_MODEN0 = UART0_EN | GPIO_EN;
// // 	//modle 1 spif(bit3)run  cache(19)run 
// // 	SYSCTL_MODEN1 = SPIFE_EN | CACHE_EN | ICTL_EN;
// // #endif
// // 	asm volatile("wfi");
// }
// #define LOW_TRIM_DVAL 0
// #define High_TRIM_DVAL 0
// #define Delay_RTC_Timer 10
// void Test_OSC32K(void)
// {
// 	const u32 mask = 0x3ff;
// 	u8 offset = 0;
// 	u32 trim;
// 	trim = (REG32(0x304D8) >> offset) & mask;
// 	trim += LOW_TRIM_DVAL;
// 	REG32(0x304D8) = (REG32(0x304D8) & (~(mask << offset))) | ((trim & mask) << offset);//LOW
// 	offset = 13;
// 	trim = (REG32(0x304D8) >> offset) & mask;
// 	trim += High_TRIM_DVAL;
// 	REG32(0x304D8) = (REG32(0x304D8) & (~(mask << offset))) | ((trim & mask) << offset);//LOW//HIGH
// 	//open pwm clk and reset pwm mode
// 	SYSCTL_MODEN0 |= PWM_EN;
// 	SYSCTL_RST0 |= PWM_EN;
// 	nop;
// 	SYSCTL_RST0 &= (~PWM_EN);
// 	nop;
// 	//set PWM[7:0] IO MUX pin multiplexing to GPA[7:0]
// 	sysctl_iomux_pwm0();
// 	sysctl_iomux_pwm1();
// 	sysctl_iomux_pwm2();
// 	sysctl_iomux_pwm3();
// 	sysctl_iomux_pwm4();
// 	sysctl_iomux_pwm5();
// 	sysctl_iomux_pwm6();
// 	sysctl_iomux_pwm7();
// 	//SYSCTL PMU CONFIG STATUS REGISTER Set  Enable CPU WFI 
// 	//WFI:Wait For Interrupt
// 	//WFE:Wait For Event(close)
// 	SYSCTL_PMUCSR |= BIT(20);
// 	//Control PWM 8 Divider
// 	PWM_CTR0_1 = 0x101;
// 	PWM_CTR2_3 = 0x101;
// 	PWM_DCR0_1 = 0x101;
// 	PWM_DCR2_3 = 0x101;
// 	PWM_DCR4_5 = 0x101;
// 	PWM_DCR6_7 = 0x101;
// 	PWM_CTRL = 0xff01;
// 	//delay 800us run 24/8M PWM(3M)
// 	vDelayXus(200);
// 	vDelayXus(200);
// 	vDelayXus(200);
// 	vDelayXus(200);
// 	//RTC WAKEUP
// 	RTC_CCR0 &= ~RTC_CCR_EN;
// 	Set_RTC_MatchVal(Delay_RTC_Timer + Get_RTC_CountVal());
// 	RTC_CCR0 |= RTC_CCR_EN | RTC_CCR_IEN | RTC_CCR_WEN | RTC_CCR_PSCLREN;
// 	//CPSR set 1 sec
// 	Set_RTC_PreScaler(LOW_CHIP_CLOCK);
// 	//SET Enable DeepSleep and Switch PLL
// 	//BIT0:1:SWITCH Internel OSC32k/196M(0:switch externel OSC32k/pll)
// 	//BIT1:0:Disable Internel OSC192M(1:ENable Internel OSC192M)
// 	//BIT2:1:Sleep Main Freq Set High Clock(0:Sleep Main Freq Set Low Clock)
// 	//BIT3:0:Sleep No close High Clock(1:wait main freq set low clock(bit2=0),close high clock)
// 	SYSCTL_SLEEPCFG = BIT3 | BIT0;
// 	//mode 0 pwm(bit11)run
// 	SYSCTL_MODEN0 = PWM_EN;
// 	//modle 1 spif(bit3)run apb(11)run cache(19)run 
// 	SYSCTL_MODEN1 = SPIFE_EN | APB_EN | CACHE_EN | RTC_EN;
// 	CPU_Sleep();
// }
// #endif

// ALIGNED(4) void Go2Ram_WaitUpdate(void)
// {
//     while(1)
//     {
//         if(MAILBOX_C2EINT == 0x2)
//         {
//             if(MAILBOX_C2EINFO0 == 0x13)
//             {
//                 PRINTF_TX = 'S';
//                 PRINTF_TX = 'U';
//                 PRINTF_TX = 'C';
//                 PRINTF_TX = 'C';
//                 PRINTF_TX = 'E';
//                 PRINTF_TX = 'S';
//                 SYSCTL_RST1 |= BIT(16);//chip reset
//                 __nop;
//                 SYSCTL_RST1 &= ~BIT(16);//release chip reset
//                 goto * 80084UL;
//             }
//         }
//     }
// }

// void Flash_Update_Function(void)
// {
//     uint32_t Update_Addr = 0;
//     uint32_t FM_size = (REG8((SRAM_BASE_ADDR + 0x104))) | (REG8((SRAM_BASE_ADDR + 0x105)) << 8) | (REG8((SRAM_BASE_ADDR + 0x106)) << 16) | (REG8((SRAM_BASE_ADDR + 0x107)) << 24);
//     uint8_t flashOption = REG8((SRAM_BASE_ADDR + 0x110));
//     printf("FM_size:%d\n", FM_size);
//     printf("flashOption:%x\n", flashOption);
//     if(SYSCTL_RST1 & 0x00000100)
//     {
//         SYSCTL_RST1 &= 0xfffffeff; // 释放复位
//     }
//     sysctl_iomux_config(GPIOB, 17, 0x1);//wp
//     sysctl_iomux_config(GPIOB, 20, 0x1);//mosi
//     sysctl_iomux_config(GPIOB, 21, 0x1);//miso
//     sysctl_iomux_config(GPIOB, 22, 0x1);//cs0
//     sysctl_iomux_config(GPIOB, 23, 0x1);//clk
//     sysctl_iomux_config(GPIOB, 30, 0x1);//hold
//     /* 中断屏蔽 */
//     Disable_Interrupt_Main_Switch();
//     MAILBOX_E2CINFO0 = 0x13;
//     MAILBOX_E2CINFO1 = flashOption;//0x0:内部flash，0x1:外部flash
//     MAILBOX_E2CINFO2 = FM_size;//更新大小
//     MAILBOX_E2CINFO3 = Update_Addr;//更新地址
//     /* 进入ram跑代码 */
//     Smf_Ptr = Load_Smfi_To_Dram(Go2Ram_WaitUpdate, 0x400);
//     MAILBOX_E2CINT = 0x2; // 触发对应中断
//     (*Smf_Ptr)(); // Do Function at malloc address
// }

// #if 1
// extern void write_efuse_data(uint32_t *data);

// int do_efuse(struct cmd_tbl *cmd, int flags, int argc, char *const argv[])
// {
//     uint32_t  data[2] = { cmd_atoi(argv[1]), cmd_atoi(argv[2]) };
//     write_efuse_data(data);
//     return 0;
// }
// #endif
// void ALIGNED(4) GLE01_RomCode_Transport_FlashToIRAM0(void)
// {
//   asm volatile("la a0, 0xC0000"); // 搬运的FLASH起始地址(0x80000+256K)
//   asm volatile("la a1, 0xC8000"); // 搬运的FLASH终止地址(0x80000+256K+32K)
//   asm volatile("la a2, 0x28000"); // 搬运的IRAM0目标地址

//   asm volatile("1:");
//   asm volatile("lw t0, (a0)");
//   asm volatile("sw t0, (a2)");
//   asm volatile("addi a0, a0, 4");
//   asm volatile("addi a2, a2, 4");
//   asm volatile("bltu a0, a1, 1b");

//   return;
// }

// void GLE01_RomCode_Transport(void)
// {
//   Disable_Interrupt_Main_Switch();
//   GLE01_RomCode_Transport_FlashToIRAM0();
//   // GLE01_RomCode_Ptr = Load_Smfi_To_Dram(GLE01_RomCode_Transport_FlashToIRAM0, 0x200);
//   // (*GLE01_RomCode_Ptr)(); // Do Function at malloc address
//   Enable_Interrupt_Main_Switch();
//   SYSCTL_CRYPTODBG_FLAG |= 1;
//   dprint("GLE01 ROMCODE Transport Flash to IRAM0\n");
// }

// void ALIGNED(4) Vtable_Tansport_FlashToIVT(void)
// {
//   asm volatile("la a0, 0x80000"); // 搬运的Vector table起始地址
//   asm volatile("la a1, 0x80084"); // 搬运的Vector table终止地址
//   asm volatile("la a2, 0x30800"); // 搬运的IVT空间目标地址

//   asm volatile("1:");
//   asm volatile("lw t0, (a0)");
//   asm volatile("sw t0, (a2)");
//   asm volatile("addi a0, a0, 4");
//   asm volatile("addi a2, a2, 4");
//   asm volatile("bltu a0, a1, 1b");

//   return;
// }

// void Vtable_Tansport(void)
// {
//   Disable_Interrupt_Main_Switch();
//   IVT_Ptr = Load_Smfi_To_Dram(Vtable_Tansport_FlashToIVT, 0x200);
//   (*IVT_Ptr)(); // Do Function at malloc address
//   Enable_Interrupt_Main_Switch();
//   // free(IVT_Ptr);
//   dprint("vector table Transport to IVT Space\n");
// }

#define I3C_TEST 0
#if I3C_TEST//收到,已确认暂时不管这一段代码
uint8_t ccc_wdata[10] = { 0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa };
uint8_t ccc_rdata[10] = { 0,0,0,0,0,0,0,0,0,0 };
uint8_t ccc_rdata1[10] = { 0,0,0,0,0,0,0,0,0,0 };
uint8_t ccc_dr_rdata[10] = { 0,0,0,0,0,0,0,0,0,0 };
uint8_t ccc_dr_rdata1[10] = { 0,0,0,0,0,0,0,0,0,0 };
#endif

// int __weak main(void)
// {
// 	AutoON_Check_AfterUpdate(); // 检查更新后重启设定标志
// #if DEBUG
// 	printr(printf_instructions_msg);
// 	printr("Core Version AE%d\n", CORE_Version);
// 	printr("Code Version %s\n", CODE_VERSION);
// 	printr("Compile Time : %s %s\n", __DATE__, __TIME__);
// #endif
// 	// 2. print Operational information
// #ifdef AE103
// 	extern void write_efuse_data(uint32_t * data);
// 	write_efuse_data(NULL);
// 	if(SYSCTL_PIO_CFG & BIT1)
// 	{
// 		// earse_internel_flash();
// 		dprint("This is external flash main\n");
// 	}
// 	else
// 	{
// 		dprint("This is internal flash main\n");
// 	}
// #endif
// 	dprint("CPU freq at %d Hz\n", CPU_FREQ);

#if I3C_TEST//收到,已确认暂时不管这一段代码
	/******仅供i3c测试,泽宇先别删这段测试代码start */
	// printf("master0/slave0测试\n");
	// I3C_MASTER_BC_CCC_WRITE(ccc_wdata, 2, SETMWL_BC_CMD, 0, 0, I3C_MASTER0);
	// printf("slave0 maxlength:%x\n", SLAVE0_MAXLIMITS);
	// I3C_MASTER_PV_WRITE_WITH7E(0x3a, ccc_wdata, 5, I3C_MASTER0);
	// I3C_SLAVE_READ(ccc_rdata, 5, I3C_SLAVE0);
	// for (int i = 0; i < 5; i++)
	// {
	// 	printf("private read data%x:%x\n", i, ccc_rdata[i]);
	// }
	// I3C_SLAVE_WRITE(ccc_wdata, 5, I3C_SLAVE0);
	// I3C_MASTER_PV_READ_WITH7E(0x3a, ccc_rdata1, 5, I3C_MASTER0);
	// for (int i = 0; i < 5; i++)
	// {
	// 	printf("private read data%x:%x\n", i, ccc_rdata1[i]);
	// }
	// I3C_MASTER_DR_CCC_WRITE(0x3a, ccc_wdata, 2, SETMWL_DR_CMD, 0, 0, I3C_MASTER0);
	// printf("slave0 maxlength:%x\n", SLAVE0_MAXLIMITS);
	// I3C_MASTER_DR_CCC_READ(0x3a, ccc_dr_rdata, 1, GETDCR_DR_CMD, 0, 0, I3C_MASTER0);
	// printf("direct read dcr:%x\n", ccc_dr_rdata[0]);

	// // I3C_SLAVE_IBI_HOTJOIN(I3C_SLAVE0_STATIC_ADDR, I3C_SLAVE0_DEFAULT_IDPARTNO, I3C_SLAVE0_DEFAULT_DCR, I3C_SLAVE0_DEFAULT_BCR, I3C_SLAVE0);
	// I3C_SLAVE_IBI_DATA(0x55, I3C_SLAVE0_DEFAULT_IDPARTNO, I3C_SLAVE0_DEFAULT_DCR, I3C_SLAVE0_DEFAULT_BCR, I3C_SLAVE0);

	// printf("master0/slave1测试\n");
	// I3C_MASTER_BC_CCC_WRITE(ccc_wdata, 2, SETMWL_BC_CMD, 0, 0, I3C_MASTER0);
	// printf("slave1 maxlength:%x\n", SLAVE1_MAXLIMITS);
	// I3C_MASTER_PV_WRITE_WITH7E(0x3b, ccc_wdata, 5, I3C_MASTER0);
	// I3C_SLAVE_READ(ccc_rdata, 5, I3C_SLAVE1);
	// for (int i = 0; i < 5; i++)
	// {
	// 	printf("private read data%x:%x\n", i, ccc_rdata[i]);
	// }
	// I3C_SLAVE_WRITE(ccc_wdata, 5, I3C_SLAVE1);
	// I3C_MASTER_PV_READ_WITH7E(0x3b, ccc_rdata1, 5, I3C_MASTER0);
	// for (int i = 0; i < 5; i++)
	// {
	// 	printf("private read data%x:%x\n", i, ccc_rdata1[i]);
	// }
	// I3C_MASTER_DR_CCC_WRITE(0x3b, ccc_wdata, 2, SETMWL_DR_CMD, 0, 0, I3C_MASTER0);
	// printf("slave1 maxlength:%x\n", SLAVE1_MAXLIMITS);
	// I3C_MASTER_DR_CCC_READ(0x3b, ccc_dr_rdata, 1, GETDCR_DR_CMD, 0, 0, I3C_MASTER0);
	// printf("direct read dcr:%x\n", ccc_dr_rdata[0]);

	// // I3C_SLAVE_IBI_HOTJOIN(I3C_SLAVE0_STATIC_ADDR, I3C_SLAVE0_DEFAULT_IDPARTNO, I3C_SLAVE0_DEFAULT_DCR, I3C_SLAVE1_DEFAULT_BCR, I3C_SLAVE0);
	// I3C_SLAVE_IBI_DATA(0x77, I3C_SLAVE0_DEFAULT_IDPARTNO, I3C_SLAVE0_DEFAULT_DCR, I3C_SLAVE0_DEFAULT_BCR, I3C_SLAVE1);

	// printf("master1/slave0测试\n");
	// I3C_MASTER_BC_CCC_WRITE(ccc_wdata, 2, SETMWL_BC_CMD, 0, 0, I3C_MASTER1);
	// printf("slave0 maxlength:%x\n", SLAVE0_MAXLIMITS);
	// I3C_MASTER_PV_WRITE_WITH7E(0x3a, ccc_wdata, 5, I3C_MASTER1);
	// I3C_SLAVE_READ(ccc_rdata, 5, I3C_SLAVE0);
	// for (int i = 0; i < 5; i++)
	// {
	// 	printf("private read data%x:%x\n", i, ccc_rdata[i]);
	// }
	// I3C_SLAVE_WRITE(ccc_wdata, 5, I3C_SLAVE0);
	// I3C_MASTER_PV_READ_WITH7E(0x3a, ccc_rdata1, 5, I3C_MASTER1);
	// for (int i = 0; i < 5; i++)
	// {
	// 	printf("private read data%x:%x\n", i, ccc_rdata1[i]);
	// }
	// I3C_MASTER_DR_CCC_WRITE(0x3a, ccc_wdata, 2, SETMWL_DR_CMD, 0, 0, I3C_MASTER1);
	// printf("slave0 maxlength:%x\n", SLAVE0_MAXLIMITS);
	// I3C_MASTER_DR_CCC_READ(0x3a, ccc_dr_rdata, 1, GETDCR_DR_CMD, 0, 0, I3C_MASTER1);
	// printf("direct read dcr:%x\n", ccc_dr_rdata[0]);

	// // I3C_SLAVE_IBI_HOTJOIN(I3C_SLAVE0_STATIC_ADDR, I3C_SLAVE0_DEFAULT_IDPARTNO, I3C_SLAVE0_DEFAULT_DCR, I3C_SLAVE0_DEFAULT_BCR, I3C_SLAVE0);
	// I3C_SLAVE_IBI_DATA(0x55, I3C_SLAVE0_DEFAULT_IDPARTNO, I3C_SLAVE0_DEFAULT_DCR, I3C_SLAVE0_DEFAULT_BCR, I3C_SLAVE0);

	// printf("master1/slave1测试\n");
	// I3C_MASTER_BC_CCC_WRITE(ccc_wdata, 2, SETMWL_BC_CMD, 0, 0, I3C_MASTER1);
	// printf("slave1 maxlength:%x\n", SLAVE1_MAXLIMITS);
	// I3C_MASTER_PV_WRITE_WITH7E(0x3b, ccc_wdata, 5, I3C_MASTER1);
	// I3C_SLAVE_READ(ccc_rdata, 5, I3C_SLAVE1);
	// for (int i = 0; i < 5; i++)
	// {
	// 	printf("private read data%x:%x\n", i, ccc_rdata[i]);
	// }
	// I3C_SLAVE_WRITE(ccc_wdata, 5, I3C_SLAVE1);
	// I3C_MASTER_PV_READ_WITH7E(0x3b, ccc_rdata1, 5, I3C_MASTER1);
	// for (int i = 0; i < 5; i++)
	// {
	// 	printf("private read data%x:%x\n", i, ccc_rdata1[i]);
	// }
	// I3C_MASTER_DR_CCC_WRITE(0x3b, ccc_wdata, 2, SETMWL_DR_CMD, 0, 0, I3C_MASTER1);
	// printf("slave1 maxlength:%x\n", SLAVE1_MAXLIMITS);
	// I3C_MASTER_DR_CCC_READ(0x3b, ccc_dr_rdata, 1, GETDCR_DR_CMD, 0, 0, I3C_MASTER1);
	// printf("direct read dcr:%x\n", ccc_dr_rdata[0]);

	// // I3C_SLAVE_IBI_HOTJOIN(I3C_SLAVE0_STATIC_ADDR, I3C_SLAVE0_DEFAULT_IDPARTNO, I3C_SLAVE0_DEFAULT_DCR, I3C_SLAVE1_DEFAULT_BCR, I3C_SLAVE0);
	// I3C_SLAVE_IBI_DATA(0x77, I3C_SLAVE1_DEFAULT_IDPARTNO, I3C_SLAVE1_DEFAULT_DCR, I3C_SLAVE1_DEFAULT_BCR, I3C_SLAVE1);
	/*******end */
#endif

// 	// 若打开GLE01功能，则需要在GLE01主系统第一次启动时，将FLASH文件的256K后的固定32K代码搬运到IRAM0
// #if ((GLE01 == 1) && (FLASH_TO_IRAM0 == 1))
// 	GLE01_RomCode_Transport();
// #endif
// 	// TaskParams Params = { (APB_UART1 | APB_REQ),0,0 };
// 	// Add_Task(Mailbox_APB2_Source_Alloc_Trigger, Params, &task_head);//分配串口1给子系统
// 	main_loop();
// 	return 0;
// }
// // SECTION(".ROM_PATCH")
// // USED spatch_typdef patch_array[8] = {
// // // 0x10084
// // #if (defined(PATCH0_ADDR) && defined(PATCH0_DATA))
// // 	[0].addrl = (PATCH0_ADDR >> 2) & 0xff,
// // 	[0].addrh = (PATCH0_ADDR >> 10) & 0x1f,
// // 	[0].data.dword = JAL_CODE(X1_RA, PATCH0_ADDR, PATCH0_DATA),
// // 	/* [0].data.byte[0]=JAL_CODE_BYTE0(X1_RA),
// // 	[0].data.byte[1]=JAL_CODE_BYTE1(X1_RA,JAL_ADDR_OFFSET(PATCH0_ADDR,PATCH0_DATA)),
// // 	[0].data.byte[2]=JAL_CODE_BYTE2(JAL_ADDR_OFFSET(PATCH0_ADDR,PATCH0_DATA)),
// // 	[0].data.byte[3]=JAL_CODE_BYTE3(JAL_ADDR_OFFSET(PATCH0_ADDR,PATCH0_DATA)), */
// // #endif
// // #if (defined(PATCH1_ADDR) && defined(PATCH1_DATA))
// // 	[1].addrl = (PATCH1_ADDR >> 2) & 0xff,
// // 	[1].addrh = (PATCH1_ADDR >> 10) & 0x1f,
// // 	[1].data.dword = JAL_CODE(X1_RA, PATCH1_ADDR, PATCH1_DATA),
// // #endif
// // #if (defined(PATCH2_ADDR) && defined(PATCH2_DATA))
// // 	[2].addrl = (PATCH2_ADDR >> 2) & 0xff,
// // 	[2].addrh = (PATCH2_ADDR >> 10) & 0x1f,
// // 	[2].data.dword = JAL_CODE(X1_RA, PATCH2_ADDR, PATCH2_DATA),
// // #endif
// // #if (defined(PATCH3_ADDR) && defined(PATCH3_DATA))
// // 	[3].addrl = (PATCH3_ADDR >> 2) & 0xff,
// // 	[3].addrh = (PATCH3_ADDR >> 10) & 0x1f,
// // 	[3].data.dword = JAL_CODE(X1_RA, PATCH3_ADDR, PATCH3_DATA),
// // #endif
// // #if (defined(PATCH4_ADDR) && defined(PATCH4_DATA))
// // 	[4].addrl = (PATCH4_ADDR >> 2) & 0xff,
// // 	[4].addrh = (PATCH4_ADDR >> 10) & 0x1f,
// // 	[4].data.dword = JAL_CODE(X1_RA, PATCH4_ADDR, PATCH4_DATA),
// // #endif
// // #if (defined(PATCH5_ADDR) && defined(PATCH5_DATA))
// // 	[5].addrl = (PATCH5_ADDR >> 2) & 0xff,
// // 	[5].addrh = (PATCH5_ADDR >> 10) & 0x1f,
// // 	[5].data.dword = JAL_CODE(X1_RA, PATCH5_ADDR, PATCH5_DATA),
// // #endif
// // #if (defined(PATCH6_ADDR) && defined(PATCH6_DATA))
// // 	[6].addrl = (PATCH6_ADDR >> 2) & 0xff,
// // 	[6].addrh = (PATCH6_ADDR >> 10) & 0x1f,
// // 	[6].data.dword = JAL_CODE(X1_RA, PATCH6_ADDR, PATCH6_DATA),
// // #endif
// // #if (defined(PATCH7_ADDR) && defined(PATCH7_DATA))
// // 	[7].addrl = (PATCH7_ADDR >> 2) & 0xff,
// // 	[7].addrh = (PATCH7_ADDR >> 10) & 0x1f,
// // 	[7].data.dword = JAL_CODE(X1_RA, PATCH7_ADDR, PATCH7_DATA),
// // #endif
// // };
// typedef struct _efuse_info
// {
// 	uint32_t trim_ldo1 : 8;//EFUSE_DATA[7:0]
// 	uint32_t trim_bg : 8;//EFUSE_DATA[15:8]
// 	uint32_t rom_patch_disable : 1;//EFUSE_DATA[16]
// 	uint32_t wakeup_bypass_security_disable : 1;//EFUSE_DATA[17]
// 	uint32_t program_disable : 1;//EFUSE_DATA[18]
// 	uint32_t crypto_normal : 1;//EFUSE_DATA[19]
// 	uint32_t trim_32k : 12;//EFUSE_DATA[31:20]
// 	uint32_t crypto_clkgate_disable : 1;//EFUSE_DATA[32]
// 	uint32_t trim_96m : 10;//EFUSE_DATA[42:33]
// 	uint32_t reserved0 : 3;//EFUSE_DATA[45:43]
// 	uint32_t crypto_rompatch_disable : 1;//EFUSE_DATA[46]
// 	uint32_t eflash_512k : 1;//EFUSE_DATA[47]
// 	uint32_t ec_debug : 1;//     EFUSE_DATA[48]
// 	uint32_t crypto_debug : 1;//EFUSE_DATA[49]
// 	uint32_t hash_size : 7;//EFUSE_DATA[56:50]
// 	uint32_t crypto_en : 1;//EFUSE_DATA[57]
// 	uint32_t crypto_jtag_disable : 1;//EFUSE_DATA[58]
// 	uint32_t jtag_disable : 1;//EFUSE_DATA[59]
// 	uint32_t security : 1;//EFUSE_DATA[60]
// 	uint32_t speedup : 1;//EFUSE_DATA[61]
// 	uint32_t reserved1 : 2;//EFUSE_DATA[63：62]
// 	uint32_t crc32_chip_config;//EFUSE_DATA[95:64]
// 	uint32_t pubkey_hash[8];//EFUSE_DATA[351:96]
// 	uint32_t aes_key[4];//EFUSE_DATA[479:352]
// 	uint32_t crc32_aes_key_hash;//EFUSE_DATA[511:480]
// } sefuse_info;
// typedef union _efuse_wd
// {
// 	sefuse_info efuse_info;
// 	uint32_t efuse_data[16];
// } uefuse_wd;
// /******************************************************************************
//  * Name:    CRC-32  x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
//  * Note:    d for闂備胶鍎甸弲娑㈡偤閵娧勬殰閻庨潧鎲℃刊濂告煥濞戞ê顏柡鍡嫹
//  *****************************************************************************/
// uint32_t  crc32_d(uint8_t *data, uint32_t length)
// {
// 	uint8_t i;
// 	uint32_t crc = 0xffffffff;        // Initial value
// 	while(length--)
// 	{
// 		crc ^= *data++;                // crc ^= *data; data++;
// 		for(i = 0; i < 8; ++i)
// 		{
// 			if(crc & 1)
// 				crc = (crc >> 1) ^ 0xEDB88320;// 0xEDB88320= reverse 0x04C11DB7
// 			else
// 				crc = (crc >> 1);
// 		}
// 	}
// 	return ~crc;
// }
// #define EFUSE_BASE_ADDR 0x32C00
// #define EFUSE_WD0_OFFSET 0x0
// #define EFUSE_WD1_OFFSET 0x4
// #define EFUSE_WD2_OFFSET 0x8
// #define EFUSE_WD3_OFFSET 0xC
// #define EFUSE_WD4_OFFSET 0x10
// #define EFUSE_WD5_OFFSET 0x14
// #define EFUSE_WD6_OFFSET 0x18
// #define EFUSE_WD7_OFFSET 0x1C
// #define EFUSE_WD8_OFFSET 0x20
// #define EFUSE_WD9_OFFSET 0x24
// #define EFUSE_WD10_OFFSET 0x28
// #define EFUSE_WD11_OFFSET 0x2C
// #define EFUSE_WD12_OFFSET 0x30
// #define EFUSE_WD13_OFFSET 0x34
// #define EFUSE_WD14_OFFSET 0x38
// #define EFUSE_WD15_OFFSET 0x3C
// #define EFUSE_CFG_OFFSET 0x40
// #define EFUSE_FPGA_CFG_OFFSET 0x44

// #define EFUSE_REG_ADDR(offset) REG_ADDR(EFUSE_BASE_ADDR, offset)
// #define EFUSE_REG(offset) REG32(EFUSE_REG_ADDR(offset))

// #define EFUSE_WD(n) EFUSE_REG((EFUSE_WD0_OFFSET + (((n) & 0x0F) << 2)))
// #define EFUSE_WD0 EFUSE_REG(EFUSE_WD0_OFFSET)//EFUSE_DATA[0:31]
// #define EFUSE_WD1 EFUSE_REG(EFUSE_WD1_OFFSET)//EFUSE_DATA[32:63]
// #define EFUSE_WD2 EFUSE_REG(EFUSE_WD2_OFFSET)//EFUSE_DATA[64:95]
// #define EFUSE_WD3 EFUSE_REG(EFUSE_WD3_OFFSET)//EFUSE_DATA[96:127]
// #define EFUSE_WD4 EFUSE_REG(EFUSE_WD4_OFFSET)//EFUSE_DATA[128:159]
// #define EFUSE_WD5 EFUSE_REG(EFUSE_WD5_OFFSET)//EFUSE_DATA[160:191]
// #define EFUSE_WD6 EFUSE_REG(EFUSE_WD6_OFFSET)//EFUSE_DATA[192:223]
// #define EFUSE_WD7 EFUSE_REG(EFUSE_WD7_OFFSET)//EFUSE_DATA[224:255]
// #define EFUSE_WD8 EFUSE_REG(EFUSE_WD8_OFFSET)//EFUSE_DATA[256:287]
// #define EFUSE_WD9 EFUSE_REG(EFUSE_WD9_OFFSET)//EFUSE_DATA[288:319]
// #define EFUSE_WD10 EFUSE_REG(EFUSE_WD10_OFFSET)//EFUSE_DATA[320:351]
// #define EFUSE_WD11 EFUSE_REG(EFUSE_WD11_OFFSET)//EFUSE_DATA[352:383]
// #define EFUSE_WD12 EFUSE_REG(EFUSE_WD12_OFFSET)//EFUSE_DATA[384:415]
// #define EFUSE_WD13 EFUSE_REG(EFUSE_WD13_OFFSET)//EFUSE_DATA[416:447]
// #define EFUSE_WD14 EFUSE_REG(EFUSE_WD14_OFFSET)//EFUSE_DATA[448:479]
// #define EFUSE_WD15 EFUSE_REG(EFUSE_WD15_OFFSET)//EFUSE_DATA[480:511]
// #define EFUSE_CFG EFUSE_REG(EFUSE_CFG_OFFSET)//EFUSE_CFG
// #define EFUSE_FPGA_CFG EFUSE_REG(EFUSE_FPGA_CFG_OFFSET)//EFUSE_FPGA_CFG

// #define EFUSE_TRIM_LDO1                     ((EFUSE_WD0&0x000000FF)>>0)//EFUSE_DATA[7:0]//CHIP POWER LDO1 TRIM
// #define EFUSE_TRIM_BG                       ((EFUSE_WD0&0x0000FF00)>>8)//EFUSE_DATA[15:8]//CHIP POWER BG TRIM
// #define EFUSE_ROMPATCH_DISABLE              ((EFUSE_WD0&0x00010000)>>16)//EFUSE_DATA[16]//0:rom patch enable 1:rom patch disable
// #define EFUSE_WAKEUP_BYPASS_SECUITY_DISABLE ((EFUSE_WD0&0x00020000)>>17)//EFUSE_DATA[17]//0:enable wakeup bypass security  1:disable wakeup bypass security 
// #define EFUSE_PROGRAM_DISABLE               ((EFUSE_WD0&0x00040000)>>18)//EFUSE_DATA[18]//0:program enable 1:program disable
// #define EFUSE_CRYPTO_NORMAL                 ((EFUSE_WD0&0x00080000)>>19)//EFUSE_DATA[19]
// #define EFUSE_TRIM_32K                      ((EFUSE_WD0&0xFFF00000)>>20)////EFUSE_DATA[31:20]
// #define EFUSE_CRYPTO_CLKGATE_DISABLE        ((EFUSE_WD1&0x00000001)>>(32-32))//EFUSE_DATA[32]
// #define EFUSE_TRIM_96M                      ((EFUSE_WD1&0x000007fe)>>(33-32))//EFUSE_DATA[42:33]
// #define EFUSE_RESERVED0                     ((EFUSE_WD1&0x00003800)>>(43-32))//EFUSE_DATA[45:43]
// #define EFUSE_CRYPTO_ROMPATCH_DISABLE       ((EFUSE_WD1&0x00004000)>>(46-32))//EFUSE_DATA[46]
// #define EFUSE_EFLASH_512K                   ((EFUSE_WD1&0x00008000)>>(47-32))//EFUSE_DATA[47]
// #define EFUSE_EC_DEBUG                      ((EFUSE_WD1&0x00010000)>>(48-32))//EFUSE_DATA[48]
// #define EFUSE_CRYPTO_DEBUG                  ((EFUSE_WD1&0x00020000)>>(49-32))//EFUSE_DATA[49]
// #define EFUSE_HASH_SIZE                     ((EFUSE_WD1&0x01fc0000)>>(50-32))//EFUSE_DATA[56:50]
// #define EFUSE_CRYPTO_EN                     ((EFUSE_WD1&0x02000000)>>(57-32))//EFUSE_DATA[57]
// #define EFUSE_CRYPTO_JTAG_DISABLE           ((EFUSE_WD1&0x04000000)>>(58-32))//EFUSE_DATA[58]
// #define EFUSE_JTAG_DISABLE                  ((EFUSE_WD1&0x08000000)>>(59-32))//EFUSE_DATA[59]
// #define EFUSE_SECURITY                      ((EFUSE_WD1&0x10000000)>>(60-32))//EFUSE_DATA[60]
// #define EFUSE_SPEEDUP                       ((EFUSE_WD1&0x20000000)>>(61-32))//EFUSE_DATA[61]
// #define EFUSE_RESERVED1                     ((EFUSE_WD1&0xC0000000)>>(62-32))//EFUSE_DATA[63:62]
// #define EFUSE_CRC32_CHIP_CONFIG             EFUSE_WD2//EFUSE_DATA[95:64]
// #define EFUSE_PUBKEY_HASH(n)                (EFUSE_WD((3+((n)&0x7))))//EFUSE_DATA[351:96]
// #define EFUSE_AES_KEY(n)                    (EFUSE_WD((11+((n)&0x3))))//EFUSE_DATA[479:352]
// #define EFUSE_CRC32_PUBKEYHASH_AESKEY        EFUSE_WD15//EFUSE_DATA[511:480]

// extern void write_efuse_data(uint32_t *data);
// void write_efuse_data(uint32_t *data)
// {
// #if 1
// 	uint8_t pubkey_hash_SHA256_aes128ecb_AESKEY_byte[32] = {
// 	//       /*test0512*/ 0x90,0x3A,0xD0,0x62,0x0E,0xD4,0x09,0xBF,0xED,0xB1,0xBB,0x0A,0x69,0xC4,0x8D,0x3E,0xA8,0x19,0x38,0xBE,0x50,0x3D,0x4F,0xFF,0xC4,0x73,0x65,0x19,0x32,0xE8,0x77,0x50
// 	//       /*test1024*/ 0xFC,0x48,0x82,0x1A,0xE7,0xAB,0x80,0x72,0x0D,0xEC,0xFF,0xAF,0x95,0xBA,0x00,0xBD,0xE4,0x87,0x84,0x65,0xDA,0x8E,0x46,0xD3,0xB0,0x32,0x6D,0x98,0xCD,0x66,0x0A,0x18
// 		/*test2048*/ 0x8A,0x69,0x77,0xA7,0x5D,0x3A,0x1F,0x1F,0x13,0x9E,0x69,0xAE,0x92,0xC0,0xE6,0x44,0x7D,0x1E,0xF4,0xCA,0x55,0x66,0xAF,0xD9,0x5E,0x8B,0xED,0x4A,0x10,0x1D,0xF1,0x66
//   //       /*test4096*/ 0x09,0x45,0x7A,0x6F,0x7C,0xC5,0x0D,0xB6,0x3D,0x8B,0xC8,0xEA,0xD3,0x2B,0x4F,0x69,0xC3,0xDF,0x65,0x30,0x02,0xCC,0xA7,0x9A,0xF4,0x94,0x6D,0x3B,0xFB,0x6D,0x26,0xC4

// 	}; // 对公钥做sah256后通过aes_key加密得到的结果
// 	uint32_t *pubkey_hash_SHA256_aes128ecb_AESKEY = (uint32_t *)pubkey_hash_SHA256_aes128ecb_AESKEY_byte;
// 	//PUBLIC KEY HASH (SHA256): 0xe7,0xcf,0x92,0xfb,0x4a,0xfe,0xc2,0xf2,0xc7,0x83,0x24,0x51,0x9c,0xa3,0xab,0xe,0xe4,0xf8,0xe5,0xdd,0x73,0x94,0x9,0xa6,0xb8,0xd1,0xc7,0xba,0xb,0x1,0x3a,0x70,
// #else
// 	uint32_t pubkey_hash_SHA256_aes128ecb_AESKEY[8] = { 0 }; // 对公钥做sah256后通过aes_key加密得到的结果
// #endif
// 	uint32_t aeskey_aes128ecb_RTLKEY[4] = {
// #if  0//V13/*AES_KEY_OLD_RTL_KEY_ENCRYPTO*/
// 		   0,0,0,0,/*AES_KEY_OLD_RTL_KEY_ENCRYPTO OLD_RTL_KEY=0x3c4fcf098815f7aba6d2ae2816157e2b={0x16157e2b,0xa6d2ae28,0x8815f7ab,0x3c4fcf09}*/
// #else//V14/*AES_KEY_NEW_RTL_KEY_ENCRYPTO*/
// 	   0x3522e3c5,0x59f2e402,0x93b180a0,0x8474cbe5,/*NEW RTL_KEY no know as software*/
// #endif
// 	};// AES_KEY=0xd50abd5db7d82c249239ca4c5137b6ad={ 0x5137b6ad,0x9239ca4c,0xb7d82c24,0xd50abd5d }; 时，通过RTLKEY加密为0；

// #if 0
//    //方案1通过数值写入直接赋值
// 	if(data == NULL)
// 	{
// 		EFUSE_WD0 = 0x0;
// 		EFUSE_WD1 = 0x0;
// 	}
// 	else
// 	{//采用赋值的方式
// 		EFUSE_WD0 = data[0];
// 		EFUSE_WD1 = data[1];
// 	}
// 	EFUSE_CRC32_CHIP_CONFIG = 0x0;//CRC32_Cal_Buffer((DWORDP)&efuse_data.efuse_data[0], 8, 0, 0, 0);
// 	for(uint32_t i = 0; i < 8; i++)
// 	{
// 		EFUSE_PUBKEY_HASH(i) = pubkey_hash_SHA256_aes128ecb_AESKEY[i];
// 	}
// 	for(uint32_t i = 0; i < 4; i++)
// 	{
// 		EFUSE_AES_KEY(i) = aeskey_aes128ecb_RTLKEY[i];
// 	}
// 	EFUSE_CRC32_PUBKEYHASH_AESKEY = 0x0;//CRC32_Cal_Buffer((DWORDP)&efuse_data.efuse_data[3], 48, 0, 0, 0);

// #elif 0
// 	   // 方案2通过结构体直接赋值
// 	sefuse_info *efuse_data = (void *)EFUSE_BASE_ADDR; //(void *)&EFUSE_WD0;
// 	if(data == NULL)
// 	{
// 		// 写efuse数据
// 		efuse_data->trim_ldo1 = 0x0;
// 		efuse_data->trim_bg = 0x0;
// 		efuse_data->rom_patch_disable = 0x0;
// 		efuse_data->wakeup_bypass_security_disable = 0x0;
// 		efuse_data->program_disable = 0x0;
// 		efuse_data->crypto_normal = 0x0;
// 		efuse_data->trim_32k = 0x0;
// 		efuse_data->crypto_clkgate_disable = 0x0;
// 		efuse_data->trim_96m = 0x0;
// 		efuse_data->reserved0 = 0x0;
// 		efuse_data->crypto_rompatch_disable = 0x0;
// 		efuse_data->eflash_512k = 0x0;
// 		efuse_data->ec_debug = 0x0;
// 		efuse_data->crypto_debug = 0x0;
// 		efuse_data->hash_size = 0x0;
// 		efuse_data->crypto_en = 0x0;
// 		efuse_data->crypto_jtag_disable = 0x0;
// 		efuse_data->jtag_disable = 0x0;
// 		efuse_data->security = 0x0;
// 		efuse_data->speedup = 0x0;
// 		efuse_data->reserved1 = 0x0;
// 	}
// 	else
// 	{
// 		sefuse_info *data_ptr = (void *)data;
// 		// 写efuse数据
// 		efuse_data->trim_ldo1 = data_ptr->trim_ldo1;
// 		efuse_data->trim_bg = data_ptr->trim_bg;
// 		efuse_data->rom_patch_disable = data_ptr->rom_patch_disable;
// 		efuse_data->wakeup_bypass_security_disable = data_ptr->wakeup_bypass_security_disable;
// 		efuse_data->program_disable = data_ptr->program_disable;
// 		efuse_data->crypto_normal = data_ptr->crypto_normal;
// 		efuse_data->trim_32k = data_ptr->trim_32k;
// 		efuse_data->crypto_clkgate_disable = data_ptr->crypto_clkgate_disable;
// 		efuse_data->trim_96m = data_ptr->trim_96m;
// 		efuse_data->reserved0 = data_ptr->reserved0;
// 		efuse_data->crypto_rompatch_disable = data_ptr->crypto_rompatch_disable;
// 		efuse_data->eflash_512k = data_ptr->eflash_512k;
// 		efuse_data->ec_debug = data_ptr->ec_debug;
// 		efuse_data->crypto_debug = data_ptr->crypto_debug;
// 		efuse_data->hash_size = data_ptr->hash_size;
// 		efuse_data->crypto_en = data_ptr->crypto_en;
// 		efuse_data->crypto_jtag_disable = data_ptr->crypto_jtag_disable;
// 		efuse_data->jtag_disable = data_ptr->jtag_disable;
// 		efuse_data->security = data_ptr->security;
// 		efuse_data->speedup = data_ptr->speedup;
// 		efuse_data->reserved1 = data_ptr->reserved1;
// 	}

// 	efuse_data->crc32_chip_config = 0x0; // CRC32_Cal_Buffer((DWORDP)&efuse_data.efuse_data[0], 8, 0, 0, 0);
// 	for(uint32_t i = 0; i < 8; i++)
// 	{
// 		efuse_data->pubkey_hash[i] = pubkey_hash_SHA256_aes128ecb_AESKEY[i];
// 	}
// 	for(uint32_t i = 0; i < 4; i++)
// 	{
// 		efuse_data->aes_key[i] = aeskey_aes128ecb_RTLKEY[i];
// 	}
// 	efuse_data->crc32_aes_key_hash = 0x0; // CRC32_Cal_Buffer((DWORDP)&efuse_data.efuse_data[3], 48, 0, 0, 0);
// #else
// 	   // 方案3通过共用体间接赋值赋值
// 	uefuse_wd efuse_data;
// 	if(data == NULL)
// 	{
// 		efuse_data.efuse_info.trim_ldo1 = 0x0;
// 		efuse_data.efuse_info.trim_bg = 0x0;
// 		efuse_data.efuse_info.rom_patch_disable = 0x0;
// 		efuse_data.efuse_info.wakeup_bypass_security_disable = 0x0;
// 		efuse_data.efuse_info.program_disable = 0x0;
// 		efuse_data.efuse_info.crypto_normal = 0x0;
// 		efuse_data.efuse_info.trim_32k = 0x0;
// 		efuse_data.efuse_info.crypto_clkgate_disable = 0x0;
// 		efuse_data.efuse_info.trim_96m = 0x0;
// 		efuse_data.efuse_info.reserved0 = 0x0;
// 		efuse_data.efuse_info.crypto_rompatch_disable = 0x0;
// 		efuse_data.efuse_info.eflash_512k = 0x0;
// 		efuse_data.efuse_info.ec_debug = 0x1;
// 		efuse_data.efuse_info.crypto_debug = 0x1;
// 		efuse_data.efuse_info.hash_size = 0;
// 		efuse_data.efuse_info.crypto_en = 0x1;
// 		efuse_data.efuse_info.crypto_jtag_disable = 0x0;
// 		efuse_data.efuse_info.jtag_disable = 0x0;
// 		efuse_data.efuse_info.security = 0x1;
// 		efuse_data.efuse_info.speedup = 0x0;
// 		efuse_data.efuse_info.reserved1 = 0x0;
// 	}
// 	else
// 	{//采用赋值的方式
// 		efuse_data.efuse_data[0] = data[0];
// 		efuse_data.efuse_data[1] = data[1];
// 	}

// 	efuse_data.efuse_info.crc32_chip_config = crc32_d((uint8_t *)&efuse_data.efuse_data[0], 8); // CRC32_Cal_Buffer((DWORDP)&efuse_data.efuse_data[0], 8, 0, 0, 0); // 0x0;
// 	for(uint32_t i = 0; i < 8; i++)
// 	{
// 		efuse_data.efuse_info.pubkey_hash[i] = pubkey_hash_SHA256_aes128ecb_AESKEY[i];
// 	}
// 	for(uint32_t i = 0; i < 4; i++)
// 	{
// 		efuse_data.efuse_info.aes_key[i] = aeskey_aes128ecb_RTLKEY[i];
// 	}
// 	efuse_data.efuse_info.crc32_aes_key_hash = crc32_d((uint8_t *)&efuse_data.efuse_data[3], 48); // CRC32_Cal_Buffer((DWORDP)&efuse_data.efuse_data[3], 48, 0, 0, 0); // 0x0;
// 	for(uint32_t i = 0; i < 16; i++)
// 	{
// 		EFUSE_WD(i) = efuse_data.efuse_data[i];
// 	}
// #endif
// 		   // 更新配置
// 	   // 配置新的EFUSE
// 	EFUSE_FPGA_CFG |= 1;
// #if 0//主系统无法重读，因此需要复位解决
// 	// 重新读取EFUSE数据
// 	EFUSE_CFG |= 1;
// 	// 等待EFUSE读取完毕
// 	while(!(EFUSE_CFG & 1))
// 	{
// 	}
// #endif
// 	// 输出EFUSE数据
// 	printf("EFUSE_TRIM_LDO1: %#x\n", efuse_data.efuse_info.trim_ldo1);
// 	printf("EFUSE_TRIM_BG: %#x\n", efuse_data.efuse_info.trim_bg);
// 	printf("EFUSE_ROMPATCH_DISABLE: %#x\n", efuse_data.efuse_info.rom_patch_disable);
// 	printf("EFUSE_WAKEUP_BYPASS_SECUITY_DISABLE: %#x\n", efuse_data.efuse_info.wakeup_bypass_security_disable);
// 	printf("EFUSE_PROGRAM_DISABLE: %#x\n", efuse_data.efuse_info.program_disable);
// 	printf("EFUSE_CRYPTO_NORMAL: %#x\n", efuse_data.efuse_info.crypto_normal);
// 	printf("EFUSE_TRIM_32K: %#x\n", efuse_data.efuse_info.trim_32k);
// 	printf("EFUSE_CRYPTO_CLKGATE_DISABLE: %#x\n", efuse_data.efuse_info.crypto_clkgate_disable);
// 	printf("EFUSE_TRIM_96M: %#x\n", efuse_data.efuse_info.trim_96m);
// 	printf("EFUSE_RESERVED0: %#x\n", efuse_data.efuse_info.reserved0);
// 	printf("EFUSE_CRYPTO_ROMPATCH_DISABLE: %#x\n", efuse_data.efuse_info.crypto_rompatch_disable);
// 	printf("EFUSE_EFLASH_512K: %#x\n", efuse_data.efuse_info.eflash_512k);
// 	printf("EFUSE_EC_DEBUG: %#x\n", efuse_data.efuse_info.ec_debug);
// 	printf("EFUSE_CRYPTO_DEBUG: %#x\n", efuse_data.efuse_info.crypto_debug);
// 	printf("EFUSE_HASH_SIZE: %#x\n", efuse_data.efuse_info.hash_size);
// 	printf("EFUSE_CRYPTO_EN: %#x\n", efuse_data.efuse_info.crypto_en);
// 	printf("EFUSE_CRYPTO_JTAG_DISABLE: %#x\n", efuse_data.efuse_info.crypto_jtag_disable);
// 	printf("EFUSE_JTAG_DISABLE: %#x\n", efuse_data.efuse_info.jtag_disable);
// 	printf("EFUSE_SECURITY: %#x\n", efuse_data.efuse_info.security);
// 	printf("EFUSE_SPEEDUP: %#x\n", efuse_data.efuse_info.speedup);
// 	printf("EFUSE_RESERVED1: %#x\n", efuse_data.efuse_info.reserved1);
// 	printf("EFUSE_CRC32_CHIP_CONFIG: %#x\n", efuse_data.efuse_info.crc32_chip_config);
// 	printf("EFUSE_PUBKEY_HASH: %#x\n", efuse_data.efuse_info.pubkey_hash[0]);
// 	printf("EFUSE_PUBKEY_HASH: %#x\n", efuse_data.efuse_info.pubkey_hash[1]);
// 	printf("EFUSE_PUBKEY_HASH: %#x\n", efuse_data.efuse_info.pubkey_hash[2]);
// 	printf("EFUSE_PUBKEY_HASH: %#x\n", efuse_data.efuse_info.pubkey_hash[3]);
// 	printf("EFUSE_PUBKEY_HASH: %#x\n", efuse_data.efuse_info.pubkey_hash[4]);
// 	printf("EFUSE_PUBKEY_HASH: %#x\n", efuse_data.efuse_info.pubkey_hash[5]);
// 	printf("EFUSE_PUBKEY_HASH: %#x\n", efuse_data.efuse_info.pubkey_hash[6]);
// 	printf("EFUSE_PUBKEY_HASH: %#x\n", efuse_data.efuse_info.pubkey_hash[7]);
// 	printf("EFUSE_AES_KEY: %#x\n", efuse_data.efuse_info.aes_key[0]);
// 	printf("EFUSE_AES_KEY: %#x\n", efuse_data.efuse_info.aes_key[1]);
// 	printf("EFUSE_AES_KEY: %#x\n", efuse_data.efuse_info.aes_key[2]);
// 	printf("EFUSE_AES_KEY: %#x\n", efuse_data.efuse_info.aes_key[3]);
// 	printf("EFUSE_CRC32_PUBKEYHASH_AESKEY: %#x\n", efuse_data.efuse_info.crc32_aes_key_hash);
// }
// void OPTIMIZE0 rom_patch_config_func(void)
// {
// 	printf("rom patch config here\n");
// 	/*设置ROM PATCH*/
// 	// 112a0
// 	PATCH_CTRL |= 0x1;	   // 使能patch0
// 	PATCH_ADDRL(0) = 0xA8; // 设置patch0_addr0_0
// 	PATCH_ADDRH(0) = 0x4;  // 设置patch0_addr0_1
// 	// 10084
// 	asm volatile(
// 		"mv a0, %0\n"
// 		:
// 	: "r"(0x00080420)
// 		: "a0");
// 	// jr a0 8502
// 	PATCH_DATA0(0) = 0x02; // 设置patch0_data0_0
// 	PATCH_DATA1(0) = 0x85; // 设置patch0_data0_1
// 	PATCH_DATA2(0) = 0x00; // 设置patch0_data0_2
// 	PATCH_DATA3(0) = 0x00; // 设置patch0_data0_3
// }
// /***********************ROM PATCH FUNCTION*************************/
// void OPTIMIZE0 rom_patch_test_func_flash(void)
// {
// 	printf("This is rom patch test,run in flash\n");
// 	PATCH_CTRL &= 0xfe; // patch0
// 	// asm volatile (
// 	//     "mv t0, %0\n"
// 	// 	"mv x1, %1\n"
// 	// 	"jr t0\n"
// 	//     :
// 	//     : "r" (0x111e8), "r" (0x112a2)
// 	//     : "t0"
// 	// );
// 	asm volatile(
// 		"lw	ra,8(sp)\n"
// 		"lw	s0,4(sp)\n"
// 		"addi sp,sp,12\n");
// 	asm volatile(
// 		"mv a0, %0\n"
// 		"jr a0\n"
// 		:
// 	: "r"(0x112a2)
// 		: "a0");
// }
// typedef DWORD(*FUNCT_PTR_D_B)(BYTE);
// typedef void (*FUNCT_PTR_V_D)(DWORD);
// typedef void (*FUNCT_PTR_V_BP)(BYTEP);
// typedef void (*FUNCT_PTR_V_D_D_BP)(DWORD, DWORD, BYTEP);
// typedef VBYTEP(*FUNCT_PTR_VBP_D_D_BP)(DWORD, DWORD, BYTEP);
// typedef BYTE(*FUNCT_PTR_B_s_DP_s)(ssign *, DWORDP, spoint *);
// typedef void (*FUNCT_PTR_V_D_B_BP_BP_BP_BP)(DWORD, BYTE, BYTEP, BYTEP, BYTEP, BYTEP);
// typedef void (*FUNCT_PTR_V_D_DP_DP)(DWORD, DWORDP, DWORDP);
// BYTE rom_patch_demo(BYTE offset, BYTE param1)
// {
// 	// char *rompatch_test_ch = "This is rompatch test\n\0";
// 	// char *receive_buffer = "ffffffffffffffffffff\n\0";
// 	// switch(offset)
// 	// {
// 	// 	case 0x0:  // 0x10084=
// 	// 	case 0x4:  // rom_main=
// 	// 	case 0x8:  // rom_wdt_feed=
// 	// 	case 0x18: // rom_uart=
// 	// 	case 0x1C: // rom_ejtag=
// 	// 	case 0x30: // ROM_SPIFE_ERASE=
// 	// 	case 0x44: // rom_exit=
// 	// 		((FUNCT_PTR_V_V)(*(DWORDP)(0x10000 + offset)))();
// 	// 		break;
// 	// 	case 0xC:  // rom_putc=
// 	// 	case 0x24: // select_flash=
// 	// 	case 0x3C: // ROM_Update
// 	// 	case 0x40: // ROM_SM_Update
// 	// 		((FUNCT_PTR_V_B) * (DWORDP)(0x10000 + offset))(param1);
// 	// 		break;
// 	// 	case 0x10: // rom_puts=
// 	// 		((FUNCT_PTR_V_BP) * (DWORDP)(0x10000 + offset))((BYTEP)rompatch_test_ch);
// 	// 		break;
// 	// 	case 0x14: // rom_get_cpu_freq=
// 	// 		((FUNCT_PTR_D_B) * (DWORDP)(0x10000 + offset))(param1);
// 	// 		break;
// 	// 	case 0x20: // rom_wdt=
// 	// 		((FUNCT_PTR_V_D) * (DWORDP)(0x10000 + offset))(0xffff);
// 	// 		break;
// 	// 	case 0x28: // rom_memset
// 	// 	case 0x2C: // ROM_strcmp
// 	// 	case 0x34: // SPIFE_Write_Interface=
// 	// 		((FUNCT_PTR_V_D_D_BP) * (DWORDP)(0x10000 + offset))(0x10, 0x22000, (BYTEP)rompatch_test_ch);
// 	// 		break;
// 	// 	case 0x38: // SPIFE_Read_Interface=
// 	// 		((FUNCT_PTR_VBP_D_D_BP) * (DWORDP)(0x10000 + offset))(0x10, 0x22000, (BYTEP)receive_buffer);
// 	// 		((FUNCT_PTR_V_BP) * (DWORDP)(0x10010))((BYTEP)receive_buffer);
// 	// 		break;
// 	// 	// case 0x48:	//rom_SM2Pulib_Key=
// 	// 	case 0x4C: // rom_SM2_VERIFY=
// 	// 		return ((FUNCT_PTR_B_s_DP_s) * (DWORDP)(0x10000 + offset))(&rom_safeONram.SM2_ssign1, rom_SM2_E, &((spoint *)(*(DWORDP)0x10048))[ROM_key_id]);
// 	// 		break;
// 	// 	case 0x50: // rom_SM4_Run
// 	// 		// FUNCT_PTR_V_D_B_BP_BP_BP_BP	(0x10000 + offset)();
// 	// 		break;
// 	// 	case 0x54: // rom_SM3_Run=
// 	// 		((FUNCT_PTR_V_D_DP_DP) * (DWORDP)(0x10000 + offset))((DWORD)rom_SM3_size, (DWORDP)0x0, rom_SM2_E);
// 	// 		break;
// 	// 	case 0x58: // ROM_interface
// 	// 		// FUNCT_PTR_V_D	(0x10000 + offset)(0x0);	//0:internal_flash; 1:external_flash
// 	// 		break;
// 	// 	default:
// 	// 		dprint("ROMPATCH_DEMO offset ERROR\n");
// 	// 		break;
// 	// }
// 	return 0;
// }
// void ALIGNED(4) OPTIMIZE0 clear_internel_flash(void)
// {
// 	u32 spif_ctrl0 = SPIFE_CTL0;
// 	REG32(0x3047C) &= 0xfffffffd;
// 	SYSCTL_RST1 |= 0x00000100; // 复位
// 	SYSCTL_RST1 &= 0xfffffeff; // 清除复位
// 	while(!(SPIFE_RDY & SPIF_RDY_READY))
// 		WDT_CRR = WDT_CRR_CRR; // 读忙
// 	while(SPIFE_STA & 0xf)
// 		WDT_CRR = WDT_CRR_CRR; // 直到写完
// 	while(!(SPIFE_RDY & SPIF_RDY_READY))
// 		WDT_CRR = WDT_CRR_CRR; // 读忙
// 	SPIFE_FTOP = 0xc7;
// 	while(!(SPIFE_RDY & SPIF_RDY_READY))
// 		WDT_CRR = WDT_CRR_CRR; // 读忙
// 	while(SPIFE_STA & 0xf)
// 		WDT_CRR = WDT_CRR_CRR; // 直到写完
// 	while(!(SPIFE_RDY & SPIF_RDY_READY))
// 		WDT_CRR = WDT_CRR_CRR; // 读忙
// 	REG32(0x3047C) |= 0x00000002;
// 	SYSCTL_RST1 |= 0x00000100; // 复位
// 	__nop;
// 	__nop;
// 	__nop;
// 	SYSCTL_RST1 &= 0xfffffeff; // 清除复位
// 	SPIFE_CTL0 = spif_ctrl0;
// 	__nop;
// 	__nop;
// 	__nop;
// 	while(SPIFE_STA & 0xf)
// 		WDT_CRR = WDT_CRR_CRR; // 直到写完
// 	while(!(SPIFE_RDY & SPIF_RDY_READY))
// 		WDT_CRR = WDT_CRR_CRR; // 读忙
// 	__nop;
// 	__nop;
// 	__nop;
// }
// void earse_internel_flash(void)
// {
// 	if(SYSCTL_PIO_CFG & BIT1)
// 	{
// 		FUNCT_PTR_V_V funct_ptr = Load_funVV_To_Dram(clear_internel_flash, 0x400);
// 		(*funct_ptr)();
// 	}
// }
// // PATCH demo
// void SECTION(".PATCH0_TRANSIT") USED PATCH0_TRANSIT(void)
// {
// 	goto * 0x80084UL;
// 	return;
// }
// void SECTION(".PATCH1_TRANSIT") USED PATCH1_TRANSIT(void)
// {
// 	main();
// 	return;
// }
// void SECTION(".PATCH2_TRANSIT") USED PATCH2_TRANSIT(void)
// {
// 	return;
// }
// void SECTION(".PATCH3_TRANSIT") USED PATCH3_TRANSIT(void)
// {
// 	return;
// }
// void SECTION(".PATCH4_TRANSIT") USED PATCH4_TRANSIT(void)
// {
// 	return;
// }
// void SECTION(".PATCH5_TRANSIT") USED PATCH5_TRANSIT(void)
// {
// 	return;
// }
// void SECTION(".PATCH6_TRANSIT") USED PATCH6_TRANSIT(void)
// {
// 	return;
// }
// void SECTION(".PATCH7_TRANSIT") USED PATCH7_TRANSIT(void)
// {
// 	return;
// }
// //3Wire模式下测试读写函数，再测试读写之前，请提前擦除FLASH
// void SPI_3Wire_Test(void)
// {
//   SPIM_CTRL = 0x6704;     // 高位优先传输，8位传输
//   SPIM_CPSR = 0x1;       //4分频
//   SPIM_IMSR = 0xff;      //屏蔽所有中断

//   uint32_t no = 64;
//   uint32_t csn = 0;
//   uint32_t ADDRess = 0x5600;
//   uint16_t s_data[256];
//   uint16_t r_data[256];
//   uint16_t rdata, rdata2;
//   for(int i = 0; i < no; i++)
//   {
//     s_data[i] = 0xaa + i;
//   }
//   //写
//   SPI_3Wire_Page_Program(ADDRess, s_data, no, csn);

//   //读
//   SPI_3Wire_Read(ADDRess, r_data, no, csn);

//   //check
//   for(int i = 0; i < no; i++)
//   {
//     rdata = s_data[i];
//     rdata2 = r_data[i];
//     if(rdata != rdata2)
//       printf("3wire ERROR!!\n");
//     else
//       printf("3wire PASS\n");
//     printf("send is 0x%x\n", s_data[i]);
//     printf("read is 0x%x\n", r_data[i]);
//   }
// }

// void SPI_Quad_Test(void)
// {
//   //SPIM初始化
//   SPIM_CTRL = 0x6704;     //12:高位优先传； 8-11：0x7,表示8位传输； 0-1：0x0，表示第一个上升沿采集
//   SPIM_CPSR = 0x1;        //时钟分频，4分频
//   SPIM_IMSR = 0xff;       //中断全部屏蔽
//   SPIM_MODE = 0x0;        //标准spi模式
//   uint32_t no = 16;
//   uint32_t csn = 0;
//   uint32_t ADDRess = 0x0;

//   uint16_t s_data[256];
//   uint16_t d_data[256];
//   uint16_t rdata, rdata2;
//   int i;
//   for(i = 0; i < no; i++)
//     s_data[i] = (((i + 0x01) << 8) | (i + 0x10));
//   printf("SPIM_MODE:%x\n", SPIM_MODE);
//   SPI_Quad_enable(csn);//设置quad模式,quad enable
//   SPI_Block_Erase(ADDRess, csn);
//   SPI_Quad_Program(ADDRess, s_data, no, csn);
//   SPI_Quad_Read(ADDRess, d_data, no, csn);
//   printf("SPIM_MODE:%x\n", SPIM_MODE);
//   //check
//   for(i = 0; i < no; i++)
//   {
//     rdata = s_data[i];
//     rdata2 = d_data[i];
//     if(rdata != rdata2)
//       printf("quad ERROR!!\n");
//     else
//       printf("quad PASS\n");
//     printf("send is 0x%x\n", s_data[i]);
//     printf("read is 0x%x\n", d_data[i]);
//   }

// }

// void SPI_Dual_Test(void)
// {
//   //SPIM初始化
//   SPIM_CTRL = 0x6704;     //12:高位优先传； 8-11：0x7,表示8位传输； 0-1：0x0，表示第一个上升沿采集
//   SPIM_CPSR = 0x1;        //时钟分频，4分频
//   SPIM_IMSR = 0xff;       //中断全部屏蔽
//   SPIM_MODE = 0x0;        //标准spi模式
//   uint32_t no = 16;
//   uint32_t csn = 0;
//   uint32_t ADDRess = 0x0;

//   uint16_t s_data[256];
//   uint16_t d_data[256];
//   uint16_t rdata, rdata2;
//   int i;
//   for(i = 0; i < no; i++)
//     s_data[i] = (((i + 0x01) << 8) | (i + 0x80));
//   printf("SPIM_MODE:%x\n", SPIM_MODE);

//   //擦除还是标准四线模式
//   SPI_Block_Erase(ADDRess, csn);
//   SPI_Quad_Program(ADDRess, s_data, no, csn);
//   SPI_Dual_Read(ADDRess, d_data, no, csn);

//   printf("SPIM_MODE:%x\n", SPIM_MODE);
//   //check
//   for(i = 0; i < no; i++)
//   {
//     rdata = s_data[i];
//     rdata2 = d_data[i];
//     if(rdata != rdata2)
//       printf("Dual ERROR!!\n");
//     else
//       printf("Dual PASS\n");
//     printf("send is 0x%x\n", s_data[i]);
//     printf("read is 0x%x\n", d_data[i]);
//   }
// }
// /*测试代码仅供参考*/
// // 模块中断唤醒测试
// void SWUC_UART_Test(void)
// {
// 	// LogicaLDevice Enable
// 	SYSCTL_HDEVEN |= (HOST_UARTA_EN | HOST_WUC_EN);
// 	Config_PNP_Access_Request();
// 	// PNP Default Config
// 	Config_PNP_Write(0x23, 0x01, 0x01);
// 	vDelayXms(5);
// 	// UART0_BAUD
// 	Config_PNP_Write(0x07, 0x1b, 0x1b);
// 	vDelayXms(5);
// 	Config_PNP_Write(0x60, 0x1b, 0x03);
// 	vDelayXms(5);
// 	Config_PNP_Write(0x61, 0x1b, 0xf8);
// 	vDelayXms(5);
// 	Config_PNP_Write(0x70, 0x1b, 0x14);
// 	vDelayXms(5);
// 	Config_PNP_Write(0x71, 0x1b, 0x01);
// 	vDelayXms(5);
// 	Config_PNP_Write(0x30, 0x1b, 0x01);
// 	vDelayXms(5);
// 	dprint("UART0_BAUD PNP Config finish!\n");
// 	// SWUC
// 	Config_PNP_Write(0x07, 0x04, 0x04);
// 	vDelayXms(5);
// 	Config_PNP_Write(0x60, 0x04, 0x00);
// 	vDelayXms(5);
// 	Config_PNP_Write(0x61, 0x04, 0x00);
// 	vDelayXms(5);
// 	Config_PNP_Write(0x70, 0x04, 0x0f);
// 	vDelayXms(5);
// 	Config_PNP_Write(0x71, 0x04, 0x01);
// 	vDelayXms(5);
// 	Config_PNP_Write(0x30, 0x04, 0x01);
// 	vDelayXms(5);
// 	dprint("SWUC PNP Config finish!\n");
// 	// Enable SWCHIER all interrupt
// 	SWUC_EventIRQ_Config(RI1_EC_IRQ | RI2_EC_IRQ | RING_EC_IRQ | SIRQ_EC_IRQ | MIRQ_EC_IRQ, ENABLE);
// 	// Logic Device Module Config
// 	PRINTF_IER |= 0x1; // Enable uart0 interrup
// }
// // RI1和RI2信号唤醒测试
// void SWUC_RI1_RI2_Test(void)
// {
// 	SWUC_PNP_Config();
// 	SWUC_EventIRQ_Config(RI1_EC_IRQ | RI2_EC_IRQ | RING_EC_IRQ | SIRQ_EC_IRQ | MIRQ_EC_IRQ, ENABLE);
// 	SWUC_RI_Config();
// }
// // RING信号唤醒测试
// void SWUC_Ring_Test(void)
// {
// 	SWUC_PNP_Config();
// 	SWUC_EventIRQ_Config(RI1_EC_IRQ | RI2_EC_IRQ | RING_EC_IRQ | SIRQ_EC_IRQ | MIRQ_EC_IRQ, ENABLE);
// 	SWUC_RING_Config();
// }
// // 软件唤醒
// void SWUC_Soft_Test(void)
// {
// 	SWUC_PNP_Config();
// 	SWUC_EventIRQ_Config(RI1_EC_IRQ | RI2_EC_IRQ | RING_EC_IRQ | SIRQ_EC_IRQ | MIRQ_EC_IRQ, ENABLE);
// 	SWUC_SW_WakeUp_Config(0);
// }
// // 传统模式唤醒
// void SWUC_LegacyMode_Test(void)
// {
// 	SWUC_PNP_Config();
// 	SWUC_ACPIIRQ_Config(PWRSLY | PWRBT, ENABLE);
// }