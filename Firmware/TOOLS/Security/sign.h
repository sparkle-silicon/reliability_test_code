/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-12-24 15:18:25
 * @Description:
 *
 *
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 以下是中文及英文版权同步声明，编码为UTF-8。
 * Copyright has legal effects and violations will be prosecuted.
 * 版权具有法律效力，违反必究。
 *
 * Copyright ©2021-2025 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2025龙晶石半导体科技（苏州）有限公司
 */
#ifndef __SPK32GLE01__SIGN__H__
#define __SPK32GLE01__SIGN__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <openssl/obj_mac.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <openssl/err.h>

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

#define ECC_BIGINT32_MAXLEN 	(512>>5)//16dword  //(384>>5)//12dword
#define RSA_MODULUS_BITS_MAX 4096//512byte//3072//384byte//模数长度最大
#define RSA_MODULUS_LEN_MAX ((RSA_MODULUS_BITS_MAX + 31) >>5)//   bit/8/4 word类型
#define RSA_PRIME_BITS_MAX	((RSA_MODULUS_BITS_MAX + 1) >>1)
#define RSA_PRIME_LEN_MAX	((RSA_PRIME_BITS_MAX   + 31)  >>5)//   bit/8/4 word类型

typedef struct//12*4+4=52byte/16*4+4=68byte
{
    uint32_t   uLen;
    uint32_t   auValue[ECC_BIGINT32_MAXLEN];
}ECC_STU_BIGINT32;
typedef struct//12*4+4=52byte/16*4+4=68byte
{
    uint32_t   uLen;
    uint8_t   auValue[ECC_BIGINT32_MAXLEN << 2];
}ECC_STU_BIGINT32_BYTE;
typedef struct//4+6*52=316byte/4+6*68=412byte
{
    uint32_t uBits;
    ECC_STU_BIGINT32  stuPrimeP;/**< 素数 P 的位数 */
    ECC_STU_BIGINT32  stuCoefficientA; /**< 系数 A，用于定义椭圆曲线方程 y^2 = x^3 + Ax + B */
    ECC_STU_BIGINT32  stuCoefficientB;/**< 系数 B，用于定义椭圆曲线方程 y^2 = x^3 + Ax + B */
    ECC_STU_BIGINT32  stuGx;/**< 基点 G 的 x 坐标 */
    ECC_STU_BIGINT32  stuGy;/**< 基点 G 的 y 坐标 */
    ECC_STU_BIGINT32  stuPrimeN;/**< 素数 N，用于定义椭圆曲线上的点的数量 */
} ECC_FP_STU_PRAMS;
typedef struct//4+6*52=316byte/4+6*68=412byte
{
    uint32_t uBits;
    ECC_STU_BIGINT32_BYTE  stuPrimeP;/**< 素数 P 的位数 */
    ECC_STU_BIGINT32_BYTE  stuCoefficientA; /**< 系数 A，用于定义椭圆曲线方程 y^2 = x^3 + Ax + B */
    ECC_STU_BIGINT32_BYTE  stuCoefficientB;/**< 系数 B，用于定义椭圆曲线方程 y^2 = x^3 + Ax + B */
    ECC_STU_BIGINT32_BYTE  stuGx;/**< 基点 G 的 x 坐标 */
    ECC_STU_BIGINT32_BYTE  stuGy;/**< 基点 G 的 y 坐标 */
    ECC_STU_BIGINT32_BYTE  stuPrimeN;/**< 素数 N，用于定义椭圆曲线上的点的数量 */
} ECC_FP_STU_PRAMS_BYTE;
typedef struct//2*52=104byte/2*68=136byte
{
    ECC_STU_BIGINT32  stuQx;
    ECC_STU_BIGINT32  stuQy;
} ECC_STU_PUBKEY;
typedef ECC_STU_PUBKEY ECC_STU_SHAREDKEY;
typedef struct//2*52=104byte/2*68=136byte
{
    ECC_STU_BIGINT32_BYTE  stuQx;
    ECC_STU_BIGINT32_BYTE  stuQy;
} ECC_STU_PUBKEY_BYTE;

typedef struct{
    uint32_t bits;                     /* length in bits of modulus */
    uint32_t modulus[RSA_MODULUS_LEN_MAX];  /* modulus */
    uint32_t exponent[RSA_MODULUS_LEN_MAX]; /* public exponent */
} STU_RSA_PUBKEY;
typedef struct{
    uint32_t bits;                     /* length in bits of modulus */
    uint8_t modulus[RSA_MODULUS_LEN_MAX << 2];  /* modulus */
    uint8_t exponent[RSA_MODULUS_LEN_MAX << 2]; /* public exponent */
} STU_RSA_PUBKEY_BYTE;
typedef struct{
    uint32_t bits;                     /* length in bits of modulus */
    uint32_t modulus[RSA_MODULUS_LEN_MAX];  /* modulus */
    uint32_t publicExponent[RSA_MODULUS_LEN_MAX];     /* public exponent */
    uint32_t exponent[RSA_MODULUS_LEN_MAX]; /* private exponent */
    uint32_t prime[2][RSA_PRIME_LEN_MAX];   /* prime factors *///pq
    uint32_t primeExponent[2][RSA_PRIME_LEN_MAX];     /* exponents for CRT */
    uint32_t coefficient[RSA_PRIME_LEN_MAX];//inv          /* CRT coefficient */
} STU_RSA_PRIVKEY;
typedef struct{
    uint32_t bits;                     /* length in bits of modulus */
    uint8_t modulus[RSA_MODULUS_LEN_MAX << 2];  /* modulus *///n：模数
    uint8_t publicExponent[RSA_MODULUS_LEN_MAX << 2];     /* public exponent */// e：公钥指数
    uint8_t exponent[RSA_MODULUS_LEN_MAX << 2]; /* private exponent *///d：私钥指数
    uint8_t prime[2][RSA_PRIME_LEN_MAX << 2];   /* prime factors *///p：最初的大素数,q：最初的大素数
    uint8_t primeExponent[2][RSA_PRIME_LEN_MAX << 2];     /* exponents for CRT */ // dmp1：e*dmp1 = 1 (mod (p-1)), dmq1：e*dmq1 = 1 (mod (q-1))
    uint8_t coefficient[RSA_PRIME_LEN_MAX << 2];//inv          /* CRT coefficient */ q*iqmp = 1 (mod p ) : coefficient*prime1 mod modulus =1 
} STU_RSA_PRIVKEY_BYTE;
typedef struct{
    uint32_t bits;                           /* length in bits of modulus */
    int useFermat4;                              /* public exponent (1 = F4, 0 = 3) */
} STU_RSA_KEY;
#pragma pack(1)
typedef struct _rom_data//关于FLASH的一些信息
{

    uint32_t EXTFlash_ID;//外部FLASH ID如果外部FLASH ID发生变动，需要除了版本号以外校验签名是否一致
    uint32_t FixedFlashInfo_Addr : 24;//外部flash中固件的相对位置位置(上次MIRROR位置,绝对地址)
} srom_data;
typedef union _publickey{
    STU_RSA_PUBKEY rsa;
    STU_RSA_PUBKEY_BYTE rsa_byte;
    ECC_STU_PUBKEY ecc;//含SM2的椭圆曲线公钥
    ECC_STU_PUBKEY_BYTE ecc_byte;//含SM2的椭圆曲线公钥
} upublickey;
typedef union _signature{
    uint32_t rsa[RSA_MODULUS_LEN_MAX];//rsa
    uint8_t rsa_byte[RSA_MODULUS_LEN_MAX << 2];//rsa
    uint32_t hash[512 >> 5];
    uint8_t hash_byte[512 >> 3];
    struct{
        ECC_STU_BIGINT32 R;
        ECC_STU_BIGINT32 S;
    }ecc;
    struct{
        ECC_STU_BIGINT32_BYTE R;
        ECC_STU_BIGINT32_BYTE S;
    }ecc_byte;
} usignature;
typedef struct _patch_typdef{
    uDword data;
    BYTE addrl;
    BYTE addrh : 5;
    BYTE last : 1;//下一个还有
    BYTE Reserved : 2;
} spatch_typdef;


typedef struct _DynamicFlashInfo{
    uint8_t Firmware_ID[16]; // 识别码，确认安全验签位置，和开头识别码做区分
    usignature sign;     // 安全签名
    upublickey publickey;// 公钥
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
typedef struct _FixedFlashInfo{
    uint8_t Firmware_ID[11]; // 识别码
    uint8_t Compiler_Version[21]; // 版本号-根据这个识别要不要更新外部FLASH
    uint32_t IVT;//中断向量表起始位置Interrupt_Vector_Table（相对Mirror_Addr的起始地址）
    uint32_t Restart;//代码复位运行起始位置Reset Start
    uint32_t Backup_OFFSET;//备份区的相对位置（相对Mirror_Addr的偏移地址）
    uint32_t DynamicFlashInfo;//尾部FLASH相关位置 
    spatch_typdef PATCH[8];
    union{
        uint32_t SYSCTL_RESERVERD;
        struct{
            uint32_t ENABLE_USED : 1;//:0：禁止外部FLASH使用（内部FLASH信息权限）1：允许外部FLASH使用
            uint32_t LAST8M_DISABLE : 1;//0:从后8M开始读取 1: 全16M
            uint32_t LowAddr_OFFSET : 2;//低4位选择0:只MIRROR上次地址，1:64k为间隔，2:16k查找范围，3:4k查找范围（默认）
            uint32_t HighAddr_SPACE : 3;//外部FLASH查找地址最高位FLASH_HighAddr_SPACE+17（小于它）,最高只查到0xffffff为止
            uint32_t MUST_MIRROR_DISABLE : 1;//0：无论是否不同，必MIRROR，1：不同才MIRROR(无效)
            uint32_t NOCARE : 8;//SYSCTL_RESERVERD 高8位//并不关心，位对齐用，因为涉及
            uint32_t PWM_Enable : 1;//无效 MIRROR期间 0：关闭pwm闪烁，1：开启pwm闪烁
            uint32_t PWMn_Switch : 3;//无效 启用后mirror期间pwm n闪烁(n== 0/1)
            uint32_t SPI_Switch : 1;//0:quad 四线SPI 1:dual 二线SPI
            uint32_t WP_Switch : 2;//0:piob17 1:piob29(default) 2:piod8 3:piob29(default)
            uint32_t CS_Switch : 1;//0:fspi_csn0(piob22)(default) 1:fspi_csn1(pioa16)
            uint32_t Firmware_4KSector : 7;//按4k为单位，MIRROR按这个大小进行拷贝//最大512k//实际大小为该值+1，因此要求除了flashinfo以外又有内容都在512k以内
            // uint32_t RESERVED : 16;//无效位
            // uint32_t RESERVED : 24;//无效位也就相当于这个RESERVED
        };
    }EXTERNAL_FLASH_CTRL;
    //0
    uint32_t MainFrequency : 6;// 主频分频系数 
    uint32_t SECVER_Enable : 1; // 1：安全验签(默认)，0：EFUSE打开则安全验签，否则进行hash校验
    uint32_t SECVER_VERIFY_Switch : 1; // 1：RSA验签（默认），0：ECC验签//无效

    uint32_t SECVER_AES_Enable : 1; // 1：AES加密，0：不加密（默认）//无效
    uint32_t SECVER_HASH_Switch : 2;// 0:SHA224 1：SHA256（默认） 2：SHA384 3：SHA512
    uint32_t SECVER_BIT_Switch : 2; // 0:ECC192 ECC256（默认） ECC384 ECC512//无效
    uint32_t SECVER_AES_MODE_Switch : 2; // 3:ECB模式 2：CBC模式 1：CFB模式 0：OFB模式//无效
    uint32_t BACKUP_LOCACTION_Switch : 1; // 0:备份区域在正式代码前面，1：备份区域在正式代码后面（默认）
     //16
    uint32_t EXIT_ReBOOT_Switch : 1; // （安全失败后一段时间）0休眠 1自动重启（默认）
    uint32_t PATCH_Disable : 1;//0:打开ROM PATCH，1：关闭rom patch（默认）
    uint32_t LPC_Enable : 1;//0：ESPI口 1：LPC口
#define ESPI_Disable LPC_Enable
    uint32_t BACKUP_Enable : 1; // 0：关闭备份，1：备份（默认）（无效）
    uint32_t EJTAG_Enable : 1;//0：关闭EJTAG，1:开启ejtag（默认）
    uint32_t EJTAG_Switch : 1;//0：PIOE[13:10]为KBS则配置PIOB[30:27]为ejtag口, 1：PIOE[13:10]（默认）
    uint32_t CRYPTO_EJTAG_Switch : 1;//0：PIOE[13:10]配置为CRYPTO口, 1：PIOE[13:10]保留默认配置
    uint32_t DEBUG_PRINTF_Enable : 1; // 0：关闭调试串口，1：打开调试口
    //24
    uint32_t DEBUGGER_Enable : 1; // 0：关闭调试器功能，
    uint32_t Uartn_Print_SWitch : 2;//该口作为调试输出口选择（UART 01AB），剩余接口作为调试器
    uint32_t DEBUGGER_UART_Enable : 1; // 0：关闭调试器口，1：打开所有调试口
    uint32_t UART1_TXD_SWITCHE : 1; //0：pioe15 1：piob1(default)
    uint32_t UART1_RXD_SWITCHE : 1; //0:pioe14 1: piob3(default)
    uint32_t UARTA_TXD_SWITCHE : 1; // 0:PIOB6 1:PIOA9(default)
    uint32_t UARTA_RXD_SWITCHE : 1; // 0:PIOA23 1:PIOA8(default)
    //32
    uint32_t DEBUG_BAUD_RATE : 7; // debug输出波特率(3200倍数，0)
    uint32_t DEBUG_LEVEL : 1; //0:更详细的过程参数1:关闭仅输出过程位置
    //40
    uint32_t DEBUG_PRINTF_DLS : 2; // 数据位(3)
    uint32_t DEBUG_PRINTF_STOP : 1; // 停止位(0)
    uint32_t DEBUG_PRINTF_PE : 1; // 校验位（0）
    uint32_t DEBUG_PRINTF_EPE : 1; //奇偶校验选择（0）

    uint32_t DEBUGGER_BAUD_RATE : 7; // debugger波特率(3200倍数)

    uint32_t DEBUGGER_DLS : 2; // 数据位(3)
    uint32_t DEBUGGER_STOP : 1; //  停止位(0)
    uint32_t DEBUGGER_PE : 1; // 校验位（0）
    uint32_t DEBUGGER_EPE : 1; //奇偶校验选择（0）

} sFixedFlashInfo;

#pragma pack(4) // 4字节对齐

#define USER_USED_SOFTWARE 0
#define INVALID_NULL_VALUE_INPUT    0x1000
#define INVALID_INPUT_LENGTH        0x1001
#define CREATE_SM2_KEY_PAIR_FAIL    0x1002
#define COMPUTE_SM3_DIGEST_FAIL     0x1003
#define ALLOCATION_MEMORY_FAIL      0x1004
#define COMPUTE_SM2_SIGNATURE_FAIL  0x1005
#define INVALID_SM2_SIGNATURE       0x1006
#define VERIFY_SM2_SIGNATURE_FAIL   0x1007

typedef struct sm2_sig_structure{
    unsigned char r_coordinate[32];
    unsigned char s_coordinate[32];
} SM2_SIGNATURE_STRUCT;
typedef struct sm2_key_pair_structure{
/* Private key is a octet string of 32-byte length. */
    unsigned char pri_key[64];
/* Public key is a octet string of 65 byte length. It is a
   concatenation of 04 || X || Y. X and Y both are SM2 public
   key coordinates of 32-byte length. */
    unsigned char pub_key[129];
} SM2_KEY_PAIR;//sm2 key(public and private )
#define GLE01 1           
#endif /* __SPK32GLE01__SIGN__H__ */