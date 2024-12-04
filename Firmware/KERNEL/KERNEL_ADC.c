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
      //printf("ADC_CTRL2:%x\n", ADC_CTRL);
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
      // if((ADC_CTRL & 0x10) != 0) // 单端模式
      // printf(":%x\n",ADC_CTRL&0x10);
      if((ADC_CTRL & 0x10) == 0) // 单端模式
      {
            if(sw == DISABLE)
            {
                  ADC_CHAN_EN &= (~(0x1 << channelx));
            }
            else
            {
                  if((((ADC_CTRL >> 7) & 0x3) == 0) || (((ADC_CTRL >> 7) & 0x3) == 0x2))//软件和连续触发只能使能一个通道
                  {
                        ADC_CHAN_EN = (0x1 << channelx);
                        // printf("ADC_CHAN_EN:%x\n",ADC_CHAN_EN);
                  }  
                  else
                  {
                         ADC_CHAN_EN |= (0x1 << channelx);
                         printf("ADC_CHAN_EN:%x\n",ADC_CHAN_EN);
                  }
            }
      }
      else
      {
            if(channelx >= 7)
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
                  // if(channelx==3)
                  // {
                  //       SYSCTL_RST0 |=(0x1<<12);
                  //       SYSCTL_RST0 &=~(0x1<<12);
                  // }
                  if((((ADC_CTRL >> 7) & 0x3) == 0) || (((ADC_CTRL >> 7) & 0x3) == 0x2))//软件和连续触发只能使能一个通道
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
 * @brief ADC中断使能or屏蔽
 *
 * @param channelx irq_type：ADCData0_intrMask ADCData1_intrMask ADCData2_intrMask....
 * @param sw DISABLE  ENABLE
 *
 * @return             无
 */
#define compare 1
void ADC_IRQ_Config(uint16_t irq_type, uint8_t sw)
{
      if(sw == ENABLE)
      {
            ADC_INTMASK |= (irq_type);
            #if compare==1
            ADC_INTMASK |= (irq_type << 8);
            #endif
      }
      else
      {
            ADC_INTMASK &= ~(irq_type);
            #if compare==1
            ADC_INTMASK &= (~(irq_type << 8));
            #endif
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
      // printf("ADC_INTSTAT:%x\n",ADC_INTSTAT);
      if(ADC_INTSTAT & (0x1 << ADC_Databuffer_channelx))
      {
            printf("ADC_INTSTAT:%x\n",ADC_INTSTAT);
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

/*yujiang add*/

/**
 * @brief 使能data_overflow_event
 *
 * @param sw 使能：1 失能：0
 *
 * @note 开启data_overflow_event中断 0：开启 1：关闭
 */
void  DataOverflow_IRQ_Config(uint8_t sw)
{

      if(sw==ENABLE)
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

      if(sw==ENABLE)
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

      if(format==CompCode)
      {
            ADC_CTRL |=(0x1<<6);
      }
      else
      {
           ADC_CTRL &=~(0x1<<6);   
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
void  ADC_Calibration_Config(uint16_t A_VALUE,uint16_t B_VALUE)
{
      ADC_CTRL &= ~(0x1<<9);  //set  samp value as x
      A_value =A_VALUE;
      B_value =B_VALUE;
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
      if(sw==ENABLE)
      {
            ADC_CHAN_EN = (0x1 << channelx);
      }
}

/**
 * @brief 单通道切换通道函数
 *
 * @param channelx 通道
 * @param Data_Chan 设置buffer所对应通道
 *
 * @note 无
 */
uint8_t ADC_Swap_Channel(uint8_t channelx, uint8_t Data_Chan)
{
      ADC_CHAN_EN = (0x1 << channelx);
      // printf("ADC_CHAN_EN:%x\n",ADC_CHAN_EN);
      switch(Data_Chan)
      {
            case 0:
                  ADC_DATA_0_CHAN = channelx;
                  // printf("ADC_DATA_0_CHAN:%x\n",ADC_DATA_0_CHAN);
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
/*yujiang add*/


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
 * @brief ADC软件触发初始化函数-单通道
 *
 * @param channelx 通道选择，取值范围为 0~11
 * @param ADC_Databuffer_channelx adc采样数据缓存通道选择，取值范围为 0~7
 * @param attenuationMode 衰减 1/3 模式，0表示不衰减模式（0~1.2V），1表示衰减模式（0~3.6V）
 * @param mode 单端或差分模式选择，0表示单端，1表示差分
 *
 * @return 无
 *
 * @note CPU 触发一次读一个采样数据。软件触发每次只能使能 1 个通道。软件触发每次只能使能 1 个通道。

 */

void ADC_SW_Sample_Init_Single(uint8_t channelx, uint8_t ADC_Databuffer_channelx, uint8_t mode)
{     
//单通道
      ADC_IRQ_Config(0x1 << ADC_Databuffer_channelx, ENABLE);
      printf("ADC_INTMASK:%x\n",ADC_INTMASK);

      ADC_CTRL |=0x1; //使能adc模拟

      // ADC_CTRL &= ~(0x2);
      ADC_CycleEvent_IRQ_Config(DISABLE);//关闭中断end_of_adc_cycle_event

      //测试data_overflow_event 0：失能 1：失能
      // ADC_CTRL &= ~(0x4);
      // ADC_CTRL |= (0x4);
      DataOverflow_IRQ_Config(DISABLE);//关闭中断data_overflow_event

      ADC__TriggerMode_Config(SW_SAMPLE);// 触发模式选择
      ADC_SampleMode_Config(mode);// 单端or差分模式选择

      //配置补码
      // ADC_CTRL |=(0x1<<6);
      ADC_Format_Config(OriginCode);//设置为原码

      //配置校准
      ADC_Calibration_Config(0x8000,0);//设置a=0x8000,b=0
      // // ADC_CTRL &= ~(0x1<<9);  //x值为采样值
      // // A_value =0xe000;
      // // B_value =0;
      // // printf("0x4820:%x\n",*(unsigned short *)0x4820);
      // // printf("0x4822:%x\n",*(unsigned short *)0x4822);
      // // printf("ADC_CTRL:%x\n",ADC_CTRL);

      ADC_Channelx_Set(channelx, ENABLE);// 通道使能
      printf("ADC_CHAN_EN:%x\n",ADC_CHAN_EN);

      MatchADCChannelToData(channelx, ADC_Databuffer_channelx); //设置ADC通道采样的数据存入哪个数据缓存通道

      printf("ADC_TRIGGER_0:%x\n",ADC_TRIGGER);
      // // printf("ADC Data:%x\n", ADC_SW_Sample(ADC_Databuffer_channelx));
      // ADC_TRIGGER = 0x1;
      ADC_Trigger();
      printf("ADC_TRIGGER_1:%x\n",ADC_TRIGGER);
}


/**
 * @brief ADC软件触发初始化函数-多通道
 *
 * @param ADC_Databuffer_channelx adc采样数据缓存通道选择，取值范围为 0~7
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

void ADC_SW_Sample_Init_Multi(uint8_t mode,uint8_t Data_Select,uint8_t Chanal_8_mode)
{ 
//多通道
      ADC_CTRL |=0x1; //使能adc模拟

      ADC_CycleEvent_IRQ_Config(DISABLE);   //关闭cycle中断
      DataOverflow_IRQ_Config(DISABLE);//关闭中断data_overflow_event

      ADC__TriggerMode_Config(SW_SAMPLE);                  // 触发模式选择
      ADC_SampleMode_Config(mode);                       // 单端or差分模式选择
      // printf("ADC_CTRL:%x\n",ADC_CTRL);

      ADC_INTMASK =0x7ff;
      // printf("ADC_INTMASK:%x\n",ADC_INTMASK);

      ADC_CHAN_EN =0xffff;
      // printf("ADC_CHAN_EN:%x\n",ADC_CHAN_EN);

      switch(Data_Select)
      {
            case 0:
                  MatchADCChannelToData(0,0);
                  MatchADCChannelToData(1,1);
                  MatchADCChannelToData(2,2);
                  MatchADCChannelToData(3,3);
                  break;
             case 1:
                  MatchADCChannelToData(4,0);
                  MatchADCChannelToData(5,1);
                  MatchADCChannelToData(6,2);
                  MatchADCChannelToData(7,3);
                  break;
            case 2:
                  MatchADCChannelToData(8,0);
                  MatchADCChannelToData(9,1);
                  MatchADCChannelToData(10,2);
                  MatchADCChannelToData(11,3);
                  break;
            case 3:
                  MatchADCChannelToData(12,0);
                  MatchADCChannelToData(13,1);
                  MatchADCChannelToData(14,2);
                  break;           
      }
      if(Chanal_8_mode)
      {
            if(Data_Select==1)
            {
                  printf("4-7 chanal has matched 0-3 buffer\n");
            }
            else
            {
                  MatchADCChannelToData(4,4);
                  MatchADCChannelToData(5,5);
                  MatchADCChannelToData(6,6);
                  MatchADCChannelToData(7,7);
            }
      }
      // printf("ADC_TRIGGER_1:%x\n",ADC_TRIGGER);
      ADC_TRIGGER = 0x1;
      // printf("ADC_TRIGGER_1:%x\n",ADC_TRIGGER);
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

void ADC_HW_Sample_Init_Single(uint8_t channelx, u_int8_t ADC_Databuffer_channelx, uint8_t mode, uint32_t timercount,char cmp_channelx,uint8_t cmp_rule,uint16_t cmp_thr)
{
      cmp_rule = cmp_rule ? 1 : 0;

      ADC__TriggerMode_Config(SW_SAMPLE);    //先进入软件模式
      ADC_CTRL &=~0x1; //adc模拟
      ADC_CTRL &= ~(0x2);                       //关闭中断end_of_adc_cycle_event
      ADC_CTRL |= (0x4);
      ADC_SampleMode_Config(mode);                       // 0:单端 1:差分
      // printf("ADC_CTRL:%x",ADC_CTRL);
      ADC_Cnt(timercount);                                //配置采样触发时间

      //测试最大值
      // ADC_CNT0 =0xffff;
      // ADC_CNT1=0x1;

      // ADC_CTRL &= ~(0x1<<9);  //x值为采样值
      // printf("ADC_CTRL:%x\n",ADC_CTRL);
      // A_value =0xe000;
      // printf("A_value:%x\n",A_value);
      // B_value =0x100;
      // printf("B_value:%x\n",B_value);

      //单通道
      ADC_IRQ_Config(0x1 << ADC_Databuffer_channelx, ENABLE); // 使能ADC数据采集中断
      //多通道

      // ADC_IRQ_Config(0x1 << 0, ENABLE); // 使能ADC数据采集中断
      // ADC_IRQ_Config(0x1 << 1, ENABLE); // 使能ADC数据采集中断
      // ADC_IRQ_Config(0x1 << 2, ENABLE); // 使能ADC数据采集中断
      // ADC_IRQ_Config(0x1 << 3, ENABLE); // 使能ADC数据采集中断
      // ADC_IRQ_Config(0x1 << 4, ENABLE); // 使能ADC数据采集中断
      // ADC_IRQ_Config(0x1 << 5, ENABLE); // 使能ADC数据采集中断
      // ADC_IRQ_Config(0x1 << 6, ENABLE); // 使能ADC数据采集中断
      // ADC_IRQ_Config(0x1 << 7, ENABLE); // 使能ADC数据采集中断
      // ADC_INTMASK =0xff;
      // printf("ADC_INTMASK:%x\n",ADC_INTMASK);

      // CMP_0_THR = 0xfff;
      // printf("CMP_0_THR:%x\n",CMP_0_THR);
      // CMP_1_THR = 0;
      // printf("CMP_1_THR:%x\n",CMP_1_THR);
      // CMP_2_THR = 0;
      // printf("CMP_2_THR:%x\n",CMP_2_THR);

      //单通道
      MatchADCChannelToData(channelx, ADC_Databuffer_channelx); //设置ADC通道采样的数据存入哪个数据缓存通道
      // 多通道使能

      //切换多通道_1 0-0 ，7-7
      // MatchADCChannelToData(0, 0);
      // MatchADCChannelToData(1, 1);
      // MatchADCChannelToData(2, 2);
      // MatchADCChannelToData(3, 3);
      // MatchADCChannelToData(4, 4);
      // MatchADCChannelToData(5, 5);
      // MatchADCChannelToData(6, 6);
      // MatchADCChannelToData(7, 7);

      //chan->buffer
      // MatchADCChannelToData(8, 0);  //8->0
      // MatchADCChannelToData(9, 1);  //9->1
      // MatchADCChannelToData(10, 2); //10->2
      // MatchADCChannelToData(11, 3); //11->3
      
      //chan->buffer
      // MatchADCChannelToData(12, 0); //12->0
      // MatchADCChannelToData(13, 1); //13->1   
      // MatchADCChannelToData(14, 2); //14->2  

      // printf("ADC_DATA_0_CHAN:%x\n",ADC_DATA_0_CHAN);
      // printf("ADC_DATA_1_CHAN:%x\n",ADC_DATA_1_CHAN);
      // printf("ADC_DATA_2_CHAN:%x\n",ADC_DATA_2_CHAN);
      // printf("ADC_DATA_3_CHAN:%x\n",ADC_DATA_3_CHAN);
      // printf("ADC_DATA_4567_CHAN:%x\n",ADC_DATA_4567_CHAN);

      ADC_CTRL |=0x1; //使能adc模拟
      // ADC__TriggerMode_Config(HW_SAMPLE);                  // 再进入硬件模式
      ADC__TriggerMode_Config(AUTO_SAMPLE);
      // printf("ADC_CTRL:%x\n",ADC_CTRL);

      // 单通道使能
      ADC_CHAN_EN = (0x1 << channelx);
      // 多通道使能 

      //切换多通道

      // ADC_CHAN_EN =0xffff;
      // ADC_CHAN_EN =0xff;
      // printf("ADC_CHAN_EN:%x\n",ADC_CHAN_EN);
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
                        CMP_0_THR =cmp_thr;
                        // printf("CMP_0_THR:%x\n",CMP_0_THR);
                        // printf("CMP_0_CTRL:%x\n",CMP_0_CTRL);
                        break;
                  case 1:
                        CMP_1_CTRL |= 0x1 << 6;//使能comparator 1
                        CMP_1_CTRL &= ~(0x1 << 5);
                        CMP_1_CTRL |= cmp_rule << 5;//设置比较规则
                        CMP_1_CTRL &= ~(0x7);
                        CMP_1_CTRL |= ADC_Databuffer_channelx;//设置比较通道对应的ADC数据缓存通道
                        CMP_1_THR =cmp_thr;
                        printf("CMP_1_THR:%x\n",CMP_1_THR);
                        printf("CMP_1_CTRL:%x\n",CMP_1_CTRL);
                        break;
                  case 2:
                        CMP_2_CTRL |= 0x1 << 6;//使能comparator 2
                        CMP_2_CTRL &= ~(0x1 << 5);
                        CMP_2_CTRL |= cmp_rule << 5;//设置比较规则
                        CMP_2_CTRL &= ~(0x7);
                        CMP_2_CTRL |= ADC_Databuffer_channelx;//设置比较通道对应的ADC数据缓存通道
                        CMP_2_THR = cmp_thr;
                        printf("CMP_2_THR:%x\n",CMP_2_THR);
                        printf("CMP_2_CTRL:%x\n",CMP_2_CTRL);
                        break;
                  default:
                        break;
            }
      }
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
void ADC_HW_Sample_Init_Multi(uint8_t mode, uint32_t timercount,uint8_t Data_Select,uint8_t Chanal_8_mode)
{
      ADC__TriggerMode_Config(SW_SAMPLE);    //先进入软件模式
      ADC_CTRL &=~0x1; //adc模拟
      ADC_CTRL &= ~(0x2);                       //关闭中断end_of_adc_cycle_event
      ADC_CTRL |= (0x4);
      ADC_SampleMode_Config(mode);                       // 0:单端 1:差分
      printf("ADC_CTRL:%x",ADC_CTRL);
      ADC_Cnt(timercount);                                //配置采样触发时间

      //测试最大值
      // ADC_CNT0 =0xffff;
      // ADC_CNT1=0x1;

      // ADC_CTRL &= ~(0x1<<9);  //x值为采样值
      // printf("ADC_CTRL:%x\n",ADC_CTRL);
      // A_value =0xe000;
      // printf("A_value:%x\n",A_value);
      // B_value =0x100;
      // printf("B_value:%x\n",B_value);


      //多通道

      // ADC_INTMASK =0xff;  
      ADC_INTMASK =0x7ff; 
      //使能比较和采样中断
      printf("ADC_INTMASK:%x\n",ADC_INTMASK);

      switch(Data_Select)
      {
            case 0:
                  MatchADCChannelToData(0,0);
                  MatchADCChannelToData(1,1);
                  MatchADCChannelToData(2,2);
                  MatchADCChannelToData(3,3);
                  break;
             case 1:
                  MatchADCChannelToData(4,0);
                  MatchADCChannelToData(5,1);
                  MatchADCChannelToData(6,2);
                  MatchADCChannelToData(7,3);
                  break;
            case 2:
                  MatchADCChannelToData(8,0);
                  MatchADCChannelToData(9,1);
                  MatchADCChannelToData(10,2);
                  MatchADCChannelToData(11,3);
                  break;
            case 3:
                  MatchADCChannelToData(12,0);
                  MatchADCChannelToData(13,1);
                  MatchADCChannelToData(14,2);
                  break;           
      }
      if(Chanal_8_mode)
      {
            if(Data_Select==1)
            {
                  printf("4-7 chanal has matched 0-3 buffer\n");
            }
            else
            {
                  MatchADCChannelToData(4,4);
                  MatchADCChannelToData(5,5);
                  MatchADCChannelToData(6,6);
                  MatchADCChannelToData(7,7);
            }
      }

      ADC_CTRL |=0x1; //使能adc模拟
      // ADC__TriggerMode_Config(HW_SAMPLE);                  // 再进入硬件模式
      ADC__TriggerMode_Config(AUTO_SAMPLE);
      printf("ADC_CTRL:%x\n",ADC_CTRL);

      // 多通道使能 
      ADC_CHAN_EN =0xffff;
      printf("ADC_CHAN_EN:%x\n",ADC_CHAN_EN);

      // if(group == -1)
      // {
      //       CMP_0_CTRL &= !(0x1 << 6);//关闭comparator 0 的使能位
      //       CMP_1_CTRL &= !(0x1 << 6);//关闭comparator 1 的使能位
      //       CMP_2_CTRL &= !(0x1 << 6);//关闭comparator 2 的使能位
      // }
      // else
      // {
      //       CMP_0_CTRL |= 0x1 << 6;//使能comparator 0
      //       CMP_0_CTRL &= ~(0x1 << 5);
      //       CMP_0_CTRL |= (cmp_rule << 5);
      //       CMP_0_CTRL &= ~(0x7);
      //       CMP_0_CTRL |= group;//设置比较通道对应的ADC数据缓存通道
      //       CMP_0_THR = cmp_thr;  //设置阈值
      //       printf("CMP_0_CTRL:%x\n",CMP_0_CTRL);
      //       printf("CMP_0_THR:%x\n",CMP_0_THR);

      //       CMP_1_CTRL |= 0x1 << 6;//使能comparator 1
      //       CMP_1_CTRL &= ~(0x1 << 5);
      //       CMP_1_CTRL |= (cmp_rule << 5);//设置比较规则
      //       CMP_1_CTRL &= ~(0x7);
      //       CMP_1_CTRL |= group+1;//设置比较通道对应的ADC数据缓存通道
      //       CMP_1_THR = cmp_thr;  //设置阈值
      //       printf("CMP_1_CTRL:%x\n",CMP_1_CTRL);
      //       printf("CMP_1_THR:%x\n",CMP_1_THR);

      //       CMP_2_CTRL |= 0x1 << 6;//使能comparator 2
      //       CMP_2_CTRL &= ~(0x1 << 5);
      //       CMP_2_CTRL |= cmp_rule << 5;//设置比较规则
      //       CMP_2_CTRL &= ~(0x7);
      //       CMP_2_CTRL |= group+2;//设置比较通道对应的ADC数据缓存通道
      //       CMP_2_THR = cmp_thr;  //设置阈值
      //       printf("CMP_2_CTRL:%x\n",CMP_2_CTRL);
      //       printf("CMP_2_THR:%x\n",CMP_2_THR);
      // }

      // if(group == -1)
      // {
      //       CMP_0_CTRL &= !(0x1 << 6);//关闭comparator 0 的使能位
      //       CMP_1_CTRL &= !(0x1 << 6);//关闭comparator 1 的使能位
      //       CMP_2_CTRL &= !(0x1 << 6);//关闭comparator 2 的使能位
      // }
      // else
      // {
      //       group &= 0x3;
      //       switch(group)
      //       {
      //             case 0:
      //                   CMP_0_CTRL |= 0x1 << 6;//使能comparator 0
      //                   CMP_0_CTRL &= ~(0x1 << 5);
      //                   CMP_0_CTRL |= cmp_rule << 5;//设置比较规则
      //                   CMP_0_CTRL &= ~(0x7);
      //                   CMP_0_CTRL |= group;//设置比较通道对应的ADC数据缓存通道
      //                   CMP_0_THR =cmp_thr;
      //                   printf("CMP_0_THR:%x\n",CMP_0_THR);
      //                   printf("CMP_0_CTRL:%x\n",CMP_0_CTRL);
      //                   break;
      //             case 1:
      //                   CMP_1_CTRL |= 0x1 << 6;//使能comparator 1
      //                   CMP_1_CTRL &= ~(0x1 << 5);
      //                   CMP_1_CTRL |= cmp_rule << 5;//设置比较规则
      //                   CMP_1_CTRL &= ~(0x7);
      //                   CMP_1_CTRL |= group;//设置比较通道对应的ADC数据缓存通道
      //                   CMP_1_THR =cmp_thr;
      //                   printf("CMP_1_THR:%x\n",CMP_1_THR);
      //                   printf("CMP_1_CTRL:%x\n",CMP_1_CTRL);
      //                   break;
      //             case 2:
      //                   CMP_2_CTRL |= 0x1 << 6;//使能comparator 2
      //                   CMP_2_CTRL &= ~(0x1 << 5);
      //                   CMP_2_CTRL |= cmp_rule << 5;//设置比较规则
      //                   CMP_2_CTRL &= ~(0x7);
      //                   CMP_2_CTRL |= group;//设置比较通道对应的ADC数据缓存通道
      //                   CMP_2_THR = cmp_thr;
      //                   printf("CMP_2_THR:%x\n",CMP_2_THR);
      //                   printf("CMP_2_CTRL:%x\n",CMP_2_CTRL);
      //                   break;
      //             default:
      //                   break;
      //       }
      // }
      
}
//一次性测试多通道
// uint8_t MatchADCChannelsToData(uint8_t channelxs, uint16_t ADC_Databuffer_channelx)
// {

// }
// void ADC_HW_Samples_Init(uint8_t channelxs, u_int8_t ADC_Databuffer_channelx, uint8_t attenuationMode, uint8_t mode, uint32_t timercount)
// {
//       ADC__TriggerMode_Config(SW_SAMPLE);    //先进入软件模式
//       ADC_CTRL &= ~(0x2);                       //关闭中断end_of_adc_cycle_event

//       ADC_CTRL |=0x1; //使能adc模拟
//       ADC__TriggerMode_Config(HW_SAMPLE);                  // 再进入硬件模式
//       ADC_SampleMode_Config(mode);                       // 单端or差分模式选择
//       printf("ADC_CTRL:%x\n",ADC_CTRL);
//       ADC_Cnt(timercount);                                //配置采样触发时间
//       // ADC_CNT0 =20;
//       // ADC_CNT1=0;
//       // MatchADCChannelToData(channelx, ADC_Databuffer_channelx); //设置ADC通道采样的数据存入哪个数据缓存通道
//       MatchADCChanneslToData(channelxs, ADC_Databuffer_channelx); //设置多通道

//       printf("ADC_DATA_0_CHAN:%x\n",ADC_DATA_0_CHAN);
//       // ADC_IRQ_Config(0x1 << ADC_Databuffer_channelx, ENABLE); // 使能ADC数据采集中断
//       printf("ADC_INTMASK:%x\n",ADC_INTMASK);
//       // ADC_Channelx_Config(channelx, ENABLE);             // 通道使能
//       printf("ADC_CHAN_EN:%x\n",ADC_CHAN_EN);
// }


/**
 * @brief ADC连续触发初始化函数
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
      // *(unsigned short*)0x4850 |=(0x1<<12);
      // printf("0x4850:%x\n",*(unsigned short*)0x4850);

      ADC__TriggerMode_Config(SW_SAMPLE);//先进入软件模式
      ADC_CTRL &= ~(0x2);//关闭中断end_of_adc_cycle_event
      ADC_CTRL |= (0x4);

      ADC_IRQ_Config(0x1 << ADC_Databuffer_channelx, ENABLE); // 使能ADC数据采集中断 

      ADC_SampleMode_Config(mode);// 单端or差分模式选择
      ADC__TriggerMode_Config(CONT_SAMPLE);
      ADC_CTRL |=0x1; //使能adc模拟

      // //配置校准
      // ADC_CTRL &= ~(0x1<<9);  //x值为采样值
      // A_value =0xe000;
      // B_value =0;

      // printf("0x4820:%x\n",*(unsigned short *)0x4820);
      // printf("0x4822:%x\n",*(unsigned short *)0x4822);
      printf("ADC_CTRL:%x\n",ADC_CTRL);

      ADC_Channelx_Set(channelx, ENABLE);// 通道使能
      printf("ADC_CHAN_EN:%x\n",ADC_CHAN_EN);

      MatchADCChannelToData(channelx, ADC_Databuffer_channelx); 

      printf("ADC_INTMASK:%x\n",ADC_INTMASK);    
      ADC_TRIGGER = 0x1;
}

/**
 * @brief ADC连续触发初始化函数
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
void ADC_Cont_Sample_Init_Multi(uint8_t mode,uint8_t Data_Select,uint8_t Chanal_8_mode)
{
      // *(unsigned short*)0x4850 |=(0x1<<12);
      // printf("0x4850:%x\n",*(unsigned short*)0x4850);

      ADC__TriggerMode_Config(SW_SAMPLE);//先进入软件模式
      ADC_CTRL &= ~(0x2);//关闭中断end_of_adc_cycle_event
      ADC_CTRL |= (0x4);

      ADC_INTMASK =0x7ff;
      printf("ADC_INTMASK:%x\n",ADC_INTMASK);

      ADC_SampleMode_Config(mode);// 单端or差分模式选择
      ADC__TriggerMode_Config(CONT_SAMPLE);
      ADC_CTRL |=0x1; //使能adc模拟

      // //配置校准
      // ADC_CTRL &= ~(0x1<<9);  //x值为采样值
      // A_value =0xe000;
      // B_value =0;

      // printf("0x4820:%x\n",*(unsigned short *)0x4820);
      // printf("0x4822:%x\n",*(unsigned short *)0x4822);
      printf("ADC_CTRL:%x\n",ADC_CTRL);

      ADC_CHAN_EN=0x7fff;
      printf("ADC_CHAN_EN:%x\n",ADC_CHAN_EN);
      
      switch(Data_Select)
      {
            case 0:
                  MatchADCChannelToData(0,0);
                  MatchADCChannelToData(1,1);
                  MatchADCChannelToData(2,2);
                  MatchADCChannelToData(3,3);
                  break;
             case 1:
                  MatchADCChannelToData(4,0);
                  MatchADCChannelToData(5,1);
                  MatchADCChannelToData(6,2);
                  MatchADCChannelToData(7,3);
                  break;
            case 2:
                  MatchADCChannelToData(8,0);
                  MatchADCChannelToData(9,1);
                  MatchADCChannelToData(10,2);
                  MatchADCChannelToData(11,3);
                  break;
            case 3:
                  MatchADCChannelToData(12,0);
                  MatchADCChannelToData(13,1);
                  MatchADCChannelToData(14,2);
                  break;           
      }
      if(Chanal_8_mode)
      {
            if(Data_Select==1)
            {
                  printf("4-7 chanal has matched 0-3 buffer\n");
            }
            else
            {
                  MatchADCChannelToData(4,4);
                  MatchADCChannelToData(5,5);
                  MatchADCChannelToData(6,6);
                  MatchADCChannelToData(7,7);
            }
      }
      printf("ADC_INTMASK:%x\n",ADC_INTMASK);    
      ADC_TRIGGER = 0x1;
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
void ADC_Auto_Sample_Init(uint8_t channelx, uint8_t ADC_Databuffer_channelx, char cmp_channelx, uint8_t cmp_rule, uint16_t threshold, uint32_t timercount,uint8_t mode)
{
      // ADC_Databuffer_channelx &= 0x7;
      // channelx &= 0xf;
      // cmp_rule = cmp_rule ? 1 : 0;
      //threshold &= 0x3ff;
      // ADC_PM = 0;                                       //关闭低功耗
      ADC_CTRL &= ~(0x2);                               //关闭中断end_of_adc_cycle_event
      ADC_CTRL |= (0x4);
      ADC__TriggerMode_Config(SW_SAMPLE);               //先进入软件模式
      // ADC_CHAN_EN = 0;                                  //关闭所有通道
      ADC_CTRL &=~0x1; //关闭adc模拟
      ADC_SampleMode_Config(mode);              //单端or差分

      // 单通道使能
      // ADC_Channelx_Config(channelx, ENABLE);            

      // 多通道使能
      ADC_CHAN_EN |= (0x1 << 0);
      ADC_CHAN_EN |= (0x1 << 1);
      ADC_CHAN_EN |= (0x1 << 2);
      ADC_CHAN_EN |= (0x1 << 3);
      ADC_CHAN_EN |= (0x1 << 4);
      ADC_CHAN_EN |= (0x1 << 5);
      ADC_CHAN_EN |= (0x1 << 6);
      ADC_CHAN_EN |= (0x1 << 7);
      printf("ADC_CHAN_EN:%x\n",ADC_CHAN_EN);

      //设置ADC通道采样的数据存入哪个数据缓存通道
      // MatchADCChannelToData(channelx, ADC_Databuffer_channelx); 

      MatchADCChannelToData(0, 0); 
      MatchADCChannelToData(1, 1); 
      MatchADCChannelToData(2, 2);
      MatchADCChannelToData(3, 3);
      MatchADCChannelToData(4, 4);
      MatchADCChannelToData(5, 5);
      MatchADCChannelToData(6, 6);
      MatchADCChannelToData(7, 7);
      printf("ADC_DATA_0_CHAN:%x\n",ADC_DATA_0_CHAN);
      printf("ADC_DATA_1_CHAN:%x\n",ADC_DATA_1_CHAN);
      printf("ADC_DATA_2_CHAN:%x\n",ADC_DATA_2_CHAN);
      printf("ADC_DATA_3_CHAN:%x\n",ADC_DATA_3_CHAN);
      printf("ADC_DATA_4567_CHAN:%x\n",ADC_DATA_4567_CHAN);

      // ADC_IRQ_Config(0x1 << ADC_Databuffer_channelx, ENABLE); // 使能ADC数据采集中断

      // ADC_IRQ_Config(0x1 << 0, ENABLE); // 使能ADC数据采集中断
      // ADC_IRQ_Config(0x1 << 1, ENABLE); // 使能ADC数据采集中断
      // ADC_IRQ_Config(0x1 << 2, ENABLE); // 使能ADC数据采集中断
      // ADC_IRQ_Config(0x1 << 3, ENABLE); // 使能ADC数据采集中断
      // ADC_IRQ_Config(0x1 << 4, ENABLE); // 使能ADC数据采集中断
      // ADC_IRQ_Config(0x1 << 5, ENABLE); // 使能ADC数据采集中断
      // ADC_IRQ_Config(0x1 << 6, ENABLE); // 使能ADC数据采集中断
      // ADC_IRQ_Config(0x1 << 7, ENABLE); // 使能ADC数据采集中断

      ADC_INTMASK =0x700;
      printf("ADC_INTMASK:%x\n",ADC_INTMASK);
      //单buffer对比

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
                        CMP_0_CTRL |= 1 << 5;//设置比较规则
                        CMP_0_CTRL &= ~(0x7);
                        CMP_0_CTRL |= 1;//设置比较通道对应的ADC数据缓存通道
                        CMP_0_THR = 0x0;
                        printf("CMP_0_CTRL:%x\n",CMP_0_CTRL);
                        break;
                  case 1:
                        CMP_1_CTRL |= 0x1 << 6;//使能comparator 1
                        CMP_1_CTRL &= ~(0x1 << 5);
                        CMP_1_CTRL |= cmp_rule << 5;//设置比较规则
                        CMP_1_CTRL &= ~(0x7);
                        CMP_1_CTRL |= ADC_Databuffer_channelx;//设置比较通道对应的ADC数据缓存通道
                        CMP_1_THR = threshold;
                        printf("CMP_1_THR:%x\n",CMP_1_THR);
                        break;
                  case 2:
                        CMP_2_CTRL |= 0x1 << 6;//使能comparator 2
                        CMP_2_CTRL &= ~(0x1 << 5);
                        CMP_2_CTRL |= cmp_rule << 5;//设置比较规则
                        CMP_2_CTRL &= ~(0x7);
                        CMP_2_CTRL |= ADC_Databuffer_channelx;//设置比较通道对应的ADC数据缓存通道
                        CMP_2_THR = threshold;
                        printf("CMP_2_THR:%x\n",CMP_2_THR);
                        break;
                  default:
                        break;
            }
      }
      //多buffer对比
      // CMP_0_CTRL |= 0x1 << 6;//使能comparator 0
      // CMP_0_CTRL &= ~(0x1 << 5);
      // CMP_0_CTRL |= cmp_rule << 5;//设置比较规则
      // CMP_0_CTRL &= ~(0x7);
      // CMP_0_CTRL |= 0+channelx;//设置比较通道对应的ADC数据缓存通道
      // CMP_0_THR = threshold;
      // printf("CMP_0_CTRL:%x\n",CMP_0_CTRL);

      // CMP_1_CTRL |= 0x1 << 6;//使能comparator 1
      // CMP_1_CTRL &= ~(0x1 << 5);
      // CMP_1_CTRL |= cmp_rule << 5;//设置比较规则
      // CMP_1_CTRL &= ~(0x7);
      // CMP_1_CTRL |= 1+channelx;//设置比较通道对应的ADC数据缓存通道
      // CMP_1_THR = threshold;
      // printf("CMP_1_THR:%x\n",CMP_1_THR);

      // CMP_2_CTRL |= 0x1 << 6;//使能comparator 2
      // CMP_2_CTRL &= ~(0x1 << 5);
      // CMP_2_CTRL |= cmp_rule << 5;//设置比较规则
      // CMP_2_CTRL &= ~(0x7);
      // CMP_2_CTRL |= 2+channelx;//设置比较通道对应的ADC数据缓存通道
      // CMP_2_THR = threshold;
      // printf("CMP_2_THR:%x\n",CMP_2_THR);


      ADC_Cnt(timercount);                                //配置采样触发时间 
      // ADC_CNT0 =0xffff;
      // ADC_CNT1 =0xffff;

      ADC_CTRL |=0x1; //adc模拟
      ADC__TriggerMode_Config(AUTO_SAMPLE);              // 再进入自动采样模式
      // ADC__TriggerMode_Config(HW_SAMPLE);
      printf("ADC_CTRL:%x\n",ADC_CTRL);
      // printf("ADC_INTMASK:%x\n",ADC_INTMASK);
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
      // Data = ADC_REG(ADC_DATA_0_OFFSET + ADC_Databuffer_channelx * 2);
      switch(ADC_Databuffer_channelx)
      {
            case 0:
                 Data=ADC_DATA_0;
                 break;
             case 1:
                 Data=ADC_DATA_1;
                 break; 
             case 2:
                 Data=ADC_DATA_2;
                 break;
              case 3:
                 Data=ADC_DATA_3;
                 break;
             case 4:
                 Data=ADC_DATA_4;
                 break;
             case 5:
                 Data=ADC_DATA_5;
                 break;
             case 6:
                 Data=ADC_DATA_6;
                 break;
             case 7:
                 Data=ADC_DATA_7;
                 break;         
      }
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
      printf("ADC_CNT0:%x\n",ADC_CNT0);
      printf("ADC_CNT1:%x\n",ADC_CNT1);
}

