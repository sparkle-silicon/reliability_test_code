/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-18 22:02:55
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
 * @brief 针对需要绝对使能或者可以关闭的模块的部分模块
 */
void Default_Module_Enable(void)//Module Clock Enable
{
	SYSCTL_MODEN0 |= (0xFFFFFFFF);
	SYSCTL_MODEN1 |= (0xFFFFFFFF);
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
 * @brief 配置时钟分频
 * @function1 Default_Mailbox_SetClockFrequency(通知子系统进行升降频)
 * @function2 Default_Freq
 */
void Default_Mailbox_SetClockFrequency(BYTE ClockDiv)
{
//告知主系统更新行为
	MAILBOX_SELF_CMD = MAILBOX_CMD_FREQ_SYNC;
	MAILBOX_SELF_INFO1 = ClockDiv; // 通知子系统设置多少分频
	// 触发子系统中断
	MAILBOX_SET_IRQ(MAILBOX_Control_IRQ_NUMBER);
	MAILBOX_WAIT_IRQ(MAILBOX_CMD_FREQ_SYNC, MAILBOX_Control_IRQ_NUMBER); // 等待子系统回复
	MAILBOX_CLEAR_IRQ(MAILBOX_Control_IRQ_NUMBER); // 清除中断状态
	//修改SPIF resume和下一次suspend之间的时间，要求最小100us
	uint32_t trsmax = ((CHIP_CLOCK_INT_HIGH / 10000) / (ClockDiv + 1));
	if(trsmax >= (1 << 13))trsmax = ((1 << 13) - 1);
	SYSCTL_TRSMAX = trsmax;//修改SPIF CLOCK中的100us间隔时间,需要再频率之前修改，修改完以后可能出问题，需要重新配置频率才能修复
	//配置时钟
	SYSCTL_CLKDIV_OSC96M = ClockDiv;
	nop; nop; //等待分频生效
	//告知子系统更新完成
	MAILBOX_SELF_CMD = MAILBOX_CMD_FREQ_SYNC;
	MAILBOX_SET_IRQ(MAILBOX_Control_IRQ_NUMBER);
	MAILBOX_WAIT_IRQ(MAILBOX_CMD_FREQ_SYNC, MAILBOX_Control_IRQ_NUMBER); // 等待子系统回复
	MAILBOX_CLEAR_IRQ(MAILBOX_Control_IRQ_NUMBER); // 清除中断状态
}
void Default_Freq(void)
{
	//时钟初始化
	if(SYSCTL_PIO_CFG & BIT1)//使用外部FLASH
	{
		uint32_t clock_div = (4 - 1);//使用默认的24MHz
		if(CHIP_CLOCK_SWITCH > 1)//外部FLASH不能使用96MHz,SPIF不支持
		{
			clock_div = (CHIP_CLOCK_SWITCH - 1); //配置主时钟分频
		}
		if((!SYSCTL_ESTAT_EFUSE_EC_DEBUG) && (!SYSCTL_ESTAT_EFUSE_CRYPTO_DEBUG))//如果是走ec_debug并且子系统没起来,此时iram0代码还没考虑,不考虑通知需求
		{
				//修改SPIF resume和下一次suspend之间的时间，要求最小100us
			uint32_t trsmax = ((CHIP_CLOCK_INT_HIGH / 10000) / (clock_div + 1));
			if(trsmax >= (1 << 13))trsmax = ((1 << 13) - 1);
			SYSCTL_TRSMAX = trsmax;//修改SPIF CLOCK中的100us间隔时间,需要再频率之前修改，修改完以后可能出问题，需要重新配置频率才能修复
			SYSCTL_CLKDIV_OSC96M = clock_div;
			nop; nop;
		}
		else
		{
			Default_Mailbox_SetClockFrequency(clock_div);
		}

	}
	else//内部FLASH
	{//Rom初始化完成,不需要再来
		// if((!SYSCTL_ESTAT_EFUSE_EC_DEBUG) && (!SYSCTL_ESTAT_EFUSE_CRYPTO_DEBUG))//如果是走ec_debug并且子系统没起来
		// {
		// 		//修改SPIF resume和下一次suspend之间的时间，要求最小100us
		// 	uint32_t trsmax = ((CHIP_CLOCK_INT_HIGH / 10000) / CHIP_CLOCK_SWITCH);
		// 	if(trsmax >= (1 << 13))trsmax = ((1 << 13) - 1);
		// 	SYSCTL_TRSMAX = trsmax;//修改SPIF CLOCK中的100us间隔时间,需要再频率之前修改，修改完以后可能出问题，需要重新配置频率才能修复
		// 	SYSCTL_CLKDIV_OSC96M = (CHIP_CLOCK_SWITCH - 1);
		// 	nop; nop;
		// }
		// else
		// {
		// 	Default_Mailbox_SetClockFrequency(CHIP_CLOCK_SWITCH - 1);
		// }
	}
	//Oscillator Calibration时钟校准
	if((SYSCTL_OSCTRIM & 0x007FEFFF) == 0x00338880)
	{
	#if SOFTWARE_TRIM_CONTROL
	#define TEST_CHIPNUMBER 0
	#if (TEST_CHIPNUMBER!=0)&&0
		{//Bypass OSC Output	
			sysctl_iomux_config(GPIOB, 31, 0x0);//将GPH7设置为GPIO，即设置为默认不输出
			GPIO_Input_EN(GPIOB, 31, DISABLE);  //GPH[7]的IE配为0
			SYSCTL_PMU_CFG |= ((1 << 0) | (1 << 9)); //寄存器0x3_0518(PMU test enable )的bit0和bit9(PMU BUF enable)均置1 
			sysctl_iomux_config(GPIOA, 15, 0x3);//将GPB7设置为BYPASS OSC32K
		}
	#endif
		{
		#if (TEST_CHIPNUMBER==1)	//1号片
		#define LOW_32K_FTRIM_DVAL 0x69
		#define LOW_32K_TTRIM_DVAL 0x8
		#define HIGH_24M_FTRIM_DVAL 0x1d
		#define HIGH_24M_TTRIM_DVAL 0x8
		#elif TEST_CHIPNUMBER==2	//2号片
		#define LOW_32K_FTRIM_DVAL 0x6A
		#define LOW_32K_TTRIM_DVAL 0x8
		#define HIGH_24M_FTRIM_DVAL 0x1d
		#define HIGH_24M_TTRIM_DVAL 0x9
		#else//默认值(不推荐改变该值)
		#define LOW_32K_FTRIM_DVAL 0x80
		#define LOW_32K_TTRIM_DVAL 0x8
		#define HIGH_24M_FTRIM_DVAL 0x1C
		#define HIGH_24M_TTRIM_DVAL 0x6
		#endif
			{//进行trim值填入
				SYSCTL_OSCTRIM = (SYSCTL_OSCTRIM & (~(0x007FEFFF))) | \
					((((LOW_32K_FTRIM_DVAL) & 0xFF) << 0) | (((LOW_32K_TTRIM_DVAL) & 0x0F) << 8) | \
					(((HIGH_24M_FTRIM_DVAL) & 0x3F) << 13) | (((HIGH_24M_TTRIM_DVAL) & 0x0F) << 19));
			}
		}
	#endif
	}
	else
	{
		//认为硬件trim完成
	}
	//高精准时钟功能开启
#if SUPPORT_OSC96MHZ_CHOPEN
	SYSCTL_OSCTRIM |= SYSCTL_OSCTRIM_CHOPEN;
#else
	SYSCTL_OSCTRIM &= ~SYSCTL_OSCTRIM_CHOPEN;
#endif
	//获取当前CPU频率
	get_cpu_freq();
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
 * @brief 配置IRam0
 */
void Default_Iram0(void)
{
	if(SYSCTL_PIO_CFG & BIT1)//使用外部FLASH
	{
		uint32_t *iram_cache = (uint32_t *)NULL;//0xC0000
		uint32_t *iram0 = (uint32_t *)IRAM0_BASE_ADDR;
		{//数据搬运
			if(iram_cache != NULL)
			{
				for(size_t i = 0; i < (32 * 1024 / 4); i++)//32个中断向量表+1个异常中断跳转指令
				{
					iram0[i] = iram_cache[i];
				}
				if((!SYSCTL_ESTAT_EFUSE_EC_DEBUG) && (!SYSCTL_ESTAT_EFUSE_CRYPTO_DEBUG))//确认子系统处于等待置位的状态
				{
					SYSCTL_CRYPTODBG_FLAG |= BIT(0);//子系统开始运行
				}
			}
		}
	}
	else//内部FLASH
	{//Rom初始化完成,不需要再来
	}
}
/*
 * @brief 如果开启启动低功耗，可通过以下两个函数配置引脚默认状态
 */
void Default_Module_Disable(void)//Module Clock Disabled
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
	for(register int i = 0; i < lens; i++)
	{
		GPIO_Input_EN(GPIO, idx + i, (!((val & BIT(i)) >> i)));
	}
}
void Default_GPIO_InputSet()
{
	Default_PinIO_Set(PinA0_7_InOut, GPIOA, 0, 8);
	Default_PinIO_Set(PinA8_15_InOut, GPIOA, 8, 8);
	Default_PinIO_Set(PinA16_23_InOut, GPIOA, 16, 8);
	Default_PinIO_Set(PinA24_31_InOut, GPIOA, 24, 8);
	Default_PinIO_Set(PinB0_7_InOut, GPIOB, 0, 8);
	Default_PinIO_Set(PinB8_15_InOut, GPIOB, 8, 8);
	Default_PinIO_Set(PinB16_23_InOut, GPIOB, 16, 8);
	Default_PinIO_Set(PinB24_31_InOut, GPIOB, 24, 8);
	Default_PinIO_Set(PinC0_7_InOut, GPIOC, 0, 8);
	Default_PinIO_Set(PinC8_15_InOut, GPIOC, 8, 8);
	Default_PinIO_Set(PinD0_7_InOut, GPIOD, 16, 8);
	Default_PinIO_Set(PinD8_InOut, GPIOD, 24, 1);
	Default_PinIO_Set(PinE0_7_InOut, GPIOE, 0, 8);
	Default_PinIO_Set(PinE8_15_InOut, GPIOE, 8, 8);
	Default_PinIO_Set(PinE16_23_InOut, GPIOE, 16, 8);
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
	Default_PinIO_Set(PinEFLASH_InOut, GPIOE, 24, 6);//8

}
/*
 * @brief 进行一些默认配置,防止异常
 */
void SECTION(".init.Default") Default_Config()
{
	//1.默认需要开启的模块使能(保障模块正常运行)
	Default_Module_Enable();
	//2.默认模块复位配置(防止后续初始化异常使用)
	Default_Module_Reset();
	//3.默认频率配置
	Default_Freq();
	//4.默认中断向量表配置
	Default_Vector();
	//5.默认搬运iram0数据
	Default_Iram0();
	//6.默认模块关闭配置(降低功耗),也是时钟关闭
	Default_Module_Disable();
	//7.默认GPIO输入关闭功能(降低功耗),也是输入使能关闭
	Default_GPIO_InputSet();
}
//----------------------------------------------------------------------------
// FUNCTION: Device_init
// edevice enable
//----------------------------------------------------------------------------
void Device_init(void)
{
// 0.Devoce's Function Init Of Modules
#if (PWM_MODULE_EN)
#if (SUPPORT_FAN1&&(FAN1_PWM_CHANNEL_SWITCH<=7))
	FAN_Init(FAN1_PWM_CHANNEL_SWITCH, PWM_CLK0, PWM_CTR0);
#endif
#if SUPPORT_FAN2&&(FAN1_PWM_CHANNEL_SWITCH<=7)
	FAN_Init(FAN2_PWM_CHANNEL_SWITCH, PWM_CLK0, PWM_CTR0);
#endif
	//LED_Init();
#endif

//1.Host Device Init
#if SUPPORT_LD_PNP_DEVBOARD
	LogicalDevice_PNP_Config();
#endif
#if SUPPORT_SHAREMEM_PNP
	ShareMem_PNP_Config();
#endif

//3.Devoce's Driver Init
	//MOUSE_Init();
	//KEYBOARD_Init();
	//TOUCH_Init();
	//Temp_Init();//
// #if SUPPORT_ANX7447//PD
// 	u8 ret = ucsi_init();
// 	if(ret != UCSI_COMMAND_SUCC)
// 	{
// 		assert_print("ucsi_init failed.\n");
// 		// return UCSI_COMMAND_FAIL;
// 	}
// #endif
//4.Service Timer 1ms Init
#if TIMER_MODULE_EN
	TIMER_Init(TIMER2, TIMER2_1ms, 0x1, 0x0); // 1ms service计时函数
#endif
	// dprint(" Devie init End\n");
} // 初始化配置
