/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-20 11:16:08
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

volatile uint32_t ROM_TIMEOUT = 0; // 超时
volatile uint64_t READ_FLASH_INFO_TIME = 0;
volatile uint64_t ROM_CSR_MCYCLE = 0;
volatile uint64_t ROM_CSR_MCYCLE_OVERHEAD = 0;

volatile uint32_t FLASH_CTRL = 0;
volatile uint32_t FLASH_ID = 0;
volatile uint32_t internal_flash_state = 0; // 外部状态
volatile uint32_t external_flash_state = 0; // 内部状态
volatile uint32_t external_flash_used = 0;	// 使用外部flash

volatile uint32_t PRINTF_FIFO = 0;
volatile uint32_t DEBUGGER_FIFO = 0;

volatile sFixedFlashInfo INT_FLASH_FIX_INFO = { 0 };
volatile sFixedFlashInfo EXT_FLASH_FIX_INFO = { 0 };
volatile sDynamicFlashInfo INT_FLASH_DYNAMIC_INFO = { 0 };
volatile sDynamicFlashInfo EXT_FLASH_DYNAMIC_INFO = { 0 };
volatile sFixedFlashInfo *FLASH_FIX_INFO_PTR = NULL;
volatile sDynamicFlashInfo *FLASH_DYNAMIC_INFO_PTR = NULL;

#if (!GLE01)
const char FIX_FIRMWARE_ID[] = "SPK32AE103\0";
const char DYN_FIRMWARE_ID[] = "SPK32 AE103\036EC\003\0";
#else
const char FIX_FIRMWARE_ID[] = "SPK32GLE01\0";
const char DYN_FIRMWARE_ID[] = "SPK32 GLE01\036EC\003\0";
#endif
void check_debug(uint32_t looplimit)
{
	if(DEBUGGER_FIFO == (uint32_t)NULL && PRINTF_FIFO == (uint32_t)NULL && 1/*SMBUS*/)return;//前提是都有
	if(((DEBUGGER_LSR | PRINTF_LSR) & UART_LSR_DR) || 0/*SMBUS*/)//检测到有值
	{
		//在对应时间内开始查找操作,f否则退出
		for(uint32_t timeout = 0; timeout <= looplimit; timeout++)
		{
			GET_DEBUGGER_CMD();
			if(debugger_flag || cmd_flag)//查找到则进入该模式
			{
				while(1)
				{
					if(debugger_flag || cmd_flag)
						GET_DEBUGGER_CMD();
					else return;
				}
			}
		}
	}
	return;
}
//----------------------------------------------------------------------------
// FUNCTION: main
// main entry
//----------------------------------------------------------------------------
FUNCT_PTR_V_V jump = NULL;
int main(void)
{
	uint32_t looplimit = (CHIP_CLOCK_INT_HIGH / 15) / (SYSCTL_CLKDIV_OSC80M + 1); // 根据实际测试80M用20M频率需要64sec，因此需要除以16才能达到80M为1sec，20M为4sec，最高32sec
	check_debug(looplimit);
	{									   // 第三步 安全功能验证
		ROM_CNT = ROM_CNT_SECURITY_VERIFY; // cnt = 4
	}
	check_debug(looplimit);
	{							  // 第四步 Mirror代码迁移功能
		ROM_CNT = ROM_CNT_MIRROR; // cnt = 5
	}
	check_debug(looplimit);
	{ // 第五步 BOOT
		uint32_t *ivtcache = (uint32_t *)(INT_FLASH_FIX_INFO.IVT + (INT_FLASH_FIX_INFO.EXTERNAL_FLASH_CTRL.SPACE_OFFSET ? 0 : FLASH_BASE_ADDR));
	#if ROM_DEBUG
		{
			printf("Read ivtcache addr = %#x\n", ivtcache);
		}
	#endif
		uint32_t *ivt = (uint32_t *)IVT_BASE_ADDR;
		while((uint32_t)ivt < IVT_BASE_ADDR + 0x84)
		{
			*ivt++ = *ivtcache++;
		}

		ROM_CNT = ROM_CNT_BOOT;//cnt = 6
		if(!FLASH_FIX_INFO_PTR->DEBUG_LEVEL)
		{
			//如果開了romdebug，則上述打印至少每個字節為76.5us酌情減去才是實際值
			uint64_t time = (uint64_t)((READ_FLASH_INFO_TIME * 1000 * 1000 * 1000) / ((CHIP_CLOCK_INT_HIGH) / (SYSCTL_CLKDIV_OSC80M + 1)));
			// printf("Read Flash_Info cycle :%Ld,time = %Ld,ns\n", READ_FLASH_INFO_TIME, time);
			printf("Read Flash_Info :%Lds %Ldms %Ldus %Ldns\n", (time / 1000 / 1000 / 1000), (time / 1000 / 1000) % 1000, (time / 1000) % 1000, (time / 1) % 1000);

		}
	}
	check_debug(looplimit);
	{//第六步 跳转
		jump = (FUNCT_PTR_V_V)(INT_FLASH_FIX_INFO.Restart + (INT_FLASH_FIX_INFO.EXTERNAL_FLASH_CTRL.SPACE_OFFSET ? 0 : FLASH_BASE_ADDR));
		printf("jump flast start = %#x\n", jump);
		// SELECT_FLASH(EXTERNAL_FLASH, DUAL_FLASH, NOPIN_FLASH, NOPIN_FLASH);
		// SELECT_FLASH(EXTERNAL_FLASH, QUAD_FLASH, PIN5571_FLASH, PIN5571_FLASH);
		// SELECT_FLASH(INTERNAL_FLASH, QUAD_FLASH, NOPIN_FLASH, NOPIN_FLASH);//已经可以不选了
		if(SPIF_CTRL0 & 0x2) // 四线给icache之前需要开启
		{
			for(volatile uint32_t counter = looplimit; (!(SPIF_READY & 0x1)) && counter; counter--)
				;
			SPIF_DBYTE = 0x3;			// DBYTE 5 bytes
			SPIF_FIFO_TOP = 0x40000077; // cmd 77h + 24bits dummy
			for(volatile uint32_t counter = looplimit; (!(SPIF_READY & 0x1)) && counter; counter--)
				;
			for(volatile uint32_t counter = looplimit; (SPIF_STATUS & 0xf) && counter; counter--)
				;
		}
		// WDT_CRR = 0x76;//复位wdt
		(jump)();
	}
	return 0;
}
#if defined(USER_AE10X_LIBC_A)

void exit(int __status)
{
	ROM_CNT = __status;
	uint32_t looplimit = (CHIP_CLOCK_INT_HIGH / 15) / (SYSCTL_CLKDIV_OSC80M + 1); // 重新配置超时机制
	if(__status == 0xFF)//初始化调试口
	{
		looplimit = (CHIP_CLOCK_INT_HIGH / 15) / (SYSCTL_CLKDIV_OSC80M + 1) * 60 * 10;
		u_int32_t bauld = 9600;
		u_int32_t lcr = 3;
		SYSCTL_PIO5_CFG |= (0b1111 << 28);//使用调试器功能，因为此时没有任何信息，所以可以直接这样子配置
		DEBUGGER_FIFO = UART1_BASE_ADDR;//输出也采用uart1
		bauld = UART_Init(UART1_CHANNEL, bauld, lcr);//默认配置
	#if ROM_DEBUG&0
		PRINTF_FIFO = UART0_BASE_ADDR;//输出也采用uart1
	#if 1
		DEBUGGER_FIFO = PRINTF_FIFO;
	#endif
		bauld = UART_Init(UART0_CHANNEL, 115200, lcr);//默认配置
		printf("NO FIRMWARE USED UART0 DEBUG , BAUD = %d\n", bauld);
	#else
		PRINTF_FIFO = (u_int32_t)NULL;
	#endif
	}
	else if(__status == 0xFE)//初始化调试口
	{

	}
	_exit(looplimit);

}

NORETURN USED void _exit(int looplimit)
{
	uint32_t timeout = 0;
	while(1)
	{
		GET_DEBUGGER_CMD();//return 1:run a cmd 0,no run cmd or get cmd,-1:exit
		if(debugger_flag || cmd_flag)
			timeout = 0;
		else
			timeout++;
		if(timeout == (uint32_t)looplimit)//达到最大值的时候重启或者退出
		{
			if(FLASH_FIX_INFO_PTR != NULL && FLASH_FIX_INFO_PTR->EXIT_ReBOOT_Switch == 0)
			{
				timeout = 0;
				//sleep
				/////////cpu csr mstatus.MIE配置为0（COU全局中断关闭）
				/////////cpu csr wfe.WFE配置为0
				/////////debugger uart irq init
				/////////low power
				/////////wfi
				//wake up
				/////////recovery
			}
			else
			{
				SYSCTL_RST1 |= BIT(16) | BIT(15);
				_start();//如果没有芯片复位，则从这里跳入
			}
		}
		nop;

	}
}
#endif