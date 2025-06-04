/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-06-04 17:22:23
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
#include <KERNEL_OWI.H>

//数据发送
void OWI_GO(u16 reset_us)
{
    OWI_GENERAL_CTRL |= OWI_GENERAL_CTRL_GO;
    while((OWI_GENERAL_CTRL & OWI_GENERAL_CTRL_INTSR) == 0);//等待等待reset结束
    vDelayXus(reset_us);
}
// 灯带数据初始化
void OWI_Set_Data(u8 led_num_start, u8 led_num_over, u32 data)
{
    data &= 0xffffff;
    if(led_num_start >= 128)led_num_start = 127;
    if(led_num_over >= 128)led_num_over = 127;
    while(led_num_start <= led_num_over)
    {
        OWI_DATA((led_num_start << 2)) = data;
        led_num_start++;
    }
}
// 数据清空
void OWI_Clear_Data(u8 led_num_start, u8 led_num_over)
{
    if(led_num_start >= 128)led_num_start = 127;
    if(led_num_over >= 128)led_num_over = 127;
    while(led_num_start <= led_num_over)
    {
        OWI_DATA((led_num_start << 2)) = 0x0UL;
        led_num_start++;
    }
}

// OWI模块初始化
void OWI_Init(u8 data_len, u8 out_clk, u8 T1H, u8 T0H)
{
    OWI_SYMBOL_CTRL = ((u32)T0H | ((u32)T1H << 8));
    data_len -= 1;
    out_clk = (out_clk <= 3) ? 3 : out_clk - 1;
    OWI_GENERAL_CTRL = OWI_GENERAL_CTRL_MSB | OWI_GENERAL_CTRL_CYCLE_NUM_CLK_DO((u32)out_clk)  \
        | OWI_GENERAL_CTRL_Length((u32)data_len) | OWI_GENERAL_CTRL_SEL_RZ;
    OWI_Clear_Data(0, 127);
}
// 同色
void OWI_Send_One_Data(u32 data)
{
    OWI_Set_Data(0, 127, data);
    OWI_GO(50);
}
