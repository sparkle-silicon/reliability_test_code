/*
 * @Author: Maple
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-06-13 19:04:42
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
#include "TEST_AUTOMATIC.H"
#if 0//(defined(AE102) || defined(AE103))
void pwm_pmc_config(BYTE dcdr)
{
	PWM_Init_channel(0, 0, 0, 0, dcdr, 0);
}
BYTE Read_MapECSpace(BYTE MapIndex)
{
	Tmp_XPntr = (VBYTEP)(ECSPACE_BASE_ADDR | MapIndex);
	return (*Tmp_XPntr);
}
void Write_MapECSpace(BYTE MapIndex, BYTE data1)
{
	Tmp_XPntr = (VBYTEP)(ECSPACE_BASE_ADDR | MapIndex);
	*Tmp_XPntr = data1;
}
void AUTO_TEST_PORT(BYTE channel)
{
	volatile BYTE var;
	BYTE cmd;
	BYTE index;
	BYTE timeout;
	WORD tach0;
	switch(channel)
	{
		case 0x0:
			if(KBC_STA & 0x08)
			{
				cmd = KBC_IB;
				while(KBC_STA & 0x1)
					;
				KBC_KOB = cmd;
			#if LPC_WAY_OPTION_SWITCH
			#else
				SET_BIT(KBC_CTL, 3);
			#endif
				switch(cmd)
				{
					case 0x03: // I2C0
						// I2c_Readb(I2C_CLR_STOP_DET_OFFSET,I2C_CHANNEL_0);//clear stop det
						/* Master switch and TAR set */
						I2c_Master_Controller_Init(I2C_CHANNEL_0, I2C0_SPEED, 1);
						I2c_Master_Set_Tar(0x4C, I2C_REGADDR_7BIT, I2C_CHANNEL_0);
						/* read temperature */
						var = I2c_Master_Read_Byte(0x1, I2C_CHANNEL_0);
						while(KBC_STA & 0x1)
							;
						KBC_KOB = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(KBC_CTL, 3);
					#endif
						var = I2c_Master_Read_Byte(0x10, I2C_CHANNEL_0);
						while(KBC_STA & 0x1)
							;
						KBC_KOB = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(KBC_CTL, 3);
					#endif
									// if(0 == I2c_Check_Stop_Det(I2C_CHANNEL_0))
									// {
									// 	I2c_Slave_debugger_Init(I2C_CHANNEL_0);
									// }
						break;
					default:
						break;
				}
			}
			else
			{
				var = KBC_IB;
				while(KBC_STA & KBC_STA_OBF)
					;
				KBC_MOB = var;
			#if LPC_WAY_OPTION_SWITCH
			#else
				SET_BIT(KBC_CTL, 3);
			#endif
			}
			break;
		case 0x1:
			if(PMC1_STR & 0x08)
			{
				cmd = PMC1_DIR;
				// dprint("PM1 Cmd = %d\n",cmd);
				while(PMC1_STR & 0x1)
					;
				PMC1_DOR = cmd;
			#if LPC_WAY_OPTION_SWITCH
			#else
				SET_BIT(PMC1_CTL, 0);
			#endif
				switch(cmd)
				{
					case 0x01: // UART
						UART0_IER = 0x0;
						while(!(PMC1_STR & 0x2))
							;
						var = PMC1_DIR;
						while(!(UART0_LSR & UART_LSR_TEMP))
							;
						UART0_TX = var; // 写
						timeout = 251;
						while(--timeout)
						{
							if(UART0_LSR & UART_LSR_DR)
								break;
							vDelayXus(1);
						}
						if(timeout)
							var = UART0_RX; // 读
						else
							var = 0x80;
						// dprint("timeout = %d\n",timeout);
						while(PMC1_STR & 0x1)
							;
						PMC1_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC1_CTL, 0);
					#endif
						break;
					case 0x03: // I2C1
						// I2c_Readb(I2C_CLR_STOP_DET_OFFSET,I2C_CHANNEL_1);//clear stop det
						/* Master switch and TAR set */
						I2c_Master_Controller_Init(I2C_CHANNEL_1, I2C1_SPEED, 1);
						I2c_Master_Set_Tar(0x4C, I2C_REGADDR_7BIT, I2C_CHANNEL_1);
						/* read temperature */
						var = I2c_Master_Read_Byte(0x1, I2C_CHANNEL_1);
						while(PMC1_STR & 0x1)
							;
						PMC1_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC1_CTL, 0);
					#endif
						var = I2c_Master_Read_Byte(0x10, I2C_CHANNEL_1);
						while(PMC1_STR & 0x1)
							;
						PMC1_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC1_CTL, 0);
					#endif
									// if(0 == I2c_Check_Stop_Det(I2C_CHANNEL_1))
									// {
									// 	I2c_Slave_debugger_Init(I2C_CHANNEL_1);
									// }
						break;
					case 0x05: // CAN0 CAN1
						while(!(PMC1_STR & 0x2))
							;
						var = PMC1_DIR;
						sysctl_iomux_can0();
						sysctl_iomux_can1();
						CAN0_Init(_125kbs, 0x123, 0xffff8000);
						CAN1_Init(_125kbs, 0x123, 0xffff8000);
						CAN0_SendData(Standard_frame, 0x123, var, CAN_DATA);
						CAN1_SendData(Standard_frame, 0x123, var, CAN_DATA);
						var = 0;
						while(!CAN1_RMC)
							;
						index = CAN1_RXB0 & 0xf;
						for(BYTE i = 0; i < index; i++)
						{
							if(CAN_DATA[i] == (*((VBYTEP)(CAN1_REG_ADDR(CAN_RXB5_OFFECT + i)))))
								var++;
						}
						CAN1_CMR = (0x04);
						while(!CAN0_RMC)
							;
						index = CAN0_RXB0 & 0xf;
						for(BYTE i = 0; i < index; i++)
						{
							if(CAN_DATA[i] == (*((VBYTEP)(CAN0_REG_ADDR(CAN_RXB5_OFFECT + i)))))
								var++;
						}
						CAN0_CMR = (0x04);
						while(PMC1_STR & 0x1)
							;
						PMC1_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC1_CTL, 0);
					#endif
						break;
					default:
						break;
				}
			}
			else
			{
				var = PMC1_DIR;
				while(PMC1_STR & 0x1)
					;
				PMC1_DOR = var;
			#if LPC_WAY_OPTION_SWITCH
			#else
				SET_BIT(PMC1_CTL, 0);
			#endif
			}
			break;
		case 0x2:
			if(PMC2_STR & 0x08)
			{
				cmd = PMC2_DIR;
				// dprint("PM2 Cmd = %d\n",cmd);
				while(PMC2_STR & 0x1)
					;
				PMC2_DOR = cmd;
			#if LPC_WAY_OPTION_SWITCH
			#else
				SET_BIT(PMC2_CTL, 0);
			#endif
				switch(cmd)
				{
					case 0x01: // UART2
						UART2_IER = 0x0;
						while(!(PMC2_STR & 0x2))
							;
						var = PMC2_DIR;
						while(!(UART2_LSR & UART_LSR_TEMP))
							;
						UART2_TX = var; // 写
						timeout = 251;
						while(--timeout)
						{
							if(UART2_LSR & UART_LSR_DR)
								break;
							vDelayXus(1);
						}
						if(timeout)
							var = UART2_RX; // 读
						else
							var = 0x80;
						// dprint("timeout = %d\n",timeout);
						while(PMC2_STR & 0x1)
							;
						PMC2_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC2_CTL, 0);
					#endif
						break;
					case 0x03: // I2C2
						// I2c_Readb(I2C_CLR_STOP_DET_OFFSET,I2C_CHANNEL_2);//clear stop det
						/* Master switch and TAR set */
						I2c_Master_Controller_Init(I2C_CHANNEL_2, I2C2_SPEED, 1);
						I2c_Master_Set_Tar(0x4C, I2C_REGADDR_7BIT, I2C_CHANNEL_2);
						/* read temperature */
						var = I2c_Master_Read_Byte(0x1, I2C_CHANNEL_2);
						while(PMC2_STR & 0x1)
							;
						PMC2_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC2_CTL, 0);
					#endif
						var = I2c_Master_Read_Byte(0x10, I2C_CHANNEL_2);
						while(PMC2_STR & 0x1)
							;
						PMC2_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC2_CTL, 0);
					#endif
									// if(0 == I2c_Check_Stop_Det(I2C_CHANNEL_2))
									// {
									// 	I2c_Slave_debugger_Init(I2C_CHANNEL_2);
									// }
						break;
					case 0x05: // CAN2 CAN3
						while(!(PMC2_STR & 0x2))
							;
						var = PMC2_DIR;
						sysctl_iomux_can0();
						sysctl_iomux_can1();
						CAN2_Init(_125kbs, 0x123, 0xffff8000);
						CAN3_Init(_125kbs, 0x123, 0xffff8000);
						CAN2_SendData(Standard_frame, 0x123, var, CAN_DATA);
						CAN3_SendData(Standard_frame, 0x123, var, CAN_DATA);
						var = 0;
						while(!CAN3_RMC)
							;
						index = CAN3_RXB0 & 0xf;
						for(BYTE i = 0; i < index; i++)
						{
							if(CAN_DATA[i] == (*((VBYTEP)(CAN3_REG_ADDR(CAN_RXB5_OFFECT + i)))))
								var++;
						}
						CAN3_CMR = (0x04);
						while(!CAN2_RMC)
							;
						index = CAN2_RXB0 & 0xf;
						for(BYTE i = 0; i < index; i++)
						{
							if(CAN_DATA[i] == (*((VBYTEP)(CAN2_REG_ADDR(CAN_RXB5_OFFECT + i)))))
								var++;
						}
						CAN2_CMR = (0x04);
						while(PMC2_STR & 0x1)
							;
						PMC2_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC2_CTL, 0);
					#endif
						break;
					default:
						break;
				}
			}
			else
			{
				var = PMC2_DIR;
				while(PMC2_STR & 0x1)
					;
				PMC2_DOR = var;
			#if LPC_WAY_OPTION_SWITCH
			#else
				SET_BIT(PMC2_CTL, 0);
			#endif
			}
			break;
		case 0x3:
			if(PMC3_STR & 0x08)
			{
				cmd = PMC3_DIR;
				// dprint("PM3 Cmd = %d\n",cmd);
				while(PMC3_STR & 0x1)
					;
				PMC3_DOR = cmd;
			#if LPC_WAY_OPTION_SWITCH
			#else
				SET_BIT(PMC3_CTL, 0);
			#endif
				switch(cmd)
				{
					case 0x01: // UART3
						UART3_IER = 0x0;
						while(!(PMC3_STR & 0x2))
							;
						var = PMC3_DIR;
						while(!(UART3_LSR & UART_LSR_TEMP))
							;
						UART3_TX = var; // 写
						timeout = 251;
						while(--timeout)
						{
							if(UART3_LSR & UART_LSR_DR)
								break;
							vDelayXus(1);
						}
						if(timeout)
							var = UART3_RX; // 读
						else
							var = 0x80;
						// dprint("timeout = %d\n",timeout);
						while(PMC3_STR & 0x1)
							;
						PMC3_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC3_CTL, 0);
					#endif
						break;
					case 0x03: // I2C3
						// I2c_Readb(I2C_CLR_STOP_DET_OFFSET,I2C_CHANNEL_3);//clear stop det
						/* Master switch and TAR set */
						I2c_Master_Controller_Init(I2C_CHANNEL_3, I2C2_SPEED, 1);
						I2c_Master_Set_Tar(0x4C, I2C_REGADDR_7BIT, I2C_CHANNEL_3);
						/* read temperature */
						var = I2c_Master_Read_Byte(0x1, I2C_CHANNEL_3);
						while(PMC3_STR & 0x1)
							;
						PMC3_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC3_CTL, 0);
					#endif
						var = I2c_Master_Read_Byte(0x10, I2C_CHANNEL_3);
						while(PMC3_STR & 0x1)
							;
						PMC3_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC3_CTL, 0);
					#endif
									// if(0 == I2c_Check_Stop_Det(I2C_CHANNEL_3))
									// {
									// 	I2c_Slave_debugger_Init(I2C_CHANNEL_3);
									// }
						break;
					default:
						break;
				}
			}
			else
			{
				var = PMC3_DIR;
				while(PMC3_STR & 0x1)
					;
				PMC3_DOR = var;
			#if LPC_WAY_OPTION_SWITCH
			#else
				SET_BIT(PMC3_CTL, 0);
			#endif
			}
			break;
		case 0x4:
			if(PMC4_STR & 0x08)
			{
				cmd = PMC4_DIR;
				// dprint("PM4 Cmd = %d\n",cmd);
				while(PMC4_STR & 0x1)
					;
				PMC4_DOR = cmd;
			#if LPC_WAY_OPTION_SWITCH
			#else
				SET_BIT(PMC4_CTL, 0);
			#endif
				switch(cmd)
				{
					case 0x01: // UART1
						UART1_IER = 0x0;
						while(!(PMC4_STR & 0x2))
							;
						var = PMC4_DIR;
						while(!(UART1_LSR & UART_LSR_TEMP))
							;
						UART1_TX = var; // 写
						timeout = 251;
						while(--timeout)
						{
							if(UART1_LSR & UART_LSR_DR)
								break;
							vDelayXus(1);
						}
						if(timeout)
							var = UART1_RX; // 读
						else
							var = 0x80;
						// dprint("timeout = %d\n",timeout);
						while(PMC4_STR & 0x1)
							;
						PMC4_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC4_CTL, 0);
					#endif
						break;
					case 0x02: // GPIOB
						SYSCTL_PIO2_CFG = 0x0;
						SYSCTL_PIO3_CFG = 0x0;
						while(!(PMC4_STR & 0x2))
							; // 0-7
						var = PMC4_DIR;
						index = 0x0;
						GPIO1_DR0 = var ? 0x00 : 0xff;
						GPIO1_DDR0 = 0x55;
						GPIO1_DR0 = var ? 0x55 : 0xaa;
						index |= (GPIO1_EXT0 ^ (var ? 0xff : 0x0)) & 0xaa;
						GPIO1_DR0 = var ? 0x00 : 0xff;
						GPIO1_DDR0 = 0xaa;
						GPIO1_DR0 = var ? 0xaa : 0x55;
						index |= (GPIO1_EXT0 ^ (var ? 0xff : 0x0)) & 0x55;
						while(PMC4_STR & 0x1)
							;
						PMC4_DOR = index;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC4_CTL, 0);
					#endif
						while(!(PMC4_STR & 0x2))
							; // 8-15
						var = PMC4_DIR;
						index = 0x0;
						GPIO1_DR1 = var ? 0x00 : 0xff;
						GPIO1_DDR1 = 0x55;
						GPIO1_DR1 = var ? 0x55 : 0xaa;
						index |= (GPIO1_EXT1 ^ (var ? 0xff : 0x0)) & 0xaa;
						// dprint("%d.ddr = %#x, dr = %#x, ext = %#x, ext^^ = %#x, index = %#x\n", var, GPIO0_DDR0, GPIO0_DR0, GPIO0_EXT0, (GPIO0_EXT0 ^ (var ? 0xff : 0x0)), index);
						GPIO1_DR1 = var ? 0x00 : 0xff;
						GPIO1_DDR1 = 0xaa;
						GPIO1_DR1 = var ? 0xaa : 0x55;
						index |= (GPIO1_EXT1 ^ (var ? 0xff : 0x0)) & 0x55;
						// dprint("%d.ddr = %#x, dr = %#x, ext = %#x, ext^^ = %#x, index = %#x\n", var, GPIO0_DDR0, GPIO0_DR0, GPIO0_EXT0, (GPIO0_EXT0 ^ (var ? 0xff : 0x0)), index);
						while(PMC4_STR & 0x1)
							;
						PMC4_DOR = index;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC4_CTL, 0);
					#endif
						while(!(PMC4_STR & 0x2))
							; // 16-23
						var = PMC4_DIR;
						index = 0x0;
						GPIO1_DR2 = var ? 0x00 : 0xff;
						GPIO1_DDR2 = 0x55;
						GPIO1_DR2 = var ? 0x55 : 0xaa;
						index |= (GPIO1_EXT2 ^ (var ? 0xff : 0x0)) & 0xaa;
						GPIO1_DR2 = var ? 0x00 : 0xff;
						GPIO1_DDR2 = 0xaa;
						GPIO1_DR2 = var ? 0xaa : 0x55;
						index |= (GPIO1_EXT2 ^ (var ? 0xff : 0x0)) & 0x55;
						while(PMC4_STR & 0x1)
							;
						PMC4_DOR = index;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC4_CTL, 0);
					#endif
						while(!(PMC4_STR & 0x2))
							; // 24-31
						var = PMC4_DIR;
						index = 0x0;
						GPIO1_DR3 = var ? 0x00 : 0xff;
						GPIO1_DDR3 = 0x55;
						GPIO1_DR3 = var ? 0x55 : 0xaa;
						index |= (GPIO1_EXT3 ^ (var ? 0xff : 0x0)) & 0xaa;
						GPIO1_DR3 = var ? 0x00 : 0xff;
						GPIO1_DDR3 = 0xaa;
						GPIO1_DR3 = var ? 0xaa : 0x55;
						index |= (GPIO1_EXT3 ^ (var ? 0xff : 0x0)) & 0x55;
						while(PMC4_STR & 0x1)
							;
						PMC4_DOR = index;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC4_CTL, 0);
					#endif
						break;
					default:
						break;
				}
			}
			else
			{
				var = PMC4_DIR;
				while(PMC4_STR & 0x1)
					;
				PMC4_DOR = var;
			#if LPC_WAY_OPTION_SWITCH
			#else
				SET_BIT(PMC4_CTL, 0);
			#endif
			}
			break;
		case 0x5:
			if(PMC5_STR & 0x08)
			{
				cmd = PMC5_DIR;
				// dprint("PM5 Cmd = %d\n",cmd);
				while(PMC5_STR & 0x1)
					;
				PMC5_DOR = cmd;
			#if LPC_WAY_OPTION_SWITCH
			#else
				SET_BIT(PMC5_CTL, 0);
			#endif
				switch(cmd)
				{
					case 0x80: // ec space read
						while(!(PMC5_STR & 0x2))
							;
						index = PMC5_DIR;
						while(PMC5_STR & 0x1)
							;
						var = Read_MapECSpace(index);
						PMC5_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						dprint("Read_MapECSpace(%d) = %d\n", index, var);
						break;
					case 0x81: // ec space write
						while(!(PMC5_STR & 0x2))
							;
						index = PMC5_DIR;
						while(PMC5_STR & 0x1)
							;
						PMC5_DOR = index;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						while(!(PMC5_STR & 0x2))
							;
						var = PMC5_DIR;
						Write_MapECSpace(index, var);
						var = Read_MapECSpace(index);
						while(PMC5_STR & 0x1)
							;
						PMC5_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						dprint("Write_MapECSpace(%d) = %d\n", index, var);
						break;
					case 0x02: // GPIOA
						SYSCTL_PIO0_CFG = 0x0;
						SYSCTL_PIO1_CFG = 0x0;
						while(!(PMC5_STR & 0x2))
							; // 0-7
						var = PMC5_DIR;
						index = 0x0;
						GPIO0_DR0 = var ? 0x00 : 0xff;
						GPIO0_DDR0 = 0x55;
						GPIO0_DR0 = var ? 0x55 : 0xaa;
						index |= (GPIO0_EXT0 ^ (var ? 0xff : 0x0)) & 0xaa;
						// dprint("%d.ddr = %#x, dr = %#x, ext = %#x, ext^^ = %#x, index = %#x\n", var, GPIO0_DDR0, GPIO0_DR0, GPIO0_EXT0, (GPIO0_EXT0 ^ (var ? 0xff : 0x0)), index);
						GPIO0_DR0 = var ? 0x00 : 0xff;
						GPIO0_DDR0 = 0xaa;
						GPIO0_DR0 = var ? 0xaa : 0x55;
						index |= (GPIO0_EXT0 ^ (var ? 0xff : 0x0)) & 0x55;
						// dprint("%d.ddr = %#x, dr = %#x, ext = %#x, ext^^ = %#x, index = %#x\n", var, GPIO0_DDR0, GPIO0_DR0, GPIO0_EXT0, (GPIO0_EXT0 ^ (var ? 0xff : 0x0)), index);
						while(PMC5_STR & 0x1)
							;
						PMC5_DOR = index;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						while(!(PMC5_STR & 0x2))
							; // 8-15
						var = PMC5_DIR;
						index = 0x0;
						GPIO0_DR1 = var ? 0x00 : 0xff;
						GPIO0_DDR1 = 0x55;
						GPIO0_DR1 = var ? 0x55 : 0xaa;
						index |= (GPIO0_EXT1 ^ (var ? 0xff : 0x0)) & 0xaa;
						// dprint("%d.ddr = %#x, dr = %#x, ext = %#x, ext^^ = %#x, index = %#x\n", var, GPIO0_DDR0, GPIO0_DR0, GPIO0_EXT0, (GPIO0_EXT0 ^ (var ? 0xff : 0x0)), index);
						GPIO0_DR1 = var ? 0x00 : 0xff;
						GPIO0_DDR1 = 0xaa;
						GPIO0_DR1 = var ? 0xaa : 0x55;
						index |= (GPIO0_EXT1 ^ (var ? 0xff : 0x0)) & 0x55;
						// dprint("%d.ddr = %#x, dr = %#x, ext = %#x, ext^^ = %#x, index = %#x\n", var, GPIO0_DDR0, GPIO0_DR0, GPIO0_EXT0, (GPIO0_EXT0 ^ (var ? 0xff : 0x0)), index);
						while(PMC5_STR & 0x1)
							;
						PMC5_DOR = index;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						while(!(PMC5_STR & 0x2))
							; // 16-23
						var = PMC5_DIR;
						index = 0x0;
						GPIO0_DR2 = var ? 0x00 : 0xff;
						GPIO0_DDR2 = 0x55;
						GPIO0_DR2 = var ? 0x55 : 0xaa;
						index |= (GPIO0_EXT2 ^ (var ? 0xff : 0x0)) & 0xaa;
						// dprint("%d.ddr = %#x, dr = %#x, ext = %#x, ext^^ = %#x, index = %#x\n", var, GPIO0_DDR0, GPIO0_DR0, GPIO0_EXT0, (GPIO0_EXT0 ^ (var ? 0xff : 0x0)), index);
						GPIO0_DR2 = var ? 0x00 : 0xff;
						GPIO0_DDR2 = 0xaa;
						GPIO0_DR2 = var ? 0xaa : 0x55;
						index |= (GPIO0_EXT2 ^ (var ? 0xff : 0x0)) & 0x55;
						// dprint("%d.ddr = %#x, dr = %#x, ext = %#x, ext^^ = %#x, index = %#x\n", var, GPIO0_DDR0, GPIO0_DR0, GPIO0_EXT0, (GPIO0_EXT0 ^ (var ? 0xff : 0x0)), index);
						while(PMC5_STR & 0x1)
							;
						PMC5_DOR = index;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						while(!(PMC5_STR & 0x2))
							; // 24-31
						var = PMC5_DIR;
						index = 0x0;
						GPIO0_DR3 = var ? 0x00 : 0xff;
						GPIO0_DDR3 = 0x55;
						GPIO0_DR3 = var ? 0x55 : 0xaa;
						index |= (GPIO0_EXT3 ^ (var ? 0xff : 0x0)) & 0xaa;
						// dprint("%d.ddr = %#x, dr = %#x, ext = %#x, ext^^ = %#x, index = %#x\n", var, GPIO0_DDR0, GPIO0_DR0, GPIO0_EXT0, (GPIO0_EXT0 ^ (var ? 0xff : 0x0)), index);
						GPIO0_DR3 = var ? 0x00 : 0xff;
						GPIO0_DDR3 = 0xaa;
						GPIO0_DR3 = var ? 0xaa : 0x55;
						index |= (GPIO0_EXT3 ^ (var ? 0xff : 0x0)) & 0x55;
						// dprint("%d.ddr = %#x, dr = %#x, ext = %#x, ext^^ = %#x, index = %#x\n", var, GPIO0_DDR0, GPIO0_DR0, GPIO0_EXT0, (GPIO0_EXT0 ^ (var ? 0xff : 0x0)), index);
						while(PMC5_STR & 0x1)
							;
						PMC5_DOR = index;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						break;
					case 0x04: // PWM/tach
						while(!(PMC5_STR & 0x2))
							;
						index = PMC5_DIR;
						while(PMC5_STR & 0x1)
							;
						PMC5_DOR = index;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						while(!(PMC5_STR & 0x2))
							;
						var = PMC5_DIR;
						if(index == 0x00)
						{
							pwm_pmc_config(var);
							sysctl_iomux_pwm0();
						}
						else
						{
							pwm_pmc_config(var);
							sysctl_iomux_pwm0();
						}
						sysctl_iomux_tach0();
						TACH_Init_Channel(0, 1, 0);
						(*(DWORD *)(0x3047c)) = 1 << 15; // GPIOD 6 pullup
						tach0 = 0x00;
						tach0 = 0x5b8d80 / TACH0_DATA;
						dprint("tach0 = %d\n", tach0);
						while(PMC5_STR & 0x1)
							;
						PMC5_DOR = (BYTE)(tach0 & (0xff));
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						while(PMC5_STR & 0x1)
							;
						PMC5_DOR = (BYTE)((tach0 & (0xff00)) >> 8);
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						break;
					case 0x5A:
					case 0xA5:
					default:
						break;
				}
			}
			else
			{
				var = PMC5_DIR;
				// dprint("PM5 DATA = %d\n",var);
				while(PMC5_STR & 0x1)
					;
				PMC5_DOR = var;
			#if LPC_WAY_OPTION_SWITCH
			#else
				SET_BIT(PMC5_CTL, 0);
			#endif
			}
			break;
		default:
			break;
	}
}
#endif