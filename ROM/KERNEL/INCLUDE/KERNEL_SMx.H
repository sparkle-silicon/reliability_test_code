/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-05-24 17:40:48
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
#ifndef KERNEL_SMx_H
#define KERNEL_SMx_H
//head include
#include <AE_GLOBAL.H>
#include <AE_REG.H>
#include <AE_CONFIG.H>
//typedef
#define SM4_ECB_ENCODE                          HEX00
#define SM4_ECB_DECODE                          HEX01
#define SM4_CBC_ENCODE                          HEX02
#define SM4_CBC_DECODE                          HEX03
//bit set
#define SM2_CONTROL_MODN_INV                    HEX00//模N反
#define SM2_CONTROL_MODN_MUL                    HEX01//模N乘
#define SM2_CONTROL_MODN_ADD                    HEX02//模N加
#define SM2_CONTROL_MODN_SUB                    HEX03//模N减
#define SM2_CONTROL_DOT_ADD                     HEX04//点加
#define SM2_CONTROL_DOT_DBL                     HEX05//倍点
#define SM2_CONTROL_DOT_MUL                     HEX06//点乘
#define SM2_CONTROL_MODP_INV                    HEX08//模P反
#define SM2_CONTROL_MODP_MUL                    HEX09//模P乘
#define SM2_CONTROL_MODP_ADD                    HEX0A//模P加
#define SM2_CONTROL_MODP_SUB                    HEX0B//模P减
#define SM2_CONTROL_FREE                        HEX0F//空闲
#define SM2_CONTROL_EN                          BIT4
#define SM2_CONTROL_SOFTRESET                   (~BIT5)
#define SM2_STATUS_FREE                         HEX00
#define SM2_STATUS_BUSY                         HEX01
#define SM2_STATUS_DONE                         HEX02
#define SM2_STATUS_ERROR                        HEX03

#define SM3_CONFIG_ENABLE                       HEX01
#define SM3_CONFIG_NEW_DATA                     HEX02
#define SM3_STATE_FINISH                        HEX01
#define SM3_STATE_FIFO_FULL                     HEX02
#define SM3_STATE_FIFO_EMPTY                    HEX04
#define SM3_STATE_FIFO_COUNT                    0xF8
#define SM3_STATE_STATE_ROUND                   0x7F00

#define SM4_MODE_ENDE0                          HEX00
#define SM4_MODE_ENDE1                          HEX01
#define SM4_MODE_KEYEX                          HEX02
#define SM4_MODE_XOR                            HEX04
#define SM4_CTRL_INTEN                          HEX02
#define SM4_CTRL_START                          HEX01
#define SM4_STATUS_BUSY                         HEX02
#define SM4_STATUS_DONE                         HEX01
#define SM4_INT_INT                             HEX01
//命令
#define TRNG_CTRL_CMD_NULL                      HEX00//空指令
#define TRNG_CTRL_CMD_RAND                      HEX01//生成随机数
#define TRNG_CTRL_CMD_BUILD_SEED                HEX02//生成随机种子
#define TRNG_CTRL_CMD_LOAD_SEED                 HEX03//导入随机种子
//状态（RO）
#define TRNG_STAT_NONCE_MODE                    HEX04//当前NONCE模式
#define TRNG_STAT_R256                          HEX08//mode设置的R256bit
#define TRNG_STAT_MISSION_MODE                  0x100//smod设置的位，测试/任务模式
#define TRNG_STAT_SEEDED                        0x200//已生成种子
#define TRNG_STAT_LAST_RECMD_SEED               0x00000//上次是随机种子生成命令
#define TRNG_STAT_LAST_RELOAD_SEED              0x30000//上次是nonce数据载入
#define TRNG_STAT_LAST_REAUTO_SEED              0x40000//上次是自动生成
#define TRNG_STAT_LAST_NOSEED                   0x70000//上次是未执行
#define TRNG_STAT_SRVC_RQST                     0x8000000//当前存在未处理命令
#define TRNG_STAT_RAND_GENERATING               0x40000000//正在生成随机数
#define TRNG_STAT_RAND_RESEEDING                0x80000000//正在产生随机种子
//模块配置
#define TRNG_MODE_R256                          HEX08//输出128bit/256bibt个随机数
#define TRNG_SMODE_NONCE_MODE                   HEX04//开启NONCE模式，上位机提供种子
#define TRNG_SMODE_MISSION_MODE                 0x100//任务模式，上位机不可访问内部状态和测试域
#define TRNG_SMODE_MAX_REJECTS(x)               ((x&0xff)<<16)//效果不好时最大放弃的bit
//中断配置
#define TRNG_IE_RAND_RDY_EN                     HEX01//随机数完成
#define TRNG_IE_SEED_DONE_EN                    HEX02//种子完成
#define TRNG_IE_AGE_ALARM_EN                    HEX04//过期警报
#define TRNG_IE_RQST_ALARM_EN                   HEX08//请求警报
#define TRNG_IE_LFSR_LOCKUP_EN                  HEX10//锁住警报
#define TRNG_IE_GLBL_EN                         0x80000000//全局中断使能
//中断状态（RW1C）
#define TRNG_ISTAT_RAND_RDY                     HEX01//随机数完成
#define TRNG_ISTAT_SEED_DONE                    HEX02//种子完成
#define TRNG_ISTAT_AGE_ALARM                    HEX04//过期警报
#define TRNG_ISTAT_RQST_ALARM                   HEX08//请求警报
#define TRNG_ISTAT_LFSR_LOCKUP                  HEX10//锁住警报
//特征状态（RO）
#define TRNG_FEATURES_MAX_RAND_LENGTH           HEX01//最大随机数长度
#define TRNG_FEATURES_RAND_SEED_AVAIL           HEX04//存在随机位产生器
#define TRNG_FEATURES_MISSION_MODE_RESET_STAT   HEX08//复位后进入测试/任务状态
//构建状态（RO）
#define TRNG_BUILD_CONFIG_MAX_PRNG_LEN          HEX04//最大支持随机数长度
#define TRNG_BUILD_CONFIG_PRNG_LEN_AFTER_RST    HEX08//复位后默认随机数长度
#define TRNG_BUILD_CONFIG_MODE_AFTER_RST        HEX10//复位到任务模式
#define TRNG_BUILD_CONFIG_AUTO_RESEED_LOOPBACK  HEX20//有随机种子自动生成功能


//extern
/**************************************************************************************
 * function:SM2_STATTE_ERROR  run code
 * note : weak code
 * ************************************************************************************/
extern void SM2_Error(void);
/**************************************************************************************
 * function:SM2_VERIFY (Visa inspection)
 * note :
 *          r :r of the digital signature {r,s}
 *          s:s of the digital signature {r,s}
 *          e:Password hash function value
 *          g:Base point of an elliptic curve
 *          PA:public key
 * ************************************************************************************/
// BYTE SM2_VERIFY(DWORDP r, DWORDP s, DWORDP e, spoint g, spoint PA);
extern BYTE Smx_output[32];
/**************************************************************************************
 * function:SM3_Run
 * note :
 *          bit:data bit size
 *          input: Plaintext password
 *          output: Ciphertext storage
 * ************************************************************************************/
extern void SM3_Run(DWORD bit, BYTEP input, BYTEP output);
/**************************************************************************************
 * function:SM4_Run
 * note :
 *          mode:0 ECB加密,1 ECB解码/2 CBC加密/3 CBC解码
 *          input: Plaintext password
 *          key:Extended secret key
 *          output: Ciphertext storage
 *          xordr :lv
 * ************************************************************************************/
extern void SM4_Run(DWORD bit, BYTE mode, BYTEP input, BYTEP key, BYTEP output, BYTEP xordr);//to run sm4
extern void TRNG_Init(u8 ie, u16 rqsts, u16 age);
extern u32 *TRNG_RUN(u32 *seed, u32 *rand);

#define SM2_VERIFY_SIGN_TRUE 0
#define SM2_VERIFY_SIGN_FALSE 1
#define SM2_SIGN_PROCESS_FALSE 2
#define SM2_KEY_PAIR_FALSE 3
#define SM2_Hash_Value_Z_FALSE 4
#if SMx_TEST
extern void SM_Test_main(void);//run SMx test code
#endif
#endif