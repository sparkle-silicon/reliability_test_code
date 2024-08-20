/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-09-13 10:32:33
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
#include <linux/module.h>//模块信息，最基本的文件，支持动态添加和卸载模块
#include <linux/init.h>//初始化、退出等信息
#include <linux/serio.h>//串行输入输出虚拟总线
#include <linux/platform_device.h>//平台设备（实现不同总线外设的统一驱动）
#include <linux/hwmon.h>//hardware monitoring framework硬件监视框架
#include <linux/hwmon-sysfs.h>//hwmon子系统的虚拟文件系统
#include <linux/pci.h>//pci设备总线相关
#include <linux/pci_ids.h>//pci的硬件相关信息
#include <linux/poll.h>//文件监视,事件监测
#include <linux/slab.h>//内存缓存管理ECE
#include <linux/proc_fs.h>//内核文件系统
#include <linux/miscdevice.h>//杂项设备
#include <linux/apm_bios.h>//BIOS电源管理方案
#include <linux/capability.h>//权限设置
#include <linux/sched.h>//进程相关信息
#include <linux/pm.h>//电源管理
#include <linux/apm-emulation.h>//apm仿真（用于无BIOS，如arm架构）
#include <linux/device.h>//设备
#include <linux/kernel.h>//内核
#include <linux/list.h>//链表
#include <linux/completion.h>//等待队列
#include <linux/kthread.h>//线程
#include <linux/delay.h>//延时
#include <linux/timer.h>//内核定时器
#include <linux/reboot.h>//重启、挂机等
#include <linux/input.h>//输入子系统
#include <linux/input/sparse-keymap.h>//输入键映射
//#include <linux/input-event-codes.h>//输入事件处理
#include <linux/fb.h>//帧缓冲驱动
#include <linux/power_supply.h>//供电设备
#include <asm/delay.h>//处理器延时
#include <asm/io.h>	//基本输入输出文件
#include "ec_wpce775l.h"
//EC_PHY_ADDRS
#define EC_I_O_BASE_ADDR (void *)0x18000000
#define EC_IRQ_BASE_ADDR (void *)0x10002000
#define EC_MEM_BASE_ADDR (void *)0x12000000
#define EC_OFFSET_MAX 0x1000
#define EC_IRQ_OFFSET_MAX 0x1000
#define EC_MEM_OFFSET_MAX 0x1000
#define EC_AP_ADDR0 0x2E 
#define EC_AP_ADDR1 0x4E 
#define EC_DP_ADDR0 0x2F 
#define EC_DP_ADDR1 0x4F 
#define EC_AP_ADDR EC_AP_ADDR0 
#define EC_DP_ADDR EC_DP_ADDR0
#define EC_LDN_CONFIG			0x07 /* Register: Logical device select */
//typedef dbg
//#define debug
#ifndef debug
#define debug printk
#else 
#undef debug
#define debug
#endif
//timer
#define delay mdelay(1);
#define ec_lock i8042_lock
struct ec_superio_info {
	unsigned char ldn;//logic device number register
	unsigned char *ld_name;//logic device name
	struct serio *port;
	int io1;
	int io2;
	int irq;
	int int_cnt;
};
struct ec_pnpcfg_info {
	void *PNP_AP_IR;
	void *PNP_DP_IR;
	void *LPC_IO_BASE_ADDR;
	void *LPC_MEM_BASE_ADDR;
	void *LPC_CTL_BASE_ADDR;
};
struct ec_pnpcfg_info host_pnpcfg={
	.PNP_AP_IR = (void *)EC_AP_ADDR0,//(default)
	.PNP_DP_IR = (void *)EC_DP_ADDR0,//(default)
	.LPC_IO_BASE_ADDR = (void *)EC_I_O_BASE_ADDR,
	.LPC_MEM_BASE_ADDR = (void *)EC_MEM_BASE_ADDR,
	.LPC_CTL_BASE_ADDR = (void *)EC_IRQ_BASE_ADDR,
};
struct ec_superio_info sio_info[24];
volatile void *i_o_base = NULL;//EC端地址
volatile void *irq_base = NULL;//EC端地址
volatile void *mem_base = NULL;//EC端地址
volatile void *addr_ptr = NULL;//EC端地址
volatile void *data_ptr = NULL;//EC端地址
extern spinlock_t ec_lock;
DEFINE_SPINLOCK(ec_lock);
void init_addr(void){//地址注册    
	i_o_base = ioremap((phys_t)host_pnpcfg.LPC_IO_BASE_ADDR,EC_OFFSET_MAX);
	irq_base = ioremap((phys_t)host_pnpcfg.LPC_CTL_BASE_ADDR,EC_IRQ_OFFSET_MAX);
	mem_base = ioremap((phys_t)host_pnpcfg.LPC_MEM_BASE_ADDR,EC_MEM_OFFSET_MAX);
	addr_ptr = (void *)((unsigned long long)i_o_base + (unsigned long long)host_pnpcfg.PNP_AP_IR);
	data_ptr = (void *)((unsigned long long)i_o_base + (unsigned long long)host_pnpcfg.PNP_DP_IR);
	debug("i_o_base = %p,\n",i_o_base);
	debug("irq_base = %p,\n",irq_base);
	debug("mem_base = %p,\n",mem_base);
	debug("addr_ptr = %p,\n",irq_base);
	debug("data_ptr = %p,\n",mem_base);
}
void exit_addr(void){//地址注销
	iounmap(host_pnpcfg.LPC_IO_BASE_ADDR);
	iounmap(host_pnpcfg.LPC_CTL_BASE_ADDR);
	iounmap(host_pnpcfg.LPC_MEM_BASE_ADDR);
}
static inline void superio_select(struct ec_superio_info *sio){
	(*((unsigned char*)(addr_ptr))) = (EC_LDN_CONFIG);
	(*((unsigned char*)(data_ptr))) = (char)(sio->ldn);
}
static inline void superio_outb(int reg, int val){
	(*((unsigned char*)(addr_ptr))) = (reg);
	(*((unsigned char*)(data_ptr))) = (val);
}
static inline int  superio_inb(int reg)
{
	(*((char*)(addr_ptr))) = (reg);
	return (*((char*)(data_ptr)));
}
static inline void
superio_enter(int reg)
{
	/*EC SPKAE10X FIRMWARE CONFIGURE SYSSCTL_PORTCFG   BIT[31:24](EFIR) AND  BIT[23:16](EFDR)
		EC FIREMWARE CODE LIKE :
			#define SIO_IR 0x2E//0x4E
			#define SIO_DR 0x2E//0x4F
			#define E2PNP_REQ 0x2
			#define SYSCTL_PORTCFG REG32(0x3040C)
			#define SYSCTL_SPCTL0 REG32(0x30410)
			SYSCTL_SPCTL0 |= BIT(27);
			SYSCTL_PORTCFG = (SIO_IR<<24)|(SIO_DR<<16)|E2PNP_REQ;
			SYSCTL_SPCTL0 &= ~BIT(27);
	*/
	debug("HOST CAN'T CONFIGURE EC SPKAE10X PLUG AND PLAY CONFIGURATION I/O PORT ADDRESS\n");
	return ;
}
static inline void  superio_exit(struct ec_superio_info *sio)
{
	superio_select(sio);
	(*((char*)(addr_ptr))) = (0x30);
	(*((char*)(data_ptr))) = 0;
}
static int superio_pnp_cfg(struct ec_superio_info *sio,int swuc_en,int irq_tp,int sio_en)
{
	superio_select(sio);//select super i/o
	if(sio->io1 <= 0xffff)//io base addr0
	{
		superio_outb(0x60,(sio->io1&0xff00)>>8);
		superio_outb(0x61,(sio->io1&0x00ff)>>0);
	}
	if(sio->io2 <= 0xffff)//io base addr1
	{
		superio_outb(0x62,(sio->io2&0xff00)>>8);
		superio_outb(0x63,(sio->io2&0x00ff)>>0);
	}
	if(swuc_en)superio_outb(0x70,(((sio->irq) &0xf)|(0x10)));//irq num 和 swuc en
	else superio_outb(0x70,((sio->irq) &0xf));//irq num
	superio_outb(0x71,irq_tp);//触发条件（高低电平&&上下边沿）
	superio_outb(0x30,((sio_en)?0x1:0x0));//打开
	if(sio_en)debug("Set Super I/O Logical Device %s,Number : %d\n",sio->ld_name,sio->ldn);
	else debug("Cancel Super I/O Logical Device %s,Number : %d\n",sio->ld_name,sio->ldn);
	if(sio_en)if(sio->irq)debug("Set Super I/O Logical Device %s IRQ : %d\n",sio->ld_name,sio->irq);
	return 0;
}
static int init_sio(void)
{
	//superio_outb(0x23,0x02);//reset sio
	//while (superio_inb(0x23) & 0x01 != 0x01)
	int sio_ser_no = 0;
    debug("Super I/O Control Register SET ENABLE\n");
	superio_outb(0x23,0x01);//set sio enable
	debug("Super I/O Control Register SET ENABLE,SIO Ctrl Reg Data = 0x%02x\n",superio_inb(0x23));
	sio_info[sio_ser_no].ldn = 0x1;
	sio_info[sio_ser_no].ld_name = "UARTA";
	sio_info[sio_ser_no].io1 = 0x2F8;
	sio_info[sio_ser_no].io2 = 0xffffffff;
	sio_info[sio_ser_no].irq = 0;//4;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,0,0);//UARTA
	
	sio_info[sio_ser_no].ldn = 0x2;
	sio_info[sio_ser_no].ld_name = "UARTB";
	sio_info[sio_ser_no].io1 = 0x3F8;
	sio_info[sio_ser_no].io2 = 0xffffffff;
	sio_info[sio_ser_no].irq = 0;//3;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,0,0);//UARTB
	
	sio_info[sio_ser_no].ldn = 0x3;
	sio_info[sio_ser_no].ld_name = "HWMON";
	sio_info[sio_ser_no].io1 = 0x100;
	sio_info[sio_ser_no].io2 = 0xffffffff;
	sio_info[sio_ser_no].irq = 0;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,0,0);//HWMON
	
	sio_info[sio_ser_no].ldn = 0x4;
	sio_info[sio_ser_no].ld_name = "HWUC";
	sio_info[sio_ser_no].io1 = 0x110;
	sio_info[sio_ser_no].io2 = 0xffffffff;
	sio_info[sio_ser_no].irq = 0;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,0,0);//HWUC
	
	sio_info[sio_ser_no].ldn = 0x5;
	sio_info[sio_ser_no].ld_name = "MOUSE_KBC";
	sio_info[sio_ser_no].io1 = 0x60;
	sio_info[sio_ser_no].io2 = 0x64;
	sio_info[sio_ser_no].irq = 12;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,1);//MOUSE_KBC
	
	sio_info[sio_ser_no].ldn = 0x6;
	sio_info[sio_ser_no].ld_name = "KEYBOARD_KBC";
	sio_info[sio_ser_no].io1 = 0x60;
	sio_info[sio_ser_no].io2 = 0x64;
	sio_info[sio_ser_no].irq = 1;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,1);//KEYBOARD_KBC
	
	sio_info[sio_ser_no].ldn = 0x8;
	sio_info[sio_ser_no].ld_name = "Parallel_Port";
	sio_info[sio_ser_no].io1 = 0x378;
	sio_info[sio_ser_no].io2 = 0xffffffff;
	sio_info[sio_ser_no].irq = 7;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,1);//Parallel_Port
	
	sio_info[sio_ser_no].ldn = 0xF;
	sio_info[sio_ser_no].ld_name = "SMFI";
	sio_info[sio_ser_no].io1 = 0x130;
	sio_info[sio_ser_no].io2 = 0xffffffff;
	sio_info[sio_ser_no].irq = 0;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,0);//SMFI
	
	sio_info[sio_ser_no].ldn = 0x10;
	sio_info[sio_ser_no].ld_name = "BRAM";
	sio_info[sio_ser_no].io1 = 0x140;
	sio_info[sio_ser_no].io2 = 0xffffffff;
	sio_info[sio_ser_no].irq = 0;//8;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,0);//BRAM
	
	sio_info[sio_ser_no].ldn = 0x11;
	sio_info[sio_ser_no].ld_name = "PMC1";
	sio_info[sio_ser_no].io1 = 0x62;
	sio_info[sio_ser_no].io2 = 0x66;
	sio_info[sio_ser_no].irq = 1;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,1);//PMC1
	
	sio_info[sio_ser_no].ldn = 0x12;
	sio_info[sio_ser_no].ld_name = "PMC2";
	sio_info[sio_ser_no].io1 = 0x68;
	sio_info[sio_ser_no].io2 = 0x6C;
	sio_info[sio_ser_no].irq = 0;//1;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,0);//HWMON
	
	sio_info[sio_ser_no].ldn = 0x13;
	sio_info[sio_ser_no].ld_name = "SSPI0";
	sio_info[sio_ser_no].io1 = 0x150;
	sio_info[sio_ser_no].io2 = 0xffffffff;
	sio_info[sio_ser_no].irq = 0;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,0);//SSPI0
	
	sio_info[sio_ser_no].ldn = 0x14;
	sio_info[sio_ser_no].ld_name = "SSPI1";
	sio_info[sio_ser_no].io1 = 0x158;
	sio_info[sio_ser_no].io2 = 0xffffffff;
	sio_info[sio_ser_no].irq = 0;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,0);//SSPI1
	
	sio_info[sio_ser_no].ldn = 0x15;
	sio_info[sio_ser_no].ld_name = "CAN0";
	sio_info[sio_ser_no].io1 = 0xF00;
	sio_info[sio_ser_no].io2 = 0xffffffff;
	sio_info[sio_ser_no].irq = 9;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,1);//CAN0
	
	sio_info[sio_ser_no].ldn = 0x16;
	sio_info[sio_ser_no].ld_name = "CAN1";
	sio_info[sio_ser_no].io1 = 0xE00;
	sio_info[sio_ser_no].io2 = 0xffffffff;
	sio_info[sio_ser_no].irq = 0;//10;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,0);//CAN1
	
	sio_info[sio_ser_no].ldn = 0x17;
	sio_info[sio_ser_no].ld_name = "PMC3";
	sio_info[sio_ser_no].io1 = 0x6A;
	sio_info[sio_ser_no].io2 = 0x6E;
	sio_info[sio_ser_no].irq = 0;//1;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,0);//PMC3
	
	sio_info[sio_ser_no].ldn = 0x18;
	sio_info[sio_ser_no].ld_name = "PMC4";
	sio_info[sio_ser_no].io1 = 0x74;
	sio_info[sio_ser_no].io2 = 0x78;
	sio_info[sio_ser_no].irq = 0;//1;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,0);//PMC4
	
	sio_info[sio_ser_no].ldn = 0x19;
	sio_info[sio_ser_no].ld_name = "PMC5";
	sio_info[sio_ser_no].io1 = 0x7A;
	sio_info[sio_ser_no].io2 = 0x7C;
	sio_info[sio_ser_no].irq = 0;//1;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,0);//PMC3
	
	sio_info[sio_ser_no].ldn = 0x1A;
	sio_info[sio_ser_no].ld_name = "CAN2";
	sio_info[sio_ser_no].io1 = 0xD00;
	sio_info[sio_ser_no].io2 = 0xffffffff;
	sio_info[sio_ser_no].irq = 0;//9;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,0);//CAN2
	
	sio_info[sio_ser_no].ldn = 0x1B;
	sio_info[sio_ser_no].ld_name = "UART0";
	sio_info[sio_ser_no].io1 = 0x4F8;
	sio_info[sio_ser_no].io2 = 0xffffffff;
	sio_info[sio_ser_no].irq = 4;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,1);//UART0
	
	sio_info[sio_ser_no].ldn = 0x1C;
	sio_info[sio_ser_no].ld_name = "UART1";
	sio_info[sio_ser_no].io1 = 0x5F8;
	sio_info[sio_ser_no].io2 = 0xffffffff;
	sio_info[sio_ser_no].irq = 3;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,1);//UART1
	
	sio_info[sio_ser_no].ldn = 0x1D;
	sio_info[sio_ser_no].ld_name = "UART2";
	sio_info[sio_ser_no].io1 = 0x6F8;
	sio_info[sio_ser_no].io2 = 0xffffffff;
	sio_info[sio_ser_no].irq = 0;//4;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,0);//UART2
	
	sio_info[sio_ser_no].ldn = 0x1E;
	sio_info[sio_ser_no].ld_name = "UART3";
	sio_info[sio_ser_no].io1 = 0x7F8;
	sio_info[sio_ser_no].io2 = 0xffffffff;
	sio_info[sio_ser_no].irq = 0;//3;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,0);//UART3
	
	sio_info[sio_ser_no].ldn = 0x1F;
	sio_info[sio_ser_no].ld_name = "CAN3";
	sio_info[sio_ser_no].io1 = 0xC00;
	sio_info[sio_ser_no].io2 = 0xffffffff;
	sio_info[sio_ser_no].irq = 0;//10;
	sio_info[sio_ser_no].port = kzalloc(sizeof(struct serio),GFP_KERNEL);if(!sio_info[sio_ser_no].port)return -ENOMEM;
	superio_pnp_cfg(&sio_info[sio_ser_no++],0,1,0);//CAN3
	return 0;
}
static int exit_sio(void)
{
	int cnt = 0;
	while(cnt<24)
	{
		superio_exit(&sio_info[cnt]);
	}
	return 0;
}
void uart_puts(struct serio * serio,unsigned char *ch)
{
	int cnt = 0;
	int	timeout = 1000;
	while(cnt<24)
	{
		if(strcmp(serio->name,sio_info[cnt].port->name))break;
		else cnt++;
	}
	*(unsigned char *)(i_o_base + sio_info[cnt].io1+0x3) &= 0x7F;//UARTn_LCR 选择发送接收缓冲
	while(*ch)
	{
		delay;
		while ((!(*(unsigned char *)(i_o_base + sio_info[cnt].io1+0x5) & 0x40))&&timeout--) {udelay(100);}
		if(timeout)*(unsigned char *)(i_o_base + sio_info[cnt].io1) = *ch;
		else {debug("SPKAE10X %s write %c(0x%2x) error\n",sio_info[cnt].ld_name,*ch,*ch);return;} 
		ch++;
	}
}
static int ec_port_open(struct serio * serio)
{
	int cnt = 0;
	//int temp = 0;
	//int ret = 0;
	while(cnt<24)
	{
		if(strcmp(serio->name,sio_info[cnt].port->name))break;
		else cnt++;
	}
		switch (sio_info[cnt].ldn)
	{
	case 0x1://UARTA
	case 0x2://UARTB
	case 0x1B://UART0
	case 0x1C://UART1
	case 0x1D://UART2
	case 0x1E://UART3
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x3) = 0x83;
		// temp = ((24576000/115200)+4)>>3;
		// if(sio_info[cnt].ldn<=0x2)temp>>1;
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x0) = temp&0xff;
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x1) = (temp>>8)&0xff;
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x2) = 0x27;
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x3) &= 0x7f;
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x1) |= 0x81;
		// ret = uart_puts(serio,"HOST EC SPKAE10X CONFIG NOW\n");
		// if(ret == -1){
		// 	debug("SPKAE10X %s no open %c(0x%2x)\n",sio_info[cnt].ld_name,c,c);
		// 	return -1;
		// }
		// delay;
		break;
	case 0x5://mouse
	case 0x6://kayboard
	case 0x11://pmc1
	case 0x12://pmc2
	case 0x17://pmc3
	case 0x18://pmc4
	case 0x19://pmc5
		break;	
	case 0x8://PP
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x2) &= 0xdf;
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x2) |= 0x10;
		break;
	case 0x10://BRAM
	case 0x3://HWMON
	case 0x4://HWUC
	case 0xf://SMFI
	case 0x13://SSPI
	case 0x14://SSPI
		break;	
	case 0x15://CAN0
	case 0x16://CAN1
	case 0x1A://CAN2
	case 0x1F://CAN3
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x0) = 0x9;
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x5) = 0x00;
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x6) = 0x28;
		
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x10) = 0x23;
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x11) = 0x01;
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x12) = 0x00;
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x13) = 0x00;
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x14) = 0x00;
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x15) = 0x80;
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x16) = 0xff;
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x17) = 0xff;
		// *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x0) &= ~0x1;
		break;	
	default:
		debug("SPKAE10X %s no open\n",sio_info[cnt].ld_name);
		return -1;
		break;
	}
	debug("SPKAE10X %s open \n",sio_info[cnt].ld_name);
	return 0;
}
static int ec_port_write(struct serio * serio,unsigned char c)
{
	int cnt = 0;
	int timeout = 0;
	int select_offset[24];
	int can_offset = 0;
	int init_write =1;
	if(init_write)
	{
		memset(select_offset,0xff,sizeof(int)*24);
		init_write=0;
	}
	while(cnt<24)
	{
		if(strcmp(serio->name,sio_info[cnt].port->name))break;
		else cnt++;
	}
	switch (sio_info[cnt].ldn)
	{
	case 0x1://UARTA
	case 0x2://UARTB
	case 0x1B://UART0
	case 0x1C://UART1
	case 0x1D://UART2
	case 0x1E://UART3
		timeout = 1000;
		*(unsigned char *)(i_o_base + sio_info[cnt].io1+0x3) &= 0x7F;//UARTn_LCR 选择发送接收缓冲
		delay;
		while ((!(*(unsigned char *)(i_o_base + sio_info[cnt].io1+0x5) & 0x40))&&timeout--) {udelay(100);}
		if(timeout)*(unsigned char *)(i_o_base + sio_info[cnt].io1) = c;
		else {debug("SPKAE10X %s write %c(0x%2x) error\n",sio_info[cnt].ld_name,c,c);return -1;} 
		delay;
		break;
	case 0x5://mouse
	case 0x6://kayboard
	case 0x11://pmc1
	case 0x12://pmc2
	case 0x17://pmc3
	case 0x18://pmc4
	case 0x19://pmc5
		if(select_offset[cnt] == 0x1)
		{
			timeout = 1000;
			while ((!(*(unsigned char *)(i_o_base + sio_info[cnt].io2) & 0x02))&&timeout--) {udelay(100);}
			if(timeout)*(unsigned char *)(i_o_base + sio_info[cnt].io1) = c;
			else {debug("SPKAE10X %s write %c(0x%2x) error\n",sio_info[cnt].ld_name,c,c);return -1;} 
			select_offset[cnt] = 0xff;
			delay;
		}
		else if(select_offset[cnt] == 0x2)
		{
			while ((!(*(unsigned char *)(i_o_base + sio_info[cnt].io2) & 0x02))&&timeout--) {udelay(100);}
			if(timeout)*(unsigned char *)(i_o_base + sio_info[cnt].io2) = c;
			else {debug("SPKAE10X %s write %c(0x%2x) error\n",sio_info[cnt].ld_name,c,c);return -1;} 
			select_offset[cnt] = 0xff;  
			delay;
		}
		else select_offset[cnt]=c;
		break;	
	case 0x10://BRAM
	case 0x8://PP
	case 0x3://HWMON
	case 0x4://HWUC
	case 0xf://SMFI
	case 0x13://SSPI
	case 0x14://SSPI
		if(select_offset[cnt] != 0xff)
		{
			*(unsigned char *)(i_o_base + sio_info[cnt].io1+select_offset[cnt]) = c;
			select_offset[cnt]=0xff;
			delay;
		}
		else{
			select_offset[cnt]=c;
		}
		break;	
	case 0x15://CAN0
	case 0x16://CAN1
	case 0x1A://CAN2
	case 0x1F://CAN3
		if(select_offset[cnt] != 0xff){
		if(!can_offset)
		{
			while((!((*(unsigned char *)(i_o_base + sio_info[cnt].io1+0x2))&0x4))&&timeout--) {udelay(100);}  //等待发送缓冲可用
			if(timeout){}
			else
			{
				select_offset[cnt] = 0xff;
				debug("SPKAE10X %s write %c(0x%2x) error\n",sio_info[cnt].ld_name,c,c);
				return -1;
			}
		}
		if(select_offset[cnt])
		{
			*(unsigned char *)(i_o_base + sio_info[cnt].io1+select_offset[cnt]+0x10+can_offset) = c;
			can_offset++;
			select_offset[cnt]--;
			delay;
		}
		if(select_offset[cnt]){}
		else
		{
			*(unsigned char *)(i_o_base + sio_info[cnt].io1+0x1)=0x1;
			while((!((*(unsigned char *)(i_o_base + sio_info[cnt].io1+0x2))&0x4))&&timeout--) {udelay(100);}
			if(timeout){
				can_offset=0;
				select_offset[cnt]=0xff;
			}
			else
			{
				debug("SPKAE10X %s write timeout\n",sio_info[cnt].ld_name);
			}
		}
		}else{
			select_offset[cnt]=c;
		}
		break;	
	default:
		debug("SPKAE10X %s no write %c(0x%2x)\n",sio_info[cnt].ld_name,c,c);
		return -1;
		break;
	}
	debug("SPKAE10X %s write %#x\n",serio->name,c);
	return 0;
}
static void ec_port_close(struct serio * serio)
{
	int cnt = 0;
	while(cnt<24)
	{
		if(strcmp(serio->name,sio_info[cnt].port->name))break;
		else cnt++;
	}
	debug("SPKAE10X %s close \n",sio_info[cnt].ld_name);
}
irqreturn_t ec_interrupt(int irq,void * dev_id)
{
	unsigned int io_flags = 0;
	unsigned int  str,data;
	int cnt = 0;
	int can_data[16]={0};
	int can_cnt = 0;
	unsigned long flag = irq;
	spin_lock_irqsave(&ec_lock,flag);
	str = 0;
	data = 0;
	cnt = 0;
	while(cnt<24)
	{
		if(dev_id == (void *)&sio_info[cnt])break;
		else cnt++;
	}
	if(cnt == 23)goto out;
		switch (sio_info[cnt].ldn)
	{
	case 0x1://UARTA
	case 0x2://UARTB
	case 0x1B://UART0
	case 0x1C://UART1
	case 0x1D://UART2
	case 0x1E://UART3
		 *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x3) &= 0x7F;//UARTn_LCR 选择发送接收缓冲
		 do
		 {
			str = *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x5);//获取状态
		 } while (/* condition */);
		 data = *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x0);//获取数据
		delay;
		break;
	case 0x5://mouse
	case 0x6://kayboard
	case 0x11://pmc1
	case 0x12://pmc2
	case 0x17://pmc3
	case 0x18://pmc4
	case 0x19://pmc5
		str = *(unsigned char *)(i_o_base + sio_info[cnt].io2);//读状态
		data = *(unsigned char *)(i_o_base + sio_info[cnt].io1);//读数据
		break;	
	case 0x8://PP
		str = *(unsigned char *)(i_o_base + sio_info[cnt].io1+1);//状态寄存器
		data = *(unsigned char *)(i_o_base + sio_info[cnt].io1+0);//数据寄存器
		break;
	case 0x4://HWUC
		str = *(unsigned char *)(i_o_base + sio_info[cnt].io1+0);//状态寄存器
		data = *(unsigned char *)(i_o_base + sio_info[cnt].io1+6);//监控寄存器
		break;
	case 0x3://HWMON
		str = *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x12);//FIFO状态位
		data = *(unsigned char *)(i_o_base + sio_info[cnt].io1+0xA);//数据寄存器
		break;	
	
	case 0x15://CAN0
	case 0x16://CAN1
	case 0x1A://CAN2
	case 0x1F://CAN3
		str = *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x3);//状态寄存器
		if(str & 0x1)
		str = *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x0C);//状态寄存器
		can_cnt = 0;
		while(can_cnt < 0x8)
		{
			can_data[can_cnt] = *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x15+can_cnt);//数据寄存器
			debug("SPKAE10X %s RX FIFO %d DATA 0x%08x\n",sio_info[cnt].ld_name,can_cnt,can_data[can_cnt]);
		}
		data = can_data[0];
        *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x1) = (0x04); //释放接收fifo
		break;
	case 0x13://SSPI
	case 0x14://SSPI
		str = *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x9);//状态寄存器
		if(str&0x10)
		{
			str = *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x3);//状态寄存器
			data = *(unsigned char *)(i_o_base + sio_info[cnt].io1+0xa)|((*(unsigned char *)(i_o_base + sio_info[cnt].io1+0xb))<<8);//
		}
		break;
		case 0xf://SMFI
		str = *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x1)|((*(unsigned char *)(i_o_base + sio_info[cnt].io1+0x2))<<8)|((*(unsigned char *)(i_o_base + sio_info[cnt].io1+0x3))<<16);//状态寄存器
		data = *(unsigned char *)(i_o_base + sio_info[cnt].io1+0x6)|((*(unsigned char *)(i_o_base + sio_info[cnt].io1+0x7))<<8)|((*(unsigned char *)(i_o_base + sio_info[cnt].io1+0x8))<<16)|((*(unsigned char *)(i_o_base + sio_info[cnt].io1+0x9))<<24);//状态寄存器
		break;
	case 0x10://BRAM
	default:
		debug("SPKAE10X %s no internet\n",sio_info[cnt].ld_name);
		return -1;
		break;
	}
	sio_info[cnt].int_cnt ++;
	debug("SPKAE10X LOGICAL DEVICE %s io2[%02x] %#x,io1[%02x] %#x\n",\
	sio_info[cnt].ld_name,sio_info[cnt].io2,str,sio_info[cnt].io1,data);
	serio_interrupt(sio_info[cnt].port, data, io_flags);
	spin_unlock_irqrestore(&ec_lock,irq);
out:
	return (IRQ_HANDLED);
}
struct platform_device *ec_platform_device = NULL;
static int ec_probe(struct platform_device * pdev)
{
	int ret =0 ;
	int cnt = 0;
	init_sio();
	debug("Enter ec_probe \n");
	ec_platform_device = pdev;
	
	while (cnt < 23)
	{
		if(sio_info[cnt].port)
		{
			sio_info[cnt].port->id.type = SERIO_8042;
			sio_info[cnt].port->dev.parent = &pdev->dev;
			sio_info[cnt].port->open = ec_port_open;
			sio_info[cnt].port->close = ec_port_close;
			sio_info[cnt].port->write = ec_port_write;
			sio_info[cnt].port->lock = ec_lock;
			snprintf(sio_info[cnt].port->name, sizeof(sio_info[cnt].port->name),sio_info[cnt].ld_name);
			snprintf(sio_info[cnt].port->phys, sizeof(sio_info[cnt].port->phys),"ae10x_%s/serio%d",sio_info[cnt].ld_name,sio_info[cnt].ldn);
			serio_register_port(sio_info[cnt].port);
		}
		if(sio_info[cnt].irq)
		{
			ret = request_irq(sio_info[cnt].irq,ec_interrupt,IRQF_SHARED|IRQF_TRIGGER_MASK,sio_info[cnt].ld_name,(void *)&sio_info[cnt]);
			if(ret < 0)
				debug("%s Request_irq failed error %d \n",sio_info[cnt].ld_name,ret);
			else {
				debug("%s Request_irq OK.\n",sio_info[cnt].ld_name);
			}
			(*((int*)(irq_base))) |= 0x80000000; //open SIRQ interrupt enable control
			(*((int*)(irq_base + 0x4))) |= 0x1 << sio_info[cnt].irq;
			(*((int*)(irq_base + 0x10))) |= 0x1 << sio_info[cnt].irq;
		}
		cnt++;
	}
	delay;
	delay;
	delay;
	return 0;
}
static int ec_remove(struct platform_device * pdev)
{
	int cnt = 0;
	debug("ec_remove function \n");
	delay;
	
	while (cnt < 23)
	{
		if(sio_info[cnt].port)
		{
			serio_unregister_port(sio_info[cnt].port);
			kzfree(sio_info[cnt].port);
		}
		if(sio_info[cnt].irq)
		{
			free_irq(sio_info[cnt].irq,(void *)&sio_info[cnt]);
			
		}
		sio_info[cnt].port=NULL;
		cnt++;
		
	}
	delay;
	(*((int*)(irq_base + 0x4))) = 0x00;//[17:0] LPC_INT_EN 关闭 [31]FIRMWARE TYPE 访问类型
	(*((int*)(irq_base + 0x10))) = 0xffffffff;
	exit_sio();
	return 0;
}
static struct platform_driver sio_driver = {
	.driver = {
		.name = "SPKAE10X",
		.owner = THIS_MODULE,
	},
	.probe = ec_probe,
	.remove = ec_remove,
};
static struct platform_device * sio_device;
static int __init sio_init(void)
{
	// struct resource res = {
	// 	.start	=  EC_I_O_BASE_ADDR,
	// 	.end	= EC_I_O_BASE_ADDR + 0x1000,
	// 	.name	= "SPKAE10X",
	// 	.flags	= IORESOURCE_IO,
	// };
    int ret=0;
    init_addr();
	ret = platform_driver_register(&sio_driver);
	if (ret)
	{
		pr_err("SPKAE10X platform_driver_register error \n");
        exit_addr();
		return -ENODEV;
	}
	sio_device = platform_device_alloc("SPKAE10X", 0);
	if (!sio_device) 
	{
		pr_err("Device allocation failed\n");
		platform_driver_unregister(&sio_driver);
        exit_addr();
		return -ENODEV;
	}
	// ret = platform_device_add_resources(sio_device, &res, 1);
	// if (ret) {
	// 	pr_err("Device resource addition failed (%d)\n", ret);
	// 	platform_device_put(sio_device);
	// 	platform_driver_unregister(&sio_driver);
    //     exit_addr();
	// 	return -ENODEV;	}
	ret = platform_device_add_data(sio_device, sio_info,
				       (sizeof(struct ec_superio_info )*24));
	if (ret) {
		pr_err("Platform data allocation failed\n");
		platform_device_put(sio_device);
		platform_driver_unregister(&sio_driver);
        exit_addr();
		return -ENODEV;
	}
	ret = platform_device_add(sio_device);
	if (ret) {
		pr_err("Device addition failed (%d)\n", ret);
		platform_device_put(sio_device);
		platform_driver_unregister(&sio_driver);
        exit_addr();
		return -ENODEV;
	}
    return ret;
}
static void __exit sio_exit(void)
{
    platform_device_unregister(sio_device);
    platform_driver_unregister(&sio_driver);
    exit_addr();
}
Module_init(sio_init);//模块初始化
module_exit(sio_exit);//模块注销
MODULE_LICENSE("GPL");//版权声明
MODULE_AUTHOR("yunfeng@Sparkle Silicon Technology Corp., Ltd.");//作者声明
MODULE_DESCRIPTION("For study EC platform module and serio Super I/O demo");//模块描述
MODULE_ALIAS("SPKAE10X");//模块别名
MODULE_VERSION("v2023.09.01");//模块版本
