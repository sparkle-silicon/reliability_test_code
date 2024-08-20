/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-05-20 17:07:54
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
//*****************************************************************************
// Include all header file
#include "KERNEL_PWM.H"
//
//*****************************************************************************
//*****************************************************************************
//
//  To enabel PWM channel [x] 
//
//  parameter :
//          channel : pwm channel
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
BYTE  PWM_Enable(BYTE  channel)
{
	channel &= 0x7;
	PWM_CTRL |= (0x100 << channel);
	return 0;
}
//*****************************************************************************
//
//  To disable PWM channel [x] 
//
//  parameter :
//          channel : pwm channel
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
BYTE  PWM_Disable(BYTE  channel)
{
	channel &= 0x7;
	PWM_CTRL &= ~(0x100 << channel);
	return 0;
}
//*****************************************************************************
//
//  To read PWM channel [x] config
//
//  parameter :
//          pwm_no : pwm struct
//			Write channel and sel in advance
//			config will write in the pwm struct
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
BYTE  PWM_Get(spwm_str *pwm_ptr)
{
	if((pwm_ptr == NULL) || (pwm_ptr->pwm_no > 7))
	{
		return -1;
	}
	pwm_ptr->prescale = (PWM_CTRL & (0x1 << (pwm_ptr->pwm_no))) >> (pwm_ptr->pwm_no);
	pwm_ptr->pwm_polarity = PWM_MODE & (0x1 << (pwm_ptr->pwm_no + 0x8)) >> (pwm_ptr->pwm_no + 0x8);
	pwm_ptr->pwm_softmode = (PWM_MODE & (0x1 << (pwm_ptr->pwm_no))) >> (pwm_ptr->pwm_no);
	/*scale*/
	switch(pwm_ptr->pwm_clk_sel)
	{
		case 0:
			pwm_ptr->pwm_scale = PWM_SCALER0;
			break;
		case 1:
			pwm_ptr->pwm_scale = PWM_SCALER1;
			break;
		case 2:
			pwm_ptr->pwm_scale = PWM_SCALER2;
			break;
		case 3:
			pwm_ptr->pwm_scale = PWM_SCALER3;
			break;
		default:
			return -1;
	}
	/*pwm_ctr*/
	switch(pwm_ptr->pwm_ctr_sel)
	{
		case 0:
			pwm_ptr->pwm_ctr = (PWM_CTR0_1 & 0xFF);
			break;
		case 1:
			pwm_ptr->pwm_ctr = (PWM_CTR0_1 & (0xFF << 8)) >> (8);
			break;
		case 2:
			pwm_ptr->pwm_ctr = (PWM_CTR2_3 & 0xFF);
			break;
		case 3:
			pwm_ptr->pwm_ctr = (PWM_CTR2_3 & (0xff << 8)) >> (8);
			break;
		default:
			return -1;
	}
	pwm_ptr->pwm_clk_sel = (PWM_CLK_SEL & (0x3 << (pwm_ptr->pwm_no * 2))) >> (pwm_ptr->pwm_no * 2);
	pwm_ptr->pwm_ctr_sel = (PWM_CTR_SEL & (0x3 << (pwm_ptr->pwm_no * 2))) >> (pwm_ptr->pwm_no * 2);
	/*pwm_dcdr*/
	if(pwm_ptr->pwm_no < 2)
	{
		pwm_ptr->pwm_dcdr = (PWM_DCR0_1 & (0xff << (pwm_ptr->pwm_no * 8))) >> (pwm_ptr->pwm_no * 8);
	}
	else if(pwm_ptr->pwm_no < 4)
	{
		pwm_ptr->pwm_dcdr = (PWM_DCR2_3 & (0xff << ((pwm_ptr->pwm_no - 2) * 8))) >> ((pwm_ptr->pwm_no - 2) * 8);
	}
	else if(pwm_ptr->pwm_no < 6)
	{
		pwm_ptr->pwm_dcdr = (PWM_DCR4_5 & (0xff << ((pwm_ptr->pwm_no - 4) * 8))) >> ((pwm_ptr->pwm_no - 4) * 8);
	}
	else
	{
		pwm_ptr->pwm_dcdr = (PWM_DCR6_7 & (0xff << ((pwm_ptr->pwm_no - 6) * 8))) >> ((pwm_ptr->pwm_no - 6) * 8);
	}
	/*step*/
	if(pwm_ptr->pwm_no < 4)
	{
		pwm_ptr->pwm_step = (PWM_STEP3_0 & (0xf << (pwm_ptr->pwm_no * 4))) >> (pwm_ptr->pwm_no * 4);
	}
	else
	{
		pwm_ptr->pwm_step = (PWM_STEP7_4 & (0xf << ((pwm_ptr->pwm_no - 4) * 4))) >> ((pwm_ptr->pwm_no - 4) * 4);
	}
	return 0;
}
//*****************************************************************************
//
//  To init PWM channel
//
//  parameter :
//          channel : pwm channel 0~7
//			polarity:pwm polarity PWM_HIGH?PWM_LOW
//			clk:CLK select CLK0/CLK1/CLK2/CLK3
//			ctr:CTR select CTR0/CTR1/CTR2/CTR3
//			dcr:Duty Cycle
//			step:step mode val(0 is close step)
//
//  return :
//          Program operation condition
//			 0 : normal
//
//	note :
//			use pwm struct as paremater and pwm_set function to init
//*****************************************************************************
BYTE  PWM_Init_channel(BYTE channel, BYTE polarity, BYTE clk, BYTE ctr, BYTE dcr, BYTE step)
{
	register BYTE offset = 0;
	register BYTE shift = 0;
	PWM_CTRL &= ~(0x100 << channel);
	channel &= 0x7;
	step &= 0xf;
	dcr &= 0xff;
	clk &= 0x3;
	ctr &= 0x3;
	polarity &= 0x1;
	/* 极性选择 */
	if(polarity)
		PWM_MODE |= (0x100 << channel);
	else
		PWM_MODE &= (~(0x100 << channel));
	/* 时钟选择 */
	shift = channel << 1;
	PWM_CLK_SEL &= ~(0x3 << shift);
	PWM_CLK_SEL |= (clk << shift);
	/* 周期选择 */
	PWM_CTR_SEL &= ~(0x3 << shift);
	PWM_CTR_SEL |= (ctr << shift);
	/* 占空比 */
	offset = PWM_DCR0_1_OFFSET + ((channel & 0x06));
	shift = ((channel & 0x1) << 3);
	PWM_REG(offset) &= ~(0xFF << shift);
	PWM_REG(offset) |= (dcr << shift);
	/*step mode*/
	offset = PWM_STEP3_0_OFFSET + ((channel & 0x4) >> 1);
	shift = ((channel & 0x3) << 2);
	if(step)
	{
		PWM_MODE |= (0x1 << channel);
		PWM_REG(offset) &= ~(0x000f << shift);
		PWM_REG(offset) |= (step << shift);
	}
	else
	{
		PWM_MODE &= ~(0x1 << channel);
		PWM_REG(offset) &= ~(0x000f << shift);
	}
	/*pwm enable*/
	PWM_CTRL |= (0x100 << channel);
	return 0;
}
//*****************************************************************************
//
//  To init PWM CLOCK
//
//  parameter :
//          none
//
//  return :
//          none
//
//	note :
//			use pwm defined config clock
//*****************************************************************************
void  PWM_CLOCK_Init(void)
{
	/*prescale select*/
#if ((PWM_TCLK_PRESCALE==480)||(PWM_TCLK_PRESCALE==0))
	PWM_CTRL &= ~0x1;
#elif ((PWM_TCLK_PRESCALE==4)||(PWM_TCLK_PRESCALE==1))
	PWM_CTRL |= 0x1;
#else
	assert_print("prescale error\n");
	return;
#endif
		/* pwm_scale */
	PWM_SCALER0 = (PWM_CLK0_PRESCALE - 1);
	PWM_SCALER1 = (PWM_CLK1_PRESCALE - 1);
	PWM_SCALER2 = (PWM_CLK2_PRESCALE - 1);
	PWM_SCALER3 = (PWM_CLK3_PRESCALE - 1);
	PWM_CTR0_1 = PWM_CTR01;
	PWM_CTR2_3 = PWM_CTR23;
}
//*****************************************************************************
//
//  To enable tach channel [x] 
//
//  parameter :
//          channel : tach channel
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
BYTE  TACH_Enable(BYTE  channel)
{
	channel &= 0x3;
	TACH_CTRL |= (0x1 << channel);
	return 0;
}
//*****************************************************************************
//
//  To disable tach channel [x] 
//
//  parameter :
//          channel : tach channel
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
BYTE  TACH_Disable(BYTE  channel)
{
	channel &= 0x3;
	TACH_CTRL &= (~(0x1 << channel));
	return 0;
}
//*****************************************************************************
//
//  To enable tach channel [x] interrupt 
//
//  parameter :
//          channel : tach channel
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
BYTE  TACH_Int_Enable(BYTE  channel)
{
	channel &= 0x3;
	TACH_INT |= (0x1 << channel);
	return 0;
}
//*****************************************************************************
//
//  To disable tach channel [x] interrupt
//
//  parameter :
//          channel : tach channel
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
BYTE  TACH_Int_Disable(BYTE  channel)
{
	channel &= 0x3;
	TACH_INT &= (~(0x1 << channel));
	return 0;
}
//*****************************************************************************
//
//  To setup tach channel [x] 
//
//  parameter :
//          channel  : tach channel
//			mode : (1圈mode+1个脉冲，)
//
//  return :
//          Program operation condition
//			 0 : normal
//
//*****************************************************************************
BYTE  TACH_Init_Channel(BYTE channel, BYTE mode, BYTE tach_int)
{
	channel &= 0x3;
	mode &= 0x3;
	BYTE shift = (channel << 1) + 8;
	TACH_CTRL |= ((0x1 << channel) | (mode << shift));
	if(tach_int)
		TACH_INT |= (0x11 << channel);//clear irq and en irq
	else
	{
		TACH_INT |= (0x10 << channel);//clear irq and en irq
		TACH_INT &= ~(0x01 << channel);//clear irq and en irq
	}
	return 0;
}

//*****************************************************************************
//
//  To clear tach channel [x] init
//
//  parameter :
//          channel : tach channel
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
BYTE  TACH_Clear_IRQ(BYTE  channel)
{
	channel &= 0x3;
	TACH_INT |= (0x10 << channel);
	return 0;
}
//*****************************************************************************
//
//  To get tach channel [x] value
//
//  parameter :
//          channel : tach channel
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
WORD TACH_Get(BYTE  channel)
{
	channel &= 0x3;
	register u32 offset = TACH0_DATA_OFFSET + (channel << 1);
	return TACH_REG(offset);
}
//*****************************************************************************
//
//  To get tach channel [x] value  （用于轮询）
//
//  parameter :
//          channel : tach channel
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
WORD TACH_Get_Polling(BYTE  channel)
{
	WORD timeout = 1000;
	channel &= 0x3;
	while(--timeout)
	{
		if(TACH_INT & (0x10 << channel))
		{
			break;
		}
	}
	if(timeout == 0)
	{
		dprint("tach get error\n");
		return -1;
	}
	TACH_Clear_IRQ(channel);
	return TACH_Get(channel);
}
