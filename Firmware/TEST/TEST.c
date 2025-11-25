/*
 * @Author: Maple
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2025-11-09 22:36:09
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
 //test file
#include "TEST.H"
#include "KERNEL_INCLUDE.H"
#include "CUSTOM_INCLUDE.H"
#define WDT_mode 0 //1中断模式 0非中断
#define PWRSW_mode 0

#define Test_Clock_Disable 0
BYTE Module_test(void)
{
	static DWORD Mtimer_Cunt = 0;
	static BYTE MCtrStep = 2;   //6:uarta     //上电之后先执行case2
	static BYTE Prt_flag = 0;
	static BYTE case2_flag = 0;      //用于第一次执行case2模块时，跳转到指定模块
	DWORD timercunt = 0;
	BYTE temp_data;
	DWORD Uart_base = 0;
	int i;

	if (PRINTF_UART_SWITCH >= 4)
	{
		Uart_base = 0x5000 + ((PRINTF_UART_SWITCH - 4) * 0x400);
	}
	else if (PRINTF_UART_SWITCH < 4)
	{
		Uart_base = 0x5800 + (PRINTF_UART_SWITCH * 0x400);
	}
	switch (MCtrStep)
	{
		case 1:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN0 &= (~GPIO_EN);
				}
				dprint("completing the jumper of GPIO, then send any key to start test\n");
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("send key 0x%x\n", temp_data);
				dprint("START\n");
				Prt_flag = 1;
			}
			if (GPIOAutoTest() == 1)
			{
				Mtimer_Cunt++;
				dprint("succeed %d times\n", Mtimer_Cunt);
				if (Mtimer_Cunt >= 10)
				{
					dprint("send the number of test module, otherwise enter next module\n");
					do
					{
						REG8(Uart_base);     //清除掉打印串口接收FIFO
						REG8(Uart_base);
						REG8(Uart_base);
						timercunt++;
						if (REG8(Uart_base + 0x5) & 0x1)
						{
							temp_data = REG8(Uart_base);
							MCtrStep = temp_data;
							Mtimer_Cunt = 0;
							Prt_flag = 0;
							dprint("start %d module test\n", MCtrStep);
							return 1;
						}
					}
					while (timercunt < 0xffffff);

					MCtrStep += 1;
					Mtimer_Cunt = 0;
					Prt_flag = 0;
					dprint("test over\n");
				}
			}
			break;

		case 2:
			if (case2_flag == 0)
			{
				printf("test start from case2\n");
				/*
				if(Test_Clock_Disable)
				{
					SYSCTL_MODEN0 &=(~UART0_EN);
				}
				*/
				//可以选择跳转到指定模块测试操作
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);    //屏蔽串口收到数据中断使能位
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
					case2_flag = 1;
				}
				while (timercunt < 0xffffff);   //软延时
			}
			if (Prt_flag == 0)                                        //等于0，是为了if以内的代码在本case循环下只执行一次
			{
				sysctl_iomux_uart0();
				serial_init(0, 115200);
				Uart_Int_Disable(0, 0);
				dprint("perpare UART0\n");
				dprint("completing the jumper, then send any key to start test\n");  //连接UART0与上位机的接线
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);    //屏蔽串口收到数据中断使能位
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));                 //收FIFO中有数没有被读出跳出while，实现任意输入字符就开始检测
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				dprint("start UART0 send\n");
				Prt_flag = 1;
			}
			Mtimer_Cunt++;
			dprint("start %d times UART0 test\n", Mtimer_Cunt);
			UART0_TX = 0x41;
			UART0_TX = 0x41; UART0_TX = 0x41;          //发送ASCII码  AAA
			UART0_TX = 0xD;  UART0_TX = 0xA;                            //0xD，0xA在ASCII码中分别表示字符 "\r"，即回车符，文本结束和字符 "\n"，即换行符
			dprint("UART0 send AAA\n");                                 //串口0发送十六进制的AA
			dprint("start receive ,send hexadecimal 0xAA by UART0\n");
			while (UART0_RX != 0xaa);
			dprint("UART0 received 0xAA\n");
			UART0_RX;                                                  //从接收FIFO中提取字符0xA，相当于释放FIFO
			UART0_RX;                                                  //从接收FIFO中提取字符0xA，相当于释放FIFO
			UART0_RX;                                                  //从接收FIFO中提取ASCII码的0xD，相当于释放FIFO
			UART0_RX;                                                  //从接收FIFO中提取ASCII码的0xA，相当于释放FIFO
			if (Mtimer_Cunt >= 5)
			{
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);           //软延时等待，是否有输入数据进行跳转到指定的模块进行测试

				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("UART0 over\n");
			}
			break;

		case 3:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN0 &= (~UART1_EN);
				}
				sysctl_iomux_uart1(UART1_TX_SEL, UART1_RX_SEL);
				serial_init(1, 115200);
				Uart_Int_Disable(1, 0);
				dprint("perpare UART1\n");
				dprint("completing the jumper, then send any key to start test\n");   //上位机同时连接UART0（用来显示打印信息），UART1（验证能否与上位机通信）
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				dprint("start UART1 send\n");
				Prt_flag = 1;
			}
			Mtimer_Cunt++;
			dprint("start %d times UART1 test\n", Mtimer_Cunt);
			UART1_TX = 0x41; UART1_TX = 0x41; UART1_TX = 0x41;
			UART1_TX = 0xD;  UART1_TX = 0xA;
			dprint("UART1 send AAA\n");
			dprint("start receive ,send hexadecimal 0xBB by UART1\n");
			while (UART1_RX != 0xbb);
			dprint("UART1 received 0xBB\n");
			if (Mtimer_Cunt >= 5)
			{
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);

				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("UART1 over\n");
			}
			break;

			// case 4:
			// 	if(Prt_flag == 0)
			// 	{
			// 		sysctl_iomux_uart2();
			// 		printf("SYSCTL_PIO3_CFG%x:\n", SYSCTL_PIO3_CFG);      //查看GPIOB23的复用
			// 	#ifdef AE102
			// 		printf("SYSCTL_PIOE_UPCFG%x:\n", SYSCTL_PIOE_UPCFG);  //查看GPIOB23是否上拉
			// 	#endif
			// 	#ifdef TEST103
			// 		printf("SYSCTL_PIOB_UPCFG%x:\n", SYSCTL_PIOB_UPCFG);  //查看GPIOB23是否上拉
			// 	#endif
			// 		serial_init(2, 115200);
			// 		Uart_Int_Disable(2, 0);
			// 		dprint("perpare UART2\n");
			// 		dprint("completing the jumper, then send any key to start test\n");//上位机同时连接UART0（用来显示打印信息），UART2（验证能否与上位机通信）
			// 		Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
			// 		REG8(Uart_base);
			// 		REG8(Uart_base);
			// 		REG8(Uart_base);
			// 		while(!(REG8(Uart_base + 0x5) & 0x1));
			// 		temp_data = REG8(Uart_base);
			// 		dprint("input 0x%x\n", temp_data);
			// 		dprint("start UART2 send\n");
			// 		Prt_flag = 1;
			// 	}
			// 	Mtimer_Cunt++;
			// 	dprint("start %d times UART2 test\n", Mtimer_Cunt);
			// 	UART2_TX = 0x41; UART2_TX = 0x41; UART2_TX = 0x41;
			// 	UART2_TX = 0xD;  UART2_TX = 0xA;
			// 	dprint("UART2 send AAA\n");
			// 	dprint("start receive ,send hexadecimal 0xCC by UART2\n");
			// 	while(UART2_RX != 0xcc);
			// 	dprint("UART2 received 0xCC\n");
			// 	if(Mtimer_Cunt >= 5)
			// 	{
			// 		REG8(Uart_base);     //清除掉打印串口接收FIFO
			// 		REG8(Uart_base);
			// 		REG8(Uart_base);
			// 		dprint("send the number of test module, otherwise enter next module\n");
			// 		do
			// 		{
			// 			timercunt++;
			// 			if(REG8(Uart_base + 0x5) & 0x1)
			// 			{
			// 				temp_data = REG8(Uart_base);
			// 				MCtrStep = temp_data;
			// 				Mtimer_Cunt = 0;
			// 				Prt_flag = 0;
			// 				dprint("start %d module test\n", MCtrStep);
			// 				return 1;
			// 			}
			// 		}
			// 		while(timercunt < 0xffffff);

			// 		MCtrStep += 1;
			// 		Mtimer_Cunt = 0;
			// 		Prt_flag = 0;
			// 		dprint("UART2 over\n");
			// 	}
			// 	break;

			// case 5:
			// 	if(Prt_flag == 0)
			// 	{
			// 		sysctl_iomux_uart3();
			// 		serial_init(3, 115200);
			// 		Uart_Int_Disable(3, 0);
			// 		dprint("perpare UART3\n");
			// 		dprint("completing the jumper, then send any key to start test\n");//上位机同时连接UART0（用来显示打印信息），UART3（验证能否与上位机通信）
			// 		Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
			// 		REG8(Uart_base);
			// 		REG8(Uart_base);
			// 		REG8(Uart_base);
			// 		while(!(REG8(Uart_base + 0x5) & 0x1));
			// 		temp_data = REG8(Uart_base);
			// 		dprint("input 0x%x\n", temp_data);
			// 		dprint("start UART3 send\n");
			// 		Prt_flag = 1;
			// 	}
			// 	Mtimer_Cunt++;
			// 	dprint("start %d times UART3 test\n", Mtimer_Cunt);
			// 	UART3_TX = 0x41; UART3_TX = 0x41; UART3_TX = 0x41;
			// 	UART3_TX = 0xD;  UART3_TX = 0xA;
			// 	dprint("UART3 send AAA\n");
			// 	dprint("start receive ,send hexadecimal 0xDD by UART3\n");
			// 	while(UART3_RX != 0xdd);
			// 	dprint("UART3 received 0xDD\n");
			// 	if(Mtimer_Cunt >= 5)
			// 	{
			// 		REG8(Uart_base);     //清除掉打印串口接收FIFO
			// 		REG8(Uart_base);
			// 		REG8(Uart_base);
			// 		dprint("send the number of test module, otherwise enter next module\n");
			// 		do
			// 		{
			// 			timercunt++;
			// 			if(REG8(Uart_base + 0x5) & 0x1)
			// 			{
			// 				temp_data = REG8(Uart_base);
			// 				MCtrStep = temp_data;
			// 				Mtimer_Cunt = 0;
			// 				Prt_flag = 0;
			// 				dprint("start %d module test\n", MCtrStep);
			// 				return 1;
			// 			}
			// 		}
			// 		while(timercunt < 0xffffff);

			// 		MCtrStep += 1;
			// 		Mtimer_Cunt = 0;
			// 		Prt_flag = 0;
			// 		dprint("UART3 over\n");
			// 	}
			// 	break;

		case 6:
			if (Prt_flag == 0)
			{
				// if(Test_Clock_Disable)
				// {
				// 	SYSCTL_MODEN0 &=(~UARTA_EN);
				// }
				sysctl_iomux_uarta(UARTA_TX_SEL, UARTA_RX_SEL);
				serial_init(UARTA_CHANNEL, 115200);

				Uart_Int_Disable(UARTA_CHANNEL, 0);
				dprint("perpare UARTA\n");
				dprint("completing the jumper, then send any key to start test\n");//上位机同时连接UART0（用来显示打印信息），UARTA（验证能否与上位机通信）
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				dprint("start UARTA send\n");
				Prt_flag = 1;
			}
			Mtimer_Cunt++;
			dprint("start %d times UARTA test\n", Mtimer_Cunt);
			UARTA_TX = 0x41; UARTA_TX = 0x41; UARTA_TX = 0x41;
			UARTA_TX = 0xD;  UARTA_TX = 0xA;
			dprint("UARTA send AAA\n");
			dprint("start receive ,send hexadecimal 0xDD by UARTA\n");
			while (UARTA_RX != 0xdd);
			dprint("UARTA received 0xDD\n");
			if (Mtimer_Cunt >= 5)
			{
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);

				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("UARTA over\n");
			}
			break;

		case 7:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN0 &= (~UARTB_EN);
				}
			#ifdef AE102
				SYSCTL_PIO5_UDCFG &= 0xffffff7f;                                ////bit7,0:为uartb，1：SMCLK
			#endif
			#ifdef TEST103
				printf("SYSCTL_PIOB_UPCFG000:%lx\n", SYSCTL_PIOB_UPCFG);  //查看GPIOB25,26是否上拉
				SYSCTL_PIOB_UPCFG &= 0xf9ffffff;                                ////bit7,0:为uartb，1：SMCLK
			#endif
						//
				sysctl_iomux_uartb();
				printf("0x30460:%x\n", *(unsigned int *)0x30460);
				serial_init(UARTB_CHANNEL, 115200);

				Uart_Int_Disable(UARTB_CHANNEL, 0);
				dprint("perpare UARTB\n");
				dprint("completing the jumper, then send any key to start test\n");//上位机同时连接UART0（用来显示打印信息），UARTB（验证能否与上位机通信）
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				dprint("start UARTB send\n");
				Prt_flag = 1;
			}
			Mtimer_Cunt++;
			dprint("start %d times UARTB test\n", Mtimer_Cunt);
			UARTB_TX = 0x41; UARTB_TX = 0x41; UARTB_TX = 0x41;
			UARTB_TX = 0xD;  UARTB_TX = 0xA;
			dprint("UARTB send AAA\n");
			dprint("start receive ,send hexadecimal 0xDD by UARTB\n");
			while (UARTB_RX != 0xdd);
			dprint("UARTB received 0xDD\n");
			if (Mtimer_Cunt >= 5)
			{
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);

				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("UARTB over\n");
			}
			break;
			//pwm测试
		case 8:
			if (Prt_flag == 0)           //只有在第一次轮询的时候Prt_flag == 0，会进入以下
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN0 &= (~PWM_EN);
				}
				sysctl_iomux_pwm0();
				sysctl_iomux_pwm1();
				sysctl_iomux_pwm2();
				sysctl_iomux_pwm3();
				sysctl_iomux_pwm4();
				sysctl_iomux_pwm5();
				sysctl_iomux_pwm6();
				sysctl_iomux_pwm7();
				dprint("prepare PWM0,1,2,3,4,5,6,7\n");
				dprint("completing the jumper, then send any key to start test\n");//用逻辑分析仪连接PWM0-7,查看是否有电平翻转
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
				dprint("PWM0,1,2,3,4,5,6,7 test start, output duty cycle is 50 percent\n");
				PWM_Init_channel(PWM_CHANNEL0, PWM_LOW, PWM_CLK0, PWM_CTR0, 50, 0);
				PWM_Init_channel(PWM_CHANNEL1, PWM_LOW, PWM_CLK0, PWM_CTR0, 50, 0);
				PWM_Init_channel(PWM_CHANNEL2, PWM_LOW, PWM_CLK0, PWM_CTR0, 50, 0);
				PWM_Init_channel(PWM_CHANNEL3, PWM_LOW, PWM_CLK0, PWM_CTR0, 50, 0);
				PWM_Init_channel(PWM_CHANNEL4, PWM_LOW, PWM_CLK0, PWM_CTR0, 50, 0);
				PWM_Init_channel(PWM_CHANNEL5, PWM_LOW, PWM_CLK0, PWM_CTR0, 50, 0);
				PWM_Init_channel(PWM_CHANNEL6, PWM_LOW, PWM_CLK0, PWM_CTR0, 50, 0);
				PWM_Init_channel(PWM_CHANNEL7, PWM_LOW, PWM_CLK0, PWM_CTR0, 50, 0);

			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt >= 40000)    //延时40s，在这40s内完成所有PWM的切换
			{
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					REG8(Uart_base);     //清除掉打印串口接收FIFO
					REG8(Uart_base);
					REG8(Uart_base);
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);      //软延时？
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("PWM0-7 over\n");
			}
			break;
		case 9:
			if (Prt_flag == 0)
			{
				//下文有专门测试timer关闭时钟的
				sysctl_iomux_config(GPIOB, 0, 0);   //用逻辑分析仪查看GPIOB0-3的电平是否翻转
				sysctl_iomux_config(GPIOB, 1, 0);
				sysctl_iomux_config(GPIOB, 2, 0);
				sysctl_iomux_config(GPIOB, 3, 0);
				GPIO1_DR0 = 0x00;        //GPIOB0-3输出0
				GPIO1_DDR0 |= 0x0f;      //GPIOB0-3方向改为输出
				dprint("completing the jumper of TIMER, then send any key to start test\n");
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
				TIMER_Init(TIMER0, TIMER0_1ms, 0x1, 0x0);  //使用定时器0的中断进行测试，每1ms翻转GPIOB0电平
				TIMER_Init(TIMER1, TIMER1_1ms, 0x1, 0x0);  //使用定时器1的中断进行测试，每1ms翻转GPIOB1电平
				TIMER_Init(TIMER2, TIMER2_1ms, 0x1, 0x0);  //使用定时器2的中断进行测试，每1ms翻转GPIOB2电平
				TIMER_Init(TIMER3, TIMER3_1ms, 0x1, 0x0);  //使用定时器3的中断进行测试，每1ms翻转GPIOB3电平
				dprint("TIMER0-3 start\n");
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt >= 10000)    //延时10s，在这10s内用逻辑分析仪连接GPIOB0-3,观察GPIOB0-3的电平翻转
			{
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					REG8(Uart_base);     //清除掉打印串口接收FIFO
					REG8(Uart_base);
					REG8(Uart_base);
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);      //软延时
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("TIMER0-3 over\n");
			}
			break;


		case 10:
			if (Prt_flag == 0)
			{
				//bypass
				dprint("completing the jumper of bypass, then send any key to start test\n");//逻辑分析仪连接GPIOB6,GPIOB2,GPIOA20,GPIOA23，查看他们会不会同GPIOC一样翻转
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
				dprint("bypass start\n");
				sysctl_iomux_config(GPIOC, 6, 0);   //GPIOI6配置成GPIOC6
				sysctl_iomux_config(GPIOC, 7, 0);   //GPIOI7配置成GPIOC7
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt == 100)    //0.1s时，把GPIOB6和GPIOA20 bypass为GPIOI6，GPIOB2和GPIOA23 bypass 为GPIOI7
			{
				sysctl_iomux_config(GPIOB, 6, 2);   //GPIOB6配置成GPIOI6
				sysctl_iomux_config(GPIOB, 2, 2);   //GPIOB2配置成GPIOI7
				sysctl_iomux_config(GPIOA, 20, 2);   //GPIOA20配置成GPIOI6
				//sysctl_iomux_config(GPIOA, 23, 2);   //fm，gle去掉了pa23的bypass功能

				GPIO2_DR0 = 0x00;    //GPIOC0-7输出0
				GPIO2_DDR0 |= 0xc0;  //GPIOC6-7配置为输出
				//每2s翻转GPIOC6-7的电平
				vDelayXms(2000);     //延时2s
				GPIO2_DR0 |= 0xc0;       //GPIOC0-7输出1
				vDelayXms(2000);     //延时2s
				GPIO2_DR0 &= ~(0xc0);    //GPIOC0-7输出0
				vDelayXms(2000);     //延时2s
				GPIO2_DR0 |= 0xc0;       //GPIOC0-7输出1
				vDelayXms(2000);     //延时2s
				GPIO2_DR0 &= ~(0xc0);    //GPIOC0-7输出0

			}
			if (Mtimer_Cunt >= 10000)
			{
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					REG8(Uart_base);     //清除掉打印串口接收FIFO
					REG8(Uart_base);
					REG8(Uart_base);
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);      //软延时
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("bypass over\n");
			}
			break;

		case 11:
			if (Prt_flag == 0)
			{

				sysctl_iomux_pwm0();
				//pwm,A0
				sysctl_iomux_tach0(TACH0_PIN_SEL);
				//tach0_pull_up();
				sysctl_iomux_tach1(TACH1_PIN_SEL);
				//tach1_pull_up();
				sysctl_iomux_tach2();
				//tach2_pull_up();
				sysctl_iomux_tach3(TACH3_PIN_SEL);
				//tach3_pull_up();
				TACH_Init_Channel(TACH_CHANNEL0, 1, 0);//用PWM0连接FAN2（PIOA0连接FAN2_PWM），用于驱动风扇转动
				TACH_Init_Channel(TACH_CHANNEL1, 1, 0);//tach0-3分别与FAN2_TACH连接，观察打印是否有打印风扇转速信息
				TACH_Init_Channel(TACH_CHANNEL2, 1, 0);
				TACH_Init_Channel(TACH_CHANNEL3, 1, 0);

				PWM_Init_channel(PWM_CHANNEL0, PWM_LOW, PWM_CLK0, PWM_CTR0, 90, 0);//pwm设置90，对应转速九百多
				dprint("perpare TACH0、1、2、3、4\n");
				dprint("completing the jumper, then send any key to start test\n");
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
				dprint("TACH0、2、3、4 start\n");

			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt % 1000 == 0)   //1s打印一次
			{
				printf("0x30458:%d", *(unsigned int *)0x30458);
				dprint("TACH0:%d\n", TACH_RPM(TACH_Get_Polling(0)));
				dprint("TACH1:%d\n", TACH_RPM(TACH_Get_Polling(1)));
				dprint("TACH2:%d\n", TACH_RPM(TACH_Get_Polling(2)));
				dprint("TACH3:%d\n", TACH_RPM(TACH_Get_Polling(3)));
			}
			if (Mtimer_Cunt >= 40000)   //延时40s
			{
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);   //软延时
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("TACH0、1、2、3、4 over\n");
			}
			break;


		case 12:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN0 &= (~SMB0_EN);
				}
				sysctl_iomux_i2c0(I2C0_CLK_SEL, I2C0_DAT_SEL);
				I2C_Channel_Init(I2C_CHANNEL_0, I2C0_SPEED, I2C_MASTER_ROLE, 0x4c, 1);
				dprint("perpare SMBUS0\n");
				dprint("completing the jumper, then send any key to start test\n");  //SMCLK0接TMP_SCL，SMDAT0接TMP_SDA
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
				dprint("SMBUS0 start\n");
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt % 1000 == 0)     //1s测一次
			{
				Get_Temperature(0);
			}
			if (Mtimer_Cunt >= 10000)      //测10s
			{
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("SMBUS0 over\n");
			}
			break;

		case 13:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN0 &= (~SMB1_EN);
				}
				sysctl_iomux_i2c1();
				I2C_Channel_Init(I2C_CHANNEL_1, I2C1_SPEED, I2C_MASTER_ROLE, 0x4c, 1);
				dprint("perpare SMBUS1\n");
				dprint("completing the jumper, then send any key to start test\n");//SMCLK1接TMP_SCL，SMDAT1接TMP_SDA
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
				dprint("SMBUS1 start\n");
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt % 1000 == 0)
			{
				Get_Temperature(1);
			}
			if (Mtimer_Cunt >= 10000)
			{
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("SMBUS1 over\n");
			}
			break;

		case 14:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN0 &= (~SMB2_EN);
				}
				sysctl_iomux_i2c2(I2C2_CLK_SEL);
				I2C_Channel_Init(I2C_CHANNEL_2, I2C2_SPEED, I2C_MASTER_ROLE, 0x4c, 1);
				dprint("perpare SMBUS2\n");
				dprint("completing the jumper, then send any key to start test\n");//SMCLK2接TMP_SCL，SMDAT2接TMP_SDA
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
				dprint("SMBUS2 start\n");
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt % 1000 == 0)
			{
				Get_Temperature(2);
			}
			if (Mtimer_Cunt >= 10000)
			{
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("SMBUS2 over\n");
			}
			break;


		case 15:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN0 &= (~SMB3_EN);
				}
				sysctl_iomux_i2c3();
				I2C_Channel_Init(I2C_CHANNEL_3, I2C3_SPEED, I2C_MASTER_ROLE, 0x4c, 1);
				dprint("perpare SMBUS3\n");
				dprint("completing the jumper, then send any key to start test\n");//SMCLK3接TMP_SCL，SMDAT3接TMP_SDA
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
				dprint("SMBUS3 start\n");
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt % 1000 == 0)
			{
				Get_Temperature(3);
			}
			if (Mtimer_Cunt >= 10000)
			{
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("SMBUS3 over\n");
			}
			break;

		case 16:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN0 &= (~SPIM_EN);
				}
				sysctl_iomux_spim(0, 0, 0);
				sysctl_iomux_spim_cs(1, 0);
				// SPIM_Init(0, SPIM_CTRL_CPOL_LOW, SPIM_CTRL_CPHA_LOW, SPIM_CTRL_LSB, 0x7, 1);
				// dprint("perpare SPIM\n");
				// dprint("completing the jumper, then send any key to start test\n");//spim_sck接SSCK，spim_mosi接SMOSI，spim_miso接SMISO
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);              //102：PIOB18接SCSN0,PIOB16接SCSN1 103：PIOB18接SCSN0,PIOB4接SCSN1
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
			}
			Mtimer_Cunt++;

			// if (Mtimer_Cunt % 6000 == 0)   //第6秒测试与FLASH0的写入和读出
			// {
			// 	printf("Write and Read test for SPIM and FLASH0\n");
			// 	// SPI_Read_ID0();
			// 	SPIM_Erase_FLASH_Data(0x0, 0);
			// 	uint8_t write_data[256];
			// 	uint8_t read_data[256];
			// 	for (int i = 0; i < 256; i++)
			// 	{
			// 		write_data[i] = i;
			// 	}
			// 	SPIM_Write_FLASH_Data(0, 256, write_data, 0);
			// 	SPIM_Send_FLASH_Cmd_Addr(read_data, 0, 256, 0);
			// 	for (int i = 0; i < 256; i++)
			// 	{
			// 		printf("read data buff is 0x%x\n", read_data[i]);
			// 	}
			// }
			// if (Mtimer_Cunt % 9900 == 0)   //第9.9秒测试与FLASH1的写入和读出
			// {
			// 	printf("Write and Read test for SPIM and FLASH1\n");
			// 	SPI_Read_ID1();
			// 	SPIM_Erase_FLASH_Data(0x0, 1);
			// 	uint8_t write_data[256];
			// 	uint8_t read_data[256];
			// 	for (int i = 0; i < 256; i++)
			// 	{
			// 		write_data[i] = i;
			// 	}
			// 	SPIM_Write_FLASH_Data(0, 256, write_data, 1);
			// 	SPIM_Send_FLASH_Cmd_Addr(read_data, 0, 256, 1);
			// 	for (int i = 0; i < 256; i++)
			// 	{
			// 		printf("read data buff is 0x%x\n", read_data[i]);
			// 	}
			// }


			if (Mtimer_Cunt >= 10000)   //延时10s
			{
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);   //软延时
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("SPIM over\n");
			}
			break;


		case 17:
			if (Prt_flag == 0)
			{
				dprint("completing the jumper of PWRSW, then send any key to start test\n");//完成GPIOB4与按键SWL的连接，SWL按下为低电平
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
				dprint("PWRSW start\n");
				if (PWRSW_mode)
				{
					PWRSW_Config(0, 1);//超时时间为1s，直接复位   
				}
				else
				{
					PWRSW_Config(0, 0);//超时时间为1s，产生中断
				}
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt == 5000) //测试cpu中断关闭
			{
				irqc_disable_interrupt(IRQC_INT_DEVICE_PWRSW); //关闭cpu中断通道5

			}
			if (Mtimer_Cunt == 10000)
			{
				irqc_enable_interrupt(IRQC_INT_DEVICE_PWRSW); //打开cpu中断通道5

			}
			if (Mtimer_Cunt >= 15000)    //延时10s，在这10s内进行测试，长按SWL超过1s会触发PWRSW_HANDLER中断
			{
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					REG8(Uart_base);     //清除掉打印串口接收FIFO
					REG8(Uart_base);
					REG8(Uart_base);
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);      //软延时
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("PWRSW over\n");
			}
			break;

			// case 18:
			//  	if(Prt_flag == 0)
			//  	{
			// 		if(Test_Clock_Disable)
			// 		{
			// 			SYSCTL_MODEN0 &=(~ADC_EN);
			// 		}
			// 		dprint("completing the jumper of ADC software single-ended test,then send any key\n");//将PIOC0-7与ADC_R连接
			// 		Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
			// 		REG8(Uart_base);
			// 		REG8(Uart_base);
			// 		REG8(Uart_base);
			// 		while(!(REG8(Uart_base + 0x5) & 0x1));
			// 		temp_data = REG8(Uart_base);
			// 		dprint("Input detected:0x%x\n", temp_data);
			// 		Prt_flag = 1;
			// 	}
			// 	Mtimer_Cunt++;
			// 	if(Mtimer_Cunt == 1000)
			// 	{
			// 		// ADC_HW_Sample_Init(ADC_CHANNEL0,0,0,SINGLE_ENDED,500);
			// 		// ADC_HW_Sample_Init(ADC_CHANNEL1,1,0,SINGLE_ENDED,500);
			// 		// ADC_HW_Sample_Init(ADC_CHANNEL2,2,0,SINGLE_ENDED,500);
			// 		// ADC_HW_Sample_Init(ADC_CHANNEL3,3,0,SINGLE_ENDED,500);
			// 		// ADC_HW_Sample_Init(ADC_CHANNEL4,4,0,SINGLE_ENDED,500);
			// 		// ADC_HW_Sample_Init(ADC_CHANNEL5,5,0,SINGLE_ENDED,500);
			// 		// ADC_HW_Sample_Init(ADC_CHANNEL6,6,0,SINGLE_ENDED,500);
			// 		// ADC_HW_Sample_Init(ADC_CHANNEL7,7,0,SINGLE_ENDED,500);

			// 	}
			// 	if(Mtimer_Cunt >= 10000)
			// 	{
			// 		dprint("Enter test module via serial, or auto-advance soon.\n");
			// 		do
			// 		{
			// 			REG8(Uart_base);     //清除掉打印串口接收FIFO
			// 			REG8(Uart_base);
			// 			REG8(Uart_base);
			// 			timercunt++;
			// 			if(REG8(Uart_base + 0x5) & 0x1)
			// 			{
			// 				temp_data = REG8(Uart_base);
			// 				MCtrStep = temp_data;
			// 				Mtimer_Cunt = 0;
			// 				Prt_flag = 0;
			// 				dprint("Proceeding to test step %d\n", MCtrStep);
			// 				return 1;
			// 			}
			// 		}
			// 		while(timercunt < 0xffffff);
			// 		MCtrStep += 1;
			// 		Mtimer_Cunt = 0;
			// 		Prt_flag = 0;
			// 	}
			// 	break;

		case 19:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN1 &= (~IRAM_EN);
				}
				dprint("prepare IRAM,Send any key to begin test\n");    //直接上电检测
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("Input detected:0x%x\n", temp_data);
				Prt_flag = 1;
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt == 1000)
			{
				dprint("Reading 48 bytes from IRAM 0x28000\n");
				for (i = 0; i < 48; i++)
				{
					dprint("0x%x:%x\n", (0x28000 + i), *((volatile uint8_t *)(IRAM_BASE_ADDR + i)));
				}
			}
			if (Mtimer_Cunt == 2000)
			{
				dprint("Writing 0-48 bytes to IRAM 0x28000\n");
				for (i = 0; i < 48; i++)
				{
					*((volatile uint8_t *)(IRAM_BASE_ADDR + i)) = i;
				}
			}
			if (Mtimer_Cunt == 3000)
			{
				dprint("Reading 48 bytes from IRAM 0x28000\n");
				for (i = 0; i < 48; i++)
				{
					if (*((volatile uint8_t *)(IRAM_BASE_ADDR + i)) != i)
					{
						dprint("地址0x%x的写入或读取的值不正确，读取的数据为%x\n", (0x28000 + i), *((volatile uint8_t *)(IRAM_BASE_ADDR + i)));
						break;
					}
				}
				if (i < 48)
					dprint("IRAM failed\n");
				else
					dprint("IRAM passed\n");
			}
			if (Mtimer_Cunt >= 5000)
			{
				dprint("Enter test module via serial, or auto-advance soon.\n");
				do
				{
					REG8(Uart_base);
					REG8(Uart_base);
					REG8(Uart_base);
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("Proceeding to test step %d\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
			}
			break;

		case 20:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN0 &= (~BRAM_EN);
				}
				dprint("prepare BRAM,Send any key to begin test\n");    //直接上电检测
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("Input detected:0x%x\n", temp_data);
				Prt_flag = 1;
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt == 1000)
			{
				dprint("Reading 48 bytes from BRAM 0x2700\n");
				for (i = 0; i < 48; i++)
				{
					dprint("0x%x:%x\n", (0x2700 + i), *((volatile uint8_t *)(BRAM_BASE_ADDR + i)));
				}
			}
			if (Mtimer_Cunt == 2000)
			{
				dprint("Writing 0-48 bytes to BRAM 0x2700\n");
				for (i = 0; i < 48; i++)
				{
					*((volatile uint8_t *)(BRAM_BASE_ADDR + i)) = i;
				}
			}
			if (Mtimer_Cunt == 3000)
			{
				dprint("Reading 48 bytes from BRAM 0x2700\n");
				for (i = 0; i < 48; i++)
				{
					if (*((volatile uint8_t *)(BRAM_BASE_ADDR + i)) != i)
					{
						dprint("地址0x%x的写入或读取的值不正确，读取的数据为%x\n", (0x2700 + i), *((volatile uint8_t *)(BRAM_BASE_ADDR + i)));
						break;
					}
				}
				if (i < 48)
					dprint("BRAM failed\n");
				else
					dprint("BRAM passed\n");
			}
			if (Mtimer_Cunt >= 5000)
			{
				dprint("Enter test module via serial, or auto-advance soon.\n");
				do
				{
					REG8(Uart_base);
					REG8(Uart_base);
					REG8(Uart_base);
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("Proceeding to test step %d\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
			}
			break;

		case 21:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN1 &= (~SPIFE_EN);
				}
				printf("0x3042c:%x\n", *(unsigned int *)0x3042c);
				*(unsigned int *)0x3042c &= ~(0x100);//gle需要关闭复位
				sysctl_iomux_config(GPIOB, 20, 1);//fspi_mosi(inout)
				sysctl_iomux_config(GPIOB, 21, 1);//fspi_miso(inout)
				sysctl_iomux_config(GPIOB, 22, 1);//fspi_csn0(out)
				sysctl_iomux_config(GPIOA, 16, 2);//fspi_csn1(out)
				sysctl_iomux_config(GPIOB, 23, 1);//fspi_sck(out)
				// sysctl_iomux_config(GPIOB, 30, 1);//fspi_hold(inout)
				printf("0x3047C:%x\n", *(unsigned int *)0x3047C);
				dprint("prepare SPIF,Send any key to begin test\n");            //不需要任何跳线，只用把FLASH放入开发板
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("Input detected:0x%x\n", temp_data);
				Prt_flag = 1;
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt == 1000)
			{
				dprint("Reading FLASH ID test\n");
				SPIFE_READ_ID();
			}
			BYTE data[256];
			if (Mtimer_Cunt == 2000)
			{
				dprint("Writing 0-255 to FLASH address 0x1CC00\n");
				for (int i = 0; i < 256; i++)
				{
					data[i] = i;
				}
				RunSPIFE_WriteFromRAM(0x01CC00, data, 256);
				dprint("FLASH write complete\n");
			}
			if (Mtimer_Cunt == 3000)
			{
				dprint("Reading 256 bytes at FLASH 0x1CC00\n");
				for (int i = 0; i < 256; i++)
				{
					data[i] = 0;
				}
				RunSPIFE_ReadFromRAM(0x01CC00, data, 256);
			}
			if (Mtimer_Cunt == 4000)
			{
				dprint("Writing 0xAA to FLASH 0x1CC00\n");
				for (int i = 0; i < 256; i++)
				{
					data[i] = 0xaa;
				}
				RunSPIFE_WriteFromRAM(0x01CC00, data, 256);
				dprint("complete\n");
			}
			if (Mtimer_Cunt == 5000)
			{
				dprint("Reading 256 bytes from FLASH 0x1CC00\n");
				for (int i = 0; i < 256; i++)
				{
					data[i] = 0;
				}
				RunSPIFE_ReadFromRAM(0x01CC00, data, 256);
			}
			if (Mtimer_Cunt >= 6000)
			{
				dprint("Enter test module via serial, or auto-advance soon.\n");
				do
				{
					REG8(Uart_base);
					REG8(Uart_base);
					REG8(Uart_base);
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("Proceeding to test step %d\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
			}
			break;

		case 22:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN0 &= (~WDT_EN);
				}
				if (WDT_mode)
				{
					WDT_Init(0x1, 0xa);     //不需要接任何跳线上电，能够进入看门狗中断就行
					sysctl_iomux_config(GPIOA, 0, 0); //GPIOA[0] 进入中断电平反转测试
				}
				else
				{
					WDT_Init(0x0, 0xff);   //到达时间直接复位，可在1sec函数中喂狗。//89s
				}
				dprint("watchDOG,Send any key to begin test\n");
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("Input detected:0x%x\n", temp_data);
				Prt_flag = 1;
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt >= 5000)
			{
				dprint("Enter test module via serial, or auto-advance soon.\n");
				do
				{
					REG8(Uart_base);
					REG8(Uart_base);
					REG8(Uart_base);
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("Proceeding to test step %d\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				printf("wdt out\r\n");
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;     //最后一个模块不使得Prt_flag = 0则不会测试下一个模块
			}
			break;


			// case 23:
			//     if(Prt_flag == 0)
			//     {
			//         TIMER_Init(TIMER3, TIMER3_1ms, 0x0, 0x0); // 1ms
			//         sysctl_iomux_adc0();
			//         sysctl_iomux_adc1();
			//         sysctl_iomux_adc2();
			//         sysctl_iomux_adc3();
			//         sysctl_iomux_adc4();
			//         sysctl_iomux_adc5();
			//         sysctl_iomux_adc6();
			//         sysctl_iomux_adc7();
			//         dprint("completing the jumper of ADC hardware differential test,then send any key\n");//硬件差分，0/1，2/3等为一组，把0接ADC_R，1接地，也可以交换着接，其他组同理
			//         Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         while(!(REG8(Uart_base + 0x5) & 0x1));
			//         temp_data = REG8(Uart_base);
			//         dprint("Input detected:0x%x\n", temp_data);
			//         Prt_flag = 1;
			//     }
			//     Mtimer_Cunt++;
			//     if(Mtimer_Cunt % 1000 == 0)
			//     {
			//         ADC_Init(((Mtimer_Cunt / 1000) % 2), 1, 1, 0);
			//         ADC_IRQ_Config(AF_IRQ, ENABLE);
			//         // ADC_Timercount_Set(3200);
			//         dprint("Reading ADC channel %d sample\n", ((Mtimer_Cunt / 1000) % 2));
			//         while((ADC_INTMASK & 0x1) == 0);
			//     }
			//     if(Mtimer_Cunt >= 17000)
			//     {
			//         dprint("Enter test module via serial, or auto-advance soon.\n");
			//         do
			//         {
			//             REG8(Uart_base);
			//             REG8(Uart_base);
			//             REG8(Uart_base);
			//             timercunt++;
			//             if(REG8(Uart_base + 0x5) & 0x1)
			//             {
			//                 temp_data = REG8(Uart_base);
			//                 MCtrStep = temp_data;
			//                 Mtimer_Cunt = 0;
			//                 Prt_flag = 0;
			//                 dprint("Proceeding to test step %d\n", MCtrStep);
			//                 return 1;
			//             }
			//         }
			//         while(timercunt < 0xffffff);
			//         MCtrStep += 1;
			//         Mtimer_Cunt = 0;
			//         Prt_flag = 0;
			//     }
			//     break;
			// case 24:
			//     if(Prt_flag == 0)
			//     {
			//         sysctl_iomux_can0();
			//         Can_Int_Enable(CAN0, rie);  //使能CAN0接收中断，在中断里把接收到的数据打印出来
			//         dprint("perpare CAN0\n");
			//         dprint("completing the jumper, then send any key to start test\n");
			//         Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         while(!(REG8(Uart_base + 0x5) & 0x1));
			//         temp_data = REG8(Uart_base);
			//         dprint("input 0x%x\n", temp_data);
			//         Prt_flag = 1;
			//     }
			//     Mtimer_Cunt++;
			//     if(Mtimer_Cunt == 1000)    //第1s测试CAN1通信，波特率为125kbs
			//     {
			//         dprint("perpare CAN0, computer baudrate:125kbs,acr:0x5ab,amr:0xfffff000,send id:0x123,amr:0xfffff000\n");
			//         dprint("completing the jumper, then send any key to start test\n");
			//         Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         while(!(REG8(Uart_base + 0x5) & 0x1));
			//         temp_data = REG8(Uart_base);
			//         dprint("input 0x%x\n", temp_data);
			//         printf("CAN0_125kbs satrt\n");
			//         CAN_Init(_125kbs, 0x123, 0xfffff000, CAN0);    //上位机发送帧验收码为0x123，屏蔽吗为0xfffff000
			//         // ec发送数据函数
			//         uint8_t CAN_DATA[8] = { 0,1,2,3,4,5,6,7 };
			//         for(int i = 0; i < 10; i++)
			//         {
			//             CAN_SendData(Standard_frame, 0x5ab, 8, CAN_DATA, CAN0);
			//         }
			//         printf("EC had sent to computer, please send to EC by computer\n");
			//         printf("After 20s,baudrate change to 100kbs\n");
			//     }
			//     if((Mtimer_Cunt == 20000))    //第20s测试CAN1通信，波特率为100kbs
			//     {
			//         dprint("perpare CAN0, computer baudrate:100kbs,acr:0x5ab,amr:0xfffff000,send id:0x123,amr:0xfffff000\n");
			//         dprint("completing the jumper, then send any key to start test\n");
			//         Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         while(!(REG8(Uart_base + 0x5) & 0x1));
			//         temp_data = REG8(Uart_base);
			//         dprint("input 0x%x\n", temp_data);
			//         printf("CAN0_100kbs satrt\n");
			//         CAN_Init(_100kbs, 0x123, 0xfffff000, CAN0);    //上位机发送帧验收码为0x123，屏蔽吗为0xfffff000

			//         //ec发送数据函数
			//         uint8_t CAN_DATA[8] = { 0,1,2,3,4,5,6,7 };
			//         for(int i = 0; i < 10; i++)
			//         {
			//             CAN_SendData(Standard_frame, 0x5ab, 8, CAN_DATA, CAN0);
			//         }
			//         printf("EC had sent to computer, please send to EC by computer\n");
			//     }
			//     if(Mtimer_Cunt >= 30000)   //延时30s
			//     {
			//         REG8(Uart_base);     //清除掉打印串口接收FIFO
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         dprint("send the number of test module, otherwise enter next module\n");
			//         do
			//         {
			//             timercunt++;
			//             if(REG8(Uart_base + 0x5) & 0x1)
			//             {
			//                 temp_data = REG8(Uart_base);
			//                 MCtrStep = temp_data;
			//                 Mtimer_Cunt = 0;
			//                 Prt_flag = 0;
			//                 dprint("start %d module test\n", MCtrStep);
			//                 return 1;
			//             }
			//         }
			//         while(timercunt < 0xffffff);   //软延时
			//         MCtrStep += 1;
			//         Mtimer_Cunt = 0;
			//         Prt_flag = 0;
			//         dprint("CAN over\n");
			//     }
			//     break;


			// case 25:
			//     if(Prt_flag == 0)
			//     {
			//         can1_ModuleClock_EN;
			//         sysctl_iomux_can1();
			//         Can_Int_Enable(CAN1, rie);  //使能CAN1接收中断，在中断里把接收到的数据打印出来
			//         dprint("perpare CAN1\n");
			//         dprint("completing the jumper, then send any key to start test\n");
			//         Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         while(!(REG8(Uart_base + 0x5) & 0x1));
			//         temp_data = REG8(Uart_base);
			//         dprint("input 0x%x\n", temp_data);
			//         Prt_flag = 1;
			//     }
			//     Mtimer_Cunt++;
			//     if(Mtimer_Cunt == 1000)    //第1s测试CAN1通信，波特率为125kbs
			//     {
			//         dprint("perpare CAN1, computer baudrate:125kbs,acr:0x5ab,amr:0xfffff000,send id:0x123,amr:0xfffff000\n");
			//         dprint("completing the jumper, then send any key to start test\n");
			//         Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         while(!(REG8(Uart_base + 0x5) & 0x1));
			//         temp_data = REG8(Uart_base);
			//         dprint("input 0x%x\n", temp_data);
			//         printf("CAN1_125kbs satrt\n");
			//         CAN_Init(_125kbs, 0x123, 0xfffff000, CAN1);    //上位机发送帧验收码为0x123，屏蔽吗为0xfffff000
			//         //ec发送数据函数
			//         uint8_t CAN_DATA[8] = { 0,1,2,3,4,5,6,7 };
			//         for(int i = 0; i < 10; i++)
			//         {
			//             CAN_SendData(Standard_frame, 0x5ab, 8, CAN_DATA, CAN1);
			//         }
			//         printf("EC had sent to computer, please send to EC by computer\n");
			//         printf("After 20s,baudrate change to 100kbs\n");
			//     }
			//     if((Mtimer_Cunt == 20000))    //第20s测试CAN1通信，波特率为100kbs
			//     {
			//         dprint("perpare CAN1, computer baudrate:100kbs,acr:0x5ab,amr:0xfffff000,send id:0x123,amr:0xfffff000\n");
			//         dprint("completing the jumper, then send any key to start test\n");
			//         Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         while(!(REG8(Uart_base + 0x5) & 0x1));
			//         temp_data = REG8(Uart_base);
			//         dprint("input 0x%x\n", temp_data);
			//         printf("CAN1_100kbs start\n");
			//         CAN_Init(_100kbs, 0x123, 0xfffff000, CAN1);    //上位机发送帧验收码为0x123，屏蔽吗为0xfffff000

			//         //ec发送数据函数
			//         uint8_t CAN_DATA[8] = { 0,1,2,3,4,5,6,7 };
			//         for(int i = 0; i < 10; i++)
			//         {
			//             CAN_SendData(Standard_frame, 0x5ab, 8, CAN_DATA, CAN1);
			//         }
			//         printf("EC had sent to computer, please send to EC by computer\n");
			//     }
			//     if(Mtimer_Cunt >= 30000)   //延时30s
			//     {
			//         REG8(Uart_base);     //清除掉打印串口接收FIFO
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         dprint("send the number of test module, otherwise enter next module\n");
			//         do
			//         {
			//             timercunt++;
			//             if(REG8(Uart_base + 0x5) & 0x1)
			//             {
			//                 temp_data = REG8(Uart_base);
			//                 MCtrStep = temp_data;
			//                 Mtimer_Cunt = 0;
			//                 Prt_flag = 0;
			//                 dprint("start %d module test\n", MCtrStep);
			//                 return 1;
			//             }
			//         }
			//         while(timercunt < 0xffffff);   //软延时
			//         MCtrStep += 1;
			//         Mtimer_Cunt = 0;
			//         Prt_flag = 0;
			//         dprint("CAN1 over\n");
			//     }
			//     break;

			// case 26:
			//     if(Prt_flag == 0)
			//     {
			//         sysctl_iomux_can2();
			//         Can_Int_Enable(CAN2, rie);  //使能CAN2接收中断，在中断里把接收到的数据打印出来
			//         dprint("perpare CAN2\n");
			//         dprint("completing the jumper, then send any key to start test\n");
			//         Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         while(!(REG8(Uart_base + 0x5) & 0x1));
			//         temp_data = REG8(Uart_base);
			//         dprint("input 0x%x\n", temp_data);
			//         Prt_flag = 1;
			//         dprint("CAN2 start\n");
			//     }
			//     Mtimer_Cunt++;
			//     if(Mtimer_Cunt == 1000)    //第1s测试CAN2通信，波特率为125kbs
			//     {
			//         dprint("perpare CAN2, computer baudrate:125kbs,acr:0x5ab,amr:0xfffff000,send id:0x123,amr:0xfffff000\n");
			//         dprint("completing the jumper, then send any key to start test\n");
			//         Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         while(!(REG8(Uart_base + 0x5) & 0x1));
			//         temp_data = REG8(Uart_base);
			//         dprint("input 0x%x\n", temp_data);
			//         printf("CAN2_125kbs start\n");
			//         CAN_Init(_125kbs, 0x123, 0xfffff000, CAN2);    //上位机发送帧验收码为0x123，屏蔽吗为0xfffff000
			//         // ec发送数据函数
			//         uint8_t CAN_DATA[8] = { 0,1,2,3,4,5,6,7 };
			//         for(int i = 0; i < 10; i++)
			//         {
			//             CAN_SendData(Standard_frame, 0x5ab, 8, CAN_DATA, CAN2);
			//         }
			//         printf("EC had sent to computer, please send to EC by computer\n");
			//         printf("After 20s,baudrate change to 100kbs\n");
			//     }
			//     if((Mtimer_Cunt == 20000))    //第20s测试CAN1通信，波特率为100kbs
			//     {
			//         dprint("perpare CAN2, computer baudrate:125kbs,acr:0x5ab,amr:0xfffff000,send id:0x123,amr:0xfffff000\n");
			//         dprint("completing the jumper, then send any key to start test\n");
			//         Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         while(!(REG8(Uart_base + 0x5) & 0x1));
			//         temp_data = REG8(Uart_base);
			//         dprint("input 0x%x\n", temp_data);
			//         printf("CAN2_100kbs start\n");
			//         CAN_Init(_100kbs, 0x123, 0xfffff000, CAN2);    //上位机发送帧验收码为0x123，屏蔽吗为0xfffff000

			//         //ec发送数据函数
			//         uint8_t CAN_DATA[8] = { 0,1,2,3,4,5,6,7 };
			//         for(int i = 0; i < 10; i++)
			//         {
			//             CAN_SendData(Standard_frame, 0x5ab, 8, CAN_DATA, CAN2);
			//         }
			//         printf("EC had sent to computer, please send to EC by computer\n");
			//     }
			//     if(Mtimer_Cunt >= 30000)   //延时30s
			//     {
			//         REG8(Uart_base);     //清除掉打印串口接收FIFO
			//         REG8(Uart_base);
			//         REG8(Uart_base);
			//         dprint("send the number of test module, otherwise enter next module\n");
			//         do
			//         {
			//             timercunt++;
			//             if(REG8(Uart_base + 0x5) & 0x1)
			//             {
			//                 temp_data = REG8(Uart_base);
			//                 MCtrStep = temp_data;
			//                 Mtimer_Cunt = 0;
			//                 Prt_flag = 0;
			//                 dprint("start %d module test\n", MCtrStep);
			//                 return 1;
			//             }
			//         }
			//         while(timercunt < 0xffffff);   //软延时
			//         MCtrStep += 1;
			//         Mtimer_Cunt = 0;
			//         Prt_flag = 0;
			//         dprint("CAN2 over\n");
			//     }
			//     break;

		case 27:
			if (Prt_flag == 0)  //注意点：实测，在测试此功能时，需要屏蔽主函数服务函数中的两个函数
				//Service_Debugger Service_CMD
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN1 &= (~DRAM_EN);
				}
				dprint("prepare DRAM,Send any key to begin test\n");    //直接上电检测
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("Input detected:0x%x\n", temp_data);
				Prt_flag = 1;
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt == 1000)
			{
				dprint("Reading 48 bytes from DRAM 0x20000\n");
				for (i = 0; i < 48; i++)
				{
					dprint("0x%x:%x\n", (0x20000 + i), *((volatile uint8_t *)(DRAM_BASE_ADDR + i)));
				}
			}
			if (Mtimer_Cunt == 2000)
			{
				dprint("Writing 0-48 bytes to DRAM 0x20000\n");
				for (i = 0; i < 48; i++)
				{
					*((volatile uint8_t *)(DRAM_BASE_ADDR + i)) = i;
				}
			}
			if (Mtimer_Cunt == 3000)
			{
				dprint("Reading 48 bytes from DRAM 0x20000\n");
				for (i = 0; i < 48; i++)
				{
					dprint("0x%x:%x\n", (0x20000 + i), *((volatile uint8_t *)(DRAM_BASE_ADDR + i)));
				}

				if (i < 48)
					dprint("DRAM failed\n");
				else
					dprint("DRAM passed\n");
			}
			if (Mtimer_Cunt >= 5000)
			{
				dprint("Enter test module via serial, or auto-advance soon.\n");
				do
				{
					REG8(Uart_base);
					REG8(Uart_base);
					REG8(Uart_base);
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("Proceeding to test step %d\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
			}
			break;

		case 28:
			if (Prt_flag == 0)
			{
				//porint
				Por_Int_Enable(0);
				sysctl_iomux_config(GPIOB, 6, 1);
				dprint("PORINT,Send any key to begin test\n");
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("Input detected:0x%x\n", temp_data);
				Prt_flag = 1;
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt >= 5000)
			{
				dprint("Enter test module via serial, or auto-advance soon.\n");
				do
				{
					REG8(Uart_base);
					REG8(Uart_base);
					REG8(Uart_base);
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("Proceeding to test step %d\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;     //最后一个模块不使得Prt_flag = 0则不会测试下一个模块
			}

			break;

		case 29:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN1 &= (~SMB4_EN);
				}
				sysctl_iomux_i2c4();
				I2C_Channel_Init(I2C_CHANNEL_4, I2C4_SPEED, I2C_MASTER_ROLE, 0x4c, 1);
				dprint("perpare SMBUS4\n");
				dprint("completing the jumper, then send any key to start test\n");//SMCLK3接TMP_SCL，SMDAT3接TMP_SDA
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
				dprint("SMBUS4 start\n");
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt % 1000 == 0)
			{
				Get_Temperature(4);
			}
			if (Mtimer_Cunt >= 10000)
			{
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("SMBUS4 over\n");
			}
			break;

		case 30:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN1 &= (~SMB5_EN);
				}
				sysctl_iomux_i2c5();
				I2C_Channel_Init(I2C_CHANNEL_5, I2C5_SPEED, I2C_MASTER_ROLE, 0x4c, 1);
				dprint("perpare SMBUS5\n");
				dprint("completing the jumper, then send any key to start test\n");//SMCLK3接TMP_SCL，SMDAT3接TMP_SDA
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
				dprint("SMBUS5 start\n");
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt % 1000 == 0)
			{
				Get_Temperature(5);
			}
			if (Mtimer_Cunt >= 10000)
			{
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("SMBUS5 over\n");
			}
			break;

		case 31:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN1 &= (~SYSCTL_EN);
				}
				dprint("all chip reset,Send any key to begin test\n");
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("Input detected:0x%x\n", temp_data);
				Prt_flag = 1;
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt == 1000)
			{
				dprint("read reserve register 0x30418\n");
				dprint("0x30418:%x\n", *(unsigned short *)0x30418);
				*(unsigned short *)0x30418 = 0xf0f0;
			}
			if (Mtimer_Cunt == 2000)
			{
				SYSCTL_RST1 |= (3 << 15);
				dprint("all chip reset\n");
			}
			if (Mtimer_Cunt >= 5000)
			{
				dprint("Enter test module via serial, or auto-advance soon.\n");
				do
				{
					REG8(Uart_base);
					REG8(Uart_base);
					REG8(Uart_base);
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("Proceeding to test step %d\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;     //最后一个模块不使得Prt_flag = 0则不会测试下一个模块
			}
			break;

		case 32:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN1 &= (~SYSCTL_EN);
				}
				dprint("all Module clock reset test,Send any key to begin test\n");
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("Input detected:0x%x\n", temp_data);
				Prt_flag = 1;
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt == 1000)
			{
				//各模块时钟开闭，中断enable,disable,mask
				SYSCTL_MODEN1 |= APB_EN;   //EP_EN

				/*从SYSCTL_MODEN1低位开始*/

				// 开时钟：bit0
				SYSCTL_MODEN1 |= TMR0_EN;
				//初始化
				TIMER0_TCR = 0xff;
				printf("TIMER0_TCR:%x\n", TIMER0_TCR);

				// 开时钟：bit1
				SYSCTL_MODEN1 |= ICTL_EN;

				//初始化
				ICTL0_INTEN0 = 0xff;
				ICTL1_INTEN0 = 0xff;
				printf("ICTL0_INTEN0:%x\n", ICTL0_INTEN0);
				printf("ICTL1_INTEN0:%x\n", ICTL1_INTEN0);

				// 开时钟：bit2
				SYSCTL_MODEN1 |= H2E_EN;
				//初始化
				//

				// 开时钟：bit3
				SYSCTL_MODEN1 |= SPIFE_EN;
				//初始化
				*((unsigned char *)0x30004) = 0xff;
				printf("*0x30004:%x\n", *((unsigned char *)0x30004));

				// 开时钟：bit4
				SYSCTL_MODEN1 |= SRAM_EN;
				//初始化
				//复位寄存器不复位sram

				// 开时钟：bit5
				SYSCTL_MODEN1 |= GPIODB_EN;
				//初始化
				GPIO1_DR0 = 0xff;
				printf("GPIO1_DR0:%x\n", GPIO1_DR0);

				// 开时钟：bit6
				SYSCTL_MODEN1 |= DMA_EN;

				//初始化
				*((unsigned char *)0x32000) = 0xff;
				printf("*0x32000:%x\n", *((unsigned char *)0x32000));

				// 开时钟：bit7
				SYSCTL_MODEN1 |= SPIFI_EN;
				//初始化
				*((unsigned char *)0x32400) = 0xff;
				printf("*0x32400:%x\n", *((unsigned char *)0x32400));

				// // 开时钟：bit8
				// SYSCTL_MODEN1 |= SM2_EN; 
				// //初始化
				// *((unsigned char *)0x30C00) =0xff;
				// printf("*0x30C00:%x\n",*((unsigned char *)0x30C00));

				// 开时钟：bit9
				SYSCTL_MODEN1 |= SYSCTL_EN;
				//初始化
				*((unsigned char *)0x30400) = 0xff;
				printf("*0x30400:%x\n", *((unsigned char *)0x30400));

				// 开时钟：bit10
				SYSCTL_MODEN1 |= DRAM_EN;
				//初始化	

				// 开时钟：bit11
				SYSCTL_MODEN1 |= APB_EN;
				//初始化
				//

				// 开时钟：bit12
				SYSCTL_MODEN1 |= ESPI_EN;
				//初始化
				*((unsigned char *)0x33000) = 0xff;
				printf("*0x33000:%x\n", *((unsigned char *)0x33000));

				// // 开时钟：bit13
				// //SYSCTL_MODEN1 |= IRAM1_EN; 
				// //初始化
				// *((unsigned char *)0x34000) =0xff;
				// printf("*0x34000:%x\n",*((unsigned char *)0x34000));	

				SYSCTL_MODEN0 |= SMB8_EN;
				//初始化
				*((unsigned char *)0x4E00) = 0xff;
				printf("*0x4E00:%x\n", *((unsigned char *)0x4E00));

				SYSCTL_MODEN0 |= SMB7_EN;
				//初始化
				*((unsigned char *)0x4D00) = 0xff;
				printf("*0x4D00:%x\n", *((unsigned char *)0x4D00));

				SYSCTL_MODEN0 |= SMB6_EN;
				//初始化
				*((unsigned char *)0x4700) = 0xff;
				printf("*0x4700:%x\n", *((unsigned char *)0x4700));

				// 开时钟：bit14
				SYSCTL_MODEN1 |= SMB5_EN;
				//初始化
				*((unsigned char *)0x4600) = 0xff;
				printf("*0x3E00:%x\n", *((unsigned char *)0x4600));

				// 开时钟：bit15
				SYSCTL_MODEN1 |= SMB4_EN;
				//初始化	
				*((unsigned char *)0x4500) = 0xff;
				printf("*0x3D00:%x\n", *((unsigned char *)0x4500));

				// 开时钟：bit16
				SYSCTL_MODEN1 |= CEC_EN;
				//初始化
				//cec0
				*((unsigned char *)0x4400) = 0xff;
				printf("*0x4400:%x\n", *((unsigned char *)0x4400));
				//cec1
				*((unsigned char *)0x4480) = 0xff;
				printf("*0x4480:%x\n", *((unsigned char *)0x4480));

				//初始化

				// 开时钟：bit18
				//SYSCTL_MODEN1 |= IVT_EN; 
				//初始化
				*((unsigned char *)0x30800) = 0xff;
				printf("*0x30800:%x\n", *((unsigned char *)0x30800));

				//SYSCTL_MODEN1 |= DMA_EN; 
				//初始化
				*((unsigned char *)0x30C00) = 0xff;
				printf("*0x30C00:%x\n", *((unsigned char *)0x30C00));

				//SYSCTL_MODEN1 |= SPIFE_EMB_EN; 
				//初始化
				*((unsigned char *)0x33400) = 0xff;
				printf("*0x33400:%x\n", *((unsigned char *)0x33400));

				//SYSCTL_MODEN1 |= MAIL_BOX_EN; 
				//初始化
				*((unsigned char *)0x32000) = 0xff;
				printf("*0x32000:%x\n", *((unsigned char *)0x32000));

				//SYSCTL
				//初始化
				*((unsigned char *)0x30400) = 0xff;
				printf("*0x30400:%x\n", *((unsigned char *)0x30400));

				// 开时钟：bit19
				SYSCTL_MODEN1 |= CACHE_EN;
				//初始化

				// 开时钟：bit20
				SYSCTL_MODEN1 |= ROM_EN;
				//初始化

				// 开时钟：bit21
				SYSCTL_MODEN1 |= RTC_EN;
				//初始化
				*((unsigned char *)0x3F00) = 0xff;
				printf("*0x3F00:%x\n", *((unsigned char *)0x3F00));

				// 开时钟：bit22
				SYSCTL_MODEN1 |= PECI_EN;
				//初始化
				*((unsigned char *)0x7C00) = 0xff;
				printf("*0x7C00:%x\n", *((unsigned char *)0x7C00));

				// 开时钟：bit23
				//SYSCTL_MODEN1 |= IRAM_EN; 
				//初始化
				*((unsigned char *)0x28000) = 0xff;
				printf("*0x28000:%x\n", *((unsigned char *)0x28000));
				*((unsigned char *)0x34000) = 0xff;
				printf("*0x34000:%x\n", *((unsigned char *)0x34000));

				// 开时钟：bit24
				//SYSCTL_MODEN1 |= I3C_SUB_EN; 
				//初始化

				// 开时钟：bit25
				//SYSCTL_MODEN1 |= I3C_MASTER_EN; 
				//初始化

				// 开时钟：bit26
				//SYSCTL_MODEN1 |= I3C_SLAVE_EN; 
				//初始化


				/*从SYSCTL_MODEN0低位开始*/

				// 开时钟：bit0
				SYSCTL_MODEN0 |= SPIM_EN;
				//初始化
				*((unsigned char *)0x6000) = 0xff;
				printf("*0x6000:%x\n", *((unsigned char *)0x6000));

				// 开时钟：bit7
				SYSCTL_MODEN0 |= UART1_EN;
				//初始化
				*((unsigned char *)0x5C00) = 0xff;
				printf("*0x5C00:%x\n", *((unsigned char *)0x5C00));

				// 开时钟：bit8
				SYSCTL_MODEN0 |= UART0_EN;
				//初始化
				//*((unsigned char *)0x5800) =0xff;
				printf("*05800:%x\n", *((unsigned char *)0x5800));

				// 开时钟：bit9
				SYSCTL_MODEN0 |= UARTB_EN;
				//初始化
				*((unsigned char *)0x5400) = 0xff;
				printf("*0x5400:%x\n", *((unsigned char *)0x5400));

				// 开时钟：bit10
				SYSCTL_MODEN0 |= UARTA_EN;
				//初始化
				*((unsigned char *)0x5000) = 0xff;
				printf("*0x5000:%x\n", *((unsigned char *)0x5000));

				// 开时钟：bit11
				SYSCTL_MODEN0 |= PWM_EN;
				//初始化
				*((unsigned char *)0x4C00) = 0xff;
				printf("*0x4C00:%x\n", *((unsigned char *)0x4C00));

				// 开时钟：bit12
				SYSCTL_MODEN0 |= ADC_EN;
				//初始化
				*((unsigned char *)0x4800) = 0xff;
				printf("*0x4800:%x\n", *((unsigned char *)0x4800));

				// 开时钟：bit13
				//SYSCTL_MODEN1 |= ; 
				//初始化

				// 开时钟：bit14
				//SYSCTL_MODEN0 |= SPIS_EN; 
				//初始化
				*((unsigned char *)0x4000) = 0xff;
				printf("*0x4000:%x\n", *((unsigned char *)0x4000));

				// 开时钟：bit15
				SYSCTL_MODEN0 |= WDT_EN;
				//初始化
				*((unsigned char *)0x3C10) = 0xff;
				printf("*0x3C10:%x\n", *((unsigned char *)0x3C10));

				// 开时钟：bit16
				SYSCTL_MODEN0 |= SMB3_EN;
				//初始化
				*((unsigned char *)0x4300) = 0xff;
				printf("*0x4300:%x\n", *((unsigned char *)0x4300));

				// 开时钟：bit17
				SYSCTL_MODEN0 |= SMB2_EN;
				//初始化
				*((unsigned char *)0x4200) = 0xff;
				printf("*0x4200:%x\n", *((unsigned char *)0x4200));

				// 开时钟：bit18
				SYSCTL_MODEN0 |= SMB1_EN;
				//初始化
				*((unsigned char *)0x4100) = 0xff;
				printf("*0x4100:%x\n", *((unsigned char *)0x4100));

				// 开时钟：bit19
				SYSCTL_MODEN0 |= SMB0_EN;
				//初始化
				*((unsigned char *)0x4000) = 0xff;
				printf("*0x4000:%x\n", *((unsigned char *)0x4000));

				// 开时钟：bit20
				SYSCTL_MODEN0 |= GPIO_EN;
				//初始化
				*((unsigned char *)0x2800) = 0xff;
				printf("*0x2800:%x\n", *((unsigned char *)0x2800));
				*((unsigned char *)0x2C00) = 0xff;
				printf("*0x2C00:%x\n", *((unsigned char *)0x2C00));
				*((unsigned char *)0x3000) = 0xff;
				printf("*0x3000:%x\n", *((unsigned char *)0x3000));
				*((unsigned char *)0x3400) = 0xff;
				printf("*0x3400:%x\n", *((unsigned char *)0x3400));

				// 开时钟：bit21
				SYSCTL_MODEN0 |= BRAM_EN;
				//初始化
				*((unsigned char *)0x2700) = 0xff;
				printf("*0x2700:%x\n", *((unsigned char *)0x2700));

				// 开时钟：bit22
				SYSCTL_MODEN0 |= SWUC_EN;
				//初始化
				*((unsigned char *)0x2600) = 0xff;
				printf("*0x2600:%x\n", *((unsigned char *)0x2600));

				// 开时钟：bit23
				SYSCTL_MODEN0 |= SHM_EN;
				//初始化
				*((unsigned char *)0x2500) = 0xff;
				printf("*0x2500:%x\n", *((unsigned char *)0x2500));

				// 开时钟：bit24
				SYSCTL_MODEN0 |= PMCKBC_EN;
				//初始化
				//pmc1-5
				*((unsigned char *)0x2400) = 0xff;
				printf("*0x2400:%x\n", *((unsigned char *)0x2400));
				*((unsigned char *)0x2410) = 0xff;
				printf("*0x2410:%x\n", *((unsigned char *)0x2410));
				*((unsigned char *)0x2420) = 0xff;
				printf("*0x2420:%x\n", *((unsigned char *)0x2420));
				*((unsigned char *)0x2430) = 0xff;
				printf("*0x2430:%x\n", *((unsigned char *)0x2430));
				*((unsigned char *)0x2440) = 0xff;
				printf("*0x2440:%x\n", *((unsigned char *)0x2440));
				//kbc
				*((unsigned char *)0x2450) = 0xff;
				printf("*0x2450:%x\n", *((unsigned char *)0x2450));
				// 开时钟：bit25
				SYSCTL_MODEN0 |= ROMP_EN;
				//初始化
				*((unsigned char *)0x2200) = 0xff;
				printf("*0x2200:%x\n", *((unsigned char *)0x2200));

				// 开时钟：bit26
				SYSCTL_MODEN0 |= PS2_0_EN;
				//初始化				
				*((unsigned char *)0x2000) = 0xff;
				printf("*0x2000:%x\n", *((unsigned char *)0x2000));

				// 开时钟：bit27
				SYSCTL_MODEN0 |= PS2_1_EN;
				//初始化				
				*((unsigned char *)0x2100) = 0xff;
				printf("*0x2100:%x\n", *((unsigned char *)0x2100));

				// 开时钟：bit28
				SYSCTL_MODEN0 |= KBS_EN;
				//初始化				
				*((unsigned char *)0x1C00) = 0xff;
				printf("*1C00:%x\n", *((unsigned char *)0x1C00));

				// 开时钟：bit29
				SYSCTL_MODEN0 |= TMR3_EN;
				//初始化				
				*((unsigned char *)(0x1800 + 0x14 * 3)) = 0xff;
				printf("*0x1800 + 0x14 *3:%x\n", *((unsigned char *)(0x1800 + 0x14 * 3)));

				// 开时钟：bit30
				SYSCTL_MODEN0 |= TMR2_EN;
				//初始化	
				*((unsigned char *)(0x1800 + 0x14 * 2)) = 0xff;
				printf("*0x1800 + 0x14 *2:%x\n", *((unsigned char *)(0x1800 + 0x14 * 2)));

				// 开时钟：bit31
				SYSCTL_MODEN0 |= TMR1_EN;
				//初始化
				*((unsigned char *)(0x1800 + 0x14 * 1)) = 0xff;
				printf("*0x1800 + 0x14 *1:%x\n", *((unsigned char *)(0x1800 + 0x14 * 1)));

				SYSCTL_MODEN1 |= TMR0_EN;
				TIMER0_TLC0 = 0xff;
				printf("TIMER0_TLC0:%x\n", TIMER0_TLC0);

				printf("SYSCTL_MODEN1:%x\n", SYSCTL_MODEN1);

				printf("start module reset\n");
				//复位
				SYSCTL_RST0 |= 0xfffffeff;
				SYSCTL_RST1 |= 0xff802fff; //bit16:chip使芯片全复位,bit12,spif_emb在用会卡住

				printf("TIMER0_TLC0:%x\n", TIMER0_TLC0);
				printf("ICTL0_INTEN0:%x\n", ICTL0_INTEN0);
				printf("ICTL1_INTEN0:%x\n", ICTL1_INTEN0);
				printf("*0x30004(spif):%x\n", *((unsigned char *)0x30004));
				printf("*0x31000:%x\n", *((unsigned char *)0x31000));
				printf("GPIO1_DR0:%x\n", GPIO1_DR0);
				printf("*0x32000:%x\n", *((unsigned char *)0x32000));
				printf("*0x32400:%x\n", *((unsigned char *)0x32400));
				printf("*0x30400(sysctl):%x\n", *((unsigned char *)0x30400));
				printf("*0x33000:%x\n", *((unsigned char *)0x33000));
				printf("*0x4E00:%x\n", *((unsigned char *)0x4E00));
				printf("*0x4D00:%x\n", *((unsigned char *)0x4D00));
				printf("*0x4700:%x\n", *((unsigned char *)0x4700));
				printf("*0x3E00:%x\n", *((unsigned char *)0x4600));
				printf("*0x3D00:%x\n", *((unsigned char *)0x4500));
				printf("*0x4400:%x\n", *((unsigned char *)0x4400));
				printf("*0x4300:%x\n", *((unsigned char *)0x4300));
				printf("*0x4200:%x\n", *((unsigned char *)0x4200));
				printf("*0x4100:%x\n", *((unsigned char *)0x4100));
				printf("*0x4000:%x\n", *((unsigned char *)0x4000));
				printf("*0x4480:%x\n", *((unsigned char *)0x4480));
				printf("*0x30800(IVT):%x\n", *((unsigned char *)0x30800));
				printf("*0x30C00:%x\n", *((unsigned char *)0x30C00));
				printf("*0x33400:%x\n", *((unsigned char *)0x33400));
				printf("*0x3F00:%x\n", *((unsigned char *)0x3F00));
				printf("*0x7C00:%x\n", *((unsigned char *)0x7C00));
				// printf("*0x28000(RAM0):%x\n",*((unsigned char *)0x28000));
				// printf("*0x34000(RAM1):%x\n",*((unsigned char *)0x34000));
				printf("*0x7800:%x\n", *((unsigned char *)0x7800));
				printf("*0x5C00:%x\n", *((unsigned char *)0x5C00));
				printf("*05800:%x\n", *((unsigned char *)0x5800));
				printf("*0x5400:%x\n", *((unsigned char *)0x5400));
				printf("*0x5000:%x\n", *((unsigned char *)0x5000));
				printf("*0x4C00:%x\n", *((unsigned char *)0x4C00));
				printf("*0x4800:%x\n", *((unsigned char *)0x4800));
				printf("*0x4000:%x\n", *((unsigned char *)0x4000));
				printf("*0x3C10:%x\n", *((unsigned char *)0x3C10));
				printf("*0x3B00:%x\n", *((unsigned char *)0x3B00));
				printf("*0x3A00:%x\n", *((unsigned char *)0x3A00));
				printf("*0x3900:%x\n", *((unsigned char *)0x3900));
				printf("*0x3800:%x\n", *((unsigned char *)0x3800));
				printf("*0x2800:%x\n", *((unsigned char *)0x2800));
				printf("*0x2C00:%x\n", *((unsigned char *)0x2C00));
				printf("*0x3000:%x\n", *((unsigned char *)0x3000));
				printf("*0x3400:%x\n", *((unsigned char *)0x3400));
				printf("*0x2700:%x\n", *((unsigned char *)0x2700));
				printf("*0x2600:%x\n", *((unsigned char *)0x2600));
				printf("*0x2500:%x\n", *((unsigned char *)0x2500));
				printf("*0x2400:%x\n", *((unsigned char *)0x2400));
				printf("*0x2410:%x\n", *((unsigned char *)0x2410));
				printf("*0x2420:%x\n", *((unsigned char *)0x2420));
				printf("*0x2430:%x\n", *((unsigned char *)0x2430));
				printf("*0x2440:%x\n", *((unsigned char *)0x2440));
				printf("*0x2450:%x\n", *((unsigned char *)0x2450));
				printf("*0x2200(rompatch):%x\n", *((unsigned char *)0x2200));
				printf("*2000:%x\n", *((unsigned char *)0x2000));
				printf("*2100:%x\n", *((unsigned char *)0x2100));
				printf("*1C00:%x\n", *((unsigned char *)0x1C00));
				printf("*0x1800 + 0x14 *3:%x\n", *((unsigned char *)(0x1800 + 0x14 * 3)));
				printf("*0x1800 + 0x14 *2:%x\n", *((unsigned char *)(0x1800 + 0x14 * 2)));
				printf("*0x1800 + 0x14 *1:%x\n", *((unsigned char *)(0x1800 + 0x14 * 1)));
				/***************************************************************************/
							//以下程序不执行了，定时器被复位了
			}
			if (Mtimer_Cunt >= 5000)
			{
				dprint("Enter test module via serial, or auto-advance soon.\n");
				do
				{
					REG8(Uart_base);
					REG8(Uart_base);
					REG8(Uart_base);
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("Proceeding to test step %d\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;     //最后一个模块不使得Prt_flag = 0则不会测试下一个模块
			}
			break;

		case 33:
			if (Prt_flag == 0)
			{
				dprint("perpare test irq enable disable mask,clock disable,TMR as example\n");
				dprint("completing the jumper, then send any key to start test\n");
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				//定时器
				TIMER_Init(TIMER0, TIMER0_1ms, 0x1, 0x0); //都是100ms打印
				TIMER_Init(TIMER1, TIMER1_1ms, 0x1, 0x0);
				TIMER_Init(TIMER3, TIMER3_1ms, 0x1, 0x0);

				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
				dprint("test irq start\n");
			}
			Mtimer_Cunt++;

			if (Mtimer_Cunt == 5000)
			{
				TIMER_Init(TIMER3, TIMER3_1ms, 0x1, 0x1);
				TIMER_Init(TIMER1, TIMER1_1ms, 0x1, 0x1);
				TIMER_Init(TIMER0, TIMER0_1ms, 0x1, 0x1); //打开屏蔽

				printf("open mask\n");
			}
			if (Mtimer_Cunt == 10000)
			{
				TIMER_Init(TIMER3, TIMER3_1ms, 0x1, 0x0);
				TIMER_Init(TIMER1, TIMER1_1ms, 0x1, 0x0);
				TIMER_Init(TIMER0, TIMER0_1ms, 0x1, 0x0);//关闭屏蔽
				printf("close mask\n");
			}

			if (Mtimer_Cunt == 15000)
			{
				TIMER_REG(0x14 * 0 + TIMER_TCR_OFFSET) &= ~(0x1 << 0);
				TIMER_REG(0x14 * 1 + TIMER_TCR_OFFSET) &= ~(0x1 << 0);
				TIMER_REG(0x14 * 3 + TIMER_TCR_OFFSET) &= ~(0x1 << 0);//disable
				printf("disable irq\n");
			}

			if (Mtimer_Cunt == 30000)
			{
				TIMER_Init(TIMER3, TIMER3_1ms, 0x1, 0x0);
				TIMER_Init(TIMER1, TIMER1_1ms, 0x1, 0x0);
				TIMER_Init(TIMER0, TIMER0_1ms, 0x1, 0x0); //enable
				printf("enable irq\n");
			}
			if (Mtimer_Cunt == 35000)
			{

				SYSCTL_MODEN1 &= (~TMR0_EN);
				SYSCTL_MODEN0 &= (~TMR1_EN);
				SYSCTL_MODEN0 &= (~TMR3_EN);
				printf("disable clock\n");
				printf("SYSCTL_MODEN0:%x\n", SYSCTL_MODEN0);
				printf("SYSCTL_MODEN1:%x\n", SYSCTL_MODEN1);
			}
			if (Mtimer_Cunt >= 40000)
			{
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("test irq enable disable mask over\n");
			}
			break;

		case 34:
			if (Prt_flag == 0)//中断源触发中断
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN1 &= (~ICTL_EN);
				}
				dprint("perpare test irq enable disable mask,clock disable,ICTL as example\n");
				dprint("completing the jumper, then send any key to start test\n");
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));

				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
				dprint("test irq start\n");

				//ICTL0通道0
				sysctl_iomux_config(GPIOA, 0, 0); //GPIOA[0]
				GPIO0_DDR0 &= ~(0x1 << 0); //输入
				GPIO0_INTTYPE_LEVEL0 |= (0x1 << 0); //边沿触发
				GPIO0_INT_POLARITY0 &= ~(0x1 << 0);  //下降沿
				GPIO0_DEBOUNCE0 |= (0x1 << 0);//消抖
				GPIO0_INTEN0 |= (0x1 << 0); //作为中断
				ICTL0_INTEN0 |= (0x1 << 0); //使能控制器通道0中断
				//ICTL0通道63
				sysctl_iomux_config(GPIOB, 30, 0); //GPIOB[30]
				GPIO1_DR3 &= ~(0x1 << 6);//输入
				GPIO1_INTTYPE_LEVEL3 |= (0x1 << 6); //边沿触发
				GPIO1_INT_POLARITY3 &= ~(0x1 << 6);  //下降沿
				GPIO1_DEBOUNCE3 |= (0x1 << 6);//消抖
				GPIO1_INTEN3 |= (0x1 << 6); //作为中断
				ICTL0_INTEN7 |= (0x1 << 6); //使能控制器通道0中断	
				ICTL0_INTMASK7 &= (~(0x1 << 6));//打开63通道中断	

				//ICTL1通道0	//  0-15  //
				sysctl_iomux_config(GPIOC, 0, 0); //GPIOC[0]
				GPIO2_DDR0 &= ~(0x1 << 0); //输入
				GPIO2_INTTYPE_LEVEL0 |= (0x1 << 0); //边沿触发
				GPIO2_INT_POLARITY0 &= ~(0x1 << 0);  //下降沿
				//GPIO0_DEBOUNCE0 |=(0x1<<0);//消抖
				GPIO2_INTEN0 |= (0x1 << 0); //作为中断
				ICTL1_INTEN0 |= (0x1 << 0); //使能控制器通道0中断					

				//ICTL1通道0	//  0-15  //
				sysctl_iomux_config(GPIOC, 15, 0); //GPIOC[0]
				GPIO2_DDR1 &= ~(0x1 << 7); //输入
				GPIO2_INTTYPE_LEVEL1 |= (0x1 << 7); //边沿触发
				GPIO2_INT_POLARITY1 &= ~(0x1 << 7);  //下降沿
				//GPIO0_DEBOUNCE0 |=(0x1<<0);//消抖
				GPIO2_INTEN1 |= (0x1 << 7); //作为中断
				ICTL1_INTEN1 |= (0x1 << 7); //使能控制器通道0中断	

				//ICTL1通道23	//  16-23  //
				sysctl_iomux_config(GPIOE, 23, 0); //GPIOE[23]
				printf("0x30468:%x\n", *(unsigned int *)(0x30468));
				GPIO3_DDR2 &= ~(0x1 << 7); //输入
				GPIO3_INTTYPE_LEVEL2 |= (0x1 << 7); //边沿触发
				GPIO3_INT_POLARITY2 &= ~(0x1 << 7);  //下降沿
				//GPIO0_DEBOUNCE0 |=(0x1<<0);//消抖
				GPIO3_INTMASK2 &= ~(0x1 << 7);
				GPIO3_INTEN2 |= (0x1 << 7); //作为中断
				ICTL1_INTEN2 |= (0x1 << 7); //使能控制器通道23中断	
				ICTL1_INTMASK2 &= (~(0x1 << 7));
				printf("start\n");
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt == 10000)
			{							  //mask
				printf("mask\n");
				ICTL0_INTMASK0 |= (0x1 << 0);//屏蔽0通道中断
				ICTL0_INTMASK7 |= (0x1 << 6);//屏蔽63通道中断
				ICTL1_INTMASK0 |= (0x1 << 0);//屏蔽0通道中断
			}
			if (Mtimer_Cunt == 15000)
			{
				printf("unsmask\n");					//unsmask
				ICTL0_INTMASK0 &= (~(0x1 << 0));//打开0通道中断
				ICTL0_INTMASK7 &= (~(0x1 << 6));//打开63通道中断
				ICTL1_INTMASK0 &= (~(0x1 << 0));//打开0通道中断
			}
			if (Mtimer_Cunt == 20000)
			{
				printf("disable\n");						//disable
				ICTL0_INTEN0 &= (~(0x1 << 0)); //控制器通道0中断
				ICTL0_INTEN7 &= (~(0x1 << 6));
				ICTL1_INTEN0 &= (~(0x1 << 0));
			}
			if (Mtimer_Cunt >= 25000)
			{
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("test irq enable disable mask over\n");
			}
			break;

		case 35:
			if (Prt_flag == 0)//强制触发中断 测试中的屏蔽
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN1 &= (~ICTL_EN);
				}
				dprint("perpare test irq force irq\n");
				dprint("completing the jumper, then send any key to start test\n");
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
				dprint("test irq start\n");

				//ICTL0
				ICTL0_INTEN0 |= 0xff; //使能中断 通道1-7
				ICTL0_INTFORCE0 |= 0xff;//强制中断 通道0-7
				ICTL0_INTMASK0 &= ~(0xff);//不屏蔽 通道0-7 

				ICTL0_INTEN1 |= 0xff; //使能中断 通道8-15
				ICTL0_INTFORCE1 |= 0xff;//强制中断 通道8-15
				ICTL0_INTMASK1 &= ~(0xff);//不屏蔽 通道8-15

				ICTL0_INTEN2 |= 0xff; //使能中断 通道16-23
				ICTL0_INTFORCE2 |= 0xff;//强制中断 通道16-23
				ICTL0_INTMASK2 &= ~(0xff);//不屏蔽 通道16-23		

				ICTL0_INTEN3 |= 0xff; //使能中断 通道24-31
				ICTL0_INTFORCE3 |= 0xff;//强制中断 通道24-31
				ICTL0_INTMASK3 &= ~(0xff);//不屏蔽 通道24-31

				ICTL0_INTEN4 |= 0xff; //使能中断 通道32-39
				ICTL0_INTFORCE4 |= 0xff;//强制中断 通道32-39
				ICTL0_INTMASK4 &= ~(0xff);//不屏蔽 通道32-39

				ICTL0_INTEN5 |= 0xff; //使能中断 通道40-47
				ICTL0_INTFORCE5 |= 0xff;//强制中断 通道40-47
				ICTL0_INTMASK5 &= ~(0xff);//不屏蔽 通道40-47

				ICTL0_INTEN6 |= 0xff; //使能中断 通道48-55
				ICTL0_INTFORCE6 |= 0xff;//强制中断 通道48-55
				ICTL0_INTMASK6 &= ~(0xff);//不屏蔽 通道48-55		

				ICTL0_INTEN7 |= 0xff; //使能中断 通道56-63  //上边代码测试通道62
				ICTL0_INTFORCE7 |= 0xff;//强制中断 通道56-63
				ICTL0_INTMASK7 &= ~(0xff);//不屏蔽 通道56-63		


				ICTL1_INTEN0 |= 0xff; //使能中断 通道1-7
				ICTL1_INTEN1 |= 0xff; //使能中断 通道8-15
				ICTL1_INTEN2 |= 0xff; //使能中断 通道16-23

				ICTL1_INTEN3 |= 0xbf; //使能中断 通道24-31	
				ICTL1_INTEN4 |= 0xcf; //使能中断 通道32-39					
				ICTL1_INTEN5 |= 0xff; //使能中断 通道40-47
				ICTL1_INTEN6 |= 0x7c; //使能中断 通道48-55
				ICTL1_INTEN7 |= 0x73; //使能中断 通道56-63  

				ICTL1_INTFORCE0 |= 0xff;//强制中断 通道0-7
				ICTL1_INTFORCE1 |= 0xff;//强制中断 通道8-15
				ICTL1_INTFORCE2 |= 0xff;//强制中断 通道16-23

				ICTL1_INTFORCE3 |= 0xbf;//强制中断 通道24-31
				ICTL1_INTFORCE4 |= 0xcf;//强制中断 通道32-39
				ICTL1_INTFORCE5 |= 0xff;//强制中断 通道40-47
				ICTL1_INTFORCE6 |= 0x7c;//强制中断 通道48-55
				ICTL1_INTFORCE7 |= 0x73;//强制中断 通道56-63

				ICTL1_INTMASK0 &= ~(0xff);//不屏蔽 通道0-7 
				ICTL1_INTMASK1 &= ~(0xff);//不屏蔽 通道8-15
				ICTL1_INTMASK2 &= ~(0xff);//不屏蔽 通道16-23	

				ICTL1_INTMASK3 &= ~(0xbf);//不屏蔽 通道24-31	
				ICTL1_INTMASK4 &= ~(0xcf);//不屏蔽 通道32-39	
				ICTL1_INTMASK5 &= ~(0xff);//不屏蔽 通道40-47
				ICTL1_INTMASK6 &= ~(0x7c);//不屏蔽 通道48-55
				ICTL1_INTMASK7 &= ~(0x73);//不屏蔽 通道56-63		
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt >= 5000)
			{
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("test irq enable disable mask over\n");
			}

			break;

		case 36:
			if (Prt_flag == 0)//RTC测试
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN1 &= (~RTC_EN);
				}
				dprint("perpare test RTC\n");
				dprint("completing the jumper, then send any key to start test\n");
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
				dprint("test RTC start\n");
				RTC_Init(0, 1, LOW_CHIP_CLOCK);
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt == 5000)//mask
			{
				printf("mask\n");
				RTC_CCR0 |= RTC_CCR_MASK;
			}
			if (Mtimer_Cunt == 10000)//unmasks
			{
				printf("unmask\n");
				RTC_CCR0 &= (~RTC_CCR_MASK);
			}
			if (Mtimer_Cunt == 15000)//disable
			{
				printf("disable\n");
				RTC_CCR0 &= (~RTC_CCR_EN);
			}
			if (Mtimer_Cunt >= 20000)
			{
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("test RTC over\n");
			}
			break;

		case 37:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN1 &= (~SMB6_EN);
				}
				sysctl_iomux_i2c6();
				I2C_Channel_Init(I2C_CHANNEL_6, I2C7_SPEED, I2C_MASTER_ROLE, 0x4c, 1);
				dprint("perpare SMBUS6\n");
				dprint("completing the jumper, then send any key to start test\n");
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
				dprint("SMBUS6 start\n");
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt % 1000 == 0)
			{
				Get_Temperature(6);
			}
			if (Mtimer_Cunt >= 10000)
			{
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("SMBUS6 over\n");
			}
			break;

		case 38:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN1 &= (~SMB7_EN);
				}
				sysctl_iomux_i2c7();
				I2C_Channel_Init(I2C_CHANNEL_7, I2C7_SPEED, I2C_MASTER_ROLE, 0x4c, 1);
				dprint("perpare SMBUS7\n");
				dprint("completing the jumper, then send any key to start test\n");
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
				dprint("SMBUS7 start\n");
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt % 1000 == 0)
			{
				Get_Temperature(7);
			}
			if (Mtimer_Cunt >= 10000)
			{
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("SMBUS7 over\n");
			}
			break;

		case 39:
			if (Prt_flag == 0)
			{
				if (Test_Clock_Disable)
				{
					SYSCTL_MODEN1 &= (~SMB8_EN);
				}
				sysctl_iomux_i2c8();
				I2C_Channel_Init(I2C_CHANNEL_8, I2C8_SPEED, I2C_MASTER_ROLE, 0x4c, 1);
				dprint("perpare SMBUS8\n");
				dprint("completing the jumper, then send any key to start test\n");
				Uart_Int_Disable(PRINTF_UART_SWITCH, 0);
				REG8(Uart_base);
				REG8(Uart_base);
				REG8(Uart_base);
				while (!(REG8(Uart_base + 0x5) & 0x1));
				temp_data = REG8(Uart_base);
				dprint("input 0x%x\n", temp_data);
				Prt_flag = 1;
				dprint("SMBUS8 start\n");
			}
			Mtimer_Cunt++;
			if (Mtimer_Cunt % 1000 == 0)
			{
				Get_Temperature(8);
			}
			if (Mtimer_Cunt >= 10000)
			{
				REG8(Uart_base);     //清除掉打印串口接收FIFO
				REG8(Uart_base);
				REG8(Uart_base);
				dprint("send the number of test module, otherwise enter next module\n");
				do
				{
					timercunt++;
					if (REG8(Uart_base + 0x5) & 0x1)
					{
						temp_data = REG8(Uart_base);
						MCtrStep = temp_data;
						Mtimer_Cunt = 0;
						Prt_flag = 0;
						dprint("start %d module test\n", MCtrStep);
						return 1;
					}
				}
				while (timercunt < 0xffffff);
				MCtrStep += 1;
				Mtimer_Cunt = 0;
				Prt_flag = 0;
				dprint("SMBUS8 over\n");
			}
			break;
		default:
			MCtrStep = 0;
			Mtimer_Cunt = 0;
			Prt_flag = 0;
			break;

	}
	return 0;
}

#if TEST
//PMC1 or PMC2 compatible mode sci test config
void SCI_Compatible_HW_Config(uint8_t PMC_Channel)
{
	if (PMC_Channel == 1)
	{
		KBC_SIRQCTL |= (0X0 << 6);  //IRQNP SET 0
		KBC_CTL |= 0x10;  //PMC12HIE SET 1
		PMC1_IE |= 0x1a;  //Enable SCIEN & HWSCIEN
		PMC1_IE &= 0xdb;  //Disable SMI
		PMC2_IE &= 0x0;   //Set PMC2 interrupt disable
		PMC1_DOR = 0xab;    //write data to PMC1DOR (provide OBF)
		PMC1_CTL |= 0x2;   //enable PMC1OBE interrupt
		//*((volatile uint8_t *)(0x2406)) &= ~(0x40) ;   //SCINP Set 0
		dprint("0x2406 data second is %#x\n", *((volatile uint8_t *)(0x2406)));
		dprint("0x2452 data second is %#x\n", *((volatile uint8_t *)(0x2452)));
	}
	else if (PMC_Channel == 2)
	{
		KBC_CTL |= 0x10;  //PMC12HIE SET 1
		KBC_SIRQCTL |= (0X0 << 6);  //IRQNP SET 0
		KBC_SIRQCTL |= 0X10;  //IRQM SET 010
		PMC2_IE |= 0x1a;  //Set PMC2 interrupt enable
		PMC2_IE &= 0xdb;  //Set PMC2 interrupt disable
		PMC1_IE &= 0x0;   //Set PMC1 interrupt disable
		PMC2_DOR = 0xab;    //write data to PMC1DOR (provide OBF)
		PMC2_CTL |= 0x2;   //enable PMC2OBE interrupt
		//PMC2_CTL &= ~(0x40) ;   //SCINP Set 0  (Reset value is 1)
		dprint("0x2416 data second is %#x\n", *((volatile uint8_t *)(0x2416)));
	}
}
//PMC1 or PMC2 compatible mode sci test config & software trig IRQ
void SCI_Compatible_SW_Config(uint8_t PMC_Channel)
{
	if (PMC_Channel == 1)
	{
		//PMC1_CTL |= 0x40 ;    //SCINP Set 1
		PMC1_CTL &= 0xbf;      //SCINP Set 0
		KBC_CTL &= 0xef;      //PMC12HIE Set 0
		//*((volatile uint8_t *)(0x2452)) |= 0x04 ;    //IRQ11B Set 1
		KBC_SIRQCTL &= (~0x04);   //IRQ11B Set 0
		PMC1_IE |= 0x2;       //SCIEN	
		//PMC2_IE&= 0xe6 ;
		PMC2_IE &= 0x0;   //Set PMC2 interrupt disable
		for (BYTE i = 0; i < 10; i++)
		{
			KBC_SIRQCTL &= (~0x04);  //IRQ11B Set 0
			vDelayXms(10);
			KBC_SIRQCTL |= 0x04;    //IRQ11B Set 1
			vDelayXms(10);
		}
	}
	else if (PMC_Channel == 2)
	{
		//PMC2_CTL |= 0x40 ;    //SCINP Set 1
		PMC2_CTL &= 0xbf;      //SCINP Set 0
		KBC_CTL &= 0xef;      //PMC12HIE Set 0
		//KBC_SIRQCTL |= 0x04 ;    //IRQ11B Set 1
		KBC_SIRQCTL &= (~0x04);   //IRQ11B Set 0
		PMC2_IE |= 0x2;       //SCIEN	
		//PMC2_IE &= 0xe6 ;
		PMC1_IE &= 0x0;   //Set PMC1 interrupt disable
		for (BYTE i = 0; i < 10; i++)
		{
			KBC_SIRQCTL &= (~0x04);    //IRQ11B Set 0
			vDelayXms(10);
			KBC_SIRQCTL |= 0x04;    //IRQ11B Set 1
			vDelayXms(10);
		}
	}
}
//PMC1 or PMC2 enhance mode sci test config 
//(Set OBF For SCI) or (Set IBF For SCI)
void SCI_enhance_HW_Config(uint8_t PMC_Channel, uint8_t type)
{
	if (PMC_Channel == 1)
	{
		if (type == OBF)
		{
			PMC1_IE |= 0x12;     //HWSCI:1 SCIEN:1
			PMC1_CTL |= 0x80;      //APM:1  SCIPM:000
			//PMC1_CTL &= (~0x40);   //SCINP:0  default value:1
			PMC1_DOSCI = 0xfb;       //write data to DOSCI & SET OBF
		}
		else
		{
			PMC1_CTL |= 0x80;      //APM:1  SCIPM:000
			PMC1_CTL &= (~0x40);   //SCINP:0  default value:1
			PMC1_IE |= 0x12;     //HWSCI:1 SCIEN:1
			BYTE  DISCI_Data = 0;
			DISCI_Data = PMC1_DISCI; //read data to DISCI
		}
	}
	else
	{
		if (type == OBF)
		{
			PMC2_IE |= 0x12;     //HWSCI:1 SCIEN:1
			//PMC2_CTL |= 0x98;      //APM:1  SCIPM:011
			//PMC2_CTL &= (~0x40);   //SCINP:0  default value:1
			PMC2_DOSCI = 0xfb;       //write data to DOSCI & SET OBF
		}
		else
		{
			PMC2_CTL |= 0x80;      //APM:1  SCIPM:000
			PMC2_CTL &= (~0x40);   //SCINP:0  default value:1
			PMC2_IE |= 0x12;     //HWSCI:1 SCIEN:1
			BYTE  DISCI_Data = 0;
			DISCI_Data = PMC2_DISCI; //read data to DISCI
		}
	}
}
void SCI_enhance_SW_Config(uint8_t PMC_Channel)
{
	if (PMC_Channel == 1)
	{
		PMC1_CTL |= 0x80;      //APM:1  SCIPM:000
		PMC1_CTL &= (~0x40);   //SCINP:0  default value:1
		PMC1_IE &= 0xef;      //HWSCI:0
		PMC1_IE |= 0x02;     //SCIEN:1
		for (BYTE i = 0; i < 100; i++)
		{
			PMC1_IC |= 0x04;      //SCISB:1
			vDelayXms(10);
			PMC1_IC &= (~0x04);    //SCISB:0
			vDelayXms(10);
		}
	}
	else if (PMC_Channel == 2)
	{
		PMC2_CTL |= 0x80;      //APM:1  SCIPM:000
		PMC2_CTL &= (~0x40);   //SCINP:0  default value:1
		PMC2_IE &= 0xef;      //HWSCI:0
		PMC2_IE |= 0x02;     //SCIEN:1
		for (BYTE i = 0; i < 100; i++)
		{
			PMC2_IC |= 0x04;      //SCISB:1
			vDelayXms(10);
			PMC2_IC &= (~0x04);    //SCISB:0
			vDelayXms(10);
		}
	}
}
void SCI_test(void)
{
#if SCI_Test
	GPIO0_REG(GPIO_INPUT_VAL) |= ((0x0) << 0);//GPIO:0
	sysctl_iomux_config(GPIOA, 27, 1);	//set GPD3 as SCI
	GPIO0_REG(GPIO_INPUT_VAL) |= ((0x1) << 0);//GPIO:1
	//PMC1 compatible mode sci test config
#if 0
	SCI_Compatible_HW_Config(1);
#endif
	//PMC1 compatible mode sci test config & software trig IRQ
#if 0
	SCI_Compatible_SW_Config(1);
#endif
	//PMC2 compatible mode sci test config(HW)
#if 0
	SCI_Compatible_HW_Config(2);
#endif
	//PMC2 compatible mode sci test config & software trig IRQ
#if 0
	SCI_Compatible_SW_Config(2);
#endif
	//PMC1 enhance mode sci test config (Set OBF For SCI)
#if 0
	SCI_enhance_HW_Config(1, OBF);
#endif
	//PMC1 enhance mode sci test config (Set IBF For SCI)
#if 0
	SCI_enhance_HW_Config(1, IBF);
#endif
	//PMC1 enhance mode sci test config (Software)
#if 0
	SCI_enhance_SW_Config(1);
#endif
	//PMC2 enhance mode sci test config (Set OBF For SCI)
#if 0
	SCI_enhance_HW_Config(2, OBF);
#endif
	//PMC2 enhance mode sci test config (Set IBF For SCI)
#if 0
	SCI_enhance_HW_Config(2, IBF);
#endif
	//PMC2 enhance mode sci test config (Software)
#if 0
	SCI_enhance_SW_Config(2);
#endif
#endif
}

void SMI_test(void)
{
#if SMI_Test
	GPIO0_REG(GPIO_INPUT_VAL) |= ((0x0) << 0);//GPIO:0
	sysctl_iomux_config(GPIOA, 28, 1);       //set GPD4 as SMI
	GPIO0_REG(GPIO_INPUT_VAL) |= ((0x1) << 0);//GPIO:0
#if 0
	//PMC1 compatible mode smi test config(HW)
	KBC_CTL |= 0x10;  //PMC12HIE SET 1
	//KBC_SIRQCTL |= 0X08 ;  //IRQM SET 001    default value:000
	KBC_SIRQCTL |= (0X0 << 6);  //IRQNP SET 0
	PMC1_IE |= 0x04;  //SMIEN:1
	PMC2_IE &= 0x0;   //Set PMC2 interrupt disable
	PMC1_DOR = 0xab;    //write data to PMC1DOR(OBF)
	//PMC1_IC &= (~0x40) ;   //SMINP:0 default value:1 
#endif
#if 0
//PMC1 compatible mode smi test config(SW)
	KBC_CTL &= 0xef;  //PMC12HIE SET 0
	PMC1_IE |= 0x04;  //SMIEN:1
	PMC1_IC &= (~0x40);   //SMINP:0 default value:1
	PMC2_IE &= 0x0;   //Set PMC2 interrupt disable
	for (BYTE i = 0; i < 10; i++)
	{
		KBC_SIRQCTL &= (~0x04);    //IRQ11B Set 0
		vDelayXms(10);
		KBC_SIRQCTL |= 0x04;    //IRQ11B Set 1
		vDelayXms(10);
	}
#endif
#if 0
	//PMC2 compatible mode smi test config(HW)
	KBC_CTL |= 0x10;  //PMC12HIE SET 1
	//KBC_SIRQCTL |= 0X38 ;  //IRQM SET 111    default value:000
	KBC_SIRQCTL |= (0X0 << 6);  //IRQNP SET 0
	PMC2_IE |= 0x04;  //SMIEN:1
	PMC1_IE &= 0x0;   //Set PMC1 interrupt disable
	PMC2_DOR = 0xab;    //write data to PMC1DOR(OBF)
	//PMC2_IC &= (~0x40) ;   //SMINP:0 default value:1 
#endif
#if 0
//PMC2 compatible mode smi test config(SW)
	KBC_CTL &= 0xef;  //PMC12HIE SET 0
	PMC2_IE |= 0x04;  //SMIEN:1
	PMC2_IC &= (~0x40);   //SMINP:0 default value:1
	PMC1_IE &= 0x0;   //Set PMC1 interrupt disable
	for (BYTE i = 0; i < 10; i++)
	{
		KBC_SIRQCTL &= (~0x04);    //IRQ11B Set 0
		vDelayXms(10);
		KBC_SIRQCTL |= 0x04;    //IRQ11B Set 1
		vDelayXms(10);
	}
#endif
#if 0
	//PMC1 Enhance mode smi test config (Set OBF for SMI)
	PMC1_CTL |= 0x80;      //APM:1
	//PMC1_IC |= 0x38;      //SMIPM:111
	//PMC1_IC &= (~0x40) ;   //SMINP:0 default value:1
	PMC1_IE |= 0x24;       //HWSMIEN:1 SMIEN:1 
	PMC1_DOSMI |= 0xfb;       //write data to DOSMI and trig smi
#endif
#if 0
	//PMC1 Enhance mode smi test config (SW)
	//PMC1_IC &= (~0x40) ;   //SMINP:0 default value:1
	PMC1_CTL |= 0x80;      //APM:1
	PMC1_IE &= 0xdf;    //HWSMIEN:0 
	PMC1_IE |= 0x04;       //SMIEN:1 
	PMC2_IE &= 0x0;   //Set PMC2 interrupt disable
	for (BYTE i = 0; i < 10; i++)
	{
		PMC1_IC &= (~0x02);    //SMISB Set 0
		vDelayXms(10);
		PMC1_IC |= 0x02;       //SMISB Set 1
		vDelayXms(10);
	}
#endif
#if 0
	//PMC2 Enhance mode smi test config (Set OBF for SMI)
	PMC2_CTL |= 0x80;      //APM:1
	PMC2_IC |= 0x38;      //SMIPM:111
	//PMC2_IC &= (~0x40) ;   //SMINP:0 default value:1
	PMC2_IE |= 0x24;       //HWSMIEN:1 SMIEN:1 
	PMC2_DOSMI |= 0xfb;       //write data to DOSMI and trig smi
#endif
#if 0
	//PMC2 Enhance mode smi test config (SW)
	PMC2_CTL |= 0x80;      //APM:1
	//PMC2_IC &= (~0x40) ;   //SMINP:0 default value:1
	PMC2_IE &= 0xdf;    //HWSMIEN:0 
	PMC2_IE |= 0x04;       //SMIEN:1 
	PMC1_IE &= 0x0;   //Set PMC1 interrupt disable
	for (BYTE i = 0; i < 10; i++)
	{
		PMC2_IC &= (~0x02);    //SMISB Set 0
		vDelayXms(10);
		PMC2_IC |= 0x02;       //SMISB Set 1
		vDelayXms(10);
	}
#endif
#endif
}

void SWUC_Test(void)
{
#if 0
	SWUC_UART_Test();
#endif
#if 0
	SWUC_RI1_RI2_Test();
#endif
#if 0
	SWUC_Ring_Test();
#endif
#if 0
	SWUC_Soft_Test();
#endif
#if 0
	SWUC_LegacyMode_Test();
#endif
#if 0
	SWUC_ACPI_Init();
#endif
#if 0
	SWUC_PWUREQ_Init();
#endif
#if 0
	SWUC_SMI_Init();
#endif
#if 0
	SWUC_KBRST_Init(1);
#endif
#if 0
	SWUC_GA20_Init(1);
#endif
}
void SYSCTL_PORINT_Test(void)
{
	sysctl_iomux_config(GPIOB, 6, 1);
	SYSCTL_PORINT |= 0x1;
}
#define AUTO_TEST 0
#if AUTO_TEST
void KBC_PORT_TEST(void)
{
	BYTE  var;
	var = KBC_IB;
	while (KBC_STA & 0x1);
#if LPC_WAY_OPTION_SWITCH
#else
	SET_BIT(KBC_CTL, 3);
#endif
	KBC_MOB = var;
}
void pwm_pmc_config(BYTE  dcdr)
{
	spwm_str pwm_set_0;
	pwm_set_0.pwm_no = 0;
	pwm_set_0.prescale = 1;
	pwm_set_0.pwm_polarity = 0;
	pwm_set_0.pwm_softmode = 0;
	pwm_set_0.pwm_scale = 0;
	pwm_set_0.pwm_ctr = 100;
	pwm_set_0.pwm_clk_sel = 0;
	pwm_set_0.pwm_ctr_sel = 0;
	pwm_set_0.pwm_dcdr = dcdr;
	pwm_set_0.pwm_step = 0;
	PWM_Set(&pwm_set_0);
	PWM_Enable(0);
}
extern int GPIO_Config(int GPIO, int gpio_no, int mode, int op_val, int int_lv, int pol);
void PMC_PORT_TEST(BYTE  channel)
{
	BYTE  var;
	BYTE  cmd;
	BYTE  index;
	switch (channel)
	{
		case 0x1:
			var = PMC1_DIR;
			while (PMC1_STR & 0x1);
			PMC1_DOR = var;
		#if LPC_WAY_OPTION_SWITCH
		#else
			SET_BIT(PMC1_CTL, 0);
		#endif
			break;
		case 0x2:
			var = PMC2_DIR;
			while (PMC2_STR & 0x1);
			PMC2_DOR = var;
		#if LPC_WAY_OPTION_SWITCH
		#else
			SET_BIT(PMC2_CTL, 0);
		#endif
			break;
		case 0x3:
			var = PMC3_DIR;
			while (PMC3_STR & 0x1);
			PMC3_DOR = var;
		#if LPC_WAY_OPTION_SWITCH
		#else
			SET_BIT(PMC3_CTL, 0);
		#endif
			break;
		case 0x4:
			var = PMC4_DIR;
			while (PMC4_STR & 0x1);
			PMC4_DOR = var;
		#if LPC_WAY_OPTION_SWITCH
		#else
			SET_BIT(PMC4_CTL, 0);
		#endif
			break;
		case 0x5:
			if (PMC5_STR & 0x08)
			{
				cmd = PMC5_DIR;
				//dprint("PM5 Cmd = %d\n",cmd);
				while (PMC5_STR & 0x1);
				PMC5_DOR = cmd;
			#if LPC_WAY_OPTION_SWITCH
			#else
				SET_BIT(PMC5_CTL, 0);
			#endif
				switch (cmd)
				{
					case 0x80://ec space read
						while (!(PMC5_STR & 0x2));
						index = PMC5_DIR;
						while (PMC5_STR & 0x1);
						var = Read_Map_ECSPACE_BASE_ADDR(index);
						PMC5_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						dprint("Read_Map_ECSPACE_BASE_ADDR(%d) = %d\n", index, var);
						break;
					case 0x81://ec space write
						while (!(PMC5_STR & 0x2));
						index = PMC5_DIR;
						while (PMC5_STR & 0x1);
						PMC5_DOR = index;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						while (!(PMC5_STR & 0x2));
						var = PMC5_DIR;
						Write_Map_ECSPACE_BASE_ADDR(index, var);
						var = Read_Map_ECSPACE_BASE_ADDR(index);
						while (PMC5_STR & 0x1);
						PMC5_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						dprint("Write_Map_ECSPACE_BASE_ADDR(%d) = %d\n", index, var);
						break;
					case 0x02://PWM/tach
						while (!(PMC5_STR & 0x2));
						index = PMC5_DIR;
						while (PMC5_STR & 0x1);
						PMC5_DOR = index;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						while (!(PMC5_STR & 0x2));
						var = PMC5_DIR;
						if (index == 0x00)
						{
							pwm_pmc_config(var);
							sysctl_iomux_pwm();
						}
						else
						{
							pwm_pmc_config(var);
							sysctl_iomux_pwm();
						}
						sysctl_iomux_tach0();
						TACH_Init_Channel(0, 1, 0);
						(*(DWORD *)(0x3047c)) = 1 << 15;//GPIOD 6 pullup
						tach0 = 0x00;
						tach0 = 0x5b8d80 / TACH0_DATA;
						dprint("tach0 = %d\n", tach0);
						while (PMC5_STR & 0x1);
						PMC5_DOR = (BYTE)(tach0 & (0xff));
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						while (PMC5_STR & 0x1);
						PMC5_DOR = (BYTE)((tach0 & (0xff00)) >> 8);
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						break;
					case 0x01:
						while (!(PMC5_STR & 0x2));
						index = PMC5_DIR;
						while (PMC5_STR & 0x1);
						PMC5_DOR = index;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						while (!(PMC5_STR & 0x2));
						var = PMC5_DIR;
						GPIO_Config(index, 0, 1, var, 0, 0);
						GPIO_Config(index, 1, 1, var, 0, 0);
						GPIO_Config(index, 2, 1, var, 0, 0);
						GPIO_Config(index, 3, 1, var, 0, 0);
						GPIO_Config(index, 4, 1, var, 0, 0);
						GPIO_Config(index, 5, 1, var, 0, 0);
						GPIO_Config(index, 6, 1, var, 0, 0);
						GPIO_Config(index, 7, 1, var, 0, 0);
						while (PMC5_STR & 0x1);
						PMC5_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						break;
					case 0x03:
						while (!(PMC5_STR & 0x2));
						index = PMC5_DIR;
						if (index == 0x00)
						{
							var = 0x00;
							while (PMC5_STR & 0x1);
							PMC5_DOR = var;
						#if LPC_WAY_OPTION_SWITCH
						#else
							SET_BIT(PMC5_CTL, 0);
						#endif
							var = 0x00;
							while (PMC5_STR & 0x1);
							PMC5_DOR = var;
						#if LPC_WAY_OPTION_SWITCH
						#else
							SET_BIT(PMC5_CTL, 0);
						#endif
						}
						else if (index == 0x01)
						{
							I2c_Readb(I2C_CLR_STOP_DET_OFFSET, I2C_CHANNEL_1);//clear stop det 
							/* Master switch and TAR set */
							I2c_Master_Controller_Init(I2C_CHANNEL_1);
							I2c_Master_Set_Tar(0x4C, I2C_CHANNEL_1);
							/* read temperature */
							var = I2c_Master_Read_Byte(I2C_CHANNEL_1);
							while (PMC5_STR & 0x1);
							PMC5_DOR = var;
						#if LPC_WAY_OPTION_SWITCH
						#else
							SET_BIT(PMC5_CTL, 0);
						#endif
							var = I2c_Master_Read_Byte(I2C_CHANNEL_1);
							while (PMC5_STR & 0x1);
							PMC5_DOR = var;
						#if LPC_WAY_OPTION_SWITCH
						#else
							SET_BIT(PMC5_CTL, 0);
						#endif
						}
						if (0 == I2c_Check_Stop_Det(I2C_CHANNEL_1))
						{
							I2c_Slave_debugger_Init(I2C_CHANNEL_1);
						}
						break;
					case 0x5A:
					case 0xA5:
					default:
						break;
				}
			}
			else
			{
				var = PMC5_DIR;
				//dprint("PM5 DATA = %d\n",var);
				while (PMC5_STR & 0x1);
				PMC5_DOR = var;
			#if LPC_WAY_OPTION_SWITCH
			#else
				SET_BIT(PMC5_CTL, 0);
			#endif
			}
			break;
		default:
			break;
	}
}
#endif
#endif
