/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-06-04 16:58:33
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
#ifndef CUSTOM_GPIO_H
#define CUSTOM_GPIO_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#define HIGH      1
#define LOW       0
 /*-----------------------------------------------------------------------------
  * External Calling Process
  *---------------------------------------------------------------------------*/
  //-----------------------------------------------------------------------------
  // GPIO A[7:0] Registers define GPA[7:0]
  //------------------------------------------------------------------------------
                          //76543210
#define PinA0_7_Init      0b00000000            // GPIO A[7:0] data register OUTPUT init
#define PinA0_7_InOut     0b00000000            // GPIO A[7:0] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinA0_7_PullUp    0b00000000            // GPIO A[7:0] IO Pull Up control register init.0 is not PULLUP,1 is PULLUP.
                          // 7 6 5 4 3 2 1 0
#define PinA0_7_IoMux     0b0000000000000000    // GPIO A[7:0] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
//------------------------------------------------------------------------------
// GPIO A[15:8] Registers define GPB[7:0]
//------------------------------------------------------------------------------
                          //76543210
#define PinA8_15_Init     0b00000000            // GPIO A[15:8] data register OUTPUT init
#define PinA8_15_InOut    0b00000000            // GPIO A[15:8] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinA8_15_PullUp   0b00000000            // GPIO A[15:8] IO Pull Up control register init.0 is not PULLUP,1 is PULLUP.
                          // 7 6 5 4 3 2 1 0
#define PinA8_15_IoMux    0b0000000000000000    // GPIO A[15:8] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
//------------------------------------------------------------------------------
// GPIO A[23:16] Registers define GPC[7:0]
//------------------------------------------------------------------------------
                          //76543210
#define PinA16_23_Init    0b00000000            // GPIO A[23:16] data register OUTPUT init
#define PinA16_23_InOut   0b00000000            // GPIO A[23:16] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinA16_23_PullUp  0b00000000            // GPIO A[23:16] IO Pull Up control register init.0 is not PULLUP,1 is PULLUP.
                          // 7 6 5 4 3 2 1 0
#define PinA16_23_IoMux   0b0000000000000000    // GPIO A[23:16] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
//------------------------------------------------------------------------------
// GPIO A[31:24] Registers define GPD[7:0]
//------------------------------------------------------------------------------
                          //76543210
#define PinA24_31_Init    0b00000000            // GPIO A[31:24] data register OUTPUT init
#define PinA24_31_InOut   0b00000000            // GPIO A[31:24] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinA24_31_PullUp	0b00000000            // GPIO A[31:24] data direction register init.0 is INPUT,1 is OUTPUT.
                          // 7 6 5 4 3 2 1 0
#define PinA24_31_IoMux   0b0000000000010000    // GPIO A[31:24] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
//------------------------------------------------------------------------------
// GPIO B[7:0] Registers define GPE[7:0]
//------------------------------------------------------------------------------
                          //76543210
#define PinB0_7_Init      0b00000000            // GPIO B[7:0] data register OUTPUT init
#define PinB0_7_InOut     0b00000000            // GPIO B[7:0] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinB0_7_PullUp    0b00000000            // GPIO B[7:0] IO Pull Up control register init.0 is not PULLUP,1 is PULLUP.
                          // 7 6 5 4 3 2 1 0
#define PinB0_7_IoMux     0b0000000000000000    // GPIO B[7:0] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
//------------------------------------------------------------------------------
// GPIO B[15:8] Registers define GPF[7:0]
//------------------------------------------------------------------------------
                          //76543210
#define PinB8_15_Init     0b00000000            // GPIO B[15:8] data register OUTPUT init
#define PinB8_15_InOut    0b00000000            // GPIO B[15:8] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinB8_15_PullUp   0b00000000            // GPIO B[15:8] IO Pull Up control register init.0 is not PULLUP,1 is PULLUP.
                          // 7 6 5 4 3 2 1 0
#define PinB8_15_IoMux    0b0000000000000000    // GPIO B[15:8] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
//------------------------------------------------------------------------------
// GPIO B[23:16] Registers define GPG[7:0]or (GPG[3:0] and GPH[3:0])
//------------------------------------------------------------------------------
                          //76543210
#define PinB16_23_Init    0b00000000          // GPIO B[23:16] data register OUTPUT init
#define PinB16_23_InOut   0b00000000          // GPIO B[23:16] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinB16_23_PullUp  0b00000000          // GPIO B[23:16] IO Pull Up control register init.0 is not PULLUP,1 is PULLUP.

                          // 7 6 5 4 3 2 1 0
#define PinB16_23_IoMux   0b0101010100000011  // GPIO B[23:16] IO multiplex control register init. refer to 'pin_iocfg.xlsx'

//------------------------------------------------------------------------------
// GPIO B[31:24] Registers define GPH[7:0] 
//------------------------------------------------------------------------------
                          //76543210
#define PinB24_31_Init    0b00000000             // GPIO B[26:24] data register OUTPUT init
#define PinB24_31_InOut   0b00000000             // GPIO B[26:24] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinB24_31_PullUp  0b00000000             // GPIO B[26:24] IO Pull Up control register init.0 is not PULLUP,1 is PULLUP.
                          // 7 6 5 4 3 2 1 0
#define PinB24_31_IoMux   0b0000000000000000     // GPIO B[26:24] IO multiplex control register init. refer to 'pin_iocfg.xlsx'

//------------------------------------------------------------------------------
// GPIO C[7:0] Registers define GPI[7:0]
//------------------------------------------------------------------------------
                          //76543210
#define PinC0_7_Init      0b00000000            // GPIO C[7:0] data register OUTPUT init
#define PinC0_7_InOut     0b00000000            // GPIO C[7:0] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinC0_7_PullUp    0b00000000            // GPIO C[7:0] IO Pull Up control register init.0 is not PULLUP,1 is PULLUP.
                          // 7 6 5 4 3 2 1 0
#define PinC0_7_IoMux     0b0000000000000000          // GPIO C[7:0] IO multiplex control register init. refer to 'pin_iocfg.xlsx'

//------------------------------------------------------------------------------
// GPIO C[15:8] Registers define GPJ[7:0]
//------------------------------------------------------------------------------
                          //76543210
#define PinC8_15_Init     0b00000000        // GPIO C[13:8] data register OUTPUT init
#define PinC8_15_InOut    0b00000000        // GPIO C[13:8] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinC8_15_PullUp   0b00000000        // GPIO C[13:8] IO Pull Up control register init.0 is not PULLUP,1 is PULLUP.
                          // 7 6 5 4 3 2 1 0
#define PinC8_15_IoMux    0b0000000000000000      // GPIO C[13:8] IO multiplex control register init. refer to 'pin_iocfg.xlsx'

//------------------------------------------------------------------------------
// GPIO D[7:0] Registers define GPM[7:0]
//------------------------------------------------------------------------------
                          //76543210
#define PinD0_7_Init      0b00000000        // GPIO D[7:0] data register OUTPUT init
#define PinD0_7_InOut     0b00000000        // GPIO D[7:0] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinD0_7_PullUp    0b00000000          // GPIO D[7:0] IO Pull Up control register init. And bit7, 0 is "Gph0 or Gph1 selects uartb function.", 1 is "Gph0 or Gph1 selects SMCLK3 function."
                          // 7 6 5 4 3 2 1 0
#define PinD0_7_IoMux     0b0001010101010101      // GPIO D[7:0] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
//------------------------------------------------------------------------------
// GPIO D[8] Registers define GPM[8]
//------------------------------------------------------------------------------
                          //0
#define PinD8_Init        0b0        // GPIO D[8] data register OUTPUT init
#define PinD8_InOut       0b0        // GPIO D[8] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinD8_PullUp      0b0        // GPIO D[8] IO Pull Up control register init. And bit7, 0 is "Gph0 or Gph1 selects uartb function.", 1 is "Gph0 or Gph1 selects SMCLK3 function."
                          // 0
#define PinD8_IoMux       0b00      // GPIO D[8] IO multiplex control register init. refer to 'pin_iocfg.xlsx'

//------------------------------------------------------------------------------
// GPIO E[7:0] Registers define GPN[7:0]
//------------------------------------------------------------------------------
                          //76543210
#define PinE0_7_Init      0b00000000         // GPIO E[7:0] data register OUTPUT init
#define PinE0_7_InOut     0b00000000         // GPIO E[7:0] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinE0_7_PullUp    0b00000000          // GPIO E[7:0] IO Pull Up control register init. And bit7, 0 is "Gph0 or Gph1 selects uartb function.", 1 is "Gph0 or Gph1 selects SMCLK3 function."
//------------------------------------------------------------------------------
// GPIO E[15:8] Registers define GPO[7:0]
//------------------------------------------------------------------------------
                          //76543210
#define PinE8_15_Init     0b00000000        // GPIO E[15:8] data register OUTPUT init
#define PinE8_15_InOut    0b00000000        // GPIO E[15:8] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinE8_15_PullUp   0b00000000         // GPIO E[15:8] IO Pull Up control register init. And bit7, 0 is "Gph0 or Gph1 selects uartb function.", 1 is "Gph0 or Gph1 selects SMCLK3 function."
//------------------------------------------------------------------------------
// GPIO E[23:16] Registers define GPP[7:0]
//------------------------------------------------------------------------------
                          //76543210
#define PinE16_23_Init    0b00000000        // GPIO E[23:16] data register OUTPUT init
#define PinE16_23_InOut   0b00000000        // GPIO E[23:16] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinE16_23_PullUp  0b00000000         // GPIO E[23:16] IO Pull Up control register init. And bit7, 0 is "Gph0 or Gph1 selects uartb function.", 1 is "Gph0 or Gph1 selects SMCLK3 function."
                          // 0
#define PinE0_23_IoMux    0b01          // GPIO E[7:0] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
/*-----------------------------------------------------------------------------
 * Special define
 *---------------------------------------------------------------------------*/
#define PinA0_15_IoMux  ((PinA8_15_IoMux<<16)+PinA0_7_IoMux)
#define PinA16_31_IoMux ((PinA24_31_IoMux<<16)+PinA16_23_IoMux)
#define PinB0_15_IoMux  ((PinB8_15_IoMux<<16)+PinB0_7_IoMux)

#define PinB16_31_IoMux ((PinB24_31_IoMux<<16)+PinB16_23_IoMux)
#define PinC0_15_IoMux  ((PinC8_15_IoMux<<16)+PinC0_7_IoMux)
#define PinDE_IoMux     ((PinE0_23_IoMux<<18)+(PinD8_IoMux<<16)+PinD0_7_IoMux)

#define PinA0_31_PullUp     ((PinA24_31_PullUp<<24)+(PinA16_23_PullUp<<16)+(PinA8_15_PullUp<<8)+PinA0_7_PullUp)
#define PinB0_31_PullUp     ((PinB24_31_PullUp<<24)+(PinB16_23_PullUp<<16)+(PinB8_15_PullUp<<8)+PinB0_7_PullUp)
#define PinCD_PullUp        ((PinD8_PullUp<<24)+(PinD0_7_PullUp<<16)+(PinC8_15_PullUp<<8)+PinC0_7_PullUp)
#define PinE0_23_PullUp     ((PinE16_23_PullUp<<16)+(PinE8_15_PullUp<<8)+PinE0_7_PullUp)


#define GPIO_INPUT_VAL  (0x00)
#define GPIO_INPUT_EN   (0x04)
#define GPIO_OUTPUT_EN  (0x08)
#define GPIO_OUTPUT_VAL (0x0C)
#define GPIO_PULLUP_EN  (0x10)
#define GPIO_DRIVE      (0x14)
#define GPIO_RISE_IE    (0x18)
#define GPIO_RISE_IP    (0x1C)
#define GPIO_FALL_IE    (0x20)
#define GPIO_FALL_IP    (0x24)
#define GPIO_HIGH_IE    (0x28)
#define GPIO_HIGH_IP    (0x2C)
#define GPIO_LOW_IE     (0x30)
#define GPIO_LOW_IP     (0x34)
#define GPIO_IOF_EN     (0x38)
#define GPIO_IOF_SEL    (0x3C)
#define GPIO_DEBOUNCE_EN   (0x48)
#define GPIO_OUTPUT_XOR    (0x40)
 /*-----------------------------------------------------------------------------
  * Parameter Definition
  *---------------------------------------------------------------------------*/
#define PALT            0x00
#define INPUT           0x80
#define OUTPUT          0x40
#define NO_PULL         0x00
#define PULL_UP         0x01
#define Driving4mA      0x00    //4mA
#define Driving8mA      0x10    //8mA
#define Driving12mA     0x18    //12mA
#define MaxDriving      0x38    //16mA
  //-----------------------------------------------------------------
  // GPIO Active
  //-----------------------------------------------------------------
#define GPin0           bit0
#define GPin1           bit1
#define GPin2           bit2
#define GPin3           bit3
#define GPin4           bit4
#define GPin5           bit5
#define GPin6           bit6
#define GPin7           bit7
#define HighActive      1
#define LowActive       0

 /*-----------------------------------------------------------------------------
  * power sequence definition example
  *---------------------------------------------------------------------------*/
#define ALW_ON()		SET_MASK(GPIO0_DR0,BIT(7))
#define ALW_OFF()		CLEAR_MASK(GPIO0_DR0,BIT(7))
#define HOT_LED_H()		SET_MASK(GPIO0_DR3,BIT(7))
#define HOT_LED_L()		CLEAR_MASK(GPIO2_DR0,BIT(0))
#define Read_ALW_PWRGD()		IS_MASK_SET(GPIO0_EXT0,BIT(2))
#define S3OK_H()		SET_MASK(GPIO0_DR0,BIT(4))
#define S3OK_L()		CLEAR_MASK(GPIO0_DR0,BIT(4))
#define P1V8_EN_H()				SET_MASK(GPIO0_DR1,BIT(2))
#define P1V8_EN_L()				CLEAR_MASK(GPIO0_DR1,BIT(2))
#define Read_SLPS3()	IS_MASK_SET(GPIO1_EXT0,BIT(5))
#define Read_SLPS4()	IS_MASK_SET(GPIO0_EXT2,BIT(4))
#define Read_SLPS5()	IS_MASK_SET(GPIO0_EXT1,BIT(1))
#define Read_HWPG()		IS_MASK_SET(GPIO0_EXT2,BIT(6))
#define DNBSWON_ON()	CLEAR_MASK(GPIO2_DR1,BIT(3))
#define DNBSWON_OFF()	SET_MASK(GPIO2_DR1,BIT(3))
#define Read_PWR_SW()		IS_MASK_CLEAR(GPIO1_EXT0,BIT(4))
#define Read_BC_ACOK()		IS_MASK_SET(GPIO0_EXT0,BIT(6))
#define Read_BATT_PRS_1()			IS_MASK_CLEAR(GPIO1_DR3,BIT(5))
#define CE_ON()		  
#define CE_OFF()
 /*-----------------------------------------------------------------------------
  * GPIO引脚输出高低定义
  *---------------------------------------------------------------------------*/
// GPIOA0 to GPIOA7 
#define GPIOA0(x) (x) ? SET_MASK(GPIO0_DR0, BIT(0)) : CLEAR_MASK(GPIO0_DR0, BIT(0))
#define GPIOA1(x) (x) ? SET_MASK(GPIO0_DR0, BIT(1)) : CLEAR_MASK(GPIO0_DR0, BIT(1))
#define GPIOA2(x) (x) ? SET_MASK(GPIO0_DR0, BIT(2)) : CLEAR_MASK(GPIO0_DR0, BIT(2))
#define GPIOA3(x) (x) ? SET_MASK(GPIO0_DR0, BIT(3)) : CLEAR_MASK(GPIO0_DR0, BIT(3))
#define GPIOA4(x) (x) ? SET_MASK(GPIO0_DR0, BIT(4)) : CLEAR_MASK(GPIO0_DR0, BIT(4))
#define GPIOA5(x) (x) ? SET_MASK(GPIO0_DR0, BIT(5)) : CLEAR_MASK(GPIO0_DR0, BIT(5))
#define GPIOA6(x) (x) ? SET_MASK(GPIO0_DR0, BIT(6)) : CLEAR_MASK(GPIO0_DR0, BIT(6))
#define GPIOA7(x) (x) ? SET_MASK(GPIO0_DR0, BIT(7)) : CLEAR_MASK(GPIO0_DR0, BIT(7))

// GPIOA8 to GPIOA15 
#define GPIOA8(x)  (x) ? SET_MASK(GPIO0_DR1, BIT(0)) : CLEAR_MASK(GPIO0_DR1, BIT(0))
#define GPIOA9(x)  (x) ? SET_MASK(GPIO0_DR1, BIT(1)) : CLEAR_MASK(GPIO0_DR1, BIT(1))
#define GPIOA10(x) (x) ? SET_MASK(GPIO0_DR1, BIT(2)) : CLEAR_MASK(GPIO0_DR1, BIT(2))
#define GPIOA11(x) (x) ? SET_MASK(GPIO0_DR1, BIT(3)) : CLEAR_MASK(GPIO0_DR1, BIT(3))
#define GPIOA12(x) (x) ? SET_MASK(GPIO0_DR1, BIT(4)) : CLEAR_MASK(GPIO0_DR1, BIT(4))
#define GPIOA13(x) (x) ? SET_MASK(GPIO0_DR1, BIT(5)) : CLEAR_MASK(GPIO0_DR1, BIT(5))
#define GPIOA14(x) (x) ? SET_MASK(GPIO0_DR1, BIT(6)) : CLEAR_MASK(GPIO0_DR1, BIT(6))
#define GPIOA15(x) (x) ? SET_MASK(GPIO0_DR1, BIT(7)) : CLEAR_MASK(GPIO0_DR1, BIT(7))

// GPIOA16 to GPIOA23 
#define GPIOA16(x) (x) ? SET_MASK(GPIO0_DR2, BIT(0)) : CLEAR_MASK(GPIO0_DR2, BIT(0))
#define GPIOA17(x) (x) ? SET_MASK(GPIO0_DR2, BIT(1)) : CLEAR_MASK(GPIO0_DR2, BIT(1))
#define GPIOA18(x) (x) ? SET_MASK(GPIO0_DR2, BIT(2)) : CLEAR_MASK(GPIO0_DR2, BIT(2))
#define GPIOA19(x) (x) ? SET_MASK(GPIO0_DR2, BIT(3)) : CLEAR_MASK(GPIO0_DR2, BIT(3))
#define GPIOA20(x) (x) ? SET_MASK(GPIO0_DR2, BIT(4)) : CLEAR_MASK(GPIO0_DR2, BIT(4))
#define GPIOA21(x) (x) ? SET_MASK(GPIO0_DR2, BIT(5)) : CLEAR_MASK(GPIO0_DR2, BIT(5))
#define GPIOA22(x) (x) ? SET_MASK(GPIO0_DR2, BIT(6)) : CLEAR_MASK(GPIO0_DR2, BIT(6))
#define GPIOA23(x) (x) ? SET_MASK(GPIO0_DR2, BIT(7)) : CLEAR_MASK(GPIO0_DR2, BIT(7))

// GPIOA24 to GPIOA31 
#define GPIOA24(x) (x) ? SET_MASK(GPIO0_DR3, BIT(0)) : CLEAR_MASK(GPIO0_DR3, BIT(0))
#define GPIOA25(x) (x) ? SET_MASK(GPIO0_DR3, BIT(1)) : CLEAR_MASK(GPIO0_DR3, BIT(1))
#define GPIOA26(x) (x) ? SET_MASK(GPIO0_DR3, BIT(2)) : CLEAR_MASK(GPIO0_DR3, BIT(2))
#define GPIOA27(x) (x) ? SET_MASK(GPIO0_DR3, BIT(3)) : CLEAR_MASK(GPIO0_DR3, BIT(3))
#define GPIOA28(x) (x) ? SET_MASK(GPIO0_DR3, BIT(4)) : CLEAR_MASK(GPIO0_DR3, BIT(4))
#define GPIOA29(x) (x) ? SET_MASK(GPIO0_DR3, BIT(5)) : CLEAR_MASK(GPIO0_DR3, BIT(5))
#define GPIOA30(x) (x) ? SET_MASK(GPIO0_DR3, BIT(6)) : CLEAR_MASK(GPIO0_DR3, BIT(6))
#define GPIOA31(x) (x) ? SET_MASK(GPIO0_DR3, BIT(7)) : CLEAR_MASK(GPIO0_DR3, BIT(7))

// GPIOB0 to GPIOB7 
#define GPIOB0(x) (x) ? SET_MASK(GPIO1_DR0, BIT(0)) : CLEAR_MASK(GPIO1_DR0, BIT(0))
#define GPIOB1(x) (x) ? SET_MASK(GPIO1_DR0, BIT(1)) : CLEAR_MASK(GPIO1_DR0, BIT(1))
#define GPIOB2(x) (x) ? SET_MASK(GPIO1_DR0, BIT(2)) : CLEAR_MASK(GPIO1_DR0, BIT(2))
#define GPIOB3(x) (x) ? SET_MASK(GPIO1_DR0, BIT(3)) : CLEAR_MASK(GPIO1_DR0, BIT(3))
#define GPIOB4(x) (x) ? SET_MASK(GPIO1_DR0, BIT(4)) : CLEAR_MASK(GPIO1_DR0, BIT(4))
#define GPIOB5(x) (x) ? SET_MASK(GPIO1_DR0, BIT(5)) : CLEAR_MASK(GPIO1_DR0, BIT(5))
#define GPIOB6(x) (x) ? SET_MASK(GPIO1_DR0, BIT(6)) : CLEAR_MASK(GPIO1_DR0, BIT(6))
#define GPIOB7(x) (x) ? SET_MASK(GPIO1_DR0, BIT(7)) : CLEAR_MASK(GPIO1_DR0, BIT(7))

// GPIOB8 to GPIOB15 
#define GPIOB8(x)  (x) ? SET_MASK(GPIO1_DR1, BIT(0)) : CLEAR_MASK(GPIO1_DR1, BIT(0))
#define GPIOB9(x)  (x) ? SET_MASK(GPIO1_DR1, BIT(1)) : CLEAR_MASK(GPIO1_DR1, BIT(1))
#define GPIOB10(x) (x) ? SET_MASK(GPIO1_DR1, BIT(2)) : CLEAR_MASK(GPIO1_DR1, BIT(2))
#define GPIOB11(x) (x) ? SET_MASK(GPIO1_DR1, BIT(3)) : CLEAR_MASK(GPIO1_DR1, BIT(3))
#define GPIOB12(x) (x) ? SET_MASK(GPIO1_DR1, BIT(4)) : CLEAR_MASK(GPIO1_DR1, BIT(4))
#define GPIOB13(x) (x) ? SET_MASK(GPIO1_DR1, BIT(5)) : CLEAR_MASK(GPIO1_DR1, BIT(5))
#define GPIOB14(x) (x) ? SET_MASK(GPIO1_DR1, BIT(6)) : CLEAR_MASK(GPIO1_DR1, BIT(6))
#define GPIOB15(x) (x) ? SET_MASK(GPIO1_DR1, BIT(7)) : CLEAR_MASK(GPIO1_DR1, BIT(7))

// GPIOB16 to GPIOB23 
#define GPIOB16(x) (x) ? SET_MASK(GPIO1_DR2, BIT(0)) : CLEAR_MASK(GPIO1_DR2, BIT(0))
#define GPIOB17(x) (x) ? SET_MASK(GPIO1_DR2, BIT(1)) : CLEAR_MASK(GPIO1_DR2, BIT(1))
#define GPIOB18(x) (x) ? SET_MASK(GPIO1_DR2, BIT(2)) : CLEAR_MASK(GPIO1_DR2, BIT(2))
#define GPIOB19(x) (x) ? SET_MASK(GPIO1_DR2, BIT(3)) : CLEAR_MASK(GPIO1_DR2, BIT(3))
#define GPIOB20(x) (x) ? SET_MASK(GPIO1_DR2, BIT(4)) : CLEAR_MASK(GPIO1_DR2, BIT(4))
#define GPIOB21(x) (x) ? SET_MASK(GPIO1_DR2, BIT(5)) : CLEAR_MASK(GPIO1_DR2, BIT(5))
#define GPIOB22(x) (x) ? SET_MASK(GPIO1_DR2, BIT(6)) : CLEAR_MASK(GPIO1_DR2, BIT(6))
#define GPIOB23(x) (x) ? SET_MASK(GPIO1_DR2, BIT(7)) : CLEAR_MASK(GPIO1_DR2, BIT(7))

// GPIOB24 to GPIOB31 
#define GPIOB24(x) (x) ? SET_MASK(GPIO1_DR3, BIT(0)) : CLEAR_MASK(GPIO1_DR3, BIT(0))
#define GPIOB25(x) (x) ? SET_MASK(GPIO1_DR3, BIT(1)) : CLEAR_MASK(GPIO1_DR3, BIT(1))
#define GPIOB26(x) (x) ? SET_MASK(GPIO1_DR3, BIT(2)) : CLEAR_MASK(GPIO1_DR3, BIT(2))
#define GPIOB27(x) (x) ? SET_MASK(GPIO1_DR3, BIT(3)) : CLEAR_MASK(GPIO1_DR3, BIT(3))
#define GPIOB28(x) (x) ? SET_MASK(GPIO1_DR3, BIT(4)) : CLEAR_MASK(GPIO1_DR3, BIT(4))
#define GPIOB29(x) (x) ? SET_MASK(GPIO1_DR3, BIT(5)) : CLEAR_MASK(GPIO1_DR3, BIT(5))
#define GPIOB30(x) (x) ? SET_MASK(GPIO1_DR3, BIT(6)) : CLEAR_MASK(GPIO1_DR3, BIT(6))
#define GPIOB31(x) (x) ? SET_MASK(GPIO1_DR3, BIT(7)) : CLEAR_MASK(GPIO1_DR3, BIT(7))
// GPIOC0 to GPIOC7 
#define GPIOC0(x) (x) ? SET_MASK(GPIO2_DR0, BIT(0)) : CLEAR_MASK(GPIO2_DR0, BIT(0))
#define GPIOC1(x) (x) ? SET_MASK(GPIO2_DR0, BIT(1)) : CLEAR_MASK(GPIO2_DR0, BIT(1))
#define GPIOC2(x) (x) ? SET_MASK(GPIO2_DR0, BIT(2)) : CLEAR_MASK(GPIO2_DR0, BIT(2))
#define GPIOC3(x) (x) ? SET_MASK(GPIO2_DR0, BIT(3)) : CLEAR_MASK(GPIO2_DR0, BIT(3))
#define GPIOC4(x) (x) ? SET_MASK(GPIO2_DR0, BIT(4)) : CLEAR_MASK(GPIO2_DR0, BIT(4))
#define GPIOC5(x) (x) ? SET_MASK(GPIO2_DR0, BIT(5)) : CLEAR_MASK(GPIO2_DR0, BIT(5))
#define GPIOC6(x) (x) ? SET_MASK(GPIO2_DR0, BIT(6)) : CLEAR_MASK(GPIO2_DR0, BIT(6))
#define GPIOC7(x) (x) ? SET_MASK(GPIO2_DR0, BIT(7)) : CLEAR_MASK(GPIO2_DR0, BIT(7))

// GPIOC8 to GPIOC15 
#define GPIOC8(x)  (x) ? SET_MASK(GPIO2_DR1, BIT(0)) : CLEAR_MASK(GPIO2_DR1, BIT(0))
#define GPIOC9(x)  (x) ? SET_MASK(GPIO2_DR1, BIT(1)) : CLEAR_MASK(GPIO2_DR1, BIT(1))
#define GPIOC10(x) (x) ? SET_MASK(GPIO2_DR1, BIT(2)) : CLEAR_MASK(GPIO2_DR1, BIT(2))
#define GPIOC11(x) (x) ? SET_MASK(GPIO2_DR1, BIT(3)) : CLEAR_MASK(GPIO2_DR1, BIT(3))
#define GPIOC12(x) (x) ? SET_MASK(GPIO2_DR1, BIT(4)) : CLEAR_MASK(GPIO2_DR1, BIT(4))
#define GPIOC13(x) (x) ? SET_MASK(GPIO2_DR1, BIT(5)) : CLEAR_MASK(GPIO2_DR1, BIT(5))
#define GPIOC14(x) (x) ? SET_MASK(GPIO2_DR1, BIT(6)) : CLEAR_MASK(GPIO2_DR1, BIT(6))
#define GPIOC15(x) (x) ? SET_MASK(GPIO2_DR1, BIT(7)) : CLEAR_MASK(GPIO2_DR1, BIT(7))
// GPIOD0 to GPIOD7 
#define GPIOD0(x) (x) ? SET_MASK(GPIO2_DR2, BIT(0)) : CLEAR_MASK(GPIO2_DR2, BIT(0))
#define GPIOD1(x) (x) ? SET_MASK(GPIO2_DR2, BIT(1)) : CLEAR_MASK(GPIO2_DR2, BIT(1))
#define GPIOD2(x) (x) ? SET_MASK(GPIO2_DR2, BIT(2)) : CLEAR_MASK(GPIO2_DR2, BIT(2))
#define GPIOD3(x) (x) ? SET_MASK(GPIO2_DR2, BIT(3)) : CLEAR_MASK(GPIO2_DR2, BIT(3))
#define GPIOD4(x) (x) ? SET_MASK(GPIO2_DR2, BIT(4)) : CLEAR_MASK(GPIO2_DR2, BIT(4))
#define GPIOD5(x) (x) ? SET_MASK(GPIO2_DR2, BIT(5)) : CLEAR_MASK(GPIO2_DR2, BIT(5))
#define GPIOD6(x) (x) ? SET_MASK(GPIO2_DR2, BIT(6)) : CLEAR_MASK(GPIO2_DR2, BIT(6))
#define GPIOD7(x) (x) ? SET_MASK(GPIO2_DR2, BIT(7)) : CLEAR_MASK(GPIO2_DR2, BIT(7))


// GPIOD8 (由GPIO2_DR3控制)
#define GPIOD8(x) (x) ? SET_MASK(GPIO2_DR3, BIT(0)) : CLEAR_MASK(GPIO2_DR3, BIT(0))
// GPIOE0 to GPIOE7 
#define GPIOE0(x) (x) ? SET_MASK(GPIO3_DR0, BIT(0)) : CLEAR_MASK(GPIO3_DR0, BIT(0))
#define GPIOE1(x) (x) ? SET_MASK(GPIO3_DR0, BIT(1)) : CLEAR_MASK(GPIO3_DR0, BIT(1))
#define GPIOE2(x) (x) ? SET_MASK(GPIO3_DR0, BIT(2)) : CLEAR_MASK(GPIO3_DR0, BIT(2))
#define GPIOE3(x) (x) ? SET_MASK(GPIO3_DR0, BIT(3)) : CLEAR_MASK(GPIO3_DR0, BIT(3))
#define GPIOE4(x) (x) ? SET_MASK(GPIO3_DR0, BIT(4)) : CLEAR_MASK(GPIO3_DR0, BIT(4))
#define GPIOE5(x) (x) ? SET_MASK(GPIO3_DR0, BIT(5)) : CLEAR_MASK(GPIO3_DR0, BIT(5))
#define GPIOE6(x) (x) ? SET_MASK(GPIO3_DR0, BIT(6)) : CLEAR_MASK(GPIO3_DR0, BIT(6))
#define GPIOE7(x) (x) ? SET_MASK(GPIO3_DR0, BIT(7)) : CLEAR_MASK(GPIO3_DR0, BIT(7))

// GPIOE8 to GPIOE15 
#define GPIOE8(x)  (x) ? SET_MASK(GPIO3_DR1, BIT(0)) : CLEAR_MASK(GPIO3_DR1, BIT(0))
#define GPIOE9(x)  (x) ? SET_MASK(GPIO3_DR1, BIT(1)) : CLEAR_MASK(GPIO3_DR1, BIT(1))
#define GPIOE10(x) (x) ? SET_MASK(GPIO3_DR1, BIT(2)) : CLEAR_MASK(GPIO3_DR1, BIT(2))
#define GPIOE11(x) (x) ? SET_MASK(GPIO3_DR1, BIT(3)) : CLEAR_MASK(GPIO3_DR1, BIT(3))
#define GPIOE12(x) (x) ? SET_MASK(GPIO3_DR1, BIT(4)) : CLEAR_MASK(GPIO3_DR1, BIT(4))
#define GPIOE13(x) (x) ? SET_MASK(GPIO3_DR1, BIT(5)) : CLEAR_MASK(GPIO3_DR1, BIT(5))
#define GPIOE14(x) (x) ? SET_MASK(GPIO3_DR1, BIT(6)) : CLEAR_MASK(GPIO3_DR1, BIT(6))
#define GPIOE15(x) (x) ? SET_MASK(GPIO3_DR1, BIT(7)) : CLEAR_MASK(GPIO3_DR1, BIT(7))

// GPIOE16 to GPIOE23 
#define GPIOE16(x) (x) ? SET_MASK(GPIO3_DR2, BIT(0)) : CLEAR_MASK(GPIO3_DR2, BIT(0))
#define GPIOE17(x) (x) ? SET_MASK(GPIO3_DR2, BIT(1)) : CLEAR_MASK(GPIO3_DR2, BIT(1))
#define GPIOE18(x) (x) ? SET_MASK(GPIO3_DR2, BIT(2)) : CLEAR_MASK(GPIO3_DR2, BIT(2))
#define GPIOE19(x) (x) ? SET_MASK(GPIO3_DR2, BIT(3)) : CLEAR_MASK(GPIO3_DR2, BIT(3))
#define GPIOE20(x) (x) ? SET_MASK(GPIO3_DR2, BIT(4)) : CLEAR_MASK(GPIO3_DR2, BIT(4))
#define GPIOE21(x) (x) ? SET_MASK(GPIO3_DR2, BIT(5)) : CLEAR_MASK(GPIO3_DR2, BIT(5))
#define GPIOE22(x) (x) ? SET_MASK(GPIO3_DR2, BIT(6)) : CLEAR_MASK(GPIO3_DR2, BIT(6))
#define GPIOE23(x) (x) ? SET_MASK(GPIO3_DR2, BIT(7)) : CLEAR_MASK(GPIO3_DR2, BIT(7))

 /*-----------------------------------------------------------------------------
  * GPIO引脚监测输入高低定义
  *---------------------------------------------------------------------------*/
// GPIOA组输入检测
#define IS_GPIOA0(x)  (x) ? IS_MASK_SET(GPIO0_EXT0, BIT(0)) : IS_MASK_CLEAR(GPIO0_EXT0, BIT(0))
#define IS_GPIOA1(x)  (x) ? IS_MASK_SET(GPIO0_EXT0, BIT(1)) : IS_MASK_CLEAR(GPIO0_EXT0, BIT(1))
#define IS_GPIOA2(x)  (x) ? IS_MASK_SET(GPIO0_EXT0, BIT(2)) : IS_MASK_CLEAR(GPIO0_EXT0, BIT(2))
#define IS_GPIOA3(x)  (x) ? IS_MASK_SET(GPIO0_EXT0, BIT(3)) : IS_MASK_CLEAR(GPIO0_EXT0, BIT(3))
#define IS_GPIOA4(x)  (x) ? IS_MASK_SET(GPIO0_EXT0, BIT(4)) : IS_MASK_CLEAR(GPIO0_EXT0, BIT(4))
#define IS_GPIOA5(x)  (x) ? IS_MASK_SET(GPIO0_EXT0, BIT(5)) : IS_MASK_CLEAR(GPIO0_EXT0, BIT(5))
#define IS_GPIOA6(x)  (x) ? IS_MASK_SET(GPIO0_EXT0, BIT(6)) : IS_MASK_CLEAR(GPIO0_EXT0, BIT(6))
#define IS_GPIOA7(x)  (x) ? IS_MASK_SET(GPIO0_EXT0, BIT(7)) : IS_MASK_CLEAR(GPIO0_EXT0, BIT(7))

#define IS_GPIOA8(x)  (x) ? IS_MASK_SET(GPIO0_EXT1, BIT(0)) : IS_MASK_CLEAR(GPIO0_EXT1, BIT(0))
#define IS_GPIOA9(x)  (x) ? IS_MASK_SET(GPIO0_EXT1, BIT(1)) : IS_MASK_CLEAR(GPIO0_EXT1, BIT(1))
#define IS_GPIOA10(x) (x) ? IS_MASK_SET(GPIO0_EXT1, BIT(2)) : IS_MASK_CLEAR(GPIO0_EXT1, BIT(2))
#define IS_GPIOA11(x) (x) ? IS_MASK_SET(GPIO0_EXT1, BIT(3)) : IS_MASK_CLEAR(GPIO0_EXT1, BIT(3))
#define IS_GPIOA12(x) (x) ? IS_MASK_SET(GPIO0_EXT1, BIT(4)) : IS_MASK_CLEAR(GPIO0_EXT1, BIT(4))
#define IS_GPIOA13(x) (x) ? IS_MASK_SET(GPIO0_EXT1, BIT(5)) : IS_MASK_CLEAR(GPIO0_EXT1, BIT(5))
#define IS_GPIOA14(x) (x) ? IS_MASK_SET(GPIO0_EXT1, BIT(6)) : IS_MASK_CLEAR(GPIO0_EXT1, BIT(6))
#define IS_GPIOA15(x) (x) ? IS_MASK_SET(GPIO0_EXT1, BIT(7)) : IS_MASK_CLEAR(GPIO0_EXT1, BIT(7))

#define IS_GPIOA16(x) (x) ? IS_MASK_SET(GPIO0_EXT2, BIT(0)) : IS_MASK_CLEAR(GPIO0_EXT2, BIT(0))
#define IS_GPIOA17(x) (x) ? IS_MASK_SET(GPIO0_EXT2, BIT(1)) : IS_MASK_CLEAR(GPIO0_EXT2, BIT(1))
#define IS_GPIOA18(x) (x) ? IS_MASK_SET(GPIO0_EXT2, BIT(2)) : IS_MASK_CLEAR(GPIO0_EXT2, BIT(2))
#define IS_GPIOA19(x) (x) ? IS_MASK_SET(GPIO0_EXT2, BIT(3)) : IS_MASK_CLEAR(GPIO0_EXT2, BIT(3))
#define IS_GPIOA20(x) (x) ? IS_MASK_SET(GPIO0_EXT2, BIT(4)) : IS_MASK_CLEAR(GPIO0_EXT2, BIT(4))
#define IS_GPIOA21(x) (x) ? IS_MASK_SET(GPIO0_EXT2, BIT(5)) : IS_MASK_CLEAR(GPIO0_EXT2, BIT(5))
#define IS_GPIOA22(x) (x) ? IS_MASK_SET(GPIO0_EXT2, BIT(6)) : IS_MASK_CLEAR(GPIO0_EXT2, BIT(6))
#define IS_GPIOA23(x) (x) ? IS_MASK_SET(GPIO0_EXT2, BIT(7)) : IS_MASK_CLEAR(GPIO0_EXT2, BIT(7))

#define IS_GPIOA24(x) (x) ? IS_MASK_SET(GPIO0_EXT3, BIT(0)) : IS_MASK_CLEAR(GPIO0_EXT3, BIT(0))
#define IS_GPIOA25(x) (x) ? IS_MASK_SET(GPIO0_EXT3, BIT(1)) : IS_MASK_CLEAR(GPIO0_EXT3, BIT(1))
#define IS_GPIOA26(x) (x) ? IS_MASK_SET(GPIO0_EXT3, BIT(2)) : IS_MASK_CLEAR(GPIO0_EXT3, BIT(2))
#define IS_GPIOA27(x) (x) ? IS_MASK_SET(GPIO0_EXT3, BIT(3)) : IS_MASK_CLEAR(GPIO0_EXT3, BIT(3))
#define IS_GPIOA28(x) (x) ? IS_MASK_SET(GPIO0_EXT3, BIT(4)) : IS_MASK_CLEAR(GPIO0_EXT3, BIT(4))
#define IS_GPIOA29(x) (x) ? IS_MASK_SET(GPIO0_EXT3, BIT(5)) : IS_MASK_CLEAR(GPIO0_EXT3, BIT(5))
#define IS_GPIOA30(x) (x) ? IS_MASK_SET(GPIO0_EXT3, BIT(6)) : IS_MASK_CLEAR(GPIO0_EXT3, BIT(6))
#define IS_GPIOA31(x) (x) ? IS_MASK_SET(GPIO0_EXT3, BIT(7)) : IS_MASK_CLEAR(GPIO0_EXT3, BIT(7))

// GPIOB组输入检测
#define IS_GPIOB0(x)  (x) ? IS_MASK_SET(GPIO1_EXT0, BIT(0)) : IS_MASK_CLEAR(GPIO1_EXT0, BIT(0))
#define IS_GPIOB1(x)  (x) ? IS_MASK_SET(GPIO1_EXT0, BIT(1)) : IS_MASK_CLEAR(GPIO1_EXT0, BIT(1))
#define IS_GPIOB2(x)  (x) ? IS_MASK_SET(GPIO1_EXT0, BIT(2)) : IS_MASK_CLEAR(GPIO1_EXT0, BIT(2))
#define IS_GPIOB3(x)  (x) ? IS_MASK_SET(GPIO1_EXT0, BIT(3)) : IS_MASK_CLEAR(GPIO1_EXT0, BIT(3))
#define IS_GPIOB4(x)  (x) ? IS_MASK_SET(GPIO1_EXT0, BIT(4)) : IS_MASK_CLEAR(GPIO1_EXT0, BIT(4))
#define IS_GPIOB5(x)  (x) ? IS_MASK_SET(GPIO1_EXT0, BIT(5)) : IS_MASK_CLEAR(GPIO1_EXT0, BIT(5))
#define IS_GPIOB6(x)  (x) ? IS_MASK_SET(GPIO1_EXT0, BIT(6)) : IS_MASK_CLEAR(GPIO1_EXT0, BIT(6))
#define IS_GPIOB7(x)  (x) ? IS_MASK_SET(GPIO1_EXT0, BIT(7)) : IS_MASK_CLEAR(GPIO1_EXT0, BIT(7))

#define IS_GPIOB8(x)  (x) ? IS_MASK_SET(GPIO1_EXT1, BIT(0)) : IS_MASK_CLEAR(GPIO1_EXT1, BIT(0))
#define IS_GPIOB9(x)  (x) ? IS_MASK_SET(GPIO1_EXT1, BIT(1)) : IS_MASK_CLEAR(GPIO1_EXT1, BIT(1))
#define IS_GPIOB10(x) (x) ? IS_MASK_SET(GPIO1_EXT1, BIT(2)) : IS_MASK_CLEAR(GPIO1_EXT1, BIT(2))
#define IS_GPIOB11(x) (x) ? IS_MASK_SET(GPIO1_EXT1, BIT(3)) : IS_MASK_CLEAR(GPIO1_EXT1, BIT(3))
#define IS_GPIOB12(x) (x) ? IS_MASK_SET(GPIO1_EXT1, BIT(4)) : IS_MASK_CLEAR(GPIO1_EXT1, BIT(4))
#define IS_GPIOB13(x) (x) ? IS_MASK_SET(GPIO1_EXT1, BIT(5)) : IS_MASK_CLEAR(GPIO1_EXT1, BIT(5))
#define IS_GPIOB14(x) (x) ? IS_MASK_SET(GPIO1_EXT1, BIT(6)) : IS_MASK_CLEAR(GPIO1_EXT1, BIT(6))
#define IS_GPIOB15(x) (x) ? IS_MASK_SET(GPIO1_EXT1, BIT(7)) : IS_MASK_CLEAR(GPIO1_EXT1, BIT(7))

#define IS_GPIOB16(x) (x) ? IS_MASK_SET(GPIO1_EXT2, BIT(0)) : IS_MASK_CLEAR(GPIO1_EXT2, BIT(0))
#define IS_GPIOB17(x) (x) ? IS_MASK_SET(GPIO1_EXT2, BIT(1)) : IS_MASK_CLEAR(GPIO1_EXT2, BIT(1))
#define IS_GPIOB18(x) (x) ? IS_MASK_SET(GPIO1_EXT2, BIT(2)) : IS_MASK_CLEAR(GPIO1_EXT2, BIT(2))
#define IS_GPIOB19(x) (x) ? IS_MASK_SET(GPIO1_EXT2, BIT(3)) : IS_MASK_CLEAR(GPIO1_EXT2, BIT(3))
#define IS_GPIOB20(x) (x) ? IS_MASK_SET(GPIO1_EXT2, BIT(4)) : IS_MASK_CLEAR(GPIO1_EXT2, BIT(4))
#define IS_GPIOB21(x) (x) ? IS_MASK_SET(GPIO1_EXT2, BIT(5)) : IS_MASK_CLEAR(GPIO1_EXT2, BIT(5))
#define IS_GPIOB22(x) (x) ? IS_MASK_SET(GPIO1_EXT2, BIT(6)) : IS_MASK_CLEAR(GPIO1_EXT2, BIT(6))
#define IS_GPIOB23(x) (x) ? IS_MASK_SET(GPIO1_EXT2, BIT(7)) : IS_MASK_CLEAR(GPIO1_EXT2, BIT(7))

#define IS_GPIOB24(x) (x) ? IS_MASK_SET(GPIO1_EXT3, BIT(0)) : IS_MASK_CLEAR(GPIO1_EXT3, BIT(0))
#define IS_GPIOB25(x) (x) ? IS_MASK_SET(GPIO1_EXT3, BIT(1)) : IS_MASK_CLEAR(GPIO1_EXT3, BIT(1))
#define IS_GPIOB26(x) (x) ? IS_MASK_SET(GPIO1_EXT3, BIT(2)) : IS_MASK_CLEAR(GPIO1_EXT3, BIT(2))
#define IS_GPIOB27(x) (x) ? IS_MASK_SET(GPIO1_EXT3, BIT(3)) : IS_MASK_CLEAR(GPIO1_EXT3, BIT(3))
#define IS_GPIOB28(x) (x) ? IS_MASK_SET(GPIO1_EXT3, BIT(4)) : IS_MASK_CLEAR(GPIO1_EXT3, BIT(4))
#define IS_GPIOB29(x) (x) ? IS_MASK_SET(GPIO1_EXT3, BIT(5)) : IS_MASK_CLEAR(GPIO1_EXT3, BIT(5))
#define IS_GPIOB30(x) (x) ? IS_MASK_SET(GPIO1_EXT3, BIT(6)) : IS_MASK_CLEAR(GPIO1_EXT3, BIT(6))
#define IS_GPIOB31(x) (x) ? IS_MASK_SET(GPIO1_EXT3, BIT(7)) : IS_MASK_CLEAR(GPIO1_EXT3, BIT(7))
// GPIOC组输入检测
#define IS_GPIOC0(x)  (x) ? IS_MASK_SET(GPIO2_EXT0, BIT(0)) : IS_MASK_CLEAR(GPIO2_EXT0, BIT(0))
#define IS_GPIOC1(x)  (x) ? IS_MASK_SET(GPIO2_EXT0, BIT(1)) : IS_MASK_CLEAR(GPIO2_EXT0, BIT(1))
#define IS_GPIOC2(x)  (x) ? IS_MASK_SET(GPIO2_EXT0, BIT(2)) : IS_MASK_CLEAR(GPIO2_EXT0, BIT(2))
#define IS_GPIOC3(x)  (x) ? IS_MASK_SET(GPIO2_EXT0, BIT(3)) : IS_MASK_CLEAR(GPIO2_EXT0, BIT(3))
#define IS_GPIOC4(x)  (x) ? IS_MASK_SET(GPIO2_EXT0, BIT(4)) : IS_MASK_CLEAR(GPIO2_EXT0, BIT(4))
#define IS_GPIOC5(x)  (x) ? IS_MASK_SET(GPIO2_EXT0, BIT(5)) : IS_MASK_CLEAR(GPIO2_EXT0, BIT(5))
#define IS_GPIOC6(x)  (x) ? IS_MASK_SET(GPIO2_EXT0, BIT(6)) : IS_MASK_CLEAR(GPIO2_EXT0, BIT(6))
#define IS_GPIOC7(x)  (x) ? IS_MASK_SET(GPIO2_EXT0, BIT(7)) : IS_MASK_CLEAR(GPIO2_EXT0, BIT(7))

#define IS_GPIOC8(x)  (x) ? IS_MASK_SET(GPIO2_EXT1, BIT(0)) : IS_MASK_CLEAR(GPIO2_EXT1, BIT(0))
#define IS_GPIOC9(x)  (x) ? IS_MASK_SET(GPIO2_EXT1, BIT(1)) : IS_MASK_CLEAR(GPIO2_EXT1, BIT(1))
#define IS_GPIOC10(x) (x) ? IS_MASK_SET(GPIO2_EXT1, BIT(2)) : IS_MASK_CLEAR(GPIO2_EXT1, BIT(2))
#define IS_GPIOC11(x) (x) ? IS_MASK_SET(GPIO2_EXT1, BIT(3)) : IS_MASK_CLEAR(GPIO2_EXT1, BIT(3))
#define IS_GPIOC12(x) (x) ? IS_MASK_SET(GPIO2_EXT1, BIT(4)) : IS_MASK_CLEAR(GPIO2_EXT1, BIT(4))
#define IS_GPIOC13(x) (x) ? IS_MASK_SET(GPIO2_EXT1, BIT(5)) : IS_MASK_CLEAR(GPIO2_EXT1, BIT(5))
#define IS_GPIOC14(x) (x) ? IS_MASK_SET(GPIO2_EXT1, BIT(6)) : IS_MASK_CLEAR(GPIO2_EXT1, BIT(6))
#define IS_GPIOC15(x) (x) ? IS_MASK_SET(GPIO2_EXT1, BIT(7)) : IS_MASK_CLEAR(GPIO2_EXT1, BIT(7))
// GPIOD组输入检测
#define IS_GPIOD0(x) (x) ? IS_MASK_SET(GPIO2_EXT2, BIT(0)) : IS_MASK_CLEAR(GPIO2_EXT2, BIT(0))
#define IS_GPIOD1(x) (x) ? IS_MASK_SET(GPIO2_EXT2, BIT(1)) : IS_MASK_CLEAR(GPIO2_EXT2, BIT(1))
#define IS_GPIOD2(x) (x) ? IS_MASK_SET(GPIO2_EXT2, BIT(2)) : IS_MASK_CLEAR(GPIO2_EXT2, BIT(2))
#define IS_GPIOD3(x) (x) ? IS_MASK_SET(GPIO2_EXT2, BIT(3)) : IS_MASK_CLEAR(GPIO2_EXT2, BIT(3))
#define IS_GPIOD4(x) (x) ? IS_MASK_SET(GPIO2_EXT2, BIT(4)) : IS_MASK_CLEAR(GPIO2_EXT2, BIT(4))
#define IS_GPIOD5(x) (x) ? IS_MASK_SET(GPIO2_EXT2, BIT(5)) : IS_MASK_CLEAR(GPIO2_EXT2, BIT(5))
#define IS_GPIOD6(x) (x) ? IS_MASK_SET(GPIO2_EXT2, BIT(6)) : IS_MASK_CLEAR(GPIO2_EXT2, BIT(6))
#define IS_GPIOD7(x) (x) ? IS_MASK_SET(GPIO2_EXT2, BIT(7)) : IS_MASK_CLEAR(GPIO2_EXT2, BIT(7))
#define IS_GPIOD8(x) (x) ? IS_MASK_SET(GPIO2_EXT3, BIT(0)) : IS_MASK_CLEAR(GPIO2_EXT3, BIT(0))
// GPIOE组输入检测
#define IS_GPIOE0(x)  (x) ? IS_MASK_SET(GPIO3_EXT0, BIT(0)) : IS_MASK_CLEAR(GPIO3_EXT0, BIT(0))
#define IS_GPIOE1(x)  (x) ? IS_MASK_SET(GPIO3_EXT0, BIT(1)) : IS_MASK_CLEAR(GPIO3_EXT0, BIT(1))
#define IS_GPIOE2(x)  (x) ? IS_MASK_SET(GPIO3_EXT0, BIT(2)) : IS_MASK_CLEAR(GPIO3_EXT0, BIT(2))
#define IS_GPIOE3(x)  (x) ? IS_MASK_SET(GPIO3_EXT0, BIT(3)) : IS_MASK_CLEAR(GPIO3_EXT0, BIT(3))
#define IS_GPIOE4(x)  (x) ? IS_MASK_SET(GPIO3_EXT0, BIT(4)) : IS_MASK_CLEAR(GPIO3_EXT0, BIT(4))
#define IS_GPIOE5(x)  (x) ? IS_MASK_SET(GPIO3_EXT0, BIT(5)) : IS_MASK_CLEAR(GPIO3_EXT0, BIT(5))
#define IS_GPIOE6(x)  (x) ? IS_MASK_SET(GPIO3_EXT0, BIT(6)) : IS_MASK_CLEAR(GPIO3_EXT0, BIT(6))
#define IS_GPIOE7(x)  (x) ? IS_MASK_SET(GPIO3_EXT0, BIT(7)) : IS_MASK_CLEAR(GPIO3_EXT0, BIT(7))

#define IS_GPIOE8(x)  (x) ? IS_MASK_SET(GPIO3_EXT1, BIT(0)) : IS_MASK_CLEAR(GPIO3_EXT1, BIT(0))
#define IS_GPIOE9(x)  (x) ? IS_MASK_SET(GPIO3_EXT1, BIT(1)) : IS_MASK_CLEAR(GPIO3_EXT1, BIT(1))
#define IS_GPIOE10(x) (x) ? IS_MASK_SET(GPIO3_EXT1, BIT(2)) : IS_MASK_CLEAR(GPIO3_EXT1, BIT(2))
#define IS_GPIOE11(x) (x) ? IS_MASK_SET(GPIO3_EXT1, BIT(3)) : IS_MASK_CLEAR(GPIO3_EXT1, BIT(3))
#define IS_GPIOE12(x) (x) ? IS_MASK_SET(GPIO3_EXT1, BIT(4)) : IS_MASK_CLEAR(GPIO3_EXT1, BIT(4))
#define IS_GPIOE13(x) (x) ? IS_MASK_SET(GPIO3_EXT1, BIT(5)) : IS_MASK_CLEAR(GPIO3_EXT1, BIT(5))
#define IS_GPIOE14(x) (x) ? IS_MASK_SET(GPIO3_EXT1, BIT(6)) : IS_MASK_CLEAR(GPIO3_EXT1, BIT(6))
#define IS_GPIOE15(x) (x) ? IS_MASK_SET(GPIO3_EXT1, BIT(7)) : IS_MASK_CLEAR(GPIO3_EXT1, BIT(7))

#define IS_GPIOE16(x) (x) ? IS_MASK_SET(GPIO3_EXT2, BIT(0)) : IS_MASK_CLEAR(GPIO3_EXT2, BIT(0))
#define IS_GPIOE17(x) (x) ? IS_MASK_SET(GPIO3_EXT2, BIT(1)) : IS_MASK_CLEAR(GPIO3_EXT2, BIT(1))
#define IS_GPIOE18(x) (x) ? IS_MASK_SET(GPIO3_EXT2, BIT(2)) : IS_MASK_CLEAR(GPIO3_EXT2, BIT(2))
#define IS_GPIOE19(x) (x) ? IS_MASK_SET(GPIO3_EXT2, BIT(3)) : IS_MASK_CLEAR(GPIO3_EXT2, BIT(3))
#define IS_GPIOE20(x) (x) ? IS_MASK_SET(GPIO3_EXT2, BIT(4)) : IS_MASK_CLEAR(GPIO3_EXT2, BIT(4))
#define IS_GPIOE21(x) (x) ? IS_MASK_SET(GPIO3_EXT2, BIT(5)) : IS_MASK_CLEAR(GPIO3_EXT2, BIT(5))
#define IS_GPIOE22(x) (x) ? IS_MASK_SET(GPIO3_EXT2, BIT(6)) : IS_MASK_CLEAR(GPIO3_EXT2, BIT(6))
#define IS_GPIOE23(x) (x) ? IS_MASK_SET(GPIO3_EXT2, BIT(7)) : IS_MASK_CLEAR(GPIO3_EXT2, BIT(7))
 /*-----------------------------------------------------------------------------
  * GPIO引脚输出高低定义(按照pin的格式)
  *---------------------------------------------------------------------------*/
#define Pin2(x) GPIOC15(x)
#define Pin3(x) GPIOB31(x)
#define Pin4(x)   GPIOA14(x)
#define Pin5(x)   GPIOD6(x)
#define Pin6(x)   GPIOD5(x)
#define Pin7(x)   GPIOD3(x)
#define Pin8(x)   GPIOD2(x)
#define Pin9(x)   GPIOD1(x)
#define Pin10(x)  GPIOD0(x)
#define Pin13(x)  GPIOD4(x)
#define Pin15(x)  GPIOA28(x)
#define Pin16(x)  GPIOA23(x)
#define Pin17(x)  GPIOB6(x)
#define Pin18(x)  GPIOA24(x)
#define Pin19(x)  GPIOB0(x)
#define Pin20(x)  GPIOB7(x)
#define Pin21(x)  GPIOA25(x)
#define Pin22(x)  GPIOA26(x)
#define Pin23(x)  GPIOA27(x)
#define Pin24(x)  GPIOA0(x)
#define Pin25(x)  GPIOA1(x)
#define Pin28(x)  GPIOA2(x)
#define Pin29(x)  GPIOA3(x)
#define Pin30(x)  GPIOA4(x)
#define Pin31(x)  GPIOA5(x)
#define Pin32(x)  GPIOA6(x)
#define Pin33(x)  GPIOA29(x)
#define Pin34(x)  GPIOA7(x)
#define Pin35(x)  GPIOB5(x)
#define Pin36(x)  GPIOE0(x)
#define Pin37(x)  GPIOE1(x)
#define Pin38(x)  GPIOE2(x)
#define Pin39(x)  GPIOE3(x)
#define Pin40(x)  GPIOE4(x)
#define Pin41(x)  GPIOE5(x)
#define Pin42(x)  GPIOE6(x)
#define Pin43(x)  GPIOE7(x)
#define Pin44(x)  GPIOE8(x)
#define Pin45(x)  GPIOE9(x)
#define Pin46(x)  GPIOE10(x)
#define Pin47(x)  GPIOA30(x)
#define Pin48(x)  GPIOA31(x)
#define Pin51(x)  GPIOE11(x)
#define Pin52(x)  GPIOE12(x)
#define Pin53(x)  GPIOE13(x)
#define Pin54(x)  GPIOE14(x)
#define Pin55(x)  GPIOE15(x)
#define Pin56(x)  GPIOA19(x)
#define Pin57(x)  GPIOA21(x)
#define Pin58(x)  GPIOE16(x)
#define Pin59(x)  GPIOE17(x)
#define Pin60(x)  GPIOE18(x)
#define Pin61(x)  GPIOE19(x)
#define Pin62(x)  GPIOE20(x)
#define Pin63(x)  GPIOE21(x)
#define Pin64(x)  GPIOE22(x)
#define Pin65(x)  GPIOE23(x)
#define Pin66(x)  GPIOC0(x)
#define Pin67(x)  GPIOC1(x)
#define Pin68(x)  GPIOC2(x)
#define Pin69(x)  GPIOC3(x)
#define Pin70(x)  GPIOC4(x)
#define Pin71(x)  GPIOC5(x)
#define Pin72(x)  GPIOC6(x)
#define Pin73(x)  GPIOC7(x)
#define Pin76(x)  GPIOC8(x)
#define Pin77(x)  GPIOC9(x)
#define Pin78(x)  GPIOC10(x)
#define Pin79(x)  GPIOC11(x)
#define Pin80(x)  GPIOC12(x)
#define Pin81(x)  GPIOC13(x)
#define Pin82(x)  GPIOB1(x)
#define Pin83(x)  GPIOB2(x)
#define Pin84(x)  GPIOB3(x)
#define Pin85(x)  GPIOB8(x)
#define Pin86(x)  GPIOB9(x)
#define Pin87(x)  GPIOB10(x)
#define Pin88(x)  GPIOB11(x)
#define Pin89(x)  GPIOB12(x)
#define Pin90(x)  GPIOB13(x)

#define Pin93(x)  GPIOB24(x)
#define Pin94(x)  GPIOB25(x)
#define Pin95(x)  GPIOB26(x)
#define Pin96(x)  GPIOB27(x)
#define Pin97(x)  GPIOB28(x)
#define Pin98(x)  GPIOB29(x)
#define Pin99(x)  GPIOB30(x)
#define Pin100(x) GPIOB18(x)
#define Pin101(x) GPIOB22(x)
#define Pin102(x) GPIOB20(x)
#define Pin103(x) GPIOB21(x)
#define Pin104(x) GPIOB19(x)
#define Pin105(x) GPIOB23(x)
#define Pin106(x) GPIOB16(x)
#define Pin107(x) GPIOB17(x)
#define Pin108(x) GPIOA8(x)
#define Pin109(x) GPIOA9(x)
#define Pin110(x) GPIOA11(x)
#define Pin111(x) GPIOA12(x)
#define Pin112(x) GPIOA15(x)
#define Pin113(x) GPIOD7(x)
#define Pin115(x) GPIOA17(x)
#define Pin116(x) GPIOA18(x)
#define Pin117(x) GPIOB14(x)
#define Pin118(x) GPIOB15(x)
#define Pin119(x) GPIOA16(x)
#define Pin120(x) GPIOA20(x)
#define Pin122(x) GPIOD8(x)
#define Pin123(x) GPIOA10(x)
#define Pin124(x) GPIOA22(x)
#define Pin125(x) GPIOB4(x)
#define Pin126(x) GPIOA13(x)
#define Pin128(x) GPIOC14(x)
 /*-----------------------------------------------------------------------------
  * GPIO引脚检测高低定义(按照pin的格式)
  *---------------------------------------------------------------------------*/
#define IS_Pin2(x)    IS_GPIOC15(x)
#define IS_Pin3(x)    IS_GPIOB31(x)
#define IS_Pin4(x)    IS_GPIOA14(x)
#define IS_Pin5(x)    IS_GPIOD6(x)
#define IS_Pin6(x)    IS_GPIOD5(x)
#define IS_Pin7(x)    IS_GPIOD3(x)
#define IS_Pin8(x)    IS_GPIOD2(x)
#define IS_Pin9(x)    IS_GPIOD1(x)
#define IS_Pin10(x)   IS_GPIOD0(x)
#define IS_Pin13(x)   IS_GPIOD4(x)
#define IS_Pin15(x)   IS_GPIOA28(x)
#define IS_Pin16(x)   IS_GPIOA23(x)
#define IS_Pin17(x)   IS_GPIOB6(x)
#define IS_Pin18(x)   IS_GPIOA24(x)
#define IS_Pin19(x)   IS_GPIOB0(x)
#define IS_Pin20(x)   IS_GPIOB7(x)
#define IS_Pin21(x)   IS_GPIOA25(x)
#define IS_Pin22(x)   IS_GPIOA26(x)
#define IS_Pin23(x)   IS_GPIOA27(x)
#define IS_Pin24(x)   IS_GPIOA0(x)
#define IS_Pin25(x)   IS_GPIOA1(x)
#define IS_Pin28(x)   IS_GPIOA2(x)
#define IS_Pin29(x)   IS_GPIOA3(x)
#define IS_Pin30(x)   IS_GPIOA4(x)
#define IS_Pin31(x)   IS_GPIOA5(x)
#define IS_Pin32(x)   IS_GPIOA6(x)
#define IS_Pin33(x)   IS_GPIOA29(x)
#define IS_Pin34(x)   IS_GPIOA7(x)
#define IS_Pin35(x)   IS_GPIOB5(x)
#define IS_Pin36(x)   IS_GPIOE0(x)
#define IS_Pin37(x)   IS_GPIOE1(x)
#define IS_Pin38(x)   IS_GPIOE2(x)
#define IS_Pin39(x)   IS_GPIOE3(x)
#define IS_Pin40(x)   IS_GPIOE4(x)
#define IS_Pin41(x)   IS_GPIOE5(x)
#define IS_Pin42(x)   IS_GPIOE6(x)
#define IS_Pin43(x)   IS_GPIOE7(x)
#define IS_Pin44(x)   IS_GPIOE8(x)
#define IS_Pin45(x)   IS_GPIOE9(x)
#define IS_Pin46(x)   IS_GPIOE10(x)
#define IS_Pin47(x)   IS_GPIOA30(x)
#define IS_Pin48(x)   IS_GPIOA31(x)
#define IS_Pin51(x)   IS_GPIOE11(x)
#define IS_Pin52(x)   IS_GPIOE12(x)
#define IS_Pin53(x)   IS_GPIOE13(x)
#define IS_Pin54(x)   IS_GPIOE14(x)
#define IS_Pin55(x)   IS_GPIOE15(x)
#define IS_Pin56(x)   IS_GPIOA19(x)
#define IS_Pin57(x)   IS_GPIOA21(x)
#define IS_Pin58(x)   IS_GPIOE16(x)
#define IS_Pin59(x)   IS_GPIOE17(x)
#define IS_Pin60(x)   IS_GPIOE18(x)
#define IS_Pin61(x)   IS_GPIOE19(x)
#define IS_Pin62(x)   IS_GPIOE20(x)
#define IS_Pin63(x)   IS_GPIOE21(x)
#define IS_Pin64(x)   IS_GPIOE22(x)
#define IS_Pin65(x)   IS_GPIOE23(x)
#define IS_Pin66(x)   IS_GPIOC0(x)
#define IS_Pin67(x)   IS_GPIOC1(x)
#define IS_Pin68(x)   IS_GPIOC2(x)
#define IS_Pin69(x)   IS_GPIOC3(x)
#define IS_Pin70(x)   IS_GPIOC4(x)
#define IS_Pin71(x)   IS_GPIOC5(x)
#define IS_Pin72(x)   IS_GPIOC6(x)
#define IS_Pin73(x)   IS_GPIOC7(x)
#define IS_Pin76(x)   IS_GPIOC8(x)
#define IS_Pin77(x)   IS_GPIOC9(x)
#define IS_Pin78(x)   IS_GPIOC10(x)
#define IS_Pin79(x)   IS_GPIOC11(x)
#define IS_Pin80(x)   IS_GPIOC12(x)
#define IS_Pin81(x)   IS_GPIOC13(x)
#define IS_Pin82(x)   IS_GPIOB1(x)
#define IS_Pin83(x)   IS_GPIOB2(x)
#define IS_Pin84(x)   IS_GPIOB3(x)
#define IS_Pin85(x)   IS_GPIOB8(x)
#define IS_Pin86(x)   IS_GPIOB9(x)
#define IS_Pin87(x)   IS_GPIOB10(x)
#define IS_Pin88(x)   IS_GPIOB11(x)
#define IS_Pin89(x)   IS_GPIOB12(x)
#define IS_Pin90(x)   IS_GPIOB13(x)

#define IS_Pin93(x)   IS_GPIOB24(x)
#define IS_Pin94(x)   IS_GPIOB25(x)
#define IS_Pin95(x)   IS_GPIOB26(x)
#define IS_Pin96(x)   IS_GPIOB27(x)
#define IS_Pin97(x)   IS_GPIOB28(x)
#define IS_Pin98(x)   IS_GPIOB29(x)
#define IS_Pin99(x)   IS_GPIOB30(x)
#define IS_Pin100(x)  IS_GPIOB18(x)
#define IS_Pin101(x)  IS_GPIOB22(x)
#define IS_Pin102(x)  IS_GPIOB20(x)
#define IS_Pin103(x)  IS_GPIOB21(x)
#define IS_Pin104(x)  IS_GPIOB19(x)
#define IS_Pin105(x)  IS_GPIOB23(x)
#define IS_Pin106(x)  IS_GPIOB16(x)
#define IS_Pin107(x)  IS_GPIOB17(x)
#define IS_Pin108(x)  IS_GPIOA8(x)
#define IS_Pin109(x)  IS_GPIOA9(x)
#define IS_Pin110(x)  IS_GPIOA11(x)
#define IS_Pin111(x)  IS_GPIOA12(x)
#define IS_Pin112(x)  IS_GPIOA15(x)
#define IS_Pin113(x)  IS_GPIOD7(x)
#define IS_Pin115(x)  IS_GPIOA17(x)
#define IS_Pin116(x)  IS_GPIOA18(x)
#define IS_Pin117(x)  IS_GPIOB14(x)
#define IS_Pin118(x)  IS_GPIOB15(x)
#define IS_Pin119(x)  IS_GPIOA16(x)
#define IS_Pin120(x)  IS_GPIOA20(x)
#define IS_Pin122(x)  IS_GPIOD8(x)
#define IS_Pin123(x)  IS_GPIOA10(x)
#define IS_Pin124(x)  IS_GPIOA22(x)
#define IS_Pin125(x)  IS_GPIOB4(x)
#define IS_Pin126(x)  IS_GPIOA13(x)
#define IS_Pin128(x)  IS_GPIOC14(x)
 /*-----------------------------------------------------------------------------
  * function extern
  *---------------------------------------------------------------------------*/
extern void pin_DefaultConfig(void);
extern void gpio_DefaultConfig(void);
#endif
