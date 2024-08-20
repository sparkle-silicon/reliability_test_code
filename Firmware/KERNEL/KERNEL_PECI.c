/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-02-07 15:57:07
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
#include "KERNEL_PECI.H" //鍏ㄥ眬鍙橀噺
#ifdef AE103
#if PECI_Softwave_AWFCS
u8 PECI_CRC8;
#endif

BYTE PECI_Get_DIB[8];
BYTE PECI_Get_DIB_TEST[10];
BYTE PECI_Get_Temp[2];
BYTE PECI_RdPkgCfg_Idx16[5];
BYTE PECI_ReadBuffer[16];
BYTE PECI_WriteBuffer[16];

// 涓婇潰娴嬭瘯鐢�
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

    if (IS_MASK_CLEAR(temp, BIT7))
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

    if (IS_MASK_CLEAR(temp, BIT7))
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
    while (timeout--)
    {

        if (IS_MASK_CLEAR(PECI_HOSTAR, PECI_HOSTAR_HOBY)) // Host free
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

    while (timeout--)
    {

        status = PECI_HOSTAR;
        if (status != 0x00)
        {
            if (IS_MASK_SET(status, PECI_HOSTAR_FINISH))
            {
                PECI_HOSTAR |= PECI_HOSTAR_FINISH; // 写1清除FINISH状态
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
            //     dprint("w0\n");
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
    if (error != 0x00)
    {
        dprint("f\n");
        result = 0x00; // error
    }
    else
    {
        result = 0x01; // finish
    }

    return (result);
}

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

    if (PECI_CheckHostBusy())
    {
        PECI_HostControl(PECI_HOCTLR_START);
        if (PECI_CheckHostFinish())
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

    if (PECI_CheckHostBusy())
    {
        PECI_HostControl(PECI_HOCTLR_START);
        if (PECI_CheckHostFinish())
        {
            for (index = 0x00; index < PECI_HORDLR; index++)
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

//*****************************************************************************
//
// PECI get temperature command
//
//  parameter :
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) Domain : 0 or 1锟紺
//                           0 : Domain 0, 1 : Domain 1锟紺
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
    if (Domain < 2)
    {
        PECI_HOCMDR = PECI_CMD_GetTemp + Domain;
    }
    else
    {
        PECI_HOCMDR = PECI_CMD_GetTemp;
    }

    if (PECI_CheckHostBusy())
    {
        PECI_HostControl(PECI_HOCTLR_START);
        if (PECI_CheckHostFinish())
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

//*****************************************************************************
//
// Read to the package configuration space (PCS) within the processor
//
//  parameter :
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) Domain : 0 or 1锟紺
//                           0 : Domain 0, 1 : Domain 1锟紺
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
    if (Domain < 2)
    {
        PECI_HOCMDR = PECI_CMD_RdPkgConfig + Domain;
    }
    else
    {
        PECI_HOCMDR = PECI_CMD_RdPkgConfig;
    }

    if (Retry < 2)
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

    if (PECI_CheckHostBusy())
    {
        PECI_HostControl(PECI_HOCTLR_START);
        if (PECI_CheckHostFinish())
        {
            if (PECI_HORDLR != 0x00)
            {
                for (cunt = 0x00; cunt < PECI_HORDLR; cunt++)
                {
                    if (cunt == 0x00)
                    {
                        PECI_CompletionCode = PECI_HORDDR;
                    }
                    else
                    {
                        *(ReadData + cunt - 1) = PECI_HORDDR;
                    }
                }

                if (PECI_CompletionCode == PECI_CC_Valid)
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
//              (3) Domain : 0 or 1锟紺
//                           0 : Domain 0, 1 : Domain 1锟紺
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

    if (Domain < 2)
    {
        PECI_HOCMDR = PECI_CMD_WrPkgConfig + Domain;
        PECI_CalcCRC8(PECI_CMD_WrPkgConfig + Domain);
    }
    else
    {
        PECI_HOCMDR = PECI_CMD_WrPkgConfig;
        PECI_CalcCRC8(PECI_CMD_WrPkgConfig);
    }

    if (Retry < 2)
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

    for (cunt = 0x00; cunt < (WriteLen - 6); cunt++)
    {
        PECI_HOWRDR = *(WriteData + cunt);
        PECI_CalcCRC8(*(WriteData + cunt));
    }

#if PECI_Softwave_AWFCS
    PECI_CRC8 ^= 0x80; // Inverted MSb of preliminary FCS reslut
    PECI_HOWRDR = PECI_CRC8;
#endif

    if (PECI_CheckHostBusy())
    {
        PECI_HostControl(PECI_HOCTLR_START);
        if (PECI_CheckHostFinish())
        {
            PECI_CompletionCode = PECI_HORDDR;
            if (PECI_CompletionCode == PECI_CC_Valid)
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
//              (3) Domain : 0 or 1锟紺
//                           0 : Domain 0, 1 : Domain 1锟紺
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
    if (Domain < 2)
    {
        PECI_HOCMDR = PECI_CMD_RdIAMSR + Domain;
    }
    else
    {
        PECI_HOCMDR = PECI_CMD_RdIAMSR;
    }

    if (Retry < 2)
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

    if (PECI_CheckHostBusy())
    {
        PECI_HostControl(PECI_HOCTLR_START);
        if (PECI_CheckHostFinish())
        {
            for (cunt = 0x00; cunt < PECI_HORDLR; cunt++)
            {
                if (cunt == 0x00)
                {
                    PECI_CompletionCode = PECI_HORDDR;
                }
                else
                {
                    *(ReadData + cunt - 1) = PECI_HORDDR;
                }
            }

            if (PECI_CompletionCode == PECI_CC_Valid)
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

#endif