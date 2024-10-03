/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-02-29 11:42:58
 * @Description: This file is used to handling PS2 interface
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
#include "KERNEL_PS2.H"
#include "KERNEL_SOC_FUNC.H"
#include "AE_DEBUGGER.H"
//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Read_Status
//
//----------------------------------------------------------------------------
BYTE PS2_PortN_Read_Status(BYTE channel)
{
	volatile u8 ps2_portn_status;
	if(channel == 0)
	{
		ps2_portn_status = PS2_PORT0_SR & 0xFF;
	}
	else if(channel == 1)
	{
		ps2_portn_status = PS2_PORT1_SR & 0xFF;
	}
	else
	{
		dprint("channel error");
	}
	return ps2_portn_status;
}
//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Read_Input
//
//----------------------------------------------------------------------------
BYTE PS2_PortN_Read_Input(BYTE channel)
{
	volatile u8 ps2_portn_input;
	if(channel == 0)
	{
		ps2_portn_input = PS2_PORT0_IBUF & 0xFF;
	}
	else if(channel == 1)
	{
		ps2_portn_input = PS2_PORT1_IBUF & 0xFF;
	}
	else
	{
		dprint("channel error");
	}
	return ps2_portn_input;
}
//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Write_Output
//
//----------------------------------------------------------------------------
void PS2_PortN_Write_Output(u8 data, BYTE channel)
{
	if(channel == 0)
	{
		PS2_PORT0_OBUF = (data & 0xFF);
	}
	else if(channel == 1)
	{
		PS2_PORT1_OBUF = (data & 0xFF);
	}
	else
	{
		dprint("channel error");
	}
}
//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Write_Command
//
//----------------------------------------------------------------------------
void PS2_PortN_Write_Command(u8 data, BYTE channel)
{
	if(channel == 0)
	{
		PS2_PORT0_CR = (data & 0xFF);
	}
	else if(channel == 1)
	{
		PS2_PORT1_CR = (data & 0xFF);
	}
	else
	{
		dprint("channel error");
	}
}
//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Read_Data
//
//----------------------------------------------------------------------------
static int PS2_PortN_Read_Data(BYTE channel)
{
	BYTE val;
	BYTE status;
	val = -1;
	status = PS2_PortN_Read_Status(channel);
	if(status & PS2_STR_IBF)
	{
		val = PS2_PortN_Read_Input(channel);
		dprint("PS2_PORT%x_Read_Data %x \n", channel, val);
		if(status & (KBD_STAT_GTO | KBD_STAT_PERR))
			val = -2;
	}
	return val;
}
//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Wait_For_Input
//
//----------------------------------------------------------------------------
static int PS2_PortN_Wait_For_Input(BYTE channel)
{
	unsigned long timeout;
	int val;
	timeout = KBD_TIMEOUT;
	val = PS2_PortN_Read_Data(channel);
	while(val < 0)
	{
		if(timeout-- == 0)
			return -1;
		udelay(50000);
		val = PS2_PortN_Read_Data(channel);
	}
	return val;
}
//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Wait_For_output
//
//----------------------------------------------------------------------------
static int PS2_PortN_Wait(BYTE channel)
{
	unsigned long timeout = KBC_TIMEOUT * 10;
	do
	{
		BYTE status = PS2_PortN_Read_Status(channel);
		if(!(status & PS2_STR_OBF))
			return 0; /* OK */
		udelay(1000);
		timeout--;
	}
	while(timeout);
	return 1;
}
//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Write_Command_W
//
//----------------------------------------------------------------------------
static void PS2_PortN_Write_Command_W(char data, BYTE channel)
{
	if(PS2_PortN_Wait(channel))
		dprint("timeout in PS2_Port%x_Write_Command_W\n", channel);
	PS2_PortN_Write_Command(data, channel);
}
//----------------------------------------------------------------------------
// FUNCTION: PS2_PORT1_Write_Output_W
//
//----------------------------------------------------------------------------
void PS2_PortN_Write_Output_W(char data, BYTE channel)
{
	if(PS2_PortN_Wait(channel))
		dprint("timeout in PS2_PORT1_Write_Command_W \n");
	PS2_PortN_Write_Output(data, channel);
}
//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Write_Cmd
//
//----------------------------------------------------------------------------
void PS2_PortN_Write_Cmd(char cmd, BYTE channel)
{
	PS2_PortN_Wait(channel);
	PS2_PortN_Write_Command(CCMD_WRITE, channel); /* CCMD_WRITE */
	PS2_PortN_Wait(channel);
	PS2_PortN_Write_Output(cmd, channel);
}
//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Write_Ack
//
//----------------------------------------------------------------------------
static void PS2_PortN_Write_Ack(char val, BYTE channel)
{
	PS2_PortN_Wait(channel);
	PS2_PortN_Write_Command(CCMD_WRITE, channel);
	PS2_PortN_Wait(channel);
	PS2_PortN_Write_Output(val, channel);
	ps2_portn_reply_expected++;
	PS2_PortN_Wait(channel);
}
//----------------------------------------------------------------------------
// FUNCTION: Send_Mouse_Data
//
//----------------------------------------------------------------------------
static int Send_Mouse_Data(BYTE data, BYTE channel)
{
	int retries = 3;
	do
	{
		unsigned long timeout = KBD_TIMEOUT;
		acknowledge = 0;
		resend = 0;
		PS2_PortN_Write_Output_W(data, channel);
		for(;;)
		{
			if(acknowledge)
				return 1;
			if(resend)
				break;
			udelay(1);
			if(!--timeout)
			{
				return 0;
			}
		}
	}
	while(retries-- > 0);
	return -1;
}
//-----------------------------------------------------------------
// Function: Wait_PS2_Device_Ack_Timeout
// Description: wait PS2 device ack until time out
//-----------------------------------------------------------------
BYTE Wait_PS2_Device_Ack_Timeout(BYTE channel)
{
	BYTE result;
	BYTE receive_id_ack[3];
	BYTE cnt = 0;
	result = 0x1;
	TIMER_Disable(TIMER1);
	TIMER_Init(TIMER1, TIMER1_10ms * 3, 0x0, 0x1);
	do
	{ // Wait PS2 transaction Done Status
		BYTE status = PS2_PortN_Read_Status(channel);
		BYTE ack;
		if(status & PS2_STR_IBF)
		{
			ack = PS2_PortN_Read_Input(channel);
			if(!(status & (KBD_STAT_PERR)))
			{
				receive_id_ack[cnt] = ack;
				dprint("0xF2 ack %#x,cnt:%x\n", receive_id_ack[cnt], cnt);
				if(cnt == 1)
				{
					if(receive_id_ack[1] == 0xAB)
					{
						if(channel == 0)
						{
							PS2_PORT0_FLAG = PORT0_DEVICE_IS_KEYBOARD;
						}
						else if(channel == 1)
						{
							PS2_PORT1_FLAG = PORT1_DEVICE_IS_KEYBOARD;
						}
						else
							dprint("channel error\n");
					}
					else
					{
						if(channel == 0)
						{
							PS2_PORT0_FLAG = PORT0_DEVICE_IS_MOUSE;
						}
						else if(channel == 1)
						{
							PS2_PORT1_FLAG = PORT1_DEVICE_IS_MOUSE;
						}
						else
							dprint("channel error\n");
					}
				}
				cnt++;
				result = 0x0;
			}
			else
			{
				dprint("ack data error\n");
			}
		}
	}
	while((TIMER_TRIS & 0x2) != 0x2); // waitting for overflow flag
	TIMER_Disable(TIMER1);
	dprint("PS2_PORT0_FLAG:%x,PS2_PORT1_FLAG:%x\n", PS2_PORT0_FLAG, PS2_PORT1_FLAG);
	return (result);
}
//-----------------------------------------------------------------
// Function: PS2_Device_Identificate
// Description: automatic identificate ps2 device
//-----------------------------------------------------------------
BYTE PS2_Device_Identificate(BYTE PortNum, BYTE cmd)
{
	BYTE result;
	PS2_PortN_Write_Output_W(cmd, PortNum);
	result = Wait_PS2_Device_Ack_Timeout(PortNum);
	return (result);
}
//-----------------------------------------------------------------
// Function: PS2_Channel_Device_Scan
// Description: KBD_SCAN ps2 channel device
//-----------------------------------------------------------------
void PS2_Channel_Device_Scan(void)
{
	/*KBD_SCAN ps2 port0/1 device*/
	for(BYTE i = 0; i < 2; i++)
	{
		PS2_Device_Identificate(i, 0xF2);
	}
}
//-----------------------------------------------------------------
// Function: PS2_Main_Device_Confirm
// Description: confirm channel main device
//-----------------------------------------------------------------
// BYTE MS_Main_CHN,KB_Main_CHN = 0;
void PS2_Main_Device_Confirm(void)
{
	MS_Main_CHN = 0;
	KB_Main_CHN = 0;
	dprint("MS_Main_CHN:%#x,KB_Main_CHN:%#x\n", MS_Main_CHN, KB_Main_CHN);
	dprint("PS2_PORT_FLAG:%#x\n", PS2_PORT0_FLAG | PS2_PORT1_FLAG);
	switch(PS2_PORT0_FLAG | PS2_PORT1_FLAG)
	{
		case 0x00:
			MS_Main_CHN = 0;
			KB_Main_CHN = 0;
			break;
		case 0x01:
			if(PS2_PORT0_FLAG & PORT0_DEVICE_IS_MOUSE)
			{
				MS_Main_CHN = 0x1;
			}
			else
			{
				MS_Main_CHN = 0x2;
			}
			KB_Main_CHN = 0;
			break;
		case 0x02:
			if(PS2_PORT0_FLAG & PORT0_DEVICE_IS_KEYBOARD)
			{
				KB_Main_CHN = 0x1;
			}
			else
			{
				KB_Main_CHN = 0x2;
			}
			MS_Main_CHN = 0;
			break;
		case 0x03:
			if(PS2_PORT0_FLAG & PORT0_DEVICE_IS_MOUSE)
			{
				MS_Main_CHN = 0x1;
				KB_Main_CHN = 0x2;
			}
			else
			{
				MS_Main_CHN = 0x2;
				KB_Main_CHN = 0x1;
			}
			break;
		default:
			MS_Main_CHN = 0;
			KB_Main_CHN = 0;
			break;
	}
	dprint("MS_Main_CHN:%#x,KB_Main_CHN:%#x\n", MS_Main_CHN, KB_Main_CHN);
}
/* ----------------------------------------------------------------------------
 * FUNCTION: PS2 controlller selftest
 * ------------------------------------------------------------------------- */
int PS2_PortN_Selftest(BYTE channel)
{
	PS2_PortN_Write_Command_W(CCMD_SELFTEST_AA, channel);
	if(PS2_PortN_Wait_For_Input(channel) != 0x55)
		return 1;
	PS2_PortN_Write_Command_W(CCMD_SELFTEST_AB, channel);
	if(PS2_PortN_Wait_For_Input(channel) != 0x00)
		return 1;
	return 0;
}
/* ----------------------------------------------------------------------------
 * FUNCTION: all PS2 controlller selftest
 * ------------------------------------------------------------------------- */
void PS2_All_Port_Selftest(void)
{
	for(BYTE i = 0; i < 2; i++)
	{
		if(PS2_PortN_Selftest(i) == 0)
		{
			dprint("ps2 port%d selfetest success\n", i);
		}
		else
		{
			dprint("ps2 port%d selfetest failed\n", i);
		}
	}
}
/* ----------------------------------------------------------------------------
 * FUNCTION: 发送FF指令给触摸板，复位触摸板
 * ------------------------------------------------------------------------- */
void Send_ResetCmd_To_MS_WaitACK(BYTE PortNum)
{
	BYTE ack;
	if(PortNum == 0) // 发送Cmd之前屏蔽中断，以防中断中断中被调用，导致嵌套
		irqc_disable_interrupt(IRQC_INT_DEVICE_PS2_CH0);
	else if(PortNum == 1)
		ICTL1_INTMASK5 |=0x40;
	PS2_PortN_Write_Output_W(0xFF, PortNum);
	TIMER_Disable(0x0);
	TIMER_Init(0x0, TIMER0_1s, 0x0, 0x1);
	do // Wait PS2 ACK 0xFA
	{
		BYTE status = PS2_PortN_Read_Status(PortNum);
		if(status & PS2_STR_IBF)
		{
			ack = PS2_PortN_Read_Input(PortNum);
		}
	}
	while(((TIMER_TRIS & 0x1) != 0x1) && (ack != 0x0)); // waitting for overflow flag
	TIMER_Disable(0x0);
	if(PortNum == 0) // 等到取出ACK后再重新使能中断
	{
		irqc_enable_interrupt(IRQC_INT_DEVICE_PS2_CH0);
	}
	else if(PortNum == 1)
	{
		ICTL1_INTMASK5&=~0x40;
	}
}
/* ----------------------------------------------------------------------------
 * FUNCTION: 根据触摸板的ID来确认触摸板的驱动类型及数据帧格式
 * ------------------------------------------------------------------------- */
void MS_DriverType_Check(VBYTE data)
{
	if(MS_ID_Count != 0x00) // Save mouse driver type(3bytes, 4bytes, or 5bytes)
	{
		if(--MS_ID_Count == 0x00)
		{
			if(data == 0x00)
			{
				MS_Driver_Type = data;
				MS_Frame_Cnt = 3;
			}
			else if(data == 0x3)
			{
				MS_Driver_Type = data;
				MS_Frame_Cnt = 4;
			}
			else if(data == 0x4)
			{
				MS_Driver_Type = data;
				MS_Frame_Cnt = 5;
			}
		}
	}
}
BYTE MS_AutoReset_Data[2];
/* ----------------------------------------------------------------------------
 * FUNCTION: 识别触摸板请求复位的数据包AA 00
 * ------------------------------------------------------------------------- */
void MS_AutoReset_Check(VBYTE MS_Data, VBYTE port)
{
	if((MS_Data == 0xAA) && (MS_AutoReset_ACK_CNT == 0)) // 两字节数据帧识别0xAA的帧头,并且此0xAA是首次识别到。
	{
		MS_AutoReset_Data[MS_AutoReset_ACK_CNT] = MS_Data;
		MS_AutoReset_ACK_CNT++;
	}
	else if(MS_AutoReset_ACK_CNT > 0) // 若识别到0xAA，则直接将0x8后面1字节数据保存到两字节数组
	{
		if(MS_Data == 0xAA) // 若判定到后续连续的两字节中有0xAA，则清除两字节数组，并重新开始接收后续连续的1字节数据
		{
			MS_AutoReset_ACK_CNT = 0;
			for(int i = 0; i < 2; i++)
			{
				MS_AutoReset_Data[i] = 0;
			}
			MS_AutoReset_Data[MS_AutoReset_ACK_CNT] = MS_Data; // 重新覆盖首字节数据
			MS_AutoReset_ACK_CNT++;
			return;
		}
		MS_AutoReset_Data[MS_AutoReset_ACK_CNT] = MS_Data;
		MS_AutoReset_ACK_CNT++;
		if(MS_AutoReset_ACK_CNT >= 2) // 将此数组与0xAA 0x00判定
		{
			if(((MS_AutoReset_Data[0]) == 0xAA) && ((MS_AutoReset_Data[1]) == 0x0))
			{
				dprint("MS Auto Reset\n");
				Send_ResetCmd_To_MS_WaitACK(port); // 发现TP异常，EC发送FF命令重启TP,并且需要等到TP的回复0xFA，不能将此回复发给HOST
			}
			for(int i = 0; i < 2; i++)
			{
				MS_AutoReset_Data[i] = 0;
			}
			MS_AutoReset_ACK_CNT = 0;
		}
	}
	else
	{
		MS_AutoReset_ACK_CNT = 0;
	}
}
/* ----------------------------------------------------------------------------
 * FUNCTION: 判断触摸板的数据有效性
 * ------------------------------------------------------------------------- */
void MS_DataValid_Check(VBYTE MS_Data, VBYTE port)
{
	MS_Data_Cnt++;
	if(((MS_Data_Cnt % MS_Frame_Cnt) == 0) && ((MS_Data & 0x8) == 0))
	{
		dprint("MS data invalid\n");
		Send_ResetCmd_To_MS_WaitACK(port); // 发现TP异常，EC发送FF命令重启TP,并且需要等到TP的回复0xFA，不能将此回复发给HOST
	}
	if(MS_Data_Cnt >= MS_Frame_Cnt)
		MS_Data_Cnt = 0; // 每一帧数据清零一次
}
/* ----------------------------------------------------------------------------
 * FUNCTION: 关闭触摸板的数据使能报告
 * ------------------------------------------------------------------------- */
void Stop_MS_Reporting(VBYTE port)
{
	PS2_PortN_Write_Output_W(0xf5, port);
	TIMER_Disable(TIMER1);
	TIMER_Init(TIMER1, TIMER1_10ms * 3, 0x0, 0x1);
	do
	{ // Wait PS2 transaction Done Status
		BYTE ack;
		if(PS2_PortN_Read_Status(port) & PS2_STR_IBF)
		{
			ack = PS2_PortN_Read_Input(port);
			dprint("0xF5 ack 0x%x,\n", ack);
			break;
		}
	}
	while((TIMER_TRIS & 0x2) != 0x2); // waitting for overflow flag
	TIMER_Disable(TIMER1);
}
//-----------------------------------------------------------------
// Function: Open_Aux
//
//-----------------------------------------------------------------
void Open_Aux(BYTE channel)
{
	PS2_PortN_Write_Command_W(CCMD_MOUSE_ENABLE, channel); /* 0xA8 */
	PS2_PortN_Write_Ack(AUX_ENABLE_DEV, channel);		   /* 0xF4 enable aux device */
	PS2_PortN_Write_Cmd(AUX_INTS_ON, channel);
	udelay(2000);
	Send_Mouse_Data(KBD_CMD_ENABLE, channel); /* 0xF4 Enable scanning */
}
//-----------------------------------------------------------------
// Function: 关闭触摸板数据报告
//
//-----------------------------------------------------------------
char Disable_Aux_Data_Reporting(BYTE channel)
{
	PS2_PortN_Write_Output_W(0xf5, channel);
	TIMER_Disable(TIMER1);
	TIMER_Init(TIMER1, TIMER1_10ms * 3, 0x0, 0x1);
	do
	{ // Wait PS2 transaction Done Status
		BYTE ack;
		if(PS2_PortN_Read_Status(channel) & PS2_STR_IBF)
		{
			ack = PS2_PortN_Read_Input(channel);
			dprint("0xF5 ack 0x%x,\n", ack);
			TIMER_Disable(TIMER1);
			return 1;
		}
	}
	while((TIMER_TRIS & 0x2) != 0x2); // waitting for overflow flag
	TIMER_Disable(TIMER1);
	return 0;
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Init PS2 Device
 * ------------------------------------------------------------------------- */
int AE10x_PS2_Init(void)
{
	/*enable ps2 channel 0/1 device for scan ps2 channel*/
	PS2_PortN_Write_Command_W(CCMD_WRITE, 0);
	PS2_PortN_Write_Output_W(0x30, 0);
	PS2_PortN_Write_Command_W(CCMD_WRITE, 1);
	PS2_PortN_Write_Output_W(0x30, 1);
	// KBD_SCAN ps2 device
	PS2_Channel_Device_Scan();
	// confirm ps2 main devices
	PS2_Main_Device_Confirm();
#if 0
// Close PS2 data report
	if(Disable_Aux_Data_Reporting(MS_Main_CHN - 1))
	{
		dprint("0xf5 send success\n");
	}
#endif
// all ps2 port selftest
	//PS2_All_Port_Selftest();
	/*disable ps2 channel 0/1 after scan ps2 channel*/
	PS2_PortN_Write_Command_W(CCMD_WRITE, 0);
	PS2_PortN_Write_Output_W(0x0, 0);
	PS2_PortN_Write_Command_W(CCMD_WRITE, 1);
	PS2_PortN_Write_Output_W(0x0, 1);
	return 0;
}
/* ----------------------------------------------------------------------------
 * FUNCTION: PS2 Device Init Interface
 * ------------------------------------------------------------------------- */
BYTE PS2_DevScan(void)
{
#if 0
	PowerChange_Var_Clear();
#endif
	dprint("init ps2 modules\n");
	if(0 != AE10x_PS2_Init())
	{
		dprint("init ps2 failed");
		return -1;
	}
	return 0;
}
BYTE mouse_val;
/* ----------------------------------------------------------------------------
 * FUNCTION: Handle PS2 Mouse Data
 * ------------------------------------------------------------------------- */
void Handle_Mouse(BYTE channel)
{
	BYTE status = PS2_PortN_Read_Status(channel);
	BYTE scancode;
	if(status & PS2_STR_IBF)
	{
		scancode = PS2_PortN_Read_Input(channel);
		if(!(status & (KBD_STAT_PERR)))
		{
			mouse_val = scancode;
		#if 0
			if((TP_ACK_CUNT == 0) && (IS_MASK_CLEAR(KBC_STA, BIT(1)))) // 避开初始化命令流，只在TP正常工作时检查
			{
				MS_DriverType_Check(mouse_val);			// 检查MS的驱动类型
				MS_DataValid_Check(mouse_val, channel); // MS数据有效性检查
			}
		#endif
			if(IS_SET(KBC_STA, 0) || (Host_Flag & 0x20) || MSPendingRXCount > 0)
			{
				MS_Data_Suspend(mouse_val);
			}
			else
			{
				Send_Aux_Data_To_Host(mouse_val);
			}
		}
		else
		{
			dprint("mouse scan_code error \n");
		}
	}
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Handle PS2 Keyboard Data
 * ------------------------------------------------------------------------- */
void Handle_Kbd(BYTE channel)
{
	BYTE status = PS2_PortN_Read_Status(channel);
	BYTE scancode;
	if(status & PS2_STR_IBF)
	{
		scancode = PS2_PortN_Read_Input(channel);
		if(!(status & (KBD_STAT_PERR)))
		{
			PS2_PortN_Data[1] = scancode;
			KBS_Buffer_Input(scancode);
		}
		else
		{
			dprint("kbd scan_code error \n");
		}
	}
}
BYTE Handle_Mouse_Event(BYTE channel)
{
	BYTE status = PS2_PortN_Read_Status(channel);
	DWORD work = 10000;
	BYTE scancode;
	dprint("handle status %#x \n", status);
	while((--work > 0) && status & PS2_STR_IBF)
	{
		scancode = PS2_PortN_Read_Input(channel);
		if(!(status & (KBD_STAT_GTO | KBD_STAT_PERR)))
		{
			dprint("scan_code %d,%x \n", scancode, scancode);
		}
		else
		{
			dprint("mouse scan_code error \n");
		}
		status = PS2_PortN_Read_Status(channel);
	}
	if(!work)
		dprint("pc_mouse: controller jammed (0x%02X).\n", status);
	return status;
}
BYTE Handle_Kbd_Event(BYTE channel)
{
	BYTE status = PS2_PortN_Read_Status(channel);
	DWORD work = 10000;
	BYTE scancode;
	dprint("handle status %#x \n", status);
	while((--work > 0) && status & PS2_STR_IBF)
	{
		scancode = PS2_PortN_Read_Input(channel);
		if(!(status & (KBD_STAT_GTO | KBD_STAT_PERR)))
		{
			dprint("scan_code %d,%x \n", scancode, scancode);
		}
		else
		{
			dprint("mouse scan_code error \n");
		}
		status = PS2_PortN_Read_Status(channel);
	}
	if(!work)
		dprint("pc_mouse: controller jammed (0x%02X).\n", status);
	return status;
}
/*----------------------PS2 Interface Handle----------------------*/
//-----------------------------------------------------------------
// Send data from aux mouse
//-----------------------------------------------------------------
int Send_Aux_Data_To_Host(BYTE auxdata)
{
#if 0
	int PS2_Timeout = PS2_WaitTime;
	if(TP_ACK_CUNT == 0) // ps2 pure
	{
		if(IS_SET(KBC_STA, 1) || IS_SET(KBC_STA, 0))
		// if(IS_SET( KBC_STA,0))
		{
		#if KBC_DEBUG
			dprint("MS_Data_Suspend \n");
		#endif
			MS_Data_Suspend(auxdata);
			return 0x00;
		}
	}
	else
	{
		// while(IS_SET( KBC_STA,0));
		while(IS_SET(KBC_STA, 0) && (PS2_Timeout > 0))
		{
			PS2_Timeout--;
			if(PS2_PORT0_SR & 0x1)
			{
				if(TP_ACK_CUNT != 0)
				{
					MS_Data_Suspend(PS2_PORT0_IBUF);
				}
			}
		}
		if(PS2_Timeout == 0)
		{
			dprint("PS2 Timeout\n");
		}
	}
#endif
	KBC_STA &= 0x0f;
	SET_BIT(KBC_STA, KBC_SAOBF);//ps2数据标志位
	if(Host_Flag_INTR_AUX)
	{
		SET_BIT(KBC_CTL, KBC_OBFMIE);
	}
	else
	{
		CLEAR_BIT(KBC_CTL, KBC_OBFMIE);
	}
#if KBC_DEBUG
	dprint("adth:%x,icr:%x,isr:%x\n", auxdata, KBC_CTL, KBC_STA);
#endif
#if SUPPORT_8042DEBUG_OUTPUT
	Write_Debug_Data_To_Sram(auxdata);
#endif
	if(TP_ACK_CUNT != 0x0)
	{
		TP_ACK_CUNT--;
	}
	KBC_MOB = auxdata;
#if ENABLE_DEBUGGER_SUPPORT
	/* Debugger record */
	Debugger_KBC_PMC_Record(1, 0, auxdata);
#endif
	return 0x1;
}
void MS_Data_Suspend(BYTE nPending)
{
#if KBC_DEBUG
	dprint("MSPendingRXCount is %#x \n", MSPendingRXCount);
#endif
	if(MSPendingRXCount >= 4)
	{
		if(((MS_Main_CHN == 1) && ((SYSCTL_PIO2_CFG & (0x3 << 16)) > 0)) || ((MS_Main_CHN == 2) && ((SYSCTL_PIO2_CFG & (0x3 << 24)) > 0))) // 若触摸板接入，且对应引脚复用为时钟线
		{
			int KDI_Timeout = 6000;
			while(!(PS2_PortN_Read_Status(MS_Main_CHN - 1) & (0x10)))
			{
				KDI_Timeout--;
				if(KDI_Timeout == 0)
				{
					dprint("KDI timeout\n");
					return;
				}
			}
		}
		if(MS_Main_CHN == 1)
		{
			if(sysctl_iomux_ps2_0()==0x1)
			{
				GPIO1_DR1 &= 0xFE;	  // GPB8 输出低
				GPIO1_DDR1 |= 0x1;	  // GPB8 配置为GPIO输出模式
				sysctl_iomux_config(GPIOB, 8, 0); // GPB8 配置为GPIO模式
			}
			else if(sysctl_iomux_ps2_0()==0x2)
			{
				GPIO1_DR1 &= 0xFB;    //GPIOB10 输出低
				GPIO1_DDR1 |= 0x04;
				sysctl_iomux_config(GPIOB, 10, 0); // GPB10 配置为GPIO模式
			}
			else if(sysctl_iomux_ps2_0()==0x4)
			{
				GPIOB27(LOW);
				GPIO1_DDR3|=0x08;
				sysctl_iomux_config(GPIOB, 27, 0); // GPB27 配置为GPIO模式
			}
		}
		else if(MS_Main_CHN == 2)
		{
			if(sysctl_iomux_ps2_1()==0x10)
			{
				GPIO1_DR1 &= 0xEF;	  // GPB12 输出低
				GPIO1_DDR1 |= 0x10;	  // GPB12 配置为GPIO输出模式
				sysctl_iomux_config(GPIOB, 12, 0); // GPB12 配置为GPIO模式
			}
			else if(sysctl_iomux_ps2_1()==0x20)
			{
				GPIO1_DR1 &= 0xFB;    //GPIOB10 输出低
				GPIO1_DDR1 |= 0x04;
				sysctl_iomux_config(GPIOB, 10, 0); // GPB10 配置为GPIO模式
			}
		}
	}
	if(MSPendingRXCount >= 6)
	{
		dprint("ps2 data pending%d overflow\n", MSPendingRXCount);
		return;
	}
	MSDataPending[(MSPendingRXCount)] = nPending;
	MSPendingRXCount++;
}
BYTE Release_MS_Data_Suspend(void)
{
	BYTE buffer_data;
	buffer_data = MSDataPending[0];
	if(MSPendingRXCount <= 1)
	{
		// 在此处处理特殊情况
	}
	else
	{
		for(BYTE i = 0; i < MSPendingRXCount - 1; i++)
		{
			MSDataPending[i] = MSDataPending[i + 1];
		}
	}
	if(MSPendingRXCount != 0)
	{
		MSPendingRXCount--;
	}
#if KBC_DEBUG
	dprint("Release_MS_Data_Suspend is %#x \n", buffer_data);
#endif
	return buffer_data;
}
//-----------------------------------------------------------------
// Send data to ps2 interface
//-----------------------------------------------------------------
void Send_Data_To_PS2(BYTE PortNum, BYTE PortData)
{
#if KBC_DEBUG
	dprint("PortNum is %#x ,Send_Data_To_PS2 %#x,MS_Main_CHN is %#x\n", PortNum, PortData, MS_Main_CHN);
#endif
	if(PortNum == 0)
		PS2_PortN_Write_Output_W(PortData, PortNum);
	else if(PortNum == 1)
		PS2_PortN_Write_Output_W(PortData, PortNum);
}
/* ----------------------------------------------------------------------------
 * FUNCTION: Handle PS2 Mouse Command
 * ------------------------------------------------------------------------- */
void Send_Cmd_To_PS2_Mouse(BYTE PortNum)
{
	Command_A8();
	TP_ACK_CUNT = 0x00;
	MS_ID_Count = 0x00;
	if(AUX2ByteCommand)
	{
		if(MS_Resolution == 0xFF)
		{
			MS_Resolution = KBHIData;
		}
		else if(MS_Sampling_Rate == 0xFF)
		{
			MS_Sampling_Rate = KBHIData;
		}
	#if KBC_DEBUG
		dprint("AUX2ByteCommand \n");
	#endif
		Send_Data_To_PS2(PortNum, KBHIData);
		AUX2ByteCommand = 0;
		return;
	}
	switch(KBHIData)
	{
		case 0xE8: // Set Resolution (2 byte)
			MS_Resolution = 0xFF;
			AUX2ByteCommand = 1;
			break;
		case 0xF3: // Set Sampling Rate (2 byte)
			MS_Sampling_Rate = 0xFF;
			AUX2ByteCommand = 1;
			break;
		case 0xF4:
			EnableTP = 1;
			MS_Report_Flag = 1;
			break;
		case 0xF5:
			MS_Report_Flag = 0;
			break;
		case 0xE6:
			break;
		case 0xE7:
			break;
		case 0xEA:
			break;
		case 0xEC:
			break;
		case 0xEE:
			break;
		case 0xF0:
			break;
		case 0xF6:
			break;
		case 0xE9:
			break;
		case 0xF2:
			MS_ID_Count = 2;
			break;
		case 0xEB:
		case 0xFE:
			break;
		case 0xFF:
			MS_Report_Flag = 0;
			EnableTP = 0;
			// TP_ACK_CUNT = 0x03;		// ACK 3 bytes
			break;
		default: // Invalid command
			break;
	}
	if(KBHIData == 0xFF) // Reset command
	{
		// SetPS2CmdACKCounter(3);
		TP_ACK_CUNT = 0x3;
	}
	else if(KBHIData == 0xF2) // Read ID command
	{
		// SetPS2CmdACKCounter(2);
		TP_ACK_CUNT = 0x2;
	}
	else if(KBHIData == 0xE9) // Read status cmmand
	{
		// SetPS2CmdACKCounter(4);
		TP_ACK_CUNT = 0x4;
	}
	else // Other commands
	{
		// SetPS2CmdACKCounter(1);
		TP_ACK_CUNT = 0x1;
	}
	Send_Data_To_PS2(PortNum, KBHIData);
}
#define IRQC_INT_DEVICE_PS2         6
/* ----------------------------------------------------------------------------
 * FUNCTION: Service_Send_PS2
 * Send PS2 Mouse pending data
 * ------------------------------------------------------------------------- */
void Service_Send_PS2(void)
{
#if (Service_Send_PS2_START == 1)
	if(MSPendingRXCount > 0) // 如果有数据pending
	{
		BYTE PS2_Int_Record = 0;
		// ps2中断状态查询，若中断是开着的，则屏蔽后再打开，若是关着的，则保持关闭中断状态。
		if(MS_Main_CHN == 1)
		{
			if(read_csr(0xBD1) & 0x40)
			{
				PS2_Int_Record |= 0x1;
				irqc_disable_interrupt(IRQC_INT_DEVICE_PS2);
			}
		}
		else if(MS_Main_CHN == 2)
		{
		#if (defined(AE103))
			if(ICTL1_INTEN5 & 0x40)
			{
				PS2_Int_Record |= 0x2;
				ICTL1_INTEN5 &= (~(0x1 << 6));
			}
		#elif (defined(AE101) || defined(AE102))
			if(ICTL1_INTEN5 & INT1_PS2_1)
			{
				PS2_Int_Record |= 0x2;
				ICTL1_INTEN5 &= (~INT1_PS2_1);//失能PS2中断
			}
		#endif
		}
		if((TP_ACK_CUNT == 0) && (IS_SET(KBC_STA, 1)))
		{
			MSPendingRXCount = 0; // 在触摸板正常工作时，主机发下来个命令，接下来需要响应命令，清空Pending
			for(BYTE i = 0; i < 8; i++)
			{
				KBDataPending[i] = 0;
				MSDataPending[i] = 0;
			}
		}
		if(IS_SET(KBC_STA, KBC_OBF)) // 此时KBC OBF是1，则拉低PS2 CLK，抑制PS2数据发出
		{
		}
		else
		{
			if(MS_Main_CHN == 1)
			{
				sysctl_iomux_ps2_0(); // 配置为PS2 CLK线
			}
			else if(MS_Main_CHN == 2)
			{
				sysctl_iomux_ps2_1(); // 配置为PS2 CLK线
			}
			MSCmdAck = Release_MS_Data_Suspend();
			Send_Aux_Data_To_Host(MSCmdAck);
		}
		// 发完PS2数据后重新打开PS2中断
		if(PS2_Int_Record & 0x1)
		{
			PS2_Int_Record &= (~(0x1 << 0));
			irqc_enable_interrupt(IRQC_INT_DEVICE_PS2);
		}
		else if(PS2_Int_Record & 0x2)
		{
			PS2_Int_Record &= (~(0x1 << 1));
		#if (defined(AE103))
			ICTL1_INTEN5 |= (0x1 << 6);
		#elif (defined(AE101) || defined(AE102))
			ICTL1_INTEN5 |= INT1_PS2_1;
		#endif
		}
	}
	else
	{
		if(IS_MASK_CLEAR(KBC_STA, KBC_STA_OBF))
		{
			MSPendingRXCount = 0;
			for(BYTE i = 0; i < 8; i++)
			{
				KBDataPending[i] = 0;
				MSDataPending[i] = 0;
			}
			if(MS_Main_CHN == 1)
			{
				sysctl_iomux_ps2_0(); //  配置回PS2 CLK线
			}
			else if(MS_Main_CHN == 2)
			{
				sysctl_iomux_ps2_1(); // 配置回PS2 CLK线
			}
		}
	}
#endif
}

BYTE PS2_PinSelect(void)
{
	BYTE PinSelect = 0;
	PinSelect|=sysctl_iomux_ps2_0();
	PinSelect|=sysctl_iomux_ps2_1();
	switch(PinSelect)
	{
		case 0x1:
		 	return (IS_GPIOB8(HIGH) && IS_GPIOB9(HIGH));
		case 0x2:
			return (IS_GPIOB10(HIGH) && IS_GPIOB11(HIGH));
		case 0x4:
			return (IS_GPIOB27(HIGH) && IS_GPIOB28(HIGH));
		case 0x10:
			return (IS_GPIOB12(HIGH) && IS_GPIOB13(HIGH));
		case 0x11:
			return ((IS_GPIOB8(HIGH) && IS_GPIOB9(HIGH)) || (IS_GPIOB12(HIGH) && IS_GPIOB13(HIGH)));
		case 0x12:
			return ((IS_GPIOB10(HIGH) && IS_GPIOB11(HIGH)) || (IS_GPIOB12(HIGH) && IS_GPIOB13(HIGH)));
		case 0x14:
			return ((IS_GPIOB27(HIGH) && IS_GPIOB28(HIGH)) || (IS_GPIOB12(HIGH) && IS_GPIOB13(HIGH)));
		case 0x20:
			return (IS_GPIOB10(HIGH) && IS_GPIOB11(HIGH));
		case 0x21:
			return ((IS_GPIOB8(HIGH) && IS_GPIOB9(HIGH)) || (IS_GPIOB10(HIGH) && IS_GPIOB11(HIGH)));
		case 0x24:
			return ((IS_GPIOB27(HIGH) && IS_GPIOB28(HIGH)) || (IS_GPIOB10(HIGH) && IS_GPIOB11(HIGH)));
		default:
			return 0;
	}
}

void InitAndIdentifyPS2(void)
{
	static WORD _10MS_Cunt = 0;
	static DWORD Temp_SYSCTLPIO2 = 0;
	static BYTE Temp_flag = 0;
	if(MS_Main_CHN == 0 && KB_Main_CHN == 0)
	{
		if(Temp_flag==0)
		{
			Temp_SYSCTLPIO2 = SYSCTL_PIO2_CFG;
			Temp_flag = 1;
		}
		if(PS2_PinSelect())
		{
			_10MS_Cunt++;
			if(_10MS_Cunt >= 10)
			{
				//PS2 RESET
				SYSCTL_RST0 |= (PS2M_RST);
				SYSCTL_RST1 |= PS2K_RST;
				SYSCTL_RST0 &= ~(PS2M_RST);
				SYSCTL_RST1 &= ~(PS2K_RST);
				//PS2 INIT
				ps2_init();
				PS2_DevScan();
				Write_KCCB(Host_Flag);
				if(MS_Main_CHN == 0)
				{
					SYSCTL_PIO2_CFG = Temp_SYSCTLPIO2;
					switch(KB_Main_CHN)
					{
						case 1:
							sysctl_iomux_ps2_0();
							break;
						case 2:
							sysctl_iomux_ps2_1();
							break;
						default:
							break;
					}
				}
				else if(KB_Main_CHN == 0)
				{
					SYSCTL_PIO2_CFG = Temp_SYSCTLPIO2;
					switch(MS_Main_CHN)
					{
						case 1:
							sysctl_iomux_ps2_0();
							break;
						case 2:
							sysctl_iomux_ps2_1();
							break;
						default:
							break;
					}
				}
				_10MS_Cunt = 0;
			}
		}
		else
		{
			_10MS_Cunt = 0;
		}
	}
}
/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/
