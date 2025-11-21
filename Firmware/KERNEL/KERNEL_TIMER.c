/*
 * @Author: Iversu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2025-10-18 20:23:05
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
#include "AE_FUNC.H"
#include "KERNEL_TIMER.H"
#include "KERNEL_ACPI.H"
#include "KERNEL_WATCHDOG.H"
#include "KERNEL_MEMORY.H"
 /**
  * @brief reload timer count
  *
  * @param delay : soft delay while cnt.
  */
void OPTIMIZE1 udelay(DWORD delay)
{
	do
	{
		delay--;
	}
	while(delay);
}
/**
 * @brief TIMER初始化
 *
 * @param index        timer0-4选择  可选参数：0x0 0x1 0x2 0x3 分别对应timer0-4选择
 * @param load_count   重装载值
 * @param loop_enable  1：循环定时模式  0：单次定时模式
 * @param int_mask     1：屏蔽定时中断  0：不屏蔽定时中断
 *
 * @return
 */
int TIMER_Init(u8 index, u32 load_count, u8 loop_enable, u8 int_mask)
{
#if !(TIMER0_CLOCK_EN | TIMER1_CLOCK_EN | TIMER2_CLOCK_EN | TIMER3_CLOCK_EN)
	return 0;
#endif
	// enable TIMER
	// set control value.
	TIMER_REG(0x14 * index + TIMER_TLC0_OFFSET) = load_count & 0xFF;
	TIMER_REG(0x14 * index + TIMER_TLC1_OFFSET) = (load_count >> 8) & 0xFF;
	TIMER_REG(0x14 * index + TIMER_TCR_OFFSET) = (0x1 << 0) | (loop_enable << 1) | (int_mask << 2);
	return 0;
}
/**
 * @brief 关闭timer定时器
 *
 * @param index        timer0-3选择  可选参数：TIMER0 TIMER1 TIMER2 TIMER3 分别对应timer0-3选择
 *
 * @return
 */
int TIMER_Disable(u8 index)
{
	TIMER_REG(0x14 * index + TIMER_TCR_OFFSET) &= ~TIMER_EN;
	return TIMER_REG(0x14 * index + TIMER_TCR_OFFSET);
}
/**
 * @brief 使能timer定时器
 *
 * @param index         timer0-3选择  可选参数：TIMER0 TIMER1 TIMER2 TIMER3 分别对应timer0-3选择
 *
 * @return
 */
void TIMER_Enable(u8 index)
{
	TIMER_REG(0x14 * index + TIMER_TCR_OFFSET) |= TIMER_EN;
}
/**
 * @brief timer定时中断屏蔽
 *
 * @param index         timer0-3选择  可选参数：TIMER0 TIMER1 TIMER2 TIMER3 分别对应timer0-3选择
 * @param int_mask     0：不屏蔽定时中断  1：屏蔽定时中断
 *
 * @return
 */
void TIMER_Int_Mask(u8 index, u8 int_mask)
{
	if(int_mask == 0)
	{
		TIMER_REG(0x14 * index + TIMER_TCR_OFFSET) &= (~TIMER_MASK_EN);
	}
	else
	{
		TIMER_REG(0x14 * index + TIMER_TCR_OFFSET) |= TIMER_MASK_EN;
	}
}
/**
 * @brief timer定时中断使能
 *
 * @param channel        timer0-3选择  可选参数：TIMER0 TIMER1 TIMER2 TIMER3 分别对应timer0-3选择
 *
 * @return
 */
BYTE Timer_Int_Enable(BYTE channel)
{
	switch(channel)
	{
		case TIMER0:
			TIMER0_TCR &= ~TIMER_MASK_EN;
			return 0;
		case TIMER1:
			TIMER1_TCR &= ~TIMER_MASK_EN;
			return 0;
		case TIMER2:
			TIMER2_TCR &= ~TIMER_MASK_EN;
			return 0;
		case TIMER3:
			TIMER3_TCR &= ~TIMER_MASK_EN;
			return 0;
		default:
			return -1;
	}
}
/**
 * @brief timer定时中断屏蔽
 *
 * @param channel         timer0-3选择  可选参数：TIMER0 TIMER1 TIMER2 TIMER3 分别对应timer0-3选择
 *
 * @return
 */
BYTE Timer_Int_Disable(BYTE channel)
{
	switch(channel)
	{
		case TIMER0:
			TIMER0_TCR |= TIMER_MASK_EN;
			return 0;
		case TIMER1:
			TIMER1_TCR |= TIMER_MASK_EN;
			return 0;
		case TIMER2:
			TIMER2_TCR |= TIMER_MASK_EN;
			return 0;
		case TIMER3:
			TIMER3_TCR |= TIMER_MASK_EN;
			return 0;
		default:
			return -1;
	}
}
/**
 * @brief 读出timer中断是否使能
 *
 * @param channel        timer0-3选择  可选参数：TIMER0 TIMER1 TIMER2 TIMER3 分别对应timer0-3选择
 *
 * @return 0：屏蔽中断  1：允许中断  -1：错误
 */
BYTE Timer_Int_Enable_Read(BYTE channel)
{
	switch(channel)
	{
		case TIMER0:
			return ((TIMER0_TCR & TIMER_MASK_EN) == 0);
		case TIMER1:
			return ((TIMER1_TCR & TIMER_MASK_EN) == 0);
		case TIMER2:
			return ((TIMER2_TCR & TIMER_MASK_EN) == 0);
		case TIMER3:
			return ((TIMER3_TCR & TIMER_MASK_EN) == 0);
		default:
			return -1;
	}
}
/**
 * @brief 读出timer中断状态
 *
 * @param channel        timer0-3选择  可选参数：TIMER0 TIMER1 TIMER2 TIMER3 分别对应timer0-3选择
 *
 * @return 1：有中断  0：无中断  -1：错误
 */
BYTE Timer_Int_Status(BYTE channel)
{
	switch(channel)
	{
		case TIMER0:
			return ((TIMER0_TIS & TIMER_INT) != 0);
		case TIMER1:
			return ((TIMER1_TIS & TIMER_INT) != 0);
		case TIMER2:
			return ((TIMER2_TIS & TIMER_INT) != 0);
		case TIMER3:
			return ((TIMER3_TIS & TIMER_INT) != 0);
		default:
			return -1;
	}
}

/**
 * @brief 清除timer中断状态
 *
 * @param channel        timer0-3选择  可选参数：TIMER0 TIMER1 TIMER2 TIMER3 分别对应timer0-3选择
 *
 */
void Timer_Int_Clear(BYTE channel)
{
	switch(channel)
	{
		case TIMER0:
			TIMER0_TEOI;
			break;
		case TIMER1:
			TIMER1_TEOI;
			break;
		case TIMER2:
			TIMER2_TEOI;
			break;
		case TIMER3:
			TIMER3_TEOI;
			break;
	}
}
//-----------------------------------------------------------------------------
// Delay 1ms function use timer 1
//-----------------------------------------------------------------------------
void vDelayXms(WORD bMS)
{
	TIMER_Disable(TIMER1);
	for(; bMS != 0; bMS--)
	{
		TIMER_Init(TIMER1, TIMER1_1ms, 0x0, 0x1);
		while((TIMER_TRIS & BIT(TIMER1)) == 0);
		TIMER1_TEOI;
		TIMER_Disable(TIMER1);
	}
}
//----------------------------------------------------------------------------
// Delay Xus function
//----------------------------------------------------------------------------
void vDelayXus(DWORD bUS)
{
	TIMER_Disable(TIMER3);
	TIMER_Init(TIMER3, TIMER3_1us * bUS, 0x0, 0x1);
	while((TIMER_TRIS & BIT(TIMER3)) == 0);
	TIMER3_TEOI;
	TIMER_Disable(TIMER3);
}
void wait_seconds(size_t n)
{
	unsigned long start_mtime;
	// Don't start measuruing until we see an mtime tick
	enable_mcycle_minstret(); // 打开定时器
	start_mtime = mtime_lo();
	while((mtime_lo() - start_mtime) <= (n * CPU_TIMER_FREQ));
	disable_mcycle_minstret(); // 关闭定时器
}
