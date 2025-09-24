/*
 * @Author: Iversu
 * @LastEditors: Iversu coolwangzi@126.com
 * @LastEditTime: 2025-07-07 11:14:56
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

 /**
 * @brief I3C地址转换函数
 *
 * @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1、I3C_SLAVE0、I3C_SLAVE1
 *
 * @return I3C baseaddr
 *
 * @note 选择配置，输出该配置对应的寄存器基地址
 */
DWORD I3c_Channel_Baseaddr(BYTE i3c_mux)
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
void I3c_WriteREG_DWORD(DWORD value, WORD regoffset, BYTE i3c_mux)
{
    REG32(REG_ADDR(I3c_Channel_Baseaddr(i3c_mux), regoffset)) = value;
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
DWORD I3c_ReadREG_DWORD(WORD regoffset, BYTE i3c_mux)
{
    return REG32(REG_ADDR(I3c_Channel_Baseaddr(i3c_mux), regoffset));
}

/**
* @brief MASTER接口初始化函数
*
* @param regoffset 寄存器地址偏移
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return true/false
*
* @note 配置接口为MASTER功能
*/
BYTE I3C_Master_Init(BYTE addr, uint32_t speed, BYTE i3c_mux)
{
    if ((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1))
        return FALSE;
    uint32_t cnt = (CHIP_CLOCK_INT_HIGH / speed);
    uint32_t hcnt = ((cnt * 1) / 2);    //50%占空比
    uint32_t lcnt = (cnt - hcnt);
    if (hcnt >= PP_TIMING_HCNT_MAX)hcnt = PP_TIMING_HCNT_MAX;
    if (lcnt >= PP_TIMING_HCNT_MAX)lcnt = PP_TIMING_LCNT_MAX;
    if (hcnt <= PP_TIMING_HCNT_MIN)hcnt = PP_TIMING_HCNT_MIN;
    if (lcnt <= PP_TIMING_LCNT_MIN)lcnt = PP_TIMING_LCNT_MIN;
    cnt = hcnt + lcnt;

    //Controls whether or not I3c_master is enabled And I2C Slave Present 
    I3c_WriteREG_DWORD((I3C_MASTER_DEVICE_CTRL_ENABLE | I3C_MASTER_DEVICE_CTRL_I2C_SLAVE_PRESENT), DEVICE_CTRL_OFFSET, i3c_mux);
    //Set Device Static Address And Type of device I2C
    I3c_WriteREG_DWORD((I3C_MASTER_DEV_ADDR_TABLE_LOC1_DEVICE_I2C | I3C_MASTER_DEV_ADDR_TABLE_LOC1_STATIC_ADDRESS(addr)), DEV_ADDR_TABLE_LOC1_OFFSET, i3c_mux);
    //Set I3C PP_TIMING
    I3c_WriteREG_DWORD((I3C_MASTER_SCL_I3C_PP_TIMING_LCNT(cnt) | I3C_MASTER_SCL_I3C_PP_TIMING_HCNT(cnt)), SCL_I3C_PP_TIMING_OFFSET, i3c_mux);
    //Set IBI SIR Request Rejection Control of Command Queue Ready,Transmit And Receive And IBI  Buffer Threshold
    I3c_WriteREG_DWORD((I3C_MASTER_INTR_STATUS_EN_BUS_RESET_DONE_STS_EN | I3C_MASTER_INTR_STATUS_EN_TRANSFER_ERR_STS_EN | I3C_MASTER_INTR_STATUS_EN_TRANSFER_ABORT_STS_EN | I3C_MASTER_INTR_STATUS_EN_RESP_READY_STS_EN | I3C_MASTER_INTR_STATUS_EN_CMD_QUEUE_READY_STS_EN | I3C_MASTER_INTR_STATUS_EN_TX_THLD_STS_EN | I3C_MASTER_INTR_STATUS_EN_RX_THLD_STS_EN), INTR_STATUS_EN_OFFSET, i3c_mux);
    //DMA  master resp thld
    I3c_WriteREG_DWORD((I3C_MASTER_QUEUE_THLD_CTRL_RESP_BUF_THLD(0x0) | I3C_MASTER_QUEUE_THLD_CTRL_CMD_EMPTY_BUF_THLD(0x1)), QUEUE_THLD_CTRL_OFFSET, i3c_mux);
    //Set data buff thld
    I3c_WriteREG_DWORD((I3C_MASTER_DATA_BUFFER_THLD_CTRL_TX_START_THLD(TX_EMPTY_BUF_THLD_1) | I3C_MASTER_DATA_BUFFER_THLD_CTRL_RX_START_THLD(RX_BUF_THLD_1)) | (I3C_MASTER_DATA_BUFFER_THLD_CTRL_RX_BUF_THLD(RX_BUF_THLD_1) | I3C_MASTER_DATA_BUFFER_THLD_CTRL_TX_EMPTY_BUF_THLD(TX_EMPTY_BUF_THLD_1)), DATA_BUFFER_THLD_CTRL_OFFSET, i3c_mux);
    return TRUE;
}

/**
* @brief MASTER读取及清除错误状态函数
*
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return 无
*
* @note 无
*/
uint32_t I3C_Master_ErrorStatus(BYTE i3c_mux)
{
    if ((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1))
    {
        i3c_dprint("controller select fault\n");
        return 0;
    }

    uint32_t err_sts = (I3c_ReadREG_DWORD(RESPONSE_QUEUE_PORT_OFFSET, i3c_mux) & I3C_MASTER_RESPONSE_QUEUE_PORT_ERR_STS_MASK);
    if (err_sts != I3C_MASTER_RESPONSE_QUEUE_PORT_ERR_STS_NO_ERROR)
    {
        i3c_error = 1;
        switch (err_sts)
        {
        case I3C_MASTER_RESPONSE_QUEUE_PORT_ERR_STS_NO_ERROR:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_NO_ERROR\n", i3c_mux);
            break;
        case I3C_MASTER_RESPONSE_QUEUE_PORT_ERR_STS_CRC_ERROR:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_CRC_ERROR\n", i3c_mux);
            break;
        case I3C_MASTER_RESPONSE_QUEUE_PORT_ERR_STS_PARITY_ERROR:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_PARITY_ERROR\n", i3c_mux);
            break;
        case I3C_MASTER_RESPONSE_QUEUE_PORT_ERR_STS_FRAME_ERROR:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_FRAME_ERROR\n", i3c_mux);
            break;
        case I3C_MASTER_RESPONSE_QUEUE_PORT_ERR_STS_I3C_BROADCAST_ADDRESS_NACK_ERROR:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_I3C_BROADCAST_ADDRESS_NACK_ERROR\n", i3c_mux);
            break;
        case I3C_MASTER_RESPONSE_QUEUE_PORT_ERR_STS_ADDRESS_NACKED:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_ADDRESS_NACKED\n", i3c_mux);
            break;
        case I3C_MASTER_RESPONSE_QUEUE_PORT_ERR_STS_RECELVE_BUFFER_OVERFLOWTRANSMIT_BUFFER_UNDERFLOW:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_RECELVE_BUFFER_OVERFLOWTRANSMIT_BUFFER_UNDERFLOW\n", i3c_mux);
            break;
        case I3C_MASTER_RESPONSE_QUEUE_PORT_ERR_STS_TRANSFER_ABORTED:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_TRANSFER_ABORTED\n", i3c_mux);
            break;
        case I3C_MASTER_RESPONSE_QUEUE_PORT_ERR_STS_I2C_SLAVE_WRITE_DATA_NACK_ERROR:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_I2C_SLAVE_WRITE_DATA_NACK_ERROR\n", i3c_mux);
            break;
        case I3C_MASTER_RESPONSE_QUEUE_PORT_ERR_STS_PEC_ERROR:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_PEC_ERROR\n", i3c_mux);
            break;
        default:
            i3c_dprint("I3C_MASTER%d_RESPONSE_QUEUE_PORT_ERR_STS_RESERVED\n", i3c_mux);
            break;
        }
        I3c_WriteREG_DWORD((I3c_ReadREG_DWORD(DEVICE_CTRL_OFFSET, i3c_mux) | I3C_MASTER_DEVICE_CTRL_RESUME), DEVICE_CTRL_OFFSET, i3c_mux);//清除错误并且复位
        for (uint32_t timeout = I3C_TIMEOUT; (I3c_ReadREG_DWORD(DEVICE_CTRL_OFFSET, i3c_mux) & I3C_MASTER_DEVICE_CTRL_RESUME); timeout--)
        {
            if (timeout == 0)
            {
                i3c_dprint("fail:I3C_MASTER%d_DEVICE_CTRL=%#x\n", i3c_mux, I3c_ReadREG_DWORD(DEVICE_CTRL_OFFSET, i3c_mux));
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
uint32_t I3C_Master_IntStatus(BYTE i3c_mux)
{
    uint32_t int_status = I3c_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux);
    if (int_status & I3C_MASTER_INTR_STATUS_BUS_RESET_DONE_STS)
    {
        i3c_dprint("I3C_MASTER%d_INTR_STATUS_BUS_RESET_DONE_STS\n", i3c_mux);//出错
        I3c_WriteREG_DWORD((I3c_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) | I3C_MASTER_INTR_STATUS_BUS_RESET_DONE_STS), INTR_STATUS_OFFSET, i3c_mux);//清除中断
    }
    if (int_status & I3C_MASTER_INTR_STATUS_TRANSFER_ERR_STS)
    {
        i3c_dprint("I3C_MASTER%d_INTR_STATUS_TRANSFER_ERR_STS\n", i3c_mux);//出错
        I3c_WriteREG_DWORD((I3c_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) | I3C_MASTER_INTR_STATUS_TRANSFER_ERR_STS), INTR_STATUS_OFFSET, i3c_mux);//清除中断
    }
    if (int_status & I3C_MASTER_INTR_STATUS_TRANSFER_ABORT_STS)
    {
        i3c_dprint("I3C_MASTER%d_INTR_STATUS_TRANSFER_ABORT_STS\n", i3c_mux);//出错
        I3c_WriteREG_DWORD((I3c_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux) | I3C_MASTER_INTR_STATUS_TRANSFER_ABORT_STS), INTR_STATUS_OFFSET, i3c_mux);//清除中断
    }
    if (int_status & I3C_MASTER_INTR_STATUS_RESP_READY_STS)
    {
        i3c_dprint("I3C_MASTER%d_INTR_STATUS_RESP_READY_STS", i3c_mux);
        I3C_Master_ErrorStatus(i3c_mux);//进入报错
    }
    if (int_status & I3C_MASTER_INTR_STATUS_CMD_QUEUE_READY_STS)
    {
        i3c_dprint("I3C_MASTER%d_INTR_STATUS_CMD_QUEUE_READY_STS", i3c_mux);
    }
    if (int_status & I3C_MASTER_INTR_STATUS_IBI_THLD_STS)
    {
        i3c_dprint("I3C_MASTER%d_INTR_STATUS_IBI_THLD_STS", i3c_mux);
    }
    if (int_status & I3C_MASTER_INTR_STATUS_RX_THLD_STS)
    {
        i3c_dprint("I3C_MASTER%d_INTR_STATUS_RX_THLD_STS", i3c_mux);
    }
    if (int_status & I3C_MASTER_INTR_STATUS_TX_THLD_STS)
    {
        i3c_dprint("I3C_MASTER%d_INTR_STATUS_TX_THLD_STS", i3c_mux);
    }

    return int_status & 0x1f;
}

/**
* @brief MASTER写操作函数
*
* @param data 写入值
* @param regoffset 寄存器地址偏移
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return 操作结果
*
* @note 无
*/
BYTE I3c_Master_Write(uint8_t* data, uint16_t bytelen, BYTE i3c_mux)
{
    uint8_t* data_ptr = data;
    uint32_t data_len = bytelen;
    if (data_ptr == NULL || data_len == 0)
    {
        i3c_dprint("argument error\n");
        return FALSE;
    }
    //等CMD_QUEUE_READY_STS中断状态
    for (uint32_t timeout = I3C_TIMEOUT; (I3C_Master_IntStatus(i3c_mux) & I3C_MASTER_INTR_STATUS_TX_THLD_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            i3c_dprint("fail:I3C_MASTER_INTR_STATUS=%#x\n", I3c_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux));
            i3c_error = 0;
            return FALSE;
        }
        nop;
    }
    //配置传输长度
    I3c_WriteREG_DWORD(I3C_MASTER_COMMAND_QUEUE_PORT_COMMAND_TRANSFER_ARGUMENT | I3C_MASTER_COMMAND_QUEUE_PORT_TRANSFER_ARGUMENT_DATA_LENGTH(bytelen), COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    for (uint32_t timeout = I3C_TIMEOUT; (I3C_Master_IntStatus(i3c_mux) & I3C_MASTER_INTR_STATUS_CMD_QUEUE_READY_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            i3c_dprint("fail:I3C_MASTER_INTR_STATUS=%#x\n", I3c_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux));
            i3c_error = 0;
            return FALSE;
        }
        nop;
    }//等待命令队列就绪
    I3c_WriteREG_DWORD(I3C_MASTER_COMMAND_QUEUE_PORT_COMMAND_TRANSFER_COMMAND | I3C_MASTER_COMMAND_QUEUE_PORT_TRANSFER_COMMAND_Write | I3C_MASTER_COMMAND_QUEUE_PORT_TRANSFER_COMMAND_STOP | I3C_MASTER_COMMAND_QUEUE_PORT_TRANSFER_COMMAND_SPEED(SDR0_SPEED), COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    for (uint32_t timeout = I3C_TIMEOUT; (I3C_Master_IntStatus(i3c_mux) & I3C_MASTER_INTR_STATUS_CMD_QUEUE_READY_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            i3c_dprint("fail:I3C_MASTER_INTR_STATUS=%#x\n", I3c_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux));
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
        for (uint32_t timeout = I3C_TIMEOUT; (I3C_Master_IntStatus(i3c_mux) & I3C_MASTER_INTR_STATUS_TX_THLD_STS) == 0; timeout--)
        {
            if (timeout == 0 || i3c_error)
            {
                i3c_dprint("fail:I3C_MASTER_INTR_STATUS=%#x\n", I3c_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux));
                i3c_error = 0;
                return FALSE;
            }
            vDelayXus(10);
        }   //等CMD_QUEUE_READY_STS中断
        I3c_WriteREG_DWORD(data_temp.dword, TX_DATA_PORT_OFFSET, i3c_mux);//发送数据
    }
    return TRUE;
}

/**
* @brief MASTER读操作函数
*
* @param regoffset 寄存器地址偏移
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return 操作结果
*
* @note 无
*/
uint8_t I3c_Master_Read(uint8_t* data, uint16_t bytelen, BYTE i3c_mux)
{
    if ((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1))
    {
        i3c_dprint("controller select fault\n");
        return FALSE;
    }

    uint8_t* data_ptr = data;
    uint32_t data_len = bytelen;
    if (data_ptr == NULL || data_len == 0)
    {
        i3c_dprint("argument error\n");
        return FALSE;
    }
    //配置传输长度
    I3c_WriteREG_DWORD(I3C_MASTER_COMMAND_QUEUE_PORT_COMMAND_TRANSFER_ARGUMENT | I3C_MASTER_COMMAND_QUEUE_PORT_TRANSFER_ARGUMENT_DATA_LENGTH(bytelen), COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    for (uint32_t timeout = I3C_TIMEOUT; (I3C_Master_IntStatus(i3c_mux) & I3C_MASTER_INTR_STATUS_CMD_QUEUE_READY_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            i3c_dprint("fail:I3C_MASTER_INTR_STATUS=%#x\n", I3c_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux));
            i3c_error = 0;
            return FALSE;
        }
        nop;
    } // 等待CMD_COMPLETE中断
    I3c_WriteREG_DWORD(I3C_MASTER_COMMAND_QUEUE_PORT_COMMAND_TRANSFER_COMMAND | I3C_MASTER_COMMAND_QUEUE_PORT_TRANSFER_COMMAND_Read | I3C_MASTER_COMMAND_QUEUE_PORT_TRANSFER_COMMAND_STOP | I3C_MASTER_COMMAND_QUEUE_PORT_TRANSFER_COMMAND_SPEED(SDR0_SPEED), COMMAND_QUEUE_PORT_OFFSET, i3c_mux);
    for (uint32_t timeout = I3C_TIMEOUT; (I3C_Master_IntStatus(i3c_mux) & I3C_MASTER_INTR_STATUS_CMD_QUEUE_READY_STS) == 0; timeout--)
    {
        if (timeout == 0 || i3c_error)
        {
            i3c_dprint("fail:I3C_MASTER_INTR_STATUS=%#x\n", I3c_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux));
            i3c_error = 0;
            return FALSE;
        }
        nop;
    } // 等待CMD_COMPLETE中断
    while (data_len)
    {
        register uint32_t len = (data_len <= 4 ? data_len : 4);
        data_len -= len;
        for (uint32_t timeout = I3C_TIMEOUT; (I3C_Master_IntStatus(i3c_mux) & I3C_MASTER_INTR_STATUS_RX_THLD_STS) == 0; timeout--)
        {
            if (timeout == 0 || i3c_error)
            {
                i3c_dprint("fail:I3C_MASTER_INTR_STATUS=%#x\n", I3c_ReadREG_DWORD(INTR_STATUS_OFFSET, i3c_mux));
                i3c_error = 0;
                return FALSE;
            }
            vDelayXus(10);
        }
        uDword data_temp;
        data_temp.dword = I3c_ReadREG_DWORD(RX_DATA_PORT_OFFSET, i3c_mux);
        for (register uint32_t i = 0; i < len; i++)
        {
            *data_ptr++ = data_temp.byte[i];
        }

    }//读取数据

    return TRUE;
}

/**
* @brief MASTER读写流操作函数
*
* @param i3c_addr 设备地址
* @param Var 写入数据指针
* @param Count 写入数据个数
* @param Var2 读出数据指针
* @param Count2 读数据个数
* @param i3c_mux 配置选择，如I3C_MASTER0、I3C_MASTER1
* @return 操作结果
*
* @note 无
*/
BYTE I3C_Master_W2R_Stream(BYTE i3c_addr, BYTE* w_var, WORD w_cnt, BYTE* r_var, WORD r_cnt, BYTE i3c_mux)
{
    if ((i3c_mux != I3C_MASTER0) && (i3c_mux != I3C_MASTER1))
    {
        i3c_dprint("controller select fault\n");
        return FALSE;
    }
    if (I3C_Master_Init(i3c_addr, SDR_DEFAULT_SPEED, i3c_mux) == FALSE)
    {
        return FALSE;
    }
    if (w_cnt != 0)
    {
        if (I3c_Master_Write(w_var, w_cnt, i3c_mux) == FALSE)
        {
            return FALSE;
        }
    }
    if (r_cnt != 0)
    {
        if (I3c_Master_Read(r_var, r_cnt, i3c_mux) == FALSE)
        {
            return FALSE;
        }
    }
    return TRUE;
}

/**
* @brief SLAVE初始化函数
*
* @param addr 设备地址
* @param i3c_mux 配置选择，如I3C_SLAVE0、I3C_SLAVE1
* @return 无
*
* @note 无
*/
void I3c_Slave_Init(BYTE addr, BYTE i3c_mux)
{
    if ((i3c_mux != I3C_SLAVE0) && (i3c_mux != I3C_SLAVE1))
    {
        i3c_dprint("controller select fault\n");
        return;
    }
    //配置静态地址
    DWORD rdata;
    rdata = I3c_ReadREG_DWORD(CONFIG_OFFSET, i3c_mux);
    rdata |= (addr << 25);
    I3c_WriteREG_DWORD(rdata, CONFIG_OFFSET, i3c_mux);
    I3c_WriteREG_DWORD(((I3c_ReadREG_DWORD(TCCLOCK_OFFSET, i3c_mux) & 0x000000FF) | ((((uint32_t)(HIGHT_CHIP_CLOCK * 2 / 1000000)) << 8) & 0x0000FF00)), TCCLOCK_OFFSET, i3c_mux);   //设置TCCLOCK
    I3c_WriteREG_DWORD(0x0, INTSET_OFFSET, i3c_mux);   //中断全部失能
    I3c_WriteREG_DWORD(0x0, DMACTRL_OFFSET, i3c_mux);   //DMA失能
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
    I3c_WriteREG_DWORD((I3c_ReadREG_DWORD(INTSET_OFFSET, i3c_mux) | tpye), INTSET_OFFSET, i3c_mux);
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
    I3c_WriteREG_DWORD((I3c_ReadREG_DWORD(INTSET_OFFSET, i3c_mux) & (~tpye)), INTSET_OFFSET, i3c_mux);
    return TRUE;
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
//     I3C_Master_Init(0x5a, I3C_MASTER1);

//     //初始话slave
//     I3c_Slave_Init(0x5a, I3C_SLAVE1);
//     I3C_SLAVE_INT_ENABLE(RXPEND_ENABLE, I3C_SLAVE1);  //使能接收FIFO满中断

//     for (int i = 0; i < 0xf; i++)
//     {
//         BYTE tx_data = 0x1+i;

//         I3c_Master_write(tx_data, I3C_MASTER1);

//         vDelayXus(20);

//         // BYTE rx_data = I3c_Master_Read(I3C_MASTER1);
//         I3c_Master_Read(I3C_MASTER1);
//     }

// }

