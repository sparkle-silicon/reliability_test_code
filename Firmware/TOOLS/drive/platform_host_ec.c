/*
 * @Author: Linyu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2023-09-08 15:31:23
 * @Description:
 *
 *
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 以下是中文及英文版权同步声明，编码为UTF-8。
 * Copyright has legal effects and violations will be prosecuted.
 * 版权具有法律效力，违反必究。
 *
 * Copyright ©2021-2025 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2025龙晶石半导体科技（苏州）有限公司
 */
#define LOCK_DEF 1//锁
#define IRQ_FUNC 1//中断
#define PLATFORM_RUN 1//平台设备
#define HEAD_FILE 1//头文件
#if HEAD_FILE
#include <linux/module.h>//模块信息，最基本的文件，支持动态添加和卸载模块
#include <linux/init.h>//初始化、退出等信息
#include <linux/serio.h>//串行输入输出虚拟总线
#include <linux/platform_device.h>//平台设备（实现不同总线外设的统一驱动）
#include <linux/hwmon.h>//hardware monitoring framework硬件监视框架
#include <linux/hwmon-sysfs.h>//hwmon子系统的虚拟文件系统
#include <linux/pci.h>//pci设备总线相关
#include <linux/pci_ids.h>//pci的硬件相关信息
#include <linux/poll.h>//文件监视,事件监测
#include <linux/slab.h>//内存缓存管理
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
#endif
#define EC_MOD_DEF 1//宏定义
#if EC_MOD_DEF
	//typedef abb
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long long  u64;
typedef short s16;
typedef int   s32;
typedef long long  s64;
#define dbg 	printk
#define I8042_STR_PARITY	0x80
#define I8042_STR_TIMEOUT	0x40
#define I8042_STR_AUXDATA	0x20
#define I8042_STR_KEYLOCK	0x10
#define I8042_STR_CMDDAT	0x08
#define I8042_STR_MUXERR	0x04
#define I8042_STR_IBF		0x02
#define I8042_STR_OBF		0x01
/* Power info cached timeout */
#define POWER_INFO_CACHED_TIMEOUT	100	/* jiffies */
//define statues of fan control
#define CONSTANT_SPEED_POLICY	0  /* at constant speed */
#define STEP_SPEED_POLICY	1  /* use up/down arrays to describe policy */
#define KERNEL_HELPER_POLICY	2  /* kernel as a helper to fan control */
//EC_PHY_ADDRS
#define EC_BASE_ADDR (void *)0x18000000
#define EC_IRQ_BASE_ADDR (char *)0x10002000
#define EC_OFFSET_MAX 0x1000
#define EC_IRQ_OFFSET_MAX 0x1000
//timer
#define delay mdelay(1);
#ifndef EC_REG_DELAY
#define EC_REG_DELAY		30000
#endif
#define SIO_INDEX_PORT		0x2E
#define SIO_DATA_PORT		0x2F
#define EC_CMD_TIMEOUT		0x1000
#define EC_SEND_TIMEOUT		0xffff
#define EC_RECV_TIMEOUT		0xffff
#define EC_CMD_PORT		0x66
#define EC_STS_PORT		0x66
#define EC_DAT_PORT		0x62
#define CMD_READ_EC		0x80	/* Read EC command. */
#define CMD_WRITE_EC		0x81	/* Write EC command. */
#define CMD_GET_EVENT_NUM	0x84	/* Query EC command, for get SCI event number. */
#define CMD_RESET		0x4E	/* Reset and poweroff the machine auto-clear: rd/wr */
#define CMD_EC_VERSION		0x4F	/* EC Version OEM command: 36 Bytes */
#define EC_IO_PORT_LOW EC_CMD_PORT
#define EC_IO_PORT_DATA EC_DAT_PORT
#define EC_IO_PORT_HIGH EC_STS_PORT
/* >>> Read/Write temperature & fan index for ACPI 80h/81h command. */
#define INDEX_TEMPERATURE_VALUE		0x1B	/* Current CPU temperature value, Read and Write(81h command). */
#define INDEX_FAN_MAXSPEED_LEVEL	0x5B	/* Fan speed maxinum levels supported. Defaut is 6. */
#define INDEX_FAN_SPEED_LEVEL		0x5C	/* FAn speed level. [0,5] or [0x06, 0x38]*/
#define INDEX_FAN_CTRLMOD		0x5D	/* Fan control mode, 0 = by EC, 1 = by Host.*/
#define INDEX_FAN_STSCTRL		0x5E	/* Fan status/control, 0 = stop, 1 = run. */
#define INDEX_FAN_ERRSTS		0x5F	/* Fan error status, 0 = no error, 1 = has error. */
#define INDEX_FAN_SPEED_LOW		0x08	/* Fan speed low byte.*/
#define INDEX_FAN_SPEED_HIGH		0x09	/* Fan speed high byte. */
/* <<< End Temp & Fan */
/* >>> Read/Write LCD backlight information/control index for ACPI 80h/81h command. */
#define INDEX_BACKLIGHT_CTRLMODE	0x57	/* LCD backlight control mode: 0 = by EC, 1 = by HOST */
#define INDEX_BACKLIGHT_STSCTRL		0x58	/* LCD backlight status or control: 0 = turn off, 1 = turn on */
#define	INDEX_DISPLAY_MAXBRIGHTNESS_LEVEL	0x59	/* LCD backlight brightness max level */
#define	INDEX_DISPLAY_BRIGHTNESS	0x5A	/* 10 stages (0~9) LCD backlight brightness adjust */
/* <<< End Backlight */
#define INDEX_TOUCHPAD_ENABLE_LED	0x56
/* >>> Read battery(BQ3060) index for ACPI 80h command */
/*
* The reported battery die temperature.
* The temperature is expressed in units of 0.25 seconds and is updated every 2.56 seconds.
* The equation to calculate reported pack temperature is:
* Temperature = 0.1 * (256 * TEMPH + TEMPL) Kelvin
* Temperature -= 273 Degrees Celsius
* The host sytem has read-only access to this register pair.
*/
#define INDEX_BATTERY_TEMP_LOW		0x20	/* Battery temperature low byte. */
#define INDEX_BATTERY_TEMP_HIGH		0x21	/* Battery temperature high byte. */
#define INDEX_BATTERY_VOL_LOW		0x22	/* Battery Voltage Low byte. */
#define INDEX_BATTERY_VOL_HIGH		0x23	/* Battery Voltage High byte. */
#define INDEX_BATTERY_CURRENT_LOW	0x24	/* Battery Current Low byte. */
#define INDEX_BATTERY_CURRENT_HIGH	0x25	/* Battery Current High byte. */
#define INDEX_BATTERY_AC_LOW		0x26	/* Battery AverageCurrent Low byte. */
#define INDEX_BATTERY_AC_HIGH		0x27	/* Battery AverageCurrent High byte. */
#define INDEX_BATTERY_CAPACITY		0x2A	/* Battery RemainingCapacity percent. */
#define INDEX_BATTERY_STATUS_LOW	0x2C	/* Battery Status low byte. */
#define INDEX_BATTERY_STATUS_HIGH	0x2D	/* Battery Status high byte. */
#define INDEX_BATTERY_RC_LOW		0x2E	/* Battery RemainingCapacity Low byte. */
#define INDEX_BATTERY_RC_HIGH		0x2F	/* Battery RemainingCapacity High byte. */
#define INDEX_BATTERY_ATTE_LOW		0x30	/* Battery AverageTimeToEmpty Low byte. */
#define INDEX_BATTERY_ATTE_HIGH		0x31	/* Battery AverageTimeToEmpty High byte. */
#define INDEX_BATTERY_ATTF_LOW		0x32	/* Battery AverageTimeToFull Low byte. */
#define INDEX_BATTERY_ATTF_HIGH		0x33	/* Battery AverageTimeToFull High byte. */
#define INDEX_BATTERY_FCC_LOW		0x34	/* Battery FullChargeCapacity Low byte. */
#define INDEX_BATTERY_FCC_HIGH		0x35	/* Battery FullChargeCapacity High byte. */
#define INDEX_BATTERY_CC_LOW		0x36	/* Battery ChargingCurrent Low byte. */
#define INDEX_BATTERY_CC_HIGH		0x37	/* Battery ChargingCurrent High byte. */
#define INDEX_BATTERY_CV_LOW		0x38	/* Battery ChargingVoltage Low byte. */
#define INDEX_BATTERY_CV_HIGH		0x39	/* Battery ChargingVoltage High byte. */
#define INDEX_BATTERY_CHGSTS_LOW	0x3A	/* Battery ChargingStatus Low byte. */
#define INDEX_BATTERY_CHGSTS_HIGH	0x3B	/* Battery ChargingStatus High byte. */
#define INDEX_BATTERY_CYCLECNT_LOW	0x3C	/* Battery CycleCount Low byte. */
#define INDEX_BATTERY_CYCLECNT_HIGH	0x3D	/* Battery CycleCount High byte. */
/* Battery static information. */
#define INDEX_BATTERY_DC_LOW		0x60	/* Battery DesignCapacity Low byte. */
#define INDEX_BATTERY_DC_HIGH		0x61	/* Battery DesignCapacity High byte. */
#define INDEX_BATTERY_DV_LOW		0x62	/* Battery DesignVoltage Low byte. */
#define INDEX_BATTERY_DV_HIGH		0x63	/* Battery DesignVoltage High byte. */
#define INDEX_BATTERY_MFD_LOW		0x64	/* Battery ManufactureDate Low byte. */
#define INDEX_BATTERY_MFD_HIGH		0x65	/* Battery ManufactureDate High byte. */
#define INDEX_BATTERY_SN_LOW		0x66	/* Battery SerialNumber Low byte. */
#define INDEX_BATTERY_SN_HIGH		0x67	/* Battery SerialNumber High byte. */
#define INDEX_BATTERY_MFN_LENG		0x68	/* Battery ManufacturerName string length. */
#define INDEX_BATTERY_MFN_START		0x69	/* Battery ManufacturerName string start byte. */
#define INDEX_BATTERY_DEVNAME_LENG	0x74	/* Battery DeviceName string length. */
#define INDEX_BATTERY_DEVNAME_START	0x75	/* Battery DeviceName string start byte. */
#define INDEX_BATTERY_DEVCHEM_LENG	0x7C	/* Battery DeviceChemitry string length. */
#define INDEX_BATTERY_DEVCHEM_START	0x7D	/* Battery DeviceChemitry string start byte. */
#define INDEX_BATTERY_MFINFO_LENG	0x81	/* Battery ManufacturerInfo string length. */
#define INDEX_BATTERY_MFINFO_START	0x82	/* Battery ManufacturerInfo string start byte. */
#define INDEX_BATTERY_CELLCNT_START	0x95    /* Battery packaging fashion string start byte(=4). Unit: ASCII. */
#define BATTERY_CELLCNT_LENG	  	4		/* Battery packaging fashion string size. */
#define FLAG_BAT_CELL_3S1P 		"3S1P"
#define BIT_BATTERY_CURRENT_PN		7       /* Battery current sign is positive or negative */
#define BIT_BATTERY_CURRENT_PIN		0x07	/* Battery current sign is positive or negative.*/
/* <<< End Battery */
#define MASK(x)	(1 << x)
#define INDEX_STOPCHG_STATUS	0xA1	/* Read currently stop charge status. */
#define INDEX_POWER_STATUS		0xA2	/* Read current power status. */
#define INDEX_DEVICE_STATUS		0xA3	/* Read Current Device Status */
#define INDEX_SHUTDOWN_ID		0xA4	/* Read Shutdown ID */
#define INDEX_SYSTEM_CFG		0xA5		/* Read System config */
#define INDEX_VOLUME_LEVEL		0xA6		/* Read Volume Level command */
#define INDEX_VOLUME_MAXLEVEL		0xA7		/* Volume MaxLevel */   
/* Camera control */
#define INDEX_CAM_STSCTRL			0xAA
/* data destroy led control */
#define INDEX_DATA_DESTROY			0xB0
/* The led of board healthy */
#define INDEX_BOARD_HEALTHY			0xB1
#define VOLUME_MAX_LEVEL		0x0A		/* Volume level max is 11 */
#define BIT_SYSCFG_TPSWITCH		(1 << 0)	/* TouchPad switch */
#define BIT_SYSCFG_WLANPRES		(1 << 1)	/* WLAN present */
#define BIT_SYSCFG_NB3GPRES		(1 << 2)	/* 3G present */
#define BIT_SYSCFG_CAMERAPRES		(1 << 3)	/* Camera Present */
#define BIT_SYSCFG_VOLCTRLEC		(1 << 4)	/* Volume control by EC */
#define BIT_SYSCFG_BLCTRLEC		(1 << 5)	/* Backlight control by EC */
#define BIT_SYSCFG_AUTOBRIGHT		(1 << 7)	/* Auto brightness */
/* EC_SC input */
/* EC Status query, by direct read 66h port. */
#define EC_SMI_EVT		(1 << 6)	/* 1 = SMI event padding */
#define EC_SCI_EVT		(1 << 5)	/* 1 = SCI event padding */
#define EC_BURST		(1 << 4)	/* 1 = Controller is in burst mode */
#define EC_CMD			(1 << 3)	/* 1 = Byte in data register is command */
#define EC_IBF			(1 << 1)	/* 1 = Input buffer full (data ready for ec) */
#define EC_OBF			(1 << 0)	/* 1 = Output buffer full (data ready for host) */
/* SCI Event Number from EC */
#define MAX_STEP_NUM	16
#define MAX_FAN_LEVEL	255
#define SCI_EVENT_NUM_START		SCI_EVENT_NUM_WLAN
#define SCI_EVENT_NUM_END		SCI_EVENT_RECOVERY
#endif 
#define VAL 1//值
#if VAL
#define ADDRS_DEF 1//地址定义
#if ADDRS_DEF
char *base = NULL;//EC端地址
char *irq_base = NULL;//EC端地址
void init_addr(void)
{//地址注册
	base = ioremap((phys_t)EC_BASE_ADDR, EC_OFFSET_MAX);
	irq_base = ioremap((phys_t)EC_IRQ_BASE_ADDR, EC_IRQ_OFFSET_MAX);
}
void exit_addr(void)
{//地址注销
	iounmap(EC_BASE_ADDR);
	iounmap(EC_IRQ_BASE_ADDR);
}
#else
void *base = EC_BASE_ADDR;//EC端地址
void *irq_base = EC_IRQ_BASE_ADDR;//EC端地址
#endif
#define ENUM_STRUCT_UNION_SET 1//声明
#ifdef ENUM_STRUCT_UNION_SET
/* Power information structure */
typedef struct ec_mod_power_supply_info{
	unsigned int adapter_in;/* 是否接入AC*/
	unsigned int bat_in;/* Battery是否存在 */
	unsigned int health;/* Battery健康状况 */
	unsigned int design_capacity;/* Battery设计容量 */
	unsigned int design_voltage;/* Battery设计电压 */
	unsigned int full_charged_capacity;/* Battery满电容量 */
	unsigned char manufacture_date[11]; /* Battery生产日期 */
	unsigned char serial_number[8];/* Battery序列号 */
	unsigned char manufacturer_name[12];/* Battery制造商名称，最大11 + 1(长度)字节 */
	unsigned char device_name[8];/* Battery设备名称，最大7 + 1(长度)字节 */
	unsigned int technology;/* Battery技术 */
	unsigned char cell_count;/* Battery单元数 */
	unsigned int voltage_now;/* Battery动态充放电电压 */
	/* Battery 动态充放电平均电流 */
	int current_now;//当前电流
	int current_sign;//电流信号
	int current_average;//平均电流
	unsigned int remain_capacity;/* Battery当前剩余容量 */
	unsigned int remain_capacity_percent;/* Battery当前剩余容量百分比 */
	unsigned int temperature;/* Battery当前温度 */
	unsigned int remain_time;/* Battery当前剩余时间(平均清空时间) */
	unsigned int fullchg_time;/* Battery充满电时间(平均满充电时间) */
	unsigned int charge_status;/* Battery状态 */
	unsigned int cycle_count;/* Battery当前循环计数 (循环计数) */
}ec_mod_power_supply_info;
ec_mod_power_supply_info power_supply_info = { 0 };
ec_mod_power_supply_info *power_info = &power_supply_info;
enum{
	DATA_DESTROY_OFF = 0,
	DATA_DESTROY_ON
};
enum fan_control_mode{
	FAN_FULL_MODE = 0,
	FAN_MANUAL_MODE = 1,
	FAN_AUTO_MODE = 2,
	FAN_MODE_END
};
/* the register operation access struct */
struct ec_reg{
	u32	addr;	/* 地址为EC flash地址和ACPI命令 */
	u8	index;	/* 索引是ACPI命令的索引 */
	u16	val;	/* val为EC flash数据和EC空间值 */
	u8	flag;	/*不同的访问方法. */
};
struct temp_range{
	u8 low;
	u8 high;
	u8 level;
};
		/* Power supply */
#define BIT_BAT_POWER_ACIN		(1 << 0)
#ifndef APM_AC_OFFLINE
enum{
	APM_AC_OFFLINE = 0,
	APM_AC_ONLINE,
	APM_AC_BACKUP,
	APM_AC_UNKNOWN = 0xff
};
#endif
enum APM_BAT_STATUS{
	APM_BAT_STATUS_HIGH = 0,
	APM_BAT_STATUS_LOW,
	APM_BAT_STATUS_CRITICAL,
	APM_BAT_STATUS_CHARGING,
	APM_BAT_STATUS_NOT_PRESENT,
	APM_BAT_STATUS_UNKNOWN = 0xff
};
enum BAT_REG/* bat_reg_flag */
{
	BAT_REG_TEMP_FLAG = 1,
	BAT_REG_VOLTAGE_FLAG,
	BAT_REG_CURRENT_FLAG,
	BAT_REG_AC_FLAG,
	BAT_REG_RC_FLAG,
	BAT_REG_FCC_FLAG,
	BAT_REG_ATTE_FLAG,
	BAT_REG_ATTF_FLAG,
	BAT_REG_RSOC_FLAG,
	BAT_REG_CYCLCNT_FLAG
};
#endif
const char *version = EC_VERSION;//版本
//irq registered val
int aux_irq_regd = 0;
int kbd_irq_regd = 0;
int spim_irq_regd = 0;
int pmc1_irq_regd = 0;
int pmc2_irq_regd = 0;
int pmc3_irq_regd = 0;
int pmc4_irq_regd = 0;
int pmc5_irq_regd = 0;
int hwuc_irq_regd = 0;
int smfi_irq_regd = 0;
int rtct_irq_regd = 0;
int can0_irq_regd = 0;
int can1_irq_regd = 0;
int can2_irq_regd = 0;
int can3_irq_regd = 0;
int uart1_irq_regd = 0;
int uart2_irq_regd = 0;
int uarta_irq_regd = 0;
int uartb_irq_regd = 0;
int hwmon_irq_regd = 0;
int uart3_irq_regd = 0;
int uart4_irq_regd = 0;
int parallel_irq_regd = 0;
//serio set port
struct serio *ec_kbc_port = NULL;
struct serio *ec_mouse_port = NULL;
//应用访问ec标志
extern unsigned char app_access_ec_flag;
//kthread 
static struct task_struct *tsk = NULL;
static struct task_struct *task;
//read val of ec
unsigned char kbc1, kbc2;
unsigned char mouse1, mouse2;
unsigned char kthread1;
//lock
extern spinlock_t index_access_lock;
extern spinlock_t port_access_lock;
DEFINE_SPINLOCK(index_access_lock);
DEFINE_SPINLOCK(port_access_lock);
#endif
#define FUNC 1//func声明
#ifdef FUNC
#define ec_write(addr,val)	(*((char*)(base + addr))) = (val)
#define ec_read(addr)	(*((char*)(base + addr)))
void spim_send_handler(void) {}
void spim_recv_handler(void) {}
unsigned char i8042_read_status(void);
unsigned char i8042_read_data(void);
#endif
#if IRQ_FUNC
DEFINE_SPINLOCK(i8042_lock);
DEFINE_SPINLOCK(pnp_lock_uart1);
DEFINE_SPINLOCK(pnp_lock_uart2);
DEFINE_SPINLOCK(pnp_lock_uart3);
DEFINE_SPINLOCK(pnp_lock_uart4);
DEFINE_SPINLOCK(pnp_lock_uarta);
DEFINE_SPINLOCK(pnp_lock_uartb);
/*********************wzk_debug interrupt register******************/
/*irqreturn_t spim_interrupt(int irq,void * dev_id)
{
	unsigned char interrupt_status = 0;
	interrupt_status = *(base + 0x9);
	if(interrupt_status & 0x1)
	{
	//spim_send_handler();
	}
		else if(interrupt_status & 0x10)
		{
		//spim_recv_handler();
		}
	return (IRQ_HANDLED);
}
irqreturn_t pmc2_interrupt(int irq,void * dev_id)
{
		char var = 0;
	char var1 = 0;
 //       unsigned int io_flags = 0;
	var1 = *(base + 0x6c);

	if(var1 & 0x1)
	{
		var = *(base + 0x68);
		//printk("pmc is %#x\n",var);
		//serio_interrupt(ec_mouse_port, var, io_flags);
	}else{
		printk("pmc debug \n");
	}
	return (IRQ_HANDLED);
}
irqreturn_t pmc3_interrupt(int irq,void * dev_id)
{
		char var = 0;
	char var1 = 0;
 //       unsigned int io_flags = 0;
	var1 = *(base + 0x6e);

	if(var1 & 0x1)
	{
		var = *(base + 0x6a);
		//printk("pmc is %#x\n",var);
		//serio_interrupt(ec_mouse_port, var, io_flags);
	}else{
		printk("pmc debug \n");
	}
	return (IRQ_HANDLED);
}
irqreturn_t pmc4_interrupt(int irq,void * dev_id)
{
		char var = 0;
	char var1 = 0;
 //       unsigned int io_flags = 0;
	var1 = *(base + 0x78);

	if(var1 & 0x1)
	{
		var = *(base + 0x74);
		//printk("pmc is %#x\n",var);
		//serio_interrupt(ec_mouse_port, var, io_flags);
	}else{
		printk("pmc debug \n");
	}
	return (IRQ_HANDLED);
}
irqreturn_t pmc5_interrupt(int irq,void * dev_id)
{
		char var = 0;
	char var1 = 0;
 //       unsigned int io_flags = 0;
	var1 = *(base + 0x7c);

	if(var1 & 0x1)
	{
		var = *(base + 0x7a);
		//printk("pmc is %#x\n",var);
		//serio_interrupt(ec_mouse_port, var, io_flags);
	}else{
		printk("pmc debug \n");
	}
	return (IRQ_HANDLED);
}
irqreturn_t uarta_interrupt(int irq, void * dev_id)
{
		volatile char var = 0;
	volatile char var1 = 0;
		unsigned int io_flags = 0;
	unsigned long flags = 0;
	printk("Enter uarta_interrupt\n");
	spin_lock_irqsave(&pnp_lock_uarta,flags);
	var = *((volatile char*)(base + 0x5F8));
	printk("uarta is %#x \n",var);
	spin_unlock_irqrestore(&pnp_lock_uarta,flags);
	return (IRQ_HANDLED);
}
irqreturn_t uartb_interrupt(int irq, void * dev_id)
{
		volatile char var = 0;
	volatile char var1 = 0;
		unsigned int io_flags = 0;
	unsigned long flags = 0;
	printk("Enter uartb_interrupt\n");
	spin_lock_irqsave(&pnp_lock_uartb,flags);
	var = *((volatile char*)(base + 0x6F8));
	printk("uartb is %#x \n",var);
	spin_unlock_irqrestore(&pnp_lock_uartb,flags);
	return (IRQ_HANDLED);
}
irqreturn_t hwmon_interrupt(int irq, void * dev_id)
{
		char var = 0;
	char var1 = 0;
	var = *(base + 0xe);

	if(var & 0x3)
	{
	 var1 ++;
	}
	return (IRQ_HANDLED);
}
irqreturn_t hwuc_interrupt(int irq, void * dev_id)
{
	char var = 0;
	char var1 = 0;
	var = *(base + 0x0);

	if(var & 0xcb)
	{
	 var1 ++;
	}
	return (IRQ_HANDLED);
}
*/
irqreturn_t parallel_interrupt(int irq, void *dev_id)
{
	char var = 0;
	char var1 = 0;
	var = *(char *)(base + 0x278 + 0x1);
	if (var & 0x4)
	{
		var1++;
	}
	return (IRQ_HANDLED);
}
irqreturn_t smfi_interrupt(int irq, void *dev_id)
{
	return (IRQ_HANDLED);
}
irqreturn_t rtct_interrupt(int irq, void *dev_id)
{
	return (IRQ_HANDLED);
}
irqreturn_t can0_interrupt(int irq, void *dev_id)
{
	return (IRQ_HANDLED);
}
irqreturn_t can1_interrupt(int irq, void *dev_id)
{
	return (IRQ_HANDLED);
}
irqreturn_t can2_interrupt(int irq, void *dev_id)
{
	return (IRQ_HANDLED);
}
irqreturn_t can3_interrupt(int irq, void *dev_id)
{
	return (IRQ_HANDLED);
}
irqreturn_t uart3_interrupt(int irq, void *dev_id)
{
	volatile char var = 0;
//	volatile char var1 = 0;
//      unsigned int io_flags = 0;
	unsigned long flags = 0;
	printk("Enter uart3_interrupt\n");
	spin_lock_irqsave(&pnp_lock_uart3, flags);
	var = *((volatile char *)(base + 0xF8));
	printk("uart3 is %#x \n", var);
	spin_unlock_irqrestore(&pnp_lock_uart3, flags);
	return (IRQ_HANDLED);
}
irqreturn_t uart4_interrupt(int irq, void *dev_id)
{
	volatile char var = 0;
//volatile char var1 = 0;
	//unsigned int io_flags = 0;
	unsigned long flags = 0;
	printk("Enter uart4_interrupt\n");
	spin_lock_irqsave(&pnp_lock_uart4, flags);
	var = *((volatile char *)(base + 0x4F8));
	printk("uart4 is %#x \n", var);
	spin_unlock_irqrestore(&pnp_lock_uart4, flags);
	return (IRQ_HANDLED);
}
/*********************wzk_debug interrupt register******************/
irqreturn_t kbc_interrupt(int irq, void *dev_id)
{
	char var = 0;
	char var1 = 0;
		//unsigned int io_flags = 0;
	var1 = *(char *)(base + 0x64);
	if (var1 & 0x1)
	{
		kbc1++;
		var = *(char *)(base + 0x60);
	#if 0
		if (var == 0x5a)
			a3++;
	#endif
	#if 0
		if (var == 0x5a)
			a3++;
		else if (var == 0x78)
			a4++;
	#endif
			//delay;
			//printk("kbc is %#x\n",var);
			//serio_interrupt(ec_kbc_port, var, io_flags);
	}
	else
	{
		kbc2++;
	//	printk("kbc debug...\n");
	}
	return (IRQ_HANDLED);
}
irqreturn_t pmc_interrupt(int irq, void *dev_id)
{
	char var = 0;
	char var1 = 0;
 //       unsigned int io_flags = 0;
	var1 = *(char *)(base + 0x66);

	if (var1 & 0x1)
	{
		var = *(char *)(base + 0x62);
		//printk("pmc is %#x\n",var);
		//serio_interrupt(ec_mouse_port, var, io_flags);
	}
	else	
{
		printk("pmc debug \n");
	}
	return (IRQ_HANDLED);
}
irqreturn_t uart1_interrupt(int irq, void *dev_id)
{
	volatile char var = 0;
//	volatile char var1 = 0;
//      unsigned int io_flags = 0;
	unsigned long flags = 0;
	printk("Enter uart1_interrupt\n");
	spin_lock_irqsave(&pnp_lock_uart1, flags);
	var = *((volatile char *)(base + 0x3F8));
	printk("uart1 is %#x \n", var);
	spin_unlock_irqrestore(&pnp_lock_uart1, flags);
	return (IRQ_HANDLED);
}
irqreturn_t uart2_interrupt(int irq, void *dev_id)
{
	volatile char var = 0;
//	volatile char var1 = 0;
//	unsigned int io_flags = 0;
	unsigned long flags = 0;
	printk("Enter uart2_interrupt\n");
	spin_lock_irqsave(&pnp_lock_uart2, flags);
	var = *((volatile char *)(base + 0x2F8));
	printk("uart2 is %#x\n", var);

	spin_unlock_irqrestore(&pnp_lock_uart2, flags);
	return (IRQ_HANDLED);
}
irqreturn_t mouse_interrupt(int irq, void *dev_id)
{
	char var = 0;
	char var1 = 0;
		//unsigned int io_flags = 0;
	//b6 ++;
	var1 = *(char *)(base + 0x64);
	if (var1 & 0x1)
	{
		mouse1++;
		var = *(char *)(base + 0x60);
	#if 0
		if (var == 0x5a)
			b3++;
		else if (var == 0x78)
			b4++;
	#endif
	#if 0
		if (var == 0x78)
			b4++;
	#endif

			//delay;
			//printk("ps mouse is %#x \n",var);
			//serio_interrupt(ec_mouse_port, var, io_flags);
	//		while(((*(base + 0x64))&0x1))
	//		{
	//			printk("var1 .....\n");
	//		}
	}
	else
	{
		mouse2++;
	//	printk("mouse debug \n");
	}
	return (IRQ_HANDLED);
}
#if 0
irqreturn_t share_interrupt(int irq, void *dev_id)
{
	char var = 0;
	char var1 = 0;
	//volatile char var = 0;
	//volatile char var1 = 0;
	unsigned int io_flags = 0;
	unsigned long flags = 0;
	spin_lock_irqsave(&i8042_lock, flags);
	var1 = *(base + 0x64);
	//var1 = *((volatile char*)(base + 0x64));
	if (var1 & 0x1)
	{
		//c1++;
		var = *(base + 0x60);
		//var = *((volatile char*)(base + 0x60));
		//printk("var1 is %#x ,var is %#x \n",var1,var);
	#if 0
		if (var == 0x5a)
			c2++;
	#endif
	#if 0
		if (var == 0x5a)
			c2++;
		else if (var == 0x78)
			c3++;
	#endif
	#if 0
		if (var1 & 0x10)
		{
			//printk("keyboard var is %#x\n",var);
			c1++;
		}
		else if (var1 & 0x20)
		{
			//printk("mouse var is %#x\n",var);
			c2++;
		#if 0
			if (var == 0x5a)
				c5++;
		#endif
		#if 0
			if (var == 0x5a)
				c5++;
			else if (var == 0x78)
				c6++;
		#endif
		}
		else
			c3++;
	#endif
	}
	else
	{
		c4++;
//		spin_unlock_irqrestore(&i8042_lock,flags);
		return (IRQ_HANDLED);
	}
//	spin_unlock_irqrestore(&i8042_lock,flags);
	return (IRQ_HANDLED);
}
#else
irqreturn_t share_interrupt(int irq, void *dev_id)
{
	//volatile char var = 0;
	//volatile char var1 = 0;
	unsigned int io_flags = 0;
	unsigned long flags = 0;
	unsigned char str, data;
	int ret = 1;
	//unsigned int dfl;
	spin_lock_irqsave(&i8042_lock, flags);
	str = i8042_read_status();
	if (unlikely(~str & I8042_STR_OBF))
	{
		spin_unlock_irqrestore(&i8042_lock, flags);
		if (irq)
			dbg("Interrupt %d,without any data\n", irq);
		ret = 0;
		goto out;
	}
	data = i8042_read_data();
	if (str & I8042_STR_AUXDATA)
	{
		printk("mouse var is %#x\n", data);
		serio_interrupt(ec_mouse_port, data, io_flags);
	}
	else if (str & I8042_STR_KEYLOCK)
	{
		printk("keyboard var is %#x\n", data);
		serio_interrupt(ec_kbc_port, data, io_flags);
	}
	spin_unlock_irqrestore(&i8042_lock, flags);
out:
	return (IRQ_HANDLED);
}
#endif
#endif
#if PLATFORM_RUN //平台设备
/****************wzk_debug*****************/
int i8042_spim_irq_registered = 0;
int i8042_pmc2_irq_registered = 0;
int i8042_pmc3_irq_registered = 0;
int i8042_pmc4_irq_registered = 0;
int i8042_pmc5_irq_registered = 0;
int i8042_uarta_irq_registered = 0;
int i8042_uartb_irq_registered = 0;
int i8042_hwmon_irq_registered = 0;
int i8042_hwuc_irq_registered = 0;
int i8042_parallel_irq_registered = 0;
int i8042_smfi_irq_registered = 0;
int i8042_rtct_irq_registered = 0;
int i8042_can0_irq_registered = 0;
int i8042_can1_irq_registered = 0;
int i8042_can2_irq_registered = 0;
int i8042_can3_irq_registered = 0;
int i8042_uart3_irq_registered = 0;
int i8042_uart4_irq_registered = 0;
/****************wzk_debug*****************/
int i8042_kbd_irq_registered = 0;
int i8042_uart1_irq_registered = 0;
int i8042_uart2_irq_registered = 0;
int i8042_aux_irq_registered = 0;
int i8042_pmc_irq_registered = 0;
unsigned char i8042_read_status(void)
{
	unsigned char str;
	str = *((volatile unsigned char *)(base + 0x64));
	return str;
}
unsigned char i8042_read_data(void)
{
	unsigned char data;
	data = *((volatile unsigned char *)(base + 0x60));
	return data;
}
void init_kbc_pmc(void)
{
	char val;
	ec_write(0x2e, 0x23);
	delay;
	ec_write(0x2f, 0x01);    //set SIOEN=1
	delay;
	val = ec_read(0x2f);
	printk("%#x\n", val);
	ec_write(0x2e, 0x07);
	delay;
	ec_write(0x2f, 0x11);  //LDN=PMC1
	delay;
#if 0
	ec_write(0x2e, 0x61);
	delay;
	ec_write(0x2f, 0x62);
	delay;
	ec_write(0x2e, 0x63);
	delay;
	ec_write(0x2f, 0x66);
	delay;
#endif
	ec_write(0x2e, 0x70);
	delay;
	ec_write(0x2f, 0x05);  //set PMC1.IRQ=5
	delay;
	ec_write(0x2e, 0x71);
	delay;
	ec_write(0x2f, 0x1);   //set PMC1.IRQTP=level trigger
	delay;
	ec_write(0x2e, 0x30);
	delay;
	ec_write(0x2f, 0x01);   //enable PMC1
	delay;
#if 1
//***********serirq_Op*************
//KBCK
	delay;
	ec_write(0x2e, 0x07);
	delay;
	ec_write(0x2f, 0x06);  //LDN= KBCK
	delay;
	ec_write(0x2e, 0x70);  //LDN= KBCK
	delay;
	ec_write(0x2f, 0x11);  //LDN= KBCK
	delay;
	ec_write(0x2e, 0x71);
	delay;
	ec_write(0x2f, 0x00);  //set KBCK.IRQTP=level trigger			
	delay;
	ec_write(0x2e, 0x30);
	delay;
	ec_write(0x2f, 0x01);  //enable KBCK
	delay;
//KBCM
	ec_write(0x2e, 0x07);
	delay;
	ec_write(0x2f, 0x05);  //LDN= KBCM
	delay;
	ec_write(0x2e, 0x70);  //LDN= KBCK
	delay;
	ec_write(0x2f, 0x1C);  //LDN= KBCK
	delay;
	ec_write(0x2e, 0x71);
	delay;
	ec_write(0x2f, 0x00);  //set KBCM.IRQTP=level trigger			
	delay;
	ec_write(0x2e, 0x30);
	delay;
	ec_write(0x2f, 0x01);  //enable KBCM
	delay;
#if 0
	ec_write(0x2e, 0x07);
	delay;
	ec_write(0x2f, 0x01);  //LDN= UART1  base_addr = 0x3F8
	delay;
	ec_write(0x2e, 0x71);
	delay;
	ec_write(0x2f, 0x01);  //set UART1.IRQTP=level trigger			
	delay;
	ec_write(0x2e, 0x30);
	delay;
	ec_write(0x2f, 0x01);  //enable KBCM
	delay;
	ec_write(0x2e, 0x07);
	delay;
	ec_write(0x2f, 0x02);  //LDN= UART2	base_addr = 0x2F8
	delay;
	ec_write(0x2e, 0x71);
	delay;
	ec_write(0x2f, 0x01);  //set UART1.IRQTP=level trigger			
	delay;
	ec_write(0x2e, 0x30);
	delay;
	ec_write(0x2f, 0x01);  //enable KBCM
	delay;
#endif
// PMC1     
#if 1
	ec_write(0x2e, 0x07);
	delay;
	ec_write(0x2f, 0x12);  //LDN=PMC2
	delay;
	ec_write(0x2e, 0x61);
	delay;
	ec_write(0x2f, 0x68);
	delay;
	ec_write(0x2e, 0x63);
	delay;
	ec_write(0x2f, 0x6c);
	delay;
	ec_write(0x2e, 0x70);
	delay;
	ec_write(0x2f, 0x05);  //set PMC1.IRQ=5
	delay;
	ec_write(0x2e, 0x71);
	delay;
	ec_write(0x2f, 0x1);   //set PMC1.IRQTP=level trigger
	delay;
	ec_write(0x2e, 0x30);
	delay;
	ec_write(0x2f, 0x01);   //enable PMC1
	delay;
#endif
#if 1
	ec_write(0x2e, 0x07);
	delay;
	ec_write(0x2f, 0x17);  //LDN=PMC3
	delay;
	ec_write(0x2e, 0x70);
	delay;
	ec_write(0x2f, 0x07);  //set PMC1.IRQ=5
	delay;
	ec_write(0x2e, 0x71);
	delay;
	ec_write(0x2f, 0x1);   //set PMC1.IRQTP=level trigger
	delay;
	ec_write(0x2e, 0x30);
	delay;
	ec_write(0x2f, 0x01);   //enable PMC1
	delay;
	ec_write(0x2e, 0x07);
	delay;
	ec_write(0x2f, 0x18);  //LDN=PMC4
	delay;
	ec_write(0x2e, 0x70);
	delay;
	ec_write(0x2f, 0x08);  //set PMC1.IRQ=5
	delay;
	ec_write(0x2e, 0x71);
	delay;
	ec_write(0x2f, 0x1);   //set PMC1.IRQTP=level trigger
	delay;
	ec_write(0x2e, 0x30);
	delay;
	ec_write(0x2f, 0x01);   //enable PMC1
	delay;
	ec_write(0x2e, 0x07);
	delay;
	ec_write(0x2f, 0x19);  //LDN=PMC5
	delay;
	ec_write(0x2e, 0x70);
	delay;
	ec_write(0x2f, 0x09);  //set PMC1.IRQ=5
	delay;
	ec_write(0x2e, 0x71);
	delay;
	ec_write(0x2f, 0x1);   //set PMC1.IRQTP=level trigger
	delay;
	ec_write(0x2e, 0x30);
	delay;
	ec_write(0x2f, 0x01);   //enable PMC1
	delay;
		/*ec_write(0x2e,0x07);
	delay;
	ec_write(0x2f,0x0f);  //LDN=SMFI
	delay;
	ec_write(0x2e,0xf4);
	delay;
	ec_write(0x2f,0x61);  //
	delay;
	ec_write(0x2e,0xf5);
	delay;
	ec_write(0x2f,0xa0);   //
	delay;
	ec_write(0x2e,0xf6);
	delay;
	ec_write(0x2f,0x5a);   //
	delay;
	ec_write(0x2e,0x30);
	delay;
	ec_write(0x2f,0x01);   //enable SMFI
	delay;*/
#endif
#endif
	printk("init kbc pmc done.\n");
}
/*
 * ./op_reg_int 0x10002004 0x3ffff         //enable 0-17
 * ./op_reg_int 0x10002004 0x22     //enable 0-17
 * ./op_reg_int 0x10002010 0x0     //low level trigger
 * cat /proc/interrupts
 */
void enable_serirq_and_level(void)
{
	(*((int *)(irq_base))) |= 0x80000000; //open SIRQ interrupt enable control

	//(*((int*)(irq_base + 0x4))) = 0x1000 |0x20 |0x2 |0x10 | 0x8 |0x80 |0x10;
	//(*((int*)(irq_base + 0x4))) |= 0x0003fffe; //wzk_debug
	//(*((int*)(irq_base + 0x4))) |= 0x20 | 0x200 | 0x4 | 0x40 | 0x100; //wzk_debug pmc12345 open 
	//(*((int*)(irq_base + 0x4))) |= 0x10 | 0x4 | 0x400 | 0x800 | 0x6000; //wzk_debug uart1234ab open 
	//(*((int*)(irq_base + 0x4))) |= 0x8000; //wzk_debug hwmon open 
	//(*((int*)(irq_base + 0x4))) |= 0x0080; //wzk_debug parallel open 
	//(*((int*)(irq_base + 0x4))) |= 0x8000; //wzk_debug hwuc open 
	//(*((int*)(irq_base + 0x4))) |= 0x8000; //wzk_debug spim open 
	//(*((int*)(irq_base + 0x4))) = 0x1022 |0x10 |0x8;
	(*((int *)(irq_base + 0x4))) = 0x1002;
	//(*((int*)(irq_base + 0x10))) = 0x0;
	(*((int *)(irq_base + 0x10))) &= 0xfff3;
	printk("mask irq is %#x ,irq_trigger is %#x \n", (*((int *)(irq_base + 0x4))), (*((int *)(irq_base + 0x10))));
	printk("Enable_serirq done.\n");
}
void disable_serirq(void)
{
	(*((int *)(irq_base + 0x4))) = 0x0;
}
static int ec_kbc_port_write(struct serio *port, unsigned char c)
{
	char var = 0;
	printk("Enter ec_kbc_port_write %#x\n", c);
//	while(1)
#if 1
	{
		var = *(char *)(base + 0x64);
//		if((var&0x2) == 0)
//			break;
		delay;
		printk("delay...var is %#x\n", var);
	}
	*(char *)(base + 0x60) = c;
#endif
	return 0;
}
static int ec_mouse_port_write(struct serio *port, unsigned char c)
{
	printk("Enter ec_mouse_port_write %#x\n", c);
#if 1
	* (base + 0x64) = 0xD4;
	delay;
	*(base + 0x60) = c;
#endif
	return 0;
}
static int ec_kbc_port_open(struct serio *serio)
{
	printk("ec_kbc_port_open \n");
	return 0;
}
static int ec_mouse_port_open(struct serio *serio)
{
	printk("ec_mouse_port_open \n");
	return 0;
}
static void ec_kbc_port_close(struct serio *serio)
{
	printk("ec_kbc_port_close \n");
}
static void ec_mouse_port_close(struct serio *serio)
{
	printk("ec_mouse_port_close \n");
}
static int thread_function(void *data)
{
	int time_count = 0;
	do
	{
		msleep(3000);
		//printk("a1 is %#x,a2 is %#x,a3 is %#x ,a4 is %#x,a5 is %#x,a6 is %#x \n",a1,a2,a3,a4,a5,a6);
		//printk("b1 is %#x,b2 is %#x,b3 is %#x ,b4 is %#x,b5 is %#x,b6 is %#x \n",b1,b2,b3,b4,b5,b6);
		printk("kbc1 is %#x,kbc2 is %#x,mouse1 is %#x,mouse2 is %#x,kthread1 is %#x \n", kbc1, kbc2, mouse1, mouse2, kthread1);
	}
	while (!kthread_should_stop());
	return time_count;
}
struct platform_device *i8042_platform_device = NULL;
static int ec_probe(struct platform_device *pdev)
{
	int ret = 0;
	printk("Enter ec_probe \n");
	i8042_platform_device = pdev;
#if 1
	ec_mouse_port = kzalloc(sizeof(struct serio), GFP_KERNEL);
	if (!ec_mouse_port)
		return -ENOMEM;
	ec_mouse_port->id.type = SERIO_8042;
	ec_mouse_port->dev.parent = &pdev->dev;
	ec_mouse_port->open = ec_mouse_port_open;
	ec_mouse_port->close = ec_mouse_port_close;
	ec_mouse_port->write = ec_mouse_port_write;
	snprintf(ec_mouse_port->name, sizeof(ec_mouse_port->name), "ec pmc %s", ("mouse"));
	snprintf(ec_mouse_port->phys, sizeof(ec_mouse_port->phys), "ec/serio%d", 1);
	serio_register_port(ec_mouse_port);
#endif
	ec_kbc_port = kzalloc(sizeof(struct serio), GFP_KERNEL);
	if (!ec_kbc_port)
		return -ENOMEM;
//	ec_kbc_port->id.type = SERIO_8042_XL;
	ec_kbc_port->id.type = SERIO_8042;
	ec_kbc_port->dev.parent = &pdev->dev;
	ec_kbc_port->open = ec_kbc_port_open;
	ec_kbc_port->close = ec_kbc_port_close;
	ec_kbc_port->write = ec_kbc_port_write;
	snprintf(ec_kbc_port->name, sizeof(ec_kbc_port->name), "ec kbc %s", ("keyboard"));
	snprintf(ec_kbc_port->phys, sizeof(ec_kbc_port->phys), "ec/serio%d", 0);
	serio_register_port(ec_kbc_port);
	delay;
	delay;
	delay;
	enable_serirq_and_level();
	/******************************wzk_debug interrupt register***************************/
	/*ret = request_irq(1,uarta_interrupt,IRQF_TRIGGER_LOW,"EC_UARTA",i8042_platform_device);
if(ret < 0)
	printk("EC_UARTA Request_irq failed error %d \n",ret);
else {
	i8042_uarta_irq_registered = 1;
	printk("EC_UARTA Request irq OK.\n");
}
	ret = request_irq(1,uartb_interrupt,IRQF_TRIGGER_LOW,"EC_UARTB",i8042_platform_device);
if(ret < 0)
	printk("EC_UARTB Request_irq failed error %d \n",ret);
else {
	i8042_uartb_irq_registered = 1;
	printk("EC_UARTB Request irq OK.\n");
}

	ret = request_irq(1,hwmon_interrupt,IRQF_TRIGGER_LOW,"EC_HWMON",i8042_platform_device);
if(ret < 0)
	printk("EC_HWMON Request_irq failed error %d \n",ret);
else {
	i8042_hwmon_irq_registered = 1;
	printk("EC_HWMON Request irq OK.\n");
}
	ret = request_irq(1,hwuc_interrupt,IRQF_TRIGGER_LOW,"EC_HWUC",i8042_platform_device);
if(ret < 0)
	printk("EC_HWUC Request_irq failed error %d \n",ret);
else {
	i8042_hwuc_irq_registered = 1;
	printk("EC_HWUC Request irq OK.\n");
}
	ret = request_irq(7,parallel_interrupt,IRQF_TRIGGER_LOW,"EC_PARALLEL",i8042_platform_device);
if(ret < 0)
	printk("EC_PARALLEL Request_irq failed error %d \n",ret);
else {
	i8042_parallel_irq_registered = 1;
	printk("EC_PARALLEL Request irq OK.\n");
}
	ret = request_irq(1,smfi_interrupt,IRQF_TRIGGER_LOW,"EC_SMFI",i8042_platform_device);
if(ret < 0)
	printk("EC_SMFI Request_irq failed error %d \n",ret);
else {
	i8042_smfi_irq_registered = 1;
	printk("EC_SMFI Request irq OK.\n");
}
	ret = request_irq(1,rtct_interrupt,IRQF_TRIGGER_LOW,"EC_RTCT",i8042_platform_device);
if(ret < 0)
	printk("EC_RTCT Request_irq failed error %d \n",ret);
else {
	i8042_rtct_irq_registered = 1;
	printk("EC_RTCT Request irq OK.\n");
}
	ret = request_irq(10,can0_interrupt,IRQF_TRIGGER_LOW,"EC_CAN0",i8042_platform_device);
if(ret < 0)
	printk("EC_CAN0 Request_irq failed error %d \n",ret);
else {
	i8042_can0_irq_registered = 1;
	printk("EC_CAN0 Request irq OK.\n");
}
	ret = request_irq(11,can1_interrupt,IRQF_TRIGGER_LOW,"EC_CAN1",i8042_platform_device);
if(ret < 0)
	printk("EC_CAN1 Request_irq failed error %d \n",ret);
else {
	i8042_can1_irq_registered = 1;
	printk("EC_CAN1 Request irq OK.\n");
}
	ret = request_irq(1,can2_interrupt,IRQF_TRIGGER_LOW,"EC_CAN2",i8042_platform_device);
if(ret < 0)
	printk("EC_CAN2 Request_irq failed error %d \n",ret);
else {
	i8042_can2_irq_registered = 1;
	printk("EC_CAN2 Request irq OK.\n");
}
	ret = request_irq(14,can3_interrupt,IRQF_TRIGGER_LOW,"EC_CAN3",i8042_platform_device);
if(ret < 0)
	printk("EC_CAN3 Request_irq failed error %d \n",ret);
else {
	i8042_can3_irq_registered = 1;
	printk("EC_CAN3 Request irq OK.\n");
}
	ret = request_irq(1,uart3_interrupt,IRQF_TRIGGER_LOW,"EC_UART3",i8042_platform_device);
if(ret < 0)
	printk("EC_UART3 Request_irq failed error %d \n",ret);
else {
	i8042_uart3_irq_registered = 1;
	printk("EC_UART3 Request irq OK.\n");
}
	ret = request_irq(1,uart4_interrupt,IRQF_TRIGGER_LOW,"EC_UART4",i8042_platform_device);
if(ret < 0)
	printk("EC_UART4 Request_irq failed error %d \n",ret);
else {
	i8042_uart4_irq_registered = 1;
	printk("EC_UART4 Request irq OK.\n");
}
	ret = request_irq(15,spim_interrupt,IRQF_TRIGGER_LOW,"EC_SPIM",i8042_platform_device);
if(ret < 0)
	printk("EC_SPIM Request_irq failed error %d \n",ret);
else {
	i8042_spim_irq_registered = 1;
	printk("EC_SPIM Request irq OK.\n");
}
	ret = request_irq(9,pmc2_interrupt,IRQF_TRIGGER_LOW,"EC_PMC2",i8042_platform_device);
if(ret < 0)
	printk("EC_PMC2 Request_irq failed error %d \n",ret);
else {
	i8042_pmc2_irq_registered = 1;
	printk("EC_PMC2 Request irq OK.\n");
}
	ret = request_irq(1,pmc3_interrupt,IRQF_TRIGGER_LOW,"EC_PMC3",i8042_platform_device);
if(ret < 0)
	printk("EC_PMC3 Request_irq failed error %d \n",ret);
else {
	i8042_pmc3_irq_registered = 1;
	printk("EC_PMC3 Request irq OK.\n");
}
	ret = request_irq(6,pmc4_interrupt,IRQF_TRIGGER_LOW,"EC_PMC4",i8042_platform_device);
if(ret < 0)
	printk("EC_PMC4 Request_irq failed error %d \n",ret);
else {
	i8042_pmc4_irq_registered = 1;
	printk("EC_PMC4 Request irq OK.\n");
}
	ret = request_irq(8,pmc5_interrupt,IRQF_TRIGGER_LOW,"EC_PMC5",i8042_platform_device);
if(ret < 0)
	printk("EC_PMC5 Request_irq failed error %d \n",ret);
else {
	i8042_pmc5_irq_registered = 1;
	printk("EC_PMC5 Request irq OK.\n");
}*/
	/******************************wzk_debug interrupt register***************************/
#if 0
	ret = request_irq(1, kbc_interrupt, IRQF_TRIGGER_LOW, "EC_KBC", NULL);
	if (ret < 0)
		printk("EC_KBC Request_irq failed error %d \n", ret);
	else
	{
		kbc_irq_flag = 1;
		printk("EC_KBC Request irq OK.\n");
	}
#else
	ret = request_irq(1, share_interrupt, IRQF_SHARED, "EC_KBC", i8042_platform_device);
	if (ret < 0)
		printk("EC_KBC Request_irq failed error %d \n", ret);
	else
	{
		i8042_kbd_irq_registered = 1;
		printk("EC_KBC Request irq OK.\n");
	}
#endif
	ret = request_irq(5, pmc_interrupt, IRQF_TRIGGER_LOW, "EC_PMC", i8042_platform_device);
	if (ret < 0)
		printk("EC_PMC Request_irq failed error %d \n", ret);
	else
	{
		printk("EC_PMC Request_irq OK.\n");
		i8042_pmc_irq_registered = 1;
	}
#if 1
	ret = request_irq(4, uart1_interrupt, IRQF_TRIGGER_LOW, "EC_UART1", i8042_platform_device);
	if (ret < 0)
		printk("EC_UART1 Request_irq failed error %d \n", ret);
	else
	{
		printk("EC_UART1 Request_irq OK.\n");
		i8042_uart1_irq_registered = 1;
	}
	ret = request_irq(3, uart2_interrupt, IRQF_TRIGGER_LOW, "EC_UART2", i8042_platform_device);
	if (ret < 0)
		printk("EC_UART2 Request_irq failed error %d \n", ret);
	else
	{
		printk("EC_UART2 Request_irq OK.\n");
		i8042_uart2_irq_registered = 1;
	}
#endif
#if 0
	ret = request_irq(12, mouse_interrupt, IRQF_TRIGGER_LOW, "EC_MOUSE", NULL);
	if (ret < 0)
		printk("EC_MOUSE Request_irq failed error %d \n", ret);
	else
	{
		printk("EC_MOUSE Request_irq OK.\n");
		mouse_irq_flag = 1;
	}
#else
	ret = request_irq(12, share_interrupt, IRQF_SHARED, "EC_MOUSE", i8042_platform_device);
	if (ret < 0)
		printk("EC_MOUSE Request_irq failed error %d \n", ret);
	else
	{
		printk("EC_MOUSE Request_irq OK.\n");
		i8042_aux_irq_registered = 1;
	}
#endif
	return 0;
}
static int ec_remove(struct platform_device *pdev)
{
	printk("ec_remove function \n");
//	disable_serirq();
	delay;
	serio_unregister_port(ec_kbc_port);
	ec_kbc_port = NULL;
	delay;
	serio_unregister_port(ec_mouse_port);
	ec_mouse_port = NULL;
		/******************************wzk_debug interrupt register***************************/

	/*if(i8042_uarta_irq_registered == 1)
		free_irq(1,i8042_platform_device);
	if(i8042_uartb_irq_registered == 1)
		free_irq(14,i8042_platform_device);
	if(i8042_hwmon_irq_registered == 1)
		free_irq(1,i8042_platform_device);
	if(i8042_hwuc_irq_registered == 1)
		free_irq(1,i8042_platform_device);
	if(i8042_parallel_irq_registered == 1)
		free_irq(7,i8042_platform_device);
	if(i8042_smfi_irq_registered == 1)
		free_irq(1,i8042_platform_device);
	if(i8042_rtct_irq_registered == 1)
		free_irq(1,i8042_platform_device);
	if(i8042_can0_irq_registered == 1)
		free_irq(10,i8042_platform_device);
	if(i8042_can1_irq_registered == 1)
		free_irq(11,i8042_platform_device);
	if(i8042_can2_irq_registered == 1)
		free_irq(1,i8042_platform_device);
	if(i8042_can3_irq_registered == 1)
		free_irq(14,i8042_platform_device);
	if(i8042_uart3_irq_registered == 1)
		free_irq(1,i8042_platform_device);
	if(i8042_uart4_irq_registered == 1)
		free_irq(1,i8042_platform_device);
		if(i8042_spim_irq_registered == 1)
		free_irq(15,i8042_platform_device);
	if(i8042_pmc2_irq_registered == 1)
		free_irq(9,i8042_platform_device);
	if(i8042_pmc3_irq_registered == 1)
		free_irq(1,i8042_platform_device);
	if(i8042_pmc4_irq_registered == 1)
		free_irq(6,i8042_platform_device);

	if(i8042_pmc5_irq_registered == 1)
		free_irq(8,i8042_platform_device);*/
		/******************************wzk_debug interrupt register***************************/
	if (i8042_kbd_irq_registered == 1)
		free_irq(1, i8042_platform_device);
	if (i8042_pmc_irq_registered == 1)
		free_irq(5, i8042_platform_device);
	if (i8042_aux_irq_registered == 1)
		free_irq(12, i8042_platform_device);
	if (i8042_uart1_irq_registered == 1)
		free_irq(4, i8042_platform_device);
	if (i8042_uart2_irq_registered == 1)
		free_irq(3, i8042_platform_device);
	//(*((int*)(irq_base + 0x4))) = 0x00;
	//(*((int*)(irq_base + 0x10))) = 0xffffffff;
	iounmap((char *)0x18000000);
	iounmap((char *)0x10002000);
	return 0;
}
static struct platform_driver ec_driver = {
	.driver = {
		.name = "host_ec",
		.owner = THIS_MODULE,
	},
	.probe = ec_probe,
	.remove = ec_remove,
};
static struct platform_device *ec_device;
/*********************************************************/
void i8042_flush_data(void)
{
	unsigned long flags = 0;
	unsigned char data, str;
	printk("Enter i8042_flush_data \n");
	spin_lock_irqsave(&i8042_lock, flags);

	while ((str = (*(base + 0x64))) & 0x1)
	{
		udelay(50);
		data = *(base + 0x60);
		printk("flush data is %#x\n", data);
	}
	spin_unlock_irqrestore(&i8042_lock, flags);
}
#endif
#define MODULE_SET 1//模块设置
#if MODULE_SET
static int __init ec_init(void)
{
	int ret = 0;
	init_addr();
#if PLATFORM_RUN
//init_kbc_pmc();  //wzk_debug
	mdelay(100);
	i8042_flush_data();
	//init_ec();
	enable_serirq_and_level();
	mdelay(100);
	ret = platform_driver_register(&ec_driver);
	if (ret)
	{
		printk("platform_driver_register error \n");
		return ret;
	}
	ec_device = platform_device_alloc("host_ec", 0);
	if (ec_device)
	{
		ret = platform_device_add(ec_device);
	}
	else
	{
		ret = -ENOMEM;
		return ret;
	}
	if (ret)
	{
		platform_device_put(ec_device);
		platform_driver_unregister(&ec_driver);
		return ret;
	}
	tsk = kthread_run(thread_function, NULL, "mythread%d", 1);
	if (IS_ERR(tsk))
	{
		printk("Create thread failed \n");
	}	
else
	{
		printk("Create thread Ok \n");
	}
#endif
	return ret;
}
static void __exit ec_exit(void)
{
#if PLATFORM_RUN
	platform_device_unregister(ec_device);
	platform_driver_unregister(&ec_driver);
	if (!IS_ERR(tsk))
	{
		kthread_stop(tsk);
		printk("stop kthread \n");
	}
#endif
	exit_addr();
}
Module_init(ec_init);//模块初始化
module_exit(ec_exit);//模块注销
MODULE_LICENSE("GPL");//版权声明
MODULE_AUTHOR("yunfeng");//作者声明
MODULE_DESCRIPTION("For study EC platform module");//模块描述
MODULE_ALIAS("host_ec");//模块别名
MODULE_VERSION("1.2");//模块版本
#endif
