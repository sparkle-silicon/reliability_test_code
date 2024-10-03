/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-05-31 16:09:12
 * @Description: GPIO custom configuration
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
#include "CUSTOM_GPIO.H"
/*-----------------------------------------------------------------------------
 * Data Structure
 *---------------------------------------------------------------------------*/
typedef struct InitGPIOReg
{
    VBYTE *reg;
    BYTE value;
} sInitGPIOReg;
typedef struct InitGPIORegXWORD
{
    VDWORD *reg;
    DWORD value;
} sInitGPIORegXWORD;
const sInitGPIOReg InitGPIORegXBYTE[] =
{
    {&GPIO0_DR0, PinA0_7_Init},
    {&GPIO0_DDR0, PinA0_7_InOut},
    {&GPIO0_DR1, PinA8_15_Init},
    {&GPIO0_DDR1, PinA8_15_InOut},
    {&GPIO0_DR2, PinA16_23_Init},
    {&GPIO0_DDR2, PinA16_23_InOut},
    {&GPIO0_DR3, PinA24_31_Init},
    {&GPIO0_DDR3, PinA24_31_InOut},
    {&GPIO1_DR0, PinB0_7_Init},
    {&GPIO1_DDR0, PinB0_7_InOut},
    {&GPIO1_DR1, PinB8_15_Init},
    {&GPIO1_DDR1, PinB8_15_InOut},
    {&GPIO1_DR2, PinB16_23_Init},
    {&GPIO1_DDR2, PinB16_23_InOut},
#if (defined(AE101)||defined(AE102))
    {&GPIO1_DR3, PinB24_26_Init},
    {&GPIO1_DDR3, PinB24_26_InOut},
#elif defined(AE103)
    {&GPIO1_DR3, PinB24_31_Init},
    {&GPIO1_DDR3, PinB24_31_InOut},
#endif
    {&GPIO2_DR0, PinC0_7_Init},
    {&GPIO2_DDR0, PinC0_7_InOut},
#if (defined(AE101)||defined(AE102))
    {&GPIO2_DR1, PinC8_13_Init},
    {&GPIO2_DDR1, PinC8_13_InOut},
    {&GPIO2_DR2, PinD0_6_Init},
    {&GPIO2_DDR2, PinD0_6_InOut},
#elif defined(AE103)
        {&GPIO2_DR1, PinC8_15_Init},
    {&GPIO2_DDR1, PinC8_15_InOut},
    {&GPIO2_DR2, PinD0_7_Init},
    {&GPIO2_DDR2, PinD0_7_InOut},
    {&GPIO2_DR3, PinD8_Init},
    {&GPIO2_DDR3, PinD8_InOut},

#endif

    {&GPIO3_DR0, PinE0_7_Init},
    {&GPIO3_DDR0, PinE0_7_InOut},
    {&GPIO3_DR1, PinE8_15_Init},
    {&GPIO3_DDR1, PinE8_15_InOut},
    {&GPIO3_DR2, PinE16_23_Init},
    {&GPIO3_DDR2, PinE16_23_InOut},
};
const sInitGPIORegXWORD InitGPIORegXWORD[] =
{
#if (defined(AE101)||defined(AE102))

    {&SYSCTL_PIO0_CFG, PinA0_15_IoMux},
    {&SYSCTL_PIO0_UDCFG, PinA0_15_PullUp},
    {&SYSCTL_PIO1_CFG, PinA16_31_IoMux},
    {&SYSCTL_PIO1_UDCFG, PinA16_31_PullUp},
    {&SYSCTL_PIO2_CFG, PinB0_15_IoMux},
    {&SYSCTL_PIO2_UDCFG, PinB0_15_PullUp},
    {&SYSCTL_PIO3_CFG, PinB16_26_IoMux},
    {&SYSCTL_PIO3_UDCFG, PinB16_26_PullUp},
    {&SYSCTL_PIO4_CFG, PinC0_13_IoMux},
    {&SYSCTL_PIO4_UDCFG, PinC0_13_PullUp},
    {&SYSCTL_PIO5_CFG, PinDE_IoMux},
    {&SYSCTL_PIO5_UDCFG, PinDE_PullUp},
#elif defined(AE103)
    {&SYSCTL_PIO0_CFG, PinA0_15_IoMux},
    {&SYSCTL_PIO1_CFG, PinA16_31_IoMux},
    {&SYSCTL_PIO0_UDCFG, PinA0_31_PullUp},
    {&SYSCTL_PIO2_CFG, PinB0_15_IoMux},
    {&SYSCTL_PIO3_CFG, PinB16_31_IoMux},
    {&SYSCTL_PIO1_UDCFG, PinB0_31_PullUp},
    {&SYSCTL_PIO4_CFG, PinC0_15_IoMux},
    {&SYSCTL_PIO5_CFG, PinDE_IoMux},
    {&SYSCTL_PIO2_UDCFG, PinCD_PullUp},
    {&SYSCTL_PIO3_UDCFG, PinE0_23_PullUp},

#endif
};
typedef struct GetGPIORegInfo
{
    VBYTE *reg;
    VBYTE *Membase;
}sGetGPIORegInfo;

typedef struct GetGPIOMuxInfo
{
    VDWORD *reg;
    VDWORD *Membase;
}sGetGPIOMuxInfo;
sGetGPIORegInfo aGetGPIORegInfo[] =
{
    {&GPIO0_DR0, &GPIO0_DR0_Value},
    {&GPIO0_DDR0, &GPIO0_DDR0_Value},
    {&GPIO0_DR1, &GPIO0_DR1_Value},
    {&GPIO0_DDR1, &GPIO0_DDR1_Value},
    {&GPIO0_DR2, &GPIO0_DR2_Value},
    {&GPIO0_DDR2, &GPIO0_DDR2_Value},
    {&GPIO0_DR3, &GPIO0_DR3_Value},
    {&GPIO0_DDR3, &GPIO0_DDR3_Value},
    {&GPIO1_DR0, &GPIO1_DR0_Value},
    {&GPIO1_DDR0, &GPIO1_DDR0_Value},
    {&GPIO1_DR1, &GPIO1_DR1_Value},
    {&GPIO1_DDR1, &GPIO1_DDR1_Value},
    {&GPIO1_DR2, &GPIO1_DR2_Value},
    {&GPIO1_DDR2, &GPIO1_DDR2_Value},
    {&GPIO1_DR3, &GPIO1_DR3_Value},
    {&GPIO1_DDR3, &GPIO1_DDR3_Value},
    {&GPIO2_DR0, &GPIO2_DR0_Value},
    {&GPIO2_DDR0, &GPIO2_DDR0_Value},
    {&GPIO2_DR1, &GPIO2_DR1_Value},
    {&GPIO2_DDR1, &GPIO2_DDR1_Value},
    {&GPIO2_DR2, &GPIO2_DR2_Value},
    {&GPIO2_DDR2, &GPIO2_DDR2_Value},
#if defined(AE103)
    {&GPIO2_DR3, &GPIO2_DR3_Value},
    {&GPIO2_DDR3, &GPIO2_DDR3_Value},
#endif
    {&GPIO3_DR0, &GPIO3_DR0_Value},
    {&GPIO3_DDR0, &GPIO3_DDR0_Value},
    {&GPIO3_DR1, &GPIO3_DR1_Value},
    {&GPIO3_DDR1, &GPIO3_DDR1_Value},
    {&GPIO3_DR2, &GPIO3_DR2_Value},
    {&GPIO3_DDR2, &GPIO3_DDR2_Value},
};
sGetGPIOMuxInfo aGetGPIOMuxInfo[] =
{
    {&SYSCTL_PIO0_CFG, &SYSCTL_PIO0_CFG_Value},
    {&SYSCTL_PIO0_UDCFG, &SYSCTL_PIO0_UDCFG_Value},
    {&SYSCTL_PIO1_CFG, &SYSCTL_PIO1_CFG_Value},
    {&SYSCTL_PIO1_UDCFG, &SYSCTL_PIO1_UDCFG_Value},
    {&SYSCTL_PIO2_CFG, &SYSCTL_PIO2_CFG_Value},
    {&SYSCTL_PIO2_UDCFG, &SYSCTL_PIO2_UDCFG_Value},
    {&SYSCTL_PIO3_CFG, &SYSCTL_PIO3_CFG_Value},
    {&SYSCTL_PIO3_UDCFG, &SYSCTL_PIO3_UDCFG_Value},
    {&SYSCTL_PIO4_CFG, &SYSCTL_PIO4_CFG_Value},
    {&SYSCTL_PIO4_UDCFG, &SYSCTL_PIO4_UDCFG_Value},
    {&SYSCTL_PIO5_CFG, &SYSCTL_PIO5_CFG_Value},
    {&SYSCTL_PIO5_UDCFG, &SYSCTL_PIO5_UDCFG_Value},
};
//-----------------------------------------------------------------------------
// The function of pin register init.
//-----------------------------------------------------------------------------
void pin_DefaultConfig(void)
{
    BYTE Index = 0x00;
    while(Index < (sizeof(InitGPIORegXWORD) / sizeof(sInitGPIORegXWORD)))
    {
        *InitGPIORegXWORD[Index].reg = InitGPIORegXWORD[Index].value;
        Index++;
    }
    Index = 0x00;
    while(Index < (sizeof(aGetGPIOMuxInfo) / sizeof(sGetGPIOMuxInfo)))
    {
        *aGetGPIOMuxInfo[Index].Membase = *aGetGPIOMuxInfo[Index].reg;
        Index++;
    }
}
//-----------------------------------------------------------------------------
// The function of GPIO default data register and I/O.
//-----------------------------------------------------------------------------
void gpio_DefaultConfig(void)
{
    BYTE iTemp = 0x00;
    while(iTemp < (sizeof(InitGPIORegXBYTE) / sizeof(sInitGPIOReg)))
    {
        *InitGPIORegXBYTE[iTemp].reg = InitGPIORegXBYTE[iTemp].value;
        iTemp++;
    }
    iTemp = 0x00;
    while(iTemp < (sizeof(aGetGPIORegInfo) / sizeof(sGetGPIORegInfo)))
    {
        *aGetGPIORegInfo[iTemp].Membase = *aGetGPIORegInfo[iTemp].reg;
        iTemp++;
    }
}
