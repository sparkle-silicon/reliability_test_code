/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-06-04 17:22:59
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
#include <KERNEL_RTC.H>
u32 Get_RTC_CountVal(void)
{
    u32 rtc_ccvr = (RTC_CCVR0 << 0) | \
        (RTC_CCVR1 << 8) | \
        (RTC_CCVR2 << 16) | \
        (RTC_CCVR3 << 24);
    return rtc_ccvr;
}
void Set_RTC_MatchVal(u32 count)
{
    RTC_CMR0 = (u8)((count >> 0) & 0xff);
    RTC_CMR1 = (u8)((count >> 8) & 0xff);
    RTC_CMR2 = (u8)((count >> 16) & 0xff);
    RTC_CMR3 = (u8)((count >> 24) & 0xff);
}
void Set_RTC_LoadVal(u32 count)
{
    RTC_CLR0 = (u8)((count >> 0) & 0xff);
    RTC_CLR1 = (u8)((count >> 8) & 0xff);
    RTC_CLR2 = (u8)((count >> 16) & 0xff);
    RTC_CLR3 = (u8)((count >> 24) & 0xff);
}
void Set_RTC_PreScaler(u32 count)
{
    RTC_CPSR0 = (u8)((count >> 0) & 0xff);
    RTC_CPSR1 = (u8)((count >> 8) & 0xff);
    RTC_CPSR2 = (u8)((count >> 16) & 0xff);
    RTC_CPSR3 = (u8)((count >> 24) & 0xff);

}
u32 Get_RTC_PrescalerCountVal(void)
{
    u32 rtc_ccvr = (RTC_CPCVR0 << 0) | \
        (RTC_CPCVR1 << 8) | \
        (RTC_CPCVR2 << 16) | \
        (RTC_CPCVR3 << 24);
    return rtc_ccvr;
}
u32 Get_RTC_Status(void)
{
    u32 rtc_stat = (RTC_CCR0 & BIT1) ? (RTC_RSTAT0) : (RTC_STAT0);
    return rtc_stat;
}

void RTC_Init(u32 loadcnt, u32 match, u32 prescaler)
{
    RTC_CCR0 &= ~RTC_CCR_EN;
    Set_RTC_LoadVal(loadcnt);
    Set_RTC_MatchVal(match);
    Set_RTC_PreScaler(prescaler);
    RTC_CCR0 |= RTC_CCR_EN | RTC_CCR_IEN | RTC_CCR_WEN | RTC_CCR_PSCLREN;/*RTC_CCR_MASK */
    dprint("RTC Version %c.%c%c\n", RTC_COMP_VERSION3, RTC_COMP_VERSION2, RTC_COMP_VERSION1);
}
void RTC_Deep_Sleep_Test(u32 sleep_time)
{
    printf("RTC SLEEP CONFIG\n");
    SYSCTL_PMUCSR |= BIT(20);
    RTC_CCR0 &= ~RTC_CCR_EN;
    Set_RTC_MatchVal(sleep_time + Get_RTC_CountVal());
    RTC_CCR0 |= RTC_CCR_EN | RTC_CCR_IEN | RTC_CCR_WEN | RTC_CCR_PSCLREN;
    //CPSR set 160 freq
    Set_RTC_PreScaler(0xA0);
        //SET Enable DeepSleep and Switch PLL
    //BIT0:1:SWITCH Internel OSC32k/196M(0:switch externel OSC32k/pll)
    //BIT1:0:Disable Internel OSC192M(1:ENable Internel OSC192M)
    //BIT2:1:Sleep Main Freq Set High Clock(0:Sleep Main Freq Set Low Clock)
    //BIT3:0:Sleep No close High Clock(1:wait main freq set low clock(bit2=0),close high clock)
    SYSCTL_SWITCH_PLL = BIT3 | BIT0;
    // SYSCTL_MODEN0 = UART0_EN; //
    SYSCTL_MODEN0 = 0;
    SYSCTL_MODEN1 = RTC_EN | SPIFE_EN | CACHE_EN;
    SYSCTL_DVDD_EN |= BIT4 | BIT5 | BIT6;
    SYSCTL_DVDD_EN &= ~(BIT0 | BIT1 | BIT2);
    CPU_Sleep();
}
void RTC_Sleep_Test(u32 sleep_time)
{
    printf("RTC SLEEP CONFIG\n");
    SYSCTL_PMUCSR |= BIT(20);
    RTC_CCR0 &= ~RTC_CCR_EN;
    Set_RTC_MatchVal(sleep_time + Get_RTC_CountVal());
    RTC_CCR0 |= RTC_CCR_EN | RTC_CCR_IEN | RTC_CCR_WEN | RTC_CCR_PSCLREN;
    //CPSR set 160 freq
    Set_RTC_PreScaler(0xA0);
        // vDelayXus(120);
    SYSCTL_MODEN0 = 0x100;
    SYSCTL_MODEN1 = RTC_EN | SPIFE_EN | CACHE_EN;
    SYSCTL_DVDD_EN |= BIT4 | BIT5 | BIT6;
    SYSCTL_DVDD_EN &= ~(BIT0 | BIT1 | BIT2);
    CPU_Sleep();

}
