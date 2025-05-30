#include "KERNEL_I3C.H"




 //*****************************************************************************
 //  I3C base address conversion function
 //
 //  parameter :
 //      i3c_mux
 //
 //  return :
 //      I3C baseaddr
 //*****************************************************************************
DWORD I3c_Channel_Baseaddr(BYTE i3c_mux)
{
	switch(i3c_mux)
	{
		case I3C_MATER0:
			return I3C_MASTER0_BASE_ADDR;
		case I3C_MATER1:
			return I3C_MASTER1_BASE_ADDR;
		case I3C_SLAVE0:
			return I3C_SLAVE0_BASE_ADDR;
		case I3C_SLAVE1:
			return I3C_SLAVE1_BASE_ADDR;
        default:
            dprint("i3c_mux error \n");
        return 0;
    }
}
//*****************************************************************************
//  I3C register write by four bytes function
//
//  parameter :
//      value : value need to be write by four bytes
//      regoffset : Offset register
//		i3c_mux : baseaddr of I3C
//
//  return :
//      none
//*****************************************************************************
void I3c_Write_DWARD(DWORD value, WORD regoffset, BYTE i3c_mux)
{
	REG32(REG_ADDR(I3c_Channel_Baseaddr(i3c_mux), regoffset)) = value;
}

//*****************************************************************************
//  I3C register read four bytes function
//
//  parameter :
//      regoffset : Offset register
//		i3c_mux : baseaddr of I3C
//
//  return :
//      read value
//*****************************************************************************
DWORD I3c_Read_DWARD(WORD regoffset, BYTE i3c_mux)
{
    return REG32(REG_ADDR(I3c_Channel_Baseaddr(i3c_mux), regoffset));
}


//*****************************************************************************
//  I3C init as Master
//
//  parameter :
//      addr        : Device Static Address
//      i3c_mux     : Only I3C_MASTER0 or I3C_MASTER1 can be selected
//  return :
//      none
//*****************************************************************************
void I3C_Master_Init(BYTE addr, BYTE i3c_mux) 
{
    DWORD wdata;
    I3c_Write_DWARD(((ENABLE_I3C << 31) | (DMA_DISABLE) << 28 ), DEVICE_CTRL_OFFSET, i3c_mux);
    I3c_Write_DWARD((addr & 0x7f), DEV_ADDR_TABLE_LOC1_OFFSET, i3c_mux);
    wdata = 0x4|(0x4<<16);    //设置I3C推挽传输的SCL时钟高电平周期和低电平周期计数
    I3c_Write_DWARD(wdata, SCL_I3C_PP_TIMING_OFFSET, i3c_mux);
    I3c_Write_DWARD(((ENABLE_I3C << 31) | (DMA_DISABLE << 28) | (I2C_Slave_PRESENT << 7)), DEVICE_CTRL_OFFSET, i3c_mux);
    I3c_Write_DWARD( ((I2C_device << 31) | (addr & 0x7f)), DEV_ADDR_TABLE_LOC1_OFFSET, i3c_mux);
    I3c_Write_DWARD( (CMD_QUEUE_READY_STAT_EN | IBI_THLD_STAT_EN | RX_THLD_STAT_EN | TX_THLD_STAT_EN), INTR_STATUS_EN_OFFSET, i3c_mux);
    wdata = (0x3<<8)|0x1;  //DMA  master1 resp thld
    I3c_Write_DWARD(wdata, QUEUE_THLD_CTRL_OFFSET, i3c_mux);
    I3c_Write_DWARD(((RX_BUF_THLD_1 << 8)| TX_EMPTY_BUF_THLD_1), DATA_BUFFER_THLD_CTRL_OFFSET, i3c_mux);

}

//*****************************************************************************
//  I3C Master write function
//
//  parameter :
//      data        : data to write
//      i3c_mux     : Only I3C_MASTER0 or I3C_MASTER1 can be selected
//  return :
//      none
//*****************************************************************************
void I3c_Master_write(DWORD data, BYTE i3c_mux)
{
    //发送1条命令
    uint32_t cmd = (1 << 16) | 0x1; //0-2：0x1Transfer Argument；16-31：Data Length，1说明只写一个

    I3c_Write_DWARD(cmd, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    while ((I3c_Read_DWARD(INTR_STATUS_OFFSET, i3c_mux) & 0x08) == 0);
    cmd = (0x1 << 30) | (0x0 << 28) | (0x0 << 27) | (0x0 << 21) | 0x0;
    I3c_Write_DWARD(cmd, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    while((I3c_Read_DWARD(INTR_STATUS_OFFSET, i3c_mux) & 0x1) == 0);   //等CMD_QUEUE_READY_STS中断
    //数据填入发送寄存器
    I3c_Write_DWARD(data, TX_DATA_PORT_OFFSET, i3c_mux);
}


//*****************************************************************************
//  I3C Master read function
// 
//  parameter :
//      i3c_mux     : Only I3C_MASTER0 or I3C_MASTER1 can be selected
//  return :
//      read data
//*****************************************************************************
uint32_t I3c_Master_Read(BYTE i3c_mux)
{
    uint32_t rdata = 0;
    uint32_t cmd = (1 << 16) | 0x1; //0-2：0x1Transfer Argument；16-31：Data Length，1说明只读取一个
    I3c_Write_DWARD(cmd, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    while ((I3c_Read_DWARD(INTR_STATUS_OFFSET, i3c_mux) & 0x08) == 0); // 等待CMD_COMPLETE中断
    cmd = (0x1 << 30) | (0x1 << 28) | (0x0 << 27) | (0x0 << 21) | 0x0;   //0-2:0x0 Transfer Command；28:0 write, 1 read
    I3c_Write_DWARD(cmd, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    while((I3c_Read_DWARD(INTR_STATUS_OFFSET, i3c_mux) & (0x1 << 1)) == 0);
    vDelayXus(30);
    // 3. 读取数据
	rdata = I3c_Read_DWARD(RX_DATA_PORT_OFFSET, i3c_mux);
    return rdata;
}
//*****************************************************************************
//  I3C init as Slave
//
//  parameter :
//      addr        : Device Static Address
//      i3c_mux     : Only I3C_SLAVE0 or I3C_SLAVE1 can be selected
//  return :
//      none
//*****************************************************************************
void I3c_Slave_Init(BYTE addr, BYTE i3c_mux)
{
    //配置静态地址
    DWORD rdata;
    rdata = I3c_Read_DWARD(CONFIG_OFFSET, i3c_mux);
    rdata |= (addr << 25);
    I3c_Write_DWARD(rdata, CONFIG_OFFSET, i3c_mux);

    //中断使能
    I3c_Write_DWARD(0x0, INTSET_OFFSET, i3c_mux);   //中断全部失能

    //DMA使能
    I3c_Write_DWARD(0x0, DMACTRL_OFFSET, i3c_mux);   //DMA失能

}


//*****************************************************************************
//  I3C slave interrupts enable
//
//  parameter :
//      tpye        : such as RXPEND_ENABLE,TXSEND_ENABLE...
//      i3c_mux     : Only I3C_SLAVE0 or I3C_SLAVE1 can be selected
//  return :
//      none
//*****************************************************************************
BYTE I3C_SLAVE_INT_ENABLE(DWORD tpye, BYTE i3c_mux)
{
    switch(i3c_mux)
    {
        case I3C_SLAVE0:
            INTSET_0 |= tpye;
            return 0;
        case I3C_SLAVE1:
            INTSET_1 |= tpye;
            return 0;
        default:
            printf("i3c_mux error\n");
            return -1;
    }
}
//*****************************************************************************
//  I3C slave interrupts disable
//
//  parameter :
//      tpye        : such as RXPEND_ENABLE,TXSEND_ENABLE...
//      i3c_mux     : Only I3C_SLAVE0 or I3C_SLAVE1 can be selected
//  return :
//      none
//*****************************************************************************
BYTE I3C_SLAVE_INT_DISABLE(DWORD tpye, BYTE i3c_mux)
{
    switch(i3c_mux)
    {
        case I3C_SLAVE0:
            INTSET_0 &= (~tpye);
            return 0;
        case I3C_SLAVE1:
            INTSET_1 &= (~tpye);
            return 0;
        default:
            printf("i3c_mux error\n");
            return -1;
    }
}

// //以下是I3C的回环测试代码，还需结合SLAVE的中断代码一起使用，具体如何使用，请查阅I3C_loop测试文档说明
// void I3C_LOOP_Test(void)
// {
//     printf("test I3C\n");
//     //引脚复用
//     sysctl_iomux_master0();
//     sysctl_iomux_slave0();
//     sysctl_iomux_master1();
//     sysctl_iomux_slave1();

//     //master初始化
//     I3C_Master_Init(0x5a, I3C_MATER1);

//     //初始话slave
//     I3c_Slave_Init(0x5a, I3C_SLAVE1);
//     I3C_SLAVE_INT_ENABLE(RXPEND_ENABLE, I3C_SLAVE1);  //使能接收FIFO满中断

//     for (int i = 0; i < 0xf; i++)
//     {
//         BYTE tx_data = 0x1+i;
        
//         I3c_Master_write(tx_data, I3C_MATER1);

//         vDelayXus(20);

//         // BYTE rx_data = I3c_Master_Read(I3C_MATER1);
//         I3c_Master_Read(I3C_MATER1);
//     }

// }

