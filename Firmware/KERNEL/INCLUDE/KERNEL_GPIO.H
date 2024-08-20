/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-02-07 12:46:44
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
#if (defined(AE101)||defined(AE102))
#define UARTB_SEL  SYSCTL_PIO5_UDCFG &= ~BIT7;
#define SMBUS3_SEL  SYSCTL_PIO5_UDCFG |=BIT7;
#elif defined(AE103)
#define UARTB_SEL  SYSCTL_PIO4_UDCFG &= ~BIT0;
#define SMBUS3_SEL  SYSCTL_PIO4_UDCFG |=BIT0;
#endif
#if (COMMAND_UART_SWITCH == 5 || DEBUG_UART_SWITCH == 5 || PRINTF_UART_SWITCH == 5)
#if SMBUS3_CLOCK_EN
// #error 
#warning "You want use UARTB,but SMBUS3(IIC3) Enable, They reuse the same pin"
#endif
#define SMBUS3_UARTB_SEL  UARTB_SEL
#else
#define SMBUS3_UARTB_SEL  SMBUS3_SEL
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
#if (defined(AE101)||defined(AE102)||defined(AE103))//If it's AE102 or AE103
#define GPIOE0_7_Int_Enable(IO_num)	    (GPIO3_INTEN0 |= (0x1<<IO_num))
#define GPIOE8_15_Int_Enable(IO_num)	(GPIO3_INTEN1 |= (0x1<<IO_num))
#define GPIOE16_23_Int_Enable(IO_num)	(GPIO3_INTEN2 |= (0x1<<IO_num))
#else
#define GPIOE0_7_Int_Enable(IO_num)	
#define GPIOE8_15_Int_Enable(IO_num)
#define GPIOE16_23_Int_Enable(IO_num)
#endif
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
#if (defined(AE101)||defined(AE102)||defined(AE103))//If it's AE102 or AE103
#define GPIOE0_7_Int_Disable(IO_num)	(GPIO3_INTEN0 &= ~(0x1<<IO_num))
#define GPIOE8_15_Int_Disable(IO_num)	(GPIO3_INTEN1 &= ~(0x1<<IO_num))
#define GPIOE16_23_Int_Disable(IO_num)	(GPIO3_INTEN2 &= ~(0x1<<IO_num))
#else
#define GPIOE0_7_Int_Disable(IO_num)	
#define GPIOE8_15_Int_Disable(IO_num)
#define GPIOE16_23_Int_Disable(IO_num)
#endif
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
#if (defined(AE101)||defined(AE102)||defined(AE103))//If it's AE102 or AE103
#define GPIOE0_7_Int_Mask(IO_num)	(GPIO3_INTMASK0 |= (0x1<<IO_num))
#define GPIOE8_15_Int_Mask(IO_num)	(GPIO3_INTMASK1 |= (0x1<<IO_num))
#define GPIOE16_23_Int_Mask(IO_num)	(GPIO3_INTMASK2 |= (0x1<<IO_num))
#else
#define GPIOE0_7_Int_Mask(IO_num)	
#define GPIOE8_15_Int_Mask(IO_num)
#define GPIOE16_23_Int_Mask(IO_num)
#endif
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
#if (defined(AE101)||defined(AE102)||defined(AE103))//If it's AE102 or AE103
#define GPIOE0_7_Int_Unmask(IO_num)	    (GPIO3_INTMASK0 &= ~(0x1<<IO_num))
#define GPIOE8_15_Int_Unmask(IO_num)	(GPIO3_INTMASK1 &= ~(0x1<<IO_num))
#define GPIOE16_23_Int_Unmask(IO_num)	(GPIO3_INTMASK2 &= ~(0x1<<IO_num))
#else
#define GPIOE0_7_Int_Unmask(IO_num)	
#define GPIOE8_15_Int_Unmask(IO_num)
#define GPIOE16_23_Int_Unmask(IO_num)
#endif
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
#if (defined(AE101)||defined(AE102)||defined(AE103))//If it's AE102 or AE103
#define GPIOE0_7_Int_Type_Edge(IO_num)	    (GPIO3_INTTYPE_LEVEL0 |= (0x1<<IO_num))
#define GPIOE8_15_Int_Type_Edge(IO_num)	    (GPIO3_INTTYPE_LEVEL1 |= (0x1<<IO_num))
#define GPIOE16_23_Int_Type_Edge(IO_num)	(GPIO3_INTTYPE_LEVEL2 |= (0x1<<IO_num))
#else
#define GPIOE0_7_Int_Type_Edge(IO_num)	
#define GPIOE8_15_Int_Type_Edge(IO_num)
#define GPIOE16_23_Int_Type_Edge(IO_num)
#endif
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
#if (defined(AE101)||defined(AE102)||defined(AE103))//If it's AE102 or AE103
#define GPIOE0_7_Int_Type_Level(IO_num)	    (GPIO3_INTTYPE_LEVEL0 &= ~(0x1<<IO_num))
#define GPIOE8_15_Int_Type_Level(IO_num)	(GPIO3_INTTYPE_LEVEL1 &= ~(0x1<<IO_num))
#define GPIOE16_23_Int_Type_Level(IO_num)	(GPIO3_INTTYPE_LEVEL2 &= ~(0x1<<IO_num))
#else
#define GPIOE0_7_Int_Type_Level(IO_num)	
#define GPIOE8_15_Int_Type_Level(IO_num)
#define GPIOE16_23_Int_Type_Level(IO_num)
#endif
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
#if (defined(AE101)||defined(AE102)||defined(AE103))//If it's AE102 or AE103
#define GPIOE0_7_Int_Polarity_High(IO_num)	    (GPIO3_INT_POLARITY0 |= (0x1<<IO_num))
#define GPIOE8_15_Int_Polarity_High(IO_num)	    (GPIO3_INT_POLARITY1 |= (0x1<<IO_num))
#define GPIOE16_23_Int_Polarity_High(IO_num)	(GPIO3_INT_POLARITY2 |= (0x1<<IO_num))
#else
#define GPIOE0_7_Int_Polarity_High(IO_num)	
#define GPIOE8_15_Int_Polarity_High(IO_num)
#define GPIOE16_23_Int_Polarity_High(IO_num)
#endif
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
#if (defined(AE101)||defined(AE102)||defined(AE103))//If it's AE102 or AE103
#define GPIOE0_7_Int_Polarity_Low(IO_num)	(GPIO3_INT_POLARITY0 &= ~(0x1<<IO_num))
#define GPIOE8_15_Int_Polarity_Low(IO_num)	(GPIO3_INT_POLARITY1 &= ~(0x1<<IO_num))
#define GPIOE16_23_Int_Polarity_Low(IO_num)	(GPIO3_INT_POLARITY2 &= ~(0x1<<IO_num))
#else
#define GPIOE0_7_Int_Polarity_Low(IO_num)	
#define GPIOE8_15_Int_Polarity_Low(IO_num)
#define GPIOE16_23_Int_Polarity_Low(IO_num)
#endif
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
#if (defined(AE101)||defined(AE102)||defined(AE103))//If it's AE102 or AE103
#define GPIOE0_7_Int_Enable_Read(IO_num)	((GPIO3_INTEN0 & (0x1<<IO_num))  != 0)
#define GPIOE8_15_Int_Enable_Read(IO_num)	((GPIO3_INTEN1 & (0x1<<IO_num))  != 0)
#define GPIOE16_23_Int_Enable_Read(IO_num)	((GPIO3_INTEN2 & (0x1<<IO_num))  != 0)
#else
#define GPIOE0_7_Int_Enable_Read(IO_num)	0
#define GPIOE8_15_Int_Enable_Read(IO_num)   0
#define GPIOE16_23_Int_Enable_Read(IO_num)   0
#endif
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
#if (defined(AE101)||defined(AE102)||defined(AE103))//If it's AE102 or AE103
#define GPIOE0_7_Int_Mask_Read(IO_num)	    ((GPIO3_INTMASK0 & (0x1<<IO_num)) != 0)
#define GPIOE8_15_Int_Mask_Read(IO_num)	    ((GPIO3_INTMASK1 & (0x1<<IO_num)) != 0)
#define GPIOE16_23_Int_Mask_Read(IO_num)	((GPIO3_INTMASK2 & (0x1<<IO_num)) != 0)
#else
#define GPIOE0_7_Int_Mask_Read(IO_num)	 0
#define GPIOE8_15_Int_Mask_Read(IO_num)	 0
#define GPIOE16_23_Int_Mask_Read(IO_num)	 0
#endif
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
#if (defined(AE101)||defined(AE102)||defined(AE103))//If it's AE102 or AE103
#define GPIOE0_7_Int_Type_Read(IO_num)	    ((GPIO3_INTTYPE_LEVEL0 & (0x1<<IO_num)) != 0)
#define GPIOE8_15_Int_Type_Read(IO_num) 	((GPIO3_INTTYPE_LEVEL1 & (0x1<<IO_num)) != 0)
#define GPIOE16_23_Int_Type_Read(IO_num)	((GPIO3_INTTYPE_LEVEL2 & (0x1<<IO_num)) != 0)
#else
#define GPIOE0_7_Int_Type_Read(IO_num)		 0
#define GPIOE8_15_Int_Type_Read(IO_num)	 0
#define GPIOE16_23_Int_Type_Read(IO_num)	 0
#endif
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
#if (defined(AE101)||defined(AE102)||defined(AE103))//If it's AE102 or AE103
#define GPIOE0_7_Int_Polarity_Read(IO_num)	    ((GPIO3_INT_POLARITY0 & (0x1<<IO_num)) != 0)
#define GPIOE8_15_Int_Polarity_Read(IO_num)	    ((GPIO3_INT_POLARITY1 & (0x1<<IO_num)) != 0)
#define GPIOE16_23_Int_Polarity_Read(IO_num)	((GPIO3_INT_POLARITY2 & (0x1<<IO_num)) != 0)
#else
#define GPIOE0_7_Int_Polarity_Read(IO_num)		 0
#define GPIOE8_15_Int_Polarity_Read(IO_num)	 0
#define GPIOE16_23_Int_Polarity_Read(IO_num)	 0
#endif
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
#if (defined(AE101)||defined(AE102)||defined(AE103))//If it's AE102 or AE103
#define GPIOE0_7_Int_Status(IO_num)	    ((GPIO3_INTSTATUS0 & (0x1<<IO_num))  != 0)
#define GPIOE8_15_Int_Status(IO_num)	((GPIO3_INTSTATUS1 & (0x1<<IO_num))  != 0)
#define GPIOE16_23_Int_Status(IO_num)	((GPIO3_INTSTATUS2 & (0x1<<IO_num))  != 0)
#else
#define GPIOE0_7_Int_Status(IO_num)		 0
#define GPIOE8_15_Int_Status(IO_num)	 0
#define GPIOE16_23_Int_Status(IO_num)	 0
#endif
#ifdef AE103
#define GPIOA HEX01
#define GPIOB HEX02
#define GPIOC HEX03
#define GPIOD HEX04
#define GPIOE HEX05
#endif
#if (defined(AE101)||defined(AE102))
#define GPIOA HEX01
#define GPIOB HEX02
#define GPIOC HEX03
#define GPIOD HEX04
#define GPIOE HEX05
#endif
#ifdef TEST101
#define GPIOA HEX01
#define GPIOB HEX02
#define GPIOC HEX03
#define GPIOD HEX04
#define GPIOE HEX05
#define GPIOF HEX06
#define GPIOG HEX07
#define GPIOH HEX08
#define GPIOI HEX09
#define GPIOJ HEX0A
#define GPIOM HEX0B
#endif
#define ENABLE      HEX01
#define DISABLE     HEX00
#define NUM_OF_GPIOA 32
#define NUM_OF_GPIOB 27
#define NUM_OF_GPIOC 14
#define NUM_OF_GPIOD 7
#define NUM_OF_GPIOE 24
/************GPIO Register define***************/
#define GPIO_DR_HIGH 				HEX00
#define GPIO_DR_LOW					HEX01
#define GPIO_DDR_IN					HEX00
#define GPIO_DDR_OUT				HEX01
#define GPIO_INTEN					HEX01
#define GPIO_INTMASK_EN				HEX01
#define GPIO_INTMASK_DISEN			HEX00
#define	GPIO_INTTYPE_LEVEL_EDGE		HEX01
#define GPIO_INTTYPE_LEVLE_ELEC		HEX00
#define GPIO_INT_POLARITY_HIGH_UP	HEX01
#define GPIO_INT_POLARITY_LOW_DOWN	HEX00
#define GPIO_EOI_CLEAR				HEX01
#define	GPIO_LS_SYNC_EN				HEX01
#define	GPIO_LS_SYNC_DISEN			HEX00
struct GPIOPinState
{
	BYTE default_state_idx;	// default state is mux_state[default_state_idx]
	BYTE mux_state[4]; // 0 is input, 1 is output
};
extern struct GPIOPinState gpio_state_table[];
//******************************************************************************************
//	Extern functions
//******************************************************************************************
extern void sysctl_iomux_uart0(void);
extern void sysctl_iomux_uart1(void);
extern void sysctl_iomux_uart2(void);
extern void sysctl_iomux_uart3(void);
extern void sysctl_iomux_uarta(void);
extern void sysctl_iomux_uartb(void);

extern void sysctl_iomux_disable_uart0(void);
extern void sysctl_iomux_disable_uart1(void);
extern void sysctl_iomux_disable_uart2(void);
extern void sysctl_iomux_disable_uart3(void);
extern void sysctl_iomux_disable_uarta(void);
extern void sysctl_iomux_disable_uartb(void);

extern void sysctl_iomux_ps2_0(void);
extern void sysctl_iomux_ps2_1(void);
extern void sysctl_iomux_spim(void);
extern void sysctl_iomux_spim_cs(void);
extern void sysctl_iomux_pwm0(void);
extern void sysctl_iomux_pwm1(void);
extern void sysctl_iomux_pwm2(void);
extern void sysctl_iomux_pwm3(void);
extern void sysctl_iomux_pwm4(void);
extern void sysctl_iomux_pwm5(void);
extern void sysctl_iomux_pwm6(void);
extern void sysctl_iomux_pwm7(void);
extern void sysctl_iomux_tach0(void);
extern void sysctl_iomux_tach1(void);
extern void sysctl_iomux_tach2(void);
extern void sysctl_iomux_tach3(void);
extern void sysctl_iomux_i2c0(void);
extern void sysctl_iomux_i2c1(void);
extern void sysctl_iomux_i2c2(void);
extern void sysctl_iomux_i2c3(void);
extern void sysctl_iomux_i2c4(void);
extern void sysctl_iomux_i2c5(void);
extern void sysctl_iomux_sci(void);
extern void sysctl_iomux_smi(void);
extern void sysctl_iomux_adc0(void);
extern void sysctl_iomux_adc1(void);
extern void sysctl_iomux_adc2(void);
extern void sysctl_iomux_adc3(void);
extern void sysctl_iomux_adc4(void);
extern void sysctl_iomux_adc5(void);
extern void sysctl_iomux_adc6(void);
extern void sysctl_iomux_adc7(void);
extern void sysctl_iomux_cec(void);
extern void sysctl_iomux_owi(void);
extern void sysctl_iomux_can0(void);
extern void sysctl_iomux_can1(void);
extern void sysctl_iomux_can2(void);
extern void sysctl_iomux_can3(void);
extern void sysctl_iomux_gpioe(void);
extern void sysctl_iomux_kbs(void);
extern void sysctl_iomux_pport(void);
extern void sysctl_iomux_lpc(void);
extern void sysctl_iomux_peci(void);

void sysctl_iomux_config(DWORD port, DWORD io, unsigned port_type);
extern void GPIO_Output_Ctrl(BYTE pin_index, BYTE pin_output_mode);
extern BYTE GPIO_Input_Status_Get(BYTE pin_index);
extern void GPIO_Mode_Ctrl(BYTE pin_index, BYTE pin_dir_mode);
extern void GPIO_Pullup_Ctrl(BYTE pin_index, BYTE pin_pullup_mode);
BYTE  GPIO_Pullup_Config(BYTE  GPIO, BYTE  Num);
void sysctl_mod_reset_finish(DWORD module);
void sysctl_mod_reset_start(DWORD module);
void sysctl_mod_enable(DWORD module);
void sysctl_mod_disable(DWORD module);
extern int GPIO_Config(int GPIO, int gpio_no, int mode, int op_val, int int_lv, int pol);
void GPIO_Input_EN(int GPIO, int gpio_no, char sw);
void GPIOAutoTest(void);
char JUDGE_MODE_IOMUX(DWORD port, DWORD io);
#include "CUSTOM_GPIO.H"
#endif /* _NUCLEI_GPIO_H */
