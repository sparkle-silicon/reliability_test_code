/*
 * @Author: dejavuwdh
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2025-06-16 15:18:33
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
// flash mirror and flash string
extern sDynamicFlashInfo Dy_flash_info;
SECTION(".FlashInfo.Fixed")
USED sFixedFlashInfo Fix_flash_info = {
	.Firmware_ID = "SPK32GLE01\0",//識別碼，不允許改動


	.Compiler_Version = __DATE__ " "__TIME__
								 "\0",
	// 外部FLASH控制
	.EXTERNAL_FLASH_CTRL.ENABLE_USED = 0b1,			// 允许使用外部FLASH（内部为准）
	.EXTERNAL_FLASH_CTRL.LAST8M_DISABLE = 0b1,		// 关闭后从后8M开始检索(内部为准)
	.EXTERNAL_FLASH_CTRL.LowAddr_OFFSET = 0b11,		// 4k（内部为准）
	.EXTERNAL_FLASH_CTRL.HighAddr_SPACE = 0b111,	//[23:x]（内部为准）
	.EXTERNAL_FLASH_CTRL.MUST_MIRROR_DISABLE = 0b1, // ，一般关闭，需要强制更新时候开启（外部为准）
	.EXTERNAL_FLASH_CTRL.PWM_Enable = 0b0,//MIRROR期间 0：关闭pwm闪烁，1：开启pwm闪烁
	.EXTERNAL_FLASH_CTRL.PWMn_Switch = 0b0,
	.EXTERNAL_FLASH_CTRL.SPI_Switch = 0b1,	// 4线
	.EXTERNAL_FLASH_CTRL.WP_Switch = 0b11,	// piob29
	.EXTERNAL_FLASH_CTRL.CS_Switch = 0b0, // fspi_csn0
	.EXTERNAL_FLASH_CTRL.Firmware_4KSector = (Firmware_nKsize / 4 - 1),

	// FLASH	地址
	.IVT = ((uint32_t)&vector_base - (FLASH_CACHE_BASE_ADDR)), // 中断向量表
	.Restart = ((uint32_t)&_start - (FLASH_CACHE_BASE_ADDR)),  // 起始地址
	// .DynamicFlashInfo = ((uint32_t)&Dy_flash_info - (FLASH_CACHE_BASE_ADDR)), // 由编译填入
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
