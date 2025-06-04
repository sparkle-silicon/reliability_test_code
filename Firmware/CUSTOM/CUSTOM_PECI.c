/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-06-04 16:54:52
 * @Description:
 *
 *
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 濞寸姰鍎扮粭鍛村及椤栨瑨鍘柡鍌氭搐瀵兘鎳熸潏銊︾€柣妤€鐗婂鍫ュ触鐏炵虎鍔勫鍦濡叉垿鏁嶅畝鈧槐顏堟儘娴ｇ柉绀婾TF-8闁靛棴鎷�
 * Copyright has legal effects and violations will be prosecuted.
 * 闁绘鐗婂鍫ュ礂闁垮绠掓繛澶嬫礀缁躲儵寮崼婵嗩潝闁挎稑鐭佺换姘跺矗瀹ュ懐绠戠紒宀冪堪閳ь剨鎷�
 *
 * Copyright 濠曪拷2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 闁绘鐗婂鍫ュ箥閳ь剟寮甸敓锟� 濠曪拷2021-2023濮掝偅鐟﹀▍鐘绘儗閸愭彃纾归悗鐢告？缂嶅绮旈幋鐐参楅柨娑樼墣鐎氬啫顔忛悜鍥╃闁哄牆顦靛娲礂椤掆偓瀵帮拷
 */

#include "KERNEL_PECI.H"
#include "CUSTOM_PECI.H"
#include "CUSTOM_GPIO.H"

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

    PECI_Set_Pad(0x3, 0x0); // 闂侇偄顦扮€氾拷1.0V閻犱礁澧介悿鍡涙晬鐏炵晫鏉介梻鍕嚀缁额參宕欓敓锟�1.05V

    // dprint("b:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);

    PECI_HOCTLR |= BIT7; // 閻犱礁澧介悿鍜笻RAE = 1

    PECI_HOCTL2R &= 0xF8;

    // dprint("c:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);
    // PECI_HOCTL2R |= 0x00;  //PECI host optimal transfer rate. = 1.8MHz
    PECI_HOCTL2R |= 0x01; // PECI host optimal transfer rate. = 1MHz <-KBL
    // PECI_HOCTL2R |= 0x04;  //PECI host optimal transfer rate. = 1.5MHz

    // dprint("d:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);

    PECI_HOCTL2R &= ~BIT7; // 閻犱礁澧介悿鍜笻RAE = 0

    PECI_ERRCNT = 0;

    // // dprint("e:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);
    PECI_HOCTLR |= BIT3; // PECI Host Enable
    // // dprint("f:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);
}

// PADsel: 1:缂備緡鍨靛畷鐢靛垝閾忚濯奸柛鎾楀懏鐎璓AD闂佹寧鍐婚幏锟�0闂佹寧绋掔喊宥堝暞闂佺鍕垫畽妞ゆ梹娲滅槐鏃堟偨閸︽弴
void PECI_Set_Pad(u16 VTTlev, u8 PADsel)
{
    if(PADsel)
    {
        SYSCTL_PAD_PECI &= ~(0b1111 << 8);
        SYSCTL_PAD_PECI |= BIT(15) | ((VTTlev & 0b1111) << 8);
    }
    else
    {
        SYSCTL_PAD_PECI &= ~BIT(15); // 閻犱礁澧介悿锟�304e4闁汇劌鍨篿t15濞戞搫鎷�0闁挎稑鐭傞埀顒€顦扮€氥劍鎷呯捄銊︽殢PECI閻庨潧瀚悺銊╁闯閵婏附闄嶉梺鏉跨Ф閻ゅ摉AD闁汇垽娼х敮锟�
        PECI_HOCTLR |= BIT7;         // 閻犱礁澧介悿鍜笻RAE = 1
        PECI_PADCTLR &= 0xfc;
        PECI_PADCTLR |= VTTlev & 0b11;
        PECI_HOCTL2R &= ~BIT7; // 閻犱礁澧介悿鍜笻RAE = 0
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

/******PECI 婵烇綀顕ф慨婵籝NC閻㈩垎宥囩闂侇偅淇虹换鍐焻韫囧海鐐婇柡鍌滄嚀缁憋繝宕ｉ幋锔瑰亾娴ｈ娈堕柟璇″櫙缁辨繈宕欓懞銉︽闁衡偓閻熸澘鏅�-start*************/
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
    PECI_HOCTLR &= (~PECI_HOCTLR_CONCTRL);    // 闂佹澘绉堕悿鍡涘箑閼姐倕娈犻柛鎰皺閻涘﹥绋夊鍡樻澒鐎殿喖鍟﹢鏉库枎閳ヨ尙鐐婇弶鍫熸尫婵炲洭鎳楅幋鎺旂Т
    PECI_HOCTLR &= (~PECI_HOCTLR_FCSERR_ABT); // 闂佹澘绉堕悿鍜禖S ERROR闁告艾绨肩粭澶愬绩閹呯＞闁哄牜鍓氶鍏煎閻樿櫣缈诲ù锝堝劵閸忔ɑ鎷呴敓锟�

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_HOCTL2R |= 0x20;             // 閺夆晜绋戦崣鍡涙焻韫囧海鐐婃俊顖椻偓宕囩

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;     // 鐎殿喒鍋撻柛姘煎灡濠€鏉库枎閳ヨ尙鐐婇弶鍫嫹

    /***************************************************************/

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x80;             // 閻犱礁澧介悿鍡涙焻韫囧海鐐婃俊顖椻偓宕囩濞戞挸顑呰ぐ鍌炴焻娑撳嚍DR

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOTRADDR = _PECI_CPU_ADDR_TEST;  // 闂佹澘绉堕悿鍜癙U PECI 濞寸姴瀛╁┃鈧柛锔芥緲濞煎啯绋夐敓锟�0x0C

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x40;             // 闂侇偄顦扮€氥劑宕ｉ幋锔瑰亾娴溾偓rite length

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOWRLR = 0x00;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x20;             // 闂侇偄顦扮€氥劑宕ｉ幋锔瑰亾娑旂ad length

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HORDLR = 0x21;

    // PECI_HOCMDR = 0x1E;

    // PECI_HOCTLR |= 0x40; // 闂佹澘绉堕悿鍜FCS_FRC_CTRL = 1

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_AWFCSV = 0xE0;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE);  // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_CONCTRL;    // 闂佹澘绉堕悿鍡涘箑閼姐倕娈犻柛鎰皺閻涘﹥绋夊鍡樻澒鐎殿喖鍟﹢鏉库枎閳ヨ尙鐐婇弶鍫熸尫婵炲洭鎳楅幋鎺旂Т
    // PECI_HOCTLR |= PECI_HOCTLR_FCSERR_ABT; // 闂佹澘绉堕悿鍜禖S ERROR闁告艾绨肩粭澶愬绩閹呯＞闁哄牜鍓氶鍏煎閻樿櫣缈诲ù锝堝劵閸忔ɑ鎷呴敓锟�

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_HOCTL2R |= 0x20;             // 閺夆晜绋戦崣鍡涙焻韫囧海鐐婃俊顖椻偓宕囩

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;     // 鐎殿喒鍋撻柛姘煎灡濠€鏉库枎閳ヨ尙鐐婇弶鍫嫹

    if(PECI_CheckHostBusy())
    {
        PECI_HOCTLR |= PECI_HOCTLR_START; // 鐎殿喒鍋撻柛姘煎灡濠€鏉库枎閳ヨ尙鐐婇弶鍫嫹
        // dprint("2:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);
        if(PECI_CheckHostFinish())
        {
            // for (index = 0x00; index < 10; index++) // 闂侇偄绻嬬槐璺何熼垾宕囩濞戞挸顑戠槐婵嬪箥閳ь剟寮垫繅鏈橝VE闁搞儳鍋熷▓鎴﹀极閻楀牆绁﹂梺顔藉灊缁辨壆鎮锝嗘澒闁告帞澧楃敮鎾绩缁傛┃FO濞戞搫鎷�
            // {
            //     // PECI_Get_DIB_TEST[index] = PECI_HORDDR;
            //     // dprint("3:%x\n", PECI_Get_DIB[index]);
            // }

            // for (index = 0x00; index < 10; index++) // 闂侇偄绻嬬槐璺何熼垾宕囩濞戞挸顑戠槐婵嬪箥閳ь剟寮垫繅鏈橝VE闁搞儳鍋熷▓鎴﹀极閻楀牆绁﹂梺顔藉灊缁辨壆鎮锝嗘澒闁告帞澧楃敮鎾绩缁傛┃FO濞戞搫鎷�
            // {
            //     if (index == 9)
            //     {
            //         // dprint("%x\n", PECI_Get_DIB_TEST[index]);
            //     }
            //     else
            //     {
            //         PECI_Get_DIB_TEST[index] = (PECI_Get_DIB_TEST[index] << 3) | (PECI_Get_DIB_TEST[index + 1] >> 5); // 闁汇垹褰夌花顒佸緞濮橆剙绲哄ù婊愭嫹5bit闁轰胶澧楀畵渚€鏁嶇仦鎯ь暡濞寸姰鍎靛〒鍓佹啺娴ｅ摜娈洪弶鈺嬫嫹5bit闁轰胶澧楀畵浣圭▔閵忕姷纾鹃柨娑樼焸閸ｆ悂寮幍顔р晜鎷呭鍥╃煁闁告艾鐗嗛崵顓烆潰閿濆洠鈧﹢鎯冮崟顒佹闁硅鍣槐婵囩▔瀹ュ牏绠栫紒妤婂厸缁斿瓨绋夐悪顧塖闁汇垹褰夌花顒傛偖椤愨剝涓㈢€殿喖鍟╃花锟犳晬鐏炴儳顣插ù鐘劚瑜把囨嚄閽樺绻侀柛鎺炴嫹8BYTE闁汇劌瀚弳鐔煎箲椤斿吋瀚查柡鍫氬亾闁告艾绨肩粩瀛樼▔閻欘檳S
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
    PECI_HOCTLR &= (~PECI_HOCTLR_CONCTRL);    // 闂佹澘绉堕悿鍡涘箑閼姐倕娈犻柛鎰皺閻涘﹥绋夊鍡樻澒鐎殿喖鍟﹢鏉库枎閳ヨ尙鐐婇弶鍫熸尫婵炲洭鎳楅幋鎺旂Т
    PECI_HOCTLR &= (~PECI_HOCTLR_FCSERR_ABT); // 闂佹澘绉堕悿鍜禖S ERROR闁告艾绨肩粭澶愬绩閹呯＞闁哄牜鍓氶鍏煎閻樿櫣缈诲ù锝堝劵閸忔ɑ鎷呴敓锟�

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_HOCTL2R |= 0x20;             // 閺夆晜绋戦崣鍡涙焻韫囧海鐐婃俊顖椻偓宕囩

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;     // 鐎殿喒鍋撻柛姘煎灡濠€鏉库枎閳ヨ尙鐐婇弶鍫嫹

    /***************************************************************/

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x80;             // 閻犱礁澧介悿鍡涙焻韫囧海鐐婃俊顖椻偓宕囩濞戞挸顑呰ぐ鍌炴焻娑撳嚍DR

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOTRADDR = _PECI_CPU_ADDR_TEST;  // 闂佹澘绉堕悿鍜癙U PECI 濞寸姴瀛╁┃鈧柛锔芥緲濞煎啯绋夐敓锟�0x0C

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x40;             // 闂侇偄顦扮€氥劑宕ｉ幋锔瑰亾娴溾偓rite length

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOWRLR = 0x00;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x20;             // 闂侇偄顦扮€氥劑宕ｉ幋锔瑰亾娑旂ad length

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HORDLR = 0x21;

    // PECI_HOCMDR = 0x1E;

    // PECI_HOCTLR |= 0x40; // 闂佹澘绉堕悿鍜FCS_FRC_CTRL = 1

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_AWFCSV = 0xE0;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE);  // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_CONCTRL;    // 闂佹澘绉堕悿鍡涘箑閼姐倕娈犻柛鎰皺閻涘﹥绋夊鍡樻澒鐎殿喖鍟﹢鏉库枎閳ヨ尙鐐婇弶鍫熸尫婵炲洭鎳楅幋鎺旂Т
    // PECI_HOCTLR |= PECI_HOCTLR_FCSERR_ABT; // 闂佹澘绉堕悿鍜禖S ERROR闁告艾绨肩粭澶愬绩閹呯＞闁哄牜鍓氶鍏煎閻樿櫣缈诲ù锝堝劵閸忔ɑ鎷呴敓锟�

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_HOCTL2R |= 0x20;             // 閺夆晜绋戦崣鍡涙焻韫囧海鐐婃俊顖椻偓宕囩

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;     // 鐎殿喒鍋撻柛姘煎灡濠€鏉库枎閳ヨ尙鐐婇弶鍫嫹

    if(PECI_CheckHostBusy())
    {
        PECI_HOCTLR |= PECI_HOCTLR_START; // 鐎殿喒鍋撻柛姘煎灡濠€鏉库枎閳ヨ尙鐐婇弶鍫嫹
        // dprint("2:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);
        if(PECI_CheckHostFinish())
        {
            for(index = 0x00; index < 10; index++) // 闂侇偄绻嬬槐璺何熼垾宕囩濞戞挸顑戠槐婵嬪箥閳ь剟寮垫繅鏈橝VE闁搞儳鍋熷▓鎴﹀极閻楀牆绁﹂梺顔藉灊缁辨壆鎮锝嗘澒闁告帞澧楃敮鎾绩缁傛┃FO濞戞搫鎷�
            {
                PECI_Get_DIB_TEST[index] = PECI_HORDDR;
                // dprint("3:%x\n", PECI_Get_DIB[index]);
            }

            for(index = 0x00; index < 10; index++) // 闂侇偄绻嬬槐璺何熼垾宕囩濞戞挸顑戠槐婵嬪箥閳ь剟寮垫繅鏈橝VE闁搞儳鍋熷▓鎴﹀极閻楀牆绁﹂梺顔藉灊缁辨壆鎮锝嗘澒闁告帞澧楃敮鎾绩缁傛┃FO濞戞搫鎷�
            {
                if(index == 9)
                {
                    // dprint("%x\n", PECI_Get_DIB_TEST[index]);
                }
                else
                {
                    PECI_Get_DIB_TEST[index] = (PECI_Get_DIB_TEST[index] << 3) | (PECI_Get_DIB_TEST[index + 1] >> 5); // 闁汇垹褰夌花顒佸緞濮橆剙绲哄ù婊愭嫹5bit闁轰胶澧楀畵渚€鏁嶇仦鎯ь暡濞寸姰鍎靛〒鍓佹啺娴ｅ摜娈洪弶鈺嬫嫹5bit闁轰胶澧楀畵浣圭▔閵忕姷纾鹃柨娑樼焸閸ｆ悂寮幍顔р晜鎷呭鍥╃煁闁告艾鐗嗛崵顓烆潰閿濆洠鈧﹢鎯冮崟顒佹闁硅鍣槐婵囩▔瀹ュ牏绠栫紒妤婂厸缁斿瓨绋夐悪顧塖闁汇垹褰夌花顒傛偖椤愨剝涓㈢€殿喖鍟╃花锟犳晬鐏炴儳顣插ù鐘劚瑜把囨嚄閽樺绻侀柛鎺炴嫹8BYTE闁汇劌瀚弳鐔煎箲椤斿吋瀚查柡鍫氬亾闁告艾绨肩粩瀛樼▔閻欘檳S
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
    PECI_HOCTLR &= (~PECI_HOCTLR_CONCTRL);    // 闂佹澘绉堕悿鍡涘箑閼姐倕娈犻柛鎰皺閻涘﹥绋夊鍡樻澒鐎殿喖鍟﹢鏉库枎閳ヨ尙鐐婇弶鍫熸尫婵炲洭鎳楅幋鎺旂Т
    PECI_HOCTLR &= (~PECI_HOCTLR_FCSERR_ABT); // 闂佹澘绉堕悿鍜禖S ERROR闁告艾绨肩粭澶愬绩閹呯＞闁哄牜鍓氶鍏煎閻樿櫣缈诲ù锝堝劵閸忔ɑ鎷呴敓锟�

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_HOCTL2R |= 0x20;             // 閺夆晜绋戦崣鍡涙焻韫囧海鐐婃俊顖椻偓宕囩

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;     // 鐎殿喒鍋撻柛姘煎灡濠€鏉库枎閳ヨ尙鐐婇弶鍫嫹

    /***************************************************************/

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x80;             // 閻犱礁澧介悿鍡涙焻韫囧海鐐婃俊顖椻偓宕囩濞戞挸顑呰ぐ鍌炴焻娑撳嚍DR

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOTRADDR = _PECI_CPU_ADDR_TEST;  // 闂佹澘绉堕悿鍜癙U PECI 濞寸姴瀛╁┃鈧柛锔芥緲濞煎啯绋夐敓锟�0x0C

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x40;             // 闂侇偄顦扮€氥劑宕ｉ幋锔瑰亾娴溾偓rite length

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOWRLR = 0x00;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x20;             // 闂侇偄顦扮€氥劑宕ｉ幋锔瑰亾娑旂ad length

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HORDLR = 0x21;

    // PECI_HOCMDR = 0x1E;

    // PECI_HOCTLR |= 0x40; // 闂佹澘绉堕悿鍜FCS_FRC_CTRL = 1

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_AWFCSV = 0xE0;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE);  // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_CONCTRL;    // 闂佹澘绉堕悿鍡涘箑閼姐倕娈犻柛鎰皺閻涘﹥绋夊鍡樻澒鐎殿喖鍟﹢鏉库枎閳ヨ尙鐐婇弶鍫熸尫婵炲洭鎳楅幋鎺旂Т
    // PECI_HOCTLR |= PECI_HOCTLR_FCSERR_ABT; // 闂佹澘绉堕悿鍜禖S ERROR闁告艾绨肩粭澶愬绩閹呯＞闁哄牜鍓氶鍏煎閻樿櫣缈诲ù锝堝劵閸忔ɑ鎷呴敓锟�

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_HOCTL2R |= 0x20;             // 閺夆晜绋戦崣鍡涙焻韫囧海鐐婃俊顖椻偓宕囩

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;     // 鐎殿喒鍋撻柛姘煎灡濠€鏉库枎閳ヨ尙鐐婇弶鍫嫹

    if(PECI_CheckHostBusy())
    {
        PECI_HOCTLR |= PECI_HOCTLR_START; // 鐎殿喒鍋撻柛姘煎灡濠€鏉库枎閳ヨ尙鐐婇弶鍫嫹
        // dprint("2:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);
        if(PECI_CheckHostFinish())
        {
            // for (index = 0x00; index < 10; index++) // 闂侇偄绻嬬槐璺何熼垾宕囩濞戞挸顑戠槐婵嬪箥閳ь剟寮垫繅鏈橝VE闁搞儳鍋熷▓鎴﹀极閻楀牆绁﹂梺顔藉灊缁辨壆鎮锝嗘澒闁告帞澧楃敮鎾绩缁傛┃FO濞戞搫鎷�
            // {
            //     PECI_Get_DIB_TEST[index] = PECI_HORDDR;
            //     // dprint("3:%x\n", PECI_Get_DIB[index]);
            // }

            // for (index = 0x00; index < 10; index++) // 闂侇偄绻嬬槐璺何熼垾宕囩濞戞挸顑戠槐婵嬪箥閳ь剟寮垫繅鏈橝VE闁搞儳鍋熷▓鎴﹀极閻楀牆绁﹂梺顔藉灊缁辨壆鎮锝嗘澒闁告帞澧楃敮鎾绩缁傛┃FO濞戞搫鎷�
            // {
            //     if (index == 9)
            //     {
            //         // dprint("%x\n", PECI_Get_DIB_TEST[index]);
            //     }
            //     else
            //     {
            //         PECI_Get_DIB_TEST[index] = (PECI_Get_DIB_TEST[index] << 3) | (PECI_Get_DIB_TEST[index + 1] >> 5); // 闁汇垹褰夌花顒佸緞濮橆剙绲哄ù婊愭嫹5bit闁轰胶澧楀畵渚€鏁嶇仦鎯ь暡濞寸姰鍎靛〒鍓佹啺娴ｅ摜娈洪弶鈺嬫嫹5bit闁轰胶澧楀畵浣圭▔閵忕姷纾鹃柨娑樼焸閸ｆ悂寮幍顔р晜鎷呭鍥╃煁闁告艾鐗嗛崵顓烆潰閿濆洠鈧﹢鎯冮崟顒佹闁硅鍣槐婵囩▔瀹ュ牏绠栫紒妤婂厸缁斿瓨绋夐悪顧塖闁汇垹褰夌花顒傛偖椤愨剝涓㈢€殿喖鍟╃花锟犳晬鐏炴儳顣插ù鐘劚瑜把囨嚄閽樺绻侀柛鎺炴嫹8BYTE闁汇劌瀚弳鐔煎箲椤斿吋瀚查柡鍫氬亾闁告艾绨肩粩瀛樼▔閻欘檳S
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
    PECI_HOCTLR &= (~PECI_HOCTLR_CONCTRL);    // 闂佹澘绉堕悿鍡涘箑閼姐倕娈犻柛鎰皺閻涘﹥绋夊鍡樻澒鐎殿喖鍟﹢鏉库枎閳ヨ尙鐐婇弶鍫熸尫婵炲洭鎳楅幋鎺旂Т
    PECI_HOCTLR &= (~PECI_HOCTLR_FCSERR_ABT); // 闂佹澘绉堕悿鍜禖S ERROR闁告艾绨肩粭澶愬绩閹呯＞闁哄牜鍓氶鍏煎閻樿櫣缈诲ù锝堝劵閸忔ɑ鎷呴敓锟�

    PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    PECI_HOCTL2R |= 0x20;             // 閺夆晜绋戦崣鍡涙焻韫囧海鐐婃俊顖椻偓宕囩

    PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;     // 鐎殿喒鍋撻柛姘煎灡濠€鏉库枎閳ヨ尙鐐婇弶鍫嫹

    /***************************************************************/

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x80;             // 閻犱礁澧介悿鍡涙焻韫囧海鐐婃俊顖椻偓宕囩濞戞挸顑呰ぐ鍌炴焻娑撳嚍DR

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOTRADDR = _PECI_CPU_ADDR_TEST;  // 闂佹澘绉堕悿鍜癙U PECI 濞寸姴瀛╁┃鈧柛锔芥緲濞煎啯绋夐敓锟�0x0C

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x40;             // 闂侇偄顦扮€氥劑宕ｉ幋锔瑰亾娴溾偓rite length

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOWRLR = 0x00;

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_PADCTLR |= 0x20;             // 闂侇偄顦扮€氥劑宕ｉ幋锔瑰亾娑旂ad length

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HORDLR = 0x21;

    // PECI_HOCMDR = 0x1E;

    // PECI_HOCTLR |= 0x40; // 闂佹澘绉堕悿鍜FCS_FRC_CTRL = 1

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_AWFCSV = 0xE0;

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE);  // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_CONCTRL;    // 闂佹澘绉堕悿鍡涘箑閼姐倕娈犻柛鎰皺閻涘﹥绋夊鍡樻澒鐎殿喖鍟﹢鏉库枎閳ヨ尙鐐婇弶鍫熸尫婵炲洭鎳楅幋鎺旂Т
    // PECI_HOCTLR |= PECI_HOCTLR_FCSERR_ABT; // 闂佹澘绉堕悿鍜禖S ERROR闁告艾绨肩粭澶愬绩閹呯＞闁哄牜鍓氶鍏煎閻樿櫣缈诲ù锝堝劵閸忔ɑ鎷呴敓锟�

    // PECI_HOCTLR |= PECI_HOCTLR_HHRAE; // HHRAE = 1
    // PECI_HOCTL2R |= 0x20;             // 閺夆晜绋戦崣鍡涙焻韫囧海鐐婃俊顖椻偓宕囩

    // PECI_HOCTL2R &= (~PECI_HOCTLR_HHRAE); // HHRAE = 0
    // PECI_HOCTLR |= PECI_HOCTLR_START;     // 鐎殿喒鍋撻柛姘煎灡濠€鏉库枎閳ヨ尙鐐婇弶鍫嫹

    if(PECI_CheckHostBusy())
    {
        PECI_HOCTLR |= PECI_HOCTLR_START; // 鐎殿喒鍋撻柛姘煎灡濠€鏉库枎閳ヨ尙鐐婇弶鍫嫹
        // dprint("2:%x,%x\n", PECI_HOSTAR, PECI_HOCTLR);
        if(PECI_CheckHostFinish())
        {
            // for (index = 0x00; index < 10; index++) // 闂侇偄绻嬬槐璺何熼垾宕囩濞戞挸顑戠槐婵嬪箥閳ь剟寮垫繅鏈橝VE闁搞儳鍋熷▓鎴﹀极閻楀牆绁﹂梺顔藉灊缁辨壆鎮锝嗘澒闁告帞澧楃敮鎾绩缁傛┃FO濞戞搫鎷�
            // {
            //     PECI_Get_DIB_TEST[index] = PECI_HORDDR;
            //     // dprint("3:%x\n", PECI_Get_DIB[index]);
            // }

            // for (index = 0x00; index < 10; index++) // 闂侇偄绻嬬槐璺何熼垾宕囩濞戞挸顑戠槐婵嬪箥閳ь剟寮垫繅鏈橝VE闁搞儳鍋熷▓鎴﹀极閻楀牆绁﹂梺顔藉灊缁辨壆鎮锝嗘澒闁告帞澧楃敮鎾绩缁傛┃FO濞戞搫鎷�
            // {
            //     if (index == 9)
            //     {
            //         // dprint("%x\n", PECI_Get_DIB_TEST[index]);
            //     }
            //     else
            //     {
            //         PECI_Get_DIB_TEST[index] = (PECI_Get_DIB_TEST[index] << 3) | (PECI_Get_DIB_TEST[index + 1] >> 5); // 闁汇垹褰夌花顒佸緞濮橆剙绲哄ù婊愭嫹5bit闁轰胶澧楀畵渚€鏁嶇仦鎯ь暡濞寸姰鍎靛〒鍓佹啺娴ｅ摜娈洪弶鈺嬫嫹5bit闁轰胶澧楀畵浣圭▔閵忕姷纾鹃柨娑樼焸閸ｆ悂寮幍顔р晜鎷呭鍥╃煁闁告艾鐗嗛崵顓烆潰閿濆洠鈧﹢鎯冮崟顒佹闁硅鍣槐婵囩▔瀹ュ牏绠栫紒妤婂厸缁斿瓨绋夐悪顧塖闁汇垹褰夌花顒傛偖椤愨剝涓㈢€殿喖鍟╃花锟犳晬鐏炴儳顣插ù鐘劚瑜把囨嚄閽樺绻侀柛鎺炴嫹8BYTE闁汇劌瀚弳鐔煎箲椤斿吋瀚查柡鍫氬亾闁告艾绨肩粩瀛樼▔閻欘檳S
            //         // dprint("%x\n", PECI_Get_DIB_TEST[index]);
            //     }
            // }

            done = 0x01;
        }
    }

    return (done);
}
/******PECI 婵烇綀顕ф慨婵籝NC閻㈩垎宥囩闂侇偅淇虹换鍐焻韫囧海鐐婇柡鍌滄嚀缁憋繝宕ｉ幋锔瑰亾娴ｈ娈堕柟璇″櫙缁辨繈宕欓懞銉︽闁衡偓閻熸澘鏅�-end*************/

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
 * Time:   1s / 2^TIME UNIT,   Default Value: 1s / 2^10 = 976 闁跨喓娣砱s
 * Energy: 1J / 2^ENERGY UNIT, Default Value: 1J / 2^16 = 15.3 闁跨喓娣砱J
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

