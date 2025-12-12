/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-10-10 17:27:10
 * @Description: This file is used for LED custom
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
#include "CUSTOM_LED.H"
#include "CUSTOM_POWER.H"
#include "KERNEL_MEMORY.H"
/* ----------------------------------------------------------------------------
 * FUNCTION: OEM_Write_Leds
 *
 * Write to SCROLL LOCK, NUM LOCK, CAPS LOCK, and any custom LEDs (including
 * an "Fn" key LED).
 *
 * Input: data (LED_ON bit) = Data to write to the LEDs
 *         bit 0 - SCROLL, Scroll Lock
 *         bit 1 - NUM,    Num Lock
 *         bit 2 - CAPS,   Caps Lock
 *         bit 3 - OVL,    Fn LED
 *         bit 4 - LED4,   Undefined
 *         bit 5 - LED5,   Undefined
 *         bit 6 - LED6,   Undefined
 *         bit 7 - LED7,   Undefined
 *
 * Updates Led_Data variable and calls send_leds if appropriate.
 * ------------------------------------------------------------------------- */
void SCROLED_ON()
{}
void SCROLED_OFF()
{}
void NUMLED_ON()
{}
void NUMLED_OFF()
{}
void CAPLED_ON()
{}
void CAPLED_OFF()
{}
void OEM_Write_Leds(void)
{
#if SUPPORT_LED_SCROLLOCK
    if (Led_Data_SCROLL) // Check ScrollLock LED Status
    {
        SCROLED_ON();
    }
    else
    {
        SCROLED_OFF();
    }
#endif
#if SUPPORT_LED_NUMLOCK
    if (Led_Data_NUM) // Check NumLock LED Status
    {
        NUMLED_ON();
    }
    else
    {
        NUMLED_OFF();
    }
#endif
#if SUPPORT_LED_CAPSLOCK
    if (Led_Data_CAPS) // Check CapsLock LED Status
    {
        CAPLED_ON();
    }
    else
    {
        CAPLED_OFF();
    }
#endif
}
void PWRLED1_ON()
{}
void PWRLED1_TG()
{}
int CARD_LED_ON()
{
    return 0;
}
int CARD_LED_OFF()
{
    return 0;
}
int STATE_CARD()
{
    return 0;
}
int BATLED1_TG()
{
    return 0;
}
void Service_LED_Indicator(void)
{
    if (System_PowerState == SYSTEM_S5)
    {
        return;
    }
    if (ACPI_STATE_S0)
    {
        PWRLED1_ON();
    }
    else if (System_PowerState == SYSTEM_S3)
    {
        PWRLED1_TG();
    }
    if (STATE_CARD())
    {
        CARD_LED_OFF(); // Hi->No Card
    }
    else
    {
        CARD_LED_ON();
    }
    // BATLED0_TG();   //BLUE COLOR
    BATLED1_TG(); // YELLOW COLOR
}
//*****************************************************************************
//
//  Breathing LED
//
//  parameter :
//          none
//
//  return :
//          Program operation condition
//			 0 : normal
//
//	note :
//*****************************************************************************
BYTE Breathing_LED_Switch(BYTE channel, BYTE step)
{
    static char dir = 0x1;
    register BYTE offset = 0;
    /* 占空比 */
    offset = PWM_DCR0_1_OFFSET + ((channel & 0x06));
    /**/
    register BYTE dcr = PWM_REG(offset) & (0xFF << ((channel & 0x1) * 8));
    if (dcr > 95)
    {
        dir = -1;
    }
    else if (dcr < 5)
    {
        dir = 1;
    }
    dcr += step * dir;
    PWM_REG(offset) = (dcr << ((channel & 0x1) * 8));
    return 0;
}

//-----------------------------------------------------------------------------
// Oem LED Here
//-----------------------------------------------------------------------------
/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/
