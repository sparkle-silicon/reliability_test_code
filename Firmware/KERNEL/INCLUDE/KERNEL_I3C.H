#ifndef __KERNEL_I3C_H
#define __KERNEL_I3C_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "AE_DEBUGGER.H"



#define I3C_MATER0 0
#define I3C_MATER1 1
#define I3C_SLAVE0 2
#define I3C_SLAVE1 3

//以下内容只是寄存器位的部分情况，详细内容请阅读I3C的寄存器手册


//Master
//Controls whether or not i3c is enable
#define DISABLE_I3C HEX00
#define ENABLE_I3C HEX01
// DMA Handshake Interface Enable
#define DMA_DISABLE HEX00
#define DMA_ENABLE HEX01
//I2C_SLAVE_PRESENT
#define I2C_Slave_NO_PRESENT HEX00
#define I2C_Slave_PRESENT HEX01
//Legacy I2C device or not
#define I2C_No_device HEX00
#define I2C_device HEX01
#define TX_THLD_STAT_EN HEX01
#define RX_THLD_STAT_EN HEX02
#define IBI_THLD_STAT_EN HEX04
#define CMD_QUEUE_READY_STAT_EN HEX08
//Transmit Buffer Threshold Value
#define TX_EMPTY_BUF_THLD_1 0x000
#define TX_EMPTY_BUF_THLD_4 0x001
#define TX_EMPTY_BUF_THLD_8 0x010
#define TX_EMPTY_BUF_THLD_16 0x011
#define TX_EMPTY_BUF_THLD_32 0x100
#define TX_EMPTY_BUF_THLD_64 0x101
//Transmit Buffer Threshold Value
#define RX_BUF_THLD_1 0x000
#define RX_BUF_THLD_4 0x001
#define RX_BUF_THLD_8 0x010
#define RX_BUF_THLD_16 0x011
#define RX_BUF_THLD_32 0x100
#define RX_BUF_THLD_64 0x101


//Slave

#define RXPEND_ENABLE 0x800     //第11位-接收FIFO满中断
#define TXSEND_ENABLE 0x1000     //第12位-发送FIFO空中断
//DMA Control register
#define DMAFB_RX_DISABLE HEX00
#define DMAFB_RX_ENABLE HEX01
#define DMATB_TX_DISABLE HEX00
#define DMATB_TX_ENABLE HEX01



extern DWORD I3c_Channel_Baseaddr(BYTE i3c_mux);
extern void I3c_Write_DWARD(DWORD value, WORD regoffset, BYTE i3c_mux);
extern DWORD I3c_Read_DWARD(WORD regoffset, BYTE i3c_mux);

extern void I3C_Master_Init(BYTE addr, BYTE i3c_mux);
extern void I3c_Master_write(DWORD data, BYTE i3c_mux);
extern uint32_t I3c_Master_Read(BYTE i3c_mux);
extern void I3c_Slave_Init(BYTE addr, BYTE i3c_mux);


extern BYTE I3C_SLAVE_INT_ENABLE(DWORD tpye, BYTE i3c_mux);
extern BYTE I3C_SLAVE_INT_DISABLE(DWORD tpye, BYTE i3c_mux);
#endif