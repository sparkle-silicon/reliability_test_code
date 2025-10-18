#include "KERNEL_MAILBOX.H"
volatile bool command_processed = true; // 用于标志子系统是否处理完命令
Task *task_head = NULL;                 // 任务链表头
// 添加任务到链表
Task *Add_Task(TaskFunction function, TaskParams params, Task **head)
{
    Task *new_task = malloc(sizeof(Task));
    if(new_task == NULL)
    {
        return NULL;
    }
    new_task->function = function;
    new_task->params = params;
    new_task->next = NULL; // 新任务的下一个指针为 NULL

    if(*head == NULL) // 链表为空，则新任务为头节点
    {
        *head = new_task;
    }
    else // 链表不为空,尾插入
    {
        Task *p = *head;
        while(p->next != NULL)
        {
            p = p->next;
        }
        p->next = new_task;
    }
    return *head; // 返回更新后的头指针
}

// 处理任务队列中的任务
void Process_Tasks(void)
{
    if(task_head != NULL)
    {
        if(command_processed == false)
            return;
        Task *task = task_head;
        task_head = task_head->next;   // 移动到下一个任务
        task->function(&task->params); // 执行任务函数，传递参数
        free(task);                    // 处理完成，释放任务内存
    }
}

void Mailbox_Init(void)
{
    MAILBOX_SELF_CMD =
        MAILBOX_SELF_INFO1 =
        MAILBOX_SELF_INFO2 =
        MAILBOX_SELF_INFO3 =
        MAILBOX_SELF_INFO4 =
        MAILBOX_SELF_INFO5 =
        MAILBOX_SELF_INFO6 =
        MAILBOX_SELF_INFO7 =
        MAILBOX_SELF_INT =
        MAILBOX_SELF_INTEN = 0x00000000;
    MAILBOX_SELF_INTEN = 0xffffffff; // 使能所有中断(后续改为有轮询效中断)
    do
    {
        MAILBOX_OTHER_INT = MAILBOX_OTHER_INT; // 清除所有中断
        nop;
    }
    while(MAILBOX_OTHER_INT);
}

void Mailbox_ExecuteFirmwareUpdate(void *param)
{
    TaskParams *params = (TaskParams *)param;
    MAILBOX_SELF_CMD = MAILBOX_CMD_FIRMWARE_MIRROR;                              // 命令字
    MAILBOX_SELF_INFO1 = params->E2C_INFO1;                 // 固件更新模式
    MAILBOX_SELF_INFO2 = params->E2C_INFO2;                 // 更新大小
    MAILBOX_SELF_INFO3 = params->E2C_INFO3;                 // 目标地址
    MAILBOX_SELF_INFO4 = params->E2C_INFO4;                 // 起始地址
    if(((BYTE)MAILBOX_SELF_INFO1 == 0x1) || ((BYTE)MAILBOX_SELF_INFO1 == 0x2))// 配置外部flash复用功能
    {
        sysctl_iomux_config(GPIOB, 17, 0x1);//wp
        sysctl_iomux_config(GPIOB, 20, 0x1);//mosi
        sysctl_iomux_config(GPIOB, 21, 0x1);//miso
        sysctl_iomux_config(GPIOB, 22, 0x1);//cs0
        sysctl_iomux_config(GPIOB, 23, 0x1);//clk
        sysctl_iomux_config(GPIOB, 30, 0x1);//hold
        if(SYSCTL_RST1 & 0x00000100)
        {
            SYSCTL_RST1 &= 0xfffffeff; // 释放复位
        }
        if(SPIFE_STA & BIT4)               // 0:2线，1:4线
        {
            SPIFE_CTL0 |= BIT1; // 成功则使用4线模式
        }
        else
        {
            SPIFE_CTL0 &= ~BIT1; // 失败使用二线模式
        }
    }
    MAILBOX_SET_IRQ(MAILBOX_Firmware_IRQ_NUMBER);                                 // 触发对应中断
    command_processed = false;
}

void Mailbox_FW_Extension_Trigger(void)
{
    MAILBOX_SELF_CMD = MAILBOX_CMD_FIRMWARE_EXTENSION;       // 命令字
    MAILBOX_SELF_INFO1 = 0x1070800; // 扩展固件信息
    MAILBOX_SET_IRQ(MAILBOX_Control_IRQ_NUMBER);         // 触发子系统中断
    command_processed = false;
}

void Mailbox_Read_EFUSE_Trigger(void)
{
    MAILBOX_SELF_CMD = MAILBOX_CMD_READ_EFUSE; // 命令字
    MAILBOX_SET_IRQ(MAILBOX_Efuse_IRQ_NUMBER);    // 触发子系统中断
    command_processed = false;
}

void Mailbox_Read_FLASHID_Trigger(void)
{
    MAILBOX_SELF_CMD = MAILBOX_CMD_READ_FLASHUID; // 命令字
    MAILBOX_SET_IRQ(MAILBOX_Control_IRQ_NUMBER);   // 触发子系统中断
    command_processed = false;
}

void Mailbox_Read_FLASHUID_Trigger(void)
{
    dprint("Read_FLASHUID_Trigger\n");
    MAILBOX_SELF_CMD = MAILBOX_CMD_READ_FLASHUUID; // 命令字
    MAILBOX_SET_IRQ(MAILBOX_Control_IRQ_NUMBER);   // 触发子系统中断
    command_processed = false;
}

void Mailbox_APB2_Source_Alloc_Trigger(void *param)
{
    TaskParams *params = (TaskParams *)param;
    MAILBOX_SELF_CMD = MAILBOX_CMD_APB_RESOURCE_ALLOC; // 命令字
    MAILBOX_SELF_INFO1 = params->E2C_INFO1;
    MAILBOX_SET_IRQ(MAILBOX_Control_IRQ_NUMBER); // 触发子系统中断
    command_processed = false;
}

void Mailbox_Cryp_Selfcheck(void *param)
{
    TaskParams *params = (TaskParams *)param;
    MAILBOX_SELF_INFO1 = params->E2C_INFO1;
    MAILBOX_SELF_CMD = MAILBOX_CMD_CRYPTO_SELFCHECK;
    MAILBOX_SET_IRQ(MAILBOX_Control_IRQ_NUMBER);
    command_processed = false;
}

void Mailbox_SetClockFrequency(void *param)
{
    TaskParams *params = (TaskParams *)param;
    MAILBOX_SELF_INFO1 = params->E2C_INFO1;//通知子系统设置多少分频
    MAILBOX_SELF_CMD = MAILBOX_CMD_FREQ_SYNC;
    MAILBOX_SET_IRQ(MAILBOX_Control_IRQ_NUMBER);
    command_processed = false;
}

extern void Service_Process_Tasks(void);
extern void Service_Mailbox(void);
void AwaitCrypSelfcheck(void)
{
    TaskParams Params;
    task_head = Add_Task((TaskFunction)Mailbox_Read_EFUSE_Trigger, Params, &task_head);//安全使能是否打开
    Mailbox_Init();
    while(EFUSE_Avail == 0)
    {
        Service_Process_Tasks();
        Service_Mailbox();
    }
    EFUSE_Avail = 0;
    if((MAILBOX_OTHER_INFO1 & BIT3) != 0)//crypto need selfcheck
    {
        Params.E2C_INFO1 = 0x0;
        task_head = Add_Task(Mailbox_Cryp_Selfcheck, Params, &task_head);//子系统自检命令触发
        while(Cry_SelfCheck_Flag != 0x1)//等待子系统自检完成
        {
            Service_Process_Tasks();
            Service_Mailbox();
            if(Cry_SelfCheck_Flag & 0x2)//crypto selfcheck err
            {
                if(CRYP_SHA_MODULES_SC_STA != 0)//SHA模块自检失败
                {
                    printf("SHA Module Status: %d\n", CRYP_SHA_MODULES_SC_STA);
                }
                if(CRYP_AES_MODULLES_SC_STA != 0)
                {
                    printf("AES Module Status: %d\n", CRYP_AES_MODULLES_SC_STA);
                }
                if(CRYP_MODULLES_RSA_SC_STA != 0)
                {
                    printf("RSA Module Status: %d\n", CRYP_MODULLES_RSA_SC_STA);
                }
                if(CRYP_MODULLES_ECC_SC_STA != 0)
                {
                    printf("ECC Module Status: %d\n", CRYP_MODULLES_ECC_SC_STA);
                }
            }
        }
    }

}

void Transfer_IntFlashToExtFlash(uint32_t destAddr, uint32_t srcAddr, size_t dataSize)
{
    TaskParams Params;
    Params.E2C_INFO1 = 0x1;//update mode
    Params.E2C_INFO2 = dataSize;
    Params.E2C_INFO3 = destAddr;
    Params.E2C_INFO4 = srcAddr;
    Add_Task(Mailbox_ExecuteFirmwareUpdate, Params, &task_head);
}

void Transfer_ExtFlashToIntFlash(uint32_t destAddr, uint32_t srcAddr, size_t dataSize)
{
    TaskParams Params;
    Params.E2C_INFO1 = 0x2;//update mode
    Params.E2C_INFO2 = dataSize;
    Params.E2C_INFO3 = destAddr;
    Params.E2C_INFO4 = srcAddr;
    Add_Task(Mailbox_ExecuteFirmwareUpdate, Params, &task_head);
}
/*************************************eRPMC Mailbox***************************************/
#define OP1_Code 0x9B
#define OP2_Code 0x96

#define Cmd_WriteRootKey 0x00
#define Cmd_UpdateHmacKey 0x01
#define Cmd_IncrementMonotonicCounter 0x02
#define Cmd_RequestMonotonicCounter 0x03

#define Counter00 0x0
#define Counter01 0x1
#define Counter02 0x2
#define Counter03 0x3

#define Reserved_Value 0x00
#define Dummy_Value 0x00

BYTE eRPMC_Busy_Status = 0;

// 模拟设置Rootkey,将RootKey填入
// void Set_RootKey(void)
// {
//     // 设置root key，由于HMAC为大端存储，因此Root_Key实际为Root_Key[0] = 0x00,Root_Key[31] = 0x1F，即0x1f1e...0100
//     for (BYTE i = 0; i < 32; i++)
//     {
//         Root_Key[i] = 31 - i;
//     }
// }

void Mailbox_WriteRootKey_Trigger(void)
{
    if(eRPMC_Busy_Status == 1)
    {
        printf("RPMC Device is in busy status\n");
        eRPMC_WriteRootKey_data.Extended_Status = 0x04;
        eSPI_OOBSend((BYTE *)&eRPMC_WriteRootKey_data);
        return;
    }
    // WriteRootKey命令  往SRAM的0x31800后填入对应eRPMC数据
    // 定义目标地址
    uint32_t *pTarget = (uint32_t *)0x3181C;
    uint32_t data = 0;
    // 将 Root_Key 搬运到目标地址
    for(int i = 0; i < 32; i += 4)
    {
        // 将 Root_Key 的四个字节组合成一个 uint32_t
        data = (uint32_t)eRPMC_WriteRootKey_m1.Root_Key[i + 3] << 24
            | (uint32_t)eRPMC_WriteRootKey_m1.Root_Key[i + 2] << 16
            | (uint32_t)eRPMC_WriteRootKey_m1.Root_Key[i + 1] << 8
            | (uint32_t)eRPMC_WriteRootKey_m1.Root_Key[i];
            // 将组合后的数据存储到目标地址
        pTarget[i / 4] = data;
    }

    //TruncatedSignature
    pTarget = (uint32_t *)0x31800;
    for(int i = 0; i < 28; i += 4)
    {
        if(i > 23)
        {
            data = (uint32_t)eRPMC_WriteRootKey_m2.TruncatedSignature0_1[1] << 24
                | (uint32_t)eRPMC_WriteRootKey_m2.TruncatedSignature0_1[0] << 16
                | (uint32_t)eRPMC_WriteRootKey_m1.TruncatedSignature2_27[i + 1] << 8
                | (uint32_t)eRPMC_WriteRootKey_m1.TruncatedSignature2_27[i];
        }
        else
        {
            data = (uint32_t)eRPMC_WriteRootKey_m1.TruncatedSignature2_27[i + 3] << 24
                | (uint32_t)eRPMC_WriteRootKey_m1.TruncatedSignature2_27[i + 2] << 16
                | (uint32_t)eRPMC_WriteRootKey_m1.TruncatedSignature2_27[i + 1] << 8
                | (uint32_t)eRPMC_WriteRootKey_m1.TruncatedSignature2_27[i];
        }
        // 将组合后的数据存储到目标地址
        pTarget[i / 4] = data;
    }

    MAILBOX_SELF_CMD = MAILBOX_CMD_WRITE_ROOTKEY;       // 命令字
    MAILBOX_SELF_INFO1 = eRPMC_WriteRootKey_m1.Opcode
        | (eRPMC_WriteRootKey_m1.Cmd_Type << 8)
        | (eRPMC_WriteRootKey_m1.Counter_Addr << 16)
        | (eRPMC_WriteRootKey_m1.Rsvd << 24);
    MAILBOX_SET_IRQ(MAILBOX_eRPMC_IRQ_NUMBER);          // 触发子系统中断
    command_processed = false;
    eRPMC_Busy_Status = 1;
}

void Mailbox_UpdateHMACKey_Trigger(void)
{
    if(eRPMC_Busy_Status == 1)
    {
        printf("RPMC Device is in busy status\n");
        eRPMC_UpdateHMACKey_data.Extended_Status = 0x04;
        eSPI_OOBSend((BYTE *)&eRPMC_UpdateHMACKey_data);
        return;
    }
    // UpdateHMACKey命令  往SRAM的0x31800后填入对应eRPMC数据
    // 定义目标地址
    uint32_t *pTarget = (uint32_t *)0x31800;
    uint32_t data = 0;
    // 将 Signature 搬运到目标地址
    for(int i = 0; i < 32; i += 4)
    {
        // 将 Root_Key 的四个字节组合成一个 uint32_t
        data = (uint32_t)eRPMC_UpdateHMACKey.Signature[i + 3] << 24
            | (uint32_t)eRPMC_UpdateHMACKey.Signature[i + 2] << 16
            | (uint32_t)eRPMC_UpdateHMACKey.Signature[i + 1] << 8
            | (uint32_t)eRPMC_UpdateHMACKey.Signature[i];
            // 将组合后的数据存储到目标地址
        pTarget[i / 4] = data;
    }
    // KeyData
    pTarget = (uint32_t *)0x31820;
    for(int i = 0; i < 4; i += 4)
    {
        // 将 Root_Key 的四个字节组合成一个 uint32_t
        data = (uint32_t)eRPMC_UpdateHMACKey.Key_Data[i + 3] << 24
            | (uint32_t)eRPMC_UpdateHMACKey.Key_Data[i + 2] << 16
            | (uint32_t)eRPMC_UpdateHMACKey.Key_Data[i + 1] << 8
            | (uint32_t)eRPMC_UpdateHMACKey.Key_Data[i];
            // 将组合后的数据存储到目标地址
        pTarget[i / 4] = data;
    }

    MAILBOX_SELF_CMD = MAILBOX_CMD_UPDATE_HMACKEY;       // 命令字
    MAILBOX_SELF_INFO1 = eRPMC_UpdateHMACKey.Opcode
        | (eRPMC_UpdateHMACKey.Cmd_Type << 8)
        | (eRPMC_UpdateHMACKey.Counter_Addr << 16)
        | (eRPMC_UpdateHMACKey.Rsvd << 24);
    printf("MAILBOX_E2CINFO1:0x%x\n", MAILBOX_SELF_INFO1);
    MAILBOX_SET_IRQ(MAILBOX_eRPMC_IRQ_NUMBER);           // 触发子系统中断
    command_processed = false;
    eRPMC_Busy_Status = 1;
}

void Mailbox_IncrementCounter_Trigger(void)
{
    if(eRPMC_Busy_Status == 1)
    {
        printf("RPMC Device is in busy status\n");
        eRPMC_IncrementCounter_data.Extended_Status = 0x04;
        eSPI_OOBSend((BYTE *)&eRPMC_IncrementCounter_data);
        return;
    }
    // IncrementCounter命令  往SRAM的0x31800后填入对应eRPMC数据
    uint32_t *pTarget = (uint32_t *)0x31800;
    uint32_t data = 0;
    // 将 Signature 搬运到目标地址
    for(int i = 0; i < 32; i += 4)
    {
        // 将 Root_Key 的四个字节组合成一个 uint32_t
        data = (uint32_t)eRPMC_IncrementCounter.Signature[i + 3] << 24
            | (uint32_t)eRPMC_IncrementCounter.Signature[i + 2] << 16
            | (uint32_t)eRPMC_IncrementCounter.Signature[i + 1] << 8
            | (uint32_t)eRPMC_IncrementCounter.Signature[i];
            // 将组合后的数据存储到目标地址
        pTarget[i / 4] = data;
    }
    // Counter_Data
    pTarget = (uint32_t *)0x31820;
    for(int i = 0; i < 4; i += 4)
    {
        // 将 Root_Key 的四个字节组合成一个 uint32_t
        data = (uint32_t)eRPMC_IncrementCounter.Counter_Data[i + 3] << 24
            | (uint32_t)eRPMC_IncrementCounter.Counter_Data[i + 2] << 16
            | (uint32_t)eRPMC_IncrementCounter.Counter_Data[i + 1] << 8
            | (uint32_t)eRPMC_IncrementCounter.Counter_Data[i];
            // 将组合后的数据存储到目标地址
        pTarget[i / 4] = data;
    }

    MAILBOX_SELF_CMD = MAILBOX_CMD_INCREMENT_COUNTER;       // 命令字
    MAILBOX_SELF_INFO1 = eRPMC_IncrementCounter.Opcode
        | (eRPMC_IncrementCounter.Cmd_Type << 8)
        | (eRPMC_IncrementCounter.Counter_Addr << 16)
        | (eRPMC_IncrementCounter.Rsvd << 24);
    printf("MAILBOX_E2CINFO1:0x%x\n", MAILBOX_SELF_INFO1);
    MAILBOX_SET_IRQ(MAILBOX_eRPMC_IRQ_NUMBER);           // 触发子系统中断
    command_processed = false;
    eRPMC_Busy_Status = 1;
}

void Mailbox_RequestCounter_Trigger(void)
{

    if(eRPMC_Busy_Status == 1)
    {
        printf("RPMC Device is in busy status\n");
        eRPMC_RequestCounter_data.Extended_Status = 0x04;
        eSPI_OOBSend((BYTE *)&eRPMC_RequestCounter_data);
        return;
    }
    // RequestCounter命令  往SRAM的0x31800后填入对应eRPMC数据
    uint32_t *pTarget = (uint32_t *)0x31800;
    uint32_t data = 0;
    // 将 Signature 搬运到目标地址
    for(int i = 0; i < 32; i += 4)
    {
        // 将 Root_Key 的四个字节组合成一个 uint32_t
        data = (uint32_t)eRPMC_RequestCounter.Signature[i + 3] << 24
            | (uint32_t)eRPMC_RequestCounter.Signature[i + 2] << 16
            | (uint32_t)eRPMC_RequestCounter.Signature[i + 1] << 8
            | (uint32_t)eRPMC_RequestCounter.Signature[i];
            // 将组合后的数据存储到目标地址
        pTarget[i / 4] = data;
    }
    // Tag
    pTarget = (uint32_t *)0x31820;
    for(int i = 0; i < 12; i += 4)
    {
        // 将 Root_Key 的四个字节组合成一个 uint32_t
        data = (uint32_t)eRPMC_RequestCounter.Tag_Arr[i + 3] << 24
            | (uint32_t)eRPMC_RequestCounter.Tag_Arr[i + 2] << 16
            | (uint32_t)eRPMC_RequestCounter.Tag_Arr[i + 1] << 8
            | (uint32_t)eRPMC_RequestCounter.Tag_Arr[i];
            // 将组合后的数据存储到目标地址
        pTarget[i / 4] = data;
    }

    MAILBOX_SELF_CMD = MAILBOX_CMD_REQUEST_COUNTER;       // 命令字
    MAILBOX_SELF_INFO1 = eRPMC_RequestCounter.Opcode
        | (eRPMC_RequestCounter.Cmd_Type << 8)
        | (eRPMC_RequestCounter.Counter_Addr << 16)
        | (eRPMC_RequestCounter.Rsvd << 24);
    MAILBOX_SET_IRQ(MAILBOX_eRPMC_IRQ_NUMBER);          // 触发子系统中断
    command_processed = false;
    eRPMC_Busy_Status = 1;
}

void Mailbox_ReadParameter_Trigger(void)
{

    if(eRPMC_Busy_Status == 1)
    {
        printf("RPMC Device is in busy status\n");
        eRPMC_ReadParameters_data.Extended_Status = 0x04;
        eSPI_OOBSend((BYTE *)&eRPMC_ReadParameters_data);
        return;
    }
    // RequestCounter命令  往SRAM的0x31800后填入对应eRPMC数据
    MAILBOX_SELF_CMD = MAILBOX_CMD_REQUEST_COUNTER;       // 命令字
    MAILBOX_SELF_INFO1 = eRPMC_ReadParameters.Opcode;
    MAILBOX_SET_IRQ(MAILBOX_eRPMC_IRQ_NUMBER);          // 触发子系统中断
    command_processed = false;
    eRPMC_Busy_Status = 1;
}

void Mailbox_eRPMC_Trigger(void)
{
#if 0
    if(eRPMC_OPCode == OP1_Code)
    {
        if(eRPMC_CMD == Cmd_WriteRootKey)
        {
            MAILBOX_SELF_CMD = MAILBOX_CMD_WRITE_ROOTKEY; // 命令字
            MAILBOX_SELF_INFO1 = (eSPI_OOB_WriteRootKet_MESSAGE1.Rsvd << 24) + (eSPI_OOB_WriteRootKet_MESSAGE1.Counter_Addr << 16) + (eSPI_OOB_WriteRootKet_MESSAGE1.Cmd_Type << 8) + eSPI_OOB_WriteRootKet_MESSAGE1.Opcode;
        }
        else if(eRPMC_CMD == Cmd_UpdateHmacKey)
        {
            MAILBOX_SELF_CMD = MAILBOX_CMD_UPDATE_HMACKEY; // 命令字
            MAILBOX_SELF_INFO1 = (eSPI_OOB_WriteRootKet_MESSAGE1.Rsvd << 24) + (eSPI_OOB_UpdateHMACKey.Counter_Addr << 16) + (eSPI_OOB_UpdateHMACKey.Cmd_Type << 8) + eSPI_OOB_UpdateHMACKey.Opcode;
        }
        else if(eRPMC_CMD == Cmd_IncrementMonotonicCounter)
        {
            MAILBOX_SELF_CMD = MAILBOX_CMD_INCREMENT_COUNTER; // 命令字
            MAILBOX_SELF_INFO1 = (eSPI_OOB_WriteRootKet_MESSAGE1.Rsvd << 24) + (eSPI_OOB_IncrementCounter.Counter_Addr << 16) + (eSPI_OOB_IncrementCounter.Cmd_Type << 8) + eSPI_OOB_IncrementCounter.Opcode;
        }
        else if(eRPMC_CMD == Cmd_RequestMonotonicCounter)
        {
            MAILBOX_SELF_CMD = MAILBOX_CMD_REQUEST_COUNTER; // 命令字
            MAILBOX_SELF_INFO1 = (eSPI_OOB_WriteRootKet_MESSAGE1.Rsvd << 24) + (eSPI_OOB_RequestCounter.Counter_Addr << 16) + (eSPI_OOB_RequestCounter.Cmd_Type << 8) + eSPI_OOB_RequestCounter.Opcode;
        }
    }
    else if(eRPMC_OPCode == OP2_Code)
    {
        MAILBOX_SELF_CMD = MAILBOX_CMD_READ_PARAMETER; // 命令字
        MAILBOX_SELF_INFO1 = eSPI_OOB_ReadParameters.Opcode;
    }
    else
    {
        printf("parameter error\n");
    }
#endif

    printf("erpmc trigger\n");

#if 0
    //WriteRootKey命令  往SRAM的0x31800后填入对应eRPMC数据
    * ((VDWORD *)0x31800) = 0xaf73d623;
    *((VDWORD *)0x31804) = 0x135e43d5;
    *((VDWORD *)0x31808) = 0x40cbc22e;
    *((VDWORD *)0x3180C) = 0xfe22f42c;
    *((VDWORD *)0x31810) = 0x4abc377d;
    *((VDWORD *)0x31814) = 0xb7d3cfbd;
    *((VDWORD *)0x31818) = 0xb3674211;

    *((VDWORD *)0x3181C) = 0x1c1d1e1f;
    *((VDWORD *)0x31820) = 0x18191a1b;
    *((VDWORD *)0x31824) = 0x14151617;
    *((VDWORD *)0x31828) = 0x10111213;
    *((VDWORD *)0x3182C) = 0x0c0d0e0f;
    *((VDWORD *)0x31830) = 0x08090a0b;
    *((VDWORD *)0x31834) = 0x04050607;
    *((VDWORD *)0x31838) = 0x00010203;
#endif

#if 0
    // UpdateHMACKey命令  往SRAM的0x31800后填入对应eRPMC数据
    * ((VDWORD *)0x31800) = 0xaf73d623;
    *((VDWORD *)0x31804) = 0x135e43d5;
    *((VDWORD *)0x31808) = 0x40cbc22e;
    *((VDWORD *)0x3180C) = 0xfe22f42c;
    *((VDWORD *)0x31810) = 0x4abc377d;
    *((VDWORD *)0x31814) = 0xb7d3cfbd;
    *((VDWORD *)0x31818) = 0xb3674211;

    *((VDWORD *)0x3181C) = 0x1c1d1e1f;
    *((VDWORD *)0x31820) = 0x18191a1b;
    *((VDWORD *)0x31824) = 0x14151617;
    *((VDWORD *)0x31828) = 0x10111213;
    *((VDWORD *)0x3182C) = 0x0c0d0e0f;
    *((VDWORD *)0x31830) = 0x08090a0b;
    *((VDWORD *)0x31834) = 0x04050607;
    *((VDWORD *)0x31838) = 0x00010203;
#endif

    MAILBOX_SELF_CMD = MAILBOX_CMD_WRITE_ROOTKEY;       // 命令字
    MAILBOX_SELF_INFO1 = 0x0000009B; // WriteRootKey模拟测试
    MAILBOX_SET_IRQ(MAILBOX_eRPMC_IRQ_NUMBER);          // 触发子系统中断
    command_processed = false;
}
/*************************************eRPMC Mailbox***************************************/
void Mailbox_Control(void)
{
    if(C2E_CMD == MAILBOX_CMD_CRYPTO_SELFCHECK)
    {
        Cry_SelfCheck_Flag = MAILBOX_OTHER_INFO1;
        /*子系统自检结果反馈*/
        if((BYTE)(Cry_SelfCheck_Flag & 0xff) == 0x1)
            printf("Crypto SelfCheck Pass\n");
        else if((BYTE)(Cry_SelfCheck_Flag & 0xff) == 0x2)
        {
            printf("Crypto SelfCheck error\n");
        }
    }
    else if(C2E_CMD == MAILBOX_CMD_FIRMWARE_EXTENSION)
    {
        /* 固件扩展结果反馈 */
        if((BYTE)(MAILBOX_OTHER_INFO1 & 0xff) == 0x1)
            printf("固件扩展完毕\n");
        else if((BYTE)(MAILBOX_OTHER_INFO1 & 0xff) == 0x2)
            printf("固件扩展失败\n");
    }
    else if(C2E_CMD == MAILBOX_CMD_APB_RESOURCE_ALLOC)
    {
        DWORD APB_ShareMod_temp = MAILBOX_OTHER_INFO1;
        if(APB_ShareMod_temp & APB_ERR) // 子系统返回失败
        {
            APB_ShareMod_temp &= ~APB_ERR;
            APB_ShareMod_Cry = APB_ShareMod_temp;
            printf("request or release err apb_share_mod_cry:0x%x\n", APB_ShareMod_Cry);
        }
        else
        {
            APB_ShareMod_Cry = APB_ShareMod_temp;
            printf("apb_share_mod_cry:0x%x\n", APB_ShareMod_Cry);
        }
    }
    else if(C2E_CMD == MAILBOX_CMD_READ_FLASHUID)
    {
        /* 读取内部FLASH ID */
        if((BYTE)(MAILBOX_OTHER_INFO1 & 0xff) == 0x1)
        {
            Flash_Capacity = (1 << ((MAILBOX_OTHER_INFO1 >> 24) & 0xff));
            printf("flash capacity:%d BYTES\n", Flash_Capacity);
            printf("flash 9f cmd return id:0x%x\n", (MAILBOX_OTHER_INFO1 >> 8));
        }
        else if((BYTE)(MAILBOX_OTHER_INFO1 & 0xff) == 0x2)
            printf("read flash failed\n");
    }
    else if(C2E_CMD == MAILBOX_CMD_FREQ_SYNC)
    {
        /* 响应子系统降频 */
        // SYSCTL_CLKDIV_OSC96M = (CHIP_CLOCK_SWITCH - 1);
        __nop
            Module_init();//暂时保留，后续根据实际情况是否需要调用初始化
        eFlash_Forbid_Flag = 1;   // 降频到48MHz后，设置eFlash禁止主系统访问标志
    }
    else if(C2E_CMD == MAILBOX_CMD_READ_FLASHUUID)
    {
        /* 读取内部FLASH ID */
        if((BYTE)(MAILBOX_OTHER_INFO1 & 0x1) == 0x1)
            printf("flash 9fcmd return id:0x%x 0x%x 0x%x 0x%x\n", MAILBOX_OTHER_INFO5, MAILBOX_OTHER_INFO4, MAILBOX_OTHER_INFO3, MAILBOX_OTHER_INFO2);
        else if((BYTE)(MAILBOX_OTHER_INFO1 & 0x2) == 0x2)
            printf("read flash failed\n");
    }
}

void Mailbox_Firmware(void)
{

    if(C2E_CMD == MAILBOX_CMD_FIRMWARE_MIRROR)
    {
        /* code */
    }
    else if(C2E_CMD == MAILBOX_CMD_PAGE_DATA_CHANGE)
    {
        /* code */
    }
    else
    {
        printf("unknown fw update cmd\n");
    }
}

void Mailbox_Efuse(void)
{
    if(C2E_CMD == MAILBOX_CMD_READ_EFUSE)
    {
        EFUSE_Avail = 1;
        printf("efuse:%x,%x,%x,%x,%x,%x,%x\n", MAILBOX_OTHER_INFO1, MAILBOX_OTHER_INFO2, MAILBOX_OTHER_INFO3, MAILBOX_OTHER_INFO4, MAILBOX_OTHER_INFO5, MAILBOX_OTHER_INFO6, MAILBOX_OTHER_INFO7);
    }
}

void Mailbox_eRPMC(void)
{
    eRPMC_Busy_Status = 0; // 清除rpmc device busy状态
    if(C2E_CMD == MAILBOX_CMD_WRITE_ROOTKEY)
        eRPMC_WriteRootKey_Response();
    else if(C2E_CMD == MAILBOX_CMD_UPDATE_HMACKEY)
        eRPMC_UpdateHMACKey_Response();
    else if(C2E_CMD == MAILBOX_CMD_INCREMENT_COUNTER)
        eRPMC_IncrementCounter_Response();
    else if(C2E_CMD == MAILBOX_CMD_REQUEST_COUNTER)
        eRPMC_RequestCounter_Response();
    else if(C2E_CMD == MAILBOX_CMD_READ_PARAMETER)
        eRPMC_ReadParameter_Response();
    eRPMC_Busy_Status = 0;
}

void Mailbox_SecretKey(void)
{}

void Mailbox_Crypto(void)
{}

void Mailbox_SecureStorage(void)
{}

void Mailbox_E2C_Service(void)
{}

void Mailbox_C2E_Service(void)
{
    if(Mailbox_Int_Store == 0)
        return;

    switch(Mailbox_Int_Store)
    {
        case 0x01:
            Mailbox_Control();
            break;
        case 0x02:
            Mailbox_Firmware();
            break;
        case 0x04:
            Mailbox_Efuse();
            break;
        case 0x08:
            Mailbox_eRPMC();
            break;
        case 0x10:
            Mailbox_SecretKey();
            break;
        case 0x20:
            Mailbox_Crypto();
            break;
        case 0x40:
            Mailbox_SecureStorage();
            break;
        default:
            dprint("Mailbox_Int_Store:%x\n", Mailbox_Int_Store);
            break;
    }
    command_processed = true;
    Mailbox_Int_Store = 0;
}