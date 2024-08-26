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

void Mailbox_Control(void)
{
    if (C2EINFO0 == 0x3)
    {
        /* 固件扩展结果反馈 */
        if ((BYTE)(C2EINFO1 & 0xff) == 0x1)
            printf("固件扩展完毕\n");
        else if ((BYTE)(C2EINFO1 & 0xff) == 0x2)
            printf("固件扩展失败\n");
    }
    else if (C2EINFO0 == 0x4)
    {
        /* APB2资源分配结果反馈 */
        if ((BYTE)(C2EINFO1 & 0xff) == 0x1)
            printf("资源分配成功\n");
        else if ((BYTE)(C2EINFO1 & 0xff) == 0x2)
            printf("资源分配失败\n");
    }
    else if (C2EINFO0 == 0x5)
    {
        /* 读取内部FLASH ID */
        if ((BYTE)(C2EINFO1 & 0xff) == 0x1)
            printf("flash id:%x\n", (DWORD)(C2EINFO1 >> 8));
        else if ((BYTE)(C2EINFO1 & 0xff) == 0x2)
            printf("read flash failed\n");
    }
    else if (C2EINFO0 == 0x6)
    {
        /* 响应子系统降频 */
        SYSCTL_CLKDIV_OSC80M = 1; // 配置内部时钟分频为2分频，降频到48M
        eFlash_Forbid_Flag = 1;   // 降频到48MHz后，设置eFlash禁止主系统访问标志
    }
}

void Mailbox_Firmware(void)
{

    if (C2EINFO1 == 0x2)
    {
        printf("更新失败，请检查参数\n");
        return;
    }
    if (C2EINFO0 == 0x10)
    {
        /* code */
        printf("c");
        // Mailbox_Update_Function((DWORD)(E2CINFO1 >> 24), (DWORD)((E2CINFO1 << 8) >> 8), 0x70800);
    }
    else if (C2EINFO0 == 0x11)
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
    if (C2EINFO0 == 0x20)
    {
        printf("efuse:%x,%x,%x,%x,%x,%x,%x\n", C2EINFO1, C2EINFO2, C2EINFO3, C2EINFO4, C2EINFO5, C2EINFO6, C2EINFO7);
    }
}

void Mailbox_RPMC(void)
{
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
        Mailbox_RPMC();
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