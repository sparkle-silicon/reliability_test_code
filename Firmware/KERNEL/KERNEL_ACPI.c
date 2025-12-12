/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-11-15 18:43:39
 * @Description: This file is used for handling ACPI Commands
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
#include "AE_DEBUGGER.H"
#include "AE_FUNC.H"
#include "KERNEL_ACPI.H"
#include "KERNEL_TIMER.H"
#include "KERNEL_MEMORY.H"
#include "CUSTOM_POWER.H"
 //****************************************************************************
 //static function  declaration
 //****************************************************************************
static BYTE ACPI_SMB_NULL(void);
static BYTE ACPI_SMB_WQuick_CMD(void);
static BYTE ACPI_SMB_RQuick_CMD(void);
static BYTE ACPI_SMB_Send_BYTE(void);
static BYTE ACPI_SMB_Recive_BYTE(void);
static BYTE ACPI_SMB_Write_BYTE(void);
static BYTE ACPI_SMB_Read_BYTE(void);
static BYTE ACPI_SMB_Write_WORD(void);
static BYTE ACPI_SMB_Read_WORD(void);
static BYTE ACPI_SMB_Write_BLOCK(void);
static BYTE ACPI_SMB_Read_BLOCK(void);
static BYTE ACPI_SMB_Process_Call(void);
static BYTE ACPI_SMB_BLOCK_Call(void);
//****************************************************************************
//static function  declaration
//****************************************************************************
//----------------------------------------------------------------------------
// CUSTOM for ACPI Read EC RAM Space
//----------------------------------------------------------------------------
BYTE Read_Map_ECSPACE_BASE_ADDR(BYTE MapIndex)
{
    Tmp_XPntr = (VBYTE *)(ECSPACE_BASE_ADDR | MapIndex);
    return (*Tmp_XPntr);
}
//----------------------------------------------------------------------------
// CUSTOM for ACPI Write EC RAM Space
//----------------------------------------------------------------------------
void Write_Map_ECSPACE_BASE_ADDR(BYTE MapIndex, BYTE data1)
{
    Tmp_XPntr = (VBYTE *)(ECSPACE_BASE_ADDR | MapIndex);
    *Tmp_XPntr = data1;
    System_PowerState = SYSTEM_S0;
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// Read external ram Space
//----------------------------------------------------------------------------
void Read_Ext_RAMSpace(void)
{
    Tmp_XPntr = (VBYTE *)((PM1Data1 << 8) + PM1Data); //  address low
    PMC1_DOR = *Tmp_XPntr;
#if ENABLE_DEBUGGER_SUPPORT
    /* Debugger record */
    Debugger_KBC_PMC_Record(1, 1, *Tmp_XPntr);
#endif
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// Write external ram Space
//----------------------------------------------------------------------------
void Write_Ext_RAMSpace(void)
{
    Tmp_XPntr = (VBYTE *)((PM1Data2 << 8) + PM1Data1);
    *Tmp_XPntr = PM1Data;
}
//----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// ACPI EC Command 0x80:
// ACPI Spec.13.3.1 Read Embedded Controller, RD_EC (0x80)
// Read Command (3 Bytes) -- First Bytes: Port 66H <-0x80
// Byte #1 (Command byte Header) Interrupt on IBF=0
//-----------------------------------------------------------------------------
void EC6266_Cmd_80(void)
{
#if ACPI_SCI_Response
    SCI_Response(1); // Interrupt on IBF=0
#endif
    PM1Step = _PM1_STEP_1;
}
//-----------------------------------------------------------------------------
// ACPI Spec.13.3.1 Read Embedded Controller, RD_EC (0x80)
// Second Bytes: Port 62H -> Address Index (EC Space)
// Byte #2 (Address byte to read) No Interrupt
//-----------------------------------------------------------------------------
void EC62_Data_Step1(void)
{
    PM1Data1 = Read_Map_ECSPACE_BASE_ADDR(PM1Data);
    PMC1_DOR = PM1Data1;
#if ACPI_SCI_Response
    SCI_Response(1); // Interrupt on OBF=1
#endif
    PM1Step = 0;
#if ENABLE_DEBUGGER_SUPPORT
    /* Debugger record */
    Debugger_KBC_PMC_Record(1, 1, PM1Data1);
#endif
}
//-----------------------------------------------------------------------------
// ACPI EC Command 0x81:
// ACPI Spec.13.3.2 Write Embedded Controller, WR_EC (0x81)
// Write Command (3 Bytes) -- First Bytes: Port 66H <-0x81
// Byte #1 (Command byte Header) Interrupt on IBF=0
//-----------------------------------------------------------------------------
void EC6266_Cmd_81(void)
{
#if ACPI_SCI_Response
    SCI_Response(1); // Interrupt on IBF=0
#endif
    PM1Step = _PM1_STEP_2;
}
//-----------------------------------------------------------------------------
// ACPI Spec.13.3.2 Write Embedded Controller, WR_EC (0x81)
// Second Bytes: Port 62H -> Address Index (EC Space)
// Byte #2 (Address byte to write) Interrupt on IBF=0
//-----------------------------------------------------------------------------
void EC62_Data_Step2(void)
{
    PM1Data1 = PM1Data; //  Byte #2 Save Address Index
#if ACPI_SCI_Response
    SCI_Response(1); // Interrupt on OBF=1
#endif
    PM1Step = _PM1_STEP_3;
}
//-----------------------------------------------------------------------------
// ACPI Spec.13.3.2 Write Embedded Controller, WR_EC (0x81)
// Third Bytes: Port 62H -> Data Write to Address Index (EC Space)
// Byte #3 (data byte to write) Interrupt on IBF=0
//-----------------------------------------------------------------------------
void EC62_Data_Step3(void)
{
#if ACPI_SCI_Response
    SCI_Response(1); // Interrupt on OBF=1
#endif
    Write_Map_ECSPACE_BASE_ADDR(PM1Data1, PM1Data);
    PM1Step = 0;
}
//-----------------------------------------------------------------------------
// ACPI EC Command 0x82:
// ACPI Spec.13.3.3 Burst Enable Embedded Controller, BE_EC (0x82)
// Burst Enable Command (2 Bytes)
// Byte #1 (Command byte Header) No Interrupt
// The Embedded Controller sets the Burst bit of the Embedded Controller
// Status Register, puts the Burst Acknowledge byte (0x90) into the SCI output
// buffer, sets the OBF bit, and generates an SCI to signal OSPM that it is
// in Burst mode.
//-----------------------------------------------------------------------------
void EC6266_Cmd_82(void)
{
    SET_BIT(PMC1_STR, BURST_Mode); // PMC1_STR.4 Set Burst mode flag  FIXME Burs
    PMC1_DOR = 0x90;      // Byte #2 (Burst acknowledge byte)
#if ENABLE_DEBUGGER_SUPPORT
    /* Debugger record */
    Debugger_KBC_PMC_Record(1, 1, 0x90);
#endif
    SET_MASK(SYSTEM_MISC1, ACPI_OS);     // Auto Set ACPI Mode if Host Do ECCmd82
#if ACPI_SCI_Response
    SCI_Response(1); // Interrupt on IBF=0
#endif
}
//-----------------------------------------------------------------------------
// ACPI EC Command 0x83:
// ACPI Spec.13.3.4 Burst Disable Embedded Controller, BD_EC (0x83)
// Burst Disable Command (1 Byte)
// Byte #1 (Command byte Header) Interrupt on IBF=0
//-----------------------------------------------------------------------------
void EC6266_Cmd_83(void)
{
    CLEAR_BIT(PMC1_STR, BURST_Mode); /* PMC1_STR.4 Set Burst mode flag.        */
    /*  Clear Burst bit in secondary Host interface status register.*/
#if ACPI_SCI_Response
    SCI_Response(1); // Interrupt on IBF=0
#endif
}
//-----------------------------------------------------------------------------
// Burst mode or normal mode
//-----------------------------------------------------------------------------
int CheckBurstMode(void)
{
    WORD BurstLoopOut;
    BurstLoopOut = T_Burst_Loop;
    while ((PMC1_STR & IBF1) == 0x00)//检查一定时间内IBF是否置位
    {
        BurstLoopOut--;
        // if( TF1 || (BurstLoopOut==0) )  // Time-Out	//FIXME xia
        if ((TIMER3_TCR & TIMER_EN) || (BurstLoopOut == 0)) // Time-Out 退出突发模式
        {
            CLEAR_FLAG(PMC1_STR, BURST);
        #if ACPI_SCI_Response
            SCI_Response(1); // Generate Interrupt
        #endif
            TIMER_Disable(TIMER3);
            return (0);
        }
    }
    return (1);
}
//-----------------------------------------------------------------------------
// ACPI EC Command 0x84:
// ACPI Spec.13.3.5 Query Embedded Controller, QR_EC (0x84)
// Query Command (2 Bytes)
// Byte #1 (Command byte Header) No Interrupt
// Byte #2 (Query value to host) Interrupt on OBF=1
//-----------------------------------------------------------------------------
void EC6266_Cmd_84(void)
{
    /* Byte #2 (Query value to host).*/
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if (SUPPORT_ACPI_SMI)
    if (Is_Flag1(PMC1_STR, 5))
    {
        SCI_LastQueryEvent = Read_SCI_Query_Value();
    }
    else if (Is_Flag1(PMC1_STR, 6))
    {
        SCI_LastQueryEvent = Read_SMI_Query_Value();
    }
    else
    {
        SCI_LastQueryEvent = 0x00;
    }
#else
    SCI_LastQueryEvent = Read_SCI_Query_Value();
#endif
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    PMC1_DOR = SCI_LastQueryEvent;
#if ENABLE_DEBUGGER_SUPPORT
    Debugger_KBC_PMC_Record(1, 1, SCI_LastQueryEvent);
#endif
#if ACPI_SCI_Response
    SCI_Response(1); // Interrupt on IBF=0
#endif
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if SCI_EVENT_LOG
    if (SCI_LastQueryEvent > 0)
    {
    }
#endif
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    SCI_QueryEvent = 0x00;
    SCI_StepTimer = 0;
    WAIT_SCI_CENTER = 0;
}
void SCI_Low(void)
{
#if SCI_PIN_MODE_VW
    ESPI_VWIDX6 |= (F_IDX6_SCI_VALID + F_IDX6_SCI);
    ESPI_VWCTRL3 |= F_VW_INDEX_6_RESEND;
#else
    CLEAR_MASK(GPIO0_DR3, BIT(3));
#endif
}
void SCI_High(void)
{
#if SCI_PIN_MODE_VW
    ESPI_VWIDX6 &= ~F_IDX6_SCI;
    ESPI_VWCTRL3 |= F_VW_INDEX_6_RESEND;
#else
    SET_MASK(GPIO0_DR3, BIT(3));
#endif
}
//-----------------------------------------------------------------------------
// Generate SCIs in response to related transactions
//-----------------------------------------------------------------------------
void SCI_Response(BYTE sci_count)
{
    if (IS_MASK_CLEAR(SYSTEM_MISC1, ACPI_OS))
        return;
#if SCI_POLLING_CONTROL
    SCI_Count = 15;
    SCI_Response_Flag += sci_count;
#else
    for (BYTE i = 0; i < sci_count; i++)
    {
    #if SCI_PIN_MODE_VW
        SET_MASK(ESPI_VWIDX6, F_IDX6_SCI_VALID);
        CLEAR_MASK(ESPI_VWIDX6, F_IDX6_SCI);     /* eSPI SCI# */
        ESPI_VWCTRL3 |= F_VW_INDEX_6_RESEND;
        Loop_Delay(16);
        SET_MASK(ESPI_VWIDX6, F_IDX6_SCI_VALID);
        SET_MASK(ESPI_VWIDX6, F_IDX6_SCI);       /* eSPI SCI# */
        ESPI_VWCTRL3 |= F_VW_INDEX_6_RESEND;
    #else 
        SCI_Low();
        Loop_Delay(16);
        SCI_High();
    #endif
    }
#endif
}
//-----------------------------------------------------------------------------
// Generate SCIs for Query event request
//-----------------------------------------------------------------------------
void SCI_Interrupt(void)
{
    SCI_Low();
    Loop_Delay(16);
    SCI_High();
}
//-----------------------------------------------------------------------------
// FUNCTION: Loop Delay - Delay a number of microseconds.
// Input:  0x00 - 0xFF for delay timing.
//-----------------------------------------------------------------------------
void Loop_Delay(BYTE delay)
{
    vDelayXus(delay);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Generate SMIs for Query event request
//-----------------------------------------------------------------------------
void SMI_Interrupt(void)
{}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Upon receipt of the QR_EC command byte, the embedded
// controller places a notification byte with a value between 0-255,
// indicating the cause of the notification. The notification byte
// indicates which interrupt handler operation should be executed
// by OSPM to process the embedded controller SCI.
// The query value of zero is reserved for a spurious query result
// and indicates no outstanding event.
//-----------------------------------------------------------------------------
// Function: Read_SCI_Query_Value
//          Read Query event Value
//-----------------------------------------------------------------------------
BYTE Read_SCI_Query_Value(void)
{
    if (SCI_Event_Out_Index != SCI_Event_In_Index)
    {
        SCI_QueryEvent = SCI_Event_Buffer[SCI_Event_Out_Index];
        SCI_Event_Buffer[SCI_Event_Out_Index] = 0x00;
        SCI_Event_Out_Index++;
        if (SCI_Event_Out_Index > EVENT_BUFFER_SIZE)
        {
            SCI_Event_Out_Index = 0x00;
        }
        if (SCI_Event_Out_Index == SCI_Event_In_Index)
        {
            CLEAR_BIT(PMC1_STR, SCI_EVENT);
        }
        return SCI_QueryEvent;
    }
    else
    {
        CLEAR_BIT(PMC1_STR, SCI_EVENT);
        return 0;
    }
}
//-----------------------------------------------------------------------------
// Function: Write_SCI_Query_Value
//          Write Query Value to Buffer
//-----------------------------------------------------------------------------
BYTE Write_SCI_Query_Value(BYTE NewSCI)
{
    if (!ACPI_STATE_S0)
        return 0x00;
    if ((SCI_Event_Buffer[SCI_Event_In_Index]) == 0x00)
    {
        SCI_Event_Buffer[SCI_Event_In_Index] = NewSCI;
        SCI_Event_In_Index++;
    }
    if (SCI_Event_In_Index > EVENT_BUFFER_SIZE)
    {
        SCI_Event_In_Index = 0x00;
    }
    SET_BIT(PMC1_STR, SCI_EVENT);
    if (WAIT_SCI_CENTER)
        return 0x00;
    WAIT_SCI_CENTER = 1; // Recovery default SCI center
#if ACPI_SCI_Response
    SCI_Response(1);
#endif
    return 0x00;
}
//-----------------------------------------------------------------------------
// Function: Read_SMI_Query_Value
//          Read Query event Value
//-----------------------------------------------------------------------------
BYTE Read_SMI_Query_Value(void)
{
    SMI_QueryEvent = SMI_Event_Buffer[SMI_Event_Out_Index];
    if (SMI_QueryEvent == 0x00)
    {
        CLEAR_BIT(PMC1_STR, SMI_EVENT);
        return SCI_QueryEvent;
    }
    SMI_Event_Buffer[SMI_Event_Out_Index] = 0x00;
    SMI_Event_Out_Index++;
    if (SMI_Event_Out_Index > EVENT_BUFFER_SIZE)
    {
        SMI_Event_Out_Index = 0x00;
    }
    return SMI_QueryEvent;
}
//-----------------------------------------------------------------------------
// Function: Write_SMI_Query_Value
//          Write Query Value to Buffer
//-----------------------------------------------------------------------------
BYTE Write_SMI_Query_Value(BYTE NewSMI)
{
    if (NewSMI == 0x00)
        return 0x00;
    if ((SMI_Event_Buffer[SMI_Event_In_Index]) == 0x00)
    {
        SMI_Event_Buffer[SMI_Event_In_Index] = NewSMI;
        SMI_Event_In_Index++;
    }
    if (SMI_Event_In_Index > EVENT_BUFFER_SIZE)
    {
        SMI_Event_In_Index = 0x00;
    }
    return 0x00;
}
//-----------------------------------------------------------------------------
// Function: Clear_Event_Buffer
//          Clear SCI/SMI Event Buffer
//-----------------------------------------------------------------------------
void Clear_Event_Buffer(void)
{
    BYTE i;
    for (i = 0; i < EVENT_BUFFER_SIZE; i++)
    {
        SMI_Event_Buffer[i] = 0;
        SCI_Event_Buffer[i] = 0;
    }
    SCI_Event_In_Index = 0x00;
    SMI_Event_In_Index = 0x00;
    SCI_Event_Out_Index = 0x00;
    SMI_Event_Out_Index = 0x00;
}
//-----------------------------------------------------------------------------
// Function: Service_Event_Center
//-----------------------------------------------------------------------------
void Service_Event_Center(void)
{
    //-------------------------------------------------------------------------
    SCI_StepTimer++;
    if (SCI_StepTimer > 40)
    {
        SCI_StepTimer = 0;
        SCI_QueryEvent = SCI_Event_Buffer[SCI_Event_Out_Index];
        if (SCI_QueryEvent > 0x00 && ACPI_STATE_S0)
        {
            if (IS_MASK_SET(SYSTEM_MISC1, ACPI_OS))
            {
                SET_BIT(PMC1_STR, SCI_EVENT);
                CLEAR_BIT(PMC1_STR, SMI_EVENT);
                SCI_Interrupt();
                return;
            }
        #if (SUPPORT_ACPI_SMI)
            else
            {
                SMI_Interrupt();
            }
        #endif
        }
    }
    //------------------------------------------------------------------------
#if (SUPPORT_ACPI_SMI)
    SMI_StepTimer++;
    if (SMI_StepTimer > 40)
    {
        SMI_StepTimer = 0;
        SMI_QueryEvent = SMI_Event_Buffer[SMI_Event_Out_Index];
        if (SMI_QueryEvent > 0x00 && ACPI_STATE_S0)
        {
            if (IS_MASK_SET(SYSTEM_MISC1, ACPI_OS))
            {
                if (!(Is_Flag1(PMC1_STR, SCI_EVENT))) // SCI Service First
                {
                    SET_BIT(PMC1_STR, SMI_EVENT);
                    SMI_Interrupt();
                    return;
                }
            }
            else
            {
                SMI_Interrupt();
            }
        }
    }
#endif
    //------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
BYTE ACPI_SMB_NULL(void)
{
    return 0x00;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 0x02 Write Quick Command
//-----------------------------------------------------------------------------
static BYTE ACPI_SMB_WQuick_CMD(void)
{
    return 0x00;
}
//-----------------------------------------------------------------------------
// 0x03 Read Quick Command
//-----------------------------------------------------------------------------
static BYTE ACPI_SMB_RQuick_CMD(void)
{
    return 0x00;
}
//-----------------------------------------------------------------------------
// 0x04 Send Byte - ACPI Spec.13.9.2.3 Send Byte
//-----------------------------------------------------------------------------
static BYTE ACPI_SMB_Send_BYTE(void)
{
    return 0x00;
}
//-----------------------------------------------------------------------------
// 0x05 Receive Byte
//-----------------------------------------------------------------------------
static BYTE ACPI_SMB_Recive_BYTE(void)
{
    return 0x00;
}
//-----------------------------------------------------------------------------
// 0x06 Write Byte -  ACPI Spec.13.9.2.5 Write Byte
//-----------------------------------------------------------------------------
static BYTE ACPI_SMB_Write_BYTE(void)
{
    return 0x00;
}
//-----------------------------------------------------------------------------
// 0x07 Read Byte
//-----------------------------------------------------------------------------
static BYTE ACPI_SMB_Read_BYTE(void)
{
    return 0x00;
}
//-----------------------------------------------------------------------------
// 0x08 Write Word
//-----------------------------------------------------------------------------
static BYTE ACPI_SMB_Write_WORD(void)
{
    return 0x00;
}
//-----------------------------------------------------------------------------
// 0x09 Read Word
//-----------------------------------------------------------------------------
static BYTE ACPI_SMB_Read_WORD(void)
{
    return 0x00;
}
//-----------------------------------------------------------------------------
// 0x0A Write Block
//-----------------------------------------------------------------------------
static BYTE ACPI_SMB_Write_BLOCK(void)
{
    return 0x00;
}
//-----------------------------------------------------------------------------
// 0x0B Read Block
//-----------------------------------------------------------------------------
static BYTE ACPI_SMB_Read_BLOCK(void)
{
    return 0x00;
}
//-----------------------------------------------------------------------------
// 0x0C Process Call
//-----------------------------------------------------------------------------
static BYTE ACPI_SMB_Process_Call(void)
{
    return 0x00;
}
//-----------------------------------------------------------------------------
// 0x0D Block Write-Block Read Process Call
//-----------------------------------------------------------------------------
static BYTE ACPI_SMB_BLOCK_Call(void)
{
    return 0x00;
}
//-----------------------------------------------------------------------------
// 13.9 SMBus Host Controller In terface via Embedded Controller
// Process OS SMBus command PROTOCOL:
// 0x00_Controller Not In Use   0x01_Reserved
// 0x02_Write Quick Command     0x03_Read Quick Command
// 0x04_Send Byte               0x05_Receive Byte
// 0x06_Write Byte              0x07_Read Byte
// 0x08_Write Word              0x09_Read Word
// 0x0A_Write Block             0x0B_Read Block
// 0x0C_Process Call            0x0D_Block Write-Block Read Process Call
//-----------------------------------------------------------------------------
const FUNCT_PTR_B_V ACPISMBusviaEC_Table[16] =
{
    // ACPI response to writes to SMBus Protocol register. (ACPI_SMB_PROTOCOL)
    ACPI_SMB_NULL,         // 0x00 Controller Not In Use
    ACPI_SMB_NULL,         // 0x01 Reserved
    ACPI_SMB_WQuick_CMD,   // 0x02 Write Quick Command
    ACPI_SMB_RQuick_CMD,   // 0x03 Read Quick Command
    ACPI_SMB_Send_BYTE,    // 0x04 Send Byte
    ACPI_SMB_Recive_BYTE,  // 0x05 Receive Byte
    ACPI_SMB_Write_BYTE,   // 0x06 Write Byte
    ACPI_SMB_Read_BYTE,    // 0x07 Read Byte
    ACPI_SMB_Write_WORD,   // 0x08 Write Word
    ACPI_SMB_Read_WORD,    // 0x09 Read Word
    ACPI_SMB_Write_BLOCK,  // 0x0A Write Block
    ACPI_SMB_Read_BLOCK,   // 0x0B Read Block
    ACPI_SMB_Process_Call, // 0x0C Process Call
    ACPI_SMB_BLOCK_Call,   // 0x0D Write Block-Read Block Process Call
    ACPI_SMB_NULL,         // 0x0E Reserved
    ACPI_SMB_NULL          // 0x0F Reserved
};
const FUNCT_PTR_V_V EC6266Cmd8X_Table[16] =
{
    EC6266_Cmd_80, // Process ACPI command 80
    EC6266_Cmd_81, // Process ACPI command 81
    EC6266_Cmd_82, // Process ACPI command 82
    EC6266_Cmd_83, // Process ACPI command 83
    EC6266_Cmd_84, // Process ACPI command 84
    EC6266_Cmd_85, // Process ACPI command 85
    EC6266_Cmd_86, // Process ACPI command 86
    EC6266_Cmd_87, // Process ACPI command 87
    EC6266_Cmd_88, // Process ACPI command 88
    EC6266_Cmd_89, // Process ACPI command 89
    EC6266_Cmd_8A, // Process ACPI command 8A
    EC6266_Cmd_8B, // Process ACPI command 8B
    EC6266_Cmd_8C, // Process ACPI command 8C
    EC6266_Cmd_8D, // Process ACPI command 8D
    EC6266_Cmd_8E, // Process ACPI command 8E
    EC6266_Cmd_8F  // Process ACPI command 8F
};
void EC_Cmd_8X(void)
{
    (EC6266Cmd8X_Table[PM1Cmd & 0x0F])();
}
const FUNCT_PTR_V_V Port66_Table[16] =
{
    EC_Cmd_0X, // Process ACPI command 0x
    EC_Cmd_1X, // Process ACPI command 1x
    EC_Cmd_2X, // Process ACPI command 2x
    EC_Cmd_3X, // Process ACPI command 3x
    EC_Cmd_4X, // Process ACPI command 4x
    EC_Cmd_5X, // Process ACPI command 5x
    EC_Cmd_6X, // Process ACPI command 6x
    EC_Cmd_7X, // Process ACPI command 7x
    EC_Cmd_8X, // Process ACPI command 8x
    EC_Cmd_9X, // Process ACPI command 9x
    EC_Cmd_AX, // Process ACPI command Ax
    EC_Cmd_BX, // Process ACPI command Bx
    EC_Cmd_CX, // Process ACPI command Cx
    EC_Cmd_DX, // Process ACPI command Dx
    EC_Cmd_EX, // Process ACPI command Ex
    EC_Cmd_FX, // Process ACPI command Fx
};
//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V Port62_Table[16] =
{
    EC62_Data_Step0, // PM1Step=0 Process ACPI EC data
    EC62_Data_Step1, // PM1Step=1 Process ACPI EC data
    EC62_Data_Step2, // PM1Step=2 Process ACPI EC data
    EC62_Data_Step3, // PM1Step=3 Process ACPI EC data
    EC62_Data_Step4, // PM1Step=4 Process ACPI EC data
    EC62_Data_Step5, // PM1Step=5 Process ACPI EC data
    EC62_Data_Step6, // PM1Step=6 Process ACPI EC data
    EC62_Data_Step7  // PM1Step=7 Process ACPI EC data
};
void Service_PCI2_Main(void)
{
    VBYTE cnt = 0;
    do
    {
        if (PMC1_STR & C_D1) // CMD:1=Byte in data register is a command byte
        {
            PM1Cmd = PMC1_DIR; // Load command from Port Buffer
        #if ENABLE_DEBUGGER_SUPPORT
                    /* Debugger Record */
            Debugger_KBC_PMC_Record(0, 1, PM1Cmd);
        #endif
        #if !(LPC_WAY_OPTION_SWITCH)
            PMC1_CTL |= IBF_INT_ENABLE;
        #endif
            PM1Step = 0;
            (Port66_Table[PM1Cmd >> 4])(); // Handle command
        }
        else // CMD:0=Byte in data register is a data byte
        {
            PM1Data = PMC1_DIR; // Load data
        #if ENABLE_DEBUGGER_SUPPORT
                    /* Debugger Record */
            Debugger_KBC_PMC_Record(0, 1, PM1Data);
        #endif
        #if !(LPC_WAY_OPTION_SWITCH)
            PMC1_CTL |= IBF_INT_ENABLE;
        #endif
            if (PM1Step != 0x00)
            {
                (Port62_Table[PM1Step & 0x07])(); // Handle command data
            }
            else
            {
                while (cnt < 0xFF)
                {
                    cnt++;
                }
            }
        }
    }
    while (Is_FLAG_SET(PMC1_STR, BURST) && CheckBurstMode());
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Service_PCI2
 * 62/66 Host Command/Data service
 * ------------------------------------------------------------------------- */
void __weak Service_PCI2(void)
{
#if (Service_PCI2_START == 1)
#if !(PMCKBC_CLOCK_EN)
    dprint("PMCKBC CLOCK NOT ENABLE\n");
    return;
#endif
#if LPC_WAY_OPTION_SWITCH
    if (Is_FLAG_CLEAR(PMC1_STR, IBF1))
        return;
    Service_PCI2_Main();
#else
    if (F_Service_PCI2 == 1)
    {
        F_Service_PCI2 = 0;
        if (Is_FLAG_CLEAR(PMC1_STR, IBF1))
            return;
        Service_PCI2_Main();
    }
#endif
#endif
}
//-----------------------------------------------------------------------------
/* ----------------------------------------------------------------------------
 * FUNCTION: Gen_SCI_Pulse - Generate an SCI interrupt.
 *
 * For a pulsed SCI (edge triggered), the SCI signal is pulsed.
 *
 * For a level triggered SCI, the SCI signal is set or cleared.  The Host will
 * send a command to read the cause of the SCI. The signal will be deactivated
 * when no more causes exist.
 * ------------------------------------------------------------------------- */
void Gen_SCI_Pulse(BYTE Qevent)
{
    if (Qevent == QeventSCI)
    {
        Disable_Interrupt_Main_Switch();
    }
    SCI_Response(1);
    if (Qevent == QeventSCI)
    {
        Enable_Interrupt_Main_Switch();
    }
}
//-------------------------------------------------------------------------------
// parameter "QeventSCI" only, For a pulsed SCI
//------------------------------------------------------------------------------
void OEM_Gen_SCI_Pulse(void)
{
    Gen_SCI_Pulse(QeventSCI);
}
//============================================================================================
// Gen_EC_QEvent -Setup cause flag for an SCI and start the ACPI_Timer.
//
// sci_number : 1 through 255.  The Host uses this number to determine the cause of the SCI.
// sci_mode   : Normal or pending mode.
//
//============================================================================================
VBYTE ITempB55;
void Gen_EC_QEvent(BYTE sci_number, BYTE sci_mode)
{
    UNUSED_VAR(sci_mode);
    // if in non-acpi mode, pull sci pin will wake up system in S3 state
    if (IS_MASK_CLEAR(SYSTEM_MISC1, ACPI_OS))
    {
        return;
    }
    dprint("\nvoid Gen_EC_QEvent(BYTE sci_number, BYTE sci_mode)%#x\n", sci_number);
#if SUPPORT_QEvent_Pending
    if ((sci_mode == SCIMode_Normal) || (sci_mode == SCIMode_Pending))
    {
        if (sci_mode == SCIMode_Normal)
        {
            if (PD_SCI_Event_In_Index != PD_SCI_Event_Out_Index)
            {
                EC_QEvent_Suspend(sci_number);
                return;
            }
        }
        if (Is_BitDef_Set(PMC1_STR, IBF1) || Is_BitDef_Set(KBC_STA, KBC_STA_IBF))
        {
            if (sci_mode == SCIMode_Pending)
            {
                if (PD_SCI_Event_Out_Index == 0x00)
                {
                    PD_SCI_Event_Out_Index = 7;
                }
                else
                {
                    PD_SCI_Event_Out_Index--;
                }
            }
            else
            {
                EC_QEvent_Suspend(sci_number);
            }
            return;
        }
    }
#endif
    ITempB55 = SCI_Event_In_Index;           // Get the input index.
    SCI_Event_Buffer[ITempB55] = sci_number; // Put the SCI number in the buffer.
    ITempB55++;                              // Increment the index.
    if (ITempB55 > 9)
    {
        ITempB55 = 0;
    }
    // If the buffer is not full, update the input index.
    if (ITempB55 != SCI_Event_Out_Index)
    {
        SCI_Event_In_Index = ITempB55;
    }
    SET_MASK(PMC1_STR, SCIEVT);
    OEM_Gen_SCI_Pulse();
}
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void EC_QEvent_Suspend(BYTE sci_number)
{
    UNUSED_VAR(sci_number);
#if SUPPORT_QEvent_Pending
    BYTE index;
    index = PD_SCI_Event_In_Index;           // Get the input index.
    PD_SCI_Event_Buffer[index] = sci_number; // Put the SCI number in the buffer.
    index++;                                 // Increment the index.
    if (index >= 8)
    {
        index = 0;
    }
    // If the buffer is not full, update the input index.
    if (index != PD_SCI_Event_Out_Index)
    {
        PD_SCI_Event_In_Index = index;
    }
#endif
}
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Release_EC_QEvent_Suspend(void)
{
#if SUPPORT_QEvent_Pending
    if (Is_BitDef_Set(SYSTEM_MISC1, ACPI_OS))
    {
        if (PD_SCI_Event_Out_Index != PD_SCI_Event_In_Index)
        {
            sci_number = PD_SCI_Event_Buffer[PD_SCI_Event_Out_Index];
            PD_SCI_Event_Out_Index++;
            if (PD_SCI_Event_Out_Index >= 8)
            {
                PD_SCI_Event_Out_Index = 0;
            }
            Gen_EC_QEvent(sci_number, SCIMode_Pending);
        }
    }
#endif
}

//----------------------------------------------------------------------------
// 该函数为轮询方式发送SCI信号，请在Hook_1msEvent函数中调用
//----------------------------------------------------------------------------
void SCI_Send(void)
{
    if (SCI_Response_Flag > 0)
    {
        SCI_Low();
        if (SCI_Count == 0)
        {
            SCI_High();
            SCI_Response_Flag--;
            SCI_Count = 15;
        }
        SCI_Count--;
    }
}