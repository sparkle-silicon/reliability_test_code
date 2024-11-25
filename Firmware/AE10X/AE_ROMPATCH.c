/*
 * @Author: dejavuwdh
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-11-25 15:51:45
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
#include "AE_ROMPATCH.H"

#ifdef AE103
// 填入绝对地址
#define PATCH0_ADDR 0x10084
#define PATCH0_DATA 0x80084 // 0x9F800//PATCH0_TRANSIT
#define PATCH1_ADDR 0x10400
#define PATCH1_DATA 0x9F880 // PATCH1_TRANSIT
#if 0
#define PATCH2_ADDR 0x10084
#define PATCH2_DATA 0x9F900 // PATCH2_TRANSIT
#define PATCH3_ADDR 0x10084
#define PATCH3_DATA 0x9F980 // PATCH3_TRANSIT
#define PATCH4_ADDR 0x10084
#define PATCH4_DATA 0x9FA00 // PATCH4_TRANSIT
#define PATCH5_ADDR 0x10084
#define PATCH5_DATA 0x9FA80 // PATCH5_TRANSIT
#define PATCH6_ADDR 0x10084
#define PATCH6_DATA 0x9FB00 // PATCH6_TRANSIT
#define PATCH7_ADDR 0x10084
#define PATCH7_DATA 0x9FB80 // PATCH7_TRANSIT
#endif
#if NEW_ROM
// flash mirror and flash string
extern sDynamicFlashInfo Dy_flash_info;
SECTION(".FlashInfo.Fixed")
USED sFixedFlashInfo Fix_flash_info = {
#if GLE01
	.Firmware_ID = "SPK32GLE01\0",
#else
	.Firmware_ID = "SPK32AE103\0", // 特殊字符
#endif

	.Compiler_Version = __DATE__ " "__TIME__
								 "\0",
	// 外部FLASH控制
	.EXTERNAL_FLASH_CTRL.ENABLE_USED = 0b1,			// 允许使用外部FLASH（内部为准）
	.EXTERNAL_FLASH_CTRL.LAST8M_DISABLE = 0b1,		// 关闭后从后8M开始检索(内部为准)
	.EXTERNAL_FLASH_CTRL.LowAddr_OFFSET = 0b11,		// 4k（内部为准）
	.EXTERNAL_FLASH_CTRL.HighAddr_SPACE = 0b111,	//[23:x]（内部为准）
	.EXTERNAL_FLASH_CTRL.MUST_MIRROR_DISABLE = 0b1, // ，一般关闭，需要强制更新时候开启（外部为准）
	.EXTERNAL_FLASH_CTRL.PWM_Enable = 0b1,
	.EXTERNAL_FLASH_CTRL.PWMn_Switch = 0b0,
	.EXTERNAL_FLASH_CTRL.SPI_Switch = 0b1,	// 4线
	.EXTERNAL_FLASH_CTRL.WP_Switch = 0b1,	// 5571pin
	.EXTERNAL_FLASH_CTRL.HOLD_Switch = 0b1, // 5571pin
	.EXTERNAL_FLASH_CTRL.Firmware_4KSector = (Firmware_nKsize / 4 - 1),

	// FLASH	地址
	.IVT = ((uint32_t)&vector_base - (FLASH_BASE_ADDR)), // 中断向量表
	.Restart = ((uint32_t)&_start - (FLASH_BASE_ADDR)),  // 起始地址
	// .DynamicFlashInfo = ((uint32_t)&Dy_flash_info - (FLASH_BASE_ADDR)), // 由编译填入
		.PATCH = {
		[0] .data.dword = 0xa001,
		[0].addrl = (PATCH0_ADDR >> 2) & 0xff,
		[0].addrh = (PATCH0_ADDR >> 10) & 0x1f,
		[0].last = 1,
		[1].data.dword = 0x00010001,
		[1].addrl = (PATCH1_ADDR >> 2) & 0xff,
		[1].addrh = (PATCH1_ADDR >> 10) & 0x1f,
		[1].last = 0,
	},
	// 主频
	.MainFrequency = CHIP_CLOCK_SWITCH,
	// 验签
	.SECVER_Enable = 0,		 // 0：EFUSE决定，1：安全验签测试输出（默认）
	// .SECVER_VERIFY_Switch = 1,//RSA//无效
	// .SECVER_AES_Enable = 0,//disable aes//无效
	.SECVER_HASH_Switch = 1,//SHA256
	// .SECVER_BIT_Switch = 1,//rsa 2048bit//无效,ecc的时候生效
	// .SECVER_AES_MODE_Switch = 3,//aes-256-ecb//无效

	//backup
	.BACKUP_Enable = 1,
	.BACKUP_LOCACTION_Switch = 1,
	.Backup_OFFSET = 0x40000,

	.EXIT_ReBOOT_Switch = 1, // （安全失败后一段时间）0休眠 1自动重启（默认）
							 // 模块
	.PATCH_Disable = 0,		 // 0:打开ROM PATCH，1：关闭rom patch（默认）
	.LPC_Enable = 1,		 // 0：ESPI口 1：LPC口
// 调试手段

	.EJTAG_Enable = 1,
	.EJTAG_Switch = 1,
	.CRYPTO_EJTAG_Switch = 1,
	.DEBUG_PRINTF_Enable = 0,
	.Uartn_Print_SWitch = PRINTF_UART_SWITCH,
	.DEBUGGER_Enable = 1,
	.DEBUGGER_UART_Enable = 1,
	.DEBUGGER_SMBUS_Enable = 1,
	.SMBUS_CLOCK_Switch = 1,
	.DEBUG_BAUD_RATE = (UART_BAUD / 3200) - 1,
	.DEBUG_LEVEL = 1,
	.DEBUG_PRINTF_DLS = 0b11,
	.DEBUG_PRINTF_STOP = 0,
	.DEBUG_PRINTF_PE = 0,
	.DEBUG_PRINTF_EPE = 0,
	.DEBUGGER_BAUD_RATE = 0,
	.DEBUGGER_DLS = 0b11,
	.DEBUGGER_STOP = 0,
	.DEBUGGER_PE = 0,
	.DEBUGGER_EPE = 0,
};
// SECTION(".FlashInfo.Dynamic")
// USED sDynamicFlashInfo Dy_flash_info = {
// #if GLE01
// 	.Firmware_ID = "SPK32 GLE01\036EC\003\0",
// #else
// 	.Firmware_ID = "SPK32 AE103\036EC\003\0", // 特殊字符
// #endif
// 	.sign = {
// 		.hash = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
// 	},
// 	.publickey = {
// 		.rsa = {
// 			.modulus = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
// 			.exponent = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
// 		},
// 	},
// 	.Info = {
// 		.EXTFlash_ID = 0xFFFFFFFF,
// 		.FixedFlashInfo_Addr = 0xFFFFFF,
// 	},
// };
#else
SECTION(".FlashInfo.Dynamic")
USED sDynamicFlashInfo flash_magic_number = {
	.magic_number = "103\036SPK\0", // 特殊字符
};

USED BYTE magic[20] = __DATE__ " "__TIME__;

SECTION(".FlashInfo.Fixed")
USED sFixedFlashInfo flash_info = {
	.debug_uart_en = 1,					  // uart 输出开关
	.debug_uart_com = PRINTF_UART_SWITCH, // uart输出端口
	.debug_uart_baud = UART_BAUD,
	.jtag_en = 0, // jtag使能
	.jtag_tdi = 1,
	.jtag_tdo = 1,
	.jtag_tms = 1,
	.jtag_tck = 1,
	.flash_size = 0b11,			   // 00:32K,01:64K,10:128K,11:256K,100:512K...
	.flash_first_offset = 0x00084, // 24
	.first_code_size = 0b0010,	   // 00:32K,01:64K,10:128K,11:256K,100:512K...
	.flash_secound_offset = (DoubleBoot_DEF_ADDR - 0x80000),
	.secound_code_size = 0b0010,		  // 00:32K,01:64K,10:128K,11:256K,100:512K...
	.main_freq = (CHIP_CLOCK_SWITCH - 1), // 4分频
	.extern_osc_flag = OSC_CLOCK_SWITCH,  // 0：内部osc，1：外部osc
#ifdef DUBLE_FIRMWARE1
	.double_boot = 0b1, // 双启动（1开0关）
#else
	.double_boot = 0b0, // 不双启动（1开0关）
#endif
	.safe_mode = 0b0,		// 安全模式（1开，0由EFUSE决定）
	.mirror_once = 0b0,		// 镜像（1开,内部，0关，外部）
	.patch_en = 0b0,		// 使能patch
	.patch_total = 2,		// patch个数
	.flash_qspi_flag = 0b0, // 0：两线，1：四线
	.flash_wp = 0,			// ec103b select spif wp (0:PIOB17,1:PIOD8)
	.flash_hold = 1,		// ec103a select spif hold (0：PIOA16,1:PIOB19)
	.rom_wdt_time = 0xffff,
};
#endif

// SECTION(".ROM_PATCH")
// USED spatch_typdef patch_array[8] = {
// // 0x10084
// #if (defined(PATCH0_ADDR) && defined(PATCH0_DATA))
// 	[0].addrl = (PATCH0_ADDR >> 2) & 0xff,
// 	[0].addrh = (PATCH0_ADDR >> 10) & 0x1f,
// 	[0].data.dword = JAL_CODE(X1_RA, PATCH0_ADDR, PATCH0_DATA),
// 	/* [0].data.byte[0]=JAL_CODE_BYTE0(X1_RA),
// 	[0].data.byte[1]=JAL_CODE_BYTE1(X1_RA,JAL_ADDR_OFFSET(PATCH0_ADDR,PATCH0_DATA)),
// 	[0].data.byte[2]=JAL_CODE_BYTE2(JAL_ADDR_OFFSET(PATCH0_ADDR,PATCH0_DATA)),
// 	[0].data.byte[3]=JAL_CODE_BYTE3(JAL_ADDR_OFFSET(PATCH0_ADDR,PATCH0_DATA)), */
// #endif
// #if (defined(PATCH1_ADDR) && defined(PATCH1_DATA))
// 	[1].addrl = (PATCH1_ADDR >> 2) & 0xff,
// 	[1].addrh = (PATCH1_ADDR >> 10) & 0x1f,
// 	[1].data.dword = JAL_CODE(X1_RA, PATCH1_ADDR, PATCH1_DATA),
// #endif
// #if (defined(PATCH2_ADDR) && defined(PATCH2_DATA))
// 	[2].addrl = (PATCH2_ADDR >> 2) & 0xff,
// 	[2].addrh = (PATCH2_ADDR >> 10) & 0x1f,
// 	[2].data.dword = JAL_CODE(X1_RA, PATCH2_ADDR, PATCH2_DATA),
// #endif
// #if (defined(PATCH3_ADDR) && defined(PATCH3_DATA))
// 	[3].addrl = (PATCH3_ADDR >> 2) & 0xff,
// 	[3].addrh = (PATCH3_ADDR >> 10) & 0x1f,
// 	[3].data.dword = JAL_CODE(X1_RA, PATCH3_ADDR, PATCH3_DATA),
// #endif
// #if (defined(PATCH4_ADDR) && defined(PATCH4_DATA))
// 	[4].addrl = (PATCH4_ADDR >> 2) & 0xff,
// 	[4].addrh = (PATCH4_ADDR >> 10) & 0x1f,
// 	[4].data.dword = JAL_CODE(X1_RA, PATCH4_ADDR, PATCH4_DATA),
// #endif
// #if (defined(PATCH5_ADDR) && defined(PATCH5_DATA))
// 	[5].addrl = (PATCH5_ADDR >> 2) & 0xff,
// 	[5].addrh = (PATCH5_ADDR >> 10) & 0x1f,
// 	[5].data.dword = JAL_CODE(X1_RA, PATCH5_ADDR, PATCH5_DATA),
// #endif
// #if (defined(PATCH6_ADDR) && defined(PATCH6_DATA))
// 	[6].addrl = (PATCH6_ADDR >> 2) & 0xff,
// 	[6].addrh = (PATCH6_ADDR >> 10) & 0x1f,
// 	[6].data.dword = JAL_CODE(X1_RA, PATCH6_ADDR, PATCH6_DATA),
// #endif
// #if (defined(PATCH7_ADDR) && defined(PATCH7_DATA))
// 	[7].addrl = (PATCH7_ADDR >> 2) & 0xff,
// 	[7].addrh = (PATCH7_ADDR >> 10) & 0x1f,
// 	[7].data.dword = JAL_CODE(X1_RA, PATCH7_ADDR, PATCH7_DATA),
// #endif
// };
void OPTIMIZE0 rom_patch_config_func(void)
{
	printf("rom patch config here\n");
	/*设置ROM PATCH*/
	// 112a0
	PATCH_CTRL |= 0x1;	   // 使能patch0
	PATCH_ADDRL(0) = 0xA8; // 设置patch0_addr0_0
	PATCH_ADDRH(0) = 0x4;  // 设置patch0_addr0_1
	// 10084
	asm volatile(
		"mv a0, %0\n"
		:
	: "r"(0x00080420)
		: "a0");
	// jr a0 8502
	PATCH_DATA0(0) = 0x02; // 设置patch0_data0_0
	PATCH_DATA1(0) = 0x85; // 设置patch0_data0_1
	PATCH_DATA2(0) = 0x00; // 设置patch0_data0_2
	PATCH_DATA3(0) = 0x00; // 设置patch0_data0_3
}
/***********************ROM PATCH FUNCTION*************************/
void OPTIMIZE0 rom_patch_test_func_flash(void)
{
	printf("This is rom patch test,run in flash\n");
	PATCH_CTRL &= 0xfe; // patch0
	// asm volatile (
	//     "mv t0, %0\n"
	// 	"mv x1, %1\n"
	// 	"jr t0\n"
	//     :
	//     : "r" (0x111e8), "r" (0x112a2)
	//     : "t0"
	// );
	asm volatile(
		"lw	ra,8(sp)\n"
		"lw	s0,4(sp)\n"
		"addi sp,sp,12\n");
	asm volatile(
		"mv a0, %0\n"
		"jr a0\n"
		:
	: "r"(0x112a2)
		: "a0");
}
typedef DWORD(*FUNCT_PTR_D_B)(BYTE);
typedef void (*FUNCT_PTR_V_D)(DWORD);
typedef void (*FUNCT_PTR_V_BP)(BYTEP);
typedef void (*FUNCT_PTR_V_D_D_BP)(DWORD, DWORD, BYTEP);
typedef VBYTEP(*FUNCT_PTR_VBP_D_D_BP)(DWORD, DWORD, BYTEP);
typedef BYTE(*FUNCT_PTR_B_s_DP_s)(ssign *, DWORDP, spoint *);
typedef void (*FUNCT_PTR_V_D_B_BP_BP_BP_BP)(DWORD, BYTE, BYTEP, BYTEP, BYTEP, BYTEP);
typedef void (*FUNCT_PTR_V_D_DP_DP)(DWORD, DWORDP, DWORDP);
BYTE rom_patch_demo(BYTE offset, BYTE param1)
{
	// char *rompatch_test_ch = "This is rompatch test\n\0";
	// char *receive_buffer = "ffffffffffffffffffff\n\0";
	// switch(offset)
	// {
	// 	case 0x0:  // 0x10084=
	// 	case 0x4:  // rom_main=
	// 	case 0x8:  // rom_wdt_feed=
	// 	case 0x18: // rom_uart=
	// 	case 0x1C: // rom_ejtag=
	// 	case 0x30: // ROM_SPIF_ERASE=
	// 	case 0x44: // rom_exit=
	// 		((FUNCT_PTR_V_V)(*(DWORDP)(0x10000 + offset)))();
	// 		break;
	// 	case 0xC:  // rom_putc=
	// 	case 0x24: // select_flash=
	// 	case 0x3C: // ROM_Update
	// 	case 0x40: // ROM_SM_Update
	// 		((FUNCT_PTR_V_B) * (DWORDP)(0x10000 + offset))(param1);
	// 		break;
	// 	case 0x10: // rom_puts=
	// 		((FUNCT_PTR_V_BP) * (DWORDP)(0x10000 + offset))((BYTEP)rompatch_test_ch);
	// 		break;
	// 	case 0x14: // rom_get_cpu_freq=
	// 		((FUNCT_PTR_D_B) * (DWORDP)(0x10000 + offset))(param1);
	// 		break;
	// 	case 0x20: // rom_wdt=
	// 		((FUNCT_PTR_V_D) * (DWORDP)(0x10000 + offset))(0xffff);
	// 		break;
	// 	case 0x28: // rom_memset
	// 	case 0x2C: // ROM_strcmp
	// 	case 0x34: // SPIF_Write_Interface=
	// 		((FUNCT_PTR_V_D_D_BP) * (DWORDP)(0x10000 + offset))(0x10, 0x22000, (BYTEP)rompatch_test_ch);
	// 		break;
	// 	case 0x38: // SPIF_Read_Interface=
	// 		((FUNCT_PTR_VBP_D_D_BP) * (DWORDP)(0x10000 + offset))(0x10, 0x22000, (BYTEP)receive_buffer);
	// 		((FUNCT_PTR_V_BP) * (DWORDP)(0x10010))((BYTEP)receive_buffer);
	// 		break;
	// 	// case 0x48:	//rom_SM2Pulib_Key=
	// 	case 0x4C: // rom_SM2_VERIFY=
	// 		return ((FUNCT_PTR_B_s_DP_s) * (DWORDP)(0x10000 + offset))(&rom_safeONram.SM2_ssign1, rom_SM2_E, &((spoint *)(*(DWORDP)0x10048))[ROM_key_id]);
	// 		break;
	// 	case 0x50: // rom_SM4_Run
	// 		// FUNCT_PTR_V_D_B_BP_BP_BP_BP	(0x10000 + offset)();
	// 		break;
	// 	case 0x54: // rom_SM3_Run=
	// 		((FUNCT_PTR_V_D_DP_DP) * (DWORDP)(0x10000 + offset))((DWORD)rom_SM3_size, (DWORDP)0x0, rom_SM2_E);
	// 		break;
	// 	case 0x58: // ROM_interface
	// 		// FUNCT_PTR_V_D	(0x10000 + offset)(0x0);	//0:internal_flash; 1:external_flash
	// 		break;
	// 	default:
	// 		dprint("ROMPATCH_DEMO offset ERROR\n");
	// 		break;
	// }
	return 0;
}
void ALIGNED(4) OPTIMIZE0 clear_internel_flash(void)
{
	u32 spif_ctrl0 = SPIF_CTRL0;
	REG32(0x3047C) &= 0xfffffffd;
	SYSCTL_RST1 |= 0x00000100; // 复位
	SYSCTL_RST1 &= 0xfffffeff; // 清除复位
	while(!(SPIF_READY & 0x1))
		WDT_CRR = WDT_CRR_CRR; // 读忙
	while(SPIF_STATUS & 0xf)
		WDT_CRR = WDT_CRR_CRR; // 直到写完
	while(!(SPIF_READY & 0x1))
		WDT_CRR = WDT_CRR_CRR; // 读忙
	SPIF_FIFO_TOP = 0xc7;
	while(!(SPIF_READY & 0x1))
		WDT_CRR = WDT_CRR_CRR; // 读忙
	while(SPIF_STATUS & 0xf)
		WDT_CRR = WDT_CRR_CRR; // 直到写完
	while(!(SPIF_READY & 0x1))
		WDT_CRR = WDT_CRR_CRR; // 读忙
	REG32(0x3047C) |= 0x00000002;
	SYSCTL_RST1 |= 0x00000100; // 复位
	__nop;
	__nop;
	__nop;
	SYSCTL_RST1 &= 0xfffffeff; // 清除复位
	SPIF_CTRL0 = spif_ctrl0;
	__nop;
	__nop;
	__nop;
	while(SPIF_STATUS & 0xf)
		WDT_CRR = WDT_CRR_CRR; // 直到写完
	while(!(SPIF_READY & 0x1))
		WDT_CRR = WDT_CRR_CRR; // 读忙
	__nop;
	__nop;
	__nop;
}
void earse_internel_flash(void)
{
	if(SYSCTL_PIO4_UDCFG & BIT1)
	{
		FUNCT_PTR_V_V funct_ptr = Load_funVV_To_Dram(clear_internel_flash, 0x400);
		(*funct_ptr)();
	}
}
// PATCH demo
void SECTION(".PATCH0_TRANSIT") USED PATCH0_TRANSIT(void)
{
	goto * 0x80084UL;
	return;
}
void SECTION(".PATCH1_TRANSIT") USED PATCH1_TRANSIT(void)
{
	main();
	return;
}
void SECTION(".PATCH2_TRANSIT") USED PATCH2_TRANSIT(void)
{
	return;
}
void SECTION(".PATCH3_TRANSIT") USED PATCH3_TRANSIT(void)
{
	return;
}
void SECTION(".PATCH4_TRANSIT") USED PATCH4_TRANSIT(void)
{
	return;
}
void SECTION(".PATCH5_TRANSIT") USED PATCH5_TRANSIT(void)
{
	return;
}
void SECTION(".PATCH6_TRANSIT") USED PATCH6_TRANSIT(void)
{
	return;
}
void SECTION(".PATCH7_TRANSIT") USED PATCH7_TRANSIT(void)
{
	return;
}
#endif