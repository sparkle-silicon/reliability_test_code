/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-01-17 15:30:46
 * @Description: This file is used for handling internal keyboard scanning
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
#include <KERNEL_KBS.H>
VBYTE KBD_BUFF[KBF_SIZE] = { 0 };
VBYTE KBS_matrix[MAX_SCAN_LINES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const BYTE KBS_repeat_tbl[] =
{
    3, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 9, 10, 11, 11, 12,
    13, 15, 16, 18, 20, 21, 23, 25, 27, 30, 33, 37, 40, 43, 47, 50 };
const BYTE KBS_delay_tbl[] = { 25, 50, 75, 100 };
/* ----------------------------------------------------------------------------
 * FUNCTION:   KBS_Buffer_Get
 *
 * Get data byte from keyboard buffer (if not empty) and update "KBD_SCAN.head"
 * with new index into keyboard buffer.
 *
 * Return: Data from buffer (VWORD of all 1's if buffer is empty).
 * ------------------------------------------------------------------------- */
BYTE KBS_Buffer_Get(BYTEP buffp)
{
    if(KBD_SCAN.head != KBD_SCAN.tail)
    {
        *buffp = KBD_BUFF[KBD_SCAN.head];
        if(KBD_SCAN.head == (KBF_SIZE - 1))
            KBD_SCAN.head = 0;
        else
            KBD_SCAN.head++;
    }
    else
    { /* Buffer is empty. */
        *buffp = 0xFF;
    }
    return (*buffp);
}
/* ----------------------------------------------------------------------------
 * FUNCTION:   KBS_Buffer_Input
 *
 * Input:  Row/Column (0iii,oooo) to put in buffer.
 * Return: TRUE operation successful, FALSE unsuccessful.
 * ------------------------------------------------------------------------- */
BYTE KBS_Buffer_Input(BYTE code)
{
    if(code == 0)
    {
        return FALSE;
    }
    KBD_BUFF[KBD_SCAN.tail] = code; /* Store Data to Buffer Tail */
    if(KBD_SCAN.tail == (KBF_SIZE - 1))
        KBD_SCAN.tail = 0; /* Wrap pointer if too large. */
    else
        KBD_SCAN.tail++;
    /* Check Overflow */
    if(KBD_SCAN.tail == KBD_SCAN.head)
    {                                  /* Overflow */
        KBD_SCAN.tail = KBD_SCAN.mark; /* Restore buffer marker. */
        KBD_BUFF[KBD_SCAN.tail] = 0;   /* Set OverFlow Mark. */
        return FALSE;                 /* Overflow Indication !!! */
    }
#if KBS_DEBUG
    dprint("code3 = %#x\n", code);
#endif
    return TRUE;
}
/* ----------------------------------------------------------------------------
 * FUNCTION:   KBS_Buffer_puts
 *
 * Places a string in the local keyboard buffer (without a terminating NULL).
 * Call KBS_Buffer_Input to place data in key buffer.
 *
 * Input:  Pointer to null terminated string.
 * Return: 0x80 if buffer overflow error, else 0.
 * ------------------------------------------------------------------------- */
BYTE KBS_Buffer_puts(const BYTEP str)
{
    while((*str != 0))
        if(KBS_Buffer_Input(*str++) == FALSE)
            return FALSE;  /* Indicate Overflow */
    return TRUE;
}
/* ----------------------------------------------------------------------------
 * FUNCTION: KBS_Find_Paths
 *
 * Input:  DOWN_STATE  scan_addr
 *         change_make_key
 *
 * Return: FALSE if no paths found, Otherwise returns TRUE.
 * ------------------------------------------------------------------------- */
static BYTE KBS_Find_Paths(BYTE key, BYTE base_addr)
{
    BYTE paths, temp, bits;
    BYTE scan_addr = base_addr;
    if(key == 0)
        return (FALSE);
    paths = KBS_matrix[scan_addr] | key;
    if(paths == 0)
        return (FALSE);
    while(1)
    {
        scan_addr++;
        if(scan_addr >= MAX_SCAN_LINES)
            scan_addr = 0; /* Wrap around */
        if(scan_addr == base_addr)
            return FALSE;
        temp = KBS_matrix[scan_addr]; /* Any paths? */
        if(temp != 0)
        {                  /* Paths found */
            temp &= paths; /* Do paths line up? */
        }
        if(temp != 0)
        {                    /* Paths line up */
            if(key != temp) /* Only 1 bit set? */
            {                /* No, ghost exists. */
                return TRUE;
            }
            bits = paths; /* Only 1 bit set? */
            while(!(bits & 0x01))
            {
                /* Shift bit out (and a zero bit in) to check next bit. */
                bits >>= 1;
            }
            bits >>= 1;
            if(bits != 0)
            { /* No, more than 1 bit set. */
                return TRUE;
            }
        } /* if (temp != 0) */
    }     /* while (1) */
}
/* ----------------------------------------------------------------------------
 * FUNCTION: KBS_Run - Service the local keyboard in response to a key press.
 * ------------------------------------------------------------------------- */
void KBS_Run(void)
{
    BYTE kso_index, ksi_index, ksitemp = 0x0;
    VBYTEP ksolsd = &KBS_KSO00LSD;
    // Pre-clear variables
    KBD_SCAN.keys = 0;
    // Scan KSO0 ~ 15, 16, or 17
    for(kso_index = 0x00; kso_index < (KBD_8_n_SWITCH); kso_index++)
    {
        ksitemp = ~(*ksolsd); // Read KSI and inverse
        ksolsd = ksolsd + 1;
        ksitemp ^= KBS_matrix[kso_index];
        if(ksitemp != 0x00)
        {
            //-----------------------------------------------------
            // Check KSI0 ~ 7
            //-----------------------------------------------------
            for(ksi_index = 0x00; ksi_index < 8; ksi_index++)
            {
                if((ksitemp & BIT(ksi_index)))
                {
                    KBS_INFO.index.kso = kso_index;                // Save KSO for type uKEY
                    KBS_INFO.index.ksi = ksi_index;                // Save KSI for type uKEY
                    if(!(KBS_matrix[kso_index] & BIT(ksi_index))) // Make
                    {
                        if(KBS_Find_Paths(ksitemp, kso_index) == FALSE) // No ghost key
                        {
                            KBS_Xlate_Code2(KBS_INFO, MAKE_EVENT); // Key make
                            KBD_SCAN.scale = TM_SCALE;             // Repeat time
                            KBS_COUNT = KBS_DELAY;                 // Delay time
                            KBS_matrix[kso_index] |= BIT(ksi_index);
                        }
                        else // ghost key found
                        {
                            KBS_INFO.byte = 0;
                        #if KBS_DEBUG
                            dprint("ghost key found \n");
                        #endif
                            return; /*Fix ghost*/
                        }
                    }
                    else // Break
                    {
                        KBS_Xlate_Code2(KBS_INFO, BREAK_EVENT); // Key break;
                        KBS_matrix[kso_index] &= (~BIT(ksi_index));
                    }
                }
            }
        }
        // Here, if current still active.
        if(KBS_matrix[kso_index])
            KBD_SCAN.keys = 1; // Set keys active bits. Check all key release.
    }
    //-----------------------------------------------------
    // All key break
    //-----------------------------------------------------
    if((KBD_SCAN.keys) == 0x00) // all key break
    {
        KBS_Clear_Break();
    }
    //-----------------------------------------------------
    // Start sending keyboard data
    //-----------------------------------------------------
    if(KBD_SCAN.head != KBD_SCAN.tail)
    {
        // F_Service_Send = 1;         // Post service request
    }
}
/* ----------------------------------------------------------------------------
 * FUNCTION: KBS_Set_KBS_INFO - Set the KBS_INFO rate.
 *
 * Input: KBS_INFO rate
 *        Bit[4:0] KBS_INFO repeat interval
 *        Bit[6:5] Delay time
 * ------------------------------------------------------------------------- */
void KBS_Set_KBS_INFO(VBYTE type_rate)
{
    /* Bit 4 - 0 KBS_INFO repeat interval index. */
    KBS_REPEAT = KBS_repeat_tbl[type_rate & 0x1F];
    /* Bit 6 - 5 delay time index. */
    KBS_DELAY = KBS_delay_tbl[(type_rate >> 5) & 0x03];
}
/* ----------------------------------------------------------------------------
 * FUNCTION: KBD_CLear_Buffer - Clear local keyboard buffer and related variables.
 * ------------------------------------------------------------------------- */
void KBD_CLear_Buffer(void)
{
    memset((void *)KBS_matrix, 0, MAX_SCAN_LINES);
    memset((void *)KBD_BUFF, 0, KBF_SIZE);
    KBD_SCAN.head = 0; // Clear keyboard buffer head
    KBD_SCAN.tail = 0; // Clear keyboard buffer tail
    KBD_SCAN.keys = 0; // Clear Scan activity flag - keys active.
    KBS_Clear_Break();
}
/* ----------------------------------------------------------------------------
 * FUNCTION: KBD_Default_KBS_INFO - Set default KBS_INFO delay and clear type-matic action.
 * ------------------------------------------------------------------------- */
void KBD_Default_KBS_INFO(void)
{
    KBS_INFO.byte = 0;
    KBS_DELAY = TM_DELAY;
    KBS_REPEAT = TM_REPEAT;
}
/* ----------------------------------------------------------------------------
 * FUNCTION: KBS_Init - Scanner Keyboard Initialization.
 * ------------------------------------------------------------------------- */
void KBS_Init(void)
{
#if (KBD_8_n_SWITCH == 18)
    KBS_KSDC2R = KBS_KSDC2R_SSEL_18 | KBS_KSDC2R_DELAY57;
#elif (KBD_8_n_SWITCH == 17)
    KBS_KSDC2R = KBS_KSDC2R_SSEL_17 | KBS_KSDC2R_DELAY57;
#elif (KBD_8_n_SWITCH == 16)
    KBS_KSDC2R = KBS_KSDC2R_SSEL_16 | KBS_KSDC2R_DELAY57;
#endif
    KBS_KSOCTRL = KBS_KSOCTRL_ODEN;                                     // open drain en
    KBS_KSDC3R = KBS_KSDC3R_DELAY29 | KBS_KSDC3R_DELAY0;                 // Wait KSO low delay 29us, spacing delay between rounds 0ms.
    KBS_KSDC1R = KBS_KSDC1R_EN | KBS_KSDC1R_INT_EN | KBS_KSDC1R_ROUND_3; // round 3,int and scan en
    KBD_CLear_Buffer();                                                  // Clear key matrix/buffer
    KBD_Default_KBS_INFO();                                             // Clear KBS_INFO
}
//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
void KBS_Clear_Break(void)
{
    memset((void *)Fn_BUNDLED_FLAG, 0, 16);
    Scanner_State = 0;//清空按下状态
    NumLockKey = 1;//如果是锁的话NumLock默认开启
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Handle_Scan
 * Service the local keyboard in response to a key press.
 * ------------------------------------------------------------------------- */
void Service_KBS(void)
{
    if(F_Service_KBS == 1)
    {
        F_Service_KBS = 0;
    #if !(KBS_CLOCK_EN)
        dprint("KBS CLOCK NOT ENABLE\n");
        return;
    #endif
        if(((KB_Scan_Flag == 0) || ((Host_Flag & PS2_PORT1_KEY_EN) != 0)))
        {
        }
        else
        {
            KBS_Run();
        }
    }
}
//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
void KBD_Check_Repeat(void)
{
    BYTE ksi_index = KBS_INFO.index.ksi;
    if(KBS_INFO.byte)
    {
        if((BIT(ksi_index) & KBS_matrix[KBS_INFO.index.kso]) == 0)//  KBS NO BREAK? 
        {
            KBS_INFO.byte = 0;	    // Clear KBS_INFO. 
            return;
        }
        KBD_SCAN.scale--;		    // Count down Prescale. 10ms
        if(KBD_SCAN.scale != 0)
        {
            return;
        }
        KBD_SCAN.scale = TM_SCALE;// Reload prescale counter. 
        KBS_COUNT--;             // Count down TMcount.//0cnt delay 600 ms,last count 30ms
        if(KBS_COUNT != 0)
        {
            return;
        }
        KBS_COUNT = KBS_REPEAT;   // 30ms 1count
        KBS_Xlate_Code2(KBS_INFO, REPEAT_EVENT);
    }
}