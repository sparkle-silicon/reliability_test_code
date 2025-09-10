/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-06-03 18:16:42
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
 /*-----------------------------------------------------------------------------
  * Local Parameter Definition
  *---------------------------------------------------------------------------*/

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
    // printf("C2EINT:%x,C2EINFO0:%x,C2EINFO1:%x\n", C2EINT, C2EINFO0, C2EINFO1);

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

void __weak Hook_1secEventC(void) // update new rpm
{
    RunTimeStamp++;

    // *(unsigned short *)0x4800 =0xa5a5;
    // printf("0x4800:%x\n",*(unsigned short *)0x4800);

    // *(unsigned short *)0x4804 =0xa5a5;
    // printf("0x4804:%x\n",*(unsigned short *)0x4804);

    // *(unsigned short *)0x4806 =0xa5a5;
    // printf("0x4806:%x\n",*(unsigned short *)0x4806);

    // *(unsigned short *)0x480c =0xa5a5;
    // printf("0x480c:%x\n",*(unsigned short *)0x480c);

    // *(unsigned short *)0x480e =0xa5a5;
    // printf("0x480e:%x\n",*(unsigned short *)0x480e);

    // *(unsigned short *)0x4814 =0xa5a5;
    // printf("0x4814:%x\n",*(unsigned short *)0x4814);

    // *(unsigned short *)0x4816 =0xa5a5;
    // printf("0x4816:%x\n",*(unsigned short *)0x4816);

    // *(unsigned short *)0x4818 =0xa5a5;
    // printf("0x4818:%x\n",*(unsigned short *)0x4818);

    // *(unsigned short *)0x481a =0xa5a5;
    // printf("0x481a:%x\n",*(unsigned short *)0x481a);

    // *(unsigned short *)0x481c =0xa5a5;
    // printf("0x481c:%x\n",*(unsigned short *)0x481c);

    // *(unsigned short *)0x4820 =0xa5a5;
    // printf("0x4820:%x\n",*(unsigned short *)0x4820);

    // *(unsigned short *)0x4822 =0xa5a5;
    // printf("0x4822:%x\n",*(unsigned short *)0x4822);

    // *(unsigned short *)0x4824 =0xa5a5;
    // printf("0x4824:%x\n",*(unsigned short *)0x4824);

    // *(unsigned short *)0x4826 =0xa5a5;
    // printf("0x4826:%x\n",*(unsigned short *)0x4826);

    // *(unsigned short *)0x4828 =0xa5a5;
    // printf("0x4828:%x\n",*(unsigned short *)0x4828);

    // *(unsigned short *)0x482a =0xa5a5;
    // printf("0x482a:%x\n",*(unsigned short *)0x482a);

    // *(unsigned short *)0x482c =0xa5a5;
    // printf("0x482c:%x\n",*(unsigned short *)0x482c);

    // *(unsigned short *)0x482e =0xa5a5;
    // printf("0x482e:%x\n",*(unsigned short *)0x482e);

    // *(unsigned short *)0x4830 =0xa5a5;
    // printf("0x4830:%x\n",*(unsigned short *)0x4830);

    // *(unsigned short *)0x4832 =0xa5a5;
    // printf("0x4832:%x\n",*(unsigned short *)0x4832);

    // *(unsigned short *)0x4834 =0xa5a5;
    // printf("0x4834:%x\n",*(unsigned short *)0x4834);

    // *(unsigned short *)0x4836 =0xa5a5;
    // printf("0x4836:%x\n",*(unsigned short *)0x4836);

    // *(unsigned short *)0x4838 =0xa5a5;
    // printf("0x4838:%x\n",*(unsigned short *)0x4838);

    // *(unsigned short *)0x483A =0xa5a5;
    // printf("0x483A:%x\n",*(unsigned short *)0x483A);

    // *(unsigned short *)0x483c =0xa5a5;
    // printf("0x483c:%x\n",*(unsigned short *)0x483c);

    // *(unsigned short *)0x483E =0xa5a5;
    // printf("0x483E:%x\n",*(unsigned short *)0x483E);

    // *(unsigned short *)0x4840 =0xa5a5;
    // printf("0x4840:%x\n",*(unsigned short *)0x4840);

    // *(unsigned short *)0x4842 =0xa5a5;
    // printf("0x4842:%x\n",*(unsigned short *)0x4842);

    // *(unsigned short *)0x4844 =0xa5a5;
    // printf("0x4844:%x\n",*(unsigned short *)0x4844);

    // *(unsigned short *)0x4846 =0xa5a5;
    // printf("0x4846:%x\n",*(unsigned short *)0x4846);

    // *(unsigned short *)0x4848 =0xa5a5;
    // printf("0x4848:%x\n",*(unsigned short *)0x4848);

    // *(unsigned short *)0x484a =0xa5a5;
    // printf("0x484a:%x\n",*(unsigned short *)0x484a);

    // *(unsigned short *)0x484c =0xa5a5;
    // printf("0x484c:%x\n",*(unsigned short *)0x484c);

    // *(unsigned short *)0x484e =0xa5a5;
    // printf("0x484e:%x\n",*(unsigned short *)0x484e);

    // *(unsigned short *)0x4850 =0xa5a5;
    // printf("0x4850:%x\n",*(unsigned short *)0x4850);

    // if (RunTimeStamp % 10 == 0)
    {
        // 连续 
        //  if(adc_cnt < 0)
        //      adc_cnt = 0;
        // ADC_Cont_Sample_Init_Single(adc_cnt, 0, 0);
        // adc_cnt++;
        // if(adc_cnt > 14)
        // {
        //     adc_cnt = 0;
        //     mode_en ^= 1;
        // }

        // 软件
        // if(counter<10)
        // {
        //     counter++;
        // printf("ADC_TRIGGER_0:%x\n",ADC_TRIGGER);
        // ADC_TRIGGER = 0x1;
        // printf("ADC_TRIGGER_1:%x\n",ADC_TRIGGER);
        // printf("ADC_INTSTAT:%x\n",ADC_INTSTAT);
        //     printf("counter:%x\n",counter);
        // }
        // else
        // {
        //     counter=0;
        //     //切换通道
        // adc_cnt++;
        // if(adc_cnt > 14)
        // {
        //     adc_cnt = 0;
        // }
        // ADC_Swap_Channel(adc_cnt,0);
        // ADC_TRIGGER = 0x1;
        // }
        //  if(adc_cnt < 0)
        //      adc_cnt = 0;
        //  mode_en &= 0x1;
        // ADC_SW_Sample_Init(2, 0, 0, mode_en);
        // printf("ADC channel:%d mode:%d\n", adc_cnt, mode_en);
        // adc_cnt++;
        // if(adc_cnt > 14)
        // {
        //     adc_cnt = 0;
        //     mode_en ^= 1;
        // }

        // 硬件
        //切换多通道
        // if(flag==0)
        // {
        //     flag ^= 1;
        //     MatchADCChannelToData(4, 0);
        //     MatchADCChannelToData(5, 1);
        //     MatchADCChannelToData(6, 2);
        //     MatchADCChannelToData(7, 3);
        // }
        // else
        // {
        //     flag ^= 1;
        //     MatchADCChannelToData(0, 0);
        //     MatchADCChannelToData(1, 1);
        //     MatchADCChannelToData(2, 2);
        //     MatchADCChannelToData(3, 3);
        // }
        // ADC_HW_Sample_Init_Single(adc_cnt,0,mode_en,500,0,0,0x10);
        // printf("ADC channel:%d mode:%d\n", adc_cnt, mode_en);
        // printf("ADC_INTSTAT:%x\n",ADC_INTSTAT);
        // adc_cnt++;
        // if(adc_cnt > 14)
        // {
        //     adc_cnt = 0;
        //     mode_en ^= 1;
        // }

        // if(adc_cnt<0)
        //     adc_cnt=0;
        // mode_en&=0x1;
        // ADC_HW_Sample_Init(adc_cnt,adc_cnt,0,mode_en,500,-1);
        // printf("ADC channel:%d mode:%d\n",adc_cnt,mode_en);
        // adc_cnt++;
        // if(adc_cnt >7)
        // {
        //     adc_cnt = 0;
        //     mode_en^=1;
        // }
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
 * FUNCTION: Service_LPCRST
 * LPC RESET
 * ------------------------------------------------------------------------- */
void Service_LPCRST(void)
{
    if (F_Service_LPCRST == 1)
    {
        F_Service_LPCRST = 0;
        A20GATE_ON(); // FIXME xia
        CLEAR_MASK(SYSTEM_MISC1, ACPI_OS);
        // Host_Flag = 0x38;
        Host_Flag_DISAB_KEY = 1;
        Host_Flag_DISAB_AUX = 1;
    }
}
