/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-10-02 11:45:21
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
/*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Oem 1ms Events/Hook Here
//-----------------------------------------------------------------------------
extern BYTE Moudle_test(void);
void __weak Hook_1msEvent(BYTE EventId)
{
#if SUPPORT_ANX7447
    ANX_HOOK_1ms();
#endif
    Event_Center(EventId);    // Polling system event
    Sys_PowerState_Control(); // System Power Control
    // KBD_ListeningEvent();
    Moudle_test();

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
void __weak Hook_1secEventA(void) // get all temp
{
#if (I2C_MODULE_EN && SUPPORT_I2C_TEMPERATURE)
    get_temperature(2);
#endif
    // printf("C2EINT:%x,C2EINFO0:%x,C2EINFO1:%x\n", C2EINT, C2EINFO0, C2EINFO1);

#if (GLE01 == 1)
    if (*((VBYTE *)(0x203B9)) == 1)
    {
        // printf("0x203B9:%x\n", *((VBYTE *)(0x203B9)));
        *((VBYTE *)(0x203B9)) = 0;

        // Mailbox_Test();
        // Mailbox_FW_Extension_Trigger();
        // Mailbox_APB2_Source_Alloc_Trigger();
        // Mailbox_Update_Function(0x3, 0x8000, 0x70800); // 发起mailbox更新
        // Mailbox_Read_FLASHUID_Trigger();
        //  Mailbox_Read_EFUSE_Trigger();
        Mailbox_WriteRootKey_Trigger();
    }
    if (*((VBYTE *)(0x203B9)) == 2)
    {
        *((VBYTE *)(0x203B9)) = 0;
        Mailbox_UpdateHMACKey_Trigger();
    }

    if (*((VBYTE *)(0x203B9)) == 3)
    {
        *((VBYTE *)(0x203B9)) = 0;
        //Mailbox_IncrementCounter_Trigger(0x3FFE);
    }

    if (*((VBYTE *)(0x203B9)) == 4)
    {
        *((VBYTE *)(0x203B9)) = 0;
        Mailbox_RequestCounter_Trigger();
    }

    if (*((VBYTE *)(0x203B9)) == 5)
    {
        *((VBYTE *)(0x203B9)) = 0;
        Mailbox_ReadParameter_Trigger();
    }


            // static int i=0;
            // i++;
            // if(i==10)
            // {
            //     i=0;
            // }
            // printf("i:%d\n",i);
            // PWM_Init_channel(PWM_CHANNEL1, PWM_HIGH, PWM_CLK0, PWM_CTR0, i*10, 0);
            // PWM_Init_channel(PWM_CHANNEL0, PWM_HIGH, PWM_CLK0, PWM_CTR0, i*10, 0);

            // DWORD Polling0 = TACH_Get_Polling(0);
            // if(Polling0 != -1)
            // {
            //     TACH0_Speed=(DWORD)6000000/Polling0;
            //     dprint("FAN1 RPM is %u\n",TACH0_Speed);
            // }
        


    // static int flag=0;
    // if(flag)
    // {
    //     flag=0;        
    // }
    // else
    // {
    //     flag=1;
    // }


    // GPIO_Config(GPIOB, 0, 1, flag, 0, 0);
    // GPIO_Config(GPIOB, 1, 1, flag, 0, 0);
    // GPIO_Config(GPIOB, 2, 1, flag, 0, 0);
    // GPIO_Config(GPIOB, 3, 1, flag, 0, 0);
    // GPIO_Config(GPIOB, 4, 1, flag, 0, 0);
    // GPIO_Config(GPIOB, 5, 1, flag, 0, 0);
    // GPIO_Config(GPIOB, 6, 1, flag, 0, 0);
    // GPIO_Config(GPIOB, 7, 1, flag, 0, 0);

    // sysctl_iomux_config(GPIOB, 0, 0);
    // sysctl_iomux_config(GPIOB, 1, 0);
    // sysctl_iomux_config(GPIOB, 2, 0);
    // sysctl_iomux_config(GPIOB, 3, 0);
    // sysctl_iomux_config(GPIOB, 4, 0);
    // sysctl_iomux_config(GPIOB, 5, 0);
    // sysctl_iomux_config(GPIOB, 6, 0);
    // sysctl_iomux_config(GPIOB, 7, 0);

    // GPIO1_DDR0 =0xff;
    // if(flag)
    // {
    //   GPIO1_DR0 = 0xff;
    // }
    // else
    // {
    //     GPIO1_DR0 = 0x0;
    // }
    // GPIO_Config(GPIOA, 0, 1,flag , 0, 0);
    // GPIO_Config(GPIOE, 4, 1, flag, 0, 0);
    //GPIO_Config(GPIOA, 5, 1, flag, 0, 0);
    // GPIO_Config(GPIOC, 11, 1, 0, 0, 0);

    // printf("0x30464:%x\n",(*(unsigned int*)(0x30464)));
    // printf("GPIO2_DDR1:%x\n",GPIO2_DDR1);
	// printf("GPIO2_DR1 :%x\n",GPIO2_DR1 );      
    // printf("GPIO2_EXT1:%x\n",GPIO2_EXT1);

    // GPIO_Config(GPIOB, 7, 1, 0, 0, 0);
    // GPIO_Config(GPIOB, 27, 1, 1, 0, 0);
    // GPIO_Config(GPIOC, 9, 1, flag, 0, 0);
    // GPIO_Config(GPIOE, 19, 1, 0, 0, 0);

    // printf("PA0\n");
    // printf("0x30454:%x\n",(*(unsigned int*)(0x30454)));
    // printf("0x3046C:%x\n",(*(unsigned int*)(0x3046C)));
    // printf("GPIO0_DDR0:%x\n",GPIO0_DDR0);
	// printf("GPIO0_DR0:%x\n",GPIO0_DR0);      
    // printf("GPIO0_EXT0:%x\n",GPIO0_EXT0);

    // printf("0x30460:%x\n",(*(unsigned int*)(0x30460)));
    // printf("GPIO1_DDR3:%x\n",GPIO1_DDR3);
	// printf("GPIO1_DR3:%x\n",GPIO1_DR3);      
    // printf("GPIO1_EXT3:%x\n",GPIO1_EXT3);

    // printf("0x3045C:%x\n",(*(unsigned int*)(0x3045C)));
    // printf("GPIO1_DDR0:%x\n",GPIO1_DDR0);
	// printf("GPIO1_DR0:%x\n",GPIO1_DR0);      
    // printf("GPIO1_EXT0:%x\n",GPIO1_EXT0);

    // printf("0x30464:%x\n",(*(unsigned int*)(0x30464)));
    // printf("GPIO2_DDR1:%x\n",GPIO2_DDR1);
	// printf("GPIO2_DR1:%x\n",GPIO2_DR1);      
    // printf("GPIO2_EXT1:%x\n",GPIO2_EXT1); 


    // printf("PD7 PD8\n");
    // printf("0x30468:%x\n",(*(unsigned int*)(0x30468)));
    // printf("0x30480:%x\n",(*(unsigned int*)(0x30480)));
    // printf("GPIO2_DDR2:%x\n",GPIO2_DDR2);
	// printf("GPIO2_DR2:%x\n",GPIO2_DR2);      
    // printf("GPIO2_EXT2:%x\n",GPIO2_EXT2);
    // printf("GPIO2_EXT3:%x\n",GPIO2_EXT3);

    // printf("PE4\n");
    // printf("0x30468:%x\n",(*(unsigned int*)(0x30468)));
    // printf("GPIO3_DDR0:%x\n",GPIO3_DDR0);
	// printf("GPIO3_DR0:%x\n",GPIO3_DR0);
    // printf("GPIO3_EXT0:%x\n",GPIO3_EXT0);

    // printf("0x30468:%x\n",(*(unsigned int*)(0x30468)));
    // printf("GPIO3_DDR2:%x\n",GPIO3_DDR2);
	// printf("GPIO3_DR2:%x\n",GPIO3_DR2);
    // printf("GPIO3_EXT2:%x\n",GPIO3_EXT2);


#endif
}
//-----------------------------------------------------------------------------
void __weak Hook_1secEventB(void) // get fan rpm
{
#if SUPPORT_FAN1 || SUPPORT_FAN2
    DWORD Polling0 = TACH_Get_Polling(0);
    if (Polling0 != -1)
    {
        TACH0_Speed = (DWORD)6000000 / Polling0;
        dprint("FAN2 RPM is %u\n", TACH0_Speed);
    }
#endif
}
//-----------------------------------------------------------------------------
u32 RunTimeStamp = 0;
char adc_cnt = 0;
char mode_en = 1;
extern void ADC_Cont_Sample_Init(uint8_t channelx, u_int8_t ADC_Databuffer_channelx, uint8_t attenuationMode, uint8_t mode);
void __weak Hook_1secEventC(void) // update new rpm
{
    RunTimeStamp++;
    if (RunTimeStamp % 10 == 0)
    {
        // 连续
        //  if(adc_cnt < 0)
        //      adc_cnt = 0;
        //  ADC_Cont_Sample_Init(adc_cnt, 0, 0, mode_en);
        //  printf("ADC channel:%d\n", adc_cnt);
        //  adc_cnt++;
        //  if(adc_cnt > 11)
        //  {
        //      adc_cnt = 0;
        //      mode_en ^= 1;
        //  }

        // 软件
        //  if(adc_cnt < 0)
        //      adc_cnt = 0;
        //  mode_en &= 0x1;
        //  ADC_SW_Sample_Init(adc_cnt, 0, 0, mode_en);
        //  printf("ADC channel:%d mode:%d\n", adc_cnt, mode_en);
        //  printf("ADC Data:0x%x\n", ADC_SW_Sample(0));
        //  adc_cnt++;
        //  if(adc_cnt > 11)
        //  {
        //      adc_cnt = 0;
        //      mode_en ^= 1;
        //  }

        // 硬件
        //  if(adc_cnt<0)
        //      adc_cnt=0;
        //  mode_en&=0x1;
        //  ADC_HW_Sample_Init(adc_cnt,0,0,mode_en,500);
        //  printf("ADC channel:%d mode:%d\n",adc_cnt,mode_en);
        //  adc_cnt++;
        //  if(adc_cnt >11)
        //  {
        //      adc_cnt = 0;
        //      mode_en^=1;
        //  }
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
