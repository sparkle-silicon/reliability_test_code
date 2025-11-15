/*
 * @Author: Iversu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2025-10-18 20:25:13
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
#ifndef __KERNEL_GPIO_H
#define __KERNEL_GPIO_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "AE_FUNC.H"
#include "KERNEL_MEMORY.H"
 /*AE_API_GPIO_H*/
#define OutputMode_High			0x01
#define OutputMode_Inverse      0x02
#define InputMode_High			0x01
#define InputMode_Low			0x00
#define GPIO_Output				0x01
#define GPIO_Input				0x00
#define GPIO_Pullup				0x01
#define GPIO_Normal				0x00
#define PULLUP_LOW_REG			0x00
#define PULLUP_HIGH_REG			0x01
/* Power Swtich GPIOE[4] */
//#define SYSCTL_PORINT	(*((volatile uint32_t *) (0x304BC)))
#define Por_Int_Enable(type)	(SYSCTL_PORINT |= (0x1<<type))
#define Por_Int_Disable(type)	(SYSCTL_PORINT &= ~(0x1<<type))
#define Por_Int_Enable_Read(type)	((SYSCTL_PORINT & (0x1<<type)) != 0)
#define Por_Int_Status(type)	((SYSCTL_PORINT & (0x1<<(type+2))) != 0)

#define UARTB_SEL  SYSCTL_PIO_CFG &= ~BIT0;
#define SMBUS3_SEL  SYSCTL_PIO_CFG |=BIT0;
#if (COMMAND_UART_SWITCH == 3 || DEBUG_UART_SWITCH == 3 || PRINTF_UART_SWITCH == 3)
#if SMBUS3_CLOCK_EN
// #error 
#warning "You want use UARTB,but SMBUS3(IIC3) Enable, They reuse the same pin"
#endif
#define DEFAULT_SMBUS3_UARTB_SEL  UARTB_SEL
#else
#define DEFAULT_SMBUS3_UARTB_SEL  SMBUS3_SEL
#endif
//******************************************************************************************
//      Interrupt set and read
//******************************************************************************************
//set
#define GPIOA0_7_Int_Enable(IO_num)	    (GPIO0_INTEN0 |= (0x1<<IO_num))
#define GPIOA8_15_Int_Enable(IO_num)	(GPIO0_INTEN1 |= (0x1<<IO_num))
#define GPIOA16_23_Int_Enable(IO_num)	(GPIO0_INTEN2 |= (0x1<<IO_num))
#define GPIOA24_31_Int_Enable(IO_num)	(GPIO0_INTEN3 |= (0x1<<IO_num))
#define GPIOB0_7_Int_Enable(IO_num)	    (GPIO1_INTEN0 |= (0x1<<IO_num))
#define GPIOB8_15_Int_Enable(IO_num)	(GPIO1_INTEN1 |= (0x1<<IO_num))
#define GPIOB16_19_Int_Enable(IO_num)	(GPIO1_INTEN2 |= (0x1<<IO_num))
#define GPIOB20_26_Int_Enable(IO_num)	(GPIO1_INTEN3 |= (0x1<<IO_num))
#define GPIOC0_7_Int_Enable(IO_num)	    (GPIO2_INTEN0 |= (0x1<<IO_num))
#define GPIOC8_13_Int_Enable(IO_num)	(GPIO2_INTEN1 |= (0x1<<IO_num))
#define GPIOE0_7_Int_Enable(IO_num)	    (GPIO3_INTEN0 |= (0x1<<IO_num))
#define GPIOE8_15_Int_Enable(IO_num)	(GPIO3_INTEN1 |= (0x1<<IO_num))
#define GPIOE16_23_Int_Enable(IO_num)	(GPIO3_INTEN2 |= (0x1<<IO_num))

#define GPIOA0_7_Int_Disable(IO_num)	(GPIO0_INTEN0 &= ~(0x1<<IO_num))
#define GPIOA8_15_Int_Disable(IO_num)	(GPIO0_INTEN1 &= ~(0x1<<IO_num))
#define GPIOA16_23_Int_Disable(IO_num)	(GPIO0_INTEN2 &= ~(0x1<<IO_num))
#define GPIOA24_31_Int_Disable(IO_num)	(GPIO0_INTEN3 &= ~(0x1<<IO_num))
#define GPIOB0_7_Int_Disable(IO_num)	(GPIO1_INTEN0 &= ~(0x1<<IO_num))
#define GPIOB8_15_Int_Disable(IO_num)	(GPIO1_INTEN1 &= ~(0x1<<IO_num))
#define GPIOB16_19_Int_Disable(IO_num)	(GPIO1_INTEN2 &= ~(0x1<<IO_num))
#define GPIOB20_26_Int_Disable(IO_num)	(GPIO1_INTEN3 &= ~(0x1<<IO_num))
#define GPIOC0_7_Int_Disable(IO_num)	(GPIO2_INTEN0 &= ~(0x1<<IO_num))
#define GPIOC8_13_Int_Disable(IO_num)	(GPIO2_INTEN1 &= ~(0x1<<IO_num))
#define GPIOE0_7_Int_Disable(IO_num)	(GPIO3_INTEN0 &= ~(0x1<<IO_num))
#define GPIOE8_15_Int_Disable(IO_num)	(GPIO3_INTEN1 &= ~(0x1<<IO_num))
#define GPIOE16_23_Int_Disable(IO_num)	(GPIO3_INTEN2 &= ~(0x1<<IO_num))

#define GPIOA0_7_Int_Mask(IO_num)	(GPIO0_INTMASK0 |= (0x1<<IO_num))
#define GPIOA8_15_Int_Mask(IO_num)	(GPIO0_INTMASK1 |= (0x1<<IO_num))
#define GPIOA16_23_Int_Mask(IO_num)	(GPIO0_INTMASK2 |= (0x1<<IO_num))
#define GPIOA24_31_Int_Mask(IO_num)	(GPIO0_INTMASK3 |= (0x1<<IO_num))
#define GPIOB0_7_Int_Mask(IO_num)	(GPIO1_INTMASK0 |= (0x1<<IO_num))
#define GPIOB8_15_Int_Mask(IO_num)	(GPIO1_INTMASK1 |= (0x1<<IO_num))
#define GPIOB16_19_Int_Mask(IO_num)	(GPIO1_INTMASK2 |= (0x1<<IO_num))
#define GPIOB20_26_Int_Mask(IO_num)	(GPIO1_INTMASK3 |= (0x1<<IO_num))
#define GPIOC0_7_Int_Mask(IO_num)	(GPIO2_INTMASK0 |= (0x1<<IO_num))
#define GPIOC8_13_Int_Mask(IO_num)	(GPIO2_INTMASK1 |= (0x1<<IO_num))
#define GPIOE0_7_Int_Mask(IO_num)	(GPIO3_INTMASK0 |= (0x1<<IO_num))
#define GPIOE8_15_Int_Mask(IO_num)	(GPIO3_INTMASK1 |= (0x1<<IO_num))
#define GPIOE16_23_Int_Mask(IO_num)	(GPIO3_INTMASK2 |= (0x1<<IO_num))

#define GPIOA0_7_Int_Unmask(IO_num)	    (GPIO0_INTMASK0 &= ~(0x1<<IO_num))
#define GPIOA8_15_Int_Unmask(IO_num)	(GPIO0_INTMASK1 &= ~(0x1<<IO_num))
#define GPIOA16_23_Int_Unmask(IO_num)	(GPIO0_INTMASK2 &= ~(0x1<<IO_num))
#define GPIOA24_31_Int_Unmask(IO_num)	(GPIO0_INTMASK3 &= ~(0x1<<IO_num))
#define GPIOB0_7_Int_Unmask(IO_num)	    (GPIO1_INTMASK0 &= ~(0x1<<IO_num))
#define GPIOB8_15_Int_Unmask(IO_num)	(GPIO1_INTMASK1 &= ~(0x1<<IO_num))
#define GPIOB16_19_Int_Unmask(IO_num)	(GPIO1_INTMASK2 &= ~(0x1<<IO_num))
#define GPIOB20_26_Int_Unmask(IO_num)	(GPIO1_INTMASK3 &= ~(0x1<<IO_num))
#define GPIOC0_7_Int_Unmask(IO_num)	    (GPIO2_INTMASK0 &= ~(0x1<<IO_num))
#define GPIOC8_13_Int_Unmask(IO_num)	(GPIO2_INTMASK1 &= ~(0x1<<IO_num))
#define GPIOE0_7_Int_Unmask(IO_num)	    (GPIO3_INTMASK0 &= ~(0x1<<IO_num))
#define GPIOE8_15_Int_Unmask(IO_num)	(GPIO3_INTMASK1 &= ~(0x1<<IO_num))
#define GPIOE16_23_Int_Unmask(IO_num)	(GPIO3_INTMASK2 &= ~(0x1<<IO_num))

#define GPIOA0_7_Int_Type_Edge(IO_num)	    (GPIO0_INTTYPE_LEVEL0 |= (0x1<<IO_num))
#define GPIOA8_15_Int_Type_Edge(IO_num)	    (GPIO0_INTTYPE_LEVEL1 |= (0x1<<IO_num))
#define GPIOA16_23_Int_Type_Edge(IO_num)	(GPIO0_INTTYPE_LEVEL2 |= (0x1<<IO_num))
#define GPIOA24_31_Int_Type_Edge(IO_num)	(GPIO0_INTTYPE_LEVEL3 |= (0x1<<IO_num))
#define GPIOB0_7_Int_Type_Edge(IO_num)	    (GPIO1_INTTYPE_LEVEL0 |= (0x1<<IO_num))
#define GPIOB8_15_Int_Type_Edge(IO_num)	    (GPIO1_INTTYPE_LEVEL1 |= (0x1<<IO_num))
#define GPIOB16_19_Int_Type_Edge(IO_num)	(GPIO1_INTTYPE_LEVEL2 |= (0x1<<IO_num))
#define GPIOB20_26_Int_Type_Edge(IO_num)	(GPIO1_INTTYPE_LEVEL3 |= (0x1<<IO_num))
#define GPIOC0_7_Int_Type_Edge(IO_num)	    (GPIO2_INTTYPE_LEVEL0 |= (0x1<<IO_num))
#define GPIOC8_13_Int_Type_Edge(IO_num)	    (GPIO2_INTTYPE_LEVEL1 |= (0x1<<IO_num))
#define GPIOE0_7_Int_Type_Edge(IO_num)	    (GPIO3_INTTYPE_LEVEL0 |= (0x1<<IO_num))
#define GPIOE8_15_Int_Type_Edge(IO_num)	    (GPIO3_INTTYPE_LEVEL1 |= (0x1<<IO_num))
#define GPIOE16_23_Int_Type_Edge(IO_num)	(GPIO3_INTTYPE_LEVEL2 |= (0x1<<IO_num))

#define GPIOA0_7_Int_Type_Level(IO_num)	    (GPIO0_INTTYPE_LEVEL0 &= ~(0x1<<IO_num))
#define GPIOA8_15_Int_Type_Level(IO_num)	(GPIO0_INTTYPE_LEVEL1 &= ~(0x1<<IO_num))
#define GPIOA16_23_Int_Type_Level(IO_num)	(GPIO0_INTTYPE_LEVEL2 &= ~(0x1<<IO_num))
#define GPIOA24_31_Int_Type_Level(IO_num)	(GPIO0_INTTYPE_LEVEL3 &= ~(0x1<<IO_num))
#define GPIOB0_7_Int_Type_Level(IO_num)	    (GPIO1_INTTYPE_LEVEL0 &= ~(0x1<<IO_num))
#define GPIOB8_15_Int_Type_Level(IO_num)	(GPIO1_INTTYPE_LEVEL1 &= ~(0x1<<IO_num))
#define GPIOB16_19_Int_Type_Level(IO_num)	(GPIO1_INTTYPE_LEVEL2 &= ~(0x1<<IO_num))
#define GPIOB20_26_Int_Type_Level(IO_num)	(GPIO1_INTTYPE_LEVEL3 &= ~(0x1<<IO_num))
#define GPIOC0_7_Int_Type_Level(IO_num)	    (GPIO2_INTTYPE_LEVEL0 &= ~(0x1<<IO_num))
#define GPIOC8_13_Int_Type_Level(IO_num)	(GPIO2_INTTYPE_LEVEL1 &= ~(0x1<<IO_num))
#define GPIOE0_7_Int_Type_Level(IO_num)	    (GPIO3_INTTYPE_LEVEL0 &= ~(0x1<<IO_num))
#define GPIOE8_15_Int_Type_Level(IO_num)	(GPIO3_INTTYPE_LEVEL1 &= ~(0x1<<IO_num))
#define GPIOE16_23_Int_Type_Level(IO_num)	(GPIO3_INTTYPE_LEVEL2 &= ~(0x1<<IO_num))

#define GPIOA0_7_Int_Polarity_High(IO_num)	    (GPIO0_INT_POLARITY0 |= (0x1<<IO_num))
#define GPIOA8_15_Int_Polarity_High(IO_num)	    (GPIO0_INT_POLARITY1 |= (0x1<<IO_num))
#define GPIOA16_23_Int_Polarity_High(IO_num)	(GPIO0_INT_POLARITY2 |= (0x1<<IO_num))
#define GPIOA24_31_Int_Polarity_High(IO_num)	(GPIO0_INT_POLARITY3 |= (0x1<<IO_num))
#define GPIOB0_7_Int_Polarity_High(IO_num)	    (GPIO1_INT_POLARITY0 |= (0x1<<IO_num))
#define GPIOB8_15_Int_Polarity_High(IO_num)	    (GPIO1_INT_POLARITY1 |= (0x1<<IO_num))
#define GPIOB16_19_Int_Polarity_High(IO_num)	(GPIO1_INT_POLARITY2 |= (0x1<<IO_num))
#define GPIOB20_26_Int_Polarity_High(IO_num)	(GPIO1_INT_POLARITY3 |= (0x1<<IO_num))
#define GPIOC0_7_Int_Polarity_High(IO_num)	    (GPIO2_INT_POLARITY0 |= (0x1<<IO_num))
#define GPIOC8_13_Int_Polarity_High(IO_num)	    (GPIO2_INT_POLARITY1 |= (0x1<<IO_num))
#define GPIOE0_7_Int_Polarity_High(IO_num)	    (GPIO3_INT_POLARITY0 |= (0x1<<IO_num))
#define GPIOE8_15_Int_Polarity_High(IO_num)	    (GPIO3_INT_POLARITY1 |= (0x1<<IO_num))
#define GPIOE16_23_Int_Polarity_High(IO_num)	(GPIO3_INT_POLARITY2 |= (0x1<<IO_num))

#define GPIOA0_7_Int_Polarity_Low(IO_num)	(GPIO0_INT_POLARITY0 &= ~(0x1<<IO_num))
#define GPIOA8_15_Int_Polarity_Low(IO_num)	(GPIO0_INT_POLARITY1 &= ~(0x1<<IO_num))
#define GPIOA16_23_Int_Polarity_Low(IO_num)	(GPIO0_INT_POLARITY2 &= ~(0x1<<IO_num))
#define GPIOA24_31_Int_Polarity_Low(IO_num)	(GPIO0_INT_POLARITY3 &= ~(0x1<<IO_num))
#define GPIOB0_7_Int_Polarity_Low(IO_num)	(GPIO1_INT_POLARITY0 &= ~(0x1<<IO_num))
#define GPIOB8_15_Int_Polarity_Low(IO_num)	(GPIO1_INT_POLARITY1 &= ~(0x1<<IO_num))
#define GPIOB16_19_Int_Polarity_Low(IO_num)	(GPIO1_INT_POLARITY2 &= ~(0x1<<IO_num))
#define GPIOB20_26_Int_Polarity_Low(IO_num)	(GPIO1_INT_POLARITY3 &= ~(0x1<<IO_num))
#define GPIOC0_7_Int_Polarity_Low(IO_num)	(GPIO2_INT_POLARITY0 &= ~(0x1<<IO_num))
#define GPIOC8_13_Int_Polarity_Low(IO_num)	(GPIO2_INT_POLARITY1 &= ~(0x1<<IO_num))
#define GPIOE0_7_Int_Polarity_Low(IO_num)	(GPIO3_INT_POLARITY0 &= ~(0x1<<IO_num))
#define GPIOE8_15_Int_Polarity_Low(IO_num)	(GPIO3_INT_POLARITY1 &= ~(0x1<<IO_num))
#define GPIOE16_23_Int_Polarity_Low(IO_num)	(GPIO3_INT_POLARITY2 &= ~(0x1<<IO_num))

//Read
#define GPIOA0_7_Int_Enable_Read(IO_num)	((GPIO0_INTEN0 & (0x1<<IO_num))  != 0)
#define GPIOA8_15_Int_Enable_Read(IO_num)	((GPIO0_INTEN1 & (0x1<<IO_num))  != 0)
#define GPIOA16_23_Int_Enable_Read(IO_num)	((GPIO0_INTEN2 & (0x1<<IO_num))  != 0)
#define GPIOA24_31_Int_Enable_Read(IO_num)	((GPIO0_INTEN3 & (0x1<<IO_num))  != 0)
#define GPIOB0_7_Int_Enable_Read(IO_num)	((GPIO1_INTEN0 & (0x1<<IO_num))  != 0)
#define GPIOB8_15_Int_Enable_Read(IO_num)	((GPIO1_INTEN1 & (0x1<<IO_num))  != 0)
#define GPIOB16_19_Int_Enable_Read(IO_num)	((GPIO1_INTEN2 & (0x1<<IO_num))  != 0)
#define GPIOB20_26_Int_Enable_Read(IO_num)	((GPIO1_INTEN3 & (0x1<<IO_num))  != 0)
#define GPIOC0_7_Int_Enable_Read(IO_num)	((GPIO2_INTEN0 & (0x1<<IO_num))  != 0)
#define GPIOC8_13_Int_Enable_Read(IO_num)	((GPIO2_INTEN1 & (0x1<<IO_num))  != 0)
#define GPIOE0_7_Int_Enable_Read(IO_num)	((GPIO3_INTEN0 & (0x1<<IO_num))  != 0)
#define GPIOE8_15_Int_Enable_Read(IO_num)	((GPIO3_INTEN1 & (0x1<<IO_num))  != 0)
#define GPIOE16_23_Int_Enable_Read(IO_num)	((GPIO3_INTEN2 & (0x1<<IO_num))  != 0)

#define GPIOA0_7_Int_Mask_Read(IO_num)	    ((GPIO0_INTMASK0 & (0x1<<IO_num)) != 0)
#define GPIOA8_15_Int_Mask_Read(IO_num)	    ((GPIO0_INTMASK1 & (0x1<<IO_num)) != 0)
#define GPIOA16_23_Int_Mask_Read(IO_num)	((GPIO0_INTMASK2 & (0x1<<IO_num)) != 0)
#define GPIOA24_31_Int_Mask_Read(IO_num)	((GPIO0_INTMASK3 & (0x1<<IO_num)) != 0)
#define GPIOB0_7_Int_Mask_Read(IO_num)	    ((GPIO1_INTMASK0 & (0x1<<IO_num)) != 0)
#define GPIOB8_15_Int_Mask_Read(IO_num)	    ((GPIO1_INTMASK1 & (0x1<<IO_num)) != 0)
#define GPIOB16_19_Int_Mask_Read(IO_num)	((GPIO1_INTMASK2 & (0x1<<IO_num)) != 0)
#define GPIOB20_26_Int_Mask_Read(IO_num)	((GPIO1_INTMASK3 & (0x1<<IO_num)) != 0)
#define GPIOC0_7_Int_Mask_Read(IO_num)	    ((GPIO2_INTMASK0 & (0x1<<IO_num)) != 0)
#define GPIOC8_13_Int_Mask_Read(IO_num)	    ((GPIO2_INTMASK1 & (0x1<<IO_num)) != 0)
#define GPIOE0_7_Int_Mask_Read(IO_num)	    ((GPIO3_INTMASK0 & (0x1<<IO_num)) != 0)
#define GPIOE8_15_Int_Mask_Read(IO_num)	    ((GPIO3_INTMASK1 & (0x1<<IO_num)) != 0)
#define GPIOE16_23_Int_Mask_Read(IO_num)	((GPIO3_INTMASK2 & (0x1<<IO_num)) != 0)

#define GPIOA0_7_Int_Type_Read(IO_num)	    ((GPIO0_INTTYPE_LEVEL0 & (0x1<<IO_num)) != 0)
#define GPIOA8_15_Int_Type_Read(IO_num)	    ((GPIO0_INTTYPE_LEVEL1 & (0x1<<IO_num)) != 0)
#define GPIOA16_23_Int_Type_Read(IO_num)	((GPIO0_INTTYPE_LEVEL2 & (0x1<<IO_num)) != 0)
#define GPIOA24_31_Int_Type_Read(IO_num)	((GPIO0_INTTYPE_LEVEL3 & (0x1<<IO_num)) != 0)
#define GPIOB0_7_Int_Type_Read(IO_num)	    ((GPIO1_INTTYPE_LEVEL0 & (0x1<<IO_num)) != 0)
#define GPIOB8_15_Int_Type_Read(IO_num)	    ((GPIO1_INTTYPE_LEVEL1 & (0x1<<IO_num)) != 0)
#define GPIOB16_19_Int_Type_Read(IO_num)	((GPIO1_INTTYPE_LEVEL2 & (0x1<<IO_num)) != 0)
#define GPIOB20_26_Int_Type_Read(IO_num)	((GPIO1_INTTYPE_LEVEL3 & (0x1<<IO_num)) != 0)
#define GPIOC0_7_Int_Type_Read(IO_num)	    ((GPIO2_INTTYPE_LEVEL0 & (0x1<<IO_num)) != 0)
#define GPIOC8_13_Int_Type_Read(IO_num)	    ((GPIO2_INTTYPE_LEVEL1 & (0x1<<IO_num)) != 0)
#define GPIOE0_7_Int_Type_Read(IO_num)	    ((GPIO3_INTTYPE_LEVEL0 & (0x1<<IO_num)) != 0)
#define GPIOE8_15_Int_Type_Read(IO_num) 	((GPIO3_INTTYPE_LEVEL1 & (0x1<<IO_num)) != 0)
#define GPIOE16_23_Int_Type_Read(IO_num)	((GPIO3_INTTYPE_LEVEL2 & (0x1<<IO_num)) != 0)

#define GPIOA0_7_Int_Polarity_Read(IO_num)	    ((GPIO0_INT_POLARITY0 & (0x1<<IO_num)) != 0)
#define GPIOA8_15_Int_Polarity_Read(IO_num)	    ((GPIO0_INT_POLARITY1 & (0x1<<IO_num)) != 0)
#define GPIOA16_23_Int_Polarity_Read(IO_num)	((GPIO0_INT_POLARITY2 & (0x1<<IO_num)) != 0)
#define GPIOA24_31_Int_Polarity_Read(IO_num)	((GPIO0_INT_POLARITY3 & (0x1<<IO_num)) != 0)
#define GPIOB0_7_Int_Polarity_Read(IO_num)	    ((GPIO1_INT_POLARITY0 & (0x1<<IO_num)) != 0)
#define GPIOB8_15_Int_Polarity_Read(IO_num)	    ((GPIO1_INT_POLARITY1 & (0x1<<IO_num)) != 0)
#define GPIOB16_19_Int_Polarity_Read(IO_num)	((GPIO1_INT_POLARITY2 & (0x1<<IO_num)) != 0)
#define GPIOB20_26_Int_Polarity_Read(IO_num)	((GPIO1_INT_POLARITY3 & (0x1<<IO_num)) != 0)
#define GPIOC0_7_Int_Polarity_Read(IO_num)	    ((GPIO2_INT_POLARITY0 & (0x1<<IO_num)) != 0)
#define GPIOC8_13_Int_Polarity_Read(IO_num)	    ((GPIO2_INT_POLARITY1 & (0x1<<IO_num)) != 0)
#define GPIOE0_7_Int_Polarity_Read(IO_num)	    ((GPIO3_INT_POLARITY0 & (0x1<<IO_num)) != 0)
#define GPIOE8_15_Int_Polarity_Read(IO_num)	    ((GPIO3_INT_POLARITY1 & (0x1<<IO_num)) != 0)
#define GPIOE16_23_Int_Polarity_Read(IO_num)	((GPIO3_INT_POLARITY2 & (0x1<<IO_num)) != 0)

#define GPIOA0_7_Int_Status(IO_num)	    ((GPIO0_INTSTATUS0 & (0x1<<IO_num))  != 0)
#define GPIOA8_15_Int_Status(IO_num)	((GPIO0_INTSTATUS1 & (0x1<<IO_num))  != 0)
#define GPIOA16_23_Int_Status(IO_num)	((GPIO0_INTSTATUS2 & (0x1<<IO_num))  != 0)
#define GPIOA24_31_Int_Status(IO_num)	((GPIO0_INTSTATUS3 & (0x1<<IO_num))  != 0)
#define GPIOB0_7_Int_Status(IO_num)	    ((GPIO1_INTSTATUS0 & (0x1<<IO_num))  != 0)
#define GPIOB8_15_Int_Status(IO_num)	((GPIO1_INTSTATUS1 & (0x1<<IO_num))  != 0)
#define GPIOB16_19_Int_Status(IO_num)	((GPIO1_INTSTATUS2 & (0x1<<IO_num))  != 0)
#define GPIOB20_26_Int_Status(IO_num)	((GPIO1_INTSTATUS3 & (0x1<<IO_num))  != 0)
#define GPIOC0_7_Int_Status(IO_num)	    ((GPIO2_INTSTATUS0 & (0x1<<IO_num))  != 0)
#define GPIOC8_13_Int_Status(IO_num)	((GPIO2_INTSTATUS1 & (0x1<<IO_num))  != 0)
#define GPIOE0_7_Int_Status(IO_num)	    ((GPIO3_INTSTATUS0 & (0x1<<IO_num))  != 0)
#define GPIOE8_15_Int_Status(IO_num)	((GPIO3_INTSTATUS1 & (0x1<<IO_num))  != 0)
#define GPIOE16_23_Int_Status(IO_num)	((GPIO3_INTSTATUS2 & (0x1<<IO_num))  != 0)

#define GPIOA HEX01
#define GPIOB HEX02
#define GPIOC HEX03
#define GPIOD HEX04
#define GPIOE HEX05
#define NUM_OF_GPIOA 32
#define NUM_OF_GPIOB 27
#define NUM_OF_GPIOC 14
#define NUM_OF_GPIOD 7
#define NUM_OF_GPIOE 24
/************GPIO Register define***************/
struct GPIOPinState {
	BYTE default_state_idx;	// default state is mux_state[default_state_idx]
	BYTE mux_state[4]; // 0 is input, 1 is output
};
extern struct GPIOPinState gpio_state_table[];
//******************************************************************************************
//	Extern functions
//******************************************************************************************
extern void sysctl_iomux_uart0(void);
extern void sysctl_iomux_uart1(uint32_t tx_sel, uint32_t rx_sel);
extern void sysctl_iomux_uarta(uint32_t tx_sel, uint32_t rx_sel);
extern void sysctl_iomux_uartb(void);
extern void sysctl_iomux_disable_uart0(void);
extern void sysctl_iomux_disable_uart1(uint32_t tx_sel, uint32_t rx_sel);
extern void sysctl_iomux_disable_uarta(uint32_t tx_sel, uint32_t rx_sel);
extern void sysctl_iomux_disable_uartb(void);

extern void sysctl_iomux_ps2_0(uint32_t clk_sel, uint32_t data_sel);
extern void sysctl_iomux_ps2_1(uint32_t clk_sel, uint32_t data_sel);

extern void sysctl_iomux_spim(uint8_t mosi_sel, uint8_t miso_sel, uint8_t qe_sel);
extern void sysctl_iomux_spim_cs(uint8_t cs0_sel, uint8_t cs1_sel);
extern void sysctl_iomux_spif(uint8_t csn_sel, uint8_t qe_sel, uint8_t wp_sel);

extern void sysctl_iomux_pwm0(void);
extern void sysctl_iomux_pwm1(void);
extern void sysctl_iomux_pwm2(void);
extern void sysctl_iomux_pwm3(void);
extern void sysctl_iomux_pwm4(void);
extern void sysctl_iomux_pwm5(void);
extern void sysctl_iomux_pwm6(void);
extern void sysctl_iomux_pwm7(void);
extern void sysctl_iomux_tach0(uint32_t tach_sel);
extern void sysctl_iomux_tach1(uint32_t tach_sel);
extern void sysctl_iomux_tach2(void);
extern void sysctl_iomux_tach3(uint32_t tach_sel);

extern void sysctl_iomux_i2c0(uint32_t clk_sel, uint32_t data_sel);
extern void sysctl_iomux_i2c1(void);
extern void sysctl_iomux_i2c2(uint32_t clk_sel);
extern void sysctl_iomux_i2c3(void);
extern void sysctl_iomux_i2c4(void);
extern void sysctl_iomux_i2c5(void);
extern void sysctl_iomux_i2c6(void);
extern void sysctl_iomux_i2c7(void);
extern void sysctl_iomux_i2c8(void);

extern void sysctl_iomux_master0(void);
extern void sysctl_iomux_master1(void);
extern void sysctl_iomux_slave0(void);
extern void sysctl_iomux_slave1(void);

extern void sysctl_iomux_adc0(void);
extern void sysctl_iomux_adc1(void);
extern void sysctl_iomux_adc2(void);
extern void sysctl_iomux_adc3(void);
extern void sysctl_iomux_adc4(void);
extern void sysctl_iomux_adc5(void);
extern void sysctl_iomux_adc6(void);
extern void sysctl_iomux_adc7(void);

extern void sysctl_iomux_cec0(uint8_t cec0_sel);
extern void sysctl_iomux_cec1(void);

extern void sysctl_iomux_gpioe(void);
extern void sysctl_iomux_kbs(uint32_t kbsn_switch);
extern void sysctl_iomux_crypto_jtag(void);
extern void sysctl_iomux_jtag(void);

extern void sysctl_iomux_ri(void);
extern void sysctl_iomux_ring(void);
extern void sysctl_iomux_host(uint8_t espi_sel, uint8_t irq_sel, uint8_t rst_sel, uint8_t clkrun_sel, uint8_t lpcpd_sel);
extern void sysctl_iomux_sci(uint8_t sci_sel);
extern void sysctl_iomux_pwureq(void);
extern void sysctl_iomux_smi_pltrst(uint8_t smi_pltrst_sel);
extern void sysctl_iomux_kbrst(void);
extern void sysctl_iomux_g20(void);
extern void sysctl_iomux_swuc(uint8_t ga20_en, uint8_t krst_en, uint8_t pwureq_en, uint8_t smi_pltrst_sel);
extern void sysctl_iomux_l80(void);

extern void sysctl_iomux_peci(void);

extern void i2c0_pull_up(uint32_t clk_sel, uint32_t data_sel);
extern void i2c1_pull_up(void);
extern void i2c2_pull_up(uint32_t clk_sel);
extern void i2c3_pull_up(void);
extern void i2c4_pull_up(void);
extern void i2c5_pull_up(void);
extern void i2c6_pull_up(void);
extern void i2c7_pull_up(void);
extern void i2c8_pull_up(void);

extern void i3c0_pull_up(void);
extern void i3c1_pull_up(void);
extern void i3c2_pull_up(void);
extern void i3c3_pull_up(void);

extern void tach0_pull_up(uint32_t tach_sel);
extern void tach1_pull_up(uint32_t tach_sel);
extern void tach2_pull_up(void);
extern void tach3_pull_up(uint32_t tach_sel);

extern void ps2_0_pull_up(uint32_t clk_sel, uint32_t data_sel);
extern void ps2_1_pull_up(uint32_t clk_sel, uint32_t data_sel);

extern void kbs_pull_up(uint32_t kbsn_switch);
extern BYTE  PIO_Pullup_Config(BYTE  GPIO, BYTE  Num);
extern char IsGpioOut(DWORD port, DWORD io);
extern int GPIO_Config(int GPIO, int gpio_no, int mode, int op_val, int int_lv, int pol);
extern void sysctl_iomux_config(DWORD port, DWORD io, unsigned port_type);
extern void sysctl_mod_reset_finish(DWORD module);
extern void sysctl_mod_reset_start(DWORD module);
extern void sysctl_mod_enable(DWORD module);
extern void sysctl_mod_disable(DWORD module);
extern void GPIO_Input_EN(int GPIO, int gpio_no, char sw);
extern char GPIOAutoTest(void);
char ReadGPIOLevel(BYTE port, BYTE pin);
#include "CUSTOM_GPIO.H"
#endif /* _NUCLEI_GPIO_H */
