/*
 * @Author: dejavuwdh
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-19 14:24:33
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
#ifndef AE_ROMPATCH_H
#define AE_ROMPATCH_H
#include "AE_GLOBAL.H"
#include "AE_INCLUDE.H"
#if PRINTF_UART_SWITCH == 0
#define UART_BAUD UART0_BAUD
#elif PRINTF_UART_SWITCH == 1
#define UART_BAUD UART1_BAUD
// #elif PRINTF_UART_SWITCH == 2
// #define UART_BAUD UART2_BAUD
// #elif PRINTF_UART_SWITCH == 3
// #define UART_BAUD UART3_BAUD
#elif PRINTF_UART_SWITCH == 2
#define UART_BAUD UARTA_BAUD
#elif PRINTF_UART_SWITCH == 3
#define UART_BAUD UARTB_BAUD
#endif

#define NEW_ROM 1
#if NEW_ROM

// extern of addr
extern BYTE _start;
extern BYTE vector_base;
extern BYTE _rom;
extern BYTE _rom_var;
extern BYTE _MAGIC_NUMBER;
extern BYTE RAM_ADDR;
typedef enum FLASH_PIN { NOPIN_FLASH = -1, PIN8528_FLASH = 0, PIN5571_FLASH } SPIF_PIN;
typedef enum FLASH_SELECT { NOSELECT_FLASH = -1, INTERNAL_FLASH = 0, EXTERNAL_FLASH } SPIF_INT_EXT;
typedef enum FLASH_MODE { NOCONFIGMODE = -1, DUAL_FLASH = 0, QUAD_FLASH } SPIF_MODE;


typedef struct _rom_data//关于FLASH的一些信息
{

    uint32_t EXTFlash_ID;//外部FLASH ID如果外部FLASH ID发生变动，需要除了版本号以外校验签名是否一致
    uint32_t FixedFlashInfo_Addr : 24;//外部flash中固件的相对位置位置(上次MIRROR位置,绝对地址)
    uint32_t Program_Method : 3; // 上次烧写方式//0mirror更新 1cmd更新,2debugeruart更新3DEBUGGERiic更新4lpc更新5espi更新6:external flash7:no know
    uint32_t MIRRO_Enable : 1; // 外部FLASH接口和镜像功能.默认是开着的，除非软件关闭(即外部FLASH设置或者DEBUGGER设置)
    uint32_t RESERVED : 4;//不生效

} srom_data;
typedef struct _patch_typdef
{
    uDword data;
    BYTE addrl;
    BYTE addrh : 5;
    BYTE last : 1;//下一个还有
    BYTE Reserved : 2;
} spatch_typdef;
typedef struct _DynamicFlashInfo
{
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
typedef struct _FixedFlashInfo
{
    uint8_t Firmware_ID[11]; // 识别码
    uint8_t Compiler_Version[21]; // 版本号
    uint32_t Mirror_Addr;//外部flash中固件的绝对位置(MIRROR起始位置)
    uint32_t IVT;//中断向量表起始位置Interrupt_Vector_Table（相对Mirror_Addr的起始地址）
    uint32_t Restart;//代码复位运行起始位置Reset Start
    uint32_t DBoot_IVT;//双启动第二段代码的中断位置
    uint32_t DBoot_Restart;//双启动第二段代码的起始位置
    uint32_t DynamicFlashInfo;//尾部FLASH相关位置 
    union
    {
        uint32_t SYSCTL_RESERVERD;
        struct
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
//0
    /*v3版本使用内部，两位无效 */
    uint32_t RESERVED_EXTCLOCK_Disable : 1;//0：启用外部时钟引脚，1：关闭外部时钟引脚(默认)
    uint32_t RESERVED_EXTClock_Switch : 1;//0：如果启用外部时钟引脚，则配置为外部时钟1：使用内部时钟（默认）
    uint32_t MainFrequency : 6;// 主频分频系数 
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
    uint32_t UART_Enable : 1; // 0关闭串口，1打开串口
    uint32_t DEBUG_PRINTF_Enable : 1; // 0：关闭调试串口，1：打开调试口
    uint32_t Uartn_Print_SWitch : 2;//该口作为调试输出口选择（UART 01AB），剩余接口作为调试器
    uint32_t DEBUGGER_Enable : 1; // 0：关闭调试器功能，
    uint32_t DEBUGGER_UART_Enable : 1; // 0：关闭调试器口，1：打开所有调试口
    uint32_t DEBUGGER_SMBUS_Enable : 1; //暂时无效 0：关闭DEBUGGER SMBUS调试口（GPIO） 1：debugger SMBUS功能生效
    uint32_t SMBUS_CLOCK_Switch : 1; //0:高速时钟400k 1: 低速100k //暂时无效

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
#else
typedef struct _rom_data
{
    BYTE magic_number[8]; // 识别码 ok
    ssign SM2_ssign1;     // SM2签名 ok
} sDynamicFlashInfo;
typedef struct _flash_info
{
    DWORD flash_first_offset : 20; // 第一个程序运行的起始地址（以flash中位置为基准）ok
    BYTE first_code_size : 4;      // 第一个程序 大小
    BYTE flash_size : 4;           // flash 大小 no
    BYTE patch_en : 1;             // patch 使能
    BYTE double_boot : 1;          // 双启动 ok
    BYTE safe_mode : 1;            // 安全模式，只需要efuse或者程序中一个设置了，必然为开启
    BYTE mirror_once : 1;          // 是否镜像（镜像最后都到内部运行，非镜像都到外部运行）
    DWORD flash_secound_offset : 20; // 第二个程序运行的起始地址（以flash中位置为基准） ok
    BYTE secound_code_size : 4;      // 第二个程序 大小
    BYTE main_freq : 6;              // 主频 ok
    BYTE extern_osc_flag : 1;        // 外部osc ok
    BYTE flash_qspi_flag : 1;        // flash 四线二线 ok
    DWORD debug_uart_baud : 24; // debug波特率
    BYTE debug_uart_en : 1;     // 调试器端口
    BYTE debug_uart_com : 3;    // uart com
    BYTE patch_total : 4;       // patch数量
    WORD jtag_tdi : 3;   // 0:A7,1:F5,2:H5,3:J1,4:J5,5:P6
    WORD jtag_tdo : 3;   // 0:B2,1:F4,2:H3,3:J0,4:J4,5:P4
    WORD jtag_tms : 3;   // 0:B4,1:C2,2:F7,3:H2,4:H4,5:P5
    WORD jtag_tck : 3;   // 0:B3,1:C1,2:F6,3:H1,4:H6,5:P7
    BYTE jtag_en : 1;    // jtag使能
    BYTE flash_wp : 1;   // ec103b select spif wp (0:PIOB17,1:PIOD8)
    BYTE flash_hold : 1; // ec103a selectspif hold (0：PIOA16,1:PIOB19)
    BYTE reserved0 : 1;  // reserved
    WORD rom_wdt_time;   // flash看门狗 ok
} sFixedFlashInfo;
#endif
void OPTIMIZE0 USED rom_patch_config_func(void);
void OPTIMIZE0 USED rom_patch_test_func_flash(void);
extern spatch_typdef patch_array[];
#define X1_RA (0x1)
#define JAL_RD(rd) ((rd & 0x1f) << 7)
#define JAL_OPCODE (0x6f)
#define JAL_ADDR_OFFSET(pc_addr, jp_addr) (jp_addr - pc_addr)
#define JAL_IMM(imm) (((imm & 0x7fe) << 20) | ((imm & 0x800) << 9) | (imm & 0xff000) | ((imm & 0x100000) << 11))
#define JAL_CODE(rd, pc_addr, jp_addr) (JAL_OPCODE | JAL_RD(rd) | JAL_IMM(JAL_ADDR_OFFSET(pc_addr, jp_addr)))
// JAL_ADDR_OFFSET(pc_addr,jp_addr)))
#define JAL_RA_CODE(pc_addr, jp_addr) (0xef | JAL_IMM(JAL_ADDR_OFFSET(pc_addr, jp_addr)))
// JAL_ADDR_OFFSET(pc_addr,jp_addr)))
#define JAL_CODE_BYTE0(rd) ((JAL_OPCODE & 0x7f) | (JAL_RD(rd) & 0x80)) & 0xff
#define JAL_CODE_BYTE1(rd, imm) (((imm & 0xf000) | (JAL_RD(rd) & 0xf00)) >> 8) & 0xff
#define JAL_CODE_BYTE2(imm) (((imm & 0xf0000) >> 16) | ((imm & 0x800) >> 7) | ((imm & 0xe) << 4)) & 0xff
#define JAL_CODE_BYTE3(imm) ((((imm & 0x7f0) >> 4) | ((imm & 0x100000) >> 13))) & 0xff
BYTE rom_patch_demo(BYTE, BYTE);
void earse_internel_flash(void);
#endif