/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-02-06 19:29:00
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
#include "KERNEL_TRIM.H"
#if defined(AE103)
u16 High_Clock_trim_get(void)
{
    return (SYSCTL_OSCTRIM >> 13) & 0x3ff;
}
u16 Low_Clock_trim_get(void)
{
    return (SYSCTL_OSCTRIM >> 0) & 0x3ff;
}
void High_Clock_trim_set(u16 new_trim)
{
    SYSCTL_OSCTRIM = (SYSCTL_OSCTRIM & (~(0x3ff << 13))) | ((new_trim & 0x3ff) << 13);
}
void Low_Clock_trim_set(u16 new_trim)
{
    SYSCTL_OSCTRIM = (SYSCTL_OSCTRIM & (~(0x3ff << 0))) | ((new_trim & 0x3ff) << 0);
}
void High_Clock_trim_add(u16 val)
{
    val &= 0x3ff;
    u16 oldtrim = High_Clock_trim_get();
    if(oldtrim + val >= 0x3ff)
    {
        High_Clock_trim_set(0x3ff);
    }
    else  High_Clock_trim_set(oldtrim + val);
}
void High_Clock_trim_sub(u16 val)
{
    val &= 0x3ff;
    u16 oldtrim = High_Clock_trim_get();
    if(oldtrim <= val)
    {
        High_Clock_trim_set(0);
    }
    else  High_Clock_trim_set(oldtrim - val);
}
void Low_Clock_trim_add(u16 val)
{
    val &= 0x3ff;
    u16 oldtrim = Low_Clock_trim_get();
    if(oldtrim + val >= 0x3ff)
    {
        Low_Clock_trim_set(0x3ff);
    }
    else  Low_Clock_trim_set(oldtrim + val);
}
void Low_Clock_trim_sub(u16 val)
{
    val &= 0x3ff;
    u16 oldtrim = Low_Clock_trim_get();
    if(oldtrim <= val)
    {
        Low_Clock_trim_set(0);
    }
    else  Low_Clock_trim_set(oldtrim - val);
}
void Low_Clock_Autotrim(void)//必须高电平准
{
    u32 freq;
    u32 base_freq;
    u32 div = 0xffffffff;
    base_freq = CHIP_CLOCK_INT_HIGH / SYSCTL_CLKDIV_OSC96M;
    do
    {
        freq = get_cpu_freq();
        if(freq > base_freq)
        {
            div = freq - base_freq;
            if(div > (base_freq / 100))
                Low_Clock_trim_add(1);
        }
        else if(freq < base_freq)
        {
            div = base_freq - freq;
            if(div > (base_freq / 100))
                Low_Clock_trim_sub(1);
        }
        else break;
        nop; nop; nop;
    /* code */
    }
    while(div > (base_freq / 100));


}
void High_Clock_Autotrim(void)//必须低电平准
{
    u32 freq;
    u32 base_freq;
    u32 div = 0xffffffff;
    base_freq = CHIP_CLOCK_INT_HIGH / SYSCTL_CLKDIV_OSC96M;
    do
    {
        freq = get_cpu_freq();
        if(freq > base_freq)
        {
            div = freq - base_freq;
            if(div > (base_freq / 100))
                High_Clock_trim_sub(1);
        }
        else if(freq < base_freq)
        {
            div = base_freq - freq;
            if(div > (base_freq / 100))
                High_Clock_trim_add(1);
        }
        else break;
        nop; nop; nop;
    /* code */
    }
    while(div > (base_freq / 100));
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
#endif