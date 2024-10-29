/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-11-03 15:19:07
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
typedef void (*TaskFunction)(void *); // 定义任务函数类型，接受参数
// 定义一个结构体来封装多个参数
typedef struct
{
    DWORD E2C_INFO1;
    DWORD E2C_INFO2;
    DWORD E2C_INFO3;
} TaskParams;

typedef struct Task
{
    TaskFunction function; // 任务函数指针
    TaskParams params;     // 任务参数
    struct Task *next;     // 指向下一个任务
} Task;
extern Task *task_head; // 任务链表头

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
#define APB_REL BIT(30)
#define APB_REQ BIT(31)

extern void mailbox_init(void);
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
extern void Mailbox_Test(void);
extern void Mailbox_WriteRootKey_Trigger(void);
extern void Mailbox_UpdateHMACKey_Trigger(void);
extern void Mailbox_IncrementCounter_Trigger(void);
extern void Mailbox_RequestCounter_Trigger(void);
extern void Mailbox_ReadParameter_Trigger(void);
extern void Mailbox_eRPMC_Trigger(void);
extern void Process_Tasks(void);
extern Task *Add_Task(TaskFunction function, TaskParams params, Task **head);
#endif