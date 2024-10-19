#include "KERNEL_MAILBOX.H"

void mailbox_init(void)
{
    E2CINTEN |= 0xFFFFFFFF; // 打开主系统到子系统32个mailbox中断使能
}

void Mailbox_Test(void)
{
    E2CINFO0 = 0x10;                              // 命令字
    E2CINFO1 = ((DWORD)(0x3 << 24) | (4 * 1024)); // BYTE3:固件位置标志 BYTE0~2:固件大小
    E2CINFO2 = 0x60000;                           // 更新起始地址
    E2CINT = 0x2;                                 // 触发对应中断
}

void Mailbox_FW_Extension_Trigger(void)
{
    E2CINFO0 = 0x3;       // 命令字
    E2CINFO1 = 0x1070800; // 扩展固件信息
    E2CINT = 0x1;         // 触发子系统中断
}

void Mailbox_Read_EFUSE_Trigger(void)
{
    E2CINFO0 = 0x20; // 命令字
    E2CINT = 0x4;    // 触发子系统中断
}

void Mailbox_Read_FLASHID_Trigger(void)
{
    E2CINFO0 = 0x5; // 命令字
    E2CINT = 0x1;   // 触发子系统中断
}

void Mailbox_Read_FLASHUID_Trigger(void)
{
    dprint("Read_FLASHUID_Trigger\n");
    E2CINFO0 = 0x8; // 命令字
    E2CINT = 0x1;   // 触发子系统中断
}

void Mailbox_APB2_Source_Alloc_Trigger(void)
{
    E2CINFO0 = 0x4; // 命令字
    // APB2资源分配，0表示资源为主系统使用，1表示资源为子系统使用
    // bit18:PECI   bit17:SPIM  bit16:PWM
    // bit15:ADC    bit14:CEC1  bit13:CEC0  bit12:SMBUS8    bit11:SMBUS7    bit10:SMBUS6    bit9:SMBUS5 bit8:SMBUS4
    // bit7:SMBUS3  bit6:SMBUS2 bit5:SMBUS1 bit4:SMBUS0     bit3:UARTB      bit2:UARTA      bit1:UART1  bit0:UART0
    E2CINFO1 = 0x2; // 主/子系统APB2资源分配
    E2CINT = 0x1;   // 触发子系统中断
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
    if (eRPMC_Busy_Status == 1)
    {
        printf("RPMC Device is in busy status\n");
        // externed status  busy
        // eRPMC_WriteRootKey_Response(); 此处不一定是该命令对应的extenedstatus，后续最好是做成统一的状态回复
        return;
    }
    // WriteRootKey命令  往SRAM的0x31800后填入对应eRPMC数据
    *((VDWORD *)0x31800) = 0xaf73d623;
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

    E2CINFO0 = 0x30;       // 命令字
    E2CINFO1 = 0x0000009B; // WriteRootKey模拟测试
    E2CINT = 0x8;          // 触发子系统中断
    eRPMC_Busy_Status = 1;
}

void Mailbox_UpdateHMACKey_Trigger(void)
{

    if (eRPMC_Busy_Status == 1)
    {
        printf("RPMC Device is in busy status\n");
        // externed status  busy
        // eRPMC_UpdateHMACKey_Response(); 此处不一定是该命令对应的extenedstatus，后续最好是做成统一的状态回复
        return;
    }
    // UpdateHMACKey命令  往SRAM的0x31800后填入对应eRPMC数据
    *((VDWORD *)0x31800) = 0x2df7cf5e;
    *((VDWORD *)0x31804) = 0xa107e1ea;
    *((VDWORD *)0x31808) = 0x5ad59fc8;
    *((VDWORD *)0x3180C) = 0xb9ef975f;
    *((VDWORD *)0x31810) = 0x8bc12b2b;
    *((VDWORD *)0x31814) = 0x788ca1d3;
    *((VDWORD *)0x31818) = 0x382b9006;
    *((VDWORD *)0x3181C) = 0xfa806034;

    // KeyData
    *((VDWORD *)0x31820) = 0x01020304;

    E2CINFO0 = 0x31;       // 命令字
    E2CINFO1 = 0x0000019B; // UpdateHMACKey模拟测试
    E2CINT = 0x8;          // 触发子系统中断
    eRPMC_Busy_Status = 1;
}

void Mailbox_IncrementCounter_Trigger(uint32_t CountData)
{

    if (eRPMC_Busy_Status == 1)
    {
        printf("RPMC Device is in busy status\n");
        // externed status  busy
        // eRPMC_UpdateHMACKey_Response(); 此处不一定是该命令对应的extenedstatus，后续最好是做成统一的状态回复
        return;
    }
    // IncrementCounter命令  往SRAM的0x31800后填入对应eRPMC数据
    // *((VDWORD *)0x31800) = 0xb5db4d28;
    // *((VDWORD *)0x31804) = 0x32d6cad1;
    // *((VDWORD *)0x31808) = 0xda8b892b;
    // *((VDWORD *)0x3180C) = 0x4907d132;
    // *((VDWORD *)0x31810) = 0xe80d47a3;
    // *((VDWORD *)0x31814) = 0xd272db0c;
    // *((VDWORD *)0x31818) = 0x1d44ec15;
    // *((VDWORD *)0x3181C) = 0x7c19eb1f;
    *((VDWORD *)0x31800) = 0x17324225;
    *((VDWORD *)0x31804) = 0x5e5ad01e;
    *((VDWORD *)0x31808) = 0xf49dcecf;
    *((VDWORD *)0x3180C) = 0xb2e9c715;
    *((VDWORD *)0x31810) = 0xa065a36f;
    *((VDWORD *)0x31814) = 0xe2efd976;
    *((VDWORD *)0x31818) = 0x331d7d0a;
    *((VDWORD *)0x3181C) = 0x215d3cfc;

    // CounterData
    *((VDWORD *)0x31820) = CountData;

    E2CINFO0 = 0x32;       // 命令字
    E2CINFO1 = 0x0000029B; // IncrementCounter模拟测试
    E2CINT = 0x8;          // 触发子系统中断
    eRPMC_Busy_Status = 1;
}

void Mailbox_RequestCounter_Trigger(void)
{

    if (eRPMC_Busy_Status == 1)
    {
        printf("RPMC Device is in busy status\n");
        // externed status  busy
        // eRPMC_UpdateHMACKey_Response(); 此处不一定是该命令对应的extenedstatus，后续最好是做成统一的状态回复
        return;
    }
    // RequestCounter命令  往SRAM的0x31800后填入对应eRPMC数据
    *((VDWORD *)0x31800) = 0xc4acbe62;
    *((VDWORD *)0x31804) = 0xfadc266a;
    *((VDWORD *)0x31808) = 0xcf62f1d2;
    *((VDWORD *)0x3180C) = 0xd78281a0;
    *((VDWORD *)0x31810) = 0x4bf94d12;
    *((VDWORD *)0x31814) = 0x9cdaff80;
    *((VDWORD *)0x31818) = 0x3fee8fd0;
    *((VDWORD *)0x3181C) = 0x0d1d587c;

    // Tag
    *((VDWORD *)0x31820) = 0x00000001;
    *((VDWORD *)0x31824) = 0x00000000;
    *((VDWORD *)0x31828) = 0x00000000;

    E2CINFO0 = 0x33;       // 命令字
    E2CINFO1 = 0x0000039B; // RequestCounter模拟测试
    E2CINT = 0x8;          // 触发子系统中断
    eRPMC_Busy_Status = 1;
}

void Mailbox_eRPMC_Trigger(void)
{
#if 0
    if (eRPMC_OPCode == OP1_Code)
    {
        if (eRPMC_CMD == Cmd_WriteRootKey)
        {
            E2CINFO0 = 0x30; // 命令字
            E2CINFO1 = (eSPI_OOB_WriteRootKet_MESSAGE1.Rsvd  << 24) + (eSPI_OOB_WriteRootKet_MESSAGE1.Counter_Addr << 16) + (eSPI_OOB_WriteRootKet_MESSAGE1.Cmd_Type << 8) + eSPI_OOB_WriteRootKet_MESSAGE1.Opcode;
        }
        else if (eRPMC_CMD == Cmd_UpdateHmacKey)
        {
            E2CINFO0 = 0x31; // 命令字
            E2CINFO1 = (eSPI_OOB_WriteRootKet_MESSAGE1.Rsvd  << 24) + (eSPI_OOB_UpdateHMACKey.Counter_Addr << 16) + (eSPI_OOB_UpdateHMACKey.Cmd_Type << 8) + eSPI_OOB_UpdateHMACKey.Opcode;
        }
        else if (eRPMC_CMD == Cmd_IncrementMonotonicCounter)
        {
            E2CINFO0 = 0x32; // 命令字
            E2CINFO1 = (eSPI_OOB_WriteRootKet_MESSAGE1.Rsvd  << 24) + (eSPI_OOB_IncrementCounter.Counter_Addr << 16) + (eSPI_OOB_IncrementCounter.Cmd_Type << 8) + eSPI_OOB_IncrementCounter.Opcode;
        }
        else if (eRPMC_CMD == Cmd_RequestMonotonicCounter)
        {
            E2CINFO0 = 0x33; // 命令字
            E2CINFO1 = (eSPI_OOB_WriteRootKet_MESSAGE1.Rsvd  << 24) + (eSPI_OOB_RequestCounter.Counter_Addr << 16) + (eSPI_OOB_RequestCounter.Cmd_Type << 8) + eSPI_OOB_RequestCounter.Opcode;
        }
    }
    else if (eRPMC_OPCode == OP2_Code)
    {
        E2CINFO0 = 0x34; // 命令字
        E2CINFO1 = eSPI_OOB_ReadParameters.Opcode;
    }
    else
    {
        printf("parameter error\n");
    }
#endif

    printf("erpmc trigger\n");

#if 0
    //WriteRootKey命令  往SRAM的0x31800后填入对应eRPMC数据
    *((VDWORD *)0x31800) = 0xaf73d623;
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
    *((VDWORD *)0x31800) = 0xaf73d623;
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

    E2CINFO0 = 0x30;       // 命令字
    E2CINFO1 = 0x0000009B; // WriteRootKey模拟测试
    E2CINT = 0x8;          // 触发子系统中断
}
/*************************************eRPMC Mailbox***************************************/
void Mailbox_Control(void)
{
    DWORD APB_cryptoModulex_temp=0;
    if (C2E_CMD == 0x3)
    {
        /* 固件扩展结果反馈 */
        if ((BYTE)(C2EINFO1 & 0xff) == 0x1)
            printf("固件扩展完毕\n");
        else if ((BYTE)(C2EINFO1 & 0xff) == 0x2)
            printf("固件扩展失败\n");
    }
    else if (C2E_CMD == 0x4)
    {
        /* APB2资源分配结果反馈 */
        APB_cryptoModulex_temp=C2EINFO1;
        APB_ShareMod_Cry=C2EINFO2;
        if(APB_cryptoModulex_temp&0x80000000)//强制申请使用权限
        {
            APB_ShareMod_Cry=APB_cryptoModulex_temp&0x7fffffff;
            E2CINFO1=0x1;
            E2CINFO2=APB_ShareMod_Cry;
            E2CINFO0 = 0x4; // 命令字
            E2CINT = 0x1;   // 触发子系统中断
            dprint("force access success cryptoModulex:%x\n",APB_cryptoModulex_temp);
            return;
        }
        printf("APB_cryptoModulex_temp:0x%x APB_ShareMod_Cry:0x%x\n",APB_cryptoModulex_temp,APB_ShareMod_Cry);
        if(APB_cryptoModulex_temp>APB_ShareMod_Cry)//使用申请
        {
            APB_cryptoModulex_temp^=APB_ShareMod_Cry;
            if(APB_cryptoModulex_temp&APB_ShareMod_EC)//冲突 拒绝申请
            {
                E2CINFO1=0x2;//申请拒绝
                E2CINFO2=APB_ShareMod_Cry;
                E2CINFO0 = 0x4; // 命令字
                E2CINT = 0x1;   // 触发子系统中断
                dprint("access conflict cryptoModulex:%x\n",APB_cryptoModulex_temp);
            }
            else//申请成功
            {
                APB_ShareMod_Cry|=APB_cryptoModulex_temp;
                E2CINFO1=0x1;//申请允许
                E2CINFO2=APB_ShareMod_Cry;
                E2CINFO0 = 0x4; // 命令字
                E2CINT = 0x1;   // 触发子系统中断
                dprint("access success cryptoModulex:%x\n",APB_cryptoModulex_temp);
            }
        }
        else//使用释放
        {
            APB_cryptoModulex_temp^=APB_ShareMod_Cry;
            APB_ShareMod_Cry&=~APB_cryptoModulex_temp;
            E2CINFO1=0x1;//释放成功
            E2CINFO2=APB_ShareMod_Cry;
            E2CINFO0 = 0x4; // 命令字
            E2CINT = 0x1;   // 触发子系统中断
            dprint("release success cryptoModulex:%x\n",APB_cryptoModulex_temp);
        }
    }
    else if (C2E_CMD == 0x5)
    {
        /* 读取内部FLASH ID */
        if ((BYTE)(C2EINFO1 & 0xff) == 0x1)
            printf("flash id:%x\n", (DWORD)(C2EINFO1 >> 8));
        else if ((BYTE)(C2EINFO1 & 0xff) == 0x2)
            printf("read flash failed\n");
    }
    else if (C2E_CMD == 0x6)
    {
        /* 响应子系统降频 */
        SYSCTL_CLKDIV_OSC80M = 1; // 配置内部时钟分频为2分频，降频到48M
        eFlash_Forbid_Flag = 1;   // 降频到48MHz后，设置eFlash禁止主系统访问标志
    }
    else if (C2E_CMD == 0x8)
    {
        /* 读取内部FLASH ID */
        if ((BYTE)(C2EINFO1 & 0x1) == 0x1)
            printf("flash 9fcmd return id:0x%x 0x%x 0x%x 0x%x\n", C2EINFO5, C2EINFO4, C2EINFO3, C2EINFO2);
        else if ((BYTE)(C2EINFO1 & 0x2) == 0x2)
            printf("read flash failed\n");
    }
}

void Mailbox_Firmware(void)
{

    if (C2EINFO1 == 0x2)
    {
        printf("更新失败，请检查参数\n");
        return;
    }
    if (C2E_CMD == 0x10)
    {
        /* code */
        printf("c");
        // Mailbox_Update_Function((DWORD)(E2CINFO1 >> 24), (DWORD)((E2CINFO1 << 8) >> 8), 0x70800);
    }
    else if (C2E_CMD == 0x11)
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
    if (C2E_CMD == 0x20)
    {
        printf("efuse:%x,%x,%x,%x,%x,%x,%x\n", C2EINFO1, C2EINFO2, C2EINFO3, C2EINFO4, C2EINFO5, C2EINFO6, C2EINFO7);
    }
}

void Mailbox_eRPMC(void)
{
    if (C2E_CMD == 0x30)
        eRPMC_WriteRootKey_Response();
    else if (C2E_CMD == 0x31)
        eRPMC_UpdateHMACKey_Response();
    else if (C2E_CMD == 0x32)
        eRPMC_IncrementCounter_Response();
    else if (C2E_CMD == 0x33)
        eRPMC_RequestCounter_Response();
    else if (C2E_CMD == 0x34)
        eRPMC_ReadParameter_Response();
}

void Mailbox_SecretKey(void)
{
}

void Mailbox_Crypto(void)
{
}

void Mailbox_SecureStorage(void)
{
}

void Mailbox_E2C_Service(void)
{
}

void Mailbox_C2E_Service(void)
{
    if (Mailbox_Int_Store == 0)
        return;

    switch (Mailbox_Int_Store)
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
    Mailbox_Int_Store = 0;
}