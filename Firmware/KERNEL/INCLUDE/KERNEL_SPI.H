/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-11-09 22:16:12
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
#include "KERNEL_GPIO.H"
 /*********************************SPIM CONFIG CLOCK*********************************/
#define SPIM_CLOCK  (HIGH_CHIP_CLOCK / (SYSCTL_CLKDIV_SPIM+1)) 
#define SPIM_DEFAULT_TIMEOUT 1000000 //(SPIM_CLOCK/(SPIM_CPSR+1)*32) // SPI超时时间
/*********************************SPIM REGISTER DEFINE*********************************/
//控制器
#define SPIM_CTRL_CPHA              0x0001                  // 时钟相位
#define SPIM_CTRL_CPOL              0x0002                  // 时钟极性
#define SPIM_CTRL_CPHA_LOW          0x0000                  // 时钟相位
#define SPIM_CTRL_CPOL_LOW          0x0000                  // 时钟极性
#define SPIM_CTRL_CPHA_HIGH         SPIM_CTRL_CPHA          // 时钟相位
#define SPIM_CTRL_CPOL_HIGH         SPIM_CTRL_CPOL          // 时钟极性

#define SPIM_CTRL_SPE               0x0004                  // SPI使能
#define SPIM_CTRL_DLY_SEL_MASK      0x0038                  // 数据帧间的时延选择MASK
#define SPIM_CTRL_DLY_SEL(select)   (((select)<< 3) & SPIM_CTRL_DLY_SEL_MASK) // 数据帧间的时延选择
#define SPIM_CTRL_DMA_RD_EN         0x0040                 // DMA读使能
#define SPIM_CTRL_DMA_WR_EN         0x0080                 // DMA写使能
#define SPIM_CTRL_DSSP_MASK         0x0F00                 // 数据帧宽度选择MASK
#define SPIM_CTRL_DSSP(dssp)        (((dssp)<< 8) & SPIM_CTRL_DSSP_MASK) // 数据帧宽度选择(实际位宽dssp+1)需注意，当用两线模式，数据帧宽度必须是2的倍数；当用四线模式，数据帧宽度必须是4的倍数
#define SPIM_CTRL_LSB               0x1000                 // LSB低位先传模式
#define SPIM_CTRL_MSB               0x0000                 // MSB高位先传模式
#define SPIM_CTRL_TX_CLR            0x2000                 // 发送FIFO清空
#define SPIM_CTRL_RX_CLR            0x4000                 // 接收FIFO清空
//分频设置
#define SPIM_CPSR_CPSR_MASK         0x00FF                 // 时钟分频设置MASK
#define SPIM_CPSR_CPSR(cpsr)        ((cpsr) & SPIM_CPSR_CPSR_MASK) // 时钟分频设置
//发送阈值
#define SPIM_TXFTLR_TFT_MASK        0x001F                 // 发送FIFO阈值MASK
#define SPIM_TXFTLR_TFT(tft)        ((tft) & SPIM_TXFTLR_TFT_MASK) // 发送FIFO阈值
//接收阈值
#define SPIM_RXFTLR_RFT_MASK        0x001F                 // 接收FIFO阈值MASK
#define SPIM_RXFTLR_RFT(rft)        ((rft) & SPIM_RXFTLR_RFT_MASK) // 接收FIFO阈值
//发送FIFO数据量
#define SPIM_TXFLR_TX_LV_MASK        0x001F                 // 发送FIFO数据量MASK
#define SPIM_TXFLR_TX_LV(tx_lv)      ((tx_lv) & SPIM_TXFLR_TX_LV_MASK) // 发送FIFO数据量
//接收FIFO数据量
#define SPIM_RXFLR_RX_LV_MASK        0x001F                 // 接收FIFO数据量MASK
#define SPIM_RXFLR_RX_LV(rx_lv)      ((rx_lv) & SPIM_RXFLR_RX_LV_MASK) // 接收FIFO数据量
//状态
#define SPIM_SR_TFNF                 0x0001                 // 发送FIFO不满标志位
#define SPIM_SR_TFE                  0x0002                 // 发送FIFO空标志位
#define SPIM_SR_RFNE                 0x0004                 // 接收FIFO不空标志位
#define SPIM_SR_RFF                  0x0008                 // 接收FIFO满标志位
#define SPIM_SR_BSY                  0x0010                 // SPI-M busy标志位
//中断屏蔽
#define SPIM_IMSR_TXEIM              0x0001                 // 发送FIFO空中断屏蔽位
#define SPIM_IMSR_TXOIM              0x0002                 // 发送FIFO上溢中断屏蔽位
#define SPIM_IMSR_RXUIM              0x0004                 // 接收FIFO下溢中断屏蔽位
#define SPIM_IMSR_RXOIM              0x0008                 // 接收FIFO溢出中断屏蔽位
#define SPIM_IMSR_RXFIM              0x0010                 // 接收FIFO满中断屏蔽位
#define SPIM_IMSR_RDOVM              0x0020                 // RDNUM接收完毕中断屏蔽位
#define SPIM_IMSR_ERRM               0x0040                 // 错误中断屏蔽位
//中断状态
#define SPIM_ISR_TXEIS               0x0001                 // 发送FIFO空中断状态位
#define SPIM_ISR_TXOIS               0x0002                 // 发送FIFO上溢中断状态位
#define SPIM_ISR_RXUIS               0x0004                 // 接收FIFO下溢中断状态位
#define SPIM_ISR_RXOIS               0x0008                 // 接收FIFO溢出中断状态位
#define SPIM_ISR_RXFIS               0x0010                 // 接收FIFO满中断状态位
#define SPIM_ISR_RDOVS               0x0020                 // RDNUM接收完毕中断状态位
#define SPIM_ISR_ERRS                0x0040                 // 错误中断状态位
//数据
#define SPIM_DA_SPDR_MASK            0xFFFF                 // 数据MASK
//片选0
#define SPIM_CSN0_DESELECT               0x0001                 // 片选0
#define SPIM_CSN0_SELECT               0x0000                 // 片选0
//片选1
#define SPIM_CSN1_DESELECT             0x0001                 // 片选1
#define SPIM_CSN1_SELECT               0x0000                 // 片选0

//控制器模式
#define SPIM_MODE_DW_MOD_MASK                       0x0003 // SPI模式选择MASK
#define SPIM_MODE_DW_MOD(dw_mod)                    ((dw_mod) & SPIM_MODE_DW_MOD_MASK) 
#define SPIM_MODE_4WIRE                             0x0000 /* 四线全双工模式: csn, sck, miso, mosi */
#define SPIM_MODE_STANDARD                          SPIM_MODE_4WIRE/* 标准模式（即四线全双工模式） */
#define SPIM_MODE_DUAL                              0x0001 /* DUAL半双工模式: csn, sck, miso(io0), mosi(io1) (双线数据) */
#define SPIM_MODE_QUAD                              0x0002  /* QUAD半双工模式: csn, sck, miso(io0), mosi(io1), io2, io3 (四线数据) */
#define SPIM_MODE_3WIRE                             0x0003 /* 三线半双工模式: csn, sck, miso(io0) (单线数据) */
#define SPIM_MODE_SIMPLEX                           SPIM_MODE_3WIRE /* 单线模式（即三线半双工模式） */
//读取个数
#define SPIM_RDNUM_SPI_RDNUM_MASK                       0xFFFFU //0xFFFFFF//读取个数MASK
#define SPIM_RDNUM_SPI_RDNUM(rdnum)                     ((rdnum) & SPIM_RDNUM_SPI_RDNUM_MASK) //读取个数
//读取采样延迟//不能大于2*（CPSR+1）-1
#define SPIM_SAMPDELAY_SAMPDELAY_MASK                   0x00FFU     //0xFFFFFF//读取采样延迟MASK
#define SPIM_SAMPDELAY_SAMPDELAY(sampdelay)             ((sampdelay) & SPIM_SAMPDELAY_SAMPDELAY_MASK) //读取采样延迟
//片选SPIM_CHIP_Select宏
#define SPIM_CHIP_SELECT_CSN0 0
#define SPIM_CHIP_SELECT_CSN1 1
#define SPIM_CHIP_SELECT_FREE 2
/*********************************SPI FLASH Demo Define*********************************/
 /* Common commands (根据实际需要更改) */
#define SPI_FLASH_CMD_WRSR		0x01	// Write status register
#define SPI_FLASH_CMD_WRITE_STATUS_REGISTERS	SPI_FLASH_CMD_WRSR
#define SPI_FLASH_CMD_PP		0x02	// Page program
#define SPI_FLASH_CMD_PAGE_PROGRAM	SPI_FLASH_CMD_PP
#define SPI_FLASH_CMD_READ		0x03	// Read data
#define SPI_FLASH_CMD_READ_DATA	SPI_FLASH_CMD_READ
#define SPI_FLASH_CMD_WRDI		0x04	// Write disable
#define SPI_FLASH_CMD_WRITE_DISABLE	SPI_FLASH_CMD_WRDI
#define SPI_FLASH_CMD_RDSR1		0x05	// Read status register-1
#define SPI_FLASH_CMD_READ_STATUS_REGISTERS1	SPI_FLASH_CMD_RDSR1
#define SPI_FLASH_CMD_WREN		0x06	// Write enable
#define SPI_FLASH_CMD_WRITE_ENABLE	SPI_FLASH_CMD_WREN

#define SPI_FLASH_CMD_FSRD	    0x0B	// Fast read data
#define SPI_FLASH_CMD_FAST_READ	SPI_FLASH_CMD_FSRD

#define SPI_FLASH_CMD_SEER		0x20	// Sector Erase
#define SPI_FLASH_CMD_SECTOR_ERASE	SPI_FLASH_CMD_SEER

#define SPI_FLASH_CMD_QIPP		0x32	//Quad Input Page Program
#define SPI_FLASH_CMD_QUAD_INPUT_PAGE_PROGRAM	SPI_FLASH_CMD_QIPP

#define SPI_FLASH_CMD_RDSR2		0x35	// Read status register-2
#define SPI_FLASH_CMD_READ_STATUS_REGISTERS2	SPI_FLASH_CMD_RDSR2

#define SPI_FLASH_CMD_FSRD_DOUT		0x3B	// Fast read dual output data
#define SPI_FLASH_CMD_FAST_READ_DUAL_OUTPUT	SPI_FLASH_CMD_FSRD_DOUT

#define SPI_FLASH_CMD_PRSR		0x42	// Program security register
#define SPI_FLASH_CMD_PROGRAM_SECURITY_REGISTER	SPI_FLASH_CMD_PRSR

#define SPI_FLASH_CMD_ERSR		0x44	// Erase security register
#define SPI_FLASH_CMD_ERASE_SECURITY_REGISTER	SPI_FLASH_CMD_ERSR

#define SPI_FLASH_CMD_RDSR		0x48	// Read security register
#define SPI_FLASH_CMD_READ_SECURITY_REGISTER	SPI_FLASH_CMD_RDSR

#define SPI_FLASH_CMD_RUIDN		0x4B	// Read unique ID number
#define SPI_FLASH_CMD_READ_UNIVERSAL_ID_NUMBER	SPI_FLASH_CMD_RUIDN

#define SPI_FLASH_CMD_WE_VSR		0x50 // Write enable volatile status register
#define SPI_FLASH_CMD_WRITE_ENABLE_VOLATILE_STATUS_REGISTER	SPI_FLASH_CMD_WE_VSR

#define SPI_FLASH_CMD_32BE		0x52	// 32KB block erase
#define SPI_FLASH_CMD_32KB_BLOCK_ERASE	SPI_FLASH_CMD_32BE

#define SPI_FLASH_CMD_RSFDPR		0x5A	// read SFDP register
#define SPI_FLASH_CMD_READ_SFDP_REGISTER	SPI_FLASH_CMD_RSFDPR

#define SPI_FLASH_CMD_CE1		0x60	// Chip erase
#define SPI_FLASH_CMD_CHIP_ERASE1	SPI_FLASH_CMD_CE1

#define SPI_FLASH_CMD_FSRD_QOUT		0x6B	// Fast read quad output data
#define SPI_FLASH_CMD_FAST_READ_QUAD_OUTPUT	SPI_FLASH_CMD_FSRD_QOUT

#define SPI_FLASH_CMD_ERST		0x66	// Enable reset
#define SPI_FLASH_CMD_ENABLE_RESET	SPI_FLASH_CMD_ERST

#define SPI_FLASH_CMD_EPS		0x75	// Erase program suspend
#define SPI_FLASH_CMD_ERASE_PROGRAM_SUSPEND	SPI_FLASH_CMD_EPS

#define SPI_FLASH_CMD_SBWW		0x77	// Set burst with wrap
#define SPI_FLASH_CMD_SET_BURST_WITH_WRAP	SPI_FLASH_CMD_SBWW

#define SPI_FLASH_CMD_EPR		0x7A	// Erase program resume
#define SPI_FLASH_CMD_ERASE_PROGRAM_RESUME	SPI_FLASH_CMD_EPR

#define SPI_FLASH_CMD_RMDID		0x90	//Read Manufacturer and Device ID
#define SPI_FLASH_CMD_READ_MANUFACTURER_DEVICE_ID	SPI_FLASH_CMD_RMDID

#define SPI_FLASH_CMD_RMDID_DIO		0x92	//Release power down /device ID Dual I/O
#define SPI_FLASH_CMD_READ_MANUFACTURER_DEVICE_ID_DUAL_IO	SPI_FLASH_CMD_RMDID_DIO

#define SPI_FLASH_CMD_RMDID_QIO		0x94	//Release power down /device ID Quad I/O
#define SPI_FLASH_CMD_READ_MANUFACTURER_DEVICE_ID_QUAD_IO	SPI_FLASH_CMD_RMDID_QIO

#define SPI_FLASH_CMD_RST		0x99	// Enable reset
#define SPI_FLASH_CMD_RESET	SPI_FLASH_CMD_RST

#define SPI_FLASH_CMD_RJID		0x9F	//Read JEDEC ID
#define SPI_FLASH_CMD_READ_JEDEC_ID	SPI_FLASH_CMD_RJID

#define SPI_FLASH_CMD_RPD_DID		0xAB	//Release power down /device ID
#define SPI_FLASH_CMD_RELEASE_POWER_DOWN_DEVICE_ID	SPI_FLASH_CMD_RPD_DID

#define SPI_FLASH_CMD_PD		0xB9	//  power down
#define SPI_FLASH_CMD_POWER_DOWN	SPI_FLASH_CMD_PD

#define SPI_FLASH_CMD_FSRD_DIO		0xBB	// Fast read dual io data
#define SPI_FLASH_CMD_FAST_READ_DUAL_IO	SPI_FLASH_CMD_FSRD_DIO

#define SPI_FLASH_CMD_CE2		0xC7	// Chip erase
#define SPI_FLASH_CMD_CHIP_ERASE2	SPI_FLASH_CMD_CE2

#define SPI_FLASH_CMD_64BE		0xD8	// 64KB block erase
#define SPI_FLASH_CMD_64KB_BLOCK_ERASE	SPI_FLASH_CMD_64BE

#define SPI_FLASH_CMD_OWRD_QIO		0xE3	// octal word read quad output data
#define SPI_FLASH_CMD_OCTAL_WORD_READ_QUAD_IO	SPI_FLASH_CMD_OWRD_QIO

#define SPI_FLASH_CMD_WRD_QIO		0xE7	// word read quad output data
#define SPI_FLASH_CMD_WORD_READ_QUAD_IO	SPI_FLASH_CMD_WRD_QIO

#define SPI_FLASH_CMD_FSRD_QIO		0xEB	// Fast read quad io data
#define SPI_FLASH_CMD_FAST_READ_QUAD_IO	SPI_FLASH_CMD_FSRD_QIO
//Register bit definitions(根据实际需要更改)
#define SPI_FLASH_SR1_BUSY		0x01	//  ERASE/WRITE IN PROGRESS
#define SPI_FLASH_SR1_WIP       SPI_FLASH_SR1_BUSY
#define SPI_FLASH_SR1_WEL		0x02	//  WRITE ENABLE LATCH
// #define SPI_FLASH_SR1_BP0		0x04	//  BLOCK PROTECT BITS 0
// #define SPI_FLASH_SR1_BP1		0x08	//  BLOCK PROTECT BITS 1
// #define SPI_FLASH_SR1_BP2		0x10	//  BLOCK PROTECT BITS 2
// #define SPI_FLASH_SR1_TB		0x20	//  TOP/BOTTOM PROTECT
// #define SPI_FLASH_SR1_BP3		SPI_FLASH_SR1_TB	//  BLOCK PROTECT BITS 3
// #define SPI_FLASH_SR1_SEC		0x40	//  SECTOR PROTECT
// #define SPI_FLASH_SR1_BP4       SPI_FLASH_SR1_SEC	//  BLOCK PROTECT BITS 4
// #define SPI_FLASH_SR1_SRP0		0x80	//  STATUS REGISTER PROTECT 0

// #define SPI_FLASH_SR2_SRP1		0x01	//  STATUS REGISTER PROTECT 1
#define SPI_FLASH_SR2_QE		0x02	//  QUAD ENABLE
// #define SPI_FLASH_SR2_LB1		0x08	//  SECURITY REGISTER LOCK BITS 1
// #define SPI_FLASH_SR2_LB2		0x10	//  SECURITY REGISTER LOCK BITS 2
// #define SPI_FLASH_SR2_LB3		0x20	//  SECURITY REGISTER LOCK BITS 3
// #define SPI_FLASH_SR2_CMP		0x40	//  COMPLEMENT PROTECT
// #define SPI_FLASH_SR2_SUS		0x80	//  SUSPEND STATUS

#define SPI_FLASH_SR_QE		(SPI_FLASH_SR2_QE<<8)	//  QUAD ENABLE

#define Spim_Int_Mask(type)			(SPIM_IMSR &= ~(0x1<<type))
#define Spim_Int_Unmask(type)		(SPIM_IMSR |= (0x1<<type))
#define	Spim_Int_Mask_Read(type)	((SPIM_IMSR & (0x1<<type)) == 0)
#define Spim_Int_Status(type)		((SPIM_ISR & (0x1<<type)) != 0)

//SPIM初始化
extern void SPIM_Init(BYTE mode, BYTE cpha, BYTE cpol, BYTE dly, BYTE dssp, BYTE lsb, BYTE cpsr, BYTE sampdelay);
extern void SPIM_DMA_Enable(BYTE wr_en, BYTE rd_en);
//中断开关
extern void SPIM_Inturrupt_Mask(uint8_t irq_mask, uint8_t sw);
extern void SPIM_Inturrupt_Enable(uint8_t enable);
extern void SPIM_Inturrupt_Disable(uint8_t disable);
extern void SPIM_Inturrupt_Enable_Bit(uint8_t bit);
extern void SPIM_Inturrupt_Disable_Bit(uint8_t bit);
//模式切换
extern void SPIM_Switch_Mode(uint8_t mode);
extern void SPIM_Modify_Polarity(BYTE cpha, BYTE cpol, BYTE lsb);
//片选管理
extern int32_t SPIM_CS_Low(uint8_t cs_select);
extern int32_t SPIM_CS_High(uint8_t cs_select);
extern int32_t SPIM_CHIP_Select(uint8_t cs_select);
//数据传输
extern int32_t SPIM_Wait_Busy(void);
extern int32_t SPIM_Wait_Send(void);
extern int32_t SPIM_Send_Data(uint16_t send_data);
extern int32_t SPIM_Send_Datas(uint16_t* data, uint32_t len);
extern uint32_t SPIM_Receive_Data(void);
extern int32_t SPIM_Receive_Datas(uint16_t* data, uint32_t len);
extern uint32_t SPIM_Send_Receive_OnceData(uint16_t data);
extern int32_t SPIM_Send_Receive_Datas(uint16_t* send, uint16_t* receive, uint32_t len);
extern uint32_t SPIM_Only_Receive_OnceData(void);
extern int32_t SPIM_Only_Receive_Datas(uint16_t* receive, uint32_t len);
//SPIM-FLASH操作DEMO
extern uint8_t SPIM_FLASH_Busy(uint8_t mode, uint8_t cs_select);
extern int32_t SPIM_FLASH_WaitBusy(uint8_t mode, uint8_t cs_select);
extern int32_t SPIM_Reset_FLASH(uint8_t mode, uint8_t cs_select);
extern void SPIM_FLASH_Wirte_Status(uint8_t mode, uint16_t data, uint8_t cs_select);
extern void SPIM_Write_Enable(uint8_t mode, uint8_t cs_select);
extern void SPIM_Write_Disable(uint8_t mode, uint8_t cs_select);
extern uint32_t SPIM_Read_FLASH_ID(uint8_t mode, uint8_t cs_select);
extern int32_t SPIM_Read_FLASH(uint8_t mode, uint8_t cs_select, uint32_t address, uint16_t bytes, uint16_t* data);
extern int32_t SPIM_Write_FLASH(uint8_t mode, uint8_t cs_select, uint32_t addr, uint32_t bytes, uint16_t* data);
extern int32_t SPIM_Erase_FLASH(uint8_t mode, uint8_t cs_select, uint8_t cmd, uint32_t address);

#endif /* _SPI_FLASH_H_ */