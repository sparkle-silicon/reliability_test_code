/*
 * @Author: Maple
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-12-22 10:18:58
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
//test file
#include "TEST.H"
#include "KERNEL_PNP.H"
#include "KERNEL_SWUC.H"
#include "KERNEL_TIMER.H"
#include "KERNEL_GPIO.H"
#if TEST
//PMC1 or PMC2 compatible mode sci test config
void SCI_Compatible_HW_Config(uint8_t PMC_Channel)
{
	if(PMC_Channel == 1)
	{
		KBC_SIRQCTL |= (0X0 << 6);  //IRQNP SET 0
		KBC_CTL |= 0x10;  //PMC12HIE SET 1
		PMC1_IE |= 0x1a;  //Enable SCIEN & HWSCIEN
		PMC1_IE &= 0xdb;  //Disable SMI
		PMC2_IE &= 0x0;   //Set PMC2 interrupt disable
		PMC1_DOR = 0xab;    //write data to PMC1DOR (provide OBF)
		PMC1_CTL |= 0x2;   //enable PMC1OBE interrupt
		//*((volatile uint8_t *)(0x2406)) &= ~(0x40) ;   //SCINP Set 0
		dprint("0x2406 data second is %#x\n", *((volatile uint8_t *)(0x2406)));
		dprint("0x2452 data second is %#x\n", *((volatile uint8_t *)(0x2452)));
	}
	else if(PMC_Channel == 2)
	{
		KBC_CTL |= 0x10;  //PMC12HIE SET 1
		KBC_SIRQCTL |= (0X0 << 6);  //IRQNP SET 0
		KBC_SIRQCTL |= 0X10;  //IRQM SET 010
		PMC2_IE |= 0x1a;  //Set PMC2 interrupt enable
		PMC2_IE &= 0xdb;  //Set PMC2 interrupt disable
		PMC1_IE &= 0x0;   //Set PMC1 interrupt disable
		PMC2_DOR = 0xab;    //write data to PMC1DOR (provide OBF)
		PMC2_CTL |= 0x2;   //enable PMC2OBE interrupt
		//PMC2_CTL &= ~(0x40) ;   //SCINP Set 0  (Reset value is 1)
		dprint("0x2416 data second is %#x\n", *((volatile uint8_t *)(0x2416)));
	}
}
//PMC1 or PMC2 compatible mode sci test config & software trig IRQ
void SCI_Compatible_SW_Config(uint8_t PMC_Channel)
{
	if(PMC_Channel == 1)
	{
		//PMC1_CTL |= 0x40 ;    //SCINP Set 1
		PMC1_CTL &= 0xbf;      //SCINP Set 0
		KBC_CTL &= 0xef;      //PMC12HIE Set 0
		//*((volatile uint8_t *)(0x2452)) |= 0x04 ;    //IRQ11B Set 1
		KBC_SIRQCTL &= (~0x04);   //IRQ11B Set 0
		PMC1_IE |= 0x2;       //SCIEN	
		//PMC2_IE&= 0xe6 ;
		PMC2_IE &= 0x0;   //Set PMC2 interrupt disable
		for(BYTE i = 0; i < 10; i++)
		{
			KBC_SIRQCTL &= (~0x04);  //IRQ11B Set 0
			vDelayXms(10);
			KBC_SIRQCTL |= 0x04;    //IRQ11B Set 1
			vDelayXms(10);
		}
	}
	else if(PMC_Channel == 2)
	{
		//PMC2_CTL |= 0x40 ;    //SCINP Set 1
		PMC2_CTL &= 0xbf;      //SCINP Set 0
		KBC_CTL &= 0xef;      //PMC12HIE Set 0
		//KBC_SIRQCTL |= 0x04 ;    //IRQ11B Set 1
		KBC_SIRQCTL &= (~0x04);   //IRQ11B Set 0
		PMC2_IE |= 0x2;       //SCIEN	
		//PMC2_IE &= 0xe6 ;
		PMC1_IE &= 0x0;   //Set PMC1 interrupt disable
		for(BYTE i = 0; i < 10; i++)
		{
			KBC_SIRQCTL &= (~0x04);    //IRQ11B Set 0
			vDelayXms(10);
			KBC_SIRQCTL |= 0x04;    //IRQ11B Set 1
			vDelayXms(10);
		}
	}
}
//PMC1 or PMC2 enhance mode sci test config 
//(Set OBF For SCI) or (Set IBF For SCI)
void SCI_enhance_HW_Config(uint8_t PMC_Channel, uint8_t type)
{
	if(PMC_Channel == 1)
	{
		if(type == OBF)
		{
			PMC1_IE |= 0x12;     //HWSCI:1 SCIEN:1
			PMC1_CTL |= 0x80;      //APM:1  SCIPM:000
			//PMC1_CTL &= (~0x40);   //SCINP:0  default value:1
			PMC1_DOSCI = 0xfb;       //write data to DOSCI & SET OBF
		}
		else
		{
			PMC1_CTL |= 0x80;      //APM:1  SCIPM:000
			PMC1_CTL &= (~0x40);   //SCINP:0  default value:1
			PMC1_IE |= 0x12;     //HWSCI:1 SCIEN:1
			BYTE  DISCI_Data = 0;
			DISCI_Data = PMC1_DISCI; //read data to DISCI
		}
	}
	else
	{
		if(type == OBF)
		{
			PMC2_IE |= 0x12;     //HWSCI:1 SCIEN:1
			//PMC2_CTL |= 0x98;      //APM:1  SCIPM:011
			//PMC2_CTL &= (~0x40);   //SCINP:0  default value:1
			PMC2_DOSCI = 0xfb;       //write data to DOSCI & SET OBF
		}
		else
		{
			PMC2_CTL |= 0x80;      //APM:1  SCIPM:000
			PMC2_CTL &= (~0x40);   //SCINP:0  default value:1
			PMC2_IE |= 0x12;     //HWSCI:1 SCIEN:1
			BYTE  DISCI_Data = 0;
			DISCI_Data = PMC2_DISCI; //read data to DISCI
		}
	}
}
void SCI_enhance_SW_Config(uint8_t PMC_Channel)
{
	if(PMC_Channel == 1)
	{
		PMC1_CTL |= 0x80;      //APM:1  SCIPM:000
		PMC1_CTL &= (~0x40);   //SCINP:0  default value:1
		PMC1_IE &= 0xef;      //HWSCI:0
		PMC1_IE |= 0x02;     //SCIEN:1
		for(BYTE i = 0; i < 100; i++)
		{
			PMC1_IC |= 0x04;      //SCISB:1
			vDelayXms(10);
			PMC1_IC &= (~0x04);    //SCISB:0
			vDelayXms(10);
		}
	}
	else if(PMC_Channel == 2)
	{
		PMC2_CTL |= 0x80;      //APM:1  SCIPM:000
		PMC2_CTL &= (~0x40);   //SCINP:0  default value:1
		PMC2_IE &= 0xef;      //HWSCI:0
		PMC2_IE |= 0x02;     //SCIEN:1
		for(BYTE i = 0; i < 100; i++)
		{
			PMC2_IC |= 0x04;      //SCISB:1
			vDelayXms(10);
			PMC2_IC &= (~0x04);    //SCISB:0
			vDelayXms(10);
		}
	}
}
void SCI_test(void)
{
#if SCI_Test
	GPIOA_REG(GPIO_INPUT_VAL) |= ((0x0) << 0);//GPIO:0
	sysctl_iomux_config(GPIOA, 27, 1);	//set GPD3 as SCI
	GPIOA_REG(GPIO_INPUT_VAL) |= ((0x1) << 0);//GPIO:1
	//PMC1 compatible mode sci test config
#if 0
	SCI_Compatible_HW_Config(1);
#endif
//PMC1 compatible mode sci test config & software trig IRQ
#if 0
	SCI_Compatible_SW_Config(1);
#endif
//PMC2 compatible mode sci test config(HW)
#if 0
	SCI_Compatible_HW_Config(2);
#endif
//PMC2 compatible mode sci test config & software trig IRQ
#if 0
	SCI_Compatible_SW_Config(2);
#endif
//PMC1 enhance mode sci test config (Set OBF For SCI)
#if 0
	SCI_enhance_HW_Config(1, OBF);
#endif
//PMC1 enhance mode sci test config (Set IBF For SCI)
#if 0
	SCI_enhance_HW_Config(1, IBF);
#endif
//PMC1 enhance mode sci test config (Software)
#if 0
	SCI_enhance_SW_Config(1);
#endif
//PMC2 enhance mode sci test config (Set OBF For SCI)
#if 0
	SCI_enhance_HW_Config(2, OBF);
#endif
//PMC2 enhance mode sci test config (Set IBF For SCI)
#if 0
	SCI_enhance_HW_Config(2, IBF);
#endif
//PMC2 enhance mode sci test config (Software)
#if 0
	SCI_enhance_SW_Config(2);
#endif
#endif
}

void SMI_test(void)
{
#if SMI_Test
	GPIOA_REG(GPIO_INPUT_VAL) |= ((0x0) << 0);//GPIO:0
	sysctl_iomux_config(GPIOA, 28, 1);       //set GPD4 as SMI
	GPIOA_REG(GPIO_INPUT_VAL) |= ((0x1) << 0);//GPIO:0
#if 0
//PMC1 compatible mode smi test config(HW)
	KBC_CTL |= 0x10;  //PMC12HIE SET 1
	//KBC_SIRQCTL |= 0X08 ;  //IRQM SET 001    default value:000
	KBC_SIRQCTL |= (0X0 << 6);  //IRQNP SET 0
	PMC1_IE |= 0x04;  //SMIEN:1
	PMC2_IE &= 0x0;   //Set PMC2 interrupt disable
	PMC1_DOR = 0xab;    //write data to PMC1DOR(OBF)
	//PMC1_IC &= (~0x40) ;   //SMINP:0 default value:1 
#endif
#if 0
//PMC1 compatible mode smi test config(SW)
	KBC_CTL &= 0xef;  //PMC12HIE SET 0
	PMC1_IE |= 0x04;  //SMIEN:1
	PMC1_IC &= (~0x40);   //SMINP:0 default value:1
	PMC2_IE &= 0x0;   //Set PMC2 interrupt disable
	for(BYTE i = 0; i < 10; i++)
	{
		KBC_SIRQCTL &= (~0x04);    //IRQ11B Set 0
		vDelayXms(10);
		KBC_SIRQCTL |= 0x04;    //IRQ11B Set 1
		vDelayXms(10);
	}
#endif
#if 0
//PMC2 compatible mode smi test config(HW)
	KBC_CTL |= 0x10;  //PMC12HIE SET 1
	//KBC_SIRQCTL |= 0X38 ;  //IRQM SET 111    default value:000
	KBC_SIRQCTL |= (0X0 << 6);  //IRQNP SET 0
	PMC2_IE |= 0x04;  //SMIEN:1
	PMC1_IE &= 0x0;   //Set PMC1 interrupt disable
	PMC2_DOR = 0xab;    //write data to PMC1DOR(OBF)
	//PMC2_IC &= (~0x40) ;   //SMINP:0 default value:1 
#endif
#if 0
//PMC2 compatible mode smi test config(SW)
	KBC_CTL &= 0xef;  //PMC12HIE SET 0
	PMC2_IE |= 0x04;  //SMIEN:1
	PMC2_IC &= (~0x40);   //SMINP:0 default value:1
	PMC1_IE &= 0x0;   //Set PMC1 interrupt disable
	for(BYTE i = 0; i < 10; i++)
	{
		KBC_SIRQCTL &= (~0x04);    //IRQ11B Set 0
		vDelayXms(10);
		KBC_SIRQCTL |= 0x04;    //IRQ11B Set 1
		vDelayXms(10);
	}
#endif
#if 0
//PMC1 Enhance mode smi test config (Set OBF for SMI)
	PMC1_CTL |= 0x80;      //APM:1
	//PMC1_IC |= 0x38;      //SMIPM:111
	//PMC1_IC &= (~0x40) ;   //SMINP:0 default value:1
	PMC1_IE |= 0x24;       //HWSMIEN:1 SMIEN:1 
	PMC1_DOSMI |= 0xfb;       //write data to DOSMI and trig smi
#endif
#if 0
//PMC1 Enhance mode smi test config (SW)
//PMC1_IC &= (~0x40) ;   //SMINP:0 default value:1
	PMC1_CTL |= 0x80;      //APM:1
	PMC1_IE &= 0xdf;    //HWSMIEN:0 
	PMC1_IE |= 0x04;       //SMIEN:1 
	PMC2_IE &= 0x0;   //Set PMC2 interrupt disable
	for(BYTE i = 0; i < 10; i++)
	{
		PMC1_IC &= (~0x02);    //SMISB Set 0
		vDelayXms(10);
		PMC1_IC |= 0x02;       //SMISB Set 1
		vDelayXms(10);
	}
#endif
#if 0
//PMC2 Enhance mode smi test config (Set OBF for SMI)
	PMC2_CTL |= 0x80;      //APM:1
	PMC2_IC |= 0x38;      //SMIPM:111
	//PMC2_IC &= (~0x40) ;   //SMINP:0 default value:1
	PMC2_IE |= 0x24;       //HWSMIEN:1 SMIEN:1 
	PMC2_DOSMI |= 0xfb;       //write data to DOSMI and trig smi
#endif
#if 0
//PMC2 Enhance mode smi test config (SW)
	PMC2_CTL |= 0x80;      //APM:1
	//PMC2_IC &= (~0x40) ;   //SMINP:0 default value:1
	PMC2_IE &= 0xdf;    //HWSMIEN:0 
	PMC2_IE |= 0x04;       //SMIEN:1 
	PMC1_IE &= 0x0;   //Set PMC1 interrupt disable
	for(BYTE i = 0; i < 10; i++)
	{
		PMC2_IC &= (~0x02);    //SMISB Set 0
		vDelayXms(10);
		PMC2_IC |= 0x02;       //SMISB Set 1
		vDelayXms(10);
	}
#endif
#endif
}

void SWUC_Test(void)
{
#if 0
	SWUC_UART_Test();
#endif
#if 0
	SWUC_RI1_RI2_Test();
#endif
#if 0
	SWUC_Ring_Test();
#endif
#if 0
	SWUC_Soft_Test();
#endif
#if 0
	SWUC_LegacyMode_Test();
#endif
#if 0
	SWUC_ACPI_Init();
#endif
#if 0
	SWUC_PWUREQ_Init();
#endif
#if 0
	SWUC_SMI_Init();
#endif
#if 0
	SWUC_KBRST_Init(1);
#endif
#if 0
	SWUC_GA20_Init(1);
#endif
}
void SYSCTL_PORINT_Test(void)
{
	sysctl_iomux_config(GPIOB, 6, 1);
	SYSCTL_PORINT |= 0x1;
}
#define AUTO_TEST 0
#if AUTO_TEST
void KBC_PORT_TEST(void)
{
	BYTE  var;
	var = KBC_IB;
	while(KBC_STA & 0x1);
#if LPC_WAY_OPTION_SWITCH
#else
	SET_BIT(KBC_CTL, 3);
#endif
	KBC_MOB = var;
}
void pwm_pmc_config(BYTE  dcdr)
{
	spwm_str pwm_set_0;
	pwm_set_0.pwm_no = 0;
	pwm_set_0.prescale = 1;
	pwm_set_0.pwm_polarity = 0;
	pwm_set_0.pwm_softmode = 0;
	pwm_set_0.pwm_scale = 0;
	pwm_set_0.pwm_ctr = 100;
	pwm_set_0.pwm_clk_sel = 0;
	pwm_set_0.pwm_ctr_sel = 0;
	pwm_set_0.pwm_dcdr = dcdr;
	pwm_set_0.pwm_step = 0;
	PWM_Set(&pwm_set_0);
	PWM_Enable(0);
}
extern int GPIO_Config(int GPIO, int gpio_no, int mode, int op_val, int int_lv, int pol);
void PMC_PORT_TEST(BYTE  channel)
{
	BYTE  var;
	BYTE  cmd;
	BYTE  index;
	switch(channel)
	{
		case 0x1:
			var = PMC1_DIR;
			while(PMC1_STR & 0x1);
			PMC1_DOR = var;
		#if LPC_WAY_OPTION_SWITCH
		#else
			SET_BIT(PMC1_CTL, 0);
		#endif
			break;
		case 0x2:
			var = PMC2_DIR;
			while(PMC2_STR & 0x1);
			PMC2_DOR = var;
		#if LPC_WAY_OPTION_SWITCH
		#else
			SET_BIT(PMC2_CTL, 0);
		#endif
			break;
		case 0x3:
			var = PMC3_DIR;
			while(PMC3_STR & 0x1);
			PMC3_DOR = var;
		#if LPC_WAY_OPTION_SWITCH
		#else
			SET_BIT(PMC3_CTL, 0);
		#endif
			break;
		case 0x4:
			var = PMC4_DIR;
			while(PMC4_STR & 0x1);
			PMC4_DOR = var;
		#if LPC_WAY_OPTION_SWITCH
		#else
			SET_BIT(PMC4_CTL, 0);
		#endif
			break;
		case 0x5:
			if(PMC5_STR & 0x08)
			{
				cmd = PMC5_DIR;
				//dprint("PM5 Cmd = %d\n",cmd);
				while(PMC5_STR & 0x1);
				PMC5_DOR = cmd;
			#if LPC_WAY_OPTION_SWITCH
			#else
				SET_BIT(PMC5_CTL, 0);
			#endif
				switch(cmd)
				{
					case 0x80://ec space read
						while(!(PMC5_STR & 0x2));
						index = PMC5_DIR;
						while(PMC5_STR & 0x1);
						var = Read_Map_ECSPACE_BASE_ADDR(index);
						PMC5_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						dprint("Read_Map_ECSPACE_BASE_ADDR(%d) = %d\n", index, var);
						break;
					case 0x81://ec space write
						while(!(PMC5_STR & 0x2));
						index = PMC5_DIR;
						while(PMC5_STR & 0x1);
						PMC5_DOR = index;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						while(!(PMC5_STR & 0x2));
						var = PMC5_DIR;
						Write_Map_ECSPACE_BASE_ADDR(index, var);
						var = Read_Map_ECSPACE_BASE_ADDR(index);
						while(PMC5_STR & 0x1);
						PMC5_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						dprint("Write_Map_ECSPACE_BASE_ADDR(%d) = %d\n", index, var);
						break;
					case 0x02://PWM/tach
						while(!(PMC5_STR & 0x2));
						index = PMC5_DIR;
						while(PMC5_STR & 0x1);
						PMC5_DOR = index;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						while(!(PMC5_STR & 0x2));
						var = PMC5_DIR;
						if(index == 0x00)
						{
							pwm_pmc_config(var);
							sysctl_iomux_pwm();
						}
						else
						{
							pwm_pmc_config(var);
							sysctl_iomux_pwm();
						}
						sysctl_iomux_tach0();
						TACH_Init_Channel(0, 1, 0);
						(*(DWORD *)(0x3047c)) = 1 << 15;//GPIOD 6 pullup
						tach0 = 0x00;
						tach0 = 0x5b8d80 / TACH0_DATA;
						dprint("tach0 = %d\n", tach0);
						while(PMC5_STR & 0x1);
						PMC5_DOR = (BYTE)(tach0 & (0xff));
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						while(PMC5_STR & 0x1);
						PMC5_DOR = (BYTE)((tach0 & (0xff00)) >> 8);
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						break;
					case 0x01:
						while(!(PMC5_STR & 0x2));
						index = PMC5_DIR;
						while(PMC5_STR & 0x1);
						PMC5_DOR = index;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						while(!(PMC5_STR & 0x2));
						var = PMC5_DIR;
						GPIO_Config(index, 0, 1, var, 0, 0);
						GPIO_Config(index, 1, 1, var, 0, 0);
						GPIO_Config(index, 2, 1, var, 0, 0);
						GPIO_Config(index, 3, 1, var, 0, 0);
						GPIO_Config(index, 4, 1, var, 0, 0);
						GPIO_Config(index, 5, 1, var, 0, 0);
						GPIO_Config(index, 6, 1, var, 0, 0);
						GPIO_Config(index, 7, 1, var, 0, 0);
						while(PMC5_STR & 0x1);
						PMC5_DOR = var;
					#if LPC_WAY_OPTION_SWITCH
					#else
						SET_BIT(PMC5_CTL, 0);
					#endif
						break;
					case 0x03:
						while(!(PMC5_STR & 0x2));
						index = PMC5_DIR;
						if(index == 0x00)
						{
							var = 0x00;
							while(PMC5_STR & 0x1);
							PMC5_DOR = var;
						#if LPC_WAY_OPTION_SWITCH
						#else
							SET_BIT(PMC5_CTL, 0);
						#endif
							var = 0x00;
							while(PMC5_STR & 0x1);
							PMC5_DOR = var;
						#if LPC_WAY_OPTION_SWITCH
						#else
							SET_BIT(PMC5_CTL, 0);
						#endif
						}
						else if(index == 0x01)
						{
							I2c_Readb(I2C_CLR_STOP_DET_OFFSET, I2C_CHANNEL_1);//clear stop det 
							/* Master switch and TAR set */
							I2c_Master_Controller_Init(I2C_CHANNEL_1);
							I2c_Master_Set_Tar(0x4C, I2C_CHANNEL_1);
							/* read temperature */
							var = I2c_Master_Read_Byte(I2C_CHANNEL_1);
							while(PMC5_STR & 0x1);
							PMC5_DOR = var;
						#if LPC_WAY_OPTION_SWITCH
						#else
							SET_BIT(PMC5_CTL, 0);
						#endif
							var = I2c_Master_Read_Byte(I2C_CHANNEL_1);
							while(PMC5_STR & 0x1);
							PMC5_DOR = var;
						#if LPC_WAY_OPTION_SWITCH
						#else
							SET_BIT(PMC5_CTL, 0);
						#endif
						}
						if(0 == I2c_Check_Stop_Det(I2C_CHANNEL_1))
						{
							I2c_Slave_debugger_Init(I2C_CHANNEL_1);
						}
						break;
					case 0x5A:
					case 0xA5:
					default:
						break;
				}
			}
			else
			{
				var = PMC5_DIR;
				//dprint("PM5 DATA = %d\n",var);
				while(PMC5_STR & 0x1);
				PMC5_DOR = var;
			#if LPC_WAY_OPTION_SWITCH
			#else
				SET_BIT(PMC5_CTL, 0);
			#endif
			}
			break;
		default:
			break;
	}
}
#endif
#endif