#include "AE_INCLUDE.H"
#include "KERNEL_MEMORY.H"
#include "TEST_RELIABILITY.H"

uint8_t MCU_ID = 0;
uint8_t Reliability_Rx_Buffer[RELIABILITY_BUFF_SIZE] = { 0 };
uint8_t Reliability_Tx_Buffer[RELIABILITY_BUFF_SIZE] = { 0 };
uint8_t Reliability_Complete_Flag = 0;
uint16_t Reliability_Rx_Cnt = 0;
uint8_t Reliability_Expected_Len = 0;
void Get_Flash_MCU_ID(void){
    //Transfer_Mailbox_Read_Mcu_Id
    //由于flash最后一页地址已经存了东西，这里不确定
}

uint16_t CRC16_Generate(uint8_t *pData, uint16_t Length)
{
    uint16_t C_Reg = 0xFFFF;
    uint16_t C_Code = 0xA001;
    uint16_t TakingH8 = 0xFF00;
    uint8_t TakingL8 = 0xFF;
    uint8_t C_DataL, C_Data_t;
    int k1, k2;

    for (k1 = 0; k1 < Length; k1++)
    {
        C_DataL = C_Reg & TakingL8; // 取低8位
        C_Reg = (C_DataL ^ pData[k1]) + (C_Reg & TakingH8);
        
        for (k2 = 0; k2 < 8; k2++)
        {
            C_Data_t = C_Reg & 1;
            C_Reg >>= 1;
            if (C_Data_t == 1)
            {
                C_Reg = C_Reg ^ C_Code;
            }
        }
    }
    
    return C_Reg;
}

void SendResonse(uint8_t command,uint16_t length,uint8_t *buf)
{
    int i;
    uint16_t crc_value = 0;
    uint16_t offset = 4;
    int tx_counter = 0;
    *((uint16_t *)&Reliability_Tx_Buffer[0]) = length + 4; // 总长度
    Reliability_Tx_Buffer[2] = MCU_ID; // MCU ID
    Reliability_Tx_Buffer[3] = command; // 命令
    for (i = 0; i < length; i++)
    {
        Reliability_Tx_Buffer[offset++] = buf[i];
    }
    crc_value = CRC16_Generate(Reliability_Tx_Buffer, length + 4);
    Reliability_Tx_Buffer[offset++] = (uint8_t)(crc_value & 0x00FF);       // CRC低8位
    Reliability_Tx_Buffer[offset++] = (uint8_t)((crc_value >> 8) & 0x00FF); // CRC高8位
    sysctl_iomux_config(GPIOA, 9, 2);                                       // tx
    while(tx_counter < (length + 6)){
        while (!(UARTA_LSR & UART_LSR_TEMP))
            ; /*当此位为空发送fifo写入数据*/
        UARTA_TX = Reliability_Tx_Buffer[tx_counter];
        tx_counter++;
    }
    while (!(UARTA_LSR & UART_LSR_TEMP));
    sysctl_iomux_config(GPIOA, 9, 0);
}


//c测试发送是否正常使用
void Reply_Voltage_Query(void)
{
    uint8_t response_data[4];
    uint16_t voltage = 3300; // 3.3V

    // 组装 Data 段
    response_data[0] = 0x3C;                      // ACK: 表示成功
    response_data[1] = (uint8_t)(voltage & 0xFF); // 电压低字节 (E8)
    response_data[2] = (uint8_t)(voltage >> 8);   // 电压高字节 (0C)
    response_data[3] = 0x01;                      // 状态位: 正常

    // 调用你的函数
    // command = 0x20, length = 4 (ACK + 3字节数据)
    SendResonse(0x20, 4, response_data);
}