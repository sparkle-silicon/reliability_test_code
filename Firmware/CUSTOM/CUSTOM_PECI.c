/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-07-12 15:22:54
 * @Description:
 *
 *
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 娴犮儰绗呴弰顖欒厬閺傚洤寮烽懟杈ㄦ瀮閻楀牊娼堥崥灞绢劄婢圭増妲戦敍宀€绱惍浣疯礋UTF-8閵嗭拷
 * Copyright has legal effects and violations will be prosecuted.
 * 閻楀牊娼堥崗閿嬫箒濞夋洖绶ラ弫鍫濆閿涘矁绻氶崣宥呯箑缁岃翰鈧拷
 *
 * Copyright 婕�2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 閻楀牊娼堥幍鈧張锟� 婕�2021-2023姒瑦娅犻惌鍐插磹鐎甸棿缍嬬粔鎴炲Η閿涘牐瀚冨鐑囩礆閺堝妾洪崗顒€寰�
 */

#include "KERNEL_PECI.H"
#include "CUSTOM_PECI.H"
#include "CUSTOM_GPIO.H"

#ifdef AE103
/*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/
#define SUPPORT_PECI_SUBROUTINE TRUE
#define SUPPORT_PECI_POWER_CTRL TRUE
#define SUPPORT_SOFTWARE_AWFCS FALSE
#define DEBUG_PECI FALSE

/*-----------------------------------------------------------------------------
 * Depends on parameter definition
 *---------------------------------------------------------------------------*/
#ifndef SUPPORT_PECI
#define SUPPORT_PECI TRUE
#endif

#if DEBUG_PECI
#define PECI_RamDebug(x) RamDebug(x)
#else
#define PECI_RamDebug(x) \
    {                    \
    }
#endif

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
#if 1
void Init_PECI(void)
{
    // dprint("a:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);

    // PECI_HOSTAR = PECI_HOSTAR_LSB_FIRST;

    PECI_Set_Pad(0x3, 0x0); // 闁瀚�1.0V鐠佸墽鐤嗛敍灞界杽闂勫懓绶崙锟�1.05V

    // dprint("b:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);

    PECI_HOCTLR |= BIT7; // 鐠佸墽鐤咹HRAE = 1

    PECI_HOCTL2R &= 0xF8;

    // dprint("c:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);
    // PECI_HOCTL2R |= 0x00;  //PECI host optimal transfer rate. = 1.8MHz
    PECI_HOCTL2R |= 0x01; // PECI host optimal transfer rate. = 1MHz <-KBL
    // PECI_HOCTL2R |= 0x04;  //PECI host optimal transfer rate. = 1.5MHz

    // dprint("d:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);

    PECI_HOCTL2R &= ~BIT7; // 鐠佸墽鐤咹HRAE = 0

    PECI_ERRCNT = 0;

    // // dprint("e:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);
    PECI_HOCTLR |= BIT3; // PECI Host Enable
    // // dprint("f:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);
}

// PADsel: 1:缂侇垵宕电划铏规媼閸撗呮瀭PAD闁挎冻鎷�0闁挎稒纰嶈啯闁秆勵殙椤旀洜绱旈悽鍦揇
void PECI_Set_Pad(u16 VTTlev, u8 PADsel)
{
    if(PADsel)
    {
        SYSCTL_PAD_PECI &= ~(0b1111 << 8);
        SYSCTL_PAD_PECI |= BIT(15) | ((VTTlev & 0b1111) << 8);
    }
    else
    {
        SYSCTL_PAD_PECI &= ~BIT(15); // 鐠佸墽鐤�304e4閻ㄥ垺it15娑擄拷0閿涘矂鈧瀚ㄦ担璺ㄦ暏PECI鐎靛嫬鐡ㄩ崳銊︽降闁板秶鐤哖AD閻㈤潧甯�
        PECI_HOCTLR |= BIT7;         // 鐠佸墽鐤咹HRAE = 1
        PECI_PADCTLR &= 0xfc;
        PECI_PADCTLR |= VTTlev & 0b11;
        PECI_HOCTL2R &= ~BIT7; // 鐠佸墽鐤咹HRAE = 0
    }
}
void PECI_Init(u16 VTTlev, u8 PADsel)
{
    // PECIHEN
    PECI_HOCTL2R &= ~BIT7; // set PECI_HOCTRL
    PECI_HOCTLR |= BIT3;   // PECI Host Enable
    PECI_Set_Pad(0x01, 0x1);
    PECI_HOSTAR = PECI_HOSTAR_LSB_FIRST;
}
void peci_write(u8 target_addr, u8 wlen, u8 rlen, u8 cmd, u8 data)
{
    // Set Target address
    PECI_HOCTL2R &= ~BIT7;
    PECI_HOTRADDR = target_addr;
    // Set Write length and read length
    PECI_HORDLR = rlen;
    PECI_HOWRDR = wlen;
    // set command data
    PECI_HOCMDR = cmd;
    // set wrte data
    PECI_HOWRDR = data;
    // start
    PECI_HOCTLR |= BIT0;
}
u8 peci_read(void)
{
    PECI_HOCTL2R &= ~BIT7;
    while(!(PECI_HOSTAR & BIT1))
    {
    }
    // get read data
    u8 rdata = PECI_HORDDR;
    // clear status register
    PECI_HOSTAR |= BIT1; // clear
                         // commmand completed
    return rdata;
}
void peci_test(void)
{
    peci_write(0x30, 0, 0, 0, 0);
    printf("PECI read: %#x\n", peci_read());
}
#endif

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
    if(PECI_DELAY > 0)
    {
        PECI_DELAY--;
        return;
    }

    // PECI_HOCTLR = 0x08; /* PECI module only enable one time */
    // PECI_HOSTAR = 0xEE; // 0xFE;      /* ResetPECIStatus  */

    //-------------------------------------------------------------------------

    // PECI_ReadDIB();

    // PECI_PING_TEST();
    // PECI_ReadDIB_TEST();
    // PECI_ReadTemp_TEST();
    // PECI_RdPkgConfig_TEST();

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

#if 0
    if(!(PECI_FLAG & F_PECI_INIT))
    {
        if(PECI_ReadDIB())
        {
            PECI_FLAG |= F_PECI_INIT;
        }
        return;
    }
#if SUPPORT_PECI_POWER_CTRL
    else if(!(PECI_FLAG & F_PECI_INIT2))
    {
        if(PECI_ReadPowerUnit())
        {
            PECI_FLAG |= F_PECI_INIT2;
        }
        return;
    }
    else if(!(PECI_FLAG & F_PECI_INIT3))
    {
        if(Read_BC_ACOK() && Read_BATT_PRS_1())
        {
            if(PECI_WritePowerLimit2(45))
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
    else if(!(PECI_FLAG & F_PECI_INIT4))
    {
        if(Read_BC_ACOK() && Read_BATT_PRS_1())
        {
            if(PECI_WritePowerLimit4(66))
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
    else if(!(PECI_FLAG & F_PECI_INIT5))
    {
        PECI_FLAG |= F_PECI_INIT5;
        //PECI_WritePowerLimit3(32);       
        return;
    }
#endif
    else
    {
        if((PECI_RdPkgCfg_Idx16[2] < 80) || (PECI_RdPkgCfg_Idx16[2] > 120))
        {   // Read TjMax again if the value is zero or not normal(lower than 80'c, larger than 120'c).
            if(PECI_RdPkgConfig(_PECI_CPU_ADDR, PECI_RdPkgCfg_Idx16, _PECI_Domain_0,
                0, _PECI_Index_TTR, 0, 0, 5, 5))
            {
                PECI_RdPkgCfg_Idx16_CC = PECI_CompletionCode;
            }
        }
        else
        {
        #if 1
            if(PECI_ReadTemp())
            {
                Calc_CPU_Temperature();
            }
        #else
            switch(PECI_CNT)
            {
                case 0:
                    if(PECI_ReadTemp())
                    {
                        Calc_CPU_Temperature();
                    }
                    PECI_CNT++;
                    break;
                case 1:
                    if(Read_BC_ACOK() && Read_BATT_PRS_1())
                    {
                        if(PECI_PowerLimit2 != 45)
                        {
                            if(PECI_WritePowerLimit2(45))
                            {
                                PECI_ReadPowerLimit2();
                                if(PECI_PowerLimit2 == 0xFF)
                                {
                                    break;
                                }
                            }
                        }
                    }
                    PECI_CNT++;
                    break;
                case 2:
                    if(Read_BC_ACOK() && Read_BATT_PRS_1())
                    {
                        if(PECI_PowerLimit4 != 66)
                        {
                            if(PECI_WritePowerLimit4(66))
                            {
                                PECI_ReadPowerLimit4();
                                if(PECI_PowerLimit4 == 0xFF)
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

    while(PECI_FLAG & F_PECI_BUSY)
    {
        if((PECI_HOSTAR & 0xEC) != 0)
        {
            /* Error Bit */
            if(PECI_ERRCNT < 255)
            {
                PECI_ERRCNT++;
            }
            PECI_ERRSTS = PECI_HOSTAR;
            PECI_HOCTLR = 0x08;      /* PECI module only enable one time */
            //PECI_HOCTLR = 0x00;    /* PECI_HostDisable (Will have Glitch Noise)*/
            PECI_HOSTAR = 0xFE;  /* ResetPECIStatus  */
            CLEAR_MASK(PECI_FLAG, F_PECI_BUSY);
        }
        else if(IS_MASK_SET(PECI_HOSTAR, PECI_HOSTAR_FINISH))
        {
            PECI_ERRCNT = 0;
            PECI_ERRSTS = PECI_HOSTAR;
            // Tmp_XPntr = (unsigned int *)PECI_MEMADR;
            if(PECI_CMD == _PECI_CMD_GetTemp)
            {
                for(i = 0; i < PECI_HORDLR; i++)
                {
                    PECI_Get_Temp[i] = PECI_HORDDR;
                }
            }
            else if(PECI_CMD == _PECI_CMD_GetDIB)
            {
                for(i = 0; i < PECI_HORDLR; i++)
                {
                    PECI_Get_DIB[i] = PECI_HORDDR;
                }
            }
            else if(PECI_CMD == _PECI_CMD_RdPkgConfig)
            {
                PECI_RdPkgCfg_Idx16_CC = PECI_HORDDR;
                for(i = 0; i < PECI_HORDLR; i++)
                {
                    PECI_RdPkgCfg_Idx16[i] = PECI_HORDDR;
                }
            }
            else
            {
                for(i = 0x00; i < PECI_HORDLR; i++)
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

            if(PECI_CMD == _PECI_CMD_GetDIB)
            {
                PECI_FLAG |= F_PECI_INIT;
            }
            if(PECI_CMD == _PECI_CMD_GetTemp)
            {
                Calc_CPU_Temperature();
            }
        }
        else
        {
            PECI_TIMER++;
            if(PECI_TIMER > 200)
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
    if(PECI_DELAY > 0)
    {
        PECI_DELAY--;
        return;
    }
    if(PECI_FLAG & F_PECI_BUSY)
    {
        return;
    }
    //-------------------------------------------------------------------------
    PECI_HOCTLR = 0x08;      /* PECI module only enable one time */
    //PECI_HOCTLR = 0x00;    /* PECI_HostDisable (Will have Glitch Noise)*/
    PECI_HOSTAR = 0xFE;      /* ResetPECIStatus  */
    /* PECI_HostEnable(); */
    PECI_HOCTLR = (PECI_HOCTLR_FIFOCLR + PECI_HOCTLR_FCSERR_ABT + PECI_HOCTLR_PECIHEN + PECI_HOCTLR_CONCTRL);
    if(!(PECI_FLAG & F_PECI_INIT))
    {
        PECI_HOTRADDR = _PECI_CPU_ADDR;
        PECI_HOWRLR = 0x01;
        PECI_HORDLR = 0x08;
        PECI_CMD = _PECI_CMD_GetDIB;
        PeciDataPointer = &PECI_Get_DIB[0];
    }
#if SUPPORT_PECI_POWER_CTRL
    else if(!(PECI_FLAG & F_PECI_INIT2))
    {
        PECI_FLAG |= F_PECI_INIT2;
        PECI_ReadPowerUnit();
        return;
    }
    else if(!(PECI_FLAG & F_PECI_INIT3))
    {
        PECI_FLAG |= F_PECI_INIT3;
        // PECI_WritePowerLimit2(15);      /* Example */
        // PECI_WritePowerLimit1(13, 20);  /* Example 13Watt, 20sec */
        return;
    }
    else if(!(PECI_FLAG & F_PECI_INIT4))
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
        if(PECI_RdPkgCfg_Idx16[2] == 0x00)
        {
            PECI_HOTRADDR = _PECI_CPU_ADDR;
            PECI_HOWRLR = 0x05;
            PECI_HORDLR = 0x05;
            PECI_CMD = _PECI_CMD_RdPkgConfig;
            PeciDataPointer = &PECI_RdPkgCfg_Idx16_CC;
        }
        else
        {
            PECI_HOTRADDR = _PECI_CPU_ADDR;
            PECI_HOWRLR = 0x01;
            PECI_HORDLR = 0x02;
            PECI_CMD = _PECI_CMD_GetTemp;
            PeciDataPointer = &PECI_Get_Temp[0];
        }
    }
    PECI_HOCMDR = PECI_CMD;

    if(PECI_CMD == _PECI_CMD_RdPkgConfig)
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
    if(PECI_FLAG & F_PECI_UPDATED)
    {
        CLEAR_MASK(PECI_FLAG, F_PECI_UPDATED);
        //---------------------------------------------------------------------
        _C1 = (PECI_Get_Temp[1] << 8) + PECI_Get_Temp[0];
        _C1 = (~_C1) + 1; /* 2's complement */
        _R8 = _C1 >> 6;   /* 1/64 degrees centigrade */
        if(Processor_Tj_max == 0)
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
            if((_R8 & 0x80) == 0x00)
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

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_ReadDIB
 * @function - The function of PECI3.0 Package read DIB
 * @upstream - By call
 * @input    - None
 * @return   - 1 : done     0 : error
 * @note     - None
 */
BYTE PECI_ReadDIB(void)
{
    BYTE done, index;
    done = 0x00;

    PECI_HOCTLR = (PECI_HOCTLR_FIFOCLR + PECI_HOCTLR_FCSERR_ABT + PECI_HOCTLR_PECIHEN + PECI_HOCTLR_CONCTRL);
    PECI_HOCTLR &= 0xFE;
    PECI_HOTRADDR = _PECI_CPU_ADDR;
    PECI_HOWRLR = 0x01;
    PECI_HORDLR = 0x08;
    PECI_HOCMDR = _PECI_CMD_GetDIB;

    // dprint("1:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);

    if(PECI_CheckHostBusy())
    {
        PECI_HOCTLR |= PECI_HOCTLR_START; /* PECI_HostControl(PECI_HOCTLR_START) */
        // dprint("2:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);
        if(PECI_CheckHostFinish())
        {
            for(index = 0x00; index < PECI_HORDLR; index++)
            {
                PECI_Get_DIB[index] = PECI_HORDDR;
                // dprint("3:%x\n", PECI_Get_DIB[index]);
            }
            done = 0x01;
        }
    }

    PECI_HOCTLR = 0x00; /* PECI_HostDisable */
    PECI_HOSTAR = 0xEE; // 0xFE;      /* ResetPECIStatus  */
    return (done);
}

/******PECI 濞ｈ濮濻YNC鐢嶇礉闁俺绻冮柅蹇庣炊閺傜懓绱￠崣鎴︹偓浣规殶閹诡噯绱濋崙鑺ユ殶閺€鐟板晸-start*************/
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
    PECI_HOCTLR &= (~PECI_HOCTLR_CONCTRL);    // 闁板秶鐤嗛幀鑽ゅ殠閸愯尙鐛婃稉宥嗘杹瀵啯婀板▎鈥茬炊鏉堟挷濞囬懗鎴掔秴
    PECI_HOCTLR &= (~PECI_HOCTLR_FCSERR_ABT); // 闁板秶鐤咶CS ERROR閸氬簼绗夐弨鎯х磾閺堫剚顐兼导鐘虹翻娴ｈ儻鍏樻担锟�

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_HOCTL2R |= 0x20;             // 鏉╂稑鍙嗛柅蹇庣炊濡€崇础

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;     // 瀵偓閸氼垱婀板▎鈥茬炊鏉堬拷

    /***************************************************************/

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x80;             // 鐠佸墽鐤嗛柅蹇庣炊濡€崇础娑撳褰傞柅涓凞DR

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOTRADDR = _PECI_CPU_ADDR_TEST;  // 闁板秶鐤咰PU PECI 娴犲孩婧€閸︽澘娼冩稉锟�0x0C

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x40;             // 闁瀚ㄩ崣鎴︹偓浜€rite length

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOWRLR = 0x00;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x20;             // 闁瀚ㄩ崣鎴︹偓涔篹ad length

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HORDLR = 0x21;

    // PECI_HOCMDR = 0x1E;

    // PECI_HOCTLR |= 0x40; // 闁板秶鐤咥WFCS_FRC_CTRL = 1

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_AWFCSV = 0xE0;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE);  // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_CONCTRL;    // 闁板秶鐤嗛幀鑽ゅ殠閸愯尙鐛婃稉宥嗘杹瀵啯婀板▎鈥茬炊鏉堟挷濞囬懗鎴掔秴
    // PECI_HOCTLR |= PECI_HOCTLR_FCSERR_ABT; // 闁板秶鐤咶CS ERROR閸氬簼绗夐弨鎯х磾閺堫剚顐兼导鐘虹翻娴ｈ儻鍏樻担锟�

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_HOCTL2R |= 0x20;             // 鏉╂稑鍙嗛柅蹇庣炊濡€崇础

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;     // 瀵偓閸氼垱婀板▎鈥茬炊鏉堬拷

    if(PECI_CheckHostBusy())
    {
        PECI_HOCTLR |= PECI_HOCTLR_START; // 瀵偓閸氼垱婀板▎鈥茬炊鏉堬拷
        // dprint("2:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);
        if(PECI_CheckHostFinish())
        {
            // for (index = 0x00; index < 10; index++) // 闁繋绱跺Ο鈥崇础娑撳绱濋幍鈧張濉朙AVE閸ョ偟娈戦弫鐗堝祦闁垝绱扮悮顐ｆ杹閸掔増甯撮弨绂橧FO娑擄拷
            // {
            //     // PECI_Get_DIB_TEST[index] = PECI_HORDDR;
            //     // dprint("3:%x\n", PECI_Get_DIB[index]);
            // }

            // for (index = 0x00; index < 10; index++) // 闁繋绱跺Ο鈥崇础娑撳绱濋幍鈧張濉朙AVE閸ョ偟娈戦弫鐗堝祦闁垝绱扮悮顐ｆ杹閸掔増甯撮弨绂橧FO娑擄拷
            // {
            //     if (index == 9)
            //     {
            //         // dprint("%x\n", PECI_Get_DIB_TEST[index]);
            //     }
            //     else
            //     {
            //         PECI_Get_DIB_TEST[index] = (PECI_Get_DIB_TEST[index] << 3) | (PECI_Get_DIB_TEST[index + 1] >> 5); // 閻㈠彉绨径姘絺娴滐拷5bit閺佺増宓侀敍灞惧娴犮儵娓剁憰浣哥殺鏉╋拷5bit閺佺増宓佹稉銏犵磾閿涘矂鍣搁弬鎵╂担宥囩矋閸氬牆鍤锝団€橀惃鍕殶閹诡噯绱濇稉宥堢箖缁楊兛绔存稉鐙S閻㈠彉绨悮顐℃丢瀵啩绨￠敍灞惧娴犮儱褰ч懗钘夌繁閸掞拷8BYTE閻ㄥ嫭鏆熼幑顔兼嫲閺堚偓閸氬簼绔存稉鐙S
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
    PECI_HOCTLR &= (~PECI_HOCTLR_CONCTRL);    // 闁板秶鐤嗛幀鑽ゅ殠閸愯尙鐛婃稉宥嗘杹瀵啯婀板▎鈥茬炊鏉堟挷濞囬懗鎴掔秴
    PECI_HOCTLR &= (~PECI_HOCTLR_FCSERR_ABT); // 闁板秶鐤咶CS ERROR閸氬簼绗夐弨鎯х磾閺堫剚顐兼导鐘虹翻娴ｈ儻鍏樻担锟�

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_HOCTL2R |= 0x20;             // 鏉╂稑鍙嗛柅蹇庣炊濡€崇础

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;     // 瀵偓閸氼垱婀板▎鈥茬炊鏉堬拷

    /***************************************************************/

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x80;             // 鐠佸墽鐤嗛柅蹇庣炊濡€崇础娑撳褰傞柅涓凞DR

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOTRADDR = _PECI_CPU_ADDR_TEST;  // 闁板秶鐤咰PU PECI 娴犲孩婧€閸︽澘娼冩稉锟�0x0C

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x40;             // 闁瀚ㄩ崣鎴︹偓浜€rite length

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOWRLR = 0x00;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x20;             // 闁瀚ㄩ崣鎴︹偓涔篹ad length

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HORDLR = 0x21;

    // PECI_HOCMDR = 0x1E;

    // PECI_HOCTLR |= 0x40; // 闁板秶鐤咥WFCS_FRC_CTRL = 1

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_AWFCSV = 0xE0;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE);  // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_CONCTRL;    // 闁板秶鐤嗛幀鑽ゅ殠閸愯尙鐛婃稉宥嗘杹瀵啯婀板▎鈥茬炊鏉堟挷濞囬懗鎴掔秴
    // PECI_HOCTLR |= PECI_HOCTLR_FCSERR_ABT; // 闁板秶鐤咶CS ERROR閸氬簼绗夐弨鎯х磾閺堫剚顐兼导鐘虹翻娴ｈ儻鍏樻担锟�

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_HOCTL2R |= 0x20;             // 鏉╂稑鍙嗛柅蹇庣炊濡€崇础

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;     // 瀵偓閸氼垱婀板▎鈥茬炊鏉堬拷

    if(PECI_CheckHostBusy())
    {
        PECI_HOCTLR |= PECI_HOCTLR_START; // 瀵偓閸氼垱婀板▎鈥茬炊鏉堬拷
        // dprint("2:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);
        if(PECI_CheckHostFinish())
        {
            for(index = 0x00; index < 10; index++) // 闁繋绱跺Ο鈥崇础娑撳绱濋幍鈧張濉朙AVE閸ョ偟娈戦弫鐗堝祦闁垝绱扮悮顐ｆ杹閸掔増甯撮弨绂橧FO娑擄拷
            {
                PECI_Get_DIB_TEST[index] = PECI_HORDDR;
                // dprint("3:%x\n", PECI_Get_DIB[index]);
            }

            for(index = 0x00; index < 10; index++) // 闁繋绱跺Ο鈥崇础娑撳绱濋幍鈧張濉朙AVE閸ョ偟娈戦弫鐗堝祦闁垝绱扮悮顐ｆ杹閸掔増甯撮弨绂橧FO娑擄拷
            {
                if(index == 9)
                {
                    // dprint("%x\n", PECI_Get_DIB_TEST[index]);
                }
                else
                {
                    PECI_Get_DIB_TEST[index] = (PECI_Get_DIB_TEST[index] << 3) | (PECI_Get_DIB_TEST[index + 1] >> 5); // 閻㈠彉绨径姘絺娴滐拷5bit閺佺増宓侀敍灞惧娴犮儵娓剁憰浣哥殺鏉╋拷5bit閺佺増宓佹稉銏犵磾閿涘矂鍣搁弬鎵╂担宥囩矋閸氬牆鍤锝団€橀惃鍕殶閹诡噯绱濇稉宥堢箖缁楊兛绔存稉鐙S閻㈠彉绨悮顐℃丢瀵啩绨￠敍灞惧娴犮儱褰ч懗钘夌繁閸掞拷8BYTE閻ㄥ嫭鏆熼幑顔兼嫲閺堚偓閸氬簼绔存稉鐙S
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
    PECI_HOCTLR &= (~PECI_HOCTLR_CONCTRL);    // 闁板秶鐤嗛幀鑽ゅ殠閸愯尙鐛婃稉宥嗘杹瀵啯婀板▎鈥茬炊鏉堟挷濞囬懗鎴掔秴
    PECI_HOCTLR &= (~PECI_HOCTLR_FCSERR_ABT); // 闁板秶鐤咶CS ERROR閸氬簼绗夐弨鎯х磾閺堫剚顐兼导鐘虹翻娴ｈ儻鍏樻担锟�

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_HOCTL2R |= 0x20;             // 鏉╂稑鍙嗛柅蹇庣炊濡€崇础

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;     // 瀵偓閸氼垱婀板▎鈥茬炊鏉堬拷

    /***************************************************************/

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x80;             // 鐠佸墽鐤嗛柅蹇庣炊濡€崇础娑撳褰傞柅涓凞DR

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOTRADDR = _PECI_CPU_ADDR_TEST;  // 闁板秶鐤咰PU PECI 娴犲孩婧€閸︽澘娼冩稉锟�0x0C

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x40;             // 闁瀚ㄩ崣鎴︹偓浜€rite length

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOWRLR = 0x00;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x20;             // 闁瀚ㄩ崣鎴︹偓涔篹ad length

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HORDLR = 0x21;

    // PECI_HOCMDR = 0x1E;

    // PECI_HOCTLR |= 0x40; // 闁板秶鐤咥WFCS_FRC_CTRL = 1

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_AWFCSV = 0xE0;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE);  // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_CONCTRL;    // 闁板秶鐤嗛幀鑽ゅ殠閸愯尙鐛婃稉宥嗘杹瀵啯婀板▎鈥茬炊鏉堟挷濞囬懗鎴掔秴
    // PECI_HOCTLR |= PECI_HOCTLR_FCSERR_ABT; // 闁板秶鐤咶CS ERROR閸氬簼绗夐弨鎯х磾閺堫剚顐兼导鐘虹翻娴ｈ儻鍏樻担锟�

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_HOCTL2R |= 0x20;             // 鏉╂稑鍙嗛柅蹇庣炊濡€崇础

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;     // 瀵偓閸氼垱婀板▎鈥茬炊鏉堬拷

    if(PECI_CheckHostBusy())
    {
        PECI_HOCTLR |= PECI_HOCTLR_START; // 瀵偓閸氼垱婀板▎鈥茬炊鏉堬拷
        // dprint("2:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);
        if(PECI_CheckHostFinish())
        {
            // for (index = 0x00; index < 10; index++) // 闁繋绱跺Ο鈥崇础娑撳绱濋幍鈧張濉朙AVE閸ョ偟娈戦弫鐗堝祦闁垝绱扮悮顐ｆ杹閸掔増甯撮弨绂橧FO娑擄拷
            // {
            //     PECI_Get_DIB_TEST[index] = PECI_HORDDR;
            //     // dprint("3:%x\n", PECI_Get_DIB[index]);
            // }

            // for (index = 0x00; index < 10; index++) // 闁繋绱跺Ο鈥崇础娑撳绱濋幍鈧張濉朙AVE閸ョ偟娈戦弫鐗堝祦闁垝绱扮悮顐ｆ杹閸掔増甯撮弨绂橧FO娑擄拷
            // {
            //     if (index == 9)
            //     {
            //         // dprint("%x\n", PECI_Get_DIB_TEST[index]);
            //     }
            //     else
            //     {
            //         PECI_Get_DIB_TEST[index] = (PECI_Get_DIB_TEST[index] << 3) | (PECI_Get_DIB_TEST[index + 1] >> 5); // 閻㈠彉绨径姘絺娴滐拷5bit閺佺増宓侀敍灞惧娴犮儵娓剁憰浣哥殺鏉╋拷5bit閺佺増宓佹稉銏犵磾閿涘矂鍣搁弬鎵╂担宥囩矋閸氬牆鍤锝団€橀惃鍕殶閹诡噯绱濇稉宥堢箖缁楊兛绔存稉鐙S閻㈠彉绨悮顐℃丢瀵啩绨￠敍灞惧娴犮儱褰ч懗钘夌繁閸掞拷8BYTE閻ㄥ嫭鏆熼幑顔兼嫲閺堚偓閸氬簼绔存稉鐙S
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
    PECI_HOCTLR &= (~PECI_HOCTLR_CONCTRL);    // 闁板秶鐤嗛幀鑽ゅ殠閸愯尙鐛婃稉宥嗘杹瀵啯婀板▎鈥茬炊鏉堟挷濞囬懗鎴掔秴
    PECI_HOCTLR &= (~PECI_HOCTLR_FCSERR_ABT); // 闁板秶鐤咶CS ERROR閸氬簼绗夐弨鎯х磾閺堫剚顐兼导鐘虹翻娴ｈ儻鍏樻担锟�

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_HOCTL2R |= 0x20;             // 鏉╂稑鍙嗛柅蹇庣炊濡€崇础

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;     // 瀵偓閸氼垱婀板▎鈥茬炊鏉堬拷

    /***************************************************************/

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x80;             // 鐠佸墽鐤嗛柅蹇庣炊濡€崇础娑撳褰傞柅涓凞DR

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOTRADDR = _PECI_CPU_ADDR_TEST;  // 闁板秶鐤咰PU PECI 娴犲孩婧€閸︽澘娼冩稉锟�0x0C

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x40;             // 闁瀚ㄩ崣鎴︹偓浜€rite length

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOWRLR = 0x00;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x20;             // 闁瀚ㄩ崣鎴︹偓涔篹ad length

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HORDLR = 0x21;

    // PECI_HOCMDR = 0x1E;

    // PECI_HOCTLR |= 0x40; // 闁板秶鐤咥WFCS_FRC_CTRL = 1

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_AWFCSV = 0xE0;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE);  // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_CONCTRL;    // 闁板秶鐤嗛幀鑽ゅ殠閸愯尙鐛婃稉宥嗘杹瀵啯婀板▎鈥茬炊鏉堟挷濞囬懗鎴掔秴
    // PECI_HOCTLR |= PECI_HOCTLR_FCSERR_ABT; // 闁板秶鐤咶CS ERROR閸氬簼绗夐弨鎯х磾閺堫剚顐兼导鐘虹翻娴ｈ儻鍏樻担锟�

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_HOCTL2R |= 0x20;             // 鏉╂稑鍙嗛柅蹇庣炊濡€崇础

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;     // 瀵偓閸氼垱婀板▎鈥茬炊鏉堬拷

    if(PECI_CheckHostBusy())
    {
        PECI_HOCTLR |= PECI_HOCTLR_START; // 瀵偓閸氼垱婀板▎鈥茬炊鏉堬拷
        // dprint("2:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);
        if(PECI_CheckHostFinish())
        {
            // for (index = 0x00; index < 10; index++) // 闁繋绱跺Ο鈥崇础娑撳绱濋幍鈧張濉朙AVE閸ョ偟娈戦弫鐗堝祦闁垝绱扮悮顐ｆ杹閸掔増甯撮弨绂橧FO娑擄拷
            // {
            //     PECI_Get_DIB_TEST[index] = PECI_HORDDR;
            //     // dprint("3:%x\n", PECI_Get_DIB[index]);
            // }

            // for (index = 0x00; index < 10; index++) // 闁繋绱跺Ο鈥崇础娑撳绱濋幍鈧張濉朙AVE閸ョ偟娈戦弫鐗堝祦闁垝绱扮悮顐ｆ杹閸掔増甯撮弨绂橧FO娑擄拷
            // {
            //     if (index == 9)
            //     {
            //         // dprint("%x\n", PECI_Get_DIB_TEST[index]);
            //     }
            //     else
            //     {
            //         PECI_Get_DIB_TEST[index] = (PECI_Get_DIB_TEST[index] << 3) | (PECI_Get_DIB_TEST[index + 1] >> 5); // 閻㈠彉绨径姘絺娴滐拷5bit閺佺増宓侀敍灞惧娴犮儵娓剁憰浣哥殺鏉╋拷5bit閺佺増宓佹稉銏犵磾閿涘矂鍣搁弬鎵╂担宥囩矋閸氬牆鍤锝団€橀惃鍕殶閹诡噯绱濇稉宥堢箖缁楊兛绔存稉鐙S閻㈠彉绨悮顐℃丢瀵啩绨￠敍灞惧娴犮儱褰ч懗钘夌繁閸掞拷8BYTE閻ㄥ嫭鏆熼幑顔兼嫲閺堚偓閸氬簼绔存稉鐙S
            //         // dprint("%x\n", PECI_Get_DIB_TEST[index]);
            //     }
            // }

            done = 0x01;
        }
    }

    return (done);
}
/******PECI 濞ｈ濮濻YNC鐢嶇礉闁俺绻冮柅蹇庣炊閺傜懓绱￠崣鎴︹偓浣规殶閹诡噯绱濋崙鑺ユ殶閺€鐟板晸-end*************/

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_ReadTemp
 * @function - The function of PECI3.0 Package read temp
 * @upstream - By call
 * @input    - None
 * @return   - 1 : done     0 : error
 * @note     - None
 */
BYTE PECI_ReadTemp(void)
{
    BYTE done, i;
    done = 0x00;

    /* PECI_HostEnable(); */
    PECI_HOCTLR = (PECI_HOCTLR_FIFOCLR + PECI_HOCTLR_FCSERR_ABT + PECI_HOCTLR_PECIHEN + PECI_HOCTLR_CONCTRL);

    if(!(PECI_FLAG & F_PECI_BUSY))
    {
        PECI_HOTRADDR = _PECI_CPU_ADDR;
        PECI_HOWRLR = 0x01;
        PECI_HORDLR = 0x02;
        PECI_HOCMDR = _PECI_CMD_GetTemp;
        PECI_HOCTLR |= PECI_HOCTLR_START; /* PECI_HostControl(PECI_HOCTLR_START) */
        PECI_TIMER = 0;
        PECI_FLAG |= F_PECI_BUSY;
    }

    while(PECI_FLAG & F_PECI_BUSY)
    {
        if((PECI_HOSTAR & 0xEC) != 0)
        {
            /* Error Bit */
            if(PECI_ERRCNT < 255)
            {
                PECI_ERRCNT++;
            }
            PECI_ERRSTS = PECI_HOSTAR;
            PECI_HOCTLR = 0x08; /* PECI module only enable one time */
            // PECI_HOCTLR = 0x00;    /* PECI_HostDisable (Will have Glitch Noise)*/
            PECI_HOSTAR = 0xFE; /* ResetPECIStatus  */
            CLEAR_MASK(PECI_FLAG, F_PECI_BUSY);
            done = 0x00;
        }
        else if(IS_MASK_SET(PECI_HOSTAR, PECI_HOSTAR_FINISH))
        {
            PECI_ERRCNT = 0;
            PECI_ERRSTS = PECI_HOSTAR;
            for(i = 0; i < PECI_HORDLR; i++)
            {
                PECI_Get_Temp[i] = PECI_HORDDR;
            }

            PECI_HOCTLR = 0x08; /* PECI module only enable one time */
            // PECI_HOCTLR = 0x00;    /* PECI_HostDisable (Will have Glitch Noise)*/
            PECI_HOSTAR = 0xFE; /* ResetPECIStatus  */

            CLEAR_MASK(PECI_FLAG, F_PECI_BUSY);
            PECI_FLAG |= F_PECI_UPDATED;
            done = 0x01;
        }
        else
        {
            PECI_TIMER++;
            if(PECI_TIMER > 200)
            {
                PECI_TIMER = 0;
                PECI_OVTCT++;
                PECI_HOCTLR = 0x00; /* PECI_HostDisable */
                PECI_HOSTAR = 0xFE; /* ResetPECIStatus  */
                CLEAR_MASK(PECI_FLAG, F_PECI_BUSY);
            }
            done = 0x00;
        }
    }
    return (done);
}

#if SUPPORT_PECI_POWER_CTRL
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
 * Power Control Register Unit Calculations:
 * Time:   1s / 2^TIME UNIT,   Default Value: 1s / 2^10 = 976 閿熺淳gs
 * Energy: 1J / 2^ENERGY UNIT, Default Value: 1J / 2^16 = 15.3 閿熺淳gJ
 * Power:  1W / 2^POWER UNIT,  Default Value: 1W / 2^3 = 1/8 W
 */
BYTE PECI_ReadPowerUnit(void)
{
    if(PECI_RdPkgConfig(_PECI_CPU_ADDR, PECI_ReadBuffer, _PECI_Domain_0,
        0, _PECI_Index_PPSH, 0, 0, 5, 5))
    {
        PECI_PowerUnit = (PECI_ReadBuffer[0] & 0x0F);  /* Bit[3:0] */
        PECI_EnergyUnit = (PECI_ReadBuffer[1] & 0x1F); /* Bit[12:8] */
        PECI_TimeUnit = (PECI_ReadBuffer[2] & 0x0F);   /* Bit[19:16] */
    #if 1
        PECI_UnitPower = 0x0008;
        PECI_UnitTime = 0x0400;
        PECI_UnitEnergy = 0x4000;
    #else
        if(PECI_PowerUnit > 0)
        {
            PECI_UnitPower = 1 << PECI_PowerUnit;
            PECI_UnitEnergy = 1 << PECI_EnergyUnit;
            PECI_UnitTime = 1 << PECI_TimeUnit;
        }
        else
        {
            PECI_UnitPower = 0x0008;
            PECI_UnitTime = 0x0400;
            PECI_UnitEnergy = 0x4000;
        }
    #endif
        return TRUE;
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_ReadPowerLimit1
 * @function - The function of PECI3.0 Package read power limit 1
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
BYTE PECI_ReadPowerLimit1(void)
{
    if(!PECI_ReadPowerUnit())
    {
        return FALSE;
    }
    if(PECI_RdPkgConfig(_PECI_CPU_ADDR, &PECI_ReadBuffer[0],
        _PECI_Domain_0,
        0, _PECI_Index_PPL1, 0, 0, 5, 5))
    {

        _C1 = (PECI_ReadBuffer[1] << 8) + PECI_ReadBuffer[0];
        _C1 &= 0x7FFF;
        PECI_PowerLimit1 = (_C1 / PECI_UnitPower);

        _R8 = PECI_ReadBuffer[2] >> 1;
        PECI_PowerLimit1T = (_R8 / PECI_UnitTime);
        return TRUE;
    }
    else
    {
        PECI_PowerLimit1 = 0xFF;
        return FALSE;
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
BYTE PECI_ReadPowerLimit2(void)
{
    if(!PECI_ReadPowerUnit())
    {
        return FALSE;
    }

    if(PECI_RdPkgConfig(_PECI_CPU_ADDR, &PECI_ReadBuffer[0],
        _PECI_Domain_0,
        0, _PECI_Index_PPL2, 0, 0, 5, 5))
    {
        _C1 = (PECI_ReadBuffer[1] << 8) + PECI_ReadBuffer[0];
        _C1 &= 0x7FFF;
        PECI_PowerLimit2 = (_C1 / PECI_UnitPower);
        return TRUE;
    }
    else
    {
        PECI_PowerLimit2 = 0xFF;
        return FALSE;
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
BYTE PECI_WritePowerLimit1(BYTE SettingWatts, BYTE SettingTimer)
{
    WORD wTEMP;

    if(!PECI_ReadPowerUnit())
    {
        return FALSE;
    }

    wTEMP = (SettingWatts * PECI_UnitPower); // 13 * 20
    PECI_WriteBuffer[0] = wTEMP;
    PECI_WriteBuffer[1] = (wTEMP >> 8) | 0x80;

    _R8 = 0x11 << 1;                  //(SettingTimer * PECI_UnitTime) << 1;
    PECI_WriteBuffer[2] = _R8 | 0x01; // Bit0: Clamp Mode
    PECI_WriteBuffer[3] = 0;

    if(PECI_WrPkgConfig(_PECI_CPU_ADDR, &PECI_WriteBuffer[0], _PECI_Domain_0,
        0, _PECI_Index_PPL1, 0, 0, 1, 10))
    {
        return TRUE;
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_WritePowerLimit2
 * @function - The function of PECI3.0 Package write power limit 2
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
BYTE PECI_WritePowerLimit2(BYTE SettingWatts)
{
    WORD wTEMP;

    if(!PECI_ReadPowerUnit())
    {
        return FALSE;
    }

    wTEMP = (PECI_UnitPower * SettingWatts);
    PECI_WriteBuffer[0] = wTEMP;
    PECI_WriteBuffer[1] = (wTEMP >> 8) | 0x80;

    _R8 = 0x11 << 1;                  //(SettingTimer * PECI_UnitTime) << 1;
    PECI_WriteBuffer[2] = _R8 | 0x01; // Bit0: Clamp Mode
    PECI_WriteBuffer[3] = 0;

    if(PECI_WrPkgConfig(_PECI_CPU_ADDR, &PECI_WriteBuffer[0],
        _PECI_Domain_0,
        0, _PECI_Index_PPL2, 0, 0, 1, 10))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_ReadPowerLimit3
 * @function - The function of PECI3.0 Package read power limit 3
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 *---------------------------------------------------------------------------*/
BYTE PECI_ReadPowerLimit3(void)
{
    if(!PECI_ReadPowerUnit())
    {
        return FALSE;
    }

    if(PECI_RdPkgConfig(_PECI_CPU_ADDR, &PECI_ReadBuffer[0],
        _PECI_Domain_0,
        0, _PECI_Index_PPL3, 0, 0, 5, 5))
    {
        _C1 = (PECI_ReadBuffer[1] << 8) + PECI_ReadBuffer[0];
        _C1 &= 0x7FFF;
        PECI_PowerLimit3 = (_C1 / PECI_UnitPower);
        return TRUE;
    }
    else
    {
        PECI_PowerLimit3 = 0xFF;
        return FALSE;
    }
}

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_WritePowerLimit3
 * @function - The function of PECI3.0 Package write power limit 3
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 *---------------------------------------------------------------------------*/
BYTE PECI_WritePowerLimit3(BYTE SettingWatts)
{
    WORD wTEMP;

    if(!PECI_ReadPowerUnit())
    {
        return FALSE;
    }

    wTEMP = (PECI_UnitPower * SettingWatts);
    PECI_WriteBuffer[0] = wTEMP;
    PECI_WriteBuffer[1] = (wTEMP >> 8) | 0x80;
    PECI_WriteBuffer[2] = 0;
    PECI_WriteBuffer[3] = 0;

    if(PECI_WrPkgConfig(_PECI_CPU_ADDR, &PECI_WriteBuffer[0],
        _PECI_Domain_0,
        0, _PECI_Index_PPL3, 0, 0, 1, 10))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_ReadPowerLimit4
 * @function - The function of PECI3.0 Package read power limit 4
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 *---------------------------------------------------------------------------*/
BYTE PECI_ReadPowerLimit4(void)
{
    if(!PECI_ReadPowerUnit())
    {
        return FALSE;
    }

    if(PECI_RdPkgConfig(_PECI_CPU_ADDR, &PECI_ReadBuffer[0],
        _PECI_Domain_0,
        0, _PECI_Index_PPL4, 0, 0, 5, 5))
    {
        _C1 = (PECI_ReadBuffer[1] << 8) + PECI_ReadBuffer[0];
        _C1 &= 0x7FFF;
        PECI_PowerLimit4 = (_C1 / PECI_UnitPower);
        return TRUE;
    }
    else
    {
        PECI_PowerLimit4 = 0xFF;
        return FALSE;
    }
}

/*-----------------------------------------------------------------------------
 * @subroutine - PECI_WritePowerLimit4
 * @function - The function of PECI3.0 Package write power limit 4
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 *---------------------------------------------------------------------------*/
BYTE PECI_WritePowerLimit4(BYTE SettingWatts)
{
    WORD wTEMP;

    if(!PECI_ReadPowerUnit())
    {
        return FALSE;
    }

    wTEMP = (SettingWatts * PECI_UnitPower);
    PECI_WriteBuffer[0] = wTEMP;
    PECI_WriteBuffer[1] = (wTEMP >> 8) | 0x80;
    PECI_WriteBuffer[2] = 0;
    PECI_WriteBuffer[3] = 0;

    if(PECI_WrPkgConfig(_PECI_CPU_ADDR, &PECI_WriteBuffer[0],
        _PECI_Domain_0,
        0, _PECI_Index_PPL4, 0, 0, 1, 10))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
//-----------------------------------------------------------------------------
#endif // SUPPORT_PECI_POWER_CTRL
//-----------------------------------------------------------------------------

#endif
