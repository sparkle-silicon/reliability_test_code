/*
 * @Author: dejavuwdh
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-03-27 21:25:24
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
#include "AE_FLASHINFO.H"

#ifdef AE103
// 填入绝对地址
#define PATCH0_ADDR 0x1000C
#define PATCH0_DATA 0x80084 // 0x9F800//PATCH0_TRANSIT
#define PATCH1_ADDR 0x10010
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
	.EXTERNAL_FLASH_CTRL.PWM_Enable = 0b1,//MIRROR期间 0：关闭pwm闪烁，1：开启pwm闪烁
	.EXTERNAL_FLASH_CTRL.PWMn_Switch = 0b0,
	.EXTERNAL_FLASH_CTRL.SPI_Switch = 0b1,	// 4线
	.EXTERNAL_FLASH_CTRL.WP_Switch = 0b11,	// piob29
	.EXTERNAL_FLASH_CTRL.CS_Switch = 0b0, // fspi_csn0
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
	.MainFrequency = CHIP_CLOCK_SWITCH,// 主频分频系数 
	// 验签
	.SECVER_Enable = 1,		 // 0：EFUSE决定，1：安全验签测试输出（默认）
	// .SECVER_VERIFY_Switch = 1,//RSA//无效
	// .SECVER_AES_Enable = 0,//disable aes//无效
	.SECVER_HASH_Switch = 1,// 0:SHA224 1：SHA256（默认） 2：SHA384 3：SHA512
	// .SECVER_BIT_Switch = 1,//rsa 2048bit//无效,ecc的时候生效
	// .SECVER_AES_MODE_Switch = 3,//aes-256-ecb//无效

	//backup
	.BACKUP_Enable = 1,
	.BACKUP_LOCACTION_Switch = 1,
	.Backup_OFFSET = (Firmware_nKsize * 1024),

	.EXIT_ReBOOT_Switch = 1, // （安全失败后一段时间）0休眠 1自动重启（默认）
							 // 模块
	.PATCH_Disable = 1,		 // 0:打开ROM PATCH，1：关闭rom patch（默认）
	.LPC_Enable = 1,		 // 0：ESPI口 1：LPC口
// 调试手段

	.EJTAG_Enable = 1,//0：关闭EJTAG，1:开启ejtag（默认）
	.EJTAG_Switch = 1,//0：PIOE[13:10]为KBS则配置PIOB[30:27]为ejtag口, 1：PIOE[13:10]（默认）
	.CRYPTO_EJTAG_Switch = 1,//0：PIOE[13:10]配置为CRYPTO口, 1：PIOE[13:10]保留默认配置
	.DEBUG_PRINTF_Enable = 1,// 0：关闭调试串口，1：打开调试口
	.Uartn_Print_SWitch = PRINTF_UART_SWITCH,//该口作为调试输出口选择（UART 01AB），剩余接口作为调试器
	.DEBUGGER_Enable = 1,// 0：关闭调试器功能，
	.DEBUGGER_UART_Enable = 1,// 0：关闭调试器口，1：打开所有调试口
	.UART1_TXD_SWITCHE = 0,//piob1
	.UART1_RXD_SWITCHE = 0,//piob3
	.UARTA_TXD_SWITCHE = 1,//PIOA9
	.UARTA_RXD_SWITCHE = 1,//PIOA8
	.DEBUG_BAUD_RATE = (UART_BAUD / 3200) - 1,// debug输出波特率(3200倍数，0)
	.DEBUG_LEVEL = 1,//0:更详细的过程参数1:关闭仅输出过程位置
	.DEBUG_PRINTF_DLS = 0b11,// 数据位(3)
	.DEBUG_PRINTF_STOP = 0,// 停止位(0)
	.DEBUG_PRINTF_PE = 0,// 校验位（0）
	.DEBUG_PRINTF_EPE = 0,//奇偶校验选择（0奇校验）
	.DEBUGGER_BAUD_RATE = (UART_BAUD / 3200) - 1,// debugger波特率(3200倍数)
	.DEBUGGER_DLS = 0b11,// 数据位(3)
	.DEBUGGER_STOP = 0,//  停止位(0)
	.DEBUGGER_PE = 0,// 校验位（0）
	.DEBUGGER_EPE = 0,//奇偶校验选择（0）
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
typedef struct _efuse_info
{
	uint32_t trim_ldo1 : 8;//EFUSE_DATA[7:0]
	uint32_t trim_bg : 8;//EFUSE_DATA[15:8]
	uint32_t rom_patch_disable : 1;//EFUSE_DATA[16]
	uint32_t wakeup_bypass_security_disable : 1;//EFUSE_DATA[17]
	uint32_t program_disable : 1;//EFUSE_DATA[18]
	uint32_t crypto_normal : 1;//EFUSE_DATA[19]
	uint32_t trim_32k : 12;//EFUSE_DATA[31:20]
	uint32_t crypto_clkgate_disable : 1;//EFUSE_DATA[32]
	uint32_t trim_96m : 10;//EFUSE_DATA[42:33]
	uint32_t reserved0 : 3;//EFUSE_DATA[45:43]
	uint32_t crypto_rompatch_disable : 1;//EFUSE_DATA[46]
	uint32_t eflash_512k : 1;//EFUSE_DATA[47]
	uint32_t ec_debug : 1;//     EFUSE_DATA[48]
	uint32_t crypto_debug : 1;//EFUSE_DATA[49]
	uint32_t hash_size : 7;//EFUSE_DATA[56:50]
	uint32_t crypto_en : 1;//EFUSE_DATA[57]
	uint32_t crypto_jtag_disable : 1;//EFUSE_DATA[58]
	uint32_t jtag_disable : 1;//EFUSE_DATA[59]
	uint32_t security : 1;//EFUSE_DATA[60]
	uint32_t speedup : 1;//EFUSE_DATA[61]
	uint32_t reserved1 : 2;//EFUSE_DATA[63：62]
	uint32_t crc32_chip_config;//EFUSE_DATA[95:64]
	uint32_t pubkey_hash[8];//EFUSE_DATA[351:96]
	uint32_t aes_key[4];//EFUSE_DATA[479:352]
	uint32_t crc32_aes_key_hash;//EFUSE_DATA[511:480]
} sefuse_info;
typedef union _efuse_wd
{
	sefuse_info efuse_info;
	uint32_t efuse_data[16];
} uefuse_wd;
/******************************************************************************
 * Name:    CRC-32  x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
 * Note:    d for闂備胶鍎甸弲娑㈡偤閵娧勬殰閻庨潧鎲℃刊濂告煥濞戞ê顏柡鍡嫹
 *****************************************************************************/
uint32_t  crc32_d(uint8_t *data, uint32_t length)
{
	uint8_t i;
	uint32_t crc = 0xffffffff;        // Initial value
	while(length--)
	{
		crc ^= *data++;                // crc ^= *data; data++;
		for(i = 0; i < 8; ++i)
		{
			if(crc & 1)
				crc = (crc >> 1) ^ 0xEDB88320;// 0xEDB88320= reverse 0x04C11DB7
			else
				crc = (crc >> 1);
		}
	}
	return ~crc;
}
#define EFUSE_BASE_ADDR 0x32C00
#define EFUSE_WD0_OFFSET 0x0
#define EFUSE_WD1_OFFSET 0x4
#define EFUSE_WD2_OFFSET 0x8
#define EFUSE_WD3_OFFSET 0xC
#define EFUSE_WD4_OFFSET 0x10
#define EFUSE_WD5_OFFSET 0x14
#define EFUSE_WD6_OFFSET 0x18
#define EFUSE_WD7_OFFSET 0x1C
#define EFUSE_WD8_OFFSET 0x20
#define EFUSE_WD9_OFFSET 0x24
#define EFUSE_WD10_OFFSET 0x28
#define EFUSE_WD11_OFFSET 0x2C
#define EFUSE_WD12_OFFSET 0x30
#define EFUSE_WD13_OFFSET 0x34
#define EFUSE_WD14_OFFSET 0x38
#define EFUSE_WD15_OFFSET 0x3C
#define EFUSE_CFG_OFFSET 0x40
#define EFUSE_FPGA_CFG_OFFSET 0x44

#define EFUSE_REG_ADDR(offset) REG_ADDR(EFUSE_BASE_ADDR, offset)
#define EFUSE_REG(offset) REG32(EFUSE_REG_ADDR(offset))

#define EFUSE_WD(n) EFUSE_REG((EFUSE_WD0_OFFSET + (((n) & 0x0F) << 2)))
#define EFUSE_WD0 EFUSE_REG(EFUSE_WD0_OFFSET)//EFUSE_DATA[0:31]
#define EFUSE_WD1 EFUSE_REG(EFUSE_WD1_OFFSET)//EFUSE_DATA[32:63]
#define EFUSE_WD2 EFUSE_REG(EFUSE_WD2_OFFSET)//EFUSE_DATA[64:95]
#define EFUSE_WD3 EFUSE_REG(EFUSE_WD3_OFFSET)//EFUSE_DATA[96:127]
#define EFUSE_WD4 EFUSE_REG(EFUSE_WD4_OFFSET)//EFUSE_DATA[128:159]
#define EFUSE_WD5 EFUSE_REG(EFUSE_WD5_OFFSET)//EFUSE_DATA[160:191]
#define EFUSE_WD6 EFUSE_REG(EFUSE_WD6_OFFSET)//EFUSE_DATA[192:223]
#define EFUSE_WD7 EFUSE_REG(EFUSE_WD7_OFFSET)//EFUSE_DATA[224:255]
#define EFUSE_WD8 EFUSE_REG(EFUSE_WD8_OFFSET)//EFUSE_DATA[256:287]
#define EFUSE_WD9 EFUSE_REG(EFUSE_WD9_OFFSET)//EFUSE_DATA[288:319]
#define EFUSE_WD10 EFUSE_REG(EFUSE_WD10_OFFSET)//EFUSE_DATA[320:351]
#define EFUSE_WD11 EFUSE_REG(EFUSE_WD11_OFFSET)//EFUSE_DATA[352:383]
#define EFUSE_WD12 EFUSE_REG(EFUSE_WD12_OFFSET)//EFUSE_DATA[384:415]
#define EFUSE_WD13 EFUSE_REG(EFUSE_WD13_OFFSET)//EFUSE_DATA[416:447]
#define EFUSE_WD14 EFUSE_REG(EFUSE_WD14_OFFSET)//EFUSE_DATA[448:479]
#define EFUSE_WD15 EFUSE_REG(EFUSE_WD15_OFFSET)//EFUSE_DATA[480:511]
#define EFUSE_CFG EFUSE_REG(EFUSE_CFG_OFFSET)//EFUSE_CFG
#define EFUSE_FPGA_CFG EFUSE_REG(EFUSE_FPGA_CFG_OFFSET)//EFUSE_FPGA_CFG

#define EFUSE_TRIM_LDO1                     ((EFUSE_WD0&0x000000FF)>>0)//EFUSE_DATA[7:0]//CHIP POWER LDO1 TRIM
#define EFUSE_TRIM_BG                       ((EFUSE_WD0&0x0000FF00)>>8)//EFUSE_DATA[15:8]//CHIP POWER BG TRIM
#define EFUSE_ROMPATCH_DISABLE              ((EFUSE_WD0&0x00010000)>>16)//EFUSE_DATA[16]//0:rom patch enable 1:rom patch disable
#define EFUSE_WAKEUP_BYPASS_SECUITY_DISABLE ((EFUSE_WD0&0x00020000)>>17)//EFUSE_DATA[17]//0:enable wakeup bypass security  1:disable wakeup bypass security 
#define EFUSE_PROGRAM_DISABLE               ((EFUSE_WD0&0x00040000)>>18)//EFUSE_DATA[18]//0:program enable 1:program disable
#define EFUSE_CRYPTO_NORMAL                 ((EFUSE_WD0&0x00080000)>>19)//EFUSE_DATA[19]
#define EFUSE_TRIM_32K                      ((EFUSE_WD0&0xFFF00000)>>20)////EFUSE_DATA[31:20]
#define EFUSE_CRYPTO_CLKGATE_DISABLE        ((EFUSE_WD1&0x00000001)>>(32-32))//EFUSE_DATA[32]
#define EFUSE_TRIM_96M                      ((EFUSE_WD1&0x000007fe)>>(33-32))//EFUSE_DATA[42:33]
#define EFUSE_RESERVED0                     ((EFUSE_WD1&0x00003800)>>(43-32))//EFUSE_DATA[45:43]
#define EFUSE_CRYPTO_ROMPATCH_DISABLE       ((EFUSE_WD1&0x00004000)>>(46-32))//EFUSE_DATA[46]
#define EFUSE_EFLASH_512K                   ((EFUSE_WD1&0x00008000)>>(47-32))//EFUSE_DATA[47]
#define EFUSE_EC_DEBUG                      ((EFUSE_WD1&0x00010000)>>(48-32))//EFUSE_DATA[48]
#define EFUSE_CRYPTO_DEBUG                  ((EFUSE_WD1&0x00020000)>>(49-32))//EFUSE_DATA[49]
#define EFUSE_HASH_SIZE                     ((EFUSE_WD1&0x01fc0000)>>(50-32))//EFUSE_DATA[56:50]
#define EFUSE_CRYPTO_EN                     ((EFUSE_WD1&0x02000000)>>(57-32))//EFUSE_DATA[57]
#define EFUSE_CRYPTO_JTAG_DISABLE           ((EFUSE_WD1&0x04000000)>>(58-32))//EFUSE_DATA[58]
#define EFUSE_JTAG_DISABLE                  ((EFUSE_WD1&0x08000000)>>(59-32))//EFUSE_DATA[59]
#define EFUSE_SECURITY                      ((EFUSE_WD1&0x10000000)>>(60-32))//EFUSE_DATA[60]
#define EFUSE_SPEEDUP                       ((EFUSE_WD1&0x20000000)>>(61-32))//EFUSE_DATA[61]
#define EFUSE_RESERVED1                     ((EFUSE_WD1&0xC0000000)>>(62-32))//EFUSE_DATA[63:62]
#define EFUSE_CRC32_CHIP_CONFIG             EFUSE_WD2//EFUSE_DATA[95:64]
#define EFUSE_PUBKEY_HASH(n)                (EFUSE_WD((3+((n)&0x7))))//EFUSE_DATA[351:96]
#define EFUSE_AES_KEY(n)                    (EFUSE_WD((11+((n)&0x3))))//EFUSE_DATA[479:352]
#define EFUSE_CRC32_PUBKEYHASH_AESKEY        EFUSE_WD15//EFUSE_DATA[511:480]

extern void write_efuse_data(uint32_t *data);
void write_efuse_data(uint32_t *data)
{
#if 1
	uint8_t pubkey_hash_SHA256_aes128ecb_AESKEY_byte[32] = {
	//       /*test0512*/ 0x90,0x3A,0xD0,0x62,0x0E,0xD4,0x09,0xBF,0xED,0xB1,0xBB,0x0A,0x69,0xC4,0x8D,0x3E,0xA8,0x19,0x38,0xBE,0x50,0x3D,0x4F,0xFF,0xC4,0x73,0x65,0x19,0x32,0xE8,0x77,0x50
	//       /*test1024*/ 0xFC,0x48,0x82,0x1A,0xE7,0xAB,0x80,0x72,0x0D,0xEC,0xFF,0xAF,0x95,0xBA,0x00,0xBD,0xE4,0x87,0x84,0x65,0xDA,0x8E,0x46,0xD3,0xB0,0x32,0x6D,0x98,0xCD,0x66,0x0A,0x18
		/*test2048*/ 0x8A,0x69,0x77,0xA7,0x5D,0x3A,0x1F,0x1F,0x13,0x9E,0x69,0xAE,0x92,0xC0,0xE6,0x44,0x7D,0x1E,0xF4,0xCA,0x55,0x66,0xAF,0xD9,0x5E,0x8B,0xED,0x4A,0x10,0x1D,0xF1,0x66
  //       /*test4096*/ 0x09,0x45,0x7A,0x6F,0x7C,0xC5,0x0D,0xB6,0x3D,0x8B,0xC8,0xEA,0xD3,0x2B,0x4F,0x69,0xC3,0xDF,0x65,0x30,0x02,0xCC,0xA7,0x9A,0xF4,0x94,0x6D,0x3B,0xFB,0x6D,0x26,0xC4

	}; // 对公钥做sah256后通过aes_key加密得到的结果
	uint32_t *pubkey_hash_SHA256_aes128ecb_AESKEY = (uint32_t *)pubkey_hash_SHA256_aes128ecb_AESKEY_byte;
	//PUBLIC KEY HASH (SHA256): 0xe7,0xcf,0x92,0xfb,0x4a,0xfe,0xc2,0xf2,0xc7,0x83,0x24,0x51,0x9c,0xa3,0xab,0xe,0xe4,0xf8,0xe5,0xdd,0x73,0x94,0x9,0xa6,0xb8,0xd1,0xc7,0xba,0xb,0x1,0x3a,0x70,
#else
	uint32_t pubkey_hash_SHA256_aes128ecb_AESKEY[8] = { 0 }; // 对公钥做sah256后通过aes_key加密得到的结果
#endif
	uint32_t aeskey_aes128ecb_RTLKEY[4] = {
#if  0//V13/*AES_KEY_OLD_RTL_KEY_ENCRYPTO*/
		   0,0,0,0,/*AES_KEY_OLD_RTL_KEY_ENCRYPTO OLD_RTL_KEY=0x3c4fcf098815f7aba6d2ae2816157e2b={0x16157e2b,0xa6d2ae28,0x8815f7ab,0x3c4fcf09}*/
#else//V14/*AES_KEY_NEW_RTL_KEY_ENCRYPTO*/
	   0x3522e3c5,0x59f2e402,0x93b180a0,0x8474cbe5,/*NEW RTL_KEY no know as software*/
#endif
	};// AES_KEY=0xd50abd5db7d82c249239ca4c5137b6ad={ 0x5137b6ad,0x9239ca4c,0xb7d82c24,0xd50abd5d }; 时，通过RTLKEY加密为0；

#if 0
   //方案1通过数值写入直接赋值
	if(data == NULL)
	{
		EFUSE_WD0 = 0x0;
		EFUSE_WD1 = 0x0;
	}
	else
	{//采用赋值的方式
		EFUSE_WD0 = data[0];
		EFUSE_WD1 = data[1];
	}
	EFUSE_CRC32_CHIP_CONFIG = 0x0;//CRC32_Cal_Buffer((DWORDP)&efuse_data.efuse_data[0], 8, 0, 0, 0);
	for(uint32_t i = 0; i < 8; i++)
	{
		EFUSE_PUBKEY_HASH(i) = pubkey_hash_SHA256_aes128ecb_AESKEY[i];
	}
	for(uint32_t i = 0; i < 4; i++)
	{
		EFUSE_AES_KEY(i) = aeskey_aes128ecb_RTLKEY[i];
	}
	EFUSE_CRC32_PUBKEYHASH_AESKEY = 0x0;//CRC32_Cal_Buffer((DWORDP)&efuse_data.efuse_data[3], 48, 0, 0, 0);

#elif 0
	   // 方案2通过结构体直接赋值
	sefuse_info *efuse_data = (void *)EFUSE_BASE_ADDR; //(void *)&EFUSE_WD0;
	if(data == NULL)
	{
		// 写efuse数据
		efuse_data->trim_ldo1 = 0x0;
		efuse_data->trim_bg = 0x0;
		efuse_data->rom_patch_disable = 0x0;
		efuse_data->wakeup_bypass_security_disable = 0x0;
		efuse_data->program_disable = 0x0;
		efuse_data->crypto_normal = 0x0;
		efuse_data->trim_32k = 0x0;
		efuse_data->crypto_clkgate_disable = 0x0;
		efuse_data->trim_96m = 0x0;
		efuse_data->reserved0 = 0x0;
		efuse_data->crypto_rompatch_disable = 0x0;
		efuse_data->eflash_512k = 0x0;
		efuse_data->ec_debug = 0x0;
		efuse_data->crypto_debug = 0x0;
		efuse_data->hash_size = 0x0;
		efuse_data->crypto_en = 0x0;
		efuse_data->crypto_jtag_disable = 0x0;
		efuse_data->jtag_disable = 0x0;
		efuse_data->security = 0x0;
		efuse_data->speedup = 0x0;
		efuse_data->reserved1 = 0x0;
	}
	else
	{
		sefuse_info *data_ptr = (void *)data;
		// 写efuse数据
		efuse_data->trim_ldo1 = data_ptr->trim_ldo1;
		efuse_data->trim_bg = data_ptr->trim_bg;
		efuse_data->rom_patch_disable = data_ptr->rom_patch_disable;
		efuse_data->wakeup_bypass_security_disable = data_ptr->wakeup_bypass_security_disable;
		efuse_data->program_disable = data_ptr->program_disable;
		efuse_data->crypto_normal = data_ptr->crypto_normal;
		efuse_data->trim_32k = data_ptr->trim_32k;
		efuse_data->crypto_clkgate_disable = data_ptr->crypto_clkgate_disable;
		efuse_data->trim_96m = data_ptr->trim_96m;
		efuse_data->reserved0 = data_ptr->reserved0;
		efuse_data->crypto_rompatch_disable = data_ptr->crypto_rompatch_disable;
		efuse_data->eflash_512k = data_ptr->eflash_512k;
		efuse_data->ec_debug = data_ptr->ec_debug;
		efuse_data->crypto_debug = data_ptr->crypto_debug;
		efuse_data->hash_size = data_ptr->hash_size;
		efuse_data->crypto_en = data_ptr->crypto_en;
		efuse_data->crypto_jtag_disable = data_ptr->crypto_jtag_disable;
		efuse_data->jtag_disable = data_ptr->jtag_disable;
		efuse_data->security = data_ptr->security;
		efuse_data->speedup = data_ptr->speedup;
		efuse_data->reserved1 = data_ptr->reserved1;
	}

	efuse_data->crc32_chip_config = 0x0; // CRC32_Cal_Buffer((DWORDP)&efuse_data.efuse_data[0], 8, 0, 0, 0);
	for(uint32_t i = 0; i < 8; i++)
	{
		efuse_data->pubkey_hash[i] = pubkey_hash_SHA256_aes128ecb_AESKEY[i];
	}
	for(uint32_t i = 0; i < 4; i++)
	{
		efuse_data->aes_key[i] = aeskey_aes128ecb_RTLKEY[i];
	}
	efuse_data->crc32_aes_key_hash = 0x0; // CRC32_Cal_Buffer((DWORDP)&efuse_data.efuse_data[3], 48, 0, 0, 0);
#else
	   // 方案3通过共用体间接赋值赋值
	uefuse_wd efuse_data;
	if(data == NULL)
	{
		efuse_data.efuse_info.trim_ldo1 = 0x0;
		efuse_data.efuse_info.trim_bg = 0x0;
		efuse_data.efuse_info.rom_patch_disable = 0x0;
		efuse_data.efuse_info.wakeup_bypass_security_disable = 0x0;
		efuse_data.efuse_info.program_disable = 0x0;
		efuse_data.efuse_info.crypto_normal = 0x0;
		efuse_data.efuse_info.trim_32k = 0x0;
		efuse_data.efuse_info.crypto_clkgate_disable = 0x0;
		efuse_data.efuse_info.trim_96m = 0x0;
		efuse_data.efuse_info.reserved0 = 0x0;
		efuse_data.efuse_info.crypto_rompatch_disable = 0x0;
		efuse_data.efuse_info.eflash_512k = 0x0;
		efuse_data.efuse_info.ec_debug = 0x1;
		efuse_data.efuse_info.crypto_debug = 0x1;
		efuse_data.efuse_info.hash_size = 0;
		efuse_data.efuse_info.crypto_en = 0x1;
		efuse_data.efuse_info.crypto_jtag_disable = 0x0;
		efuse_data.efuse_info.jtag_disable = 0x0;
		efuse_data.efuse_info.security = 0x1;
		efuse_data.efuse_info.speedup = 0x0;
		efuse_data.efuse_info.reserved1 = 0x0;
	}
	else
	{//采用赋值的方式
		efuse_data.efuse_data[0] = data[0];
		efuse_data.efuse_data[1] = data[1];
	}

	efuse_data.efuse_info.crc32_chip_config = crc32_d((uint8_t *)&efuse_data.efuse_data[0], 8); // CRC32_Cal_Buffer((DWORDP)&efuse_data.efuse_data[0], 8, 0, 0, 0); // 0x0;
	for(uint32_t i = 0; i < 8; i++)
	{
		efuse_data.efuse_info.pubkey_hash[i] = pubkey_hash_SHA256_aes128ecb_AESKEY[i];
	}
	for(uint32_t i = 0; i < 4; i++)
	{
		efuse_data.efuse_info.aes_key[i] = aeskey_aes128ecb_RTLKEY[i];
	}
	efuse_data.efuse_info.crc32_aes_key_hash = crc32_d((uint8_t *)&efuse_data.efuse_data[3], 48); // CRC32_Cal_Buffer((DWORDP)&efuse_data.efuse_data[3], 48, 0, 0, 0); // 0x0;
	for(uint32_t i = 0; i < 16; i++)
	{
		EFUSE_WD(i) = efuse_data.efuse_data[i];
	}
#endif
		   // 更新配置
	   // 配置新的EFUSE
	EFUSE_FPGA_CFG |= 1;
#if 0//主系统无法重读，因此需要复位解决
	// 重新读取EFUSE数据
	EFUSE_CFG |= 1;
	// 等待EFUSE读取完毕
	while(!(EFUSE_CFG & 1))
	{
	}
#endif
	// 输出EFUSE数据
	printf("EFUSE_TRIM_LDO1: %#x\n", efuse_data.efuse_info.trim_ldo1);
	printf("EFUSE_TRIM_BG: %#x\n", efuse_data.efuse_info.trim_bg);
	printf("EFUSE_ROMPATCH_DISABLE: %#x\n", efuse_data.efuse_info.rom_patch_disable);
	printf("EFUSE_WAKEUP_BYPASS_SECUITY_DISABLE: %#x\n", efuse_data.efuse_info.wakeup_bypass_security_disable);
	printf("EFUSE_PROGRAM_DISABLE: %#x\n", efuse_data.efuse_info.program_disable);
	printf("EFUSE_CRYPTO_NORMAL: %#x\n", efuse_data.efuse_info.crypto_normal);
	printf("EFUSE_TRIM_32K: %#x\n", efuse_data.efuse_info.trim_32k);
	printf("EFUSE_CRYPTO_CLKGATE_DISABLE: %#x\n", efuse_data.efuse_info.crypto_clkgate_disable);
	printf("EFUSE_TRIM_96M: %#x\n", efuse_data.efuse_info.trim_96m);
	printf("EFUSE_RESERVED0: %#x\n", efuse_data.efuse_info.reserved0);
	printf("EFUSE_CRYPTO_ROMPATCH_DISABLE: %#x\n", efuse_data.efuse_info.crypto_rompatch_disable);
	printf("EFUSE_EFLASH_512K: %#x\n", efuse_data.efuse_info.eflash_512k);
	printf("EFUSE_EC_DEBUG: %#x\n", efuse_data.efuse_info.ec_debug);
	printf("EFUSE_CRYPTO_DEBUG: %#x\n", efuse_data.efuse_info.crypto_debug);
	printf("EFUSE_HASH_SIZE: %#x\n", efuse_data.efuse_info.hash_size);
	printf("EFUSE_CRYPTO_EN: %#x\n", efuse_data.efuse_info.crypto_en);
	printf("EFUSE_CRYPTO_JTAG_DISABLE: %#x\n", efuse_data.efuse_info.crypto_jtag_disable);
	printf("EFUSE_JTAG_DISABLE: %#x\n", efuse_data.efuse_info.jtag_disable);
	printf("EFUSE_SECURITY: %#x\n", efuse_data.efuse_info.security);
	printf("EFUSE_SPEEDUP: %#x\n", efuse_data.efuse_info.speedup);
	printf("EFUSE_RESERVED1: %#x\n", efuse_data.efuse_info.reserved1);
	printf("EFUSE_CRC32_CHIP_CONFIG: %#x\n", efuse_data.efuse_info.crc32_chip_config);
	printf("EFUSE_PUBKEY_HASH: %#x\n", efuse_data.efuse_info.pubkey_hash[0]);
	printf("EFUSE_PUBKEY_HASH: %#x\n", efuse_data.efuse_info.pubkey_hash[1]);
	printf("EFUSE_PUBKEY_HASH: %#x\n", efuse_data.efuse_info.pubkey_hash[2]);
	printf("EFUSE_PUBKEY_HASH: %#x\n", efuse_data.efuse_info.pubkey_hash[3]);
	printf("EFUSE_PUBKEY_HASH: %#x\n", efuse_data.efuse_info.pubkey_hash[4]);
	printf("EFUSE_PUBKEY_HASH: %#x\n", efuse_data.efuse_info.pubkey_hash[5]);
	printf("EFUSE_PUBKEY_HASH: %#x\n", efuse_data.efuse_info.pubkey_hash[6]);
	printf("EFUSE_PUBKEY_HASH: %#x\n", efuse_data.efuse_info.pubkey_hash[7]);
	printf("EFUSE_AES_KEY: %#x\n", efuse_data.efuse_info.aes_key[0]);
	printf("EFUSE_AES_KEY: %#x\n", efuse_data.efuse_info.aes_key[1]);
	printf("EFUSE_AES_KEY: %#x\n", efuse_data.efuse_info.aes_key[2]);
	printf("EFUSE_AES_KEY: %#x\n", efuse_data.efuse_info.aes_key[3]);
	printf("EFUSE_CRC32_PUBKEYHASH_AESKEY: %#x\n", efuse_data.efuse_info.crc32_aes_key_hash);
}
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
	if(SYSCTL_PIO_CFG & BIT1)
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