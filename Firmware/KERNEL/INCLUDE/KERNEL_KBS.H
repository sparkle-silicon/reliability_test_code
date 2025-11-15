/*
 * @Author: Iversu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2023-12-08 18:48:13
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
#ifndef KERNEL_KBS_H
#define KERNEL_KBS_H
#include <AE_GLOBAL.H>
#include <AE_REG.H>
#include <AE_CONFIG.H>
#include <AE_PRINTF.H>
#include <AE_FUNC.H>
#define KBD_Int_Enable	(KBS_KSDC1R |= (0x1<<5))
#define KBD_Int_Disable	(KBS_KSDC1R &= ~(0x1<<5))
#define KBD_Int_Enable_Read	((KBS_KSDC1R & (0x1<<5)) != 0)
 //kbs reg define
#define KBS_KSOCTRL_ODEN                HEX01
#define KBS_KSDC1R_EN                    BIT7
#define KBS_KSDC1R_INT_EN                BIT5
#define KBS_KSDC1R_ROUND_1               HEX00
#define KBS_KSDC1R_ROUND_2               HEX01
#define KBS_KSDC1R_ROUND_3               HEX02
#define KBS_KSDC1R_ROUND_4               HEX03
#define KBS_KSDC1R_ROUND_5               HEX04
#define KBS_KSDC1R_ROUND_6               HEX05
#define KBS_KSDC1R_ROUND_7               HEX06
#define KBS_KSDC1R_ROUND_8               HEX07
#define KBS_KSDC2R_SSEL_16               HEX00
#define KBS_KSDC2R_SSEL_17               HEX40
#define KBS_KSDC2R_SSEL_18               HEX80
#define KBS_KSDC2R_DELAY24               HEX00
#define KBS_KSDC2R_DELAY32               HEX01
#define KBS_KSDC2R_DELAY40               HEX02
#define KBS_KSDC2R_DELAY49               HEX03
#define KBS_KSDC2R_DELAY57               HEX04
#define KBS_KSDC2R_DELAY65               HEX05
#define KBS_KSDC2R_DELAY74               HEX06
#define KBS_KSDC2R_DELAY82               HEX07
#define KBS_KSDC2R_DELAY90               HEX08
#define KBS_KSDC2R_DELAY99               HEX09
#define KBS_KSDC3R_DELAY0                HEX00
#define KBS_KSDC3R_DELAY1                HEX01
#define KBS_KSDC3R_DELAY2                HEX02
#define KBS_KSDC3R_DELAY3                HEX03
#define KBS_KSDC3R_DELAY4                HEX04
#define KBS_KSDC3R_DELAY5                HEX05
#define KBS_KSDC3R_DELAY6                HEX06
#define KBS_KSDC3R_DELAY7                HEX07
#define KBS_KSDC3R_DELAY8                HEX08
#define KBS_KSDC3R_DELAY9                HEX09
#define KBS_KSDC3R_DELAY10               HEX0A
#define KBS_KSDC3R_DELAY11               HEX00
#define KBS_KSDC3R_DELAY13               HEX10
#define KBS_KSDC3R_DELAY15               HEX20
#define KBS_KSDC3R_DELAY17               0x30
#define KBS_KSDC3R_DELAY19               0x40
#define KBS_KSDC3R_DELAY21               0x50
#define KBS_KSDC3R_DELAY23               0x60
#define KBS_KSDC3R_DELAY25               0x70
#define KBS_KSDC3R_DELAY27               0x80
#define KBS_KSDC3R_DELAY29               0x90
//-----------------------------------------------------------------------------
// Key KBD_SCAN interface
//-----------------------------------------------------------------------------
#define MAKE_EVENT   0          //按键事件
#define BREAK_EVENT  1	        //间断事件
#define REPEAT_EVENT 2          //重复的事件
// TM_xxx definitions are part of typematic handling for scanner keyboard.
#define TM_SCALE    			2		// Pre-scale timer base is 5ms (5ms X 2 = 10ms)
#define TM_REPEAT   			3
#define TM_DELAY    			60
#define STANDARD_SCAN_LINES 	16		// Don't modify this one
#define MAX_SCAN_LINES 			19		// Don't greater than 19，按键状态缓存数组
#define KBF_SIZE                16
#define SSKEY2_SPE_CODE 		0x9C  //组合按键
#define SSKEY2_OVL_CODE 		0xE0  //特殊功能
// Definitions for indices into Key Code Process Table.
#define DO_SIMPLE_CODE          0
#define DO_E0_PREFIX_CODE       1
#define DO_CURSOR_SHIFT         2
#define DO_CURSOR_NUMLOCK       3
#define DO_SPECIAL_CODE         4
#define DO_CONTROL_EFFECT       5
#define DO_CONTROL_EFFECT_E0    6
#define DO_COSTOMER_FUNCTION    7	
#define KU_ACK_Start            0xFF
#define KU_ACK_FullKeyboard     0x44
#define KU_ACK_Keyboard         0x33
#define KBDebug   		BIT6    // 1: for keyboard matrix utility
#define SMBusDebug		BIT7    // 1: for smbus utility
#define SDEN            BIT7        // Scan data enable
#define INTSDVEN        BIT5        // Interrupt from KBD_SCAN data valid enable
// BIT3 ~ BIT0  Wait KSO high delay
#define WKSOHDLY_23us   0x00
#define WKSOHDLY_31us   0x01
#define WKSOHDLY_39us   0x02
#define WKSOHDLY_47us   0x03
#define WKSOHDLY_55us   0x04
#define WKSOHDLY_63us   0x05
#define WKSOHDLY_71us   0x06
#define WKSOHDLY_79us   0x07
#define WKSOHDLY_87us   0x08
#define WKSOHDLY_95us   0x09
//---------------------------------
// BIT7 ~ BIT4  Wait KSO low delay
#define WKSOLDLY_11us   (0x00 << 4)
#define WKSOLDLY_13us   (0x01 << 4)
#define WKSOLDLY_15us   (0x02 << 4)
#define WKSOLDLY_17us   (0x03 << 4)
#define WKSOLDLY_19us   (0x04 << 4)
#define WKSOLDLY_21us   (0x05 << 4)
#define WKSOLDLY_23us   (0x06 << 4)
#define WKSOLDLY_25us   (0x07 << 4)
#define WKSOLDLY_27us   (0x08 << 4)
#define WKSOLDLY_29us   (0x09 << 4)
// BIT3 ~ BIT0  Spacing delay between rounds
#define SDLYBR_00ms     0x00
#define SDLYBR_01ms     0x01
#define SDLYBR_02ms     0x02
#define SDLYBR_03ms     0x03
#define SDLYBR_04ms     0x04
#define SDLYBR_05ms     0x05
#define SDLYBR_06ms     0x06
#define SDLYBR_07ms     0x07
#define SDLYBR_08ms     0x08
#define SDLYBR_09ms     0x09
#define SDLYBR_10ms     0x0A
#define SDLYBR_11ms     0x0B
#define SDLYBR_12ms     0x0C
#define SDLYBR_13ms     0x0D
#define SDLYBR_14ms     0x0E
#define SDLYBR_15ms     0x0F
//#define --        BIT7        // Reserved
//#define --        BIT6        // Reserved
//#define --        BIT5        // Reserved
//#define --        BIT4        // Reserved
//#define --        BIT3        // Reserved
//#define --        BIT2        // Reserved
//#define --        BIT1        // Reserved
#define SDV         BIT0        // Scan data valid   
#define KSO16CtrlReg	GPCRC3
#define KSO17CtrlReg	GPCRC5

//KBS调试打印
#if KBS_DEBUG 
#define kbs_dprint dprint
#else 
#define kbs_dprint(fmt, args...)
#endif

/*--------------------------------------------------------------------------*/
extern void Service_KBS(void);
extern BYTE KBS_Buffer_Get(BYTEP);
extern void KBS_Run(void);
extern void KBS_Set_KBS_INFO(VBYTE type_rate);
extern void KBD_CLear_Buffer(void);//zb 10.26 add
extern void KBS_Init(void);
extern BYTE KBS_Buffer_Input(BYTE   row_column);
extern BYTE  KBS_Buffer_puts(const BYTE* pntr);
extern void KBD_Default_Typematic(void);
extern void KBS_Clear_Break(void);
extern void KBD_Check_Repeat(void);
#include <KERNEL_XLATE.H>
#include <KERNEL_MEMORY.H>
#include <KERNEL_GPIO.H>
//-----------------------------------------------------------------------------
#endif
