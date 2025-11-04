/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-01-03 10:09:21
 * @Description: Fan support
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
#include "CUSTOM_FAN.H"
#include "CUSTOM_TEMPERATURE.H"
 /*FAN TEMP,Level and */
VBYTE	 last_dcr = 0, Level = 1;
/*
FAN FREQ 25kHz demo:
			pclk		prescale	scaler 	ctr			duty	frequency
			24.576M 	480  		1		1  			0~1		25.6k
			24.576M		4			23		10 			0~10	25.6k
			24.576M		4			2/3		100 		0~100	30.72/20.48k
			24.576M		4			0		255			0~255	24.094k//推荐，精度最高
			24.576M		4			0		245/246		0~246	25.077k/24.975k//推荐，频率标准
...
FAN DUTY CYCLE 20%-100%,MIN_PWM 20%,MIN_RPM 30%,MAX PWM/RPM 100%
* */
const thermal ThermalTalbe1[] =//24.576M		4			0		255			0~255	24.094k//推荐，精度最高
{
	{
		0,//保护机制，防止低温是由于温度获取错误
		-35,
		-128,
		FAN0_MAX_DCR,
	}, // Level 0 //-35~-128°C//100% 
	{
		0,
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 0 + TEMP_LEV),
		-40,
		((FAN0_MAX_DCR + 5) / 10 * 0 + FAN0_MIN_DCR),
	}, // Level 1 //-40~20°C//start:0% 
	{
		4,
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 1 + TEMP_LEV),
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 0 + TEMP_LEV - (TEMP_MAX - TEMP_LEV + 10) / 20),
		((FAN0_MAX_DCR + 5) / 10 * 1 + FAN0_MIN_DCR),//start:10% MAX:10%+28/FAN0_MAX_DCR
	}, // Level 2 //18~25°C
	{
		4,
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 2 + TEMP_LEV),
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 1 + TEMP_LEV - (TEMP_MAX - TEMP_LEV + 10) / 20),
		((FAN0_MAX_DCR + 5) / 10 * 2 + FAN0_MIN_DCR),//start:20% MAX:20%+28/FAN0_MAX_DCR
	}, // Level 3 //23~30°C
	{
		5,
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 3 + TEMP_LEV),
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 2 + TEMP_LEV - (TEMP_MAX - TEMP_LEV + 10) / 20),
		((FAN0_MAX_DCR + 5) / 10 * 3 + FAN0_MIN_DCR),//start:30% MAX:30%+35/FAN0_MAX_DCR
	}, // Level 4 //28~35°C
	{
		5,
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 4 + TEMP_LEV),
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 3 + TEMP_LEV - (TEMP_MAX - TEMP_LEV + 10) / 20),
		((FAN0_MAX_DCR + 5) / 10 * 4 + FAN0_MIN_DCR),//start:40% MAX:40%+35/FAN0_MAX_DCR
	}, // Level 5 //33~40°C
	{
		6,
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 5 + TEMP_LEV),
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 4 + TEMP_LEV - (TEMP_MAX - TEMP_LEV + 10) / 20),
		((FAN0_MAX_DCR + 5) / 10 * 5 + FAN0_MIN_DCR),//start:50% MAX:65%+42/FAN0_MAX_DCR
	}, // Level 6 //38~45°C
	{
		7,
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 6 + TEMP_LEV),
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 5 + TEMP_LEV - (TEMP_MAX - TEMP_LEV + 10) / 20),
		((FAN0_MAX_DCR + 10) / 20 * 13 + FAN0_MIN_DCR),//start:65% MAX:80%+49/FAN0_MAX_DCR
	}, // Level 7 //43~50°C
	{
		6,
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 7 + TEMP_LEV),
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 6 + TEMP_LEV - (TEMP_MAX - TEMP_LEV + 10) / 20),
		((FAN0_MAX_DCR + 10) / 20 * 16 + FAN0_MIN_DCR),//start:80% MAX:100%
	}, // Level 8 //48~55°C
	{
		3,
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 8 + TEMP_LEV),
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 7 + TEMP_LEV - (TEMP_MAX - TEMP_LEV + 10) / 20),
		((FAN0_MAX_DCR + 10) / 20 * 19 + FAN0_MIN_DCR),//start:95% MAX:100%
	}, // Level 9//53~60°C
	{
		0,
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 9 + TEMP_LEV),
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 8 + TEMP_LEV - (TEMP_MAX - TEMP_LEV + 10) / 20),
		FAN0_MAX_DCR,//start:100% MAX:100%
	}, // Level 10//58~65°C//最高温度，一直使用最大转速
	{
		0,
		127,
		((TEMP_MAX - TEMP_LEV + 5) / 10 * 9 + TEMP_LEV - (TEMP_MAX - TEMP_LEV + 10) / 20),
		FAN0_MAX_DCR,//start:100% MAX:100%
	}, // Level 11//保护性代码
};
static s8 ReadTemp(void)
{
	register s8 cpu_temp, sys_temp, max_temp;
	cpu_temp = ReadCPUTemp();
	sys_temp = ReadSysTemp();
	max_temp = (cpu_temp > sys_temp) ? cpu_temp : sys_temp;
	return max_temp;
}
static BYTE ReadLastDCR(BYTE channel)
{
	register BYTE offset = 0;
	register BYTE shift = 0;
	offset = PWM_DCR0_1_OFFSET + ((channel & 0x06));
	shift = ((channel & 0x1) << 3);
	last_dcr = (PWM_REG(offset) & (0xFF << shift)) >> shift;
	if (!(PWM_MODE & (0x100 << channel)))//低有效则占空比取反
	{
		last_dcr = FAN0_MAX_DCR - last_dcr;
	}
	return last_dcr;
}
static BYTE FAN_Update_PWM_DCR(BYTE channel, BYTE dcr, BYTE step)
{

	/* 引脚复用 */
	sysctl_iomux_config(GPIOA, channel, 1);
	/* 极性检查 */
	if (!(PWM_MODE & (0x100 << channel)))//低有效则占空比取反
	{
		dcr = FAN0_MAX_DCR - dcr;
	}
	register BYTE offset = 0;
	register BYTE shift = 0;
	/*更新step值*/
	offset = PWM_STEP3_0_OFFSET + ((channel & 0x4) >> 1);
	shift = ((channel & 0x3) << 2);
	PWM_REG(offset) &= ~(0x000f << shift);
	if (step)
	{
		PWM_MODE |= (0x1 << channel);
		PWM_REG(offset) |= (step << shift);
	}
	else
	{
		PWM_MODE &= ~(0x1 << channel);
	}
	/* 更新占空比 */
	offset = PWM_DCR0_1_OFFSET + ((channel & 0x06));
	shift = ((channel & 0x1) << 3);
	PWM_REG(offset) &= ~(0xFF << shift);
	PWM_REG(offset) |= (dcr << shift);
	return dcr;
}
//-----------------------------------------------------------------------------
// Function Name : ReadCPUTemp
//-----------------------------------------------------------------------------
s8 ReadCPUTemp(void)
{
	/*peci读取cpu温度，暂时不做处???*/
	// cpu_temp
	return (s8)CPU_TEMP;
}
//-----------------------------------------------------------------------------
// Function Name : ReadSysTemp
//-----------------------------------------------------------------------------
s8 ReadSysTemp(void)
{
	/*iic接口读取温感*/
	// get_temperature(1);
	// sys_temp
	return (s8)SYSTEM_TEMP;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: FAN_Init
 *
 * Read CPU/SYSTEM MAX TEMP to FIND FAN LEVEL,
 * SET FAN PWM DCR STEP
 *
 * Input: channel :PWM_CHANNELn(n=0~7)
 *			clk:PWM_CLKn(n=0~3)
 *			ctr:PWM_CLKn(n=0~3)
 * return :PWM dcr(duty=dcr/ctr)
 * ------------------------------------------------------------------------- */
BYTE FAN_Init(BYTE channel, BYTE clk, BYTE ctr)
{

	channel &= 0x7;
	clk &= 0x3;
	ctr &= 0x3;
	register BYTE offset = 0;
	register BYTE shift = 0;
	PWM_CTRL |= 0x1;//4预分频，24M约6M
	/* pwm_scale */
	offset = PWM_SCALER0_OFFSET + (clk << 1);//1分频
	PWM_REG(offset) = 0;
	offset = PWM_CTR0_1_OFFSET + (ctr & 0x2);//FAN0_CTR//频率为1周期
	shift = ((ctr & 0x1) << 3);
	PWM_REG(offset) = (PWM_REG(offset) & (~(0xff << shift))) | ((FAN0_CTR & 0xff) << shift);
	//defaylt level and last dcr
	CPU_TEMP = SYSTEM_TEMP = 127;//默认温度最高
	Level = 0;//默认最高等级，直到下次进行等级运算
	last_dcr = FAN0_MAX_DCR;//默认最高转速
	PWM_Init_channel(channel, PWM_HIGH, clk, ctr, last_dcr, ThermalTalbe1[Level].CSTEP);
	return last_dcr;
}
/* ----------------------------------------------------------------------------
 * FUNCTION: FAN_LEV
 *
 * Read CPU/SYSTEM MAX TEMP to FIND FAN LEVEL,
 * SET FAN PWM DCR STEP
 *
 * Input: channel :PWM_CHANNELn(n=0~7)
 *
 *
 * return :PWM dcr(duty=dcr/ctr)
 * ------------------------------------------------------------------------- */
BYTE FAN_LEV(BYTE channel)//等级算法
{
	channel &= 0x7;
	static signed char last_temp = 127;
	/*1.获取温度*/
	s8 temp = ReadTemp();
	last_dcr = ReadLastDCR(channel);
	if (temp == last_temp)//恒温
	{
		return last_dcr;
	}
	/*2:获取等级*/
RE_LEVEL_ADD:
	if (Level < (sizeof(ThermalTalbe1) / sizeof(thermal) - 1))/*如果风扇不是最大的转速，可以进入这个判断处理流程当中*/
	{
		if (temp > ThermalTalbe1[Level].CFanOn)
		{
			Level += 1;
			goto RE_LEVEL_ADD;//直到对应等级
		}
	}
RE_LEVEL_SUB:
	if (Level > 0)/*如果风扇不是最低档转速的存在，就可以进入这个流程，处理降低风扇转???*/
	{
		if (temp < ThermalTalbe1[Level].CFanOff)
		{
			Level -= 1;
			goto RE_LEVEL_SUB;//直到对应等级
		}
	}
	/*3:根据新的等级和参数算出占空比*/
	u8 dcr = ThermalTalbe1[Level].CDCR;//计算新的dcr
	u8 step = ThermalTalbe1[Level].CSTEP & 0xf;
	if (dcr != 0)
		if (dcr < FAN0_MAX_DCR)
		{
			if (temp > last_temp)//升温
			{
				//调节后转速 = 初始转速 +（当前温度 – 开始温度）* 转速调节系数
				signed char tmp = (temp - ThermalTalbe1[Level].CFanOff) * step;
				if (FAN0_MAX_DCR - dcr >= tmp)
					dcr = dcr + tmp;
				else dcr = FAN0_MAX_DCR;
				if (dcr < last_dcr)
				{
					if (last_dcr <= FAN0_MAX_DCR - step)
						dcr = last_dcr + step;
					else dcr = FAN0_MAX_DCR;
				}//升温的情况要比原先更大转速
			}
			else if (temp < last_temp)//降温
			{
				//调节后转速 = 初始转速 +[ (当前温度+当前区间差值)/2] – 开始温度）* 转速调节系数
				char tmp = (ThermalTalbe1[Level].CFanOn - ThermalTalbe1[Level].CFanOff) >> 1;
				tmp = (temp + tmp - ThermalTalbe1[Level].CFanOff) * step;
				if (FAN0_MAX_DCR - dcr >= tmp)
					dcr = dcr + tmp;
				else dcr = FAN0_MAX_DCR;
				if (dcr > last_dcr)dcr = last_dcr;//降温情况如果转速更大则保持不变
			}
		}
	if (dcr > FAN0_MAX_DCR)dcr = FAN0_MAX_DCR;
	/*4:更新配置*/
	if (dcr == 0)
		GPIO_Config(GPIOA, channel, 1, 0, 0, 0);
	else if (dcr == FAN0_MAX_DCR)
		GPIO_Config(GPIOA, channel, 1, 1, 0, 0);
	else
		FAN_Update_PWM_DCR(channel, dcr, step);
	assert_print("\nlast:temp=%d\tdcr=%d\nnew :temp=%d\tdcr=%d\n", last_temp, last_dcr, temp, dcr);
	last_temp = temp;
	return dcr;
}
/* ----------------------------------------------------------------------------
 * FUNCTION: FAN_Dynamic
 *
 * Read CPU/SYSTEM MAX TEMP AND LAST DCR
 * SET NEW FAN PWM DCR STEP
 *
 * Input: channel :PWM_CHANNELn(n=0~7)
 *
 *
 * return :PWM dcr(duty=dcr/ctr)
 * ------------------------------------------------------------------------- */
BYTE FAN_Dynamic(BYTE channel)//动态算法,需自行调整参数
{
	channel &= 0x7;
	static s8 last_temp = 127;
	/*1.获取温度*/
	s8 temp = ReadTemp();
	last_dcr = ReadLastDCR(channel);
	/*2:根据温度调节转速*/
	u8 dcr = last_dcr;//计算新的dcr
	u8 step;
	if (temp >= TEMP_MAX)//在这个值之上永远满速运行
	{
		step = 0;
		dcr = FAN0_MAX_DCR;
	}
	else if (temp > TEMP_SETPOINT)
	{
		step = (temp >> 4);
		if (temp >= last_temp)//升温
		{
			//调节后转速 = (当前转速 +（当前温度 – 上次温度+1）* 转速调节系数)
			char tmp = (temp - last_temp + 1) * step + (step >> 4);
			if (FAN0_MAX_DCR - dcr >= tmp)
				dcr = dcr + tmp;
			else dcr = FAN0_MAX_DCR;
		}
		else if (temp < last_temp)//降温则可以缓慢减少风扇转速直到
		{
			step >>= 1;
			//调节后转速 = 当前转速 -(温差值* 转速调节系数+1)
			char tmp = (last_temp - temp) * step + 1;
			if (dcr >= tmp)
				dcr = dcr - tmp;
			else if (dcr)
				dcr--;
		}
		if (dcr > FAN0_MAX_DCR)dcr = FAN0_MAX_DCR;
	}
	else if (temp == TEMP_SETPOINT)
	{
		step = 0;//等于这个值就不加减
	}
	else//低于这个参考值则逐渐降低转速
	{
		step = 1;
		if (dcr)//逐渐减少到0
			dcr--;
	}
	/*3:更新配置*/
	if (dcr == 0)
		GPIO_Config(GPIOA, channel, 1, 0, 0, 0);
	else if (dcr == FAN0_MAX_DCR)
		GPIO_Config(GPIOA, channel, 1, 1, 0, 0);
	else
		FAN_Update_PWM_DCR(channel, dcr, step);
	assert_print("\nlast:temp=%d\tdcr=%d\nnew :temp=%d\tdcr=%d\n", last_temp, last_dcr, temp, dcr);
	last_temp = temp;
	return dcr;
}
/* ----------------------------------------------------------------------------
 * FUNCTION: FAN_PID
 *
 * Read CPU/SYSTEM MAX TEMP AND LAST DCR
 * SET NEW FAN PWM DCR STEP
 *
 * Input: channel :PWM_CHANNELn(n=0~7)
 *
 *
 * return :PWM dcr(duty=dcr/ctr)
 * ------------------------------------------------------------------------- */
BYTE FAN_PID(BYTE channel)//PID算法预留，需自行调整参数
{
	channel &= 0x7;
	static s16 tmp = 0;
	static s16 intergral = 0;//累计误差
	static s16 last_tmp = 0;//上次误差
	/*1.获取温度*/
	s8 temp = ReadTemp();
	last_dcr = ReadLastDCR(channel);
	/*2:根据温度调节转速*/
	u8 dcr = last_dcr;//计算新的dcr
	u8 step;
	if (temp >= TEMP_MAX)//在保护值之上永远满速运行
	{
		step = 0;
		dcr = FAN0_MAX_DCR;
	}
	else
	{
		step = (temp >> 3);
		tmp = (temp - TEMP_SETPOINT2);
		intergral = last_tmp * _Ki;
		s8 pout = (tmp * _Kp);
		s8 dout = ((tmp - last_tmp) * _Kd);
		s16 pidValue = pout + intergral + dout;
		last_tmp = tmp;
		if (last_tmp < 0)
		{
			dcr = dcr + pidValue;
		}
		else
		{
			if (pidValue > (FAN0_MAX_DCR))
				dcr = FAN0_MAX_DCR;
			else
				dcr = dcr + pidValue;
		}
		if (dcr > FAN0_MAX_DCR)
			dcr = FAN0_MAX_DCR;
	}
	/*3:更新配置*/
	if (dcr == 0)
		GPIO_Config(GPIOA, channel, 1, 0, 0, 0);
	else if (dcr == FAN0_MAX_DCR)
		GPIO_Config(GPIOA, channel, 1, 1, 0, 0);
	else
		FAN_Update_PWM_DCR(channel, dcr, step);
	return dcr;
}
