/*
 * @Author: Iversu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2025-11-09 22:15:06
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
#include "KERNEL_MEMORY.H"
 /*------------------------------SPIM Interface Code-----------------------------*/
/**
 * @brief SPIM初始化函数
 *
 * @param mode  SPI模式，可选值为：SPIM_MODE_STANDARD, SPIM_MODE_DUAL, SPIM_MODE_QUAD, SPIM_MODE_SIMPLEX
 * @param cpha  时钟相位，可选值为：SPIM_CTRL_CPHA, 0
 * @param cpol  时钟极性，可选值为：SPIM_CTRL_CPOL, 0
 * @param dly    延迟设置:0-7:0无延迟，1：1个延时，2：2个延时，3：4个延时，4：6个延时，5：8个延时，6：12个延时，7：15个延时
 * @param dssp   数据帧位宽选择，0-15（实际值为dssp+1,即1-16）
 * @param lsb    传输模式，可选值为：SPIM_CTRL_LSB, 0
 * @param cpsr   时钟预分频设置，0-0xff（实际值为(cpsr+1)*2,即2-512）
 * @param sampdelay 采样延迟设置，0-0xff（注意不能大于（cpsr+1）*2-1）
 *
 * @return 无
 *
 * @note 此函数用于初始化SPI模块，包括SPI模式、时钟、数据位、片选、中断等。
 */
void SPIM_Init(BYTE mode, BYTE cpha, BYTE cpol, BYTE dly, BYTE dssp, BYTE lsb, BYTE cpsr, BYTE sampdelay)
{
#if !(SPIM_CLOCK_EN)
  return;
#endif
  //进行复位配置
  // SPIM_CTRL = 0x0; // 清除SPI控制寄存器
  // SPIM_CTRL &= ~SPIM_CTRL_SPE; // 关闭SPI
  SPIM_CTRL |= SPIM_CTRL_TX_CLR | SPIM_CTRL_RX_CLR; // 清除FIFO

  //配置模式
  if (mode > SPIM_MODE_DW_MOD_MASK)
  {
    dprint("SPIM_Init: Invalid mode,Used default mode:SPIM_MODE_STANDARD\n");
    mode = SPIM_MODE_STANDARD;
  }
  SPIM_MODE = SPIM_MODE_DW_MOD(mode);
  //配置分频设置寄存器
  if (cpsr > 0xFF)cpsr = 0xFF;//防止溢出，实际上不会触碰
  SPIM_CPSR = SPIM_CPSR_CPSR(cpsr);                                                // 时钟分频
  //读取采样延迟
  uint32_t sampdelay_max = ((SPIM_CPSR + 1) << 1) - 1;
  if (sampdelay_max > 0xFF)sampdelay_max = 0xFF;
  if (sampdelay > sampdelay_max)sampdelay = sampdelay_max;
  SPIM_SAMPDELAY = SPIM_SAMPDELAY_SAMPDELAY(sampdelay);

  //配置SPI控制寄存器
  if (SPIM_MODE == SPIM_MODE_QUAD && (dssp + 1) & 0x3)
  {
    dprint("SPIM_Init: Data width must be 4-bit in quad mode\n");
    dssp = ((dssp + 5) & (0xC)) - 1; // 调整为4的倍数
  }
  if (SPIM_MODE == SPIM_MODE_DUAL && (dssp + 1) & 0x1)
  {
    dprint("SPIM_Init: Data width must be 1-bit in dual mode\n");
    dssp = ((dssp + 3) & (0xE)) - 1; // 调整为4的倍数
  }
  if (dssp > 15)dssp = 15;
  uint16_t ctrl_reg = 0;
  ctrl_reg |= SPIM_CTRL_DLY_SEL(dly) | SPIM_CTRL_DSSP(dssp);
  if (cpha)ctrl_reg |= SPIM_CTRL_CPHA;
  if (cpol)ctrl_reg |= SPIM_CTRL_CPOL;
  if (lsb)ctrl_reg |= SPIM_CTRL_LSB;
  SPIM_CTRL = ctrl_reg; // 配置SPI控制寄存器

  //一般默认配置
  //取消片选
  SPIM_CSN0 = SPIM_CSN0_DESELECT; // 片选0
  SPIM_CSN1 = SPIM_CSN1_DESELECT; // 片选1
  //设置FIFO阈值
  SPIM_TXFTLR = SPIM_TXFLR_TX_LV(0x10); // 发送阈值FIFO
  SPIM_RXFTLR = SPIM_RXFLR_RX_LV(0x10); // 接收阈值FIFO
  //设置中断屏蔽
  SPIM_IMSR |= SPIM_IMSR_ERRM | SPIM_IMSR_RDOVM | \
    SPIM_IMSR_RXFIM | SPIM_IMSR_RXOIM | SPIM_IMSR_RXUIM | \
    SPIM_IMSR_TXOIM | SPIM_IMSR_TXEIM; // 屏蔽所有中断
   // SPIM_IMSR &= ~(0);//不屏蔽SPI中断
   ///清除中断
  SPIM_ISR |= SPIM_ISR_ERRS | SPIM_ISR_RDOVS | \
    SPIM_ISR_RXOIS | SPIM_ISR_RXUIS | \
    SPIM_ISR_TXOIS; // 清除SPI中断状态寄存器

  SPIM_CTRL |= SPIM_CTRL_SPE; // 使能SPI
}
/**
 * @brief SPIMDMA使能函数
 *
 * @param wr_en 写使能，可选值为：ENABLE, DISABLE
 * @param rd_en 读使能，可选值为：ENABLE, DISABLE
 *
 * @return 无
 *
 * @note 此函数用于初始化SPI模块的DMA模式，包括写使能、读使能，写失能、读失能等。
 */
void SPIM_DMA_Enable(BYTE wr_en, BYTE rd_en)
{
  register uint16_t ctrl_reg = SPIM_CTRL;
  if (wr_en)ctrl_reg |= SPIM_CTRL_DMA_WR_EN;
  else ctrl_reg &= (~SPIM_CTRL_DMA_WR_EN);
  if (rd_en)ctrl_reg |= SPIM_CTRL_DMA_RD_EN;
  else ctrl_reg &= (~SPIM_CTRL_DMA_RD_EN);
  SPIM_CTRL = ctrl_reg; // 使能DMA
  //其他适配dma配置
}
/**
 * @brief 设置SPI中断屏蔽和开关
 *
 * @param irq_mask 中断屏蔽掩码，可选值为：txeim, SPIM_IMSR_TXOIM, SPIM_IMSR_RXUIM, SPIM_IMSR_RXOIM, SPIM_IMSR_RXFIM
 * @param sw 开关控制，可选值为：ENABLE, DISABLE
 *
 * @return 无
 *
 * @note 此函数用于设置SPI中断屏蔽和开关，可选择的中断屏蔽掩码包括：TXEIM - 发送缓冲区为空中断屏蔽，
 *       TXOIM - 发送上溢中断屏蔽，RXUIM - 接收缓冲区下溢中断屏蔽，RXOIM - 接收溢出中断屏蔽，
 *       RXFIM - 接收缓冲区满中断屏蔽。开关控制可选值包括：ENABLE - 使能中断，DISABLE - 禁止中断。
 */
void SPIM_Inturrupt_Mask(uint8_t mask, uint8_t sw)
{
  if (sw == DISABLE)
  {
    SPIM_IMSR |= mask;
  }
  else
  {
    SPIM_IMSR &= (~mask);
  }
}
/**
 * @brief 设置中断使能/使能位
 * @function SPIM_Inturrupt_Enable
 * @function SPIM_Inturrupt_Disable
 * @function SPIM_Inturrupt_Enable_Bit
 * @function SPIM_Inturrupt_Disable_Bit
 * @param enable 使能位，可选值为：txeim, SPIM_IMSR_TXOIM, SPIM_IMSR_RXUIM, SPIM_IMSR_RXOIM, rxfim中的任意数量
 * @param disable 禁止位，可选值为：txeim, SPIM_IMSR_TXOIM, SPIM_IMSR_RXUIM, SPIM_IMSR_RXOIM, rxfim中的任意数量
 * @bit :单独使能/失能某一中断位
 */
void SPIM_Inturrupt_Enable(uint8_t enable)
{
  SPIM_IMSR &= (~enable);//使能
}
void SPIM_Inturrupt_Disable(uint8_t disable)
{
  SPIM_IMSR |= disable;//屏蔽
}
void SPIM_Inturrupt_Enable_Bit(uint8_t bit)
{
  if (bit <= 6)
    SPIM_IMSR &= (~(1 << bit));//使能
}
void SPIM_Inturrupt_Disable_Bit(uint8_t bit)
{
  if (bit <= 6)
    SPIM_IMSR |= (1 << bit);//屏蔽
}
/**
 * @brief 设置SPI模式
 *
 * @param mode 要设置的模式，可选值为SPIM_MODE_STANDARD或SPIM_MODE_QUAD等
 * @param cs_select 片选选择，可选值为0或1，或者其他（释放）
 * @return 无
 * @note 此函数用于设置SPI模式，并选择片选信号。
 * @note 如果存在dssp，cpsr，dly，sampdelay之类的时钟设置，请重新调度SPIM_Init初始化
 */
void SPIM_Switch_Mode(uint8_t mode)
{
  SPIM_MODE = SPIM_MODE_DW_MOD(mode);//选择模式
  return;//选择片选信号
}
/**
 * @brief 修改数据传输方式
 *
 * @param cpha 时钟相位，可选值为：SPIM_CTRL_CPHA, 0
 * @param cpol 时钟极性，可选值为：SPIM_CTRL_CPOL, 0
 * @param lsb 传输模式，可选值为：SPIM_CTRL_LSB, 0
 * @return 无
 * @note 此函数用于修改SPI模块的数据传输方式。
 */
void SPIM_Modify_Polarity(BYTE cpha, BYTE cpol, BYTE lsb)
{
  if (cpha)SPIM_CTRL |= SPIM_CTRL_CPHA;
  else SPIM_CTRL &= (~SPIM_CTRL_CPHA);
  if (cpol)SPIM_CTRL |= SPIM_CTRL_CPOL;
  else SPIM_CTRL &= (~SPIM_CTRL_CPOL);
  if (lsb)SPIM_CTRL |= SPIM_CTRL_LSB;
  else SPIM_CTRL &= (~SPIM_CTRL_LSB);
  return;
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
int32_t SPIM_CS_High(uint8_t cs_select)
{
  if (cs_select == 0)
  {
    SPIM_CSN0 = 0x1;
    if (SPIM_CSN0 != 0x1)
    {
      return -1;
    }
  }
  else if (cs_select == 1)
  {
    SPIM_CSN1 = 0x1;
    if (SPIM_CSN1 != 0x1)
    {
      return -1;
    }
  }
  return 0;
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
int32_t SPIM_CS_Low(uint8_t cs_select)
{
  if (cs_select == 0)
  {
    SPIM_CSN0 = 0x0;
    if (SPIM_CSN0 != 0x0)
    {
      return -1;
    }
  }
  else if (cs_select == 1)
  {
    SPIM_CSN1 = 0x0;
    if (SPIM_CSN1 != 0x0)
    {
      return -1;
    }
  }
  return 0;
}
/**
 *@brief SPIM片选控制函数
 *
 * @param cs_select 片选选择，可选值为SPIM_CHIP_SELECT_CSN0或SPIM_CHIP_SELECT_CSN1或SPIM_CHIP_SELECT_FREE
 * @return uint8_t
 */
int32_t SPIM_CHIP_Select(uint8_t cs_select)
{
  //先释放片选
  SPIM_CSN1 = SPIM_CSN1_DESELECT;
  SPIM_CSN0 = SPIM_CSN0_DESELECT;
  if (cs_select == SPIM_CHIP_SELECT_CSN0)
  {
    SPIM_CSN0 = SPIM_CSN0_SELECT;//选择0
    // if (SPIM_CSN0 != SPIM_CSN0_SELECT)
    // {
    //   return -1;
    // }
  }
  else if (cs_select == SPIM_CHIP_SELECT_CSN1)
  {
    SPIM_CSN1 = SPIM_CSN1_SELECT;//选择1
    // if (SPIM_CSN1 != SPIM_CSN1_SELECT)
    // {
    //   return -1;
    // }
  }
  return 0;
}
/**
 * @brief 等待SPIM空闲
 *
 * @return 成功返回0，失败返回-1
 *
 * @note 此函数用于等待SPIM总线空闲。
 */
int32_t SPIM_Wait_Busy(void)
{
  SPIM_Timeout = SPIM_DEFAULT_TIMEOUT;
  while (SPIM_SR & SPIM_SR_BSY) // 发送FIFO空标志位
  {
    SPIM_Timeout--;
    if (SPIM_Timeout == 0)
    {
      dprint("SPIM WaitBusy Timeout\n");
      return -1;
    }
  }
  return 0;
}
/**
 * @brief 等待发送完成
 *
 * @return 成功返回0，失败返回-1
 *
 * @note 此函数用于等待SPI总线发送完成。
 */
int32_t SPIM_Wait_Send(void)
{
  SPIM_Timeout = SPIM_DEFAULT_TIMEOUT;
  while (!(SPIM_SR & SPIM_SR_TFE)) // 发送FIFO空标志位
  {
    SPIM_Timeout--;
    if (SPIM_Timeout == 0)
    {
      dprint("SPIM WaitSend Timeout\n");
      return -1;
    }
  }
  return 0;
}
/**
 * @brief 发送一个数据
 *
 * @param data 要发送的数据
 *
 * @return 成功返回0，失败返回-1
 *
 * @note 此函数用于向SPI总线发送一个(dssp+1)数据
 */
int32_t SPIM_Send_Data(uint16_t data)
{
  SPIM_Timeout = SPIM_DEFAULT_TIMEOUT;
  while (!(SPIM_SR & SPIM_SR_TFNF)) // 发送FIFO不满标志位
  {
    SPIM_Timeout--;
    if (SPIM_Timeout == 0)
    {
      dprint("SPIM SendByte Timeout\n");
      return -1;
    }
  }
  SPIM_DA = data; // 发送数据
  return 0;
}
/**
 * @brief 发送一组数据（无接收）
 *
 * @param data 要发送的数据
 * @param len 要发送的数据长度
 *
 * @return 成功返回0，失败返回-1
 *
 * @note 此函数用于向SPI总线发送一组数据（无接收）。
 */
int32_t SPIM_Send_Datas(uint16_t *data, uint32_t len)
{
  while (len--)
  {
    if (SPIM_Send_Data(*data++))
    {
      dprint("SPIM SendDatas Error\n");
      return -1;
    }
  }
  return 0;
}
/**
 * @brief 读取一字节数据
 *
 * @return 接收到的数据
 *
 * @note 此函数用于从SPI总线读取一字节数据。
 */
uint32_t SPIM_Receive_Data(void)
{
  SPIM_Timeout = SPIM_DEFAULT_TIMEOUT;
  while (!(SPIM_SR & SPIM_SR_RFNE)) // 接收FIFO不空标志位
  {
    SPIM_Timeout--;
    if (SPIM_Timeout == 0)
    {
      dprint("SPIM ReceiveData Timeout\n");
      return 0xFFFFFFFF;//32bit数据
    }
  }
  return SPIM_DA;//16bit数据
}
/**
 * @brief 接收一组数据（无发送）
 *
 * @param data 要接收的数据
 * @param len 要接收的数据长度
 *
 * @return 成功返回0，失败返回-1(无失败判断方案)
 *
 * @note 此函数用于向SPI总线接收一组数据（无发送）。
 */
int32_t SPIM_Receive_Datas(uint16_t *data, uint32_t len)
{
  while (len--)
  {
    uint32_t temp = SPIM_Receive_Data();
    // if (temp == 0xFFFFFFFF)return -1; //接收异常
    if (temp > 0xFFFF)return -1; //接收异常
    *data++ = temp & 0xFFFF;
  }
  return 0;
}
/**
 *@brief 发送和接收一个数据（如果不关心发送，可以用SPIM_Only_Receive_OnceData）
 *
 * @param data
 * @return uint16_t receive data
 *
 * @note 此函数用于发送和接收一个数据，适用于全双工的标准模式。
 */
uint32_t SPIM_Send_Receive_OnceData(uint16_t data)
{
#if 0//先排除其他数据
  while (SPIM_RXFLR)//如果接收不为空，则舍弃原接收数据
  {
    SPIM_Receive_Data();
  }
#endif
  if (SPIM_Send_Data(data) == -1) return 0xFFFFFFFF;
  return SPIM_Receive_Data();
}
/**
 *@brief 发送和接收一组数据（如果不关心发送，可以用SPIM_Only_Receive_Datas）
 *
 * @param data
 * @return uint16_t receive data
 *
 * @note 此函数用于发送和接收一组数据，适用于全双工的标准模式。
 */
int32_t SPIM_Send_Receive_Datas(uint16_t *send, uint16_t *receive, uint32_t len)
{
#if 0//先排除其他数据
  while (SPIM_RXFLR)//如果接收不为空，则舍弃原接收数据
  {
    SPIM_Receive_Data();
  }
#endif
  while (len--)
  {
    uint32_t temp = SPIM_Send_Receive_OnceData(*send++);
    // if (temp == 0xFFFFFFFF)return -1; //接收异常
    if (temp > 0xFFFF)return -1; //接收异常
    *receive++ = temp & 0xFFFF;
  }
  return 0;
}
/**
 * @brief 仅接收一次数据
 *
 * @return uint16_t receive data
 *
 * @note 此函数用于仅接收一次数据。
 */
uint32_t SPIM_Only_Receive_OnceData(void)//不关注发送的数据
{
  SPIM_Wait_Send();//等待发送完成
#if 0//先排除其他数据
  while (SPIM_RXFLR)//如果接收不为空，则舍弃原接收数据
  {
    SPIM_Receive_Data();
  }
#endif
  SPIM_RDNUM = 1;//发起读取
  uint32_t data = SPIM_Receive_Data();//读取数据
  SPIM_RDNUM = 0; //关闭读取
  return data;
}
/**
 * @brief 仅接收一组数据
 *
 * @return uint16_t receive data
 *
 * @note 此函数用于仅接收一组数据。
 */
int32_t SPIM_Only_Receive_Datas(uint16_t *receive, uint32_t len)//不关注发送的数据
{
  SPIM_Wait_Send();//等待发送完成
#if 0//先排除其他数据
  while (SPIM_RXFLR)//如果接收不为空，则舍弃原接收数据
  {
    SPIM_Receive_Data();
  }
#endif
  uint16_t count = len >> 16;
  uint16_t remain = len & 0xFFFF;
  uint16_t *data = receive;
  while (count--)
  {
    SPIM_RDNUM = 0xFFFF;//发起读取
    if (SPIM_Receive_Datas(data, 0xFFFF))return -1;
    data += 0xFFFF;
  }
  if (remain)
  {
    SPIM_RDNUM = remain;//发起读取
    if (SPIM_Receive_Datas(data, remain))return -1;
  }
  SPIM_RDNUM = 0; //关闭读取
  return 0;
}
/*******************************************************SPI FLASH DEMO*******************************************************/
/**
 * @brief 判断对SPI flash是否忙碌
 *
 * @param cs_select SPI控制器选择，可选值为0或1
 *
 * @return 返回SPI控制器的状态，0表示空闲，1表示忙碌
 *
 * @note 此函数用于判断指定SPI控制器选中的flash是否忙碌
 */
uint8_t SPIM_FLASH_Busy(uint8_t mode, uint8_t cs_select)
{
  SPIM_CHIP_Select(cs_select);// 使用片选
  if (mode == SPIM_MODE_SIMPLEX)
  {//此时由于三线属于并线，实际接收即发送通道,因此不需要考虑接收问题
    SPIM_Switch_Mode(SPIM_MODE_SIMPLEX);//
    // SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
    SPIM_Send_Data(SPI_FLASH_CMD_READ_STATUS_REGISTERS1);
    SPIM_Wait_Send();//  SPIM_Wait_Send();//SPIM_Wait_Busy();//等待发送完成
  }
  else//其他模式下均以标准模式读写
  {
    SPIM_Switch_Mode(SPIM_MODE_STANDARD);//
// SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
    SPIM_Send_Receive_OnceData(SPI_FLASH_CMD_READ_STATUS_REGISTERS1);
  }
  FLASH_SR = SPIM_Only_Receive_OnceData();
  SPIM_CHIP_Select(SPIM_CHIP_SELECT_FREE);// 释放片选
  return (FLASH_SR & SPI_FLASH_SR1_BUSY);
}
/**
 * @brief 等待flash空闲
 *
 * @param cs_select SPI控制器选择，可选值为0或1
 *
 * @return 无
 *
 */
int32_t SPIM_FLASH_WaitBusy(uint8_t mode, uint8_t cs_select)
{
  SPIM_Timeout = SPIM_DEFAULT_TIMEOUT;
  while (SPIM_FLASH_Busy(mode, cs_select) || (SPIM_Timeout--));
  if (SPIM_Timeout == 0)return -1;
  return 0;
}
/**
 * @brief 对SPI Flash进行复位
 *
 * @param cs_select 选择哪个SPI控制器
 *
 * @return None
 */
int32_t SPIM_Reset_FLASH(uint8_t mode, uint8_t cs_select)
{
  SPIM_CHIP_Select(cs_select);// 使用片选
  if (mode == SPIM_MODE_SIMPLEX)
  {//此时由于三线属于并线，实际接收即发送通道,因此不需要考虑接收问题
    SPIM_Switch_Mode(SPIM_MODE_SIMPLEX);//
    // SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
    SPIM_Send_Data(SPI_FLASH_CMD_ENABLE_RESET);
    SPIM_Wait_Send();//  SPIM_Wait_Send();//SPIM_Wait_Busy();//等待发送完成
    SPIM_CS_High(cs_select);  // 释放片选
    SPIM_CS_Low(cs_select);// 选中片选
    SPIM_Send_Data(SPI_FLASH_CMD_RESET);//复位
    SPIM_Wait_Send();//  SPIM_Wait_Send();//SPIM_Wait_Busy();//等待发送完成
  }
  else//其他模式下均以标准模式读写
  {
    SPIM_Switch_Mode(SPIM_MODE_STANDARD);//
     // SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
    SPIM_Send_Receive_OnceData(SPI_FLASH_CMD_ENABLE_RESET);//使能复位
    SPIM_CS_High(cs_select);   // 释放片选
    SPIM_CS_Low(cs_select);// 选中片选
    SPIM_Send_Receive_OnceData(SPI_FLASH_CMD_RESET);//复位
  }
  SPIM_CHIP_Select(SPIM_CHIP_SELECT_FREE);// 释放片选
  dprint("SPIM Reset FLASH\n");
  return 0;
}
/**
 * @brief 写SPI FLASH 状态寄存器
 *
 * @param data 要写入的数据
 * @param cs_select SPI控制器选择，可选值为0或1
 *
 * @return 返回SPI控制器的状态，0表示空闲，1表示忙碌
 *
 * @note 此函数用于向指定SPI控制器选中的flash的状态寄存器写入指定数据，适用全双工的四线
 */
void SPIM_FLASH_Wirte_Status(uint8_t mode, uint16_t data, uint8_t cs_select)
{
  SPIM_CHIP_Select(cs_select);// 使用片选
  if (mode == SPIM_MODE_SIMPLEX)
  {//此时由于三线属于并线，实际接收即发送通道,因此不需要考虑接收问题
    SPIM_Switch_Mode(SPIM_MODE_SIMPLEX);//
    // SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
    SPIM_Send_Data(SPI_FLASH_CMD_WRITE_STATUS_REGISTERS);
    SPIM_Send_Data(data & 0xff);
    SPIM_Send_Data((data >> 8) & 0xff);
    SPIM_Wait_Send();//  SPIM_Wait_Send();//SPIM_Wait_Busy();//等待发送完成
  }
  else//其他模式下均以标准模式读写
  {
    SPIM_Switch_Mode(SPIM_MODE_STANDARD);//
// SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
    SPIM_Send_Receive_OnceData(SPI_FLASH_CMD_WRITE_STATUS_REGISTERS);
    SPIM_Send_Receive_OnceData(data & 0xff);
    SPIM_Send_Receive_OnceData((data >> 8) & 0xff);
  }
  SPIM_CHIP_Select(SPIM_CHIP_SELECT_FREE);// 释放片选
  dprint("SPIM Write Status:%#x\n", data);
  return;
}
/**
 * @brief 使能SPIM QUAD模式(不支持三线)
 *
 * @param cs_select 选择哪个SPI控制器，可选值为0或1
 *
 * @return 0:成功，-1:失败
 *
 * @note 此函数用于向指定SPI控制器选中的flash的状态寄存器写入指定数据
 */
int32_t SPIM_QUAD_Enable(uint8_t cs_select)
{
#if 1
  {
    uint16_t status = 0;
    //读状态
    SPIM_Switch_Mode(SPIM_MODE_STANDARD);//
 // SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
    SPIM_Send_Receive_OnceData(SPI_FLASH_CMD_READ_STATUS_REGISTERS1);
    status = SPIM_Send_Receive_OnceData(0xff);
    SPIM_CS_High(cs_select);  // 释放片选
    SPIM_CS_Low(cs_select);// 选中片选
    SPIM_Send_Receive_OnceData(SPI_FLASH_CMD_READ_STATUS_REGISTERS2);
    status |= (SPIM_Send_Receive_OnceData(0xff) << 8);
    status |= 0x0200;
    SPIM_CHIP_Select(SPIM_CHIP_SELECT_FREE);// 释放片选
//更新状态
    SPIM_FLASH_Wirte_Status(SPIM_MODE_STANDARD, status, cs_select);//使能QUAD模式 
  }
#else
  SPIM_FLASH_Wirte_Status(SPIM_MODE_STANDARD, 0x0200, cs_select);//使能QUAD模式 
#endif
  dprint("SPIM QUAD Enable\n");
  return 0;
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
void SPIM_Write_Enable(uint8_t mode, uint8_t cs_select)
{
  SPIM_CHIP_Select(cs_select);// 使用片选
  if (mode == SPIM_MODE_SIMPLEX)
  {//此时由于三线属于并线，实际接收即发送通道,因此不需要考虑接收问题
    SPIM_Switch_Mode(SPIM_MODE_SIMPLEX);//
    // SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
    SPIM_Send_Data(SPI_FLASH_CMD_WRITE_ENABLE);
    SPIM_Wait_Send();//  SPIM_Wait_Send();//SPIM_Wait_Busy();//等待发送完成
  }
  else//其他模式下均以标准模式读写
  {
    SPIM_Switch_Mode(SPIM_MODE_STANDARD);//
// SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
    SPIM_Send_Receive_OnceData(SPI_FLASH_CMD_WRITE_ENABLE);
  }
  SPIM_CHIP_Select(SPIM_CHIP_SELECT_FREE);// 释放片选
  dprint("SPIM Write Enable\n");
}
/**
 * @brief SPI写失能
 *
 * @param cs_select 选择哪个SPI控制器，可选值为0或1
 *
 * @return 无
 *
 * @note
 *  此函数用于SPI写使能，即向SPI Flash发送写使能命令。
 */
void SPIM_Write_Disable(uint8_t mode, uint8_t cs_select)
{
  SPIM_CHIP_Select(cs_select);// 使用片选
  if (mode == SPIM_MODE_SIMPLEX)
  {//此时由于三线属于并线，实际接收即发送通道,因此不需要考虑接收问题
    SPIM_Switch_Mode(SPIM_MODE_SIMPLEX);//
    // SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
    SPIM_Send_Data(SPI_FLASH_CMD_WRITE_DISABLE);
    SPIM_Wait_Send();//  SPIM_Wait_Send();//SPIM_Wait_Busy();//等待发送完成
  }
  else//其他模式下均以标准模式读写
  {
    SPIM_Switch_Mode(SPIM_MODE_STANDARD);//
// SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
    SPIM_Send_Receive_OnceData(SPI_FLASH_CMD_WRITE_DISABLE);
  }
  SPIM_CHIP_Select(SPIM_CHIP_SELECT_FREE);// 释放片选
  dprint("SPIM Write Disable\n");
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
uint32_t SPIM_Read_FLASH_ID(uint8_t mode, uint8_t cs_select)
{
  SPIM_CHIP_Select(cs_select);// 使用片选
  if (mode == SPIM_MODE_SIMPLEX)
  {//此时由于三线属于并线，实际接收即发送通道
    SPIM_Switch_Mode(SPIM_MODE_SIMPLEX);//
    // SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
    SPIM_Send_Data(SPI_FLASH_CMD_READ_JEDEC_ID);//SPI_FLASH_CMD_RMDID//SPI_FLASH_CMD_RJID
    SPIM_Wait_Send();//SPIM_Wait_Busy();//  SPIM_Wait_Send();//等待发送完成
    // SPI_FLAHS_ID |= SPIM_Only_Receive_OnceData();
    // SPI_FLAHS_ID <<= 8;
    // SPI_FLAHS_ID |= SPIM_Only_Receive_OnceData();
    // SPI_FLAHS_ID <<= 8;
    // SPI_FLAHS_ID |= SPIM_Only_Receive_OnceData();
    uint16_t id[3];
    SPIM_Only_Receive_Datas(id, 3); // 读取剩余的3个字节
    SPI_FLAHS_ID |= id[0];
    SPI_FLAHS_ID <<= 8;
    SPI_FLAHS_ID |= id[1];
    SPI_FLAHS_ID <<= 8;
    SPI_FLAHS_ID |= id[2];
  }
  else
  {
    SPIM_Switch_Mode(SPIM_MODE_STANDARD);
// SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
// SPIM_Wait_Busy();//等待总线释放
//因为是标准模式，所以会有读取行为，此处需要空读
    SPIM_Send_Receive_OnceData(SPI_FLASH_CMD_READ_JEDEC_ID);//SPI_FLASH_CMD_RMDID//SPI_FLASH_CMD_RJID//SPI_FLASH_CMD_RMDID_QIO//SPI_FLASH_CMD_RMDID_DIO
    SPI_FLAHS_ID |= SPIM_Send_Receive_OnceData(0xff);
    SPI_FLAHS_ID <<= 8;
    SPI_FLAHS_ID |= SPIM_Send_Receive_OnceData(0xff);
    SPI_FLAHS_ID <<= 8;
    SPI_FLAHS_ID |= SPIM_Send_Receive_OnceData(0xff);
  }
  SPIM_CHIP_Select(SPIM_CHIP_SELECT_FREE);// 释放片选
  dprint("SPIM Read FLASH ID:%#x\n", SPI_FLAHS_ID);
  return SPI_FLAHS_ID;
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
int32_t SPIM_Read_FLASH(uint8_t mode, uint8_t cs_select, uint32_t address, uint16_t bytes, uint16_t *data)
{
  SPIM_CHIP_Select(cs_select);// 使用片选
  if (mode == SPIM_MODE_SIMPLEX)
  {//此时由于三线属于并线，实际接收即发送通道
    uint16_t cmd = SPI_FLASH_CMD_READ_DATA;//SPI_FLASH_CMD_READ_DATA//SPI_FLASH_CMD_FAST_READ
    SPIM_Switch_Mode(SPIM_MODE_SIMPLEX);//
    // SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
    SPIM_Send_Data(cmd);
    SPIM_Send_Data((address >> 16) & 0xff);//  
    SPIM_Send_Data((address >> 8) & 0xff);// 
    SPIM_Send_Data((address >> 0) & 0xff);//
    if (cmd == 0x0b)
      SPIM_Send_Data(0xff);// 0B专属 ,dummy

    SPIM_Wait_Busy();//  SPIM_Wait_Send();//等待发送完成
    SPIM_RDNUM = bytes;//发起读取
    while (bytes--)
    {
      *data++ = SPIM_Receive_Data();
    }
    SPIM_RDNUM = 0;
  }
  else if (mode == SPIM_MODE_DUAL)
  {
    uint16_t cmd = SPI_FLASH_CMD_FAST_READ_DUAL_OUTPUT;//SPI_FLASH_CMD_FAST_READ_DUAL_OUTPUT//SPI_FLASH_CMD_FAST_READ_DUAL_IO
    SPIM_Switch_Mode(SPIM_MODE_STANDARD);//
    SPIM_Send_Receive_OnceData(cmd);
    SPIM_Send_Receive_OnceData((address >> 16) & 0xff);//  
    SPIM_Send_Receive_OnceData((address >> 8) & 0xff);// 
    SPIM_Send_Receive_OnceData((address >> 0) & 0xff);//
    SPIM_Send_Receive_OnceData(0xFF);//dummy Clock
    SPIM_Wait_Send();//  SPIM_Wait_Busy();//等待发送完成
    SPIM_Switch_Mode(SPIM_MODE_DUAL);//
    SPIM_RDNUM = bytes;//发起读取
    while (bytes--)
    {
      *data++ = SPIM_Receive_Data();
    }
    SPIM_RDNUM = 0;//清空RDNUM
  }
  else if (mode == SPIM_MODE_QUAD)
  {
    uint16_t cmd = SPI_FLASH_CMD_FAST_READ_QUAD_OUTPUT;//SPI_FLASH_CMD_FAST_READ_QUAD_OUTPUT //SPI_FLASH_CMD_FAST_READ_QUAD_IO
    SPIM_Switch_Mode(SPIM_MODE_STANDARD);//
    SPIM_Send_Receive_OnceData(cmd);
    SPIM_Send_Receive_OnceData((address >> 16) & 0xff);//  
    SPIM_Send_Receive_OnceData((address >> 8) & 0xff);// 
    SPIM_Send_Receive_OnceData((address >> 0) & 0xff);//
    SPIM_Send_Receive_OnceData(0xFF);//dummy Clock=
    SPIM_Wait_Send();//  SPIM_Wait_Busy();//等待发送完成
    SPIM_Switch_Mode(SPIM_MODE_QUAD);//
    SPIM_RDNUM = bytes;//发起读取
    while (bytes--)
    {
      *data++ = SPIM_Receive_Data();
    }
    SPIM_RDNUM = 0;//清空RDNUM
  }
  else if (mode == SPIM_MODE_STANDARD)
  {
    uint16_t cmd = SPI_FLASH_CMD_FAST_READ_DUAL_OUTPUT;//SPI_FLASH_CMD_FAST_READ_DUAL_OUTPUT//SPI_FLASH_CMD_FAST_READ_DUAL_IO
    SPIM_Switch_Mode(SPIM_MODE_STANDARD);//
    SPIM_Send_Receive_OnceData(cmd); // 读命令
    SPIM_Send_Receive_OnceData((address & 0xFF0000) >> 16);
    SPIM_Send_Receive_OnceData((address & 0xFF00) >> 8);
    SPIM_Send_Receive_OnceData((address >> 0) & 0xff);
    if (cmd == 0x0b)
      SPIM_Send_Receive_OnceData(0xFF);// 0B专属 ,dummy
    while (bytes--)
    {
      *data++ = SPIM_Send_Receive_OnceData(0xFF);
    }
  }
  SPIM_CHIP_Select(SPIM_CHIP_SELECT_FREE);// 释放片选
  dprint("SPIM Read FLASH Data\n");
  return 0;
}
/**
 * @brief 页写入函数,要先写使能
 * @param  mode: 写入模式
 * @param  cs_select: 选择哪个片选
 * @param  address: 写入的起始地址
 * @param  bytes: 写入多少字节
 * @param  data: 写入的数据指针（注意为16位，需先做字节序转换处理）
 * @return 0成功，其他失败。
 */
int32_t SPIM_Write_FLASH(uint8_t mode, uint8_t cs_select, uint32_t address, uint32_t bytes, uint16_t *data)
{
  SPIM_CHIP_Select(cs_select);// 使用片选
  if (mode == SPIM_MODE_SIMPLEX)
  {
    SPIM_Switch_Mode(SPIM_MODE_SIMPLEX);//选择单线模式
   // SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
    SPIM_Send_Data(SPI_FLASH_CMD_PAGE_PROGRAM);// Page Program 命令
    SPIM_Send_Data((address >> 16) & 0xFF);
    SPIM_Send_Data((address >> 8) & 0xFF);
    SPIM_Send_Data((address >> 0) & 0xFF);
    while (bytes--)
    {
      SPIM_Send_Data(*data++);
    }
    SPIM_Wait_Send();//SPIM_Wait_Busy();//等待发送完成
  }
  else if (mode == SPIM_MODE_QUAD)
  {
    SPIM_Switch_Mode(SPIM_MODE_STANDARD);//选择标准模式，发送命令地址
   // SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
    SPIM_Send_Receive_OnceData(SPI_FLASH_CMD_QUAD_INPUT_PAGE_PROGRAM); //QUAD Page Program 命令
    SPIM_Send_Receive_OnceData((address >> 16) & 0xFF);
    SPIM_Send_Receive_OnceData((address >> 8) & 0xFF);
    SPIM_Send_Receive_OnceData((address >> 0) & 0xFF);
    SPIM_Switch_Mode(SPIM_MODE_QUAD);//选择QUAD模式传输数据
    while (bytes--)
    {
      SPIM_Send_Data(*data++);
    }
    SPIM_Wait_Send();//SPIM_Wait_Busy();//等待发送完成
  }
  else
  {
    SPIM_Switch_Mode(SPIM_MODE_STANDARD);//选择标准模式
   // SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
    SPIM_Send_Receive_OnceData(SPI_FLASH_CMD_PAGE_PROGRAM); // Page Program 命令
    SPIM_Send_Receive_OnceData((address >> 16) & 0xFF);
    SPIM_Send_Receive_OnceData((address >> 8) & 0xFF);
    SPIM_Send_Receive_OnceData((address >> 0) & 0xFF);
    while (bytes--)
    {
      SPIM_Send_Receive_OnceData(*data++);
    }
  }
  SPIM_CHIP_Select(SPIM_CHIP_SELECT_FREE);// 释放片选
  dprint("SPIM Write FLASH\n");
  return 0;
}
/**
 * @brief  擦除SPI Flash,要先写使能
 * @param  mode: 数据传输模式
 * @param  cs_select: 片选信号选择
 * @param  cmd: 擦除命令
 * @param  address: 需要擦除的扇区地址
 * @retval 0成功，其他失败。
 */
int32_t SPIM_Erase_FLASH(uint8_t mode, uint8_t cs_select, uint8_t cmd, uint32_t address)
{
  if (cmd == SPI_FLASH_CMD_SECTOR_ERASE)//4KB sector erase
  {
    address &= 0xFFFFF000;//4KB对齐
  }
  else if (cmd == SPI_FLASH_CMD_32KB_BLOCK_ERASE)//32KB block erase
  {
    address &= 0xFFFF8000;//32KB对齐
  }
  else if (cmd == SPI_FLASH_CMD_64KB_BLOCK_ERASE)//64KB block erase
  {
    address &= 0xFFFF0000;//4KB对齐
  }
  else if (cmd == SPI_FLASH_CMD_CHIP_ERASE1 || cmd == SPI_FLASH_CMD_CHIP_ERASE2)//chip erase
  {
    address = 0;//地址无效
  }
  else
  {
    return -1;
  }
  SPIM_CHIP_Select(cs_select);// 使用片选
  if (mode == SPIM_MODE_SIMPLEX)
  {
    SPIM_Switch_Mode(SPIM_MODE_SIMPLEX);//选择3线模式（单数据线）
   // SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
    SPIM_Send_Data(cmd);
    if (cmd == SPI_FLASH_CMD_SECTOR_ERASE || cmd == SPI_FLASH_CMD_32KB_BLOCK_ERASE || cmd == SPI_FLASH_CMD_64KB_BLOCK_ERASE)
    {
      SPIM_Send_Data((address >> 16) & 0xFF);
      SPIM_Send_Data((address >> 8) & 0xFF);
      SPIM_Send_Data((address >> 0) & 0xFF);
    }
    SPIM_Wait_Send();//SPIM_Wait_Busy();//等待发送完成
  }
  else
  {
    SPIM_Switch_Mode(SPIM_MODE_STANDARD);//选择标准模式
   // SPIM_Modify_Polarity(0, 0, 0);//如有需要设置SPIM的时钟极性和相位，此处为标准模式
    SPIM_Send_Receive_OnceData(cmd);

    if (cmd == SPI_FLASH_CMD_SECTOR_ERASE || cmd == SPI_FLASH_CMD_32KB_BLOCK_ERASE || cmd == SPI_FLASH_CMD_64KB_BLOCK_ERASE)
    {
      SPIM_Send_Receive_OnceData((address >> 16) & 0xFF);
      SPIM_Send_Receive_OnceData((address >> 8) & 0xFF);
      SPIM_Send_Receive_OnceData((address >> 0) & 0xFF);
    }
  }
  SPIM_CHIP_Select(SPIM_CHIP_SELECT_FREE);// 释放片选
  dprint("SPIM Erase FLASH\n");
  return 0;
}

