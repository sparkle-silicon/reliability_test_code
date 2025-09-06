/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-06-04 17:51:55
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
#include "KERNEL_IRQ.H"
#include "KERNEL_INTC.H"
#include "AE_DEBUGGER.H"
#include "AE_PRINTF.H"
#include "AE_COMMAND.H"
#include "KERNEL_SPIF.H"
#include "CUSTOM_POWER.H"
#include "KERNEL_MEMORY.H"
#include "KERNEL_RTC.H"

#define MOUDLE_TEST 0
#if ENABLE_DEBUGGER_SUPPORT
extern char Uart_buffer[UART_BUFFER_SIZE]; // An array of data transferred by the debugger
#endif
BYTE intr_st0, intr_st1, intr_st2, intr_st3, intr_st4, intr_st5, intr_st6, intr_st7;
BYTE intr_st;
/***************weak声明*********************/
void __interrupt SECTION(".interrupt.CPUS_HANDLER") CPUS_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.CPUT_HANDLER") CPUT_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.CPUR_HANDLER") CPUR_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.SWUC_HANDLER") SWUC_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.LPC_ESPI_RST_HANDLER") LPC_ESPI_RST_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.PWRSW_HANDLER") PWRSW_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.PS2_0_HANDLER") PS2_0_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.KBS_SDV_HANDLER") KBS_SDV_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.KBS_PRESS_HANDLER") KBS_PRESS_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.TACH0_HANDLER") TACH0_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.TACH1_HANDLER") TACH1_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.TACH2_HANDLER") TACH2_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.TACH3_HANDLER") TACH3_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.KBC_IBF_HANDLER") KBC_IBF_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.KBC_OBE_HANDLER") KBC_OBE_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.PMC1_IBF_HANDLER") PMC1_IBF_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.PMC1_OBE_HANDLER") PMC1_OBE_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.WU42_HANDLER") WU42_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.RTC_HANDLER") RTC_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.WDT_HANDLER") WDT_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.ADC_HANDLER") ADC_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.UART0_HANDLER") UART0_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.UARTA_HANDLER") UARTA_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.UARTB_HANDLER") UARTB_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.SM_EC_HANDLER") SM_EC_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.SM_HOST_HANDLER") SM_HOST_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.TIMER0_HANDLER") TIMER0_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.TIMER1_HANDLER") TIMER1_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.TIMER2_HANDLER") TIMER2_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.TIMER3_HANDLER") TIMER3_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.INTR0_HANDLER") INTR0_HANDLER(void) __weak;
void __interrupt SECTION(".interrupt.INTR1_HANDLER") INTR1_HANDLER(void) __weak;
/***************weak声明*********************/
/*----------------------------------LINE------------------------------------*/
void irqc_enable_interrupt(uint32_t source)
{
	set_csr(0xBD1, 1 << source);
}
void irqc_disable_interrupt(uint32_t source)
{
	clear_csr(0xBD1, 1 << source);
}
void irqc_set_pending(uint32_t source)
{
	set_csr(0xBD0, 1 << source);
}
void irqc_clear_pending(uint32_t source)
{
	clear_csr(0xBD0, 1 << source);
}
void irqc_set_level_trig(uint32_t source)
{
	set_csr(0xBD2, 1 << source);
}
void irqc_set_posedge_trig(uint32_t source)
{
	clear_csr(0xBD2, 1 << source);
	clear_csr(0xBD3, 1 << source);
}
void irqc_set_negedge_trig(uint32_t source)
{
	clear_csr(0xBD2, 1 << source);
	set_csr(0xBD3, 1 << source);
}
void enable_irq(uint32_t source, uint32_t lvl, uint32_t edge)
{
	irqc_enable_interrupt(source);
	if (lvl)
		irqc_set_level_trig(source);
	else if (edge)
		irqc_set_negedge_trig(source);
	else
		irqc_set_posedge_trig(source);
}
void disable_irq(uint32_t source, uint32_t lvl, uint32_t edge)
{
	UNUSED_VAR(edge); UNUSED_VAR(lvl);
	irqc_disable_interrupt(source);
}
// Set CPU CSR IRQCIE IRQCLVL and IRQCEDGE
void cpu_irq_en(void)
{
	// Have to enable the interrupt both at the GPIO level,
	// and at the IRQC level.
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_CPUS, en, 0, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_CPUT, en, 0, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_CPUR, en, 0, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_SWUC, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_LPCRST, dis, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_PWRSW, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_PS2_0, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_KBS_SDV, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_KBS_PRESS, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_TACH0, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_TACH1, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_TACH2, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_TACH3, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_KBC_IBF, en, 0, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_KBC_OBE, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_PMC1_IBF, en, 0, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_PMC1_OBE, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_WU42, en, 0, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_RTC, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_WDT, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_ADC, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_UART0, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_UARTA, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_UARTB, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_SM_EC, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_SM_HOST, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_TIMER0, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_TIMER1, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_TIMER2, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_TIMER3, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_INTR0, en, 1, 0);
	CSR_IRQC_CONFIG(IRQC_INT_DEVICE_INTR1, en, 1, 0);
}
/*
 * IRQC Init about intc moudles and CPU IRQ
 */
void SECTION(".init.irq") Irqc_init(void)
{
#if IRQC_MODULE_EN
	// Set Interrupt controller
	intc_init();
	// Set CPU CSR IRQ
	cpu_irq_en();
	// Enable interrupts in general.
	set_csr(mstatus, MSTATUS_MIE);
#endif
}
#if IRQC_DEBUG
char __weak* irq_string = "ISR: %s,IRQ: %d.\n";
#endif
void __interrupt SECTION(".interrupt.CPUS_HANDLER") CPUS_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[0]++;
#endif
	irqprint(irq_string, __FUNCTION__, 0);
};
void __interrupt SECTION(".interrupt.CPUT_HANDLER") CPUT_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[1]++;
#endif
	irqprint(irq_string, __FUNCTION__, 1);
#if SUPPORT_CPUTIMER_WAKEUP
	if (Low_Power_Flag)
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
void __interrupt SECTION(".interrupt.CPUR_HANDLER") CPUR_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[2]++;
#endif
	irqprint(irq_string, __FUNCTION__, 2);
};
void __interrupt SECTION(".interrupt.SWUC_HANDLER") SWUC_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[3]++;
#endif
	irqprint(irq_string, __FUNCTION__, 3);
	irqprint("SWCHSTR status is %#x\n", SWUSTA);
#if 1
	if (SWUSTA & 0x80)
	{
		SWUSTA |= 0x80;
		irqprint("This is Logic Device Module trig interrupt!\n");
	}
	else if (SWUSTA & 0x40)
	{
		if (!(SWUCTL1 & 0x20))
		{
			irqprint("This is Soft Event trig interrupt! HOST Clear!\n");
		}
		else
		{
			SWUSTA |= 0x40;
			irqprint("This is Soft Event trig interrupt! EC Clear!\n");
		}
	}
	else if (SWUSTA & 0x8)
	{
		SWUSTA |= 0x8;
		irqprint("This is RING signal trig interrupt!\n");
	}
	else if (SWUSTA & 0x2)
	{
		SWUSTA |= 0x2;
		irqprint("This is RI2 signal trig interrupt!\n");
	}
	else if (SWUSTA & 0x1)
	{
		SWUSTA |= 0x1;
		irqprint("This is RI1 signal trig interrupt!\n");
	}
	else if ((SWUCTL2 & 0x40) && (SWUIE & 0x40))
	{
		irqprint("This is legacy mode way scrdpbm_ec trig interrupt!\n");
		SWUCTL2 |= 0x40;
	}
	else if ((SWUCTL2 & 0x80) && (SWUIE & 0x80))
	{
		irqprint("This is legacy mode way scrdpso_ec trig interrupt!\n");
		SWUCTL2 |= 0x80;
	}
	else if ((SWUCTL2 & (0x02)))
	{
		irqprint("SWCTL2 data is %#x!\n", SWUCTL2);
		irqprint("This is ACPI S1 request interrupt!\n");
		(SWUCTL2) |= 0x02;
	}
	else if ((SWUCTL2 & (0x04)))
	{
		irqprint("SWCTL2 data is %#x!\n", SWUCTL2);
		irqprint("This is ACPI S2 request interrupt!\n");
		(SWUCTL2) |= 0x04;
	}
	else if ((SWUCTL2 & (0x08)))
	{
		irqprint("SWCTL2 data is %#x!\n", SWUCTL2);
		irqprint("This is ACPI S3 request interrupt!\n");
		SWUCTL2 |= 0x08;
	}
	else if ((SWUCTL2 & (0x10)))
	{
		irqprint("SWCTL2 data is %#x!\n", SWUCTL2);
		irqprint("This is ACPI S4 request interrupt!\n");
		SWUCTL2 |= 0x10;
	}
	else if ((SWUCTL2 & (0x20)))
	{
		irqprint("SWCTL2 data is %#x!\n", SWUCTL2);
		irqprint("This is ACPI S5 request interrupt!\n");
		SWUCTL2 |= 0x20;
	}
#endif
}
void __interrupt SECTION(".interrupt.LPC_ESPI_RST_HANDLER") LPC_ESPI_RST_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[4]++;
#endif
	irqprint(irq_string, __FUNCTION__, 4);
	SYSCTL_RST1 |= (0x0 << 18);
};
void __interrupt SECTION(".interrupt.PWRSW_HANDLER") PWRSW_HANDLER(void)
{
#if (SYSCTL_CLOCK_EN)
	SYSCTL_PWRSWCSR |= 0x20; // clear interrupt status flag
#endif
#if SUPPORT_PWRSW_WAKEUP
	SYSCTL_MODEN1 |= DRAM_EN | SYSCTL_EN;//回复现场会配置sysctl和dram
#if DEBUG
	SYSCTL_MODEN1 |= APB_EN;//uart挂在apb下
	SYSCTL_MODEN0 |= ((UARTA_EN >> ((!(PRINTF_UART_SWITCH & 0b100)) << 1)) >> (PRINTF_UART_SWITCH & 0b11));//优先开启调试功能
#else
#endif
	nop;
	Exit_LowPower_Mode();
#endif
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[5]++;
#endif
	irqprint(irq_string, __FUNCTION__, 5);
	printf("PWRSW\n");
};
void __interrupt SECTION(".interrupt.PS2_0_HANDLER") PS2_0_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[6]++;
#endif
	irqprint(irq_string, __FUNCTION__, 6);
	if (MS_Main_CHN == 1)
		Handle_Mouse(MS_Main_CHN - 1);
	else if (KB_Main_CHN == 1)
		Handle_Kbd(KB_Main_CHN - 1);
};
void __interrupt SECTION(".interrupt.KBS_SDV_HANDLER") KBS_SDV_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[7]++;
#endif
	irqprint(irq_string, __FUNCTION__, 7);
#if SUPPORT_KBS_WAKEUP
	if (Low_Power_Flag)
	{
		if (SystemIsS3)
			Exit_LowPower_Mode(); // support KBS wake cpu up when S3
	}
#endif
	// if(KBS_KSDSR & BIT0)
	F_Service_KBS = 1;
	KBS_KSDSR = BIT0;
};
void __interrupt SECTION(".interrupt.KBS_PRESS_HANDLER") KBS_PRESS_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[8]++;
#endif
	irqprint(irq_string, __FUNCTION__, 8);
	//kbs_init();
	// KayBoardScan_config();
	// KBS_KSDC1R = KBS_KSDC1R_EN | KBS_KSDC1R_INT_EN;
	//CSR_IRQC_CONFIG(IRQC_INT_DEVICE_KBS_SDV, en, 1, 0);
};
void __interrupt SECTION(".interrupt.TACH0_HANDLER") TACH0_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[9]++;
#endif
	irqprint(irq_string, __FUNCTION__, 9);
	TACH0_Speed = TACH_Get(0);
	TACH_Clear_IRQ(0);
	TACH0_Speed = 0x5b8d80 / TACH0_Speed;
	irqprint("FanRPM_Tach0 is %d r/min  \n", TACH0_Speed);
	TACH_Int_Disable(0);
};
void __interrupt SECTION(".interrupt.TACH1_HANDLER") TACH1_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[10]++;
#endif
	irqprint(irq_string, __FUNCTION__, 10);
	TACH1_Speed = TACH_Get(1);
	TACH_Clear_IRQ(1);
	TACH1_Speed = 0x5b8d80 / TACH1_Speed;
	irqprint("FanRPM_Tach1 is %d r/min \n", TACH1_Speed);
	TACH_Int_Disable(1);
};
void __interrupt SECTION(".interrupt.TACH2_HANDLER") TACH2_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[11]++;
#endif
	irqprint(irq_string, __FUNCTION__, 11);
	TACH2_Speed = TACH_Get(2);
	TACH_Clear_IRQ(2);
	TACH2_Speed = 0x5b8d80 / TACH2_Speed;
	TACH_Int_Disable(2);
};
void __interrupt SECTION(".interrupt.TACH3_HANDLER") TACH3_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[12]++;
#endif
	irqprint(irq_string, __FUNCTION__, 12);
	TACH3_Speed = TACH_Get(3);
	TACH_Clear_IRQ(3);
	TACH3_Speed = 0x5b8d80 / TACH3_Speed;
	TACH_Int_Disable(3);
};
void __interrupt SECTION(".interrupt.KBC_IBF_HANDLER") KBC_IBF_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[13]++;
#endif
	irqprint(irq_string, __FUNCTION__, 13);
	irqc_disable_interrupt(IRQC_INT_DEVICE_KBC_IBF); // disable kbc ibf int
	F_Service_PCI = 1;
};
void __interrupt SECTION(".interrupt.KBC_OBE_HANDLER") KBC_OBE_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[14]++;
#endif
	irqprint(irq_string, __FUNCTION__, 14);
	irqc_disable_interrupt(IRQC_INT_DEVICE_KBC_OBE);
	// handle_kbd_event();
};
void __interrupt SECTION(".interrupt.PMC1_IBF_HANDLER") PMC1_IBF_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[15]++;
#endif
	irqprint(irq_string, __FUNCTION__, 15);
	F_Service_PCI2 = 1;
	PMC1_CTL &= ~IBF_INT_ENABLE; // 清除中断，读走以后再打开
	// Service_PCI2_Main();
};
void __interrupt SECTION(".interrupt.PMC1_OBE_HANDLER") PMC1_OBE_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[16]++;
#endif
	irqprint(irq_string, __FUNCTION__, 16);
	PMC1_DOR = 0x5a;
#if ENABLE_DEBUGGER_SUPPORT
	Debugger_KBC_PMC_Record(1, 1, 0x5a);
#endif
};
void __interrupt SECTION(".interrupt.WU42_HANDLER") WU42_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[17]++;
#endif
	irqprint(irq_string, __FUNCTION__, 17);
	/* irqprint("WU42_HANDLER\n");
	irqprint("mepc is %#x\n",read_csr(mepc));//指针
	irqprint("mcause is %#x\n",read_csr(mcause));//控制位
	irqprint("irqcip is %#x\n",read_csr(0xBD0));//中断源等待
	irqprint("irqcie is %#x\n",read_csr(0xBD1));//中断源使能
	irqprint("irqclvl is %#x\n",read_csr(0xBD2));//中断源电平触发
	irqprint("irqcedge is %#x\n",read_csr(0xBD3));//中断源边沿触发
	irqprint("irqcinfo is %d\n",0x3f&read_csr(0xBD4));//中断源个数 */
};

void __interrupt SECTION(".interrupt.RTC_HANDLER") RTC_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[18]++;
#endif
#if SUPPORT_RTC_WAKEUP
	SYSCTL_MODEN1 |= 0xffffffff;//all open
	SYSCTL_MODEN0 |= 0xffffffff;//all open
#endif
	irqprint(irq_string, __FUNCTION__, 18);
	Set_RTC_MatchVal(Get_RTC_CountVal() + 1);
	RTC_EOI0;//clear interrupt
	printf("RTC\n");
};

void __interrupt SECTION(".interrupt.WDT_HANDLER") WDT_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[19]++;
#endif
	irqprint(irq_string, __FUNCTION__, 19);
	if (WDT_FLAG)
	{
		WDT_Clear_IRQ(); // 重启则清除中断
	}
	else
	{
		WDT_FeedDog();// 不重启则喂狗
	}
};
#define TEST_10TIMES 0
void __interrupt SECTION(".interrupt.ADC_HANDLER") ADC_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[20]++;
#endif
	irqprint(irq_string, __FUNCTION__, 20);
	BYTE ADC_ValidStatus_1 = ADC_INTSTAT & 0xff;
	BYTE ADC_ValidStatus_2 = (ADC_INTSTAT >> 8) & 0xff;
	// dprint("ADC_INTSTAT_int:%x\n",ADC_INTSTAT);
	// dprint("ADC_ValidStatus_1 is 0x%x\n",ADC_ValidStatus_1);
	// dprint("ADC_ValidStatus_2 is 0x%x\n",ADC_ValidStatus_2);
	for (short i = 0; i < 8; i++)
	{
		if (ADC_ValidStatus_1 & (1 << i))
		{
			// printf("i:======%x\n",i);
			(&ADC_Data0)[i] = ADC_ReadData(i);
			ADC_INTSTAT |= 1 << i;		//清除中断
			dprint("ADC%d:0x%x\n", i, (&ADC_Data0)[i]);
		}
		if (ADC_ValidStatus_2 & (1 << i))
		{
			if (i < 3 && i >= 0)
			{
				ADC_INTSTAT |= 1 << (i + 8);		//清除中断
				printf("compare%d: compare failed\n", i);
			}
		}
	}
#if TEST_10TIMES
	if ((((ADC_CTRL >> 7) & 0x3) == 0x2) || (((ADC_CTRL >> 7) & 0x3) == 0x3) || (((ADC_CTRL >> 7) & 0x3) == 0x1))//连续触发 auto samp
		// if((((ADC_CTRL>>7)&0x3)==0x2))
	{
		//在此测试连续，
		if (counter_1 < 10)
		{
			counter_1++;
			printf("counter_1:%x\n", counter_1);
		}
		else
		{
			counter_1 = 0;
			ADC__TriggerMode_Config(SW_SAMPLE); //停止连续模式
		}
	}
#endif
}
void __interrupt SECTION(".interrupt.UART0_HANDLER") UART0_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[21]++;
#endif
#if ((DEBUG_UART_SWITCH == 0) || (PRINTF_UART_SWITCH == 0) || (COMMAND_UART_SWITCH == 0))
#else
	irqprint(irq_string, __FUNCTION__, 21);
#endif
	register u8 iir = (UART0_IIR & 0xf);
	if (iir == UART_IIR_RLS)//奇偶校验、溢出、格式错误、失效中断
	{
		register u8 lsr = UART0_LSR;
		if (lsr & (UART_LSR_BI | UART_LSR_FE | UART_LSR_PE))//打断传输,格式错误，奇偶校验，
		{
			UART0_RX;//读出异常值
			irqprint("Receive error\n");//报错
			SYSCTL_PIO0_UDCFG |= (1 << 24);
		}//接收错误

	}
#if (ENABLE_COMMAND_SUPPORT && COMMAND_UART_SWITCH == 0)
	if (F_Service_CMD == 1)
	{
		char temp = UART0_RX;
#if (!IRQC_DEBUG)
		UNUSED_VAR(temp);
#endif
		irqprint("erro of CMD_RUN:%#x,%c\n", temp, temp);
		return;
	}
	CMD_UART_BUFF[CMD_UART_CNT] = UART0_RX;
	if (CMD_UART_BUFF[CMD_UART_CNT] == '\n' || CMD_UART_CNT == CMD_BUFF_MAX)
	{
		CMD_UART_BUFF[CMD_UART_CNT] = '\0';
		if (CMD_UART_BUFF[CMD_UART_CNT - 1] == ' ')
		{
			CMD_UART_CNT--;
			CMD_UART_BUFF[CMD_UART_CNT] = '\0';
		}
		F_Service_CMD = 1;
	}
	else if (CMD_UART_BUFF[CMD_UART_CNT] == '\r')
		CMD_UART_BUFF[CMD_UART_CNT] = '\0';
	else
		CMD_UART_CNT++;
#endif
	/* Debugger Function Entry */
#if ENABLE_DEBUGGER_SUPPORT
#if DEBUGGER_OUTPUT_SWITCH == 0
#if DEBUG_UART_SWITCH == 0
	char temp = UART0_RX;
	Uart_buffer[Uart_Rx_index++] = temp;
	Uart_Rx_index %= UART_BUFFER_SIZE;
	Debugger_Cmd_IRQ(temp);
#endif
#endif
#endif
}
void __interrupt SECTION(".interrupt.UARTA_HANDLER") UARTA_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[22]++;
#endif
#if ((DEBUG_UART_SWITCH == 2) || (PRINTF_UART_SWITCH == 2) || (COMMAND_UART_SWITCH == 2))
#else
	irqprint(irq_string, __FUNCTION__, 22);
#endif
	register u8 iir = (UARTA_IIR & 0xf);
	if (iir == UART_IIR_RLS)//奇偶校验、溢出、格式错误、失效中断
	{
		register u8 lsr = UARTA_LSR;
		if (lsr & (UART_LSR_BI | UART_LSR_FE | UART_LSR_PE))//打断传输,格式错误，奇偶校验，
		{
			UARTA_RX;//读出异常值
			irqprint("Receive error\n");//报错
			SYSCTL_PIO0_UDCFG |= (1 << 8);
		}//接收错误

	}
#if (ENABLE_COMMAND_SUPPORT && COMMAND_UART_SWITCH == 2)
	if (F_Service_CMD == 1)
	{
		char temp = UARTA_RX;
#if (!IRQC_DEBUG)
		UNUSED_VAR(temp);
#endif
		irqprint("erro of CMD_RUN:%#x,%c\n", temp, temp);
		return;
	}
	CMD_UART_BUFF[CMD_UART_CNT] = UARTA_RX;
	if (CMD_UART_BUFF[CMD_UART_CNT] == '\n' || CMD_UART_CNT == CMD_BUFF_MAX)
	{
		CMD_UART_BUFF[CMD_UART_CNT] = '\0';
		if (CMD_UART_BUFF[CMD_UART_CNT - 1] == ' ')
		{
			CMD_UART_CNT--;
			CMD_UART_BUFF[CMD_UART_CNT] = '\0';
		}
		F_Service_CMD = 1;
	}
	else if (CMD_UART_BUFF[CMD_UART_CNT] == '\r')
		CMD_UART_BUFF[CMD_UART_CNT] = '\0';
	else
		CMD_UART_CNT++;
#endif
	/* Debugger Function Entry */
#if ENABLE_DEBUGGER_SUPPORT
#if DEBUGGER_OUTPUT_SWITCH == 0
#if DEBUG_UART_SWITCH == 4
	char temp = UARTA_RX;
	Uart_buffer[Uart_Rx_index++] = temp;
	Uart_Rx_index %= UART_BUFFER_SIZE;
	Debugger_Cmd_IRQ(temp);
#endif
#endif
#endif
	uart_crtpram_updatebuffer[uart_crypram_updateindex] = UARTA_RX;
	printf("%d rx:%x\n", uart_crypram_updateindex, uart_crtpram_updatebuffer[uart_crypram_updateindex]);
	uart_crypram_updateindex++;
	if ((uart_crtpram_updatebuffer[0] == 0x64) && (uart_crypram_updateindex >= 12))
	{
		printf("crt update\n");
		if (memcmp(update_crypram_cmd, uart_crtpram_updatebuffer, sizeof(update_crypram_cmd)) == 0)
		{
			update_crypram_flag = 1;
			uart_crypram_updateindex = 0;
			uart_crtpram_updatebuffer[0] = 0;
		}
	}
	else if (uart_crtpram_updatebuffer[0] == 0x75 && (uart_crypram_updateindex >= 16))
	{
		printf("intf update\n");
		if (memcmp(update_extflash_cmd, uart_crtpram_updatebuffer, sizeof(update_extflash_cmd)) == 0)
		{
			update_intflash_flag = 1;
			uart_crypram_updateindex = 0;
			uart_crtpram_updatebuffer[0] = 0;
		}
	}
	else if ((uart_crtpram_updatebuffer[0] != 0x64) && (uart_crtpram_updatebuffer[0] != 0x75))
	{
		uart_crypram_updateindex = 0;
		update_crypram_flag = 0;
		update_intflash_flag = 0;
	}

}
void __interrupt SECTION(".interrupt.UARTB_HANDLER") UARTB_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[23]++;
#endif
#if ((DEBUG_UART_SWITCH == 3) || (PRINTF_UART_SWITCH == 3) || (COMMAND_UART_SWITCH == 3))
#else
	irqprint(irq_string, __FUNCTION__, 23);
#endif
	register u8 iir = (UARTB_IIR & 0xf);
	if (iir == UART_IIR_RLS)//奇偶校验、溢出、格式错误、失效中断
	{
		register u8 lsr = UARTB_LSR;
		if (lsr & (UART_LSR_BI | UART_LSR_FE | UART_LSR_PE))//打断传输,格式错误，奇偶校验，
		{
			UARTB_RX;//读出异常值
			irqprint("Receive error\n");//报错
			SYSCTL_PIO1_UDCFG |= (1 << 25);
		}//接收错误

	}
#if (ENABLE_COMMAND_SUPPORT && COMMAND_UART_SWITCH == 3)
	if (F_Service_CMD == 1)
	{
		char temp = UARTB_RX;
#if (!IRQC_DEBUG)
		UNUSED_VAR(temp);
#endif
		irqprint("erro of CMD_RUN:%#x,%c\n", temp, temp);
		return;
	}
	CMD_UART_BUFF[CMD_UART_CNT] = UARTB_RX;
	if (CMD_UART_BUFF[CMD_UART_CNT] == '\n' || CMD_UART_CNT == CMD_BUFF_MAX)
	{
		CMD_UART_BUFF[CMD_UART_CNT] = '\0';
		if (CMD_UART_BUFF[CMD_UART_CNT - 1] == ' ')
		{
			CMD_UART_CNT--;
			CMD_UART_BUFF[CMD_UART_CNT] = '\0';
		}
		F_Service_CMD = 1;
	}
	else if (CMD_UART_BUFF[CMD_UART_CNT] == '\r')
		CMD_UART_BUFF[CMD_UART_CNT] = '\0';
	else
		CMD_UART_CNT++;
#endif
	/* Debugger Function Entry */
#if ENABLE_DEBUGGER_SUPPORT
#if DEBUGGER_OUTPUT_SWITCH == 0
#if DEBUG_UART_SWITCH == 5
	char temp = UARTB_RX;
	Uart_buffer[Uart_Rx_index++] = temp;
	Uart_Rx_index %= UART_BUFFER_SIZE;
	Debugger_Cmd_IRQ(temp);
#endif
#endif
#endif
}
void __interrupt SECTION(".interrupt.SM_EC_HANDLER") SM_EC_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[24]++;
#endif
	irqprint(irq_string, __FUNCTION__, 24);
};
void __interrupt SECTION(".interrupt.SM_HOST_HANDLER") SM_HOST_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[25]++;
#endif
	irqprint(irq_string, __FUNCTION__, 25);
};
void __interrupt SECTION(".interrupt.TIMER0_HANDLER") TIMER0_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[26]++;
#endif
#if 0
	irqprint(irq_string, __FUNCTION__, 26);
#endif

#if MOUDLE_TEST
	int static num = 0;
	if ((GPIO1_DR0) & (0x1 << 0))//pb0翻转
	{
		GPIO1_DR0 &= (0x0 << 0);//GPIO输出寄存器原为1时清0
	}
	else
	{
		GPIO1_DR0 |= (0x1 << 0);//GPIO输出寄存器原为0时置1
	}
	if (num++ == 100)
	{
		num = 0;
		printf("TIMER0\n");
	}
#endif

	if ((TIMER0_TIS & 0x1) == 0x1)
		TIMER0_TEOI; // clear int
};
void __interrupt SECTION(".interrupt.TIMER1_HANDLER") TIMER1_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[27]++;
#endif
#if 0
	irqprint(irq_string, __FUNCTION__, 27);
#endif

#if MOUDLE_TEST
	int static num = 0;
	if ((GPIO1_DR0) & (0x1 << 1))//pb1翻转
	{
		GPIO1_DR0 &= (0x0 << 1);//GPIO输出寄存器原为1时清0
	}
	else
	{
		GPIO1_DR0 |= (0x1 << 1);//GPIO输出寄存器原为0时置1
	}
	if (num++ == 100)
	{
		num = 0;
		printf("TIMER1\n");
	}
#endif

	if (TIMER1_TIS & 0x1) // read int status
	{
		TIMER1_TEOI; // clear int
	}
};
void __interrupt SECTION(".interrupt.TIMER2_HANDLER") TIMER2_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[28]++;
#endif
#if 0
	irqprint(irq_string, __FUNCTION__, 28);
#endif

#if MOUDLE_TEST
	// if((GPIO1_DR0)&(0x1<<2))//pb1翻转
	// {
	// 	GPIO1_DR0 &=(0x0<<2);//GPIO输出寄存器原为1时清0
	// }
	// else
	// {
	// 	GPIO1_DR0 |=(0x1<<2);//GPIO输出寄存器原为0时置1
	// }
	// printf("TIMER2\n");
#endif

	// irqprint ("%s","--------Begin TIMER2 handler----Vector mode\n");
	if (TIMER2_TIS & 0x1)
	{
		TIMER2_TEOI;
	}
	if (!(F_Service_MS_1))
	{
		F_Service_MS_1 = 1; // Request 5 mS timer service.
	}
};
void __interrupt SECTION(".interrupt.TIMER3_HANDLER") TIMER3_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[29]++;
#endif
#if 0
	irqprint(irq_string, __FUNCTION__, 29);
#endif

#if MOUDLE_TEST
	int static num = 0;
	if ((GPIO1_DR0) & (0x1 << 3))//pb1翻转
	{
		GPIO1_DR0 &= (0x0 << 3);//GPIO输出寄存器原为1时清0
	}
	else
	{
		GPIO1_DR0 |= (0x1 << 3);//GPIO输出寄存器原为0时置1
	}
	if (num++ == 100)
	{
		num = 0;
		printf("TIMER3\n");
	}
#endif

	// irqprint ("%s","--------Begin TIMER3 handler----Vector mode\n");
	if (TIMER3_TIS & 0x1)
	{
		TIMER3_TEOI;
	}
};
/*Intr0*/
void __interrupt SECTION(".interrupt.INTR0_HANDLER") INTR0_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[30]++;
#endif
	int i = 0;
	int j = 0;
	int num = 0;
	int flag = 0;
	for (i = 0; i < 8; i++)
	{
		intr_st = REG8(INTC0_REG_ADDR(INTC_ICTL_MASKSTATUS_L + i));
		for (j = 0; j < 8; j++)
		{
			if (intr_st & (1 << j))
			{
				num = i * 8 + j;
				flag = 1;
				break;
			}
		}
		if (flag == 1)
			break;
	}
	irqprint("ISR: %s,IRQ: %d. 2nd-level IRQ[%d]\n", __FUNCTION__, 30, num);
	(intr0_service[(num)])(); // Dispatch to service handler.
}
/*Intr1*/
void __interrupt SECTION(".interrupt.INTR1_HANDLER") INTR1_HANDLER(void)
{
#if ENABLE_DEBUGGER_SUPPORT
	Intr_num[31]++;
#endif
	int i = 0;
	int j = 0;
	int num = 0;
	int flag = 0;
	for (i = 0; i < 8; i++)
	{
		intr_st = REG8(INTC1_REG_ADDR(INTC_ICTL_MASKSTATUS_L + i));
		for (j = 0; j < 8; j++)
		{
			if (intr_st & (1 << j))
			{
				num = i * 8 + j;
				flag = 1;
				break;
			}
		}
		if (flag == 1)
			break;
	}
#if ((DEBUG_UART_SWITCH == 1))
	if (intr1_service[(num)] == &intr1_uart1)
	{
	}
	// #elif ((DEBUG_UART_SWITCH == 2))
	// 	if(intr1_service[(num)] == &intr1_uart2)
	// 	{
	// 	}
	// #elif ((DEBUG_UART_SWITCH == 3))
	// 	if(intr1_service[(num)] == &intr1_uart3)
	// 	{
	// 	}
#else
	if (FALSE)
	{
	}
#endif
#if ((COMMAND_UART_SWITCH == 1))
	else if (intr1_service[(num)] == &intr1_uart1)
	{
	}
	// #elif ((COMMAND_UART_SWITCH == 2))
	// 	else if(intr1_service[(num)] == &intr1_uart2)
	// 	{
	// 	}
	// #elif ((COMMAND_UART_SWITCH == 3))
	// 	else if(intr1_service[(num)] == &intr1_uart3)
	// 	{
	// 	}
#else
	else if (FALSE)
	{
	}
#endif
#if ((PRINTF_UART_SWITCH == 1))
	else if (intr1_service[(num)] != &intr1_uart1)
		// #elif ((PRINTF_UART_SWITCH == 2))
		// else if(intr1_service[(num)] != &intr1_uart2)
		// #elif ((PRINTF_UART_SWITCH == 3))
		// else if(intr1_service[(num)] != &intr1_uart3)
		// #else
	else if (TRUE)
#endif
	{
		irqprint("ISR: %s,IRQ: %d. 2nd-level IRQ[%d]\n", __FUNCTION__, 31, num);
	}
	(intr1_service[(num)])(); // Dispatch to service handler.
}
const char* Exception_Code[] = { NULL,
								"Instruction access error",
								"Illegal instruction",
								"Break point",
								"Load address misaligned",
								"Load access error",
								"Store address misaligned",
								"Store access error",
								NULL, NULL, NULL,
								"Machine mode environment call" };
DWORD SECTION(".interrupt.kernel_trap") kernel_trap(uintptr_t mcause, uintptr_t sp)
{
	UNUSED_VAR(sp);
	VBYTE trap_flag = 0x1f & mcause;
	void* pc = (void*)read_csr(mepc);
	DWORD val;
	if ((int)pc % 2)
	{
		val = *(BYTEP)pc;
	}
	else if ((int)pc % 4)
	{
		val = *(WORDP)pc;
	}
	else
	{
		val = *(DWORDP)pc;
	}
	F_Service_PUTC = 1;
	dprint(" \ntrap\nmcause is %d:%s\n mepc is 0x%lx,val is %#x\n", trap_flag, Exception_Code[trap_flag], read_csr(mepc), val);
	F_Service_PUTC = 0;
	_exit(mcause);
	return 0;
}
