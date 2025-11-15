/*
 * @Author: Iversu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2025-10-21 17:40:12
 * @Description: Power sequnce control function example
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
#include "CUSTOM_POWER.H"
#include "CUSTOM_GPIO.H"
#include "KERNEL_PWM.H"
#include "KERNEL_PS2.H"
 /***************Power Sequence Debugger Information Record ******************/
sPowSeq_Pin_Record PowSeq_Record[40];
/*******************************************************/
// 记录每条时序动作信息到PowSeq_Record_To_Array
/*******************************************************/
void PowerSequence_Record_To_Array(char group, BYTE pin, BYTE level, WORD delay)
{
    PowSeq_Record_Cnt++;
    // Record GPIO Group informae_tion
    if (group > 0x0 && group <= 0x5)
    {
        PowSeq_Record[Debugger_PowerSequence_Step].pin_group = group - 1;
    }
    else
    {
        dprint("GPIO group input error\n");
    }
    // Record GPIO Pin Information
    if (pin >= 0x0 && pin <= 0x1F)
    {
        PowSeq_Record[Debugger_PowerSequence_Step].pin_location = pin;
    }
    else
    {
        dprint("gpio pin input error\n");
    }
    // Record GPIO Output Level Information
    if (level == 0)
    {
        PowSeq_Record[Debugger_PowerSequence_Step].pin_level = 0x0;
    }
    else if (level == 1)
    {
        PowSeq_Record[Debugger_PowerSequence_Step].pin_level = 0x1;
    }
    else
    {
        dprint("gpio level input error\n");
    }
    if ((IsGpioOut(group, pin)) == 0)
    {
        PowSeq_Record[Debugger_PowerSequence_Step].pin_level = ReadGPIOLevel(group, pin);
    }
    // Record GPIO Delay Information
    PowSeq_Record[Debugger_PowerSequence_Step].pin_delay = delay;
    Debugger_PowerSequence_Step++;
}
/*******************************************************/
// 将时序记录信息拷贝到SRAM中，以便调试器读取该信息
/*******************************************************/
void Copy_Power_Sequence_Record_To_Sram(VBYTE Record_Flag)
{
    for (int i = 0; i <= 1024; i++)
    {
        *((VBYTE*)(0x31000 + i)) = 0x0;
    }
    BYTE* PowSeq_Mem_Ptr = (BYTE*)0x31002;
    BYTE length_record;
    /*Record PowerSequence type flag to 0x31000*/
    switch (Record_Flag)
    {
    case 1:
        *((VBYTE*)(SRAM_BASE_ADDR)) = 0x1;
        break;
    case 2:
        *((VBYTE*)(SRAM_BASE_ADDR)) = 0x2;
        break;
    case 3:
        *((VBYTE*)(SRAM_BASE_ADDR)) = 0x3;
        break;
    case 4:
        *((VBYTE*)(SRAM_BASE_ADDR)) = 0x4;
        break;
    case 5:
        *((VBYTE*)(SRAM_BASE_ADDR)) = 0x5;
        break;
    case 6:
        *((VBYTE*)(SRAM_BASE_ADDR)) = 0x6;
        break;
    case 7:
        *((VBYTE*)(SRAM_BASE_ADDR)) = 0x7;
        break;
    default:
        *((VBYTE*)(0x31000)) = 0x0;
        break;
    }
    /*Record PowerSequence Count to 0x31001 */
    *((VBYTE*)(0x31001)) = PowSeq_Record_Cnt;
    PowSeq_Record_Cnt = 0;
    Debugger_PowerSequence_Step = 1;
    /*Copy PowerSequence information to Sram*/
    length_record = sizeof(PowSeq_Record);
    memcpy(PowSeq_Mem_Ptr, PowSeq_Record, length_record);
    // printf("memcpy to sram\n");
}
/*******************************************************/
// 记录当前GPIO状态信息
/*******************************************************/
void GPIO_Record(void)
{
    BYTE GPIO_record[14];
    GPIO_record[0] = GPIO0_EXT0;
    GPIO_record[1] = GPIO0_EXT1;
    GPIO_record[2] = GPIO0_EXT2;
    GPIO_record[3] = GPIO0_EXT3;
    GPIO_record[4] = GPIO1_EXT0;
    GPIO_record[5] = GPIO1_EXT1;
    GPIO_record[6] = GPIO1_EXT2;
    GPIO_record[7] = GPIO1_EXT3;
    GPIO_record[8] = GPIO2_EXT0;
    GPIO_record[9] = GPIO2_EXT1;
    GPIO_record[10] = GPIO2_EXT2;
    GPIO_record[11] = GPIO3_EXT0;
    GPIO_record[12] = GPIO3_EXT1;
    GPIO_record[13] = GPIO3_EXT2;
    memcpy((BYTEP)0x31800, GPIO_record, sizeof(GPIO_record));
}
BYTE PF_CHARGE_LED_ON(void)
{
    return 0;
}
BYTE PF_CHARGE_LED_OFF(void)
{
    return 0;
}
BYTE PF_CHARGE_FULL_LED_ON(void)
{
    return 0;
}
BYTE PF_CHARGE_FULL_LED_OFF(void)
{
    return 0;
}
//-----------------------------------------------------------------------------
// The example of S5 -> S0 sequence for example
//-----------------------------------------------------------------------------
BYTE S5S0_EXAMPLE_1(void)
{
    /*仅为示例时序，需根据实际需求进行修改*/
    // This is S5S0 power sequence 1
    dprint("S5->S0\n");
    ALW_ON();                                       // 操作GPIO的时序函数需要按照后缀为_ON、_OFF、_H、_L的格式来编写，以便调试器识别
    PowerSequence_Record_To_Array(GPIOA, 7, 1, 5); // record ALW_ON for debugger
    return 0;
}
BYTE S5S0_EXAMPLE_2(void)
{
    // This is S5S0 power sequence 2
    ALW_OFF();                                       // 操作GPIO的时序函数需要按照后缀为_ON、_OFF、_H、_L的格式来编写，以便调试器识别
    PowerSequence_Record_To_Array(GPIOA, 7, 0, 10); // record ALW_OFF for debugger
    return 0;
}
BYTE S5S0_EXAMPLE_3(void)
{
    // This is S5S0 power sequence 3
    HOT_LED_H();                                     // 操作GPIO的时序函数需要按照后缀为_ON、_OFF、_H、_L的格式来编写，以便调试器识别
    PowerSequence_Record_To_Array(GPIOA, 31, 1, 5); // record HOT_LED_H for debugger
    return 0;
}
BYTE S5S0_EXAMPLE_4(void)
{
    // This is S5S0 power sequence 4
    HOT_LED_L();                                      // 操作GPIO的时序函数需要按照后缀为_ON、_OFF、_H、_L的格式来编写，以便调试器识别
    PowerSequence_Record_To_Array(GPIOC, 0, 0, 10); // record HOT_LED_L for debugger
    return 0;
}
BYTE S5S0_EXAMPLE_5(void)
{
    // This is S5S0 power sequence 5
    if (Read_ALW_PWRGD()) // 调试器不需要识别状态判定函数
    {
        return (1);
    }
    return 0;
}
//-----------------------------------------------------------------------------
// The example of S0 -> S5 sequence
//-----------------------------------------------------------------------------
BYTE S0S5_EXAMPLE_1(void)
{
    // This is S5S0_EXAMPLE 1
    dprint("S0->S5\n");
    S3OK_L();
    PowerSequence_Record_To_Array(GPIOA, 4, 0, 0); // record HOT_LED_L for debugger
    return 0;
}
BYTE S0S5_EXAMPLE_2(void)
{
    // This is S5S0_EXAMPLE 2
    ALW_ON();
    PowerSequence_Record_To_Array(GPIOA, 7, 1, 0); // record HOT_LED_L for debugger
    return 0;
}
BYTE S0S5_EXAMPLE_3(void)
{
    // This is S5S0_EXAMPLE 2
    P1V8_EN_H();
    PowerSequence_Record_To_Array(GPIOA, 10, 1, 0); // record HOT_LED_L for debugger
    return 0;
}
//-----------------------------------------------------------------------------
// The example of S4 -> S0 sequence
//-----------------------------------------------------------------------------
BYTE S4S0_EXAMPLE_1(void)
{
    // This is S5S0_EXAMPLE 1
    dprint("S4->S0\n");
    GPIOB6(HIGH);
    return 0;
}
BYTE S4S0_EXAMPLE_2(void)
{
    // This is S5S0_EXAMPLE 2
    GPIOB6(LOW);
    return 0;
}
//-----------------------------------------------------------------------------
// The example of S0 -> S4 sequence
//-----------------------------------------------------------------------------
BYTE S0S4_EXAMPLE_1(void)
{
    // This is S5S0_EXAMPLE 1
    dprint("S0->S4\n");
    GPIOB6(LOW);
    return 0;
}
BYTE S0S4_EXAMPLE_2(void)
{
    // This is S5S0_EXAMPLE 2
    GPIOB6(HIGH);
    return 0;
}
//-----------------------------------------------------------------------------
// The example of S3 -> S0 sequence
//-----------------------------------------------------------------------------
BYTE S3S0_EXAMPLE_1(void)
{
    // This is S5S0_EXAMPLE 1
    dprint("S3->S0\n");
    GPIOC6(HIGH);
    return 0;
}
BYTE S3S0_EXAMPLE_2(void)
{
    // This is S5S0_EXAMPLE 2
    GPIOC6(LOW);
    return 0;
}
//-----------------------------------------------------------------------------
// The example of S0 -> S3 sequence
//-----------------------------------------------------------------------------
BYTE S0S3_EXAMPLE_1(void)
{
    // This is S5S0_EXAMPLE 1
    dprint("S0->S3\n");
    GPIOC6(LOW);
    return 0;
}
BYTE S0S3_EXAMPLE_2(void)
{
    // This is S5S0_EXAMPLE 2
    GPIOC6(HIGH);
    return 0;
}
//-----------------------------------------------------------------------------
// The example of Reboot sequence
//-----------------------------------------------------------------------------
BYTE Reboot_EXAMPLE_1(void)
{
    // This is REBOOT_EXAMPLE 1
    dprint("Reboot start\n");
    GPIOA30(HIGH);
    return 0;
}
BYTE Reboot_EXAMPLE_2(void)
{
    // This is REBOOT_EXAMPLE 2
    return 0;
}
//-----------------------------------------------------------------------------
//  The CUSTOM variable of ReBoot
//-----------------------------------------------------------------------------
void Custom_Reboot_Var(void)
{
    PowerState_Monitor_Index = 0x00;
    PowerChange_Var_Clear();
    KBS_KSDC1R |= (KBS_KSDC1R_EN | KBS_KSDC1R_INT_EN);//keep KBS enable and interrupt enable
    if (Set_AutoON_AfterUpdate() == TRUE)
    {
    }
    else
    {
        Auto_On = 1;
    }
}
//-----------------------------------------------------------------------------
//  The variable of Reboot
//-----------------------------------------------------------------------------
void Reboot_Var(void)
{
    Custom_Reboot_Var();
}
//-----------------------------------------------------------------------------
//  The CUSTOM variable of Sx -> S0
//-----------------------------------------------------------------------------
void Custom_SX_S0_Var(void)
{
    xOOB_PauseTimer = 200;
    PowerState_Monitor_Index = 0x00;
    KBS_KSDC1R |= (KBS_KSDC1R_EN | KBS_KSDC1R_INT_EN);//keep KBS enable and interrupt enable
}
//-----------------------------------------------------------------------------
//  The variable of Sx -> S0
//-----------------------------------------------------------------------------
void SX_S0_Var(void)
{
    SET_FLAG(SYSTEM_MISC1, ACPI_OS);
    System_PowerState = SYSTEM_S0;
    Custom_SX_S0_Var();
}
//-----------------------------------------------------------------------------
//  The CUSTOM variable of S0 -> Sx
//-----------------------------------------------------------------------------
void Custom_S0_SX_Var(void)
{
    xOOB_PauseTimer = 200;
    PowerState_Monitor_Index = 0x00;
#if (ENABLE_S3_KEYBOARD_INTR&ENABLE_S5_KEYBOARD_INTR)  //如果需要支持S3/S5状态下，按键唤醒，则不关闭KBS扫描和使能
#else
    KBS_KSDC1R &= ~KBS_KSDC1R_EN;//关闭KBS扫描
#endif
    PowerChange_Var_Clear();
}
//-----------------------------------------------------------------------------
//  The variable of S0 -> Sx
//-----------------------------------------------------------------------------
void S0_SX_Var(void)
{
    CLEAR_FLAG(SYSTEM_MISC1, ACPI_OS);
    Custom_S0_SX_Var();
}
//-----------------------------------------------------------------------------
// The variable of S5 -> S0
//-----------------------------------------------------------------------------
BYTE S5_S0_Variable(void)
{
    dprint("S5_S0_Variable\n");
    SX_S0_Var();
    return 0;
}
//-----------------------------------------------------------------------------
// The variable of S4 -> S0
//-----------------------------------------------------------------------------
BYTE S4_S0_Variable(void)
{
    SX_S0_Var();
    return 0;
}
//-----------------------------------------------------------------------------
// The variable of S3 -> S0
//-----------------------------------------------------------------------------
BYTE S3_S0_Variable(void)
{
    SX_S0_Var();
    return 0;
}
//-----------------------------------------------------------------------------
//  The variable of S0 -> S5
//-----------------------------------------------------------------------------
BYTE S0_S5_Variable(void)
{
    System_PowerState = SYSTEM_S5;
    S0_SX_Var();
    CHIPRESET_Check_AfterUpdate();
    return 0;
}
//-----------------------------------------------------------------------------
//  The variable of S0 -> S4
//-----------------------------------------------------------------------------
BYTE S0_S4_Variable(void)
{
    System_PowerState = SYSTEM_S4;
    S0_SX_Var();
    return 0;
}
//-----------------------------------------------------------------------------
// The variable of S0 -> S3
//-----------------------------------------------------------------------------
BYTE S0_S3_Variable(void)
{
    System_PowerState = SYSTEM_S3;
    S0_SX_Var();
    return 0;
}
//-----------------------------------------------------------------------------
// The variable of Reboot
//-----------------------------------------------------------------------------
BYTE Reboot_Variable(void)
{
    System_PowerState = SYSTEM_S5;  //此处仅为重启示例代码，需要根据具体主板时序来修改相关时序及状态
    Reboot_Var();
    return 0;
}
//-----------------------------------------------------------------------------
// Provide power on timing information for the debugger
//-----------------------------------------------------------------------------
BYTE GetS5S0_PowerOnSequenceInfo(void)
{
    PowerSequence_Record_To_Array(GPIOA, 7, 1, 5); // record Info for debugger
    PowerSequence_Record_To_Array(GPIOA, 7, 0, 10);
    PowerSequence_Record_To_Array(GPIOA, 31, 1, 5);
    PowerSequence_Record_To_Array(GPIOA, 31, 0, 10);
    return 0;
}

//-----------------------------------------------------------------------------
// Provide power on timing information for the debugger
//-----------------------------------------------------------------------------
BYTE GetS0S5_PowerOnSequenceInfo(void)
{
    PowerSequence_Record_To_Array(GPIOA, 4, 0, 0); // record Info for debugger
    PowerSequence_Record_To_Array(GPIOA, 7, 1, 0);
    PowerSequence_Record_To_Array(GPIOA, 10, 1, 0);
    return 0;
}
//-----------------------------------------------------------------------------
// Provide power on timing information for the debugger
//-----------------------------------------------------------------------------
BYTE GetS4S0_PowerOnSequenceInfo(void)
{
    PowerSequence_Record_To_Array(GPIOB, 6, 1, 5);
    PowerSequence_Record_To_Array(GPIOB, 6, 0, 0);
    return 0;
}
//-----------------------------------------------------------------------------
// Provide power on timing information for the debugger
//-----------------------------------------------------------------------------
BYTE GetS0S4_PowerOnSequenceInfo(void)
{
    PowerSequence_Record_To_Array(GPIOB, 6, 0, 5);
    PowerSequence_Record_To_Array(GPIOB, 6, 1, 0);
    return 0;
}
//-----------------------------------------------------------------------------
// Provide power on timing information for the debugger
//-----------------------------------------------------------------------------
BYTE GetS3S0_PowerOnSequenceInfo(void)
{
    PowerSequence_Record_To_Array(GPIOC, 6, 1, 5);
    PowerSequence_Record_To_Array(GPIOC, 6, 0, 0);
    return 0;
}
//-----------------------------------------------------------------------------
// Provide power on timing information for the debugger
//-----------------------------------------------------------------------------
BYTE GetS0S3_PowerOnSequenceInfo(void)
{
    PowerSequence_Record_To_Array(GPIOC, 6, 0, 5);
    PowerSequence_Record_To_Array(GPIOC, 6, 1, 0);
    return 0;
}
//-----------------------------------------------------------------------------
// Provide power on timing information for the debugger
//-----------------------------------------------------------------------------
BYTE Getreboot_PowerOnSequenceInfo(void)
{
    PowerSequence_Record_To_Array(GPIOA, 30, 1, 0);
    PowerSequence_Record_To_Array(GPIOA, 30, 0, 5);
    PowerSequence_Record_To_Array(GPIOA, 30, 1, 0);
    PowerSequence_Record_To_Array(GPIOA, 29, 1, 0);
    PowerSequence_Record_To_Array(GPIOA, 29, 0, 5);
    PowerSequence_Record_To_Array(GPIOA, 29, 1, 0);
    return 0;
}
//-------------------------------------------------------------------------------------
//  Do S5 -> S0 power sequnce
//  S5S0时序仅为支持调试器的示例时序，客户可根据需求更改时序
//  指示信息-> 0：GPIO时序 1：GPIO输入判定状态 2：其他执行函数，如配置PNP，修改变量等，不涉及时序
//-------------------------------------------------------------------------------------
const sPowerSEQ PowerSequence_S5S0[] =
{
    // 函数指针                     延时      指示信息
    { 0x0000,                       0,          0,}, // Dummy step always in the top of tabel
    { S5S0_EXAMPLE_1,               5,          0,},
    { S5S0_EXAMPLE_2,               10,         0,},
    { S5S0_EXAMPLE_3,               5,          0,},
    { S5S0_EXAMPLE_4,               10,         0,},
    { S5S0_EXAMPLE_5,               0,          1,},
    { LD_PNP_Config,                0,          2,},
    { S5_S0_Variable,               0,          2,}, // always in the end of tabel
};
//-------------------------------------------------------------------------------------
//  Do S0 -> S5 power sequnce
//-------------------------------------------------------------------------------------
const sPowerSEQ PowerSequence_S0S5[] =
{
    // 函数指针                     延时      指示信息
    {0x0000,                        0,          0,}, // Dummy step always in the top of tabel
    {S0S5_EXAMPLE_1,                0,          0,},
    {S0S5_EXAMPLE_2,                0,          0,},
    {S0S5_EXAMPLE_3,                0,          0,},
    {S0_S5_Variable,                0,          0,}, // always in the end of tabel
};
//-------------------------------------------------------------------------------------
//  Do S4 -> S0 power sequnce
//-------------------------------------------------------------------------------------
const sPowerSEQ PowerSequence_S4S0[] =
{
    // 函数指针                     延时      指示信息
    {0x0000,                        0,          0,}, // Dummy step always in the top of tabel
    {S4S0_EXAMPLE_1,                5,          0,},
    {S4S0_EXAMPLE_2,                0,          0,},
    {S4_S0_Variable,                0,          0,}, // always in the end of tabel
};
//-------------------------------------------------------------------------------------
//  Do S0 -> S4 power sequnce
//-------------------------------------------------------------------------------------
const sPowerSEQ PowerSequence_S0S4[] =
{
    // 函数指针                     延时      指示信息
    {0x0000,                        0,          0,}, // Dummy step always in the top of tabel
    {S0S4_EXAMPLE_1,                5,          0,},
    {S0S4_EXAMPLE_2,                0,          0,},
    {S0_S4_Variable,                0,          0,}, // always in the end of tabel
};
//-------------------------------------------------------------------------------------
//  Do S3 -> S0 power sequnce
//-------------------------------------------------------------------------------------
const sPowerSEQ PowerSequence_S3S0[] =
{
    // 函数指针                     延时      指示信息
    {0x0000,                        0,          0,}, // Dummy step always in the top of tabel
    {S3S0_EXAMPLE_1,                5,          0,},
    {S3S0_EXAMPLE_2,                0,          0,},
    {S3_S0_Variable,                0,          0,}, // always in the end of tabel
};
//-------------------------------------------------------------------------------------
//  Do S0 -> S3 power sequnce
//-------------------------------------------------------------------------------------
const sPowerSEQ PowerSequence_S0S3[] =
{
    // 函数指针                     延时      指示信息
    {0x0000,                        0,          0,}, // Dummy step always in the top of tabel
    {S0S3_EXAMPLE_1,                5,          0,},
    {S0S3_EXAMPLE_2,                0,          0,},
    {S0_S3_Variable,                0,          0,}, // always in the end of tabel
};
//-------------------------------------------------------------------------------------
//  Do Reboot power sequnce
//-------------------------------------------------------------------------------------
const sPowerSEQ PowerSequence_Reboot[] =
{
    // 函数指针                     延时      指示信息
    {0x0000,                        0,          0,}, // Dummy step always in the top of tabel
    {Reboot_EXAMPLE_1,              0,          0,},
    {Reboot_EXAMPLE_2,              5,          0,},
    {Reboot_Variable,               0,          0,}, // always in the end of tabel
};
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Custom_S3_S0_Trigger(void)
{
    if (SystemIsS0)
    {
        return;
    }
#if ENABLE_DEBUGGER_SUPPORT
    GPIO_Record();
    memset(PowSeq_Record, 0xff, 40 * sizeof(sPowSeq_Pin_Record));
#endif
    System_PowerState = SYSTEM_S3_S0;
    PowerSequence_Step = 1;
    PowerSequence_Delay = 0x00;
}
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Custom_S4_S0_Trigger(void)
{
    if (SystemIsS0)
    {
        return;
    }
#if ENABLE_DEBUGGER_SUPPORT
    GPIO_Record();
    memset(PowSeq_Record, 0xff, 40 * sizeof(sPowSeq_Pin_Record));
#endif
    assert_print("Custom_S4_S0_Trigger");
    System_PowerState = SYSTEM_S4_S0;
    PowerSequence_Step = 1;
    PowerSequence_Delay = 0x00;
}
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Custom_S5_S0_Trigger(void)
{
    if (SystemIsS0)
    {
        return;
    }
#if ENABLE_DEBUGGER_SUPPORT
    GPIO_Record();
    memset(PowSeq_Record, 0xff, 40 * sizeof(sPowSeq_Pin_Record));
#endif
    dprint("Custom_S5_S0_Trigger\n");
    System_PowerState = SYSTEM_S5_S0;
    PowerSequence_Step = 1;
    PowerSequence_Delay = 0x00;
}
//----------------------------------------------------------------------------
// S5 to S0 sequnce
//----------------------------------------------------------------------------
void Custom_S5S0_Sequence(void)
{
    if (PowerSequence_Delay != 0x00)
    {
        PowerSequence_Delay--; // 1m Sec count base
        if (PowerSequence_Delay != 0x00)
        {
            return;
        }
    }
    while (PowerSequence_Step < (sizeof(PowerSequence_S5S0) / sizeof(sPowerSEQ)))
    {
        if (PowerSequence_S5S0[PowerSequence_Step].checkstatus == 0x00) // Do function
        {
            (PowerSequence_S5S0[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_S5S0[PowerSequence_Step].delay;
            PowerSequence_Step++;
            if (PowerSequence_Delay != 0x00)
            {
                break;
            }
        }
        else if (PowerSequence_S5S0[PowerSequence_Step].checkstatus == 0x01) // Check input status pin
        {
            if ((PowerSequence_S5S0[PowerSequence_Step].func)())
            {
                PowerSequence_Delay = PowerSequence_S5S0[PowerSequence_Step].delay;
                PowerSequence_Step++;
                PowerSequence_WaitTime = Wait_Time;
                if (PowerSequence_Delay != 0x00)
                {
                    break;
                }
            }
            else    //引脚输入不满足条件时的处理
            {
                PowerSequence_WaitTime--;
                if (PowerSequence_WaitTime == 0)
                {
                    dprint("S5->S0 Step:%d status error\n", PowerSequence_Step);
                    PowerSequence_WaitTime = Wait_Time;
#if (!Wait_until_Correct)
                    PowerSequence_Delay = PowerSequence_S5S0[PowerSequence_Step].delay;
                    PowerSequence_Step++;
                    if (PowerSequence_Delay != 0x00)
                    {
                        break;
                    }
#endif
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            (PowerSequence_S5S0[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_S5S0[PowerSequence_Step].delay;
            PowerSequence_Step++;
            if (PowerSequence_Delay != 0x00)
            {
                break;
            }
        }
    }
#if ENABLE_DEBUGGER_SUPPORT
    PowSeq_Record_Flag = D_PowerOn;
    if (PowerSequence_Step == ((sizeof(PowerSequence_S5S0) / sizeof(sPowerSEQ))))
    {
        Copy_Power_Sequence_Record_To_Sram(PowSeq_Record_Flag);
}
#endif
}
//----------------------------------------------------------------------------
// S4 to S0 sequnce
//----------------------------------------------------------------------------
void Custom_S4S0_Sequence(void)
{
    if (PowerSequence_Delay != 0x00)
    {
        PowerSequence_Delay--; // 1m Sec count base
        if (PowerSequence_Delay != 0x00)
        {
            return;
        }
    }
    while (PowerSequence_Step < (sizeof(PowerSequence_S4S0) / sizeof(sPowerSEQ)))
    {
        if (PowerSequence_S4S0[PowerSequence_Step].checkstatus == 0x00) // Do function
        {
            (PowerSequence_S4S0[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_S4S0[PowerSequence_Step].delay;
            PowerSequence_Step++;
            if (PowerSequence_Delay != 0x00)
            {
                break;
            }
        }
        else if (PowerSequence_S4S0[PowerSequence_Step].checkstatus == 0x01) // Check input status pin
        {
            if ((PowerSequence_S4S0[PowerSequence_Step].func)())
            {
                PowerSequence_Delay = PowerSequence_S4S0[PowerSequence_Step].delay;
                PowerSequence_Step++;
                PowerSequence_WaitTime = Wait_Time;
                if (PowerSequence_Delay != 0x00)
                {
                    break;
                }
            }
            else
            {
                /*引脚输入不满足条件时的处理*/
                PowerSequence_WaitTime--;
                if (PowerSequence_WaitTime == 0)
                {
                    dprint("S4->S0 Step:%d status error\n", PowerSequence_Step);
                    PowerSequence_WaitTime = Wait_Time;
#if (!Wait_until_Correct)
                    PowerSequence_Delay = PowerSequence_S4S0[PowerSequence_Step].delay;
                    PowerSequence_Step++;
                    if (PowerSequence_Delay != 0x00)
                    {
                        break;
                    }
#endif
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            (PowerSequence_S4S0[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_S4S0[PowerSequence_Step].delay;
            PowerSequence_Step++;
            if (PowerSequence_Delay != 0x00)
            {
                break;
            }
        }
    }
#if ENABLE_DEBUGGER_SUPPORT
    PowSeq_Record_Flag = D_HibernateWakeup;
    if (PowerSequence_Step == (sizeof(PowerSequence_S4S0) / sizeof(sPowerSEQ)))
    {
        Copy_Power_Sequence_Record_To_Sram(PowSeq_Record_Flag);
}
#endif
}
//----------------------------------------------------------------------------
// S3 to S0 sequnce
//----------------------------------------------------------------------------
void Custom_S3S0_Sequence(void)
{
    if (PowerSequence_Delay != 0x00)
    {
        PowerSequence_Delay--; // 1m Sec count base
        if (PowerSequence_Delay != 0x00)
        {
            return;
        }
    }
    while (PowerSequence_Step < (sizeof(PowerSequence_S3S0) / sizeof(sPowerSEQ)))
    {
        if (PowerSequence_S3S0[PowerSequence_Step].checkstatus == 0x00) // Do function
        {
            (PowerSequence_S3S0[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_S3S0[PowerSequence_Step].delay;
            PowerSequence_Step++;
            if (PowerSequence_Delay != 0x00)
            {
                break;
            }
        }
        else if (PowerSequence_S3S0[PowerSequence_Step].checkstatus == 0x01) // Check input status pin
        {
            if ((PowerSequence_S3S0[PowerSequence_Step].func)())
            {
                PowerSequence_Delay = PowerSequence_S3S0[PowerSequence_Step].delay;
                PowerSequence_Step++;
                PowerSequence_WaitTime = Wait_Time;
                if (PowerSequence_Delay != 0x00)
                {
                    break;
                }
            }
            else
            {
                /*引脚输入不满足条件时的处理*/
                PowerSequence_WaitTime--;
                if (PowerSequence_WaitTime == 0)
                {
                    dprint("S3->S0 Step:%d status error\n", PowerSequence_Step);
                    PowerSequence_WaitTime = Wait_Time;
#if (!Wait_until_Correct)
                    PowerSequence_Delay = PowerSequence_S3S0[PowerSequence_Step].delay;
                    PowerSequence_Step++;
                    if (PowerSequence_Delay != 0x00)
                    {
                        break;
                    }
#endif
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            (PowerSequence_S3S0[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_S3S0[PowerSequence_Step].delay;
            PowerSequence_Step++;
            if (PowerSequence_Delay != 0x00)
            {
                break;
            }
        }
    }
#if ENABLE_DEBUGGER_SUPPORT
    PowSeq_Record_Flag = D_SleepWakeup;
    if (PowerSequence_Step == (sizeof(PowerSequence_S3S0) / sizeof(sPowerSEQ)))
    {
        Copy_Power_Sequence_Record_To_Sram(PowSeq_Record_Flag);
}
#endif
}
//-----------------------------------------------------------------------------
// Check SUSB and SUSC
//-----------------------------------------------------------------------------
//------------------------------------------------------------------------------
// The function of checking slp s3
//------------------------------------------------------------------------------
void Sleep_S3_Monitor(void)
{
    CLEAR_MASK(CTRL_FLAG2, SLEEP_S3_ON);
    if (Read_SLPS3())
    {
        SET_MASK(CTRL_FLAG2, SLEEP_S3_ON);
    }
}
//------------------------------------------------------------------------------
// The function of checking slp s4
//------------------------------------------------------------------------------
void Sleep_S4_Monitor(void)
{
    CLEAR_MASK(CTRL_FLAG2, SLEEP_S4_ON);
    if (Read_SLPS4())
    {
        SET_MASK(CTRL_FLAG2, SLEEP_S4_ON);
    }
}
//------------------------------------------------------------------------------
// The function of checking slp s5
//------------------------------------------------------------------------------
void Sleep_S5_Monitor(void)
{
    CLEAR_MASK(CTRL_FLAG2, SLEEP_S5_ON);
    if (Read_SLPS5())
    {
        SET_MASK(CTRL_FLAG2, SLEEP_S5_ON);
    }
}
void Change_PowerState(void)
{
    BYTE presentstatus;
    presentstatus = (CTRL_FLAG2 & (SLEEP_S3_ON + SLEEP_S4_ON + SLEEP_S5_ON));
    if (PowerStatusBK != presentstatus) // To make sure all SLP is checked.
    {
        PowerStatusBK = presentstatus;
        return;
    }
    switch (CTRL_FLAG2 & (SLEEP_S3_ON + SLEEP_S4_ON + SLEEP_S5_ON))
    {
    case 0: // goto S5
        switch (System_PowerState)
        {
        case SYSTEM_S0:
            Custom_S0_S5_Trigger(SC_S0SLPOff);
            break;
        case SYSTEM_S4:
            break;
        case SYSTEM_S3:
            Custom_S0_S5_Trigger(SC_S3S4SLPOff);
            break;
        }
        break;
    case (SLEEP_S3_ON + SLEEP_S4_ON + SLEEP_S5_ON): // goto S0
        switch (System_PowerState)
        {
        case SYSTEM_S3:
            Custom_S3_S0_Trigger();
            break;
        case SYSTEM_S4:
            Custom_S4_S0_Trigger();
            break;
        case SYSTEM_S5:
            Custom_S5_S0_Trigger();
            break;
        }
        break;
    case (SLEEP_S4_ON + SLEEP_S5_ON): // goto S3
        switch (System_PowerState)
        {
        case SYSTEM_S0:
            Custom_S0_S3_Trigger(0x30);
            break;
        }
        break;
    case SLEEP_S5_ON: // goto S4
        switch (System_PowerState)
        {
        case SYSTEM_S0:
            Custom_S0_S4_Trigger(0x40);
            break;
        }
        break;
    }
}
//------------------------------------------------------------------------------
void PowerState_Monitor(void)
{
    switch (PowerState_Monitor_Index)
    {
    case 0:
        Sleep_S3_Monitor(); // Check SLP_S3#(or SUS_B#)
        PowerState_Monitor_Index++;
        break;
    case 1:
        Sleep_S4_Monitor(); // Check SLP_S4#
        PowerState_Monitor_Index++;
        break;
    case 2:
        Sleep_S5_Monitor(); // Check SLP_S5#(or SUS_C#)
        PowerState_Monitor_Index++;
        break;
    case 3:
        Change_PowerState();
        PowerState_Monitor_Index = 0x00;
        break;
    default:
        PowerState_Monitor_Index = 0x00;
        break;
    }
}
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Custom_S0_S3_Trigger(BYTE cause)
{
    if (SystemIsS3)
    {
        return;
    }
#if ENABLE_DEBUGGER_SUPPORT
    GPIO_Record();
    memset(PowSeq_Record, 0xff, 40 * sizeof(sPowSeq_Pin_Record));
#endif
    ShutDnCause = cause; // setup shutdown case
    System_PowerState = SYSTEM_S0_S3;
    PowerSequence_Step = 1;
    PowerSequence_Delay = 0x00;
}
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Custom_S0_S4_Trigger(BYTE cause)
{
    if (SystemIsS4)
    {
        return;
    }
#if ENABLE_DEBUGGER_SUPPORT
    GPIO_Record();
    memset(PowSeq_Record, 0xff, 40 * sizeof(sPowSeq_Pin_Record));
#endif
    ShutDnCause = cause; // setup shutdown case
    System_PowerState = SYSTEM_S0_S4;
    PowerSequence_Step = 1;
    PowerSequence_Delay = 0x00;
}
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Custom_S0_S5_Trigger(BYTE cause)
{
    if (SystemIsS5)
    {
        return;
    }
#if ENABLE_DEBUGGER_SUPPORT
    GPIO_Record();
    memset(PowSeq_Record, 0xff, 40 * sizeof(sPowSeq_Pin_Record));
#endif
    ShutDnCause = cause; // setup shutdown case
    System_PowerState = SYSTEM_S0_S5;
    PowerSequence_Step = 1;
    PowerSequence_Delay = 0x00;
}
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Custom_Reboot_Trigger(BYTE cause)
{
    if (SystemIsS5)
    {
        return;
    }
#if ENABLE_DEBUGGER_SUPPORT
    GPIO_Record();
    memset(PowSeq_Record, 0xff, 40 * sizeof(sPowSeq_Pin_Record));
#endif
    ShutDnCause = cause; // setup shutdown case
    System_PowerState = SYSTEM_REBOOT;
    PowerSequence_Step = 1;
    PowerSequence_Delay = 0x00;
}
//-----------------------------------------------------------------------------
// process POWER OFF sequence
// entry: 	1 power shutdown case
//			2 S5 off or not
// Shutdown cause:
//	1:	0x8A command of 66 port
// 	2:	0x8B command of 66 port
//	3:	4secs Overrided
// 	4:	HWPG low
//	5:	0x8C command of 66 port
// 	6:	0x8D command of 66 port
//	7:	temperature to high
// 	8:
//	9:	Init. thermal chip fail
// 	10:	Check SLP // S0 -> S4
//	11:	Check SLP // S3 -> S5
// 	12:	Check SLP // S0 -> S5
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// S0 to S3 sequnce
//----------------------------------------------------------------------------
void Custom_S0S3_Sequence(void)
{
    if (PowerSequence_Delay != 0x00)
    {
        PowerSequence_Delay--; // 1m Sec count base
        if (PowerSequence_Delay != 0x00)
        {
            return;
        }
    }
    while (PowerSequence_Step < (sizeof(PowerSequence_S0S3) / sizeof(sPowerSEQ)))
    {
        if (PowerSequence_S0S3[PowerSequence_Step].checkstatus == 0x00) // Do function
        {
            (PowerSequence_S0S3[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_S0S3[PowerSequence_Step].delay;
            PowerSequence_Step++;
            if (PowerSequence_Delay != 0x00)
            {
                break;
            }
        }
        else if (PowerSequence_S0S3[PowerSequence_Step].checkstatus == 0x01) // Check input status pin
        {
            if ((PowerSequence_S0S3[PowerSequence_Step].func)())
            {
                PowerSequence_Delay = PowerSequence_S0S3[PowerSequence_Step].delay;
                PowerSequence_Step++;
                PowerSequence_WaitTime = Wait_Time;
                if (PowerSequence_Delay != 0x00)
                {
                    break;
                }
            }
            else
            {
                /*引脚输入不满足条件时的处理*/
                PowerSequence_WaitTime--;
                if (PowerSequence_WaitTime == 0)
                {
                    dprint("S0->S3 Step:%d status error\n", PowerSequence_Step);
                    PowerSequence_WaitTime = Wait_Time;
#if (!Wait_until_Correct)
                    PowerSequence_Delay = PowerSequence_S0S3[PowerSequence_Step].delay;
                    PowerSequence_Step++;
                    if (PowerSequence_Delay != 0x00)
                    {
                        break;
                    }
#endif
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            (PowerSequence_S0S3[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_S0S3[PowerSequence_Step].delay;
            PowerSequence_Step++;
            if (PowerSequence_Delay != 0x00)
            {
                break;
            }
        }
    }
#if ENABLE_DEBUGGER_SUPPORT
    PowSeq_Record_Flag = D_Sleep;
    if (PowerSequence_Step == (sizeof(PowerSequence_S0S3) / sizeof(sPowerSEQ)))
    {
        Copy_Power_Sequence_Record_To_Sram(PowSeq_Record_Flag);
}
#endif
}
//----------------------------------------------------------------------------
// S0 to S4 sequnce
//----------------------------------------------------------------------------
void Custom_S0S4_Sequence(void)
{
    if (PowerSequence_Delay != 0x00)
    {
        PowerSequence_Delay--; // 1m Sec count base
        if (PowerSequence_Delay != 0x00)
        {
            return;
        }
    }
    while (PowerSequence_Step < (sizeof(PowerSequence_S0S4) / sizeof(sPowerSEQ)))
    {
        if (PowerSequence_S0S4[PowerSequence_Step].checkstatus == 0x00) // Do function
        {
            (PowerSequence_S0S4[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_S0S4[PowerSequence_Step].delay;
            PowerSequence_Step++;
            if (PowerSequence_Delay != 0x00)
            {
                break;
            }
        }
        else if (PowerSequence_S0S4[PowerSequence_Step].checkstatus == 0x01) // Check input status pin
        {
            if ((PowerSequence_S0S4[PowerSequence_Step].func)())
            {
                PowerSequence_Delay = PowerSequence_S0S4[PowerSequence_Step].delay;
                PowerSequence_Step++;
                PowerSequence_WaitTime = Wait_Time;
                if (PowerSequence_Delay != 0x00)
                {
                    break;
                }
            }
            else
            {
                /*引脚输入不满足条件时的处理*/
                PowerSequence_WaitTime--;
                if (PowerSequence_WaitTime == 0)
                {
                    dprint("S0->S4 Step:%d status error\n", PowerSequence_Step);
                    PowerSequence_WaitTime = Wait_Time;
#if (!Wait_until_Correct)
                    PowerSequence_Delay = PowerSequence_S0S4[PowerSequence_Step].delay;
                    PowerSequence_Step++;
                    if (PowerSequence_Delay != 0x00)
                    {
                        break;
                    }
#endif
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            (PowerSequence_S0S4[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_S0S4[PowerSequence_Step].delay;
            PowerSequence_Step++;
            if (PowerSequence_Delay != 0x00)
            {
                break;
            }
        }
    }
#if ENABLE_DEBUGGER_SUPPORT
    PowSeq_Record_Flag = D_Hibernate;
    if (PowerSequence_Step == (sizeof(PowerSequence_S0S4) / sizeof(sPowerSEQ)))
    {
        Copy_Power_Sequence_Record_To_Sram(PowSeq_Record_Flag);
}
#endif
}
//-----------------------------------------------------------------
// S0 to S5 sequnce
//-----------------------------------------------------------------
void Custom_S0S5_Sequence(void)
{
    if (PowerSequence_Delay != 0x00)
    {
        PowerSequence_Delay--; // 1m Sec count base
        if (PowerSequence_Delay != 0x00)
        {
            return;
        }
    }
    while (PowerSequence_Step < (sizeof(PowerSequence_S0S5) / sizeof(sPowerSEQ)))
    {
        if (PowerSequence_S0S5[PowerSequence_Step].checkstatus == 0x00) // Do function
        {
            (PowerSequence_S0S5[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_S0S5[PowerSequence_Step].delay;
            PowerSequence_Step++;
            if (PowerSequence_Delay != 0x00)
            {
                break;
            }
        }
        else if (PowerSequence_S0S5[PowerSequence_Step].checkstatus == 0x01) // Check input status pin
        {
            if ((PowerSequence_S0S5[PowerSequence_Step].func)())
            {
                PowerSequence_Delay = PowerSequence_S0S5[PowerSequence_Step].delay;
                PowerSequence_Step++;
                PowerSequence_WaitTime = Wait_Time;
                if (PowerSequence_Delay != 0x00)
                {
                    break;
                }
            }
            else
            {
                /*引脚输入不满足条件时的处理*/
                PowerSequence_WaitTime--;
                if (PowerSequence_WaitTime == 0)
                {
                    dprint("S0->S5 Step:%d status error\n", PowerSequence_Step);
                    PowerSequence_WaitTime = Wait_Time;
#if (!Wait_until_Correct)
                    PowerSequence_Delay = PowerSequence_S0S5[PowerSequence_Step].delay;
                    PowerSequence_Step++;
                    if (PowerSequence_Delay != 0x00)
                    {
                        break;
                    }
#endif
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            (PowerSequence_S0S5[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_S0S5[PowerSequence_Step].delay;
            PowerSequence_Step++;
            if (PowerSequence_Delay != 0x00)
            {
                break;
            }
        }
    }
#if ENABLE_DEBUGGER_SUPPORT
    PowSeq_Record_Flag = D_Shutdown;
    if (PowerSequence_Step == ((sizeof(PowerSequence_S0S5) / sizeof(sPowerSEQ))))
    {
        Copy_Power_Sequence_Record_To_Sram(PowSeq_Record_Flag);
}
#endif
}
void Custom_Reboot_Sequence(void)
{
    if (PowerSequence_Delay != 0x00)
    {
        PowerSequence_Delay--; // 1m Sec count base
        if (PowerSequence_Delay != 0x00)
        {
            return;
        }
    }
    while (PowerSequence_Step < (sizeof(PowerSequence_Reboot) / sizeof(sPowerSEQ)))
    {
        if (PowerSequence_Reboot[PowerSequence_Step].checkstatus == 0x00) // Do function
        {
            (PowerSequence_Reboot[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_Reboot[PowerSequence_Step].delay;
            PowerSequence_Step++;
            if (PowerSequence_Delay != 0x00)
            {
                break;
            }
        }
        else if (PowerSequence_Reboot[PowerSequence_Step].checkstatus == 0x01) // Check input status pin
        {
            if ((PowerSequence_Reboot[PowerSequence_Step].func)())
            {
                PowerSequence_Delay = PowerSequence_Reboot[PowerSequence_Step].delay;
                PowerSequence_Step++;
                PowerSequence_WaitTime = Wait_Time;
                if (PowerSequence_Delay != 0x00)
                {
                    break;
                }
            }
            else
            {
                /*引脚输入不满足条件时的处理*/
                PowerSequence_WaitTime--;
                if (PowerSequence_WaitTime == 0)
                {
                    dprint("Reboot Step:%d status error\n", PowerSequence_Step);
                    PowerSequence_WaitTime = Wait_Time;
#if (!Wait_until_Correct)
                    PowerSequence_Delay = PowerSequence_Reboot[PowerSequence_Step].delay;
                    PowerSequence_Step++;
                    if (PowerSequence_Delay != 0x00)
                    {
                        break;
                    }
#endif
                }
                else
                {
                    break;
                }
            }
        }
        else // Check input status pin
        {
            (PowerSequence_Reboot[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_Reboot[PowerSequence_Step].delay;
            PowerSequence_Step++;
            if (PowerSequence_Delay != 0x00)
            {
                break;
            }
        }
    }
#if ENABLE_DEBUGGER_SUPPORT
    PowSeq_Record_Flag = D_Reboot;
    if (PowerSequence_Step == ((sizeof(PowerSequence_Reboot) / sizeof(sPowerSEQ))))
    {
        Copy_Power_Sequence_Record_To_Sram(PowSeq_Record_Flag);
}
#endif
}
//-----------------------------------------------------------------------------
// The function of checking HWPG
//-----------------------------------------------------------------------------
void HWPG_Monitor(void)
{
    if (SystemIsS0)
    {
        if (!Read_HWPG())
        {
            HWPG_TIMER++;
            if (HWPG_TIMER > T_HWPG)
            {
                Custom_S0_S5_Trigger(SC_HWPS0Off);
                HWPG_TIMER = 0x00;
            }
        }
        else
        {
            HWPG_TIMER = 0x00;
        }
    }
}
//----------------------------------------------------------------------------
// EC auto power on function
//----------------------------------------------------------------------------
void Auto_PowerOn_Monitor(void)
{
    if (ECAutoPowerOn == 1)
    {
        ECAutoPowerOn = 0;
        DelayDoPowerSeq = 1000;
        Custom_S5_S0_Trigger();
    }
    if (ECWDTRSTNow)
    {
        ECWDTRSTNow = 0;
        System_PowerState = SYSTEM_EC_WDTRST;
    }
}
//-----------------------------------------------------------------------------
// Handle System Power Control
// Called by service_1mS -> Event_1ms -> Hook_1msEvent
//-----------------------------------------------------------------------------
void Sys_PowerState_Control(void)
{
    if (SystemIsS0)
    {
        // reserved
    }
    // Check if delay doing PowerSeq is needed.
    if (DelayDoPowerSeq != 0x00)
    {
        DelayDoPowerSeq--;
        if (DelayDoPowerSeq != 0x00)
        {
            return;
        }
    }
    /*if((SystemIsS0)||(SystemIsS3)||(SystemIsS4)||(SystemIsS5))
    {
        PowerState_Monitor();
    }*/
    // Check current System PowerState and do corresponding powersequence
    switch (System_PowerState)
    {
    case SYSTEM_S0:
        // HWPG_Monitor();
        break;
    case SYSTEM_S3:
        break;
    case SYSTEM_S4:
        break;
    case SYSTEM_S5:
        // Auto_PowerOn_Monitor();
        break;
    case SYSTEM_S4_S0:
        // dprint("Custom_S4S0_Sequence\n");
        Custom_S4S0_Sequence();
        break;
    case SYSTEM_S5_S0:
        // dprint("Custom_S5S0_Sequence\n");
        Custom_S5S0_Sequence();
        break;
    case SYSTEM_S3_S0:
        // dprint("Custom_S3S0_Sequence\n");
        Custom_S3S0_Sequence();
        break;
    case SYSTEM_S0_S3:
        // dprint("Custom_S0S3_Sequence\n");
        Custom_S0S3_Sequence();
        break;
    case SYSTEM_S0_S4:
        // dprint("Custom_S0S4_Sequence\n");
        Custom_S0S4_Sequence();
        break;
    case SYSTEM_S0_S5:
        //dprint("Custom_S0S5_Sequence\n");
        Custom_S0S5_Sequence();
        break;
    case SYSTEM_REBOOT:
        //dprint("Custom_Reboot_Sequence\n");
        Custom_Reboot_Sequence();
        break;
    case SYSTEM_EC_WDTRST:
        // InternalWDTNow();
        break;
    default:
        System_PowerState = SYSTEM_S5;
        break;
    }
}
void ResetSource_Monitor(void)
{
    switch (RSTStatus & 0x03)
    {
    case 0:
    case 1:
        if (GPCRA0 == 0x84)
        {
            ShutDnCause = SC_ECColdBoot;
        }
        else
        {
            ShutDnCause = SC_EC_0x0000;
        }
        break;
    case 2:
        if (GPCRA0 == 0x84)
        {
            ShutDnCause = SC_IntWatchDog;
        }
        else
        {
            ShutDnCause = SC_EC_0x0000;
        }
        break;
    case 3:
        if (GPCRA0 == 0x84)
        {
            ShutDnCause = SC_ExtWatchDog;
        }
        else
        {
            ShutDnCause = SC_EC_0x0000;
        }
        break;
    }
}
//-----------------------------------------------------------------------------
// The function of pulsing S.B. power button 100ms
//-----------------------------------------------------------------------------
void SB_PowerButton_Enable(void)
{
    DNBSWON_ON();
    SBSWReleaseCunt = 20; // decrease timer base is 10ms total time 200ms
}
//-----------------------------------------------------------------------------
// The function of releasing S.B. power button
//-----------------------------------------------------------------------------
void SB_PowerButton_Monitor(void)
{
    if (SBSWReleaseCunt != 0x00)
    {
        SBSWReleaseCunt--;
        if (SBSWReleaseCunt == 0x00)
        {
            DNBSWON_OFF();
        }
    }
}
/**
 * @brief POWER SWITCH初始化模块
 *
 * @param    timeout     timeout后产生power switch timeout 事件
 *           可选参数：   000：1000ms  001：4000ms  010：5000ms ... 111:12000ms
 * @param    mode        0:timeout后产生中断，不复位     1:timeout后直接复位
 *
 * @return   无
 */
#define PWRSW_WDTIME(timeout) ((timeout&0b111)<<1)/*102:1/4/5/6/7/8/10/12sec,103:TBT*2/8/10/12/14/16/20/24*/
#define PWRSW_INT BIT5//WDT Timeout IRQ status
#define PWRSW_DBTIMEL(dbtime) ((dbtime&0b11)<<6)/*102:disable/64/96/1000ms,103:disable/DBBT*4/6/64*/
#define PWRSW_RSTOEN 0x100/*GPIOC5(pin71) 500ms low value*/
#define PWRSW_DBTIMEL_64ms 0x40
#define PWRSW_DBTIMEL_96ms 0x80
#define PWRSW_DBTIMEL_1000ms 0xC0
#define PWRSW_INT_WDT 0x20
#define PWRSW_RSTMODE 0x10
#define PWRSW_WDTIME_12000ms 0x0E
#define PWRSW_WDTIME_10000ms 0x0C
#define PWRSW_WDTIME_8000ms 0x0A
#define PWRSW_WDTIME_7000ms 0x08
#define PWRSW_WDTIME_6000ms 0x06
#define PWRSW_WDTIME_5000ms 0x04
#define PWRSW_WDTIME_4000ms 0x02
#define PWRSW_WDTIME_1000ms 0x0
#define PWRSW_EN 0x1
#define PWRSW_TBT(tbt) ((tbt&0x3fff)<<9)/*Ttbt = ref_clock * (PWRSW_TBT + 1)*/
#define PWRSW_DBBT(dbbt) ((dbbt&0x1ff)<<23)/*Tdbbt = ref_clock * (PWRSW_DBBT + 1)*/
#define PWRSW_PIN_SEL 2
void PWRSW_Config(BYTE timeout, BYTE mode)
{
#if PWRSW_PIN_SEL==1
    sysctl_iomux_config(GPIOB, 4, 1); // 设置GPB4复用功能
#elif PWRSW_PIN_SEL==2
    sysctl_iomux_config(GPIOA, 11, 2);
#elif PWRSW_PIN_SEL==3
    sysctl_iomux_config(GPIOB, 17, 3);
#endif
    //sysctl_iomux_config(GPIOC, 5, 3); // 设置GPC5复用功能
#if (SYSCTL_CLOCK_EN)
    u_int32_t pwrswcsr = 0;
    pwrswcsr |= PWRSW_WDTIME(timeout) | PWRSW_EN;
    if (mode)
    {
        pwrswcsr |= PWRSW_RSTMODE;
    }
    pwrswcsr |= PWRSW_DBTIMEL(0x01); // 去抖64ms
    pwrswcsr |= PWRSW_RSTOEN;//GPIOC5输出500ms低电平
    pwrswcsr |= PWRSW_TBT(15999);//32k的16000分频（2Hz）
    pwrswcsr |= PWRSW_DBBT(511);//32k的512分频(62.5Hz)

    SYSCTL_PWRSWCSR = pwrswcsr;
#endif
}
//-----------------------------------------------------------------------------
// WDT timeout产生复位前令GPI5输出100ms长度的低电平(GPI5此功能优先级高于其他复用功能)，
// 因此WDT timeout时间将比 pwrsw_sel 设置长100ms
//-----------------------------------------------------------------------------
void PWRSW_Rstoutputen(void)
{
    sysctl_iomux_config(GPIOC, 5, 3); // 设置GPI5复用功能
#if (SYSCTL_CLOCK_EN)
    SYSCTL_PWRSWCSR |= 0x100;
#endif
}
//-----------------------------------------------------------------------------
// Function:检查固件更新后设置的重启标志
//-----------------------------------------------------------------------------
void AutoON_Check_AfterUpdate(void)
{
    if ((SHARE_RAM8(0xFFF) == 0xDC) && ((SHARE_RAM8(0xFFE) == 0xCD)))
    {
        SHARE_RAM8(0xFFE) = 0x0;
        SHARE_RAM8(0xFFF) = 0x0;
        Auto_On = 1;
    }
}
//-----------------------------------------------------------------------------
// Function:固件更新关机后后复位芯片动作
//-----------------------------------------------------------------------------
void CHIPRESET_Check_AfterUpdate(void)
{
    if (SYSCTL_CFG & BIT3)
    {
        SYSCTL_CFG &= (~BIT3);
        SYSCTL_RST1 |= 0x00010000;
        vDelayXms(5);
        SYSCTL_RST1 &= ~(0x00010000); // EC CHIP RESET
    }
}
//-----------------------------------------------------------------------------
// Function:固件更新后设置重启标志
//-----------------------------------------------------------------------------
BYTE Set_AutoON_AfterUpdate(void)
{
    if (SYSCTL_CFG & BIT3)
    {
        SYSCTL_CFG &= (~BIT3);
        SYSCTL_RST1 |= 0x00010000;
        vDelayXms(5);
        SYSCTL_RST1 &= ~(0x00010000); // EC CHIP RESET
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
// Function:系统状态由S0切换为其他状态时，需要将相关的变量清除
//-----------------------------------------------------------------------------
void PowerChange_Var_Clear(void)
{
    KBC_Variable_Clear(); // clear kbc variable
    Clear_Event_Buffer(); // clear qevent buffer
}

/*-----------------------------------------------------------------------------
 * @subroutine - Hook_EC_ACK_eSPI_Reset
 * @function - Hook_EC_ACK_eSPI_Reset
 * @upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 */
void Hook_EC_ACK_eSPI_Reset(void)
{


}
