/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-19 16:00:36
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
typedef struct _Bit{
    BYTE bit0 : 1;
    BYTE bit1 : 1;
    BYTE bit2 : 1;
    BYTE bit3 : 1;
    BYTE bit4 : 1;
    BYTE bit5 : 1;
    BYTE bit6 : 1;
    BYTE bit7 : 1;
} sBit;
typedef struct _Mode{
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
#if  (defined(AE101) || defined(AE102))
    BYTE I2C : 4;
#endif
#if  (defined(AE103))
    BYTE I2C : 6;
#endif
    BYTE WDT : 1;
    BYTE PPORT : 1;
    BYTE ADC : 1;
    BYTE PWM : 8;
    BYTE UART : 6;
#if (defined(AE103)||defined(AE102))
    BYTE CAN : 4;
#endif
    BYTE SPIM : 1;
    BYTE P80 : 1;
}sMode;
typedef union _Byte{
    BYTE byte;
    sBit bit;
} uByte;
typedef union _Word{
    WORD word;
    BYTE byte[2];
    sBit bit[2];
} uWord;
typedef union _Dword{
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
#define WARNING    __attribute__((warning))
//set not IO 
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
// kbs typedef
typedef struct _A2_TABLE{
    BYTE comb;        // Combination Key Setting
    const BYTE *pntr; // Pointer to array for key
} A2_TABLE;
typedef struct BootHotKeyStruct{
    BYTE matrix;
    FUNCT_PTR_V_B pfunction;
} sBootHotKeyStruct;
extern const FUNCT_PTR_V_B HotKey_Fn_Fx[];
extern const BYTE sskey3_80_table[];
extern const BYTE sskey2_overlay_table[];
extern const A2_TABLE sskey2_A2_table[];
/*--------------------KBS typedef-----------------*/
#pragma pack(1) // 1字节对齐
typedef struct _scan{
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
typedef union _KEY{
    BYTE byte;
    struct{
        BYTE ksi : 3; /* Bit 0 - 2. Bit address of the KI bit. 列号,0~8*/
        BYTE kso : 5; /* Bit 3 - 7. Bit address of the KO bit.行号 32位*/
    } index;
} uKEY;
typedef struct _sign{
    DWORD r[8];
    DWORD s[8];
} ssign;
typedef struct _sign_string{
    char head[2];
    char space1;
    char r[64];
    char space2;
    char s[64];
} ssign_string;
typedef struct _point{
    DWORD x[8];
    DWORD y[8];
} spoint;
#pragma pack(1)

typedef struct _rom_data//关于FLASH的一些信息
{

    uint32_t EXTFlash_ID;//外部FLASH ID如果外部FLASH ID发生变动，需要除了版本号以外校验签名是否一致
    uint32_t FixedFlashInfo_Addr : 24;//外部flash中固件的相对位置位置(上次MIRROR位置,绝对地址)
    uint32_t Program_Method : 3; // 上次烧写方式//0mirror更新 1cmd更新,2debugeruart更新3DEBUGGERiic更新4lpc更新5espi更新6:external flash7:no know
    uint32_t MIRRO_Enable : 1; // 外部FLASH接口和镜像功能.默认是开着的，除非软件关闭(即外部FLASH设置或者DEBUGGER设置)
    uint32_t RESERVED : 4;//不生效

} srom_data;
typedef struct _patch_typdef{
    uDword data;
    BYTE addrl;
    BYTE addrh : 5;
    BYTE last : 1;//下一个还有
    BYTE Reserved : 2;
} spatch_typdef;
typedef struct _DynamicFlashInfo{
    uint8_t Firmware_ID[16]; // 识别码，确认安全验签位置，和开头识别码做区分
    ssign SM2_ssign1;     // SM2签名
    spatch_typdef PATCH[8];

    srom_data Info;//rom相关的可变动的信息（不进行配置，由rom填充后烧写）
} sDynamicFlashInfo;

#define EXTERNAL_FLASH_ENABLE(reg) (((reg)>>0)&1)
#define FLASH_LAST8M(reg) (((reg)>>1)&1)
#define FLASH_LowAddr_OFFSET(reg) (((reg)>>2)&3)
#define FLASH_HighAddr_SPACE(reg) (((reg)>>4)&7)
#define FLASH_MUST_MIRROR(reg) (((reg)>>7)&1)
#define FLASH_UPDATA_FUNCTION(reg) (((reg)>>8)&1)

#define FLASH_RAM_CTRL(reg) (((reg)>>15)&1)

#define SYSCTL_RAM_CTRL FLASH_RAM_CTRL(SYSCTL_RESERVER)//开启这位的时候下列配置生效，否则按内部FLASH配置进行使用否则
#define SPACE_OFFSET_VAR 1
#define Firmware_nKsize 512

#define MIRROR_ADDR 0x0
#pragma pack(1)
typedef struct _FixedFlashInfo{
    uint8_t Firmware_ID[11]; // 识别码
    uint8_t Compiler_Version[21]; // 版本号
    uint32_t Mirror_Addr;//外部flash中固件的绝对位置(MIRROR起始位置)
    uint32_t IVT;//中断向量表起始位置Interrupt_Vector_Table（相对Mirror_Addr的起始地址）
    uint32_t Restart;//代码复位运行起始位置Reset Start
    uint32_t DBoot_IVT;//双启动第二段代码的中断位置
    uint32_t DBoot_Restart;//双启动第二段代码的起始位置
    uint32_t DynamicFlashInfo;//尾部FLASH相关位置 
    union{
        uint32_t SYSCTL_RESERVERD;
        struct//查找相关以内部为准，MIRROR，spi控制相关以外部为准
        {
            uint32_t ENABLE_USED : 1;//:0：禁止外部FLASH使用（内部FLASH信息权限）1：允许外部FLASH使用
            uint32_t LAST8M_DISABLE : 1;//0:从后8M开始读取 1: 全16M
            uint32_t LowAddr_OFFSET : 2;//低4位选择0:只MIRROR上次地址，1:64k为间隔，2:16k查找范围，3:4k查找范围（默认）
            uint32_t HighAddr_SPACE : 3;//外部FLASH查找地址最高位FLASH_HighAddr_SPACE+17（小于它）,最高只查到0xffffff为止
            uint32_t MUST_MIRROR_DISABLE : 1;//0：无论是否不同，必MIRROR，1：不同才MIRROR
            uint32_t NOCARE : 8;//SYSCTL_RESERVERD 高8位//并不关心
            uint32_t SPACE_OFFSET : 1; // 相对于Mirror_Addr而言0：地址为偏移的绝对值 1：地址外地址为EC映射地址的绝对值(默认)
            uint32_t INFO_EXPLICIT : 1; // 0：隐式(即该部分可不在程序内):1：显式（即在程序内），隐式时安全验签时候0x100-0x200的字节由0xff（所有bit为1）覆盖计算
            uint32_t EXTERNAL_NOBOOT : 1; //0：如果起始地址在外部flahs前512k则从外部flash启动，否则MIRROR，1：MIRROR机制
            uint32_t PWM_Enable : 1;//MIRROR期间 0：关闭pwm闪烁，1：开启pwm闪烁
            uint32_t PWMn_Switch : 1;//启用后mirror期间pwm n闪烁(n== 0/1)
            uint32_t SPI_Switch : 1;//0:quad 四线SPI 1:dual 二线SPI
            uint32_t WP_Switch : 1;//0:8528pin2pin 1:5571pin2pin(default)
            uint32_t HOLD_Switch : 1;//0:8528pin2pin 1:5571pin2pin(default)
            uint32_t Firmware_4KSector : 7;//按4k为单位，MIRROR按这个大小进行拷贝//最大512k//实际大小为该值+1，因此要求除了flashinfo以外又有内容都在512k以内
            uint32_t RESERVED : 1;//无效位
            // uint32_t RESERVED : 16;//无效位
            // uint32_t RESERVED : 24;//无效位也就相当于这个RESERVED
        };
    }EXTERNAL_FLASH_CTRL;
//0 以下根据判断选择内外部（外部和内部不同选择外部，否则内部或者当没有）
    /*v3版本开始只有内部时钟，该两位无效*/
    uint32_t RESERVED_EXTCLOCK_Disable : 1;//0：启用外部时钟引脚，1：关闭外部时钟引脚(默认)
    uint32_t RESERVED_EXTClock_Switch : 1;//0：如果启用外部时钟引脚，则配置为外部时钟1：使用内部时钟（默认）
    uint32_t MainFrequency : 6;// 主频分频倍率 
//8 
    uint32_t SECVER_Enable : 1; // 0：EFUSE决定，1：安全验签测试输出（默认）
    uint32_t EXIT_ReBOOT_Switch : 1; // （安全失败后一段时间）0休眠 1自动重启（默认）
    uint32_t PATCH_Disable : 1;//0:打开ROM PATCH，1：关闭rom patch（默认）
    uint32_t LPC_Enable : 1;//0：ESPI口 1：LPC口
#define ESPI_Disable LPC_Enable
    uint32_t DoubleBoot_Disable : 1;//0：打开双启动 1：关闭双启动（默认）
    uint32_t WDT_Disable : 1; //0：开启看门狗 1： 关闭看门狗（默认）
    uint32_t EJTAG_Enable : 1;//0：关闭EJTAG，1:开启ejtag（默认）
    uint32_t EJTAG_Switch : 1;//0：PIOE[13:10]为KBS则配置PIOB[30:27]为ejtag口 1：PIOE[13:10]（默认）
//16
    uint32_t UART_Enable : 1; // 0关闭串口，1打开所有串口
    uint32_t DEBUG_PRINTF_Enable : 1; // 0：关闭调试串口，1：打开调试口
    uint32_t Uartn_Print_SWitch : 2;//该口作为调试输出口选择（UART 01AB），剩余接口作为调试器
    uint32_t DEBUGGER_Enable : 1; // 0：关闭调试器功能，
    uint32_t DEBUGGER_UART_Enable : 1; // 0：关闭调试器口，1：打开所有调试口
    /*由于未使用smbus验证调试器功能，该两位无效*/
    uint32_t DEBUGGER_SMBUS_Enable : 1; // 0：关闭DEBUGGER SMBUS调试口（GPIO） 1：debugger SMBUS功能生效
    uint32_t SMBUS_CLOCK_Switch : 1; //0:高速时钟400k 1: 低速100k 

    //24
    uint32_t DEBUG_BAUD_RATE : 7; // debug输出波特率(3200倍数，0)
    uint32_t DEBUG_LEVEL : 1; //0:更详细的过程参数1:关闭仅输出过程位置
    //32
    uint32_t DEBUG_PRINTF_DLS : 2; // 数据位(3)
    uint32_t DEBUG_PRINTF_STOP : 1; // 停止位(0)
    uint32_t DEBUG_PRINTF_PE : 1; // 校验位（0）
    uint32_t DEBUG_PRINTF_EPE : 1; //奇偶校验选择（0）
    uint32_t RESERVED0 : 3;//无效位
    //40
    /*调试器波特率可能无效(9600)为准，暂时未定*/
    uint32_t DEBUGGER_BAUD_RATE : 7; // debugger波特率(3200倍数)
    uint32_t DEBUGGER_LEVEL : 1; // 0:允许写入，1：未报错只允许读取
    //48
    uint32_t DEBUGGER_DLS : 2; // 数据位(3)
    uint32_t DEBUGGER_STOP : 1; //  停止位(0)
    uint32_t DEBUGGER_PE : 1; // 校验位（0）
    uint32_t DEBUGGER_EPE : 1; //奇偶校验选择（0）
    uint32_t RESERVED1 : 3;//无效位
    //56
    uint32_t RESERVED2 : 8;//无效位
} sFixedFlashInfo;
#pragma pack(4) // 4字节对齐
extern int main();
extern sMode Mode;

extern volatile uint32_t PRINTF_FIFO;
extern volatile uint32_t DEBUGGER_FIFO;
/*-------------------------------------set reg Information-------------------------------------*/
#include <AE_REG.H>
#include <AE_CONFIG.H>
#include <AE_LIBC.H>
#include <AE_MALLOC.H>
#include <AE_PRINTF.H>
#include <AE_FUNC.H>
#endif
