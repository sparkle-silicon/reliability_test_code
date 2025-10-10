/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-09 17:27:56
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
#include "KERNEL_GPIO.H"
#include "AE_FUNC.H"
#include "AE_GLOBAL.H"
struct GPIOPinState gpio_state_table[] =
{
	// GPIOA
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 0, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 0, 0}},
	{0, {1, 1, 0, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 1, 0}},
	{0, {1, 0, 1, 0}},
	{0, {1, 0, 1, 0}},
	{0, {1, 1, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 0}},
	{0, {1, 0, 1, 0}},
	{0, {1, 0, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 0, 0, 1}},
	{0, {1, 0, 0, 1}},
	{0, {0, 0, 0, 0}}, // 23
	{0, {1, 0, 0, 1}},
	{0, {1, 0, 1, 1}},
	{1, {1, 0, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 0, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	// GPIOB
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 1, 1, 0}},
	{0, {1, 1, 1, 0}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 0}},
	{0, {1, 0, 1, 0}},
	{3, {1, 1, 1, 1}},
	{3, {1, 0, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 0, 0, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 1, 0, 0}},
	{0, {1, 0, 0, 0}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 1, 1, 1}},
	// GPIOC
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 0, 1}},
	{0, {1, 0, 0, 1}},
	{0, {1, 0, 0, 1}},
	{0, {1, 1, 1, 0}},
	{0, {1, 0, 1, 0}},
	{0, {1, 1, 0, 1}},
	{0, {1, 0, 0, 1}},
	{0, {1, 0, 0, 0}},
	{0, {1, 1, 0, 0}},
	// GPIOD
	{1, {1, 0, 1, 1}},
	{1, {1, 0, 1, 1}},
	{1, {1, 0, 1, 1}},
	{1, {1, 0, 1, 1}},
	{1, {1, 0, 1, 1}},
	{1, {1, 0, 1, 1}},
	{1, {1, 0, 1, 1}},
	// GPIOE
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 1, 1}},
	{1, {1, 0, 1, 1}},
	{1, {1, 0, 1, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 1, 0}},
	{1, {1, 0, 1, 0}},
	{1, {1, 0, 1, 0}},
	{1, {1, 0, 1, 0}},
};
//*****************************************************************************
//
//  Reserved
//
//  parameter :
//      module :
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_mod_disable(DWORD module)
{
#if (SYSCTL_CLOCK_EN)
	SYSCTL_MODEN0 &= (~(1 << (module % 32)));
#endif
}
void sysctl_mod_enable(DWORD module)
{
#if (SYSCTL_CLOCK_EN)
	SYSCTL_MODEN0 |= (1 << (module % 32));
#endif
}
void sysctl_mod_reset_start(DWORD module)
{
#if (SYSCTL_CLOCK_EN)
	SYSCTL_RST0 |= (1 << (module % 32));
#endif
}
void sysctl_mod_reset_finish(DWORD module)
{
#if (SYSCTL_CLOCK_EN)
	SYSCTL_RST0 &= (~(1 << (module % 32)));
#endif
}
void enable_necessary_input(DWORD port, DWORD io, unsigned port_type)
{
	int idx = 0;
	if(port == GPIOA)
	{
		idx = io;
	}
	else if(port == GPIOB)
	{
		idx = NUM_OF_GPIOA + io;
	}
	else if(port == GPIOC)
	{
		idx = NUM_OF_GPIOA + NUM_OF_GPIOB + io;
	}
	else if(port == GPIOD)
	{
		idx = NUM_OF_GPIOA + NUM_OF_GPIOB + NUM_OF_GPIOC + io;
	}
	else if(port == GPIOE)
	{
		idx = NUM_OF_GPIOA + NUM_OF_GPIOB + NUM_OF_GPIOC + NUM_OF_GPIOD + io;
	}
	int state = gpio_state_table[idx].mux_state[port_type];
	if(state)
	{
		GPIO_Input_EN(port, io, DISABLE);
	}
	else
	{
		GPIO_Input_EN(port, io, ENABLE);
	}
	return;
}
/**
 * @brief GPIO引脚复用功能配置
 *
 * @param    port     		可选参数：GPIOA ,GPIOB...GPIOE
 *
 * @param    io      		GPIO序列号选择如：GPIOA10
 *
 * @param    port_type      复用功能选择：可选参数0-3
 *
 * @return   无
 */
const char *IOMUX_NUM_ERROR = "GPIO%X[%d] iomux config error\n";
void sysctl_iomux_config(DWORD port, DWORD io, unsigned port_type)
{
	if(port == GPIOA)
	{
		if(io <= 15)
		{
			SYSCTL_PIO0_CFG &= ~(011 << (io << 1));
			SYSCTL_PIO0_CFG |= ((port_type & 0b11) << (io << 1));
		}
		else if(io <= 31)
		{
			io -= 16;
			SYSCTL_PIO1_CFG &= ~(011 << (io << 1));
			SYSCTL_PIO1_CFG |= ((port_type & 0b11) << (io << 1));
		}
		else
		{
			goto Error_Return;
		}
	}
	else if(port == GPIOB)
	{
		if(io <= 15)
		{
			SYSCTL_PIO2_CFG &= ~(011 << (io << 1));
			SYSCTL_PIO2_CFG |= ((port_type & 0b11) << (io << 1));
		}
		else if(io <= 31)
		{
			io -= 16;
			SYSCTL_PIO3_CFG &= ~(011 << (io << 1));
			SYSCTL_PIO3_CFG |= ((port_type & 0b11) << (io << 1));
		}
		else
		{
			goto Error_Return;
		}
	}
	else if(port == GPIOC)
	{
		if(io <= 15)
		{
			SYSCTL_PIO4_CFG &= ~(011 << (io << 1));
			SYSCTL_PIO4_CFG |= ((port_type & 0b11) << (io << 1));
		}
		else
		{
			goto Error_Return;
		}
	}
	else if(port == GPIOD)
	{
		if(io <= 8)
		{
			SYSCTL_PIO5_CFG &= ~(011 << (io << 1));
			SYSCTL_PIO5_CFG |= ((port_type & 0b11) << (io << 1));
		}
		else
		{
			goto Error_Return;
		}
	}
	else if(port == GPIOE)
	{
		if((io >= 10) && (io <= 15))
		{
			SYSCTL_PIO5_CFG &= ~(0b11 << (io << 1));
			SYSCTL_PIO5_CFG |= ((port_type & 0b11) << (io << 1));
		}
		else if(((io >= 16) && (io <= 23)) || ((io >= 0) && (io <= 9)))
		{
			SYSCTL_PIO5_CFG &= ~(011 << 18);
			SYSCTL_PIO5_CFG |= ((port_type & 011) << 18);
		}
		else
		{
			goto Error_Return;
		}
	}
	else
	{
		goto Error_Return;
	}
	return;
Error_Return:
	dprint(IOMUX_NUM_ERROR, (port + 9), io);
	return;
	// #endif
}
//*****************************************************************************
//
//  To setup uart0 iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_uart0()
{
	sysctl_iomux_config(GPIOA, 24, 2);
	sysctl_iomux_config(GPIOA, 25, 2);
}
void sysctl_iomux_disable_uart0()
{
	sysctl_iomux_config(GPIOA, 24, 0);
	sysctl_iomux_config(GPIOA, 25, 0);
}
//*****************************************************************************
//
//  To setup uart1 iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_uart1(uint32_t tx_sel, uint32_t rx_sel)
{
	if(tx_sel == 0)
		sysctl_iomux_config(GPIOB, 1, 1);//tx
	else if(tx_sel == 1)
		sysctl_iomux_config(GPIOE, 15, 3);//tx
	if(rx_sel == 0)
		sysctl_iomux_config(GPIOB, 3, 1);//rx
	else if(rx_sel == 1)
		sysctl_iomux_config(GPIOE, 14, 3);//rx

}
void sysctl_iomux_disable_uart1(uint32_t tx_sel, uint32_t rx_sel)
{
	if(tx_sel == 0)
		sysctl_iomux_config(GPIOB, 1, 0);//tx
	else if(tx_sel == 1)
		sysctl_iomux_config(GPIOE, 15, 1);//tx
	if(rx_sel == 0)
		sysctl_iomux_config(GPIOB, 3, 0);//rx
	else if(rx_sel == 1)
		sysctl_iomux_config(GPIOE, 14, 1);//rx
}
//*****************************************************************************
//
//  To setup uarta iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_uarta(uint32_t tx_sel, uint32_t rx_sel)
{
	if(tx_sel == 0)
		sysctl_iomux_config(GPIOA, 9, 2);//tx
	else if(tx_sel == 1)
		sysctl_iomux_config(GPIOB, 6, 3);//tx
	if(rx_sel == 0)
		sysctl_iomux_config(GPIOA, 8, 2);//rx
	else if(rx_sel == 1)
		sysctl_iomux_config(GPIOA, 23, 2);//rx
}
void sysctl_iomux_disable_uarta(uint32_t tx_sel, uint32_t rx_sel)
{
	if(tx_sel == 0)
		sysctl_iomux_config(GPIOA, 9, 0);//tx
	else if(tx_sel == 1)
		sysctl_iomux_config(GPIOB, 6, 0);//tx
	if(rx_sel == 0)
		sysctl_iomux_config(GPIOA, 8, 0);//rx
	else if(rx_sel == 1)
		sysctl_iomux_config(GPIOA, 23, 0);//rx
}
//*****************************************************************************
//
//  To setup uartb iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_uartb()
{
	UARTB_SEL;// select uartb
	sysctl_iomux_config(GPIOB, 25, 2);//rx
	sysctl_iomux_config(GPIOB, 26, 2);//tx
}
void sysctl_iomux_disable_uartb()
{
	sysctl_iomux_config(GPIOB, 25, 0);
	sysctl_iomux_config(GPIOB, 26, 0);
}
//*****************************************************************************
//
//  To setup ps2 iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_ps2_0(uint32_t clk_sel, uint32_t data_sel)
{
	if(clk_sel == 0)
		sysctl_iomux_config(GPIOB, 8, 1);
	else if(clk_sel == 1)
		sysctl_iomux_config(GPIOB, 10, 1);
	else if(clk_sel == 2)
		sysctl_iomux_config(GPIOB, 27, 1);
	if(data_sel == 0)
		sysctl_iomux_config(GPIOB, 9, 1);
	else if(data_sel == 1)
		sysctl_iomux_config(GPIOB, 11, 1);
	else if(data_sel == 2)
		sysctl_iomux_config(GPIOB, 28, 1);
}
void sysctl_iomux_ps2_1(uint32_t clk_sel, uint32_t data_sel)
{
	if(clk_sel == 0)
		sysctl_iomux_config(GPIOB, 12, 1);
	else if(clk_sel == 1)
		sysctl_iomux_config(GPIOB, 10, 3);
	if(data_sel == 0)
		sysctl_iomux_config(GPIOB, 13, 1);
	else if(data_sel == 1)
		sysctl_iomux_config(GPIOB, 11, 3);
}
//*****************************************************************************
//
//  To setup spim iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_spim()
{
	sysctl_iomux_config(GPIOA, 6, 2);  // spim_sck
	sysctl_iomux_config(GPIOA, 19, 2); // spim_mosi
	sysctl_iomux_config(GPIOA, 21, 2); // spim_miso
	if(SPIM_QE_SEL)
	{
		sysctl_iomux_config(GPIOC, 15, 2); // spim_io2
		sysctl_iomux_config(GPIOC, 14, 2); // spim_io3
	}
}
void sysctl_iomux_spim_cs()
{
	if(SPIM_CS_SEL == 0)
		sysctl_iomux_config(GPIOB, 18, 2); // csn0
	else
		sysctl_iomux_config(GPIOB, 4, 2); // csn1
}

//*****************************************************************************
//
//  To setup pwm0-7 iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_pwm0()
{
	sysctl_iomux_config(GPIOA, 0, 1);
}
void sysctl_iomux_pwm1()
{
	sysctl_iomux_config(GPIOA, 1, 1);
}
void sysctl_iomux_pwm2()
{
	sysctl_iomux_config(GPIOA, 2, 1);
}
void sysctl_iomux_pwm3()
{
	sysctl_iomux_config(GPIOA, 3, 1);
}
void sysctl_iomux_pwm4()
{
	sysctl_iomux_config(GPIOA, 4, 1);
}
void sysctl_iomux_pwm5()
{
	sysctl_iomux_config(GPIOA, 5, 1);
}
void sysctl_iomux_pwm6()
{
	sysctl_iomux_config(GPIOA, 6, 1);
}
void sysctl_iomux_pwm7()
{
	sysctl_iomux_config(GPIOA, 7, 1);
}
//*****************************************************************************
//
//  To setup tach0-3 iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_tach0(uint32_t tach_sel)
{
	if(tach_sel == 0)
		sysctl_iomux_config(GPIOA, 30, 1); // tach0
	else if(tach_sel == 1)
		sysctl_iomux_config(GPIOA, 22, 1); // tach0
}
void sysctl_iomux_tach1(uint32_t tach_sel)
{
	if(tach_sel == 0)
		sysctl_iomux_config(GPIOA, 31, 1); // tach1
	else if(tach_sel == 1)
		sysctl_iomux_config(GPIOC, 9, 2); // tach1
	else if(tach_sel == 2)
		sysctl_iomux_config(GPIOC, 14, 1); // tach1
}
void sysctl_iomux_tach2()
{
	sysctl_iomux_config(GPIOC, 8, 1); // tach2
}
void sysctl_iomux_tach3(uint32_t tach_sel)
{
	if(tach_sel == 0)
		sysctl_iomux_config(GPIOC, 9, 1);
	else if(tach_sel == 1)
		sysctl_iomux_config(GPIOC, 11, 2); // tach30


}
//*****************************************************************************
//
//  To setup i2c iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_i2c0(uint32_t clk_sel, uint32_t data_sel)
{
	if(clk_sel == 0)
		sysctl_iomux_config(GPIOA, 11, 1);//clk
	else if(clk_sel == 1)
		sysctl_iomux_config(GPIOB, 10, 2);//clk
	if(data_sel == 0)
		sysctl_iomux_config(GPIOA, 12, 1);
	else if(data_sel == 1)
		sysctl_iomux_config(GPIOB, 11, 2);
}
void sysctl_iomux_i2c1()
{
	sysctl_iomux_config(GPIOA, 17, 1);//clk
	sysctl_iomux_config(GPIOA, 18, 1);
}
void sysctl_iomux_i2c2(uint32_t clk_sel)
{
	if(clk_sel == 0)
		sysctl_iomux_config(GPIOA, 23, 3);//clk
	else if(clk_sel == 1)
		sysctl_iomux_config(GPIOB, 14, 1);//clk
	sysctl_iomux_config(GPIOB, 15, 1);

}
void sysctl_iomux_i2c3()
{
	SMBUS3_SEL;// select smbus3
	sysctl_iomux_config(GPIOB, 25, 2);
	sysctl_iomux_config(GPIOB, 26, 2);
}
void sysctl_iomux_i2c4()
{
	sysctl_iomux_config(GPIOB, 0, 2);
	sysctl_iomux_config(GPIOB, 7, 2);
}
void sysctl_iomux_i2c5()
{
	sysctl_iomux_config(GPIOA, 4, 3);
	sysctl_iomux_config(GPIOA, 5, 3);
}
void sysctl_iomux_i2c6()
{
	sysctl_iomux_config(GPIOA, 24, 3);//clk
	sysctl_iomux_config(GPIOA, 25, 3);
}
void sysctl_iomux_i2c7()
{
	sysctl_iomux_config(GPIOA, 10, 2);//clk
	sysctl_iomux_config(GPIOB, 24, 2);
}
void sysctl_iomux_i2c8()
{
	sysctl_iomux_config(GPIOA, 13, 3);//clk
	sysctl_iomux_config(GPIOC, 14, 3);
}
//*****************************************************************************
//
//  To setup i3c iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_master0()
{
	sysctl_iomux_config(GPIOC, 11, 3);//i3c0_scl
	sysctl_iomux_config(GPIOC, 12, 3);//i3c0_sda
}
void sysctl_iomux_master1()
{
	sysctl_iomux_config(GPIOC, 13, 3);//i3c1_scl
	sysctl_iomux_config(GPIOB, 1, 3); //i3c1_sda
}
void sysctl_iomux_slave0()
{
	sysctl_iomux_config(GPIOB, 2, 3);//i3c2_scl
	sysctl_iomux_config(GPIOB, 3, 3);//i3c2_sda
}
void sysctl_iomux_slave1()
{
	sysctl_iomux_config(GPIOB, 8, 3);//i3c3_scl
	sysctl_iomux_config(GPIOB, 9, 3);//i3c3_sda
}

//*****************************************************************************
//
//  To setup pmc iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_sci()
{
	sysctl_iomux_config(GPIOA, 27, 1);//sci
}
void sysctl_iomux_smi()
{
	sysctl_iomux_config(GPIOA, 27, 1);//sci
}
//*****************************************************************************
//
//  To setup adc iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_adc0()
{
	sysctl_iomux_config(GPIOC, 0, 1);

}
void sysctl_iomux_adc1()
{
	sysctl_iomux_config(GPIOC, 1, 1);
}
void sysctl_iomux_adc2()
{
	sysctl_iomux_config(GPIOC, 2, 1);
}
void sysctl_iomux_adc3()
{
	sysctl_iomux_config(GPIOC, 3, 1);
}
void sysctl_iomux_adc4()
{
	sysctl_iomux_config(GPIOC, 4, 1);
}
void sysctl_iomux_adc5()
{
	sysctl_iomux_config(GPIOC, 5, 1);
}
void sysctl_iomux_adc6()
{
	sysctl_iomux_config(GPIOC, 6, 1);
}
void sysctl_iomux_adc7()
{
	sysctl_iomux_config(GPIOC, 7, 1);
}
//*****************************************************************************
//
//  To setup cec iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_cec0(uint8_t cec0_sel)
{
	if(cec0_sel == 0)
		sysctl_iomux_config(GPIOA, 13, 1);
	else
		sysctl_iomux_config(GPIOB, 8, 2);
}
void sysctl_iomux_cec1(void)
{
	sysctl_iomux_config(GPIOB, 31, 2);
}
void sysctl_iomux_owi()
{
	sysctl_iomux_config(GPIOA, 29, 1);
	sysctl_iomux_config(GPIOB, 5, 1);
}
void sysctl_iomux_peci()
{
	sysctl_iomux_config(GPIOB, 14, 2);
	sysctl_iomux_config(GPIOB, 15, 2);
}

//*****************************************************************************
//
//  To setup gpio or kbs or pport iomux
//
//  parameter :
//      func :0:gpio,1:kbs,2:crypto jtag 
//
//  return :
//      none
//
//*****************************************************************************
static void sysctl_iomux_switch_kbs_jtag(VBYTE func)
{
	if(func <= 1)
	{
		sysctl_iomux_config(GPIOE, 0, (func & 0x3));
		sysctl_iomux_config(GPIOE, 10, (func & 0x3));
		sysctl_iomux_config(GPIOE, 11, (func & 0x3));
		sysctl_iomux_config(GPIOE, 12, (func & 0x3));
		sysctl_iomux_config(GPIOE, 13, (func & 0x3));
		sysctl_iomux_config(GPIOE, 14, (func & 0x3));
		sysctl_iomux_config(GPIOE, 15, (func & 0x3));
	}
	else
	{

	}
}
//*****************************************************************************
//
//  To setup gpioe  iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_gpioe(void)
{
	sysctl_iomux_switch_kbs_jtag(0);
}
//*****************************************************************************
//
//  To setup kbs  iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_kbs(void)
{
	sysctl_iomux_switch_kbs_jtag(1);
#if (KBD_8_n_SWITCH == 17||KBD_8_n_SWITCH == 18)    
	sysctl_iomux_config(GPIOA, 19, 1); // C3 kso 16
#endif
#if(KBD_8_n_SWITCH == 18)
	sysctl_iomux_config(GPIOA, 21, 1); // C3 kso 17
#endif
}
//*****************************************************************************
//
//  To setup jtag  iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_crypto_jtag(void)
{
	sysctl_iomux_switch_kbs_jtag(2);
}
void sysctl_iomux_jtag(void)
{
	sysctl_iomux_switch_kbs_jtag(3);
}
//*****************************************************************************
//
//  To setup lpc iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_lpc(void)
{
	sysctl_iomux_config(GPIOD, 0, 1);//lad[0]
	sysctl_iomux_config(GPIOD, 1, 1);//lad[1]
	sysctl_iomux_config(GPIOD, 2, 1);//lad[2]
	sysctl_iomux_config(GPIOD, 3, 1);//lad[3]
	sysctl_iomux_config(GPIOD, 4, 1);//lpcclk
	sysctl_iomux_config(GPIOD, 5, 1);//lframe
	sysctl_iomux_config(GPIOD, 6, 1);//serirq
}
//*****************************************************************************
//
//  To setup lpc iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_espi(void)
{
#if AE5571
	sysctl_iomux_config(GPIOD, 0, 1);//eio[0]
	sysctl_iomux_config(GPIOD, 1, 1);//eio[1]
	sysctl_iomux_config(GPIOD, 2, 1);//eio[2]
	sysctl_iomux_config(GPIOD, 3, 1);//eio[3]
	sysctl_iomux_config(GPIOD, 4, 1);//esck
	sysctl_iomux_config(GPIOD, 5, 1);//ecs
	sysctl_iomux_config(GPIOD, 6, 1);//alert
#endif
}
/**
 * @brief Pin IO上拉设置
 *
 * @param    GPIO     可选参数：GPIOA ,GPIOB...GPIOE
 *
 * @param    Num      号码选择如：GPIOA[Num] Num可选0-31
 *
 * @return   无
 */
BYTE PIO_Pullup_Config(BYTE GPIO, BYTE Num)
{
	switch(GPIO)
	{
		case GPIOA:
			SYSCTL_PIO0_UDCFG |= (0x1 << Num);
			break;
		case GPIOB:
			SYSCTL_PIO1_UDCFG |= (0x1 << Num);
			break;
		case GPIOC:
			SYSCTL_PIO2_UDCFG |= (0x1 << Num);
			break;
		case GPIOD:
			SYSCTL_PIO2_UDCFG |= (0x1 << (Num + 16));
			break;
		case GPIOE:
			SYSCTL_PIO3_UDCFG |= (0x1 << Num);
			break;
		default:
			dprint("参数选择错误\n");
			break;
	}

	return 0;
}
//*****************************************************************************
//
//  To setup i2c pull up
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void i2c0_pull_up(uint32_t clk_sel, uint32_t data_sel)
{
	if(clk_sel == 0)
	{
		if((I2C0_EXTERNAL_PULL_UP == 0) && (IS_GPIOA11(LOW) || I2C0_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOA, 11);
	}
	else if(clk_sel == 1)
	{
		if((I2C0_EXTERNAL_PULL_UP == 0) && (IS_GPIOB10(LOW) || I2C0_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 10);
	}
	if(data_sel == 0)
	{
		if((I2C0_EXTERNAL_PULL_UP == 0) && (IS_GPIOA12(LOW) || I2C0_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOA, 12);
	}
	else if(data_sel == 1)
	{
		if((I2C0_EXTERNAL_PULL_UP == 0) && (IS_GPIOB11(LOW) || I2C0_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 11);
	}
}
void i2c1_pull_up(void)
{
	{
		if((I2C1_EXTERNAL_PULL_UP == 0) && (IS_GPIOA17(LOW) || I2C1_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOA, 17);
	}
	{
		if((I2C1_EXTERNAL_PULL_UP == 0) && (IS_GPIOA18(LOW) || I2C1_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOA, 18);
	}
}
void i2c2_pull_up(uint32_t clk_sel)
{
	if(clk_sel == 0)
	{
		if((I2C2_EXTERNAL_PULL_UP == 0) && (IS_GPIOA23(LOW) || I2C2_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOA, 23);
	}
	else if(clk_sel == 1)
	{
		if((I2C2_EXTERNAL_PULL_UP == 0) && (IS_GPIOB14(LOW) || I2C2_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 14);
	}
	{
		if((I2C2_EXTERNAL_PULL_UP == 0) && (IS_GPIOB15(LOW) || I2C2_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 15);
	}
}
void i2c3_pull_up(void)
{
	{
		if((I2C3_EXTERNAL_PULL_UP == 0) && (IS_GPIOB25(LOW) || I2C3_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 25);
	}
	{
		if((I2C3_EXTERNAL_PULL_UP == 0) && (IS_GPIOB26(LOW) || I2C3_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 26);
	}
}
void i2c4_pull_up(void)
{
	{
		if((I2C5_EXTERNAL_PULL_UP == 0) && (IS_GPIOB0(LOW) || I2C5_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 0);
	}
	{
		if((I2C5_EXTERNAL_PULL_UP == 0) && (IS_GPIOB7(LOW) || I2C5_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 7);
	}
}
void i2c5_pull_up(void)
{
	{
		if((I2C4_EXTERNAL_PULL_UP == 0) && (IS_GPIOA4(LOW) || I2C4_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOA, 4);
	}
	{
		if((I2C4_EXTERNAL_PULL_UP == 0) && (IS_GPIOA5(LOW) || I2C4_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOA, 5);
	}

}
void i2c6_pull_up(void)
{
	{
		if((I2C6_EXTERNAL_PULL_UP == 0) && (IS_GPIOA24(LOW) || I2C6_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOA, 24);  //SMCLK6
	}
	{
		if((I2C6_EXTERNAL_PULL_UP == 0) && (IS_GPIOA25(LOW) || I2C6_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOA, 25);  //SMDAT6
	}
}
void i2c7_pull_up(void)
{
	{
		if((I2C7_EXTERNAL_PULL_UP == 0) && (IS_GPIOA10(LOW) || I2C7_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOA, 10);  //SMCLK7
	}
	{
		if((I2C7_EXTERNAL_PULL_UP == 0) && (IS_GPIOB24(LOW) || I2C7_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 24);  //SMDAT7
	}
}
void i2c8_pull_up(void)
{
	{
		if((I2C8_EXTERNAL_PULL_UP == 0) && (IS_GPIOA13(LOW) || I2C8_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOA, 13);  //SMCLK8
	}
	{
		if((I2C8_EXTERNAL_PULL_UP == 0) && (IS_GPIOC14(LOW) || I2C8_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOC, 14);  //SMDAT8
	}
}
//*****************************************************************************
//
//  To setup tach pull up
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void tach0_pull_up(uint32_t tach_sel)
{
	if(tach_sel == 0)
	{
		if((TACH0_EXTERNAL_PULL_UP == 0) && (IS_GPIOA30(LOW) || TACH0_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOA, 30);
	}
	else if(tach_sel == 1)
	{
		if((TACH0_EXTERNAL_PULL_UP == 0) && (IS_GPIOA22(LOW) || TACH0_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOA, 22);
	}

}
void tach1_pull_up(uint32_t tach_sel)
{
	if(tach_sel == 0)
	{
		if((TACH1_EXTERNAL_PULL_UP == 0) && (IS_GPIOA31(LOW) || TACH1_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOA, 31);
	}
	else if(tach_sel == 1)
	{
		if((TACH1_EXTERNAL_PULL_UP == 0) && (IS_GPIOC9(LOW) || TACH1_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOC, 9);
	}
	else if(tach_sel == 2)
	{
		if((TACH1_EXTERNAL_PULL_UP == 0) && (IS_GPIOC14(LOW) || TACH1_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOC, 14);
	}

}
void tach2_pull_up(void)
{
	{
		if((TACH2_EXTERNAL_PULL_UP == 0) && (IS_GPIOC8(LOW) || TACH2_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOC, 8);
	}
}
void tach3_pull_up(uint32_t tach_sel)
{
	if(tach_sel == 0)
	{
		if((TACH3_EXTERNAL_PULL_UP == 0) && (IS_GPIOC9(LOW) || TACH3_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOC, 9);
	}
	else if(tach_sel == 1)
	{
		if((TACH3_EXTERNAL_PULL_UP == 0) && (IS_GPIOC11(LOW) || TACH3_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOC, 11);
	}
}
//*****************************************************************************
//
//  To setup ps2 pull up
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void ps2_0_pull_up(uint32_t clk_sel, uint32_t data_sel)
{
	if(clk_sel == 0)
	{
		if((PS2_0_EXTERNAL_PULL_UP == 0) && (IS_GPIOB8(LOW) || PS2_0_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 8);
	}
	else if(clk_sel == 1)
	{
		if((PS2_0_EXTERNAL_PULL_UP == 0) && (IS_GPIOB10(LOW) || PS2_0_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 10);
	}
	else if(clk_sel == 2)
	{
		if((PS2_0_EXTERNAL_PULL_UP == 0) && (IS_GPIOB27(LOW) || PS2_0_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 27);
	}
	if(data_sel == 0)
	{
		if((PS2_0_EXTERNAL_PULL_UP == 0) && (IS_GPIOB9(LOW) || PS2_0_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 9);
	}
	else if(data_sel == 1)
	{
		if((PS2_0_EXTERNAL_PULL_UP == 0) && (IS_GPIOB11(LOW) || PS2_0_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 11);
	}
	else if(data_sel == 2)
	{
		if((PS2_0_EXTERNAL_PULL_UP == 0) && (IS_GPIOB28(LOW) || PS2_0_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 28);
	}
}
void ps2_1_pull_up(uint32_t clk_sel, uint32_t data_sel)
{
	if(clk_sel == 0)
	{
		if((PS2_1_EXTERNAL_PULL_UP == 0) && (IS_GPIOB12(LOW) || PS2_1_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 12);
	}
	else if(clk_sel == 1)
	{
		if((PS2_1_EXTERNAL_PULL_UP == 0) && (IS_GPIOB10(LOW) || PS2_1_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 10);
	}
	if(data_sel == 0)
	{
		if((PS2_1_EXTERNAL_PULL_UP == 0) && (IS_GPIOB13(LOW) || PS2_1_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 13);
	}
	else if(data_sel == 1)
	{
		if((PS2_1_EXTERNAL_PULL_UP == 0) && (IS_GPIOB13(LOW) || PS2_1_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 11);
	}


}
//*****************************************************************************
//
//  To setup cec pull up
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void cec0_pull_up(uint8_t cec0_sel)
{
	if(cec0_sel == 0)
	{
		if((CEC0_EXTERNAL_PULL_UP == 0) && (IS_GPIOA13(LOW) || CEC0_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOA, 13);
	}
	else
	{
		if((CEC0_EXTERNAL_PULL_UP == 0) && (IS_GPIOB8(LOW) || CEC0_INTERNAL_PULL_UP))
			PIO_Pullup_Config(GPIOB, 8);
	}
}
void cec1_pull_up(void)
{
	if((CEC1_EXTERNAL_PULL_UP == 0) && (IS_GPIOB31(LOW) || CEC1_INTERNAL_PULL_UP))
		PIO_Pullup_Config(GPIOB, 31);
}
//*****************************************************************************
//
//  To setup kbs pull up
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void kbs_pull_up(void)
{
	for(register unsigned char i = 8; i <= 31; i++)
	{
		//考虑KBS扫描逻辑，不做低电平判断
		if((((KBS_EXTERNAL_PULL_UP) & (1 << (i - 8))) == 0) && ((((KBS_INTERNAL_PULL_UP) & (1 << (i - 8))) == (1 << (i - 8)))))
			PIO_Pullup_Config(GPIOE, i); // gpe all pull up
	}
#if (KBD_8_n_SWITCH == 17 || KBD_8_n_SWITCH == 18)
	//考虑KBS扫描逻辑，不做低电平判断
	if((((KBS_EXTERNAL_PULL_UP) & (1 << (24))) == 0) && ((((KBS_INTERNAL_PULL_UP) & (1 << (24))) == (1 << (24)))))
		PIO_Pullup_Config(GPIOA, 19); // C3 kso 16
#endif
#if (KBD_8_n_SWITCH == 18)
	//考虑KBS扫描逻辑，不做低电平判断
	if((((KBS_EXTERNAL_PULL_UP) & (1 << (25))) == 0) && ((((KBS_INTERNAL_PULL_UP) & (1 << (25))) == (1 << (25)))))
		PIO_Pullup_Config(GPIOA, 21); // C3 kso 17
#endif
	SYSCTL_PIO3_UDCFG |= 0x00ff0000;//kbs in maintain pull up 
}
/**
 * @brief GPIO配置函数，用于配置GPIO的工作模式及中断相关参数
 *
 * @param GPIO   GPIO组编号，参数可选：GPIOA GPIOB...
 * @param gpio_no   GPIO具体引脚号码，例如：GPIOA[5]则该参数为5
 * @param mode   GPIO的工作模式，0为输入模式，1为输出模式，2为中断模式
 * @param op_val   在输出模式下，该参数用于设置输出的电平，0为低电平，1为高电平，在其他模式下该参数无效
 * @param int_lv   在中断模式下，该参数用于设置中断触发电平或边沿，0为电平触发，1为边沿触发
 * @param pol   在中断模式下，该参数用于设置中断触发极性，1为上升沿（电平）触发  0为下降沿（低电平触发）
 *
 * @return 成功返回0，失败返回-1
 */
int GPIO_Config(int GPIO, int gpio_no, int mode, int op_val, int int_lv, int pol)
{
#if (!GPIO_CLOCK_EN)
	return 0;
#endif
#if (!GPIODB_CLOCK_EN)
	return 0;
#endif
	sysctl_iomux_config(GPIO, gpio_no, 0);
	if(GPIO == 1)
	{
		if(mode == 1) // output
		{
			if(op_val > 0)
			{
				GPIOA_REG((GPIO_INPUT_VAL + (gpio_no / 8))) |= (VBYTE)((0x1) << (gpio_no % 8)); // 配置输出值
				GPIOA_REG((GPIO_INPUT_EN + (gpio_no / 8))) |= (VBYTE)((0x1) << (gpio_no % 8));	 // 配置输出模式
				GPIOA_REG((GPIO_LOW_IE + (gpio_no / 8))) &= (VBYTE)(~((0x1) << (gpio_no % 8)));	 // 配置GPIO模式(非中断)
				//dprint("set GPIO%d,%d over val %d\n", GPIO, gpio_no, op_val);
			}
			else
			{
				GPIOA_REG((GPIO_INPUT_VAL + (gpio_no / 8))) &= (VBYTE)(~((0x1) << (gpio_no % 8))); // 配置输出值
				GPIOA_REG((GPIO_INPUT_EN + (gpio_no / 8))) |= (VBYTE)((0x1) << (gpio_no % 8));		// 配置输出模式
				GPIOA_REG((GPIO_LOW_IE + (gpio_no / 8))) &= (VBYTE)(~((0x1) << (gpio_no % 8)));		// 配置GPIO模式(非中断)
				//dprint("set GPIO%d,%d over val %d\n", GPIO, gpio_no, op_val);
			}
		}
		else if(mode == 0) // input
		{
			//(*((BYTEP )(SYSCTL_PIO0_UDCFG)))|=((0x1)<<gpio_no);//配置上拉
			GPIOA_REG(GPIO_INPUT_EN + (gpio_no / 8)) &= (VBYTE)(~((0x1) << (gpio_no % 8))); // 配置输入模式
			GPIOA_REG(GPIO_LOW_IE + (gpio_no / 8)) &= (VBYTE)(~((0x1) << (gpio_no % 8)));   // 配置GPIO模式(非中断)
		}
		else // interrupt
		{
			GPIOA_REG((GPIO_INPUT_EN + (gpio_no / 8))) &= (VBYTE)(~((0x1) << (gpio_no % 8)));  // 设置为输入模式
			GPIOA_REG((GPIO_LOW_IP + (gpio_no / 8))) &= (VBYTE)(~((0x1) << (gpio_no % 8)));	  // 禁止中断屏蔽
			GPIOA_REG((GPIO_IOF_EN + (gpio_no / 8))) = (VBYTE)((int_lv) << (gpio_no % 8));	  // 设置触发模式
			GPIOA_REG((GPIO_IOF_SEL + (gpio_no / 8))) = (VBYTE)((pol) << (gpio_no % 8));	  // 设置触发极性
			GPIOA_REG((GPIO_DEBOUNCE_EN + (gpio_no / 8))) = (VBYTE)((0x1) << (gpio_no % 8)); // 设置消抖
			GPIOA_REG((GPIO_LOW_IE + (gpio_no / 8))) |= (VBYTE)((0x1) << (gpio_no % 8));	  // 使能中断
			//dprint("触发方式:%#x\n", GPIOA_REG((GPIO_IOF_EN + (gpio_no / 8))));
			//dprint("触发极性:%#x\n", GPIOA_REG((GPIO_IOF_SEL + (gpio_no / 8))));
		}
	}
	else if(GPIO == 2)
	{
		if(mode == 1) // output
		{
			if(op_val > 0)
			{
				GPIOB_REG((GPIO_INPUT_VAL + (gpio_no / 8))) |= ((0x1) << (gpio_no % 8)); // 配置输出值
				GPIOB_REG((GPIO_INPUT_EN + (gpio_no / 8))) |= ((0x1) << (gpio_no % 8));	 // 配置输出模式
				GPIOB_REG((GPIO_LOW_IE + (gpio_no / 8))) &= (~((0x1) << (gpio_no % 8)));	 // 配置GPIO模式(非中断)
			}
			else
			{
				GPIOB_REG((GPIO_INPUT_VAL + (gpio_no / 8))) &= (~((0x1) << (gpio_no % 8))); // 配置输出值
				GPIOB_REG((GPIO_INPUT_EN + (gpio_no / 8))) |= ((0x1) << (gpio_no % 8));		// 配置输出模式
				GPIOB_REG((GPIO_LOW_IE + (gpio_no / 8))) &= (~((0x1) << (gpio_no % 8)));		// 配置GPIO模式(非中断)
			}
		}
		else if(mode == 0) // input
		{
			//(*((BYTEP )(SYSCTL_PIO0_UDCFG)))|=((0x1)<<gpio_no);//配置上拉
			GPIOB_REG(GPIO_INPUT_EN + (gpio_no / 8)) &= (~((0x1) << (gpio_no % 8))); // 配置输入模式
			GPIOB_REG(GPIO_LOW_IE + (gpio_no / 8)) &= (~((0x1) << (gpio_no % 8)));   // 配置GPIO模式(非中断)
		}
		else // interrupt
		{
			GPIOB_REG((GPIO_INPUT_EN + (gpio_no / 8))) &= (~((0x1) << (gpio_no % 8)));  // 设置为输入模式
			GPIOB_REG((GPIO_LOW_IP + (gpio_no / 8))) &= (~((0x1) << (gpio_no % 8)));	  // 禁止中断屏蔽
			GPIOB_REG((GPIO_IOF_EN + (gpio_no / 8))) = ((int_lv) << (gpio_no % 8));	  // 设置触发模式
			GPIOB_REG((GPIO_IOF_SEL + (gpio_no / 8))) = ((pol) << (gpio_no % 8));	  // 设置触发极性
			GPIOB_REG((GPIO_DEBOUNCE_EN + (gpio_no / 8))) = ((0x1) << (gpio_no % 8)); // 设置消抖
			GPIOB_REG((GPIO_LOW_IE + (gpio_no / 8))) |= ((0x1) << (gpio_no % 8));	  // 使能中断
			//dprint("触发方式:%#x\n", GPIOB_REG((GPIO_IOF_EN + (gpio_no / 8))));
			//dprint("触发极性:%#x\n", GPIOB_REG((GPIO_IOF_SEL + (gpio_no / 8))));
		}
	}
	else
	{
		if(GPIO == 4)
		{
			if(mode == 1) // output
			{
				if(op_val > 0)
				{
					if(gpio_no == 8)//piod8
					{
						GPIOC_REG((0xd)) |= 0x1;
						GPIOC_REG((0x11)) |= 0x1;
					}
					else
					{
						GPIOC_REG((0xc)) |= ((0x1) << gpio_no);	 // 配置输出值
						GPIOC_REG((0x10)) |= ((0x1) << gpio_no); // 配置输出模式
					}
				}
				else
				{
					if(gpio_no == 8)
					{
						GPIOC_REG((0xd)) &= ~(0x1); // 配置输出值
						GPIOC_REG((0x11)) |= 0x1;   // 配置输出模式						
					}
					else
					{
						GPIOC_REG((0xc)) &= (~((0x1) << gpio_no)); // 配置输出值
						GPIOC_REG((0x10)) |= ((0x1) << gpio_no);   // 配置输出模式
					}

				}
			}
			else if(mode == 0) // input
			{
				if(gpio_no == 8)
				{
					GPIOC_REG(0x11) &= (~(0x1)); // 配置输入模式	
				}
				else
				{
					//(*((BYTEP )(SYSCTL_PIO0_UDCFG)))|=((0x1)<<gpio_no);//配置上拉
					GPIOC_REG(0x10) &= (~((0x1) << gpio_no)); // 配置输入模式					
				}

			}
		}
		else if((GPIO == 3))
		{
			if(mode == 1) // output
			{
				if(op_val > 0)
				{
					GPIOC_REG((GPIO_INPUT_VAL + (gpio_no / 8))) |= ((0x1) << (gpio_no % 8)); // 配置输出值
					GPIOC_REG((GPIO_INPUT_EN + (gpio_no / 8))) |= ((0x1) << (gpio_no % 8));	 // 配置输出模式
					GPIOC_REG((GPIO_LOW_IE + (gpio_no / 8))) &= (~((0x1) << (gpio_no % 8)));	 // 配置GPIO模式(非中断)
				}
				else
				{
					GPIOC_REG((GPIO_INPUT_VAL + (gpio_no / 8))) &= (~((0x1) << (gpio_no % 8))); // 配置输出值
					GPIOC_REG((GPIO_INPUT_EN + (gpio_no / 8))) |= ((0x1) << (gpio_no % 8));		// 配置输出模式
					GPIOC_REG((GPIO_LOW_IE + (gpio_no / 8))) &= (~((0x1) << (gpio_no % 8)));		// 配置GPIO模式(非中断)
				}
			}
			else if(mode == 0) // input
			{
				//(*((BYTEP )(SYSCTL_PIO0_UDCFG)))|=((0x1)<<gpio_no);//配置上拉
				GPIOC_REG(GPIO_INPUT_EN + (gpio_no / 8)) &= (~((0x1) << (gpio_no % 8))); // 配置输入模式
				GPIOC_REG(GPIO_LOW_IE + (gpio_no / 8)) &= (~((0x1) << (gpio_no % 8)));   // 配置GPIO模式(非中断)
			}
			else // interrupt
			{
				GPIOC_REG((GPIO_INPUT_EN + (gpio_no / 8))) &= (~((0x1) << (gpio_no % 8)));  // 设置为输入模式
				GPIOC_REG((GPIO_LOW_IP + (gpio_no / 8))) &= (~((0x1) << (gpio_no % 8)));	  // 禁止中断屏蔽
				GPIOC_REG((GPIO_IOF_EN + (gpio_no / 8))) = ((int_lv) << (gpio_no % 8));	  // 设置触发模式
				GPIOC_REG((GPIO_IOF_SEL + (gpio_no / 8))) = ((pol) << (gpio_no % 8));	  // 设置触发极性
				GPIOC_REG((GPIO_DEBOUNCE_EN + (gpio_no / 8))) = ((0x1) << (gpio_no % 8)); // 设置消抖
				GPIOC_REG((GPIO_LOW_IE + (gpio_no / 8))) |= ((0x1) << (gpio_no % 8));	  // 使能中断
				//dprint("触发方式:%#x\n", GPIOC_REG((GPIO_IOF_EN + (gpio_no / 8))));
				//dprint("触发极性:%#x\n", GPIOC_REG((GPIO_IOF_SEL + (gpio_no / 8))));
			}
		}
		else if((GPIO == 5))
		{
			if(mode == 1) // output
			{
				if(op_val > 0)
				{
					GPIOE_REG((GPIO_INPUT_VAL + (gpio_no / 8))) |= ((0x1) << (gpio_no % 8)); // 配置输出值
					GPIOE_REG((GPIO_INPUT_EN + (gpio_no / 8))) |= ((0x1) << (gpio_no % 8));	 // 配置输出模式
					GPIOE_REG((GPIO_LOW_IE + (gpio_no / 8))) &= (~((0x1) << (gpio_no % 8)));	 // 配置GPIO模式(非中断)
				}
				else
				{
					GPIOE_REG((GPIO_INPUT_VAL + (gpio_no / 8))) &= (~((0x1) << (gpio_no % 8))); // 配置输出值
					GPIOE_REG((GPIO_INPUT_EN + (gpio_no / 8))) |= ((0x1) << (gpio_no % 8));		// 配置输出模式
					GPIOE_REG((GPIO_LOW_IE + (gpio_no / 8))) &= (~((0x1) << (gpio_no % 8)));		// 配置GPIO模式(非中断)
				}
			}
			else if(mode == 0) // input
			{
				//(*((BYTEP )(SYSCTL_PIO0_UDCFG)))|=((0x1)<<gpio_no);//配置上拉
				GPIOE_REG(GPIO_INPUT_EN + (gpio_no / 8)) &= (~((0x1) << (gpio_no % 8))); // 配置输入模式
				GPIOE_REG(GPIO_LOW_IE + (gpio_no / 8)) &= (~((0x1) << (gpio_no % 8)));   // 配置GPIO模式(非中断)
			}
			else // interrupt
			{
				GPIOE_REG((GPIO_INPUT_EN + (gpio_no / 8))) &= (~((0x1) << (gpio_no % 8)));  // 设置为输入模式
				GPIOE_REG((GPIO_LOW_IP + (gpio_no / 8))) &= (~((0x1) << (gpio_no % 8)));	  // 禁止中断屏蔽
				GPIOE_REG((GPIO_IOF_EN + (gpio_no / 8))) = ((int_lv) << (gpio_no % 8));	  // 设置触发模式
				GPIOE_REG((GPIO_IOF_SEL + (gpio_no / 8))) = ((pol) << (gpio_no % 8));	  // 设置触发极性
				GPIOE_REG((GPIO_DEBOUNCE_EN + (gpio_no / 8))) = ((0x1) << (gpio_no % 8)); // 设置消抖
				GPIOE_REG((GPIO_LOW_IE + (gpio_no / 8))) |= ((0x1) << (gpio_no % 8));	  // 使能中断
				//dprint("触发方式:%#x\n", GPIOE_REG((GPIO_IOF_EN + (gpio_no / 8))));
				//dprint("触发极性:%#x\n", GPIOE_REG((GPIO_IOF_SEL + (gpio_no / 8))));
			}
		}
	}
	return 0;
}
/**
 * @brief GPIO引脚输入使能寄存器，当引脚不作为输入时不使能该寄存器能进一步降低功耗
 *
 * @param GPIO   	GPIO组编号，参数可选：GPIOA GPIOB...
 * @param gpio_no   GPIO具体引脚号码，例如：GPIOA[5]则该参数为5
 * @param sw   		可选参数：ENABLE 使能  DISABLE 失能
 *
 * @return none
 */
void GPIO_Input_EN(int GPIO, int gpio_no, char sw)
{
	switch(GPIO)
	{
		case GPIOA:
			if(sw == ENABLE)
			{
				SYSCTL_PIO0_IECFG |= (0x1 << gpio_no);

			}
			else
			{
				SYSCTL_PIO0_IECFG &= ~(0x1 << gpio_no);
			}
			break;
		case GPIOB:
			if(sw == ENABLE)
			{
				SYSCTL_PIO1_IECFG |= (0x1 << gpio_no);
			}
			else
			{
				SYSCTL_PIO1_IECFG &= ~(0x1 << gpio_no);
			}
			break;
		case GPIOC:
		case GPIOD:
			if(sw == ENABLE)
			{
				SYSCTL_PIO2_IECFG |= (0x1 << gpio_no);
			}
			else
			{
				SYSCTL_PIO2_IECFG &= ~(0x1 << gpio_no);
			}
			break;
		case GPIOE:
			if(sw == ENABLE)
			{
				SYSCTL_PIO3_IECFG |= (0x1 << gpio_no);
			}
			else
			{
				SYSCTL_PIO3_IECFG &= ~(0x1 << gpio_no);
			}
			break;
		default:
			break;
	}

}
/**
 * @brief 1.8V IO配置
 *
 * @param GPIO   	GPIO组编号，参数可选：GPIOA GPIOB...
 * @param gpio_no   GPIO具体引脚号码，例如：GPIOA[5]则该参数为5
 * @param sw   		可选参数：ENABLE 使能  DISABLE 失能
 *
 * @return none
 */

void GPIO_1V8(int GPIO, int gpio_no, char sw)
{
	u8 offset = 31;
	if(GPIO == GPIOA)
	{
		switch(gpio_no)
		{
			case 14:
				offset = 0;
				break;
			case 28:
				offset = 1;
				break;
				// case 23:
				// 	offset = 2;
				// 	break;
			case 24:
				offset = 4;
				break;
			case 25:
				offset = 7;
				break;
			case 26:
				offset = 8;
				break;
			case 27:
				offset = 9;
				break;
			case 4:
				offset = 10;
				break;
			case 5:
				offset = 11;
				break;
			case 17:
				offset = 19;
				break;
			case 18:
				offset = 20;
				break;
			case 16:
				offset = 23;
				break;
			case 13:
				offset = 25;
				break;

			default:
				break;
		}
	}
	else if(GPIO == GPIOB)
	{
		switch(gpio_no)
		{
			case 6:
				offset = 3;
				break;
				// case 0:
				// 	offset = 5;
				// 	break;
				// case 7:
				// 	offset = 6;
				// 	break;
			case 10:
				offset = 12;
				break;
			case 11:
				offset = 13;
				break;
			case 12:
				offset = 14;
				break;
			case 13:
				offset = 15;
				break;
			case 20:
				offset = 16;
				break;
			case 21:
				offset = 17;
				break;
			case 22:
				offset = 18;
				break;
			case 14:
				offset = 21;
				break;
			case 15:
				offset = 22;
				break;

			default:
				break;
		}
	}
	else if((GPIO == GPIOD) && (gpio_no == 8))
	{
		offset = 24;
	}
	if(sw)
		SYSCTL_PAD_1P8 |= BIT(offset);
	else
		SYSCTL_PAD_1P8 &= ~BIT(offset);

}

char GPIOAutoTest(void)//102
{
	BYTE gpa0, gpa1, gpa2, gpa3;
	BYTE gpb0, gpb1, gpb2, gpb3;
	BYTE gpc0, gpc1, gpc2, gpc3;
	BYTE gpd0, gpd1, gpd2;

	BYTE gpa0z, gpa1z, gpa2z, gpa3z;
	BYTE gpb0z, gpb1z, gpb2z, gpb3z;
	BYTE gpc0z, gpc1z, gpc2z;
	BYTE gpd0z, gpd1z, gpd2z;

	static char flag = 1;
	if(flag)
	{
		flag = 0;
	}
	else
	{
		flag = 1;
	}
	for(int i = 0; i < 32; i++)
	{

		if(i != 24 && i != 25)//uart0
		{
			GPIO_Config(GPIOA, i, 1, flag, 0, 0);
		}
	}
	for(int i = 0; i < 32; i++)
	{
		if(i != 16 && i != 19 && i != 20 && i != 21 && i != 22 && i != 23)
		{
			GPIO_Config(GPIOB, i, 1, flag, 0, 0);
		}
	}
	for(int i = 0; i < 16; i++)
	{
		if(i != 10)
		{
			GPIO_Config(GPIOC, i, 1, flag, 0, 0);
		}
	}
	for(int i = 0; i < 9; i++)
	{
		GPIO_Config(GPIOD, i, 1, flag, 0, 0);
	}
	for(int i = 0; i < 24; i++)
	{
		GPIO_Config(GPIOE, i, 1, flag, 0, 0);
	}
	gpa0 = GPIO0_EXT0;
	gpa1 = GPIO0_EXT1;
	gpa2 = GPIO0_EXT2;
	gpa3 = GPIO0_EXT3;

	gpb0 = GPIO1_EXT0;
	gpb1 = GPIO1_EXT1;
	gpb2 = GPIO1_EXT2;
	gpb3 = GPIO1_EXT3;

	gpc0 = GPIO2_EXT0;//C:0-7
	gpc1 = GPIO2_EXT1;//C:8-15
	gpc2 = GPIO2_EXT2;//D:0-7
	gpc3 = GPIO2_EXT3;//D:8-15 

	gpd0 = GPIO3_EXT0;
	gpd1 = GPIO3_EXT1;
	gpd2 = GPIO3_EXT2;
	if(flag)
	{
		gpa3 |= 0x3;//uart0
		gpb2 |= 0xf9;//pb16 pb19用作电源 pb20/21/22/23作为两线flash引脚，当代码运行在外部flash不需要检测,需要设置iecfg，否则可能失败
		gpc1 |= 0x4;//pc10用作电源

		printf("\n");
		dprint("GPIO0: 0x%x 0x%x 0x%x 0x%x\n", gpa0, gpa1, gpa2, gpa3);
		dprint("GPIO1: 0x%x 0x%x 0x%x 0x%x\n", gpb0, gpb1, gpb2, gpb3);
		dprint("GPIO2: 0x%x 0x%x 0x%x 0x%x\n", gpc0, gpc1, gpc2, gpc3);
		dprint("GPIO3: 0x%x 0x%x 0x%x\n", gpd0, gpd1, gpd2);
		gpa0z = ~gpa0;
		gpa1z = ~gpa1;
		gpa2z = ~gpa2;
		gpa3z = ~gpa3;
		gpb0z = ~gpb0;
		gpb1z = ~gpb1;
		gpb2z = ~gpb2;
		gpb3z = ~gpb3;
		gpc0z = ~gpc0;
		gpc1z = ~gpc1;
		gpc2z = ~gpc2;
		gpd0z = ~gpd0;
		gpd1z = ~gpd1;
		gpd2z = ~gpd2;
		//如果全部加起来等于0
		if((gpa0z + gpa1z + gpa2z + gpa3z + gpb0z + gpb1z + gpb2z + gpb3z + gpc0z + gpc1z + gpc2z + gpd0z + gpd1z + gpd2z))
		{
			printf("Self-test failed\n");
			return -1;
		}
		else
		{
			printf("Self-test successful\n");
			return 1;
		}
	}
	else
	{
		gpa3 &= (~0x3);//uart0
		gpb2 &= (~0xf9);//pb16 pb19用作电源 pb20/21/22/23作为两线flash引脚，当代码运行在外部flash不需要检测,需要设置iecfg，否则可能失败
		gpc1 &= (~0x4);//pc10用作电源

		printf("\n");
		dprint("GPIO0: 0x%x 0x%x 0x%x 0x%x\n", gpa0, gpa1, gpa2, gpa3);
		dprint("GPIO1: 0x%x 0x%x 0x%x 0x%x\n", gpb0, gpb1, gpb2, gpb3);
		dprint("GPIO2: 0x%x 0x%x 0x%x 0x%x\n", gpc0, gpc1, gpc2, gpc3);
		dprint("GPIO3: 0x%x 0x%x 0x%x\n", gpd0, gpd1, gpd2);
		//gpa0 gpa1...全部加起来不等于0
		if((gpa0 + gpa1 + gpa2 + gpa3 + gpb0 + gpb1 + gpb2 + gpb3 + gpc0 + gpc1 + gpc2 + gpd0 + gpd1 + gpd2))
		{
			printf("Self-test failed\n");
			return -1;
		}
		else
		{
			printf("Self-test successful\n");
			return 1;
		}
	}
}

/*
判断传入的pin脚是否是输出状态，是否复用为GPIO模式，如果确定为输出状态，并且复用为GPIO模式，则返回1，反之则是0
*/
char IsGpioOut(DWORD port, DWORD io)
{
	DWORD addr;
	DWORD cfg_val;
	if(port <= 3)
	{
		addr = SYSCTL_BASE_ADDR + SYSCTL_PIO0_CFG_OFFSET + ((port - 1) * 0x8);
		if(io <= 15)
		{
			cfg_val = *(DWORDP)addr;
			cfg_val &= ~(3 << (io * 2));
			if(cfg_val == *(DWORDP)addr)
			{
				//assert_print("port: %d,io: %d\n", port, io);
				if(port == 1)
				{
					cfg_val = GPIOA_REG((GPIO_INPUT_EN + (io / 8)));
					cfg_val |= (VBYTE)((0x1) << (io % 8));
					if(cfg_val == GPIOA_REG((GPIO_INPUT_EN + (io / 8))))
					{
						//assert_print("is output\n");
						return 1;
					}
					else
					{
						//assert_print("not is output\n");
						return 0;
					}
				}
				else if(port == 2)
				{
					cfg_val = GPIOB_REG((GPIO_INPUT_EN + (io / 8)));
					cfg_val |= (VBYTE)((0x1) << (io % 8));
					if(cfg_val == GPIOB_REG((GPIO_INPUT_EN + (io / 8))))
					{
						//assert_print("is output\n");
						return 1;
					}
					else
					{
						//assert_print("not is output\n");
						return 0;
					}
				}
				else
				{
					cfg_val = GPIOC_REG((GPIO_INPUT_EN + (io / 8)));
					cfg_val |= (VBYTE)((0x1) << (io % 8));
					if(cfg_val == GPIOC_REG((GPIO_INPUT_EN + (io / 8))))
					{
						//assert_print("is output\n");
						return 1;
					}
					else
					{
						//assert_print("not is output\n");
						return 0;
					}
				}
			}
			else
			{
				//assert_print("pin is not GPIO\n");
				return 0;
			}
		}
		else if(io <= 31)
		{
			cfg_val = *(DWORDP)(addr + 0x4);
			cfg_val &= ~(3 << (io * 2));
			if(cfg_val == *(DWORDP)(addr + 0x4))
			{
				//assert_print("port: %d,io: %d\n", port, io);
				if(port == 1)
				{
					cfg_val = GPIOA_REG((GPIO_INPUT_EN + (io / 8)));
					cfg_val |= (VBYTE)((0x1) << (io % 8));
					if(cfg_val == GPIOA_REG((GPIO_INPUT_EN + (io / 8))))
					{
						//assert_print("is output\n");
						return 1;
					}
					else
					{
						//assert_print("not is output\n");
						return 0;
					}
				}
				else if(port == 2)
				{
					cfg_val = GPIOB_REG((GPIO_INPUT_EN + (io / 8)));
					cfg_val |= (VBYTE)((0x1) << (io % 8));
					if(cfg_val == GPIOB_REG((GPIO_INPUT_EN + (io / 8))))
					{
						//assert_print("is output\n");
						return 1;
					}
					else
					{
						//assert_print("not is output\n");
						return 0;
					}
				}
				else
				{
					cfg_val = GPIOC_REG((GPIO_INPUT_EN + (io / 8)));
					cfg_val |= (VBYTE)((0x1) << (io % 8));
					if(cfg_val == GPIOC_REG((GPIO_INPUT_EN + (io / 8))))
					{
						//assert_print("is output\n");
						return 1;
					}
					else
					{
						//assert_print("not is output\n");
						return 0;
					}
				}
			}
			else
			{
				//assert_print("pin is not GPIO\n");
				return 0;
			}
		}
		else
		{
			//dprint("GPIO  number input error\n");
			return 0;
		}
	}
	else
	{
		addr = (SYSCTL_BASE_ADDR + SYSCTL_PIO5_CFG_OFFSET);
		if(port == 4)
		{
			cfg_val = *(DWORDP)addr;
			cfg_val &= ~(3 << (io * 2));
			if(cfg_val == *(DWORDP)addr)
			{
				//assert_print("port: %d,io: %d\n", port, io);
				cfg_val = GPIOD_REG((0x10));
				cfg_val |= ((0x1) << (io % 8));
				if(cfg_val == GPIOD_REG((0x10)))
				{
					//assert_print("is output\n");
					return 1;
				}
				else
				{
					//assert_print("not is output\n");
					return 0;
				}
			}
			else
			{
				//assert_print("pin is not GPIO\n");
				return 0;
			}
		}
		else if(port == 5)
		{
			cfg_val = *(DWORDP)addr;
			cfg_val &= ~(3 << 18);
			if(cfg_val == *(DWORDP)addr)
			{
				//dprint("port: %d,io: %d\n", port, io);
				cfg_val = GPIOE_REG((GPIO_INPUT_EN + (io / 8)));
				cfg_val |= ((0x1) << (io % 8));
				if(cfg_val == GPIOE_REG((GPIO_INPUT_EN + (io / 8))))
				{
					return 1; //dprint("is output\n");
				}
				else
				{
					return 0;  //dprint("is not output\n");
				}
			}
			else
			{
				return 0; //dprint("pin is not GPIO\n");
			}
		}
		else
		{
			dprint("GPIO group number input error\n");
			return 0;
		}
	}

}
/**
 * @brief       读取GPIO引脚电平值
 *
 * @param       port  引脚所在的GPIO组
 * @param       pin   引脚号
 *
 * @return      引脚电平值（0或1）
 */
char ReadGPIOLevel(BYTE port, BYTE pin)
{
	volatile unsigned char *portBase;
	unsigned char pinValue = 0;
	if(port < 3)
	{
		portBase = (volatile unsigned char *)(GPIO0_BASE_ADDR + 0x50 + (port - 1) * 0x400 + (pin / 8));
		// 读取引脚的电平值
		pinValue = *portBase & (0x1 << (pin % 8));
		// 返回引脚电平值（0或1）
		return (pinValue != 0) ? 1 : 0;
	}
	else if(port == 3)
	{
		portBase = (volatile unsigned char *)(GPIO0_BASE_ADDR + 0x50 + (port - 1) * 0x400 + (pin / 8));
		// 读取引脚的电平值
		pinValue = *portBase & (0x1 << (pin % 8));
		// 返回引脚电平值（0或1）
		return (pinValue != 0) ? 1 : 0;
	}
	else if(port == 4)
	{
		portBase = (volatile unsigned char *)(GPIO2_BASE_ADDR + 0x54);
		// 读取引脚的电平值
		pinValue = *portBase & (0x1 << (pin % 8));
		// 返回引脚电平值（0或1）
		return (pinValue != 0) ? 1 : 0;

	}
	else if(port == 5)
	{
		portBase = (volatile unsigned char *)(GPIO3_BASE_ADDR + 0x50 + (pin / 8));
		// 读取引脚的电平值
		pinValue = *portBase & (0x1 << (pin % 8));
		// 返回引脚电平值（0或1）
		return (pinValue != 0) ? 1 : 0;
	}
	else
	{
		printf("GPIO group number input error\n");
		return -1;
	}
}
