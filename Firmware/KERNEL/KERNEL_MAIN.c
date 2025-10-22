/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-22 17:42:57
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
#include "KERNEL_MAIN.H"
#define printf_instructions_msg " \
\n\
************************************************************************************\n\
*                    Embedded Controller  GDMEC01 Series Firmware                  *\n\
* Copyright ©2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved. *\n\
************************************************************************************\n\
\n\
"
extern unsigned char iicFeedback, iic_flag, iic_int_flag;
//------------------------------------------------------------
// 1ms events
//------------------------------------------------------------
void Event_1ms(void)
{
	Hook_1msEvent(timer_1ms_count);
}
//-----------------------------------------------------------------------------
// 5ms Events
//-----------------------------------------------------------------------------
void Event_5ms(void)
{
#if SUPPORT_KBS_REPEAT
	KBD_Check_Repeat();
#endif
	Hook_5msEvent();
}
//-----------------------------------------------------------------------------
// 10ms Events
//-----------------------------------------------------------------------------
void EventA_10ms(void)
{
	Hook_10msEventA();
}
//-----------------------------------------------------------------------------
void EventB_10ms(void)
{
	Hook_10msEventB();
}
//-----------------------------------------------------------------------------
// 50ms Events
//-----------------------------------------------------------------------------
void EventA_50ms(void)
{
	Hook_50msEventA();
}
//-----------------------------------------------------------------------------
void EventB_50ms(void)
{
	Hook_50msEventB();
}
//-----------------------------------------------------------------------------
void EventC_50ms(void)
{
	Hook_50msEventC();
}
//-----------------------------------------------------------------------------
// 100ms Events
//-----------------------------------------------------------------------------
void EventA_100ms(void)
{
	Hook_100msEventA();
}
//-----------------------------------------------------------------------------
void EventB_100ms(void)
{
	Hook_100msEventB();
}
//-----------------------------------------------------------------------------
void EventC_100ms(void)
{
	Hook_100msEventC();
}
//-----------------------------------------------------------------------------
// 500ms Events
//-----------------------------------------------------------------------------
void EventA_500ms(void)
{
	Hook_500msEventA();
}
//-----------------------------------------------------------------------------
void EventB_500ms(void)
{
	Hook_500msEventB();
}
//-----------------------------------------------------------------------------
void EventC_500ms(void)
{
	Hook_500msEventC();
}
//-----------------------------------------------------------------------------
// 1 second events
//-----------------------------------------------------------------------------
void EventA_1s(void)
{
	Hook_1secEventA();
}
//-----------------------------------------------------------------------------
void EventB_1s(void)
{
	Hook_1secEventB();
}
//-----------------------------------------------------------------------------
void EventC_1s(void)
{
	Hook_1secEventC();
}
//-----------------------------------------------------------------------------
// 1 min events
//-----------------------------------------------------------------------------
void Event_1min(void)
{
	Hook_1minEvent();
}
//----------------------------------------------------------------------------
// FUNCTION: Service_MS_1
// 1ms fundation function
//----------------------------------------------------------------------------
void Service_MS_1(void)
{
#if (Service_MS_1_START == 1)
	if(F_Service_MS_1)
	{
		F_Service_MS_1 = 0;
	}
	else
	{
		return;
	}
	Event_1ms();
	timer_1ms_count++;
	if(timer_1ms_count >= 10)
	{
		timer_1ms_count = 0;
	}
	if((timer_1ms_count % 5) == 0) // 5ms
	{
		Event_5ms();
		timer_5ms_count++;
		if(timer_5ms_count % 2 == 1) // 10ms events
		{
			EventA_10ms();
		}
		else
		{
			EventB_10ms();
		}
		if(timer_5ms_count == 2)
		{
			EventA_50ms();
		}
		else if(timer_5ms_count == 4)
		{
			EventB_50ms();
		}
		else if(timer_5ms_count == 6)
		{
			EventC_50ms();
		}
		else if(timer_5ms_count == 8)
		{
			timer_100ms_count++;
			if(timer_100ms_count % 2 == 1)
			{
				EventA_100ms();
			}
			else
			{
				EventB_100ms();
			}
		}
		else if(timer_5ms_count >= 10)
		{
			timer_5ms_count = 0;
		}
		if(timer_5ms_count == 0) // 50ms
		{
			timer_50ms_count++;
			if(timer_50ms_count % 2 == 0)
			{
				EventC_100ms();
			}
			if(timer_50ms_count == 4)
			{
				EventA_500ms();
			}
			else if(timer_50ms_count == 6)
			{
				EventB_500ms();
			}
			else if(timer_50ms_count == 8)
			{
				EventC_500ms();
			}
			else if(timer_50ms_count == 10)
			{
				EventA_1s();
			}
			else if(timer_50ms_count == 12)
			{
				EventB_1s();
			}
			else if(timer_50ms_count == 14)
			{
				EventA_500ms();
			}
			else if(timer_50ms_count == 16)
			{
				EventB_500ms();
			}
			else if(timer_50ms_count == 18)
			{
				EventC_500ms();
			}
			else if(timer_50ms_count == 20)
			{
				EventC_1s();
				timer_50ms_count = 0;
				timer_1s_count++;
				if(timer_1s_count == 60)
				{
					Event_1min();
					timer_1s_count = 0;
					timer_1min_count++;
					if(timer_1min_count == 60)
					{
						timer_1min_count = 0;
						timer_1hours_count++;
					}
				}
			}
		}
	}
#endif
}

//-----------------------------------------------------------------------------
//  Function Pointers
//-----------------------------------------------------------------------------
/* FUNCT_PTR_V_V is a pointer to a function that returns nothing
   (V for void) and takes nothing for a parameter (V for void). */
   //-----------------------------------------------------------------------------
const FUNCT_PTR_V_V service_table[] =
{
	// Hi-Level Service
	//高优先级
	Service_PCI,              // Host send to ec 60/64 Command/Data service
	Service_Send,             // Send byte from KBC to host service
	Service_Send_PS2,		  // Send PS2 interface pending data to host service

	// Mi-Level Service
	//中优先级
	Service_MS_1,             // 1 millisecond Service
	Service_PCI2,             // PMC1 Host Command/Data service
	Service_KBS,              // Keyboard scanner service
	Service_PROCESS_TASKS,    // Security SubSystem(Crypto CPU) Mailbox Process Tasks service
	Service_Mailbox,          // Security SubSystem(Crypto CPU) Mailbox Commands service

	// Lo-Level Service
	//低优先级
	Service_PCI3,             // PMC2 Host Command/Data service
	Service_PCI4,             // PMC3 Host Command/Data service
	Service_HOST_RST,         //  lpc reset and espi reset Service
	Service_PCI5,             // PMC4 Host Command/Data service
	Service_PCI6,             // PMC5 Host Command/Data service
	Service_LowPower,         // CPU LowPower Service

	// DEBUG Service
#if SUPPORT_REAL_OR_DELAY_PRINTF
	Service_PUTC,			  // UART delay printf service 
#endif
#if ENABLE_COMMAND_SUPPORT
	Service_CMD,              // UART char command service
#endif
#if ENABLE_DEBUGGER_SUPPORT
	Service_Debugger,         // DEBUGGER service
	Service_Debugger_Send_KBD,        // DEBUGGER
#endif

};
//----------------------------------------------------------------------------
// FUNCTION: main_service
// polling service table
//----------------------------------------------------------------------------
void main_service(void)
{

	if(_R1 >= (sizeof(service_table) / sizeof(FUNCT_PTR_V_V)))
		_R1 = 0;
	(service_table[_R1])();
#if SUPPORT_MAIN_SERVICE_LEVEL_POLL
#if (HIGH_LEVEL_SERVICE_NUM > 0)
	if(_R1 >= HIGH_LEVEL_SERVICE_NUM)//开启高等级穿插式执行服务
	{
		(service_table[(_R1 % HIGH_LEVEL_SERVICE_NUM)])();
	#if (MIDDLE_LEVEL_SERVICE_NUM > 0)
		if(_R1 >= (HIGH_LEVEL_SERVICE_NUM + MIDDLE_LEVEL_SERVICE_NUM))//开启中等级穿插式执行服务
		{
			(service_table[HIGH_LEVEL_SERVICE_NUM + (_R1 % (MIDDLE_LEVEL_SERVICE_NUM))])();
		}
	#endif
	}
#endif
#endif
	_R1++;
}
//----------------------------------------------------------------------------
// FUNCTION: main loop
// enter main loop
//----------------------------------------------------------------------------
void main_loop(void)
{
	dprint("Enter main_service.\n");
	while(1)
	{
		main_service();
	}
}
#define I3C_TEST 1
#if I3C_TEST//收到,已确认暂时不管这一段代码
uint8_t ccc_wdata[10] = { 0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa };
uint8_t ccc_rdata[10] = { 0,0,0,0,0,0,0,0,0,0 };
uint8_t ccc_rdata1[10] = { 0,0,0,0,0,0,0,0,0,0 };
uint8_t ccc_dr_rdata[10] = { 0,0,0,0,0,0,0,0,0,0 };
uint8_t ccc_dr_rdata1[10] = { 0,0,0,0,0,0,0,0,0,0 };
#endif
//----------------------------------------------------------------------------
// FUNCTION: main
// main entry
//----------------------------------------------------------------------------
int __weak main(void)
{
	AutoON_Check_AfterUpdate(); // 检查更新后重启设定标志
#if DEBUG
	printr(printf_instructions_msg);
	printr("Core Name %s\n", CORE_NAME);
	printr("Core Version %d\n", CORE_Version);
	printr("Code Version %s\n", CODE_VERSION);
	printr("Compile Time : %s %s\n", __DATE__, __TIME__);
#endif
	// 2. print Operational information
	dprint("This is %s flash main\n", (SYSCTL_PIO_CFG & BIT1) ? "external" : "internal");
	dprint("CPU freq at %d Hz\n", CPU_FREQ);
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
	// printf("private read dcr:%x\n", ccc_dr_rdata[0]);

	// I3C_SLAVE_IBI_HOTJOIN(I3C_SLAVE0_STATIC_ADDR, I3C_SLAVE0_DEFAULT_IDPARTNO, I3C_SLAVE0_DEFAULT_DCR, I3C_SLAVE0_DEFAULT_BCR, I3C_SLAVE0);
	// I3C_SLAVE_IBI_DATA(0xAA, I3C_SLAVE0_DEFAULT_IDPARTNO, I3C_SLAVE0_DEFAULT_DCR, I3C_SLAVE0_DEFAULT_BCR, I3C_SLAVE0);

	// printf("master1/slave1测试\n");
	// I3C_MASTER_BC_CCC_WRITE(ccc_wdata, 2, SETMWL_BC_CMD, 0, 0, I3C_MASTER1);
	// printf("slave1 maxlength:%x\n", SLAVE1_MAXLIMITS);
	// I3C_MASTER_PV_WRITE_WITH7E(0x3a, ccc_wdata, 5, I3C_MASTER1);
	// I3C_SLAVE_READ(ccc_rdata, 5, I3C_SLAVE1);
	// for (int i = 0; i < 5; i++)
	// {
	// 	printf("private read data%x:%x\n", i, ccc_rdata[i]);
	// }
	// I3C_SLAVE_WRITE(ccc_wdata, 5, I3C_SLAVE1);
	// I3C_MASTER_PV_READ_WITH7E(0x3a, ccc_rdata1, 5, I3C_MASTER1);
	// for (int i = 0; i < 5; i++)
	// {
	// 	printf("private read data%x:%x\n", i, ccc_rdata1[i]);
	// }
	// I3C_MASTER_DR_CCC_WRITE(0x3a, ccc_wdata, 2, SETMWL_DR_CMD, 0, 0, I3C_MASTER1);
	// printf("slave1 maxlength:%x\n", SLAVE1_MAXLIMITS);
	// I3C_MASTER_DR_CCC_READ(0x3a, ccc_dr_rdata, 1, GETDCR_DR_CMD, 0, 0, I3C_MASTER1);
	// printf("private read dcr:%x\n", ccc_dr_rdata[0]);

	// I3C_SLAVE_IBI_HOTJOIN(I3C_SLAVE1_STATIC_ADDR, I3C_SLAVE1_DEFAULT_IDPARTNO, I3C_SLAVE1_DEFAULT_DCR, I3C_SLAVE1_DEFAULT_BCR, I3C_SLAVE1);
	// I3C_SLAVE_IBI_DATA(0xAA, I3C_SLAVE1_DEFAULT_IDPARTNO, I3C_SLAVE1_DEFAULT_DCR, I3C_SLAVE1_DEFAULT_BCR, I3C_SLAVE1);
	/*******end */
#endif
	main_loop();
	return 0;
}
