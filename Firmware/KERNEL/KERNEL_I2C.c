/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-20 16:48:41
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
#define SMBUS_BIT7_ADDR_MASK 0x7f
#define SMBUS_BIT10_ADDR_MASK 0x3ff
#define SMBUS_BITn_ADDR_MASK SMBUS_BIT7_ADDR_MASK
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
	if (i2c_channel <= SMBUS5_BASE_ADDR && i2c_channel >= SMBUS0_BASE_ADDR)
	{
		return (i2c_channel & 0xff00);
	}
	// 根据通道选择基地址
	switch (i2c_channel)
	{
	case I2C_CHANNEL_0:
		return SMBUS0_BASE_ADDR;
	case I2C_CHANNEL_1:
		return SMBUS1_BASE_ADDR;
	case I2C_CHANNEL_2:
		return SMBUS2_BASE_ADDR;
	case I2C_CHANNEL_3:
		return SMBUS3_BASE_ADDR;
	case I2C_CHANNEL_4:
		return SMBUS4_BASE_ADDR;
	case I2C_CHANNEL_5:
		return SMBUS5_BASE_ADDR;
	case I2C_CHANNEL_6:
		return SMBUS6_BASE_ADDR;
	case I2C_CHANNEL_7:
		return SMBUS7_BASE_ADDR;
	case I2C_CHANNEL_8:
		return SMBUS8_BASE_ADDR;
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
uint8_t I2c_Readb(WORD regoffset, WORD i2c_channel)
{
#if SMBUS_REG_SIZE==DEC8
	return SMBUSn_REG(i2c_channel, regoffset);
#elif SMBUS_REG_SIZE==DEC16
	return SMBUSn_REG(i2c_channel, (regoffset & (~1))) >> ((regoffset & 1) << 3);
#else
	return REG8(REG_ADDR(I2c_Channel_Baseaddr(i2c_channel), regoffset));
#endif
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
#if SMBUS_REG_SIZE==DEC8
	SMBUSn_REG(i2c_channel, regoffset) = value;
#elif SMBUS_REG_SIZE==DEC16
	uint32_t offset = ((regoffset & 1) << 3);
	regoffset &= (~1);
	SMBUSn_REG(i2c_channel, regoffset) = (SMBUSn_REG(i2c_channel, regoffset) & (0xff00 >> offset)) | ((uint16_t)value << offset);
#else
	REG8(REG_ADDR(I2c_Channel_Baseaddr(i2c_channel), regoffset)) = value;
#endif
}

//写16位,value为低八位
void I2c_Write16bit(WORD value, WORD regoffset, WORD i2c_channel)
{
#if SMBUS_REG_SIZE==DEC8
	SMBUSn_REG(i2c_channel, regoffset) = value;
	SMBUSn_REG(i2c_channel, (regoffset + SMBUS_REG1_OFFSET)) = (value >> 8);
#elif SMBUS_REG_SIZE==DEC16
	SMBUSn_REG(i2c_channel, regoffset) = value;
#else
	REG8(REG_ADDR(I2c_Channel_Baseaddr(i2c_channel), regoffset)) = value;
	REG8(REG_ADDR(I2c_Channel_Baseaddr(i2c_channel), regoffset + 1)) = (value >> 8);
#endif
}


WORD I2c_Read_Short(WORD regoffset, WORD i2c_channel)
{
#if SMBUS_REG_SIZE==DEC8
	return SMBUSn_REG(i2c_channel, regoffset) | (SMBUSn_REG(i2c_channel, regoffset + SMBUS_REG1_OFFSET) << 8);
#elif SMBUS_REG_SIZE==DEC16
	return SMBUSn_REG(i2c_channel, regoffset);
#else
	return REG16(REG_ADDR(I2c_Channel_Baseaddr(i2c_channel), regoffset));
#endif
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
#if SMBUS_REG_SIZE==DEC8
	SMBUSn_REG(i2c_channel, regoffset) = value;
	SMBUSn_REG(i2c_channel, (regoffset + SMBUS_REG1_OFFSET)) = (value >> 8);
#elif SMBUS_REG_SIZE==DEC16
	SMBUSn_REG(i2c_channel, regoffset) = value;
#else
	REG16(REG_ADDR(I2c_Channel_Baseaddr(i2c_channel), regoffset)) = value;
#endif
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
	// SYSCTL_CFG &= ~(7 << 16); // I2C_Mux: bit9~10  clear default: model: 0
	// SYSCTL_CFG |= (model << 16);
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
	SMBUSn_ENABLE0(i2c_channel) = DISABLE;// Disable before setup
	if (REGADDR_BIT == I2C_REGADDR_7BIT)
	{
		SMBUSn_TAR0(i2c_channel) = (TAR & SMBUS_BIT7_ADDR_MASK);// Set TAR
	}
	else if (REGADDR_BIT == I2C_REGADDR_10BIT)
	{
		SMBUSn_TAR0(i2c_channel) = (TAR & SMBUS_BIT10_ADDR_MASK) | (I2C_REGADDR_10BIT);
	}
	SMBUSn_ENABLE0(i2c_channel) = ENABLE;
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
	SMBUSn_ENABLE0(i2c_channel) = DISABLE;// Disable before setup
	SMBUSn_SAR0(i2c_channel) = SAR & SMBUS_BIT7_ADDR_MASK;  // Set SAR
	SMBUSn_ENABLE0(i2c_channel) = ENABLE;
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
	while ((SMBUSn_STATUS0(i2c_channel) & I2C_MST_ACTIVITY))
	{
		--loop;
		if (0 == loop)
		{
			i2c_dprint("i2c TXE error\n");
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

	while (!(SMBUSn_RAW_INTR_STAT0(i2c_channel) & TX_EMPTY))
	{
		--loop;
		if (0 == loop)
		{
			i2c_dprint("i2c TXE error\n");
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
	while (!(SMBUSn_RAW_INTR_STAT0(i2c_channel) & RX_FULL))
	{
		--loop;
		if (0 == loop)
		{
			i2c_dprint("i2c RXF error\n");
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
	while (!(SMBUSn_STATUS0(i2c_channel) & I2C_STATUS_TFE)) // loop wait while Send FIFO not empty
	{
		--loop;
		if (0 == loop)
		{
			i2c_dprint("i2c TFE error\n");
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

	while (!(SMBUSn_STATUS0(i2c_channel) & I2C_STATUS_TFNF)) // loop wait while Send FIFO full
	{
		--loop;
		if (0 == loop)
		{
			i2c_dprint("i2c TFNF error\n");
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
	while (!(SMBUSn_STATUS0(i2c_channel) & I2C_STATUS_RFNE)) // loop wait while receive FIFO empty
	{
		--loop;
		if (0 == loop)
		{
			i2c_dprint("i2c RFNE error\n");
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

	while (!(SMBUSn_RAW_INTR_STAT0(i2c_channel) & I2C_STOP_DET))
	{
		--loop;
		if (0 == loop)
		{
			i2c_dprint("i2c wait stop error\n");
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
	while (SMBUSn_STATUS0(i2c_channel) & ACTIVITY)
	{
		--loop;
		if (0 == loop)
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
	if (0 == I2c_Check_TFE(i2c_channel))
	{
		SMBUSn_DATA_CMD0(i2c_channel) = (I2C_WRITE) | reg;
		SMB_Temp_Addr = reg;
	}
	/*write data and stop*/
	if (0 == I2c_Check_TFE(i2c_channel))
	{
		SMBUSn_DATA_CMD0(i2c_channel) = (I2C_WRITE | I2C_STOP) | data;
		SMB_Temp_Data = data;
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
	if (0 == I2c_Check_TFE(i2c_channel))
	{
		SMBUSn_DATA_CMD0(i2c_channel) = reg | I2C_WRITE;
	}
	// send the addr cmd to slave
	if (0 == I2c_Check_TFE(i2c_channel))
	{
		SMBUSn_DATA_CMD0(i2c_channel) = I2C_READ | I2C_STOP;
	}
	// read the data from slave
	if (0 == I2c_Check_RFNE(i2c_channel))
	{
		data = SMBUSn_DATA_CMD0(i2c_channel) & 0xff;
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
void I2c_Master_Write_Word(BYTE* data, BYTE reg, WORD i2c_channel)
{
	/*write offset*/
	if (0 == I2c_Check_TFE(i2c_channel))
	{
		SMBUSn_DATA_CMD0(i2c_channel) = reg | I2C_WRITE;
		SMB_Temp_Addr = reg;
	}
	/*write data without stop*/
	if (0 == I2c_Check_TFE(i2c_channel))
	{
		SMBUSn_DATA_CMD0(i2c_channel) = data[0] | I2C_WRITE;
		SMB_Temp_Data = data[0];
	}
	/*write data and stop*/
	if (0 == I2c_Check_TFE(i2c_channel))
	{
		SMBUSn_DATA_CMD0(i2c_channel) = data[1] | I2C_WRITE | I2C_STOP;
		SMB_Temp_Data = data[1];
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
void I2cM_Write_Word(BYTE i2c_addr, BYTE* data, BYTE reg, WORD i2c_channel)
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
void I2c_Master_Read_Word(BYTE* data, BYTE reg, WORD i2c_channel)
{
	/*write offset*/
	if (0 == I2c_Check_TFE(i2c_channel))
	{
		SMBUSn_DATA_CMD0(i2c_channel) = reg | I2C_WRITE;
	}
	/*read without stop*/
	SMBUSn_DATA_CMD0(i2c_channel) = I2C_READ;
	if (0 == I2c_Check_RFNE(i2c_channel))
	{
		data[0] = SMBUSn_DATA_CMD0(i2c_channel) & 0xFF;
	}
	/*read and stop*/
	SMBUSn_DATA_CMD0(i2c_channel) = I2C_READ | I2C_STOP;
	if (0 == I2c_Check_RFNE(i2c_channel))
	{
		data[1] = SMBUSn_DATA_CMD0(i2c_channel) & 0xFF;
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
void I2cM_Read_Word(BYTE i2c_addr, BYTE* data, BYTE reg, WORD i2c_channel)
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
void I2c_Master_Write_Block(BYTE* data, BYTE length, BYTE reg, WORD i2c_channel)
{
	WORD loop = 0;
	if (length)
		length--;
	else length = 1;
	if (0 == I2c_Check_TFE(i2c_channel))
	{
		SMBUSn_DATA_CMD0(i2c_channel) = reg | I2C_WRITE;
		SMB_Temp_Addr = reg;
	}
	/*write data without stop*/
	while (loop < length)
	{
		if (0 == I2c_Check_TFE(i2c_channel))
		{
			SMBUSn_DATA_CMD0(i2c_channel) = data[loop] | I2C_WRITE;
			SMB_Temp_Data = *(data + loop);
			loop++;
		}
	}
	/*write data and stop*/
	if (0 == I2c_Check_TFE(i2c_channel))
	{
		SMBUSn_DATA_CMD0(i2c_channel) = data[loop] | I2C_WRITE | I2C_STOP;
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
void I2cM_Write_Block(BYTE i2c_addr, BYTE* data, BYTE length, BYTE reg, WORD i2c_channel)
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
void I2c_Master_Read_Block(BYTE* data, BYTE length, BYTE reg, WORD i2c_channel)
{
	WORD loop = 0;
	if (length)
		length--;
	else length = 1;
	if (0 == I2c_Check_TFE(i2c_channel))
	{
		SMBUSn_DATA_CMD0(i2c_channel) = reg | I2C_WRITE;
	}
	/*read without stop*/
	while (loop < length)
	{
		SMBUSn_DATA_CMD0(i2c_channel) = I2C_READ;
		if (0 == I2c_Check_RFNE(i2c_channel))
		{
			data[loop] = SMBUSn_DATA_CMD0(i2c_channel) & 0xFF;
			// dprint("loop = %d, data = %#x \n", loop, *(data + loop));
		}
		loop++;
	}
	/*read and stop*/
	SMBUSn_DATA_CMD0(i2c_channel) = I2C_READ | I2C_STOP;
	if (0 == I2c_Check_RFNE(i2c_channel))
	{
		data[loop] = SMBUSn_DATA_CMD0(i2c_channel) & 0xFF;
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
void I2cM_Read_Block(BYTE i2c_addr, BYTE* data, BYTE length, BYTE reg, WORD i2c_channel)
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
void I2c_Slave_Write_Byte(uint8_t data, WORD i2c_channel)
{
	/*write data and stop*/
	if (0 == I2c_Check_TFE(i2c_channel))
	{
		SMBUSn_DATA_CMD0(i2c_channel) = data | I2C_WRITE;
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
uint8_t I2c_Slave_Read_Byte(WORD i2c_channel)
{
	BYTE data = 0;
	/*write data and stop*/
	if (0 == I2c_Check_RFNE(i2c_channel))
	{
		data = (SMBUSn_DATA_CMD0(i2c_channel) & 0xff);
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
void I2c_Slave_Write_Block(uint8_t* data, BYTE length, WORD i2c_channel)
{
	BYTE loop = 0;
	length--;
	/*write data*/
	while (loop <= length)
	{
		if (0 == I2c_Check_TFE(i2c_channel))
		{
			SMBUSn_DATA_CMD0(i2c_channel) = data[loop++] | I2C_WRITE;
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
	uint16_t hcnt, lcnt;
	uint16_t control;
	/*Disable*/
	SMBUSn_ENABLE0(i2c_channel) = DISABLE;
	control = SMBUSn_CON0(i2c_channel);
	control &= ~I2C_CON_SPEED_HIGH;// clear speed mode set
	/* SET HCNT & LCNT*/
	hcnt = (((SMUBUS_CLOCK / speed + 1) / 2 * 1) - 9 - spklen); // PCLK/(SMBUS_HZ/2(H or L) /USER_FREQ)
	lcnt = (((SMUBUS_CLOCK / speed + 1) / 2 * 1) - 3); // PCLK/(SMBUS_HZ/2(H or L) /USER_FREQ)	

	if (speed <= 100000)//100kHz
	{
		SMBUSn_FS_SPKLEN0(i2c_channel) = spklen;
		SMBUSn_SS_SCL_HCNT0(i2c_channel) = hcnt;
		SMBUSn_SS_SCL_LCNT0(i2c_channel) = lcnt;
		control |= I2C_CON_SPEED_STD; // speed mode set		
	}
	else if (speed <= 400000)//400kHz
	{
		SMBUSn_FS_SPKLEN0(i2c_channel) = spklen;
		SMBUSn_FS_SCL_HCNT0(i2c_channel) = hcnt;
		SMBUSn_FS_SCL_LCNT0(i2c_channel) = lcnt;
		control |= I2C_CON_SPEED_FAST;
	}
	else//目前不支持
	{
		// SMBUSn_HS_SPKLEN0(i2c_channel) = spklen;
		// SMBUSn_HS_SCL_HCNT0(i2c_channel) = hcnt;
		// SMBUSn_HS_SCL_LCNT0(i2c_channel) = lcnt;
		// control |= I2C_CON_SPEED_HIGH;
	}
	/* SET I2C_CON*/
	control |= I2C_CON_SLAVE_DISABLE | I2C_CON_MASTER;	 // disable slave set master
	control &= ~(I2C_CON_RESTART_EN | I2C_CON_10BITADDR_MASTER);		 // disable restart and 7 bit ADDR
	control &= ~I2C_CON_10BITADDR_MASTER;
	SMBUSn_CON0(i2c_channel) = control;

	/* Set threshold*/
	SMBUSn_RX_TL0(i2c_channel) = SMBUSn_TX_TL0(i2c_channel) = 4;
	/* Mask Interrupt */
	SMBUSn_INTR_MASK0(i2c_channel) = I2C_INTR_TX_ABRT;
	/* Enable */
	SMBUSn_ENABLE0(i2c_channel) = ENABLE;
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
	uint16_t hcnt, lcnt;
	uint16_t control;
	/*Disable*/
	SMBUSn_ENABLE0(i2c_channel) = DISABLE;
	control = SMBUSn_CON0(i2c_channel);
	control &= ~I2C_CON_SPEED_HIGH;// clear speed mode set

	/* SET HCNT & LCNT*/
	hcnt = (((SMUBUS_CLOCK / speed + 1) / 2 * 1) - 9 - spklen); // PCLK/(SMBUS_HZ/2(H or L) /USER_FREQ)
	lcnt = (((SMUBUS_CLOCK / speed + 1) / 2 * 1) - 3); // PCLK/(SMBUS_HZ/2(H or L) /USER_FREQ)

	if (speed <= 100000)//100kHz
	{
		SMBUSn_FS_SPKLEN0(i2c_channel) = spklen;
		SMBUSn_SS_SCL_HCNT0(i2c_channel) = hcnt;
		SMBUSn_SS_SCL_LCNT0(i2c_channel) = lcnt;
		control |= I2C_CON_SPEED_STD; // speed mode set		
	}
	else if (speed <= 400000)//400kHz
	{
		SMBUSn_FS_SPKLEN0(i2c_channel) = spklen;
		SMBUSn_FS_SCL_HCNT0(i2c_channel) = hcnt;
		SMBUSn_FS_SCL_LCNT0(i2c_channel) = lcnt;
		control |= I2C_CON_SPEED_FAST;
	}
	else//目前不支持
	{
		// SMBUSn_HS_SPKLEN0(i2c_channel) = spklen;
		// SMBUSn_HS_SCL_HCNT0(i2c_channel) = hcnt;
		// SMBUSn_HS_SCL_LCNT0(i2c_channel) = lcnt;
		// control |= I2C_CON_SPEED_HIGH;
	}

	/* SET I2C_CON*/
	control &= ~(I2C_CON_SLAVE_DISABLE | I2C_CON_MASTER | I2C_CON_10BITADDR_MASTER);	 // Enable slave// Disable master// 7 bit ADDR
	control |= I2C_CON_RESTART_EN;		 // enable restart

	SMBUSn_CON0(i2c_channel) = control;

	/* Set threshold*/
	SMBUSn_RX_TL0(i2c_channel) = 0;
	SMBUSn_TX_TL0(i2c_channel) = 15;

	/* Mask Interrupt */
	SMBUSn_INTR_MASK0(i2c_channel) = I2C_INTR_RD_REQ | I2C_INTR_TX_ABRT | I2C_INTR_RX_FULL;
	/* Enable */
	SMBUSn_ENABLE0(i2c_channel) = ENABLE;
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
	if (!speed)
	{
		return;
	}
	if (!spklen)
	{
		spklen = 1;
	}
	if (role == I2C_MASTER_ROLE)
	{
		I2c_Master_Controller_Init(channel, speed, spklen);
		I2c_Master_Set_Tar(addr, I2C_REGADDR_7BIT, channel);
	}
	else if (role == I2C_SLAVE_ROLE)
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
	if (channel <= I2C_CHANNEL_8)
	{
		SMBUSn_INTR_MASK0(channel) &= ~(0x1 << type);
		return 0;
	}
	else
	{
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
	if (channel <= I2C_CHANNEL_8)
	{
		SMBUSn_INTR_MASK0(channel) |= (0x1 << type);
		return 0;
	}
	else
	{
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
	if (channel <= I2C_CHANNEL_8)
	{
		return ((SMBUSn_INTR_MASK0(channel) & (0x1 << type)) == 0);
	}
	else
	{
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
	if (channel <= I2C_CHANNEL_8)
	{
		return ((SMBUSn_INTR_STAT0(channel) & (0x1 << type)) != 0);
	}
	else
	{
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
#if 1
	uint16_t intr_stat = SMBUSn_INTR_STAT0(i2c_channel);
	if (intr_stat & I2C_INTR_RX_UNDER)
	{
		SMBUSn_CLR_RX_UNDER0(i2c_channel);
	}
	else if (intr_stat & I2C_INTR_RX_OVER)
	{
		SMBUSn_CLR_RX_OVER0(i2c_channel);
	}
	else if (intr_stat & I2C_INTR_RX_FULL)
	{//需要等待FIFO写出去
	}
	else if (intr_stat & I2C_INTR_TX_OVER)
	{
		SMBUSn_CLR_TX_OVER0(i2c_channel);
	}
	else if (intr_stat & I2C_INTR_TX_EMPTY)
	{//需要读FIFO
	}
	else if (intr_stat & I2C_INTR_RD_REQ)
	{
		SMBUSn_CLR_RD_REQ0(i2c_channel);
	}
	else if (intr_stat & I2C_INTR_TX_ABRT)
	{
		SMBUSn_CLR_TX_ABRT0(i2c_channel);
	}
	else if (intr_stat & I2C_INTR_RX_DONE)
	{
		SMBUSn_CLR_RX_DONE0(i2c_channel);
	}
	else if (intr_stat & I2C_INTR_ACTIVITY)
	{
		SMBUSn_CLR_ACTIVITY0(i2c_channel);
	}
	else if (intr_stat & I2C_INTR_STOP_DET)
	{
		SMBUSn_CLR_STOP_DET0(i2c_channel);
	}
	else if (intr_stat & I2C_INTR_START_DET)
	{
		SMBUSn_CLR_START_DET0(i2c_channel);
	}
	else if (intr_stat & I2C_INTR_GEN_CALL)
	{
		SMBUSn_CLR_GEN_CALL0(i2c_channel);
	}
	return intr_stat;
#else

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
	uint16_t stat = SMBUSn_INTR_STAT0(channel);
	/*
	 * Do not use the IC_CLR_INTR register to clear interrupts, or
	 * you'll miss some interrupts, triggered during the period from
	 * readl(IC_INTR_STAT) to readl(IC_CLR_INTR).
	 *
	 * Instead, use the separately-prepared IC_CLR_* registers.
	 */

	if (stat & I2C_INTR_RX_UNDER)
		SMBUSn_CLR_RX_UNDER0(i2c_channel);
	if (stat & I2C_INTR_RX_OVER)
		SMBUSn_CLR_RX_OVER0(i2c_channel);
	if (stat & I2C_INTR_TX_OVER)
		SMBUSn_CLR_TX_OVER0(i2c_channel);
	if (stat & I2C_INTR_RD_REQ)
		SMBUSn_CLR_RD_REQ0(i2c_channel);
	if (stat & I2C_INTR_TX_ABRT)
	{
		/*
		 * The IC_TX_ABRT_SOURCE register is cleared whenever
		 * the IC_CLR_TX_ABRT is read.  Preserve it beforehand.
		 */
		 // dev->abort_source = i2c_readl(I2C_TX_ABRT_SOURCE_OFFSET,i2c_channel);
		SMBUSn_CLR_TX_ABRT0(i2c_channel);
	}
	if (stat & I2C_INTR_RX_DONE)
		SMBUSn_CLR_RX_DONE0(i2c_channel);
	if (stat & I2C_INTR_ACTIVITY)
		SMBUSn_CLR_ACTIVITY0(i2c_channel);
	if (stat & I2C_INTR_STOP_DET)
		SMBUSn_CLR_STOP_DET0(i2c_channel);
	if (stat & I2C_INTR_START_DET)
		SMBUSn_CLR_START_DET0(i2c_channel);
	if (stat & I2C_INTR_GEN_CALL)
		SMBUSn_CLR_GEN_CALL0(i2c_channel);
	return stat;
#endif
}

/**
 * @brief i2c tx abort source
 * @param i2c_channel
 */
void i2c_dw_tx_abrt(WORD i2c_channel)
{
	dprint("Enter txabrt, source = %#x %#x\n", SMBUSn_TX_ABRT_SOURCE0(i2c_channel));
}