/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-07-12 16:16:48
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
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_ESPI.H" //全局变量
#include "CUSTOM_POWER.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
#include "KERNEL_PECI.H"
#include "KERNEL_MAILBOX.H"
extern BYTE eRPMC_Busy_Status;
/*-----------------------------------------------------------------------------
 * eSPI Module Proess Definition
 *---------------------------------------------------------------------------*/
#define SUPPORT_OOB_SERVICE_MODULE TRUE
#define DEBUG_OOB_MESSAGE FALSE

 /* eSPI <-> OOB <-> PCH */
#define SUPPORT_OOB_PCH_TEMPERATURE TRUE
#define PCH_TEMP_GET_OOB_SEC 5

#define SUPPORT_OOB_PCH_RTC_TIME FALSE
#define RTC_TIME_ONLY_S0 FALSE
#define RTC_TIME_AUTO_COUNT TRUE
#define RTC_TIME_GET_OOB_SEC 120

/* eSPI <-> OOB <-> PECI */
#define SUPPORT_OOB_PECI_POWER_CTRL TRUE
#define SUPPORT_OOB_PECI_GetTemp FALSE
#define OOB_PECI_GetTemp_SEC 1

/* eSPI <-> OOB <-> Intel CrashLog */
#define SUPPORT_OOB_INTEL_CRASHLOG FALSE
#define _EFLASH_CRASHLOG_ERASE_ADDR 0x01B000
#define _EFLASH_CRASHLOG_SIZE_K 20
#define _EFLASH_CRASHLOG_CPU_ADDR 0x01B000
#define _EFLASH_CRASHLOG_PCH_ADDR 0x01D000
#define _EFLASH_CRASHLOG_INFO_ADDR 0x01FE00
#define _EFLASH_CRASHLOG_CPU_ADRH (_EFLASH_CRASHLOG_CPU_ADDR >> 16)
#define _EFLASH_CRASHLOG_CPU_ADRM (_EFLASH_CRASHLOG_CPU_ADDR >> 8)
#define _EFLASH_CRASHLOG_CPU_ADRL (_EFLASH_CRASHLOG_CPU_ADDR)
#define _EFLASH_CRASHLOG_PCH_ADRH (_EFLASH_CRASHLOG_PCH_ADDR >> 16)
#define _EFLASH_CRASHLOG_PCH_ADRM (_EFLASH_CRASHLOG_PCH_ADDR >> 8)
#define _EFLASH_CRASHLOG_PCH_ADRL (_EFLASH_CRASHLOG_PCH_ADDR)
#define _EFLASH_CRASHLOG_INFO_ADRH (_EFLASH_CRASHLOG_INFO_ADDR >> 16)
#define _EFLASH_CRASHLOG_INFO_ADRM (_EFLASH_CRASHLOG_INFO_ADDR >> 8)
#define _EFLASH_CRASHLOG_INFO_ADRL (_EFLASH_CRASHLOG_INFO_ADDR)

//*****************************************************************************
/* eSPI Slave to PCH OOB PACKET */
//*****************************************************************************
eSPI_OOB_WriteRootKet_MESSAGE1 eRPMC_WriteRootKey_m1;
eSPI_OOB_WriteRootKet_MESSAGE2 eRPMC_WriteRootKey_m2;
eSPI_OOB_WriteRootKet_RESPONSE eRPMC_WriteRootKey_data;
eSPI_OOB_UpdateHMACKey eRPMC_UpdateHMACKey;
eSPI_OOB_UpdateHMACKey_RESPONSE eRPMC_UpdateHMACKey_data;
eSPI_OOB_IncrementCounter eRPMC_IncrementCounter;
eSPI_OOB_IncrementCounter_RESPONSE eRPMC_IncrementCounter_data;
eSPI_OOB_RequestCounter eRPMC_RequestCounter;
eSPI_OOB_RequestCounter_RESPONSE eRPMC_RequestCounter_data;
eSPI_OOB_ReadParameters eRPMC_ReadParameters;
eSPI_OOB_ReadParameters_RESPONSE eRPMC_ReadParameters_data;

BYTE TO_PCH_TEMPERATURE[7] =
{ 0x21, 0x00, 0x04, 0x02, 0x01, 0x01, 0x0F };
BYTE TO_PCH_RTC_TIME[7] =
{ 0x21, 0x00, 0x04, 0x02, 0x02, 0x01, 0x0F };
BYTE TO_PCH_PMC_CRASHLOG[7] =
{ 0x21, 0x00, 0x04, 0x20, 0x20, 0x01, 0x0F };
/* Intel OOB Crash Log Debug */
// Byte#2: Length[7:0] = 04h
// Byte#3: Dest Slave Addr [7:1] 10h (Intel PCH PMC SMBus MCTP)
// Byte#4: Command Code = 20h-24h(Trigger_CrashLog_and_Extract),Bit0:0
// Byte#5: Byte Count = 01h
// Byte#6: Source Slave Address[7:0] = 0Fh(eSPI slave 0/EC),Bit0:1

/* eSPI OOB <--> PECI */
BYTE TO_PCH_PECI_GET_DIB[11] =
{ 0x21, 0x00, 0x08, 0x20, 0x01, 0x05, 0x0F, 0x30, 0x01, 0x08, 0xF7 };
BYTE TO_PCH_PECI_GET_TEMP[11] =
{ 0x21, 0x00, 0x08, 0x20, 0x01, 0x05, 0x0F, 0x30, 0x01, 0x02, 0x01 };

BYTE Peripheral_Message_Send[8] =
{ 0x05, 0x00, 0x00, 0x01, 0x84, 0x04, 0x00, 0x00 };
/* Peripheral Message format */
// Byte#0: Cycle type : 05h -> no data bytes   06h -> have data bytes
// Byte#1: TAG & Length[11:8]
// Byte#2: Length[7:0]  -> data byte numbers
// Byte#3: Message code
// Byte#4-7: Specific message codes
// Byte#5-N: Data bytes

BYTE Peripheral_Memory_Read32[7] =
{ 0x02, 0x01, 0x04, 0xFF, 0xFF, 0xFF, 0x0F };
/* Peripheral Memory Rd32 format */
// Byte#0: Cycle type
// Byte#1: TAG & Length[11:8]
// Byte#2: Length[7:0]  -> data length you want to read
// Byte#3-6: 32bits address

BYTE Peripheral_Memory_Read64[11] =
{ 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03 };
/* Peripheral Memory Rd64 format */
// Byte#0: Cycle type
// Byte#1: TAG & Length[11:8]
// Byte#2: Length[7:0]  -> data length you want to read
// Byte#3-6: 64bits address
/* RPMC<-->OOB Message ARRAY */
BYTE RPMC_OOB_TempArr[80];

/*-----------------------------------------------------------------------------
 * eSPI array definition
 *---------------------------------------------------------------------------*/
BYTE *OOB_Table_Pntr = NULL;
BYTE *Peri_Table_Pntr = NULL;
BYTE eSPI_PCH_TMPR[16];
BYTE eSPI_RTC_DATA[12];
BYTE OOB_PECI_Temper[2];
BYTE eSPI_FLASH_DATA[80] = { 0xF0, 0xF1, 0xF2, 0xF3 };
BYTE eSPI_Peri_MemRd32[4];
BYTE eSPI_Peri_MemRd64[4];

/*-----------------------------------------------------------------------------
 * Data Structure Prototype
 *---------------------------------------------------------------------------*/
enum intel_crashlog_step{
    _CRASHLOG_END = 0,
    _CRASHLOG_ERASE_DATA = 1,
    _CRASHLOG_GET_CPU_SIZES = 2,
    _CRASHLOG_READ_CPU_LOG = 3,
    _CRASHLOG_SAVE_CPU_LOG = 4,
    _CRASHLOG_EXIT_CPU_LOG = 5,
    _CRASHLOG_READ_PCH_LOG = 6,
    _CRASHLOG_SAVE_PCH_LOG = 7,
    _CRASHLOG_EXIT_PCH_LOG = 8,
    _CRASHLOG_SAVE_INFO = 9,
};

#ifndef SPK_CHECK_RSMRST_HI
#define SPK_CHECK_RSMRST_HI(x) IS_GPIOB12(x)
#endif
#ifndef SystemWarmBoot
#define SystemWarmBoot(x)
#endif

//*****************************************************************************
// eSPI Bus
//*****************************************************************************
void ESPI_Init(void)
{
#if SUPPORT_INTERFACE_eSPI
    SYSCTL_PMUCSR &= 0xffffbfff;    // bypass 1.8V
    SYSCTL_PMU_CFG &= 0xffffff81;
    SYSCTL_PMU_CFG |= BIT7;         // set ec recognized 1.8V
    SYSCTL_PIO_CFG |= BIT2;         // select espi/lpc   bit2 -> 0:lpc 1:espi 
#if SPK_eSPI_LOW_FREQUENCY
    REG_3105 = _eSPI_MAX_FREQ_20MHZ;    // Low Frequency PLL change is not required
#else
    REG_3105 = _eSPI_OP_FREQ_20MHZ;
#endif
    ES_ACK_TIMEOUT_NUMBER = 0x18;   // 设置ack timeout number为1
#if ITE_eSPI_SAFS_MODE
    REG_3115 = _SUPPORT_SAFS;
#else
    REG_3115 = _SUPPORT_MAFS;
#endif
    ESGCTRL1 = F_eSPI_INT_ENABLE;                /* REG@31A1.7: eSPI Interrupt Enable */
    ESGCTRL2 = F_eSPI_TO_WUC_ENABLE;                /* REG@31A1.4: eSPI To WUC Enable */
    VWCTRL0 &= (~F_VW_UPDATE_FLAG_SEL);
    VWUPDATEMODESEL = 0x0;
    ESUCTRL0 |= Upstream_INT_EN;    // 使能upsteam中断使能
#endif
}
/*-----------------------------------------------------------------------------
 * @subroutine - EC_ACK_eSPI_Reset
 * @function - EC_ACK_eSPI_Reset
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     -
 * Peripheral channel is reset when eSPI Host Bridge is reset by Platform
 * Reset (PLTRST#). Prior to PLTRST# assertion, eSPI master and slave complete
 * the HOST_RST_WARN and HOST_RST_ACK Virtual Wires handshake. After sending
 * the HOST_RST_ACK, eSPI slave must not send any Peripheral channel
 * transaction, nor any host domain Virtual Wires (i.e. Virtual Wires reset
 * by PLTRST#) such as SMI#, SCI#, RCIN# or IRQ. Until PLTRST# is
 * non-assertion, no transaction shall occur on eSPI peripheral channel and
 * no host domain Virtual Wires shall be sent from eSPI master or slave.
 * eSPI peripheral channel is enabled by default after PLTRST# non-assertion.
 */
void EC_ACK_eSPI_Reset(void)
{
    if ((IS_MASK_SET(VWIDX7, F_IDX7_HOST_RST_WARN)) && (IS_MASK_SET(VWIDX7, F_IDX7_HOST_RST_WARN_VALID)))
    {
        VWIDX6 = (F_IDX6_HOST_RST_ACK_VALID +
            F_IDX6_HOST_RST_ACK +
            F_IDX6_RCIN +
            F_IDX6_SMI +
            F_IDX6_SCI);

        if (IS_MASK_SET(REG_3104, F_ALERT_MODE))
        {
            sysctl_iomux_config(GPIOD, 6, 1); // alert
        }

        VWCTRL3 |= F_VW_INDEX_6_RESEND;     //send VW INDEX6

        _C1 = 60000;
        while (IS_MASK_SET(VWIDX7, F_IDX7_HOST_RST_WARN))
        {
            _C1--;
        #if 1 /* Timeout if need */
            if (_C1 == 0)
            {
                break;
            }
        #endif
        }

        VWIDX6 = (F_IDX6_HOST_RST_ACK_VALID +
            F_IDX6_RCIN +
            F_IDX6_SMI +
            F_IDX6_SCI);

        if (IS_MASK_SET(REG_3104, F_ALERT_MODE))
        {
            sysctl_iomux_config(GPIOD, 6, 1); // alert
        }

        VWCTRL3 |= F_VW_INDEX_6_RESEND;     //send VW INDEX6
        Hook_EC_ACK_eSPI_Reset();
    }

    if ((IS_MASK_SET(VWIDX3, F_IDX3_OOB_RST_WARN)) && (IS_MASK_SET(VWIDX3, F_IDX3_OOB_RST_WARN_VALID)))
    {
        VWIDX4 |= (F_IDX4_OOB_RST_ACK_VALID +
            F_IDX4_OOB_RST_ACK);

        if (IS_MASK_SET(REG_3104, F_ALERT_MODE))
        {
            sysctl_iomux_config(GPIOD, 6, 1); // alert
        }

        VWCTRL3 |= F_VW_INDEX_4_RESEND;     //send VW INDEX4

        _C1 = 60000;
        while (IS_MASK_SET(VWIDX3, F_IDX3_OOB_RST_WARN))
        {
            _C1--;
        #if 1 /* Timeout if need */
            if (_C1 == 0)
            {
                break;
            }
        #endif
        }

        _R5 = (VWIDX4 | F_IDX4_OOB_RST_ACK_VALID);
        _R5 &= (~F_IDX4_OOB_RST_ACK_VALID);
        VWIDX4 = _R5;  //(F_IDX4_OOB_RST_ACK_VALID);

        if (IS_MASK_SET(REG_3104, F_ALERT_MODE))
        {
            sysctl_iomux_config(GPIOD, 6, 1); // alert
        }

        VWCTRL3 |= F_VW_INDEX_4_RESEND;     //send VW INDEX4
    }
}
/*-----------------------------------------------------------------------------
 * @subroutine - EC_ACK_eSPI_SUS_WARN
 * @function - EC_ACK_eSPI_SUS_WARN
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     -
 * SUSPWRDNACK: Used by PCH/SoC to inform EC it OK to remove suspend well
 * power. Asserted by the PCH/SoC on behalf of the Intel ME when it does not
 * require the suspend well.
 * SUS_WARN# asserts when the PCH is preparing to remove Primary well power
 * (using SLP_SUS#) to enter the DeepSx power state. EC must observe edges on
 * this Virtual Wire, preparing for Primary well power loss on a falling edge
 * and preparing for Primary well related activities on a rising edge.
 * SUSACK# indicates that external devices are ready for Deep Sx entry
 * (i.e., for Primary well power loss) in response to a SUS_WARN# assertion
 * (only for platforms that support DeepSx).
 */
void EC_ACK_eSPI_SUS_WARN(void)
{
#if 0
    /* Use internal registers */
    if (IS_MASK_SET(REG_32A6, BIT3))
    {
        VWIDX40 |= (F_IDX40_VALID + F_IDX40_SUSACK);
        REG_32A6 = 0x0F;
    }
#else
    /* Use formal registers */
    if (IS_MASK_SET(VWIDX41, F_IDX41_SUS_WARN) &&
        IS_MASK_SET(VWIDX41, F_IDX41_VALID))
    {
        if (IS_MASK_SET(VWIDX40, F_IDX40_VALID))
            return;
        VWIDX40 |= (F_IDX40_SUSACK + F_IDX40_VALID);

        if (IS_MASK_SET(REG_3104, F_ALERT_MODE))
        {
            sysctl_iomux_config(GPIOD, 6, 1); // alert
        }

        VWCTRL3 |= F_VW_INDEX_40_RESEND;     //send VW INDEX40
    }
#endif
}
/*-----------------------------------------------------------------------------
 * @subroutine - EC_ACK_eSPI_Boot_Ready
 * @function - EC_ACK_eSPI_Boot_Ready
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     -
 * Sent when EC or BMC has completed its boot process as indication to eSPI-MC
 * to continue with the G3 to S0 exit. eSPI master waits for the assertion of
 * this virtual wire before proceeding with the SLP_S5# deassertion. Intent is
 * that it is never changed except on a G3 exit ?its reset on G3 entry.
 */
BYTE EC_ACK_eSPI_Boot_Ready(void)
{
#if SUPPORT_EN_VW_ACK_BOOT_LOAD
    if (IS_MASK_SET(REG_310F, F_VW_CHN_ENABLE))
    {
        if (IS_MASK_SET(REG_310F, F_VW_CHN_READY))
        {
            if (VWIDX5 == (F_IDX5_SLAVE_BOOT_LOAD_STATUS_VALID +
                F_IDX5_SLAVE_BOOT_LOAD_DONE_VALID +
                F_IDX5_SLAVE_BOOT_LOAD_STATUS +
                F_IDX5_SLAVE_BOOT_LOAD_DONE))
                return 0;

            VWIDX5 = (F_IDX5_SLAVE_BOOT_LOAD_STATUS_VALID +
                F_IDX5_SLAVE_BOOT_LOAD_DONE_VALID +
                F_IDX5_SLAVE_BOOT_LOAD_STATUS +
                F_IDX5_SLAVE_BOOT_LOAD_DONE);

            /* Detect eSPI Configuration - Alert Mode
               0b: EIO1 (I/O[1] ) pin is used to signal the Alert event.
               1b: A dedicated ALERT# pin is used to signal the Alert event.
            */
            if (IS_MASK_SET(REG_3104, F_ALERT_MODE))
            {
                sysctl_iomux_config(GPIOD, 6, 1); // alert
            }

            VWCTRL3 |= F_VW_INDEX_5_RESEND;     //send VW INDEX5
            return 1;
        }
    }
#endif
    return 0;
}

/*-----------------------------------------------------------------------------
 * @subroutine - EC_SET_eSPI_CHN_Ready
 * @function - EC_SET_eSPI_CHN_Ready
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     -
 * Polling CHANNEL ENABLE bit then set eSPI corresponding channel ready bit
 */
void EC_SET_eSPI_CHN_Ready(void)
{
    if (IS_MASK_SET(REG_310F, F_VW_CHN_ENABLE))
    {
        SET_MASK(ES_CHANNEL_READY, SET_VW_CHANNEL_READY);
    }
    if (IS_MASK_SET(REG_3113, F_OOB_CHN_ENABLE))
    {
        SET_MASK(ES_CHANNEL_READY, SET_OOB_CHANNEL_READY);
    }
    if (IS_MASK_SET(REG_3117, F_FLASH_CHN_ENABLE))
    {
        SET_MASK(ES_CHANNEL_READY, SET_FAS_CHANNEL_READY);
    }
}

/*-----------------------------------------------------------------------------
 * @subroutine - OOB_Check_Upstream_Authority_EN
 * @function - OOB_Check_Upstream_Authority_EN
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
BYTE OOB_Check_Upstream_Authority_EN(void)
{
    ESUCTRL0 |= (Upstream_Req_Authority << 0); // Request upstream anthority
    /* Check upstream anthority en */
    xOOB_Failed = 0;
    xOOB_Timeout = 255; // 255;
    while (1)
    {
        if (ESUCTRL0 & Upstream_Read_Authority_EN)
        {
            break;
        }
        if (xOOB_Timeout > 0)
        {
            xOOB_Timeout--;
        }
        else
        {
            xOOB_Failed = 1;
            break;
        }
        /* Delay 15.26 us */
        vDelayXus(16);
    }

    if (xOOB_Failed > 0)
    {
        xOOB_FailedCounter++;
        ESUCTRL0 &= (~Upstream_Req_Authority);
        return FALSE;
    }
    ESUCTRL0 &= (~Upstream_Req_Authority);/* Wrtie-0 to clear Upstream Authority Request */
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * @subroutine - OOB_Check_Upstream_Authority_Disable
 * @function - OOB_Check_Upstream_Authority_Disable
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
BYTE OOB_Check_Upstream_Authority_Disable(void)
{
    /* Check upstream anthority disable */
    xOOB_Failed = 0;
    xOOB_Timeout = 255; // 255;
    while (1)
    {
        if ((ESUCTRL0 & Upstream_Read_Authority_EN) == 0x00)
        {
            break;
        }
        if (xOOB_Timeout > 0)
        {
            xOOB_Timeout--;
        }
        else
        {
            xOOB_Failed = 1;
            break;
        }
        /* Delay 15.26 us */
        vDelayXus(16);
    }

    if (xOOB_Failed > 0)
    {
        xOOB_FailedCounter++;
        ESUCTRL0 &= ~Upstream_GO;
        return FALSE;
    }
    ESUCTRL0 &= ~Upstream_GO;
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * @subroutine - OOB_Check_Upstream_Done
 * @function - OOB_Check_Upstream_Done
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
BYTE OOB_Check_Upstream_Done(void)
{
    /* Check upstream done */
    xOOB_Failed = 0;
    xOOB_Timeout = 255; // 255;
    while (1)
    {
        if (ESUCTRL0 & Upstream_Done)
        {
            break;
        }
        if (xOOB_Timeout > 0)
        {
            xOOB_Timeout--;
        }
        else
        {
            xOOB_Failed = 1;
            break;
        }
        /* Delay 15.26 us */
        vDelayXus(16);
    }

    if (xOOB_Failed > 0)
    {
        xOOB_FailedCounter++;
        ESUCTRL0 |= Upstream_Done;  /* Wrtie-1 to clear Upstream Done */
        return FALSE;
    }

    ESUCTRL0 |= Upstream_Done;
    return TRUE;
}
/*-----------------------------------------------------------------------------
 * @subroutine - OOB_Check_OOB_Status
 * @function - OOB_Check_OOB_Status
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
BYTE OOB_Check_OOB_Status(void)
{
    /* Check OOB status */
    xOOB_Failed = 0;
    xOOB_Timeout = 255; // 255;
    while (1)
    {
        if (ESOCTRL0 & PUT_OOB_STATUS)
        {
            break;
        }
        if (xOOB_Timeout > 0)
        {
            xOOB_Timeout--;
        }
        else
        {
            xOOB_Failed = 1;
            break;
        }
        /* Delay 15.26 us */
        vDelayXus(16);
    }
    if (xOOB_Failed > 0)
    {
        xOOB_FailedCounter++;
        ESOCTRL0 |= PUT_OOB_STATUS;  /* Write clear for next OOB receive */
        return FALSE;
    }

    ESOCTRL0 |= PUT_OOB_STATUS;  /* Write clear for next OOB receive */
    return TRUE;
}

#if SUPPORT_OOB_SERVICE_MODULE
/*-----------------------------------------------------------------------------
 * @subroutine - Get_OOB_RTC_Time
 * @function - Get_OOB_RTC_Time
 * @Upstream - Service_OOB_Message
 * @input    - None
 * @return   - None
 * @note     - None
 */
void Get_OOB_RTC_Time(void)
{
#if RTC_TIME_ONLY_S0
    if (System_PowerState != SYSTEM_S0)
    {
        return;
    }
#else
    if (IS_MASK_SET(System_PowerState, F_PST_BUSY))
    {
        xOOB_GetPCH_RTC_Timer = RTC_TIME_GET_OOB_SEC;
        return;
    }
#endif
    if (xOOB_GetPCH_RTC_Timer > 0)
    {
        xOOB_GetPCH_RTC_Timer--;
    #if RTC_TIME_AUTO_COUNT
        xTIME_SS++;
        if (xTIME_SS > 59)
        {
            xTIME_SS = 0x00;
            xTIME_MM++;
            if (xTIME_MM > 59)
            {
                xTIME_MM = 0x00;
                xTIME_HH++;
                if (xTIME_HH > 23)
                {
                    xTIME_HH = 0x00;
                }
            }
        }
    #endif
    }
    else
    {
        xOOB_GetPCH_RTC_Timer = RTC_TIME_GET_OOB_SEC;
        OOB_Table_Pntr = TO_PCH_RTC_TIME;
        Tmp_XPntr = &eSPI_RTC_DATA[0];
        xOOB_PacketMaxLength = 12;
        Process_eSPI_OOB_Message();
        /* Check & Transfer BCD to Dec */
        _R5 = (eSPI_RTC_DATA[5] & 0x0F);
        if (_R5 > 9)
        {
            return;
        }
        _R6 = ((eSPI_RTC_DATA[5] >> 4) & 0x0F) * 10;
        if (_R6 > 50)
        {
            return;
        }
        _R7 = (eSPI_RTC_DATA[6] & 0x0F);
        if (_R7 > 9)
        {
            return;
        }
        _R8 = ((eSPI_RTC_DATA[6] >> 4) & 0x0F) * 10;
        if (_R8 > 50)
        {
            return;
        }
        _R2 = (eSPI_RTC_DATA[7] & 0x0F);
        if (_R2 > 9)
        {
            return;
        }
        _R3 = ((eSPI_RTC_DATA[7] >> 4) & 0x0F) * 10;
        if (_R3 > 20)
        {
            return;
        }
        xTIME_SS = _R5 + _R6;
        xTIME_MM = _R7 + _R8;
        xTIME_HH = _R2 + _R3;
        xTIME_SYNC++;
    }
}

/*-----------------------------------------------------------------------------
 * @subroutine - Get_OOB_PCH_Temperature
 * @function - Get_OOB_PCH_Temperature
 * @Upstream - Service_OOB_Message
 * @input    - None
 * @return   - None
 * @note     - None
 */
void Get_OOB_PCH_Temperature(void)
{
    OOB_Table_Pntr = TO_PCH_TEMPERATURE;
    Tmp_XPntr = &eSPI_PCH_TMPR[0];
    xOOB_PacketMaxLength = 16;
    if (Process_eSPI_OOB_Message())
    {
        xOOB_PCH_Temperature = eSPI_PCH_TMPR[4];
        dprint("xOOB_PCH_Temperature = %d\n", xOOB_PCH_Temperature);
    }
}

/*-----------------------------------------------------------------------------
 * @subroutine - OOB_PECI_GetDIB
 * @function - OOB_PECI_GetDIB
 * @Upstream - Service_OOB_Message
 * @input    - None
 * @return   - None
 * @note     - None
 */
void OOB_PECI_GetDIB(void)
{
    OOB_Table_Pntr = TO_PCH_PECI_GET_DIB;
    Tmp_XPntr = &PECI_ReadBuffer[0];
    xOOB_PacketMaxLength = 16;
    Process_eSPI_OOB_Message();
}

/*-----------------------------------------------------------------------------
 * @subroutine - OOB_PECI_GetTemp
 * @function - OOB_PECI_GetTemp
 * @Upstream - Service_OOB_Message
 * @input    - None
 * @return   - None
 * @note     - None
 */
void OOB_PECI_GetTemp(void)
{
    OOB_Table_Pntr = TO_PCH_PECI_GET_TEMP;
    Tmp_XPntr = &PECI_ReadBuffer[0];
    xOOB_PacketMaxLength = 16;
    if (Process_eSPI_OOB_Message())
    {
        OOB_PECI_Temper[0] = PECI_ReadBuffer[5];
        OOB_PECI_Temper[1] = PECI_ReadBuffer[6];
        _C1 = (WORD)(OOB_PECI_Temper[1] << 8) + OOB_PECI_Temper[0];
        _C1 = (~_C1) + 1; /* 2's complement */
        _R5 = _C1 >> 6;   /* 1/64 degrees centigrade */
        if (xOOB_PECI_Tj_max == 0)
        {
            xOOB_PECI_CPU_T = 100 - _R5;
        }
        else
        {
            if ((_R5 & 0x80) == 0x00)
            {
                xOOB_PECI_CPU_T = xOOB_PECI_Tj_max - _R5;
            }
            else
            {
                _R5 = (~_R5);
                xOOB_PECI_CPU_T = xOOB_PECI_Tj_max + _R5;
            }
        }
    }
}

/*-----------------------------------------------------------------------------
 * @subroutine - Process_eSPI_OOB_Message
 * @function - Process_eSPI_OOB_Message
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
BYTE Process_eSPI_OOB_Message(void)
{
    /* Check upstream authority enable */
    if (!OOB_Check_Upstream_Authority_EN())
    {
        return FALSE;
    }

    _R5 = *OOB_Table_Pntr;
    OOB_Table_Pntr++;
    _R6 = OOB_Message;
    if (_R5 == 0x21)
    {
        _R6 = OOB_Message;
    }

    ESUCTRL1 = _R6;            //cycle type
    _R7 = *OOB_Table_Pntr;
    OOB_Table_Pntr++;
    ESUCTRL2 = _R7;     //tag + length[11:8],

    _R7 = *OOB_Table_Pntr;
    OOB_Table_Pntr++;
    xOOB_PacketLength = _R7;
    ESUCTRL3 = xOOB_PacketLength;   //length[7:0],
    _R6 = xOOB_PacketLength;
    _R5 = 0;
    while (_R6)
    {
        register uint32_t len = (_R6 <= 4 ? _R6 : 4);
        _R6 -= len;
        uDword data_temp;
        for (register uint32_t i = 0; i < len; i++)
        {
            data_temp.byte[i] = *OOB_Table_Pntr;
            OOB_Table_Pntr++;
        }
        REG32(UPSTREAM_DATA + _R5) = data_temp.dword;
        _R5++;
    }

#if 1
    /* Patch this can let data stable ? */
    ESOCTRL0 = PUT_OOB_STATUS;  /* Write clear for next OOB receive */
#endif
    ESUCTRL0 |= Upstream_EN;    //Set upstream enable
    ESUCTRL0 |= Upstream_GO;    //Set upstream go

    /* Check upstream done */
    // if (!OOB_Check_Upstream_Done())
    // {
    //     return FALSE;
    // }

    /* Check upstream authority auto disable */
    if (!OOB_Check_Upstream_Authority_Disable())
    {
        return FALSE;
    }

    /* Check put_oob status */
    if (!OOB_Check_OOB_Status())
    {
        return FALSE;
    }

    /* Delay 15.26 us */
    // vDelayXus(16);

    /* Store Put_OOB Length to xOOB_PacketLength */
    _R5 = ESOCTRL4;
    xOOB_PacketLength = _R5;
    if (_R5 > xOOB_PacketMaxLength)
    {
        _R5 = xOOB_PacketMaxLength;
    }
    _R6 = 0;
    while (_R5 > 0)
    {
        /* Read OOB return data */
        *Tmp_XPntr = (VBYTE)((REG32(PUT_OOB_DATA + (_R6 / 4))) >> ((_R6 % 4) * 8));
        _R6++;
        Tmp_XPntr++;
        _R5--;
    }

    // ESOCTRL0 = PUT_OOB_STATUS;  /* Write clear for next OOB receive */
    xOOB_DataCounter++;
    return TRUE;
}

BYTE eSPI_OOB_Receive(BYTE *OOB_Meg_Table)
{
    /* Check put_oob status */
    if (!OOB_Check_OOB_Status())
    {
        return FALSE;
    }
    Tmp_XPntr = OOB_Meg_Table;
    /* Store Put_OOB Length to xOOB_PacketLength */
    _R5 = ESOCTRL4;
    xOOB_PacketLength = _R5;
    if (xOOB_PacketLength == 0)
        return FALSE;
    _R6 = 0;
    while (_R5 > 0)
    {
        /* Read OOB return data */
        *Tmp_XPntr = (VBYTE)((REG32(PUT_OOB_DATA + (_R6 / 4))) >> ((_R6 % 4) * 8));
        _R6++;
        Tmp_XPntr++;
        _R5--;
    }

    // ESOCTRL0 = PUT_OOB_STATUS;  /* Write clear for next OOB receive */
    return TRUE;
}

BYTE eSPI_OOB_Send(BYTE *OOB_Meg_Table)
{
    /*check if upstream is busy*//* Check upstream authority enable */
    if ((ESUCTRL0 & Upstream_Busy) || (!OOB_Check_Upstream_Authority_EN()))
        return FALSE;

    OOB_Table_Pntr = OOB_Meg_Table;
    _R5 = *OOB_Table_Pntr;
    OOB_Table_Pntr++;
    _R6 = OOB_Message;
    if (_R5 == 0x21)
    {
        _R6 = OOB_Message;
    }
    ESUCTRL1 = _R6;            //cycle type
    _R7 = *OOB_Table_Pntr;
    OOB_Table_Pntr++;
    ESUCTRL2 = _R7;     //tag + length[11:8],

    _R7 = *OOB_Table_Pntr;
    OOB_Table_Pntr++;
    xOOB_PacketLength = _R7;
    ESUCTRL3 = xOOB_PacketLength;   //length[7:0],

    _R6 = xOOB_PacketLength;
    _R5 = 0;
    while (_R6)
    {
        register uint32_t len = (_R6 <= 4 ? _R6 : 4);
        _R6 -= len;
        uDword data_temp;
        for (register uint32_t i = 0; i < len; i++)
        {
            data_temp.byte[i] = *OOB_Table_Pntr;
            OOB_Table_Pntr++;
        }
        REG32(UPSTREAM_DATA + _R5) = data_temp.dword;
        _R5++;
    }

#if 0
    /* Patch this can let data stable ? */
    // ESOCTRL0 = PUT_OOB_STATUS;  /* Write clear for next OOB receive */
#endif
    ESUCTRL0 |= Upstream_EN;    //Set upstream enable
    ESUCTRL0 |= Upstream_GO;    //Set upstream go

    /* Check upstream authority auto disable */
    if (!OOB_Check_Upstream_Authority_Disable())
    {
        return FALSE;
    }
    return TRUE;
}
/*-----------------------------------------------------------------------------
 * @subroutine - Process_eSPI_OOB_CrashLog
 * @function - Process_eSPI_OOB_CrashLog
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
void Process_eSPI_OOB_CrashLog(void)
{
    _R5 = *OOB_Table_Pntr;
    OOB_Table_Pntr++;
    _R6 = OOB_Message;
    if (_R5 == 0x21)
    {
        _R6 = OOB_Message;
    }
    ESUCTRL1 = _R6;             // cycle type
    ESUCTRL2 = *OOB_Table_Pntr; // tag + length[11:8],
    OOB_Table_Pntr++;
    xOOB_PacketLength = *OOB_Table_Pntr;
    OOB_Table_Pntr++;
    _R6 = xOOB_PacketLength;
    _R5 = 0;
    while (_R6)
    {
        register uint32_t len = (_R6 <= 4 ? _R6 : 4);
        _R6 -= len;
        uDword data_temp;
        for (register uint32_t i = 0; i < len; i++)
        {
            data_temp.byte[i] = *OOB_Table_Pntr;
            OOB_Table_Pntr++;
        }
        REG32(UPSTREAM_DATA + _R5) = data_temp.dword;
        _R5++;
    }

    ESUCTRL3 = xOOB_PacketLength; // length[7:0],

    ESUCTRL0 |= Upstream_EN; // set upstream enable
    ESUCTRL0 |= Upstream_GO; // set upstream go

    /* Check upstream done */
    if (!OOB_Check_Upstream_Done())
    {
        return;
    }
    /* Check OOB status */
    xOOB_Failed = 0;
    xOOB_Timeout = 255;
    while (1)
    {
        if (ESOCTRL0 & PUT_OOB_STATUS)
        {
            break;
        }
        if (xOOB_Timeout > 0)
        {
            xOOB_Timeout--;
        }
        else
        {
            xOOB_Failed = 1;
            break;
        }
        /* Delay 15.26 us */
        vDelayXus(16);
    }
#if 0 /* 1: Skip Failed */
    if (xOOB_Failed > 0)
    {
        xOOB_FailedCounter++;
        ESUCTRL0 |= Upstream_Done;
        ESOCTRL0 = PUT_OOB_STATUS;  /* Write clear for next OOB receive */
        return;
    }
#endif
    /* Store Put_OOB Length to xOOB_PacketLength */
    _R5 = ESOCTRL4;
    xOOB_PacketLength = _R5;
    if (_R5 > xOOB_PacketMaxLength)
    {
        _R5 = xOOB_PacketMaxLength;
    }
    _R6 = 0;
    while (_R5 > 0)
    {
        /* Read OOB return data */
        *Tmp_XPntr = (VBYTE)((REG32(PUT_OOB_DATA + (_R6 / 4))) >> ((_R6 % 4) * 8));
        _R6++;
        Tmp_XPntr++;
        _R5--;
    }
    ESUCTRL0 |= Upstream_Done;
    ESOCTRL0 = PUT_OOB_STATUS; /* Write clear for next OOB receive */
    xOOB_DataCounter++;
}

/*-----------------------------------------------------------------------------
 * @subroutine - eSPI_Flash_Read
 * @function - eSPI_Flash_Read
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
BYTE eSPI_Flash_Read(BYTE addr3, BYTE addr2, BYTE addr1, BYTE addr0,
    BYTE length, VBYTE *bufferindex)
{
    if ((IS_MASK_CLEAR(REG_3117, F_FLASH_CHN_READY)) ||
        (IS_MASK_CLEAR(REG_3117, F_FLASH_CHN_ENABLE)))
    {
        return FALSE;
    }
    if ((length == 0) || (length > 64))
    {
        return FALSE;
    }

    ESUCTRL1 = OOB_Flash_Read; // cycle type
    ESUCTRL2 = 0x20; // tag + length[11:8]
    ESUCTRL3 = length; // length[7:0]   ,max support  64 bytes

    REG32(UPSTREAM_DATA) = (DWORD)(addr3 | (addr2 << 8) | (addr1 << 16) | (addr0 << 24));

    ESUCTRL0 |= Upstream_EN;    //Set upstream enable
    ESUCTRL0 |= Upstream_GO;    //Set upstream go

    /* Check upstream done */
    if (!OOB_Check_Upstream_Done())
    {
        return FALSE;
    }

    /* Check PUT_FLASH_C cycle Type */
    xOOB_Failed = 0;
    xOOB_Timeout = 255;
    while (1)
    {
        /* Check PUT_FLASH_C cycle Type */
        if ((ESUCTRL6 & 0x0F) ==
            Successful_Completion_With_Data_the_only_Completion)
        {
            break;
        }
        if (xOOB_Timeout > 0)
        {
            xOOB_Timeout--;
        }
        else
        {
            xOOB_Failed = 1;
            break;
        }
        /* Delay 15.26 us */
        vDelayXus(16);
    }
    if (xOOB_Failed > 0)
    {
        xOOB_FailedCounter++;
        ESUCTRL0 |= Upstream_Done;  /* Wrtie-1 to clear Upstream Done */
        ESOCTRL0 = PUT_OOB_STATUS;  /* Write clear for next OOB receive */
        return FALSE;
    }
    /* Store Put_OOB Length to xOOB_PacketLength */
    _R5 = length;
    _R6 = 0;
    while (_R5 > 0)
    {
        /* Read OOB return data */
        *bufferindex = (VBYTE)((REG32(UPSTREAM_DATA + (_R6 / 4))) >> ((_R6 % 4) * 8));
        _R6++;
        bufferindex++;
        _R5--;
    }

    ESUCTRL0 |= Upstream_Done;  /* Wrtie-1 to clear Upstream Done */
    // ESOCTRL0 = PUT_OOB_STATUS;  /* Write clear for next OOB receive */
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * @subroutine - eSPI_Flash_Erase
 * @function - eSPI_Flash_Erase
 * @upstream - By call
 * @input    - mode: 0x01:4K Byte erase 0x02:64K Byte erase
 * @return   - None
 * @note     - None
 */
BYTE eSPI_Flash_Erase(BYTE addr3, BYTE addr2, BYTE addr1, BYTE addr0, BYTE mode)
{
    if ((IS_MASK_CLEAR(REG_3117, F_FLASH_CHN_READY)) ||
        (IS_MASK_CLEAR(REG_3117, F_FLASH_CHN_ENABLE)))
    {
        return FALSE;
    }
    ESUCTRL1 = OOB_Flash_Erase; // cycle type
    ESUCTRL2 = 0x20; // tag + length[11:8],

    /* ESUCTRL3 001:Support 4K Byte erase
                010:Support 64K Byte erase
                011:Support 4K && 64K Byte erase
    */
    ESUCTRL3 = mode;
    REG32(UPSTREAM_DATA) = (DWORD)(addr3 | (addr2 << 8) | (addr1 << 16) | (addr0 << 24));

    ESUCTRL0 |= Upstream_EN;    //Set upstream enable
    ESUCTRL0 |= Upstream_GO;    //Set upstream go

    /* Check upstream done */
    if (!OOB_Check_Upstream_Done())
    {
        return FALSE;
    }
    /* Check PUT_FLASH_C cycle Type */
    if ((ESUCTRL6 & 0x0F) == Successful_Completion_Without_Data)
    {
        ESUCTRL0 |= Upstream_Done;  /* Wrtie-1 to clear Upstream Done */
        // ESOCTRL0 = PUT_OOB_STATUS;  /* Write clear for next OOB receive */
        return TRUE;
    }

    ESUCTRL0 |= Upstream_Done;  /* Wrtie-1 to clear Upstream Done */
    // ESOCTRL0 = PUT_OOB_STATUS;  /* Write clear for next OOB receive */
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * @subroutine - eSPI_Flash_Write
 * @function - eSPI_Flash_Write
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
BYTE eSPI_Flash_Write(BYTE addr3, BYTE addr2, BYTE addr1, BYTE addr0,
    BYTE length, VBYTE *bufferindex)
{
    if ((IS_MASK_CLEAR(REG_3117, F_FLASH_CHN_READY)) ||
        (IS_MASK_CLEAR(REG_3117, F_FLASH_CHN_ENABLE)))
    {
        return FALSE;
    }

    // ESOCTRL0 = PUT_OOB_STATUS; /* Write clear for next OOB receive */
    ESUCTRL1 = OOB_Flash_Write; // cycle type
    ESUCTRL2 = 0x20; // tag + length[11:8]
    ESUCTRL3 = length; // length[7:0]   ,max support  64 bytes
    REG32(UPSTREAM_DATA) = (DWORD)(addr3 | (addr2 << 8) | (addr1 << 16) | (addr0 << 24));

    _R5 = 1;
    _R6 = length;
    while (_R6)
    {
        register uint32_t len = (_R6 <= 4 ? _R6 : 4);
        _R6 -= len;
        uDword data_temp;
        for (register uint32_t i = 0; i < len; i++)
        {
            data_temp.byte[i] = *bufferindex;
            bufferindex++;
        }
        REG32(UPSTREAM_DATA + _R5) = data_temp.dword;
        _R5++;
    }

    ESUCTRL0 |= Upstream_EN;    //Set upstream enable
    ESUCTRL0 |= Upstream_GO;    //Set upstream go

    /* Check upstream done */
    if (!OOB_Check_Upstream_Done())
    {
        return FALSE;
    }
    ESUCTRL0 |= Upstream_Done; // upstream done write 1 clear

    /* Check PUT_FLASH_C cycle Type */
    if ((ESUCTRL6 & 0x0F) == Successful_Completion_Without_Data)
    {
        ESUCTRL0 |= Upstream_Done;  /* Wrtie-1 to clear Upstream Done */
        ESOCTRL0 = PUT_OOB_STATUS;  /* Write clear for next OOB receive */
        return TRUE;
    }

    ESUCTRL0 |= Upstream_Done;  /* Wrtie-1 to clear Upstream Done */
    ESOCTRL0 = PUT_OOB_STATUS;  /* Write clear for next OOB receive */

    return FALSE;
}

#if SUPPORT_OOB_PECI_POWER_CTRL
/*-----------------------------------------------------------------------------
 * @subroutine - PECI_ReadPowerUnit
 * @function - The function of PECI3.0 Package read power unit
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - Package Power SKU UNIT Read
 * This feature enables the PECI host to read the units of time, energy and
 * power used in the processor and DRAM power control registers for
 * calculating power and timing parameters. The default value of the power
 * unit field [19:16] is 0111b,energy unit [11:8] is 0000b
 */
void OOB_PECI_ReadPowerUnit(void)
{
    if (OOB_PECI_RdPkgConfig(_PECI_CPU_ADDR, &PECI_ReadBuffer[0],
        _PECI_Domain_0,
        0, _PECI_Index_PPSH, 0, 0, 5, 5))
    {
        PECI_PowerUnit = (PECI_ReadBuffer[0] & 0x0F);  /* Bit[3:0] */
        PECI_EnergyUnit = (PECI_ReadBuffer[1] & 0x1F); /* Bit[12:8] */
        PECI_TimeUnit = (PECI_ReadBuffer[2] & 0x0F);   /* Bit[19:16] */

        PECI_UnitEnergy = 1 << PECI_EnergyUnit;
        PECI_UnitTime = 1 << PECI_TimeUnit;
        PECI_UnitPower = 1 << PECI_PowerUnit;
    }
    if (PECI_UnitEnergy == 0)
    {
        PECI_UnitEnergy = 1;
    }
    if (PECI_UnitTime == 0)
    {
        PECI_UnitTime = 1;
    }
    if (PECI_UnitPower == 0)
    {
        PECI_UnitPower = 1;
    }
}

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_RdPkgConfig
 * @function - Read to the package configuration space (PCS) within the
 *             processor
 * @upstream - Hook_Timer10msEventB
 * @input    - (1) addr : The address of processor
 *             (2) *ReadData : the start address of variable to save data
 *             (3) Domain : 0 or 1?
 *                          0 : Domain 0, 1 : Domain 1?
 *             (4) Retry   0 or 1
 *             (5) Index
 *             (6) LSB of parameter
 *             (7) MSB of parameter
 *             (8) ReadLen read length 2 or 3 or 5
 *             (9) WriteLen write length 5
 *
 * @return   - 1 : done     0 : error
 * @note     - None
 */
BYTE OOB_PECI_RdPkgConfig(BYTE addr, BYTE *ReadData,
    BYTE Domain, BYTE Retry, BYTE Index,
    BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen)
{
    ESUCTRL1 = OOB_Message; // eSPI Cycle Type
    ESUCTRL2 = 0x20;        // Tag[3:0]+Length[11:8],
    ESUCTRL3 = 12;          // Length[7:0]=N+3
    *UPSTREAM_DATA = (DWORD)(0x0F << 24) + (DWORD)(0x09 << 16) + (DWORD)(0x01 << 8) + (DWORD)0x20;  //eSPI Slave 0/EC, Byte Count N,PECI Command,PCH

    if (Domain < 2)
    {
        *(UPSTREAM_DATA + 1) = ((PECI_CMD_RdPkgConfig + Domain) << 24) + (ReadLen << 16) + (WriteLen << 8) + addr; //PECI Target Address,WriteLen,ReadLen,Domain
    }
    else
    {
        *(UPSTREAM_DATA + 1) = (PECI_CMD_RdPkgConfig << 24) + (ReadLen << 16) + (WriteLen << 8) + addr; //PECI Target Address,WriteLen,ReadLen,Domain
    }
    if (Retry < 2)
    {
        *(UPSTREAM_DATA + 2) = (MSB << 24) + (LSB << 16) + (Index << 8) + ((_PECI_HostID << 1) + Retry); //INDEX,LSB.MSB,RETRY
    }
    else
    {
        *(UPSTREAM_DATA + 2) = (MSB << 24) + (LSB << 16) + (Index << 8) + (_PECI_HostID << 1); //INDEX,LSB.MSB,RETRY
    }

    ESUCTRL0 |= Upstream_EN; // set upstream enable
    ESUCTRL0 |= Upstream_GO; // set upstream go

    /* Check upstream done */
    if (!OOB_Check_Upstream_Done())
    {
        return FALSE;
    }
    /* Check upstream done */
    if (!OOB_Check_OOB_Status())
    {
        return FALSE;
    }
    /* Store Put_OOB Length to xOOB_PacketLength */
    _R5 = ESOCTRL4;
    xOOB_PacketLength = _R5;
    if (_R5 > 16)
    {
        _R5 = 16;
    }

    _R6 = 0;
    while (_R5 > 0)
    {
        /* Read OOB return data */
        *ReadData = (VBYTE)((REG32(PUT_OOB_DATA + (_R6 / 4))) >> ((_R6 % 4) * 8));
        _R6++;
        ReadData++;
        _R5--;
    }

    ESOCTRL0 = PUT_OOB_STATUS; /* Write clear for next OOB receive */
    xOOB_DataCounter++;
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_WrPkgConfig
 * @function - Write to the package configuration space (PCS) within the
 *             processor
 * @upstream - Hook_Timer10msEventB
 * @input    - (1) addr : The address of processor
 *             (2) *WriteData : the start address of variable to wirte data
 *             (3) Domain : 0 or 1?
 *                           0 : Domain 0, 1 : Domain 1?
 *             (4) Retry   0 or 1
 *             (5) Index
 *             (6) LSB of parameter
 *             (7) MSB of parameter
 *             (8) ReadLen read length 1
 *             (9) WriteLen write length 0x07 or 0x08 or 0x0A
 *
 * @return   - 1 : done     0 : error
 * @note     - None
 */
BYTE OOB_PECI_WrPkgConfig(BYTE addr, BYTE *WriteData,
    BYTE Domain, BYTE Retry, BYTE Index,
    BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen)
{
    ESUCTRL1 = OOB_Message;  // eSPI Cycle Type
    ESUCTRL2 = 0x20;         // Tag[3:0]+Length[11:8],
    ESUCTRL3 = 5 + WriteLen; // Length[7:0]=N+3
    *UPSTREAM_DATA = (0x0F << 24) + ((2 + WriteLen) << 16) + (0x01 << 8) + 0x20;  //eSPI Slave 0/EC, Byte Count N,PECI Command,PCH

    if (Domain < 2)
    {
        *(UPSTREAM_DATA + 1) = ((PECI_CMD_WrPkgConfig + Domain) << 24) + (ReadLen << 16) + (WriteLen << 8) + addr; //PECI Target Address,WriteLen,ReadLen,Domain
    }
    else
    {
        *(UPSTREAM_DATA + 1) = (PECI_CMD_WrPkgConfig << 24) + (ReadLen << 16) + (WriteLen << 8) + addr; //PECI Target Address,WriteLen,ReadLen,Domain
    }
    if (Retry < 2)
    {
        *(UPSTREAM_DATA + 2) = (MSB << 24) + (LSB << 16) + (Index << 8) + ((_PECI_HostID << 1) + Retry); //INDEX,LSB.MSB,RETRY
    }
    else
    {
        *(UPSTREAM_DATA + 2) = (MSB << 24) + (LSB << 16) + (Index << 8) + (_PECI_HostID << 1); //INDEX,LSB.MSB,RETRY
    }

    // _R6 = 12;
#if 0
    /* Removed Reserved Bytes */
    //for (_R5 = 0; _R5 < (WriteLen - 7); _R5++)
#endif

    _R6 = 3;
    _R5 = WriteLen - 6;
    while (_R5)
    {
        register uint32_t len = (_R5 <= 4 ? _R5 : 4);
        _R5 -= len;
        uDword data_temp;
        for (register uint32_t i = 0; i < len; i++)
        {
            data_temp.byte[i] = *WriteData;
            WriteData++;
        }
        REG32(UPSTREAM_DATA + _R6) = data_temp.dword;
        _R6++;
    }

    ESUCTRL0 |= Upstream_EN; // set upstream enable
    ESUCTRL0 |= Upstream_GO; // set upstream go

    /* Check upstream done */
    if (!OOB_Check_Upstream_Done())
    {
        return FALSE;
    }
    /* Check upstream done */
    if (!OOB_Check_OOB_Status())
    {
        return FALSE;
    }

    ESOCTRL0 = PUT_OOB_STATUS; /* Write clear for next OOB receive */
    xOOB_DataCounter++;
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_ReadPowerLimit1
 * @function - The function of PECI3.0 Package read power limit 1
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
void OOB_PECI_ReadPowerLimit1(void)
{
    if (PECI_UnitPower == 0)
    {
        OOB_PECI_ReadPowerUnit();
        if (PECI_UnitPower == 0)
        {
            return;
        }
    }
    if (OOB_PECI_RdPkgConfig(_PECI_CPU_ADDR, &PECI_ReadBuffer[0],
        _PECI_Domain_0,
        0, _PECI_Index_PPL1, 0, 0, 5, 5))
    {

        _C1 = (PECI_ReadBuffer[7] << 8) + PECI_ReadBuffer[6];
        _C1 &= 0x7FFF;
        PECI_PowerLimit1 = (_C1 / PECI_UnitPower);

        _R5 = PECI_ReadBuffer[8] >> 1;
        if (PECI_UnitTime > 0)
        {
            PECI_PowerLimit1T = (_R5 / PECI_UnitTime);
        }
    }
    else
    {
        PECI_PowerLimit1 = 0xFF;
    }
}

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_WritePowerLimit1
 * @function - The function of PECI3.0 Package write power limit 1
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
void OOB_PECI_WritePowerLimit1(BYTE SettingWatts, BYTE SettingTimer)
{
    _C1 = (SettingWatts * PECI_UnitPower);
    PECI_WriteBuffer[0] = _C1;
    PECI_WriteBuffer[1] = (_C1 >> 8) | 0x80;

    _R5 = (SettingTimer * PECI_UnitTime) << 1;
    PECI_WriteBuffer[2] = _R5 | 0x01; // Bit0: Clamp Mode

    if (OOB_PECI_WrPkgConfig(_PECI_CPU_ADDR, &PECI_WriteBuffer[0],
        _PECI_Domain_0,
        0, _PECI_Index_PPL1, 0, 0, 1, 10))
    {
        ;
    }
}

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_ReadPowerLimit2
 * @function - The function of PECI3.0 Package read power limit 2
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
void OOB_PECI_ReadPowerLimit2(void)
{
    if (PECI_UnitPower == 0)
    {
        OOB_PECI_ReadPowerUnit();
        if (PECI_UnitPower == 0)
        {
            return;
        }
    }
    if (OOB_PECI_RdPkgConfig(_PECI_CPU_ADDR, &PECI_ReadBuffer[0],
        _PECI_Domain_0,
        0, _PECI_Index_PPL2, 0, 0, 5, 5))
    {
        _C1 = (PECI_ReadBuffer[7] << 8) + PECI_ReadBuffer[6];
        _C1 &= 0x7FFF;
        PECI_PowerLimit2 = (_C1 / PECI_UnitPower);
    }
    else
    {
        PECI_PowerLimit2 = 0xFF;
    }
}

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_WritePowerLimit2
 * @function - The function of PECI3.0 Package write power limit 2
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
void OOB_PECI_WritePowerLimit2(BYTE SettingWatts)
{
    _C1 = (PECI_UnitPower * SettingWatts);
    PECI_WriteBuffer[0] = _C1;
    PECI_WriteBuffer[1] = (_C1 >> 8) | 0x80;
    PECI_WriteBuffer[2] = 0;
    PECI_WriteBuffer[3] = 0;

    if (OOB_PECI_WrPkgConfig(_PECI_CPU_ADDR, &PECI_WriteBuffer[0],
        _PECI_Domain_0,
        0, _PECI_Index_PPL2, 0, 0, 1, 10))
    {
        ;
    }
}

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_ReadPowerLimit3
 * @function - The function of PECI3.0 Package read power limit 3
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
void OOB_PECI_ReadPowerLimit3(void)
{
    if (PECI_UnitPower == 0)
    {
        OOB_PECI_ReadPowerUnit();
        return;
    }
    if (OOB_PECI_RdPkgConfig(_PECI_CPU_ADDR, &PECI_ReadBuffer[0],
        _PECI_Domain_0,
        0, _PECI_Index_PPL3, 0, 0, 5, 5))
    {
        _C1 = (PECI_ReadBuffer[7] << 8) + PECI_ReadBuffer[6];
        _C1 &= 0x7FFF;
        PECI_PowerLimit3 = (_C1 / PECI_UnitPower);
    }
    else
    {
        PECI_PowerLimit3 = 0xFF;
    }
}

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_WritePowerLimit3
 * @function - The function of PECI3.0 Package write power limit 3
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
void OOB_PECI_WritePowerLimit3(BYTE SettingWatts)
{
    _C1 = (PECI_UnitPower * SettingWatts);
    PECI_WriteBuffer[0] = _C1;
    PECI_WriteBuffer[1] = (_C1 >> 8) | 0x80;
    PECI_WriteBuffer[2] = 0;
    PECI_WriteBuffer[3] = 0;

    if (OOB_PECI_WrPkgConfig(_PECI_CPU_ADDR, &PECI_WriteBuffer[0],
        _PECI_Domain_0,
        0, _PECI_Index_PPL3, 0, 0, 1, 10))
    {
        ;
    }
}

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_ReadPowerLimit4
 * @function - The function of PECI3.0 Package read power limit 4
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
void OOB_PECI_ReadPowerLimit4(void)
{
    if (PECI_UnitPower == 0)
    {
        OOB_PECI_ReadPowerUnit();
        return;
    }
    if (OOB_PECI_RdPkgConfig(_PECI_CPU_ADDR, &PECI_ReadBuffer[0],
        _PECI_Domain_0,
        0, _PECI_Index_PPL4, 0, 0, 5, 5))
    {
        _C1 = (PECI_ReadBuffer[7] << 8) + PECI_ReadBuffer[6];
        _C1 &= 0x7FFF;
        PECI_PowerLimit4 = (_C1 / PECI_UnitPower);
    }
    else
    {
        PECI_PowerLimit4 = 0xFF;
    }
}

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_WritePowerLimit4
 * @function - The function of PECI3.0 Package write power limit 4
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
void OOB_PECI_WritePowerLimit4(BYTE SettingWatts)
{
    if (PECI_UnitPower == 0)
    {
        OOB_PECI_ReadPowerUnit();
        if (PECI_UnitPower == 0)
        {
            return;
        }
    }
    _C1 = (SettingWatts * PECI_UnitPower);
    PECI_WriteBuffer[0] = _C1;
    PECI_WriteBuffer[1] = (_C1 >> 8) | 0x80;
    PECI_WriteBuffer[2] = 0;
    PECI_WriteBuffer[3] = 0;

    if (OOB_PECI_WrPkgConfig(_PECI_CPU_ADDR, &PECI_WriteBuffer[0],
        _PECI_Domain_0,
        0, _PECI_Index_PPL4, 0, 0, 1, 10))
    {
        ;
    }
}
#endif // SUPPORT_OOB_PECI_POWER_CTRL

/*-----------------------------------------------------------------------------
 * @subroutine - Get_OOB_PMC_CrashLog
 * @function - Get_OOB_PMC_CrashLog
 * @Upstream - Service_OOB_Message
 * @input    - None
 * @return   - None
 * @note     - How to get failure log? Access Memory 4C000000 in Windows OS.
 */
 // void Get_OOB_PMC_CrashLog(void)
 // {
 //     BYTE    i, j;

 //     if(xOOB_GET_CRASHLOG == _CRASHLOG_ERASE_DATA)
 //     {
 //         /* Erase eFlash Data Sectors 0x01B000 ~ 0x01FFFF */
 //         xOOB_CrashLogAddrH = _EFLASH_CRASHLOG_ERASE_ADDR >> 16;
 //         xOOB_CrashLogAddrM = _EFLASH_CRASHLOG_ERASE_ADDR >> 8;
 //     #if 1   /* 0: Debug */
 //         Do_SPI_Read_ID();
 //         Do_SPIM_Write_Enable();
 //         Do_SPI_Write_Status(SPIStatus_UnlockAll);
 //         i = 0;
 //         while(1)
 //         {
 //             Do_SPIM_Write_Enable();
 //             Do_SPI_Erase(SPICmd_SectorErase,
 //                          xOOB_CrashLogAddrH,
 //                          xOOB_CrashLogAddrM,
 //                          0x00);
 //             Do_SPI_Write_Disable();
 //             xOOB_CrashLogAddrM += 0x04;
 //             if(xOOB_CrashLogAddrM == 0)
 //             {
 //                 xOOB_CrashLogAddrH++;
 //             }
 //             i++;
 //             if(i >= _EFLASH_CRASHLOG_SIZE_K)
 //             {
 //                 break;
 //             }
 //         }
 //     #endif
 //         xOOB_GET_CRASHLOG = _CRASHLOG_GET_CPU_SIZES;
 //         return;
 //     }
 //     else if(xOOB_GET_CRASHLOG == _CRASHLOG_GET_CPU_SIZES)
 //     {
 //         PECI_RdPkgConfig(_PECI_CPU_ADDR,
 //                         &PECI_ReadBuffer[0],
 //                         _PECI_Domain_0,
 //                         0,
 //                         73,
 //                         0,
 //                         0,
 //                         9,
 //                         5);
 //         wOOB_CrashCpuDataLines = (WORD)(PECI_ReadBuffer[1] << 8) +
 //             PECI_ReadBuffer[0];
 //         wOOB_CrashCpuDataBytes = wOOB_CrashCpuDataLines * 8;
 //         wOOB_CrashLogDataIndex = 0;
 //         xOOB_CrashCpuIndex = 0;
 //         xOOB_CrashCpuIndexH = 0;
 //         xOOB_CrashLogAddrH = _EFLASH_CRASHLOG_CPU_ADRH;
 //         xOOB_CrashLogAddrM = _EFLASH_CRASHLOG_CPU_ADRM;
 //         xOOB_GET_CRASHLOG = _CRASHLOG_READ_CPU_LOG;
 //         return;
 //     }
 //     else if(xOOB_GET_CRASHLOG == _CRASHLOG_READ_CPU_LOG)
 //     {
 //         i = 0;
 //         while(1)
 //         {
 //             PECI_RdPkgConfig(_PECI_CPU_ADDR,
 //                             &eSPI_OOB_DATA[i],
 //                             _PECI_Domain_0,
 //                             0,
 //                             74,
 //                             xOOB_CrashCpuIndex,
 //                             xOOB_CrashCpuIndexH,
 //                             9,
 //                             5);
 //             /* Get First 16 Bytes length inforamtion and update total bytes. */
 //             if((xOOB_CrashCpuIndexH == 0) && (xOOB_CrashCpuIndex == 0))
 //             {
 //                 /* eSPI_OOB_DATA[4][5]:= DWORD unit. (4 Bytes) */
 //                 wOOB_CrashCpuDataBytes = (WORD)(eSPI_OOB_DATA[5] << 8) +
 //                     eSPI_OOB_DATA[4];
 // /* Protocol is 8 Bytes per read */
 //                 wOOB_CrashCpuDataBytes <<= 2;
 //             }

 //             xOOB_CrashCpuIndex++;
 //             if(xOOB_CrashCpuIndex == 0)
 //             {
 //                 xOOB_CrashCpuIndexH++;
 //             }
 //             wOOB_CrashLogDataIndex += 8;
 //             i += 8;
 //             if(i == 0)
 //             {
 //                 break;
 //             }
 //         }
 //         xOOB_GET_CRASHLOG = _CRASHLOG_SAVE_CPU_LOG;
 //         return;
 //     }
 //     else if(xOOB_GET_CRASHLOG == _CRASHLOG_SAVE_CPU_LOG)
 //     {
 //     #if 1   /* 0: Debug */
 //         Do_SPI_Read_ID();
 //         Do_SPIM_Write_Enable();
 //         Do_SPI_Write_Status(SPIStatus_UnlockAll);
 //         Do_SPIM_Write_Enable();
 //         Do_SPI_Write_256Bytes(&eSPI_OOB_DATA[0],
 //                               xOOB_CrashLogAddrH,
 //                               xOOB_CrashLogAddrM,
 //                               0x00);
 //         xOOB_CrashLogAddrM += 1;
 //         Do_SPI_Write_Disable();
 //     #endif
 //         if(wOOB_CrashLogDataIndex >= wOOB_CrashCpuDataBytes)
 //         {
 //             xOOB_GET_CRASHLOG = _CRASHLOG_EXIT_CPU_LOG;
 //             return;
 //         }
 //         xOOB_GET_CRASHLOG = _CRASHLOG_READ_CPU_LOG;
 //         return;
 //     }
 //     else if(xOOB_GET_CRASHLOG == _CRASHLOG_EXIT_CPU_LOG)
 //     {
 //         xOOB_CrashLogAddrH = _EFLASH_CRASHLOG_PCH_ADRH;
 //         xOOB_CrashLogAddrM = _EFLASH_CRASHLOG_PCH_ADRM;
 //         wOOB_CrashPchDataSizes = 0;
 //         wOOB_CrashLogDataIndex = 0;
 //         xOOB_GET_CRASHLOG = _CRASHLOG_READ_PCH_LOG;
 //         if(ECHIPVER <= _CHIP_CX)
 //         {
 //             xOOB_GET_CRASHLOG = _CRASHLOG_SAVE_INFO;
 //         }
 //         return;
 //     }
 //     else if(xOOB_GET_CRASHLOG == _CRASHLOG_READ_PCH_LOG)
 //     {
 //         OOB_Table_Pntr = &TO_PCH_PMC_CRASHLOG;
 //         Tmp_XPntr = &eSPI_FLASH_DATA[0];
 //         xOOB_PacketMaxLength = 128;
 //         Process_eSPI_OOB_CrashLog();
 //         if(xOOB_PacketLength == 4)
 //         {
 //             xOOB_GET_CRASHLOG = _CRASHLOG_EXIT_PCH_LOG;
 //             return;
 //         }
 //         i = 4;   /* Data Start at 4th Byte */
 //         while(1)
 //         {
 //             j = (wOOB_CrashLogDataIndex & 0xFF);
 //             eSPI_OOB_DATA[j] = eSPI_FLASH_DATA[i];
 //             wOOB_CrashLogDataIndex++;
 //             wOOB_CrashPchDataSizes++;
 //             i++;
 //             if(i > 67)
 //             {
 //                 break;
 //             }
 //         }
 //         xOOB_GET_CRASHLOG = _CRASHLOG_SAVE_PCH_LOG;
 //         return;
 //     }
 //     else if(xOOB_GET_CRASHLOG == _CRASHLOG_SAVE_PCH_LOG)
 //     {
 //         if(wOOB_CrashLogDataIndex > 255)
 //         {
 //             wOOB_CrashLogDataIndex = 0;
 //         #if 1   /* 0: Debug */
 //             Do_SPI_Read_ID();
 //             Do_SPIM_Write_Enable();
 //             Do_SPI_Write_Status(SPIStatus_UnlockAll);
 //             Do_SPIM_Write_Enable();
 //             Do_SPI_Write_256Bytes(&eSPI_OOB_DATA[0],
 //                                   xOOB_CrashLogAddrH,
 //                                   xOOB_CrashLogAddrM,
 //                                   0x00);
 //             Do_SPI_Write_Disable();
 //         #endif
 //             xOOB_CrashLogAddrM++;
 //             /* Clear 256 Bytes Buffer */
 //             i = 0;
 //             while(1)
 //             {
 //                 eSPI_OOB_DATA[i] = 0;
 //                 eSPI_FLASH_DATA[i] = 0;
 //                 i++;
 //                 if(i == 0)
 //                 {
 //                     break;
 //                 }
 //             }
 //         }
 //         if(xOOB_PacketLength == 0x44)
 //         {
 //             xOOB_GET_CRASHLOG = _CRASHLOG_READ_PCH_LOG;
 //             return;
 //         }
 //         /* Save last data */
 //         xOOB_GET_CRASHLOG = _CRASHLOG_EXIT_PCH_LOG;
 //         return;
 //     }
 //     else if(xOOB_GET_CRASHLOG == _CRASHLOG_EXIT_PCH_LOG)
 //     {
 //         if(wOOB_CrashLogDataIndex > 0)
 //         {
 //             wOOB_CrashLogDataIndex = 0;
 //         #if 1   /* 0: Debug */
 //             Do_SPI_Read_ID();
 //             Do_SPIM_Write_Enable();
 //             Do_SPI_Write_Status(SPIStatus_UnlockAll);
 //             Do_SPIM_Write_Enable();
 //             Do_SPI_Write_256Bytes(&eSPI_OOB_DATA[0],
 //                                   xOOB_CrashLogAddrH,
 //                                   xOOB_CrashLogAddrM,
 //                                   0x00);
 //             Do_SPI_Write_Disable();
 //         #endif
 //         }
 //         xOOB_GET_CRASHLOG = _CRASHLOG_SAVE_INFO;
 //         return;
 //     }
 //     else if(xOOB_GET_CRASHLOG == _CRASHLOG_SAVE_INFO)
 //     {
 //         /* Write Information to tail of eFlash */
 //         eSPI_OOB_DATA[0] = 'C';
 //         eSPI_OOB_DATA[1] = 'R';
 //         eSPI_OOB_DATA[2] = 'A';
 //         eSPI_OOB_DATA[3] = ':';

 //         eSPI_OOB_DATA[4] = 0x00;
 //         eSPI_OOB_DATA[5] = _EFLASH_CRASHLOG_CPU_ADRH;
 //         eSPI_OOB_DATA[6] = _EFLASH_CRASHLOG_CPU_ADRM;
 //         eSPI_OOB_DATA[7] = _EFLASH_CRASHLOG_CPU_ADRL;
 //         eSPI_OOB_DATA[8] = wOOB_CrashCpuDataBytes & 0xFF;
 //         eSPI_OOB_DATA[9] = wOOB_CrashCpuDataBytes >> 8;

 //         eSPI_OOB_DATA[10] = 0x00;
 //         eSPI_OOB_DATA[11] = _EFLASH_CRASHLOG_PCH_ADRH;
 //         eSPI_OOB_DATA[12] = _EFLASH_CRASHLOG_PCH_ADRM;
 //         eSPI_OOB_DATA[13] = _EFLASH_CRASHLOG_PCH_ADRL;
 //         eSPI_OOB_DATA[14] = wOOB_CrashPchDataSizes & 0xFF;
 //         eSPI_OOB_DATA[15] = wOOB_CrashPchDataSizes >> 8;

 //         eSPI_OOB_DATA[16] = 0x00;
 //         eSPI_OOB_DATA[17] = _EFLASH_CRASHLOG_PCH_ADRH;
 //         eSPI_OOB_DATA[18] = _EFLASH_CRASHLOG_PCH_ADRM;
 //         eSPI_OOB_DATA[19] = _EFLASH_CRASHLOG_PCH_ADRL;
 //         eSPI_OOB_DATA[20] = wOOB_CrashCpuDataLines & 0xFF;
 //         eSPI_OOB_DATA[21] = wOOB_CrashCpuDataLines >> 8;

 //         i = 16;
 //         while(1)
 //         {
 //             eSPI_OOB_DATA[i] = 0xFF;
 //             i++;
 //             if(i == 0)
 //             {
 //                 break;
 //             }
 //         }
 //     #if 1   /* 0: Debug */
 //         Do_SPIM_Write_Enable();
 //         Do_SPI_Write_Status(SPIStatus_UnlockAll);
 //         Do_SPIM_Write_Enable();
 //         Do_SPI_Write_256Bytes(&eSPI_OOB_DATA[0],
 //                               _EFLASH_CRASHLOG_INFO_ADRH,
 //                               _EFLASH_CRASHLOG_INFO_ADRM,
 //                               _EFLASH_CRASHLOG_INFO_ADRL);
 //         Do_SPI_Write_Disable();
 //     #endif
 //     }
 //     xOOB_GET_CRASHLOG = _CRASHLOG_END;
 // }

/*-----------------------------------------------------------------------------
 * @subroutine - Service_OOB_Message
 * @function - Service_OOB_Messages
 * @Upstream - Hook_Timer100msEventC
 * @input    - None
 * @return   - None
 * @note     - None
 */
void Service_OOB_Message(void)
{
    if ((IS_MASK_CLEAR(REG_3113, F_OOB_CHN_READY)) ||
        (IS_MASK_CLEAR(REG_3113, F_OOB_CHN_ENABLE)))
        return;

#if SUPPORT_OOB_INTEL_CRASHLOG
    if (xOOB_GET_CRASHLOG > 0)
    {
        return;
    }
#endif

#if 1
    xOOB_Scan++;
    switch (xOOB_Scan)
    {
        case 1:
        #if SUPPORT_OOB_PCH_RTC_TIME
            Get_OOB_RTC_Time();
        #else
                /* Manual Debug Mode */
                // if (xOOB_GET_RTC_DATA > 0)
                // {
                //     xOOB_GET_RTC_DATA = 0;
                //     Get_OOB_RTC_Time();
                // }
        #endif
            break;
        case 2:
        #if SUPPORT_OOB_PCH_TEMPERATURE
            if (System_PowerState == SYSTEM_S0)
            {
                if (xOOB_GetPCH_Temper > 1)
                {
                    xOOB_GetPCH_Temper--;
                }
                else
                {
                    xOOB_GetPCH_Temper = PCH_TEMP_GET_OOB_SEC;
                    Get_OOB_PCH_Temperature();
                }
            }
        #else
                /* Manual Debug Mode */
            if (xOOB_GET_PCH_TMPR > 0)
            {
                xOOB_GET_PCH_TMPR = 0;
                xOOB_GetPCH_Temper = 0;
                Get_OOB_PCH_Temperature();
            }
        #endif
            break;
        case 3:
        #if 0 // GET_FLASH TEST SAMPLE CODE
            if (xOOB_GET_FLASH_ADR3 & 0x80)
            {
                eSPI_Flash_Read((xOOB_GET_FLASH_ADR3 & 0x7F),
                    xOOB_GET_FLASH_ADR2,
                    xOOB_GET_FLASH_ADR1,
                    xOOB_GET_FLASH_ADR0,
                    64,
                    &eSPI_FLASH_DATA[0]);
                xOOB_GET_FLASH_ADR3 = 0;
            }
        #endif
            break;
        case 4:
            break;
        case 5:
        #if 0 // OOB->PECI TEST SAMPLE CODE
            if (xOOB_PECI_TEST == 1)
            {
                OOB_PECI_GetDIB();
                xOOB_PECI_TEST = 0;
            }
        #endif
            break;
        case 6:
            break;
        case 7:
        #if SUPPORT_OOB_PECI_GetTemp
            if (System_PowerState == SYSTEM_S0)
            {
                if (xOOB_PeciGetCpuT_Timer > 1)
                {
                    xOOB_PeciGetCpuT_Timer--;
                }
                else
                {
                    xOOB_PeciGetCpuT_Timer = OOB_PECI_GetTemp_SEC;
                    OOB_PECI_GetTemp();
                }
            }
        #endif
            break;
        case 8:
            break;
        case 9:
        #if 0 // OOB->PECI TEST SAMPLE CODE
            if (xOOB_PECI_PLx_Index == 1)
            {
                xOOB_PECI_PLx_Index = 0;
                OOB_PECI_ReadPowerLimit1();
            }
            if (xOOB_PECI_PLx_Index == 2)
            {
                xOOB_PECI_PLx_Index = 0;
                OOB_PECI_ReadPowerLimit2();
            }
            if (xOOB_PECI_PLx_Index == 3)
            {
                xOOB_PECI_PLx_Index = 0;
                OOB_PECI_ReadPowerLimit3();
            }
            if (xOOB_PECI_PLx_Index == 4)
            {
                xOOB_PECI_PLx_Index = 0;
                OOB_PECI_ReadPowerLimit4();
            }
            if (xOOB_PECI_PLx_Index == 0x81)
            {
                xOOB_PECI_PLx_Index = 0;
                OOB_PECI_WritePowerLimit1(xOOB_PECI_PLx_Data0, xOOB_PECI_PLx_Data1);
            }
            if (xOOB_PECI_PLx_Index == 0x82)
            {
                xOOB_PECI_PLx_Index = 0;
                OOB_PECI_WritePowerLimit2(xOOB_PECI_PLx_Data0);
            }
            if (xOOB_PECI_PLx_Index == 0x83)
            {
                xOOB_PECI_PLx_Index = 0;
                OOB_PECI_WritePowerLimit3(xOOB_PECI_PLx_Data0);
            }
            if (xOOB_PECI_PLx_Index == 0x84)
            {
                xOOB_PECI_PLx_Index = 0;
                OOB_PECI_WritePowerLimit4(xOOB_PECI_PLx_Data0);
            }
        #endif
            break;
        default:
            if (xOOB_Scan > 9)
            {
                xOOB_Scan = 0;
            }
            break;
    }
#endif
}

//-----------------------------------------------------------------------------
#endif // SUPPORT_OOB_SERVICE_MODULE
/*---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * @subroutine - Process_Peripheral_Message_Send
 * @function - Process_Peripheral_Message_Send
 * @Upstream - Service_Peripheral_Message
 * @input    - None
 * @return   - None
 * @note     - Send message by peripheral channel.
 */
BYTE Process_Peripheral_Message_Send(void)
{
    Peri_Table_Pntr = Peripheral_Message_Send;

    /* Check upstream authority enable */
    if (!OOB_Check_Upstream_Authority_EN())
    {
        return FALSE;
    }

    _R5 = *Peri_Table_Pntr;
    Peri_Table_Pntr++;
    ESUCTRL1 = _R6;             //cycle type

    _R6 = *Peri_Table_Pntr;
    Peri_Table_Pntr++;
    ESUCTRL2 = _R7;             //tag + length[11:8],

    _R7 = *Peri_Table_Pntr;
    Peri_Table_Pntr++;
    xOOB_PacketLength = _R7;
    ESUCTRL3 = xOOB_PacketLength;   //length[7:0],

    _R6 = xOOB_PacketLength;
    _R5 = 0;
    while (_R6)
    {
        register uint32_t len = (_R6 <= 4 ? _R6 : 4);
        _R6 -= len;
        uDword data_temp;
        for (register uint32_t i = 0; i < len; i++)
        {
            data_temp.byte[i] = *Peri_Table_Pntr;
            Peri_Table_Pntr++;
        }
        REG32(UPSTREAM_DATA + _R5) = data_temp.dword;
        _R5++;
    }

#if 1
    /* Patch this can let data stable ? */
    ESOCTRL0 = PUT_OOB_STATUS;  /* Write clear for next OOB receive */
#endif
    ESUCTRL0 |= Upstream_EN;    //Set upstream enable
    ESUCTRL0 |= Upstream_GO;    //Set upstream go

    /* Check upstream done */
    if (!OOB_Check_Upstream_Done())
    {
        return FALSE;
    }

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * @subroutine - Process_Peripheral_Memory_Read32
 * @function - Process_Peripheral_Memory_Read32
 * @Upstream - Service_Peripheral_Message
 * @input    - None
 * @return   - None
 * @note     - Read 32bit address data by peripheral channel.
 * @waining  - Reading the system memory may cause an unexpected system crash. Use this function with caution
 */
BYTE Process_Peripheral_Memory_Read32(void)
{
    Peri_Table_Pntr = Peripheral_Memory_Read32;
    Tmp_XPntr = &eSPI_Peri_MemRd32[0];

    /* Check upstream authority enable */
    if (!OOB_Check_Upstream_Authority_EN())
    {
        return FALSE;
    }

    _R5 = *Peri_Table_Pntr;
    Peri_Table_Pntr++;
    ESUCTRL1 = _R6;            //cycle type

    _R6 = *Peri_Table_Pntr;
    Peri_Table_Pntr++;
    ESUCTRL2 = _R7;     //tag + length[11:8],

    _R7 = *Peri_Table_Pntr;
    Peri_Table_Pntr++;
    xOOB_PacketLength = _R7;
    ESUCTRL3 = xOOB_PacketLength;   //length[7:0],

    _R6 = xOOB_PacketLength;
    _R5 = 0;
    while (_R6)
    {
        register uint32_t len = (_R6 <= 4 ? _R6 : 4);
        _R6 -= len;
        uDword data_temp;
        for (register uint32_t i = 0; i < len; i++)
        {
            data_temp.byte[i] = *Peri_Table_Pntr;
            Peri_Table_Pntr++;
        }
        REG32(UPSTREAM_DATA + _R5) = data_temp.dword;
        _R5++;
    }

#if 1
    /* Patch this can let data stable ? */
    ESOCTRL0 = PUT_OOB_STATUS;  /* Write clear for next OOB receive */
#endif
    ESUCTRL0 |= Upstream_EN;    //Set upstream enable
    ESUCTRL0 |= Upstream_GO;    //Set upstream go

    /* Check upstream done */
    if (!OOB_Check_Upstream_Done())
    {
        return FALSE;
    }

    /* Check PUT_FLASH_C cycle Type */
    xOOB_Failed = 0;
    xOOB_Timeout = 255;
    while (1)
    {
        /* Check PUT_FLASH_C cycle Type */
        if ((ESUCTRL6 & 0x0F) ==
            Successful_Completion_With_Data_the_only_Completion)
        {
            break;
        }
        if (xOOB_Timeout > 0)
        {
            xOOB_Timeout--;
        }
        else
        {
            xOOB_Failed = 1;
            break;
        }
        /* Delay 15.26 us */
        vDelayXus(16);
    }
    if (xOOB_Failed > 0)
    {
        xOOB_FailedCounter++;
        ESUCTRL0 |= Upstream_Done;  /* Wrtie-1 to clear Upstream Done */
        ESOCTRL0 = PUT_OOB_STATUS;  /* Write clear for next OOB receive */
        return FALSE;
    }

    /* Store data */
    _R5 = Peripheral_Memory_Read32[2];
    _R6 = 0;
    while (_R5 > 0)
    {
        /* Read data return data */
        *Tmp_XPntr = (VBYTE)((REG32(PUT_PC_DATA + (_R6 / 4))) >> ((_R6 % 4) * 8));
        _R6++;
        Tmp_XPntr++;
        _R5--;
    }
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * @subroutine - Service_Peripheral_Message
 * @function - Service_Peripheral_Message
 * @Upstream - Hook_Timer100msEventC
 * @input    - None
 * @return   - None
 * @note     - None
 */
void Service_Peripheral_Message(void)
{
    if ((IS_MASK_CLEAR(REG_310B, F_Peripheral_Channel_Ready)) ||
        (IS_MASK_CLEAR(REG_310B, F_PeripheralChannelEnable)))
        return;

    if (System_PowerState == SYSTEM_S0)
    {
        Process_Peripheral_Message_Send();
        // Process_Peripheral_Memory_Read32();
    }
}

/***************************************** eRPMC OOB ************************************************/
#if 1
void eRPMC_WriteRootKey_Response(void)
{
    //DWORD Temp_Info1 = C2EINFO1;
    eRPMC_WriteRootKey_data.eSPI_Cycle_Type = 0x21;
    eRPMC_WriteRootKey_data.Length_High = 0x00;
    eRPMC_WriteRootKey_data.Tag = 0x0;
    eRPMC_WriteRootKey_data.Length_Low = 0x0c;
    eRPMC_WriteRootKey_data.Reserved1 = 0x00;
    eRPMC_WriteRootKey_data.Dest_Slave_Addr = 0x08;
    eRPMC_WriteRootKey_data.Command_Code = 0x0F;
    eRPMC_WriteRootKey_data.Byte_Count = 0x09;
    eRPMC_WriteRootKey_data.Reserved2 = 0x00;
    eRPMC_WriteRootKey_data.Source_Slave_Address = 0x07;
    eRPMC_WriteRootKey_data.Header_Version = 0x0;
    eRPMC_WriteRootKey_data.MCTP = 0x0;
    eRPMC_WriteRootKey_data.Destination_Endpoint_ID = 0x00;
    eRPMC_WriteRootKey_data.Source_Endpoint_ID = 0x00;
    eRPMC_WriteRootKey_data.Message_Tag = 0x00;
    eRPMC_WriteRootKey_data.TO = 0x0;
    eRPMC_WriteRootKey_data.Packet_Seq = 0x00;
    eRPMC_WriteRootKey_data.EOM = 0x0;
    eRPMC_WriteRootKey_data.SOM = 0x0;
    eRPMC_WriteRootKey_data.Message_Type = 0x7D;
    eRPMC_WriteRootKey_data.IC = 0x0;
    eRPMC_WriteRootKey_data.RPMC_Device = 0x01;
    eRPMC_WriteRootKey_data.Counter_Addr = 0x00;
    eRPMC_WriteRootKey_data.Extended_Status = MAILBOX_C2EINFO2 & 0xff;
    //eSPI_OOB_Send((BYTE *)&eRPMC_WriteRootKey_data);
    // 填入OOB回复HOST的OOB MTCP Packet
}

void eRPMC_UpdateHMACKey_Response(void)
{
    //DWORD Temp_Info1 = C2EINFO1;
    eRPMC_UpdateHMACKey_data.eSPI_Cycle_Type = 0x21;
    eRPMC_UpdateHMACKey_data.Length_High = 0x00;
    eRPMC_UpdateHMACKey_data.Tag = 0x0;
    eRPMC_UpdateHMACKey_data.Length_Low = 0x0c;
    eRPMC_UpdateHMACKey_data.Reserved1 = 0x00;
    eRPMC_UpdateHMACKey_data.Dest_Slave_Addr = 0x08;
    eRPMC_UpdateHMACKey_data.Command_Code = 0x0F;
    eRPMC_UpdateHMACKey_data.Byte_Count = 0x09;
    eRPMC_UpdateHMACKey_data.Reserved2 = 0x00;
    eRPMC_UpdateHMACKey_data.Source_Slave_Address = 0x07;
    eRPMC_UpdateHMACKey_data.Header_Version = 0x0;
    eRPMC_UpdateHMACKey_data.MCTP = 0x0;
    eRPMC_UpdateHMACKey_data.Destination_Endpoint_ID = 0x00;
    eRPMC_UpdateHMACKey_data.Source_Endpoint_ID = 0x00;
    eRPMC_UpdateHMACKey_data.Message_Tag = 0x00;
    eRPMC_UpdateHMACKey_data.TO = 0x0;
    eRPMC_UpdateHMACKey_data.Packet_Seq = 0x00;
    eRPMC_UpdateHMACKey_data.EOM = 0x0;
    eRPMC_UpdateHMACKey_data.SOM = 0x0;
    eRPMC_UpdateHMACKey_data.Message_Type = 0x7D;
    eRPMC_UpdateHMACKey_data.IC = 0x0;
    eRPMC_UpdateHMACKey_data.RPMC_Device = 0x00;
    eRPMC_UpdateHMACKey_data.Counter_Addr = 0x00;
    eRPMC_UpdateHMACKey_data.Extended_Status = MAILBOX_C2EINFO2 & 0xff;
    //SPI_OOBSend((BYTE *)&eRPMC_UpdateHMACKey_data)
    // 填入OOB回复HOST的OOB MTCP Packet
}

void eRPMC_IncrementCounter_Response(void)
{
    //DWORD Temp_Info1 = C2EINFO1;
    eRPMC_IncrementCounter_data.eSPI_Cycle_Type = 0x21;
    eRPMC_IncrementCounter_data.Length_High = 0x00;
    eRPMC_IncrementCounter_data.Tag = 0x0;
    eRPMC_IncrementCounter_data.Length_Low = 0x0c;
    eRPMC_IncrementCounter_data.Reserved1 = 0x00;
    eRPMC_IncrementCounter_data.Dest_Slave_Addr = 0x08;
    eRPMC_IncrementCounter_data.Command_Code = 0x0F;
    eRPMC_IncrementCounter_data.Byte_Count = 0x09;
    eRPMC_IncrementCounter_data.Reserved2 = 0x00;
    eRPMC_IncrementCounter_data.Source_Slave_Address = 0x07;
    eRPMC_IncrementCounter_data.Header_Version = 0x0;
    eRPMC_IncrementCounter_data.MCTP = 0x0;
    eRPMC_IncrementCounter_data.Destination_Endpoint_ID = 0x00;
    eRPMC_IncrementCounter_data.Source_Endpoint_ID = 0x00;
    eRPMC_IncrementCounter_data.Message_Tag = 0x00;
    eRPMC_IncrementCounter_data.TO = 0x0;
    eRPMC_IncrementCounter_data.Packet_Seq = 0x0;
    eRPMC_IncrementCounter_data.EOM = 0x0;
    eRPMC_IncrementCounter_data.SOM = 0x0;
    eRPMC_IncrementCounter_data.Message_Type = 0x7D;
    eRPMC_IncrementCounter_data.IC = 0x0;
    eRPMC_IncrementCounter_data.RPMC_Device = 0x00;
    eRPMC_IncrementCounter_data.Counter_Addr = 0x00;
    eRPMC_IncrementCounter_data.Extended_Status = MAILBOX_C2EINFO2 & 0xff;
    //eSPI_OOB_Send((BYTE *)&eRPMC_IncrementCounter_data)
    // 填入OOB回复HOST的OOB MTCP Packet
    RPMC_OOB_TempArr[13] = 0x9B;
    RPMC_OOB_TempArr[14] = 0x03;
    memcpy(&eRPMC_RequestCounter, RPMC_OOB_TempArr, sizeof(eRPMC_RequestCounter));
    TaskParams Params;
    task_head = Add_Task((TaskFunction)Mailbox_RequestCounter_Trigger, Params, &task_head);
}

void eRPMC_RequestCounter_Response(void)
{
    //DWORD Temp_Info1 = C2EINFO1;
    eRPMC_RequestCounter_data.eSPI_Cycle_Type = 0x21;
    eRPMC_RequestCounter_data.Length_High = 0x00;
    eRPMC_RequestCounter_data.Tag = 0x0;
    eRPMC_RequestCounter_data.Length_Low = 0x3C;
    eRPMC_RequestCounter_data.Reserved1 = 0x00;
    eRPMC_RequestCounter_data.Dest_Slave_Addr = 0x08;
    eRPMC_RequestCounter_data.Command_Code = 0x0F;
    eRPMC_RequestCounter_data.Byte_Count = 0x39;
    eRPMC_RequestCounter_data.Reserved2 = 0x00;
    eRPMC_RequestCounter_data.Source_Slave_Address = 0x07;
    eRPMC_RequestCounter_data.Header_Version = 0x0;
    eRPMC_RequestCounter_data.MCTP = 0x0;
    eRPMC_RequestCounter_data.Destination_Endpoint_ID = 0x00;
    eRPMC_RequestCounter_data.Source_Endpoint_ID = 0x00;
    eRPMC_RequestCounter_data.Message_Tag = 0x00;
    eRPMC_RequestCounter_data.TO = 0x0;
    eRPMC_RequestCounter_data.Packet_Seq = 0x00;
    eRPMC_RequestCounter_data.EOM = 0x0;
    eRPMC_RequestCounter_data.SOM = 0x0;
    eRPMC_RequestCounter_data.Message_Type = 0x7D;
    eRPMC_RequestCounter_data.IC = 0x0;
    eRPMC_RequestCounter_data.RPMC_Device = 0x00;
    eRPMC_RequestCounter_data.Counter_Addr = 0x00;
    eRPMC_RequestCounter_data.Extended_Status = MAILBOX_C2EINFO2 & 0xff;
    /*数组留有赋值接口*/
    for (_R5 = 0; _R5 < 12; _R5++)
    {
        eRPMC_RequestCounter_data.Tag_Arr[_R5] = REG8(0x31800 + _R5);
    }
    for (_R5 = 0; _R5 < 4; _R5++)
    {
        eRPMC_RequestCounter_data.CounterReadData[_R5] = REG8(0x3180c + _R5);
    }
    for (_R5 = 0; _R5 < 32; _R5++)
    {
        eRPMC_RequestCounter_data.Signature[_R5] = REG8(0x31810 + _R5);
    }
    //eSPI_OOB_Send((BYTE *)&eRPMC_RequestCounter_data)
    RPMC_OOB_TempArr[13] = 0x9B;
    RPMC_OOB_TempArr[14] = 0x02;
    for (_R5 = 0; _R5 < 4; _R5++)
    {
        RPMC_OOB_TempArr[17 + _R5] = eRPMC_RequestCounter_data.CounterReadData[_R5];
    }
    memcpy(&eRPMC_IncrementCounter, RPMC_OOB_TempArr, sizeof(eRPMC_IncrementCounter));
    TaskParams Params;
    task_head = Add_Task((TaskFunction)Mailbox_IncrementCounter_Trigger, Params, &task_head);
}

void eRPMC_ReadParameter_Response(void)
{
    //DWORD Temp_Info1 = C2EINFO1;
    eRPMC_ReadParameters_data.eSPI_Cycle_Type = 0x21;
    eRPMC_ReadParameters_data.Length_High = 0x00;
    eRPMC_ReadParameters_data.Tag = 0x0;
    eRPMC_ReadParameters_data.Length_Low = 0x12;
    eRPMC_ReadParameters_data.Reserved1 = 0x00;
    eRPMC_ReadParameters_data.Dest_Slave_Addr = 0x08;
    eRPMC_ReadParameters_data.Command_Code = 0x0F;
    eRPMC_ReadParameters_data.Byte_Count = 0x0F;
    eRPMC_ReadParameters_data.Reserved2 = 0x00;
    eRPMC_ReadParameters_data.Source_Slave_Address = 0x07;
    eRPMC_ReadParameters_data.Header_Version = 0x0;
    eRPMC_ReadParameters_data.MCTP = 0x0;
    eRPMC_ReadParameters_data.Destination_Endpoint_ID = 0x00;
    eRPMC_ReadParameters_data.Source_Endpoint_ID = 0x00;
    eRPMC_ReadParameters_data.Message_Tag = 0x00;
    eRPMC_ReadParameters_data.TO = 0x0;
    eRPMC_ReadParameters_data.Packet_Seq = 0x00;
    eRPMC_ReadParameters_data.EOM = 0x0;
    eRPMC_ReadParameters_data.SOM = 0x0;
    eRPMC_ReadParameters_data.Message_Type = 0x7D;
    eRPMC_ReadParameters_data.IC = 0x0;
    eRPMC_ReadParameters_data.Extended_Status = MAILBOX_C2EINFO2 & 0xff;
    eRPMC_ReadParameters_data.RPMC_ParameterTable = MAILBOX_C2EINFO3;
    eRPMC_ReadParameters_data.RPMC_Parameters_Device0 = MAILBOX_C2EINFO4;
    //if (eSPI_OOB_Send((BYTE *)&eRPMC_ReadParameters_data))
    // 填入OOB回复HOST的OOB MTCP Packet
}

#if 0
BYTE eSPI_OOBRPMC_Handler(void)
{
    if ((eRPMC_Handler_Rec == 0) && (eRPMC_Handler_Res == 0) && (eRPMC_Handler_Force == 0))
        return FALSE;
    if (eRPMC_Handler_Rec == 1)
    {
        if (eSPI_OOB_Receive(RPMC_OOB_TempArr))
        {
            eRPMC_Handler_Rec = 0;
            if (RPMC_OOB_TempArr[2] == 0) // receive message length is 0, means no message
            {
                return FALSE;
            }
            eRPMC_Handler_Force = 1; // susccess receive message wait send crypto
        }
    }
    if (eRPMC_Handler_Force == 1)
    {
        if (eRPMC_Busy_Status == 1)
        {
            eRPMC_Handler_Force = 1;
            return FALSE;
        }
        switch (RPMC_OOB_TempArr[14]) // cmd type
        {
            case 0x0:                            // WriteRootKey
                if (RPMC_OOB_TempArr[2] == 0x48) // WriteRootKey message1
                {
                }
                else if (RPMC_OOB_TempArr[2] == 0x0B) // WriteRootKey message2
                {
                    /*mailbox WriteRootKey trigger*/
                    Mailbox_WriteRootKey_Trigger();
                }
                break;
            case 0x1:                            // UpdateHMACKey
                if (RPMC_OOB_TempArr[2] == 0x32) // UpdateHMACKey message
                {
                    Mailbox_UpdateHMACKey_Trigger();
                }
                break;
            case 0x2:                            // IncrementCounter
                if (RPMC_OOB_TempArr[2] == 0x32) // IncrementCounter message
                {
                    // Mailbox_IncrementCounter_Trigger();
                }
                break;
            case 0x3:                            // RequestCounter
                if (RPMC_OOB_TempArr[2] == 0x3A) // RequestCounter message
                {
                    // Mailbox_RequestCounter_Trigger();
                }
                break;
            case 0x4: // ReadParameters
                if (RPMC_OOB_TempArr[2] == 0x0B)
                {
                }
                break;
            default:
                break;
        }
        eRPMC_Handler_Force = 0;
    }
    if (eRPMC_Handler_Res == 1)
    {
        switch (RMPC_ResType)
        {
            case 0x1: // WriteRootKey
                if (eSPI_OOB_Send((BYTE *)&eRPMC_WriteRootKey_data))
                    eRPMC_Handler_Res = 0;
                break;
            case 0x2: // UpdateHMACKey
                if (eSPI_OOB_Send((BYTE *)&eRPMC_UpdateHMACKey_data))
                    eRPMC_Handler_Res = 0;
                break;
            case 0x3: // IncrementCounter
                if (eSPI_OOB_Send((BYTE *)&eRPMC_IncrementCounter_data))
                    eRPMC_Handler_Res = 0;
                break;
            case 0x4: // RequestCounter
                if (eSPI_OOB_Send((BYTE *)&eRPMC_RequestCounter_data))
                    eRPMC_Handler_Res = 0;
                break;
            case 0x5: // ReadParameters
                if (eSPI_OOB_Send((BYTE *)&eRPMC_ReadParameters_data))
                    eRPMC_Handler_Res = 0;
                break;
            default:
                RMPC_ResType = 0;
                break;
        }
    }
    return TRUE;
}
#endif
#endif
/***************************************** eRPMC OOB ************************************************/

/* ----------------------------------------------------------------------------
 * FUNCTION: Service_eSPI
 * Polling RSMRST high & vw channel enable then set vw channel ready
 * ------------------------------------------------------------------------- */
void Service_eSPI(void)
{
    //-----------------------------------
    // eSPI Interface Control
    //-----------------------------------
    if (SPK_CHECK_RSMRST_HI(HIGH))
    {
        EC_SET_eSPI_CHN_Ready();

        if (VWIDX5 == (F_IDX5_SLAVE_BOOT_LOAD_STATUS_VALID +
            F_IDX5_SLAVE_BOOT_LOAD_DONE_VALID +
            F_IDX5_SLAVE_BOOT_LOAD_STATUS +
            F_IDX5_SLAVE_BOOT_LOAD_DONE))
        {
            EC_ACK_eSPI_SUS_WARN(); // if SUS_WARN vw wire support
            EC_ACK_eSPI_Reset(); // if HOST_RST_WARN vw wire support

        #if SUPPORT_HOOK_WARMBOOT
            if (eSPI_PLTRST_TAG == F_PLTRST_DETECTED)
            {
                eSPI_PLTRST_TAG = F_PLTRST_HI_LEVEL;
                SystemWarmBoot();
            }
        #endif
        }
        else
        {
            EC_ACK_eSPI_Boot_Ready();
        }
    }
#if 0
    eSPI_OOBRPMC_Handler();
#endif
}