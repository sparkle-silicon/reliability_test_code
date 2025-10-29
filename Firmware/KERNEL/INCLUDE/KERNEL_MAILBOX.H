/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-24 11:08:58
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
#ifndef KERNEL_MAILBOX_H
#define KERNEL_MAILBOX_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#define MAX_TASK_COUNT 12 //最大任务数
typedef void (*TaskFunction)(void *); // 定义任务函数类型，接受参数
// 定义一个结构体来封装多个参数
typedef struct{
    DWORD E2C_INFO1;
    DWORD E2C_INFO2;
    DWORD E2C_INFO3;
    DWORD E2C_INFO4;
    DWORD E2C_INFO5;
    DWORD E2C_INFO6;
} TaskParams;

typedef struct Task{
    TaskFunction function; // 任务函数指针
    TaskParams params;     // 任务参数
    struct Task *next;     // 指向下一个任务
} Task;
extern Task *task_head; // 任务链表头
extern volatile bool command_processed;

//设置宏过程，便携阅读
#define MAILBOX_EC_CMD MAILBOX_E2CINFO0
#define MAILBOX_EC_INFO1 MAILBOX_E2CINFO1
#define MAILBOX_EC_INFO2 MAILBOX_E2CINFO2
#define MAILBOX_EC_INFO3 MAILBOX_E2CINFO3
#define MAILBOX_EC_INFO4 MAILBOX_E2CINFO4
#define MAILBOX_EC_INFO5 MAILBOX_E2CINFO5
#define MAILBOX_EC_INFO6 MAILBOX_E2CINFO6
#define MAILBOX_EC_INFO7 MAILBOX_E2CINFO7
#define MAILBOX_EC_INT MAILBOX_E2CINT
#define MAILBOX_EC_INTEN MAILBOX_E2CINTEN

#define MAILBOX_CRYPTO_CMD MAILBOX_C2EINFO0
#define MAILBOX_CRYPTO_INFO1 MAILBOX_C2EINFO1
#define MAILBOX_CRYPTO_INFO2 MAILBOX_C2EINFO2
#define MAILBOX_CRYPTO_INFO3 MAILBOX_C2EINFO3
#define MAILBOX_CRYPTO_INFO4 MAILBOX_C2EINFO4
#define MAILBOX_CRYPTO_INFO5 MAILBOX_C2EINFO5
#define MAILBOX_CRYPTO_INFO6 MAILBOX_C2EINFO6
#define MAILBOX_CRYPTO_INFO7 MAILBOX_C2EINFO7
#define MAILBOX_CRYPTO_INT MAILBOX_C2EINT
#define MAILBOX_CRYPTO_INTEN MAILBOX_C2EINTEN

#ifdef crypto_subsystem
#define MAILBOX_SELF_CMD MAILBOX_CRYPTO_CMD
#define MAILBOX_SELF_INFO1 MAILBOX_CRYPTO_INFO1
#define MAILBOX_SELF_INFO2 MAILBOX_CRYPTO_INFO2
#define MAILBOX_SELF_INFO3 MAILBOX_CRYPTO_INFO3
#define MAILBOX_SELF_INFO4 MAILBOX_CRYPTO_INFO4
#define MAILBOX_SELF_INFO5 MAILBOX_CRYPTO_INFO5
#define MAILBOX_SELF_INFO6 MAILBOX_CRYPTO_INFO6
#define MAILBOX_SELF_INFO7 MAILBOX_CRYPTO_INFO7
#define MAILBOX_SELF_INT MAILBOX_CRYPTO_INT
#define MAILBOX_SELF_INTEN MAILBOX_CRYPTO_INTEN

#define MAILBOX_OTHER_CMD MAILBOX_EC_CMD
#define MAILBOX_OTHER_INFO1 MAILBOX_EC_INFO1
#define MAILBOX_OTHER_INFO2 MAILBOX_EC_INFO2
#define MAILBOX_OTHER_INFO3 MAILBOX_EC_INFO3
#define MAILBOX_OTHER_INFO4 MAILBOX_EC_INFO4
#define MAILBOX_OTHER_INFO5 MAILBOX_EC_INFO5
#define MAILBOX_OTHER_INFO6 MAILBOX_EC_INFO6
#define MAILBOX_OTHER_INFO7 MAILBOX_EC_INFO7
#define MAILBOX_OTHER_INT MAILBOX_EC_INT
#define MAILBOX_OTHER_INTEN MAILBOX_EC_INTEN
#else
#define MAILBOX_SELF_CMD MAILBOX_EC_CMD
#define MAILBOX_SELF_INFO1 MAILBOX_EC_INFO1
#define MAILBOX_SELF_INFO2 MAILBOX_EC_INFO2
#define MAILBOX_SELF_INFO3 MAILBOX_EC_INFO3
#define MAILBOX_SELF_INFO4 MAILBOX_EC_INFO4
#define MAILBOX_SELF_INFO5 MAILBOX_EC_INFO5
#define MAILBOX_SELF_INFO6 MAILBOX_EC_INFO6
#define MAILBOX_SELF_INFO7 MAILBOX_EC_INFO7
#define MAILBOX_SELF_INT MAILBOX_EC_INT
#define MAILBOX_SELF_INTEN MAILBOX_EC_INTEN

#define MAILBOX_OTHER_CMD MAILBOX_CRYPTO_CMD
#define MAILBOX_OTHER_INFO1 MAILBOX_CRYPTO_INFO1
#define MAILBOX_OTHER_INFO2 MAILBOX_CRYPTO_INFO2
#define MAILBOX_OTHER_INFO3 MAILBOX_CRYPTO_INFO3
#define MAILBOX_OTHER_INFO4 MAILBOX_CRYPTO_INFO4
#define MAILBOX_OTHER_INFO5 MAILBOX_CRYPTO_INFO5
#define MAILBOX_OTHER_INFO6 MAILBOX_CRYPTO_INFO6
#define MAILBOX_OTHER_INFO7 MAILBOX_CRYPTO_INFO7
#define MAILBOX_OTHER_INT MAILBOX_CRYPTO_INT
#define MAILBOX_OTHER_INTEN MAILBOX_CRYPTO_INTEN
#endif
#define MAILBOX_IRQ_BIT_MASK 0x1F
#define MAILBOX_Control_IRQ_NUMBER 0
#define MAILBOX_Firmware_IRQ_NUMBER 1
#define MAILBOX_Efuse_IRQ_NUMBER 2
#define MAILBOX_eRPMC_IRQ_NUMBER 3
#define MAILBOX_SecretKey_IRQ_NUMBER 4
#define MAILBOX_Crypto_IRQ_NUMBER 5
#define MAILBOX_SecureStorage_IRQ_NUMBER 6

#define MAILBOX_IRQ_BIT(irq_number) (1 << (irq_number&0x1F))

#define MAILBOX_SET_IRQ(irq_number) {MAILBOX_SELF_INT |= MAILBOX_IRQ_BIT(irq_number); };
#define MAILBOX_WAIT_IRQ(cmd,irq_number) {\
uint32_t irq_bit =  MAILBOX_IRQ_BIT(irq_number);\
while(!((MAILBOX_OTHER_INT & irq_bit) && (MAILBOX_OTHER_CMD == cmd)));\
};
#define MAILBOX_CLEAR_IRQ(irq_number) {\
do\
{\
MAILBOX_OTHER_INT = MAILBOX_IRQ_BIT(irq_number);\
nop;\
}\
while(MAILBOX_OTHER_INT&MAILBOX_IRQ_BIT(irq_number)); \
};

#define MAILBOX_CMD_CRYPTO_SELFCHECK 0x1//子系统自检
// #define MAILBOX_CMD_SECURE_STORAGE 0x2//安全区域设置
#define MAILBOX_CMD_FIRMWARE_EXTENSION 0x3//固件扩展
// #define MAILBOX_CMD_FIRMWARE_EXTENSION 0x01000000//固件扩展
#define MAILBOX_CMD_APB_RESOURCE_ALLOC 0x4//APB资源分配
#define MAILBOX_CMD_READ_FLASHUID 0x5//读EFLASHID
#define MAILBOX_CMD_FREQ_SYNC 0x6//升降频同步
#define MAILBOX_CMD_FLASH_DUAL_QUAD_SWITCH 0x7//FLASH二四线切换
#define MAILBOX_CMD_READ_FLASHUUID 0x8//读FLASHUUID
#define MAILBOX_CMD_SECURE_BOOT 0x9//安全启动
#define MAILBOX_CMD_FLASH_ENTERLOWPOWER 0xA
#define MAILBOX_CMD_FLASH_EXITLOWPOWER  0xC
#define MAILBOX_CMD_FIRMWARE_MIRROR 0x10//固件更新
#define MAILBOX_CMD_PAGE_DATA_CHANGE 0x11//固件页数据更改
#define MAILBOX_CMD_READ_EFUSE 0x20//读EFUSE数据
#define MAILBOX_CMD_WRITE_ROOTKEY 0x30//RPMC写根密钥
#define MAILBOX_CMD_UPDATE_HMACKEY 0x31//RPMC更新HMAC密钥
#define MAILBOX_CMD_INCREMENT_COUNTER 0x32//RPMC增加单调计数器
#define MAILBOX_CMD_REQUEST_COUNTER 0x33//RPMC请求单调计数器
#define MAILBOX_CMD_READ_PARAMETER 0x34//RPMC读参数
// #define MAILBOX_CMD 0x40//密钥管理
// #define MAILBOX_CMD 0x100//加解密算法
// #define MAILBOX_CMD//安全存储
// #define MAILBOX_CMD//联想文档GetKey等5个命令
// #define MAILBOX_CMD//ROM PATCH

/*APB 共享模块使用情况定义*/
#define APB_SMBUS0 BIT(0)
#define APB_SMBUS1 BIT(1)
#define APB_SMBUS2 BIT(2)
#define APB_SMBUS3 BIT(3)
#define APB_CEC0 BIT(4)
#define APB_CEC1 BIT(5)
#define APB_SMBUS4 BIT(6)
#define APB_SMBUS5 BIT(7)
#define APB_ADC BIT(8)
#define APB_PWM BIT(9)
#define APB_SMBUS6 BIT(10)
#define APB_SMBUS7 BIT(11)
#define APB_UARTA BIT(12)
#define APB_UARTB BIT(13)
#define APB_UART0 BIT(14)
#define APB_UART1 BIT(15)
#define APB_SPIM BIT(16)
#define APB_PECI BIT(17)
#define APB_ERR BIT(29)
#define APB_REL BIT(30)
#define APB_REQ BIT(31)

extern void Mailbox_Init(void);
void Mailbox_Control();
void Mailbox_Firmware();
void Mailbox_Efuse();
void Mailbox_eRPMC();
void Mailbox_SecretKey();
void Mailbox_Crypto();
void Mailbox_SecureStorage();
extern void Mailbox_C2E_Service(void);
extern void Mailbox_Read_FLASHID_Trigger(void);
extern void Mailbox_Read_EFUSE_Trigger(void);
extern void Mailbox_FW_Extension_Trigger(void);
extern void Mailbox_APB2_Source_Alloc_Trigger(void *param);
extern void Mailbox_Read_FLASHUID_Trigger(void);
extern void Mailbox_ExecuteFirmwareUpdate(void *param);
extern void Mailbox_WriteRootKey_Trigger(void);
extern void Mailbox_UpdateHMACKey_Trigger(void);
extern void Mailbox_IncrementCounter_Trigger(void);
extern void Mailbox_RequestCounter_Trigger(void);
extern void Mailbox_ReadParameter_Trigger(void);
extern void Mailbox_eRPMC_Trigger(void);
extern void Process_Tasks(void);
extern Task *Add_Task(TaskFunction function, TaskParams params, Task **head);
extern void Mailbox_Cryp_Selfcheck(void *param);
extern void Mailbox_SetClockFrequency(void *param);
void Transfer_IntFlashToExtFlash(uint32_t destAddr, uint32_t srcAddr, size_t dataSize);
void Transfer_ExtFlashToIntFlash(uint32_t destAddr, uint32_t srcAddr, size_t dataSize);
extern void Service_Process_Tasks(void);
extern void Service_Mailbox(void);
#endif