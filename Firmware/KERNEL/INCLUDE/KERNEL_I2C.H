/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-12-21 17:40:01
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
#ifndef __KERNEL_I2C_H
#define __KERNEL_I2C_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "AE_DEBUGGER.H"
#define I2C_STANDARD 0
#define I2C_FAST 1
#define I2C_HIGH 2
#define I2C_SPEED 0
/*register offsets*/
//all registers are 8 bits width
#define I2C_REG_PRERlo          0x00
#define I2C_REG_PRERhi          0x01
#define I2C_REG_CTR             0x02
#define I2C_REG_TXR             0x03
#define I2C_REG_RXR             0x03
#define I2C_REG_CR              0x04
#define I2C_REG_SR              0x04
#define I2C_CTR_EN              (1 << 7)
#define I2C_CTR_IE              (1 << 6)
#define I2C_CR_STA              (1 << 7)
#define I2C_CR_STO              (1 << 6)
#define I2C_CR_RD               (1 << 5)
#define I2C_CR_WR               (1 << 4)
#define I2C_CR_ACK              (1 << 3)
#define I2C_CR_IACK             (1 << 0)
#define I2C_TXR_WRRD            (1 << 0)//0:write to slave; 1:read from slave;
#define I2C_SR_RXACK            (1 << 7)//0:received; 1:no ack resceived
#define I2C_SR_BUSY             (1 << 6)//0:after 'STOP' detected; 1:after 'START' detected
#define I2C_SR_AL               (1 << 5)
#define I2C_SR_TIP              (1 << 1)//0:transfer complete; 1:transfering
#define I2C_SR_IF               (1 << 0)
/*fileds*/
#if 0
#define I2C_CTR_ENABLE          1
#define I2C_CTR_DISABLE         0 
#define I2C_CTR_INTEN           1
#define I2C_CTR_INTDIS          0
#define I2C_TXR_RFS             1       //read from slave
#define I2C_TXR_WTS             0       //write to slave
#define I2C
#endif
/*API register define*/
//all registers are 8 bits width
#define I2C_REGADDR     			0x5
#define I2C_DATA_CMD_RWDIR  		0x11
#define I2C_RAW_INTR_STAT_H 		0x35
#define I2C_COMP_PARAM_1   		 	0xf4
#define I2C_TXFLR   				0x74
#define I2C_TX_ABRT_SOURCE_H    	0x81
#define I2C_CON_MASTER				0x1
#define I2C_CON_SPEED_STD			0x2
#define I2C_CON_SPEED_FAST			0x4
#define I2C_CON_SPEED_HIGH      	0x6 	
#define I2C_CON_10BITADDR_MASTER	0x10
#define I2C_CON_10BITADDR_SLAVE		0x8
#define I2C_CON_RESTART_EN			0x20
#define I2C_CON_SLAVE_DISABLE		0x40
#define I2C_REGADDR_7BIT	0x00
#define I2C_REGADDR_10BIT	0x10
#define I2C_INTR_RX_UNDER	0x01
#define I2C_INTR_RX_OVER	0x02
#define I2C_INTR_RX_FULL	0x04
#define I2C_INTR_TX_OVER	0x08
#define I2C_INTR_TX_EMPTY	0x10
#define I2C_INTR_RD_REQ		0x20
#define I2C_INTR_TX_ABRT	0x40
#define I2C_INTR_RX_DONE	0x80
#define I2C_INTR_ACTIVITY	0x100
#define I2C_INTR_STOP_DET	0x200
#define I2C_INTR_START_DET	0x400
#define I2C_INTR_GEN_CALL	0x800
#define I2C_INTR_DEFAULT_MASK		(I2C_INTR_RX_FULL | \
							 I2C_INTR_TX_EMPTY | \
							 I2C_INTR_TX_ABRT | \
							 I2C_INTR_STOP_DET)
#define I2C_STATUS_ACTIVITY	0x1
#define I2C_STATUS_TFNF		0x2
#define I2C_STATUS_TFE		0x4
#define I2C_STATUS_RFNE		0x8
#define I2C_STATUS_RFF		0x10
#define I2C_MST_ACTIVITY	0x20
#define I2C_SLV_ACTIVITY	0x40
#define I2C_RAW_STATUS_STOP_DET	0x200
#define I2C_ERR_TX_ABRT		0x40 
/*
 *  * status codes
 *   */
#define STATUS_IDLE					0x0
#define STATUS_WRITE_IN_PROGRESS	0x1
#define STATUS_READ_IN_PROGRESS		0x2
#define TIMEOUT			20 /* ms */
#define TX_EMPTY	0x10
#define RX_FULL		0x4
#define ACTIVITY    0x1
/*
 *  * hardware abort codes from the I2C_TX_ABRT_SOURCE register
 *   *
 *    * only expected abort codes are listed here
 *     * refer to the datasheet for the full list
 *      */
#define ABRT_7B_ADDR_NOACK	0
#define ABRT_10ADDR1_NOACK	1
#define ABRT_10ADDR2_NOACK	2
#define ABRT_TXDATA_NOACK	3
#define ABRT_GCALL_NOACK	4
#define ABRT_GCALL_READ		5
#define ABRT_SBYTE_ACKDET	7
#define ABRT_SBYTE_NORSTRT	9
#define ABRT_10B_RD_NORSTRT	10
#define ABRT_MASTER_DIS		11
#define ARB_LOST			12
#define I2C_TX_ABRT_7B_ADDR_NOACK	(1UL << ABRT_7B_ADDR_NOACK)
#define I2C_TX_ABRT_10ADDR1_NOACK	(1UL << ABRT_10ADDR1_NOACK)
#define I2C_TX_ABRT_10ADDR2_NOACK	(1UL << ABRT_10ADDR2_NOACK)
#define I2C_TX_ABRT_TXDATA_NOACK	(1UL << ABRT_TXDATA_NOACK)
#define I2C_TX_ABRT_GCALL_NOACK	(1UL << ABRT_GCALL_NOACK)
#define I2C_TX_ABRT_GCALL_READ	(1UL << ABRT_GCALL_READ)
#define I2C_TX_ABRT_SBYTE_ACKDET	(1UL << ABRT_SBYTE_ACKDET)
#define I2C_TX_ABRT_SBYTE_NORSTRT	(1UL << ABRT_SBYTE_NORSTRT)
#define I2C_TX_ABRT_10B_RD_NORSTRT	(1UL << ABRT_10B_RD_NORSTRT)
#define I2C_TX_ABRT_MASTER_DIS	(1UL << ABRT_MASTER_DIS)
#define I2C_TX_ARB_LOST		(1UL << ARB_LOST)
#define I2C_TX_ABRT_NOACK		(I2C_TX_ABRT_7B_ADDR_NOACK | \
							 I2C_TX_ABRT_10ADDR1_NOACK | \
							 I2C_TX_ABRT_10ADDR2_NOACK | \
							 I2C_TX_ABRT_TXDATA_NOACK | \
							 I2C_TX_ABRT_GCALL_NOACK)
//#define I2C_BASE	I2C_CTRL_ADDR            
#define TAR_GC_OR_START_START	0x400
#define TAR_SPECAIL_START	0x800
#define I2C_STOP_DET        0x2
/*****************************************/
#define SS_MIN_SCL_HIGH		4000
#define	SS_MIN_SCL_LOW		4700
#define FS_MIN_SCL_HIGH		600
#define	FS_MIN_SCL_LOW		1300
#define HS_MIN_SCL_HIGH_100PF	60
#define	HS_MIN_SCL_LOW_100PF	120
#define I2C_NO_ROLE 0
#define I2C_SLAVE_ROLE 1
#define I2C_MASTER_ROLE 2
#define I2C_CHANNEL_0 0
#define I2C_CHANNEL_1 1
#define I2C_CHANNEL_2 2
#define I2C_CHANNEL_3 3
#define I2C_CHANNEL_4 4
#define I2C_CHANNEL_5 5
/*yujiang add*/
#define I2C_CHANNEL_6 6
#define I2C_CHANNEL_7 7
#define I2C_CHANNEL_8 8
/*yujiang add*/
#define I2C_WRITE 0x0
#define I2C_READ 0x1
#define I2C_STOP 0x2
#define I2C_RESTART 0x4
#define I2CTimeOut	1000	//TimeOut set
/************************I3C******************************/

#define I3C_Master 1
#define I3C_Slave  0
//*****************************************************************************
// extern functions
//*****************************************************************************
extern WORD I2c_Channel_Baseaddr(WORD i2c_channel_num);
extern BYTE  I2c_Int_Mask(BYTE  channel, BYTE  type);
extern BYTE  I2c_Int_Unmask(BYTE  channel, BYTE  type);
extern BYTE  I2c_Int_Mask_Read(BYTE  channel, BYTE  type);
extern BYTE  I2c_Int_Status(BYTE  channel, BYTE  type);
extern BYTE I2c_Master_Read_Byte(BYTE reg, WORD i2c_channel);
extern BYTE I2cM_Read_Byte(BYTE i2c_addr, BYTE reg, WORD i2c_channel);

extern int i2c_dw_read_clear_intrbits(WORD baseaddr);
extern char I2c_Readb(WORD regoffset, WORD i2c_channel);
extern char I2c_Check_TXE(WORD i2c_channel);
extern char I2c_Check_RXF(WORD i2c_channel);
extern char I2c_Check_TFE(WORD i2c_channel);
extern char I2c_Check_TFNF(WORD i2c_channel);
extern char I2c_Check_RFNE(WORD i2c_channel);
extern char I2c_Check_Stop_Det(WORD i2c_channel);
extern char I2c_Check_Active(WORD i2c_channel);
extern char I2c_Slave_Read_Byte(WORD i2c_channel);

extern void I2c_Writeb(BYTE value, WORD regoffset, WORD i2c_channel);
extern void I2c_Write16bit(WORD value, WORD regoffset, WORD i2c_channel);
extern void I2c_Mux_set(BYTE modle);
extern void I2c_Master_Set_Tar(WORD TAR, WORD BIT, WORD i2c_channel);
extern void I2c_Master_Write_Byte(BYTE data, BYTE reg, WORD i2c_channel);
extern void I2c_Master_Write_Word(BYTE *data, BYTE reg, WORD i2c_channel);
extern void I2c_Master_Read_Word(BYTE *data, BYTE reg, WORD i2c_channel);
extern void I2c_Master_Write_Block(BYTE *data, BYTE lenth, BYTE reg, WORD i2c_channel);
extern void I2c_Master_Read_Block(BYTE *data, BYTE lenth, BYTE reg, WORD i2c_channel);
extern void I2cM_Write_Byte(BYTE i2c_addr, BYTE reg, BYTE data, WORD i2c_channel);
extern void I2cM_Write_Word(BYTE i2c_addr, BYTE *data, BYTE reg, WORD i2c_channel);
extern void I2cM_Read_Word(BYTE i2c_addr, BYTE *data, BYTE reg, WORD i2c_channel);
extern void I2cM_Write_Block(BYTE i2c_addr, BYTE *data, BYTE length, BYTE reg, WORD i2c_channel);
extern void I2cM_Read_Block(BYTE i2c_addr, BYTE *data, BYTE length, BYTE reg, WORD i2c_channel);
extern void I2c_Slave_Write_Byte(char data, WORD i2c_channel);
extern void I2c_Slave_Write_Block(char *data, BYTE  length, WORD i2c_channel);
extern void I2c_Master_Controller_Init(WORD i2c_channel, DWORD speed, BYTE spklen);
extern void I2c_Slave_Init(WORD i2c_channel, DWORD speed, BYTE spklen);
extern void I2c_Channel_Init(BYTE channel, DWORD speed, BYTE role, BYTE addr, BYTE spklen);
extern void i2c_dw_tx_abrt(WORD baseaddr);
#endif