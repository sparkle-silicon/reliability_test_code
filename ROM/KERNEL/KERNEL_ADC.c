/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-02-06 15:33:22
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
 * @param mode 0软件 1硬件 2连续
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
            default:
                  break;
      }
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
      if(sw == DISABLE)
      {
            ADC_CHAN_EN &= (~(0x1 << channelx));
      }
      else
      {
            ADC_CHAN_EN = (0x1 << channelx);
      }
}
/**
 * @brief ADC中断使能or屏蔽
 *
 * @param channelx irq_type：uv_irq ov_irq af_irq
 * @param sw DISABLE  ENABLE
 *
 * @return             无
 */
void ADC_IRQ_Config(uint8_t irq_type, uint8_t sw)
{
      if(sw == DISABLE)
      {
            ADC_INTMASK |= (irq_type);
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
 * @return 采样到有效数据返回1，否则返回0
 */
uint8_t ADC_Ready(void)
{
      uint8_t adc_ready_state = 0;
      adc_ready_state = ADC_READY;
      if(adc_ready_state & 0x1)
      {
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
#ifdef AE103
/**
 * @brief 103的初始化auto sample功能
 *
 * @param channelx 通道选择，取值范围为 0~11
 * @param auto_number auto sample通道选择，取值范围为 0~1
 * @param comp_rule 1大于则触发，0:小于则触发
 * @param trigger 触发值
 * @param timercount 定时器计数器值
 *
 * @return 无
 *
 * @note 函数只在配置为硬件触发时，当 timercount 递减到 0 时才进行采样
 */
void ADC_Auto_Sample_Init(uint8_t channelx, uint8_t auto_number, uint8_t comp_rule, uint16_t trigger, uint32_t timercount)
{
      channelx &= 0xf;
      auto_number &= BIT0;
      comp_rule = comp_rule ? 1 : 0;
      trigger &= 0xfff;
      ADC_CTRL &= ~(0b11 << 7);//关闭自动触发的几种方式
      ADC_CHAN_EN = 0;//所有通道关闭使能
      ADC_INTMASK = ADC_INTMASK & (~(BIT3 << auto_number));
      ADC_Timercount_Set(timercount);
      if(auto_number)
      {
            ADC_AUTO_SAMPLE_CTRL1 = (comp_rule << 14) | (0b11 << 12) | trigger;
            ADC_AUTO_SAMPLE_CHAN_NUM |= BIT(8);

      }
      else
      {
            ADC_AUTO_SAMPLE_CTRL0 = (comp_rule << 14) | (0b11 << 12) | trigger;
      }
      ADC_AUTO_SAMPLE_CHAN_NUM &= (~(0xf << (auto_number << 2)));
      ADC_AUTO_SAMPLE_CHAN_NUM |= (channelx << (auto_number << 2));
      ADC_CTRL |= (0b11 << 7);
}
#endif
/**
 * @brief ADC初始化函数
 *
 * @param channelx 通道选择，取值范围为 0~11
 * @param attenuationMode 衰减 1/3 模式，0表示不衰减模式（0~1.2V），1表示衰减模式（0~3.6V）
 * @param trigger 触发模式选择，0表示软件触发，1表示硬件触发，2表示连续模式
 * @param mode 单端或差分模式选择，1表示单端，0表示差分
 *
 * @return 无
 *
 * @note 此函数用于初始化ADC模块，包括通道选择、触发模式选择和单端/差分模式选择等。
 */
void ADC_Init(uint8_t channelx, uint8_t attenuationMode, uint8_t trigger, uint8_t mode)
{
      ADC_CTRL = 0x3 | (attenuationMode ? BIT5 : 0);      // 配置为原码 fifo满阈值为3
      ADC_PM = 0x0;                                      // 关闭低功耗
      ADC_IRQ_Config(uv_irq | ov_irq | af_irq, DISABLE); // 屏蔽所有中断
      ADC__TriggerMode_Config(trigger);                  // 触发模式选择
      ADC_SampleMode_Config(mode);                       // 单端or差分模式选择
      ADC_Channelx_Config(channelx, ENABLE);             // 通道使能
}
/**
 * @brief 触发ADC采样操作
 *
 * @param 无
 *
 * @return 无
 *
 * @note 此函数用于触发ADC进行采样操作。
 */
void ADC_Trigger_operation(void)
{
      ADC_TRIGGER = 0x1;
}
/**
 * @brief 读取ADC采集到的值
 *
 * @param 无
 *
 * @return 返回ADC采集到的短整型数据
 *
 * @note 此函数用于读取ADC采集到的值。
 */
short ADC_ReadData(void)
{
      uint16_t Data = 0;
      Data = ADC_DATA;
      return Data;
}
/**
 * @brief 在软件模式下进行ADC采样
 *
 * @param 无
 *
 * @return 返回ADC采集到的短整型数据
 *
 * @note 此函数用于在软件模式下触发ADC进行采样操作，并返回采集到的数据。
 */
short ADC_SW_Sample(void)
{
      ADC_TRIGGER = 0x1;
      while(!ADC_Ready());
      return ADC_DATA;
}


