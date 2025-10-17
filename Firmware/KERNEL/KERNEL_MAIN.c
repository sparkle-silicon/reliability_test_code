/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-04 14:35:44
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
#include "KERNEL_INCLUDE.H"
#include "CUSTOM_INCLUDE.H"
extern Task* task_head;
#define printf_instructions_msg " \
\n\
************************************************************************************\n\
*                    Embedded Controller  SPK32 Series Firmware                    *\n\
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
#define Stress_test_printf 0
#if Stress_test_printf
	for (int i = 4096; i; i--)
		dprint("AAAAA\n");
#endif
#if (Service_MS_1_START == 1)
	if (F_Service_MS_1 == 0)
		return;
	else /*if (F_Service_MS_1 == 1)*/
	{
		F_Service_MS_1 = 0;
	}
	//  else//  Exception handling chicken
	//  {
	//  	dprint("F_Service_MS_1 is %#x ...\n", F_Service_MS_1);
	//  }
	Event_1ms();
	timer_1ms_count++;
	if (timer_1ms_count >= 10)
	{
		timer_1ms_count = 0;
	}
	if ((timer_1ms_count % 5) == 0) // 5ms
	{
		Event_5ms();
		timer_5ms_count++;
		if (timer_5ms_count % 2 == 1) // 10ms events
		{
			EventA_10ms();
		}
		else
		{
			EventB_10ms();
		}
		if (timer_5ms_count == 2)
		{
			EventA_50ms();
		}
		else if (timer_5ms_count == 4)
		{
			EventB_50ms();
		}
		else if (timer_5ms_count == 6)
		{
			EventC_50ms();
		}
		else if (timer_5ms_count == 8)
		{
			timer_100ms_count++;
			if (timer_100ms_count % 2 == 1)
			{
				EventA_100ms();
			}
			else
			{
				EventB_100ms();
			}
		}
		else if (timer_5ms_count >= 10)
		{
			timer_5ms_count = 0;
		}
		if (timer_5ms_count == 0) // 50ms
		{
			timer_50ms_count++;
			if (timer_50ms_count % 2 == 0)
			{
				EventC_100ms();
			}
			if (timer_50ms_count == 4)
			{
				EventA_500ms();
			}
			else if (timer_50ms_count == 6)
			{
				EventB_500ms();
			}
			else if (timer_50ms_count == 8)
			{
				EventC_500ms();
			}
			else if (timer_50ms_count == 10)
			{
				EventA_1s();
			}
			else if (timer_50ms_count == 12)
			{
				EventB_1s();
			}
			else if (timer_50ms_count == 14)
			{
				EventA_500ms();
			}
			else if (timer_50ms_count == 16)
			{
				EventB_500ms();
			}
			else if (timer_50ms_count == 18)
			{
				EventC_500ms();
			}
			else if (timer_50ms_count == 20)
			{
				EventC_1s();
				timer_50ms_count = 0;
				timer_1s_count++;
				if (timer_1s_count == 60)
				{
					Event_1min();
					timer_1s_count = 0;
					timer_1min_count++;
					if (timer_1min_count == 60)
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
//----------------------------------------------------------------------------
// FUNCTION: Service_Mailbox
//----------------------------------------------------------------------------
void Service_Mailbox(void)
{
#if (Service_Mailbox_START == 1)
	if (F_Service_Mailbox == 1)
	{
		F_Service_Mailbox = 0;
		Mailbox_C2E_Service();
	}
#endif
}
//----------------------------------------------------------------------------
// FUNCTION: Service_Reserved2
//----------------------------------------------------------------------------
void Service_Reserved2(void)
{
#if (Service_Reserved2_START == 1)
	if (F_Service_Reserved2)
	{
		F_Service_Reserved2 = 0;
	}
#endif
}
//----------------------------------------------------------------------------
// FUNCTION: Service_Reserved2
//----------------------------------------------------------------------------
void Service_Reserved3(void)
{
#if (Service_Reserved3_START == 1)
	if (F_Service_Reserved3)
	{
		F_Service_Reserved3 = 0;
	}
#endif
}
//----------------------------------------------------------------------------
// FUNCTION: Service_CPU_WakeUp
// CPU Wake Up finction
//----------------------------------------------------------------------------
void Service_CPU_WakeUp(void)
{
	if (F_Service_WakeUp)
	{
		F_Service_WakeUp = 0;
		dprint("cpu wake service complete!\n");
		Enter_LowPower_Mode();
	}
}
//----------------------------------------------------------------------------
// FUNCTION: Service_PUTC
// UART putchar support function
//----------------------------------------------------------------------------
void Service_PUTC(void)
{
#if SUPPORT_REAL_OR_DELAY_PRINTF
	if (F_Service_PUTC && (!print_number))
		return;
	// if(print_number&&(PRINTF_LSR & UART_LSR_TEMP) )
	while (print_number && (!(PRINTF_LSR & UART_LSR_THRE)))
	{
		PRINTF_TX = print_buff[PRINT_SERVICE_CNT];
		// print_buff[PRINT_SERVICE_CNT]='\0';
		PRINT_SERVICE_CNT++;
		print_number--;
		if (PRINT_SERVICE_CNT >= PRINT_MAX_SIZE)
			PRINT_SERVICE_CNT = 0;
	}
#endif
}
//----------------------------------------------------------------------------
// FUNCTION: Service_CMD
// UART Command support function
//----------------------------------------------------------------------------
void Service_CMD(void)
{
#if ENABLE_COMMAND_SUPPORT
	if (F_Service_CMD)
	{
		CMD_RUN((volatile char*)&CMD_UART_CNT, (char*)CMD_UART_BUFF);
		CMD_UART_CNT = 0;
		F_Service_CMD = 0;
	}
#endif
}
#if ENABLE_DEBUGGER_SUPPORT
void Service_Debugger(void)
{
	if (F_Service_Debugger)
	{
		Debug_Timeout_Count = 5000;
		I2C0_INTR_MASK &= ~(0x1 << 2); // 屏蔽接收中断
		// Uart_Int_Disable(UARTA_CHANNEL, 0);
		while (F_Service_Debugger_Cmd != F_Service_Debugger_Cnt)
			Deubgger_Cmd_Parsing(Debugger_Cmd[F_Service_Debugger_Cnt++]);
		F_Service_Debugger = 0;
		// Uart_Int_Enable(UARTA_CHANNEL, 0);
		I2C0_INTR_MASK |= (0x1 << 2);
	}
	else
	{
		Debug_Timeout_Count--;
		if (Debug_Timeout_Count == 0)
		{
			Debug_Timeout_Count = 5000;
			Buf_flag = 0;
			Debug_Temp = 0;
			F_Service_Debugger_Cmd = F_Service_Debugger_Cnt = 0;
		}
	}
	// if (F_Service_Debugger_Send)
	// {
	// 	Debugger_I2c_Send(DEBUGGER_I2C_CHANNEL);
	// 	assert_print("iicFeedback %#x\n", iicFeedback);
	// 	F_Service_Debugger_Send = 0;
	// }
	if (F_Service_Debugger_Rrq)
	{
		Debugger_I2c_Req(DEBUGGER_I2C_CHANNEL);
		assert_print("iicFeedback %#x\n", iicFeedback);
		F_Service_Debugger_Rrq = 0;
		switch (DEBUGGER_I2C_CHANNEL) // 打开RD_REQ中断
		{
		case I2C_CHANNEL_0:
			I2C0_INTR_MASK |= 0x20;
		case I2C_CHANNEL_1:
			I2C1_INTR_MASK |= 0x20;
		case I2C_CHANNEL_2:
			I2C2_INTR_MASK |= 0x20;
		case I2C_CHANNEL_3:
			I2C3_INTR_MASK |= 0x20;
		case I2C_CHANNEL_4:
			I2C4_INTR_MASK |= 0x20;
		case I2C_CHANNEL_5:
			I2C5_INTR_MASK |= 0x20;
		}
	}
}
#endif
//----------------------------------------------------------------------------
// FUNCTION: Service_PUTC
// UART putchar support function
//----------------------------------------------------------------------------
void Service_Process_Tasks(void)
{
	Process_Tasks();
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
	Service_CPU_WakeUp, // cpu wake up
	Service_PUTC,		// Service printf
	Service_CMD,		// CMD Run
	Service_PCI,		// Host send to ec 60/64 Command/Data service
	Service_Send,		// Send byte from KBC to host
	Service_Send_PS2,	// Send PS2 interface pending data
	Service_PCI2,		// PMC1 Host Command/Data service
	Service_MS_1,		// 5 millisecond elapsed for CUSTOM
	Service_KBS,		// Keyboard scanner service
		Service_Mailbox, // Mailbox service
		Service_Process_Tasks, // Process Tasks
		// Lo-Level Service
Service_PCI3,	   // PMC2 Host Command/Data service
Service_PCI4,	   // PMC3 Host Command/Data service
Service_LPCRST,	   // Service LPCRST
Service_PCI5,	   // PMC4 Host Command/Data service
Service_PCI6,	   // PMC5 Host Command/Data service
Service_Reserved2, // Reserved
#if ENABLE_DEBUGGER_SUPPORT
		Service_Debugger,
		Debugger_Send_KBD,
#endif
		Service_Reserved3, // Reserved
};
const short array_count = sizeof(service_table) / sizeof(FUNCT_PTR_V_V);
//----------------------------------------------------------------------------
// FUNCTION: main_service
// polling service table
//----------------------------------------------------------------------------
void main_service(void)
{

	if (_R1 >= sizeof(service_table) / sizeof(FUNCT_PTR_V_V))
		_R1 = 0;
	(service_table[_R1])();
	_R1++;
}
//----------------------------------------------------------------------------
// FUNCTION: main loop
// enter main loop
//----------------------------------------------------------------------------
void main_loop(void)
{
	dprint("Enter main_service \n");

	while (1)
	{
		if (E2CINFO7 & 0x1)
		{
			E2CINFO7 = 0x0;
			Enter_LowPower_Mode();
		}

		main_service();
	}
}

VBYTEP OPTIMIZE0 USED SPIF_Read_Interface(register DWORD size, register DWORD addr, BYTEP read_buff)
{
	BYTE SPIF_READ_COMMAND;
	DWORD SPIF_J;
#define temp_addrs SPIF_addrs
#define temp_data SPIF_data
	if (size & 0b11)
	{
		size += 0b100;
		size &= ~0b11;
	}
	// PRINTF_TX('M');
	while (!(SPIF_READY & 0x1))
		;
	// PRINTF_TX('N');
	// rom_wdt_feed();
	while (SPIF_STATUS & 0xf)
		;
	// PRINTF_TX('W');
	// rom_wdt_feed(); // 直到写完
	while (!(SPIF_READY & 0x1))
		;
	// rom_wdt_feed(); // 读忙
	SPIF_DBYTE = (size - 1) & 0xff;
	while (!(SPIF_READY & 0x1))
		;
	// rom_wdt_feed();
	if (SPIF_CTRL0 & BIT1)
		SPIF_READ_COMMAND = 0x6b;
	else
		SPIF_READ_COMMAND = 0x3b;
	SPIF_FIFO_TOP = (((addr & 0xFF) << 24) + ((addr & 0xFF00) << 8) + ((addr & 0xFF0000) >> 8) + SPIF_READ_COMMAND);
	for (SPIF_J = 0; SPIF_J < (size >> 2); SPIF_J++)
	{
		while ((SPIF_FIFO_CNT & 0x3) == 0)
			;
		// rom_wdt_feed();
		// printf("TOP:%x", SPIF_FIFO_TOP);
		(*((DWORDP)(&(read_buff[SPIF_J << 2])))) = SPIF_FIFO_TOP;
	}
	return read_buff;
#undef temp_addrs
#undef temp_data
}

uint8_t ccc_data[10] = { 0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa };

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
	I3C_MASTER_BC_CCC_WRITE(ccc_data, 5, SETMWL_BC_CMD, 0, 0, I3C_MASTER0);
	// I3C_MASTER_PV_WRITE_WITH7E(uint8_t dynamic_addr, uint8_t * data, uint16_t bytelen, BYTE i3c_mux);
	main_loop();
	return 0;
}
