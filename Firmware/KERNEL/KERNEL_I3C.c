/*
 * @Author: Iversu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2025-10-18 21:03:35
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

#include "KERNEL_I3C.H"
#include <math.h>

 /******** availiable dynamic addr for use *********/
 // 7'h08~3D
 // 7'h3F~5D
 // 7'h5F~6D
 // 7'h6F~75
 // 7'h77
 /**************************************************/
//data structure define
BYTE MASTER0_DEV_DYNAMIC_ADDR_TABLE[] = { 0x3A,0x3B,0x3C,0x3D };    //动态地址表是MASTER需要分配给SLAVE的,需要开发人员定义 
BYTE MASTER1_DEV_DYNAMIC_ADDR_TABLE[] = { 0x3A,0x3B,0x3C,0x3D };    //动态地址表是MASTER需要分配给SLAVE的,需要开发人员定义  
BYTE MASTER0_DEV_STATIC_ADDR_TABLE[] = { 0x5A };     //静态地址表是开发人员需要查阅SLAVE设备DATASHEET获取后填入
BYTE MASTER1_DEV_STATIC_ADDR_TABLE[] = { 0 };     //静态地址表是开发人员需要查阅SLAVE设备DATASHEET获取后填入
uint8_t get_status[2] = { 0 };  //ibi 请求读状态
sDEV_CHAR_TABLE master0_dev_read_char_table[] = { 0 };  //读取master0 dct
sDEV_CHAR_TABLE master1_dev_read_char_table[] = { 0 };  //读取master0 dct
BYTE master0_dev_cnt = 0;
BYTE master1_dev_cnt = 0;

/**
* @brief I3C地址转换函数
*
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1、I3C_SLAVE0、I3C_SLAVE1
*
* @return I3C baseaddr
*
* @note 选择配置，输出该配置对应的寄存器基地址
*/
DWORD I3C_Channel_Baseaddr(BYTE i3c_mux)
{
    switch (i3c_mux)
    {
    case I3C_MASTER0:
        return I3C_MASTER0_BASE_ADDR;
    case I3C_MASTER1:
        return I3C_MASTER1_BASE_ADDR;
    case I3C_SLAVE0:
        return I3C_SLAVE0_BASE_ADDR;
    case I3C_SLAVE1:
        return I3C_SLAVE1_BASE_ADDR;
    default:
        i3c_dprint("i3c_mux error \n");
        return 0;
    }
}

/**
* @brief 写寄存器接口函数
*
* @param value 写入值
* @param regoffset 寄存器地址偏移
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1、I3C_SLAVE0、I3C_SLAVE1
* @return 无
*
* @note 往寄存器写入DWORD值
*/
void I3C_WriteREG_DWORD(DWORD value, WORD regoffset, BYTE i3c_mux)
{
    REG32(REG_ADDR(I3C_Channel_Baseaddr(i3c_mux), regoffset)) = value;
}

/**
* @brief 读寄存器接口函数
*
* @param regoffset 寄存器地址偏移
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1、I3C_SLAVE0、I3C_SLAVE1
* @return 读取值
*
* @note 读取寄存器DWORD值
*/
DWORD I3C_ReadREG_DWORD(WORD regoffset, BYTE i3c_mux)
{
    return REG32(REG_ADDR(I3C_Channel_Baseaddr(i3c_mux), regoffset));
}

/**
* @brief 等待SDA上拉OK
*
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return 读取值
*
* @note 操作结果
*/
BYTE I3C_WAIT_SDA_PU(BYTE i3c_mux)
{
    if ((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1))
    {
        i3c_dprint("controller select fault\n");
        return 0;
    }

    if (i3c_mux == I3C_MASTER0)
    {
        for (uint32_t timeout = I3C_TIMEOUT; IS_GPIOC12(LOW) == 1; timeout--)
        {
            if (timeout == 0)
            {
                i3c_dprint("I3C MASTER0 PULLUP FAILED\n");
                return FALSE;
            }
            nop;
        }
    }
    else if (i3c_mux == I3C_MASTER1)
    {
        for (uint32_t timeout = I3C_TIMEOUT; IS_GPIOB1(LOW) == 1; timeout--)
        {
            if (timeout == 0)
            {
                i3c_dprint("I3C MASTER1 PULLUP FAILED\n\n");
                return FALSE;
            }
            nop;
        }
    }
    return TRUE;
}
/**
* @brief 查找当前动态地址对应DAT寄存器的offset
*
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return 无
*
* @note 无
*/
BYTE serach_current_dynamic_cnt(uint8_t dynamic_addr, BYTE i3c_mux)
{
    uint32_t temp_data = 0;
    BYTE match_cnt = 0;
    uint8_t cnt = 0;

    if (i3c_mux == I3C_MASTER0)
        cnt = sizeof(MASTER0_DEV_DYNAMIC_ADDR_TABLE);
    else if (i3c_mux == I3C_MASTER1)
        cnt = sizeof(MASTER1_DEV_DYNAMIC_ADDR_TABLE);
    for (int i = 0;i < cnt;i++)
    {
        temp_data = I3C_ReadREG_DWORD(DEV_ADDR_TABLE1_LOC1_OFFSET + i * 4, i3c_mux);
        if (((temp_data >> 16) & 0x7F) == dynamic_addr)
        {
            match_cnt = i;
            return match_cnt;
        }
        else
            match_cnt = 0;
    }
    return match_cnt;
}
/**
* @brief MASTER读取及清除错误状态函数
*
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return 无
*
* @note 无
*/
uint32_t I3C_Master_RespData_ERRSTS(BYTE i3c_mux)
{
    if ((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1))
    {
        i3c_dprint("controller select fault\n");
        return 0;
    }

    uint32_t err_sts = (I3C_ReadREG_DWORD(RESPONSE_QUEUE_PORT_OFFSET, i3c_mux) & RESPONSE_QUEUE_PORT_ERR_STS_MASK);
    if (err_sts != RESPONSE_QUEUE_PORT_ERR_STS_NO_ERROR)
    {
        i3c_error = 1;
        switch (err_sts)
        {
        case RESPONSE_QUEUE_PORT_ERR_STS_NO_ERROR:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_NO_ERROR\n", i3c_mux);
            break;
        case RESPONSE_QUEUE_PORT_ERR_STS_CRC_ERROR:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_CRC_ERROR\n", i3c_mux);
            break;
        case RESPONSE_QUEUE_PORT_ERR_STS_PARITY_ERROR:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_PARITY_ERROR\n", i3c_mux);
            break;
        case RESPONSE_QUEUE_PORT_ERR_STS_FRAME_ERROR:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_FRAME_ERROR\n", i3c_mux);
            break;
        case RESPONSE_QUEUE_PORT_ERR_STS_I3C_BROADCAST_ADDRESS_NACK_ERROR:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_I3C_BROADCAST_ADDRESS_NACK_ERROR\n", i3c_mux);
            break;
        case RESPONSE_QUEUE_PORT_ERR_STS_ADDRESS_NACKED:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_ADDRESS_NACKED\n", i3c_mux);
            break;
        case RESPONSE_QUEUE_PORT_ERR_STS_RECELVE_BUFFER_OVERFLOWTRANSMIT_BUFFER_UNDERFLOW:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_RECELVE_BUFFER_OVERFLOWTRANSMIT_BUFFER_UNDERFLOW\n", i3c_mux);
            break;
        case RESPONSE_QUEUE_PORT_ERR_STS_TRANSFER_ABORTED:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_TRANSFER_ABORTED\n", i3c_mux);
            break;
        case RESPONSE_QUEUE_PORT_ERR_STS_I2C_SLAVE_WRITE_DATA_NACK_ERROR:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_I2C_SLAVE_WRITE_DATA_NACK_ERROR\n", i3c_mux);
            break;
        case RESPONSE_QUEUE_PORT_ERR_STS_PEC_ERROR:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_PEC_ERROR\n", i3c_mux);
            break;
        default:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_RESERVED\n", i3c_mux);
            break;
        }
        I3C_WriteREG_DWORD((I3C_ReadREG_DWORD(DEVICE_CTRL_OFFSET, i3c_mux) | DEVICE_CTRL_RESUME), DEVICE_CTRL_OFFSET, i3c_mux);//清除错误并且复位
        for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(DEVICE_CTRL_OFFSET, i3c_mux) & DEVICE_CTRL_RESUME); timeout--)
        {
            if (timeout == 0)
            {
                i3c_dprint("fail:I3C_MASTER%d_DEVICE_CTRL=%#x\n", i3c_mux, I3C_ReadREG_DWORD(DEVICE_CTRL_OFFSET, i3c_mux));
                break;
            }
        }
    }
    return err_sts;
}

/**
* @brief MASTER中断状态函数
*
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return 中断状态
*
* @note 无
*/
uint32_t I3C_Master_IntrStatus(BYTE i3c_mux)
{
    uint32_t int_status = I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux);
    if (int_status & INTR_STATUS_BUS_RESET_DONE_STS)
    {
        i3c_dprint("I3C_MASTER%d_INTR_STATUS_BUS_RESET_DONE_STS\n", i3c_mux);//出错
        I3C_WriteREG_DWORD((I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) | INTR_STATUS_BUS_RESET_DONE_STS), INTR_STATUS_OFFSET, i3c_mux);//清除中断
    }
    if (int_status & INTR_STATUS_TRANSFER_ERR_STS)
    {
        i3c_dprint("I3C_MASTER%d_INTR_STATUS_TRANSFER_ERR_STS\n", i3c_mux);//出错
        I3C_WriteREG_DWORD(RESET_CTRL_CMD_QUEUE_RST, RESET_CTRL_OFFSET, i3c_mux);
        I3C_WriteREG_DWORD((DEVICE_CTRL_RESUME | I3C_ReadREG_DWORD(DEVICE_CTRL_OFFSET, i3c_mux)), DEVICE_CTRL_OFFSET, i3c_mux);
        I3C_WriteREG_DWORD((I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) | INTR_STATUS_TRANSFER_ERR_STS), INTR_STATUS_OFFSET, i3c_mux);//清除中断
    }
    if (int_status & INTR_STATUS_TRANSFER_ABORT_STS)
    {
        i3c_dprint("I3C_MASTER%d_INTR_STATUS_TRANSFER_ABORT_STS\n", i3c_mux);//出错
        I3C_WriteREG_DWORD((I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) | INTR_STATUS_TRANSFER_ABORT_STS), INTR_STATUS_OFFSET, i3c_mux);//清除中断
    }
    if (int_status & INTR_STATUS_RESP_READY_STS)
    {
        i3c_dprint("I3C_MASTER%d_INTR_STATUS_RESP_READY_STS\n", i3c_mux);
        I3C_Master_RespData_ERRSTS(i3c_mux);//进入报错
    }
    if (int_status & INTR_STATUS_CMD_QUEUE_READY_STS)
    {
        i3c_dprint("I3C_MASTER%d_INTR_STATUS_CMD_QUEUE_READY_STS\n", i3c_mux);
    }
    if (int_status & INTR_STATUS_IBI_THLD_STS)
    {
        i3c_dprint("I3C_MASTER%d_INTR_STATUS_IBI_THLD_STS\n", i3c_mux);
        I3C_MASTER_IBI_Handler(i3c_mux);
    }
    if (int_status & INTR_STATUS_RX_THLD_STS)
    {
        i3c_dprint("I3C_MASTER%d_INTR_STATUS_RX_THLD_STS\n", i3c_mux);
    }
    if (int_status & INTR_STATUS_TX_THLD_STS)
    {
        i3c_dprint("I3C_MASTER%d_INTR_STATUS_TX_THLD_STS\n", i3c_mux);
    }

    return int_status & 0x1f;
}

/**
* @brief Legacy I2C MASTER接口初始化函数
*
* @param speed 总线速度
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return true/false
*
* @note 配置接口为MASTER功能
*/
BYTE I3C_Legacy_Master_Init(uint32_t speed, BYTE i3c_mux)
{
    if ((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1))
    {
        i3c_dprint("controller select fault\n");
        return FALSE;
    }
    uint32_t cnt = (CHIP_CLOCK_INT_HIGH / speed);
    uint32_t hcnt = ((cnt * 1) / 2);    //50%占空比
    uint32_t lcnt = (cnt - hcnt);
    uint32_t temp_data = 0;
    if (hcnt >= 0x7fff)hcnt = 0x7fff;
    if (lcnt >= 0x7fff)lcnt = 0x7fff;
    cnt = hcnt + lcnt;
    speed = (CHIP_CLOCK_INT_HIGH / cnt);//实际配置速率
    i2c_legacy_fm_plus_flag = (speed > 400000 ? 1 : 0);

    //Controls whether or not I3C_master is enabled And I2C Slave Present
    I3C_WriteREG_DWORD((DEVICE_CTRL_ENABLE | DEVICE_CTRL_I2C_SLAVE_PRESENT), DEVICE_CTRL_OFFSET, i3c_mux);
    //Set Device Static Address And Type of device I2C
    I3C_WriteREG_DWORD(DEV_ADDR_TABLE_LOC1_DEVICE_I2C, DEV_ADDR_TABLE1_LOC1_OFFSET, i3c_mux);
    //Set I3C fm/fmp Count. 
    if (i2c_legacy_fm_plus_flag)
        I3C_WriteREG_DWORD((SCL_I2C_FMP_TIMING_LCNT(lcnt) | SCL_I2C_FMP_TIMING_HCNT(hcnt)), SCL_I2C_FMP_TIMING_OFFSET, i3c_mux);
    else
        I3C_WriteREG_DWORD((SCL_I2C_FM_TIMING_LCNT(lcnt) | SCL_I2C_FM_TIMING_HCNT(hcnt)), SCL_I2C_FM_TIMING_OFFSET, i3c_mux);
    //set queue thld ctrl
    I3C_WriteREG_DWORD((QUEUE_THLD_CTRL_RESP_BUF_THLD(RESP_BUF_THLD_1) | QUEUE_THLD_CTRL_CMD_EMPTY_BUF_THLD(CMD_EMPTY_BUF_THLD_ALLEMPTY)), QUEUE_THLD_CTRL_OFFSET, i3c_mux);
    //Set data buff thld
    temp_data = I3C_ReadREG_DWORD(DATA_BUFFER_THLD_CTRL_OFFSET, i3c_mux);
    temp_data &= DATA_BUFFER_THLD_CTRL_RX_START_THLD0 & DATA_BUFFER_THLD_CTRL_TX_EMPTY_BUF_THLD0 & DATA_BUFFER_THLD_CTRL_RX_BUF_THLD0;
    temp_data |= DATA_BUFFER_THLD_CTRL_TX_START_THLD(TX_EMPTY_BUF_THLD_64);
    I3C_WriteREG_DWORD(temp_data, DATA_BUFFER_THLD_CTRL_OFFSET, i3c_mux);
    //Set intr status en
    I3C_WriteREG_DWORD((INTR_STATUS_EN_BUS_RESET_DONE_STS_EN | INTR_STATUS_EN_TRANSFER_ERR_STS_EN | INTR_STATUS_EN_TRANSFER_ABORT_STS_EN | INTR_STATUS_EN_RESP_READY_STS_EN | INTR_STATUS_EN_CMD_QUEUE_READY_STS_EN | INTR_STATUS_EN_TX_THLD_STS_EN | INTR_STATUS_EN_RX_THLD_STS_EN), INTR_STATUS_EN_OFFSET, i3c_mux);
    //Set intr signal en
    I3C_WriteREG_DWORD(INTR_SIGNAL_EN_TRANSFER_ABORT_SIGNAL_EN | INTR_SIGNAL_EN_TRANSFER_ERR_SIGNAL_EN | INTR_SIGNAL_EN_BUS_RESET_DONE_SIGNAL_EN, INTR_SIGNAL_EN_OFFSET, i3c_mux);
    return TRUE;
}

/**
* @brief legacy i2c MASTER写操作函数
*
* @param static_addr 要通信的目标SLAVE的静态地址
* @param data 写入值的指针
* @param bytelen 读数据的个数
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return 操作结果
*
* @note 无
*/
BYTE I3C_Legacy_Master_Write(uint8_t static_addr, uint8_t* data, uint16_t bytelen, BYTE i3c_mux)
{
    uint8_t* data_ptr = data;
    uint32_t data_len = bytelen;
    uint32_t temp_data = 0;
    if (((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1)) || (data_ptr == NULL) || (data_len == 0))
    {
        i3c_dprint("argument error\n");
        return FALSE;
    }

    //Set Device Static Address And Type of device I2C
    I3C_WriteREG_DWORD((DEV_ADDR_TABLE_LOC1_DEVICE_I2C | DEV_ADDR_TABLE_LOC1_STATIC_ADDRESS(static_addr)), DEV_ADDR_TABLE1_LOC1_OFFSET, i3c_mux);

    for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_CMD_QUEUE_READY_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            I3C_Master_IntrStatus(i3c_mux);
            i3c_error = 0;
            return FALSE;
        }
        nop;
    } // 等待INTR_STATUS_CMD_QUEUE_READY_STS中断
    //配置传输长度
    temp_data |= COMMAND_QUEUE_PORT_COMMAND_TRANSFER_ARGUMENT | COMMAND_QUEUE_PORT_TRANSFER_ARGUMENT_DATA_LENGTH(bytelen);
    I3C_WriteREG_DWORD(temp_data, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_CMD_QUEUE_READY_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            I3C_Master_IntrStatus(i3c_mux);
            i3c_error = 0;
            return FALSE;
        }
        nop;
    } // 等待配置完成
    temp_data = 0;
    temp_data &= (COMMAND_QUEUE_PORT_COMMAND_TRANSFER_COMMAND & COMMAND_QUEUE_PORT_TRANSFER_COMMAND_CP0 & COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DEV_INDX0 & COMMAND_QUEUE_PORT_TRANSFER_COMMAND_WRITE);
    temp_data |= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_TOC;
    if (i2c_legacy_fm_plus_flag)
        temp_data |= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_SPEED(i2c_legacy_fm_plus_flag);
    else
        temp_data &= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_SPEED_FM;
    I3C_WriteREG_DWORD(temp_data, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_CMD_QUEUE_READY_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            I3C_Master_IntrStatus(i3c_mux);
            i3c_error = 0;
            return FALSE;
        }
        nop;
    }//等待配置完成
    //开始
    while (data_len)
    {
        register uint32_t len = (data_len <= 4 ? data_len : 4);
        data_len -= len;
        uDword data_temp;
        for (register uint32_t i = 0; i < len; i++)
        {
            data_temp.byte[i] = *data_ptr++;
        }
        for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_TX_THLD_STS) == 0; timeout--)
        {
            if (timeout == 0 || i3c_error)
            {
                I3C_Master_IntrStatus(i3c_mux);
                i3c_error = 0;
                return FALSE;
            }
            nop;
        }   //等INTR_STATUS_TX_THLD_STS
        I3C_WriteREG_DWORD(data_temp.dword, TX_DATA_PORT_OFFSET, i3c_mux);//发送数据
    }
    return TRUE;
}

/**
* @brief legacy i2c MASTER读操作函数
*
* @param static_addr 要通信的目标SLAVE的静态地址
* @param data 存放数据的指针
* @param bytelen 读数据的个数
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return 操作结果
*
* @note 无
*/
uint8_t I3C_Legacy_Master_Read(uint8_t static_addr, uint8_t* data, uint16_t bytelen, BYTE i3c_mux)
{
    uint8_t* data_ptr = data;
    uint32_t data_len = bytelen;
    uint32_t temp_data = 0;
    if (((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1)) || (data_ptr == NULL) || (data_len == 0))
    {
        i3c_dprint("argument error\n");
        return FALSE;
    }

    //Set Device Static Address And Type of device I2C
    I3C_WriteREG_DWORD((DEV_ADDR_TABLE_LOC1_DEVICE_I2C | DEV_ADDR_TABLE_LOC1_STATIC_ADDRESS(static_addr)), DEV_ADDR_TABLE1_LOC1_OFFSET, i3c_mux);

    for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_CMD_QUEUE_READY_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            I3C_Master_IntrStatus(i3c_mux);
            i3c_error = 0;
            return FALSE;
        }
        nop;
    } // 等待INTR_STATUS_CMD_QUEUE_READY_STS中断
    //配置传输长度
    temp_data |= COMMAND_QUEUE_PORT_COMMAND_TRANSFER_ARGUMENT | COMMAND_QUEUE_PORT_TRANSFER_ARGUMENT_DATA_LENGTH(bytelen);
    I3C_WriteREG_DWORD(temp_data, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_CMD_QUEUE_READY_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            I3C_Master_IntrStatus(i3c_mux);
            i3c_error = 0;
            return FALSE;
        }
        nop;
    } // 等待INTR_STATUS_CMD_QUEUE_READY_STS中断
    temp_data = 0;
    temp_data &= COMMAND_QUEUE_PORT_COMMAND_TRANSFER_COMMAND;
    temp_data |= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_READ | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_STOP;
    if (i2c_legacy_fm_plus_flag)
        temp_data |= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_SPEED(i2c_legacy_fm_plus_flag);
    else
        temp_data &= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_SPEED_FM;
    I3C_WriteREG_DWORD(temp_data, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_CMD_QUEUE_READY_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            I3C_Master_IntrStatus(i3c_mux);
            i3c_error = 0;
            return FALSE;
        }
        nop;
    } // 等待CMD_COMPLETE中断
    while (data_len)
    {
        register uint32_t len = (data_len <= 4 ? data_len : 4);
        data_len -= len;
        for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_RX_THLD_STS) == 0; timeout--)
        {
            if (timeout == 0 || i3c_error)
            {
                I3C_Master_IntrStatus(i3c_mux);
                i3c_error = 0;
                return FALSE;
            }
            vDelayXus(10);
        }
        uDword data_temp;
        data_temp.dword = I3C_ReadREG_DWORD(RX_DATA_PORT_OFFSET, i3c_mux);
        for (register uint32_t i = 0; i < len; i++)
        {
            *data_ptr++ = data_temp.byte[i];
        }

    }//读取数据

    return TRUE;
}

/**
* @brief Legacy i2c MASTER读写流操作函数
*
* @param static_addr 要通信的目标SLAVE的静态地址
* @param Var 写入数据指针
* @param Count 写入数据个数
* @param Var2 读出数据指针
* @param Count2 读数据个数
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return 操作结果
*
* @note 无
*/
BYTE I3C_Legacy_Master_W2R_Stream(BYTE static_addr, BYTE* w_var, WORD w_cnt, BYTE* r_var, WORD r_cnt, BYTE i3c_mux)
{
    if ((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1))
    {
        i3c_dprint("controller select fault\n");
        return FALSE;
    }
    if (w_cnt != 0)
    {
        if (I3C_Legacy_Master_Write(static_addr, w_var, w_cnt, i3c_mux) == FALSE)
        {
            return FALSE;
        }
    }
    if (r_cnt != 0)
    {
        if (I3C_Legacy_Master_Read(static_addr, r_var, r_cnt, i3c_mux) == FALSE)
        {
            return FALSE;
        }
    }
    return TRUE;
}
/**
* @brief I3C MASTER i3cmode接口初始化函数
*
* @param speed 总线速度,最大8M
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return true/false
*
* @note 配置接口为MASTER功能
*/
BYTE I3C_SDR_Master_Init(uint32_t speed, BYTE i3c_mux)
{
    if ((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1))
        return FALSE;
    uint32_t temp_data = 0;
    uint32_t cnt = (CHIP_CLOCK_INT_HIGH / speed);
    uint32_t hcnt = ((cnt * 1) / 2);    //50%占空比
    uint32_t lcnt = (cnt - hcnt);
    uint32_t pp_hcnt, pp_lcnt, od_hcnt, od_lcnt;
    pp_hcnt = hcnt;
    pp_lcnt = lcnt;
    od_hcnt = hcnt;
    od_lcnt = lcnt;
    if (hcnt >= PP_TIMING_HCNT_MAX)pp_hcnt = PP_TIMING_HCNT_MAX;
    if (lcnt >= PP_TIMING_HCNT_MAX)pp_lcnt = PP_TIMING_LCNT_MAX;
    if (hcnt <= PP_TIMING_HCNT_MIN)pp_hcnt = PP_TIMING_HCNT_MIN;
    if (lcnt <= PP_TIMING_LCNT_MIN)pp_lcnt = PP_TIMING_LCNT_MIN;

    if (hcnt >= OD_TIMING_HCNT_MAX)od_hcnt = OD_TIMING_HCNT_MAX;
    if (lcnt >= OD_TIMING_HCNT_MAX)od_lcnt = OD_TIMING_LCNT_MAX;
    if (hcnt <= OD_TIMING_HCNT_MIN)od_hcnt = OD_TIMING_HCNT_MIN;
    if (lcnt <= OD_TIMING_LCNT_MIN)od_lcnt = OD_TIMING_LCNT_MIN;
    i3c_dprint("pp_hcnt:%x,pp_lcnt:%x,od_hcnt:%x,od_lcnt:%x\n", pp_hcnt, pp_lcnt, od_hcnt, od_lcnt);

    //Controls whether or not I3C_master is enabled And I2C Slave Present 
    I3C_WriteREG_DWORD(DEVICE_CTRL_ENABLE | DEVICE_CTRL_IBA_INCLUDE, DEVICE_CTRL_OFFSET, i3c_mux);
    //Set I3C OD_TIMING
    I3C_WriteREG_DWORD((SCL_I3C_OD_TIMING_LCNT((od_lcnt)) | SCL_I3C_OD_TIMING_HCNT(od_hcnt)), SCL_I3C_OD_TIMING_OFFSET, i3c_mux);
    i3c_dprint("od:%x\n", I3C_ReadREG_DWORD(SCL_I3C_OD_TIMING_OFFSET, i3c_mux));
    //Set I3C PP_TIMING
    I3C_WriteREG_DWORD((SCL_I3C_PP_TIMING_LCNT(pp_lcnt) | SCL_I3C_PP_TIMING_HCNT(pp_hcnt)), SCL_I3C_PP_TIMING_OFFSET, i3c_mux);
    i3c_dprint("pp:%x\n", I3C_ReadREG_DWORD(SCL_I3C_PP_TIMING_OFFSET, i3c_mux));
    //set queue thld ctrl
    temp_data = 0;
    temp_data &= QUEUE_THLD_CTRL_RESP_BUF_THLD0 & QUEUE_THLD_CTRL_CMD_EMPTY_BUF_THLD0 & QUEUE_THLD_CTRL_IBI_STATUS_THLD0;
    temp_data |= QUEUE_THLD_CTRL_IBI_DATA_THLD(1);
    I3C_WriteREG_DWORD(temp_data, QUEUE_THLD_CTRL_OFFSET, i3c_mux);
    //Set data buff thld ctrl
    temp_data = I3C_ReadREG_DWORD(DATA_BUFFER_THLD_CTRL_OFFSET, i3c_mux);
    temp_data &= DATA_BUFFER_THLD_CTRL_RX_START_THLD0 & DATA_BUFFER_THLD_CTRL_TX_EMPTY_BUF_THLD0 & DATA_BUFFER_THLD_CTRL_RX_BUF_THLD0;
    temp_data |= DATA_BUFFER_THLD_CTRL_TX_START_THLD(TX_EMPTY_BUF_THLD_64);
    I3C_WriteREG_DWORD(temp_data, DATA_BUFFER_THLD_CTRL_OFFSET, i3c_mux);
    //Set intr status en
    I3C_WriteREG_DWORD((INTR_STATUS_EN_BUS_RESET_DONE_STS_EN | INTR_STATUS_EN_TRANSFER_ERR_STS_EN | INTR_STATUS_EN_TRANSFER_ABORT_STS_EN | INTR_STATUS_EN_RESP_READY_STS_EN | INTR_STATUS_EN_IBI_THLD_STS_EN | INTR_STATUS_EN_CMD_QUEUE_READY_STS_EN | INTR_STATUS_EN_TX_THLD_STS_EN | INTR_STATUS_EN_RX_THLD_STS_EN), INTR_STATUS_EN_OFFSET, i3c_mux);
    //Set intr signal en
    I3C_WriteREG_DWORD(INTR_SIGNAL_EN_IBI_THLD_SIGNAL_EN | INTR_SIGNAL_EN_TRANSFER_ABORT_SIGNAL_EN | INTR_SIGNAL_EN_TRANSFER_ERR_SIGNAL_EN | INTR_SIGNAL_EN_BUS_RESET_DONE_SIGNAL_EN, INTR_SIGNAL_EN_OFFSET, i3c_mux);

    return TRUE;
}

/**
* @brief I3C MASTER ENTDAA函数
*
* @param dct DCT动态地址表指针,用于动态地址分配后存放DCT
* @param dynamic_addr DAT动态地址表指针,必须填DEV_DYNAMIC_ADDR_TABLE
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return true/false
*
* @note 初始化配置设备动态地址
*/
BYTE I3C_MASTER_ENTDAA(sDEV_CHAR_TABLE* dct, BYTE* dynamic_addr, BYTE i3c_mux)
{
    uint32_t temp_data = 0;
    BYTE dev_tmpcnt = 0;
    BYTE dev_num = 0;
    if ((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1))
    {
        i3c_dprint("controller select fault\n");
        return FALSE;
    }

    //DEV_ADDR_TABLE1_LOC1 is a fifo,can be written several times
    if (i3c_mux == I3C_MASTER0)
    {
        dev_tmpcnt = master0_dev_cnt;
        dev_num = sizeof(MASTER0_DEV_DYNAMIC_ADDR_TABLE);
    }
    else if (i3c_mux == I3C_MASTER1)
    {
        dev_tmpcnt = master1_dev_cnt;
        dev_num = sizeof(MASTER1_DEV_DYNAMIC_ADDR_TABLE);
    }
    while (dev_tmpcnt < dev_num) //循环发命令
    {
        //Set Device dynamic Address And Type of device I3C
        temp_data = 0;
        temp_data &= (DEV_ADDR_TABLE_LOC1_DEVICE_I3C);
        temp_data |= DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR(*(dynamic_addr + dev_tmpcnt)) | DEV_ADDR_TABLE_LOC1_IBI_WITH_DATA;
        if (DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR_PARITY(*(dynamic_addr + dev_tmpcnt)) != 0)
            temp_data |= DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR_PARITY(*(dynamic_addr + dev_tmpcnt));
        else
            temp_data &= DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR_PARITY0;
        I3C_WriteREG_DWORD(temp_data, (DEV_ADDR_TABLE1_LOC1_OFFSET + dev_tmpcnt * 4), i3c_mux);

        for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_CMD_QUEUE_READY_STS) == 0; timeout--)
        {
            if (timeout == 0 || i3c_error)
            {
                I3C_Master_IntrStatus(i3c_mux);
                i3c_error = 0;
                return FALSE;
            }
        }   //等CMD_QUEUE_READY_STS中断
        //config cmd port
        temp_data = 0;
        if (dev_tmpcnt != 0)
            temp_data |= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DEV_INDX(dev_tmpcnt);
        else
            temp_data &= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DEV_INDX0;
        temp_data |= COMMAND_QUEUE_PORT_COMMAND_ADDRESS_ASSIGNMENT | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_CMD(ENTDAA_BC_CMD) | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_TID(0x1000) | COMMAND_QUEUE_PORT_ADDRESS_ASSIGNMENT_DEV_COUNT(1) | COMMAND_QUEUE_PORT_ADDRESS_ASSIGNMENT_TOC | COMMAND_QUEUE_PORT_ADDRESS_ASSIGNMENT_ROC;
        I3C_WriteREG_DWORD(temp_data, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
        for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_RESP_READY_STS) == 0; timeout--)
        {
            if (timeout == 0 || i3c_error)
            {
                I3C_Master_IntrStatus(i3c_mux);
                i3c_error = 0;
                return FALSE;
            }
            nop;
        }//等待回复
        DWORD Response_error_status = 0;
        Response_error_status = I3C_ReadREG_DWORD(RESPONSE_QUEUE_PORT_OFFSET, i3c_mux) & RESPONSE_QUEUE_PORT_ERR_STS_MASK;
        if (RESPONSE_QUEUE_PORT_ERR_STS_NO_ERROR == Response_error_status)
        {
            //将DCT pointer指向最后一个成功分配动态地址的SLAVE设备
            temp_data = I3C_ReadREG_DWORD(DEV_CHAR_TABLE_POINTER_OFFSET, i3c_mux) & (~0xFFF80000);
            temp_data |= (dev_tmpcnt >> 19);
            I3C_WriteREG_DWORD(temp_data, DEV_CHAR_TABLE_POINTER_OFFSET, i3c_mux);
            //依次将所有设备的DCT属性读出并存入数组
            for (int i = 0; i < 4; i++)
            {
                (dct + dev_tmpcnt)->dev_char_table1_loc[i] = I3C_ReadREG_DWORD(DEV_CHAR_TABLE1_LOC1_OFFSET + i * 4, i3c_mux);
            }
            dev_tmpcnt++;
        }
        else if (RESPONSE_QUEUE_PORT_ERR_STS_ADDRESS_NACKED == Response_error_status)
        {
            i3c_dprint("DAT alloc finish\n");
        }
        else
        {
            i3c_dprint("No.%d's slave addr allocating failure,Response error = %#x\n", dev_tmpcnt, Response_error_status);
            I3C_Master_IntrStatus(i3c_mux);
            return FALSE;
        }
    }
    return TRUE;
}

/**
* @brief I3C MASTER SETDASA函数
*
* @param static_addr 静态地址
* @param dynamic_addr 动态地址
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return true/false
*
* @note 单独分配设备动态地址
*/
// BYTE I3C_MASTER_SETDASA(BYTE static_addr, BYTE dynamic_addr, BYTE i3c_mux)
// {
//     uint32_t temp_data = 0;
//     uint8_t match_cnt = 0;
//     if ((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1))
//     {
//         i3c_dprint("controller select fault\n");
//         return FALSE;
//     }

//     //Set Device Static Address And Type of device I3C
//     temp_data &= (DEV_ADDR_TABLE_LOC1_DEVICE_I3C);
//     temp_data |= DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR(dynamic_addr) | DEV_ADDR_TABLE_LOC1_STATIC_ADDRESS(static_addr);
//     if (DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR_PARITY(dynamic_addr) != 0)
//         temp_data |= DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR_PARITY(dynamic_addr);
//     else
//         temp_data &= DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR_PARITY0;
//     I3C_WriteREG_DWORD(temp_data, DEV_ADDR_TABLE1_LOC1_OFFSET + dev_tmpcnt * 4, i3c_mux);

//     for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_CMD_QUEUE_READY_STS) == 0; timeout--)
//     {
//         if (timeout == 0 || i3c_error)
//         {
//             I3C_Master_IntrStatus(i3c_mux);
//             i3c_error = 0;
//             return FALSE;
//         }

//     }   //等CMD_QUEUE_READY_STS中断
//     //config CMD port
//     temp_data = 0;
//     match_cnt = serach_current_dynamic_cnt(dynamic_addr, i3c_mux);
//     if (match_cnt != 0)
//     temp_data |= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DEV_INDX(match_cnt);
//     else
//     temp_data &= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DEV_INDX0;
//     temp_data |= COMMAND_QUEUE_PORT_COMMAND_ADDRESS_ASSIGNMENT | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_CMD(SETDASA_DR_CMD) | COMMAND_QUEUE_PORT_ADDRESS_ASSIGNMENT_DEV_COUNT(0x1) | COMMAND_QUEUE_PORT_ADDRESS_ASSIGNMENT_ROC | COMMAND_QUEUE_PORT_ADDRESS_ASSIGNMENT_TOC;
//     I3C_WriteREG_DWORD(temp_data, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
//     for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_RESP_READY_STS) == 0; timeout--)
//     {
//         if (timeout == 0 || i3c_error)
//         {
//             I3C_Master_IntrStatus(i3c_mux);
//             i3c_error = 0;
//             return FALSE;
//         }
//         nop;
//     }//等待回复
//     DWORD Response_error_status = 0;
//     Response_error_status = I3C_ReadREG_DWORD(RESPONSE_QUEUE_PORT_OFFSET, i3c_mux) & RESPONSE_QUEUE_PORT_ERR_STS_MASK;
//     if (RESPONSE_QUEUE_PORT_ERR_STS_NO_ERROR == Response_error_status)
//     {
//         // 将DCT pointer指向最后一个成功分配动态地址的SLAVE设备
//         temp_data = I3C_ReadREG_DWORD(DEV_CHAR_TABLE_POINTER_OFFSET, i3c_mux) & (~0xFFF80000);
//         temp_data |= (dev_tmpcnt >> 19);
//         I3C_WriteREG_DWORD(temp_data, DEV_CHAR_TABLE_POINTER_OFFSET, i3c_mux);
//         //依次将所有设备的DCT属性读出并存入数组
//         for (int i = 0; i < 4; i++)
//         {
//             (dct + dev_tmpcnt)->dev_char_table1_loc[i] = I3C_ReadREG_DWORD(DEV_CHAR_TABLE1_LOC1_OFFSET + i * 4, i3c_mux);
//         }
//         dev_tmpcnt++;
//         return TRUE;
//     }
//     else
//     {
//         I3C_Master_IntrStatus(i3c_mux);
//         return FALSE;
//     }
// }

/**
* @brief I3C MASTER broadcast ccc write函数
*
* @param data 传输数据指针
* @param bytelen 传输数据长度
* @param cmd broadcast cmd
* @param dbp 是否支持defining byte  0:不支持 1:支持
* @param db defining byte
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return true/false
*
* @note 无
*/
BYTE I3C_MASTER_BC_CCC_WRITE(uint8_t* data, uint16_t bytelen, BYTE cmd, uint8_t dbp, uint8_t db, BYTE i3c_mux)
{
    uint8_t* data_ptr = data;
    uint32_t data_len = bytelen;
    uint32_t temp_data = 0;
    if (((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1)) || (data_ptr == NULL) || (cmd > BROADCAST_CMD_MAX))
    {
        i3c_dprint("argument error\n");
        return FALSE;
    }

    if (bytelen != 0)
    {
        //配置传输数据长度
        if (dbp != 0)   //开启defining byte则写入defining byte
            temp_data |= COMMAND_QUEUE_PORT_COMMAND_TRANSFER_ARGUMENT | COMMAND_QUEUE_PORT_TRANSFER_ARGUMENT_DATA_LENGTH(bytelen) | COMMAND_QUEUE_PORT_TRANSFER_ARGUMENT_DB(db);
        else
            temp_data |= COMMAND_QUEUE_PORT_COMMAND_TRANSFER_ARGUMENT | COMMAND_QUEUE_PORT_TRANSFER_ARGUMENT_DATA_LENGTH(bytelen);
        I3C_WriteREG_DWORD(temp_data, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
        while (data_len)
        {
            register uint32_t len = (data_len <= 4 ? data_len : 4);
            data_len -= len;
            uDword data_temp;
            for (register uint32_t i = 0; i < len; i++)
            {
                data_temp.byte[i] = *data_ptr++;
            }

            for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_TX_THLD_STS) == 0; timeout--)
            {
                if (timeout == 0 || i3c_error)
                {
                    I3C_Master_IntrStatus(i3c_mux);
                    i3c_error = 0;
                    return FALSE;
                }

            }   //等TX THLD中断状态
            I3C_WriteREG_DWORD(data_temp.dword, TX_DATA_PORT_OFFSET, i3c_mux);//发送数据
        }
    }
    //issue broadcast ccc cmd
    temp_data = 0;
    temp_data &= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_BC_TRANSFER & COMMAND_QUEUE_PORT_COMMAND_TRANSFER_COMMAND & COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DEV_INDX0 & COMMAND_QUEUE_PORT_TRANSFER_COMMAND_WRITE & COMMAND_QUEUE_PORT_TRANSFER_COMMAND_SPEED_SDR0;
    temp_data |= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_CMD(cmd) | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_CP | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_ROC | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_TOC;
    if (dbp != 0)
        temp_data |= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DBP;
    I3C_WriteREG_DWORD(temp_data, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_RESP_READY_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            I3C_Master_IntrStatus(i3c_mux);
            i3c_error = 0;
            return FALSE;
        }

    }   //等INTR_STATUS_RESP_READY_STS中断
    DWORD Response_error_status = 0;
    Response_error_status = I3C_ReadREG_DWORD(RESPONSE_QUEUE_PORT_OFFSET, i3c_mux) & RESPONSE_QUEUE_PORT_ERR_STS_MASK;
    if (RESPONSE_QUEUE_PORT_ERR_STS_NO_ERROR != Response_error_status)
    {
        I3C_Master_IntrStatus(i3c_mux);
        return FALSE;
    }
    return TRUE;
}

/**
* @brief I3C MASTER directed ccc write函数
*
* @param dynamic_addr 设备动态地址
* @param data 传输数据指针
* @param bytelen 传输数据长度
* @param cmd directed cmd
* @param dbp 是否支持defining byte  0:不支持 1:支持
* @param db defining byte
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return true/false
*
* @note 无
*/
BYTE I3C_MASTER_DR_CCC_WRITE(uint8_t dynamic_addr, uint8_t* data, uint16_t bytelen, BYTE cmd, uint8_t dbp, uint8_t db, BYTE i3c_mux)
{
    uint8_t* data_ptr = data;
    uint32_t data_len = bytelen;
    uint32_t temp_data = 0;
    uint8_t match_cnt = 0;

    if (((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1)) || (data_ptr == NULL) || (cmd < DIRECTED_CMD_MIN))
    {
        i3c_dprint("argument error\n");
        return FALSE;
    }

    if (bytelen != 0)
    {
        //配置传输数据长度
        if (dbp != 0)   //开启defining byte则写入defining byte
            temp_data |= COMMAND_QUEUE_PORT_COMMAND_TRANSFER_ARGUMENT | COMMAND_QUEUE_PORT_TRANSFER_ARGUMENT_DATA_LENGTH(bytelen) | COMMAND_QUEUE_PORT_TRANSFER_ARGUMENT_DB(db);
        else
            temp_data |= COMMAND_QUEUE_PORT_COMMAND_TRANSFER_ARGUMENT | COMMAND_QUEUE_PORT_TRANSFER_ARGUMENT_DATA_LENGTH(bytelen);
        I3C_WriteREG_DWORD(temp_data, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
        while (data_len)
        {
            register uint32_t len = (data_len <= 4 ? data_len : 4);
            data_len -= len;
            uDword data_temp;
            for (register uint32_t i = 0; i < len; i++)
            {
                data_temp.byte[i] = *data_ptr++;
            }
            for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_TX_THLD_STS) == 0; timeout--)
            {
                if (timeout == 0 || i3c_error)
                {
                    I3C_Master_IntrStatus(i3c_mux);
                    i3c_error = 0;
                    return FALSE;
                }

            }   //等CMD_QUEUE_READY_STS中断
            I3C_WriteREG_DWORD(data_temp.dword, TX_DATA_PORT_OFFSET, i3c_mux);//发送数据
        }
    }
    //config dynamic addr
    temp_data = I3C_ReadREG_DWORD(DEV_ADDR_TABLE1_LOC1_OFFSET, i3c_mux);
    temp_data &= (DEV_ADDR_TABLE_LOC1_DEVICE_I3C);
    temp_data |= DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR(dynamic_addr);
    if (DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR_PARITY(dynamic_addr) != 0)
        temp_data |= DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR_PARITY(dynamic_addr);
    else
        temp_data &= DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR_PARITY0;
    I3C_WriteREG_DWORD(temp_data, DEV_ADDR_TABLE1_LOC1_OFFSET, i3c_mux);
    //issue directed ccc cmd
    temp_data = 0;
    temp_data &= COMMAND_QUEUE_PORT_COMMAND_TRANSFER_COMMAND & COMMAND_QUEUE_PORT_TRANSFER_COMMAND_WRITE & COMMAND_QUEUE_PORT_TRANSFER_COMMAND_SPEED_SDR0;
    temp_data |= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DR_TRANSFER | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_CMD(cmd) | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_CP | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_ROC | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_TOC;
    match_cnt = serach_current_dynamic_cnt(dynamic_addr, i3c_mux);
    if (match_cnt != 0)
        temp_data |= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DEV_INDX(match_cnt);
    else
        temp_data &= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DEV_INDX0;
    if (dbp != 0)
        temp_data |= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DBP;
    I3C_WriteREG_DWORD(temp_data, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_RESP_READY_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            I3C_Master_IntrStatus(i3c_mux);
            i3c_error = 0;
            return FALSE;
        }
    }   //等INTR_STATUS_RESP_READY_STS中断
    DWORD Response_error_status = 0;
    Response_error_status = I3C_ReadREG_DWORD(RESPONSE_QUEUE_PORT_OFFSET, i3c_mux) & RESPONSE_QUEUE_PORT_ERR_STS_MASK;
    if (RESPONSE_QUEUE_PORT_ERR_STS_NO_ERROR != Response_error_status)
    {
        I3C_Master_IntrStatus(i3c_mux);
        return FALSE;
    }
    return TRUE;
}

/**
* @brief I3C MASTER directed ccc read函数
*
* @param dynamic_addr 设备动态地址
* @param data 传输数据指针
* @param bytelen 传输数据长度
* @param cmd directed cmd
* @param dbp 是否支持defining byte  0:不支持 1:支持
* @param db defining byte
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return true/false
*
* @note 无
*/
BYTE I3C_MASTER_DR_CCC_READ(uint8_t dynamic_addr, uint8_t* data, uint16_t bytelen, BYTE cmd, uint8_t dbp, uint8_t db, BYTE i3c_mux)
{
    uint8_t* data_ptr = data;
    uint32_t data_len = bytelen;
    uint32_t temp_data = 0;
    uint8_t match_cnt = 0;

    if (((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1)) || (data_ptr == NULL) || (data_len == 0) || (cmd < DIRECTED_CMD_MIN))
    {
        i3c_dprint("argument error\n");
        return FALSE;
    }

    //配置传输数据长度
    if (dbp != 0)   //开启defining byte则写入defining byte
        temp_data |= COMMAND_QUEUE_PORT_COMMAND_TRANSFER_ARGUMENT | COMMAND_QUEUE_PORT_TRANSFER_ARGUMENT_DATA_LENGTH(bytelen) | COMMAND_QUEUE_PORT_TRANSFER_ARGUMENT_DB(db);
    else
        temp_data |= COMMAND_QUEUE_PORT_COMMAND_TRANSFER_ARGUMENT | COMMAND_QUEUE_PORT_TRANSFER_ARGUMENT_DATA_LENGTH(bytelen);
    I3C_WriteREG_DWORD(temp_data, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    //config dynamic addr
    temp_data = I3C_ReadREG_DWORD(DEV_ADDR_TABLE1_LOC1_OFFSET, i3c_mux);
    temp_data &= (DEV_ADDR_TABLE_LOC1_DEVICE_I3C);
    temp_data |= DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR(dynamic_addr);
    if (DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR_PARITY(dynamic_addr) != 0)
        temp_data |= DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR_PARITY(dynamic_addr);
    else
        temp_data &= DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR_PARITY0;
    I3C_WriteREG_DWORD(temp_data, DEV_ADDR_TABLE1_LOC1_OFFSET, i3c_mux);
    //issue directed ccc cmd
    temp_data = 0;
    temp_data &= COMMAND_QUEUE_PORT_COMMAND_TRANSFER_COMMAND & COMMAND_QUEUE_PORT_TRANSFER_COMMAND_SPEED_SDR0;
    temp_data |= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DR_TRANSFER | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_CMD(cmd) | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_CP | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_ROC | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_TOC | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_READ;
    match_cnt = serach_current_dynamic_cnt(dynamic_addr, i3c_mux);
    if (match_cnt != 0)
        temp_data |= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DEV_INDX(match_cnt);
    else
        temp_data &= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DEV_INDX0;
    if (dbp != 0)
        temp_data |= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DBP;
    I3C_WriteREG_DWORD(temp_data, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_RESP_READY_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            I3C_Master_IntrStatus(i3c_mux);
            i3c_error = 0;
            return FALSE;
        }

    }   //等INTR_STATUS_RESP_READY_STS中断

    while (data_len)
    {
        register uint32_t len = (data_len <= 4 ? data_len : 4);
        data_len -= len;
        for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_RX_THLD_STS) == 0; timeout--)
        {
            if (timeout == 0 || i3c_error)
            {
                I3C_Master_IntrStatus(i3c_mux);
                i3c_error = 0;
                return FALSE;
            }
        }
        uDword data_temp;
        data_temp.dword = I3C_ReadREG_DWORD(RX_DATA_PORT_OFFSET, i3c_mux);
        for (register uint32_t i = 0; i < len; i++)
        {
            *data_ptr++ = data_temp.byte[i];
        }
    }//读取数据
    for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_RESP_READY_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            I3C_Master_IntrStatus(i3c_mux);
            i3c_error = 0;
            return FALSE;
        }
    }   //等INTR_STATUS_RESP_READY_STS中断
    DWORD Response_error_status = 0;
    Response_error_status = I3C_ReadREG_DWORD(RESPONSE_QUEUE_PORT_OFFSET, i3c_mux) & RESPONSE_QUEUE_PORT_ERR_STS_MASK;
    if (RESPONSE_QUEUE_PORT_ERR_STS_NO_ERROR != Response_error_status)
    {
        I3C_Master_IntrStatus(i3c_mux);
        return FALSE;
    }
    return TRUE;
}

/**
* @brief I3C MASTER private write函数
*
* @param dynamic_addr 动态地址
* @param data 传输数据指针
* @param bytelen 传输数据长度
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return true/false
*
* @note 无
*/
BYTE I3C_MASTER_PV_WRITE_WITH7E(uint8_t dynamic_addr, uint8_t* data, uint16_t bytelen, BYTE i3c_mux)
{
    uint8_t* data_ptr = data;
    uint32_t data_len = bytelen;
    uint32_t temp_data = 0;
    uint8_t match_cnt = 0;

    if (((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1)) || (data_ptr == NULL) || (data_len == 0))
    {
        i3c_dprint("argument error\n");
        return FALSE;
    }

    if (bytelen != 0)
    {
        //配置传输数据长度
        temp_data |= COMMAND_QUEUE_PORT_COMMAND_TRANSFER_ARGUMENT | COMMAND_QUEUE_PORT_TRANSFER_ARGUMENT_DATA_LENGTH(bytelen);
        I3C_WriteREG_DWORD(temp_data, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
        while (data_len)
        {
            register uint32_t len = (data_len <= 4 ? data_len : 4);
            data_len -= len;
            uDword data_temp;
            for (register uint32_t i = 0; i < len; i++)
            {
                data_temp.byte[i] = *data_ptr++;
            }
            for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_TX_THLD_STS) == 0; timeout--)
            {
                if (timeout == 0 || i3c_error)
                {
                    I3C_Master_IntrStatus(i3c_mux);
                    i3c_error = 0;
                    return FALSE;
                }
            }   //等待INTR_STATUS_TX_THLD_STS 
            I3C_WriteREG_DWORD(data_temp.dword, TX_DATA_PORT_OFFSET, i3c_mux);//发送数据
        }
    }
    //config dynamic addr
    temp_data = I3C_ReadREG_DWORD(DEV_ADDR_TABLE1_LOC1_OFFSET, i3c_mux);
    temp_data &= (DEV_ADDR_TABLE_LOC1_DEVICE_I3C);
    temp_data |= DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR(dynamic_addr);
    if (DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR_PARITY(dynamic_addr) != 0)
        temp_data |= DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR_PARITY(dynamic_addr);
    else
        temp_data &= DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR_PARITY0;
    I3C_WriteREG_DWORD(temp_data, DEV_ADDR_TABLE1_LOC1_OFFSET, i3c_mux);
    //issue private transfer
    temp_data = 0;
    temp_data &= (COMMAND_QUEUE_PORT_COMMAND_TRANSFER_COMMAND & COMMAND_QUEUE_PORT_TRANSFER_COMMAND_CP0 & COMMAND_QUEUE_PORT_TRANSFER_COMMAND_SPEED_SDR0 & COMMAND_QUEUE_PORT_TRANSFER_COMMAND_WRITE);
    temp_data |= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_ROC | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_TOC;
    match_cnt = serach_current_dynamic_cnt(dynamic_addr, i3c_mux);
    if (match_cnt != 0)
        temp_data |= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DEV_INDX(match_cnt);
    else
        temp_data &= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DEV_INDX0;
    I3C_WriteREG_DWORD(temp_data, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_RESP_READY_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            I3C_Master_IntrStatus(i3c_mux);
            i3c_error = 0;
            return FALSE;
        }
    }   //等INTR_STATUS_RESP_READY_STS中断
    DWORD Response_error_status = 0;
    Response_error_status = I3C_ReadREG_DWORD(RESPONSE_QUEUE_PORT_OFFSET, i3c_mux) & RESPONSE_QUEUE_PORT_ERR_STS_MASK;
    if (RESPONSE_QUEUE_PORT_ERR_STS_NO_ERROR != Response_error_status)
    {
        I3C_Master_IntrStatus(i3c_mux);
        return FALSE;
    }
    return TRUE;
}

/**
* @brief I3C MASTER private read函数
*
* @param dynamic_addr 动态地址
* @param data 传输数据指针
* @param bytelen 传输数据长度
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return true/false
*
* @note 无
*/
BYTE I3C_MASTER_PV_READ_WITH7E(uint8_t dynamic_addr, uint8_t* data, uint16_t bytelen, BYTE i3c_mux)
{
    uint8_t* data_ptr = data;
    uint32_t data_len = bytelen;
    uint32_t temp_data = 0;
    uint8_t match_cnt = 0;

    if (((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1)) || (data_ptr == NULL) || (data_len == 0))
    {
        i3c_dprint("argument error\n");
        return FALSE;
    }

    //配置传输长度
    temp_data |= COMMAND_QUEUE_PORT_COMMAND_TRANSFER_ARGUMENT | COMMAND_QUEUE_PORT_TRANSFER_ARGUMENT_DATA_LENGTH(bytelen);
    I3C_WriteREG_DWORD(temp_data, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    //config dynamic addr
    temp_data = I3C_ReadREG_DWORD(DEV_ADDR_TABLE1_LOC1_OFFSET, i3c_mux);
    temp_data &= (DEV_ADDR_TABLE_LOC1_DEVICE_I3C);
    temp_data |= DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR(dynamic_addr);
    if (DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR_PARITY(dynamic_addr) != 0)
        temp_data |= DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR_PARITY(dynamic_addr);
    else
        temp_data &= DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR_PARITY0;
    I3C_WriteREG_DWORD(temp_data, DEV_ADDR_TABLE1_LOC1_OFFSET, i3c_mux);
    //issue private transfer
    temp_data = 0;
    temp_data &= (COMMAND_QUEUE_PORT_COMMAND_TRANSFER_COMMAND & COMMAND_QUEUE_PORT_TRANSFER_COMMAND_CP0 & COMMAND_QUEUE_PORT_TRANSFER_COMMAND_SPEED_SDR0);
    temp_data |= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_ROC | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_TOC | COMMAND_QUEUE_PORT_TRANSFER_COMMAND_READ;
    match_cnt = serach_current_dynamic_cnt(dynamic_addr, i3c_mux);
    if (match_cnt != 0)
        temp_data |= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DEV_INDX(match_cnt);
    else
        temp_data &= COMMAND_QUEUE_PORT_TRANSFER_COMMAND_DEV_INDX0;
    I3C_WriteREG_DWORD(temp_data, COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_RESP_READY_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            I3C_Master_IntrStatus(i3c_mux);
            i3c_error = 0;
            return FALSE;
        }
    }   //等INTR_STATUS_RESP_READY_STS中断
    while (data_len)
    {
        register uint32_t len = (data_len <= 4 ? data_len : 4);
        data_len -= len;
        for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_RX_THLD_STS) == 0; timeout--)
        {
            if (timeout == 0 || i3c_error)
            {
                I3C_Master_IntrStatus(i3c_mux);
                i3c_error = 0;
                return FALSE;
            }
        }
        uDword data_temp;
        data_temp.dword = I3C_ReadREG_DWORD(RX_DATA_PORT_OFFSET, i3c_mux);
        for (register uint32_t i = 0; i < len; i++)
        {
            *data_ptr++ = data_temp.byte[i];
        }
    }//读取数据
    for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_RESP_READY_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            I3C_Master_IntrStatus(i3c_mux);
            i3c_error = 0;
            return FALSE;
        }
    }   //等INTR_STATUS_RESP_READY_STS中断
    DWORD Response_error_status = 0;
    Response_error_status = I3C_ReadREG_DWORD(RESPONSE_QUEUE_PORT_OFFSET, i3c_mux) & RESPONSE_QUEUE_PORT_ERR_STS_MASK;
    if (RESPONSE_QUEUE_PORT_ERR_STS_NO_ERROR != Response_error_status)
    {
        I3C_Master_IntrStatus(i3c_mux);
        return FALSE;
    }
    return TRUE;
}

/**
* @brief I3C MASTER private write/read函数
*
* @param dynamic_addr 动态地址
* @param wdata 写入数据结构指针
* @param wbytelen 写入数据长度
* @param rdata 读出数据结构指针
* @param rbytelen 读出数据长度
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return true/false
*
* @note 无
*/
BYTE I3C_MASTER_PV_WRITE_THEN_READ_WITH7E(uint8_t dynamic_addr, uint8_t* wdata, uint16_t wbytelen, uint8_t* rdata, uint16_t rbytelen, BYTE i3c_mux)
{
    uint8_t* wdata_ptr = wdata;
    uint32_t wdata_len = wbytelen;
    uint8_t* rdata_ptr = rdata;
    uint32_t rdata_len = rbytelen;
    uint32_t temp_data = 0;

    if (((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1)) || wdata_ptr == NULL || wdata_len == 0 || rdata_ptr == NULL || rdata_len == 0)
    {
        i3c_dprint("argument error\n");
        return FALSE;
    }

    //需要提前禁掉INTR_STATUS_RX_THLD_STS中断，采取在函数等状态位的方式来读
    temp_data = I3C_ReadREG_DWORD(INTR_SIGNAL_EN_OFFSET, i3c_mux) & (~INTR_SIGNAL_EN_RX_THLD_SIGNAL_EN);
    I3C_WriteREG_DWORD(temp_data, INTR_SIGNAL_EN_OFFSET, i3c_mux);

    if (wbytelen != 0)
    {
        if (I3C_MASTER_PV_WRITE_WITH7E(dynamic_addr, wdata, wbytelen, i3c_mux) == FALSE)
        {
            return FALSE;
        }
    }
    for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) & INTR_STATUS_RX_THLD_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            I3C_Master_IntrStatus(i3c_mux);
            i3c_error = 0;
            return FALSE;
        }

    }
    if (rbytelen != 0)
    {
        if (I3C_MASTER_PV_READ_WITH7E(dynamic_addr, rdata, rbytelen, i3c_mux) == FALSE)
        {
            return FALSE;
        }
    }
    //读完数据后重新使能INTR_STATUS_RX_THLD_STS中断
    temp_data = I3C_ReadREG_DWORD(INTR_SIGNAL_EN_OFFSET, i3c_mux) | (INTR_SIGNAL_EN_RX_THLD_SIGNAL_EN);
    I3C_WriteREG_DWORD(temp_data, INTR_SIGNAL_EN_OFFSET, i3c_mux);
    return TRUE;
}

/**
* @brief I3C MASTER IBI函数
*
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return true/false
*
* @note 当中断服务函数中触发IBI中断请求，则调用此函数处理IBI中断
*/
void I3C_MASTER_IBI_Handler(BYTE i3c_mux)
{
    //disable intr signal en
    I3C_WriteREG_DWORD((I3C_ReadREG_DWORD(INTR_SIGNAL_EN_OFFSET, i3c_mux)) & (~INTR_SIGNAL_EN_IBI_THLD_SIGNAL_EN), INTR_SIGNAL_EN_OFFSET, i3c_mux);

    //read ibi_queue_status
    uint32_t ibi_queue_status = 0;
    uint8_t data_length = 0;
repeat_ibi:
    ibi_queue_status = I3C_ReadREG_DWORD(IBI_QUEUE_STATUS_OFFSET, i3c_mux);
    data_length = ibi_queue_status & IBI_QUEUE_STATUS_DATA_LENGTH;
    if (((ibi_queue_status & IBI_QUEUE_STATUS_IBI_STATUS) == 0) && (data_length > 0))
    {
        uint8_t* ibi_ptr = (uint8_t*)malloc(data_length);
        while (data_length)
        {
            register uint32_t len = (data_length <= 4 ? data_length : 4);
            data_length -= len;
            uDword data_temp;
            data_temp.dword = I3C_ReadREG_DWORD(IBI_QUEUE_DATA_OFFSET, i3c_mux);
            i3c_dprint("data_temp.dword:%x\n", data_temp.dword);
            for (register uint32_t i = 0; i < len; i++)
            {
                *ibi_ptr++ = data_temp.byte[i];
            }
        }//读取数据
    }
    else
    {
        //register ibi_id
        uint8_t ibi_id = (uint8_t)((ibi_queue_status & IBI_QUEUE_STATUS_IBI_ID) >> 8);
        uint8_t new_static_addr = (uint8_t)((ibi_id >> 1) & 0x7F);
        if ((ibi_id & 0x00000001UL) == 0)//hot-join
        {
            i3c_dprint("hot join\n");
            if (i3c_mux == I3C_MASTER0)
            {
                MASTER0_DEV_STATIC_ADDR_TABLE[(sizeof(MASTER0_DEV_STATIC_ADDR_TABLE) + 1)] = new_static_addr;    //store new dynamic addr to DEV_STATIC_ADDR_TABLE
                I3C_MASTER_ENTDAA(master0_dev_read_char_table, MASTER0_DEV_DYNAMIC_ADDR_TABLE, i3c_mux); //specify a dynamic addr
            }

            else if (i3c_mux == I3C_MASTER1)
            {
                MASTER1_DEV_STATIC_ADDR_TABLE[(sizeof(MASTER1_DEV_STATIC_ADDR_TABLE) + 1)] = new_static_addr;    //store new dynamic addr to DEV_STATIC_ADDR_TABLE
                I3C_MASTER_ENTDAA(master1_dev_read_char_table, MASTER1_DEV_DYNAMIC_ADDR_TABLE, i3c_mux); //specify a dynamic addr
            }
        }
        else
        {  //if no data and rw =1,then get ibi status
            I3C_MASTER_DR_CCC_READ(new_static_addr, get_status, 2, GETSTATUS_DR_CMD, 0, 0, i3c_mux);
            i3c_dprint("ibi status:0x%04x\n", get_status);
        }
    }
    if ((I3C_ReadREG_DWORD(QUEUE_STATUS_LEVEL_OFFSET, i3c_mux) & QUEUE_STATUS_LEVEL_IBI_STS_CNT) > 0)   //have other ibi req
        goto repeat_ibi;
    else
        //enable intr signal en
        I3C_WriteREG_DWORD(((I3C_ReadREG_DWORD(INTR_SIGNAL_EN_OFFSET, i3c_mux)) | INTR_SIGNAL_EN_IBI_THLD_SIGNAL_EN), INTR_SIGNAL_EN_OFFSET, i3c_mux);
}
/**
* @brief SLAVE初始化函数
*
* @param static_addr 设备静态地址
* @param idpartno 部件编号
* @param dcr 设备类型
* @param bcr 总线能力
* @param i3c_mux 配置选择，如I3C_SLAVE0、I3C_SLAVE1
* @return 无
*
* @note 无
*/
void I3C_Slave_Init(BYTE static_addr, uint32_t idpartno, uint8_t dcr, uint8_t bcr, uint8_t i3c_mux)
{
    uint32_t temp_data = 0;
    if ((i3c_mux != I3C_SLAVE0) && (i3c_mux != I3C_SLAVE1))
    {
        i3c_dprint("controller select fault\n");
        return;
    }
    //配置静态地址
    DWORD rdata;
    rdata = I3C_ReadREG_DWORD(CONFIG_OFFSET, i3c_mux);
    rdata &= (~0xFE000000);
    rdata |= (static_addr << 25) | CONFIG_SLVENA;
    I3C_WriteREG_DWORD(rdata, CONFIG_OFFSET, i3c_mux);

    temp_data = idpartno;
    I3C_WriteREG_DWORD(temp_data, IDPARTNO_OFFSET, i3c_mux);    //set idpartno
    temp_data = 0;
    temp_data |= IDEXT_DCR(dcr) | IDEXT_BCR(bcr);
    I3C_WriteREG_DWORD(temp_data, IDEXT_OFFSET, i3c_mux);   //set dcr&bcr

    I3C_WriteREG_DWORD(((I3C_ReadREG_DWORD(TCCLOCK_OFFSET, i3c_mux) & 0x000000FF) | ((((uint32_t)(HIGH_CHIP_CLOCK * 2 / 1000000)) << 8) & 0x0000FF00)), TCCLOCK_OFFSET, i3c_mux);   //设置TCCLOCK
    I3C_WriteREG_DWORD(0x0, INTSET_OFFSET, i3c_mux);   //中断全部失能
    I3C_WriteREG_DWORD(0x0, DMACTRL_OFFSET, i3c_mux);   //DMA失能
}

/**
* @brief SLAVE中断类型使能
*
* @param type 中断类型
* @param i3c_mux 配置选择，如I3C_SLAVE0、I3C_SLAVE1
* @return 无
*
* @note 无
*/
BYTE I3C_SLAVE_INT_ENABLE(DWORD tpye, BYTE i3c_mux)
{
    if ((i3c_mux != I3C_SLAVE0) && (i3c_mux != I3C_SLAVE1))
    {
        i3c_dprint("controller select fault\n");
        return FALSE;
    }
    I3C_WriteREG_DWORD((I3C_ReadREG_DWORD(INTSET_OFFSET, i3c_mux) | tpye), INTSET_OFFSET, i3c_mux);
    return TRUE;
}

/**
* @brief SLAVE中断失能函数
*
* @param type 中断类型
* @param i3c_mux 配置选择，如I3C_SLAVE0、I3C_SLAVE1
* @return 操作结果
*
* @note 无
*/
BYTE I3C_SLAVE_INT_DISABLE(DWORD tpye, BYTE i3c_mux)
{
    if ((i3c_mux != I3C_SLAVE0) && (i3c_mux != I3C_SLAVE1))
    {
        i3c_dprint("controller select fault\n");
        return FALSE;
    }
    I3C_WriteREG_DWORD((I3C_ReadREG_DWORD(INTSET_OFFSET, i3c_mux) & (~tpye)), INTSET_OFFSET, i3c_mux);
    return TRUE;
}

/**
* @brief SLAVE ibi hotjoin函数
*
* @param addr slave static addr
* @param idpartno 部件编号
* @param dcr 设备类型
* @param bcr 总线能力
* @param i3c_mux 配置选择，如I3C_SLAVE0、I3C_SLAVE1
* @return 操作结果
*
* @note 调用此接口发起slave ibi hotjoin
*/
BYTE I3C_SLAVE_IBI_HOTJOIN(uint8_t addr, uint32_t idpartno, uint8_t dcr, uint8_t bcr, BYTE i3c_mux)
{
    uint32_t temp_data = 0;
    if ((i3c_mux != I3C_SLAVE0) && (i3c_mux != I3C_SLAVE1))
    {
        i3c_dprint("controller select fault\n");
        return FALSE;
    }

    temp_data |= idpartno;
    I3C_WriteREG_DWORD(temp_data, IDPARTNO_OFFSET, i3c_mux);    //set idpartno
    temp_data = 0;
    temp_data |= IDEXT_DCR(dcr) | IDEXT_BCR(bcr);
    I3C_WriteREG_DWORD(temp_data, IDEXT_OFFSET, i3c_mux);   //set dcr&bcr
    temp_data = 0;
    temp_data |= HOTJOIN_REQUEST;
    I3C_WriteREG_DWORD(temp_data, CTRL_OFFSET, i3c_mux);   //set event
    temp_data = 0;
    temp_data |= CONFIG_ADDR(addr) | CONFIG_SLVENA;
    I3C_WriteREG_DWORD(temp_data, CONFIG_OFFSET, i3c_mux);   //config 7bit static addr & slvena
    return TRUE;
}

/**
* @brief SLAVE write data函数
*
* @param addr slave static addr
* @param ibi_data ibi需要发送的data
* @param idpartno 部件编号
* @param dcr 设备类型
* @param bcr 总线能力
* @param i3c_mux 配置选择，如I3C_SLAVE0、I3C_SLAVE1
* @return 操作结果
*
* @note 调用此接口发起ibi 发送data
*/
BYTE I3C_SLAVE_IBI_DATA(uint8_t ibi_data, uint32_t idpartno, uint8_t dcr, uint8_t bcr, BYTE i3c_mux)
{
    uint32_t temp_data = 0;
    if ((i3c_mux != I3C_SLAVE0) && (i3c_mux != I3C_SLAVE1))
    {
        i3c_dprint("controller select fault\n");
        return FALSE;
    }

    temp_data |= idpartno;
    I3C_WriteREG_DWORD(temp_data, IDPARTNO_OFFSET, i3c_mux);    //set idpartno
    temp_data = 0;
    temp_data |= IDEXT_DCR(dcr) | IDEXT_BCR(bcr);
    I3C_WriteREG_DWORD(temp_data, IDEXT_OFFSET, i3c_mux);      //set dcr&bcr

    temp_data = 0;
    temp_data |= START_IBI | IBI_DATA(ibi_data);
    I3C_WriteREG_DWORD(temp_data, CTRL_OFFSET, i3c_mux);   //set event

    temp_data = 0;
    temp_data |= CONFIG_SLVENA;
    I3C_WriteREG_DWORD(temp_data, CONFIG_OFFSET, i3c_mux);   //config 7bit static addr & slvena

    return TRUE;
}

/**
* @brief SLAVE write data函数
*
* @param data 写入数据的存放位置指针
* @param bytelen 需要写入的数据长度
* @param i3c_mux 配置选择，如I3C_SLAVE0、I3C_SLAVE1
* @return 操作结果
*
* @note 调用此接口发起ibi 发送data
*/
BYTE I3C_SLAVE_WRITE(uint8_t* data, uint8_t bytelen, BYTE i3c_mux)
{
    uint8_t* data_ptr = data;
    uint32_t data_len = bytelen;
    uint32_t temp_data = 0;

    if ((data_ptr == NULL || data_len == 0) || ((i3c_mux != I3C_SLAVE0) && (i3c_mux != I3C_SLAVE1)))
    {
        i3c_dprint("argument error\n");
        return FALSE;
    }

    while (data_len--)
    {
        temp_data = 0;
        if (data_len == 0)
        {
            temp_data |= WDATAB_LASTBYTE;   //lastbyte need set flag
        }
        temp_data |= *(data_ptr + bytelen - data_len - 1);
        I3C_WriteREG_DWORD(temp_data, WDATAB_OFFSET, i3c_mux);
    }
    return TRUE;
}

/**
* @brief SLAVE read data函数
*
* @param data 读出数据的存放位置指针
* @param bytelen 需要读出的数据长度
* @param i3c_mux 配置选择，如I3C_SLAVE0、I3C_SLAVE1
* @return 操作结果
*
* @note 调用此接口发起ibi 发送data
*/
BYTE I3C_SLAVE_READ(uint8_t* data, uint8_t bytelen, BYTE i3c_mux)
{
    uint8_t* data_ptr = data;
    uint32_t data_len = bytelen;

    if ((data_ptr == NULL || data_len == 0) || ((i3c_mux != I3C_SLAVE0) && (i3c_mux != I3C_SLAVE1)))
    {
        i3c_dprint("argument error\n");
        return FALSE;
    }

    for (uint32_t timeout = I3C_TIMEOUT; (I3C_ReadREG_DWORD(STATUS_OFFSET, i3c_mux) & STATUS_RXPEND) == 0; timeout--)
    {
        if (timeout == 0)
        {
            i3c_dprint("no data can be received\n");
            return FALSE;
        }
    }   //等STATUS_RXPEND置位

    while (data_len--)
    {
        *(data_ptr++) = I3C_ReadREG_DWORD(RDATAB_OFFSET, i3c_mux);
    }
    return TRUE;
}
