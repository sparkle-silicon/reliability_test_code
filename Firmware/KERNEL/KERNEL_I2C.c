/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-12-21 17:46:57
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
#include "KERNEL_I2C.H"

 //*****************************************************************************
 //  I2C channel number and base address conversion function
 //
 //  parameter :
 //      I2C channel number
 //
 //  return :
 //      I2C channel baseaddr
 //*****************************************************************************
WORD I2c_Channel_Baseaddr(WORD i2c_channel)
{
	// 如果传进来是基地址那么直接返回
#if (defined(TEST101) || defined(AE101) || defined(AE102) || defined(AE103))
	if(i2c_channel <= SMBUS3_BASE_ADDR && i2c_channel >= SMBUS0_BASE_ADDR)
	{
		return (i2c_channel & 0xff00);
	}
#elif (defined(AE103))
	if(i2c_channel <= SMBUS5_BASE_ADDR && i2c_channel >= SMBUS0_BASE_ADDR)
	{
		return (i2c_channel & 0xff00);
	}
#endif
	// 根据通道选择基地址
	switch(i2c_channel)
	{
		case I2C_CHANNEL_0:
			return SMBUS0_BASE_ADDR;
		case I2C_CHANNEL_1:
			return SMBUS1_BASE_ADDR;
		case I2C_CHANNEL_2:
			return SMBUS2_BASE_ADDR;
		case I2C_CHANNEL_3:
			return SMBUS3_BASE_ADDR;
		#ifdef AE103
		case I2C_CHANNEL_4:
			return SMBUS4_BASE_ADDR;
		case I2C_CHANNEL_5:
			return SMBUS5_BASE_ADDR;
		#endif
		/*yujiang add*/
		case I2C_CHANNEL_6:
			return SMBUS6_BASE_ADDR;
		case I2C_CHANNEL_7:
			return SMBUS7_BASE_ADDR;
		case I2C_CHANNEL_8:
			return SMBUS8_BASE_ADDR;
		/*yujiang add*/
		default:
			dprint("channel or baseaddr error \n");
			return 0;
	}
}

//*****************************************************************************
//  I2C register read by byte function
//
//  parameter :
//      regoffset : Offset register
//		baseaddr : baseaddr of I2C channel
//
//  return :
//      read value
//*****************************************************************************
char I2c_Readb(WORD regoffset, WORD i2c_channel)
{
	return REG8(REG_ADDR(I2c_Channel_Baseaddr(i2c_channel), regoffset));
}

//*****************************************************************************
//  I2C register write by byte function
//
//  parameter :
//      value : value need to be write by byte
//      regoffset : Offset register
//		baseaddr : baseaddr of I2C channel
//
//  return :
//      none
//*****************************************************************************
void I2c_Writeb(BYTE value, WORD regoffset, WORD i2c_channel)
{
	REG8(REG_ADDR(I2c_Channel_Baseaddr(i2c_channel), regoffset)) = value;
}

//写16位,value为低八位
void I2c_Write16bit(WORD value, WORD regoffset, WORD i2c_channel)
{
	REG8(REG_ADDR(I2c_Channel_Baseaddr(i2c_channel), regoffset)) = value;
	REG8(REG_ADDR(I2c_Channel_Baseaddr(i2c_channel), regoffset + 1)) = (value >> 8);
}


WORD I2c_Read_Short(WORD regoffset, WORD i2c_channel)
{
	return REG16(REG_ADDR(I2c_Channel_Baseaddr(i2c_channel), regoffset));
}

//*****************************************************************************
//  I2C register write by byte function
//
//  parameter :
//      value : value need to be write by byte
//      regoffset : Offset register
//		baseaddr : baseaddr of I2C channel
//
//  return :
//      none
//*****************************************************************************
void I2c_Write_Short(WORD value, WORD regoffset, WORD i2c_channel)
{
	REG16(REG_ADDR(I2c_Channel_Baseaddr(i2c_channel), regoffset)) = value;
}
//*****************************************************************************
//  I2C Mux channel map
//
//  parameter :
//      value : the channle of smbus0 mapping
//		0: smbus0 -> sda0/scl0   1: smbus0 -> sda1/scl1  .....
//  return :
//      none
//*****************************************************************************
void I2c_Mux_set(BYTE model)
{
#if (defined(AE101) || defined(AE102))
	SYSCTL_RESERVED &= ~(3 << 9); // I2C_Mux: bit9~10  clear default: model: 0
	SYSCTL_RESERVED |= (model << 9);
#elif (defined(AE103))
	SYSCTL_RESERVED &= ~(7 << 16); // I2C_Mux: bit9~10  clear default: model: 0
	SYSCTL_RESERVED |= (model << 16);
#endif
	return;
}

//*****************************************************************************
//  I2C Sets the Slave address of the target device
//
//  parameter :
//      TAR : Slave address of the target device
//      REGADDR_BIT: SLAVE ADDRESS SIZE(I2C_REGADDR_7BIT/I2C_REGADDR_10BIT)
//  return :
//      none
//*****************************************************************************
void I2c_Master_Set_Tar(WORD TAR, WORD REGADDR_BIT, WORD i2c_channel)
{
	// I2c_Writeb(0, I2C_ENABLE_OFFSET, i2c_channel); // Disable before setup
	I2c_Write_Short(0, I2C_ENABLE_OFFSET, i2c_channel);

	if(REGADDR_BIT == I2C_REGADDR_7BIT)
	{
		// I2c_Writeb(I2C_REGADDR_7BIT, I2C_TAR_OFFSET + 1, i2c_channel);
		// I2c_Writeb(TAR & 0x7f, I2C_TAR_OFFSET, i2c_channel);  // Set TAR
	I2c_Write_Short((TAR & 0x7f) & 0x007f, I2C_TAR_OFFSET, i2c_channel);
	}
	else if(REGADDR_BIT == I2C_REGADDR_10BIT)
	{
		I2c_Writeb(TAR & 0xff, I2C_TAR_OFFSET, i2c_channel);  // Set TAR
		I2c_Writeb(I2C_REGADDR_10BIT | ((TAR >> 8) & 0x3), I2C_TAR_OFFSET + 1, i2c_channel);
	}
	// I2c_Writeb(1, I2C_ENABLE_OFFSET, i2c_channel);
	I2c_Write_Short(1, I2C_ENABLE_OFFSET, i2c_channel);
}

//*****************************************************************************
//  I2C Sets the Slave address of the device
//
//  parameter :
//      TAR : Slave address of the device
//
//  return :
//      none
//*****************************************************************************
void I2c_Slave_Set_Sar(WORD SAR, WORD i2c_channel)
{
	// I2c_Writeb(0, I2C_ENABLE_OFFSET, i2c_channel); // Disable before setup
	// I2c_Writeb(SAR & 0x7f, I2C_SAR_OFFSET, i2c_channel);  // Set SAR
	// I2c_Writeb(1, I2C_ENABLE_OFFSET, i2c_channel);

	I2c_Write_Short(0, I2C_ENABLE_OFFSET, i2c_channel); // Disable before setup
	I2c_Write_Short(SAR & 0x007f, I2C_SAR_OFFSET, i2c_channel);  // Set SAR
	I2c_Write_Short(1, I2C_ENABLE_OFFSET, i2c_channel);
}

//*****************************************************************************
//	Check I2C MASTER is not IDLE status
//	Timeout
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//*****************************************************************************
char I2c_Check_MST_ACTIVITY(WORD i2c_channel)
{
	DWORD loop = I2CTimeOut;
	while((I2c_Read_Short(I2C_STATUS_OFFSET, i2c_channel) & I2C_MST_ACTIVITY))
	{
		--loop;
		if(0 == loop)
		{
		#if I2C_DEBUG
			dprint("i2c TXE error \n");
		#endif
			return -1; // return error
		}
	}
	return 0;
}

//*****************************************************************************
//  Check I2C Send FIFO almost empty
//	The function will wait until the Send FIFO  less than TX_TL
//	Timeout
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//*****************************************************************************
char I2c_Check_TXE(WORD i2c_channel)
{
	DWORD loop = I2CTimeOut;
	// the loop will out while the txFIFO less TX_TL
	while(!(I2c_Readb(I2C_RAW_INTR_STAT_OFFSET, i2c_channel) & TX_EMPTY))
	{
		--loop;
		if(0 == loop)
		{
		#if I2C_DEBUG
			dprint("i2c TXE error \n");
		#endif
			return -1; // return error
		}
	}
	return 0;
}

//*****************************************************************************
//  Check I2C Recieve FIFO almost full
//	The function will wait until the Rx FIFO more than RX_TL
//	Timeout
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//*****************************************************************************
char I2c_Check_RXF(WORD i2c_channel)
{
	DWORD loop = I2CTimeOut;
	// loop will out while RX more than RX_TL
	while(!(I2c_Readb(I2C_RAW_INTR_STAT_OFFSET, i2c_channel) & RX_FULL))
	{
		--loop;
		if(0 == loop)
		{
		#if I2C_DEBUG
			dprint("i2c RXF error \n");
		#endif
			return -1; // return error
		}
	}
	return 0;
}

//*****************************************************************************
//  Check I2C Send FIFO empty
//	The function will wait until Send FIFO is empty
//	Timeout
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//*****************************************************************************
char I2c_Check_TFE(WORD i2c_channel)
{
	DWORD loop = I2CTimeOut;
	while(!(I2c_Read_Short(I2C_STATUS_OFFSET, i2c_channel) & I2C_STATUS_TFE)) // loop wait while Send FIFO not empty
	{
		--loop;
		if(0 == loop)
		{
		#if I2C_DEBUG
			dprint("i2c TFE error \n");
		#endif
			return -1; // return error
		}
	}
	return 0;
}

//*****************************************************************************
//  Check I2C Send FIFO full
//	The function will wait until Send FIFO is not full
//	Timeout
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//*****************************************************************************
char I2c_Check_TFNF(WORD i2c_channel)
{
	DWORD loop = I2CTimeOut;
	while(!(I2c_Readb(I2C_STATUS_OFFSET, i2c_channel) & I2C_STATUS_TFNF)) // loop wait while Send FIFO full
	{
		--loop;
		if(0 == loop)
		{
		#if I2C_DEBUG
			dprint("i2c TFNF error \n");
		#endif
			return -1; // return error
		}
	}
	return 0;
}

//*****************************************************************************
//  Check I2C Recieved FIFO empty
//	The function will wait until Recieved FIFO is not empty
//	Timeout
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//*****************************************************************************
char I2c_Check_RFNE(WORD i2c_channel)
{
	DWORD loop = I2CTimeOut;
	while(!(I2c_Readb(I2C_STATUS_OFFSET, i2c_channel) & I2C_STATUS_RFNE)) // loop wait while receive FIFO empty
	{
		--loop;
		if(0 == loop)
		{
		#if I2C_DEBUG
			dprint("i2c RFNE error \n");
		#endif
			return -1; // return error
		}
	}
	return 0;
}

//*****************************************************************************
//  Check I2C Stop signal
//	The function will wait until I2C send Stop
//	Timeout
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//*****************************************************************************
char I2c_Check_Stop_Det(WORD i2c_channel)
{
	DWORD loop = I2CTimeOut;
	while(!(I2c_Readb(I2C_RAW_INTR_STAT_H, i2c_channel) & I2C_STOP_DET))
	{
		--loop;
		if(0 == loop)
		{
		#if I2C_DEBUG
			dprint("i2c wait stop error \n");
		#endif
			return -1; // return error
		}
	}
	return 0;
}


//*****************************************************************************
//  Check if the SMBUS bus is active
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//*****************************************************************************
char I2c_Check_Active(WORD i2c_channel)
{
	DWORD loop = I2CTimeOut;
	while(I2c_Readb(I2C_STATUS_OFFSET, i2c_channel) & ACTIVITY)
	{
		--loop;
		if(0 == loop)
		{
			return -1;
		}
	}
	return 0;
}

//*****************************************************************************
//  I2C master writes byte
//
//  parameter :
//		data : need to be write
//		reg : register offset
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//*****************************************************************************
void I2c_Master_Write_Byte(BYTE data, BYTE reg, WORD i2c_channel)
{
	if(0 == I2c_Check_TFE(i2c_channel))
	{
		I2c_Writeb(reg, I2C_DATA_CMD_OFFSET, i2c_channel);
		SMB_Temp_Addr = reg;
		I2c_Writeb(I2C_WRITE, I2C_DATA_CMD_RWDIR, i2c_channel);
	}
	/*write data and stop*/
	if(0 == I2c_Check_TFE(i2c_channel))
	{
		I2c_Writeb(data, I2C_DATA_CMD_OFFSET, i2c_channel);
		SMB_Temp_Data = data;
		I2c_Writeb(I2C_WRITE | I2C_STOP, I2C_DATA_CMD_RWDIR, i2c_channel);
	}
	I2c_Check_MST_ACTIVITY(i2c_channel);
}

//*****************************************************************************
//  Set slave address and I2C master writes byte
//
//  parameter :
//		i2c_addr : slave addr
//		data : need to be write
//		reg : register offset
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//*****************************************************************************
void I2cM_Write_Byte(BYTE i2c_addr, BYTE reg, BYTE data, WORD i2c_channel)
{
	I2c_Master_Set_Tar(i2c_addr, I2C_REGADDR_7BIT, i2c_channel);
	I2c_Master_Write_Byte(data, reg, i2c_channel);
}

//*****************************************************************************
//  I2C reads byte
//
//  parameter :
//		reg : register offset
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      byte
//*****************************************************************************
BYTE I2c_Master_Read_Byte(BYTE reg, WORD i2c_channel)
{
	BYTE data;
	if(0 == I2c_Check_TFE(i2c_channel))
	{
		// I2c_Writeb(reg, I2C_DATA_CMD_OFFSET, i2c_channel);
		// I2c_Writeb(I2C_WRITE, I2C_DATA_CMD_RWDIR, i2c_channel);
		//printf("I2C_DATA_CMD_OFFSET:%x\n",REG16(REG_ADDR(I2c_Channel_Baseaddr(i2c_channel), I2C_DATA_CMD_OFFSET)));
		I2c_Write_Short(reg & 0x00ff, I2C_DATA_CMD_OFFSET, i2c_channel);
	}
	// send the addr cmd to slave
	if(0 == I2c_Check_TFE(i2c_channel))
	{
		// I2c_Writeb(I2C_READ | I2C_STOP, I2C_DATA_CMD_RWDIR, i2c_channel);
		// printf("I2C_DATA_CMD_OFFSET:%x\n",REG16(REG_ADDR(I2c_Channel_Baseaddr(i2c_channel), I2C_DATA_CMD_OFFSET)));
		I2c_Write_Short(((I2C_READ | I2C_STOP)<<8), I2C_DATA_CMD_OFFSET, i2c_channel);
	}
	// read the data from slave
	if(0 == I2c_Check_RFNE(i2c_channel))
	{
		// data = I2c_Readb(I2C_DATA_CMD_OFFSET, i2c_channel) & 0xff;
		// printf("I2C_DATA_CMD_OFFSET:%x\n",REG16(REG_ADDR(I2c_Channel_Baseaddr(i2c_channel), I2C_DATA_CMD_OFFSET)));
		data = I2c_Read_Short(I2C_DATA_CMD_OFFSET,i2c_channel) & 0xff;
	}
	I2c_Check_MST_ACTIVITY(i2c_channel);
	return data;
}

//*****************************************************************************
//  Set slave address and I2C master reads byte
//
//  parameter :
//		i2c_addr : slave addr
//		reg : register offset
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      byte
//*****************************************************************************
BYTE I2cM_Read_Byte(BYTE i2c_addr, BYTE reg, WORD i2c_channel)
{
	BYTE datal = 0;
	I2c_Master_Set_Tar(i2c_addr, I2C_REGADDR_7BIT, i2c_channel);
	datal = (I2c_Master_Read_Byte(reg, i2c_channel));
	return datal;
}

//*****************************************************************************
//  I2C writes word
//
//  parameter :
//		*data : write data buffer
//		reg : register offset
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//*****************************************************************************
void I2c_Master_Write_Word(BYTE *data, BYTE reg, WORD i2c_channel)
{
	/*write offset*/
	if(0 == I2c_Check_TFE(i2c_channel))
	{
		I2c_Writeb(reg, I2C_DATA_CMD_OFFSET, i2c_channel);
		SMB_Temp_Addr = reg;
		I2c_Writeb(I2C_WRITE, I2C_DATA_CMD_RWDIR, i2c_channel);
	}
	/*write data without stop*/
	if(0 == I2c_Check_TFE(i2c_channel))
	{
		I2c_Writeb((*data), I2C_DATA_CMD_OFFSET, i2c_channel);
		SMB_Temp_Data = *data;
		I2c_Writeb(I2C_WRITE, I2C_DATA_CMD_RWDIR, i2c_channel);
	}
	/*write data and stop*/
	if(0 == I2c_Check_TFE(i2c_channel))
	{
		I2c_Writeb((*(data + 1)), I2C_DATA_CMD_OFFSET, i2c_channel);
		SMB_Temp_Data = *(data + 1);
		I2c_Writeb(I2C_WRITE | I2C_STOP, I2C_DATA_CMD_RWDIR, i2c_channel);
	}
	I2c_Check_MST_ACTIVITY(i2c_channel);
}

//*****************************************************************************
//  Set slave address and I2C master writes byte
//
//  parameter :
//		i2c_addr : slave addr
//		*data : write data buffer
//		reg : register offset
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//*****************************************************************************
void I2cM_Write_Word(BYTE i2c_addr, BYTE *data, BYTE reg, WORD i2c_channel)
{
	I2c_Master_Set_Tar(i2c_addr, I2C_REGADDR_7BIT, i2c_channel);
	I2c_Master_Write_Word(data, reg, i2c_channel);
}

//*****************************************************************************
//  I2C reads word
//
//  parameter :
//		*data : read data buffer
//		reg : register offset
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//		none
//*****************************************************************************
void I2c_Master_Read_Word(BYTE *data, BYTE reg, WORD i2c_channel)
{
	/*write offset*/
	if(0 == I2c_Check_TFE(i2c_channel))
	{
		I2c_Writeb(reg, I2C_DATA_CMD_OFFSET, i2c_channel);
		I2c_Writeb(I2C_WRITE, I2C_DATA_CMD_RWDIR, i2c_channel);
	}
	/*read without stop*/
	I2c_Writeb(I2C_READ, I2C_DATA_CMD_RWDIR, i2c_channel);
	if(0 == I2c_Check_RFNE(i2c_channel))
	{
		*(data) = I2c_Readb(I2C_DATA_CMD_OFFSET, i2c_channel) & 0xFF;
	}
	/*read and stop*/
	I2c_Writeb(I2C_READ | I2C_STOP, I2C_DATA_CMD_RWDIR, i2c_channel);
	if(0 == I2c_Check_RFNE(i2c_channel))
	{
		*(data + 1) = I2c_Readb(I2C_DATA_CMD_OFFSET, i2c_channel) & 0xFF;
	}
	I2c_Check_MST_ACTIVITY(i2c_channel);
}

//*****************************************************************************
//  Set slave address and I2C master writes byte
//
//  parameter :
//		i2c_addr : slave addr
//		*data : read data buffer
//		reg : register offset
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//		none
//*****************************************************************************
void I2cM_Read_Word(BYTE i2c_addr, BYTE *data, BYTE reg, WORD i2c_channel)
{
	I2c_Master_Set_Tar(i2c_addr, I2C_REGADDR_7BIT, i2c_channel);
	I2c_Master_Read_Word(data, reg, i2c_channel);
}

//*****************************************************************************
//  I2C writes block
//
//  parameter :
//		*data : write data buffer
//		lenth : write length
//		reg : register offset
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//		none
//*****************************************************************************
void I2c_Master_Write_Block(BYTE *data, BYTE length, BYTE reg, WORD i2c_channel)
{
	WORD loop = 0;
	if(length)
		length--;
	else length = 1;
	if(0 == I2c_Check_TFE(i2c_channel))
	{
		I2c_Writeb(reg, I2C_DATA_CMD_OFFSET, i2c_channel);
		SMB_Temp_Addr = reg;
		I2c_Writeb(I2C_WRITE, I2C_DATA_CMD_RWDIR, i2c_channel);
	}
	/*write data without stop*/
	while(loop < length)
	{
		if(0 == I2c_Check_TFE(i2c_channel))
		{
			I2c_Writeb((*(data + loop)), I2C_DATA_CMD_OFFSET, i2c_channel);
			SMB_Temp_Data = *(data + loop);
			I2c_Writeb(I2C_WRITE, I2C_DATA_CMD_RWDIR, i2c_channel);
			loop++;
		}
	}
	/*write data and stop*/
	if(0 == I2c_Check_TFE(i2c_channel))
	{
		I2c_Writeb((*(data + loop)), I2C_DATA_CMD_OFFSET, i2c_channel);
		I2c_Writeb(I2C_WRITE | I2C_STOP, I2C_DATA_CMD_RWDIR, i2c_channel);
	}
	I2c_Check_MST_ACTIVITY(i2c_channel);
}

//*****************************************************************************
//  Set slave address and I2C master writes block
//
//  parameter :
//		i2c_addr : slave addr
//		*data : write data buffer
//		lenth : write length
//		reg : register offset
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//		none
//*****************************************************************************
void I2cM_Write_Block(BYTE i2c_addr, BYTE *data, BYTE length, BYTE reg, WORD i2c_channel)
{
	I2c_Master_Set_Tar(i2c_addr, I2C_REGADDR_7BIT, i2c_channel);
	I2c_Master_Write_Block(data, length, reg, i2c_channel);
}

//*****************************************************************************
//  I2C reads block
//
//  parameter :
//		*data : read data buffer
//		lenth : read length
//		reg : register offset
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//		none
//*****************************************************************************
void I2c_Master_Read_Block(BYTE *data, BYTE length, BYTE reg, WORD i2c_channel)
{
	WORD loop = 0;
	if(length)
		length--;
	else length = 1;
	if(0 == I2c_Check_TFE(i2c_channel))
	{
		I2c_Writeb(reg, I2C_DATA_CMD_OFFSET, i2c_channel);
		I2c_Writeb(I2C_WRITE, I2C_DATA_CMD_RWDIR, i2c_channel);
	}
	/*read without stop*/
	while(loop < length)
	{
		I2c_Writeb(I2C_READ, I2C_DATA_CMD_RWDIR, i2c_channel);
		if(0 == I2c_Check_RFNE(i2c_channel))
		{
			*(data + loop) = I2c_Readb(I2C_DATA_CMD_OFFSET, i2c_channel) & 0xFF;
			// dprint("loop = %d, data = %#x \n", loop, *(data + loop));
		}
		loop++;
	}
	/*read and stop*/
	I2c_Writeb(I2C_READ | I2C_STOP, I2C_DATA_CMD_RWDIR, i2c_channel);
	if(0 == I2c_Check_RFNE(i2c_channel))
	{
		*(data + loop) = I2c_Readb(I2C_DATA_CMD_OFFSET, i2c_channel) & 0xFF;
		//dprint("loop = %d, data = %#x \n", loop, *(data + loop));
	}
	I2c_Check_MST_ACTIVITY(i2c_channel);
}

//*****************************************************************************
//  Set slave address and I2C master reads block
//
//  parameter :
//		i2c_addr : slave addr
//		*data : read data buffer
//		lenth : read length
//		reg : register offset
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//		none
//*****************************************************************************
void I2cM_Read_Block(BYTE i2c_addr, BYTE *data, BYTE length, BYTE reg, WORD i2c_channel)
{
	I2c_Master_Set_Tar(i2c_addr, I2C_REGADDR_7BIT, i2c_channel);
	I2c_Master_Read_Block(data, length, reg, i2c_channel);
}

//*****************************************************************************
//  I2C slave writes in byte
//
//  parameter :
//		data : data to write
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//*****************************************************************************
void I2c_Slave_Write_Byte(char data, WORD i2c_channel)
{
	/*write data and stop*/
	if(0 == I2c_Check_TFE(i2c_channel))
	{
		I2c_Writeb(data, I2C_DATA_CMD_OFFSET, i2c_channel);
		I2c_Writeb(I2C_WRITE, I2C_DATA_CMD_RWDIR, i2c_channel);
	}
}

//*****************************************************************************
//  I2C slave read in byte
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      data
//*****************************************************************************
char I2c_Slave_Read_Byte(WORD i2c_channel)
{
	BYTE data = 0;
	/*write data and stop*/
	if(0 == I2c_Check_RFNE(i2c_channel))
	{
		data = (I2c_Readb(I2C_DATA_CMD_OFFSET, i2c_channel) & 0xff);
	}
	return data;
}

//*****************************************************************************
//  I2C slave write in block
//
//  parameter :
//		*data : write data buffer
//		lenth : write length
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//*****************************************************************************
void I2c_Slave_Write_Block(char *data, BYTE length, WORD i2c_channel)
{
	BYTE loop = 0;
	length--;
	/*write data*/
	while(loop <= length)
	{
		if(0 == I2c_Check_TFE(i2c_channel))
		{
			I2c_Writeb(*(data + loop), I2C_DATA_CMD_OFFSET, i2c_channel);
			I2c_Writeb(I2C_WRITE, I2C_DATA_CMD_RWDIR, i2c_channel);
			loop++;
		}
	}
}

//*****************************************************************************
//  I2C init as Master
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//*****************************************************************************
extern void I2c_Write_Short(WORD value, WORD regoffset, WORD i2c_channel);
extern WORD I2c_Read_Short(WORD regoffset, WORD i2c_channel);

void I2c_Master_Controller_Init(WORD i2c_channel, DWORD speed, BYTE spklen)
{
	short hcnt, lcnt;
	char ic_con;
	/*Disable*/
	// I2c_Writeb(DISABLE, I2C_ENABLE_OFFSET, i2c_channel);
	// ic_con = I2c_Readb(I2C_CON_OFFSET, i2c_channel);
	// hcnt = (((HIGHT_CHIP_CLOCK / speed + 1) / 2 * 1) - 9 - spklen); // PCLK/(SMBUS_HZ/2(H or L) /USER_FREQ)
	// lcnt = (((HIGHT_CHIP_CLOCK / speed + 1) / 2 * 1) - 3); // PCLK/(SMBUS_HZ/2(H or L) /USER_FREQ)
	I2c_Write_Short(DISABLE,I2C_ENABLE_OFFSET,i2c_channel);
	ic_con = I2c_Read_Short(I2C_CON_OFFSET, i2c_channel);
	hcnt = (((HIGHT_CHIP_CLOCK / speed + 1) / 2 * 1) - 9 - spklen); // PCLK/(SMBUS_HZ/2(H or L) /USER_FREQ)
	lcnt = (((HIGHT_CHIP_CLOCK / speed + 1) / 2 * 1) - 3); // PCLK/(SMBUS_HZ/2(H or L) /USER_FREQ)	
	/* SET HCNT & LCNT*/
	if(speed <= 100000)//100kHz
	{
		// I2c_Writeb(spklen, I2C_FS_SPKLEN_OFFSET, i2c_channel);
		// I2c_Write16bit(hcnt, I2C_SS_SCL_HCNT_OFFSET, i2c_channel);
		// I2c_Write16bit(lcnt, I2C_SS_SCL_LCNT_OFFSET, i2c_channel);
		// ic_con &= ~I2C_CON_SPEED_HIGH;		 // clear speed mode set
		// ic_con |= I2C_CON_SPEED_STD; // speed mode set

		I2c_Write_Short(spklen, I2C_FS_SPKLEN_OFFSET, i2c_channel);
		I2c_Write_Short(hcnt, I2C_SS_SCL_HCNT_OFFSET, i2c_channel);
		I2c_Write_Short(lcnt, I2C_SS_SCL_LCNT_OFFSET, i2c_channel);
		ic_con &= ~I2C_CON_SPEED_HIGH;		 // clear speed mode set
		ic_con |= I2C_CON_SPEED_STD; // speed mode set		
	}
	else if(speed <= 400000)//400kHz
	{
		// I2c_Writeb(spklen, I2C_FS_SPKLEN_OFFSET, i2c_channel);
		// I2c_Write16bit(hcnt, I2C_FS_SCL_HCNT_OFFSET, i2c_channel);
		// I2c_Write16bit(lcnt, I2C_FS_SCL_LCNT_OFFSET, i2c_channel);
		// ic_con &= ~I2C_CON_SPEED_HIGH;
		// ic_con |= I2C_CON_SPEED_FAST;

		I2c_Write_Short(spklen, I2C_FS_SPKLEN_OFFSET, i2c_channel);
		I2c_Write_Short(hcnt, I2C_FS_SCL_HCNT_OFFSET, i2c_channel);
		I2c_Write_Short(lcnt, I2C_FS_SCL_LCNT_OFFSET, i2c_channel);
		ic_con &= ~I2C_CON_SPEED_HIGH;
		ic_con |= I2C_CON_SPEED_FAST;
	}
	else//目前不支持
	{
		// I2c_Writeb(spklen, I2C_HS_SPKLEN_OFFSET, i2c_channel);
		// I2c_Write16bit(hcnt, I2C_HS_SCL_HCNT_OFFSET, i2c_channel);
		// I2c_Write16bit(lcnt, I2C_HS_SCL_LCNT_OFFSET, i2c_channel);
		// ic_con &= ~I2C_CON_SPEED_HIGH;
		// ic_con |= I2C_CON_SPEED_HIGH;
	}
	/* SET I2C_CON*/
	ic_con |= I2C_CON_SLAVE_DISABLE;	 // disable slave
	ic_con |= I2C_CON_MASTER;			 // set master
	ic_con &= ~I2C_CON_RESTART_EN;		 // disable restart
	ic_con &= ~I2C_CON_10BITADDR_MASTER; // 7 bit ADDR
	// I2c_Writeb(ic_con, I2C_CON_OFFSET, i2c_channel);

	I2c_Write_Short(ic_con & 0x007f, I2C_CON_OFFSET, i2c_channel);

	/* Set threshold*/
	// I2c_Writeb(4, I2C_RX_TL_OFFSET, i2c_channel);
	// I2c_Writeb(4, I2C_TX_TL_OFFSET, i2c_channel);

	I2c_Write_Short(4, I2C_RX_TL_OFFSET, i2c_channel);
	I2c_Write_Short(4, I2C_TX_TL_OFFSET, i2c_channel);

	/* Mask Interrupt */
	// I2c_Writeb(I2C_INTR_TX_ABRT, I2C_INTR_MASK_OFFSET, i2c_channel);

	I2c_Write_Short(I2C_INTR_TX_ABRT, I2C_INTR_MASK_OFFSET, i2c_channel);

	/* Enable */
	// I2c_Writeb(ENABLE, I2C_ENABLE_OFFSET, i2c_channel);

	I2c_Write_Short(ENABLE, I2C_ENABLE_OFFSET, i2c_channel);
}

//*****************************************************************************
//  I2C init as Slave
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//*****************************************************************************
void I2c_Slave_Init(WORD i2c_channel, DWORD speed, BYTE spklen)
{
	short hcnt, lcnt;
	char ic_con;
	// register WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	/*Disable*/

	// I2c_Writeb(DISABLE, I2C_ENABLE_OFFSET, i2c_channel);
	I2c_Write_Short(DISABLE,I2C_ENABLE_OFFSET,i2c_channel);

	// ic_con = I2c_Readb(I2C_CON_OFFSET, i2c_channel);
	ic_con = I2c_Read_Short(I2C_CON_OFFSET, i2c_channel);

	/* SET HCNT & LCNT*/
	hcnt = (((HIGHT_CHIP_CLOCK / speed + 1) / 2 * 1) - 9 - spklen); // PCLK/(SMBUS_HZ/2(H or L) /USER_FREQ)
	lcnt = (((HIGHT_CHIP_CLOCK / speed + 1) / 2 * 1) - 3); // PCLK/(SMBUS_HZ/2(H or L) /USER_FREQ)
	/* SET HCNT & LCNT*/
	if(speed <= 100000)//100kHz
	{
		// I2c_Writeb(spklen, I2C_FS_SPKLEN_OFFSET, i2c_channel);
		// I2c_Write16bit(hcnt, I2C_SS_SCL_HCNT_OFFSET, i2c_channel);
		// I2c_Write16bit(lcnt, I2C_SS_SCL_LCNT_OFFSET, i2c_channel);
		// ic_con &= ~I2C_CON_SPEED_HIGH;		 // clear speed mode set
		// ic_con |= I2C_CON_SPEED_STD; // speed mode set

		I2c_Write_Short(spklen, I2C_FS_SPKLEN_OFFSET, i2c_channel);
		I2c_Write_Short(hcnt, I2C_SS_SCL_HCNT_OFFSET, i2c_channel);
		I2c_Write_Short(lcnt, I2C_SS_SCL_LCNT_OFFSET, i2c_channel);
		ic_con &= ~I2C_CON_SPEED_HIGH;		 // clear speed mode set
		ic_con |= I2C_CON_SPEED_STD; // speed mode set
	}
	else if(speed <= 400000)//400kHz
	{
		// I2c_Writeb(spklen, I2C_FS_SPKLEN_OFFSET, i2c_channel);
		// I2c_Write16bit(hcnt, I2C_FS_SCL_HCNT_OFFSET, i2c_channel);
		// I2c_Write16bit(lcnt, I2C_FS_SCL_LCNT_OFFSET, i2c_channel);
		// ic_con &= ~I2C_CON_SPEED_HIGH;
		// ic_con |= I2C_CON_SPEED_FAST;

		I2c_Write_Short(spklen, I2C_FS_SPKLEN_OFFSET, i2c_channel);
		I2c_Write_Short(hcnt, I2C_FS_SCL_HCNT_OFFSET, i2c_channel);
		I2c_Write_Short(lcnt, I2C_FS_SCL_LCNT_OFFSET, i2c_channel);
		ic_con &= ~I2C_CON_SPEED_HIGH;
		ic_con |= I2C_CON_SPEED_FAST;
	}
	else//目前不支持
	{
		// I2c_Writeb(spklen, I2C_HS_SPKLEN_OFFSET, i2c_channel);
		// I2c_Write16bit(hcnt, I2C_HS_SCL_HCNT_OFFSET, i2c_channel);
		// I2c_Write16bit(lcnt, I2C_HS_SCL_LCNT_OFFSET, i2c_channel);
		// ic_con &= ~I2C_CON_SPEED_HIGH;
		// ic_con |= I2C_CON_SPEED_HIGH;
	}
	/* SET I2C_CON*/
	ic_con &= ~I2C_CON_SLAVE_DISABLE;	 // Enable slave
	ic_con &= ~I2C_CON_MASTER;			 // Disable master
	ic_con |= I2C_CON_RESTART_EN;		 // enable restart
	ic_con &= ~I2C_CON_10BITADDR_MASTER; // 7 bit ADDR

	// I2c_Writeb(ic_con, I2C_CON_OFFSET, i2c_channel);
	I2c_Write_Short(ic_con & 0x007f, I2C_CON_OFFSET, i2c_channel);

	/* Set threshold*/
	// I2c_Writeb(0, I2C_RX_TL_OFFSET, i2c_channel);
	// I2c_Writeb(15, I2C_TX_TL_OFFSET, i2c_channel);
	I2c_Write_Short(0, I2C_RX_TL_OFFSET, i2c_channel);
	I2c_Write_Short(15, I2C_TX_TL_OFFSET, i2c_channel);
	
	/* Mask Interrupt */
	// I2c_Writeb(I2C_INTR_RD_REQ | I2C_INTR_TX_ABRT, I2C_INTR_MASK_OFFSET, i2c_channel);
	I2c_Write_Short(I2C_INTR_RD_REQ | I2C_INTR_TX_ABRT, I2C_INTR_MASK_OFFSET, i2c_channel);

	/* Enable */
	// I2c_Writeb(ENABLE, I2C_ENABLE_OFFSET, i2c_channel);
	I2c_Write_Short(ENABLE, I2C_ENABLE_OFFSET, i2c_channel);
}

//*****************************************************************************
//  I2C init
//
//  parameter :
//      channel:SMBUS/IIC number
//		speed: 	SMBUS0_SPEED
//		role:	I2C_MASTER_ROLE:MASTER,I2C_SLAVE_ROLE:SLAVE
//		addr: MASTER OR SLAVE IIC ADDRESS
//		spklen: SPKlen register value
//  return :
//      none
//*****************************************************************************
void I2c_Channel_Init(BYTE channel, DWORD speed, BYTE role, BYTE addr, BYTE spklen)
{
	if(!speed)
	{
		return;
	}
	if(!spklen)
	{
		spklen = 1;
	}
	if(role == I2C_MASTER_ROLE)
	{
		I2c_Master_Controller_Init(channel, speed, spklen);
		I2c_Master_Set_Tar(addr, I2C_REGADDR_7BIT, channel);
	}
	else if(role == I2C_SLAVE_ROLE)
	{
		I2c_Slave_Init(channel, speed, spklen);
		I2c_Slave_Set_Sar(addr, channel);
	}
	else {}
}

/**
 * @brief i2c interrupts mask
 * @param channel
 * @param type
 * @return
 */
BYTE I2c_Int_Mask(BYTE channel, BYTE type)
{
	switch(channel)
	{
		case I2C_CHANNEL_0:
			I2C0_INTR_MASK &= ~(0x1 << type);
			return 0;
		case I2C_CHANNEL_1:
			I2C1_INTR_MASK &= ~(0x1 << type);
			return 0;
		case I2C_CHANNEL_2:
			I2C2_INTR_MASK &= ~(0x1 << type);
			return 0;
		case I2C_CHANNEL_3:
			I2C3_INTR_MASK &= ~(0x1 << type);
			return 0;
		#ifdef AE103
		case I2C_CHANNEL_4:
			I2C4_INTR_MASK &= ~(0x1 << type);
			return 0;
		case I2C_CHANNEL_5:
			I2C5_INTR_MASK &= ~(0x1 << type);
			return 0;
		#endif
		default:
			return -1;
	}
}

/**
 * @brief i2c interrupts unmask
 * @param channel
 * @param type
 * @return
 */
BYTE I2c_Int_Unmask(BYTE channel, BYTE type)
{
	switch(channel)
	{
		case I2C_CHANNEL_0:
			I2C0_INTR_MASK |= (0x1 << type);
			return 0;
		case I2C_CHANNEL_1:
			I2C1_INTR_MASK |= (0x1 << type);
			return 0;
		case I2C_CHANNEL_2:
			I2C2_INTR_MASK |= (0x1 << type);
			return 0;
		case I2C_CHANNEL_3:
			I2C3_INTR_MASK |= (0x1 << type);
			return 0;
		#ifdef AE103
		case I2C_CHANNEL_4:
			I2C4_INTR_MASK |= (0x1 << type);
			return 0;
		case I2C_CHANNEL_5:
			I2C5_INTR_MASK |= (0x1 << type);
			return 0;
		#endif
		default:
			return -1;
	}
}

/**
 * @brief i2c interrupts mask read
 * @param channel
 * @param type
 * @return
 */
BYTE I2c_Int_Mask_Read(BYTE channel, BYTE type)
{
	switch(channel)
	{
		case I2C_CHANNEL_0:
			return ((I2C0_INTR_MASK & (0x1 << type)) == 0);
		case I2C_CHANNEL_1:
			return ((I2C1_INTR_MASK & (0x1 << type)) == 0);
		case I2C_CHANNEL_2:
			return ((I2C2_INTR_MASK & (0x1 << type)) == 0);
		case I2C_CHANNEL_3:
			return ((I2C3_INTR_MASK & (0x1 << type)) == 0);
		#ifdef AE103
		case I2C_CHANNEL_4:
			return ((I2C4_INTR_MASK & (0x1 << type)) == 0);
		case I2C_CHANNEL_5:
			return ((I2C5_INTR_MASK & (0x1 << type)) == 0);
		#endif
		default:
			return -1;
	}
}

/**
 * @brief i2c interrupts status
 * @param channel
 * @param type
 * @return
 */
BYTE I2c_Int_Status(BYTE channel, BYTE type)
{
	switch(channel)
	{
		case I2C_CHANNEL_0:
			return ((I2C0_INTR_STAT & (0x1 << type)) != 0);
		case I2C_CHANNEL_1:
			return ((I2C1_INTR_STAT & (0x1 << type)) != 0);
		case I2C_CHANNEL_2:
			return ((I2C2_INTR_STAT & (0x1 << type)) != 0);
		case I2C_CHANNEL_3:
			return ((I2C3_INTR_STAT & (0x1 << type)) != 0);
		#ifdef AE103
		case I2C_CHANNEL_4:
			return ((I2C4_INTR_STAT & (0x1 << type)) != 0);
		case I2C_CHANNEL_5:
			return ((I2C5_INTR_STAT & (0x1 << type)) != 0);
		#endif
		default:
			return -1;
	}
}
//*****************************************************************************
//  To clear i2c interrupts
//
//  parameter :
//      baseaddr : i2c channel base address
//
//  return :
//      i2c interrupts status
//*****************************************************************************
int i2c_dw_read_clear_intrbits(WORD i2c_channel)
{
	/*
	 * The IC_INTR_STAT register just indicates "enabled" interrupts.
	 * Ths unmasked raw version of interrupt status bits are available
	 * in the IC_RAW_INTR_STAT register.
	 *
	 * That is,
	 *   stat = readl(IC_INTR_STAT);
	 * equals to,
	 *   stat = readl(IC_RAW_INTR_STAT) & readl(IC_INTR_MASK);
	 *
	 * The raw version might be useful for debugging purposes.
	 */
	int stat = I2c_Readb(I2C_INTR_STAT_OFFSET, i2c_channel);
	/*
	 * Do not use the IC_CLR_INTR register to clear interrupts, or
	 * you'll miss some interrupts, triggered during the period from
	 * readl(IC_INTR_STAT) to readl(IC_CLR_INTR).
	 *
	 * Instead, use the separately-prepared IC_CLR_* registers.
	 */
	if(stat & I2C_INTR_RX_UNDER)
		I2c_Readb(I2C_INTR_MASK_OFFSET, i2c_channel);
	if(stat & I2C_INTR_RX_OVER)
		I2c_Readb(I2C_CLR_RX_OVER_OFFSET, i2c_channel);
	if(stat & I2C_INTR_TX_OVER)
		I2c_Readb(I2C_CLR_TX_OVER_OFFSET, i2c_channel);
	if(stat & I2C_INTR_RD_REQ)
		I2c_Readb(I2C_CLR_RD_REQ_OFFSET, i2c_channel);
	if(stat & I2C_INTR_TX_ABRT)
	{
		/*
		 * The IC_TX_ABRT_SOURCE register is cleared whenever
		 * the IC_CLR_TX_ABRT is read.  Preserve it beforehand.
		 */
		// dev->abort_source = i2c_readl(I2C_TX_ABRT_SOURCE_OFFSET,i2c_channel);
		I2c_Readb(I2C_CLR_TX_ABRT_OFFSET, i2c_channel);
	}
	if(stat & I2C_INTR_RX_DONE)
		I2c_Readb(I2C_CLR_RX_DONE_OFFSET, i2c_channel);
	if(stat & I2C_INTR_ACTIVITY)
		I2c_Readb(I2C_CLR_ACTIVITY_OFFSET, i2c_channel);
	if(stat & I2C_INTR_STOP_DET)
		I2c_Readb(I2C_CLR_STOP_DET_OFFSET, i2c_channel);
	if(stat & I2C_INTR_START_DET)
		I2c_Readb(I2C_CLR_START_DET_OFFSET, i2c_channel);
	if(stat & I2C_INTR_GEN_CALL)
		I2c_Readb(I2C_CLR_GEN_CALL_OFFSET, i2c_channel);
	return stat;
}

/**
 * @brief i2c tx abort source
 * @param i2c_channel
 */
void i2c_dw_tx_abrt(WORD i2c_channel)
{
	dprint("Enter txabrt, source = %#x %#x\n", I2c_Readb(I2C_TX_ABRT_SOURCE_OFFSET, i2c_channel), I2c_Readb(I2C_TX_ABRT_SOURCE_H, i2c_channel));
}
