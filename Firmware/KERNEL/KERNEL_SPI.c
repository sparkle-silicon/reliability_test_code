/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-03-27 21:35:13
 * @Description: This file is used for SPI Interface
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
#include "KERNEL_SPI.H"
#include "KERNEL_TIMER.H"
/*------------------------------SPIM Interface Code-----------------------------*/
BYTE SPI_Write_Buff[256] = { 0 };
struct SPI_INS
{
  uint8_t flash_id_cmd[4];
  uint8_t flash_busy_cmd[2];
  uint8_t flash_suspend_cmd[2];
  uint8_t flash_write_en_cmd;
  uint8_t flash_sector_erase_cmd[4];
  uint8_t flash_chip_erase_cmd;
  uint8_t flash_read_data_cmd[5];
  uint8_t flash_page_program_cmd[5];
  uint8_t flash_reset_cmd;
} spi_instruction = {
    {0x9f, 0xff, 0xff, 0xff},
    {0x5, 0xff},
    {0x35, 0xff},
    0x6,
    {0x20, 0x0, 0x0, 0x0},
    0xc7,
    {0x3, 0x0, 0x0, 0x0, 0xff},
    {0x2, 0x0, 0x0, 0x0, 0xaf},
    0x99 };
/**
 * @brief 设置SPI中断屏蔽和开关
 *
 * @param irq_mask 中断屏蔽掩码，可选值为：txeim, txoim, rxuim, rxoim, rxfim
 * @param sw 开关控制，可选值为：ENABLE, DISABLE
 *
 * @return 无
 *
 * @note 此函数用于设置SPI中断屏蔽和开关，可选择的中断屏蔽掩码包括：TXEIM - 发送缓冲区为空中断屏蔽，
 *       TXOIM - 发送上溢中断屏蔽，RXUIM - 接收缓冲区下溢中断屏蔽，RXOIM - 接收溢出中断屏蔽，
 *       RXFIM - 接收缓冲区满中断屏蔽。开关控制可选值包括：ENABLE - 使能中断，DISABLE - 禁止中断。
 */
void SPI_IRQ_Config(uint8_t irq_mask, uint8_t sw)
{
  if(sw == 0)
  {
    SPIM_IMSR |= irq_mask;
  }
  else
  {
    SPIM_IMSR &= (~irq_mask);
  }
}
/**
 * @brief SPI初始化
 *
 * @return 无
 *
 * @note 此函数用于SPI初始化，
 *          * dly：SPI帧延时dly个周期
 *          * cpol：极性（空闲时为HIGH or LOW）
 *          * cpha：相位（cpha&0x1+1个跳变沿），
 *          * lsb：1为LSB，0为MSB
 *          * dssp：数据宽度
 *          * cpsr：分频数等于（cpsr+1）*2
 */
void SPI_Init(BYTE dly, BYTE cpol, BYTE cpha, BYTE lsb, BYTE dssp, BYTE cpsr)
{
#if !(SPIM_CLOCK_EN)
  return;
#endif
  uint16_t reg_value = 0;
  BYTE dma_rd_en, dma_wr_en;
/*
dma_rd_en:接收FIFO的DMA传输使能位1：enable;0:disable
dma_wr_en：发送FIFO的DMA传输使能位1：enable;0:disable
*/
  dma_rd_en = 0x0; //接收DMA使能
  dma_wr_en = 0x0; //测试DMA功能需要配置
  reg_value |= (cpha & 0x1) << 0;      // bit0: CPHA
  reg_value |= (cpol & 0x1) << 1;      // bit1: CPOL
  reg_value |= 0x1 << 2;       // bit2: SPE
  reg_value |= (dly & 0x07) << 3;   // bit[5:3]: DLY_SEL
  reg_value |= (dma_rd_en & 0x1) << 6; // bit6: DMA_RD_EN
  reg_value |= (dma_wr_en & 0x1) << 7; // bit7: DMA_WR_EN
  reg_value |= (dssp & 0xF) << 8;      // bit[11:8]: DSSP
  reg_value |= (lsb & 0x1) << 12;      // bit12: LSP
  SPIM_CTRL = reg_value;
  // printf("SPI_CTRL = 0x%x\n", SPIM_CTRL);
  // SPIM_CTRL = 0x6754;
  SPIM_CPSR = cpsr;                                                // 时钟分频
  // SPIM_TXFTLR = 0x10;                                              // 发送fifo为1
  // SPIM_RXFTLR = 0x10;                                              // 接收fifo为1
  // SPI_IRQ_Config(txeim | txoim | rxuim | rxoim | rxfim, DISABLE); // 屏蔽所有中断
  SPIM_IMSR = 0x1f;
}
/**
 * 用于四线模式
 * @brief 发送一字节数据
 *
 * @param send_data 要发送的数据
 *
 * @return 接收到的数据
 *
 * @note 此函数用于向SPI总线发送一字节数据并接收一字节数据，仅适用全双工的四线模式
 */
uint8_t SPI_Send_Byte(uint8_t send_data)
{
  SPI_Timeout = 100000;
  while(!((SPIM_SR)&SPI_TFE)) // 等待发送fifo空
  {
    SPI_Timeout--;
    if(SPI_Timeout == 0)
    {
      dprint("spi等待发送超时\n");
      return 0;
    }
  }
  SPIM_DA = send_data; // 发送数据
  SPI_Timeout = 1000000;
  while(!((SPIM_SR)&SPI_RFNE)) // 等待接收fifo不空
  {
    SPI_Timeout--;
    if(SPI_Timeout == 0)
    {
      dprint("spi等待接收超时\n");
      return 0;
    }
  }
  return (SPIM_DA);
}
/**
 * @brief 读取一字节数据
 *
 * @return 接收到的数据
 *
 * @note 此函数用于从SPI总线读取一字节数据。
 */
uint8_t SPI_Read_Byte(void)
{
  return SPI_Send_Byte(0xff);
}
/**
 * @brief 拉低片选
 *
 * @param cs_select 片选选择，可选值为0或1
 *
 * @return 无
 *
 * @note 此函数用于拉低SPI Flash的片选信号，使其处于选中状态。
 */
void SPI_Flash_CS_Low(uint8_t cs_select)
{
  if(cs_select == 0)
  {
    SPIM_CSN0 = 0x0;
    if(SPIM_CSN0 != 0x0)
      dprint("CS0 select failed!\n");
    else if(SPIM_CSN0 == 0x0)
    {
    }
           // dprint("CS0 select success!\n");
  }
  else if(cs_select == 1)
  {
    SPIM_CSN1 = 0x0;
    if(SPIM_CSN1 != 0x0)
      dprint("CS1 select failed!\n");
    else if(SPIM_CSN1 == 0x0)
    {
    }
           // dprint("CS1 select success!\n");
  }
}
/**
 * @brief 拉高片选信号
 *
 * @param cs_select 片选信号选择，可选值为0或1
 *
 * @return 无
 *
 * @note 此函数用于拉高SPI Flash的片选信号，使其处于未选中状态。
 */
void SPI_Flash_CS_High(uint8_t cs_select)
{
  if(cs_select == 0)
  {
    SPIM_CSN0 = 0x1;
    if(SPIM_CSN0 != 0x1)
      dprint("CS0 free failed!\n");
    else if(SPIM_CSN0 == 0x1)
    {
    }
       // dprint("CS0 free success!\n");
  }
  else if(cs_select == 1)
  {
    SPIM_CSN1 = 0x1;
    if(SPIM_CSN1 != 0x1)
      dprint("CS1 free failed!\n");
    else if(SPIM_CSN1 == 0x1)
    {
    }
       // dprint("CS1 free success!\n");
  }
}
/**
 * @brief 读取SPI Flash0的ID信息
 *
 * @param 无
 *
 * @return 无
 *
 * @note 此函数用于读取SPI Flash0的ID信息，以便进行芯片型号和厂商的识别。
 */
void SPI_Read_ID0(void)
{
  SPI_Flash_CS_Low(0);
  vDelayXms(5);
  SPI_Send_Byte(CMD_READ_ID); // 发送读取flash id 命令(24位id)
  SPI_FLAHS_ID |= SPI_Send_Byte(0xff);
  SPI_FLAHS_ID <<= 8;
  SPI_FLAHS_ID |= SPI_Send_Byte(0xff);
  SPI_FLAHS_ID <<= 8;
  SPI_FLAHS_ID |= SPI_Send_Byte(0xff);
  vDelayXms(5);
  SPI_Flash_CS_High(0);
  dprint("flash id:%#x\n", SPI_FLAHS_ID);
}
// 读取id1
void SPI_Read_ID1(void)
{
  SPI_Flash_CS_Low(1);
  vDelayXms(5);
  SPI_Send_Byte(CMD_READ_ID); // 发送读取flash id 命令(24位id)
  SPI_FLAHS_ID |= SPI_Send_Byte(0xff);
  SPI_FLAHS_ID <<= 8;
  SPI_FLAHS_ID |= SPI_Send_Byte(0xff);
  SPI_FLAHS_ID <<= 8;
  SPI_FLAHS_ID |= SPI_Send_Byte(0xff);
  vDelayXms(5);
  SPI_Flash_CS_High(1);
  dprint("flash id:%#x\n", SPI_FLAHS_ID);
}
// 发送命令函数
uint8_t SPI_Send_Cmd(uint8_t cmd, uint8_t cs_select)
{
  SPI_WriteDATA = 0;
  SPI_Flash_CS_Low(cs_select);
  SPI_WriteDATA = SPI_Send_Byte(cmd);
  SPI_WriteDATA = SPI_Send_Byte(0xFF);
  SPI_Flash_CS_High(cs_select);
  return SPI_WriteDATA;
}
/**
 * @brief 判断flash是否忙碌
 *
 * @param cs_select SPI控制器选择，可选值为0或1
 *
 * @return 返回SPI控制器的状态，0表示空闲，1表示忙碌
 *
 * @note 此函数用于判断指定SPI控制器选中的flash是否忙碌,适用全双工的四线
 */
uint8_t SPI_Busy(uint8_t cs_select)
{
  FLASH_SR = 0;
  FLASH_SR = SPI_Send_Cmd(CMD_READ_STATUS1, cs_select);
  return (FLASH_SR & 0x1);
}
/**
 * @brief 等待flash空闲
 *
 * @param cs_select SPI控制器选择，可选值为0或1
 *
 * @return 无
 *
 */
void SPI_WaitnoBUSY(uint8_t cs_select)
{
  FLASH_SR = 0;
  SPI_Flash_CS_Low(cs_select);
  SPI_Send_Byte(CMD_READ_STATUS1);
  do
  {
    FLASH_SR = SPI_Send_Byte(0xff);
  }
  while((FLASH_SR & 0x01) == 1);
  SPI_Flash_CS_High(cs_select);
}
/**
 * @brief 判断SPI Flash是否处于挂起状态
 *
 * @param cs_select SPI控制器选择，可选值为0或1
 *
 * @return 返回值为0表示Flash未挂起，返回1表示Flash已挂起
 *
 * @note 此函数用于判断指定SPI控制器上的Flash是否处于挂起状态。当Flash在进行扇区擦除或
 *       编程操作时，若另一次操作的写入或读取命令到达，Flash将暂停当前操作并执行新命令，
 *       此时Flash处于挂起状态。通过本函数，可以判断Flash是否处于挂起状态，以便进行后续操作。
 */
uint8_t SPI_Suspend(uint8_t cs_select)
{
  FLASH_SR = 0;
  FLASH_SR = SPI_Send_Cmd(CMD_READ_STATUS2, cs_select);
  dprint("spi flash status2 is %#x\n", FLASH_SR);
  return (FLASH_SR & 0x8);
}
/**
 * @brief SPI写使能
 *
 * @param cs_select 选择哪个SPI控制器，可选值为0或1
 *
 * @return 无
 *
 * @note
 *  此函数用于SPI写使能，即向SPI Flash发送写使能命令。
 */
void SPI_Write_Enable(uint8_t cs_select)
{
  SPI_Flash_CS_Low(cs_select);
  SPI_Send_Byte(CMD_WRITE_ENABLE);
  SPI_Flash_CS_High(cs_select);
}
/**
 * @brief 读取flash的某个地址的内容
 *
 * @param pbuffer 存储读取到的内容的地址
 * @param read_addr 读取的起始地址
 * @param num_byte_to_read 读取的字节数
 * @param cs_select 选择的SPI控制器
 *
 * @return 无
 *
 * @note 读取到的内容存储到pbuffer地址，读取num_byte_to_read个字节
 */
void SPI_Send_Cmd_Addr(uint8_t *pbuffer, uint32_t read_addr, uint16_t num_byte_to_read, uint8_t cs_select)
{
  dprint("spi read flash start!\n");
  SPI_Flash_CS_Low(cs_select);
  SPI_Send_Byte(CMD_READ_DATA); // 读命令
  SPI_Send_Byte((read_addr & 0xFF0000) >> 16);
  SPI_Send_Byte((read_addr & 0xFF00) >> 8);
  SPI_Send_Byte(read_addr & 0xFF);
  while(num_byte_to_read--)
  {
    *pbuffer = SPI_Read_Byte();
    pbuffer++;
  }
  SPI_Flash_CS_High(cs_select);
  // while(SPI_Busy(0));
  dprint("read flash data completed!\n");
}
/**
 * @brief  擦除SPI Flash所选扇区（大小为4KB）
 * @param  address: 需要擦除的扇区地址
 * @param  cs_select: 选择SPI控制器
 * @retval 无
 */
void SPI_Block_Erase(uint32_t address, uint8_t cs_select)
{
  dprint("block erase start!\n");
  SPI_Write_Enable(cs_select); // 擦除扇区之前要执行写启用指令,位置必须在CS为0之前
  SPI_Flash_CS_Low(cs_select);
  SPI_Send_Byte(CMD_BLOCK_ERASE_64);
  SPI_Send_Byte((address & 0xFF0000) >> 16);
  SPI_Send_Byte((address & 0xFF00) >> 8);
  SPI_Send_Byte(address & 0xFF);
  SPI_Flash_CS_High(cs_select);
  SPI_WaitnoBUSY(cs_select);
  dprint("block erase completed!\n");
}
void SPI_Chip_Erase(uint8_t cs_select)
{
  dprint("chip erase start!\n");
  // Write Enable must be executed first
  SPI_Write_Enable(0);
  SPI_Flash_CS_Low(cs_select);
  // send chip erase cmd
  SPI_Send_Byte(CMD_CHIP_ERASE);
  SPI_Flash_CS_High(cs_select);
  while(SPI_Busy(cs_select));
  dprint("chip erase completed!\n");
}
/**
 * @brief 页写入函数
 * @param  write_addr: 写入的地址
 *         byte_num: 写入多少字节
 *         byte_data: 写入的数据指针
 *         cs_select: 选择哪个片选
 * @return None
 */
void SPI_Page_Program(uint32_t write_addr, uint32_t byte_num, uint8_t *byte_data, uint8_t cs_select)
{
  dprint("flash page program start!\n");
  // Write Enable must be executed first
  SPI_Write_Enable(cs_select);
  SPI_Flash_CS_Low(cs_select);
  // send page program cmd
  SPI_Send_Byte(CMD_PAGE_PROGRAM); // Page Program 命令
  // send page program start address
  SPI_Send_Byte((write_addr & 0xFF0000) >> 16);
  SPI_Send_Byte((write_addr & 0xFF00) >> 8);
  SPI_Send_Byte(write_addr & 0xFF);
  // send data
  while(byte_num--)
  {
    SPI_Send_Byte(*byte_data);
    byte_data++;
  }
  SPI_Flash_CS_High(cs_select);
  while(SPI_Busy(cs_select));
  dprint("chip page program completed!\n");
}
/**
 * @brief 对SPI Flash进行复位
 *
 * @param cs_select 选择哪个SPI控制器
 *
 * @return None
 */
void SPI_Flash_Reset(uint8_t cs_select)
{
  dprint("spi flash reset start!\n");
  while(SPI_Busy(cs_select) | SPI_Suspend(cs_select));
  SPI_Send_Cmd(0x66, cs_select);
  SPI_Send_Cmd(0x99, cs_select);
  dprint("spi flash reset finish!\n");
}
/**
 * @brief 发送读取数据命令
 *
 * @param cmd 发送的读还是写命令
 * @param read_addr 读取的起始地址
 * @param cs_select 选择的SPI控制器
 *
 * @return 无
 *
 * @note 这个函数配合中断使用，比如发送读命令，然后再使能SPI_IRQ_Config(rxfim,ENABLE) 接收满中断
 * 再发送SPIM_DA0=0xff这条命令进入中断中读取数据
 */
void SPI_Send_RWcmd(uint8_t cmd, uint32_t read_addr, uint8_t cs_select)
{
  SPI_Flash_CS_Low(cs_select);
  SPI_Send_Byte(cmd);
  SPI_Send_Byte((read_addr & 0xFF0000) >> 16);
  SPI_Send_Byte((read_addr & 0xFF00) >> 8);
  SPI_Send_Byte(read_addr & 0xFF);
  SPI_IRQ_Config(rxfim, ENABLE);
  dprint("read flash data completed!\n");
}
// 进行读写擦除测试
void SPI_Flash_Test(void)
{
  int i;
  uint8_t read_buff[256] = { 0 };
  uint8_t write_buff[256] = { 0x1, 0x2, 0x3, 0x4, 0x5, 0x6 };
  /**************CSN0***************/
  SPI_Block_Erase(0x0, 0);
  SPI_Send_Cmd_Addr(read_buff, 0, 256, 0);
  for(i = 0; i < 256; i++)
  {
    dprint("read data buff is %#x\n", read_buff[i]);
  }
  SPI_Page_Program(0, 256, write_buff, 0);
  SPI_Send_Cmd_Addr(read_buff, 0, 256, 0);
  for(i = 0; i < 256; i++)
  {
    dprint("read data buff is %#x\n", read_buff[i]);
  }
  /**************CSN1***************/
  /*SPI_Block_Erase(0x0, 1);
  SPI_Send_Cmd_Addr(read_buff, 0, 256, 1);
  for(i=0;i<256;i++)
  {
      dprint("read data buff is %#x\n", read_buff[i]);
  }
  SPI_Page_Program(0, 256, write_buff, 1);
  SPI_Send_Cmd_Addr(read_buff, 0, 256, 1);
  for(i=0;i<256;i++)
  {
      dprint("read data buff is %#x\n", read_buff[i]);
  }*/
}


/*
*
*注：上述代码适配全双工(四线模式)不支持半双工，后续代码则是支持半双工（dual_mode,quad_mode,3线模式）
*
*/

//等待非四线模式下SPI空闲
void spibusy(void)
{
  while((SPIM_SR & 0x10) == 0x10);//wait busy
}
//单byte发送函数
static uint8_t spibyte(uint8_t byte)
{
  while((SPIM_SR & 0x1) == 0x0);//wait tx_fifo not full
  REG16(0x6012) = byte;
  while((SPIM_SR & 0x4) == 0x0);//wait rx_fifo not empty
  return(REG8(0x6012));
}

//DMA接收函数
// static void spibyte_exp(uint8_t byte);
// static void spibyte_exp(uint8_t byte)
// {
//     // 等待 TX FIFO 非满
//   while((SPIM_SR & 0x1) == 0x0);//wait tx_fifo not full

//   // 写入 SPI 数据寄存器
//   REG16(0x6012) = byte;

//   // 等待 RX FIFO 非空
//   while((SPIM_SR & 0x4) == 0x0);//wait rx_fifo not empty

//   // 不读取 SPI 数据，而是启动 DMA 传输
//   DMA_ChEnReg = (0x1 | 0x1 << 8);
// }


//双byte发送函数
void spi2byte_w(uint16_t data)
{
  while((SPIM_SR & 0x1) == 0x0);//wait tx_fifo not full
  REG16(0x6012) = data;
}

//发送一个字节的命令函数
static uint8_t sendcmd1b(uint8_t cmd, int csn);
static uint8_t sendcmd1b(uint8_t cmd, int csn)
{
  uint8_t data;
  SPI_Flash_CS_Low(csn);
  spibyte(cmd);
  data = spibyte(0xff);
  SPI_Flash_CS_High(csn);
  return(data);
}

//除3线模式以外的状态判断
static uint8_t spiwip(int csn);
static uint8_t spiwip(int csn)
{
  uint8_t sts;
  // 发送命令0x05，检查SPI接口是否空闲
  SPIM_MODE = 0x0; // 切回常规四线模式，识别命令
  // SPIM_CTRL  = 0x6754;
  sts = sendcmd1b(0x05, csn);
  return(sts & 0x1);
}

//擦除扇区
void sectorerase(int addr, int csn)
{
  uint8_t ad[3];
  ad[0] = (addr >> 16) & 0xff;
  ad[1] = (addr >> 8) & 0xff;
  ad[2] = (addr) & 0xff;
  // SPIM_CTRL  = 0x6754;
  SPI_Flash_CS_Low(csn);
  spibyte(0x06);
  SPI_Flash_CS_High(csn);
  SPI_Flash_CS_Low(csn);
  printf("flash csn%x sectorerase\n", csn);
  spibyte(0x20);
  spibyte(ad[0]);
  spibyte(ad[1]);
  spibyte(ad[2]);
  SPI_Flash_CS_High(csn);
  printf("wait spi wip\n");
  while(spiwip(csn));
}

//quad enable set
void spiqe(int csn)
{
  // uint8_t sts;
  SPI_Flash_CS_Low(csn);
  spibyte(0x6);//写使能
  SPI_Flash_CS_High(csn);
  SPI_Flash_CS_Low(csn);
  spibyte(0x01);
  spibyte(0x00);
  spibyte(0x02);
  SPI_Flash_CS_High(csn);
  // printf("set qe,flash csn%x spiwip:sts: 0x%x\n",csn,sts);
  printf("wait spi wip\n");
  while(spiwip(csn));
}


//三线模式状态判断函数
static uint8_t spi3wirewip(int csn);
static uint8_t spi3wirewip(int csn)
{
  uint8_t sts;
  SPI_Flash_CS_Low(csn);
  spi2byte_w(CMD_READ_STATUS1);
  spibusy();
  printf("====wait busy over=====\n");
  SPIM_RDNUM = 1;
  while((SPIM_SR & SPI_RFNE) == 0x0);//wait rx_fifo not empty
  sts = REG16(0x6012);
  SPI_Flash_CS_High(csn);
  return(sts & 0x1);
}

//三线写函数
void spi3wireprogram(int addr, uint16_t *data, int num, int csn)
{
  uint8_t ad[3];
  ad[0] = ((addr & 0xff0000) >> 16);
  ad[1] = ((addr & 0xff00) >> 8);
  ad[2] = (addr) & 0xff;
  printf("flash csn%x write enable\n", csn);
  SPIM_CTRL = 0x6704;//dssp 8 bit
  SPIM_MODE = 0x3; // 3线模式
  SPI_Flash_CS_Low(0);
  spi2byte_w(CMD_WRITE_ENABLE);
  spibusy();
  SPI_Flash_CS_High(0);
  printf("flash csn%x 3wire program\n", csn);
  SPI_Flash_CS_Low(0);
  spi2byte_w(CMD_PAGE_PROGRAM);
  spi2byte_w(ad[0]);
  spi2byte_w(ad[1]);
  spi2byte_w(ad[2]);
  while(num--)
    spi2byte_w(*data++);
  spibusy();
  SPI_Flash_CS_High(0);
  printf("wait spi wip\n");
  while(spi3wirewip(csn));
}


//SPI三线模式读函数
void spi3wireread(int addr, uint16_t *data, int num, int csn)
{
  uint8_t ad[3];
  ad[0] = ((addr & 0xff0000) >> 16);
  ad[1] = ((addr & 0xff00) >> 8);
  ad[2] = (addr) & 0xff;
  SPIM_CTRL = 0x6704;
  SPIM_MODE = 0x3; // 3线模式
  SPI_Flash_CS_Low(csn);
  spi2byte_w(CMD_READ_DATA);
  spi2byte_w(ad[0]);
  spi2byte_w(ad[1]);
  spi2byte_w(ad[2]);
  spibusy();
  SPIM_RDNUM = num;
  for(; num > 0; num--)
  {
    while((SPIM_SR & 0x4) == 0x0);//wait rx_fifo not empty
    *data++ = REG16(0x6012);
  }
  SPI_Flash_CS_High(csn);
}

//3线模式读写测试函数
/*
  csn:片选号
  no:测试数据长度
*/
void spim_rw_3wire(int csn, int no)
{
  uint16_t s_data[256];
  uint16_t d_data[256];
  for(int i = 0; i < 256; i++)
  {
    s_data[i] = i;
  }
  uint16_t rdata, rdata2;
  sectorerase(0x4000, csn);
  spi3wireprogram(0x4000, s_data, no, csn);
  spi3wireread(0x4000, d_data, no, csn);

  // check
  for(int i = 0; i < no; i++)
  {
    rdata = s_data[i];
    rdata2 = d_data[i];
  }
  if(rdata != rdata2)
  {
    printf("error!!\n");
  }
  else
  {
    printf("PASS!! :quad :spim csn%x, 2BYTE write/read OK\n", csn);
  }
  for(int i = 0; i < no; i++)
  {
    printf("send is 0x%x\n", s_data[i]);
    printf("read is 0x%x\n", d_data[i]);
  }
}

/*
  dual模式读写测试函数
  csn:片选号
  no:测试数据长度
*/
void spim_rw_dual(int csn, int no)
{
  uint16_t s_data[256];
  uint16_t d_data[256];
  uint16_t rdata, rdata2;
  int i;
  for(i = 0; i < no; i++)
    s_data[i] = i;
  sectorerase(0x7000, csn);
  spiquadprogram(0x7000, s_data, no, csn);
  spidualread(0x7000, d_data, no, csn);
  // check
  for(int i = 0; i < no; i++)
  {
    rdata = s_data[i];
    rdata2 = d_data[i];
  }
  if(rdata != rdata2)
  {
    printf("error dual!!\n");
  }
  else
  {
    printf("PASS dual!!\n");
  }
  for(int i = 0; i < no; i++)
  {
    printf("send is 0x%x\n", s_data[i]);
    printf("read is 0x%x\n", d_data[i]);
  }
}

//dual模式读测试函数
void spidualread(int addr, uint16_t *data, int num, int csn)
{
  uint16_t ad[3];
  ad[0] = (addr >> 16) & 0xff;
  ad[1] = (addr >> 8) & 0xff;
  ad[2] = (addr) & 0xff;
  printf("flash csn%x dual read\n", csn);
  SPI_Flash_CS_Low(csn);
  spibyte(0x3b); //地址任意位置开始，自动递增，地址单线输出，数据两位输出
  spibyte(ad[0]);
  spibyte(ad[1]);
  spibyte(ad[2]);
  spibyte(0);//dummy 
  SPIM_MODE = 0x1; //dual mode
  SPIM_CTRL = 0x6f04;//dssp 16 bit
  SPIM_RDNUM = num;
  for(; num > 0; num--)
  {
    while((SPIM_SR & 0x4) == 0x0);//wait rx_fifo not empty
    *data++ = REG16(0x6012);
  }
  SPI_Flash_CS_High(csn);
}


/*
quad模式的写测试函数
*/

void spiquadprogram(int addr, uint16_t *data, int num, int csn)
{
  uint8_t ad[3];
  ad[0] = ((addr & 0xff0000) >> 16);
  ad[1] = ((addr & 0xff00) >> 8);
  ad[2] = (addr) & 0xff;
  SPI_Flash_CS_Low(csn);
  spibyte(0x6);//写使能
  SPI_Flash_CS_High(csn);
  printf("flash csn%x quad program\n", csn);
  SPI_Flash_CS_Low(csn);
  spibyte(0x32);
  spibyte(ad[0]);
  spibyte(ad[1]);
  spibyte(ad[2]);
  SPIM_MODE = 0x2; // quad模式
  SPIM_CTRL = 0x6f04;//dssp 16 bit
  while(num--)
    spi2byte_w(*data++);
  printf("wait busy\n");
  spibusy();
  SPI_Flash_CS_High(csn);
  // SPIM_CTRL = 0x6704;
  SPIM_CTRL = 0x6754;
  while(spiwip(csn));
}

//quad模式读数据
void spiquadread(int addr, uint16_t *data, int num, int csn)
{
  uint16_t ad[3];
  ad[0] = (addr >> 16) & 0xff;
  ad[1] = (addr >> 8) & 0xff;
  ad[2] = (addr) & 0xff;
  printf("flash csn%x quad read\n", csn);
  SPI_Flash_CS_Low(csn);
  spibyte(0x6b);
  spibyte(ad[0]);
  spibyte(ad[1]);
  spibyte(ad[2]);

  spibyte(0);//dummy

  SPIM_MODE = 0x2; //quad mode
  printf("SPIM_MODE :%x\n", SPIM_MODE);
  SPIM_CTRL = 0x6f04;//dssp 16 bit
  SPIM_RDNUM = num;
  for(; num > 0; num--)
  {
    while((SPIM_SR & 0x4) == 0x0);//wait rx_fifo not empty
    *data++ = REG16(0x6012);
  }
  SPI_Flash_CS_High(csn);
}


//quad模式测试函数(调用这个)
/*
  csn:片选号
  no:测试数据长度

*/

void spim_rw_quad(int csn, int no)
{
  uint16_t s_data[256];
  uint16_t d_data[256];
  uint16_t rdata, rdata2;
  int i;
  for(i = 0; i < no; i++)
    s_data[i] = i;
  spiqe(csn);//设置quad模式,quad enable
  sectorerase(0x8000, csn);
  spiquadprogram(0x8000, s_data, no, csn);
  spiquadread(0x8000, d_data, no, csn);

  //check
  for(i = 0; i < no; i++)
  {
    rdata = s_data[i];
    rdata2 = d_data[i];
  }
  if(rdata != rdata2)
    printf("quad ERROR!!\n");
  else
    printf("quad PASS\n");
  for(int i = 0; i < no; i++)
  {
    printf("send is 0x%x\n", s_data[i]);
    printf("read is 0x%x\n", d_data[i]);
  }
}

void spibyteprogram(int addr, uint8_t *data, int num, int csn)
{
  unsigned char ad[3];
  ad[0] = (addr >> 16) & 0xff;
  ad[1] = (addr >> 8) & 0xff;
  ad[2] = (addr) & 0xff;
  // SPIM_CTRL = 0x6704;//dssp 8 bit
  SPI_Flash_CS_Low(csn);
  spibyte(0x6);//写使能
  SPI_Flash_CS_High(csn);
  printf("flash csn%x byte program\n", csn);
  SPI_Flash_CS_Low(csn);
  spibyte(0x02);
  spibyte(ad[0]);
  spibyte(ad[1]);
  spibyte(ad[2]);
  //发送数据
  SPIM_CTRL = 0x6704;//dssp 16 bit
  while(num--)
  {
    spibyte(*data++);
  }
  SPI_Flash_CS_High(csn);
  printf("wait spi wip\n");
  while(spiwip(csn));
}


void sendcmd4b(unsigned char cmd, unsigned char *addr, uint8_t *data, unsigned int num, int csn)
{
  SPI_Flash_CS_Low(csn);
  // SPIM_CTRL = 0x6794;//dssp 8 bit
  spibyte(cmd);
  spibyte(*addr++);
  spibyte(*addr++);
  spibyte(*addr);

  //接收数据
  for(; num > 0; num--)
  {
    *data++ = spibyte(0xff);
  }
  SPIM_CTRL = 0x6f54;//dssp 8 bit
//使用DMA接收数据
// for(;num>0;num--){
//   spibyte_exp(0xff);
// }
  SPI_Flash_CS_High(csn);
}

void spiread(int addr, uint8_t *data, int num, int csn)
{
  unsigned char ad[3];
  ad[0] = (addr >> 16) & 0xff;
  ad[1] = (addr >> 8) & 0xff;
  ad[2] = (addr) & 0xff;
  // SPIM_CTRL = 0x6754;//dssp 8 bit
  sendcmd4b(0x03, ad, data, num, csn);
}


//四线模式测试函数
void spim_rw(int csn, int no)
{
  uint8_t s_data[256];
  uint8_t d_data[256];
  uint8_t rdata, rdata2;
  int i;
  for(i = 0; i < no; i++)
    s_data[i] = 0xaaf1 + i;

  sectorerase(0x8000, csn);
  SPI_Flash_CS_Low(csn);
  // SPIM_CTRL = 0x6704;//dssp 8 bit
  spibyte(0x6);//写使能
  SPI_Flash_CS_High(csn);

  spibyteprogram(0x8000, s_data, no, csn);
  SPIM_CTRL = 0x6704;//dssp 8 bit
  spiread(0x8000, d_data, no * 2, csn);

  //check
  for(i = 0; i < no; i++)
  {
    rdata = s_data[i];
    rdata2 = d_data[i];
    if(rdata != rdata2)
      printf("ERROR :4wire\n");
    else
      printf("PASS :4wire\n");
    printf("send is 0x%x,d_data is %x\n", s_data[i], d_data[i]);
  }

  // printf("PASS :4wire :spim csn%x, BYTE write/read OK\n",csn);
}