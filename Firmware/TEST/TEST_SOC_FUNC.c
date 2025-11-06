/*
 * @Author: Maple
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-18 20:51:25
 * @Description: This file is used for soc module initiliazation and configuration
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
#include "TEST_SOC_FUNC.H"
#if TEST
char* err = "PS2 %d self check error\n";
void ps2_0_init_test(void)
{
	register u8 timeout;
	PS2_PORT0_CR = 0xAA;
	timeout = 100;
	do
	{
		if (PS2_PORT0_IBUF == 0x55)break;
	} while (--timeout);
	if (!timeout)
		dprint(err, 0);
}

void ps2_1_init_test(void)
{
	register u8 timeout;
	PS2_PORT1_CR = 0xAA;
	timeout = (PS2_CLOCK / 100000);
	do
	{
		if (PS2_PORT1_IBUF == 0x55)break;
	} while (--timeout);
	if (!timeout)
		dprint(err, 1);
}
void can_lpc_init(void)
{
#if CAN_MODULE_EN
	can0_ModuleClock_EN;
	can1_ModuleClock_EN;
	can2_ModuleClock_EN;
	can3_ModuleClock_EN;
	sysctl_iomux_can();
	/*can*/
	// 波特率为125kbs 验证id为0x123 掩码0xffff8000
	CAN0_Init(_125kbs, 0x123, 0xffff8000);
	CAN1_Init(_125kbs, 0x123, 0xffff8000);
	CAN2_Init(_125kbs, 0x123, 0xffff8000);
	CAN3_Init(_125kbs, 0x123, 0xffff8000);
#if CAN0_Receive
	Can_Int_Enable(CAN0, rie);
#endif
#if CAN1_Receive
	Can_Int_Enable(CAN1, rie);
#endif
#if CAN2_Receive
	Can_Int_Enable(CAN2, rie);
#endif
#if CAN3_Receive
	Can_Int_Enable(CAN3, rie);
#endif
	dprint("Can init done\n");
#endif
#if LPC_MODULE_EN
	LPC_MON_CNT = 0;
	LPC_SOF = 0;
	LPC_EOF = 0;
	Lpc_To_Ram_Config();
#endif
}

#endif