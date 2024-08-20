/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-08 16:14:15
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
void ADC__TriggerMode_Config(uint8_t mode)
{
      switch(mode)
      {
            case 0:
                  ADC_CTRL &= (~(0x3 << 7)); // 软件触发
                  break;
            case 1:
                  ADC_CTRL |= (0x1 << 7);
                  ADC_CTRL &= (~(0x1 << 8)); // 硬件触发
                  break;
            case 2:
                  ADC_CTRL |= (0x1 << 8);
                  ADC_CTRL &= (~(0x1 << 7)); // 连续触发
                  break;
            case 3:
                  ADC_CTRL |= (0x3 << 7); // 自动采样
            default:
                  break;
      }
      printf("ADC_CTRL2:%x\n", ADC_CTRL);
}
/**
 * @brief 单端or差分模式选择
 *
 * @param mode 0差分  1单端
 *
 * @return             无
 */
void ADC_SampleMode_Config(uint8_t mode)
{
      switch(mode)
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
 * @return             无
 */
void ADC_Channelx_Config(uint8_t channelx, uint8_t sw)
{
      if((ADC_CTRL & 0x10) != 0) // 单端模式
      {
            if(sw == DISABLE)
            {
                  ADC_CHAN_EN &= (~(0x1 << channelx));
            }
            else
            {
                  if((((ADC_CTRL >> 7) & 0x3) == 0) || (((ADC_CTRL >> 7) & 0x3) == 0x2))//软件和连续触发只能使能一个通道
                        ADC_CHAN_EN = (0x1 << channelx);
                  else
                        ADC_CHAN_EN |= (0x1 << channelx);
                  if((((ADC_CTRL >> 7) & 0x3) == 0) || (((ADC_CTRL >> 7) & 0x3) == 0x2))//软件和连续触发只能使能一个通道
                        ADC_CHAN_EN = (0x1 << channelx);
                  else
                        ADC_CHAN_EN |= (0x1 << channelx);
            }
      }
      else
      {
            if(channelx >= 4)
            {
                  dprint("The maximum value of ADC channel parameters in differential mode is 3\n");
                  return;
            }
            if(sw == DISABLE)
            {
                  ADC_CHAN_EN &= (~(0x1 << channelx));
            }
            else
            {
                  if((((ADC_CTRL >> 7) & 0x3) == 0) || (((ADC_CTRL >> 7) & 0x3) == 0x2))//软件和连续触发只能使能一个通道
                        ADC_CHAN_EN = (0x1 << channelx);
                  else
                        ADC_CHAN_EN |= (0x1 << channelx);
            }
      }

}
/**
 * @brief ADC中断使能or屏蔽
 *
 * @param channelx irq_type：ADCData0_intrMask ADCData1_intrMask ADCData2_intrMask....
 * @param sw DISABLE  ENABLE
 *
 * @return             无
 */
void ADC_IRQ_Config(uint16_t irq_type, uint8_t sw)
{
      if(sw == ENABLE)
      {
            ADC_INTMASK |= (irq_type);
            ADC_INTMASK |= (irq_type << 8);
      }
      else
      {
            ADC_INTMASK &= (~(irq_type));
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
      if(ADC_INTSTAT & (0x1 << ADC_Databuffer_channelx))
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
 * @param channelx 通道选择，取值范围为 0~11
 * @param ADC_Databuffer_channelx adc采样数据缓存通道选择，取值范围为 0~7
 *
 * @return 成功返回1，失败返回0
 *
 * @note 无
 */

uint8_t MatchADCChannelToData(uint8_t channelx, uint16_t ADC_Databuffer_channelx)
{
      switch(ADC_Databuffer_channelx)
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
                  if(channelx == 4)
                        ADC_DATA_4567_CHAN |= (0x1 << 4);
                  else
                        dprint("To store in buffers 4, 5, and 6, only ADC channels 4, 5, and 6 are allowed\n");
                  return 0;
                  return 0;
            case 5:
                  if(channelx == 5)
                        ADC_DATA_4567_CHAN |= (0x1 << 5);
                  else
                        dprint("To store in buffers 4, 5, and 6, only ADC channels 4, 5, and 6 are allowed\n");
                  return 0;
                  return 0;
            case 6:
                  if(channelx == 6)
                        ADC_DATA_4567_CHAN |= (0x1 << 6);
                  else
                        dprint("To store in buffers 4, 5, and 6, only ADC channels 4, 5, and 6 are allowed\n");
                  return 0;
                  return 0;
            case 7:
                  if(channelx == 7)
                        ADC_DATA_4567_CHAN |= (0x1 << 7);
                  else
                        dprint("To store in buffers 4, 5, and 6, only ADC channels 4, 5, and 6 are allowed\n");
                  return 0;
                  return 0;
            default:
                  return 0;
      }
      return 1;
}

/**
 * @brief ADC自动采样初始化函数
 *
 * @param channelx 通道选择，取值范围为 0~11
 * @param ADC_Databuffer_channelx adc采样数据缓存通道选择，取值范围为 0~7
 * @param cmp_channelx 比较器通道选择，取值范围为 -1~3 如果为-1，则不使用比较器且参数comp_rule与threshold无效
 * @param comp_rule 如果配置了ADC_INTMASK寄存器中的比较中断，则该位为1大于阈值触发中断，0:小于等于阈值值则触发
 * @param threshold 触发值
 * @param timercount 定时器计数器值
 *
 * @return 无
 *
 * @note 函数只在配置为硬件触发时，当 timercount 递减到 0 时才进行采样
 */
// void ADC_Auto_Sample_Init(uint8_t channelx, uint8_t ADC_Databuffer_channelx, char cmp_channelx, uint8_t cmp_rule, uint16_t threshold, uint32_t timercount)
// {
//       ADC_Databuffer_channelx &= 0x7;
//       channelx &= 0xf;
//       cmp_rule = cmp_rule ? 1 : 0;
//       threshold &= 0x3ff;
//       ADC__TriggerMode_Config(0);    //先进入软件模式
//       ADC_CHAN_EN = 0;//所有通道关闭使能
//       MatchADCChannelToData(channelx, ADC_Databuffer_channelx); //设置ADC通道采样的数据存入哪个数据缓存通道
//       ADC_IRQ_Config(0x1 << ADC_Databuffer_channelx, ENABLE); // 使能ADC数据采集中断
//       if(cmp_channelx == -1)
//       {
//             CMP_0_CTRL &= !(0x1 << 6);//关闭comparator 0 的使能位
//             CMP_1_CTRL &= !(0x1 << 6);//关闭comparator 1 的使能位
//             CMP_2_CTRL &= !(0x1 << 6);//关闭comparator 2 的使能位
//       }
//       else
//       {
//             cmp_channelx &= 0x3;
//             switch(cmp_channelx)
//             {
//                   case 0:
//                         CMP_0_CTRL |= 0x1 << 6;//使能comparator 0
//                         CMP_0_CTRL &= ~(0x1 << 5);
//                         CMP_0_CTRL |= cmp_rule << 5;//设置比较规则
//                         CMP_0_CTRL &= ~(0x7);
//                         CMP_0_CTRL |= ADC_Databuffer_channelx;//设置比较通道对应的ADC数据缓存通道
//                         CMP_0_THR = threshold;
//                         break;
//                   case 1:
//                         CMP_1_CTRL |= 0x1 << 6;//使能comparator 1
//                         CMP_1_CTRL &= ~(0x1 << 5);
//                         CMP_1_CTRL |= cmp_rule << 5;//设置比较规则
//                         CMP_1_CTRL &= ~(0x7);
//                         CMP_1_CTRL |= ADC_Databuffer_channelx;//设置比较通道对应的ADC数据缓存通道
//                         CMP_1_THR = threshold;
//                         break;
//                   case 2:
//                         CMP_2_CTRL |= 0x1 << 6;//使能comparator 2
//                         CMP_2_CTRL &= ~(0x1 << 5);
//                         CMP_2_CTRL |= cmp_rule << 5;//设置比较规则
//                         CMP_2_CTRL &= ~(0x7);
//                         CMP_2_CTRL |= ADC_Databuffer_channelx;//设置比较通道对应的ADC数据缓存通道
//                         CMP_2_THR = threshold;
//                         break;
//                   default:
//                         break;
//             }
//       }
//       uint32_t count = (32000 * timercount) / 1000;
//       // 设置低16位到 ADC_CNT0
//       ADC_CNT0 = count & 0xFFFF;
//       // 设置高16位到 ADC_CN1
//       ADC_CNT1 = (count >> 16) & 0xFFFF;
//       ADC__TriggerMode_Config(3);              // 再进入自动采样模式
//       ADC_Channelx_Config(channelx, ENABLE);             // 通道使能
// }

/**
 * @brief ADC软件触发初始化函数
 *
 * @param channelx 通道选择，取值范围为 0~11
 * @param ADC_Databuffer_channelx adc采样数据缓存通道选择，取值范围为 0~7
 * @param attenuationMode 衰减 1/3 模式，0表示不衰减模式（0~1.2V），1表示衰减模式（0~3.6V）
 * @param mode 单端或差分模式选择，1表示单端，0表示差分
 *
 * @return 无
 *
 * @note CPU 触发一次读一个采样数据。软件触发每次只能使能 1 个通道。软件触发每次只能使能 1 个通道。

 */

void ADC_SW_Sample_Init(uint8_t channelx, uint8_t ADC_Databuffer_channelx, uint8_t attenuationMode, uint8_t mode)
{
      ADC_CTRL &= ~(0x2);//关闭中断end_of_adc_cycle_event
      ADC_CTRL |= (attenuationMode ? BIT5 : 0);      // 衰减模式选择
      ADC_PM = 0x0;                                      // 关闭低功耗
      ADC__TriggerMode_Config(SW_SAMPLE);                  // 触发模式选择
      ADC_SampleMode_Config(mode);                       // 单端or差分模式选择
      MatchADCChannelToData(channelx, ADC_Databuffer_channelx); //设置ADC通道采样的数据存入哪个数据缓存通道
      ADC_Channelx_Config(channelx, ENABLE);             // 通道使能
      ADC_SW_Sample(ADC_Databuffer_channelx);               //触发软件采样

}

/**
 * @brief ADC硬件触发初始化函数
 *
 * @param channelx 通道选择，取值范围为 0~11
 * @param ADC_Databuffer_channelx adc采样数据缓存通道选择，取值范围为 0~7
 * @param attenuationMode 衰减 1/3 模式，0表示不衰减模式（0~1.2V），1表示衰减模式（0~3.6V）
 * @param mode 单端或差分模式选择，1表示单端，0表示差分
 * @param timercount 多少时间采样一次，单位为ms
 *
 * @return 无
 *
 * @note 可以配置采样时间，对多个通道进行采样。
 */
void ADC_HW_Sample_Init(uint8_t channelx, u_int8_t ADC_Databuffer_channelx, uint8_t attenuationMode, uint8_t mode, uint32_t timercount)
{
      ADC__TriggerMode_Config(SW_SAMPLE);    //先进入软件模式
      ADC_CTRL &= ~(0x2);                       //关闭中断end_of_adc_cycle_event
      ADC_CTRL |= (attenuationMode ? BIT5 : 0);      // 衰减模式选择
      ADC_PM = 0x0;                                      // 关闭低功耗
      ADC__TriggerMode_Config(HW_SAMPLE);                  // 再进入硬件模式
      ADC_SampleMode_Config(mode);                       // 单端or差分模式选择
      ADC_Cnt(timercount);                                //配置采样触发时间
      MatchADCChannelToData(channelx, ADC_Databuffer_channelx); //设置ADC通道采样的数据存入哪个数据缓存通道
      ADC_IRQ_Config(0x1 << ADC_Databuffer_channelx, ENABLE); // 使能ADC数据采集中断
      ADC_Channelx_Config(channelx, ENABLE);             // 通道使能
}

/**
 * @brief ADC连续触发初始化函数
 *
 * @param channelx 通道选择，取值范围为 0~11
 * @param ADC_Databuffer_channelx adc采样数据缓存通道选择，取值范围为 0~7
 * @param attenuationMode 衰减 1/3 模式，0表示不衰减模式（0~1.2V），1表示衰减模式（0~3.6V）
 * @param mode 单端或差分模式选择，1表示单端，0表示差分
 *
 * @return 无
 *
 * @note 连续触发时针对 1 个通道，间隔固定时间（20 个 adc_clk）采样。
 */
void ADC_Cont_Sample_Init(uint8_t channelx, u_int8_t ADC_Databuffer_channelx, uint8_t attenuationMode, uint8_t mode)
{
      ADC__TriggerMode_Config(SW_SAMPLE);                     //先进入软件模式
      ADC_CTRL &= ~(0x2);                               //关闭中断end_of_adc_cycle_event
      ADC_CTRL |= (attenuationMode ? BIT5 : 0);       // 衰减模式选择
      ADC_PM = 0x0;                                   // 关闭低功耗
      ADC__TriggerMode_Config(CONT_SAMPLE);                     // 再进入连续模式
      ADC_SampleMode_Config(mode);                    // 单端or差分模式选择
      MatchADCChannelToData(channelx, ADC_Databuffer_channelx); //设置ADC通道采样的数据存入哪个数据缓存通道
      ADC_IRQ_Config(0x1 << ADC_Databuffer_channelx, ENABLE); // 使能ADC数据采集中断
      ADC_Channelx_Config(channelx, ENABLE);             // 通道使能
      ADC_TRIGGER = 0x1;                               // 触发进行连续采样
}
/**
 * @brief ADC自动采样初始化函数
 *
 * @param channelx 通道选择，取值范围为 0~11
 * @param ADC_Databuffer_channelx adc采样数据缓存通道选择，取值范围为 0~7
 * @param cmp_channelx 比较器通道选择，取值范围为 -1~3 如果为-1，则不使用比较器且参数comp_rule与threshold无效
 * @param cmp_rule 如果配置了ADC_INTMASK寄存器中的比较中断，则该位为1大于阈值触发中断，0:小于等于阈值值则触发
 * @param threshold 触发值
 * @param timercount 定时器计数器值
 *
 * @return 无
 *
 * @note 函数只在配置为硬件触发时，当 timercount 递减到 0 时才进行采样
 */
void ADC_Auto_Sample_Init(uint8_t channelx, uint8_t ADC_Databuffer_channelx, char cmp_channelx, uint8_t cmp_rule, uint16_t threshold, uint32_t timercount)
{
      ADC_Databuffer_channelx &= 0x7;
      channelx &= 0xf;
      cmp_rule = cmp_rule ? 1 : 0;
      threshold &= 0x3ff;
      ADC_PM = 0;                                       //关闭低功耗
      ADC__TriggerMode_Config(SW_SAMPLE);               //先进入软件模式
      ADC_CHAN_EN = 0;                                  //关闭所有通道
      ADC_SampleMode_Config(SINGLE_ENDED);              //单端or差分
      MatchADCChannelToData(channelx, ADC_Databuffer_channelx); //设置ADC通道采样的数据存入哪个数据缓存通道
      ADC_IRQ_Config(0x1 << ADC_Databuffer_channelx, ENABLE); // 使能ADC数据采集中断
      ADC_INTMASK = 0x0; //关闭所有中断
      if(cmp_channelx == -1)
      {
            CMP_0_CTRL &= !(0x1 << 6);//关闭comparator 0 的使能位
            CMP_1_CTRL &= !(0x1 << 6);//关闭comparator 1 的使能位
            CMP_2_CTRL &= !(0x1 << 6);//关闭comparator 2 的使能位
      }
      else
      {
            cmp_channelx &= 0x3;
            switch(cmp_channelx)
            {
                  case 0:
                        CMP_0_CTRL |= 0x1 << 6;//使能comparator 0
                        CMP_0_CTRL &= ~(0x1 << 5);
                        CMP_0_CTRL |= cmp_rule << 5;//设置比较规则
                        CMP_0_CTRL &= ~(0x7);
                        CMP_0_CTRL |= ADC_Databuffer_channelx;//设置比较通道对应的ADC数据缓存通道
                        CMP_0_THR = threshold;
                        break;
                  case 1:
                        CMP_1_CTRL |= 0x1 << 6;//使能comparator 1
                        CMP_1_CTRL &= ~(0x1 << 5);
                        CMP_1_CTRL |= cmp_rule << 5;//设置比较规则
                        CMP_1_CTRL &= ~(0x7);
                        CMP_1_CTRL |= ADC_Databuffer_channelx;//设置比较通道对应的ADC数据缓存通道
                        CMP_1_THR = threshold;
                        break;
                  case 2:
                        CMP_2_CTRL |= 0x1 << 6;//使能comparator 2
                        CMP_2_CTRL &= ~(0x1 << 5);
                        CMP_2_CTRL |= cmp_rule << 5;//设置比较规则
                        CMP_2_CTRL &= ~(0x7);
                        CMP_2_CTRL |= ADC_Databuffer_channelx;//设置比较通道对应的ADC数据缓存通道
                        CMP_2_THR = threshold;
                        break;
                  default:
                        break;
            }
      }
      ADC_Cnt(timercount);                                //配置采样触发时间  
      ADC__TriggerMode_Config(AUTO_SAMPLE);              // 再进入自动采样模式
      ADC_Channelx_Config(channelx, ENABLE);             // 通道使能
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
      ADC_PM = 0x7;
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
      Data = ADC_REG(ADC_DATA_0_OFFSET + ADC_Databuffer_channelx * 2);
      return Data;
}
/**
 * @brief 在软件模式下进行ADC采样
 *
 * @param ADC_Databuffer_channelx:adc采样数据缓存通道选择，取值范围为 0~7
 *
 * @return 返回ADC采集到的短整型数据
 *
 * @note 此函数用于在软件模式下触发ADC进行采样操作，并返回采集到的数据。
 */
short ADC_SW_Sample(uint8_t ADC_Databuffer_channelx)
{
      ADC_TRIGGER = 0x1;
      while(!CheckADCDataValidity(ADC_Databuffer_channelx));
      return ADC_ReadData(ADC_Databuffer_channelx);
}


/**
 * @brief ADC配置采样时间
 *
 * @param timercount 配置采样触发的时间（ms）
 */
void ADC_Cnt(uint32_t timercount)
{
      uint32_t count = (32000 * timercount) / 1000;
      // 设置低16位到 ADC_CNT0
      ADC_CNT0 = count & 0xFFFF;
      // 设置高16位到 ADC_CNT1
      ADC_CNT1 = (count >> 16) & 0xFFFF;
}

