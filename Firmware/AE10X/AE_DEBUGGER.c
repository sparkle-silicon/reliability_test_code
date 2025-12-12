/*
 * @Author: Linyu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-11-15 18:21:14
 * @Description:
 *
 *
 * @LastEditors: daweslinyu
 * @LastEditTime: 2023-09-18 16:57:45
 * @Description:
 *
 *
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 以下是中文及英文版权同步声明，编码为UTF-8。
 * Copyright has legal effects and violations will be prosecuted.
 * 版权具有法律效力，违反必究。
 *
 * Copyright ©2021-2025 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2025龙晶石半导体科技（苏州）有限公司
 */
#include "AE_DEBUGGER.H"
#include "KERNEL_I2C.H"
#include "KERNEL_KBS.H"
#include "KERNEL_GPIO.H"
#include "CUSTOM_POWER.H"
#if ENABLE_DEBUGGER_SUPPORT
 //*****************************************************************************
 //
 // global variables for Debugger api
 //
 //*****************************************************************************
char Debugger_Data[DEBUGGER_BUFF_SIZE] = { 0 }; // An array of data transferred by the debugger
WORD Intr_num[DEBUGGER_BUFF_SIZE] = { 0 };	   // Interrupts count array
char Debugger_Cmd[DEBUGGER_BUFF_SIZE] = { 0 };  // An array of data transferred by the debugger
short buf[7];				   // Used to record register addresses and values when reading or writing registers
static struct KBD_Event kbd_event = { {0}, 0, 0 };
char KBC_PMC_Event[DEBUGGER_BUFF_SIZE] = { 0 }; // An array of records for KBC/PMC data
unsigned char iicFeedback = 0, iic_flag = 0, iic_int_flag = 0;
WORD iic_count, pos = 0;
/*The debugger scans interface variables*/
const char Handshake_data[] = { 0x99,0x99,0x99,0x99 }; // Debugger handshake data
const char Handshake_length = sizeof(Handshake_data);
const char Version_data[] = { 0x5A,'E','C','1','0','1' }; //0x5A and version data
const char Version_length = sizeof(Version_data);
BYTE  Handshake_cunt = 0;
static char Handshake_Flag = 0;
char Uart_buffer[UART_BUFFER_SIZE] = { 0 }; // An array of data transferred by the debugger
//*****************************************************************************
//
// local variables for Debugger api
//
//*****************************************************************************
BYTE Debuger_Cmd_Data = 0x0;		 // Debugger CMD data
char KBC_PMC_Pending[3][DEBUGGER_BUFF_SIZE] = { 0 };	 // KBC/PMC record value Pending array
int KBC_PMC_PendingRXCount = 0;		 // KBC/PMC Pending in index
int KBC_PMC_PendingTXCount = 0;		 // KBC/PMC Pending out index
BYTE dbg_int_buf[3];				 // Debugger interrupt function instruction array
/*------------Only the slave debugger function is available-------------*/
void Service_Debugger(void)
{
	if (F_Service_Debugger)
	{
		Debug_Timeout_Count = 5000;
		I2C0_INTR_MASK &= ~(0x1 << 2); // 屏蔽接收中断
		// Uart_Int_Disable(UARTA_CHANNEL, 0);
		while (F_Service_Debugger_Cmd != F_Service_Debugger_Cnt)
			Deubgger_Cmd_Parsing(Debugger_Cmd[F_Service_Debugger_Cnt++]);
		F_Service_Debugger = 0;
		// Uart_Int_Enable(UARTA_CHANNEL, 0);
		I2C0_INTR_MASK |= (0x1 << 2);
	}
	else
	{
		Debug_Timeout_Count--;
		if (Debug_Timeout_Count == 0)
		{
			Debug_Timeout_Count = 5000;
			Buf_flag = 0;
			Debug_Temp = 0;
			F_Service_Debugger_Cmd = F_Service_Debugger_Cnt = 0;
		}
	}
	// if (F_Service_Debugger_Send)
	// {
	// 	Debugger_I2c_Send(DEBUGGER_I2C_CHANNEL);
	// 	assert_print("iicFeedback %#x\n", iicFeedback);
	// 	F_Service_Debugger_Send = 0;
	// }
	if (F_Service_Debugger_Rrq)
	{
		Debugger_I2c_Req(DEBUGGER_I2C_CHANNEL);
		assert_print("iicFeedback %#x\n", iicFeedback);
		F_Service_Debugger_Rrq = 0;
		switch (DEBUGGER_I2C_CHANNEL) // 打开RD_REQ中断
		{
			case I2C_CHANNEL_0:
				I2C0_INTR_MASK |= 0x20;
			case I2C_CHANNEL_1:
				I2C1_INTR_MASK |= 0x20;
			case I2C_CHANNEL_2:
				I2C2_INTR_MASK |= 0x20;
			case I2C_CHANNEL_3:
				I2C3_INTR_MASK |= 0x20;
			case I2C_CHANNEL_4:
				I2C4_INTR_MASK |= 0x20;
			case I2C_CHANNEL_5:
				I2C5_INTR_MASK |= 0x20;
		}
	}
}
//*****************************************************************************
//
//  The Debugger KBC PMC DataPending
//
//  parameter :
//      KBC_PMC_PendingData : Data Point Need to Pending
//
//  return :
//      none
//
//	note :
//		This function will put the full array into the pending array
//		and then empty the array for further data storage.
//		Therefore, the data in the pending array should be printed first in order
//*****************************************************************************
void KBC_PMC_DataPending(char *KBC_PMC_PendingData)
{
	if (KBC_PMC_PendingRXCount >= KBC_PMC_PendingTXCount)
	{
		if (((KBC_PMC_PendingRXCount - KBC_PMC_PendingTXCount) >= 2) && (KBC_PMC_Pending_Num >= 256))
		{
			dprint("KBC_PMC_Pending error\n");
			return;
		}
	}
	else
	{
		if (((KBC_PMC_PendingTXCount - KBC_PMC_PendingRXCount) == 1) && (KBC_PMC_Pending_Num >= 256))
		{
			dprint("KBC_PMC_Pending error\n");
			return;
		}
	}

	KBC_PMC_Pending[KBC_PMC_PendingRXCount][KBC_PMC_Pending_Num] = KBC_PMC_PendingData[0]; // record direction and channel
	KBC_PMC_Pending_Num++;
	if (KBC_PMC_PendingRXCount >= KBC_PMC_PendingTXCount)
	{
		if (((KBC_PMC_PendingRXCount - KBC_PMC_PendingTXCount) >= 2) && (KBC_PMC_Pending_Num >= 256))
		{
			dprint("KBC_PMC_Pending error\n");
			return;
		}
	}
	else
	{
		if (((KBC_PMC_PendingTXCount - KBC_PMC_PendingRXCount) == 1) && (KBC_PMC_Pending_Num >= 256))
		{
			dprint("KBC_PMC_Pending error\n");
			return;
		}
	}
	KBC_PMC_Pending[KBC_PMC_PendingRXCount][KBC_PMC_Pending_Num] = KBC_PMC_PendingData[1]; // record value
	KBC_PMC_Pending_Num++;
	if (KBC_PMC_PendingRXCount >= KBC_PMC_PendingTXCount)
	{
		if (((KBC_PMC_PendingRXCount - KBC_PMC_PendingTXCount) >= 2) && (KBC_PMC_Pending_Num >= 256))
		{
			dprint("KBC_PMC_Pending error\n");
			return;
		}
	}
	else
	{
		if (((KBC_PMC_PendingTXCount - KBC_PMC_PendingRXCount) == 1) && (KBC_PMC_Pending_Num >= 256))
		{
			dprint("KBC_PMC_Pending error\n");
			return;
		}
	}

	if (KBC_PMC_Pending_Num >= 256)
	{
		KBC_PMC_PendingRXCount = (KBC_PMC_PendingRXCount + 1) % 3;
		KBC_PMC_Pending_Num = 0;
	}

}
//*****************************************************************************
//
//  KBC PMC Log Pending Out Function
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void GetKBC_PMC_PendingData(void)
{
	if (KBC_PMC_Event_Num != 0)
	{
		if (KBC_PMC_Event_Num >= PMC_KBC_SendCnt)
		{
			buf[3] = PMC_KBC_SendCnt - 1;
			KBC_PMC_Event_Num = KBC_PMC_Event_Num - PMC_KBC_SendCnt;
		}
		else
		{
			buf[3] = KBC_PMC_Event_Num - 1;
			KBC_PMC_Event_Num = 0;
		}
		for (short i = 0; i < (buf[3] + 1); i++)
		{
			Debugger_Data[i] = KBC_PMC_Event[i]; // get value
		}
		if (KBC_PMC_Event_Num != 0)
		{
			for (short i = 0; i < KBC_PMC_Event_Num; i++)
			{
				KBC_PMC_Event[i] = KBC_PMC_Event[i + (buf[3] + 1)];
			}
		}
		else
		{
			if (KBC_PMC_PendingRXCount != KBC_PMC_PendingTXCount)
			{
				for (short i = 0; i < 256; i++)
				{
					KBC_PMC_Event[i] = KBC_PMC_Pending[KBC_PMC_PendingTXCount][i]; // Dumps the currently pending array to the output array
				}
				KBC_PMC_Event_Num = 256;
				KBC_PMC_PendingTXCount = (KBC_PMC_PendingTXCount + 1) % 3;
			}
			else
			{
				for (short i = 0; i < KBC_PMC_Pending_Num; i++)
				{
					KBC_PMC_Event[i] = KBC_PMC_Pending[KBC_PMC_PendingTXCount][i]; // Dumps the currently pending array to the output array
				}
				KBC_PMC_Event_Num = KBC_PMC_Pending_Num;
				KBC_PMC_Pending_Num = 0;
			}
		}
	}
}
//*****************************************************************************
//
//  The Debugger Function for Record KBD Value
//
//  parameter :
//      value : value need to be record
//
//  return :
//      Function running condition
//
//*****************************************************************************
BYTE Debugger_KBD_Record(BYTE value)
{
	if (F_Service_KBL)
	{
		if (KBD_Overflag == 1)
		{
			dprint("kbd buffer is overflow!");
			return 1;
		}
		// dprint("kdb_event.tail: %x\n", kbd_event.tail);
		kbd_event.data[kbd_event.tail++] = value;
		// dprint("kdb_event write: %x\n", kbd_event.data[kbd_event.tail - 1]);
		if (kbd_event.tail == KBD_BUFF_SIZE)
		{
			kbd_event.tail = 0;
		}
		if (kbd_event.tail == kbd_event.head)
		{

			KBD_Overflag = 1;
			return 1;
		}
	}

	return 0;
}
//*****************************************************************************
//
//  The Debugger Function  Entrance
//
//  parameter :
//
//  return :
//      none
//
//	note :
//		Buf_flag to make sure after default not get in this if function
//		0x55--read request
//		0xff--write request
//		0x88--Cancel set to slave so can get Debug_Temp
//		0xDD--Set to slave so can read data
//		0xAA--KBD read request
//		0xBB--cancel KBD read request
//		0xCC--Record KBC PMC data
//		0xE0--Reset chip
//		0xB0--Switching keyboard Codes
//		0x75--Firmware update
//		0x11--Interrupt function block
//****************************************************************************
void Deubgger_Cmd_Parsing(BYTE data)
{
	if ((!Buf_flag) && (data == READ_REQUEST || data == WRITE_REQUEST || data == GET_DEBUG_TEMP ||
		data == KBD_READ_REQUEST || data == CANCEL_KBD_READ_REQUEST || data == RECOARD_KBC_PMC_DATA ||
		data == STOP_RECOARD_KBC_PMC_DATA || data == RESET_CHIP_USE_WATCHDOG || data == SLAVE_READ_DATA ||
		data == SWITCH_KEYBOARD_CODE || data == FIRMWARE_UPDATE || data == INTERRUPT_FUNCTION || data == HANDSHAKE_REQUEST))
	{
	#if DEBUGGER_DEBUG
		assert_print("cmd:%#x", data);
	#endif
		switch (data)
		{
			case GET_DEBUG_TEMP:
				Slave_flag = 0;
				if (DEBUGGER_OUTPUT_SWITCH == 1)
					I2C0_INTR_MASK &= ~(1 << 2);
				break;
			case SLAVE_READ_DATA:
				Slave_flag = 1;
				break;
			case KBD_READ_REQUEST:
			#if DEBUGGER_DEBUG
				assert_print();
			#endif
				F_Service_KBL = 1;
				KBD_Overflag = 0;
				break;
			case CANCEL_KBD_READ_REQUEST:
			#if DEBUGGER_DEBUG
				assert_print();
			#endif
				F_Service_KBL = 0;
				if (DEBUGGER_OUTPUT_SWITCH == 1)
				{
					iicFeedback = 0;
					pos = 0;
				}
				break;
			case RECOARD_KBC_PMC_DATA:
			#if DEBUGGER_DEBUG
				assert_print();
			#endif
				KBC_PMC_Flag = 1;
				for (int i = 0; i < DEBUGGER_BUFF_SIZE; i++)
					KBC_PMC_Event[i] = 0; // clear
				KBC_PMC_Event_Num = 0;
				KBC_PMC_Pending_Num = 0;
				break;
			case STOP_RECOARD_KBC_PMC_DATA:
			#if DEBUGGER_DEBUG
				assert_print();
			#endif
				KBC_PMC_Flag = 0;
				break;
			case RESET_CHIP_USE_WATCHDOG: // reset chip
			#if DEBUGGER_DEBUG
				assert_print();
			#endif
				WDT_Init(0, 0);
				break;
			case SWITCH_KEYBOARD_CODE: // change keyboard code type
			#if DEBUGGER_DEBUG
				assert_print();
			#endif
				Host_Flag_XLATE_PC = !Host_Flag_XLATE_PC;
				break;
			case FIRMWARE_UPDATE:
				update_reg_ptr = (VDWORD)DEBUGGER_UART;
				while (Debugger_Cmd[F_Service_Debugger_Cmd - 1] != '\n')//0xa是换行符的十六进制
				{
					if (Debugger_Cmd[F_Service_Debugger_Cmd - 1] == '-')
						if (Debugger_Cmd[F_Service_Debugger_Cmd] == 'c' || Debugger_Cmd[F_Service_Debugger_Cmd] == 'C')
						{
							uart_updata_flag |= 2;
						}
					if (Debugger_Cmd[F_Service_Debugger_Cmd - 1] == (char)0xEE)
					{
						F_Service_Debugger_Cmd = F_Service_Debugger_Cnt = 0;
						return;
					}
				}
				F_Service_Debugger_Cmd = F_Service_Debugger_Cnt = 0;
				UPDATE_IER = 0; // 关闭中断
				uart_updata_flag |= 0x1;
				Flash_Update_Function();
			case HANDSHAKE_REQUEST:
				Buf_num = 0;
				Buf_flag = 1;
				Debug_Temp = data;
				Debug_Timeout_Count = 0xffffffff;
				break;
			default:
			#if DEBUGGER_DEBUG
				assert_print();
			#endif
				int_buf_index = 0;
				Buf_num = 0;
				Buf_flag = 1;
				Debug_Temp = data;
				return;
		}
		if (data != HANDSHAKE_REQUEST)
			DEBUGER_putchar(data);
	}
	if (Buf_flag)
	{
	#if DEBUGGER_DEBUG
		assert_print("data:0x%x", data);
	#endif
		switch (Debug_Temp)
		{
			case READ_REQUEST:
			#if DEBUGGER_DEBUG
				assert_print();
			#endif
				buf[Buf_num] = data; // store received addr
				Buf_num++;
				if (Buf_num == 4) // received addr finished
				{
					Buf_flag = 0; // clear flag
					Debug_Temp = 0;
				#if DEBUGGER_DEBUG
					assert_print("buf[3]:%d", buf[3]);
				#endif
					DEBUGGER_DATA(); // enable DEBUGGER_DATA function in main loop
				}
				break;
			case WRITE_REQUEST:
			#if DEBUGGER_DEBUG
				assert_print("Write_Request");
			#endif
				buf[Buf_num] = data;
				Buf_num++;
				if (Buf_num == 7)
				{
					Buf_flag = 0;
					Debug_Temp = 0;
					DEBUGGER_Change(); // enable DEBUGGER_Change function in main loop
				}
				break;
			case INTERRUPT_FUNCTION:
			#if DEBUGGER_DEBUG
				assert_print();
			#endif
				dbg_int_buf[int_buf_index] = data;
				int_buf_index++;
				if (int_buf_index == 3)
				{
					int_buf_index = 0;
					Buf_flag = 0; // clear flag
					Debug_Temp = 0;
					DEBUGGER_Int(); // enable DEBUGGER_Int function in main loop
				}
				break;
			case HANDSHAKE_REQUEST:
			#if DEBUGGER_DEBUG
				assert_print();
			#endif
				if (Debugger_Handshake(data))// handshake success
				{
					Buf_flag = 0; // clear flag
					Debug_Temp = 0;
				}
				else
				{
				}
				break;
			default:
				break;
		}
	}
}
void Debugger_Cmd_IRQ(BYTE debug_data)
{
	if (F_Service_Debugger_Cmd == F_Service_Debugger_Cnt) // 代表结束
	{
		F_Service_Debugger_Cmd = F_Service_Debugger_Cnt = 0;
	}
	if (F_Service_Debugger_Cmd >= 0xff)
	{
	#if DEBUGGER_DEBUG
		assert_print("warring %#x", F_Service_Debugger_Cmd);
	#endif
		return;
	}
	else
	{
	#if DEBUGGER_DEBUG
		dprint("F_Service_Debugger_Cmd:0x%x data:0x%x\n", F_Service_Debugger_Cmd, debug_data);
	#endif
		Debugger_Cmd[F_Service_Debugger_Cmd++] = debug_data;
		F_Service_Debugger = 1;
		if (DEBUGGER_OUTPUT_SWITCH == 1)
			I2C0_INTR_MASK &= ~(0x1 << 2); // 屏蔽接收中断
	}
}
//*****************************************************************************
//
//  The Debugger Function I2C Request
//
//  parameter :
//      channel : I2C channel
//
//  return :
//      none
//
//*****************************************************************************
void Debugger_I2c_Req(WORD channel)
{
	// int data;
	// char wdata[16];
	Slave_flag = 1; // set slave
	if (iic_flag == 1)
	{ // if the feedback datas have been sent
		// #if DEBUGGER_DEBUG
		//assert_print();
		//#endif

		volatile uint16_t int_status = SMBUSn_INTR_MASK0(channel);
		int_status |= I2C_INTR_TX_EMPTY;
		int_status &= (~I2C_INTR_RD_REQ);
		// enable the tx_empty irqc and mask the req irqc to star transmit datas
		SMBUSn_INTR_MASK0(channel) = int_status;
		iic_flag = 0;
	}
	else
	{
		// assert_print("iicFeedback %#x", iicFeedback);
		switch (iicFeedback)
		{
			case 0:
				//  return "fe dd" mean the master read while slave does not recive any command
				I2C_Slave_Write_Byte(0xFE, channel);
				I2C_Slave_Write_Byte(0xDD, channel);
				return;
			case 1:
				{
					// return two byte data to indicate master the "255 +1" datas need to be read
					I2C_Slave_Write_Byte(0x01, channel);
					I2C_Slave_Write_Byte(buf[3], channel);
					iic_count = Num_flag = 0; // clear the count num// the number of datas have send by iic
				}
				break;
			case 2:// int mode 
			case 3:
				{
					// return two byte data to indicate master the "0 +1" datas need to be read
					I2C_Slave_Write_Byte(0x00, channel);
					I2C_Slave_Write_Byte(0x01, channel);
				}
				break;
			case 4:
				{					   // kbc send
					F_Service_KBL = 0; // stop the kbd record until the datas have been sent
					I2C_Slave_Write_Byte(0x00, channel);
					I2C_Slave_Write_Byte(pos, channel);
					iic_count = Num_flag = 0; // clear the count num// the number of datas have send by iic
				}
				break;
			default:
				return;
		}
		iic_flag = 1;

	}
}
//*****************************************************************************
//
//  The Debugger Function Data Send Function
//  When I2C almost empty interrupts
//  Fill data in FIFO to send from I2C
//
//  parameter :
//      channel : I2C channel
//
//  return :
//      none
//
//*****************************************************************************
void Debugger_I2c_Send(WORD channel)
{
	// assert_print("iicFeedback %#x", iicFeedback);
	// int data;
	uint8_t wdata[16];
	volatile BYTE int_status;
	switch (iicFeedback)
	{
		case 0:
			break;
		case 1:
			{
				for (int i = 0; i < 8; i++)
				{
					wdata[i] = Debugger_Data[i + Num_flag * 8];
					iic_count++;
					if (iic_count == buf[3] + 1)
					{
						char temp = iic_count % 8; // 计算剩余待发送字节
						if (temp == 0)
						{
							if (Num_flag == 31)
								break;
							temp = 8;
						}
						I2C_Slave_Write_Block(wdata, temp, channel);
						int_status = SMBUSn_INTR_MASK0(channel);
						int_status |= I2C_INTR_RD_REQ;
						int_status &= (~I2C_INTR_TX_EMPTY);
						// enable the tx_empty irqc and mask the req irqc to star transmit datas
						SMBUSn_INTR_MASK0(channel) = int_status;
						iicFeedback = 0;
						return;
					}
				}
				Num_flag++;
				I2C_Slave_Write_Block(wdata, 8, channel); // i2c slave send data to fifo
				/* Finish data send*/
				if (Num_flag == 32)
				{
					int_status = SMBUSn_INTR_MASK0(channel);
					int_status |= I2C_INTR_RD_REQ;
					int_status &= (~I2C_INTR_TX_EMPTY);
					// enable the tx_empty irqc and mask the req irqc to star transmit datas
					SMBUSn_INTR_MASK0(channel) = int_status;
					iicFeedback = 0;
				}
			}
			break;
		case 2:
			{ // int mode
				// the real data master want to read
				I2C_Slave_Write_Byte(0xFF, channel);
				int_status = SMBUSn_INTR_MASK0(channel);
				int_status |= I2C_INTR_RD_REQ;
				int_status &= (~I2C_INTR_TX_EMPTY);
				// enable the tx_empty irqc and mask the req irqc to star transmit datas
				SMBUSn_INTR_MASK0(channel) = int_status;
				iicFeedback = 0;
			}
			break;
		case 3:
			{
				if (iic_int_flag == 1)
				{
					I2C_Slave_Write_Byte(0x01, channel);
				}
				else if (iic_int_flag == 2)
				{
					I2C_Slave_Write_Byte(0x00, channel);
				}
				else if (iic_int_flag == 3)
				{
					I2C_Slave_Write_Byte(0x11, channel);
				}
				else
				{
					I2C_Slave_Write_Byte(0xDD, channel);
				}
				int_status = SMBUSn_INTR_MASK0(channel);
				int_status |= I2C_INTR_RD_REQ;
				int_status &= (~I2C_INTR_TX_EMPTY);
				// enable the tx_empty irqc and mask the req irqc to star transmit datas
				SMBUSn_INTR_MASK0(channel) = int_status;
				iicFeedback = 0;
			}
			break;
		case 4:
			{ // kbc send
				for (int i = 0; i < 16; i++)
				{
					wdata[i] = Debugger_Data[i + Num_flag * 16];
					iic_count++;
					if (iic_count == pos)
					{
						char temp = iic_count % 16; // 计算剩余待发送字节
						if (temp == 0)
						{
							if (Num_flag == 15)
								break;
							temp = 16;
						}
						I2C_Slave_Write_Block(wdata, temp, channel);
						/* Complete sending; Restore the Context*/
						F_Service_KBL = 1;
						iicFeedback = 0;
						pos = 0;
						int_status = SMBUSn_INTR_MASK0(channel);
						int_status |= I2C_INTR_RD_REQ;
						int_status &= (~I2C_INTR_TX_EMPTY);
						// enable the tx_empty irqc and mask the req irqc to star transmit datas
						SMBUSn_INTR_MASK0(channel) = int_status;					return;
					}
				}
				Num_flag++;
				I2C_Slave_Write_Block(wdata, 16, channel); // i2c slave send data to fifo
				/* Complete sending; Restore the Context*/
				if (Num_flag == 16)
				{
					F_Service_KBL = 1;
					iicFeedback = 0;
					pos = 0;
					int_status = SMBUSn_INTR_MASK0(channel);
					int_status |= I2C_INTR_RD_REQ;
					int_status &= (~I2C_INTR_TX_EMPTY);
					// enable the tx_empty irqc and mask the req irqc to star transmit datas
					SMBUSn_INTR_MASK0(channel) = int_status;
				}
			}
			break;
		default:
			return;
	}
}
//*****************************************************************************
//
//  The Debugger Function for Data Read
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void DEBUGGER_DATA(void) // Debugger Function read
{
	/* Declaration of variables */
	int data_base;
	int length = 1;
	// char wdata[16]; // data send to i2c
	uint8_t temp1; // For data lenth convert
	uint16_t temp2;
	uint32_t temp4;
	/* Addr sequence transformation */
	data_base = (buf[0] << 16) | (buf[1] << 8) | buf[2];
#if DEBUGGER_DEBUG
	assert_print("data_base:%#x", data_base);
#endif
	/* Register length conversion */
	if (data_base >= 0x30000 && data_base <= 0x30FFF) // Different addr part has different lenth
	{
		length = 4;
	}
	else if (data_base >= 0x4800 && data_base <= 0x4FFF) // register lenth change
		// else if(data_base >= 0x4000 && data_base <= 0x7BFF)
	{
		length = 2;
	}
	else
	{
		length = 1; // lyx debug --spi need to read as 1 byte
	}
#if DEBUGGER_DEBUG
	assert_print("length:%#x", length);
#endif
	/* Cumulative number of interrupts */
	if (data_base == 0x10000) // this addr will get int num
	{
		for (int i = 0; i < DEBUGGER_BUFF_SIZE; i++)
		{
			Debugger_Data[i] = Intr_num[i];
		}
	}
	/* KBD key code */
	else if (data_base == 0x10001)
	{
		/*
		 The command to read the keyboard code in the debugger,
		 because the web end polling tends to cause the interrupt nesting problem,
		 so the case is discarded and replaced with the active send.
		*/
		/* Service_Debugger_Send_KBD */
	}
	/* KBC/PMC value */
	else if (data_base == 0x10002) // this addr will get kbc/pmc value
	{
		if (((KBC_PMC_PendingRXCount != KBC_PMC_PendingTXCount)) || (KBC_PMC_Pending_Num > 0)) // get pending data if has
		{
			GetKBC_PMC_PendingData();
		}
		else // pending no data
		{
			if (KBC_PMC_Event_Num == 0)//如果没有捕捉到PMC/KBC数据则回复两个字节0xff
			{
				buf[3] = 1;
				Debugger_Data[0] = 0xff;
				Debugger_Data[1] = 0xff;
			}
			else
			{
				if (KBC_PMC_Event_Num >= PMC_KBC_SendCnt)
				{
					buf[3] = PMC_KBC_SendCnt - 1;
					KBC_PMC_Event_Num = KBC_PMC_Event_Num - PMC_KBC_SendCnt;
				}
				else
				{
					buf[3] = KBC_PMC_Event_Num - 1;
					KBC_PMC_Event_Num = 0;
				}
				for (short i = 0; i < (buf[3] + 1); i++)
				{
					Debugger_Data[i] = KBC_PMC_Event[i]; // get value
				}
				if (KBC_PMC_Event_Num != 0)
				{
					for (short i = 0; i < KBC_PMC_Event_Num; i++)
					{
						KBC_PMC_Event[i] = KBC_PMC_Event[i + (buf[3] + 1)];
					}
				}
			}
		}
	#if DEBUGGER_DEBUG
		printf("PendingRXCount:0x%x PendingTXCount:0x%x Pending_Num:0x%x Event_Num:0x%x\n", KBC_PMC_PendingRXCount, KBC_PMC_PendingTXCount, KBC_PMC_Pending_Num, KBC_PMC_Event_Num);
	#endif
	}
	/* read value form addr */
	else
	{
		switch (length)
		{
			case 1:
				for (int i = 0; i < 256; i++)
				{
					temp1 = (*((uint8_t *)(data_base)));
					Debugger_Data[i] = temp1 & 0xff;
					data_base++;
				}
				break;
			case 2:
				for (int i = 0; i < 128; i++)
				{
					temp2 = (*((uint16_t *)(data_base)));
					Debugger_Data[i * 2] = temp2 & 0xff;
					Debugger_Data[i * 2 + 1] = (temp2 >> 8) & 0xff;
					data_base += 2;
				}
				break;
			case 4:
				for (int i = 0; i < 64; i++)
				{
					temp4 = (*((uint32_t *)(data_base)));
					Debugger_Data[i * 4] = temp4 & 0xff;
					Debugger_Data[i * 4 + 1] = (temp4 >> 8) & 0xff;
					Debugger_Data[i * 4 + 2] = (temp4 >> 16) & 0xff;
					Debugger_Data[i * 4 + 3] = (temp4 >> 24) & 0xff;
					data_base += 4;
				}
				break;
			default:
				for (int i = 0; i < DEBUGGER_BUFF_SIZE; i++)
				{
					Debugger_Data[i] = i;
				}
				break;
		}
	}
#if (DEBUGGER_OUTPUT_SWITCH == 1)
	iicFeedback = 1; // enable the iic feedback
#else
	/*Output from UART */
	for (int i = 0; i < buf[3] + 1; i++)
	{
		DEBUGER_putchar(Debugger_Data[i]);
	}
#endif
}
void Service_Debugger_Send_KBD(void)
{
	if (!F_Service_KBL)
		return;
	if (kbd_event.head == kbd_event.tail)//没有数据
		return;
	pos = 0;
	//assert_print("iicFeedback 0x%x", iicFeedback);
	while ((((kbd_event.head) % KBD_BUFF_SIZE) != kbd_event.tail))
	{
		iicFeedback = 0; // disable the iic feedback
		Debugger_Data[pos++] = kbd_event.data[kbd_event.head++];
		if (kbd_event.head == KBD_BUFF_SIZE)
		{
			kbd_event.head = 0;
		}
		iicFeedback = 4; // enable the kbc send
		//assert_print();
	}
	KBD_Overflag = 0;
	//assert_print("iicFeedback 0x%x", iicFeedback);
#if (DEBUGGER_OUTPUT_SWITCH == 0)
	for (int i = 0; i < pos; i++)
	{
		DEBUGER_putchar(Debugger_Data[i]);
	}
#endif
}
//*****************************************************************************
//
//  The Debugger Function for Data Write
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void DEBUGGER_Change(void)
{
	/* Declaration of variables */
	int data_base;
	// int channel = 0x3800;
	int length = 0;
	/* Addr sequence transformation */
	data_base = (buf[0] << 16) | (buf[1] << 8) | buf[2];
	/* Register write protection */
	if (data_base >= 0x32000 || (data_base >= 0x30C00 && data_base <= 0x30FFF) ||
		(data_base >= 0x28000 && data_base <= 0x2FFFF) || (data_base >= 0x7C00 && data_base <= 0x1FFFF)
		|| (data_base >= 0x4400 && data_base <= 0x47FF) || data_base <= 0xFFF)
	{
		DEBUGER_putchar(0xee);
		return;
	}
	/* Register length conversion */
	if (data_base >= 0x30000 && data_base <= 0x30FFF)
	{
		length = 4;
	}
	else if (data_base >= 0x32000 && data_base <= 0xBFFFF)
	{
		length = 4;
	}
	else if (data_base >= 0x20000 && data_base <= 0x27FFF)//配合web改为四字节读写
	{
		length = 4;
	}
	else if (data_base >= 0x4800 && data_base <= 0x4FFF)
	{
		length = 2;
	}
	else
	{
		length = 1;
	}
#if DEBUGGER_DEBUG
	assert_print("data_base:0x%x  length:0x%x", data_base, length);
#endif
	/* write value to addr */
	switch (length)
	{
		case 1:
		#if DEBUGGER_DEBUG
			printf("vaule:0x%x\n", (buf[6]));
		#endif
			*((uint8_t *)(data_base)) = buf[6];
			break;
		case 2:
		#if DEBUGGER_DEBUG
			printf("vaule:0x%x\n", (buf[6] | (buf[5] << 8)));
		#endif
			if (data_base % 2 != 0)//2字节访问防止访问到奇数地址
			{
				*((uint16_t *)(data_base - 1)) = ((*((uint16_t *)(data_base - 1)) & 0xff) | (buf[6] << 8));
			}
			else
			{
				*((uint16_t *)(data_base)) = (buf[6] | (buf[5] << 8));
			}
			break;
		case 4:
		#if DEBUGGER_DEBUG
			printf("vaule:0x%x\n", (buf[6] | (buf[5] << 8) | (buf[4] << 16) | (buf[3] << 24)));
		#endif
			if (data_base % 4 != 0)//4字节访问防止访问到奇数地址
			{
				int Correct_address = data_base - (data_base % 4);
				uint32_t temp_data = (*((uint32_t *)(Correct_address)));
				uint32_t mask = 0xFF << ((data_base % 4) * 8);
				temp_data &= ~mask;  // 清除指定字节
				(*((uint32_t *)(Correct_address))) = (temp_data | (buf[6] << ((data_base % 4) * 8)));
			}
			else
			{
				*((uint32_t *)(data_base)) = (buf[6] | (buf[5] << 8) | (buf[4] << 16) | (buf[3] << 24));
			}
			break;
	}
#if (DEBUGGER_OUTPUT_SWITCH == 1)
	/* Output from I2C */
	/*Write 0xFF as a signal through I2C*/
	iicFeedback = 2;
#else
	/* Output from UART */
	DEBUGER_putchar(0xff);
#endif
}
//*****************************************************************************
//
//  The Debugger Function for Interrupt
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void DEBUGGER_Int(void) // Debugger Interrupt Function
{
	BYTE value = 0;
	BYTE read_flag = 0;
	BYTE illegal_operation = 0;
	if (dbg_int_buf[0] < 0x3)
	{
		int index = dbg_int_buf[0] - 0x0;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				CPU_Int_Enable(index);
				break;
			case 0x1:
				CPU_Int_Disable(index);
				break;
			case 0x4:
				CPU_Int_Type_Edge(index);
				break;
			case 0x5:
				CPU_Int_Type_Level(index);
				break;
			case 0x8:
				value = CPU_Int_Enable_Read(index);
				read_flag = 1;
				break;
			case 0xA:
				value = CPU_Int_Type_Read(index);
				read_flag = 1;
				break;
			case 0xB:
				value = CPU_Int_Polarity_Read(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] == 0x3)
	{
		switch (dbg_int_buf[1])
		{
			case 0x0:
				SWUC_Int_Enable(dbg_int_buf[2]);
				break;
			case 0x1:
				SWUC_Int_Disable(dbg_int_buf[2]);
				break;
			case 0x8:
				value = SWUC_Int_Enable_Read(dbg_int_buf[2]);
				read_flag = 1;
				break;
			case 0xC:
				value = SWUC_Int_Status(dbg_int_buf[2]);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] == 0x4)//LPC_RST中断
	{
		switch (dbg_int_buf[1])
		{
			case 0x0:
				CPU_Int_Enable(4);
				break;
			case 0x1:
				CPU_Int_Disable(4);
				break;
			case 0x4:
				CPU_Int_Type_Edge(4);
				break;
			case 0x5:
				CPU_Int_Type_Level(4);
				break;
			case 0x6:
				CPU_Int_Polarity_HIGH(4);
				break;
			case 0x7:
				CPU_Int_Polarity_LOW(4);
				break;
			case 0x8:
				value = CPU_Int_Enable_Read(4);
				read_flag = 1;
				break;
			case 0xA:
				value = CPU_Int_Type_Read(4);
				read_flag = 1;
				break;
			case 0xB:
				value = CPU_Int_Polarity_Read(4);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] == 0x5)
	{
		switch (dbg_int_buf[1])
		{
			case 0x0:
				PWRSW_Config(0, 0);//enable
				break;
			case 0x1:
				SYSCTL_PWRSWCSR &= ~0x1; //disable
				break;
			case 0x8:
				value = 0;
				if (((SYSCTL_PWRSWCSR & 0x1) != 0) && ((SYSCTL_PWRSWCSR & 0x10) == 0))
				{
					value = 1;
				}
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] == 0x6)
	{
		switch (dbg_int_buf[1])
		{
			case 0x0:
				PS2_PORT0_CR = CCMD_WRITE;
				PS2_PORT0_OBUF |= 0x2;
				break;
			case 0x1:
				PS2_PORT0_CR = CCMD_WRITE;
				PS2_PORT0_OBUF &= ~0x2;
				break;
			case 0x8:
				PS2_PORT0_CR = CCMD_READ;
				value = (PS2_PORT0_IBUF & 0x2);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x7 && dbg_int_buf[0] <= 0x8)
	{
		switch (dbg_int_buf[1])
		{
			case 0x0:
				KBD_Int_Enable;
				break;
			case 0x1:
				KBD_Int_Disable;
				break;
			case 0x8:
				value = KBD_Int_Enable_Read;
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x9 && dbg_int_buf[0] <= 0xC)
	{
		int index = dbg_int_buf[0] - 0x9;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				Tach_Int_Enable(index);
				break;
			case 0x1:
				Tach_Int_Disable(index);
				break;
			case 0x8:
				value = Tach_Int_Enable_Read(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0xD && dbg_int_buf[0] <= 0xE)
	{
		int index = dbg_int_buf[0] - 0xD;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				KBC_Int_Enable(3 - index);
				break;
			case 0x1:
				KBC_Int_Disable(3 - index);
				break;
			case 0x8:
				value = KBC_Int_Enable_Read(3 - index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0xF && dbg_int_buf[0] <= 0x10)
	{
		int index = dbg_int_buf[0] - 0xF;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				PMC1_Int_Enable(index);
				break;
			case 0x1:
				PMC1_Int_Disable(index);
				break;
			case 0x8:
				value = PMC1_Int_Enable_Read(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x11 && dbg_int_buf[0] <= 0x12)
	{
		int index = dbg_int_buf[0] - 0x11;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				PMC2_Int_Enable(index);
				break;
			case 0x1:
				PMC2_Int_Disable(index);
				break;
			case 0x8:
				value = PMC2_Int_Enable_Read(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] == 0x13)//修改
	{
		switch (dbg_int_buf[1])
		{
			case 0x0:
				WDT_Init(0x1, 0xa);
				break;
			case 0x1:
				WDT_CR &= ~0x1;
				break;
			case 0x2:
				break;
			case 0x8:
				value = (WDT_CR & 0x1);
				read_flag = 1;
				break;
			case 0xC:
				value = Wdt_Int_Status;
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] == 0x14)//修改
	{
		switch (dbg_int_buf[1])
		{
			case 0x2:
				ADC_Int_Mask(dbg_int_buf[2]);
				break;
			case 0x3:
				ADC_Int_Unmask(dbg_int_buf[2]);
				break;
			case 0x9:
				value = ADC_Int_Mask_Read(dbg_int_buf[2]);
				read_flag = 1;
				break;
			case 0xC:
				value = ADC_Int_Status(dbg_int_buf[2]);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] == 0x15)
	{
		switch (dbg_int_buf[1])
		{
			case 0x0:
				Uart_Int_Enable(0, dbg_int_buf[2]);
				break;
			case 0x1:
				Uart_Int_Disable(0, dbg_int_buf[2]);
				break;
			case 0x8:
				value = Uart_Int_Enable_Read(0, dbg_int_buf[2]);
				read_flag = 1;
				break;
			case 0xC:
				value = Uart_Int_Status(0, dbg_int_buf[2]);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x16 && dbg_int_buf[0] <= 0x17)
	{
		int index = dbg_int_buf[0] - 0x16;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				Uart_Int_Enable(4 + index, dbg_int_buf[2]);
				break;
			case 0x1:
				Uart_Int_Disable(4 + index, dbg_int_buf[2]);
				break;
			case 0x8:
				value = Uart_Int_Enable_Read(4 + index, dbg_int_buf[2]);
				read_flag = 1;
				break;
			case 0xC:
				value = Uart_Int_Status(4 + index, dbg_int_buf[2]);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] == 0x18)//修改
	{
		switch (dbg_int_buf[1])
		{
			case 0x0:
				SMSEC_Int_Enable(dbg_int_buf[1]);
				break;
			case 0x1:
				SMSEC_Int_Disable(dbg_int_buf[1]);
				break;
			case 0x8:
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] == 0x19)
	{
		switch (dbg_int_buf[1])
		{
			// case 0x0:
			// 	SMSHOST_Int_Enable(dbg_int_buf[1]);
			// 	break;
			// case 0x1:
			// 	SMSHOST_Int_Disable(dbg_int_buf[1]);
			// 	break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x1A && dbg_int_buf[0] <= 0x1D)
	{
		int index = dbg_int_buf[0] - 0x1A;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				Timer_Int_Enable(index);
				break;
			case 0x1:
				Timer_Int_Disable(index);
				break;
			case 0x8:
				value = Timer_Int_Enable_Read(index);
				read_flag = 1;
				break;
			case 0xC:
				value = Timer_Int_Status(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] == 0x1E)
	{
		switch (dbg_int_buf[1])
		{
			case 0x0:
				ICTL0_INTEN0 = 0xff; ICTL0_INTEN1 = 0xff;
				ICTL0_INTEN2 = 0xff; ICTL0_INTEN3 = 0xff;
				ICTL0_INTEN4 = 0xff; ICTL0_INTEN5 = 0xff;
				ICTL0_INTEN6 = 0xff; ICTL0_INTEN7 = 0x7;
				break;
			case 0x1:
				ICTL0_INTEN0 = 0x00; ICTL0_INTEN1 = 0x00;
				ICTL0_INTEN2 = 0x00; ICTL0_INTEN3 = 0x00;
				ICTL0_INTEN4 = 0x00; ICTL0_INTEN5 = 0x00;
				ICTL0_INTEN6 = 0x00; ICTL0_INTEN7 = 0x00;
				break;
			case 0x2:
				ICTL0_INTMASK0 = 0xff; ICTL0_INTMASK1 = 0xff;
				ICTL0_INTMASK2 = 0xff; ICTL0_INTMASK3 = 0xff;
				ICTL0_INTMASK4 = 0xff; ICTL0_INTMASK5 = 0xff;
				ICTL0_INTMASK6 = 0xff; ICTL0_INTMASK7 = 0xff;
				break;
			case 0x3:
				ICTL0_INTMASK0 = 0x00; ICTL0_INTMASK1 = 0x00;
				ICTL0_INTMASK2 = 0x00; ICTL0_INTMASK3 = 0x00;
				ICTL0_INTMASK4 = 0x00; ICTL0_INTMASK5 = 0x00;
				ICTL0_INTMASK6 = 0x00; ICTL0_INTMASK7 = 0xf8;
				break;
			case 0x8:
				if (ICTL0_INTEN0 == 0xff && ICTL0_INTEN1 == 0xff && ICTL0_INTEN2 == 0xff
					&& ICTL0_INTEN3 == 0xff && ICTL0_INTEN4 == 0xff && ICTL0_INTEN5 == 0xff
					&& ICTL0_INTEN6 == 0xff && ((ICTL0_INTEN7 & 0x7) == 0x7))
				{
					value = 1;
				}
				else
				{
					value = 0;
				}
				read_flag = 1;
				break;
			case 0x9://是否屏蔽中断
				if (ICTL0_INTMASK0 == 0xff && ICTL0_INTMASK1 == 0xff && ICTL0_INTMASK2 == 0xff
					&& ICTL0_INTMASK3 == 0xff && ICTL0_INTMASK4 == 0xff && ICTL0_INTMASK5 == 0xff
					&& ICTL0_INTMASK6 == 0xff && ((ICTL0_INTMASK7 & 0x7) == 0x7))
				{
					value = 1;
				}
				else
				{
					value = 0;
				}
				read_flag = 1;
				break;
			case 0xC:
				value = Int_Control0_Status(dbg_int_buf[2]);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] == 0x1F)
	{
		switch (dbg_int_buf[1])
		{
			case 0x0:
				ICTL1_INTEN0 = 0xff; ICTL1_INTEN1 = 0xff;
				ICTL1_INTEN2 = 0xff; ICTL1_INTEN3 = 0xff;
				ICTL1_INTEN4 = 0xff; ICTL1_INTEN5 = 0xff;
				ICTL1_INTEN6 = 0xff; ICTL1_INTEN7 = 0xff;
				break;
			case 0x1:
				ICTL1_INTEN0 = 0x00; ICTL1_INTEN1 = 0x00;
				ICTL1_INTEN2 = 0x00; ICTL1_INTEN3 = 0x00;
				ICTL1_INTEN4 = 0x00; ICTL1_INTEN5 = 0x00;
				ICTL1_INTEN6 = 0x00; ICTL1_INTEN7 = 0x00;
				break;
			case 0x2:
				ICTL1_INTMASK0 = 0xff; ICTL1_INTMASK1 = 0xff;
				ICTL1_INTMASK2 = 0xff; ICTL1_INTMASK3 = 0xff;
				ICTL1_INTMASK4 = 0xff; ICTL1_INTMASK5 = 0xff;
				ICTL1_INTMASK6 = 0xff; ICTL1_INTMASK7 = 0xff;
				break;
			case 0x3:
				ICTL1_INTMASK0 = 0x00; ICTL1_INTMASK1 = 0x00;
				ICTL1_INTMASK2 = 0x00; ICTL1_INTMASK3 = 0x00;
				ICTL1_INTMASK4 = 0x00; ICTL1_INTMASK5 = 0x00;
				ICTL1_INTMASK6 = 0x00; ICTL1_INTMASK7 = 0x00;
				break;
			case 0x8://是否使能
				if (ICTL1_INTEN0 == 0xff && ICTL1_INTEN1 == 0xff && ICTL1_INTEN2 == 0xff
					&& ICTL1_INTEN3 == 0xff && ICTL1_INTEN4 == 0xff && ICTL1_INTEN5 == 0xff
					&& ICTL1_INTEN6 == 0xff && ((ICTL1_INTEN7 & 0x7) == 0x7))
				{
					value = 1;
				}
				else
				{
					value = 0;
				}
				read_flag = 1;
				break;
			case 0x9:
				if (ICTL1_INTMASK0 == 0xff && ICTL1_INTMASK1 == 0xff && ICTL1_INTMASK2 == 0xff
					&& ICTL1_INTMASK3 == 0xff && ICTL1_INTMASK4 == 0xff && ICTL1_INTMASK5 == 0xff
					&& ICTL1_INTMASK6 == 0xff && ((ICTL1_INTMASK7 & 0x7) == 0x7))
				{
					value = 1;
				}
				else
				{
					value = 0;
				}
				read_flag = 1;
				break;
			case 0xC:
				value = Int_Control1_Status(dbg_int_buf[2]);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x20 && dbg_int_buf[0] <= 0x27)
	{
		int index = dbg_int_buf[0] - 0x20;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				GPIOA0_7_Int_Enable(index);
				break;
			case 0x1:
				GPIOA0_7_Int_Disable(index);
				break;
			case 0x2:
				GPIOA0_7_Int_Mask(index);
				break;
			case 0x3:
				GPIOA0_7_Int_Unmask(index);
				break;
			case 0x4:
				GPIOA0_7_Int_Type_Edge(index);
				break;
			case 0x5:
				GPIOA0_7_Int_Type_Level(index);
				break;
			case 0x6:
				GPIOA0_7_Int_Polarity_High(index);
				break;
			case 0x7:
				GPIOA0_7_Int_Polarity_Low(index);
				break;
			case 0x8:
				value = GPIOA0_7_Int_Enable_Read(index);
				read_flag = 1;
				break;
			case 0x9:
				value = GPIOA0_7_Int_Mask_Read(index);
				read_flag = 1;
				break;
			case 0xA:
				value = GPIOA0_7_Int_Type_Read(index);
				read_flag = 1;
				break;
			case 0xB:
				value = GPIOA0_7_Int_Polarity_Read(index);
				read_flag = 1;
				break;
			case 0xC:
				value = GPIOA0_7_Int_Status(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x28 && dbg_int_buf[0] <= 0x2F)
	{
		int index = dbg_int_buf[0] - 0x28;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				GPIOA8_15_Int_Enable(index);
				break;
			case 0x1:
				GPIOA8_15_Int_Disable(index);
				break;
			case 0x2:
				GPIOA8_15_Int_Mask(index);
				break;
			case 0x3:
				GPIOA8_15_Int_Unmask(index);
				break;
			case 0x4:
				GPIOA8_15_Int_Type_Edge(index);
				break;
			case 0x5:
				GPIOA8_15_Int_Type_Level(index);
				break;
			case 0x6:
				GPIOA8_15_Int_Polarity_High(index);
				break;
			case 0x7:
				GPIOA8_15_Int_Polarity_Low(index);
				break;
			case 0x8:
				value = GPIOA8_15_Int_Enable_Read(index);
				read_flag = 1;
				break;
			case 0x9:
				value = GPIOA8_15_Int_Mask_Read(index);
				read_flag = 1;
				break;
			case 0xA:
				value = GPIOA8_15_Int_Type_Read(index);
				read_flag = 1;
				break;
			case 0xB:
				value = GPIOA8_15_Int_Polarity_Read(index);
				read_flag = 1;
				break;
			case 0xC:
				value = GPIOA8_15_Int_Status(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x30 && dbg_int_buf[0] <= 0x37)
	{
		int index = dbg_int_buf[0] - 0x30;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				GPIOA16_23_Int_Enable(index);
				break;
			case 0x1:
				GPIOA16_23_Int_Disable(index);
				break;
			case 0x2:
				GPIOA16_23_Int_Mask(index);
				break;
			case 0x3:
				GPIOA16_23_Int_Unmask(index);
				break;
			case 0x4:
				GPIOA16_23_Int_Type_Edge(index);
				break;
			case 0x5:
				GPIOA16_23_Int_Type_Level(index);
				break;
			case 0x6:
				GPIOA16_23_Int_Polarity_High(index);
				break;
			case 0x7:
				GPIOA16_23_Int_Polarity_Low(index);
				break;
			case 0x8:
				value = GPIOA16_23_Int_Enable_Read(index);
				read_flag = 1;
				break;
			case 0x9:
				value = GPIOA16_23_Int_Mask_Read(index);
				read_flag = 1;
				break;
			case 0xA:
				value = GPIOA16_23_Int_Type_Read(index);
				read_flag = 1;
				break;
			case 0xB:
				value = GPIOA16_23_Int_Polarity_Read(index);
				read_flag = 1;
				break;
			case 0xC:
				value = GPIOA16_23_Int_Status(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x38 && dbg_int_buf[0] <= 0x3F)
	{
		int index = dbg_int_buf[0] - 0x38;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				GPIOA24_31_Int_Enable(index);
				break;
			case 0x1:
				GPIOA24_31_Int_Disable(index);
				break;
			case 0x2:
				GPIOA24_31_Int_Mask(index);
				break;
			case 0x3:
				GPIOA24_31_Int_Unmask(index);
				break;
			case 0x4:
				GPIOA24_31_Int_Type_Edge(index);
				break;
			case 0x5:
				GPIOA24_31_Int_Type_Level(index);
				break;
			case 0x6:
				GPIOA24_31_Int_Polarity_High(index);
				break;
			case 0x7:
				GPIOA24_31_Int_Polarity_Low(index);
				break;
			case 0x8:
				value = GPIOA24_31_Int_Enable_Read(index);
				read_flag = 1;
				break;
			case 0x9:
				value = GPIOA24_31_Int_Mask_Read(index);
				read_flag = 1;
				break;
			case 0xA:
				value = GPIOA24_31_Int_Type_Read(index);
				read_flag = 1;
				break;
			case 0xB:
				value = GPIOA24_31_Int_Polarity_Read(index);
				read_flag = 1;
				break;
			case 0xC:
				value = GPIOA24_31_Int_Status(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x40 && dbg_int_buf[0] <= 0x47)
	{
		int index = dbg_int_buf[0] - 0x40;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				GPIOB0_7_Int_Enable(index);
				break;
			case 0x1:
				GPIOB0_7_Int_Disable(index);
				break;
			case 0x2:
				GPIOB0_7_Int_Mask(index);
				break;
			case 0x3:
				GPIOB0_7_Int_Unmask(index);
				break;
			case 0x4:
				GPIOB0_7_Int_Type_Edge(index);
				break;
			case 0x5:
				GPIOB0_7_Int_Type_Level(index);
				break;
			case 0x6:
				GPIOB0_7_Int_Polarity_High(index);
				break;
			case 0x7:
				GPIOB0_7_Int_Polarity_Low(index);
				break;
			case 0x8:
				value = GPIOB0_7_Int_Enable_Read(index);
				read_flag = 1;
				break;
			case 0x9:
				value = GPIOB0_7_Int_Mask_Read(index);
				read_flag = 1;
				break;
			case 0xA:
				value = GPIOB0_7_Int_Type_Read(index);
				read_flag = 1;
				break;
			case 0xB:
				value = GPIOB0_7_Int_Polarity_Read(index);
				read_flag = 1;
				break;
			case 0xC:
				value = GPIOB0_7_Int_Status(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x48 && dbg_int_buf[0] <= 0x4F)
	{
		int index = dbg_int_buf[0] - 0x48;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				GPIOB8_15_Int_Enable(index);
				break;
			case 0x1:
				GPIOB8_15_Int_Disable(index);
				break;
			case 0x2:
				GPIOB8_15_Int_Mask(index);
				break;
			case 0x3:
				GPIOB8_15_Int_Unmask(index);
				break;
			case 0x4:
				GPIOB8_15_Int_Type_Edge(index);
				break;
			case 0x5:
				GPIOB8_15_Int_Type_Level(index);
				break;
			case 0x6:
				GPIOB8_15_Int_Polarity_High(index);
				break;
			case 0x7:
				GPIOB8_15_Int_Polarity_Low(index);
				break;
			case 0x8:
				value = GPIOB8_15_Int_Enable_Read(index);
				read_flag = 1;
				break;
			case 0x9:
				value = GPIOB8_15_Int_Mask_Read(index);
				read_flag = 1;
				break;
			case 0xA:
				value = GPIOB8_15_Int_Type_Read(index);
				read_flag = 1;
				break;
			case 0xB:
				value = GPIOB8_15_Int_Polarity_Read(index);
				read_flag = 1;
				break;
			case 0xC:
				value = GPIOB8_15_Int_Status(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	// sws----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if (dbg_int_buf[0] >= 0x50 && dbg_int_buf[0] <= 0x53)
	{
		int index = dbg_int_buf[0] - 0x50;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				GPIOB16_19_Int_Enable(index);
				break;
			case 0x1:
				GPIOB16_19_Int_Disable(index);
				break;
			case 0x2:
				GPIOB16_19_Int_Mask(index);
				break;
			case 0x3:
				GPIOB16_19_Int_Unmask(index);
				break;
			case 0x4:
				GPIOB16_19_Int_Type_Edge(index);
				break;
			case 0x5:
				GPIOB16_19_Int_Type_Level(index);
				break;
			case 0x6:
				GPIOB16_19_Int_Polarity_High(index);
				break;
			case 0x7:
				GPIOB16_19_Int_Polarity_Low(index);
				break;
			case 0x8:
				value = GPIOB16_19_Int_Enable_Read(index);
				read_flag = 1;
				break;
			case 0x9:
				value = GPIOB16_19_Int_Mask_Read(index);
				read_flag = 1;
				break;
			case 0xA:
				value = GPIOB16_19_Int_Type_Read(index);
				read_flag = 1;
				break;
			case 0xB:
				value = GPIOB16_19_Int_Polarity_Read(index);
				read_flag = 1;
				break;
			case 0xC:
				value = GPIOB16_19_Int_Status(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x54 && dbg_int_buf[0] <= 0x5A)
	{
		int index = dbg_int_buf[0] - 0x54;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				GPIOB20_26_Int_Enable(index);
				break;
			case 0x1:
				GPIOB20_26_Int_Disable(index);
				break;
			case 0x2:
				GPIOB20_26_Int_Mask(index);
				break;
			case 0x3:
				GPIOB20_26_Int_Unmask(index);
				break;
			case 0x4:
				GPIOB20_26_Int_Type_Edge(index);
				break;
			case 0x5:
				GPIOB20_26_Int_Type_Level(index);
				break;
			case 0x6:
				GPIOB20_26_Int_Polarity_High(index);
				break;
			case 0x7:
				GPIOB20_26_Int_Polarity_Low(index);
				break;
			case 0x8:
				value = GPIOB20_26_Int_Enable_Read(index);
				read_flag = 1;
				break;
			case 0x9:
				value = GPIOB20_26_Int_Mask_Read(index);
				read_flag = 1;
				break;
			case 0xA:
				value = GPIOB20_26_Int_Type_Read(index);
				read_flag = 1;
				break;
			case 0xB:
				value = GPIOB20_26_Int_Polarity_Read(index);
				read_flag = 1;
				break;
			case 0xC:
				value = GPIOB20_26_Int_Status(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x5B && dbg_int_buf[0] <= 0x62)
	{
		int index = dbg_int_buf[0] - 0x5B;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				GPIOC0_7_Int_Enable(index);
				break;
			case 0x1:
				GPIOC0_7_Int_Disable(index);
				break;
			case 0x2:
				GPIOC0_7_Int_Mask(index);
				break;
			case 0x3:
				GPIOC0_7_Int_Unmask(index);
				break;
			case 0x4:
				GPIOC0_7_Int_Type_Edge(index);
				break;
			case 0x5:
				GPIOC0_7_Int_Type_Level(index);
				break;
			case 0x6:
				GPIOC0_7_Int_Polarity_High(index);
				break;
			case 0x7:
				GPIOC0_7_Int_Polarity_Low(index);
				break;
			case 0x8:
				value = GPIOC0_7_Int_Enable_Read(index);
				read_flag = 1;
				break;
			case 0x9:
				value = GPIOC0_7_Int_Mask_Read(index);
				read_flag = 1;
				break;
			case 0xA:
				value = GPIOC0_7_Int_Type_Read(index);
				read_flag = 1;
				break;
			case 0xB:
				value = GPIOC0_7_Int_Polarity_Read(index);
				read_flag = 1;
				break;
			case 0xC:
				value = GPIOC0_7_Int_Status(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x63 && dbg_int_buf[0] <= 0x68)
	{
		int index = dbg_int_buf[0] - 0x63;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				GPIOC8_13_Int_Enable(index);
				break;
			case 0x1:
				GPIOC8_13_Int_Disable(index);
				break;
			case 0x2:
				GPIOC8_13_Int_Mask(index);
				break;
			case 0x3:
				GPIOC8_13_Int_Unmask(index);
				break;
			case 0x4:
				GPIOC8_13_Int_Type_Edge(index);
				break;
			case 0x5:
				GPIOC8_13_Int_Type_Level(index);
				break;
			case 0x6:
				GPIOC8_13_Int_Polarity_High(index);
				break;
			case 0x7:
				GPIOC8_13_Int_Polarity_Low(index);
				break;
			case 0x8:
				value = GPIOC8_13_Int_Enable_Read(index);
				read_flag = 1;
				break;
			case 0x9:
				value = GPIOC8_13_Int_Mask_Read(index);
				read_flag = 1;
				break;
			case 0xA:
				value = GPIOC8_13_Int_Type_Read(index);
				read_flag = 1;
				break;
			case 0xB:
				value = GPIOC8_13_Int_Polarity_Read(index);
				read_flag = 1;
				break;
			case 0xC:
				value = GPIOC8_13_Int_Status(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x69 && dbg_int_buf[0] <= 0x70)
	{
		int index = dbg_int_buf[0] - 0x69;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				GPIOE0_7_Int_Enable(index);
				break;
			case 0x1:
				GPIOE0_7_Int_Disable(index);
				break;
			case 0x2:
				GPIOE0_7_Int_Mask(index);
				break;
			case 0x3:
				GPIOE0_7_Int_Unmask(index);
				break;
			case 0x4:
				GPIOE0_7_Int_Type_Edge(index);
				break;
			case 0x5:
				GPIOE0_7_Int_Type_Level(index);
				break;
			case 0x6:
				GPIOE0_7_Int_Polarity_High(index);
				break;
			case 0x7:
				GPIOE0_7_Int_Polarity_Low(index);
				break;
			case 0x8:
				value = GPIOE0_7_Int_Enable_Read(index);
				read_flag = 1;
				break;
			case 0x9:
				value = GPIOE0_7_Int_Mask_Read(index);
				read_flag = 1;
				break;
			case 0xA:
				value = GPIOE0_7_Int_Type_Read(index);
				read_flag = 1;
				break;
			case 0xB:
				value = GPIOE0_7_Int_Polarity_Read(index);
				read_flag = 1;
				break;
			case 0xC:
				value = GPIOE0_7_Int_Status(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x71 && dbg_int_buf[0] <= 0x78)
	{
		int index = dbg_int_buf[0] - 0x71;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				GPIOE8_15_Int_Enable(index);
				break;
			case 0x1:
				GPIOE8_15_Int_Disable(index);
				break;
			case 0x2:
				GPIOE8_15_Int_Mask(index);
				break;
			case 0x3:
				GPIOE8_15_Int_Unmask(index);
				break;
			case 0x4:
				GPIOE8_15_Int_Type_Edge(index);
				break;
			case 0x5:
				GPIOE8_15_Int_Type_Level(index);
				break;
			case 0x6:
				GPIOE8_15_Int_Polarity_High(index);
				break;
			case 0x7:
				GPIOE8_15_Int_Polarity_Low(index);
				break;
			case 0x8:
				value = GPIOE8_15_Int_Enable_Read(index);
				read_flag = 1;
				break;
			case 0x9:
				value = GPIOE8_15_Int_Mask_Read(index);
				read_flag = 1;
				break;
			case 0xA:
				value = GPIOE8_15_Int_Type_Read(index);
				read_flag = 1;
				break;
			case 0xB:
				value = GPIOE8_15_Int_Polarity_Read(index);
				read_flag = 1;
				break;
			case 0xC:
				value = GPIOE8_15_Int_Status(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x79 && dbg_int_buf[0] <= 0x80)
	{
		int index = dbg_int_buf[0] - 0x79;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				GPIOE16_23_Int_Enable(index);
				break;
			case 0x1:
				GPIOE16_23_Int_Disable(index);
				break;
			case 0x2:
				GPIOE16_23_Int_Mask(index);
				break;
			case 0x3:
				GPIOE16_23_Int_Unmask(index);
				break;
			case 0x4:
				GPIOE16_23_Int_Type_Edge(index);
				break;
			case 0x5:
				GPIOE16_23_Int_Type_Level(index);
				break;
			case 0x6:
				GPIOE16_23_Int_Polarity_High(index);
				break;
			case 0x7:
				GPIOE16_23_Int_Polarity_Low(index);
				break;
			case 0x8:
				value = GPIOE16_23_Int_Enable_Read(index);
				read_flag = 1;
				break;
			case 0x9:
				value = GPIOE16_23_Int_Mask_Read(index);
				read_flag = 1;
				break;
			case 0xA:
				value = GPIOE16_23_Int_Type_Read(index);
				read_flag = 1;
				break;
			case 0xB:
				value = GPIOE16_23_Int_Polarity_Read(index);
				read_flag = 1;
				break;
			case 0xC:
				value = GPIOE16_23_Int_Status(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x81 && dbg_int_buf[0] <= 0x82)
	{
		int index = dbg_int_buf[0] - 0x81;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				PMC3_Int_Enable(index);
				break;
			case 0x1:
				PMC3_Int_Disable(index);
				break;
			case 0x8:
				value = PMC3_Int_Enable_Read(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x83 && dbg_int_buf[0] <= 0x84)
	{
		int index = dbg_int_buf[0] - 0x83;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				PMC4_Int_Enable(index);
				break;
			case 0x1:
				PMC4_Int_Disable(index);
				break;
			case 0x8:
				value = PMC4_Int_Enable_Read(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x85 && dbg_int_buf[0] <= 0x86)
	{
		int index = dbg_int_buf[0] - 0x85;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				PMC5_Int_Enable(index);
				break;
			case 0x1:
				PMC5_Int_Disable(index);
				break;
			case 0x8:
				value = PMC5_Int_Enable_Read(index);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] == 0x87)
	{
		switch (dbg_int_buf[1])
		{
			case 0x0:
				PS2_PORT1_CR = CCMD_WRITE;
				PS2_PORT1_OBUF |= 0x1;
				break;
			case 0x1:
				PS2_PORT1_CR = CCMD_WRITE;
				PS2_PORT1_OBUF &= ~0x1;
				break;
			case 0x8:
				PS2_PORT1_CR = 0x20;
				value = (PS2_PORT1_IBUF & 0x1);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x88 && dbg_int_buf[0] <= 0x8A)
	{
		int index = dbg_int_buf[0] - 0x87;
		switch (dbg_int_buf[1])
		{
			case 0x0:
				Uart_Int_Enable(index, dbg_int_buf[2]);
				break;
			case 0x1:
				Uart_Int_Disable(index, dbg_int_buf[2]);
				break;
			case 0x8:
				value = Uart_Int_Enable_Read(index, dbg_int_buf[2]);
				read_flag = 1;
				break;
			case 0xC:
				value = Uart_Int_Status(index, dbg_int_buf[2]);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] == 0x8B)//修改
	{
		switch (dbg_int_buf[1])
		{
			case 0x2:
				Spim_Int_Mask(4);//接收满中断
				break;
			case 0x3:
				Spim_Int_Unmask(4);
				break;
			case 0x9:
				value = Spim_Int_Mask_Read(4);
				read_flag = 1;
				break;
			case 0xC:
				value = Spim_Int_Status(dbg_int_buf[2]);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x8C && dbg_int_buf[0] <= 0x8F)//修改
	{
		int index = dbg_int_buf[0] - 0x8C;
		switch (dbg_int_buf[1])
		{
			case 0x2:
				I2C_Int_Mask(index, 2);//接收满
				break;
			case 0x3:
				I2C_Int_Unmask(index, 2);//接收满
				break;
			case 0x9:
				value = I2C_Int_Mask_Read(index, 2);//接收满
				read_flag = 1;
				break;
			case 0xC:
				value = I2C_Int_Status(index, 2);//接收满
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] >= 0x90 && dbg_int_buf[0] <= 0x93)//修改
	{
		switch (dbg_int_buf[1])
		{
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] == 0x94)
	{
		// switch(dbg_int_buf[1])
		// {
		// 	case 0:
		// 		Pport_Int_Enable;
		// 		break;
		// 	case 0x1:
		// 		Pport_Int_Disable;
		// 		break;
		// 	case 0x8:
		// 		value = Pport_Int_Enable_Read;
		// 		read_flag = 1;
		// 		break;
		// 	case 0xC:
		// 		value = Pport_Int_Status;
		// 		read_flag = 1;
		// 		break;
		// 	default:
		// 		illegal_operation = 1;
		// 		break;
		// }
	}
	if (dbg_int_buf[0] == 0x95)
	{
		switch (dbg_int_buf[1])
		{
			case 0x0:
				Por_Int_Enable(dbg_int_buf[2]);
				break;
			case 0x1:
				Por_Int_Disable(dbg_int_buf[2]);
				break;
			case 0x8:
				value = Por_Int_Enable_Read(dbg_int_buf[2]);
				read_flag = 1;
				break;
			case 0xC:
				value = Por_Int_Status(dbg_int_buf[2]);
				read_flag = 1;
				break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (dbg_int_buf[0] == 0x96)
	{
		switch (dbg_int_buf[1])
		{
			// case 0x2:
			// 	Mon_Int_Mask(dbg_int_buf[2]);
			// 	break;
			// case 0x3:
			// 	Mon_Int_Unmask(dbg_int_buf[2]);
			// 	break;
			// case 0x9:
			// 	value = Mon_Int_Mask_Read(dbg_int_buf[2]);
			// 	read_flag = 1;
			// 	break;
			// case 0xC:
			// 	value = Mon_Int_Mask_Read(dbg_int_buf[2]);
			// 	read_flag = 1;
			// 	break;
			default:
				illegal_operation = 1;
				break;
		}
	}
	if (!(dbg_int_buf[0] <= 0x96))
	{
		illegal_operation = 1;
	}
	if (0 == illegal_operation)
	{
		if (1 == read_flag)
		{
		#if DEBUGGER_DEBUG
			dprint("send back read value:0x%x \n", value);
		#endif
			if (value != 0)
			{
			#if (DEBUGGER_OUTPUT_SWITCH == 1)
							/* Output from I2C */
				iicFeedback = 3;
				iic_int_flag = 1;
			#else
							/* Output from UART */
				DEBUGER_putchar(0x01);
			#endif
				return;
			}
			else
			{
			#if (DEBUGGER_OUTPUT_SWITCH == 1)
							/* Output from I2C */
				iicFeedback = 3;
				iic_int_flag = 2;
			#else
							/* Output from UART */
				DEBUGER_putchar(0x00);
			#endif
				return;
			}
		}
		else
		{
		#if (DEBUGGER_OUTPUT_SWITCH == 1)
					/* Output from I2C */
			iicFeedback = 3;
			iic_int_flag = 3;
		#else
					/* Output from UART */
			// dprint("send back 0x11 \n");
			DEBUGER_putchar(0x11);
		#endif
			return;
		}
	}
	else
	{
	#if (DEBUGGER_OUTPUT_SWITCH == 1)
		iicFeedback = 3;
		iic_int_flag = 4;
	#else
			/* Output from UART */
		// dprint("send back 0xDD \n");
		DEBUGER_putchar(0xDD);
	#endif
		return;
	}
}
//*****************************************************************************
//
//  The Debugger Function for record KBC or PMC
//
//  parameter :
//		direction :  0-input from host;1-output to host
//		channel :  0-KBC;1-PMC1;2-PMC2;3-PMC3;4-PMC4
//      value : value need to be record
//
//  return :
//      Function running condition
//
//*****************************************************************************
BYTE Debugger_KBC_PMC_Record(BYTE direction, BYTE channel, BYTE value)
{
	if (KBC_PMC_Flag)
	{
		/*first  0-input;1-output*/
		/*second 0-KBC;1-PMC1;2-PMC2;3-PMC3;4-PMC4*/
		if ((KBC_PMC_Event_Num >= 256) || (KBC_PMC_Pending_Num > 0) || (KBC_PMC_PendingRXCount != KBC_PMC_PendingTXCount))
		{
			char KBC_PMC_Tempdata[2] = { 0 };
			KBC_PMC_Tempdata[0] = (direction) << 4 | (channel); // record direction and channel
			KBC_PMC_Tempdata[1] = value; // record value
			KBC_PMC_DataPending(KBC_PMC_Tempdata);
			return 0;
		}
		KBC_PMC_Event[KBC_PMC_Event_Num] = (direction) << 4 | (channel); // record direction and channel
		KBC_PMC_Event_Num++;
		KBC_PMC_Event[KBC_PMC_Event_Num] = value; // record value
		KBC_PMC_Event_Num++;
	}
	return 0;
}
// #else //when chip set as debugger master
/*------------Only the forwarding function is available-------------*/
//*****************************************************************************
//
//  Debugger Master read in byte
//
//  parameter :
//      i2c_channel : I2C channel number 
//
//  return :
//      none
//
//*****************************************************************************
char Debugger_Master_Read_Byte(WORD i2c_channel)
{
	char data;
	if (0 == I2C_Check_TFNF(i2c_channel))
	{
		SMBUSn_DATA_CMD0(i2c_channel) = I2C_READ | I2C_STOP;
	}
	if (0 == I2C_Check_RFNE(i2c_channel))
	{
		data = SMBUSn_DATA_CMD0(i2c_channel) & 0xff;
	}
	return data;
}
//*****************************************************************************
//
//  Debugger Master write in byte
//
//  parameter :
//		data : need to be write
//      i2c_channel : I2C channel number 
//
//  return :
//      none
//
//*****************************************************************************
void Debugger_Master_Write_Byte(char data, WORD i2c_channel)
{
	if (0 == I2C_Check_TFNF(i2c_channel))
	{
		SMBUSn_DATA_CMD0(i2c_channel) = data | I2C_WRITE | I2C_STOP;
	}
}
#if (DEBUGGER_OUTPUT_SWITCH == 0)
//*****************************************************************************
//
//  Debugger I2c Connect
//
//  parameter :
//      none
//
//  return :
//      none
//
//	note :
//		Send to Slave for Connect Notification
//*****************************************************************************
void I2c_Connect(void)
{
	Debugger_Master_Write_Byte(0xDD, 0);
	BYTE data = Debugger_Master_Read_Byte(0);
	if (data == 0xDD)
	{
		DEBUGER_putchar(0xDD);
	}
}
//*****************************************************************************
//
//  Debugger I2c Disconnect
//
//  parameter :
//      none
//
//  return :
//      none
//
//	note :
//		Send to Slave for Disconnect Notification
//*****************************************************************************
void I2c_Disconnect(void)
{
	BYTE data;
	Debugger_Master_Write_Byte(0x88, 0);
	data = Debugger_Master_Read_Byte(0) & 0xFF;
	if (data == 0x88)
	{
		DEBUGER_putchar(0x88);
	}
}
//*****************************************************************************
//
//  Debugger Master Read Block
//
//  parameter :
//      buf[] : addr array
//		channel : I2c channel
//
//  return :
//      none
//
//	note :
//		Send Addr to Slave and Read Data Back
//*****************************************************************************
void Debugger_Master_Read(short buf[], DWORD channel)
{
	int i, j;
	int data[DEBUGGER_BUFF_SIZE];
	if (0 == I2C_Check_TFE(channel))
	{
		SMBUSn_DATA_CMD0(channel) = 0x55 | I2C_WRITE;
	}
	if (0 == I2C_Check_TFNF(channel))
	{
		SMBUSn_DATA_CMD0(channel) = buf[0] | I2C_WRITE;
	}
	if (0 == I2C_Check_TFNF(channel))
	{
		SMBUSn_DATA_CMD0(channel) = buf[1] | I2C_WRITE;
	}
	if (0 == I2C_Check_TFNF(channel))
	{
		SMBUSn_DATA_CMD0(channel) = buf[2] | I2C_WRITE | I2C_STOP;
	}
	for (j = 0; j < 32; j++)
	{
		for (i = 0; i < 7; i++)
		{
			if (0 == I2C_Check_TFNF(channel))
			{
				SMBUSn_DATA_CMD0(channel) = I2C_READ;
			}
		}
		if (j == 31)
		{
			if (0 == I2C_Check_TFNF(channel))
			{
				SMBUSn_DATA_CMD0(channel) = I2C_READ | I2C_STOP;
			}
		}
		else
		{
			if (0 == I2C_Check_TFNF(channel))
			{
				SMBUSn_DATA_CMD0(channel) = I2C_READ;
			}
		}
		for (i = 0; i < 8; i++)
		{
			if (0 == I2C_Check_RFNE(channel))
			{
				data[i + j * 8] = SMBUSn_DATA_CMD0(channel) & 0xff;
			}
		}
	}
	for (j = 0; j < buf[3] + 1; j++)
	{
		DEBUGER_putchar(data[j]);
	}
}
//*****************************************************************************
//
//  Debugger Write Read Block
//
//  parameter :
//      buf[] : addr array
//		channel : I2c channel
//
//  return :
//      none
//
//	note :
//		Send Addr and Data to Slave
//*****************************************************************************
void Debugger_Master_Write(short buf[], DWORD channel)
{
	int get_data;
	int i;
	if (0 == I2C_Check_TFNF(channel))
	{
		SMBUSn_DATA_CMD0(channel) = 0xFF | I2C_WRITE;
	}
	for (i = 0; i < 6; i++)
	{
		if (0 == I2C_Check_TFNF(channel))
		{
			SMBUSn_DATA_CMD0(channel) = buf[i] | I2C_WRITE;
		}
	}
	if (0 == I2C_Check_TFNF(channel))
	{
		SMBUSn_DATA_CMD0(channel) = buf[i] | I2C_WRITE | I2C_STOP;
	}
	if (0 == I2C_Check_TFNF(channel))
	{
		SMBUSn_DATA_CMD0(channel) = I2C_WRITE | I2C_STOP;
	}
	if (0 == I2C_Check_RFNE(channel))
	{
		get_data = SMBUSn_DATA_CMD0(channel) & 0xFF;
	}
	DEBUGER_putchar(get_data);
}
//*****************************************************************************
//
//  Debugger Master Data Forwarding Function
//
//  parameter :
//      none
//
//  return :
//      none
//
//	note :
//		Use This Chip as I2c Master to Forward Data to Target Slave Chip
//*****************************************************************************
void Debugger_Master_Retrans(BYTE debuger)
{
#if !(SMBUS1_CLOCK_EN)
	dprint("SMBUS1 CLOCK NOT ENABLE\n");
	return;
#endif
	Debuger_Cmd_Data = debuger & 0xff;
	// dprint("uartdata = %#x\n", Debuger_Cmd_Data);
	if ((!Buf_Flag) && (Debuger_Cmd_Data == 0x55 || Debuger_Cmd_Data == 0xff || Debuger_Cmd_Data == 0xDD || Debuger_Cmd_Data == 0x88 || Debuger_Cmd_Data == 0xAA || Debuger_Cmd_Data == 0xBB))
	{
		switch (Debuger_Cmd_Data)
		{
			case 0x88:
				I2c_Disconnect();
				break;
			case 0xDD:
				I2c_Connect();
				break;
			case 0xAA:
				DEBUGER_putchar(0xAA);
				Debugger_Master_Write_Byte(0xAA, 0);
				break;
			case 0xBB:
				DEBUGER_putchar(0xBB);
				Debugger_Master_Write_Byte(0xBB, 0);
				break;
			case 0xE0: // reset chip
				DEBUGER_putchar(0xE0);
				Debugger_Master_Write_Byte(0xE0, 0);
				break;
			case 0xB0: // change keyboard code type
				DEBUGER_putchar(0xB0);
				Debugger_Master_Write_Byte(0xB0, 0);
				break;
			default:
				Buf_Num = 0;
				Buf_Flag = 1;
				Debug_Temp = Debuger_Cmd_Data;
				return;
		}
	}
	if (Buf_Flag)
	{
		switch (Debug_Temp)
		{
			case 0x55:
				buf[Buf_Num] = Debuger_Cmd_Data;
				Buf_Num++;
				if (Buf_Num == 4)
				{
					Buf_Flag = 0;
					Debug_Temp = 0;
					Debugger_Master_Read(buf, 0);
				}
				break;
			case 0xFF:
				buf[Buf_Num] = Debuger_Cmd_Data;
				Buf_Num++;
				if (Buf_Num == 7)
				{
					Buf_Flag = 0;
					Debug_Temp = 0;
					Debugger_Master_Write(buf, 0);
				}
				break;
			default:
				break;
		}
	}
}


char Check_Debugger_Data(const char *buffer, short write_index)
{
	// 在缓冲区中查找匹配的数据串
	for (short i = 0; i < UART_BUFFER_SIZE; ++i)
	{
		char matched = 1;
		for (unsigned char j = 0; j < Handshake_length; ++j)
		{
			short index = (write_index - Handshake_length + j + UART_BUFFER_SIZE + i) % UART_BUFFER_SIZE;
			if (buffer[index] != Handshake_data[j])
			{
				matched = 0;
				break;
			}
		}
		if (matched)
		{
			return 1; // 找到匹配的数据串
		}
	}
	return 0; // 未找到匹配的数据串
}



char Debugger_Handshake(BYTE data)
{
	if (Handshake_Flag == 0)
	{
		if (Check_Debugger_Data(Uart_buffer, Uart_Rx_index))
		{
			DEBUGER_putchar(0xA5);//第二次握手

			//清除整个Uart_buffer和fifo
			for (unsigned char i = 0; i < UART_BUFFER_SIZE; i++)
			{
				DEBUGGER_TX;
				Uart_buffer[i] = 0;
			}
			//发送0x5A 以及版本号
			for (unsigned char i = 4; i > 0; i--) // 第三次握手
			{
				DEBUGER_putchar(((SYSCTL_IDVER >> (8 * (i - 1))) & 0xff));
			}
			Handshake_Flag = 1;
		}
		else//失败
		{
			Handshake_cunt++;
			if (Handshake_cunt >= 36)//失败次数超过36次
			{

			}
			return -1;
		}
	}
	if (Handshake_Flag != 0)
	{
		if (data == 0xA5)//第四次握手
		{
			dprint("Debugger handshake success!\n");
			Handshake_Flag = 0;
			return 1;
		}
	}
	return 0;
}
#endif
#endif