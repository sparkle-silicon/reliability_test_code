/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-09-12 15:04:50
 * @Description: This is about the  national crypto algorithm implementation
 *
 *
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 以下是中文及英文版权同步声明，编码为UTF-8。
 * Copyright has legal effects and violations will be prosecuted.
 * 版权具有法律效力，违反必究。
 *
 * Copyright ©2021-2025 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2025龙晶石半导体科技（苏州）有限公司
 */
#include "CUSTOM_PECI.H"
#include "CUSTOM_GPIO.H"
#include "KERNEL_MEMORY.H"
 /*-----------------------------------------------------------------------------
  * @subroutine - PECI_Init
  * @function - PECI_Init
  * @upstream - By call
  * @input    - None
  * @return   - None
  * @note     - None
  */
void PECI_Init(void)
{
    SYSCTL_CLKDIV_PECI = PECI_CLKDIV_1;
    SYSCTL_PMU_CFG |= PMU_EN_IBIAS | PMU_EN_BUF;
    SYSCTL_PECI_PAD_CTRL |= PECI_RESERVED_REG;
    // PECI_STAR = PECI_STAR_LSB_FIRST;     //if lsb first, need enable this bit
    PECI_Set_Pad(VTTlev_110V, PADSEL_PECI);     //set pad voltage
    PECI_CTRL1R |= PECI_CTRL1R_HHRAE;
    PECI_CTRL2R &= 0xF8;
    PECI_CTRL2R |= TRANSFER_RATE_1_0MHz;       // PECI host optimal transfer rate. = 1MHz <-KBL
    PECI_CTRL2R &= ~PECI_CTRL1R_HHRAE;
    PECI_ERRCNT = 0;
    PECI_CTRL1R |= PECI_HOST_ENABLE;            // PECI Host Enable
}

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_Set_Pad
 * @function - PECI_Set_Pad
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
void PECI_Set_Pad(u16 VTTlev, u8 PADsel)
{
    if (PADsel)
    {
        SYSCTL_PECI_PAD_CTRL &= ~(0b1111 << 8);
        SYSCTL_PECI_PAD_CTRL |= BIT(15) | ((VTTlev & 0b1111) << 8);
    }
    else
    {
        SYSCTL_PECI_PAD_CTRL &= ~PECI_VTT_SEL;
        PECI_CTRL1R |= PECI_CTRL1R_HHRAE;
        PECI_PADCTLR &= 0xfc;
        PECI_PADCTLR |= VTTlev & 0b11;
        PECI_CTRL2R &= ~PECI_CTRL1R_HHRAE;
    }
}
//-----------------------------------------------------------------------------
#if SUPPORT_PECI
/*-----------------------------------------------------------------------------
 * @subroutine - Service_PECI
 * @function - Service_PECI
 * @upstream - Hook_Timer500msEventA
 * @input    - None
 * @return   - None
 * @note     - None
 */
BYTE temp_data[10] = { 0 };
void Service_PECI(void)
{
    if (PECI_DELAY > 0)
    {
        PECI_DELAY--;
        return;
    }

    PECI_CTRL1R = 0x08;     /* PECI module only enable one time */
    PECI_STAR |= 0xEE;    /* ResetPECIStatus  */

#if 1
    if (!(PECI_FLAG & F_PECI_INIT))
    {
        if (PECI_ReadDIB())
        {
            PECI_FLAG |= F_PECI_INIT;
        }
        return;
    }
#if SUPPORT_PECI_POWER_CTRL
    else if (!(PECI_FLAG & F_PECI_INIT2))
    {
        if (PECI_ReadPowerUnit())
        {
            PECI_FLAG |= F_PECI_INIT2;
        }
        return;
    }
    else if (!(PECI_FLAG & F_PECI_INIT3))
    {
        if (Read_BC_ACOK() && Read_BATT_PRS_1())
        {
            if (PECI_WritePowerLimit2(45))
            {
                PECI_FLAG |= F_PECI_INIT3;
            }
        }
        else
        {
            PECI_FLAG |= F_PECI_INIT3;
        }
        return;
    }
    else if (!(PECI_FLAG & F_PECI_INIT4))
    {
        if (Read_BC_ACOK() && Read_BATT_PRS_1())
        {
            if (PECI_WritePowerLimit4(66))
            {
                PECI_FLAG |= F_PECI_INIT4;
            }
        }
        else
        {
            PECI_FLAG |= F_PECI_INIT4;
        }
        return;
    }
    else if (!(PECI_FLAG & F_PECI_INIT5))
    {
        PECI_FLAG |= F_PECI_INIT5;
        //PECI_WritePowerLimit3(32);
        return;
    }
#endif
    else
    {
        if ((PECI_RdPkgCfg_Idx16[2] < 80) || (PECI_RdPkgCfg_Idx16[2] > 120))
        {   // Read TjMax again if the value is zero or not normal(lower than 80'c, larger than 120'c).
            if (PECI_RdPkgConfig(_PECI_CPU_ADDR, PECI_RdPkgCfg_Idx16, _PECI_Domain_0,
                0, _PECI_Index_TTR, 0, 0, 5, 5))
            {
                PECI_RdPkgCfg_Idx16_CC = PECI_CompletionCode;
            }
        }
        else
        {
        #if 0
            if (PECI_ReadTemp())
            {
                Calc_CPU_Temperature();
            }
        #else
            switch (PECI_CNT)
            {
                case 0:
                    if (PECI_ReadTemp())
                    {
                        Calc_CPU_Temperature();
                    }
                    PECI_CNT++;
                    break;
                case 1:
                    if (Read_BC_ACOK() && Read_BATT_PRS_1())
                    {
                        if (PECI_PowerLimit2 != 45)
                        {
                            if (PECI_WritePowerLimit2(45))
                            {
                                PECI_ReadPowerLimit2();
                                if (PECI_PowerLimit2 == 0xFF)
                                {
                                    break;
                                }
                            }
                        }
                    }
                    PECI_CNT++;
                    break;
                case 2:
                    if (Read_BC_ACOK() && Read_BATT_PRS_1())
                    {
                        if (PECI_PowerLimit4 != 66)
                        {
                            if (PECI_WritePowerLimit4(66))
                            {
                                PECI_ReadPowerLimit4();
                                if (PECI_PowerLimit4 == 0xFF)
                                {
                                    break;
                                }
                            }
                        }
                    }
                    //                     PECI_CNT++;
                    PECI_CNT = 0;
                    break;
                    //                 case 3:
                    ////                     PECI_ReadPowerLimit3();
                    //                     PECI_CNT++;
                    //                     break;
                    //                 case 4:
                    ////                     PECI_ReadPowerLimit4();
                    //                     PECI_CNT++;
                    //                     break;
                    //                 case 5:
                    ////                     PECI_WritePowerLimit4_Offset(100);
                    //                     PECI_CNT++;
                    //                     break;
                    //                 case 6:
                    //                     PECI_ReadPowerLimit4_Offset();
                    //                     PECI_CNT = 0;
                    //                     break;
                default:
                    PECI_CNT = 0;
                    break;
            }
        #endif
        }
    }
#endif
    PECI_DELAY = 20; /* 20 x 100ms = 2 sec */
}

#if 0
/*-----------------------------------------------------------------------------
 * @subroutine - Service_PECI_Data
 * @function - Service_PECI_Data
 * @upstream - Hook_Timer10msEventB
 * @input    - None
 * @return   - None
 * @note     - Max:~38us,
 */
void Service_PECI_Data(void)
{
    BYTE    i;
    BYTE    PeciBuffer[16];

    while (PECI_FLAG & F_PECI_BUSY)
    {
        if ((PECI_STAR & 0xEC) != 0)
        {
            /* Error Bit */
            if (PECI_ERRCNT < 255)
            {
                PECI_ERRCNT++;
            }
            PECI_ERRSTS = PECI_STAR;
            PECI_CTRL1R = 0x08;      /* PECI module only enable one time */
            //PECI_CTRL1R = 0x00;    /* PECI_HostDisable (Will have Glitch Noise)*/
            PECI_STAR = 0xFE;  /* ResetPECIStatus  */
            CLEAR_MASK(PECI_FLAG, F_PECI_BUSY);
        }
        else if (IS_MASK_SET(PECI_STAR, PECI_STAR_FINISH))
        {
            PECI_ERRCNT = 0;
            PECI_ERRSTS = PECI_STAR;
            // Tmp_XPntr = (unsigned int *)PECI_MEMADR;
            if (PECI_CMD == PECI_CMD_GetTemp)
            {
                for (i = 0; i < PECI_RDLR; i++)
                {
                    PECI_Get_Temp[i] = PECI_RDDAR;
                }
            }
            else if (PECI_CMD == PECI_CMD_GetDIB;)
            {
                for (i = 0; i < PECI_RDLR; i++)
                {
                    PECI_Get_DIB[i] = PECI_RDDAR;
                }
            }
            else if (PECI_CMD == PECI_CMD_RdPkgConfig)
            {
                PECI_RdPkgCfg_Idx16_CC = PECI_RDDAR;
                for (i = 0; i < PECI_RDLR; i++)
                {
                    PECI_RdPkgCfg_Idx16[i] = PECI_RDDAR;
                }
            }
            else
            {
                for (i = 0x00; i < PECI_RDLR; i++)
                {
                    PeciBuffer[i] = PECI_RDDAR;
                    //PeciDataPointer++;
                }
            }
            PECI_CTRL1R = 0x08;      /* PECI module only enable one time */
            //PECI_CTRL1R = 0x00;    /* PECI_HostDisable (Will have Glitch Noise)*/
            PECI_STAR = 0xFE;  /* ResetPECIStatus  */

            CLEAR_MASK(PECI_FLAG, F_PECI_BUSY);
            PECI_FLAG |= F_PECI_UPDATED;

            if (PECI_CMD == PECI_CMD_GetDIB;)
            {
                PECI_FLAG |= F_PECI_INIT;
            }
            if (PECI_CMD == PECI_CMD_GetTemp)
            {
                Calc_CPU_Temperature();
            }
        }
        else
        {
            PECI_TIMER++;
            if (PECI_TIMER > 200)
            {
                PECI_TIMER = 0;
                PECI_OVTCT++;
                PECI_CTRL1R = 0x00;  /* PECI_HostDisable */
                PECI_STAR = 0xFE;  /* ResetPECIStatus  */
                CLEAR_MASK(PECI_FLAG, F_PECI_BUSY);
            }
        }
        return;
    }
}

/*-----------------------------------------------------------------------------
 * @subroutine - Service_PECI_Command
 * @function - Service_PECI_Command
 * @upstream - Hook_Timer100msEventA
 * @input    - None
 * @return   - None
 * @note     - Max:~33us,
 */
void Service_PECI_Command(void)
{
    //-------------------------------------------------------------------------
    //if (SysPowState != SYSTEM_S0)
    //{
    //    PECI_FLAG = 0;
    //    return;
    //}
    //-------------------------------------------------------------------------
    if (PECI_DELAY > 0)
    {
        PECI_DELAY--;
        return;
    }
    if (PECI_FLAG & F_PECI_BUSY)
    {
        return;
    }
    //-------------------------------------------------------------------------
    PECI_CTRL1R = 0x08;      /* PECI module only enable one time */
    //PECI_CTRL1R = 0x00;    /* PECI_HostDisable (Will have Glitch Noise)*/
    PECI_STAR = 0xFE;      /* ResetPECIStatus  */
    /* PECI_HostEnable(); */
    PECI_CTRL1R = (PECI_CTRL1R_FIFOCLR + PECI_CTRL1R_FCSERR_ABT + PECI_CTRL1R_PECIHEN + PECI_CTRL1R_CONCTRL);
    if (!(PECI_FLAG & F_PECI_INIT))
    {
        PECI_TAR_ADDR = _PECI_CPU_ADDR;
        PECI_WRLR = 0x01;
        PECI_RDLR = 0x08;
        PECI_CMD = PECI_CMD_GetDIB;;
        PeciDataPointer = &PECI_Get_DIB[0];
    }
#if SUPPORT_PECI_POWER_CTRL
    else if (!(PECI_FLAG & F_PECI_INIT2))
    {
        PECI_FLAG |= F_PECI_INIT2;
        PECI_ReadPowerUnit();
        return;
    }
    else if (!(PECI_FLAG & F_PECI_INIT3))
    {
        PECI_FLAG |= F_PECI_INIT3;
        // PECI_WritePowerLimit2(15);      /* Example */
        // PECI_WritePowerLimit1(13, 20);  /* Example 13Watt, 20sec */
        return;
    }
    else if (!(PECI_FLAG & F_PECI_INIT4))
    {
        PECI_FLAG |= F_PECI_INIT4;
        PECI_ReadPowerLimit1();
        PECI_ReadPowerLimit2();
        PECI_ReadPowerLimit3();
        PECI_ReadPowerLimit4();
        return;
    }
#endif
    else
    {
        if (PECI_RdPkgCfg_Idx16[2] == 0x00)
        {
            PECI_TAR_ADDR = _PECI_CPU_ADDR;
            PECI_WRLR = 0x05;
            PECI_RDLR = 0x05;
            PECI_CMD = PECI_CMD_RdPkgConfig;
            PeciDataPointer = &PECI_RdPkgCfg_Idx16_CC;
        }
        else
        {
            PECI_TAR_ADDR = _PECI_CPU_ADDR;
            PECI_WRLR = 0x01;
            PECI_RDLR = 0x02;
            PECI_CMD = PECI_CMD_GetTemp;
            PeciDataPointer = &PECI_Get_Temp[0];
        }
    }
    PECI_CMDR = PECI_CMD;

    if (PECI_CMD == PECI_CMD_RdPkgConfig)
    {
        PECI_WRDAR = 0x02;
        PECI_WRDAR = 16;
        PECI_WRDAR = 0x00;
        PECI_WRDAR = 0x00;
    }
    PECI_CTRL1R |= PECI_CTRL1R_START;    /* PECI_HostControl(PECI_CTRL1R_START) */
    PECI_TIMER = 0;

    PECI_DELAY = 4;    /* 4 x 500ms = 2.0sec */
    PECI_FLAG |= F_PECI_BUSY;
}
#endif

/*-----------------------------------------------------------------------------
 * @subroutine - Calc_CPU_Temperature
 * @function - Calc_CPU_Temperature
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
void Calc_CPU_Temperature(void)
{
    if (PECI_FLAG & F_PECI_UPDATED)
    {
        CLEAR_MASK(PECI_FLAG, F_PECI_UPDATED);
        //---------------------------------------------------------------------
        _C1 = (PECI_Get_Temp[1] << 8) + PECI_Get_Temp[0];
        _C1 = (~_C1) + 1; /* 2's complement */
        _R8 = _C1 >> 6;   /* 1/64 degrees centigrade */
        if (Processor_Tj_max == 0)
        {
            PECI_CPU_T = 100 - _R8;
            // CPU_Shutdown_T = 100;
            // SCI_Shutdown_T = 95;
            _C1 = (PECI_CPU_T * 10);
            xEC_TSR1 = _C1;
            xEC_TSR1H = _C1 >> 8;
        }
        else
        {
            // CPU_Shutdown_T = Processor_Tj_max + 3;
            // SCI_Shutdown_T = Processor_Tj_max + 0;
            if ((_R8 & 0x80) == 0x00)
            {
                PECI_CPU_T = Processor_Tj_max - _R8;
            }
            else
            {
                _R8 = (~_R8);
                PECI_CPU_T = Processor_Tj_max + _R8;
            }
            _C1 = (PECI_CPU_T * 10);
            xEC_TSR1 = _C1;
            xEC_TSR1H = _C1 >> 8;
        }
    }
}
#endif
//-----------------------------------------------------------------------------


