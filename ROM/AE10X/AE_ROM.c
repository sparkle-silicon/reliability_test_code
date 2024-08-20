/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-07-19 15:34:04
 * @Description:
 * 				Note :It's AE103 funcion
 * 				first update of rom code Burn through in flash and run
 * 				second .lds flash addr set rom addr(rom code Size less than 32KB) and remake
 * 				thirdly send code at uart to ROM and run
 * 				Finally, the rom is used to boot
 *
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 以下是中文及英文版权同步声明，编码为UTF-8。
 * Copyright has legal effects and violations will be prosecuted.
 * 版权具有法律效力，违反必究。
 *
 * Copyright ©2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2023龙晶石半导体科技（苏州）有限公司
 */
#include <AE_ROM.H>
#ifdef AE103
#elif 0
SECTION(".ROM.var.magic_number")
USED BYTE magic_number[8] = "103\036SPK\0";
// 模块执行函数
#define rom_wdt_feed() WDT_CRR = WDT_CRR_CRR; // wdt 喂狗
void OPTIMIZE0 SECTION(".ROM.Func.putc") USED rom_putc(register BYTE ch) // 字符输出
{
	if(rom_dataONram.debug_uart_en)
	{
		while(!(REG8(REG_ADDR((DWORD)UART_PTR, UART_LSR_OFFSET)) & UART_LSR_TEMP))
			rom_wdt_feed();
		REG8(REG_ADDR((DWORD)UART_PTR, UART_THR_OFFSET)) = ch;
	}
}
void OPTIMIZE0 SECTION(".ROM.Func.puts") USED rom_puts(register char *s) // 字符串输出
{
	while(*s)
	{
		rom_putc(*s++);
	}
}
// 模块初始化函数
// 获取时钟频率
DWORD OPTIMIZE0 SECTION(".ROM.Func.freq") USED NOINLINE rom_get_cpu_freq(register BYTE num)
{
#define start_mtime ROM_TEMP0
#define delta_mtime ROM_TEMP1
#define temp_mtime ROM_TEMP2
#define start_mcycle ROM_TEMP3
#define delta_mcycle ROM_TEMP4
#define divA ROM_TEMP5
#define divB ROM_TEMP6
	asm volatile("csrci 0x320, 0x5\n"); // 开启计数器
	// Don't start measuruing until we see an mtime tick
	temp_mtime = read_csr(0xBDA); // 獲取計時器
	do
	{
		start_mtime = read_csr(0xBDA);
	}
	while(start_mtime == temp_mtime);
	start_mcycle = read_csr(mcycle);
	do
	{
		delta_mtime = read_csr(0xBDA) - start_mtime; // 进行一段时间计时
	}
	while(delta_mtime < num);
	delta_mcycle = read_csr(mcycle) - start_mcycle; // 判断时间周期
	divA = delta_mcycle;							// 周期除以计时
	divB = delta_mtime;
	while(divA) // delta_mcycle/delta_mtime
	{
		if(divA >= divB)
		{
			divA -= divB;
		}
		else
		{
			divB = divA;
			divA = 0;
		}
	}
	divA = delta_mcycle + divB;
	// divA<<=16;
	divB = delta_mtime;
	while(divA) // 周期加余数除以计时
	{
		if(divA >= divB)
		{
			divA -= divB;
			CPU_FREQ++;
		}
		else
		{
			divB = divA;
			divA = 0;
		}
	}
	// （delta_mcycle/delta_mtime+delta_mcycle%delta_mtime）/delta_mtime*32.576k*2
  //24M时钟周期/32k时钟周期*预定32k的频率+24M时钟周期%32k时钟周期*预定32k频率/32k时钟周期
	CPU_FREQ <<= 16;					// cpu频率
	asm volatile("csrsi 0x320, 0x5\n"); // 关闭计时器
	return CPU_FREQ;
#undef start_mtime
#undef delta_mtime
#undef temp_mtime
#undef start_mcycle
#undef delta_mcycle
#undef divA
#undef divB
}
// 初始化uart
void OPTIMIZE0 SECTION(".ROM.Func.uart") USED rom_uart(void) // uart初始化
{
#define uart_dl ROM_TEMP0
#define uart_divA ROM_TEMP1
#define uart_divB ROM_TEMP2
	SYSCTL_MODEN1 |= BIT(9); // sysctl 模块时钟使能
	if(rom_dataONram.debug_uart_com == 0)
	{ // 选择串口
		UART_PTR = UART0_BASE_ADDR;
		SYSCTL_MODEN0 |= BIT(8);
		SYSCTL_PIO1_CFG &= 0xfff0ffff; // 2425
		SYSCTL_PIO1_CFG |= 0x000a0000;
	}
	else if(rom_dataONram.debug_uart_com == 1)
	{
		UART_PTR = UART1_BASE_ADDR;
		SYSCTL_MODEN0 |= BIT(7);
		SYSCTL_PIO2_CFG &= 0xffffff33; // 13
		SYSCTL_PIO2_CFG |= 0x00000044;
	}
	else if(rom_dataONram.debug_uart_com == 2)
	{
		UART_PTR = UART2_BASE_ADDR;
		SYSCTL_MODEN0 |= BIT(6);
		SYSCTL_PIO3_CFG &= 0xf33fffff;
		SYSCTL_PIO3_CFG |= 0x04400000;
	}
	else if(rom_dataONram.debug_uart_com == 3)
	{
		UART_PTR = UART3_BASE_ADDR;
		SYSCTL_MODEN0 |= BIT(5);
		SYSCTL_PIO0_CFG &= 0xffCfffff;
		SYSCTL_PIO0_CFG |= 0x00200000;
		SYSCTL_PIO4_CFG &= 0xff3fffff;
		SYSCTL_PIO4_CFG |= 0x00400000;
	}
	else if(rom_dataONram.debug_uart_com == 4)
	{
		UART_PTR = UARTA_BASE_ADDR;
		SYSCTL_MODEN0 |= BIT(10);
		SYSCTL_PIO0_CFG &= 0xfff0ffff;
		SYSCTL_PIO0_CFG |= 0x000a0000;
	}
	else if(rom_dataONram.debug_uart_com == 5)
	{
		UART_PTR = UARTB_BASE_ADDR;
		SYSCTL_MODEN0 |= BIT(9);
		SYSCTL_PIO3_CFG &= 0xffC3ffff;
		SYSCTL_PIO3_CFG |= 0x00280000;
	}
	else
	{
		rom_dataONram.debug_uart_en = 0;
		return;
	}
	REG8(REG_ADDR(UART_PTR, UART_LCR_OFFSET)) = UART_LCR_DLAB | UART_LCR_DLS_8bit; // 串口配置
	uart_divA = CPU_FREQ;														   // 默认CPU频率
	uart_divB = (rom_dataONram.debug_uart_baud << 3);
	uart_dl = 0;
	while(uart_divA) // CPU_FREQ/(BAUD <<3)
	{
		if(uart_divA >= uart_divB)
		{
			uart_divA -= uart_divB;
			uart_dl++;
		}
		else
		{
			uart_divA <<= 1;
			if(uart_divB <= uart_divA)
				uart_dl++;
			uart_divB = uart_divA >> 1;
			uart_divA = 0;
		}
	}
	if(rom_dataONram.debug_uart_com > 3)
		uart_dl >>= 1;
	// uart_dl=27;
	REG8(REG_ADDR(UART_PTR, UART_DLL_OFFSET)) = uart_dl;
	REG8(REG_ADDR(UART_PTR, UART_DLH_OFFSET)) = (uart_dl >> 8);
	REG8(REG_ADDR(UART_PTR, UART_LCR_OFFSET)) = UART_LCR_DATA8BIT;
	REG8(REG_ADDR(UART_PTR, UART_FCR_OFFSET)) = UART_FCR_RFR | UART_FCR_FIFOEN | UART_FCR_TFR;
#undef uart_dl
#undef uart_divA
#undef uart_divB
}
void OPTIMIZE0 SECTION(".ROM.Func.ejtag") USED rom_ejtag(void) // ejtag 初始化
{
	SYSCTL_MODEN1 |= BIT(9);
	if(rom_dataONram.jtag_tdi == 0) // GPA7
		SYSCTL_PIO0_CFG |= 0b11 << 14;
	else if(rom_dataONram.jtag_tdi == 1) // GPF5
		SYSCTL_PIO2_CFG |= 0b11 << 26;
	else if(rom_dataONram.jtag_tdi == 2) // GPH5
		SYSCTL_PIO3_CFG |= 0b11 << 26;
	else if(rom_dataONram.jtag_tdi == 3) // GPJ1
		SYSCTL_PIO4_CFG |= 0b11 << 18;
	else if(rom_dataONram.jtag_tdi == 4) // GPJ5
		SYSCTL_PIO4_CFG |= 0b11 << 26;
	else if(rom_dataONram.jtag_tdi == 5) // GPP6
		SYSCTL_PIO5_CFG |= 0b11 << 18;
	if(rom_dataONram.jtag_tdo == 0) // GPB2
		SYSCTL_PIO0_CFG |= 0b11 << 20;
	else if(rom_dataONram.jtag_tdo == 1) // GPF4
		SYSCTL_PIO2_CFG |= 0b11 << 24;
	else if(rom_dataONram.jtag_tdo == 2) // GPH3
		SYSCTL_PIO3_CFG |= 0b11 << 22;
	else if(rom_dataONram.jtag_tdo == 3) // GPJ0
		SYSCTL_PIO4_CFG |= 0b11 << 16;
	else if(rom_dataONram.jtag_tdo == 4) // GPJ4
		SYSCTL_PIO4_CFG |= 0b11 << 24;
	else if(rom_dataONram.jtag_tdo == 5) // GPP4
		SYSCTL_PIO5_CFG |= 0b11 << 18;
	if(rom_dataONram.jtag_tms == 0) // GPB4
		SYSCTL_PIO0_CFG |= 0b11 << 24;
	else if(rom_dataONram.jtag_tms == 1) // GPC2
		SYSCTL_PIO1_CFG |= 0b11 << 4;
	else if(rom_dataONram.jtag_tms == 2) // GPF7
		SYSCTL_PIO2_CFG |= 0b11 << 30;
	else if(rom_dataONram.jtag_tms == 3) // GPH2
		SYSCTL_PIO3_CFG |= 0b11 << 20;
	else if(rom_dataONram.jtag_tms == 4) // GPH4
		SYSCTL_PIO3_CFG |= 0b11 << 24;
	else if(rom_dataONram.jtag_tms == 5) // GPP5
		SYSCTL_PIO5_CFG |= 0b11 << 18;
	if(rom_dataONram.jtag_tck == 0) // GPB3
		SYSCTL_PIO0_CFG |= 0b11 << 22;
	else if(rom_dataONram.jtag_tck == 1) // GPC1
		SYSCTL_PIO1_CFG |= 0b11 << 2;
	else if(rom_dataONram.jtag_tck == 2) // GPF6
		SYSCTL_PIO2_CFG |= 0b11 << 28;
	else if(rom_dataONram.jtag_tck == 3) // GPH1
		SYSCTL_PIO3_CFG |= 0b11 << 18;
	else if(rom_dataONram.jtag_tck == 4) // GPH6
		SYSCTL_PIO3_CFG |= 0b11 << 28;
	else if(rom_dataONram.jtag_tck == 5) // GPP7
		SYSCTL_PIO5_CFG |= 0b11 << 18;
}
void OPTIMIZE0 SECTION(".ROM.Func.wdt") USED rom_wdt(register u32 count) // wdt 初始化
{
	SYSCTL_MODEN0 |= BIT(15);
	if(count >= 0xf)
		WDT_TORR = count;
	else
		WDT_TORR = 0xf;
	WDT_CR = 0x1d;
	rom_wdt_feed();
}
void OPTIMIZE0 SECTION(".ROM.Func.patch") USED rom_patch(void) // patch 初始化
{
	register u32 PATCH_PTR;
	for(ROM_I = 0; ROM_I < rom_dataONram.patch_total && ROM_I <= 7; ROM_I++)
	{
		PATCH_CTRL |= 1 << rom_PATCHONram[ROM_I].num;
		if(rom_PATCHONram[ROM_I].num == 0)
			PATCH_PTR = (ROM_PATCH_ADDR + PATCH0_OFESET);
		else if(rom_PATCHONram[ROM_I].num == 1)
			PATCH_PTR = (ROM_PATCH_ADDR + PATCH1_OFESET);
		else if(rom_PATCHONram[ROM_I].num == 2)
			PATCH_PTR = (ROM_PATCH_ADDR + PATCH2_OFESET);
		else if(rom_PATCHONram[ROM_I].num == 3)
			PATCH_PTR = (ROM_PATCH_ADDR + PATCH3_OFESET);
		else if(rom_PATCHONram[ROM_I].num == 4)
			PATCH_PTR = (ROM_PATCH_ADDR + PATCH4_OFESET);
		else if(rom_PATCHONram[ROM_I].num == 5)
			PATCH_PTR = (ROM_PATCH_ADDR + PATCH5_OFESET);
		else if(rom_PATCHONram[ROM_I].num == 6)
			PATCH_PTR = (ROM_PATCH_ADDR + PATCH6_OFESET);
		else if(rom_PATCHONram[ROM_I].num == 7)
			PATCH_PTR = (ROM_PATCH_ADDR + PATCH7_OFESET);
		else
			continue;
		*(u8 *)(PATCH_PTR + PATCH_ADDRL_OFFSET) = rom_PATCHONram[ROM_I].addrl;
		*(u8 *)(PATCH_PTR + PATCH_ADDRH_OFFSET) = rom_PATCHONram[ROM_I].addrh;
		*(u8 *)(PATCH_PTR + PATCH_DATA0_OFFSET) = rom_PATCHONram[ROM_I].data.byte[0];
		*(u8 *)(PATCH_PTR + PATCH_DATA1_OFFSET) = rom_PATCHONram[ROM_I].data.byte[1];
		*(u8 *)(PATCH_PTR + PATCH_DATA2_OFFSET) = rom_PATCHONram[ROM_I].data.byte[2];
		*(u8 *)(PATCH_PTR + PATCH_DATA3_OFFSET) = rom_PATCHONram[ROM_I].data.byte[3];
	}
}
// flash 选择函数
void OPTIMIZE0 SECTION(".ROM.Func.flash") USED spif_read_qe(void) // 获取flash四路/二路状态
{
	register u32 temp_data;
	for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--);
	for(volatile uint32_t counter = ROM_TIMEOUT; (SPIF_STATUS & 0xf) && counter; counter--)
		rom_wdt_feed(); // 判断写完
	// while (!(SPIF_READY & 0x1))rom_wdt_feed();
	SPIF_DBYTE = 0x0;
	// while (!(SPIF_READY & 0x1))rom_wdt_feed(); // 读忙
	SPIF_FIFO_TOP = 0X35; // 读状态寄存器2
	// while (!(SPIF_READY & 0x1))
	// rom_wdt_feed(); // 读忙
	// while ((SPIF_FIFO_CNT & 0x3) == 0)rom_wdt_feed();
	for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--);
	// while (SPIF_STATUS & 0xf)rom_wdt_feed(); // 判断写完
	// while (!(SPIF_READY & 0x1))rom_wdt_feed();
	while(!(SPIF_FIFO_CNT & 0x3))
		rom_wdt_feed();
	temp_data = SPIF_FIFO_TOP;
	temp_data >>= 24;
	if((temp_data & BIT1))
		ROM_flashqe_flag = 1; // 判断是否置位
	else
		ROM_flashqe_flag = 0;
}
void OPTIMIZE0 SECTION(".ROM.Func.flash") USED select_qe(register BYTE select_qe) // 选择四路/二路
{
	if(select_qe) // 四线
	{
		spif_read_qe();
		if(!(ROM_flashqe_flag))
		{
			// while (!(SPIF_READY & 0x1))rom_wdt_feed(); // 读忙
			// SPIF_FIFO_TOP = 0x06;
			for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--)
				rom_wdt_feed(); // 读忙
			SPIF_DBYTE = 0x1;
			for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--)
				rom_wdt_feed(); // 读忙
			SPIF_FIFO_TOP = 0x02020001;
			for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--)
				rom_wdt_feed();
			for(volatile uint32_t counter = ROM_TIMEOUT; (SPIF_STATUS & 0xf) && counter; counter--)
				rom_wdt_feed(); // 判断写完
			for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--)
				rom_wdt_feed();
		}
		while(!(SPIF_STATUS & BIT4))
		{
			spif_read_qe();
			rom_wdt_feed();
		} // 判断写完
		SPIF_CTRL0 |= BIT1;
	}
	else
	{
		SPIF_CTRL0 &= ~BIT1;
	}
}
void OPTIMIZE0 SECTION(".ROM.Func.flash") USED select_flash(register BYTE select_internal)
{
	ROM_flash_flag = select_internal;
	if(select_internal == INTERNAL_FLASH)
	{
		// internal
		REG32(0x3047C) &= 0xfffffffd; // sysclt 0x3047c bit1=0;
	}
	else if(select_internal == EXTERNAL_FLASH) // set extern now
	{
		// external flash iomux
		if(rom_dataONram.flash_qspi_flag)
		{
			if(rom_dataONram.flash_wp) // ec103b select spif wp (0:PIOB17,1:PIOD8)
			{
				SYSCTL_PIO3_CFG &= 0xfffffff3;
				SYSCTL_PIO5_CFG &= 0xfffcffff; // sysclt 0x30468,d8;
				SYSCTL_PIO5_CFG |= 0x00010000;
			}
			else
			{
				SYSCTL_PIO5_CFG &= 0xfffcffff;
				SYSCTL_PIO3_CFG &= 0xfffffff3; // sysclt 0x30460 G1==func1;
				SYSCTL_PIO3_CFG |= 0x00000004;
			}
			if(rom_dataONram.flash_hold) // ec103a select spif hold (0：PIOA16,1:PIOB19)
			{
				SYSCTL_PIO1_CFG &= 0xfffffffc;
				SYSCTL_PIO3_CFG &= 0xffffff3f; // ysclt 0x30460 G3==func1;
				SYSCTL_PIO3_CFG |= 0x00000040;
			}
			else
			{
				SYSCTL_PIO3_CFG &= 0xffffff3f;
				SYSCTL_PIO1_CFG &= 0xfffffffc; // sysclt 0x30458 a16==func2;
				SYSCTL_PIO1_CFG |= 0x00000002;
			}
		}
		SYSCTL_PIO3_CFG &= 0xffff00ff; // sysclt 0x30460 G7-4==func1;
		SYSCTL_PIO3_CFG |= 0x00005500;
		// external
		REG32(0x3047C) |= 0x00000002; // sysclt 0x3047c bit1=1;
	}
	SYSCTL_RST1 |= 0x00000100; // 复位
	SYSCTL_RST1 &= 0xfffffeff; // 清除复位
	select_qe(((!select_internal) || (rom_dataONram.flash_qspi_flag)));
}
// 库函数
OPTIMIZE0 SECTION(".ROM.Func.memset") USED void *rom_memset(register void *str, register int c, register size_t n)
{
	c &= 0xff;
	for(ROM_I = 0; ROM_I < n; ROM_I++)
	{
		*((BYTEP)str + ROM_I) = c;
		rom_wdt_feed();
	}
	return str;
}
int OPTIMIZE0 SECTION(".ROM.Func.str") USED ROM_strcmp(register const char *str1, register const char *str2)
{
	ROM_J = 0;
	while(*(str1 + ROM_J) == *(str2 + ROM_J))
	{
		if(*(str1 + ROM_J) == '\0')
			return 0;
		ROM_J++;
		rom_wdt_feed();
	}
	return ++ROM_J;
}
// SPIF操作函数
void OPTIMIZE0 SECTION(".ROM.Func.UPDATE") USED ROM_SPIF_ERASE(void) // flash 擦写
{
	for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--); // 读忙
	for(volatile uint32_t counter = ROM_TIMEOUT; (SPIF_STATUS & 0xf) && counter; counter--)
		rom_wdt_feed(); // 直到写完
	for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--); // 读忙
	SPIF_FIFO_TOP = 0xc7;
	for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--); // 读忙
	for(volatile uint32_t counter = ROM_TIMEOUT; (SPIF_STATUS & 0xf) && counter; counter--)
		rom_wdt_feed(); // 直到写完
	for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--); // 读忙
}
void OPTIMIZE0 SECTION(".ROM.Func.spif") USED SPIF_Write_Interface(register DWORD size, register DWORD addr, BYTEP write_buff)
{
#define temp_addrs SPIF_addrs
#define write_data SPIF_data
	// 写
	if(size & 0b11)
	{
		size += 0b100;
		size &= ~0b11;
	}
	for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--);
	for(volatile uint32_t counter = ROM_TIMEOUT; (SPIF_STATUS & 0xf) && counter; counter--)
		rom_wdt_feed();
	for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--);
	SPIF_DBYTE = size - 1; // 准备写256字节
	for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--);
	if(SPIF_CTRL0 & BIT1)
		SPIF_WRITE_COMMAND = 0x32;
	else
		SPIF_WRITE_COMMAND = 0x2;
	SPIF_FIFO_TOP = ((addr & 0xFF) << 24) + ((addr & 0xFF00) << 8) + ((addr & 0xFF0000) >> 8) + SPIF_WRITE_COMMAND; // 设置地址
	for(SPIF_J = 0; SPIF_J < (size >> 2); SPIF_J++)																// 页写入
	{
		while((SPIF_FIFO_CNT & 0x3) >= 2)
			rom_wdt_feed();
		SPIF_FIFO_TOP = (*((DWORDP)(&(write_buff[SPIF_J << 2]))));
	}
	for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--);
	for(volatile uint32_t counter = ROM_TIMEOUT; (SPIF_STATUS & 0xf) && counter; counter--)
		rom_wdt_feed();
	for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--);
#undef temp_addrs
#undef write_data
}
VBYTEP OPTIMIZE0 SECTION(".ROM.Func.spif") USED SPIF_Read_Interface(register DWORD size, register DWORD addr, BYTEP read_buff)
{
#define temp_addrs SPIF_addrs
#define temp_data SPIF_data
	if(size & 0b11)
	{
		size += 0b100;
		size &= ~0b11;
	}
	for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--);
	for(volatile uint32_t counter = ROM_TIMEOUT; (SPIF_STATUS & 0xf) && counter; counter--)
		rom_wdt_feed(); // 直到写完
	for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--); // 读忙
	SPIF_DBYTE = (size - 1) & 0xff;
	for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--);
	if(SPIF_CTRL0 & BIT1)
		SPIF_READ_COMMAND = 0x6b;
	else
		SPIF_READ_COMMAND = 0x3b;
	SPIF_FIFO_TOP = (((addr & 0xFF) << 24) + ((addr & 0xFF00) << 8) + ((addr & 0xFF0000) >> 8) + SPIF_READ_COMMAND);
	for(SPIF_J = 0; SPIF_J < (size >> 2); SPIF_J++)
	{
		while((SPIF_FIFO_CNT & 0x3) == 0)
			rom_wdt_feed();
		(*((DWORDP)(&(read_buff[SPIF_J << 2])))) = SPIF_FIFO_TOP;
	}
	return read_buff;
#undef temp_addrs
#undef temp_data
}
SECTION(".ROM.var.UPDATE")
USED char *rom_UPDATE_ch = "UPDATE\n\0";
SECTION(".ROM.var.reboot")
USED char *rom_reboot_ch = "reboot\n\0";
void OPTIMIZE0 SECTION(".ROM.Func.UPDATE") USED ROM_Update(register BYTE flash_select, register DWORD u_base_addr)
{
#define buf_p ROM_TEMP0
#define data_cnt ROM_TEMP1
#define check_p ROM_TEMP2
#define check ROM_TEMP3
#define addr ROM_TEMP4
	clear_csr(mstatus, MSTATUS_MIE);
	clear_csr(0xBD1, 0xffffffff);
	UART_PTR = u_base_addr;
	rom_dataONram.debug_uart_en = 1;
	rom_puts(rom_UPDATE_ch);
	rom_putc(0xff);
	SYSCTL_RST1 |= BIT(8);
	SYSCTL_RST1 &= 0xfffffeff;
	rom_wdt(0xffff); // 开启看门狗
	rom_putc('0');
	rom_dataONram.flash_qspi_flag = 0;
	select_flash(flash_select); // 选择flash1
	// 初始化数据
	update_state = 0;
	update_cnt = 0;
	update_rx_cnt = 0;
	update_addrs = 0;
	/***********数据擦写********/
	rom_putc('1');
	ROM_SPIF_ERASE();
	rom_putc('2');
	while(1)
	{
		/*****************************数据接收*****************/
		rom_wdt_feed();
		buf_p = (VDWORD)update_buff;
		// 请求传包
		rom_putc('A');
		// 等待数据接收
		while(update_rx_cnt < update_size)
		{
			while(!(REG8(REG_ADDR(UART_PTR, UART_LSR_OFFSET)) & UART_LSR_DR))
				rom_wdt_feed();
			update_buff[update_rx_cnt] = REG8(REG_ADDR(UART_PTR, UART_RBR_OFFSET));
			update_rx_cnt++;
			rom_wdt_feed();
		}
		update_rx_cnt = 0;
		rom_putc('B');						   // 结束传输块
		update_state = (VWORD) * (u16 *)buf_p; // 记录状态位
		buf_p += update_state_size;
		data_cnt = (VDWORD) * (u16 *)buf_p;
		buf_p += update_cnt_size;
		if(update_cnt != (VWORD)data_cnt)
		{
			rom_putc('E');
			while(!(REG8(REG_ADDR((DWORD)UART_PTR, UART_LSR_OFFSET)) & UART_LSR_TEMP))
				rom_wdt_feed();
			REG8(REG_ADDR((DWORD)UART_PTR, UART_THR_OFFSET)) = update_cnt; // 报错误计数
			while(REG8(REG_ADDR((DWORD)UART_PTR, UART_RBR_OFFSET)) != 0x06)
				rom_wdt_feed(); // 请求回应
			continue;			// 退出本次循环
		}
		rom_wdt_feed();
		check_p = buf_p;
		buf_p += update_check_size;
		/*********************************校验码计算比较**************/
		rom_putc('C');
		rom_putc('C');	 // 进入校验
		check = check_p; // 计算
		for(ROM_I = 0; ROM_I < update_check_size; ROM_I++)
			if(*(VBYTEP)(check_p + ROM_I) != *(VBYTEP)(check + ROM_I))
			{
				rom_putc('F');
				while(REG8(REG_ADDR((DWORD)UART_PTR, UART_RBR_OFFSET)) != 0x06)
					rom_wdt_feed();
				update_cnt--;
				continue; // 退出本次循环//这部分不完善，等完善后再写入
			}			  // 比较
		rom_wdt_feed();
		/**********************数据写入*************************/
		/*************数据写入******/
		rom_putc('D'); // 正在写入
		addr = (u32)update_cnt * update_code_size;
		for(ROM_I = 0; ROM_I < 4; ROM_I++)
		{
			SPIF_Write_Interface(256, addr, (BYTEP)buf_p);
			addr += 256;
			buf_p += 256;
		}
		/*************数据校验处理******/
		/**********************数据处理*************************/
		rom_wdt_feed();
		rom_memset((void *)update_buff, 0, update_size);
		update_cnt++;
		if(update_state)
			break; // 判断继续/结束轮询
	}
	rom_putc('Z'); // 结束//等待重启
	/********发送重启指令****/
	set_csr(mstatus, MSTATUS_MIE);
	while(1)
	{
		rom_wdt_feed();
		rom_puts(rom_reboot_ch); // 等待硬件复位
	}
#undef buf_p
#undef data_cnt
#undef check_p
#undef check
#undef addr
}
void OPTIMIZE0 SECTION(".ROM.Func.UPDATE") USED ROM_SHIO_Update(register BYTE flash_select, register DWORD u_base_addr)
{
	clear_csr(mstatus, MSTATUS_MIE);
	clear_csr(0xBD1, 0xffffffff);
	UART_PTR = u_base_addr;
	rom_dataONram.debug_uart_en = 1;
	rom_puts(rom_UPDATE_ch);
	rom_putc(0xff);
	SYSCTL_RST1 |= BIT(8);
	SYSCTL_RST1 &= 0xfffffeff;
	rom_wdt(0xffff); // 开启看门狗
	rom_putc('0');
	rom_dataONram.flash_qspi_flag = 0;
	select_flash(flash_select); // 选择flash1
	uint32_t data[64];
	uint8_t SHIO_CNT1, SHIO_CNT2 = 0;
	uint8_t SM_Addr0, SM_Addr1, SM_Addr2, SM_Data0, SM_Data1, SM_Data2, SM_Data3;
	uint32_t SM_PageWrite_Addr;
	ROM_SPIF_ERASE();
	rom_putc('a');
	PMC2_DOR = 0x5A; // response to erase flash ok
	while(1)
	{
		if(IS_MASK_CLEAR(PMC2_STR, IBF2))
			continue;
		rom_putc('b');
		ECU_Cmd = PMC2_DIR;
		if(ECU_Cmd == 0xB5) // start page write flash
		{
			rom_putc('c');
			ECU_Cmd = 0x0;
			// get address
			SM_Addr0 = SMF_FADDR0;
			SM_Addr1 = SMF_FADDR1;
			SM_Addr2 = SMF_FADDR2;
			SM_PageWrite_Addr = (uint32_t)(SM_Addr0 << 24) + (uint32_t)(SM_Addr1 << 16) + (uint32_t)(SM_Addr2 << 8) + (uint32_t)(0x2);
			rom_putc('d');
			PMC2_DOR = 0x5B;
		}
		else if(ECU_Cmd == 0xC5)
		{
			rom_putc('e');
			if(SHIO_CNT2 >= 64)
				SHIO_CNT2 = 0; // 子坤debug
			ECU_Cmd = 0x0;
			SM_Data0 = SMF_DR0;
			SM_Data1 = SMF_DR1;
			SM_Data2 = SMF_DR2;
			SM_Data3 = SMF_DR3;
			data[SHIO_CNT2] = (uint32_t)(SM_Data3 << 24) + (uint32_t)(SM_Data2 << 16) + (uint32_t)(SM_Data1 << 8) + (uint32_t)SM_Data0;
			SHIO_CNT2++;
			if(SHIO_CNT2 >= 64)
			{
				for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--)
					WDT_CRR = 0x76;
				SPIF_DBYTE = 0xff;
				for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--)
					WDT_CRR = 0x76;
				SPIF_FIFO_TOP = SM_PageWrite_Addr;
				for(SHIO_CNT1 = 0; SHIO_CNT1 < 64; SHIO_CNT1++)
				{
					while((SPIF_FIFO_CNT & 0x3) != 0)
						WDT_CRR = 0x76;
					SPIF_FIFO_TOP = data[SHIO_CNT1];
				}
				for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--)
					WDT_CRR = 0x76;
				while(SPIF_STATUS & 0x1)
					WDT_CRR = 0x76;
				for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--)
					WDT_CRR = 0x76;
				rom_putc('g');
			}
			rom_putc('f');
			PMC2_DOR = SHIO_CNT2;
		}
		else
		{
			rom_putc('h');
		}
	}
}
SECTION(".ROM.var.exit")
USED char *rom_exit_ch = "rom exit\n\0";
void OPTIMIZE0 SECTION(".ROM.Func.exit") USED rom_exit(void)
{
	rom_puts(rom_exit_ch);
	while(1)
		rom_wdt_feed(); // 喂狗;
}
// 国密安全(变量可能有问题)
SECTION(".ROM.pulib")
USED spoint rom_SM2Pulib_Key[16] = {
	// 公钥
	[0] .x[0] = 0x160b0494,
	[0].x[1] = 0xc554e9ee,
	[0].x[2] = 0x8931c0cf,
	[0].x[3] = 0xf93a6a69,
	[0].x[4] = 0xd9fda1e1,
	[0].x[5] = 0xe6ddf3e8,
	[0].x[6] = 0x1af31e23,
	[0].x[7] = 0x7e8c8954,
	[0].y[0] = 0xd37e8208,
	[0].y[1] = 0xb724ff9e,
	[0].y[2] = 0xf4285008,
	[0].y[3] = 0x1d48dd3f,
	[0].y[4] = 0x5b1c1801,
	[0].y[5] = 0x049585ca,
	[0].y[6] = 0x9e86a120,
	[0].y[7] = 0xbf169cb5,
	[1].x[0] = 0xc19a7be5,
	[1].x[1] = 0x4fa68938,
	[1].x[2] = 0xd85d59b8,
	[1].x[3] = 0x38352caf,
	[1].x[4] = 0x05791a01,
	[1].x[5] = 0x2395b039,
	[1].x[6] = 0x382f4dbb,
	[1].x[7] = 0x4bed5d76,
	[1].y[0] = 0xcf7a35bf,
	[1].y[1] = 0x08daadae,
	[1].y[2] = 0x88c25a82,
	[1].y[3] = 0xb6192e08,
	[1].y[4] = 0x402c5f24,
	[1].y[5] = 0xc0762645,
	[1].y[6] = 0xd7efabc1,
	[1].y[7] = 0xa154d0a1,
	[2].x[0] = 0xac4490ae,
	[2].x[1] = 0x35758c01,
	[2].x[2] = 0x3a388cb0,
	[2].x[3] = 0xd4eaae6f,
	[2].x[4] = 0xbeaf1fee,
	[2].x[5] = 0x45579f35,
	[2].x[6] = 0xa8dd6aab,
	[2].x[7] = 0x26d2f95d,
	[2].y[0] = 0xc19f8209,
	[2].y[1] = 0x94c59cd4,
	[2].y[2] = 0x2fc81fe4,
	[2].y[3] = 0x8334d23a,
	[2].y[4] = 0x65062a04,
	[2].y[5] = 0x61d714e1,
	[2].y[6] = 0xc100d3a2,
	[2].y[7] = 0xd4e22164,
	[3].x[0] = 0x0bb7ab13,
	[3].x[1] = 0x2a96f35d,
	[3].x[2] = 0x8e114d37,
	[3].x[3] = 0x1febd90b,
	[3].x[4] = 0xaa053465,
	[3].x[5] = 0x5998541e,
	[3].x[6] = 0xc26b4391,
	[3].x[7] = 0x4c26d25e,
	[3].y[0] = 0x8a848b95,
	[3].y[1] = 0xf54154f3,
	[3].y[2] = 0xa4136b1f,
	[3].y[3] = 0xcc1dee5e,
	[3].y[4] = 0x42cbb2ed,
	[3].y[5] = 0x7c89336e,
	[3].y[6] = 0xf7d5fa22,
	[3].y[7] = 0xcb3ab5c3,
	[4].x[0] = 0xa2b09dac,
	[4].x[1] = 0x9b7e82c0,
	[4].x[2] = 0x31cbca20,
	[4].x[3] = 0x180eeb69,
	[4].x[4] = 0x2e118b19,
	[4].x[5] = 0x389097dd,
	[4].x[6] = 0x73a9db3e,
	[4].x[7] = 0xca469e46,
	[4].y[0] = 0x96016db8,
	[4].y[1] = 0x1c669e52,
	[4].y[2] = 0xf5f32a48,
	[4].y[3] = 0xd5616a1b,
	[4].y[4] = 0xef02d28d,
	[4].y[5] = 0x4580e4e6,
	[4].y[6] = 0x5c76700f,
	[4].y[7] = 0x0e3ab8ca,
	[5].x[0] = 0xe3b15b85,
	[5].x[1] = 0xf7d80e08,
	[5].x[2] = 0x88d64825,
	[5].x[3] = 0xed8e0d4d,
	[5].x[4] = 0xc401a47e,
	[5].x[5] = 0x3b20ef9d,
	[5].x[6] = 0x0bee3f4c,
	[5].x[7] = 0x0709e6df,
	[5].y[0] = 0x93daf74c,
	[5].y[1] = 0x2d4e2aa5,
	[5].y[2] = 0x74de83b5,
	[5].y[3] = 0x64ca36f2,
	[5].y[4] = 0xa509241c,
	[5].y[5] = 0xd94ab59f,
	[5].y[6] = 0x490e5eef,
	[5].y[7] = 0x797d9f2c,
	[6].x[0] = 0x12b58637,
	[6].x[1] = 0xd2516157,
	[6].x[2] = 0xe32ba1d9,
	[6].x[3] = 0x98ced817,
	[6].x[4] = 0x42246a1e,
	[6].x[5] = 0x492cbf94,
	[6].x[6] = 0xfce362cf,
	[6].x[7] = 0xa3c88695,
	[6].y[0] = 0x52243c1c,
	[6].y[1] = 0xb2d71b58,
	[6].y[2] = 0xa67935db,
	[6].y[3] = 0xb36288bc,
	[6].y[4] = 0x12fb92b6,
	[6].y[5] = 0xd45e4cbe,
	[6].y[6] = 0x28ad0096,
	[6].y[7] = 0xb3391fd9,
	[7].x[0] = 0xd7b2c544,
	[7].x[1] = 0x97db2af9,
	[7].x[2] = 0x616b0d22,
	[7].x[3] = 0x4f53f120,
	[7].x[4] = 0x9a2a3bb4,
	[7].x[5] = 0x1b483edc,
	[7].x[6] = 0xd44495d8,
	[7].x[7] = 0x451d36da,
	[7].y[0] = 0x11d4f472,
	[7].y[1] = 0xe71f894d,
	[7].y[2] = 0xae191f3a,
	[7].y[3] = 0xfacb13b7,
	[7].y[4] = 0xff672f4e,
	[7].y[5] = 0xb30ed309,
	[7].y[6] = 0x46193ff8,
	[7].y[7] = 0x1706f694,
	[8].x[0] = 0x7b87ce42,
	[8].x[1] = 0xcbf29663,
	[8].x[2] = 0xc305c79b,
	[8].x[3] = 0x625cf51b,
	[8].x[4] = 0x05be878a,
	[8].x[5] = 0xb77b121d,
	[8].x[6] = 0x59f3e96e,
	[8].x[7] = 0x8f6937a4,
	[8].y[0] = 0xcba32337,
	[8].y[1] = 0xa247178e,
	[8].y[2] = 0xe73ffac6,
	[8].y[3] = 0x5798ba0a,
	[8].y[4] = 0x9d25ad9b,
	[8].y[5] = 0xd606b585,
	[8].y[6] = 0x5bbbc26f,
	[8].y[7] = 0x181f2d7e,
	[9].x[0] = 0x82851d2b,
	[9].x[1] = 0x8c298b24,
	[9].x[2] = 0xa14c7bd7,
	[9].x[3] = 0xd7877ad1,
	[9].x[4] = 0x3150fe34,
	[9].x[5] = 0x590668b8,
	[9].x[6] = 0xda870184,
	[9].x[7] = 0x3bb92f51,
	[9].y[0] = 0xce2ec03e,
	[9].y[1] = 0xe3049dd3,
	[9].y[2] = 0x4a602d79,
	[9].y[3] = 0x99aabf1d,
	[9].y[4] = 0x506382ec,
	[9].y[5] = 0xc1e58809,
	[9].y[6] = 0xc3a5edb2,
	[9].y[7] = 0xbbd92e96,
	[10].x[0] = 0xfe25732a,
	[10].x[1] = 0xaad6adfd,
	[10].x[2] = 0xaac6dca8,
	[10].x[3] = 0x38916aed,
	[10].x[4] = 0x49cc1ca8,
	[10].x[5] = 0xd74f72b3,
	[10].x[6] = 0x6ef3e3f9,
	[10].x[7] = 0xdcd27a1f,
	[10].y[0] = 0x02f57f07,
	[10].y[1] = 0x2c190433,
	[10].y[2] = 0x142d03dc,
	[10].y[3] = 0x76ae0db8,
	[10].y[4] = 0xeeeac9f0,
	[10].y[5] = 0xc3e1fb92,
	[10].y[6] = 0xf3217ad6,
	[10].y[7] = 0xb3d2a118,
	[11].x[0] = 0x2d134150,
	[11].x[1] = 0x9c9f8fb5,
	[11].x[2] = 0x2d2259b5,
	[11].x[3] = 0x6352f5d4,
	[11].x[4] = 0x853d22b1,
	[11].x[5] = 0x57057c50,
	[11].x[6] = 0x0df199eb,
	[11].x[7] = 0xed95b24e,
	[11].y[0] = 0x9df7c4f5,
	[11].y[1] = 0xab05e2e7,
	[11].y[2] = 0xd4c968be,
	[11].y[3] = 0xfe6c3da9,
	[11].y[4] = 0x01ec36ac,
	[11].y[5] = 0x12aa6ed0,
	[11].y[6] = 0x13876786,
	[11].y[7] = 0xcac6ea95,
	[12].x[0] = 0xbac2b82d,
	[12].x[1] = 0x470240c7,
	[12].x[2] = 0xc89103d6,
	[12].x[3] = 0xdfeea052,
	[12].x[4] = 0x9322ac89,
	[12].x[5] = 0x062873e7,
	[12].x[6] = 0x0fdc1c60,
	[12].x[7] = 0xbcd7c53f,
	[12].y[0] = 0xc66cefc6,
	[12].y[1] = 0x29a08c76,
	[12].y[2] = 0xc91b2d93,
	[12].y[3] = 0xc6b39492,
	[12].y[4] = 0x210c6da5,
	[12].y[5] = 0x95963dff,
	[12].y[6] = 0x4eccc328,
	[12].y[7] = 0xc7940557,
	[13].x[0] = 0x5e469603,
	[13].x[1] = 0x5a20ff84,
	[13].x[2] = 0x18171177,
	[13].x[3] = 0x039f95e7,
	[13].x[4] = 0xb48d8276,
	[13].x[5] = 0x3688d7be,
	[13].x[6] = 0x2834107f,
	[13].x[7] = 0x03ba3e55,
	[13].y[0] = 0xb7def308,
	[13].y[1] = 0x3e0bd2c1,
	[13].y[2] = 0xab50277e,
	[13].y[3] = 0x00fb5a8d,
	[13].y[4] = 0x1378d556,
	[13].y[5] = 0xce6e731a,
	[13].y[6] = 0x9484ae0b,
	[13].y[7] = 0xe2f177d2,
	[14].x[0] = 0xb3495e21,
	[14].x[1] = 0xdf01affc,
	[14].x[2] = 0xf00c46d2,
	[14].x[3] = 0x4d939738,
	[14].x[4] = 0xba3d0680,
	[14].x[5] = 0x845c131b,
	[14].x[6] = 0xa2eec306,
	[14].x[7] = 0xab4a0d64,
	[14].y[0] = 0x75491963,
	[14].y[1] = 0x16076cb5,
	[14].y[2] = 0xd38511e8,
	[14].y[3] = 0x0175e744,
	[14].y[4] = 0x06adb4cb,
	[14].y[5] = 0xf460d2ff,
	[14].y[6] = 0xf22bc547,
	[14].y[7] = 0xdeb456f0,
	[15].x[0] = 0x20d49802,
	[15].x[1] = 0xbf850a90,
	[15].x[2] = 0x5f99f8f1,
	[15].x[3] = 0x8457af5e,
	[15].x[4] = 0x53a9c685,
	[15].x[5] = 0xd30478c0,
	[15].x[6] = 0x42a54780,
	[15].x[7] = 0x596fa534,
	[15].y[0] = 0x89f2f8d8,
	[15].y[1] = 0xc6e7cbe4,
	[15].y[2] = 0x8891dae1,
	[15].y[3] = 0x2f69c285,
	[15].y[4] = 0x1bf5a6eb,
	[15].y[5] = 0x1f5ff364,
	[15].y[6] = 0x6d106927,
	[15].y[7] = 0x67fd3c2b,
};
SECTION(".ROM.var.spoint")
USED spoint rom_SM2_Elipse_G = {
	.x[0] = 0x334C74C7,
	.x[1] = 0x715A4589,
	.x[2] = 0xF2660BE1,
	.x[3] = 0x8FE30BBF,
	.x[4] = 0x6A39C994,
	.x[5] = 0x5F990446,
	.x[6] = 0x1F198119,
	.x[7] = 0x32C4AE2C,
	.y[0] = 0x2139F0A0,
	.y[1] = 0x02DF32E5,
	.y[2] = 0xC62A4740,
	.y[3] = 0xD0A9877C,
	.y[4] = 0x6B692153,
	.y[5] = 0x59BDCEE3,
	.y[6] = 0xF4F6779C,
	.y[7] = 0xBC3736A2 };
static OPTIMIZE0 SECTION(".ROM.Func.SMx") DWORD rom_SMx_Change(register DWORD data)
{
	return (((data & 0x000000ff) << 24) | ((data & 0x0000ff00) << 8) | ((data & 0x00ff0000) >> 8) | ((data & 0xff000000) >> 24));
}
// SM2
void OPTIMIZE0 SECTION(".ROM.Func.SM2.io") USED rom_SM2_IO(VDWORDP addr1, VDWORDP addr2)
{
	ROM_I = 0;
	while(ROM_I < 8)
	{
		*(addr1 + ROM_I) = *(addr2 + ROM_I);
		ROM_I++;
	}
}
void OPTIMIZE0 SECTION(".ROM.Func.SM2.datain") USED rom_SM2_Data_IN(DWORDP X1, DWORDP X2, DWORDP Y1, DWORDP Y2, DWORDP K)
{
	if(X1 != NULL)
		rom_SM2_IO(&SM2_P1X0, (VDWORDP)X1);
	if(X2 != NULL)
		rom_SM2_IO(&SM2_P2X0, (VDWORDP)X2);
	if(Y1 != NULL)
		rom_SM2_IO(&SM2_P1Y0, (VDWORDP)Y1);
	if(Y2 != NULL)
		rom_SM2_IO(&SM2_P2Y0, (VDWORDP)Y2);
	if(K != NULL)
		rom_SM2_IO(&SM2_K0, (VDWORDP)K);
}
void OPTIMIZE0 SECTION(".ROM.Func.SM2.dataout") USED rom_SM2_Data_Out(DWORDP X3, DWORDP Y3)
{
	if(X3 != NULL)
		rom_SM2_IO((VDWORDP)X3, &SM2_P3X0);
	if(Y3 != NULL)
		rom_SM2_IO((VDWORDP)Y3, &SM2_P3Y0);
}
void OPTIMIZE0 SECTION(".ROM.Func.SM2.alu") USED rom_SM2_ALU(DWORDP X1, DWORDP X2, DWORDP Y1, DWORDP Y2, DWORDP K, DWORD control, DWORDP X3, DWORDP Y3)
{
	rom_SM2_Data_IN(X1, X2, Y1, Y2, K);		// input
	SM2_CONTROL = SM2_CONTROL_EN | control; // run
	while(SM2_STATUS != SM2_STATUS_DONE)	// wait
	{
		rom_wdt_feed();
	}
	rom_SM2_Data_Out(X3, Y3);		// out
	SM2_CONTROL = SM2_CONTROL_FREE; // free
}
void OPTIMIZE0 SECTION(".ROM.Func.SM2.nadd") USED rom_SM2_N_ADD(DWORDP input1, DWORDP input2, DWORDP output)
{
	rom_SM2_ALU(input1, input2, NULL, NULL, NULL, SM2_CONTROL_MODN_ADD, NULL, output);
}
void OPTIMIZE0 SECTION(".ROM.Func.SM2.dotadd") USED rom_SM2_DOT_ADD(spoint *input1, spoint *input2, spoint *output)
{
	rom_SM2_ALU(input1->x, input2->x, input1->y, input2->y, NULL, SM2_CONTROL_DOT_ADD, output->x, output->y);
}
void OPTIMIZE0 SECTION(".ROM.Func.SM2.dotdbl") USED rom_SM2_DOT_DBL(spoint input, spoint *output)
{
	rom_SM2_ALU(input.x, NULL, input.y, NULL, NULL, SM2_CONTROL_DOT_DBL, output->x, output->y);
}
void OPTIMIZE0 SECTION(".ROM.Func.SM2.dotmul") USED rom_SM2_DOT_MUL(spoint *input, DWORDP k, spoint *output)
{
	rom_SM2_ALU(input->x, NULL, input->y, NULL, k, SM2_CONTROL_DOT_MUL, output->x, output->y);
}
/**************************************************************************************
 * Author :Linyu
 * date :23.2.8
 * function:run SM2
 * note :
 *
 *          input:
 *          key:
 *          output:
 *          xordr :lv
 * ************************************************************************************/
BYTE OPTIMIZE0 SECTION(".ROM.Func.SM2.verify") USED rom_SM2_VERIFY(ssign *sig, DWORDP e, spoint *PA)
{
	rom_SM2_N_ADD(sig->r, sig->s, (DWORDP)(rom_T));		 // t=(r'+s')modn
	rom_SM2_DOT_MUL(&rom_SM2_Elipse_G, sig->s, &rom_P0); // p0 =[s']G
	rom_SM2_DOT_MUL(PA, (DWORDP)(rom_T), &rom_P1);		 // p1 = [t]PA
	rom_SM2_DOT_ADD(&rom_P0, &rom_P1, &rom_P2);	 // P2=P1+P0
	rom_SM2_N_ADD(e, rom_P2.x, (DWORDP)(rom_R)); // R=(e'+P2.x')modn
	for(ROM_I = 0; ROM_I < 8; ROM_I++)
		if(sig->r[ROM_I] != rom_R[ROM_I])
			return 1;
	return 0;
}
// SM4
static void OPTIMIZE0 SECTION(".ROM.Func.SM4.keyset") USED rom_SM4_Key_Set(DWORDP key)
{
	SM4_MODE = SM4_MODE_KEYEX;
	SM4_KEY0 = rom_SMx_Change(*(key + 0));
	SM4_KEY1 = rom_SMx_Change(*(key + 1));
	SM4_KEY2 = rom_SMx_Change(*(key + 2));
	SM4_KEY3 = rom_SMx_Change(*(key + 3));
	SM4_CTRL |= SM4_CTRL_START;
	while(!(SM4_STATUS & SM4_STATUS_DONE))
	{
		rom_wdt_feed();
	}
}
static void OPTIMIZE0 SECTION(".ROM.Func.SM4.datain") USED rom_SM4_Data_IN(DWORDP input)
{
	SM4_DATA0 = rom_SMx_Change(*(input + 0));
	SM4_DATA1 = rom_SMx_Change(*(input + 1));
	SM4_DATA2 = rom_SMx_Change(*(input + 2));
	SM4_DATA3 = rom_SMx_Change(*(input + 3));
}
static void OPTIMIZE0 SECTION(".ROM.Func.SM4.xordrin") USED rom_SM4_XORDR_IN(DWORDP xordr)
{
	SM4_DATA0 = rom_SMx_Change(*(xordr + 0));
	SM4_DATA1 = rom_SMx_Change(*(xordr + 1));
	SM4_DATA2 = rom_SMx_Change(*(xordr + 2));
	SM4_DATA3 = rom_SMx_Change(*(xordr + 3));
}
static void OPTIMIZE0 SECTION(".ROM.Func.SM4.modeset") USED rom_SM4_Mode_Set(register BYTE mode)
{
	if(mode == 0)
		SM4_MODE = SM4_MODE_ENDE1;
	else if(mode == 1)
		SM4_MODE = SM4_MODE_ENDE0;
	else if(mode == 2)
		SM4_MODE = SM4_MODE_XOR | SM4_MODE_ENDE1;
	else if(mode == 3)
		SM4_MODE = SM4_MODE_ENDE0;
}
static void OPTIMIZE0 SECTION(".ROM.Func.SM4.dataout") USED rom_SM4_Data_Out(DWORDP output)
{
	SM4_STATUS |= SM4_STATUS_DONE;
	SM4_CTRL |= SM4_CTRL_INTEN | SM4_CTRL_START;
	while(!(SM4_STATUS & SM4_STATUS_DONE))
	{
		rom_wdt_feed();
	}
	SM4_INT |= SM4_INT_INT;
	*(output + 0) = rom_SMx_Change(SM4_DATA0);
	*(output + 1) = rom_SMx_Change(SM4_DATA1);
	*(output + 2) = rom_SMx_Change(SM4_DATA2);
	*(output + 3) = rom_SMx_Change(SM4_DATA3);
}
static void OPTIMIZE0 SECTION(".ROM.Func.SM4.xor_software") USED rom_SM4_XOR_Software(DWORDP input, DWORDP output, DWORDP xordr)
{
	*(output + 0) = (*(input + 0)) ^ (*(xordr + 0));
	*(output + 1) = (*(input + 1)) ^ (*(xordr + 1));
	*(output + 2) = (*(input + 2)) ^ (*(xordr + 2));
	*(output + 3) = (*(input + 3)) ^ (*(xordr + 3));
}
/**************************************************************************************
 * Author :Linyu
 * date :23.2.8
 * function:run SM4
 * note :
 *          mode:0 ECB加密,1 ECB解码/2 CBC加密/3 CBC解码
 *          input: Plaintext password
 *          key:Extended secret key
 *          output: Ciphertext storage
 *          xordr :lv
 * ************************************************************************************/
void OPTIMIZE0 SECTION(".ROM.Func.SM4.run") USED rom_SM4_Run(register DWORD bit, register BYTE mode, BYTEP input, BYTEP key, BYTEP output, BYTEP xordr)
{
	rom_SM4_Key_Set((DWORDP)key);
	rom_SM4_Mode_Set(mode);
	if(mode == SM4_CBC_ENCODE || mode == SM4_CBC_DECODE)
	{
		rom_SM4_XORDR_IN((DWORDP)xordr); // set xordr
	}
	for(ROM_I = 0; ROM_I * 128 < bit; ROM_I++)
	{
		rom_SM4_Data_IN((DWORDP)(input + ROM_I * 16));	 // input data
		rom_SM4_Data_Out((DWORDP)(output + ROM_I * 16)); // output data
	}
}
/* void OPTIMIZE0 SECTION(".ROM.Func.SM4.run") USED rom_SM4_Run(register  BYTE mode,  BYTEP input,  BYTEP key,  BYTEP output,  BYTEP xordr)
{
	rom_SM4_Key_Set((DWORDP)key);	// set key
	rom_SM4_Data_IN((DWORDP)input); // input data
	rom_SM4_Mode_Set(mode);
	if (mode == 2)
	{
		rom_SM4_XORDR_IN((DWORDP)xordr); // Functionality not implemented
		rom_SM4_XOR_Software((DWORDP)input, (DWORDP)output, (DWORDP)xordr);
		rom_SM4_Data_IN((DWORDP)output); // reinput data
		rom_SM4_Data_Out((DWORDP)output);
		return;
	}
	else if (mode == 3)
	{
		rom_SM4_Data_Out((DWORDP)output);
		rom_SM4_XOR_Software((DWORDP)output, (DWORDP)output, (DWORDP)xordr);
		return;
	}
	rom_SM4_Data_Out((DWORDP)output);
} */
// SM3
static void OPTIMIZE0 SECTION(".ROM.Func.SM3.autowrite") USED rom_SM3_Auto_Write(register DWORD data)
{
	/*
	rom_putc(0);
	rom_putc((data>>0)&0xff);
	rom_putc((data>>8)&0xff);
	rom_putc((data>>16)&0xff);
	rom_putc((data>>24)&0xff);
	rom_putc(0); //*/
	SM3_REG(SM3_DATA0_OFFSET + sm3_data_offset) = data;
	sm3_data_offset = (sm3_data_offset + 0x4) & 0x3f;
	while(!(sm3_data_offset || (SM3_INT & 0x1)))
	{
		rom_wdt_feed();
	}
	if(SM3_INT & 0x1)
		SM3_INT = 0x1;
}
static void OPTIMIZE0 SECTION(".ROM.Func.SM3.datain") USED rom_SM3_Data_In(register DWORD bit, DWORDP input)
{
#define rom_reg_dword ROM_TEMP0
#define rbyte ROM_TEMP1
#define rbit ROM_TEMP2
#define block ROM_TEMP3
	rom_reg_dword = bit >> 5;		  // double word size
	rbyte = (bit >> 3) & 0x03;		  // Remainder byte
	rbit = (bit >> 5) - (rbyte << 3); // Remainder bit
	block = (bit & 0x1ff) >> 5;		  // final block byte size
	for(ROM_I = 0; ROM_I < rom_reg_dword; ROM_I++)
	{
		SPIF_Read_Interface(4, (DWORD)(input + ROM_I), (BYTEP)&rom_smx_buffer);
		// rom_smx_buffer.dword = (*(input + ROM_I));
		rom_SM3_Auto_Write(rom_smx_buffer.dword);
	}
	/*
	rom_putc(0);
	rom_putc((rbyte>>0)&0xff);
	rom_putc((rbyte>>8)&0xff);
	rom_putc((rbyte>>16)&0xff);
	rom_putc((rbyte>>24)&0xff);
	rom_putc(0); //*/
	for(ROM_J = 0; ROM_J < rbyte; ROM_J++)
	{
		SPIF_Read_Interface(1, (DWORD)((BYTEP)input + ROM_I * 8 + ROM_J), (BYTEP)&rom_smx_buffer.byte[ROM_J]);
		// rom_smx_buffer.byte[ROM_J] = *((BYTEP)input + ROM_I * 8 + ROM_J);
	}
	rbit &= 0x7;
	if(rbit)
		SPIF_Read_Interface(1, (DWORD)((BYTEP)input + ROM_I * 8 + ROM_J), (BYTEP)&rom_smx_buffer.byte[ROM_J]);
	// rom_smx_buffer.byte[ROM_J] = *((BYTEP)input + ROM_I * 8 + ROM_J);
	else
		rom_smx_buffer.byte[ROM_J] = 0x0;
	for(ROM_I = 0; ROM_I < rbit; ROM_I++)
	{
		rom_smx_buffer.byte[ROM_J] &= (~(0x80 >> ROM_I));
	}
	if(rbit || rbyte)
	{
		rom_smx_buffer.byte[ROM_J] |= (0x80 >> ROM_I);
		rom_SM3_Auto_Write(rom_smx_buffer.dword);
		block++;
	}
	else
	{
		rom_smx_buffer.dword = 0x80;
		rom_SM3_Auto_Write(rom_smx_buffer.dword);
		block++;
	}
	if(block >= 16)
		block &= 0xf;
	while(block < 15)
	{
		rom_smx_buffer.dword = 0;
		rom_SM3_Auto_Write(rom_smx_buffer.dword);
		block++;
	}
	rom_smx_buffer.dword = rom_SMx_Change(bit);
	rom_SM3_Auto_Write(rom_smx_buffer.dword);
#undef rom_reg_dword
#undef rbyte
#undef rbit
#undef block
}
static void OPTIMIZE0 SECTION(".ROM.Func.SM3.dataout") USED rom_SM3_Data_Out(DWORDP output)
{
	*(output + 7) = SM3_OUT7; // rom_SMx_Change(SM3_OUT7);
	*(output + 6) = SM3_OUT6; // rom_SMx_Change(SM3_OUT6);
	*(output + 5) = SM3_OUT5; // rom_SMx_Change(SM3_OUT5);
	*(output + 4) = SM3_OUT4; // rom_SMx_Change(SM3_OUT4);
	*(output + 3) = SM3_OUT3; // rom_SMx_Change(SM3_OUT3);
	*(output + 2) = SM3_OUT2; // rom_SMx_Change(SM3_OUT2);
	*(output + 1) = SM3_OUT1; // rom_SMx_Change(SM3_OUT1);
	*(output + 0) = SM3_OUT0; // rom_SMx_Change(SM3_OUT0);
}
/**************************************************************************************
 * Author :Linyu
 * date :23.2.8
 * function:run SM3
 * note :
 *          bit:data bit size
 *          input: Plaintext password
 *          output: Ciphertext storage
 * ************************************************************************************/
void OPTIMIZE0 SECTION(".ROM.Func.SM3.run") USED rom_SM3_Run(register DWORD bit, DWORDP input, DWORDP output)
{
	SM3_CONIFG |= SM3_CONFIG_ENABLE;	// 使能sm3
	SM3_CONIFG &= ~SM3_CONFIG_NEW_DATA; // data in
	sm3_data_offset = 0;
	rom_SM3_Data_In(bit, input);
	SM3_CONIFG |= SM3_CONFIG_NEW_DATA | SM3_CONFIG_ENABLE;
	while(!(SM3_STATE & SM3_STATE_FINISH))
	{
		rom_wdt_feed();
	}
	rom_SM3_Data_Out(output);
}
void OPTIMIZE0 SECTION(".ROM.Func.SPIF.CACHE") USED rom_SPIF_Icache(register FUNCT_PTR_V_V jump_addr)
{
	if(SPIF_CTRL0 & 0x2)
		ROM_TEMP0 = 1; // 判断ROM操作SPIF是否开启四线来决定cache是否需要开启四线
	else
		ROM_TEMP0 = 0;
	// step 1.disable spif softreset
	SYSCTL_RST1 |= 0x00000100; // 复位
	SYSCTL_RST1 &= 0xfffffeff; // 清除复位
	if(ROM_TEMP0)
	{
		// step 2
		for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--)
			WDT_CRR = 0x76;
		SPIF_DBYTE = 0x1;			// DBYTE
		SPIF_FIFO_TOP = 0x02020001; // flash qe
		for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--)
			WDT_CRR = 0x76;
		for(volatile uint32_t counter = ROM_TIMEOUT; (SPIF_STATUS & 0xf) && counter; counter--)
			WDT_CRR = 0x76;
		for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--)
			WDT_CRR = 0x76;
		SPIF_DBYTE = 0x1;			// DBYTE
		SPIF_FIFO_TOP = 0x00000035; // flash qe
		for(volatile uint32_t counter = ROM_TIMEOUT; (SPIF_STATUS & 0xf) && counter; counter--)
			WDT_CRR = 0x76;
		// step 3
		SPIF_CTRL0 = 0x3; // do not susb,4 wire
		// step 4
		for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--)
			WDT_CRR = 0x76;
		SPIF_DBYTE = 0x3;			// DBYTE 5 bytes
		SPIF_FIFO_TOP = 0x40000077; // cmd 77h + 24bits dummy
		for(volatile uint32_t counter = ROM_TIMEOUT; (!(SPIF_READY & 0x1)) && counter; counter--)
			WDT_CRR = 0x76;
		for(volatile uint32_t counter = ROM_TIMEOUT; (SPIF_STATUS & 0xf) && counter; counter--)
			WDT_CRR = 0x76;
	}
	// step 5
	(*jump_addr)();
}
// rom主函数
SECTION(".ROM.var.main.ch1")
USED char rom_main_ch1[] = "It's rom\n\0";
SECTION(".ROM.var.main.ch1")
USED char rom_main_ch2[] = "freq set OK\n\0";
SECTION(".ROM.var.main.ch3")
USED char rom_main_ch3[] = "It's internal flash\n\0";
SECTION(".ROM.var.main.ch4")
USED char rom_main_ch4[] = "It's external flash\n\0";
SECTION(".ROM.var.main.ch6")
USED char rom_main_ch6[] = "not \0";
SECTION(".ROM.var.main.ch7")
USED char rom_main_ch7[] = "second \0";
SECTION(".ROM.var.main.ch8")
USED char rom_main_ch8[] = "first \0";
SECTION(".ROM.var.main.ch9")
USED char rom_main_ch9[] = "double boot\n\0";
SECTION(".ROM.var.main.ch10")
USED char rom_main_ch10[] = "no judge rom_string\n\0";
SECTION(".ROM.var.main.ch11")
USED char rom_main_ch11[] = "SM3 HASH\n\0";
SECTION(".ROM.var.main.ch12")
USED char rom_main_ch12[] = "SM2_VERIFY\n\0";
SECTION(".ROM.var.main.ch13")
USED char rom_main_ch13[] = "erron\n\0";
SECTION(".ROM.var.main.ch14")
USED char rom_main_ch14[] = "OK\n\0";
SECTION(".ROM.var.main.ch15")
USED char rom_main_ch15[] = "It's mirror\n\0";
SECTION(".ROM.var.main.ch16")
USED char rom_main_ch16[] = "copy \0";
SECTION(".ROM.var.main.ch19")
USED char rom_main_ch17[] = "goto DOUBLE_BOOT\n\0";
SECTION(".ROM.var.main.ch20")
USED char rom_main_ch18[] = "flash start\n\0";
SECTION(".ROM.var.main.ejtag")
USED char rom_main_ch19[] = "set ejtag\n\0";
SECTION(".ROM.var.main.onlyext")
USED char rom_main_ch20[] = "only external\n\0";
void OPTIMIZE0 SECTION(".ROM.Func.main") USED rom_main(void)
{
	/* 	select_flash(EXTERNAL_FLASH);
		ROM_jump=(FUNCT_PTR_V_V)0x80084;
		rom_SPIF_Icache(ROM_jump);
		(ROM_jump)(); */
	//_R8=0;
	asm volatile( // 初始化ram空间
		"la a0, 0x20000\n"
		"la a1, 0x28000\n"
		"1:"
		"sw zero, (a0)\n"
		"addi a0, a0, 4\n"
		"bltu a0, a1, 1b\n");
	// rom_get_cpu_freq(1);rom_get_cpu_freq(100);
	// rom_uart();//调试信息口
	// rom_puts(rom_main_ch1);//输出 调试信息
	//_R8=1;
rom_main_head:
	if((EFUSE_DATA0 & BIT(19)) && (rom_bootmode)) // 屏蔽flash启动并且bootmode位确定时只用外部flash启动
	{
		rom_puts(rom_main_ch20);
		ROM_only_extern_flag = 1;
		select_flash(EXTERNAL_FLASH); // 选择外部flash
	}
	else
	{
		ROM_only_extern_flag = 0;
		select_flash(INTERNAL_FLASH);											   // 选择内部flash
		ROM_magic_number_addr = ((VDWORD)magic_number_addr - flash_cache_addr);	   // 128k的最后256字节
		SPIF_Read_Interface(8, (DWORD)ROM_magic_number_addr, (BYTEP)flash_string); // 读取识别字符串
		if(ROM_strcmp((const char *)flash_string, (const char *)magic_number))
			select_flash(EXTERNAL_FLASH); // 判断字符串是否正确，错误则切换到外部flash
		else
		{
			SPIF_Read_Interface(sizeof(sDynamicFlashInfo), (DWORD)ROM_magic_number_addr, (BYTEP)&rom_safeONram); // 正确则读取flash信息
			rom_puts(rom_main_ch3);																		 // 输出 调试信息
		}
	}
EXT_BOOT:
	if(REG32(0x3047C) & BIT1)																				   // 如果是外部flash
		for(ROM_I = 0, ROM_magic_number_addr = 0x20000; ROM_I < 4; ROM_I++, ROM_magic_number_addr += 0x20000) // 读取flash128k，256k，384k，512k最后256字节
		{
			SPIF_Read_Interface(8, (DWORD)(ROM_magic_number_addr - 0x100), (BYTEP)flash_string); // 读取识别字符串
			if(ROM_strcmp((const char *)flash_string, (const char *)magic_number))
			{
				if(ROM_I == 3)
				{
					rom_puts(rom_main_ch10);
					if(ROM_error_cnt)
						rom_exit(); // 没读取到，死循环
					else
					{
						ROM_error_cnt++;
						goto rom_main_head; // 第一次没读取到，重新读取
					}
				}
			}
			else
			{
				SPIF_Read_Interface(sizeof(sDynamicFlashInfo), (DWORD)(ROM_magic_number_addr - 0x100), (BYTEP)&rom_safeONram); // 读取签名信息
				rom_puts(rom_main_ch4);
				break;
			}
		}
	SPIF_Read_Interface(sizeof(sFixedFlashInfo), (DWORD)flash_info_addr, (BYTEP)&rom_dataONram); // 读取配置信息
	if(REG32(0x3047C) & BIT1)																 // 如果是外部flash
		select_flash(EXTERNAL_FLASH);														 // 重新配置外部flash4/2线
	//_R8=2;
	// patch info
	if((EFUSE_DATA0 & BIT(16)) && rom_dataONram.patch_en) // 如果都硬件和软件开启patch功能
	{
		SPIF_Read_Interface(sizeof(spatch_typdef) << 3, 0x200, (BYTEP)rom_PATCHONram); // 读取patch信息
		rom_patch();																   // patch配置
	}
	//_R8=3;
	if(rom_dataONram.jtag_en)
		rom_ejtag();				   // 初始化ejtag
	if(rom_dataONram.extern_osc_flag) // 选择外部时钟
	{
		(*(VDWORDP)0x304D4) &= ~BIT0;										 // 切换外部
		(*(VDWORDP)0x304D4) &= ~BIT1;										 // 关闭内部osc192M（低功耗考虑）
		SYSCTL_RESERVER = 0x40800000 | ((DWORD)rom_dataONram.main_freq << 16); // PLL分频
	}
	else
	{													 // 选择外部时钟
		(*(VDWORDP)0x304D4) |= 1;						 // 切换内部时钟
		REG32(0x304CC) = (DWORD)rom_dataONram.main_freq; // 设置osc分频
	}
	rom_get_cpu_freq(1);
	rom_get_cpu_freq(100); // 更换新的串口频率
	if(rom_dataONram.debug_uart_en)
		rom_uart();			// 调试信息口
	rom_puts(rom_main_ch1); // 输出 调试信息
	rom_puts(rom_main_ch2); // 输出 调试信息
	//_R8=4;
	// 获取双启动信息
	if((!(EFUSE_DATA0 & BIT(17))) && (rom_dataONram.double_boot)) // 是否双启动（硬件允许双启动且软件开启双启动）
	{
		rom_wdt((u32)rom_dataONram.rom_wdt_time); // 启东看门狗
		if(SYSCTL_ESTAT & BIT(3))				  // 判断是否为看门狗复位
		{
			rom_puts(rom_main_ch7);
			ROM_jump = (FUNCT_PTR_V_V)((DWORD)rom_dataONram.flash_secound_offset + flash_cache_addr); // 是则用后半部分
		}
		else
		{
			rom_puts(rom_main_ch8);
			ROM_jump = (FUNCT_PTR_V_V)((DWORD)rom_dataONram.flash_first_offset + flash_cache_addr); // 否用前半部分
		}
	}
	else
	{
		rom_puts(rom_main_ch6);
		ROM_jump = (FUNCT_PTR_V_V)((DWORD)rom_dataONram.flash_first_offset + flash_cache_addr); // 不是双启动使用flash头
	}
	rom_puts(rom_main_ch9);
	rom_wdt_feed(); // 喂狗
	//_R8=5;
	// 安全
	if(EFUSE_DATA0 & BIT6 || rom_dataONram.safe_mode) // 使能安全或者软件安全开启
	{
		ROM_error_cnt = 0;
	ROM_SAFE:
		if(EFUSE_DATA0 & BIT4)
			ROM_key_id = EFUSE_DATA0 & 0xf; // 选择efuse id角
		else
		{
			// ID引脚选择
			ROM_key_id = SYSCTL_ID & 0xf; // 选择gpio id脚
		}
		rom_puts(rom_main_ch11);
		rom_SM3_size = 0x1C000 << 3;							  // 112k*8bit
		rom_SM3_Run((DWORD)rom_SM3_size, (DWORDP)0x0, rom_SM2_E); // 112k，SM3 hash
		rom_puts(rom_main_ch12);
		if(rom_SM2_VERIFY(&rom_safeONram.SM2_ssign1, rom_SM2_E, &rom_SM2Pulib_Key[ROM_key_id])) // 根据密钥，签名，hash值验签
		{
			rom_puts(rom_main_ch13);
			if(ROM_error_cnt)
				rom_exit(); // 如果两次都没验签成功则进入特定状态
			else
			{
				ROM_error_cnt++;
				goto ROM_SAFE; // 否则再验签一次
			}
		}
		/* 		else
				{
					#if 0
					rom_puts(rom_main_ch11);
					rom_SM3_size>>=3;
					rom_SM3_Run((((DWORD)rom_dataONram.hash2_size_byte)<<3),(DWORDP)rom_SM3_size,rom_SM2_E);//二级
					rom_puts(rom_main_ch12);
					if (rom_SM2_VERIFY(&rom_safeONram.SM2_ssign2, rom_SM2_E, &rom_safeONram.flash_SM2Pulib_Key))
					{
						rom_puts(rom_main_ch13);
						rom_exit();
					}
					else
					rom_puts(rom_main_ch14);
					#endif
				} */
	}
	else
	{
		rom_puts(rom_main_ch6);
		rom_puts(rom_main_ch11);
	}
	rom_wdt_feed(); // 喂狗
					//_R8=6;
	// flash mirror
	// mirror now
	rom_puts(rom_main_ch15);
	if(ROM_only_extern_flag) // 如果外部启动
	{
		rom_puts(rom_main_ch18);
		// SYSCTL_RST1|=0x00000100;//复位
		// SYSCTL_RST1&=0xfffffeff;//清除复位
		// REG8(0x000bffffUL)=0x0;//0xff;//0x0
		// rom_putc(REG8(0x000bffffUL));
		//_R8=7;
		rom_wdt_feed(); // 喂狗
		rom_SPIF_Icache(ROM_jump);
	}
	else if(rom_dataONram.mirror_once) // 如果开启mirror
	{
		if(REG32(0x3047C) & BIT1) // 如果是外部flash则镜像到内部flash
		{
			// copy extern flash to int
			rom_puts(rom_main_ch16);
			//
			select_flash(INTERNAL_FLASH);
			rom_putc('1');
			ROM_SPIF_ERASE(); // 擦除
			//
			rom_putc('2');
			ROM_COPY_CNT = 0;
			ROM_code_size = 0x8000 << rom_dataONram.first_code_size; // 计算大小
			while(ROM_COPY_CNT < ROM_code_size)
			{
				rom_putc('a');
				select_flash(EXTERNAL_FLASH);
				SPIF_Read_Interface(256, (DWORD)((rom_dataONram.flash_first_offset & 0xffffff00) + ROM_COPY_CNT), (BYTEP)flash_string);
				rom_putc('b');
				select_flash(INTERNAL_FLASH);
				SPIF_Write_Interface(256, (DWORD)((rom_dataONram.flash_first_offset & 0xffffff00) + ROM_COPY_CNT), (BYTEP)flash_string);
				rom_putc('c');
				ROM_COPY_CNT += 256;
			}
			//
			if((!(EFUSE_DATA0 & BIT(17))) && (rom_dataONram.double_boot)) // 是否双启动
			{
				ROM_COPY_CNT = 0;
				ROM_code_size = 0x8000 << rom_dataONram.secound_code_size;
				while(ROM_COPY_CNT < ROM_code_size)
				{
					rom_putc('A');
					select_flash(EXTERNAL_FLASH);
					SPIF_Read_Interface(256, (DWORD)((rom_dataONram.flash_secound_offset & 0xffffff00) + ROM_COPY_CNT), (BYTEP)flash_string);
					rom_putc('B');
					select_flash(INTERNAL_FLASH);
					SPIF_Write_Interface(256, (DWORD)((rom_dataONram.flash_secound_offset & 0xffffff00) + ROM_COPY_CNT), (BYTEP)flash_string);
					rom_putc('C');
					ROM_COPY_CNT += 256;
				}
			}
			//
			rom_putc('\n');
			rom_puts(rom_main_ch14);
		}
		select_flash(INTERNAL_FLASH); // 进入内部flash
		rom_puts(rom_main_ch2);
		rom_puts(rom_main_ch3);
	}
	else
	{
		rom_puts(rom_main_ch4);
		if(REG32(0x3047C) & BIT1) // 如果是外部flash则直接跳转
		{
		}
		else // 否则切换到外部flash再来一次
		{
			select_flash(EXTERNAL_FLASH);
			rom_puts(rom_main_ch17);
			goto EXT_BOOT;
		}
	}
	// ROM_I=24576000;
	// while(ROM_I--);
	rom_puts(rom_main_ch18);
	// 已经选择好内外部flash，交接操作SPIF（cache）
	rom_SPIF_Icache(ROM_jump);
	//_R8=7;
	// rom_wdt_feed();//喂狗
	//(ROM_jump)();	//跳转
}
#endif
#endif
#endif