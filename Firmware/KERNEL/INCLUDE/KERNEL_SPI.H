/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-22 18:40:28
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
#ifndef KERNEL_SPI_H_
#define KERNEL_SPI_H_
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"

#define SPIM_CLOCK  (HIGH_CHIP_CLOCK / (SYSCTL_CLKDIV_SPIM+1)) 

#define SPIM_CPOL_HIGH HIGH
#define SPIM_CPOL_LOW LOW
#define SPIM_CPHA_FE 0
#define SPIM_CPHA_SE 1
#define SPIM_MSB 0
#define SPIM_LSB 1
 /* Common commands */
#define CMD_READ_ID			0x9f
#define CMD_READ_STATUS1    0x05   //S0-S7状态
#define CMD_READ_STATUS2    0x35   //S8-S15状态
#define CMD_WRITE_ENABLE    0x06   //写使能
#define CMD_READ_DATA       0x03   //读数据
#define CMD_BLOCK_ERASE_64  0xd8   //Block Erase (64KB)
#define CMD_SECTOR_ERASE    0x20    //扇区擦除指令
#define CMD_CHIP_ERASE      0xc7   //芯片擦除
#define CMD_PAGE_PROGRAM    0x02   //页写入
#define CMD_Status_Register 0x01   //写FLASH状态寄存器指令
#define CMD_Quad_Input_Page_Program 0x32    //Quad页编程指令
#define CMD_Fast_Read_Quad 0x6B             //Quad模式快速读取
#define CMD_Fast_Read_Dual 0x3B             //Dual模式快速读取
//#define CMD_READ_ID			0x15
#define CMD_READ_ARRAY_SLOW		0x03
#define CMD_READ_ARRAY_FAST		0x0b
#define CMD_READ_ARRAY_LEGACY		0xe8
/*---------------------------------SPIM---------------------------------------*/
#define ENABLE              HEX01
#define DISABLE             HEX00
/************中断屏蔽宏***************/
#define txeim				HEX01 //发送FIFO空中断屏蔽
#define txoim				HEX02 //发送FIFO上溢中断屏蔽
#define rxuim				HEX04 //接收FIFO下溢中断屏蔽
#define rxoim				HEX08 //接收FIFO溢出中断屏蔽
#define rxfim				HEX10 //接收FIFO满中断屏蔽
#define rdovm				HEX20 //RDNUM接收完毕中断屏蔽位
#define errm				HEX40 //错误中断屏蔽位
/************SPI Register define***************/
#define SPIFE_CTL_FOUR       HEX02
#define SPIFE_CTL_TWO        HEX00
#define SPIFE_SUSPEND_EN     HEX01
#define SPIFE_SUSPEND_DISEN  HEX00
/* SPI 状态寄存器位定 */
#define SPI_BSY      HEX10 // SPI-M busy标志位，0x0 表示空闲，0x1 表示传输中
#define SPI_RFF      HEX08 // 接收FIFO满标志位，0x0 表示不满，0x1 表示满
#define SPI_RFNE     HEX04 // 接收FIFO不空标志位，0x0 表示为空，0x1 表示不为空
#define SPI_TFE      HEX02 // 发送FIFO空标志位，0x0 表示不空，0x1 表示为空
#define SPI_TFNF     HEX01 // 发送FIFO不满标志位，0x0 表示满，0x1 表示未满
/************SPI MODE***************/
#define SPI_Standard HEX00
#define SPI_Dual     HEX01
#define SPI_Quad     HEX02
#define SPI_3Wire    HEX03


extern BYTE SPI_Write_Buff[256];
extern void SPIM_Init(BYTE dly, BYTE cpol, BYTE cpha, BYTE lsb, BYTE dssp, BYTE cpsr);
extern void SPIS_Init(BYTE cpol, BYTE cpha, BYTE dssp);
extern void SPI_IRQ_Config(uint8_t irq_mask, uint8_t sw);
extern void SPI_Read_ID0(void);
extern void SPI_Read_ID1(void);
extern void SPI_Flash_CS_Low(uint8_t cs_select);
extern void SPI_Flash_CS_High(uint8_t cs_select);
extern uint8_t SPI_Send_Byte(uint8_t send_data);
extern uint8_t SPI_Read_Byte(void);
extern uint8_t SPI_Send_Cmd(uint8_t cmd, uint8_t cs_select);
extern uint8_t SPI_Busy(uint8_t cs_select);
extern void SPI_WaitnoBUSY(uint8_t cs_select);
extern uint8_t SPI_Suspend(uint8_t cs_select);
extern void SPI_Write_Enable(uint8_t cs_select);
extern void SPI_Block_Erase(uint32_t address, uint8_t cs_select);
extern void SPI_Chip_Erase(uint8_t cs_select);
extern void SPI_Send_Cmd_Addr(uint8_t *pbuffer, uint32_t read_addr, uint16_t num_byte_to_read, uint8_t cs_select);
extern void SPI_Page_Program(uint32_t write_addr, uint32_t byte_num, uint8_t *byte_data, uint8_t cs_select);
extern void SPI_Flash_Reset(uint8_t cs_select);
extern void SPI_Flash_Test(void);
extern void SPI_Send_RWcmd(uint8_t cmd, uint32_t read_addr, uint8_t cs_select);
#define Spim_Int_Mask(type)			(SPIM_IMSR &= ~(0x1<<type))
#define Spim_Int_Unmask(type)		(SPIM_IMSR |= (0x1<<type))
#define	Spim_Int_Mask_Read(type)	((SPIM_IMSR & (0x1<<type)) == 0)
#define Spim_Int_Status(type)		((SPIM_ISR & (0x1<<type)) != 0)
#endif /* _SPI_FLASH_H_ */

extern void SPI_LOOP_TEST(void);
extern void spibusy(void);
extern void SPI_Send_2byte(uint16_t send_data);
extern void SPI_3Wire_Page_Program(int addr, uint16_t *data, int num, int csn);
extern void SPI_3Wire_Read(int addr, uint16_t *data, int num, int csn);
extern void SPI_Quad_enable(int csn);
extern void SPI_Dual_Read(int addr, uint16_t *data, int num, int csn);
extern void SPI_Quad_Program(int addr, uint16_t *data, int num, int csn);
extern void SPI_Quad_Read(int addr, uint16_t *data, int num, int csn);
extern uint8_t SPI_3wire_BUSY(int csn);

extern void SPI_3Wire_Test(void);
extern void SPI_Quad_Test(void);
extern void SPI_Dual_Test(void);