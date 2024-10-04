/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-10-12 16:08:34
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
#include "KERNEL_SWUC.H"
#include "KERNEL_TIMER.H"
#include "KERNEL_GPIO.H"
/******SWUC_PNP初始化******/
void SWUC_PNP_Config(void)
{
#if (!SWUC_CLOCK_EN)
	dprint("SWUC NO CLOCK");
	return;
#endif
	// HOST LogicaL Device Enable
	SYSCTL_HDEVEN |= (HOST_UARTA_EN | HOST_WUC_EN);
	Config_PNP_Access_Request();
	// PNP Default Config
	Config_PNP_Write(0x23, 0x01, 0x01);
	Config_PNP_Write(0x07, 0x04, 0x04);
	Config_PNP_Write(0x60, 0x04, 0x00);
	Config_PNP_Write(0x61, 0x04, 0x00);
	Config_PNP_Write(0x70, 0x04, 0x0f);
	Config_PNP_Write(0x71, 0x04, 0x01);
	Config_PNP_Write(0x30, 0x04, 0x01);
	Config_PNP_Access_Release();
	dprint("SWUC PNP Config finish!\n");
}
/**
 * @brief SWUC EC端事件中断配置
 *
 * @param    irq_type   中断类型可选：RI1_EC_IRQ RI2_EC_IRQ RING_EC_IRQ SIRQ_EC_IRQ MIRQ_EC_IRQ
 * @param    sw         中断开关可选参数：ENABLE DISABLE
 *
 * @return   无
 */
void SWUC_EventIRQ_Config(BYTE irq_type, BYTE sw)
{
	if(sw == DISABLE)
	{
		SWUHEIE &= (~irq_type);
	}
	else
	{
		SWUHEIE |= irq_type;
	}
}
// 配置RI引脚复用功能
void SWUC_RI_Config(void)
{
	sysctl_iomux_config(GPIOA, 24, 1);
	sysctl_iomux_config(GPIOA, 25, 1);
}
// 配置RING引脚
void SWUC_RING_Config(void)
{
	sysctl_iomux_config(GPIOA, 15, 1);
}
// 软件唤醒
void SWUC_SW_WakeUp_Config(uint8_t hsecm_ec)
{
	if(hsecm_ec == 0)
	{
		SWUCTL1 &= 0xdf;
	}
	else
	{
		SWUCTL1 |= 0x20;
	}
}
void SWUC_PWUREQ_Config(void)
{
	sysctl_iomux_config(GPIOA, 23, 1);
}
void SWUC_SMI_Config(void)
{
	sysctl_iomux_config(GPIOA, 28, 1);
}
void SWUC_KBRST_Config(void)
{
	sysctl_iomux_config(GPIOA, 14, 2);
}
void SWUC_G20_Config(void)
{
	sysctl_iomux_config(GPIOA, 13, 2);
}
void SWUC_Int_Enable(BYTE type)
{
	if(type <= 0x7)
	{
		SWUHEIE |= (0x1 << type);
	}
	else
	{
		SWUIE |= (0x1 << (type - 0x8));
	}
}
/**
 * @brief SWUC EC端传统模式或ACPI模式唤醒中断配置
 *
 * @param    irq_type   中断类型可选：RTC_IRQ ACPI_S1 ...
 * @param    sw         中断开关可选参数：ENABLE DISABLE
 *
 * @return   无
 */
void SWUC_ACPIIRQ_Config(BYTE irq_type, BYTE sw)
{
	if(sw == DISABLE)
	{
		SWUIE &= (~irq_type);
	}
	else
	{
		SWUIE |= irq_type;
	}
}
void SWUC_Int_Disable(BYTE type)
{
	if(type <= 0x7)
	{
		SWUHEIE &= ~(0x1 << type);
	}
	else
	{
		SWUIE &= ~(0x1 << (type - 0x8));
	}
}
BYTE SWUC_Int_Enable_Read(BYTE type)
{
	if(type <= 0x7)
	{
		return ((SWUHEIE & (0x1 << type)) != 0);
	}
	else
	{
		return ((SWUIE & (0x1 << (type - 0x8))) != 0);
	}
}
/*测试代码仅供参考*/
// 模块中断唤醒测试
void SWUC_UART_Test(void)
{
	// LogicaLDevice Enable
	SYSCTL_HDEVEN |= (HOST_UARTA_EN | HOST_WUC_EN);
	Config_PNP_Access_Request();
	// PNP Default Config
	Config_PNP_Write(0x23, 0x01, 0x01);
	vDelayXms(5);
#if (defined(AE102) || defined(AE103))
	// UART0_BAUD
	Config_PNP_Write(0x07, 0x1b, 0x1b);
	vDelayXms(5);
	Config_PNP_Write(0x60, 0x1b, 0x03);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x1b, 0xf8);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x1b, 0x14);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x1b, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x1b, 0x01);
	vDelayXms(5);
	dprint("UART0_BAUD PNP Config finish!\n");
#endif
	// SWUC
	Config_PNP_Write(0x07, 0x04, 0x04);
	vDelayXms(5);
	Config_PNP_Write(0x60, 0x04, 0x00);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x04, 0x00);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x04, 0x0f);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x04, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x04, 0x01);
	vDelayXms(5);
	dprint("SWUC PNP Config finish!\n");
	// Enable SWCHIER all interrupt
	SWUC_EventIRQ_Config(RI1_EC_IRQ | RI2_EC_IRQ | RING_EC_IRQ | SIRQ_EC_IRQ | MIRQ_EC_IRQ, ENABLE);
	// Logic Device Module Config
	PRINTF_IER |= 0x1; // Enable uart0 interrup
}
// RI1和RI2信号唤醒测试
void SWUC_RI1_RI2_Test(void)
{
	SWUC_PNP_Config();
	SWUC_EventIRQ_Config(RI1_EC_IRQ | RI2_EC_IRQ | RING_EC_IRQ | SIRQ_EC_IRQ | MIRQ_EC_IRQ, ENABLE);
	SWUC_RI_Config();
}
// RING信号唤醒测试
void SWUC_Ring_Test(void)
{
	SWUC_PNP_Config();
	SWUC_EventIRQ_Config(RI1_EC_IRQ | RI2_EC_IRQ | RING_EC_IRQ | SIRQ_EC_IRQ | MIRQ_EC_IRQ, ENABLE);
	SWUC_RING_Config();
}
// 软件唤醒
void SWUC_Soft_Test(void)
{
	SWUC_PNP_Config();
	SWUC_EventIRQ_Config(RI1_EC_IRQ | RI2_EC_IRQ | RING_EC_IRQ | SIRQ_EC_IRQ | MIRQ_EC_IRQ, ENABLE);
	SWUC_SW_WakeUp_Config(0);
}
// 传统模式唤醒
void SWUC_LegacyMode_Test(void)
{
	SWUC_PNP_Config();
	SWUC_ACPIIRQ_Config(PWRSLY | PWRBT, ENABLE);
}
// acpi唤醒事件产生中断
void SWUC_ACPI_Init(void)
{
	SWUC_PNP_Config();
	// ACPI status change wakeup Config
	Config_PNP_Access_Request();
	Config_PNP_Write(0x2d, 0x01, 0x01); // select ACPI mode
	Config_PNP_Access_Release();
	vDelayXms(5);
	SWUC_ACPIIRQ_Config(ACPI_S1 | ACPI_S2 | ACPI_S3 | ACPI_S4 | ACPI_S5, ENABLE); // set all s1-s5 interrupt enable
}
// 产生PWUREQ信号测试
void SWUC_PWUREQ_Init(void)
{
	SWUC_PNP_Config();
	SWUC_EventIRQ_Config(RI1_EC_IRQ | RI2_EC_IRQ | RING_EC_IRQ | SIRQ_EC_IRQ | MIRQ_EC_IRQ, ENABLE); // Enable SWCHIER all interrupt
	SWUC_RING_Config();																				 // RING Config
	SWUC_PWUREQ_Config();																			 // set GPC7 PWUREQ
}
// 产生SMI信号
void SWUC_SMI_Init(void)
{
	SWUC_PNP_Config();
	SWUC_RING_Config();
	SWUC_SMI_Config();
}
// 3种方式选择KBRST
void SWUC_KBRST_Init(uint8_t mode)
{
	SWUC_KBRST_Config(); // set GPB6 KBRST
	if(mode == 1)
	{
		KBC_KOB &= 0xfe;   // set KBCHIKDOR bit0 0
		(SWUCTL1) |= 0x80; // set krstga20r 1
	}
	else if(mode == 2)
	{
		sysctl_iomux_config(GPIOB, 6, 1); // set GPE6 lpc_pd
		SWUCTL3 |= 0x2;					  // set lpcpf_ec 1
	}
	else if(mode == 3)
	{
		sysctl_iomux_config(GPIOA, 14, 2);
		for(BYTE i = 0; i < 20; i++)
		{
			SWUCTL1 &= 0xfe; // set hrst_ec 0
			vDelayXms(5);
			SWUCTL1 |= 0x01; // set hrst_ec 1
			vDelayXms(5);
		}
	}
}
/**
 * @brief SWUC GPIOA20 Init
 *
 * @param    mode   1 set
 *
 * @return   无
 */
void SWUC_GA20_Init(uint8_t mode)
{
	SWUC_G20_Config(); // set GPIOA20
	if(mode == 1)
	{
		KBC_KOB &= 0xfd; // set KBCHIKDOR bit1 0
		for(BYTE i = 0; i < 20; i++)
		{
			(SWUCTL1) &= 0x7f; // set krstga20r 0
			vDelayXms(5);
			(SWUCTL1) |= 0x80; // set krstga20r 1
			vDelayXms(5);
		}
	}
	else
	{
		sysctl_iomux_config(GPIOB, 6, 1); // set GPE6 lpc_pd
		SWUCTL3 |= 0x2;					  // set lpcpf_ec 1
	}
}