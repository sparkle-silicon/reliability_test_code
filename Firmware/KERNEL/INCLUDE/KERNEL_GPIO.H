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

/* Power Swtich PIOB[4] */
#define Por_Int_Enable(type)	    (SYSCTL_PORINT |= (0x1<<type))
#define Por_Int_Disable(type)	    (SYSCTL_PORINT &= ~(0x1<<type))
#define Por_Int_Enable_Read(type)	((SYSCTL_PORINT & (0x1<<type)) != 0)
#define Por_Int_Status(type)	    ((SYSCTL_PORINT & (0x1<<(type+2))) != 0)

/*GPIO AF*/
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
#define GPIOA0_7_Int_Enable(IO_num)	          (GPIO0_INTEN0 |= (0x1<<IO_num))
#define GPIOA8_15_Int_Enable(IO_num)	        (GPIO0_INTEN1 |= (0x1<<IO_num))
#define GPIOA16_23_Int_Enable(IO_num)	        (GPIO0_INTEN2 |= (0x1<<IO_num))
#define GPIOA24_31_Int_Enable(IO_num)	        (GPIO0_INTEN3 |= (0x1<<IO_num))
#define GPIOB0_7_Int_Enable(IO_num)	          (GPIO1_INTEN0 |= (0x1<<IO_num))
#define GPIOB8_15_Int_Enable(IO_num)	        (GPIO1_INTEN1 |= (0x1<<IO_num))
#define GPIOB16_19_Int_Enable(IO_num)	        (GPIO1_INTEN2 |= (0x1<<IO_num))
#define GPIOB20_26_Int_Enable(IO_num)	        (GPIO1_INTEN3 |= (0x1<<IO_num))
#define GPIOC0_7_Int_Enable(IO_num)	          (GPIO2_INTEN0 |= (0x1<<IO_num))
#define GPIOC8_13_Int_Enable(IO_num)	        (GPIO2_INTEN1 |= (0x1<<IO_num))
#define GPIOE0_7_Int_Enable(IO_num)	          (GPIO3_INTEN0 |= (0x1<<IO_num))
#define GPIOE8_15_Int_Enable(IO_num)	        (GPIO3_INTEN1 |= (0x1<<IO_num))
#define GPIOE16_23_Int_Enable(IO_num)	        (GPIO3_INTEN2 |= (0x1<<IO_num))

#define GPIOA0_7_Int_Disable(IO_num)	        (GPIO0_INTEN0 &= ~(0x1<<IO_num))
#define GPIOA8_15_Int_Disable(IO_num)	        (GPIO0_INTEN1 &= ~(0x1<<IO_num))
#define GPIOA16_23_Int_Disable(IO_num)	      (GPIO0_INTEN2 &= ~(0x1<<IO_num))
#define GPIOA24_31_Int_Disable(IO_num)	      (GPIO0_INTEN3 &= ~(0x1<<IO_num))
#define GPIOB0_7_Int_Disable(IO_num)	        (GPIO1_INTEN0 &= ~(0x1<<IO_num))
#define GPIOB8_15_Int_Disable(IO_num)	        (GPIO1_INTEN1 &= ~(0x1<<IO_num))
#define GPIOB16_19_Int_Disable(IO_num)	      (GPIO1_INTEN2 &= ~(0x1<<IO_num))
#define GPIOB20_26_Int_Disable(IO_num)	      (GPIO1_INTEN3 &= ~(0x1<<IO_num))
#define GPIOC0_7_Int_Disable(IO_num)	        (GPIO2_INTEN0 &= ~(0x1<<IO_num))
#define GPIOC8_13_Int_Disable(IO_num)	        (GPIO2_INTEN1 &= ~(0x1<<IO_num))
#define GPIOE0_7_Int_Disable(IO_num)	        (GPIO3_INTEN0 &= ~(0x1<<IO_num))
#define GPIOE8_15_Int_Disable(IO_num)	        (GPIO3_INTEN1 &= ~(0x1<<IO_num))
#define GPIOE16_23_Int_Disable(IO_num)	      (GPIO3_INTEN2 &= ~(0x1<<IO_num))

#define GPIOA0_7_Int_Mask(IO_num)	            (GPIO0_INTMASK0 |= (0x1<<IO_num))
#define GPIOA8_15_Int_Mask(IO_num)	          (GPIO0_INTMASK1 |= (0x1<<IO_num))
#define GPIOA16_23_Int_Mask(IO_num)	          (GPIO0_INTMASK2 |= (0x1<<IO_num))
#define GPIOA24_31_Int_Mask(IO_num)	          (GPIO0_INTMASK3 |= (0x1<<IO_num))
#define GPIOB0_7_Int_Mask(IO_num)	            (GPIO1_INTMASK0 |= (0x1<<IO_num))
#define GPIOB8_15_Int_Mask(IO_num)	          (GPIO1_INTMASK1 |= (0x1<<IO_num))
#define GPIOB16_19_Int_Mask(IO_num)	          (GPIO1_INTMASK2 |= (0x1<<IO_num))
#define GPIOB20_26_Int_Mask(IO_num)	          (GPIO1_INTMASK3 |= (0x1<<IO_num))
#define GPIOC0_7_Int_Mask(IO_num)	            (GPIO2_INTMASK0 |= (0x1<<IO_num))
#define GPIOC8_13_Int_Mask(IO_num)	          (GPIO2_INTMASK1 |= (0x1<<IO_num))
#define GPIOE0_7_Int_Mask(IO_num)	            (GPIO3_INTMASK0 |= (0x1<<IO_num))
#define GPIOE8_15_Int_Mask(IO_num)	          (GPIO3_INTMASK1 |= (0x1<<IO_num))
#define GPIOE16_23_Int_Mask(IO_num)	          (GPIO3_INTMASK2 |= (0x1<<IO_num))

#define GPIOA0_7_Int_Unmask(IO_num)	          (GPIO0_INTMASK0 &= ~(0x1<<IO_num))
#define GPIOA8_15_Int_Unmask(IO_num)	        (GPIO0_INTMASK1 &= ~(0x1<<IO_num))
#define GPIOA16_23_Int_Unmask(IO_num)	        (GPIO0_INTMASK2 &= ~(0x1<<IO_num))
#define GPIOA24_31_Int_Unmask(IO_num)	        (GPIO0_INTMASK3 &= ~(0x1<<IO_num))
#define GPIOB0_7_Int_Unmask(IO_num)	          (GPIO1_INTMASK0 &= ~(0x1<<IO_num))
#define GPIOB8_15_Int_Unmask(IO_num)	        (GPIO1_INTMASK1 &= ~(0x1<<IO_num))
#define GPIOB16_19_Int_Unmask(IO_num)	        (GPIO1_INTMASK2 &= ~(0x1<<IO_num))
#define GPIOB20_26_Int_Unmask(IO_num)	        (GPIO1_INTMASK3 &= ~(0x1<<IO_num))
#define GPIOC0_7_Int_Unmask(IO_num)	          (GPIO2_INTMASK0 &= ~(0x1<<IO_num))
#define GPIOC8_13_Int_Unmask(IO_num)	        (GPIO2_INTMASK1 &= ~(0x1<<IO_num))
#define GPIOE0_7_Int_Unmask(IO_num)	          (GPIO3_INTMASK0 &= ~(0x1<<IO_num))
#define GPIOE8_15_Int_Unmask(IO_num)	        (GPIO3_INTMASK1 &= ~(0x1<<IO_num))
#define GPIOE16_23_Int_Unmask(IO_num)	        (GPIO3_INTMASK2 &= ~(0x1<<IO_num))

#define GPIOA0_7_Int_Type_Edge(IO_num)	      (GPIO0_INTTYPE_LEVEL0 |= (0x1<<IO_num))
#define GPIOA8_15_Int_Type_Edge(IO_num)	      (GPIO0_INTTYPE_LEVEL1 |= (0x1<<IO_num))
#define GPIOA16_23_Int_Type_Edge(IO_num)	    (GPIO0_INTTYPE_LEVEL2 |= (0x1<<IO_num))
#define GPIOA24_31_Int_Type_Edge(IO_num)	    (GPIO0_INTTYPE_LEVEL3 |= (0x1<<IO_num))
#define GPIOB0_7_Int_Type_Edge(IO_num)	      (GPIO1_INTTYPE_LEVEL0 |= (0x1<<IO_num))
#define GPIOB8_15_Int_Type_Edge(IO_num)	      (GPIO1_INTTYPE_LEVEL1 |= (0x1<<IO_num))
#define GPIOB16_19_Int_Type_Edge(IO_num)	    (GPIO1_INTTYPE_LEVEL2 |= (0x1<<IO_num))
#define GPIOB20_26_Int_Type_Edge(IO_num)	    (GPIO1_INTTYPE_LEVEL3 |= (0x1<<IO_num))
#define GPIOC0_7_Int_Type_Edge(IO_num)	      (GPIO2_INTTYPE_LEVEL0 |= (0x1<<IO_num))
#define GPIOC8_13_Int_Type_Edge(IO_num)	      (GPIO2_INTTYPE_LEVEL1 |= (0x1<<IO_num))
#define GPIOE0_7_Int_Type_Edge(IO_num)	      (GPIO3_INTTYPE_LEVEL0 |= (0x1<<IO_num))
#define GPIOE8_15_Int_Type_Edge(IO_num)	      (GPIO3_INTTYPE_LEVEL1 |= (0x1<<IO_num))
#define GPIOE16_23_Int_Type_Edge(IO_num)	    (GPIO3_INTTYPE_LEVEL2 |= (0x1<<IO_num))

#define GPIOA0_7_Int_Type_Level(IO_num)	      (GPIO0_INTTYPE_LEVEL0 &= ~(0x1<<IO_num))
#define GPIOA8_15_Int_Type_Level(IO_num)	    (GPIO0_INTTYPE_LEVEL1 &= ~(0x1<<IO_num))
#define GPIOA16_23_Int_Type_Level(IO_num)	    (GPIO0_INTTYPE_LEVEL2 &= ~(0x1<<IO_num))
#define GPIOA24_31_Int_Type_Level(IO_num)	    (GPIO0_INTTYPE_LEVEL3 &= ~(0x1<<IO_num))
#define GPIOB0_7_Int_Type_Level(IO_num)	      (GPIO1_INTTYPE_LEVEL0 &= ~(0x1<<IO_num))
#define GPIOB8_15_Int_Type_Level(IO_num)	    (GPIO1_INTTYPE_LEVEL1 &= ~(0x1<<IO_num))
#define GPIOB16_19_Int_Type_Level(IO_num)	    (GPIO1_INTTYPE_LEVEL2 &= ~(0x1<<IO_num))
#define GPIOB20_26_Int_Type_Level(IO_num)	    (GPIO1_INTTYPE_LEVEL3 &= ~(0x1<<IO_num))
#define GPIOC0_7_Int_Type_Level(IO_num)	      (GPIO2_INTTYPE_LEVEL0 &= ~(0x1<<IO_num))
#define GPIOC8_13_Int_Type_Level(IO_num)	    (GPIO2_INTTYPE_LEVEL1 &= ~(0x1<<IO_num))
#define GPIOE0_7_Int_Type_Level(IO_num)	      (GPIO3_INTTYPE_LEVEL0 &= ~(0x1<<IO_num))
#define GPIOE8_15_Int_Type_Level(IO_num)	    (GPIO3_INTTYPE_LEVEL1 &= ~(0x1<<IO_num))
#define GPIOE16_23_Int_Type_Level(IO_num)	    (GPIO3_INTTYPE_LEVEL2 &= ~(0x1<<IO_num))

#define GPIOA0_7_Int_Polarity_High(IO_num)	  (GPIO0_INT_POLARITY0 |= (0x1<<IO_num))
#define GPIOA8_15_Int_Polarity_High(IO_num)	  (GPIO0_INT_POLARITY1 |= (0x1<<IO_num))
#define GPIOA16_23_Int_Polarity_High(IO_num)	(GPIO0_INT_POLARITY2 |= (0x1<<IO_num))
#define GPIOA24_31_Int_Polarity_High(IO_num)	(GPIO0_INT_POLARITY3 |= (0x1<<IO_num))
#define GPIOB0_7_Int_Polarity_High(IO_num)	  (GPIO1_INT_POLARITY0 |= (0x1<<IO_num))
#define GPIOB8_15_Int_Polarity_High(IO_num)	  (GPIO1_INT_POLARITY1 |= (0x1<<IO_num))
#define GPIOB16_19_Int_Polarity_High(IO_num)	(GPIO1_INT_POLARITY2 |= (0x1<<IO_num))
#define GPIOB20_26_Int_Polarity_High(IO_num)	(GPIO1_INT_POLARITY3 |= (0x1<<IO_num))
#define GPIOC0_7_Int_Polarity_High(IO_num)	  (GPIO2_INT_POLARITY0 |= (0x1<<IO_num))
#define GPIOC8_13_Int_Polarity_High(IO_num)	  (GPIO2_INT_POLARITY1 |= (0x1<<IO_num))
#define GPIOE0_7_Int_Polarity_High(IO_num)	  (GPIO3_INT_POLARITY0 |= (0x1<<IO_num))
#define GPIOE8_15_Int_Polarity_High(IO_num)	  (GPIO3_INT_POLARITY1 |= (0x1<<IO_num))
#define GPIOE16_23_Int_Polarity_High(IO_num)	(GPIO3_INT_POLARITY2 |= (0x1<<IO_num))

#define GPIOA0_7_Int_Polarity_Low(IO_num)	    (GPIO0_INT_POLARITY0 &= ~(0x1<<IO_num))
#define GPIOA8_15_Int_Polarity_Low(IO_num)	  (GPIO0_INT_POLARITY1 &= ~(0x1<<IO_num))
#define GPIOA16_23_Int_Polarity_Low(IO_num)	  (GPIO0_INT_POLARITY2 &= ~(0x1<<IO_num))
#define GPIOA24_31_Int_Polarity_Low(IO_num)	  (GPIO0_INT_POLARITY3 &= ~(0x1<<IO_num))
#define GPIOB0_7_Int_Polarity_Low(IO_num)	    (GPIO1_INT_POLARITY0 &= ~(0x1<<IO_num))
#define GPIOB8_15_Int_Polarity_Low(IO_num)	  (GPIO1_INT_POLARITY1 &= ~(0x1<<IO_num))
#define GPIOB16_19_Int_Polarity_Low(IO_num)	  (GPIO1_INT_POLARITY2 &= ~(0x1<<IO_num))
#define GPIOB20_26_Int_Polarity_Low(IO_num)	  (GPIO1_INT_POLARITY3 &= ~(0x1<<IO_num))
#define GPIOC0_7_Int_Polarity_Low(IO_num)	    (GPIO2_INT_POLARITY0 &= ~(0x1<<IO_num))
#define GPIOC8_13_Int_Polarity_Low(IO_num)	  (GPIO2_INT_POLARITY1 &= ~(0x1<<IO_num))
#define GPIOE0_7_Int_Polarity_Low(IO_num)	    (GPIO3_INT_POLARITY0 &= ~(0x1<<IO_num))
#define GPIOE8_15_Int_Polarity_Low(IO_num)	  (GPIO3_INT_POLARITY1 &= ~(0x1<<IO_num))
#define GPIOE16_23_Int_Polarity_Low(IO_num)	  (GPIO3_INT_POLARITY2 &= ~(0x1<<IO_num))

//Read
#define GPIOA0_7_Int_Enable_Read(IO_num)	    ((GPIO0_INTEN0 & (0x1<<IO_num))  != 0)
#define GPIOA8_15_Int_Enable_Read(IO_num)	    ((GPIO0_INTEN1 & (0x1<<IO_num))  != 0)
#define GPIOA16_23_Int_Enable_Read(IO_num)	  ((GPIO0_INTEN2 & (0x1<<IO_num))  != 0)
#define GPIOA24_31_Int_Enable_Read(IO_num)	  ((GPIO0_INTEN3 & (0x1<<IO_num))  != 0)
#define GPIOB0_7_Int_Enable_Read(IO_num)	    ((GPIO1_INTEN0 & (0x1<<IO_num))  != 0)
#define GPIOB8_15_Int_Enable_Read(IO_num)	    ((GPIO1_INTEN1 & (0x1<<IO_num))  != 0)
#define GPIOB16_19_Int_Enable_Read(IO_num)	  ((GPIO1_INTEN2 & (0x1<<IO_num))  != 0)
#define GPIOB20_26_Int_Enable_Read(IO_num)	  ((GPIO1_INTEN3 & (0x1<<IO_num))  != 0)
#define GPIOC0_7_Int_Enable_Read(IO_num)	    ((GPIO2_INTEN0 & (0x1<<IO_num))  != 0)
#define GPIOC8_13_Int_Enable_Read(IO_num)	    ((GPIO2_INTEN1 & (0x1<<IO_num))  != 0)
#define GPIOE0_7_Int_Enable_Read(IO_num)	    ((GPIO3_INTEN0 & (0x1<<IO_num))  != 0)
#define GPIOE8_15_Int_Enable_Read(IO_num)	    ((GPIO3_INTEN1 & (0x1<<IO_num))  != 0)
#define GPIOE16_23_Int_Enable_Read(IO_num)	  ((GPIO3_INTEN2 & (0x1<<IO_num))  != 0)

#define GPIOA0_7_Int_Mask_Read(IO_num)	      ((GPIO0_INTMASK0 & (0x1<<IO_num)) != 0)
#define GPIOA8_15_Int_Mask_Read(IO_num)	      ((GPIO0_INTMASK1 & (0x1<<IO_num)) != 0)
#define GPIOA16_23_Int_Mask_Read(IO_num)	    ((GPIO0_INTMASK2 & (0x1<<IO_num)) != 0)
#define GPIOA24_31_Int_Mask_Read(IO_num)	    ((GPIO0_INTMASK3 & (0x1<<IO_num)) != 0)
#define GPIOB0_7_Int_Mask_Read(IO_num)	      ((GPIO1_INTMASK0 & (0x1<<IO_num)) != 0)
#define GPIOB8_15_Int_Mask_Read(IO_num)	      ((GPIO1_INTMASK1 & (0x1<<IO_num)) != 0)
#define GPIOB16_19_Int_Mask_Read(IO_num)	    ((GPIO1_INTMASK2 & (0x1<<IO_num)) != 0)
#define GPIOB20_26_Int_Mask_Read(IO_num)	    ((GPIO1_INTMASK3 & (0x1<<IO_num)) != 0)
#define GPIOC0_7_Int_Mask_Read(IO_num)	      ((GPIO2_INTMASK0 & (0x1<<IO_num)) != 0)
#define GPIOC8_13_Int_Mask_Read(IO_num)	      ((GPIO2_INTMASK1 & (0x1<<IO_num)) != 0)
#define GPIOE0_7_Int_Mask_Read(IO_num)	      ((GPIO3_INTMASK0 & (0x1<<IO_num)) != 0)
#define GPIOE8_15_Int_Mask_Read(IO_num)	      ((GPIO3_INTMASK1 & (0x1<<IO_num)) != 0)
#define GPIOE16_23_Int_Mask_Read(IO_num)	    ((GPIO3_INTMASK2 & (0x1<<IO_num)) != 0)

#define GPIOA0_7_Int_Type_Read(IO_num)	      ((GPIO0_INTTYPE_LEVEL0 & (0x1<<IO_num)) != 0)
#define GPIOA8_15_Int_Type_Read(IO_num)	      ((GPIO0_INTTYPE_LEVEL1 & (0x1<<IO_num)) != 0)
#define GPIOA16_23_Int_Type_Read(IO_num)	    ((GPIO0_INTTYPE_LEVEL2 & (0x1<<IO_num)) != 0)
#define GPIOA24_31_Int_Type_Read(IO_num)	    ((GPIO0_INTTYPE_LEVEL3 & (0x1<<IO_num)) != 0)
#define GPIOB0_7_Int_Type_Read(IO_num)	      ((GPIO1_INTTYPE_LEVEL0 & (0x1<<IO_num)) != 0)
#define GPIOB8_15_Int_Type_Read(IO_num)	      ((GPIO1_INTTYPE_LEVEL1 & (0x1<<IO_num)) != 0)
#define GPIOB16_19_Int_Type_Read(IO_num)	    ((GPIO1_INTTYPE_LEVEL2 & (0x1<<IO_num)) != 0)
#define GPIOB20_26_Int_Type_Read(IO_num)	    ((GPIO1_INTTYPE_LEVEL3 & (0x1<<IO_num)) != 0)
#define GPIOC0_7_Int_Type_Read(IO_num)	      ((GPIO2_INTTYPE_LEVEL0 & (0x1<<IO_num)) != 0)
#define GPIOC8_13_Int_Type_Read(IO_num)	      ((GPIO2_INTTYPE_LEVEL1 & (0x1<<IO_num)) != 0)
#define GPIOE0_7_Int_Type_Read(IO_num)	      ((GPIO3_INTTYPE_LEVEL0 & (0x1<<IO_num)) != 0)
#define GPIOE8_15_Int_Type_Read(IO_num) 	    ((GPIO3_INTTYPE_LEVEL1 & (0x1<<IO_num)) != 0)
#define GPIOE16_23_Int_Type_Read(IO_num)	    ((GPIO3_INTTYPE_LEVEL2 & (0x1<<IO_num)) != 0)

#define GPIOA0_7_Int_Polarity_Read(IO_num)	  ((GPIO0_INT_POLARITY0 & (0x1<<IO_num)) != 0)
#define GPIOA8_15_Int_Polarity_Read(IO_num)	  ((GPIO0_INT_POLARITY1 & (0x1<<IO_num)) != 0)
#define GPIOA16_23_Int_Polarity_Read(IO_num)	((GPIO0_INT_POLARITY2 & (0x1<<IO_num)) != 0)
#define GPIOA24_31_Int_Polarity_Read(IO_num)	((GPIO0_INT_POLARITY3 & (0x1<<IO_num)) != 0)
#define GPIOB0_7_Int_Polarity_Read(IO_num)	  ((GPIO1_INT_POLARITY0 & (0x1<<IO_num)) != 0)
#define GPIOB8_15_Int_Polarity_Read(IO_num)	  ((GPIO1_INT_POLARITY1 & (0x1<<IO_num)) != 0)
#define GPIOB16_19_Int_Polarity_Read(IO_num)	((GPIO1_INT_POLARITY2 & (0x1<<IO_num)) != 0)
#define GPIOB20_26_Int_Polarity_Read(IO_num)	((GPIO1_INT_POLARITY3 & (0x1<<IO_num)) != 0)
#define GPIOC0_7_Int_Polarity_Read(IO_num)	  ((GPIO2_INT_POLARITY0 & (0x1<<IO_num)) != 0)
#define GPIOC8_13_Int_Polarity_Read(IO_num)	  ((GPIO2_INT_POLARITY1 & (0x1<<IO_num)) != 0)
#define GPIOE0_7_Int_Polarity_Read(IO_num)	  ((GPIO3_INT_POLARITY0 & (0x1<<IO_num)) != 0)
#define GPIOE8_15_Int_Polarity_Read(IO_num)	  ((GPIO3_INT_POLARITY1 & (0x1<<IO_num)) != 0)
#define GPIOE16_23_Int_Polarity_Read(IO_num)	((GPIO3_INT_POLARITY2 & (0x1<<IO_num)) != 0)

#define GPIOA0_7_Int_Status(IO_num)	          ((GPIO0_INTSTATUS0 & (0x1<<IO_num))  != 0)
#define GPIOA8_15_Int_Status(IO_num)	        ((GPIO0_INTSTATUS1 & (0x1<<IO_num))  != 0)
#define GPIOA16_23_Int_Status(IO_num)	        ((GPIO0_INTSTATUS2 & (0x1<<IO_num))  != 0)
#define GPIOA24_31_Int_Status(IO_num)	        ((GPIO0_INTSTATUS3 & (0x1<<IO_num))  != 0)
#define GPIOB0_7_Int_Status(IO_num)	          ((GPIO1_INTSTATUS0 & (0x1<<IO_num))  != 0)
#define GPIOB8_15_Int_Status(IO_num)	        ((GPIO1_INTSTATUS1 & (0x1<<IO_num))  != 0)
#define GPIOB16_19_Int_Status(IO_num)	        ((GPIO1_INTSTATUS2 & (0x1<<IO_num))  != 0)
#define GPIOB20_26_Int_Status(IO_num)	        ((GPIO1_INTSTATUS3 & (0x1<<IO_num))  != 0)
#define GPIOC0_7_Int_Status(IO_num)	          ((GPIO2_INTSTATUS0 & (0x1<<IO_num))  != 0)
#define GPIOC8_13_Int_Status(IO_num)	        ((GPIO2_INTSTATUS1 & (0x1<<IO_num))  != 0)
#define GPIOE0_7_Int_Status(IO_num)	          ((GPIO3_INTSTATUS0 & (0x1<<IO_num))  != 0)
#define GPIOE8_15_Int_Status(IO_num)	        ((GPIO3_INTSTATUS1 & (0x1<<IO_num))  != 0)
#define GPIOE16_23_Int_Status(IO_num)	        ((GPIO3_INTSTATUS2 & (0x1<<IO_num))  != 0)

/*-----------------------------------------------------------------------------
* GPIO引脚输出高低定义
*---------------------------------------------------------------------------*/
// GPIOA0 to GPIOA7 
#define GPIOA0(x) ((x) ? SET_MASK(GPIO0_DR0, BIT(0)) : CLEAR_MASK(GPIO0_DR0, BIT(0)))
#define GPIOA1(x) ((x) ? SET_MASK(GPIO0_DR0, BIT(1)) : CLEAR_MASK(GPIO0_DR0, BIT(1)))
#define GPIOA2(x) ((x) ? SET_MASK(GPIO0_DR0, BIT(2)) : CLEAR_MASK(GPIO0_DR0, BIT(2)))
#define GPIOA3(x) ((x) ? SET_MASK(GPIO0_DR0, BIT(3)) : CLEAR_MASK(GPIO0_DR0, BIT(3)))
#define GPIOA4(x) ((x) ? SET_MASK(GPIO0_DR0, BIT(4)) : CLEAR_MASK(GPIO0_DR0, BIT(4)))
#define GPIOA5(x) ((x) ? SET_MASK(GPIO0_DR0, BIT(5)) : CLEAR_MASK(GPIO0_DR0, BIT(5)))
#define GPIOA6(x) ((x) ? SET_MASK(GPIO0_DR0, BIT(6)) : CLEAR_MASK(GPIO0_DR0, BIT(6)))
#define GPIOA7(x) ((x) ? SET_MASK(GPIO0_DR0, BIT(7)) : CLEAR_MASK(GPIO0_DR0, BIT(7)))

// GPIOA8 to GPIOA15 
#define GPIOA8(x)  ((x) ? SET_MASK(GPIO0_DR1, BIT(0)) : CLEAR_MASK(GPIO0_DR1, BIT(0)))
#define GPIOA9(x)  ((x) ? SET_MASK(GPIO0_DR1, BIT(1)) : CLEAR_MASK(GPIO0_DR1, BIT(1)))
#define GPIOA10(x) ((x) ? SET_MASK(GPIO0_DR1, BIT(2)) : CLEAR_MASK(GPIO0_DR1, BIT(2)))
#define GPIOA11(x) ((x) ? SET_MASK(GPIO0_DR1, BIT(3)) : CLEAR_MASK(GPIO0_DR1, BIT(3)))
#define GPIOA12(x) ((x) ? SET_MASK(GPIO0_DR1, BIT(4)) : CLEAR_MASK(GPIO0_DR1, BIT(4)))
#define GPIOA13(x) ((x) ? SET_MASK(GPIO0_DR1, BIT(5)) : CLEAR_MASK(GPIO0_DR1, BIT(5)))
#define GPIOA14(x) ((x) ? SET_MASK(GPIO0_DR1, BIT(6)) : CLEAR_MASK(GPIO0_DR1, BIT(6)))
#define GPIOA15(x) ((x) ? SET_MASK(GPIO0_DR1, BIT(7)) : CLEAR_MASK(GPIO0_DR1, BIT(7)))

// GPIOA16 to GPIOA23 
#define GPIOA16(x) ((x) ? SET_MASK(GPIO0_DR2, BIT(0)) : CLEAR_MASK(GPIO0_DR2, BIT(0)))
#define GPIOA17(x) ((x) ? SET_MASK(GPIO0_DR2, BIT(1)) : CLEAR_MASK(GPIO0_DR2, BIT(1)))
#define GPIOA18(x) ((x) ? SET_MASK(GPIO0_DR2, BIT(2)) : CLEAR_MASK(GPIO0_DR2, BIT(2)))
#define GPIOA19(x) ((x) ? SET_MASK(GPIO0_DR2, BIT(3)) : CLEAR_MASK(GPIO0_DR2, BIT(3)))
#define GPIOA20(x) ((x) ? SET_MASK(GPIO0_DR2, BIT(4)) : CLEAR_MASK(GPIO0_DR2, BIT(4)))
#define GPIOA21(x) ((x) ? SET_MASK(GPIO0_DR2, BIT(5)) : CLEAR_MASK(GPIO0_DR2, BIT(5)))
#define GPIOA22(x) ((x) ? SET_MASK(GPIO0_DR2, BIT(6)) : CLEAR_MASK(GPIO0_DR2, BIT(6)))
#define GPIOA23(x) ((x) ? SET_MASK(GPIO0_DR2, BIT(7)) : CLEAR_MASK(GPIO0_DR2, BIT(7)))

// GPIOA24 to GPIOA31 
#define GPIOA24(x) ((x) ? SET_MASK(GPIO0_DR3, BIT(0)) : CLEAR_MASK(GPIO0_DR3, BIT(0)))
#define GPIOA25(x) ((x) ? SET_MASK(GPIO0_DR3, BIT(1)) : CLEAR_MASK(GPIO0_DR3, BIT(1)))
#define GPIOA26(x) ((x) ? SET_MASK(GPIO0_DR3, BIT(2)) : CLEAR_MASK(GPIO0_DR3, BIT(2)))
#define GPIOA27(x) ((x) ? SET_MASK(GPIO0_DR3, BIT(3)) : CLEAR_MASK(GPIO0_DR3, BIT(3)))
#define GPIOA28(x) ((x) ? SET_MASK(GPIO0_DR3, BIT(4)) : CLEAR_MASK(GPIO0_DR3, BIT(4)))
#define GPIOA29(x) ((x) ? SET_MASK(GPIO0_DR3, BIT(5)) : CLEAR_MASK(GPIO0_DR3, BIT(5)))
#define GPIOA30(x) ((x) ? SET_MASK(GPIO0_DR3, BIT(6)) : CLEAR_MASK(GPIO0_DR3, BIT(6)))
#define GPIOA31(x) ((x) ? SET_MASK(GPIO0_DR3, BIT(7)) : CLEAR_MASK(GPIO0_DR3, BIT(7)))

// GPIOB0 to GPIOB7 
#define GPIOB0(x) ((x) ? SET_MASK(GPIO1_DR0, BIT(0)) : CLEAR_MASK(GPIO1_DR0, BIT(0)))
#define GPIOB1(x) ((x) ? SET_MASK(GPIO1_DR0, BIT(1)) : CLEAR_MASK(GPIO1_DR0, BIT(1)))
#define GPIOB2(x) ((x) ? SET_MASK(GPIO1_DR0, BIT(2)) : CLEAR_MASK(GPIO1_DR0, BIT(2)))
#define GPIOB3(x) ((x) ? SET_MASK(GPIO1_DR0, BIT(3)) : CLEAR_MASK(GPIO1_DR0, BIT(3)))
#define GPIOB4(x) ((x) ? SET_MASK(GPIO1_DR0, BIT(4)) : CLEAR_MASK(GPIO1_DR0, BIT(4)))
#define GPIOB5(x) ((x) ? SET_MASK(GPIO1_DR0, BIT(5)) : CLEAR_MASK(GPIO1_DR0, BIT(5)))
#define GPIOB6(x) ((x) ? SET_MASK(GPIO1_DR0, BIT(6)) : CLEAR_MASK(GPIO1_DR0, BIT(6)))
#define GPIOB7(x) ((x) ? SET_MASK(GPIO1_DR0, BIT(7)) : CLEAR_MASK(GPIO1_DR0, BIT(7)))

// GPIOB8 to GPIOB15 
#define GPIOB8(x)  ((x) ? SET_MASK(GPIO1_DR1, BIT(0)) : CLEAR_MASK(GPIO1_DR1, BIT(0)))
#define GPIOB9(x)  ((x) ? SET_MASK(GPIO1_DR1, BIT(1)) : CLEAR_MASK(GPIO1_DR1, BIT(1)))
#define GPIOB10(x) ((x) ? SET_MASK(GPIO1_DR1, BIT(2)) : CLEAR_MASK(GPIO1_DR1, BIT(2)))
#define GPIOB11(x) ((x) ? SET_MASK(GPIO1_DR1, BIT(3)) : CLEAR_MASK(GPIO1_DR1, BIT(3)))
#define GPIOB12(x) ((x) ? SET_MASK(GPIO1_DR1, BIT(4)) : CLEAR_MASK(GPIO1_DR1, BIT(4)))
#define GPIOB13(x) ((x) ? SET_MASK(GPIO1_DR1, BIT(5)) : CLEAR_MASK(GPIO1_DR1, BIT(5)))
#define GPIOB14(x) ((x) ? SET_MASK(GPIO1_DR1, BIT(6)) : CLEAR_MASK(GPIO1_DR1, BIT(6)))
#define GPIOB15(x) ((x) ? SET_MASK(GPIO1_DR1, BIT(7)) : CLEAR_MASK(GPIO1_DR1, BIT(7)))

// GPIOB16 to GPIOB23 
#define GPIOB16(x) ((x) ? SET_MASK(GPIO1_DR2, BIT(0)) : CLEAR_MASK(GPIO1_DR2, BIT(0)))
#define GPIOB17(x) ((x) ? SET_MASK(GPIO1_DR2, BIT(1)) : CLEAR_MASK(GPIO1_DR2, BIT(1)))
#define GPIOB18(x) ((x) ? SET_MASK(GPIO1_DR2, BIT(2)) : CLEAR_MASK(GPIO1_DR2, BIT(2)))
#define GPIOB19(x) ((x) ? SET_MASK(GPIO1_DR2, BIT(3)) : CLEAR_MASK(GPIO1_DR2, BIT(3)))
#define GPIOB20(x) ((x) ? SET_MASK(GPIO1_DR2, BIT(4)) : CLEAR_MASK(GPIO1_DR2, BIT(4)))
#define GPIOB21(x) ((x) ? SET_MASK(GPIO1_DR2, BIT(5)) : CLEAR_MASK(GPIO1_DR2, BIT(5)))
#define GPIOB22(x) ((x) ? SET_MASK(GPIO1_DR2, BIT(6)) : CLEAR_MASK(GPIO1_DR2, BIT(6)))
#define GPIOB23(x) ((x) ? SET_MASK(GPIO1_DR2, BIT(7)) : CLEAR_MASK(GPIO1_DR2, BIT(7)))

// GPIOB24 to GPIOB31 
#define GPIOB24(x) ((x) ? SET_MASK(GPIO1_DR3, BIT(0)) : CLEAR_MASK(GPIO1_DR3, BIT(0)))
#define GPIOB25(x) ((x) ? SET_MASK(GPIO1_DR3, BIT(1)) : CLEAR_MASK(GPIO1_DR3, BIT(1)))
#define GPIOB26(x) ((x) ? SET_MASK(GPIO1_DR3, BIT(2)) : CLEAR_MASK(GPIO1_DR3, BIT(2)))
#define GPIOB27(x) ((x) ? SET_MASK(GPIO1_DR3, BIT(3)) : CLEAR_MASK(GPIO1_DR3, BIT(3)))
#define GPIOB28(x) ((x) ? SET_MASK(GPIO1_DR3, BIT(4)) : CLEAR_MASK(GPIO1_DR3, BIT(4)))
#define GPIOB29(x) ((x) ? SET_MASK(GPIO1_DR3, BIT(5)) : CLEAR_MASK(GPIO1_DR3, BIT(5)))
#define GPIOB30(x) ((x) ? SET_MASK(GPIO1_DR3, BIT(6)) : CLEAR_MASK(GPIO1_DR3, BIT(6)))
#define GPIOB31(x) ((x) ? SET_MASK(GPIO1_DR3, BIT(7)) : CLEAR_MASK(GPIO1_DR3, BIT(7)))
// GPIOC0 to GPIOC7 
#define GPIOC0(x) ((x) ? SET_MASK(GPIO2_DR0, BIT(0)) : CLEAR_MASK(GPIO2_DR0, BIT(0)))
#define GPIOC1(x) ((x) ? SET_MASK(GPIO2_DR0, BIT(1)) : CLEAR_MASK(GPIO2_DR0, BIT(1)))
#define GPIOC2(x) ((x) ? SET_MASK(GPIO2_DR0, BIT(2)) : CLEAR_MASK(GPIO2_DR0, BIT(2)))
#define GPIOC3(x) ((x) ? SET_MASK(GPIO2_DR0, BIT(3)) : CLEAR_MASK(GPIO2_DR0, BIT(3)))
#define GPIOC4(x) ((x) ? SET_MASK(GPIO2_DR0, BIT(4)) : CLEAR_MASK(GPIO2_DR0, BIT(4)))
#define GPIOC5(x) ((x) ? SET_MASK(GPIO2_DR0, BIT(5)) : CLEAR_MASK(GPIO2_DR0, BIT(5)))
#define GPIOC6(x) ((x) ? SET_MASK(GPIO2_DR0, BIT(6)) : CLEAR_MASK(GPIO2_DR0, BIT(6)))
#define GPIOC7(x) ((x) ? SET_MASK(GPIO2_DR0, BIT(7)) : CLEAR_MASK(GPIO2_DR0, BIT(7)))

// GPIOC8 to GPIOC15 
#define GPIOC8(x)  ((x) ? SET_MASK(GPIO2_DR1, BIT(0)) : CLEAR_MASK(GPIO2_DR1, BIT(0)))
#define GPIOC9(x)  ((x) ? SET_MASK(GPIO2_DR1, BIT(1)) : CLEAR_MASK(GPIO2_DR1, BIT(1)))
#define GPIOC10(x) ((x) ? SET_MASK(GPIO2_DR1, BIT(2)) : CLEAR_MASK(GPIO2_DR1, BIT(2)))
#define GPIOC11(x) ((x) ? SET_MASK(GPIO2_DR1, BIT(3)) : CLEAR_MASK(GPIO2_DR1, BIT(3)))
#define GPIOC12(x) ((x) ? SET_MASK(GPIO2_DR1, BIT(4)) : CLEAR_MASK(GPIO2_DR1, BIT(4)))
#define GPIOC13(x) ((x) ? SET_MASK(GPIO2_DR1, BIT(5)) : CLEAR_MASK(GPIO2_DR1, BIT(5)))
#define GPIOC14(x) ((x) ? SET_MASK(GPIO2_DR1, BIT(6)) : CLEAR_MASK(GPIO2_DR1, BIT(6)))
#define GPIOC15(x) ((x) ? SET_MASK(GPIO2_DR1, BIT(7)) : CLEAR_MASK(GPIO2_DR1, BIT(7)))
// GPIOD0 to GPIOD7 
#define GPIOD0(x) ((x) ? SET_MASK(GPIO2_DR2, BIT(0)) : CLEAR_MASK(GPIO2_DR2, BIT(0)))
#define GPIOD1(x) ((x) ? SET_MASK(GPIO2_DR2, BIT(1)) : CLEAR_MASK(GPIO2_DR2, BIT(1)))
#define GPIOD2(x) ((x) ? SET_MASK(GPIO2_DR2, BIT(2)) : CLEAR_MASK(GPIO2_DR2, BIT(2)))
#define GPIOD3(x) ((x) ? SET_MASK(GPIO2_DR2, BIT(3)) : CLEAR_MASK(GPIO2_DR2, BIT(3)))
#define GPIOD4(x) ((x) ? SET_MASK(GPIO2_DR2, BIT(4)) : CLEAR_MASK(GPIO2_DR2, BIT(4)))
#define GPIOD5(x) ((x) ? SET_MASK(GPIO2_DR2, BIT(5)) : CLEAR_MASK(GPIO2_DR2, BIT(5)))
#define GPIOD6(x) ((x) ? SET_MASK(GPIO2_DR2, BIT(6)) : CLEAR_MASK(GPIO2_DR2, BIT(6)))
#define GPIOD7(x) ((x) ? SET_MASK(GPIO2_DR2, BIT(7)) : CLEAR_MASK(GPIO2_DR2, BIT(7)))


// GPIOD8 (由GPIO2_DR3控制)
#define GPIOD8(x) ((x) ? SET_MASK(GPIO2_DR3, BIT(0)) : CLEAR_MASK(GPIO2_DR3, BIT(0)))
// GPIOE0 to GPIOE7 
#define GPIOE0(x) ((x) ? SET_MASK(GPIO3_DR0, BIT(0)) : CLEAR_MASK(GPIO3_DR0, BIT(0)))
#define GPIOE1(x) ((x) ? SET_MASK(GPIO3_DR0, BIT(1)) : CLEAR_MASK(GPIO3_DR0, BIT(1)))
#define GPIOE2(x) ((x) ? SET_MASK(GPIO3_DR0, BIT(2)) : CLEAR_MASK(GPIO3_DR0, BIT(2)))
#define GPIOE3(x) ((x) ? SET_MASK(GPIO3_DR0, BIT(3)) : CLEAR_MASK(GPIO3_DR0, BIT(3)))
#define GPIOE4(x) ((x) ? SET_MASK(GPIO3_DR0, BIT(4)) : CLEAR_MASK(GPIO3_DR0, BIT(4)))
#define GPIOE5(x) ((x) ? SET_MASK(GPIO3_DR0, BIT(5)) : CLEAR_MASK(GPIO3_DR0, BIT(5)))
#define GPIOE6(x) ((x) ? SET_MASK(GPIO3_DR0, BIT(6)) : CLEAR_MASK(GPIO3_DR0, BIT(6)))
#define GPIOE7(x) ((x) ? SET_MASK(GPIO3_DR0, BIT(7)) : CLEAR_MASK(GPIO3_DR0, BIT(7)))

// GPIOE8 to GPIOE15 
#define GPIOE8(x)  ((x) ? SET_MASK(GPIO3_DR1, BIT(0)) : CLEAR_MASK(GPIO3_DR1, BIT(0)))
#define GPIOE9(x)  ((x) ? SET_MASK(GPIO3_DR1, BIT(1)) : CLEAR_MASK(GPIO3_DR1, BIT(1)))
#define GPIOE10(x) ((x) ? SET_MASK(GPIO3_DR1, BIT(2)) : CLEAR_MASK(GPIO3_DR1, BIT(2)))
#define GPIOE11(x) ((x) ? SET_MASK(GPIO3_DR1, BIT(3)) : CLEAR_MASK(GPIO3_DR1, BIT(3)))
#define GPIOE12(x) ((x) ? SET_MASK(GPIO3_DR1, BIT(4)) : CLEAR_MASK(GPIO3_DR1, BIT(4)))
#define GPIOE13(x) ((x) ? SET_MASK(GPIO3_DR1, BIT(5)) : CLEAR_MASK(GPIO3_DR1, BIT(5)))
#define GPIOE14(x) ((x) ? SET_MASK(GPIO3_DR1, BIT(6)) : CLEAR_MASK(GPIO3_DR1, BIT(6)))
#define GPIOE15(x) ((x) ? SET_MASK(GPIO3_DR1, BIT(7)) : CLEAR_MASK(GPIO3_DR1, BIT(7)))

// GPIOE16 to GPIOE23 
#define GPIOE16(x) ((x) ? SET_MASK(GPIO3_DR2, BIT(0)) : CLEAR_MASK(GPIO3_DR2, BIT(0)))
#define GPIOE17(x) ((x) ? SET_MASK(GPIO3_DR2, BIT(1)) : CLEAR_MASK(GPIO3_DR2, BIT(1)))
#define GPIOE18(x) ((x) ? SET_MASK(GPIO3_DR2, BIT(2)) : CLEAR_MASK(GPIO3_DR2, BIT(2)))
#define GPIOE19(x) ((x) ? SET_MASK(GPIO3_DR2, BIT(3)) : CLEAR_MASK(GPIO3_DR2, BIT(3)))
#define GPIOE20(x) ((x) ? SET_MASK(GPIO3_DR2, BIT(4)) : CLEAR_MASK(GPIO3_DR2, BIT(4)))
#define GPIOE21(x) ((x) ? SET_MASK(GPIO3_DR2, BIT(5)) : CLEAR_MASK(GPIO3_DR2, BIT(5)))
#define GPIOE22(x) ((x) ? SET_MASK(GPIO3_DR2, BIT(6)) : CLEAR_MASK(GPIO3_DR2, BIT(6)))
#define GPIOE23(x) ((x) ? SET_MASK(GPIO3_DR2, BIT(7)) : CLEAR_MASK(GPIO3_DR2, BIT(7)))

 /*-----------------------------------------------------------------------------
  * GPIO引脚监测输入高低定义
  *---------------------------------------------------------------------------*/
  // GPIOA组输入检测
#define IS_GPIOA0(x)  ((x) ? IS_MASK_SET(GPIO0_EXT0, BIT(0)) : IS_MASK_CLEAR(GPIO0_EXT0, BIT(0)))
#define IS_GPIOA1(x)  ((x) ? IS_MASK_SET(GPIO0_EXT0, BIT(1)) : IS_MASK_CLEAR(GPIO0_EXT0, BIT(1)))
#define IS_GPIOA2(x)  ((x) ? IS_MASK_SET(GPIO0_EXT0, BIT(2)) : IS_MASK_CLEAR(GPIO0_EXT0, BIT(2)))
#define IS_GPIOA3(x)  ((x) ? IS_MASK_SET(GPIO0_EXT0, BIT(3)) : IS_MASK_CLEAR(GPIO0_EXT0, BIT(3)))
#define IS_GPIOA4(x)  ((x) ? IS_MASK_SET(GPIO0_EXT0, BIT(4)) : IS_MASK_CLEAR(GPIO0_EXT0, BIT(4)))
#define IS_GPIOA5(x)  ((x) ? IS_MASK_SET(GPIO0_EXT0, BIT(5)) : IS_MASK_CLEAR(GPIO0_EXT0, BIT(5)))
#define IS_GPIOA6(x)  ((x) ? IS_MASK_SET(GPIO0_EXT0, BIT(6)) : IS_MASK_CLEAR(GPIO0_EXT0, BIT(6)))
#define IS_GPIOA7(x)  ((x) ? IS_MASK_SET(GPIO0_EXT0, BIT(7)) : IS_MASK_CLEAR(GPIO0_EXT0, BIT(7)))

#define IS_GPIOA8(x)  ((x) ? IS_MASK_SET(GPIO0_EXT1, BIT(0)) : IS_MASK_CLEAR(GPIO0_EXT1, BIT(0)))
#define IS_GPIOA9(x)  ((x) ? IS_MASK_SET(GPIO0_EXT1, BIT(1)) : IS_MASK_CLEAR(GPIO0_EXT1, BIT(1)))
#define IS_GPIOA10(x) ((x) ? IS_MASK_SET(GPIO0_EXT1, BIT(2)) : IS_MASK_CLEAR(GPIO0_EXT1, BIT(2)))
#define IS_GPIOA11(x) ((x) ? IS_MASK_SET(GPIO0_EXT1, BIT(3)) : IS_MASK_CLEAR(GPIO0_EXT1, BIT(3)))
#define IS_GPIOA12(x) ((x) ? IS_MASK_SET(GPIO0_EXT1, BIT(4)) : IS_MASK_CLEAR(GPIO0_EXT1, BIT(4)))
#define IS_GPIOA13(x) ((x) ? IS_MASK_SET(GPIO0_EXT1, BIT(5)) : IS_MASK_CLEAR(GPIO0_EXT1, BIT(5)))
#define IS_GPIOA14(x) ((x) ? IS_MASK_SET(GPIO0_EXT1, BIT(6)) : IS_MASK_CLEAR(GPIO0_EXT1, BIT(6)))
#define IS_GPIOA15(x) ((x) ? IS_MASK_SET(GPIO0_EXT1, BIT(7)) : IS_MASK_CLEAR(GPIO0_EXT1, BIT(7)))

#define IS_GPIOA16(x) ((x) ? IS_MASK_SET(GPIO0_EXT2, BIT(0)) : IS_MASK_CLEAR(GPIO0_EXT2, BIT(0)))
#define IS_GPIOA17(x) ((x) ? IS_MASK_SET(GPIO0_EXT2, BIT(1)) : IS_MASK_CLEAR(GPIO0_EXT2, BIT(1)))
#define IS_GPIOA18(x) ((x) ? IS_MASK_SET(GPIO0_EXT2, BIT(2)) : IS_MASK_CLEAR(GPIO0_EXT2, BIT(2)))
#define IS_GPIOA19(x) ((x) ? IS_MASK_SET(GPIO0_EXT2, BIT(3)) : IS_MASK_CLEAR(GPIO0_EXT2, BIT(3)))
#define IS_GPIOA20(x) ((x) ? IS_MASK_SET(GPIO0_EXT2, BIT(4)) : IS_MASK_CLEAR(GPIO0_EXT2, BIT(4)))
#define IS_GPIOA21(x) ((x) ? IS_MASK_SET(GPIO0_EXT2, BIT(5)) : IS_MASK_CLEAR(GPIO0_EXT2, BIT(5)))
#define IS_GPIOA22(x) ((x) ? IS_MASK_SET(GPIO0_EXT2, BIT(6)) : IS_MASK_CLEAR(GPIO0_EXT2, BIT(6)))
#define IS_GPIOA23(x) ((x) ? IS_MASK_SET(GPIO0_EXT2, BIT(7)) : IS_MASK_CLEAR(GPIO0_EXT2, BIT(7)))

#define IS_GPIOA24(x) ((x) ? IS_MASK_SET(GPIO0_EXT3, BIT(0)) : IS_MASK_CLEAR(GPIO0_EXT3, BIT(0)))
#define IS_GPIOA25(x) ((x) ? IS_MASK_SET(GPIO0_EXT3, BIT(1)) : IS_MASK_CLEAR(GPIO0_EXT3, BIT(1)))
#define IS_GPIOA26(x) ((x) ? IS_MASK_SET(GPIO0_EXT3, BIT(2)) : IS_MASK_CLEAR(GPIO0_EXT3, BIT(2)))
#define IS_GPIOA27(x) ((x) ? IS_MASK_SET(GPIO0_EXT3, BIT(3)) : IS_MASK_CLEAR(GPIO0_EXT3, BIT(3)))
#define IS_GPIOA28(x) ((x) ? IS_MASK_SET(GPIO0_EXT3, BIT(4)) : IS_MASK_CLEAR(GPIO0_EXT3, BIT(4)))
#define IS_GPIOA29(x) ((x) ? IS_MASK_SET(GPIO0_EXT3, BIT(5)) : IS_MASK_CLEAR(GPIO0_EXT3, BIT(5)))
#define IS_GPIOA30(x) ((x) ? IS_MASK_SET(GPIO0_EXT3, BIT(6)) : IS_MASK_CLEAR(GPIO0_EXT3, BIT(6)))
#define IS_GPIOA31(x) ((x) ? IS_MASK_SET(GPIO0_EXT3, BIT(7)) : IS_MASK_CLEAR(GPIO0_EXT3, BIT(7)))

// GPIOB组输入检测
#define IS_GPIOB0(x)  ((x) ? IS_MASK_SET(GPIO1_EXT0, BIT(0)) : IS_MASK_CLEAR(GPIO1_EXT0, BIT(0)))
#define IS_GPIOB1(x)  ((x) ? IS_MASK_SET(GPIO1_EXT0, BIT(1)) : IS_MASK_CLEAR(GPIO1_EXT0, BIT(1)))
#define IS_GPIOB2(x)  ((x) ? IS_MASK_SET(GPIO1_EXT0, BIT(2)) : IS_MASK_CLEAR(GPIO1_EXT0, BIT(2)))
#define IS_GPIOB3(x)  ((x) ? IS_MASK_SET(GPIO1_EXT0, BIT(3)) : IS_MASK_CLEAR(GPIO1_EXT0, BIT(3)))
#define IS_GPIOB4(x)  ((x) ? IS_MASK_SET(GPIO1_EXT0, BIT(4)) : IS_MASK_CLEAR(GPIO1_EXT0, BIT(4)))
#define IS_GPIOB5(x)  ((x) ? IS_MASK_SET(GPIO1_EXT0, BIT(5)) : IS_MASK_CLEAR(GPIO1_EXT0, BIT(5)))
#define IS_GPIOB6(x)  ((x) ? IS_MASK_SET(GPIO1_EXT0, BIT(6)) : IS_MASK_CLEAR(GPIO1_EXT0, BIT(6)))
#define IS_GPIOB7(x)  ((x) ? IS_MASK_SET(GPIO1_EXT0, BIT(7)) : IS_MASK_CLEAR(GPIO1_EXT0, BIT(7)))

#define IS_GPIOB8(x)  ((x) ? IS_MASK_SET(GPIO1_EXT1, BIT(0)) : IS_MASK_CLEAR(GPIO1_EXT1, BIT(0)))
#define IS_GPIOB9(x)  ((x) ? IS_MASK_SET(GPIO1_EXT1, BIT(1)) : IS_MASK_CLEAR(GPIO1_EXT1, BIT(1)))
#define IS_GPIOB10(x) ((x) ? IS_MASK_SET(GPIO1_EXT1, BIT(2)) : IS_MASK_CLEAR(GPIO1_EXT1, BIT(2)))
#define IS_GPIOB11(x) ((x) ? IS_MASK_SET(GPIO1_EXT1, BIT(3)) : IS_MASK_CLEAR(GPIO1_EXT1, BIT(3)))
#define IS_GPIOB12(x) ((x) ? IS_MASK_SET(GPIO1_EXT1, BIT(4)) : IS_MASK_CLEAR(GPIO1_EXT1, BIT(4)))
#define IS_GPIOB13(x) ((x) ? IS_MASK_SET(GPIO1_EXT1, BIT(5)) : IS_MASK_CLEAR(GPIO1_EXT1, BIT(5)))
#define IS_GPIOB14(x) ((x) ? IS_MASK_SET(GPIO1_EXT1, BIT(6)) : IS_MASK_CLEAR(GPIO1_EXT1, BIT(6)))
#define IS_GPIOB15(x) ((x) ? IS_MASK_SET(GPIO1_EXT1, BIT(7)) : IS_MASK_CLEAR(GPIO1_EXT1, BIT(7)))

#define IS_GPIOB16(x) ((x) ? IS_MASK_SET(GPIO1_EXT2, BIT(0)) : IS_MASK_CLEAR(GPIO1_EXT2, BIT(0)))
#define IS_GPIOB17(x) ((x) ? IS_MASK_SET(GPIO1_EXT2, BIT(1)) : IS_MASK_CLEAR(GPIO1_EXT2, BIT(1)))
#define IS_GPIOB18(x) ((x) ? IS_MASK_SET(GPIO1_EXT2, BIT(2)) : IS_MASK_CLEAR(GPIO1_EXT2, BIT(2)))
#define IS_GPIOB19(x) ((x) ? IS_MASK_SET(GPIO1_EXT2, BIT(3)) : IS_MASK_CLEAR(GPIO1_EXT2, BIT(3)))
#define IS_GPIOB20(x) ((x) ? IS_MASK_SET(GPIO1_EXT2, BIT(4)) : IS_MASK_CLEAR(GPIO1_EXT2, BIT(4)))
#define IS_GPIOB21(x) ((x) ? IS_MASK_SET(GPIO1_EXT2, BIT(5)) : IS_MASK_CLEAR(GPIO1_EXT2, BIT(5)))
#define IS_GPIOB22(x) ((x) ? IS_MASK_SET(GPIO1_EXT2, BIT(6)) : IS_MASK_CLEAR(GPIO1_EXT2, BIT(6)))
#define IS_GPIOB23(x) ((x) ? IS_MASK_SET(GPIO1_EXT2, BIT(7)) : IS_MASK_CLEAR(GPIO1_EXT2, BIT(7)))

#define IS_GPIOB24(x) ((x) ? IS_MASK_SET(GPIO1_EXT3, BIT(0)) : IS_MASK_CLEAR(GPIO1_EXT3, BIT(0)))
#define IS_GPIOB25(x) ((x) ? IS_MASK_SET(GPIO1_EXT3, BIT(1)) : IS_MASK_CLEAR(GPIO1_EXT3, BIT(1)))
#define IS_GPIOB26(x) ((x) ? IS_MASK_SET(GPIO1_EXT3, BIT(2)) : IS_MASK_CLEAR(GPIO1_EXT3, BIT(2)))
#define IS_GPIOB27(x) ((x) ? IS_MASK_SET(GPIO1_EXT3, BIT(3)) : IS_MASK_CLEAR(GPIO1_EXT3, BIT(3)))
#define IS_GPIOB28(x) ((x) ? IS_MASK_SET(GPIO1_EXT3, BIT(4)) : IS_MASK_CLEAR(GPIO1_EXT3, BIT(4)))
#define IS_GPIOB29(x) ((x) ? IS_MASK_SET(GPIO1_EXT3, BIT(5)) : IS_MASK_CLEAR(GPIO1_EXT3, BIT(5)))
#define IS_GPIOB30(x) ((x) ? IS_MASK_SET(GPIO1_EXT3, BIT(6)) : IS_MASK_CLEAR(GPIO1_EXT3, BIT(6)))
#define IS_GPIOB31(x) ((x) ? IS_MASK_SET(GPIO1_EXT3, BIT(7)) : IS_MASK_CLEAR(GPIO1_EXT3, BIT(7)))
// GPIOC组输入检测
#define IS_GPIOC0(x)  ((x) ? IS_MASK_SET(GPIO2_EXT0, BIT(0)) : IS_MASK_CLEAR(GPIO2_EXT0, BIT(0)))
#define IS_GPIOC1(x)  ((x) ? IS_MASK_SET(GPIO2_EXT0, BIT(1)) : IS_MASK_CLEAR(GPIO2_EXT0, BIT(1)))
#define IS_GPIOC2(x)  ((x) ? IS_MASK_SET(GPIO2_EXT0, BIT(2)) : IS_MASK_CLEAR(GPIO2_EXT0, BIT(2)))
#define IS_GPIOC3(x)  ((x) ? IS_MASK_SET(GPIO2_EXT0, BIT(3)) : IS_MASK_CLEAR(GPIO2_EXT0, BIT(3)))
#define IS_GPIOC4(x)  ((x) ? IS_MASK_SET(GPIO2_EXT0, BIT(4)) : IS_MASK_CLEAR(GPIO2_EXT0, BIT(4)))
#define IS_GPIOC5(x)  ((x) ? IS_MASK_SET(GPIO2_EXT0, BIT(5)) : IS_MASK_CLEAR(GPIO2_EXT0, BIT(5)))
#define IS_GPIOC6(x)  ((x) ? IS_MASK_SET(GPIO2_EXT0, BIT(6)) : IS_MASK_CLEAR(GPIO2_EXT0, BIT(6)))
#define IS_GPIOC7(x)  ((x) ? IS_MASK_SET(GPIO2_EXT0, BIT(7)) : IS_MASK_CLEAR(GPIO2_EXT0, BIT(7)))

#define IS_GPIOC8(x)  ((x) ? IS_MASK_SET(GPIO2_EXT1, BIT(0)) : IS_MASK_CLEAR(GPIO2_EXT1, BIT(0)))
#define IS_GPIOC9(x)  ((x) ? IS_MASK_SET(GPIO2_EXT1, BIT(1)) : IS_MASK_CLEAR(GPIO2_EXT1, BIT(1)))
#define IS_GPIOC10(x) ((x) ? IS_MASK_SET(GPIO2_EXT1, BIT(2)) : IS_MASK_CLEAR(GPIO2_EXT1, BIT(2)))
#define IS_GPIOC11(x) ((x) ? IS_MASK_SET(GPIO2_EXT1, BIT(3)) : IS_MASK_CLEAR(GPIO2_EXT1, BIT(3)))
#define IS_GPIOC12(x) ((x) ? IS_MASK_SET(GPIO2_EXT1, BIT(4)) : IS_MASK_CLEAR(GPIO2_EXT1, BIT(4)))
#define IS_GPIOC13(x) ((x) ? IS_MASK_SET(GPIO2_EXT1, BIT(5)) : IS_MASK_CLEAR(GPIO2_EXT1, BIT(5)))
#define IS_GPIOC14(x) ((x) ? IS_MASK_SET(GPIO2_EXT1, BIT(6)) : IS_MASK_CLEAR(GPIO2_EXT1, BIT(6)))
#define IS_GPIOC15(x) ((x) ? IS_MASK_SET(GPIO2_EXT1, BIT(7)) : IS_MASK_CLEAR(GPIO2_EXT1, BIT(7)))
// GPIOD组输入检测
#define IS_GPIOD0(x) ((x) ? IS_MASK_SET(GPIO2_EXT2, BIT(0)) : IS_MASK_CLEAR(GPIO2_EXT2, BIT(0)))
#define IS_GPIOD1(x) ((x) ? IS_MASK_SET(GPIO2_EXT2, BIT(1)) : IS_MASK_CLEAR(GPIO2_EXT2, BIT(1)))
#define IS_GPIOD2(x) ((x) ? IS_MASK_SET(GPIO2_EXT2, BIT(2)) : IS_MASK_CLEAR(GPIO2_EXT2, BIT(2)))
#define IS_GPIOD3(x) ((x) ? IS_MASK_SET(GPIO2_EXT2, BIT(3)) : IS_MASK_CLEAR(GPIO2_EXT2, BIT(3)))
#define IS_GPIOD4(x) ((x) ? IS_MASK_SET(GPIO2_EXT2, BIT(4)) : IS_MASK_CLEAR(GPIO2_EXT2, BIT(4)))
#define IS_GPIOD5(x) ((x) ? IS_MASK_SET(GPIO2_EXT2, BIT(5)) : IS_MASK_CLEAR(GPIO2_EXT2, BIT(5)))
#define IS_GPIOD6(x) ((x) ? IS_MASK_SET(GPIO2_EXT2, BIT(6)) : IS_MASK_CLEAR(GPIO2_EXT2, BIT(6)))
#define IS_GPIOD7(x) ((x) ? IS_MASK_SET(GPIO2_EXT2, BIT(7)) : IS_MASK_CLEAR(GPIO2_EXT2, BIT(7)))
#define IS_GPIOD8(x) ((x) ? IS_MASK_SET(GPIO2_EXT3, BIT(0)) : IS_MASK_CLEAR(GPIO2_EXT3, BIT(0)))
// GPIOE组输入检测
#define IS_GPIOE0(x)  ((x) ? IS_MASK_SET(GPIO3_EXT0, BIT(0)) : IS_MASK_CLEAR(GPIO3_EXT0, BIT(0)))
#define IS_GPIOE1(x)  ((x) ? IS_MASK_SET(GPIO3_EXT0, BIT(1)) : IS_MASK_CLEAR(GPIO3_EXT0, BIT(1)))
#define IS_GPIOE2(x)  ((x) ? IS_MASK_SET(GPIO3_EXT0, BIT(2)) : IS_MASK_CLEAR(GPIO3_EXT0, BIT(2)))
#define IS_GPIOE3(x)  ((x) ? IS_MASK_SET(GPIO3_EXT0, BIT(3)) : IS_MASK_CLEAR(GPIO3_EXT0, BIT(3)))
#define IS_GPIOE4(x)  ((x) ? IS_MASK_SET(GPIO3_EXT0, BIT(4)) : IS_MASK_CLEAR(GPIO3_EXT0, BIT(4)))
#define IS_GPIOE5(x)  ((x) ? IS_MASK_SET(GPIO3_EXT0, BIT(5)) : IS_MASK_CLEAR(GPIO3_EXT0, BIT(5)))
#define IS_GPIOE6(x)  ((x) ? IS_MASK_SET(GPIO3_EXT0, BIT(6)) : IS_MASK_CLEAR(GPIO3_EXT0, BIT(6)))
#define IS_GPIOE7(x)  ((x) ? IS_MASK_SET(GPIO3_EXT0, BIT(7)) : IS_MASK_CLEAR(GPIO3_EXT0, BIT(7)))

#define IS_GPIOE8(x)  ((x) ? IS_MASK_SET(GPIO3_EXT1, BIT(0)) : IS_MASK_CLEAR(GPIO3_EXT1, BIT(0)))
#define IS_GPIOE9(x)  ((x) ? IS_MASK_SET(GPIO3_EXT1, BIT(1)) : IS_MASK_CLEAR(GPIO3_EXT1, BIT(1)))
#define IS_GPIOE10(x) ((x) ? IS_MASK_SET(GPIO3_EXT1, BIT(2)) : IS_MASK_CLEAR(GPIO3_EXT1, BIT(2)))
#define IS_GPIOE11(x) ((x) ? IS_MASK_SET(GPIO3_EXT1, BIT(3)) : IS_MASK_CLEAR(GPIO3_EXT1, BIT(3)))
#define IS_GPIOE12(x) ((x) ? IS_MASK_SET(GPIO3_EXT1, BIT(4)) : IS_MASK_CLEAR(GPIO3_EXT1, BIT(4)))
#define IS_GPIOE13(x) ((x) ? IS_MASK_SET(GPIO3_EXT1, BIT(5)) : IS_MASK_CLEAR(GPIO3_EXT1, BIT(5)))
#define IS_GPIOE14(x) ((x) ? IS_MASK_SET(GPIO3_EXT1, BIT(6)) : IS_MASK_CLEAR(GPIO3_EXT1, BIT(6)))
#define IS_GPIOE15(x) ((x) ? IS_MASK_SET(GPIO3_EXT1, BIT(7)) : IS_MASK_CLEAR(GPIO3_EXT1, BIT(7)))

#define IS_GPIOE16(x) ((x) ? IS_MASK_SET(GPIO3_EXT2, BIT(0)) : IS_MASK_CLEAR(GPIO3_EXT2, BIT(0)))
#define IS_GPIOE17(x) ((x) ? IS_MASK_SET(GPIO3_EXT2, BIT(1)) : IS_MASK_CLEAR(GPIO3_EXT2, BIT(1)))
#define IS_GPIOE18(x) ((x) ? IS_MASK_SET(GPIO3_EXT2, BIT(2)) : IS_MASK_CLEAR(GPIO3_EXT2, BIT(2)))
#define IS_GPIOE19(x) ((x) ? IS_MASK_SET(GPIO3_EXT2, BIT(3)) : IS_MASK_CLEAR(GPIO3_EXT2, BIT(3)))
#define IS_GPIOE20(x) ((x) ? IS_MASK_SET(GPIO3_EXT2, BIT(4)) : IS_MASK_CLEAR(GPIO3_EXT2, BIT(4)))
#define IS_GPIOE21(x) ((x) ? IS_MASK_SET(GPIO3_EXT2, BIT(5)) : IS_MASK_CLEAR(GPIO3_EXT2, BIT(5)))
#define IS_GPIOE22(x) ((x) ? IS_MASK_SET(GPIO3_EXT2, BIT(6)) : IS_MASK_CLEAR(GPIO3_EXT2, BIT(6)))
#define IS_GPIOE23(x) ((x) ? IS_MASK_SET(GPIO3_EXT2, BIT(7)) : IS_MASK_CLEAR(GPIO3_EXT2, BIT(7)))
 /*-----------------------------------------------------------------------------
  * GPIO引脚输出高低定义(按照pin的格式)
  *---------------------------------------------------------------------------*/
#define Pin2(x)   GPIOC15(x)
#define Pin3(x)   GPIOB31(x)
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

#define GPIO_INPUT_VAL      (0x00)
#define GPIO_INPUT_EN       (0x04)
#define GPIO_OUTPUT_EN      (0x08)
#define GPIO_OUTPUT_VAL     (0x0C)
#define GPIO_PULLUP_EN      (0x10)
#define GPIO_DRIVE          (0x14)
#define GPIO_RISE_IE        (0x18)
#define GPIO_RISE_IP        (0x1C)
#define GPIO_FALL_IE        (0x20)
#define GPIO_FALL_IP        (0x24)
#define GPIO_HIGH_IE        (0x28)
#define GPIO_HIGH_IP        (0x2C)
#define GPIO_LOW_IE         (0x30)
#define GPIO_LOW_IP         (0x34)
#define GPIO_IOF_EN         (0x38)
#define GPIO_IOF_SEL        (0x3C)
#define GPIO_DEBOUNCE_EN    (0x48)
#define GPIO_OUTPUT_XOR     (0x40)
/*-----------------------------------------------------------------------------
 * GPIO state table
 *---------------------------------------------------------------------------*/
struct GPIOPinState {
  BYTE default_state_idx;	// default state is mux_state[default_state_idx]
  BYTE mux_state[4]; // 0 is input, 1 is output
};
extern struct GPIOPinState gpio_state_table[];
/*-----------------------------------------------------------------------------
 *	Extern functions
 *---------------------------------------------------------------------------*/
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
extern char ReadGPIOLevel(BYTE port, BYTE pin);

#endif
