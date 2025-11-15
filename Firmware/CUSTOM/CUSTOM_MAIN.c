/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-20 18:47:03
 * @Description: Custom function support
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
#include "CUSTOM_MAIN.H"
#include "CUSTOM_EVENT.H"
#include "CUSTOM_TEMPERATURE.H"
#include "CUSTOM_BATTERY.H"
#include "CUSTOM_POWER.H"
#include "CUSTOM_PMC1.H"
#include "CUSTOM_LED.H"
#include "CUSTOM_FAN.H"
#include "CUSTOM_ANX7447_PORT.H"
#include "AE_UPDATE.H"
#include "KERNEL_SLEEP_RESET.H"
#include "KERNEL_INCLUDE.H"
#include "KERNEL_I3C.H"
#include "CUSTOM_PECI.H"

u32 RunTimeStamp = 0;

//-----------------------------------------------------------------------------
// Oem 1ms Events/Hook Here
//-----------------------------------------------------------------------------
void __weak Hook_1msEvent(BYTE EventId)
{
#if SUPPORT_ANX7447
    ANX_HOOK_1ms();
#endif
    Event_Center(EventId);    // Polling system event
    Sys_PowerState_Control(); // System Power Control

#if SCI_POLLING_CONTROL
    SCI_Send();
#endif
}
//-----------------------------------------------------------------------------
// Oem 5ms Events/Hook Here
//-----------------------------------------------------------------------------
void __weak Hook_5msEvent(void)
{
#if SUPPORT_QEvent_Pending
    Get_PendingECQEvent();
#endif
}
//-----------------------------------------------------------------------------
// Oem 10ms Events/Hook Here
//-----------------------------------------------------------------------------
void __weak Hook_10msEventA(void)
{
#if SUPPORT_BREATHING_LED
    Breathing_LED_Switch(0, 1);
#endif
}
void __weak Hook_10msEventB(void)
{
#if SUPPORT_ANX7447
    ANX_HOOK_10ms();
#endif
#if SUPPORT_BATTERY_CHARGE
    BatChgControlCenter();
#endif
#if SUPPROT_PS2DEV_SCAN
    InitAndIdentifyPS2();
#endif

    //set oob execute delay timer
    if (xOOB_PauseTimer > 0)
    {
        xOOB_PauseTimer--;
        if (xOOB_PauseTimer == 0)
        {
            xOOB_Scan = 0;
            xOOB_GetPCH_Temper = 5;
            xOOB_GetPCH_RTC_Timer = 120;
            xOOB_PeciGetCpuT_Timer = 2;
        }
    }
}
//-----------------------------------------------------------------------------
// Oem 50ms Events/Hook Here
//-----------------------------------------------------------------------------
void __weak Hook_50msEventA(void)
{
    //子系统dram更新测试用例
#if SUPPORT_UPDATE_FUNCTION
    if (update_crypram_flag == 1)
    {
        Cryp_Update_Function();
        update_crypram_flag = 0;
    }
    if (update_intflash_flag == 1)
    {
        ExtFlash_Update_Function();
        update_intflash_flag = 0;
    }
#endif
}
//-----------------------------------------------------------------------------
void __weak Hook_50msEventB(void)
{
}
//-----------------------------------------------------------------------------
void __weak Hook_50msEventC(void)
{
}
//-----------------------------------------------------------------------------
// Oem 100ms Events/Hook Here
//-----------------------------------------------------------------------------
void __weak Hook_100msEventA(void)
{
}
//-----------------------------------------------------------------------------
void __weak Hook_100msEventB(void)
{
}
//-----------------------------------------------------------------------------
void __weak Hook_100msEventC(void)
{
    //led light control
    Service_LED_Indicator();

    //service oob call
    if (xOOB_GET_CRASHLOG > 0)
    {
        return;
    }
    if (xOOB_PauseTimer == 0)
    {
        Service_OOB_Message();
    }

#if SUPPORT_PECI
    if (System_PowerState == SYSTEM_S0)
    {
        Service_PECI();
#if 0
        Service_PECI_Command();
        while (PECI_FLAG & F_PECI_BUSY)
        {
            Service_PECI_Data();
        }
#endif
    }
    else
    {
        PECI_FLAG = 0;
    }
#endif  //SUPPORT_PECI

}
//-----------------------------------------------------------------------------
// Oem 500ms Events/Hook Here
//-----------------------------------------------------------------------------
void __weak Hook_500msEventA(void)
{
}
//-----------------------------------------------------------------------------
void __weak Hook_500msEventB(void)
{
}
//-----------------------------------------------------------------------------
void __weak Hook_500msEventC(void)
{
#if SUPPORT_FIRMWARE_UPDATE
    Flash_Update_Function();
#endif
}
//-----------------------------------------------------------------------------
// Oem 1sec Events/Hook Here
//-----------------------------------------------------------------------------
void __weak Hook_1secEventA(void) // get all temp
{
#if (I2C_MODULE_EN && SUPPORT_I2C_TEMPERATURE)
    get_temperature(2);
#endif

#if 0       //erpmc test
    if (*((VBYTE*)(0x203B9)) == 1)
    {
        *((VBYTE*)(0x203B9)) = 0;
        // Mailbox_Test();
        // Mailbox_FW_Extension_Trigger();
        // Mailbox_APB2_Source_Alloc_Trigger();
        // Mailbox_Update_Function(0x3, 0x8000, 0x70800); // 发起mailbox更新
        // Mailbox_Read_FLASHUID_Trigger();
        // Mailbox_Read_EFUSE_Trigger();
        Mailbox_WriteRootKey_Trigger();
    }
    if (*((VBYTE*)(0x203B9)) == 2)
    {
        *((VBYTE*)(0x203B9)) = 0;
        Mailbox_UpdateHMACKey_Trigger();
    }
    if (*((VBYTE*)(0x203B9)) == 3)
    {
        *((VBYTE*)(0x203B9)) = 0;
        //Mailbox_IncrementCounter_Trigger(0x3FFE);
    }
    if (*((VBYTE*)(0x203B9)) == 4)
    {
        *((VBYTE*)(0x203B9)) = 0;
        Mailbox_RequestCounter_Trigger();
    }
    if (*((VBYTE*)(0x203B9)) == 5)
    {
        *((VBYTE*)(0x203B9)) = 0;
        Mailbox_ReadParameter_Trigger();
    }
#endif
}
//-----------------------------------------------------------------------------
void __weak Hook_1secEventB(void) // get fan rpm
{
#if SUPPORT_FAN1 || SUPPORT_FAN2
    DWORD Polling0 = TACH_Get_Polling(0);
    if (Polling0 != -1)
    {
        TACH0_Speed = (DWORD)TACH_RPM(Polling0);
        dprint("FAN2 RPM is %u\n", TACH0_Speed);
    }
#endif
}
//-----------------------------------------------------------------------------
void __weak Hook_1secEventC(void) // update new rpm
{
    RunTimeStamp++;

#if FAN_Dynamic_Adj
    FAN_Dynamic(3);
#endif
#if FAN_LEV_Adj
    FAN_LEV(3);
#endif
#if FAN_PID_Adj
    FAN_PID(3);
#endif
}
//-----------------------------------------------------------------------------
// 1 min events
//-----------------------------------------------------------------------------
void __weak Hook_1minEvent(void)
{
#if SUPPORT_8042DEBUG_OUTPUT
    MinuteCnt++;
    if (MinuteCnt >= 2)
    {
        MinuteCnt = 0;
        for (int i = 0; i <= Debug_Num; i++)
        {
            dprint("Cnt:%d,Data:%x\n", i, Debug_8042[Debug_Num]);
            Debug_8042[Debug_Num] = 0;
        }
    }
#endif
}
//-----------------------------------------------------------------------------
// End of Handle Timer Events
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: Service_HOST_RST
 * LPC RESET
 * ------------------------------------------------------------------------- */
void Service_HOST_RST(void)
{
    if (F_Service_HOST_RST == 1)
    {
        F_Service_HOST_RST = 0;
        A20GATE_ON(); // FIXME xia
        CLEAR_MASK(SYSTEM_MISC1, ACPI_OS);
        // Host_Flag = 0x38;
        Host_Flag_DISAB_KEY = 1;
        Host_Flag_DISAB_AUX = 1;
    }
}
