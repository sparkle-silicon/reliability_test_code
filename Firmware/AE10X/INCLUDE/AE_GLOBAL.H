/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-06-04 16:47:08
 * @Description:
 *               define chip_type
 *               define identifier
 *               define  hexxx
 *               define true/false
 *               define base addr
 *               define  use macros of all files
 *
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 以下是中文及英文版权同步声明，编码为UTF-8。
 * Copyright has legal effects and violations will be prosecuted.
 * 版权具有法律效力，违反必究。
 *
 * Copyright ©2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2023龙晶石半导体科技（苏州）有限公司
 */
#ifndef AE_GLOBAL_H
#define AE_GLOBAL_H
/*-------------------------------------STD HEAD INCLUDE-------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/asm.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <limits.h>
// #include <signal.h>
// #include <errno.h>
// #include <assert.h>
// #include <errno.h>
// #include <locale.h>
// #include <setjmp.h>
// #include <math.h>
#include <RISCV.H>

#ifndef CODE_VERSION
#define CODE_VERSION "Unknown version"
#endif
/*-------------------------------------AE10X SELECITON-------------------------------------*/
#if !(defined(USER_AE10X_LIBC_A) || defined(USER_RISCV_LIBC_A))
#define USER_AE10X_LIBC_A
#endif
/*-------------------------------------SET DEF OF VARIABLE TYPE IDENTIFIER-------------------------------------*/
// set stdtypedef
#ifndef _UINT8_T_DECLARED
typedef unsigned char uint8_t;
#define _UINT8_T_DECLARED
#endif
#ifndef _UINT16_T_DECLARED
typedef unsigned short uint16_t;
#define _UINT16_T_DECLARED
#endif
#ifndef _UINT32_T_DECLARED
typedef unsigned int uint32_t;
#define _UINT32_T_DECLARED
#endif
#ifndef _UINTPTR_T_DECLARED
typedef uint32_t *uintptr_t;
#define _UINTPTR_T_DECLARED
#endif
// Named after the data structure
// typedef unsigned char bool;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
// Name it by the size of the data
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
// (internal)volatile sram (if  new char have external ram please recompose)
typedef volatile unsigned char VBYTE;
typedef volatile unsigned short VWORD;
typedef volatile unsigned int VDWORD;
// pointer
#define BYTEP BYTE *
#define WORDP WORD *
#define DWORDP DWORD *
#define VBYTEP VBYTE *
#define VWORDP VWORD *
#define VDWORDP VDWORD *
// type bit stuct
typedef struct _Bit
{
    BYTE bit0 : 1;
    BYTE bit1 : 1;
    BYTE bit2 : 1;
    BYTE bit3 : 1;
    BYTE bit4 : 1;
    BYTE bit5 : 1;
    BYTE bit6 : 1;
    BYTE bit7 : 1;
} sBit;
typedef struct _Mode
{
    BYTE SYSCTL : 1;
    BYTE PNP : 1;
    BYTE SPIF : 1;
    BYTE PMC : 1;
    BYTE KBC : 1;
    BYTE SM : 1;
    BYTE SWUC : 1;
    BYTE BRAM : 1;
    BYTE INTC : 1;
    BYTE TIMER : 4;
    BYTE KBS : 1;
    BYTE PS2 : 2;
    BYTE GPIO : 5;
    BYTE I2C : 6;
    BYTE WDT : 1;
    BYTE PPORT : 1;
    BYTE ADC : 1;
    BYTE PWM : 8;
    BYTE UART : 6;
    BYTE SPIM : 1;
    BYTE P80 : 1;
} sMode;
typedef union _Byte
{
    BYTE byte;
    sBit bit;
} uByte;
typedef union _Word
{
    WORD word;
    BYTE byte[2];
    sBit bit[2];
} uWord;
typedef union _Dword
{
    DWORD dword;
    WORD word[2];
    BYTE byte[4];
    sBit bit[4];
} uDword;
#define UNUSED_VAR(x) ((void)x)
// set weak
#define WEAK __attribute__((weak))
#define __weak WEAK
// set interrupt
#define INTERRUPT __attribute__((interrupt))
#define Interrupt INTERRUPT
#define __interrupt INTERRUPT
// set function Byte alignment
#define ALIGNED(x) __attribute__((aligned(x)))
// set  code no used with no optimize of gcc
#define USED __attribute__((used))
// set no used no warning  of gcc
#define UNUSED __attribute__((unused))
// no return
#define NORETURN __attribute__((noreturn))
// no inline
#define NOINLINE __attribute__((noinline))
// set section
#define SECTION(x) __attribute__((section(x)))
// set optimize of gcc
#define OPTIMIZE(x) __attribute__((optimize(x)))
#define OPTIMIZE_DEFAULT __attribute__((optimize("O")))
#define OPTIMIZE0 __attribute__((optimize("O0")))
#define OPTIMIZE1 __attribute__((optimize("O1")))
#define OPTIMIZE2 __attribute__((optimize("O2")))
#define OPTIMIZE3 __attribute__((optimize("O3")))
#define OPTIMIZEA __attribute__((optimize("Os")))
#define OPTIMIZEF __attribute__((optimize("Ofast")))
#define OPTIMIZEG __attribute__((optimize("Og")))
// set warning of function
#define WARNING __attribute__((warning))
// set not IO
#ifndef __pure2
#define __pure2 __attribute__((__const__))
#endif
#ifndef __packed
#define __packed __attribute__((__packed__))
#endif
// #define _at_(addr) __attribute__((at(addr)))
#ifndef __nop
#define __nop                \
    {                        \
        asm volatile("nop"); \
    }
#endif
#ifndef __nop__
#define __nop__ __nop
#endif
#define nop __nop
/*-------------------------------------set hex used 0-3bit -------------------------------------*/
#define HEX00 0x00
#define HEX01 0x01
#define HEX02 0x02
#define HEX03 0x03
#define HEX04 0x04
#define HEX05 0x05
#define HEX06 0x06
#define HEX07 0x07
#define HEX08 0x08
#define HEX09 0x09
#define HEX0A 0x0A
#define HEX0B 0x0B
#define HEX0C 0x0C
#define HEX0D 0x0D
#define HEX0E 0x0E
#define HEX0F 0x0F
/*-------------------------------------set hex Commonly used 4-7bit-------------------------------------*/
#define HEX10 0x10
#define HEX11 0x11
#define HEX12 0x12
#define HEX13 0x13
#define HEX14 0x14
#define HEX15 0x15
#define HEX16 0x16
#define HEX17 0x17
#define HEX18 0x18
#define HEX19 0x19
#define HEX1A 0x1A
#define HEX1B 0x1B
#define HEX1C 0x1C
#define HEX1D 0x1D
#define HEX1E 0x1E
#define HEX1F 0x1F
#define HEX20 0x20
#define HEX21 0x21
#define HEX22 0x22
#define HEX23 0x23
#define HEX24 0x24
#define HEX25 0x25
#define HEX26 0x26
#define HEX27 0x27
#define HEX28 0x28
#define HEX29 0x29
#define HEX2A 0x2A
#define HEX2B 0x2B
#define HEX2C 0x2C
#define HEX2D 0x2D
#define HEX2E 0x2E
#define HEX2F 0x2F
#define HEX40 0x40
#define HEX80 0x80
/*-------------------------------------set dec Commonly used-------------------------------------*/
#define DEC8 HEX08
#define DEC16 HEX10
#define DEC32 HEX20
#define DEC64 HEX40
#define DEC128 HEX80
#define DEC256 256
#define DEC512 512
#define DEC1024 1024
/*-------------------------------------set byte bit-------------------------------------*/
#define BIT0 HEX01
#define BIT1 HEX02
#define BIT2 HEX04
#define BIT3 HEX08
#define BIT4 HEX10
#define BIT5 HEX20
#define BIT6 HEX40
#define BIT7 HEX80
#define BIT(X) (1 << (X))
/*-------------------------------------set bit mask flga config-------------------------------------*/
#define SET_BIT(reg, bit) ((reg) |= (0x1 << (bit)))
#define CLEAR_BIT(reg, bit) ((reg) &= (~(0x1 << (bit))))
#define IS_SET(reg, bit) (((reg) & (0x1 << (bit))) != 0)
#define IS_CLEAR(reg, bit) (((reg) & (0x1 << (bit))) == 0)
#define SET_MASK(reg, bit_mask) ((reg) |= (bit_mask))
#define CLEAR_MASK(reg, bit_mask) ((reg) &= (~(bit_mask)))
#define IS_MASK_SET(reg, bit_mask) (((reg) & (bit_mask)) != 0)
#define IS_MASK_CLEAR(reg, bit_mask) (((reg) & (bit_mask)) == 0)
#define INVERSE_REG(reg, bit) ((reg) ^= (0x1 << (bit)))
#define INVERSE_REG_MASK(reg, bit_mask) ((reg) ^= (bit_mask))
#define Cpl_Byte_Mask(x) (~(1 << (x)))
#define Cpl_Word_Mask(x) (~(1 << (x)))
#define SET_FLAG(reg, bit) ((reg) |= (bit))
#define CLEAR_FLAG(reg, bit) ((reg) &= (~(bit)))
#define Is_FLAG_SET(reg, bit) (((reg) & (bit)) != 0)
#define Is_FLAG_CLEAR(reg, bit) (((reg) & (bit)) == 0)
/*-------------------------------------set bool of true and false-------------------------------------*/
#define FALSE 0
#define TRUE 1
/*-------------------------------------set level val-------------------------------------*/
#define LOW 0
#define HIGH 1
//-----------------------------------------------------------------------------
//  Function Pointers
//-----------------------------------------------------------------------------
typedef void (*FUNCT_PTR_V_V)(void);
typedef void (*FUNCT_PTR_V_W)(WORD);
typedef WORD(*FUNCT_PTR_W_V)(void);
typedef WORD(*FUNCT_PTR_W_W)(WORD);
typedef WORD(*FUNCT_PTR_W_B)(BYTE);
typedef BYTE(*FUNCT_PTR_B_B)(BYTE);
typedef BYTE(*FUNCT_PTR_B_V)(void);
typedef BYTE(*FUNCT_PTR_S_V)(void);
typedef void (*FUNCT_PTR_V_B)(BYTE);
typedef void (*FUNCT_PTR_V_B_B)(BYTE, BYTE);
typedef void (*FUNCT_PTR_V_D_BP)(DWORD, BYTEP);
typedef void (*FUNCT_PTR_B_D_D)(BYTE, DWORD, DWORD);
typedef void (*FUNCT_PTR_V_D_BP_L)(DWORD, BYTE *, WORD);
// kbs typedef
typedef struct _A2_TABLE
{
    BYTE comb;        // Combination Key Setting
    const BYTE *pntr; // Pointer to array for key
} A2_TABLE;
typedef struct BootHotKeyStruct
{
    BYTE matrix;
    FUNCT_PTR_V_B pfunction;
} sBootHotKeyStruct;
extern const FUNCT_PTR_V_B HotKey_Fn_Fx[];
extern const BYTE sskey3_80_table[];
extern const BYTE sskey2_overlay_table[];
extern const A2_TABLE sskey2_A2_table[];
/*--------------------KBS typedef-----------------*/
#pragma pack(1) // 1字节对齐
typedef struct _scan
{
    /*buffer缓存相关的指针*/
    BYTE head : 4; /* Keyboard buffer head (0-15). 扫描缓存的头索引*/
    BYTE tail : 4; /* Keyboard buffer tail (0-15). 扫描缓存的尾索引*/
    BYTE mark : 4; /* Keyboard buffer overflow marker (0-15). */
    /*重复多次扫描需要*/
    BYTE scale : 4; /* Typematic scale. */
    /*扫描的某一行0~15*/
    BYTE line : 4;
    /*扫描的flag*/
    BYTE make : 1; /* Scan activity flag - make. make. 产生make事件 */
    BYTE back : 1; /* Scan activity flag - break. break.产生break事件*/
    BYTE keys : 1; /* Scan activity flag - keys active.keys active. 按键活跃状态 */
} sscan;
typedef union _KEY
{
    BYTE byte;
    struct
    {
        BYTE ksi : 3; /* Bit 0 - 2. Bit address of the KI bit. 列号,0~8*/
        BYTE kso : 5; /* Bit 3 - 7. Bit address of the KO bit.行号 32位*/
    } index;
} uKEY;
typedef struct _sign
{
    DWORD r[8];
    DWORD s[8];
} ssign;
typedef struct _sign_string
{
    char head[2];
    char space1;
    char r[64];
    char space2;
    char s[64];
} ssign_string;
typedef struct _point
{
    DWORD x[8];
    DWORD y[8];
} spoint;

#pragma pack(4) // 4字节对齐
extern int main();
extern sMode Mode;
// extern u_int32_t CHIP_CLOCK_SWITCH;

typedef struct _CryptoFlashInfo
{
    uint8_t Firmware_ID[16]; // 识别码，确认安全验签位置，和开头识别码做区分
    uint32_t Crc32Data;
    uint8_t HASH_AES_ECB_RTL_KEY[64];
    uint32_t Crypto_CopyStartAddr;
    uint32_t Crypto_CopySize;
} sCryptoFlashInfo;

/*-------------------------------------set reg Information-------------------------------------*/

#define SYSCTL_ESTAT_BOOTMODE_SYNC                              ((SYSCTL_ESTAT&0x00000003)>>0)//[1:0]
#define SYSCTL_ESTAT_BUSERR_STATUS                              ((SYSCTL_ESTAT&0x00000004)>>2)//[2]
#define SYSCTL_ESTAT_BOOTWDTRST                                 ((SYSCTL_ESTAT&0x00000008)>>3)//[3]
#define SYSCTL_ESTAT_EFUSE_CRYPTO_NORMAL                        ((SYSCTL_ESTAT&0x00000010)>>4)//[4]
#define SYSCTL_ESTAT_EFUSE_SECURITY                             ((SYSCTL_ESTAT&0x00000020)>>5)//[5]
#define SYSCTL_ESTAT_BOOTPWRSWRST                               ((SYSCTL_ESTAT&0x00000040)>>6)//[6]
#define SYSCTL_ESTAT_CRYPTO_BOOTCTRL                            ((SYSCTL_ESTAT&0x00000080)>>7)//[7]

#define SYSCTL_ESTAT_LRS_AON                                    ((SYSCTL_ESTAT&0x00001F00)>>8)//[12:8]
#define SYSCTL_ESTAT_PMUC_STATE                                 ((SYSCTL_ESTAT&0x00006000)>>13)//[14:13]
#define SYSCTL_ESTAT_EFUSE_CRYPTO_JTAG_DISABLE                  ((SYSCTL_ESTAT&0x00008000)>>15)//[15]

#define SYSCTL_ESTAT_EFUSE_JTAG_DISABLE                         ((SYSCTL_ESTAT&0x00010000)>>16)//[16]
#define SYSCTL_ESTAT_EFUSE_ROMPATCH_DISABLE                     ((SYSCTL_ESTAT&0x00020000)>>17)//[17]
#define SYSCTL_ESTAT_EFUSE_WAKEUP_BYPASS_SECURITY_DISABLE       ((SYSCTL_ESTAT&0x00040000)>>18)//[18]
#define SYSCTL_ESTAT_EFUSE_PROGRAM_DISABLE                      ((SYSCTL_ESTAT&0x00080000)>>19)//[19]
#define SYSCTL_ESTAT_EFUSE_CRYTO_EN                             ((SYSCTL_ESTAT&0x00100000)>>20)//[20]
#define SYSCTL_ESTAT_EFUSE_EC_DEBUG                             ((SYSCTL_ESTAT&0x00200000)>>21)//[21]
#define SYSCTL_ESTAT_EFUSE_CRYPTO_DEBUG                         ((SYSCTL_ESTAT&0x00400000)>>22)//[22]
#define SYSCTL_ESTAT_EFUSE_EFLASH_512K                          ((SYSCTL_ESTAT&0x00800000)>>23)//[23]

#define SYSCTL_ESTAT_EFUSE_CRYPTO_SPEEDUP                       ((SYSCTL_ESTAT&0x01000000)>>24)//[24]
#define SYSCTL_ESTAT_EFUSE_CRYPTO_CLKGATE_DISABLE               ((SYSCTL_ESTAT&0x02000000)>>25)//[25]
#define SYSCTL_ESTAT_RESERVED                                   ((SYSCTL_ESTAT&0xFC000000)>>26)//[31:26]

#include <AE_REG.H>
#include <AE_CONFIG.H>
#include <AE_LIBC.H>
#include <AE_MALLOC.H>
#include <AE_PRINTF.H>
#include <AE_FUNC.H>
#endif
