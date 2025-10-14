#include <stdio.h>
#include <stdlib.h>
typedef __uint32_t uint32_t;
typedef __uint16_t uint16_t;
typedef __uint8_t uint8_t;

typedef struct _efuse_info{
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
typedef union _efuse_wd{
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
uint32_t EFUSE[64] = { 0 };
#define EFUSE_BASE_ADDR (&EFUSE[0])
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

#define REG_ADDR(addr, offset) ((uint8_t *)(addr)+(offset))
#define REG32(addr) (*(uint32_t *)(addr))
#define EFUSE_REG_ADDR(offset) REG_ADDR(EFUSE_BASE_ADDR, offset)
#define EFUSE_REG(offset) REG32(EFUSE_REG_ADDR(offset))
// #define EFUSE_REG(offset)
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

int main(int argc, char **argv)
{
    uint32_t input_val[2] = { 0 };
    uint32_t *data = NULL;
    if(argc >= 2)
    {
        // printf("GEt %s of EFUSE_WD[0]\n", argv[1]);
        input_val[0] = strtol(argv[1], NULL, 16);
        printf("GET %#x of EFUSE_WD[0]\n", input_val[0]);
        if(argc >= 3)
        {
            // printf("GEt %s of EFUSE_WD[1]\n", argv[2]);
            input_val[1] = strtol(argv[2], NULL, 16);
            printf("GET %#x of EFUSE_WD[1]\n", input_val[1]);
        }
        data = input_val;

    }
#if 1
    uint8_t pubkey_hash_SHA256_aes128ecb_AESKEY_byte[32] = {
    //       /*test0512*/ 0x90,0x3A,0xD0,0x62,0x0E,0xD4,0x09,0xBF,0xED,0xB1,0xBB,0x0A,0x69,0xC4,0x8D,0x3E,0xA8,0x19,0x38,0xBE,0x50,0x3D,0x4F,0xFF,0xC4,0x73,0x65,0x19,0x32,0xE8,0x77,0x50
    //       /*test1024*/ 0xFC,0x48,0x82,0x1A,0xE7,0xAB,0x80,0x72,0x0D,0xEC,0xFF,0xAF,0x95,0xBA,0x00,0xBD,0xE4,0x87,0x84,0x65,0xDA,0x8E,0x46,0xD3,0xB0,0x32,0x6D,0x98,0xCD,0x66,0x0A,0x18
        // /*test2048*/ 0x8A,0x69,0x77,0xA7,0x5D,0x3A,0x1F,0x1F,0x13,0x9E,0x69,0xAE,0x92,0xC0,0xE6,0x44,0x7D,0x1E,0xF4,0xCA,0x55,0x66,0xAF,0xD9,0x5E,0x8B,0xED,0x4A,0x10,0x1D,0xF1,0x66
        /*test4096*/ 0x09,0x45,0x7A,0x6F,0x7C,0xC5,0x0D,0xB6,0x3D,0x8B,0xC8,0xEA,0xD3,0x2B,0x4F,0x69,0xC3,0xDF,0x65,0x30,0x02,0xCC,0xA7,0x9A,0xF4,0x94,0x6D,0x3B,0xFB,0x6D,0x26,0xC4

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
        printf("EFUSE_WD[%d] = %#x\n", i, EFUSE_WD(i));
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
    printf("EFUSE_PUBKEY_HASH[0]: %#x\n", efuse_data.efuse_info.pubkey_hash[0]);
    printf("EFUSE_PUBKEY_HASH[1]: %#x\n", efuse_data.efuse_info.pubkey_hash[1]);
    printf("EFUSE_PUBKEY_HASH[2]: %#x\n", efuse_data.efuse_info.pubkey_hash[2]);
    printf("EFUSE_PUBKEY_HASH[3]: %#x\n", efuse_data.efuse_info.pubkey_hash[3]);
    printf("EFUSE_PUBKEY_HASH[4]: %#x\n", efuse_data.efuse_info.pubkey_hash[4]);
    printf("EFUSE_PUBKEY_HASH[5]: %#x\n", efuse_data.efuse_info.pubkey_hash[5]);
    printf("EFUSE_PUBKEY_HASH[6]: %#x\n", efuse_data.efuse_info.pubkey_hash[6]);
    printf("EFUSE_PUBKEY_HASH[7]: %#x\n", efuse_data.efuse_info.pubkey_hash[7]);
    printf("EFUSE_AES_KEY[0]: %#x\n", efuse_data.efuse_info.aes_key[0]);
    printf("EFUSE_AES_KEY[1]: %#x\n", efuse_data.efuse_info.aes_key[1]);
    printf("EFUSE_AES_KEY[2]: %#x\n", efuse_data.efuse_info.aes_key[2]);
    printf("EFUSE_AES_KEY[3]: %#x\n", efuse_data.efuse_info.aes_key[3]);
    printf("EFUSE_CRC32_PUBKEYHASH_AESKEY: %#x\n", efuse_data.efuse_info.crc32_aes_key_hash);
}
