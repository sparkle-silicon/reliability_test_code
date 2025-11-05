/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-01-11 17:33:24
 * @Description: This file is used for handling host interface
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
#include "KERNEL_HOST.H"
#include "AE_DEBUGGER.H"
 //****************************************************************************
 //static function  declaration
 //****************************************************************************
static BYTE Send_To_Host(BYTE data_word, BYTE break_prefix_flag);
static BYTE Common_Send_To_HOST(BYTE data_word, BYTE break_prefix_flag);
static BYTE Switch_Scan_Code(BYTE data_word, BYTE break_prefix_flag);
//****************************************************************************
//static function  declaration
//****************************************************************************
/* ----------------------------------------------------------------------------
 * FUNCTION: Switch_Scan_Code
 *
 * Translate KBD_SCAN code from "set 2 KBD_SCAN code" to "set 1 KBD_SCAN code".
 *
 * Input: data - KBD_SCAN code received from aux keyboard or local keyboard.
 *        break_prefix_flag - TRUE if last KBD_SCAN code was a break prefix.
 *
 * Return: a value with all bits set if data is break prefix (0xF0);
 *         otherwise, returns translated key.
 * ------------------------------------------------------------------------- */
static BYTE Switch_Scan_Code(BYTE data_word, BYTE break_prefix_flag)
{
    /* Scan code set 2 to KBD_SCAN code set 1 translation table.  First byte is a
       dummy entry because KBD_SCAN code "0" is not translated. */
    static const BYTE scan2_table[] =
    {
        0x00, 0x43, 0x41, 0x3F, 0x3D, 0x3B, 0x3C, 0x58,
        0x64, 0x44, 0x42, 0x40, 0x3E, 0x0F, 0x29, 0x59,
        0x65, 0x38, 0x2A, 0x70, 0x1D, 0x10, 0x02, 0x5A,
        0x66, 0x71, 0x2C, 0x1F, 0x1E, 0x11, 0x03, 0x5B,
        0x67, 0x2E, 0x2D, 0x20, 0x12, 0x05, 0x04, 0x5C,
        0x68, 0x39, 0x2F, 0x21, 0x14, 0x13, 0x06, 0x5D,
        0x69, 0x31, 0x30, 0x23, 0x22, 0x15, 0x07, 0x5E,
        0x6A, 0x72, 0x32, 0x24, 0x16, 0x08, 0x09, 0x5F,
        0x6B, 0x33, 0x25, 0x17, 0x18, 0x0B, 0x0A, 0x60,
        0x6C, 0x34, 0x35, 0x26, 0x27, 0x19, 0x0C, 0x61,
        0x6D, 0x73, 0x28, 0x74, 0x1A, 0x0D, 0x62, 0x6E,
        0x3A, 0x36, 0x1C, 0x1B, 0x75, 0x2B, 0x63, 0x76,
        0x55, 0x56, 0x77, 0x78, 0x79, 0x7A, 0x0E, 0x7B,
        0x7C, 0x4F, 0x7D, 0x4B, 0x47, 0x7E, 0x7F, 0x6F,
        0x52, 0x53, 0x50, 0x4C, 0x4D, 0x48, 0x01, 0x45,
        0x57, 0x4E, 0x51, 0x4A, 0x37, 0x49, 0x46, 0x54 };
    BYTE check_break_bit = FALSE;
    if (data_word == 0xF0)
    { /* Signify that break code prefix was encountered. */
        data_word = 0xFF;
    }
    else if (data_word == 0x00)
    {
        data_word = 0x00; /* Key detection error/overrun. */
    }
    else if ((data_word & 0x80) == 0)
    { /* Translate codes 01 thru 7F. */
        /* The variable "data" has KBD_SCAN code (set 2) to translate.
           Set "data" to the translated (to set 1) KBD_SCAN code. */
        data_word = scan2_table[data_word];
        // dprint("scan2_1 data_word is %#x \n",data_word);
        check_break_bit = TRUE;
    }
    else if (data_word == 0x83) /* ID code for 101/102 keys. */
    {
        data_word = 0x41; /* Translate ID code. */
        check_break_bit = TRUE;
    }
    else if (data_word == 0x84) /* ID code for 84 keys. */
    {
        data_word = 0x54; /* Translate ID code. */
        check_break_bit = TRUE;
    }
    if (check_break_bit && break_prefix_flag)
    { /* Last code received by this routine was the break prefix.This must be
         a break code. Set high bit to indicate that this is a break code. */
        data_word |= 0x80;
    }
    return (data_word);
}
static BYTE Common_Send_To_HOST(BYTE data_word, BYTE break_prefix_flag)
{
#if KBS_DEBUG
    dprint("Common_Send_To_HOST data is %#x\n", data_word);
#endif
    BYTE send_it = FALSE;
    if (Host_Flag_XLATE_PC == 0) // Send data as is.
    {
#if KBS_DEBUG
        dprint("Host_Flag_XLATE_PC is 0\n");
#endif
        send_it = TRUE;
        break_prefix_flag = FALSE;
    }
    else // Translation mode is enabled.
    {
        data_word = Switch_Scan_Code(data_word, break_prefix_flag);
        if (data_word == 0xFF)
        {
            break_prefix_flag = TRUE; // Don't send break code prefix.
        }
        else if (data_word == 0x00)
        {
            break_prefix_flag = TRUE; // Don't send break code prefix.
        }
        else
        {
            break_prefix_flag = FALSE;
            send_it = TRUE;
        }
    }
#if KBS_DEBUG
    if (send_it == FALSE)
        dprint("send_it FALSE\n");
    if (break_prefix_flag == FALSE)
        dprint("break_prefix_flag FALSE\n");
#endif
    if (send_it)
    {
        Transmit_Data_To_Host(data_word);
    }
    return (break_prefix_flag);
}
/* ----------------------------------------------------------------------------
 * FUNCTION: send_ext_to_pc, Send_To_Host
 *
 * send_ext_to_pc is just a 'pre-entry' to the original Send_To_Host
 * routine.  It's sole purpose is to check auxiliary keyboard entries
 * for external hotkey functions.  This allows it to set flags for
 * CTRL and ALT states, then if BOTH are set, hotkeys can be checked.
 *
 * Send data to Host.  If password is enabled, the data
 * will not be sent until the password is entered correctly.
 *
 * Input: data - Data to send to PC.
 *        break_prefix_flag - TRUE if last KBD_SCAN code was a break prefix.
 *
 * Returns: TRUE if translation mode is enabled and
 *          KBD_SCAN code was a break prefix.
 * ----------------------------------------------------------------------------*/
static BYTE Send_To_Host(BYTE data_word, BYTE break_prefix_flag)
{
    return (Common_Send_To_HOST(data_word, break_prefix_flag));
}
/* ----------------------------------------------------------------------------
 * FUNCTION: KB_DataToHost
 *
 * Clear error bits in the Host Interface status port and sends a command
 * response byte or a byte of keyboard data to the Host.  Generate Host IRQ1
 * if keyboard interrupts are enabled in controller command byte.
 *
 * Input:  data to send to Host.
 * ------------------------------------------------------------------------- */
BYTE Send_KB_Data_To_Host(BYTE nKBData)
{
    if (IS_SET(KBC_STA, KBC_OBF) || IS_SET(KBC_STA, KBC_IBF))
    {
        KBC_Data_Suspend(nKBData);
        return 0x0;
    }
    else
    {
        Transmit_Data_To_Host(nKBData); // Send data to host.
        return 0x1;
    }
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Service_Send
 *
 * Send data from the scanner keyboard or from multibyte command responses to
 * the Host.
 *
 * Send KBD_SCAN codes from scanner keyboard to the Host.  Also handle multiple
 * byte transmissions for standard commands and extended commands that return
 * more than one byte to the Host.
 *
 * When sending multiple bytes, the 1st byte is sent immediately, but the
 * remaining bytes are sent by generating another send request via the
 * function "handle_unlock" which will call "Start_Scan_Transmission".
 * If more bytes are to be sent, "Start_Scan_Transmission" will start Timer A,
 * and the Timer A interrupt handler will generate the send request when the
 * response timer has expired!
 * ------------------------------------------------------------------------- */
void Service_Send(void)
{
#if (Service_Send_START == 1)
    //-------------------------------------------------------------------------
    BYTE nKBData;
    BYTE bBreak;
    BYTE KBCmdAck;
    // if(F_Service_Send == 0)return ;
#if ENABLE_DEBUGGER_SUPPORT
#else
    if (Is_FLAG_SET(KBC_STA, KBC_STA_OBF) || Is_FLAG_SET(KBC_STA, KBC_STA_IBF))
    {
        return;
    }
#endif
    if (KBPendingRXCount > KBPendingTXCount)
    {
        KBCmdAck = Release_KBC_Data_Suspend();
#if KBS_DEBUG
        dprint("Service Send. pending data is %#x\n", KBCmdAck);
#endif
        Send_KB_Data_To_Host(KBCmdAck);
        return;
    }
    /* ------------------------------------------------------------------------
     * Keyboard Buffer Data Send to Host(System)
     * ---------------------------------------------------------------------- */
    if (Host_Flag_DISAB_KEY)
        return;
    nKBData = KBS_Buffer_Get(&nKBData);
    // FORCE_NO_XLATE = 0;
    if (nKBData == 0xFF) // 0xFF: No key data in buffer
    {
        return;
    }
    else
    {
        bBreak = Gen_Info_BREAK_SCAN;
        Gen_Info_BREAK_SCAN = 0;
        if (Send_To_Host(nKBData, bBreak))
        {
            Gen_Info_BREAK_SCAN = 1; // Break prefix code.
        }
#if KBS_DEBUG
        dprint("Service Send data is %#x\n", nKBData);
#endif
    }
    //-------------------------------------------------------------------------
#endif
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Transmit_Data_To_Host
 *
 * Clear error bits in the Host Interface status port and sends a command
 * response byte or a byte of keyboard data to the Host.  Generate Host IRQ1
 * if keyboard interrupts are enabled in controller command byte.
 *
 * Input:  data to send to Host.
 * ------------------------------------------------------------------------- */
void Transmit_Data_To_Host(BYTE data_byte)
{
    irqc_disable_interrupt(IRQC_INT_DEVICE_PS2_0);
    ICTL1_INTMASK5 |= 0x40;//disable ps2 irq
    //KBC_STA &= 0x0f;
    SET_BIT(KBC_STA, KBC_KL);
    CLEAR_BIT(KBC_STA, KBC_SAOBF);
    if (Host_Flag_INTR_KEY)
    {
        SET_BIT(KBC_CTL, KBC_OBFKIE);
    }
    else
    {
        CLEAR_BIT(KBC_CTL, KBC_OBFKIE);
    }
#if KBC_DEBUG
    dprint("dth:%x,icr:%x,isr:%x\n", data_byte, KBC_CTL, KBC_STA);
#endif
#if SUPPORT_8042DEBUG_OUTPUT
    Write_Debug_Data_To_Sram(data_byte);
#endif
    KBC_KOB = data_byte;
#if ENABLE_DEBUGGER_SUPPORT
    /* Debugger record */
    dprint("Transmit_Data_To_Host\n");
    Debugger_KBC_PMC_Record(1, 0, data_byte);
    Debugger_KBD_Record(data_byte);
#endif
    irqc_enable_interrupt(IRQC_INT_DEVICE_PS2_0);
    ICTL1_INTMASK5 &= ~0x40;//enable ps2 irq
}
void KBC_Data_Suspend(BYTE nPending)
{
#if KBC_DEBUG
    dprint("KBPendingRXCount is %#x \n", KBPendingRXCount);
#endif
    if (KBPendingRXCount > 3)
        return;
    KBDataPending[(KBPendingRXCount & 0x03)] = nPending;
    KBPendingRXCount++;
}
BYTE Release_KBC_Data_Suspend(void)
{
    BYTE buffer_data;
    buffer_data = KBDataPending[(KBPendingTXCount & 0x03)];
    KBPendingTXCount++;
    if (KBPendingTXCount >= KBPendingRXCount)
    {
        KBPendingTXCount = 0;
        KBPendingRXCount = 0;
    }
#if KBC_DEBUG
    dprint("Release_KBC_Data_Suspend is %#x \n", buffer_data);
#endif
    return buffer_data;
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Mouse_Data_To_Host - Send auxiliary device (mouse) data to the Host.
 *
 * Clear error bits in the Host Interface status port and sends a byte of
 * aux device (mouse) data to the Host.  Generates Host IRQ12 if aux device
 * (mouse) interrupts are enabled in controller command byte.
 *
 * Input: data to send to Host.
 * ------------------------------------------------------------------------- */
void Mouse_Data_To_Host(BYTE data_byte)
{
    KBC_STA &= 0x0F;
    SET_BIT(KBC_STA, KBC_SAOBF);
    if (Host_Flag_INTR_AUX)
    {
        SET_BIT(KBC_CTL, KBC_OBFMIE);
    }
    else
    {
        CLEAR_BIT(KBC_CTL, KBC_OBFMIE);
    }
#if SUPPORT_8042DEBUG_OUTPUT
    Write_Debug_Data_To_Sram(data_byte);
#endif
#if KBC_DEBUG
    dprint("adth1:%x,icr:%x,isr:%x\n", data_byte, KBC_CTL, KBC_STA);
#endif
    KBC_MOB = data_byte;
#if ENABLE_DEBUGGER_SUPPORT
    Debugger_KBC_PMC_Record(1, 0, data_byte);
#endif
}

void kbc_init(void)
{
    SET_BIT(KBC_CTL, KBC_IBFIE); // set KBC_IBF enable
}
void pmc1_init(void)
{
    PMC1_IE = 0x3f;
#if !(LPC_WAY_OPTION_SWITCH)
    PMC1_CTL |= IBF_INT_ENABLE;
#endif
}
void pmc2_init(void)
{
    PMC2_IE = 0x3f;
#if !(LPC_WAY_OPTION_SWITCH)
    PMC2_CTL |= IBF_INT_ENABLE;
#endif
}
void pmc3_init(void)
{
    PMC3_IE = 0x3f;
#if !(LPC_WAY_OPTION_SWITCH)
    PMC3_CTL |= IBF_INT_ENABLE;
#endif
}
void pmc4_init(void)
{
    PMC4_IE = 0x3f;
#if !(LPC_WAY_OPTION_SWITCH)
    PMC4_CTL |= IBF_INT_ENABLE;
#endif
}
void pmc5_init(void)
{
    PMC5_IE = 0x3f;
#if !(LPC_WAY_OPTION_SWITCH)
    PMC5_CTL |= IBF_INT_ENABLE;
#endif
}
void pmc_init(void)
{
    pmc1_init();
    pmc2_init();
    pmc3_init();
    pmc4_init();
    pmc5_init();
}