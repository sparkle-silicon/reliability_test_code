/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-04 17:18:26
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
#include "CUSTOM_INIT.H"

// u_int32_t CHIP_CLOCK_SWITCH = CHIP_CLOCK_SWITCH; // 1(96) 2(48) 3(32) 4（24）5（19.2）分频数（时钟）  
/****************************************************************************
* SPKAE10X Init FLOW :
* 1. DoubleBoot (Custom Configuration Double Boot Addr，Function Is Get_DoubleBoot_ADDR() In File CUSTOM_INIT.c)
* 2. Default  Config(Custom Configuration All ,Function Is Default_Config() In File CUSTOM_INIT.c)
* 3. Modlue Init (Custom Configuration All ,Function Is Module_init() In File KERNEL_SOC_FUNC.c)
* 4. Interrupt Configuration (Custom Configuration All , Function Is Irqc_init() In File KERNEL_IRQ.c)
* 5. return start and goto main() function
****************************************************************************/
/*
 * @brief 提供初始化 boot的id（boot id），boot跳转地址
 * @id boot0 id: = NULL(不跳转,默认)
 * @id boot1 id: = 0xff0(直接跳转)
 * @id boot2 id: = 0xff1(看门狗复位则跳转)
 * @id boot3 id: = 0xff2(PWRSW 超时复位)
 * @return (id<<20)|(addr)
 */
FUNCT_PTR_V_V Get_DoubleBoot_ADDR(void)
{
	register FUNCT_PTR_V_V db_ptr = NULL;
#ifdef DUBLE_FIRMWARE1
	db_ptr = (FUNCT_PTR_V_V)ADDR_OR_ID(DoubleBoot_DEF_ADDR, 0xFF1); // 0xff1,ff3,ff5
#elif defined(DUBLE_FIRMWARE2)
	db_ptr = (FUNCT_PTR_V_V)NULL;
#else
	db_ptr = (FUNCT_PTR_V_V)NULL;
#endif
	return db_ptr;
}
/*
 * @brief 针对需要复位重新初始化的部分模块
 */
void Default_Module_Reset(void)
{
	SYSCTL_RST0 &= ~(0);
	SYSCTL_RST0 |= (0);
	SYSCTL_RST1 &= ~(0);
	SYSCTL_RST1 |= (0);


}
/*
 * @brief 针对需要绝对使能或者可以关闭的模块的部分模块
 */
void Default_Module_Enable()//Module Clock Enable
{
	SYSCTL_MODEN0 |= (0);
	SYSCTL_MODEN1 |= (0);
}
void Default_Module_Disable()//Module Clock Disabled
{
	SYSCTL_MODEN0 &= ~(0);
	SYSCTL_MODEN1 &= ~(0);
}
/*
 * @brief 如果开启启动低功耗，可通过以下两个函数配置引脚默认状态
 * @function1 ：Default_PinIO_Set
 * @function2 ：Default_GPIO_LowPower
 */
void Default_PinIO_Set(int val, int GPIO, int idx, int lens)
{
	for(register int i = 0; i <= lens; i++)
	{
		GPIO_Input_EN(GPIO, idx + i, (!((val & BIT(i)) >> i)));
	}
}
void Default_GPIO_InputSet()
{
	Default_PinIO_Set(PinA0_7_InOut, GPIOA, 0, 7);
	Default_PinIO_Set(PinA8_15_InOut, GPIOA, 8, 7);
	Default_PinIO_Set(PinA16_23_InOut, GPIOA, 16, 7);
	Default_PinIO_Set(PinA24_31_InOut, GPIOA, 24, 7);
	Default_PinIO_Set(PinB0_7_InOut, GPIOB, 0, 7);
	Default_PinIO_Set(PinB8_15_InOut, GPIOB, 8, 7);
	Default_PinIO_Set(PinB16_23_InOut, GPIOB, 16, 7);
	Default_PinIO_Set(PinB24_31_InOut, GPIOB, 24, 7);
	Default_PinIO_Set(PinC0_7_InOut, GPIOC, 0, 7);
	Default_PinIO_Set(PinC8_15_InOut, GPIOC, 8, 7);
	Default_PinIO_Set(PinD0_7_InOut, GPIOD, 0, 7);
	Default_PinIO_Set(PinD8_InOut, GPIOD, 8, 1);
	Default_PinIO_Set(PinE0_7_InOut, GPIOE, 0, 7);
	Default_PinIO_Set(PinE8_15_InOut, GPIOE, 8, 7);
	Default_PinIO_Set(PinE16_23_InOut, GPIOE, 16, 7);
}
/*
 * @brief 配置主时钟分频
 */
void Default_Freq(void)
{
	if(SYSCTL_PIO_CFG & BIT1)//使用外部FLASH
	{
		if(CHIP_CLOCK_SWITCH > 1)//外部FLASH不能使用96MHz,SPIF不支持
		{
			SYSCTL_CLKDIV_OSC96M = (CHIP_CLOCK_SWITCH - 1); //配置主时钟分频
		}
		else
		{//使用默认的24MHz
			SYSCTL_CLKDIV_OSC96M = (4 - 1);//配置主时钟分频
		}
	}
	else//内部FLASH
	{//Rom初始化完成,不需要再来
		// SYSCTL_CLKDIV_OSC96M = (CHIP_CLOCK_SWITCH - 1); //配置主时钟分频
	}
	nop; nop;//等待两个时钟周期
}
/*
 * @brief 配置中断向量表
 */
void Default_Vector(void)
{
	if(SYSCTL_PIO_CFG & BIT1)//使用外部FLASH
	{
		uint32_t *vector = (uint32_t *)&vector_base;
		uint32_t *ivt = (uint32_t *)IVT_BASE_ADDR;
		for(size_t i = 0; i < 33; i++)//32个中断向量表+1个异常中断跳转指令
		{
			ivt[i] = vector[i];
		}
	}
	else//内部FLASH
	{//Rom初始化完成,不需要再来
	}
}
/*
 * @brief 进行一些默认配置,防止异常
 */
void SECTION(".init.Default") Default_Config()
{
	// 默认频率配置
	Default_Freq();
	//默认中断向量表配置
	Default_Vector();
	//默认需要开启的模块使能
	Default_Module_Enable();
	//默认模块复位配置(防止后续初始化异常使用)
	Default_Module_Reset();
	//默认模块关闭配置(降低功耗),也是时钟关闭
	Default_Module_Disable();
	//默认GPIO输入关闭功能(降低功耗),也是输入使能关闭
	// Default_GPIO_InputSet();
}
//----------------------------------------------------------------------------
// FUNCTION: Device_init
// edevice enable
//----------------------------------------------------------------------------
void Device_init(void)
{
	// device PD CHIP Init
#if SUPPORT_ANX7447
	u8 ret = ucsi_init();
	if(ret != UCSI_COMMAND_SUCC)
	{
		assert_print("ucsi_init failed.\n");
		// return UCSI_COMMAND_FAIL;
	}
#endif
#if (PWM_MODULE_EN)
#if (SUPPORT_FAN1&&(FAN1_PWM_CHANNEL_SWITCH<=7))
	FAN_Init(FAN1_PWM_CHANNEL_SWITCH, PWM_CLK0, PWM_CTR0);
#endif
#if SUPPORT_FAN2&&(FAN1_PWM_CHANNEL_SWITCH<=7)
	FAN_Init(FAN2_PWM_CHANNEL_SWITCH, PWM_CLK0, PWM_CTR0);
#endif
#endif
#if SUPPORT_LD_PNP_DEVBOARD
	LogicalDevice_PNP_Config();
#endif
#if SUPPORT_SHAREMEM_PNP
	ShareMem_PNP_Config();
#endif
} // 初始化配置
