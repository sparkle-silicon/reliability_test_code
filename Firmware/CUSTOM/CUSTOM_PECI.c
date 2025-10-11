/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-09-12 15:04:50
 * @Description: This is about the  national crypto algorithm implementation
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

#include "KERNEL_PECI.H"
#include "CUSTOM_PECI.H"
#include "CUSTOM_GPIO.H"

 //-----------------------------------------------------------------------------
#if SUPPORT_PECI
/*-----------------------------------------------------------------------------
 * @subroutine - Init_PECI
 * @function - Init_PECI
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
void Init_PECI(void)
{
    // PECI_HOSTAR = PECI_HOSTAR_LSB_FIRST;
    PECI_Set_Pad(0x3, 0x0);
    PECI_HOCTLR |= BIT7;
    PECI_HOCTL2R &= 0xF8;
    PECI_HOCTL2R |= 0x01; // PECI host optimal transfer rate. = 1MHz <-KBL
    PECI_HOCTL2R &= ~BIT7;
    PECI_ERRCNT = 0;
    PECI_HOCTLR |= BIT3; // PECI Host Enable
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
        SYSCTL_PAD_PECI &= ~(0b1111 << 8);
        SYSCTL_PAD_PECI |= BIT(15) | ((VTTlev & 0b1111) << 8);
    }
    else
    {
        SYSCTL_PAD_PECI &= ~BIT(15);
        PECI_HOCTLR |= BIT7;
        PECI_PADCTLR &= 0xfc;
        PECI_PADCTLR |= VTTlev & 0b11;
        PECI_HOCTL2R &= ~BIT7;
    }
}

/*-----------------------------------------------------------------------------
 * @subroutine - Service_PECI
 * @function - Service_PECI
 * @upstream - Hook_Timer500msEventA
 * @input    - None
 * @return   - None
 * @note     - None
 */
void Service_PECI(void)
{
    if (PECI_DELAY > 0)
    {
        PECI_DELAY--;
        return;
    }

    // PECI_HOCTLR = 0x08; /* PECI module only enable one time */
    // PECI_HOSTAR = 0xEE; // 0xFE;      /* ResetPECIStatus  */

    // switch (PECI_CNT)
    // {
    // case 0:
    //     PECI_PING_TEST();
    //     PECI_CNT++;
    //     break;
    // case 1:
    //     PECI_ReadDIB_TEST();
    //     PECI_CNT++;
    //     break;
    // case 2:
    //     PECI_ReadTemp_TEST();
    //     PECI_CNT++;
    //     break;
    // case 3:
    //     PECI_ReadDIB_TEST();
    //     PECI_CNT = 0;
    //     break;
    // default:
    //     PECI_CNT = 0;
    //     break;
    // }

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
#if 1
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
        if ((PECI_HOSTAR & 0xEC) != 0)
        {
            /* Error Bit */
            if (PECI_ERRCNT < 255)
            {
                PECI_ERRCNT++;
            }
            PECI_ERRSTS = PECI_HOSTAR;
            PECI_HOCTLR = 0x08;      /* PECI module only enable one time */
            //PECI_HOCTLR = 0x00;    /* PECI_HostDisable (Will have Glitch Noise)*/
            PECI_HOSTAR = 0xFE;  /* ResetPECIStatus  */
            CLEAR_MASK(PECI_FLAG, F_PECI_BUSY);
        }
        else if (IS_MASK_SET(PECI_HOSTAR, PECI_HOSTAR_FINISH))
        {
            PECI_ERRCNT = 0;
            PECI_ERRSTS = PECI_HOSTAR;
            // Tmp_XPntr = (unsigned int *)PECI_MEMADR;
            if (PECI_CMD == PECI_CMD_GetTemp)
            {
                for (i = 0; i < PECI_HORDLR; i++)
                {
                    PECI_Get_Temp[i] = PECI_HORDDR;
                }
            }
            else if (PECI_CMD == PECI_CMD_GetDIB;)
            {
                for (i = 0; i < PECI_HORDLR; i++)
                {
                    PECI_Get_DIB[i] = PECI_HORDDR;
                }
            }
            else if (PECI_CMD == PECI_CMD_RdPkgConfig)
            {
                PECI_RdPkgCfg_Idx16_CC = PECI_HORDDR;
                for (i = 0; i < PECI_HORDLR; i++)
                {
                    PECI_RdPkgCfg_Idx16[i] = PECI_HORDDR;
                }
            }
            else
            {
                for (i = 0x00; i < PECI_HORDLR; i++)
                {
                    PeciBuffer[i] = PECI_HORDDR;
                    //PeciDataPointer++;
                }
            }
            PECI_HOCTLR = 0x08;      /* PECI module only enable one time */
            //PECI_HOCTLR = 0x00;    /* PECI_HostDisable (Will have Glitch Noise)*/
            PECI_HOSTAR = 0xFE;  /* ResetPECIStatus  */

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
                PECI_HOCTLR = 0x00;  /* PECI_HostDisable */
                PECI_HOSTAR = 0xFE;  /* ResetPECIStatus  */
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
    //if (SysPowState != _SYSTEM_S0)
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
    PECI_HOCTLR = 0x08;      /* PECI module only enable one time */
    //PECI_HOCTLR = 0x00;    /* PECI_HostDisable (Will have Glitch Noise)*/
    PECI_HOSTAR = 0xFE;      /* ResetPECIStatus  */
    /* PECI_HostEnable(); */
    PECI_HOCTLR = (PECI_HOCTLR_FIFOCLR + PECI_HOCTLR_FCSERR_ABT + PECI_HOCTLR_PECIHEN + PECI_HOCTLR_CONCTRL);
    if (!(PECI_FLAG & F_PECI_INIT))
    {
        PECI_HOTRADDR = _PECI_CPU_ADDR;
        PECI_HOWRLR = 0x01;
        PECI_HORDLR = 0x08;
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
            PECI_HOTRADDR = _PECI_CPU_ADDR;
            PECI_HOWRLR = 0x05;
            PECI_HORDLR = 0x05;
            PECI_CMD = PECI_CMD_RdPkgConfig;
            PeciDataPointer = &PECI_RdPkgCfg_Idx16_CC;
        }
        else
        {
            PECI_HOTRADDR = _PECI_CPU_ADDR;
            PECI_HOWRLR = 0x01;
            PECI_HORDLR = 0x02;
            PECI_CMD = PECI_CMD_GetTemp;
            PeciDataPointer = &PECI_Get_Temp[0];
        }
    }
    PECI_HOCMDR = PECI_CMD;

    if (PECI_CMD == PECI_CMD_RdPkgConfig)
    {
        PECI_HOWRDR = 0x02;
        PECI_HOWRDR = 16;
        PECI_HOWRDR = 0x00;
        PECI_HOWRDR = 0x00;
    }
    PECI_HOCTLR |= PECI_HOCTLR_START;    /* PECI_HostControl(PECI_HOCTLR_START) */
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

#if 1
BYTE PECI_PING_TEST(void)
{
    BYTE done, index;
    UNUSED_VAR(index);
    done = 0x00;

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_PADCTLR &= (~0x40);

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    PECI_HOWRLR = 0x03;

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_PADCTLR &= (~0x20);

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    PECI_HORDLR = 0x04;

    PECI_HOCMDR = 0x0C;

    PECI_HOWRDR = 0x00;
    PECI_HOWRDR = 0x00;
    PECI_HOWRDR = 0x00;

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE);     // HHRAE = 0
    PECI_HOCTLR &= (~PECI_HOCTLR_CONCTRL);
    PECI_HOCTLR &= (~PECI_HOCTLR_FCSERR_ABT);

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_HOCTL2R |= 0x20;

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;

    /***************************************************************/

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x80;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOTRADDR = _PECI_CPU_ADDR_TEST;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x40;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOWRLR = 0x00;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x20;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HORDLR = 0x21;

    // PECI_HOCMDR = 0x1E;

    // PECI_HOCTLR |= 0x40;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_AWFCSV = 0xE0;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE);  // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_CONCTRL;
    // PECI_HOCTLR |= PECI_HOCTLR_FCSERR_ABT;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_HOCTL2R |= 0x20;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;

    if (PECI_CheckHostBusy())
    {
        PECI_HOCTLR |= PECI_HOCTLR_START;
        if (PECI_CheckHostFinish())
        {
            // for (index = 0x00; index < 10; index++)
            // {
            //     // PECI_Get_DIB_TEST[index] = PECI_HORDDR;
            //     // dprint("3:%x\n", PECI_Get_DIB[index]);
            // }

            // for (index = 0x00; index < 10; index++)
            // {
            //     if (index == 9)
            //     {
            //         // dprint("%x\n", PECI_Get_DIB_TEST[index]);
            //     }
            //     else
            //     {
            //         PECI_Get_DIB_TEST[index] = (PECI_Get_DIB_TEST[index] << 3) | (PECI_Get_DIB_TEST[index + 1] >> 5);
            //         // dprint("%x\n", PECI_Get_DIB_TEST[index]);
            //     }
            // }

            done = 0x01;
        }
    }

    return (done);
}

BYTE PECI_ReadDIB_TEST(void)
{
    BYTE done, index;
    done = 0x00;

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_PADCTLR &= (~0x40);

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    PECI_HOWRLR = 0x04;

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_PADCTLR &= (~0x20);

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    PECI_HORDLR = 0x0E;

    PECI_HOCMDR = 0x0C;

    PECI_HOWRDR = 0x00;
    PECI_HOWRDR = 0x21;
    PECI_HOWRDR = 0x1E;
    PECI_HOWRDR = 0x20;

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE);     // HHRAE = 0
    PECI_HOCTLR &= (~PECI_HOCTLR_CONCTRL);
    PECI_HOCTLR &= (~PECI_HOCTLR_FCSERR_ABT);

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_HOCTL2R |= 0x20;

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;

    /***************************************************************/

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x80;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOTRADDR = _PECI_CPU_ADDR_TEST;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x40;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOWRLR = 0x00;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x20;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HORDLR = 0x21;

    // PECI_HOCMDR = 0x1E;

    // PECI_HOCTLR |= 0x40;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_AWFCSV = 0xE0;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE);  // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_CONCTRL;
    // PECI_HOCTLR |= PECI_HOCTLR_FCSERR_ABT;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_HOCTL2R |= 0x20;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;

    if (PECI_CheckHostBusy())
    {
        PECI_HOCTLR |= PECI_HOCTLR_START;
        // dprint("2:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);
        if (PECI_CheckHostFinish())
        {
            for (index = 0x00; index < 10; index++)
            {
                PECI_Get_DIB_TEST[index] = PECI_HORDDR;
                // dprint("3:%x\n", PECI_Get_DIB[index]);
            }

            for (index = 0x00; index < 10; index++)
            {
                if (index == 9)
                {
                    // dprint("%x\n", PECI_Get_DIB_TEST[index]);
                }
                else
                {
                    PECI_Get_DIB_TEST[index] = (PECI_Get_DIB_TEST[index] << 3) | (PECI_Get_DIB_TEST[index + 1] >> 5);
                    // dprint("%x\n", PECI_Get_DIB_TEST[index]);
                }
            }

            done = 0x01;
        }
    }

    return (done);
}

BYTE PECI_ReadTemp_TEST(void)
{
    BYTE done, index;
    UNUSED_VAR(index);
    done = 0x00;

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_PADCTLR &= (~0x40);

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    PECI_HOWRLR = 0x04;

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_PADCTLR &= (~0x20);

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    PECI_HORDLR = 0x09;

    PECI_HOCMDR = 0x0C;

    PECI_HOWRDR = 0x00;
    PECI_HOWRDR = 0x20;
    PECI_HOWRDR = 0x20;
    PECI_HOWRDR = 0x20;

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE);     // HHRAE = 0
    PECI_HOCTLR &= (~PECI_HOCTLR_CONCTRL);
    PECI_HOCTLR &= (~PECI_HOCTLR_FCSERR_ABT);

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_HOCTL2R |= 0x20;

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;

    /***************************************************************/

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x80;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOTRADDR = _PECI_CPU_ADDR_TEST;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x40;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOWRLR = 0x00;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x20;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HORDLR = 0x21;

    // PECI_HOCMDR = 0x1E;

    // PECI_HOCTLR |= 0x40;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_AWFCSV = 0xE0;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE);  // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_CONCTRL;
    // PECI_HOCTLR |= PECI_HOCTLR_FCSERR_ABT;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_HOCTL2R |= 0x20;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;

    if (PECI_CheckHostBusy())
    {
        PECI_HOCTLR |= PECI_HOCTLR_START;
        // dprint("2:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);
        if (PECI_CheckHostFinish())
        {
            // for (index = 0x00; index < 10; index++)
            // {
            //     PECI_Get_DIB_TEST[index] = PECI_HORDDR;
            //     // dprint("3:%x\n", PECI_Get_DIB[index]);
            // }

            // for (index = 0x00; index < 10; index++)
            // {
            //     if (index == 9)
            //     {
            //         // dprint("%x\n", PECI_Get_DIB_TEST[index]);
            //     }
            //     else
            //     {
            //         PECI_Get_DIB_TEST[index] = (PECI_Get_DIB_TEST[index] << 3) | (PECI_Get_DIB_TEST[index + 1] >> 5);
            //         // dprint("%x\n", PECI_Get_DIB_TEST[index]);
            //     }
            // }

            done = 0x01;
        }
    }

    return (done);
}

BYTE PECI_RdPkgConfig_TEST(void)
{
    BYTE done, index;
    UNUSED_VAR(index);
    done = 0x00;

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_PADCTLR &= (~0x40);

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    PECI_HOWRLR = 0x08;

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_PADCTLR &= (~0x20);

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    PECI_HORDLR = 0x0F;

    PECI_HOCMDR = 0x0C;

    PECI_HOWRDR = 0x00;
    PECI_HOWRDR = 0xA0;
    PECI_HOWRDR = 0xB4;
    PECI_HOWRDR = 0x20;
    PECI_HOWRDR = 0x43;
    PECI_HOWRDR = 0xC0;
    PECI_HOWRDR = 0x00;
    PECI_HOWRDR = 0x00;

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE);     // HHRAE = 0
    PECI_HOCTLR &= (~PECI_HOCTLR_CONCTRL);
    PECI_HOCTLR &= (~PECI_HOCTLR_FCSERR_ABT);

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_HOCTL2R |= 0x20;

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;

    /***************************************************************/

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x80;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOTRADDR = _PECI_CPU_ADDR_TEST;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x40;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOWRLR = 0x00;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x20;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HORDLR = 0x21;

    // PECI_HOCMDR = 0x1E;

    // PECI_HOCTLR |= 0x40;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_AWFCSV = 0xE0;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE);  // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_CONCTRL;
    // PECI_HOCTLR |= PECI_HOCTLR_FCSERR_ABT;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_HOCTL2R |= 0x20;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;

    if (PECI_CheckHostBusy())
    {
        PECI_HOCTLR |= PECI_HOCTLR_START;
        // dprint("2:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);
        if (PECI_CheckHostFinish())
        {
            // for (index = 0x00; index < 10; index++)
            // {
            //     PECI_Get_DIB_TEST[index] = PECI_HORDDR;
            //     // dprint("3:%x\n", PECI_Get_DIB[index]);
            // }

            // for (index = 0x00; index < 10; index++)
            // {
            //     if (index == 9)
            //     {
            //         // dprint("%x\n", PECI_Get_DIB_TEST[index]);
            //     }
            //     else
            //     {
            //         PECI_Get_DIB_TEST[index] = (PECI_Get_DIB_TEST[index] << 3) | (PECI_Get_DIB_TEST[index + 1] >> 5);
            //         // dprint("%x\n", PECI_Get_DIB_TEST[index]);
            //     }
            // }

            done = 0x01;
        }
    }

    return (done);
}

#if 0
// void PECI_Init(u16 VTTlev, u8 PADsel)
// {
//     // PECIHEN
//     PECI_HOCTL2R &= ~BIT7; // set PECI_HOCTRL
//     PECI_HOCTLR |= BIT3;   // PECI Host Enable
//     PECI_Set_Pad(0x01, 0x1);
//     PECI_HOSTAR = PECI_HOSTAR_LSB_FIRST;
// }
// void peci_write(u8 target_addr, u8 wlen, u8 rlen, u8 cmd, u8 data)
// {
//     // Set Target address
//     PECI_HOCTL2R &= ~BIT7;
//     PECI_HOTRADDR = target_addr;
//     // Set Write length and read length
//     PECI_HORDLR = rlen;
//     PECI_HOWRDR = wlen;
//     // set command data
//     PECI_HOCMDR = cmd;
//     // set wrte data
//     PECI_HOWRDR = data;
//     // start
//     PECI_HOCTLR |= BIT0;
// }
// u8 peci_read(void)
// {
//     PECI_HOCTL2R &= ~BIT7;
//     while (!(PECI_HOSTAR & BIT1))
//     {
//     }
//     // get read data
//     u8 rdata = PECI_HORDDR;
//     // clear status register
//     PECI_HOSTAR |= BIT1; // clear
//     // commmand completed
//     return rdata;
// }
// void peci_test(void)
// {
//     peci_write(0x30, 0, 0, 0, 0);
//     printf("PECI read: %#x\n", peci_read());
// }
#endif
#endif


