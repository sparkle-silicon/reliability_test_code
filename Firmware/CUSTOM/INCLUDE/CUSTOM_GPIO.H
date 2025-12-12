/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-10-21 16:23:35
 * @Description:
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
#ifndef CUSTOM_GPIO_H
#define CUSTOM_GPIO_H
#include "AE_GLOBAL.H"

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
                          // 6 5 4 3 2 1 0
#define PinE0_23_IoMux    0b01010101010101  // GPIO E[23:0] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
//------------------------------------------------------------------------------
// EMB FLASH Registers define 
//------------------------------------------------------------------------------
#define PinEmbFLASH_MISO_InOut 0b0
#define PinEmbFLASH_MOSI_InOut 0b0
#define PinEmbFLASH_HOLD_InOut 0b0
#define PinEmbFLASH_WP_InOut 0b0
                         //76543210
#define PinEFLASH_InOut (0b11000000 | \
						(((PinEmbFLASH_MISO_InOut & 0b1) << 0) | \
						 ((PinEmbFLASH_MOSI_InOut & 0b1) << 1) | \
						 ((PinEmbFLASH_HOLD_InOut & 0b1) << 4) | \
						 ((PinEmbFLASH_WP_InOut & 0b1) << 5)))
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
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
extern void pin_DefaultConfig(void);
extern void gpio_DefaultConfig(void);
#endif
