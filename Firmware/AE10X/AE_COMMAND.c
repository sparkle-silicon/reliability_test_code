/*
 * @Author: Linyu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2025-10-21 16:25:46
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
#include "AE_COMMAND.H"
#include "KERNEL_GPIO.H"
#include "AE_PRINTF.H"
#include "AE_UPDATE.H"
#include "CUSTOM_POWER.H"
#if ENABLE_COMMAND_SUPPORT
/*
 * 文档说明:简单设定调试命令行
 * 作者:Linyu
 */
char command[16] = { 0 }; // 命令参数
char *args = NULL;
int command_flag = 0;
char parameter[8][16] = { '\0' };
char CMD_UART_BUFF[CMD_BUFF_MAX + 1] = { 0 };
char *const cmd_argv[] = { command, parameter[0], parameter[1], parameter[2], parameter[3], parameter[4], parameter[5], parameter[6], parameter[7] };
const char *print_mdw = "addr:0x%p data:%#x\n";
sMode Mode;
// 命令执行函数声明
int do_help(struct cmd_tbl *cmd, int flags, int argc, char *const argv[]);
int do_md(struct cmd_tbl *cmd, int flags, int argc, char *const argv[]);
int do_mw(struct cmd_tbl *cmd, int flags, int argc, char *const argv[]);
int do_update(struct cmd_tbl *cmd, int flags, int argc, char *const argv[]);
int do_gpio_config(struct cmd_tbl *cmd, int flags, int argc, char *const argv[]);
// int do_trim(struct cmd_tbl *cmd, int flags, int argc, char *const argv[]);
int do_jump(struct cmd_tbl *cmd, int flags, int argc, char *const argv[]);
int do_dbg(struct cmd_tbl *cmd, int flags, int argc, char *const argv[]);
int do_power(struct cmd_tbl *cmd, int flags, int argc, char *const argv[]);

// 命令列表
const cmd_tbl cmd_menu[] = {
    {.name = "help", .maxargs = 1, .cmd = &do_help, .usage = "help <command> or help\n", .help = "help - Help View Commands\n"},
    {.name = "md", .maxargs = 3, .cmd = &do_md, .usage = "md.[b,w,l] address [# of objects]\n", .help = "md - memory display\n"},
    {.name = "mw", .maxargs = 3, .cmd = &do_mw, .usage = "mw.[b,w,l] address value [count]\n", .help = "mw - memory write (fill)\n"},
    {.name = "update", .maxargs = 3, .cmd = &do_update, .usage = "update <name> -[c]\n", .help = "update [Firmware/other]\n\t -c:check\n"},
    {.name = "gpio", .maxargs = 6, .cmd = &do_gpio_config, .usage = "", .help = ""},
    // {.name = "trim", .maxargs = 2, .cmd = &do_trim, .usage = "", .help = ""},
    {.name = "jump", .maxargs = 2, .cmd = &do_jump, .usage = "jump addr", .help = ""} ,
{.name = "debug", .maxargs = 5, .cmd = &do_dbg, .usage = "debug <Mode> [1,10] level [0,10]", .help = ""} ,

 {.name = "power", .maxargs = 1, .cmd = &do_power, .usage = "power [on,off]", .help = "power - power on/off host"},
};
#ifdef USER_AE10X_LIBC_A
int system(const char *string)
{
    size_t cnt = strlen(string);
    CMD_RUN((volatile char *)&cnt, (char *)string);
    return 0;
}
#endif
static const cmd_tbl *CMD_FIND(char *cmd) // 命令查找函数
{
    unsigned int i = 0;
    while(i < (sizeof(cmd_menu) / sizeof(cmd_tbl)))
    {
        if(strcmp(cmd_menu[i].name, cmd))
        {
            i++;
        }
        else
        {
            return &cmd_menu[i];
        }
    }
    printf(" NOT TO FIND COMMAND %s\n", cmd);
    return &cmd_menu[0];
}
static void DO_CMD(char *cmd, int flag, int argc, char **argv) // 命令执行函数
{
    const cmd_tbl *tbl = CMD_FIND(cmd);
    tbl->cmd((cmd_tbl *)tbl, flag, argc, argv);
}
static char *Cmd_Parse(char *buf) // 命令解析
{
    args = NULL;
    char *ch_p = buf;
    int i = 0;
    while(*ch_p != '\0')
    {
        if(*ch_p != ' ' && *ch_p != '.')
        {
            command[i++] = *ch_p;
            *ch_p++ = ' ';
            if(i >= 16)
            {
                command[15] = '\0';
                printf("CMD very long ,system can't call\n");
                break;
            }
        }
        else
            break;
    }
    command[i] = '\0';
    args = ch_p;
    // dprint(" * CMD:%s\n", command);
    return command;
}
static int CMD_FLAG(char *buf) // 标志位解析
{
    command_flag = 0;
    char buff = 0;
    while(*buf != '\0')
    {
        if(*buf == '.' || *buf == '-')
        {
            *buf++ = ' ';
            while(*buf != ' ' && *buf != '\0')
            {
                buff = (*buf - 'A');
                if(buff >= 32)
                    buff -= 32;
                command_flag |= (1 << buff);
                *buf++ = ' ';
            }
        }
        else
            buf++;
    }
    // dprint(" * FLAG :%#x\n", command_flag);
    return command_flag;
}
static int CMD_PARAMETER(char *buf) // 参数解析函数
{
    int i = 0, j = 0;
    while(*buf != '\0')
    {
        if(*buf != ' ' && *buf != '\0' && *buf != ',')
        {
            while(*buf != ' ' && *buf != '\0' && *buf != ',')
            {
                parameter[i][j++] = *buf;
                *buf++ = ' ';
                if(j >= 16)
                {
                    printf("Parameters for too long:%s...\n", parameter[i]);
                    break;
                }
            }
            parameter[i++][j] = '\0';
            j = 0;
        }
        else
            buf++;
    }
    // j = i;
    // while(j--)
    // {
    //     dprint(" * PARAMETER[%d] : %s\n", j, parameter[j]);
    // }
    i++;
    // dprint(" * argc = %d\n", i);
    return i;
}
void CMD_RUN(volatile char *cnt, char *buf)
{
    char *cmd = NULL;
    int flag = 0;
    int argc = 0;
    // dprint(" * CMD_CNT:%d\n * EC_RX:%s\n", *cnt, buf);
    cmd = Cmd_Parse(buf);       // 命令解析
    flag = CMD_FLAG(args);      // 标志位解析
    argc = CMD_PARAMETER(args); // 参数解析
    DO_CMD(cmd, flag, argc, (char **)cmd_argv); // 执行
    for(int i = 0; i < 9; i++)
        memset((void *)cmd_argv[i], 0, 16); // 清空缓存
}
//----------------------------------------------------------------------------
// FUNCTION: Service_CMD
// UART Command support function
//----------------------------------------------------------------------------
void Service_CMD(void)
{
    if(F_Service_CMD)
    {
        CMD_RUN((volatile char *)&CMD_UART_CNT, (char *)CMD_UART_BUFF);
        CMD_UART_CNT = 0;
        F_Service_CMD = 0;
    }
}
int cmd_atoi(char *buf) // 字符串转数字
{
    int i = 0;
    int base = 0;
    if(*buf == '0')
    {
        buf++;
        if(*buf == 'x' || *buf == 'X')
        {
            base = 16;
            buf++;
        }
        else
            base = 8;
    }
    else
        base = 10;
    while(*buf != '\0')
    {
        if(*buf >= 'A' && *buf <= 'F')
            *buf += 32;
        if(*buf >= '0' && *buf <= '9')
            i = i * base + *buf - '0';
        else if(*buf >= 'a' && *buf <= 'f')
            i = i * base + *buf - 'a' + 10;
        buf++;
    }
    return i;
}
// 执行函数
int do_help(struct cmd_tbl *cmd, int flags, int argc, char *const argv[])
{
    UNUSED_VAR(cmd);
    if(argc == 1)
    {
        unsigned int i = 0;
        while(i < (sizeof(cmd_menu) / 20))
        {
            if(flags & (1 << ('h' - 'a')))
                printf(" * help: %s\n", cmd_menu[i].help);
            else if(flags & (1 << ('u' - 'a')))
                printf("* usage: %s\n", cmd_menu[i].usage);
            else
                printf(" * help: %s\n * usage: %s\n", cmd_menu[i].help, cmd_menu[i].usage);
            i++;
        }
    }
    else
    {
        const cmd_tbl *tbl_p = CMD_FIND(argv[1]);
        printf("-----%s-----\n", argv[0]);
        printf("cmd name : %s\n", argv[1]);
        if(flags & (1 << ('h' - 'a')))
            printf(" * help: %s\n", tbl_p->help);
        else if(flags & (1 << ('u' - 'a')))
            printf("* usage: %s\n", tbl_p->usage);
        else
            printf(" * help: %s * usage: %s\n", tbl_p->help, tbl_p->usage);
    }
    return 0;
}
int do_md(struct cmd_tbl *cmd, int flags, int argc, char *const argv[])
{
    UNUSED_VAR(cmd);
    UNUSED_VAR(argc);
    int ret = 0;
    int addr = cmd_atoi(argv[1]);
    if(argc == 3)
    {
        int repeat = cmd_atoi(argv[2]);
        while(repeat--)
        {
            if(flags & (1 << ('w' - 'a')))
            {
                ret = REG16(addr);
                printf(print_mdw, addr, ret);
                addr += 2;
            }
            else if(flags & (1 << ('b' - 'a')))
            {
                ret = REG8(addr);
                printf(print_mdw, addr, ret);
                addr += 1;
            }
            else
            {
                ret = REG32(addr);
                printf(print_mdw, addr, ret);
                addr += 4;
            }
        }
        return 0;
    }
    if(flags & (1 << ('w' - 'a')))
        ret = REG16(addr);
    else if(flags & (1 << ('b' - 'a')))
        ret = REG8(addr);
    else
        ret = REG32(addr);
    printf(print_mdw, addr, ret);
    return 0;
}
int do_mw(struct cmd_tbl *cmd, int flags, int argc, char *const argv[])
{
    UNUSED_VAR(cmd);
    UNUSED_VAR(argc);
    int addr = cmd_atoi(argv[1]);
    int val = cmd_atoi(argv[2]);
    if(flags & (1 << ('w' - 'a')))
    {
        REG16(addr) = val;
        printf(print_mdw, addr, val);
    }
    else if(flags & (1 << ('b' - 'a')))
    {
        REG8(addr) = val;
        printf(print_mdw, addr, val);
    }
    else
    {
        REG32(addr) = val;
        printf(print_mdw, addr, val);
    }
    return 0;
}
/******************************************************************************************************
 * 可优化方向:
 * 添加代码尺寸判断,接收数据包大小,优化各模块,添加功能 优化数据传输解析,烧写速度与大小等等
 *作者:linyu
 ******************************************************************************************************/
int do_update(struct cmd_tbl *cmd, int flags, int argc, char *const argv[])
{
    UNUSED_VAR(cmd);
    UNUSED_VAR(argc);
#if SUPPORT_FIRMWARE_UPDATE
    if(!strcmp(argv[1], "firmware"))
    {
        PRINTF_LCR &= 0x7F;
        PRINTF_IER &= 0xfe;
        uart_updata_flag &= 0x0; // flag位空
        uart_updata_flag |= 0x1;
        if(flags & (1 << ('C' - 'A')))
            uart_updata_flag |= 0x2; // 校验开启
        update_reg_ptr = (VDWORD)&PRINTF_TX;
        Flash_Update_Function();
        PRINTF_LCR &= 0x7F;
        PRINTF_IER |= 0x01;
    }
#endif
    return 0;
}
extern void sysctl_iomux_config(DWORD port, DWORD io, unsigned port_type);
int do_gpio_config(struct cmd_tbl *cmd, int flags, int argc, char *const argv[])
{
    UNUSED_VAR(cmd);
    UNUSED_VAR(flags);
    UNUSED_VAR(argc);
    int GPIO = cmd_atoi(argv[1]);
    int gpio_no = cmd_atoi(argv[2]);
    int mode = cmd_atoi(argv[3]);
    int op_val = cmd_atoi(argv[4]);
    int int_lv = cmd_atoi(argv[5]);
    int pol = cmd_atoi(argv[6]);
    GPIO_Config(GPIO, gpio_no, mode, op_val, int_lv, pol);
    return 0;
}
// int do_trim(struct cmd_tbl *cmd, int flags, int argc, char *const argv[])
// {
//     UNUSED_VAR(cmd);
//     UNUSED_VAR(flags);
//     UNUSED_VAR(argc);
//     u8 offset;
//     const u32 mask = 0x3ff;
//     u32 new_trim;
//     u32 value;
//     if(flags & (1 << ('h' - 'a')))
//     {
//         offset = 13;
//     }
//     else if(flags & (1 << ('l' - 'a')))
//     {
//         offset = 0;
//     }
//     else return -1;
//     if(argc == 3)value = cmd_atoi(argv[2]);
//     else value = 1;
//     new_trim = ((SYSCTL_OSCTRIM >> offset) & mask);
//     if(!strcmp("add", argv[1]))
//     {
//         if(new_trim + value >= mask)
//         {
//             dprint("max\n");
//             new_trim = mask;
//         }
//         else
//             new_trim += value;
//     }
//     else if(!strcmp("sub", argv[1]))
//     {
//         if(new_trim <= value)
//         {
//             dprint("min\n");
//             new_trim = 0;
//         }
//         else
//             new_trim -= value;
//     }

//     SYSCTL_OSCTRIM = (new_trim & mask) << offset;
//     nop;
//     nop;
//     nop;
//     return 0;
// }
int do_jump(struct cmd_tbl *cmd, int flags, int argc, char *const argv[])
{
    UNUSED_VAR(cmd);
    UNUSED_VAR(flags);
    UNUSED_VAR(argc);
    int addrs = cmd_atoi(argv[1]);
    FUNCT_PTR_V_V jump = (FUNCT_PTR_V_V)addrs;
    F_Service_PUTC = 1;
    dprint("comand jump %#x\n", (u32)jump);
    F_Service_PUTC = 0;
    (*jump)();
    F_Service_PUTC = 1;
    dprint("comand jump %#x return\n", (u32)jump);
    F_Service_PUTC = 0;
    return 0;
}
/******************************************************************************************************
 命令格式:
        debug <Mode> [1,10] level [0,10]

        Mode：模块及对应通道，只有一个通道无需在最后写0
        1：使能对应通道debug
        0：失能对应通道debug
        level：设置当前LEVEL等级
 ******************************************************************************************************/
int do_dbg(struct cmd_tbl *cmd, int flags, int argc, char *const argv[])
{
    UNUSED_VAR(cmd);
    UNUSED_VAR(flags);
    UNUSED_VAR(argc);
    int mode_flag = cmd_atoi(argv[2]);
    int dbg_level = cmd_atoi(argv[4]);
    int mode_offset = 0;
    int mode_len = strlen(argv[1]);
    char temp = argv[1][mode_len - 1];
    if(temp >= 48 && temp <= 57)//读出设备通道号
    {
        argv[1][mode_len - 1] = '\0';
        mode_offset = temp - '0';
    }
    if((dbg_level >= 0 && dbg_level <= 10) && (mode_flag == 0 || mode_flag == 1))//判断命令格式是否正确
    {
        Debug_PRINTF_LEVEL = dbg_level;//设定level值
        if(!strcmp("SYSCTL", argv[1]))//判断Mode类型
        {
            Mode.SYSCTL = mode_flag;
        }
        else if(!strcmp("PNP", argv[1]))
        {
            Mode.PNP = mode_flag;
        }
        else if(!strcmp("SPIF", argv[1]))
        {
            Mode.SPIF = mode_flag;
        }
        else if(!strcmp("PMC", argv[1]))
        {
            Mode.PMC = mode_flag;
        }
        else if(!strcmp("KBC", argv[1]))
        {
            Mode.KBC = mode_flag;
        }
        else if(!strcmp("SM", argv[1]))
        {
            Mode.SM = mode_flag;
        }
        else if(!strcmp("SWUC", argv[1]))
        {
            Mode.SWUC = mode_flag;
        }
        else if(!strcmp("BRAM", argv[1]))
        {
            Mode.BRAM = mode_flag;
        }
        else if(!strcmp("KBS", argv[1]))
        {
            Mode.KBS = mode_flag;
        }
        else if(!strcmp("WDT", argv[1]))
        {
            Mode.WDT = mode_flag;
        }
        else if(!strcmp("PPOT", argv[1]))
        {
            Mode.PPORT = mode_flag;
        }
        else if(!strcmp("ADC", argv[1]))
        {
            Mode.ADC = mode_flag;
        }
        else if(!strcmp("P80", argv[1]))
        {
            Mode.P80 = mode_flag;
        }
        else if(!strcmp("SPIM", argv[1]))
        {
            Mode.SPIM = mode_flag;
        }
        else if(!strcmp("I2C", argv[1]))
        {
            if(mode_flag & 1)
            {
                Mode.I2C |= (1 << mode_offset);
            }
            else
            {
                Mode.I2C &= ~(1 << mode_offset);
            }
        }
        else if(!strcmp("TIMER", argv[1]))
        {
            if(mode_flag & 1)
            {
                Mode.TIMER |= (1 << mode_offset);
            }
            else
            {
                Mode.TIMER &= ~(1 << mode_offset);
            }
        }
        else if(!strcmp("GPIO", argv[1]))
        {
            if(mode_flag & 1)
            {
                Mode.GPIO |= (1 << mode_offset);
            }
            else
            {
                Mode.GPIO &= ~(1 << mode_offset);
            }
        }
        else if(!strcmp("PS2", argv[1]))
        {
            if(mode_flag & 1)
            {
                Mode.PS2 |= (1 << mode_offset);
            }
            else
            {
                Mode.TIMER &= ~(1 << mode_offset);
            }
        }
        else if(!strcmp("PWM", argv[1]))
        {
            if(mode_flag & 1)
            {
                Mode.PWM |= (1 << mode_offset);
            }
            else
            {
                Mode.PWM &= ~(1 << mode_offset);
            }
        }
        else if(!strcmp("UART", argv[1]))
        {
            if(mode_flag & 1)
            {
                Mode.UART |= (1 << mode_offset);
            }
            else
            {
                Mode.UART &= ~(1 << mode_offset);
            }
        }
        else if(!strcmp("NONE", argv[1]))
        {
        }
    }
    return 0;
}


int do_power(struct cmd_tbl *cmd, int flags, int argc, char *const argv[])
{
    UNUSED_VAR(cmd);
    UNUSED_VAR(flags);
    UNUSED_VAR(argc);
    char str1[] = "on";
    char str2[] = "off";
    if(strcmp(argv[1], str1) == 0)
    {
        Custom_S5_S0_Trigger();
        printf("power on host\n");
    }
    else if(strcmp(argv[1], str2) == 0)
    {
        Custom_S0_S5_Trigger(0x01);
        printf("power off host\n");
    }
    else
    {
        printf("argc error\n");
    }
    return 0;
}
#endif
