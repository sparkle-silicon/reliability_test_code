/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-07-22 14:45:37
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
 * @param    port     		AE102可选参数：GPIOA ,GPIOB...GPIOE
 *
 * @param    io      		GPIO序列号选择如：GPIOA10
 *
 * @param    port_type      复用功能选择：可选参数0-3
 *
 * @return   无
 */
void sysctl_iomux_config(DWORD port, DWORD io, unsigned port_type)
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
			cfg_val |= port_type << (io * 2);
			*(DWORDP)addr = cfg_val;
		}
		else if(io <= 31)
		{
			cfg_val = *(DWORDP)(addr + 0x4);
			io = (io - 16);
			cfg_val &= ~(3 << (io * 2));
			cfg_val |= port_type << (io * 2);
			*(DWORDP)(addr + 0x4) = cfg_val;
		}
		else
		{
			printf("GPIO  number input error\n");
		}
	}
	else
	{
		addr = (SYSCTL_BASE_ADDR + SYSCTL_PIO5_CFG_OFFSET);
		if(port == 4)
		{
			cfg_val = *(DWORDP)addr;
			cfg_val &= ~(3 << (io * 2));
			cfg_val |= port_type << (io * 2);
			*(DWORDP)addr = cfg_val;
		}
		else if(port == 5)
		{
			cfg_val = *(DWORDP)addr;
		#if (defined AE102 || defined AE101)
			cfg_val &= ~(3 << 14);
			cfg_val |= port_type << 14;
		#endif
		#if (defined AE103)
			cfg_val &= ~(3 << 18);
			cfg_val |= port_type << 18;
		#endif
			*(DWORDP)addr = cfg_val;
		}
		else
		{
			printf("GPIO group number input error\n");
		}
	}
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
void sysctl_iomux_uart1()
{
#if (defined(AE102) || defined(AE103))
	sysctl_iomux_config(GPIOB, 1, 1);
	sysctl_iomux_config(GPIOB, 3, 1);
#endif
}
void sysctl_iomux_disable_uart1()
{
#if (defined(AE102) || defined(AE103))
	sysctl_iomux_config(GPIOB, 1, 0);
	sysctl_iomux_config(GPIOB, 3, 0);
#endif
}
//*****************************************************************************
//
//  To setup uart2 iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_uart2()
{
#if (defined(AE101) || defined(AE102))
	sysctl_iomux_config(GPIOB, 23, 1);
	sysctl_iomux_config(GPIOB, 25, 1);
#endif
#ifdef AE103
	sysctl_iomux_config(GPIOB, 27, 1);    //txd
	sysctl_iomux_config(GPIOB, 29, 1);	   //rxd
#endif
}
void sysctl_iomux_disable_uart2()
{
#if (defined(AE101) || defined(AE102))
	sysctl_iomux_config(GPIOB, 23, 0);
	sysctl_iomux_config(GPIOB, 25, 0);
#endif
#ifdef AE103
	sysctl_iomux_config(GPIOB, 27, 0);    //txd
	sysctl_iomux_config(GPIOB, 29, 0);	   //rxd
#endif
}
//*****************************************************************************
//
//  To setup uart3 iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_uart3()
{
#if (defined(AE101) || defined(AE102))
	sysctl_iomux_config(GPIOA, 15, 2);
	sysctl_iomux_config(GPIOC, 11, 1);
#endif
#ifdef AE103
	sysctl_iomux_config(GPIOA, 10, 2);			//txd
	sysctl_iomux_config(GPIOC, 11, 1);			//rxd
#endif
}
void sysctl_iomux_disable_uart3()
{
#if (defined(AE101) || defined(AE102))
	sysctl_iomux_config(GPIOA, 15, 0);
	sysctl_iomux_config(GPIOC, 11, 0);
#endif
#ifdef AE103
	sysctl_iomux_config(GPIOA, 10, 0);			//txd
	sysctl_iomux_config(GPIOC, 11, 0);			//rxd
#endif
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
void sysctl_iomux_uarta()
{
	sysctl_iomux_config(GPIOA, 8, 2);
	sysctl_iomux_config(GPIOA, 9, 2);
}
void sysctl_iomux_disable_uarta()
{
	sysctl_iomux_config(GPIOA, 8, 0);
	sysctl_iomux_config(GPIOA, 9, 0);
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
#if (defined(AE101) || defined(AE102))
	sysctl_iomux_config(GPIOB, 21, 2);
	sysctl_iomux_config(GPIOB, 22, 2);
#endif
#ifdef AE103
	sysctl_iomux_config(GPIOB, 25, 2);
	sysctl_iomux_config(GPIOB, 26, 2);
#endif
}
void sysctl_iomux_disable_uartb()
{
#if (defined(AE101) || defined(AE102))
	sysctl_iomux_config(GPIOB, 21, 0);
	sysctl_iomux_config(GPIOB, 22, 0);
#endif
#ifdef AE103
	sysctl_iomux_config(GPIOB, 25, 0);
	sysctl_iomux_config(GPIOB, 26, 0);
#endif
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
void sysctl_iomux_ps2_0()
{
	sysctl_iomux_config(GPIOB, 8, 1);
	sysctl_iomux_config(GPIOB, 9, 1);
}
void sysctl_iomux_ps2_1()
{
	sysctl_iomux_config(GPIOB, 12, 1);
	sysctl_iomux_config(GPIOB, 13, 1);
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
}
void sysctl_iomux_spim_cs()
{
	sysctl_iomux_config(GPIOB, 18, 2); // csn0
	sysctl_iomux_config(GPIOB, 16, 2); // csn1
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
void sysctl_iomux_tach0()
{
	sysctl_iomux_config(GPIOA, 30, 1); // tach0
}
void sysctl_iomux_tach1()
{
	sysctl_iomux_config(GPIOA, 31, 1); // tach1
}
void sysctl_iomux_tach2()
{
	sysctl_iomux_config(GPIOC, 10, 2); // tach2
}
void sysctl_iomux_tach3()
{
	sysctl_iomux_config(GPIOC, 11, 2); // tach3
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
void sysctl_iomux_i2c0()
{
	sysctl_iomux_config(GPIOA, 11, 1);
	sysctl_iomux_config(GPIOA, 12, 1);
}
void sysctl_iomux_i2c1()
{
	sysctl_iomux_config(GPIOA, 17, 1);
	sysctl_iomux_config(GPIOA, 18, 1);
}
void sysctl_iomux_i2c2()
{
	sysctl_iomux_config(GPIOB, 14, 1);
	sysctl_iomux_config(GPIOB, 15, 1);
}
void sysctl_iomux_i2c3()
{
	SMBUS3_UARTB_SEL;// select smbus3
#if (defined(TEST101) || defined(AE101) || defined(AE102))
	sysctl_iomux_config(GPIOB, 21, 2);
	sysctl_iomux_config(GPIOB, 22, 2);
#elif (defined(AE103))
	sysctl_iomux_config(GPIOB, 25, 2);
	sysctl_iomux_config(GPIOB, 26, 2);
#endif
}
void sysctl_iomux_i2c4()
{
#if (defined(AE103))
#if AE5571
	sysctl_iomux_config(GPIOB, 0, 2);
	sysctl_iomux_config(GPIOB, 7, 2);
#endif
#endif
}
void sysctl_iomux_i2c5()
{
#if (defined(AE103))
#if AE5571
	sysctl_iomux_config(GPIOA, 4, 3);
	sysctl_iomux_config(GPIOA, 5, 3);
#endif
#endif
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
#ifdef AE103
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
void sysctl_iomux_cec()
{
	sysctl_iomux_config(GPIOB, 8, 2);
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
#endif
//*****************************************************************************
//
//  To setup gpio or kbs or pport iomux
//
//  parameter :
//      func :0:gpio,1:kbs,2:pport
//
//  return :
//      none
//
//*****************************************************************************
static void sysctl_iomux_switch_kbs_pport(VBYTE func)
{
	sysctl_iomux_config(GPIOE, 0, (func & 0x3));
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
	sysctl_iomux_switch_kbs_pport(0);
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
	sysctl_iomux_switch_kbs_pport(1);
#if (KBD_8_n_SWITCH == 17||KBD_8_n_SWITCH == 18)    
	sysctl_iomux_config(GPIOA, 19, 1); // C3 kso 16
#endif
#if(KBD_8_n_SWITCH == 18)
	sysctl_iomux_config(GPIOA, 21, 1); // C3 kso 17
#endif
}
//*****************************************************************************
//
//  To setup pport  iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_pport(void)
{
	sysctl_iomux_switch_kbs_pport(2);
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
//*****************************************************************************
//
//  To setup can0-3 iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_can0(void)
{
	sysctl_iomux_config(GPIOA, 13, 1); // CAN0 tx
	sysctl_iomux_config(GPIOA, 14, 1); // CAN0 rx
}
void sysctl_iomux_can1(void)
{
	sysctl_iomux_config(GPIOA, 4, 2); // CAN1 tx
	sysctl_iomux_config(GPIOA, 5, 2); // CAN1 rx
}
void sysctl_iomux_can2(void)
{
	sysctl_iomux_config(GPIOC, 8, 1); // CAN2 tx
	sysctl_iomux_config(GPIOC, 9, 1); // CAN2 rx
}
void sysctl_iomux_can3(void)
{
	sysctl_iomux_config(GPIOA, 22, 2); // CAN3
#if (defined(AE102))
	sysctl_iomux_config(GPIOB, 26, 2); // CAN3
#elif defined(AE103) 
	sysctl_iomux_config(GPIOA, 30, 2); // CAN3
#endif
}
/**
 * @brief GPIO上拉设置
 *
 * @param    GPIO     AE102可选参数：GPIOA ,GPIOB...GPIOE
 *                    AE103可选参数：GPIOA ,GPIOB...GPIOE
 * @param    Num      号码选择如：GPIOA[Num] Num可选0-31
 *
 * @return   无
 */
BYTE GPIO_Pullup_Config(BYTE GPIO, BYTE Num)
{
#ifdef AE103
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
			printf("参数选择错误\n");
			break;
	}
#endif
#if (defined(AE101) || defined(AE102))
	switch(GPIO)
	{
		case GPIOA:
			if(Num <= 15)
			{
				SYSCTL_PIO0_UDCFG |= (0x1 << Num);
			}
			else
			{
				SYSCTL_PIO1_UDCFG |= (0x1 << (Num - 16));
			}
			break;
		case GPIOB:
			if(Num <= 15)
			{
				SYSCTL_PIO2_UDCFG |= (0x1 << Num);
			}
			else
			{
				SYSCTL_PIO3_UDCFG |= (0x1 << (Num - 16));
			}
			break;
		case GPIOC:
			if(Num <= 15)
			{
				SYSCTL_PIO4_UDCFG |= (0x1 << Num);
			}
			else
			{
				printf("GPIO serial number input error");
			}
			break;
		case GPIOD:
			SYSCTL_PIO5_UDCFG |= (0x1 << Num);
			break;
		case GPIOE:
			if(Num <= 15)
			{
				SYSCTL_PIO5_UDCFG |= (0x1 << (Num + 16));
			}
			else if(Num <= 23)
			{
				SYSCTL_PIO5_UDCFG |= (0x1 << (Num - 8));
			}
			break;
		default:
			printf("参数选择错误\n");
			break;
	}
#endif
	return 0;
}

#if defined(AE103)
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
#endif

void GPIOAutoTest(void)//102
{
	BYTE gpa0, gpa1, gpa2, gpa3;
	BYTE gpb0, gpb1, gpb2, gpb3;
	BYTE gpc0, gpc1, gpc2;
	BYTE gpd0, gpd1, gpd2;

	BYTE gpa0z, gpa1z, gpa2z, gpa3z;
	BYTE gpb0z, gpb1z, gpb2z, gpb3z;
	BYTE gpc0z, gpc1z, gpc2z;
	BYTE gpd0z, gpd1z, gpd2z;

	static char flag = 1;
	for(int i = 0; i < 32; i++)
	{
		if(i != 24 && i != 25)//uart0
		{
			GPIO_Config(GPIOA, i, 1, flag, 0, 0);
		}
	}
	for(int i = 0; i < 27; i++)
	{
		GPIO_Config(GPIOB, i, 1, flag, 0, 0);
	}
	for(int i = 0; i < 14; i++)
	{
		GPIO_Config(GPIOC, i, 1, flag, 0, 0);
	}
	for(int i = 0; i < 7; i++)
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

	gpc0 = GPIO2_EXT0;
	gpc1 = GPIO2_EXT1;
	gpc2 = GPIO2_EXT2;

	gpd0 = GPIO3_EXT0;
	gpd1 = GPIO3_EXT1;
	gpd2 = GPIO3_EXT2;
	if(flag)
	{
		gpb3 |= 0xf8;//只有低三位有效
		gpc1 |= 0xc0;//没有高两位
		gpc2 |= 0x80;//没有最高位

		printf("\n");
		printf("GPIO0: 0x%x 0x%x 0x%x 0x%x\n", gpa0, gpa1, gpa2, gpa3);
		printf("GPIO1: 0x%x 0x%x 0x%x 0x%x\n", gpb0, gpb1, gpb2, gpb3);
		printf("GPIO2: 0x%x 0x%x 0x%x\n", gpc0, gpc1, gpc2);
		printf("GPIO3: 0x%x 0x%x 0x%x\n", gpd0, gpd1, gpd2);
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
		}
		else
		{
			printf("Self-test successful\n");
		}
	}
	else
	{
		gpa3 &= (~0x3);//uart0
		gpb3 &= (~0xf8);//只有低三位有效
		gpc1 &= (~0xc0);//没有高两位
		gpc2 &= (~0x80);//没有最高位
		printf("\n");
		printf("GPIO0: 0x%x 0x%x 0x%x 0x%x\n", gpa0, gpa1, gpa2, gpa3);
		printf("GPIO1: 0x%x 0x%x 0x%x 0x%x\n", gpb0, gpb1, gpb2, gpb3);
		printf("GPIO2: 0x%x 0x%x 0x%x\n", gpc0, gpc1, gpc2);
		printf("GPIO3: 0x%x 0x%x 0x%x\n", gpd0, gpd1, gpd2);
		//gpa0 gpa1...全部加起来不等于0
		if((gpa0 + gpa1 + gpa2 + gpa3 + gpb0 + gpb1 + gpb2 + gpb3 + gpc0 + gpc1 + gpc2 + gpd0 + gpd1 + gpd2))
		{
			printf("Self-test failed\n");
		}
		else
		{
			printf("Self-test successful\n");
		}
	}

	if(flag)
	{
		flag = 0;
	}
	else
	{
		flag = 1;
	}
}


