/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-17 23:28:15
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
#include "KERNEL_ADC.H"

 /**
  * @brief ADC触发模式选择
  *
  * @param mode 0软件 1硬件 2连续 3自动采样
  *
  * @return             无
  */
void ADC_TriggerMode_Config(uint8_t mode)
{
      switch (mode)
      {
      case 0:
            ADC_CTRL &= (~(0x3 << 7));    // 软件触发
            break;
      case 1:
            ADC_CTRL |= (0x1 << 7);
            ADC_CTRL &= (~(0x1 << 8));    // 硬件触发
            break;
      case 2:
            ADC_CTRL |= (0x1 << 8);
            ADC_CTRL &= (~(0x1 << 7));    // 连续触发
            break;
      case 3:
            ADC_CTRL |= (0x3 << 7);       // 自动采样
            break;
      default:
            break;
      }
}

/**
 * @brief 单端or差分模式选择
 *
 * @param mode 0:单端  1:差分
 *
 * @return             无
 */
void ADC_SampleMode_Config(uint8_t mode)
{
      switch (mode)
      {
      case 0:
            ADC_CTRL &= (~(0x1 << 4));
            break;
      case 1:
            ADC_CTRL |= (0x1 << 4);
            break;
      default:
            break;
      }
}

/**
 * @brief ADC通道使能
 *
 * @param channelx 通道选择，取值范围为 0~11
 * @param sw DISABLE  ENABLE
 *
 * @return 无
 */
void ADC_Channelx_Config(uint8_t channelx, uint8_t sw)
{
      if ((ADC_CTRL & 0x10) == 0) // 单端模式
      {
            if (sw == DISABLE)
            {
                  ADC_CHAN_EN &= (~(0x1 << channelx));
            }
            else
            {
                  if ((((ADC_CTRL >> 7) & 0x3) == 0) || (((ADC_CTRL >> 7) & 0x3) == 0x2))//软件和连续触发只能使能一个通道
                  {
                        ADC_CHAN_EN = (0x1 << channelx);
                  }
                  else
                  {
                        ADC_CHAN_EN |= (0x1 << channelx);
                  }
            }
      }
      else
      {
            if (channelx >= 7)
            {
                  dprint("The value of ADC channel is exceeded\n");
                  return;
            }
            if (sw == DISABLE)
            {
                  ADC_CHAN_EN &= (~(0x1 << channelx));
            }
            else
            {
                  if ((((ADC_CTRL >> 7) & 0x3) == 0) || (((ADC_CTRL >> 7) & 0x3) == 0x2))//软件和连续触发只能使能一个通道
                  {
                        ADC_CHAN_EN = (0x1 << channelx);
                  }
                  else
                  {
                        ADC_CHAN_EN |= (0x1 << channelx);
                  }
            }
      }

}

/**
 * @brief ADC中断使能和屏蔽
 *
 * @param channelx irq_type：ADCData0_intrMask ADCData1_intrMask ADCData2_intrMask....
 * @param sw DISABLE  ENABLE
 * @param comparator_on DISABLE  ENABLE
 *
 * @return             无
 */
void ADC_IRQ_Config(uint16_t irq_type, uint8_t sw, uint8_t comparator_on)
{
      if (sw == ENABLE)
      {
            ADC_INTMASK |= (irq_type);
            if (comparator_on == ENABLE)
                  ADC_INTMASK |= (irq_type << 8);
      }
      else
      {
            ADC_INTMASK &= ~(irq_type);
            if (comparator_on == DISABLE)
                  ADC_INTMASK &= (~(irq_type << 8));

      }
}

/**
 * @brief ADC清除中断状态位
 *
 * @param channelx irq_type：uv_irq ov_irq
 *
 * @return             无
 */
void ADC_Clear_IRQ(uint8_t irq_type)
{
      ADC_INTSTAT |= irq_type;
}

/**
 * @brief 检查ADC采样是否完成
 *
 * @param ADC_Databuffer_channelx:adc采样数据缓存通道选择，取值范围为 0~7
 *
 * @return 采样到有效数据返回1，否则返回0
 */
uint8_t CheckADCDataValidity(uint8_t ADC_Databuffer_channelx)
{
      if (ADC_INTSTAT & (0x1 << ADC_Databuffer_channelx))
      {
            ADC_INTSTAT |= (0x1 << ADC_Databuffer_channelx);
            return 1;
      }
      else
      {
            return 0;
      }
}

/**
 * @brief 配置定时器计数器值
 *
 * @param timercount 定时器计数器值
 *
 * @return 无
 *
 * @note 函数只在配置为硬件触发时，当 timercount 递减到 0 时才进行采样
 */
void ADC_Timercount_Set(uint32_t timercount)
{
      ADC_CNT0 = (timercount);
      ADC_CNT1 = (timercount >> 16);
}

/**
 * @brief 设置ADC通道采样的数据存入哪个数据缓存通道
 *
 * @param channelx 通道选择，取值范围为 0~15
 * @param ADC_Databuffer_channelx adc采样数据缓存通道选择，取值范围为 0~7
 *
 * @return 无
 *
 * @note 无
 */

void MatchADCChannelToData(uint8_t channelx, uint16_t ADC_Databuffer_channelx)
{
      switch (ADC_Databuffer_channelx)
      {
      case 0:
            ADC_DATA_0_CHAN = channelx;
            break;
      case 1:
            ADC_DATA_1_CHAN = channelx;
            break;
      case 2:
            ADC_DATA_2_CHAN = channelx;
            break;
      case 3:
            ADC_DATA_3_CHAN = channelx;
            break;
      case 4:
            if (channelx == 4)
                  ADC_DATA_4567_CHAN |= (0x1 << 4);
            break;
      case 5:
            if (channelx == 5)
                  ADC_DATA_4567_CHAN |= (0x1 << 5);
            break;
      case 6:
            if (channelx == 6)
                  ADC_DATA_4567_CHAN |= (0x1 << 6);
            break;
      case 7:
            if (channelx == 7)
                  ADC_DATA_4567_CHAN |= (0x1 << 7);
            break;
      default:
            break;
      }
}

/**
 * @brief 使能data_overflow_event
 *
 * @param sw 使能：1 失能：0
 *
 * @note 开启data_overflow_event中断 0：开启 1：关闭
 */
void  DataOverflow_IRQ_Config(uint8_t sw)
{
      if (sw == ENABLE)
      {
            ADC_CTRL &= ~(0x4);
      }
      else
      {
            ADC_CTRL |= (0x4);
      }
}

/**
 * @brief 使能end_of_adc_cycle_event
 *
 * @param sw 使能：1 失能：0
 *
 * @note 开启使能end_of_adc_cycle_event中断 1：开启 0：关闭
 */
void  ADC_CycleEvent_IRQ_Config(uint8_t sw)
{
      if (sw == ENABLE)
      {
            ADC_CTRL |= (0x2);
      }
      else
      {
            ADC_CTRL &= ~(0x2);
      }
}

/**
 * @brief 开启补码
 *
 * @param sw 使能：1 失能：0
 *
 * @note 无
 */
void  ADC_Format_Config(uint8_t format)
{

      if (format == CompCode)
      {
            ADC_CTRL |= (0x1 << 6);
      }
      else
      {
            ADC_CTRL &= ~(0x1 << 6);
      }
}

/**
 * @brief 计算校正值
 *
 * @param A_VALUE 设置A的值
 * @param B_VALUE 设置B的值
 *
 * @note 公式：A * X + B ,X为采样值,采样定点小数方式计算
 */
void  ADC_Calibration_Config(uint16_t A_VALUE, uint16_t B_VALUE)
{
      ADC_CTRL &= ~(0x1 << 9);  //set  samp value as x
      ADC_A_VALUE = A_VALUE;
      ADC_B_VALUE = B_VALUE;
}

/**
 * @brief 软件触发和连续触发采样值必须进行trigger
 *
 * @note 无
 */
void ADC_Trigger()
{
      ADC_TRIGGER = 0x1;
}

/**
 * @brief 使能单通道
 *
 * @param channelx 通道
 * @param sw 使能：1 失能：0
 *
 * @note 无
 */
void ADC_Channelx_Set(uint8_t channelx, uint8_t sw)
{
      if (sw == ENABLE)
      {
            ADC_CHAN_EN = (0x1 << channelx);
      }
}

/**
 * @brief ADC配置采样时间
 *
 * @param timercount 配置采样触发的时间（ms）
 */
void ADC_Cnt(uint32_t timercount)
{
      uint32_t count = (CHIP_CLOCK_INT_LOW * timercount) / 1000;
      ADC_CNT0 = count & 0xFFFF;    // 设置低16位到 ADC_CNT0
      ADC_CNT1 = (count >> 16) & 0xFFFF;  // 设置高16位到 ADC_CNT1
}

/**
 * @brief ADC单通道软件触发采样初始化函数-单通道
 *
 * @param channelx 通道选择，取值范围为 0~11
 * @param ADC_Databuffer_channelx adc采样数据缓存通道选择，取值范围为 0~7
 * @param mode 单端或差分模式选择，0表示单端，1表示差分
 *
 * @return 无
 *
 * @note CPU 触发一次读一个采样数据。软件触发每次只能使能 1 个通道。软件触发每次只能使能 1 个通道
 */
void ADC_SW_Sample_Init_Single(uint8_t channelx, uint8_t ADC_Databuffer_channelx, uint8_t mode)
{
      //单通道
      ADC_IRQ_Config(0x1 << ADC_Databuffer_channelx, ENABLE, DISABLE);
      ADC_CTRL |= 0x1; //使能adc模拟
      ADC_CycleEvent_IRQ_Config(DISABLE);//关闭中断end_of_adc_cycle_event
      DataOverflow_IRQ_Config(DISABLE);//关闭中断data_overflow_event
      ADC_TriggerMode_Config(SW_SAMPLE);// 触发模式选择
      ADC_SampleMode_Config(mode);// 单端or差分模式选择

      //配置补码
      ADC_Format_Config(OriginCode);//设置为原码

      //配置校准
      ADC_Calibration_Config(0x8000, 0);//设置a=0x8000,b=0
      ADC_Channelx_Set(channelx, ENABLE);// 通道使能
      MatchADCChannelToData(channelx, ADC_Databuffer_channelx); //设置ADC通道采样的数据存入哪个数据缓存通道

      //配置触发
      ADC_Trigger();
}

/**
 * @brief ADC多通道软件触发采样初始化函数
 *
 * @param mode 单端或差分模式选择，0表示单端，1表示差分
 * @param Data_Select 0:0-3 chananl -> 0-3buffer
 *                    1:4-7 chanal -> 0-3buffer
 *                    2:8->11 chanal -> 0-3buffer
 *                    3:12-14 chanal -> 0-2buffer
 * @param Chanal_8_mode 0：4通道 1：8通道
 * @return 无
 *
 * @note
 */
void ADC_SW_Sample_Init_Multi(uint8_t mode, uint8_t Data_Select, uint8_t Chanal_8_mode)
{
      ADC_CTRL |= 0x1; //使能adc模拟
      // 单端or差分模式选择
      ADC_INTMASK = 0xff;
      ADC_CycleEvent_IRQ_Config(DISABLE);   //关闭cycle中断
      DataOverflow_IRQ_Config(DISABLE);//关闭中断data_overflow_event
      ADC_TriggerMode_Config(SW_SAMPLE);                  // 触发模式选择
      ADC_SampleMode_Config(mode);
      ADC_CHAN_EN = 0xffff;   //使能多通道

      switch (Data_Select)
      {
      case 0:
            MatchADCChannelToData(0, 0);
            MatchADCChannelToData(1, 1);
            MatchADCChannelToData(2, 2);
            MatchADCChannelToData(3, 3);
            break;
      case 1:
            MatchADCChannelToData(4, 0);
            MatchADCChannelToData(5, 1);
            MatchADCChannelToData(6, 2);
            MatchADCChannelToData(7, 3);
            break;
      case 2:
            MatchADCChannelToData(8, 0);
            MatchADCChannelToData(9, 1);
            MatchADCChannelToData(10, 2);
            MatchADCChannelToData(11, 3);
            break;
      case 3:
            MatchADCChannelToData(12, 0);
            MatchADCChannelToData(13, 1);
            MatchADCChannelToData(14, 2);
            break;
      }
      if (Chanal_8_mode)
      {
            if (Data_Select == 1)
            {
                  dprint("4-7 chanal has matched 0-3 buffer\n");
            }
            else
            {
                  MatchADCChannelToData(4, 4);
                  MatchADCChannelToData(5, 5);
                  MatchADCChannelToData(6, 6);
                  MatchADCChannelToData(7, 7);
            }
      }

      ADC_TRIGGER = 0x1;
}

/**
 * @brief ADC单通道软件连续触发采样初始化函数
 *
 * @param channelx 通道选择，取值范围为 0~11
 * @param ADC_Databuffer_channelx adc采样数据缓存通道选择，取值范围为 0~7
 * @param mode 单端或差分模式选择，0表示单端，1表示差分
 *
 * @return 无
 *
 * @note 连续触发时针对 1 个通道，间隔固定时间（20 个 adc_clk）采样。
 */
void ADC_Cont_Sample_Init_Single(uint8_t channelx, u_int8_t ADC_Databuffer_channelx, uint8_t mode)
{
      ADC_TriggerMode_Config(SW_SAMPLE);//先进入软件模式
      ADC_CTRL &= ~(0x2);//关闭中断end_of_adc_cycle_event
      ADC_CTRL |= (0x4);

      ADC_IRQ_Config(0x1 << ADC_Databuffer_channelx, ENABLE, DISABLE); // 使能ADC数据采集中断 

      ADC_SampleMode_Config(mode);// 单端or差分模式选择
      ADC_TriggerMode_Config(CONT_SAMPLE);
      ADC_CTRL |= 0x1; //使能adc模拟

      // //配置校准
      // ADC_CTRL &= ~(0x1<<9);  //x值为采样值
      // ADC_A_VALUE =0xe000;
      // ADC_B_VALUE =0;

      ADC_Channelx_Set(channelx, ENABLE);// 通道使能
      MatchADCChannelToData(channelx, ADC_Databuffer_channelx);

      ADC_TRIGGER = 0x1;
}

/**
 * @brief ADC多通道软件连续触发采样初始化函数
 *
 * @param mode 单端或差分模式选择，0表示单端，1表示差分
 * @param Data_Select 0:0-3 chananl -> 0-3buffer
 *                    1:4-7 chanal -> 0-3buffer
 *                    2:8->11 chanal -> 0-3buffer
 *                    3:12-14 chanal -> 0-2buffer
 * @param Chanal_8_mode 0：4通道 1：8通道
 * @return 无
 *
 * @note 连续触发时针对 1 个通道，间隔固定时间（20 个 adc_clk）采样。
 */
void ADC_Cont_Sample_Init_Multi(uint8_t mode, uint8_t Data_Select, uint8_t Chanal_8_mode)
{
      ADC_TriggerMode_Config(SW_SAMPLE);//先进入软件模式
      ADC_CTRL &= ~(0x2);//关闭中断end_of_adc_cycle_event
      ADC_CTRL |= (0x4);

      ADC_INTMASK = 0xff;

      ADC_SampleMode_Config(mode);// 单端or差分模式选择
      ADC_TriggerMode_Config(CONT_SAMPLE);
      ADC_CTRL |= 0x1; //使能adc模拟

      // //配置校准
      // ADC_CTRL &= ~(0x1<<9);  //x值为采样值
      // ADC_A_VALUE =0xe000;
      // ADC_B_VALUE =0;

      ADC_CHAN_EN = 0x7fff;

      switch (Data_Select)
      {
      case 0:
            MatchADCChannelToData(0, 0);
            MatchADCChannelToData(1, 1);
            MatchADCChannelToData(2, 2);
            MatchADCChannelToData(3, 3);
            break;
      case 1:
            MatchADCChannelToData(4, 0);
            MatchADCChannelToData(5, 1);
            MatchADCChannelToData(6, 2);
            MatchADCChannelToData(7, 3);
            break;
      case 2:
            MatchADCChannelToData(8, 0);
            MatchADCChannelToData(9, 1);
            MatchADCChannelToData(10, 2);
            MatchADCChannelToData(11, 3);
            break;
      case 3:
            MatchADCChannelToData(12, 0);
            MatchADCChannelToData(13, 1);
            MatchADCChannelToData(14, 2);
            break;
      }
      if (Chanal_8_mode)
      {
            if (Data_Select == 1)
            {
                  dprint("4-7 chanal has matched 0-3 buffer\n");
            }
            else
            {
                  MatchADCChannelToData(4, 4);
                  MatchADCChannelToData(5, 5);
                  MatchADCChannelToData(6, 6);
                  MatchADCChannelToData(7, 7);
            }
      }

      ADC_TRIGGER = 0x1;
}

/**
 * @brief ADC单通道硬件触发初始化函数
 *
 * @param channelx 通道选择，取值范围为 0~11
 * @param ADC_Databuffer_channelx adc采样数据缓存通道选择，取值范围为 0~7
 * @param mode 单端或差分模式选择，0:单端 1:差分
 * @param timercount 多少时间采样一次，单位为ms
 *
 * @return 无
 *
 * @note 无
 */

void ADC_HW_Sample_Init_Single(uint8_t channelx, u_int8_t ADC_Databuffer_channelx, uint8_t mode, uint32_t timercount)
{
      ADC_TriggerMode_Config(SW_SAMPLE);    //先进入软件模式
      ADC_CTRL &= ~0x1; //adc模拟
      ADC_CTRL &= ~(0x2);                       //关闭中断end_of_adc_cycle_event
      ADC_CTRL &= ~(0x4);
      // ADC_CTRL |= (0x4);
      ADC_SampleMode_Config(mode);                       // 0:单端 1:差分
      ADC_Cnt(timercount);                                //配置采样触发时间

      //单通道
      ADC_IRQ_Config(0x1 << ADC_Databuffer_channelx, ENABLE, DISABLE); // 使能ADC数据采集中断
      MatchADCChannelToData(channelx, ADC_Databuffer_channelx); //设置ADC通道采样的数据存入哪个数据缓存通道

      ADC_TriggerMode_Config(HW_SAMPLE);                  // 再进入硬件模式

      // 单通道使能
      ADC_CHAN_EN |= (0x1 << channelx);
      ADC_CTRL |= 0x1; //使能adc模拟
}

/**
 * @brief ADC多通道硬件触发初始化函数
 *
 * @param Data_Select 0:0-3 chananl -> 0-3buffer
 *                    1:4-7 chanal -> 0-3buffer
 *                    2:8->11 chanal -> 0-3buffer
 *                    3:12-14 chanal -> 0-2buffer
 * @param Chanal_8_mode 0：4通道 1：8通道
 * @param mode 单端或差分模式选择，0:单端 1:差分
 * @param timercount 多少时间采样一次，单位为ms
 *
 * @return 无
 *
 * @note 可以配置采样时间，对多个通道进行采样。
 */
void ADC_HW_Sample_Init_Multi(uint8_t mode, uint32_t timercount, uint8_t Data_Select, uint8_t Chanal_8_mode)
{
      ADC_TriggerMode_Config(SW_SAMPLE);       //先进入软件模式
      ADC_CTRL &= ~0x1; //adc模拟
      ADC_CTRL &= ~(0x2);                       //关闭中断end_of_adc_cycle_event
      ADC_CTRL |= (0x4);
      ADC_SampleMode_Config(mode);              // 0:单端 1:差分
      ADC_Cnt(timercount);                      //配置采样触发时间
      ADC_INTMASK = 0xff;

      switch (Data_Select)
      {
      case 0:
            MatchADCChannelToData(0, 0);
            MatchADCChannelToData(1, 1);
            MatchADCChannelToData(2, 2);
            MatchADCChannelToData(3, 3);
            break;
      case 1:
            MatchADCChannelToData(4, 0);
            MatchADCChannelToData(5, 1);
            MatchADCChannelToData(6, 2);
            MatchADCChannelToData(7, 3);
            break;
      case 2:
            MatchADCChannelToData(8, 0);
            MatchADCChannelToData(9, 1);
            MatchADCChannelToData(10, 2);
            MatchADCChannelToData(11, 3);
            break;
      case 3:
            MatchADCChannelToData(12, 0);
            MatchADCChannelToData(13, 1);
            MatchADCChannelToData(14, 2);
            break;
      }
      if (Chanal_8_mode)
      {
            if (Data_Select == 1)
            {
                  dprint("4-7 chanal has matched 0-3 buffer\n");
            }
            else
            {
                  MatchADCChannelToData(4, 4);
                  MatchADCChannelToData(5, 5);
                  MatchADCChannelToData(6, 6);
                  MatchADCChannelToData(7, 7);
            }
      }

      ADC_CTRL |= 0x1; //使能adc模拟
      ADC_TriggerMode_Config(HW_SAMPLE);                  // 再进入硬件模式

      // 多通道使能 
      ADC_CHAN_EN = 0xffff;
}

/**
 * @brief ADC单通道自动采样初始化函数
 *
 * @param channelx 通道选择，取值范围为 0~11
 * @param ADC_Databuffer_channelx adc采样数据缓存通道选择，取值范围为 0~7
 * @param cmp_channelx 比较器通道选择，取值范围为 -1~2 如果为-1，则不使用比较器且参数comp_rule与threshold无效
 * @param cmp_rule 如果配置了ADC_INTMASK寄存器中的比较中断，1:大于阈值触发中断，0:小于等于阈值值触发
 * @param threshold 触发值
 * @param timercount 定时器计数器值
 * @param mode 0:单端 1:差分
 *
 * @return 无
 *
 * @note 函数只在配置为硬件或autosample触发时，当 timercount 递减到 0 时才进行采样
 */
void ADC_Auto_Sample_Init_Single(uint8_t channelx, uint8_t ADC_Databuffer_channelx, char cmp_channelx, uint8_t cmp_rule, uint16_t threshold, uint32_t timercount, uint8_t mode)
{
      ADC_CTRL &= 0x000;                        //清除ADC_CTRL,先默认进入软件模式

      ADC_CTRL |= (1 << 0);                     //打开adc模拟
      ADC_SampleMode_Config(mode);              // 0:单端 1:差分
      ADC_CTRL |= (1 << 5);                     //scale打开
      ADC_TriggerMode_Config(AUTO_SAMPLE);

      // 多通道使能
      ADC_CHAN_EN |= (0x1 << channelx);
      MatchADCChannelToData(channelx, ADC_Databuffer_channelx);
      ADC_Cnt(timercount);                      //配置采样触发时间 
      ADC_INTMASK |= ((1 << channelx) | (1 << (cmp_channelx + 8)));

      if (cmp_channelx == -1)
      {
            ADC_CMP_0_CTRL &= ~(0x1 << 6);//关闭comparator 0 的使能位
            ADC_CMP_1_CTRL &= ~(0x1 << 6);//关闭comparator 1 的使能位
            ADC_CMP_2_CTRL &= ~(0x1 << 6);//关闭comparator 2 的使能位
      }
      else
      {
            cmp_channelx &= 0x3;
            switch (cmp_channelx)
            {
            case 0:
                  ADC_CMP_0_CTRL |= 0x1 << 6;//使能comparator 0
                  ADC_CMP_0_CTRL &= ~(0x1 << 5);
                  ADC_CMP_0_CTRL |= cmp_rule << 5;//设置比较规则
                  ADC_CMP_0_CTRL &= ~(0x7);
                  ADC_CMP_0_CTRL |= ADC_Databuffer_channelx;//设置比较通道对应的ADC数据缓存通道
                  ADC_CMP_0_THR = threshold;
                  break;
            case 1:
                  ADC_CMP_1_CTRL |= 0x1 << 6;//使能comparator 1
                  ADC_CMP_1_CTRL &= ~(0x1 << 5);
                  ADC_CMP_1_CTRL |= cmp_rule << 5;//设置比较规则
                  ADC_CMP_1_CTRL &= ~(0x7);
                  ADC_CMP_1_CTRL |= ADC_Databuffer_channelx;//设置比较通道对应的ADC数据缓存通道
                  ADC_CMP_1_THR = threshold;
                  break;
            case 2:
                  ADC_CMP_2_CTRL |= 0x1 << 6;//使能comparator 2
                  ADC_CMP_2_CTRL &= ~(0x1 << 5);
                  ADC_CMP_2_CTRL |= cmp_rule << 5;//设置比较规则
                  ADC_CMP_2_CTRL &= ~(0x7);
                  ADC_CMP_2_CTRL |= ADC_Databuffer_channelx;//设置比较通道对应的ADC数据缓存通道
                  ADC_CMP_2_THR = threshold;
                  break;
            default:
                  ADC_CMP_0_CTRL &= ~(0x1 << 6);//关闭comparator 0 的使能位
                  ADC_CMP_1_CTRL &= ~(0x1 << 6);//关闭comparator 1 的使能位
                  ADC_CMP_2_CTRL &= ~(0x1 << 6);//关闭comparator 2 的使能位
                  break;
            }
      }
}
/**
 * @brief ADC总体关闭函数
 *
 * @param 无
 *
 * @return 无
 *
 */
void ADC_Close(void)
{
      ADC_CTRL &= (~(0x3 << 7)); // 软件触发
}

/**
 * @brief 读取ADC采集到的值
 *
 * @param ADC_Databuffer_channelx:adc采样数据缓存通道选择，取值范围为 0~7
 *
 * @return 返回ADC采集到的短整型数据
 *
 * @note 此函数用于读取ADCdatabuffer中采集到的值。根据初始化中设置的ADC_Databuffer_channelx，读取对应的ADC通道采集到的数据。
 */
short ADC_ReadData(uint8_t ADC_Databuffer_channelx)
{
      short Data = 0;
      // Data = ADC_REG(ADC_DATA_0_OFFSET + ADC_Databuffer_channelx * 2);
      switch (ADC_Databuffer_channelx)
      {
      case 0:
            Data = ADC_DATA_0;
            break;
      case 1:
            Data = ADC_DATA_1;
            break;
      case 2:
            Data = ADC_DATA_2;
            break;
      case 3:
            Data = ADC_DATA_3;
            break;
      case 4:
            Data = ADC_DATA_4;
            break;
      case 5:
            Data = ADC_DATA_5;
            break;
      case 6:
            Data = ADC_DATA_6;
            break;
      case 7:
            Data = ADC_DATA_7;
            break;
      }
      return Data;
}
