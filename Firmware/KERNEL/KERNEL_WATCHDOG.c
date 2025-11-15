/*
 * @Author: Iversu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2025-10-17 22:16:56
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
#include "KERNEL_WATCHDOG.H"
/**
 * @brief 清除看门狗超时中断但不进行喂狗
 *
 * @param    无
 *
 * @return   无
 */
void WDT_Clear_IRQ(void)
{
	if(WDT_STAT & WDT_ISR)
	{
		/*Clear interruption*/
		WDT_EOI;
		return;
	}
}
/**
 * @brief 清除看门狗超时中断并进行喂狗操作
 *
 * @return             无
 */
void WDT_FeedDog(void)
{
WDT_ReFeed:
	WDT_CRR = WDT_CRR_CRR; // 喂狗
	if(WDT_STAT & WDT_ISR)   // 判断如果没清除中断则出现异常，手动清除后喂狗
	{
		/*Clear interruption*/
		WDT_EOI;
		dprint("WDT Feed Fail");
		goto WDT_ReFeed;
	}
	return;
}
/**
 * @brief 看门狗初始化
 *
 * @param    mode       0:超时复位      1：超时中断
 * @param    count      重装载计数值
 *
 * @return   无
 */
void WDT_Init(BYTE mode, WORD count)
{
#if !(WDT_CLOCK_EN)
	assert_print("WDT NO CLOCK");
	return;
#endif
	WDT_CR = 0x1d | ((mode & 0x1) << 1);
	uWord torr = { .word = count };
	WDT_TORR0 = torr.byte[0]; WDT_TORR1 = torr.byte[1];
	WDT_FeedDog();
	return;
}
