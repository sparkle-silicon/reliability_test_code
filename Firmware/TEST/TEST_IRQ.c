/*
 * @Author: Maple
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2024-02-29 14:06:41
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
#include "TEST_IRQ.H"
#include "AE_DEBUGGER.H"
#include "AE_COMMAND.H"
extern char *irq_string;
extern BYTE Write_buff[256];
#if TEST
#if IRQC_DEBUG
char *irq_string = "irqc %d\n";
#endif
void __interrupt CPUS_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[0]++;
#endif
	irqprint(irq_string, 0);
};
void __interrupt CPUT_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[1]++;
#endif
	irqprint(irq_string, 1);
#if SUPPORT_CPUTIMER_WAKEUP
	if(Low_Power_Flag)
	{
		// Low_Power_Flag = 0;
		// Restore_Context();
		Exit_LowPower_Mode();
		// stop timer
		write_csr(0xBDB, 0x1);
		// clear timer
		write_csr(0xBDA, 0x0);
		// disable timer
		int32_t val = read_csr(0xBD1);
		write_csr(0xBD1, (val & 0xfffffffd));
		F_Service_WakeUp = 1;
	}
#endif
};
void __interrupt CPUR_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[2]++;
#endif
	irqprint(irq_string, 2);
};
void __interrupt SWUC_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[3]++;
#endif
	irqprint(irq_string, 3);
	irqprint("SWCHSTR status is %#x\n", SWUSTA);
#if 1
	if(SWUSTA & 0x80)
	{
		SWUSTA |= 0x80;
		irqprint("This is Logic Device Module trig interrupt!\n");
	}
	else if(SWUSTA & 0x40)
	{
		if(!(SWUCTL1 & 0x20))
		{
			irqprint("This is Soft Event trig interrupt! HOST Clear!\n");
		}
		else
		{
			SWUSTA |= 0x40;
			irqprint("This is Soft Event trig interrupt! EC Clear!\n");
		}
	}
	else if(SWUSTA & 0x8)
	{
		SWUSTA |= 0x8;
		irqprint("This is RING signal trig interrupt!\n");
	}
	else if(SWUSTA & 0x2)
	{
		SWUSTA |= 0x2;
		irqprint("This is RI2 signal trig interrupt!\n");
	}
	else if(SWUSTA & 0x1)
	{
		SWUSTA |= 0x1;
		irqprint("This is RI1 signal trig interrupt!\n");
	}
	else if((SWUCTL2 & 0x40) && (SWUIE & 0x40))
	{
		irqprint("This is legacy mode way scrdpbm_ec trig interrupt!\n");
		SWUCTL2 |= 0x40;
	}
	else if((SWUCTL2 & 0x80) && (SWUIE & 0x80))
	{
		irqprint("This is legacy mode way scrdpso_ec trig interrupt!\n");
		SWUCTL2 |= 0x80;
	}
	else if((SWUCTL2 & (0x02)))
	{
		irqprint("SWCTL2 data is %#x!\n", SWUCTL2);
		irqprint("This is ACPI S1 request interrupt!\n");
		(SWUCTL2) |= 0x02;
	}
	else if((SWUCTL2 & (0x04)))
	{
		irqprint("SWCTL2 data is %#x!\n", SWUCTL2);
		irqprint("This is ACPI S2 request interrupt!\n");
		(SWUCTL2) |= 0x04;
	}
	else if((SWUCTL2 & (0x08)))
	{
		irqprint("SWCTL2 data is %#x!\n", SWUCTL2);
		irqprint("This is ACPI S3 request interrupt!\n");
		SWUCTL2 |= 0x08;
	}
	else if((SWUCTL2 & (0x10)))
	{
		irqprint("SWCTL2 data is %#x!\n", SWUCTL2);
		irqprint("This is ACPI S4 request interrupt!\n");
		SWUCTL2 |= 0x10;
	}
	else if((SWUCTL2 & (0x20)))
	{
		irqprint("SWCTL2 data is %#x!\n", SWUCTL2);
		irqprint("This is ACPI S5 request interrupt!\n");
		SWUCTL2 |= 0x20;
	}
#endif
}
void __interrupt LPC_ESPI_RST_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[4]++;
#endif
	irqprint(irq_string, 4);
};
void __interrupt PWRSW_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[5]++;
#endif
	irqprint(irq_string, 5);
#if (SYSCTL_CLOCK_EN)
	SYSCTL_PWRSWCSR |= 0x20; // clear interrupt status flag
#endif
#if SUPPORT_PWRSW_WAKEUP
	if(Low_Power_Flag)
	{
		// Low_Power_Flag = 0;
		// Restore_Context();
		Exit_LowPower_Mode();
		printf("Restore Context flag\n");
	}
#endif
};
void __interrupt PS2_0_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[6]++;
#endif
	irqprint(irq_string, 6);
	if(MS_Main_CHN == 1)
		Handle_Mouse(MS_Main_CHN - 1);
	else if(KB_Main_CHN == 1)
		Handle_Kbd(KB_Main_CHN - 1);
};
void __interrupt KBS_SDV_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[7]++;
#endif
	irqprint(irq_string, 7);
	irqprint("KBS_SDV_HANDLER\n");
#if SUPPORT_KBS_WAKEUP
	if(Low_Power_Flag)
	{
		if(SystemIsS3)
			Exit_LowPower_Mode(); // support KBS wake cpu up when S3
	}
#endif
	F_Service_KBS = 1;
	KBS_KSDSR = 0x1;
};
void __interrupt KBS_PRESS_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[8]++;
#endif
	irqprint(irq_string, 8);
	irqprint("KBS_PRESS_HANDLER\n");
};
void __interrupt TACH0_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[9]++;
#endif
	irqprint(irq_string, 9);
	TACH0_Speed = TACH_Get(0);
	TACH_Clear_IRQ(0);
	TACH0_Speed = 0x5b8d80 / TACH0_Speed;
	irqprint("FanRPM_Tach0 is %d r/min  \n", TACH0_Speed);
	TACH_Int_Disable(0);
};
void __interrupt TACH1_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[10]++;
#endif
	irqprint(irq_string, 10);
	TACH1_Speed = TACH_Get(1);
	TACH_Clear_IRQ(1);
	TACH1_Speed = 0x5b8d80 / TACH1_Speed;
	irqprint("FanRPM_Tach1 is %d r/min \n", TACH1_Speed);
	TACH_Int_Disable(1);
};
void __interrupt TACH2_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[11]++;
#endif
	irqprint(irq_string, 11);
	TACH2_Speed = TACH_Get(2);
	TACH_Clear_IRQ(2);
	TACH2_Speed = 0x5b8d80 / TACH2_Speed;
	TACH_Int_Disable(2);
};
void __interrupt TACH3_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[12]++;
#endif
	irqprint(irq_string, 12);
	TACH3_Speed = TACH_Get(3);
	TACH_Clear_IRQ(3);
	TACH3_Speed = 0x5b8d80 / TACH3_Speed;
	TACH_Int_Disable(3);
};
void __interrupt KBC_IBF_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[13]++;
#endif
	irqprint(irq_string, 13);
#if LPC_PRESSURE_TEST
	CLEAR_BIT(KBC_CTL, 3);
#else
	irqc_disable_interrupt(IRQC_INT_DEVICE_KBC_IBF); // disable kbc ibf int
#endif
	F_Service_PCI = 1;
};
void __interrupt KBC_OBE_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[14]++;
#endif
	irqprint(irq_string, 14);
	irqprint("%s", "--------KBC_OutputBufferEmpty\n");
	// handle_kbd_event();
};
void __interrupt PMC1_IBF_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[15]++;
#endif
	irqprint(irq_string, 15);
	// dprint ("%s","--------PMC1_InputBufferFull\n");
	PMC1_CTL = 0x0;
	F_Service_PCI2 = 1;
	// Service_PCI2_Main();
};
void __interrupt PMC1_OBE_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[16]++;
#endif
	irqprint(irq_string, 16);
	// irqprint ("%s","--------PMC1_OutputBufferEmpty\n");
	PMC1_DOR = 0x5a;
#if ENABLE_DEBUGGER_SUPPORT
	Debugger_KBC_PMC_Record(1, 1, 0x5a);
#endif
};
static int PCI3_flag = 0;
#if (defined(TEST101) || defined(AE101) || defined(AE102))
void __interrupt PMC2_IBF_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[17]++;
#endif
	irqprint(irq_string, 17);
	PMC2_CTL = 0x0;
	F_Service_PCI3 = 1;
};
void __interrupt PMC2_OBE_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[18]++;
#endif
	irqprint(irq_string, 18);
};
#endif
#ifdef AE103
void __interrupt WU42_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[17]++;
#endif
	irqprint(irq_string, 17);
	/* irqprint("WU42_HANDLER\n");
	irqprint("mepc is %#x\n",read_csr(mepc));//指针
	irqprint("mcause is %#x\n",read_csr(mcause));//控制位
	irqprint("irqcip is %#x\n",read_csr(0xBD0));//中断源等待
	irqprint("irqcie is %#x\n",read_csr(0xBD1));//中断源使能
	irqprint("irqclvl is %#x\n",read_csr(0xBD2));//中断源电平触发
	irqprint("irqcedge is %#x\n",read_csr(0xBD3));//中断源边沿触发
	irqprint("irqcinfo is %d\n",0x3f&read_csr(0xBD4));//中断源个数 */
};
void __interrupt RTC_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[18]++;
#endif
	irqprint(irq_string, 18);
	SYSCTL_MODEN0 |= 0x100;
	SYSCTL_MODEN1 |= BIT(0) | BIT(21) | BIT(11);
	// if (Low_Power_Flag)
	// {
	// 	// Low_Power_Flag = 0;
	// 	// Restore_Context();
	// 	Exit_LowPower_Mode();
	// 	printf("Restore Context flag\n");
	// }
	printf("EOI:%x\n", REG8(0x3F18));
	// CCR
	REG8(0x3F0C) = 0x0;
	REG8(0x3F0D) = 0x0;
	REG8(0x3F0E) = 0x0;
	REG8(0x3F0F) = 0x0;
};
#endif
void __interrupt WDT_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[19]++;
#endif
	irqprint(irq_string, 19);
#if ENABLE_DEBUGGER_SUPPORT
	if(0 == ResetChipFlag)
	{
		WDT_FeedDog();
	}
#endif
	// irqprint("0x3041c= %#x\n",((*(BYTE *)0x3041c)));
};
void __interrupt ADC_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[20]++;
#endif
	irqprint(irq_string, 20);
};
void __interrupt UART0_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[21]++;
#endif
	irqprint(irq_string, 21);
#if (ENABLE_COMMAND_SUPPORT && COMMAND_UART_SWITCH == 0)
	if(F_Service_CMD == 1)
	{
		char temp = UART0_RX;
	#if (!IRQC_DEBUG)
		UNUSED_VAR(temp);
	#endif
		irqprint("erro of CMD_RUN:%#x,%c\n", temp, temp);
		return;
	}
	CMD_UART_BUFF[CMD_UART_CNT] = UART0_RX;
	if(CMD_UART_BUFF[CMD_UART_CNT] == '\n' || CMD_UART_CNT == CMD_BUFF_MAX)
	{
		CMD_UART_BUFF[CMD_UART_CNT] = '\0';
		if(CMD_UART_BUFF[CMD_UART_CNT - 1] == ' ')
		{
			CMD_UART_CNT--;
			CMD_UART_BUFF[CMD_UART_CNT] = '\0';
		}
		F_Service_CMD = 1;
	}
	else if(CMD_UART_BUFF[CMD_UART_CNT] == '\r')
		CMD_UART_BUFF[CMD_UART_CNT] = '\0';
	else
		CMD_UART_CNT++;
#endif
/* Debugger Function Entry */
#if ENABLE_DEBUGGER_SUPPORT
#if DEBUGGER_UARTA_CHANNEL
	Debugger_Cmd_IRQ(DEBUGGER_UART_REG(UART_RBR_OFFSET));
#endif
#endif
}
void __interrupt UARTA_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[22]++;
#endif
	irqprint(irq_string, 22);
#if (ENABLE_COMMAND_SUPPORT && COMMAND_UART_SWITCH == 4)
	if(F_Service_CMD == 1)
	{
		char temp = UARTA_RX;
	#if (!IRQC_DEBUG)
		UNUSED_VAR(temp);
	#endif
		irqprint("erro of CMD_RUN:%#x,%c\n", temp, temp);
		return;
	}
	CMD_UART_BUFF[CMD_UART_CNT] = UARTA_RX;
	if(CMD_UART_BUFF[CMD_UART_CNT] == '\n' || CMD_UART_CNT == CMD_BUFF_MAX)
	{
		CMD_UART_BUFF[CMD_UART_CNT] = '\0';
		if(CMD_UART_BUFF[CMD_UART_CNT - 1] == ' ')
		{
			CMD_UART_CNT--;
			CMD_UART_BUFF[CMD_UART_CNT] = '\0';
		}
		F_Service_CMD = 1;
	}
	else if(CMD_UART_BUFF[CMD_UART_CNT] == '\r')
		CMD_UART_BUFF[CMD_UART_CNT] = '\0';
	else
		CMD_UART_CNT++;
#endif
/* Debugger Function Entry */
#if ENABLE_DEBUGGER_SUPPORT
	Debugger_Cmd_IRQ(DEBUGGER_UART_REG(UART_RBR_OFFSET));
#endif
}
void __interrupt UARTB_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[23]++;
#endif
	irqprint(irq_string, 23);
#if (ENABLE_COMMAND_SUPPORT && COMMAND_UART_SWITCH == 5)
	if(F_Service_CMD == 1)
	{
		char temp = UARTB_RX;
	#if (!IRQC_DEBUG)
		UNUSED_VAR(temp);
	#endif
		irqprint("erro of CMD_RUN:%#x,%c\n", temp, temp);
		return;
	}
	CMD_UART_BUFF[CMD_UART_CNT] = UARTB_RX;
	if(CMD_UART_BUFF[CMD_UART_CNT] == '\n' || CMD_UART_CNT == CMD_BUFF_MAX)
	{
		CMD_UART_BUFF[CMD_UART_CNT] = '\0';
		if(CMD_UART_BUFF[CMD_UART_CNT - 1] == ' ')
		{
			CMD_UART_CNT--;
			CMD_UART_BUFF[CMD_UART_CNT] = '\0';
		}
		F_Service_CMD = 1;
	}
	else if(CMD_UART_BUFF[CMD_UART_CNT] == '\r')
		CMD_UART_BUFF[CMD_UART_CNT] = '\0';
	else
		CMD_UART_CNT++;
#endif
/* Debugger Function Entry */
#if ENABLE_DEBUGGER_SUPPORT
	Debugger_Cmd_IRQ(DEBUGGER_UART_REG(UART_RBR_OFFSET));
#endif
}
void __interrupt SM_EC_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[24]++;
#endif
	irqprint(irq_string, 24);
	irqprint("This is SM_EC Interrupt!\n");
	clear_csr(mstatus, MSTATUS_MIE);
	nop;
	nop;
	nop;
	*((volatile uint8_t *)(0x250a)) |= 0x1; // clear sm_ec interrupt status flag
	if(SMF_CMD == 0x2)						// 写
	{
		unsigned short i = 0;
		if(SMF_NUM > 4)
		{
			Write_buff[0] = SMF_DR0;
			Write_buff[1] = SMF_DR1;
			Write_buff[2] = SMF_DR2;
			Write_buff[3] = SMF_DR3;
			for(i = 4; i < SMF_NUM; i++)
			{
				Write_buff[i] = *((volatile unsigned char *)(0x31000 + (SMF_SRA << 4) + (i - 4)));
			}
		}
		else
		{
			Write_buff[0] = SMF_DR0;
			Write_buff[1] = SMF_DR1;
			Write_buff[2] = SMF_DR2;
			Write_buff[3] = SMF_DR3;
		}
		/* for(i=0;i<SMF_NUM;i++)
		{
			printf("Write_buff[%d] is %#x\n",i,Write_buff[i]);
		} */
		RunSPIFE_WriteFromRAM((SMF_FADDR0 & 0xff) + ((SMF_FADDR1 << 8) & 0xff00) + ((SMF_FADDR2 << 16) & 0xff0000), Write_buff);
	}
	else if(SMF_CMD == 0x3) // 读
	{
	}
	else if(SMF_CMD == 0x13) // firmware cycle 读
	{
	}
	else if(SMF_CMD == 0x12) // firmware cycle 写
	{
		Write_buff[0] = SMF_DR0;
		Write_buff[1] = SMF_DR1;
		Write_buff[2] = SMF_DR2;
		Write_buff[3] = SMF_DR3;
		printf("Write_buff[0] is %#x\n", Write_buff[0]);
		printf("SMF_FADDR0 is %#x\n", SMF_FADDR0);
		printf("SMF_FADDR1 is %#x\n", SMF_FADDR1);
		printf("SMF_FADDR2 is %#x\n", SMF_FADDR2);
		printf("addr is %#x\n", (SMF_FADDR0 & 0xff) + ((SMF_FADDR1 << 8) & 0xff00) + ((SMF_FADDR2 << 16) & 0xff0000));
		RunSPIFE_WriteFromRAM((SMF_FADDR0 & 0xff) + ((SMF_FADDR1 << 8) & 0xff00) + ((SMF_FADDR2 << 16) & 0xff0000), Write_buff);
	}
};
void __interrupt SM_HOST_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[25]++;
#endif
	irqprint(irq_string, 25);
};
void __interrupt TIMER0_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[26]++;
#endif
#if 0
	irqprint(irq_string, 26);
#endif
	if((TIMER0_TIS & 0x1) == 0x1)
		TIMER0_TEOI; // clear int
};
void __interrupt TIMER1_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[27]++;
#endif
#if 0
	irqprint(irq_string, 27);
#endif
	if(TIMER1_TIS & 0x1) // read int status
	{
		TIMER1_TEOI; // clear int
	}
};
void __interrupt TIMER2_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[28]++;
#endif
#if 0
	irqprint(irq_string, 28);
#endif
	// irqprint ("%s","--------Begin TIMER2 handler----Vector mode\n");
	if(TIMER2_TIS & 0x1)
	{
		TIMER2_TEOI;
	}
	if(!(F_Service_MS_1))
	{
		F_Service_MS_1 = 1; // Request 5 mS timer service.
	}
};
void __interrupt TIMER3_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[29]++;
#endif
#if 0
	irqprint(irq_string, 29);
#endif
	// irqprint ("%s","--------Begin TIMER3 handler----Vector mode\n");
	if(TIMER3_TIS & 0x1)
	{
		TIMER3_TEOI;
	}
};
extern BYTE intr_st;
/*Intr0*/
void __interrupt INTC0_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[30]++;
#endif
	irqprint(irq_string, 30);
	// irqprint("\nIn main function, the mstatus is %#x\n", read_csr(mstatus));
	int i = 0;
	int j = 0;
	int num = 0;
	int flag = 0;
	for(i = 0; i < 8; i++)
	{
		intr_st = REG8(INTC0_REG_ADDR(INTC_ICTL_MASKSTATUS_L + i));
		for(j = 0; j < 8; j++)
		{
			if(intr_st & (1 << j))
			{
				num = i * 8 + j;
				flag = 1;
				break;
			}
		}
		if(flag == 1)
			break;
	}
	irqprint("intr0[%d]\n", num);
	(intr0_service[(num)])(); // Dispatch to service handler.
}
/*Intr1*/
void __interrupt INTC1_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[31]++;
#endif
	irqprint(irq_string, 31);
	int i = 0;
	int j = 0;
	int num = 0;
	int flag = 0;
	for(i = 0; i < 8; i++)
	{
		intr_st = REG8(INTC1_REG_ADDR(INTC_ICTL_MASKSTATUS_L + i));
		for(j = 0; j < 8; j++)
		{
			if(intr_st & (1 << j))
			{
				num = i * 8 + j;
				flag = 1;
				break;
			}
		}
		if(flag == 1)
			break;
	}
	irqprint("intr1[%d]\t", num);
	(intr1_service[(num)])(); // Dispatch to service handler.
}
#endif