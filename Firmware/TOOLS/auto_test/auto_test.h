/*
 * @Author: Linyu
 * @LastEditors: Linyu
 * @LastEditTime: 2023-04-02 21:17:28
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
#ifndef _AUTO_TEST_H
#define _AUTO_TEST_H
//头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <pthread.h>
#include <wait.h>
//定义
#define test_version "v23.8.29"
#define test_usage  " EC_AUTO_TEST of host_ec\n\
 Usage:sudo ./auto_test  <EC_TEST_DEV> <time=(mtime)> <thread/process>...\n\
    EC_TEST_DEV :\n\
    KBC:I2C0\n\
    PMC1:UART0,CAN01,I2C1\n\
    PMC2:UART2,CAN23,I2C2\n\
    PMC3:UART3,I2C3\n\
    PMC4:UART1,GPIOB\n\
    PMC5:SPACE,GPIOA,PWM0\n\
    SMRAND\tSMRDWR\n"
#define msleep(ms,us) usleep(ms*1000+us)
#if 1//定义映射
#define MAP_SIZE	    0x01000000
#define EC_REG_BASE     0x1C000000
#define BASE_DEF        0xff000000
#define OFFSET_DEF      0x00ffffff
#endif
#if 1//定义KBC端口
#define KBC_DR      0x60
#define KBC_DIR     KBC_DR
#define KBC_DOR     KBC_DR
#define KBC_SR_CMD  0x64
#define KBC_STR     KBC_SR_CMD
#define KBC_CMD     KBC_SR_CMD
#endif
#if 1//定义pmc端口转换关系
#define PMC1_DR         0x62
#define PMC1_DIR        PMC1_DR
#define PMC1_DOR        PMC1_DR
#define PMC1_SR_CMD     0x66
#define PMC1_CMD        PMC1_SR_CMD
#define PMC1_STR        PMC1_SR_CMD
#define PMC2_DR         0x68
#define PMC2_DIR        PMC2_DR
#define PMC2_DOR        PMC2_DR
#define PMC2_SR_CMD     0x6C
#define PMC2_CMD        PMC2_SR_CMD
#define PMC2_STR        PMC2_SR_CMD
#define PMC3_DR         0x6A
#define PMC3_DIR        PMC3_DR
#define PMC3_DOR        PMC3_DR
#define PMC3_SR_CMD     0x6E
#define PMC3_CMD        PMC3_SR_CMD
#define PMC3_STR        PMC3_SR_CMD
#define PMC4_DR         0x74
#define PMC4_DIR        PMC4_DR
#define PMC4_DOR        PMC4_DR
#define PMC4_SR_CMD     0x78
#define PMC4_CMD        PMC4_SR_CMD
#define PMC4_STR        PMC4_SR_CMD
#define PMC5_DR         0x7A
#define PMC5_DIR        PMC5_DR
#define PMC5_DOR        PMC5_DR
#define PMC5_SR_CMD     0x7C
#define PMC5_CMD        PMC5_SR_CMD
#define PMC5_STR        PMC5_SR_CMD
#endif
#if 1//定义位信息
#define BIT(X)  (1 << (X))
#define BIT0    0x01
#define BIT1    0x02
#define BIT2    0x04
#define BIT3    0x08
#define BIT4    0x10
#define BIT5    0x20
#define BIT6    0x40
#define BIT7	0x80
#endif
#if 1//定义状态读取位
#define OBFS    BIT0
#define IBFS    BIT1
#define GPF     BIT2
#define A2      BIT3
#define STS     0xf0
#endif
#if 1//定义ioctl读写配置
#define	EC_IOC_MAGIC		'E'
#define	IOCTL_RDREG		_IOR(EC_IOC_MAGIC, 1, int)
#define	IOCTL_WRREG		_IOW(EC_IOC_MAGIC, 2, int)
#endif
#if 0//定义ECSPACE信息
#define   CTRL_FLAG1					(0xB0)   // EC power flag 1
#define adapter_in      BIT0        // 1 AC adapter exist
#define power_on        BIT1        // 1 system power on
#define enter_S3        BIT2        // 1 host enter suspend to RAM mode
#define pwrbtn_press    BIT3        // 1 waiting for PSW released
#define enter_S4        BIT4        // 1 host enter suspend to HD mode
#define system_on       BIT5        // 1 system on (SWPG issued)
#define wait_HWPG       BIT6        // 1 wait hardware power good
#define rvcc_on         BIT7        // 1 RVCC turnned on
#define   EC_CPU_FAN_DRIVE_LEVEL		(0xF0) 
#define   EC_GPU_FAN_DRIVE_LEVEL		(0xF1)
#define   EC_CPU_FAN_RPM_L				(0xF2)
#define   EC_CPU_FAN_RPM_H				(0xF3)
#define   EC_GPU_FAN_RPM_L				(0xF4)
#define   EC_GPU_FAN_RPM_H				(0xF5)
#define   BAT1_CC_L               (0x80)   // Battery 1        
#define   BAT1_CC_H               (0x81)   // Battery 1        
#define   BAT1_CV_L               (0x82)   // Battery 1
#define   BAT1_CV_H               (0x83)   // Battery 1
#define   BAT1_mode_L         (0x84)   // Battery 1
#define   BAT1_mode_H         (0x85)   // Battery 1
#define   TDIM0_temp          (0x86)   // DRAM 0 temperature
#define   TDIM1_temp          (0x87)   // DRAM 1 temperature
#define   TPkg_temp           (0x88)   // 
#define   TSys_temp           (0x89)   // system Temperature
#define   TCPU_temp           (0x8A)   // CPU Temperature
#define    BAT1_ALARM1        (0x8B)
#define   PECI_CPU_temp       (0x8C)   // Use PECI interface to read CPU temperature
#define   Charger_Mode        (0x60)   // LJP
#define   TimerCount_L          (0x61)   // LJP
#define   TimerCount_H          (0x62)   // LJP
#define   SMB_DATA            (0x63)       // EC SMB1 Data register array (32 bytes)
#define   CrisisKeyStatus     (0x64)   // Crisis key flag
#define   BT1_STATUS1         (0x70)   // Battery 1 status1 flag
#define   BT1_STATUS2         (0x71)   // Battery 1 status2 flag
#define   BT1_STATUS3         (0x72)   // Battery 1
#define   SMB_BCNT            (0x73)       // EC SMB1 Block Count register
#define   POWER_FLAG2         (0x74)   // EC power flag 2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define	SLP_S3_ON			BIT0        // 1:S3 power status
#define	SLP_S4_ON			BIT1        // 1:S4 power status
#define	SLP_S5_ON			BIT2        // 1:S5 power status
#define susb_on         BIT0        // 1 On SUSB Status
#define susc_on         BIT1        // 1 On SUSC Status
#define fan_on          BIT2        // 1 Fan on
#define   POWER_FLAG3         (0x75)   // EC power flag 3
#define   SYS_MISC1           (0x76)   // System Misc flag 1
#define T_sensor0       BIT(0x00)
#define T_sensor1       BIT(0x01)
#define ACPI_OS         BIT(0x02)
#define T_sensor2       BIT(0x03)
#define EC_bank0        BIT(0x04)
#define EC_bank1        BIT(0x05)
#define EC_bank2        BIT(0x06)
#define EC_bank3        BIT(0x07)
#define   SYS_MISC2           (0x77)   // System Misc flag 2
#define   SYS_MISC3           (0x78)   // System Misc flag 3
#define   EVT_STATUS1         (0x79)   // Event status flag 1
#define   EVT_STATUS2         (0x7A)   // Event status flag 2
#define   EVT_STATUS3         (0x7B)   // Event status flag 3
#define   MISC_FLAG1          (0x7C)   // Misc flag 1
#define   MISC_FLAG2          (0x7D)   // Misc flag 2
#define   MISC_FLAG3          (0x7E)   // Misc flag 3
#define   MISC_FLAG4          (0x7F)   // Misc flag 4
#define   READ_ECSPACE_CMD    (0x80)
#define   WRITE_ECSPACE_CMD   (0x81)
#define   UART_MODULE_EN      (0x01)
#define   GPIO_INIT           (0x02)
#define   I2C_MODULE_EN       (0x03)
#define   PWM_MODULE_EN       (0x04)
#define   CAN_MODULE_EN       (0x05)
#endif
#define   TO_CMD              (0x00)
#define   TO_IO               (0x01)
//定义结构体及函数指针
typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef void *(*func_VP_VP) (void *);
typedef struct peripheral_list{
    char *name;
    int select;
    pthread_t id;
    u32 succeed;
    u32 error;
    func_VP_VP func;
    u32 IO;
    u32 SR_CMD;
    pthread_mutex_t *lock;
}peripheral_list;
typedef struct EC_Space{
    char *name;
    u8 addr;
    u8 val;
}EC_Space;
//声明
void *pmc_subroutine(void *name);
void *kbc_subroutine(void *name);
void *sm_subroutine(void *name);
void *rw_subroutine(void *name);
void *Space_subroutine(void *name);
void *PWM_subroutine(void *name);
void *GPIO_subroutine(void *name);
void *I2C_subroutine(void *name);
void *UART_subroutine(void *name);
void *CAN_subroutine(void *name);
//ecspace
EC_Space ECSpace[] = {
    {.name = "EC VERSION MINOR",.addr = 0x00},
    {.name = "EC VERSION SENIOR",.addr = 0x01},
    {.name = "SMB PRTC",.addr = 0x06},
    {.name = "SMB STS",.addr = 0x07},
    {.name = "SMB ADDR",.addr = 0x08},
    {.name = "SMB CMD",.addr = 0x09},
    {.name = "EC POWER CTRL",.addr = 0x0A},
    {.name = "EC RESET MODE",.addr = 0x0B},
    {.name = "EC S3 TIMEOUT L",.addr = 0x0C},
    {.name = "EC S3 TIMEOUT H",.addr = 0x0D},
    {.name = "BrgLevel Ctrl",.addr = 0x0E},
    {.name = "BrgLevel",.addr = 0x0F},
    {.name = "BAT1 RSOC",.addr = 0x21},
    {.name = "BAT1 FCcap L",.addr = 0x24},
    {.name = "BAT1 FCcap H",.addr = 0x25},
    {.name = "BAT1 RMcap L",.addr = 0x26},
    {.name = "BAT1 RMcap H",.addr = 0x27},
    {.name = "BAT1 temp L",.addr = 0x28},
    {.name = "BAT1 temp H",.addr = 0x29},
    {.name = "BAT1 volt L",.addr = 0x2A},
    {.name = "BAT1 volt H",.addr = 0x2B},
    {.name = "BAT1 current L",.addr = 0x2C},
    {.name = "BAT1 current H",.addr = 0x2D},
    {.name = "BAT1 Acurrent L",.addr = 0x2E},
    {.name = "BAT1 Acurrent H",.addr = 0x2F},
    {.name = "BAT1 DCap L",.addr = 0x38},
    {.name = "BAT1 DCap H",.addr = 0x39},
    {.name = "BAT1 Dvolt L",.addr = 0x3A},
    {.name = "BAT1 Dvolt H",.addr = 0x3B},
    {.name = "BAT1 STATUS",.addr = 0x3C},
    {.name = "BAT1 SN L",.addr = 0x3E},
    {.name = "BAT1 SN H",.addr = 0x3F},
    {.name = "POWER FLAG1(bit0:adapter in,bit1:power on,bit2:enter S3,bit3:wait PSW off)",.addr = 0xB0},
    {.name = "EC CPU TEMP",.addr = 0x98},
    {.name = "EC GPU TEMP",.addr = 0x9A},
    {.name = "FT CPU TEMP",.addr = 0xA0},
    {.name = "FT GPU TEMP",.addr = 0xA1},
};
#endif
