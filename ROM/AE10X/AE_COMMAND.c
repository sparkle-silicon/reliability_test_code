/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-16 17:04:22
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
/*
 * 文档说明:简单设定调试命令行
 * 作者:Linyu
 */
u_char command[16] = { 0 }; // 命令参数
u_char *args = NULL;
int command_flag = 0;

u_char parameter[8][16] = { '\0' };
u_char *const cmd_argv[] = { command, parameter[0], parameter[1], parameter[2], parameter[3], parameter[4], parameter[5], parameter[6], parameter[7] };
const char *print_mdw = "addr:0x%p data:%#x\n";
sMode Mode;
// 命令执行函数声明
int do_md(struct cmd_tbl *cmd, int flags, int argc, u_char *const argv[]);
int do_mw(struct cmd_tbl *cmd, int flags, int argc, u_char *const argv[]);
int do_update(struct cmd_tbl *cmd, int flags, int argc, u_char *const argv[]);
int do_exit(struct cmd_tbl *cmd, int flags, int argc, u_char *const argv[]);
int do_reboot(struct cmd_tbl *cmd, int flags, int argc, u_char *const argv[]);

// 命令列表
const cmd_tbl cmd_menu[] = {
    {.name = "md", .cmd = &do_md, },
    {.name = "mw", .cmd = &do_mw, },
    {.name = "update",.cmd = &do_update, },
    {.name = "exit",.cmd = &do_exit, },
     {.name = "reboot",.cmd = &do_reboot, },
};
#ifdef USER_AE10X_LIBC_A
int system(const char *string)
{
    CMD_RUN((const u_char *)string);
    return 0;
}
#endif
static const cmd_tbl *CMD_FIND(u_char *cmd) // 命令查找函数
{
    unsigned int i = 0;
    while(i < (sizeof(cmd_menu) / sizeof(cmd_tbl)))
    {
        if(strcmp(cmd_menu[i].name, (const char *)cmd))
        {
            i++;
        }
        else
        {
            return &cmd_menu[i];
        }
    }
    printf(" NOT TO FIND COMMAND %s,%02x\n", cmd, *cmd);
    return NULL;
}
static void DO_CMD(u_char *cmd, int flag, int argc, u_char **argv) // 命令执行函数
{
    const cmd_tbl *tbl = CMD_FIND(cmd);
    if(tbl != NULL)
        tbl->cmd((cmd_tbl *)tbl, flag, argc, argv);
}
static u_char *Cmd_Parse(const u_char *buf) // 命令解析
{
    args = NULL;
    u_char *ch_p = (u_char *)buf;
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
    // printf(" * CMD:%s\n", command);
    return command;
}
static int CMD_FLAG(u_char *buf) // 标志位解析
{
    command_flag = 0;
    u_char buff = 0;
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
    // printf(" * FLAG :%#x\n", command_flag);
    return command_flag;
}
static int CMD_PARAMETER(u_char *buf) // 参数解析函数
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
    //     printf(" * PARAMETER[%d] : %s\n", j, parameter[j]);
    // }
    i++;
    // printf(" * argc = %d\n", i);
    return i;
}
void CMD_RUN(const u_char *buf)
{
    u_char *cmd = NULL;
    int flag = 0;
    int argc = 0;
    // printf(" * CMD_CNT:%d\n * EC_RX:%s\n", *cnt, buf);
    cmd = Cmd_Parse(buf);       // 命令解析
    flag = CMD_FLAG(args);      // 标志位解析
    argc = CMD_PARAMETER(args); // 参数解析
    DO_CMD(cmd, flag, argc, (u_char **)cmd_argv); // 执行
    for(int i = 0; i < 9; i++)
        memset((void *)cmd_argv[i], 0, 16); // 清空缓存
}
int cmd_atoi(u_char *buf) // 字符串转数字
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
int do_md(struct cmd_tbl *cmd, int flags, int argc, u_char *const argv[])
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
int do_mw(struct cmd_tbl *cmd, int flags, int argc, u_char *const argv[])
{
    UNUSED_VAR(cmd);
    UNUSED_VAR(argc);
    int addr = cmd_atoi(argv[1]);
    int val = cmd_atoi(argv[2]);
    if(flags & (1 << ('w' - 'a')))
    {
        REG16(addr) = val;
        printf(print_mdw, addr, REG16(addr));
    }
    else if(flags & (1 << ('b' - 'a')))
    {
        REG8(addr) = val;
        printf(print_mdw, addr, REG8(addr));
    }
    else
    {
        REG32(addr) = val;
        printf(print_mdw, addr, REG32(addr));
    }
    return 0;
}
int do_update(struct cmd_tbl *cmd, int flags, int argc, u_char *const argv[])
{
    UNUSED_VAR(cmd);
    UNUSED_VAR(argc);
    if(!strcmp((const char *)argv[1], "firmware"))
    {
        uint32_t chceck_flag = 0;
        if(flags & (1 << ('C' - 'A')))
            chceck_flag = 1; // 校验开启
        update_fifo = PRINTF_FIFO;
        EC_UART_Update(chceck_flag);
    }
    return 0;
}
int do_exit(struct cmd_tbl *cmd, int flags, int argc, u_char *const argv[])
{
    UNUSED_VAR(argc);
    UNUSED_VAR(flags);
    UNUSED_VAR(argc);
    UNUSED_VAR(argv);
    cmd_flag = 0;
    printf("%s\n", cmd->name);
    return 0;
}
int do_reboot(struct cmd_tbl *cmd, int flags, int argc, u_char *const argv[])
{
    UNUSED_VAR(argc);
    UNUSED_VAR(flags);
    UNUSED_VAR(argc);
    UNUSED_VAR(argv);
    printf("%s\n", cmd->name);
    SYSCTL_RST1 |= BIT(16) | BIT(15);
    _start();//如果没有芯片复位，则从这里跳入
    return 0;
}

