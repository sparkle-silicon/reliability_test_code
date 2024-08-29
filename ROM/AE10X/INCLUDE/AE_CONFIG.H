/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-16 15:30:20
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
#ifndef AE_CONFIG_H
#define AE_CONFIG_H
/****************************************************************************
 * Include Headfile
 *****************************************************************************/
#include "AE_GLOBAL.H"
#include "AE_REG.H"

// GLE01主系统相关控制宏
#define ROM_DEBUG 1
// 子系统从外部FLASH mirror代码到内部FLASH支持宏
#define Crypto_Flash_Mirror_Support 0

typedef enum FLASH_PIN
{
    NOPIN_FLASH = -1,
    PIN8528_FLASH = 0,
    PIN5571_FLASH
} SPIF_PIN;
typedef enum FLASH_SELECT
{
    NOSELECT_FLASH = -1,
    INTERNAL_FLASH = 0,
    EXTERNAL_FLASH
} SPIF_INT_EXT;
typedef enum FLASH_MODE
{
    NOCONFIGMODE = -1,
    DUAL_FLASH = 0,
    QUAD_FLASH
} SPIF_MODE;

// 默认时钟配置
#define OSC_CLOCK_SWITCH 0                                         // 1（外部） 0（内部）
#define CHIP_CLOCK_INT_LOW 32000                                   //
#define CHIP_CLOCK_INT_HIGH 96000000                               // 1(80) 2(40) 3(26.6M) 4（20）5（38.4）分频数（时钟）//192000000  // 1(192) 2(96) 3(64) 4（48）5（38.4）分频数（时钟）
#define CHIP_CLOCK_EXT 32768                                       // 32000         // 1(96) 2(48) 3(32) 4（24）5（19.2）分频数（时钟）
#define CHIP_CLOCK_SWITCH 4                                        // 1(192) 2(96) 3(64) 4（48）8（24）分频数（时钟）//2457600
#define LOW_CHIP_CLOCK CHIP_CLOCK_INT_LOW                          // 32768
#define HIGHT_CHIP_CLOCK (CHIP_CLOCK_INT_HIGH / CHIP_CLOCK_SWITCH) // 192M/CHIP_CLOCK_SWITCH
#define TIMER_FREQ LOW_CHIP_CLOCK * 2

#define rom_bootmode ((*(VDWORDP)(SYSCTL_BASE_ADDR + 0x1c)) & BIT4)

#define SYSCTL_ID (((*(VDWORDP)(SYSCTL_BASE_ADDR + 0x4)) >> 24) & 0xff)
#define flash_cache_addr FLASH_BASE_ADDR
#define flash_info_addr 0x100UL
#define magic_number_addr ((sDynamicFlashInfo *)(&_MAGIC_NUMBER))
#define ROM_RAM_BASE_ADDR (DRAM_BASE_ADDR + 0x800)

// 运行参数区//0x0

#define ROM_CNT (*(volatile u_int32_t *)(ROM_RAM_BASE_ADDR + 0x0))
#define ROM_CNT_ROMREADY 1U
#define ROM_CNT_READ_INFO 2U
#define ROM_CNT_MOD_FUNC_EN 3U
#define ROM_CNT_SECURITY_VERIFY 4U
#define ROM_CNT_MIRROR 5U
#define ROM_CNT_BOOT 6U
#define ROM_CNT_SECURITY_FAILED 0xFEU
#define ROM_CNT_READ_INFO_ERROR 0xFFU

#define ROM_ERROR (*(volatile uint32_t *)(ROM_RAM_BASE_ADDR + 0x4))

#define SPIF_COMMAND_READDATA 0x03
#define SPIF_COMMAND_FASTREAD 0x0B

#define SPIF_COMMAND_FASTREAD_DUAL_OUTPUT 0x3B
#define SPIF_COMMAND_FASTREAD_QUAD_OUTPUT 0x6B
#define SPIF_COMMAND_FASTREAD_DUAL_IO 0xBB
#define SPIF_COMMAND_FASTREAD_QUAD_IO 0xEB
#define SPIF_COMMAND_WORDREAD_QUAD_IO 0xE7
#define SPIF_COMMAND_OCTAL_WORDREAD_QUAD_IO 0xE3
#define SPIF_COMMAND_SET_BURST_WRAP 0x77
#define SPIF_COMMAND_READ_STATUS_REG_2 0x35

#define SPIF_COMMAND_PAGE_PROGRAM 0x02
// #define SPIF_COMMAND_DUAD_PAGE_PROGRAM 0xA2//P25Q21H
#define SPIF_COMMAND_QUAD_PAGE_PROGRAM 0x32

#define SPIF_COMMAND_4KSECTIOR_ERASE 0x20
#define SPIF_COMMAND_32KBLOCK_ERASE 0x52
#define SPIF_COMMAND_64KBLOCK_ERASE 0xd8
#define SPIF_COMMAND_CHIP_ERASE1 0xc7
#define SPIF_COMMAND_CHIP_ERASE2 0x60

#define SPIF_COMMAND_MID_DID 0x90
#define SPIF_COMMAND_DUALIO_MID_DID 0x92
#define SPIF_COMMAND_QUADIO_MID_DID 0x94

#define ROM_Firmware_Reader 0x0
#define ROM_NOFLASH 0x1
#define ROM_NOFirmware 0x2
#define ROM_NODynamic 0x3
#define READ_CSR_MCYCLE_VAR (uint64_t)(((uint64_t)read_csr(0xb00)) | (((uint64_t)read_csr(0xb80)) << 32))

extern void _start();

extern volatile uint32_t ROM_TIMEOUT;
extern volatile uint64_t READ_FLASH_INFO_TIME;
extern volatile uint64_t ROM_CSR_MCYCLE;
extern volatile uint64_t ROM_CSR_MCYCLE_OVERHEAD;

extern volatile uint32_t FLASH_CTRL;
extern volatile uint32_t FLASH_ID;
extern volatile uint32_t internal_flash_state; // 外部状态
extern volatile uint32_t external_flash_state; // 内部状态
extern volatile uint32_t external_flash_used;  // 使用外部flash

extern volatile sFixedFlashInfo INT_FLASH_FIX_INFO;
extern volatile sFixedFlashInfo EXT_FLASH_FIX_INFO;
extern volatile sDynamicFlashInfo INT_FLASH_DYNAMIC_INFO;
extern volatile sDynamicFlashInfo EXT_FLASH_DYNAMIC_INFO;
extern volatile sFixedFlashInfo *FLASH_FIX_INFO_PTR;
extern volatile sDynamicFlashInfo *FLASH_DYNAMIC_INFO_PTR;

extern const char FIX_FIRMWARE_ID[];
extern const char DYN_FIRMWARE_ID[];
#endif /* AE_CONFIG_H */
