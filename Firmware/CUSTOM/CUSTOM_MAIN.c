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
    // KBD_ListeningEvent();

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
#if 0
    //MAILBOX SMSha256 updata 测试用例
    if (REG8((SRAM_BASE_ADDR + 0x100)) == 0xAB)
    {
        printf("准备更新固件\n");
        TaskParams Params;
        Add_Task((TaskFunction)Flash_Update_Function, Params, &task_head);//sms update
    }
#endif
    //子系统dram更新测试用例
#if 1
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
    Service_LED_Indicator();

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
    // printf("31000:%x\n", *((VDWORD *)(0x31000)));
#endif
}
//-----------------------------------------------------------------------------
// Oem 1sec Events/Hook Here
//-----------------------------------------------------------------------------
void I2c_Write_Short(WORD value, WORD regoffset, WORD i2c_channel);
void __weak Hook_1secEventA(void) // get all temp
{
#if (I2C_MODULE_EN && SUPPORT_I2C_TEMPERATURE)
    get_temperature(2);
#endif
    // printf("MAILBOX_C2EINT:%x,MAILBOX_C2EINFO0:%x,MAILBOX_C2EINFO1:%x\n", MAILBOX_C2EINT, MAILBOX_C2EINFO0, MAILBOX_C2EINFO1);

    /* Channel 1 of I2C is used to get the temperature */
    // char temperature[3];
    // uint8_t a0 = 0x00;
    // uint8_t a1 = 0x01;
    // uint8_t a2 = 0x10;
    // {
    //     /* read temperature */
    //     // temperature[0] = I2c_Master_Read_Byte(0x4c, 0x0, i2c_channel);
    //     I3C_Legacy_Master_Write(0x4c, &a0, 1, I3C_MASTER0);
    //     I3C_Legacy_Master_Read(0x4c, (uint8_t*)temperature, 1, I3C_MASTER0);
    //     printf("t0:%x\n", temperature[0]);
    //     // temperature[1] = I2c_Master_Read_Byte(0x4c,0x1, i2c_channel);
    //     I3C_Legacy_Master_Write(0x4c, &a1, 1, I3C_MASTER0);
    //     I3C_Legacy_Master_Read(0x4c, ((uint8_t*)temperature + 1), 1, I3C_MASTER0);
    //     printf("t1:%x\n", temperature[1]);
    //     // temperature[2] = I2c_Master_Read_Byte(0x4c,0x10, i2c_channel);
    //     I3C_Legacy_Master_Write(0x4c, &a2, 1, I3C_MASTER0);
    //     I3C_Legacy_Master_Read(0x4c, (uint8_t*)(temperature + 2), 1, I3C_MASTER0);
    //     printf("t2:%x\n", temperature[2]);
    //     /* Write temp into ECSPACE_BASE_ADDR */
    //     CPU_TEMP = temperature[0];
    //     SYSTEM_TEMP = temperature[1];
    //     Req_flag = 0;
    //     dprint("I3C temperature=%d temp[1]=%d.%d\n", temperature[0], temperature[1], ((((BYTE)temperature[2]) >> 5) & 0x7) * 125);
    // }

    if (*((VBYTE*)(0x203B9)) == 1)
    {
        // printf("0x203B9:%x\n", *((VBYTE *)(0x203B9)));
        *((VBYTE*)(0x203B9)) = 0;

        // Mailbox_Test();
        // Mailbox_FW_Extension_Trigger();
        // Mailbox_APB2_Source_Alloc_Trigger();
        // Mailbox_Update_Function(0x3, 0x8000, 0x70800); // 发起mailbox更新
        // Mailbox_Read_FLASHUID_Trigger();
        //  Mailbox_Read_EFUSE_Trigger();
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

    if (*((VBYTE*)(0x203B9)) == 6)
    {
        *((VBYTE*)(0x203B9)) = 0;
        SYSCTL_RST0 |= 0x40;
        SYSCTL_RST0 &= (~0x40);
        vDelayXms(1);
        sysctl_iomux_slave0();
        I3C_Slave_Init(SLAVE0_SET_STATICADDR, SLAVE0_SET_IDPARTNO, SLAVE0_SET_DCR, SLAVE0_SET_BCR, I3C_SLAVE0);
        vDelayXms(10);
        I3C_MASTER_ENTDAA(master0_dev_read_char_table, MASTER0_DEV_DYNAMIC_ADDR_TABLE, I3C_MASTER0); //specify a dynamic addr
        vDelayXms(1);
        printf("SLAVE0的动态地址为:%x\n", I3C_ReadREG_DWORD(DYNADDR_OFFSET, I3C_SLAVE0));
        printf("SLAVE1的动态地址为:%x\n", I3C_ReadREG_DWORD(DYNADDR_OFFSET, I3C_SLAVE1));
    }
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
    // GPIO1_DDR0 |=0x1; 
    // GPIO1_DR0 |=0x1;
    // GPIO1_DR0 &=~0x1;
}
//-----------------------------------------------------------------------------
u32 RunTimeStamp = 0;
char adc_cnt = 0;
char mode_en = 0;
char compare = 0;
char counter = 0;
char flag = 0;

extern void ADC_Cont_Sample_Init_Single(uint8_t channelx, u_int8_t ADC_Databuffer_channelx, uint8_t mode);
extern void ADC_Cont_Sample_Init_Multi(uint8_t mode, uint8_t Data_Select, uint8_t Chanal_8_mode);
extern uint8_t MatchADCChannelToData(uint8_t channelx, uint16_t ADC_Databuffer_channelx);

char cec_sdata[8] = { 1,2,3,4,5,6,7,8 };

void __weak Hook_1secEventC(void) // update new rpm
{
    RunTimeStamp++;
    //printf("RunTimeStamp:%d\n", RunTimeStamp);
    if (RunTimeStamp % 5 == 0)
    {
        //Enter_LowPower_Mode();
    }

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
    // dprint("Service Run time stamp: %d \n", RunTimeStamp);

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

//-----------------------------------------------------------------------------
// FUNCTION: Service_WaitPS2_Handle
//
//-----------------------------------------------------------------------------
void Service_WaitPS2_Handle(void)
{
}
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
