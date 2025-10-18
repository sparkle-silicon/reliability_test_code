/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-18 21:05:35
 * @Description: This is about the  national crypto algorithm implementation
 *
 *
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 浠ヤ笅鏄腑鏂囧強鑻辨枃鐗堟潈鍚屾澹版槑锛岀紪鐮佷负UTF-8銆�
 * Copyright has legal effects and violations will be prosecuted.
 * 鐗堟潈鍏锋湁娉曞緥鏁堝姏锛岃繚鍙嶅繀绌躲€�
 *
 * Copyright 漏2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 鐗堟潈鎵€鏈� 漏2021-2023榫欐櫠鐭冲崐瀵间綋绉戞妧锛堣嫃宸烇級鏈夐檺鍏徃
 */
#include "KERNEL_PECI.H" //闁稿繈鍔岄惇顒勫矗濮椻偓閸ｏ拷
#if PECI_Softwave_AWFCS
u8 PECI_CRC8;
#endif

BYTE PECI_Get_DIB[8];
BYTE PECI_Get_DIB_TEST[10];
BYTE PECI_Get_Temp[2];
BYTE PECI_RdPkgCfg_Idx16[5];
BYTE PECI_ReadBuffer[16];
BYTE PECI_WriteBuffer[16];

//*****************************************************************************
//
// Enables the PECI host controller.
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void PECI_HostEnable(void)
{
    PECI_HOCTLR |= (PECI_HOCTLR_FIFOCLR + PECI_HOCTLR_FCSERR_ABT + PECI_HOCTLR_PECIHEN + PECI_HOCTLR_CONCTRL);
}

//*****************************************************************************
//
// Disable the PECI host controller.
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void PECI_HostDisable(void)
{
    // HOCTLR = 0x00;
    PECI_HOCTLR = PECI_HOCTLR_PECIHEN; // Do not disable PECI host controller if PECI host controller already enabling.
}

//*****************************************************************************
//
// The PECI host controller will perform the desired transaction.
//
//  parameter :
//      control : PECI_HOCTLR_START || AWFCS_EN
//
//  return :
//      none
//
//*****************************************************************************
void PECI_HostControl(BYTE control)
{
    PECI_HOCTLR |= control;
}

#if SUPPORT_PECI_SUBROUTINE
//*****************************************************************************
//
// Check PECI interface busy or not
//
//  parameter :
//      none
//
// return :
//          0 : [PECI_InitTimeOutTimer()] time out
//          1 : OK
//
//*****************************************************************************
BYTE PECI_CheckHostBusy(void)
{
    BYTE result = 0x00;
    u16 timeout = PECI_TIMEOUT;
    while(timeout--)
    {

        if(IS_MASK_CLEAR(PECI_HOSTAR, PECI_HOSTAR_HOBY)) // Host free
        {
            result = 0x01;
            break;
        }
        else // Host Busy
        {
        }
    }

    return (result);
}

//*****************************************************************************
//
// Check PECI interface finish or not
//
//  parameter :
//      none
//
// return :
//          0 : time-out or error
//          1 : finish
//
//*****************************************************************************
BYTE PECI_CheckHostFinish(void)
{
    BYTE error;
    BYTE status, result;
    error = 0x04; // Pre-set error

    u16 timeout = PECI_TIMEOUT;

    while(timeout--)
    {

        status = PECI_HOSTAR;
        if(status != 0x00)
        {
            if(IS_MASK_SET(status, PECI_HOSTAR_FINISH))
            {
                PECI_HOSTAR |= PECI_HOSTAR_FINISH; // 閸愶拷1濞撳懘娅嶧INISH閻樿埖鈧拷
                error = 0x00;
                break;
            }
            // else if (IS_MASK_SET(status, PECI_HOSTAR_RD_FCS_ERR))
            // {
            //     error = 0x01;
            //     break;
            // }
            // else if (IS_MASK_SET(status, PECI_HOSTAR_WR_FCS_ERR))
            // {
            //     error = 0x01;
            //     break;
            // }
            // else if (IS_MASK_SET(status, PECI_HOSTAR_EXTERR))
            // {
            //     SYSCTL_RST1 |= BIT(29); // Reset PECI interface
            //     error = 0x02;
            //     SYSCTL_RST1 &= ~BIT(29); // Reset PECI interface
            //     break;
            // }
            // else if (IS_MASK_SET(status, PECI_HOSTAR_BUSERR))
            // {
            //     dprint("w1\n");
            //     SYSCTL_RST1 |= BIT(29); // Reset PECI interface
            //     error = 0x02;
            //     SYSCTL_RST1 &= ~BIT(29); // Reset PECI interface
            //     break;
            // }
            // else if (IS_MASK_SET(status, PECI_HOSTAR_RCV_ERRCODE))
            // {
            //     error = 0x03;
            //     break;
            // }
        }
    }
    if(error != 0x00)
    {
        result = 0x00; // error
    }
    else
    {
        result = 0x01; // finish
    }

    return (result);
}

// 濞戞挸锕鏉棵圭€ｎ厾妲搁柣顫嫹
//*****************************************************************************
//
//  CRC-8 polynomial
//
//   parameter :
//       sourcebyte :
//
//   return :
//       none
//
//*****************************************************************************
void PECI_CalcCRC8(BYTE sourcebyte)
{
#if PECI_Softwave_AWFCS
    BYTE temp;
    PECI_CRC8 ^= sourcebyte;
    temp = PECI_CRC8;

    if(IS_MASK_CLEAR(temp, BIT7))
    {
        temp = temp << 1;
        PECI_CRC8 ^= temp;
    }
    else
    {
        temp = temp << 1;
        PECI_CRC8 ^= 0x09;
        PECI_CRC8 ^= temp;
    }

    if(IS_MASK_CLEAR(temp, BIT7))
    {
        temp = temp << 1;
        PECI_CRC8 ^= temp;
    }
    else
    {
        temp = temp << 1;
        PECI_CRC8 ^= 0x07;
        PECI_CRC8 ^= temp;
    }
#endif
}

//*****************************************************************************
//
// Read to the package configuration space (PCS) within the processor
//
//  parameter :
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) Domain : 0 or 1闁跨喓鏄�
//                           0 : Domain 0, 1 : Domain 1闁跨喓鏄�
//              (4) Retry   0 or 1
//              (5) Index
//              (6) LSB of parameter
//              (7) MSB of parameter
//              (8) ReadLen read length 2 or 3 or 5
//              (9) WriteLen write length 5
//  return :
//              1 : done
//              0 : error
//
//*****************************************************************************
BYTE PECI_RdPkgConfig(BYTE addr, BYTE *ReadData, BYTE Domain, BYTE Retry, BYTE Index, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen)
{
    BYTE done, cunt;
    done = 0x00;

    PECI_HostEnable();
    PECI_HOTRADDR = addr;
    PECI_HOWRLR = WriteLen;
    PECI_HORDLR = ReadLen;
    if(Domain < 2)
    {
        PECI_HOCMDR = PECI_CMD_RdPkgConfig + Domain;
    }
    else
    {
        PECI_HOCMDR = PECI_CMD_RdPkgConfig;
    }

    if(Retry < 2)
    {
        PECI_HOWRDR = (PECI_HostID << 1) + Retry;
    }
    else
    {
        PECI_HOWRDR = (PECI_HostID << 1);
    }

    PECI_HOWRDR = Index;
    PECI_HOWRDR = LSB;
    PECI_HOWRDR = MSB;

    if(PECI_CheckHostBusy())
    {
        PECI_HostControl(PECI_HOCTLR_START);
        if(PECI_CheckHostFinish())
        {
            if(PECI_HORDLR != 0x00)
            {
                for(cunt = 0x00; cunt < PECI_HORDLR; cunt++)
                {
                    if(cunt == 0x00)
                    {
                        PECI_CompletionCode = PECI_HORDDR;
                    }
                    else
                    {
                        *(ReadData + cunt - 1) = PECI_HORDDR;
                    }
                }

                if(PECI_CompletionCode == PECI_CC_Valid)
                {
                    done = 0x01;
                }
                else
                {
                    done = 0x00;
                }
            }
            else
            {
                done = 0x00;
            }
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return (done);
}

//*****************************************************************************
//
// Write to the package configuration space (PCS) within the processor
//
//  Input :
//              (1) addr : The address of processor
//              (2) *WriteData : the start address of variable to wirte data
//              (3) Domain : 0 or 1闁跨喓鏄�
//                           0 : Domain 0, 1 : Domain 1闁跨喓鏄�
//              (4) Retry   0 or 1
//              (5) Index
//              (6) LSB of parameter
//              (7) MSB of parameter
//              (8) ReadLen read length 1
//              (9) WriteLen write length 0x07 or 0x08 or 0x0A
//  return :
//              1 : done
//              0 : error
//
//*****************************************************************************
BYTE PECI_WrPkgConfig(BYTE addr, BYTE *WriteData, BYTE Domain, BYTE Retry, BYTE Index, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen)
{
    BYTE done, cunt;
    done = 0x00;

    PECI_CompletionCode = 0x00;
    PECI_HostEnable();

#if PECI_Softwave_AWFCS
    PECI_CRC8 = 0x00;
#else
    PECI_HostControl(AWFCS_EN);
#endif

    PECI_HOTRADDR = addr;
    PECI_CalcCRC8(addr);
    PECI_HOWRLR = WriteLen;
    PECI_CalcCRC8(WriteLen);
    PECI_HORDLR = ReadLen;
    PECI_CalcCRC8(ReadLen);

    if(Domain < 2)
    {
        PECI_HOCMDR = PECI_CMD_WrPkgConfig + Domain;
        PECI_CalcCRC8(PECI_CMD_WrPkgConfig + Domain);
    }
    else
    {
        PECI_HOCMDR = PECI_CMD_WrPkgConfig;
        PECI_CalcCRC8(PECI_CMD_WrPkgConfig);
    }

    if(Retry < 2)
    {
        PECI_HOWRDR = (PECI_HostID << 1) + Retry;
        PECI_CalcCRC8((PECI_HostID << 1) + Retry);
    }
    else
    {
        PECI_HOWRDR = (PECI_HostID << 1);
        PECI_CalcCRC8(PECI_HostID << 1);
    }

    PECI_HOWRDR = Index;
    PECI_CalcCRC8(Index);
    PECI_HOWRDR = LSB;
    PECI_CalcCRC8(LSB);
    PECI_HOWRDR = MSB;
    PECI_CalcCRC8(MSB);

    for(cunt = 0x00; cunt < (WriteLen - 6); cunt++)
    {
        PECI_HOWRDR = *(WriteData + cunt);
        PECI_CalcCRC8(*(WriteData + cunt));
    }

#if PECI_Softwave_AWFCS
    PECI_CRC8 ^= 0x80; // Inverted MSb of preliminary FCS reslut
    PECI_HOWRDR = PECI_CRC8;
#endif

    if(PECI_CheckHostBusy())
    {
        PECI_HostControl(PECI_HOCTLR_START);
        if(PECI_CheckHostFinish())
        {
            PECI_CompletionCode = PECI_HORDDR;
            if(PECI_CompletionCode == PECI_CC_Valid)
            {
                done = 0x01;
            }
            else
            {
                done = 0x00;
            }
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return (done);
}

//*****************************************************************************
//
// Read to Model Specific Registers function
//
//  Input :
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) Domain : 0 or 1闁跨喓鏄�
//                           0 : Domain 0, 1 : Domain 1闁跨喓鏄�
//              (4) Retry   0 or 1
//              (5) ProcessorID
//              (6) LSB of parameter
//              (7) MSB of parameter
//              (8) ReadLen read length 0x02 or 0x03 or 0x05 or 0x09
//              (9) WriteLen write length 0x05
//  return :
//              1 : done
//              0 : error
//
//*****************************************************************************
BYTE PECI_RdIAMSR(BYTE addr, BYTE *ReadData, BYTE Domain, BYTE Retry, BYTE ProcessorID, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen)
{
    BYTE done, cunt;
    done = 0x00;

    PECI_HostEnable();
    PECI_HOTRADDR = addr;
    PECI_HOWRLR = WriteLen;
    PECI_HORDLR = ReadLen;
    if(Domain < 2)
    {
        PECI_HOCMDR = PECI_CMD_RdIAMSR + Domain;
    }
    else
    {
        PECI_HOCMDR = PECI_CMD_RdIAMSR;
    }

    if(Retry < 2)
    {
        PECI_HOWRDR = (PECI_HostID << 1) + Retry;
    }
    else
    {
        PECI_HOWRDR = (PECI_HostID << 1);
    }

    PECI_HOWRDR = ProcessorID;
    PECI_HOWRDR = LSB;
    PECI_HOWRDR = MSB;

    if(PECI_CheckHostBusy())
    {
        PECI_HostControl(PECI_HOCTLR_START);
        if(PECI_CheckHostFinish())
        {
            for(cunt = 0x00; cunt < PECI_HORDLR; cunt++)
            {
                if(cunt == 0x00)
                {
                    PECI_CompletionCode = PECI_HORDDR;
                }
                else
                {
                    *(ReadData + cunt - 1) = PECI_HORDDR;
                }
            }

            if(PECI_CompletionCode == PECI_CC_Valid)
            {
                done = 0x01;
            }
            else
            {
                done = 0x00;
            }
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return (done);
}
#endif  //SUPPORT_PECI_SUBROUTINE

//*****************************************************************************
//
// The function of clearing Host Status Register
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void ResetPECIStatus(void)
{
    PECI_HOSTAR = 0xFE;
}

//*****************************************************************************
//
// PECI Ping function
//
//  parameter :
//              (1) addr : The address of processor
//              (2) ReadLen read length always 0
//              (3) WriteLen Write length always 0
//  return :
//              1 : The targeted address of processor is able to respond
//              0 : no respond
//
//*****************************************************************************
BYTE PECI_Ping(BYTE addr, BYTE ReadLen, BYTE WriteLen)
{
    BYTE done;
    done = 0x00;

    PECI_HostEnable();
    PECI_HOTRADDR = addr;
    PECI_HOWRLR = WriteLen;
    PECI_HORDLR = ReadLen;

    if(PECI_CheckHostBusy())
    {
        PECI_HostControl(PECI_HOCTLR_START);
        if(PECI_CheckHostFinish())
        {
            done = 0x01;
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return (done);
}

//*****************************************************************************
//
// PECI get temperature command
//
//  parameter :
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) ReadLen read length always 8
//              (4) WriteLen Write length always 1
//  return :
//              1 : done
//              0 : error
//
//*****************************************************************************
BYTE PECI_GetDIB(BYTE addr, BYTE *ReadData, BYTE ReadLen, BYTE WriteLen)
{
    BYTE done, index;
    done = 0x00;

    PECI_HostEnable();
    PECI_HOTRADDR = addr;
    PECI_HOWRLR = WriteLen;
    PECI_HORDLR = ReadLen;
    PECI_HOCMDR = PECI_CMD_GetDIB;

    if(PECI_CheckHostBusy())
    {
        PECI_HostControl(PECI_HOCTLR_START);
        if(PECI_CheckHostFinish())
        {
            for(index = 0x00; index < PECI_HORDLR; index++)
            {
                *(ReadData + index) = PECI_HORDDR;
            }
            done = 0x01;
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return (done);
}

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
    PECI_HOCMDR = PECI_CMD_GetDIB;

    if(PECI_CheckHostBusy())
    {
        PECI_HOCTLR |= PECI_HOCTLR_START; /* PECI_HostControl(PECI_HOCTLR_START) */
        if(PECI_CheckHostFinish())
        {
            for(index = 0x00; index < PECI_HORDLR; index++)
            {
                PECI_Get_DIB[index] = PECI_HORDDR;
            }
            done = 0x01;
        }
    }

    PECI_HOCTLR = 0x00; /* PECI_HostDisable */
    PECI_HOSTAR = 0xEE; // 0xFE;      /* ResetPECIStatus  */
    return (done);
}

//*****************************************************************************
//
// PECI get temperature command
//
//  parameter :
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) Domain : 0 or 1闁跨喓鏄�
//                           0 : Domain 0, 1 : Domain 1闁跨喓鏄�
//              (4) ReadLen read length always 2
//              (5) WriteLen Write length always 1
//  return :
//              1 : done
//              0 : error
//
//*****************************************************************************
BYTE PECI_GetTemp(BYTE addr, BYTE *ReadData, BYTE Domain, BYTE ReadLen, BYTE WriteLen)
{
    BYTE done;
    done = 0x00;

    PECI_HostEnable();
    PECI_HOTRADDR = addr;
    PECI_HOWRLR = WriteLen;
    PECI_HORDLR = ReadLen;
    if(Domain < 2)
    {
        PECI_HOCMDR = PECI_CMD_GetTemp + Domain;
    }
    else
    {
        PECI_HOCMDR = PECI_CMD_GetTemp;
    }

    if(PECI_CheckHostBusy())
    {
        PECI_HostControl(PECI_HOCTLR_START);
        if(PECI_CheckHostFinish())
        {
            *ReadData = PECI_HORDDR;
            *(++ReadData) = PECI_HORDDR;
            done = 0x01;
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return (done);
}

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
        PECI_HOCMDR = PECI_CMD_GetTemp;
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
 * Time:   1s / 2^TIME UNIT,   Default Value: 1s / 2^10 = 976 闂佽法鍠撳ǎ鐮眘
 * Energy: 1J / 2^ENERGY UNIT, Default Value: 1J / 2^16 = 15.3 闂佽法鍠撳ǎ鐮盝
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

