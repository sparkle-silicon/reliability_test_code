/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-16 15:22:18
 * @Description:
 *
 *
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-09-18 16:57:45
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
 // Include all header file
#include "AE_DEBUGGER.H"
#include "AE_COMMAND.H"

extern void DEBUGER_putchar(char ch);
extern u_char DEBUGER_getchar(void);


//*****************************************************************************
//
// local variables for Debugger api
//
//*****************************************************************************
/*------------Only the slave debugger function is available-------------*/
void Deubgger_READ_REG(DWORD addr, DWORD size)
{
	uDword data[64];
	size += 1;
	if(addr >= 0x10000 && addr <= 0xFFFFF)//AHB1/AHB2
	{
		uint32_t *ptr = (uint32_t *)addr;
		for(size_t i = 0; i < 64; i++)
		{
			data[i].dword = ptr[i];
		}

	}
	else if(addr >= 0x4000 && addr <= 0x4FFF)//SPIS CEC ADC PWM/TACH
	{
		uint16_t *ptr = (uint16_t *)addr;
		for(size_t i = 0; i < 128; i++)
		{
			data[i >> 1].word[i & 1] = ptr[i];
		}
	}
	else//APB1-8bit
	{
		uint8_t *ptr = (uint8_t *)addr;
		for(size_t i = 0; i < 256; i++)
		{
			data[i >> 2].byte[i & 3] = ptr[i];
		}
	}
// #ifdef ROM_DEBUG
// 	printf("Deubgger_READ_REG\n");
// #endif
	/*Output from UART */
	for(size_t i = 0; i < size; i++)
	{
		DEBUGER_putchar(data[i >> 2].byte[i & 3]);
	}
}
void Deubgger_WRITE_REG(DWORD addr, DWORD data)
{
	if(addr >= 0x10000 && addr <= 0xFFFFF)
	{
		*((uint32_t *)(addr)) = data;

	}
	else if(addr >= 0x4000 && addr <= 0x4FFF)
	{
		*((uint16_t *)(addr)) = (data & 0xffff);
	}
	else
	{
		*((uint8_t *)(addr)) = (data & 0xff);
	}
// #ifdef ROM_DEBUG
// 	printf("Deubgger_WRITE_REG\n");
// #endif
	DEBUGER_putchar(0xff);
}

uint32_t Deubgger_HANDSHAKE(const unsigned char *cmd)
{
	for(size_t i = 0; i < 4; i++)
	{
		if(cmd[i] != DEBUGGER_CMD_HANDSHAKE)
			return (uint32_t)-1;
	}
	DEBUGER_putchar(0xA5);
	nop;
	nop;
	nop;
	while(DEBUGGER_LSR & UART_LSR_DR)
	{
		DEBUGER_getchar();//清空FIFO内容
	}
	uDword ID;
	ID.dword = SYSCTL_IDVER;
	//第三次握手
	DEBUGER_putchar(ID.byte[3]);
	DEBUGER_putchar(ID.byte[2]);
	DEBUGER_putchar(ID.byte[1]);
	DEBUGER_putchar(ID.byte[0]);
// #if ROM_DEBUG
// 	printf("ID(%#x)\n", ID.dword, ID.byte[3], ID.byte[2], ID.byte[1], ID.byte[0]);
// #endif
	for(size_t i = 0; i < HIGHT_CHIP_CLOCK; i++)
	{
		if(DEBUGGER_LSR & UART_LSR_DR)
		{
			if(DEBUGER_getchar() == 0xA5)
			{
			#if ROM_DEBUG
				printf("debugger start\n");
			#endif
				debugger_flag = 1; return 0;
			}//清空FIFO内容
			else i = 0;
		}
		else nop;
	}
	debugger_flag = 0;//超时未响应则无效
	return (uint32_t)-2;
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
//		0xE0--Reset chip
//		0xB0--Switching keyboard Codes
//		0x75--Firmware update
//		0x11--Interrupt function block
//****************************************************************************

uint32_t  Deubgger_Cmd_Parsing(const unsigned char *cmd)
{

	if(cmd[0] == DEBUGGER_CMD_HANDSHAKE)//握手独立出去
	{
		return Deubgger_HANDSHAKE(cmd);
	}
	else if(debugger_flag)
	{
		DWORD addr = ((cmd[1] << 16) | (cmd[2] << 8) | cmd[3]);
		DWORD parameter = 0;
		// #ifdef ROM_DEBUG
		// 	printf("addr = %#x\n", addr);
		// #endif
		switch(cmd[0])
		{
			case DEBUGGER_CMD_READ_REQUEST://读
				parameter = (DWORD)(cmd[4]);
				// #ifdef ROM_DEBUG
				// 	printf("size = %#x\n", size);
				// #endif
				Deubgger_READ_REG(addr, parameter);
				break;
			case DEBUGGER_CMD_WRITE_REQUEST://写
				parameter = (cmd[7] | (cmd[6] << 8) | (cmd[5] << 16) | (cmd[4] << 24));
				// #ifdef ROM_DEBUG
				// 	printf("data = %#x\n", data);
				// #endif
				Deubgger_WRITE_REG(addr, parameter);
				break;
			case DEBUGGER_CMD_RESET_CHIP://复位
				DEBUGER_putchar(DEBUGGER_CMD_RESET_CHIP);
				SYSCTL_RST1 |= BIT(16) | BIT(15);
				_start();//如果没有芯片复位，则从这里跳入
				break;
			case DISCONNECTION_REQUEST://关闭握手，如果在rom会退出调试器功能继续执行流程
				DEBUGER_putchar(DISCONNECTION_REQUEST);
				debugger_flag = 0;
				return  -1;
			case DEBUGGER_CMD_UPDATE:
			default:
				parameter = PRINTF_FIFO;
				PRINTF_FIFO = DEBUGGER_FIFO;//接口暂时切换为DEBUGGER，执行完后恢复
				CMD_RUN(cmd);
				PRINTF_FIFO = parameter;
				break;
		}
	}
	return 1;
}
