/*
 * @Author: Linyu
 * @LastEditors: Linyu
 * @LastEditTime: 2023-04-02 21:16:41
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>
#include <dirent.h>
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char 
//电源状态
enum power_supply_property{
    /* Properties of type `int' */
    POWER_SUPPLY_PROP_STATUS = 0,
    POWER_SUPPLY_PROP_CHARGE_TYPE,
    POWER_SUPPLY_PROP_HEALTH,
    POWER_SUPPLY_PROP_PRESENT,
    POWER_SUPPLY_PROP_ONLINE,
    POWER_SUPPLY_PROP_AUTHENTIC,
    POWER_SUPPLY_PROP_TECHNOLOGY,
    POWER_SUPPLY_PROP_CYCLE_COUNT,
    POWER_SUPPLY_PROP_VOLTAGE_MAX,
    POWER_SUPPLY_PROP_VOLTAGE_MIN,
    POWER_SUPPLY_PROP_VOLTAGE_MAX_DESIGN,
    POWER_SUPPLY_PROP_VOLTAGE_MIN_DESIGN,
    POWER_SUPPLY_PROP_VOLTAGE_NOW,
    POWER_SUPPLY_PROP_VOLTAGE_AVG,
    POWER_SUPPLY_PROP_VOLTAGE_OCV,
    POWER_SUPPLY_PROP_VOLTAGE_BOOT,
    POWER_SUPPLY_PROP_CURRENT_MAX,
    POWER_SUPPLY_PROP_CURRENT_NOW,
    POWER_SUPPLY_PROP_CURRENT_AVG,
    POWER_SUPPLY_PROP_CURRENT_BOOT,
    POWER_SUPPLY_PROP_POWER_NOW,
    POWER_SUPPLY_PROP_POWER_AVG,
    POWER_SUPPLY_PROP_CHARGE_FULL_DESIGN,
    POWER_SUPPLY_PROP_CHARGE_EMPTY_DESIGN,
    POWER_SUPPLY_PROP_CHARGE_FULL,
    POWER_SUPPLY_PROP_CHARGE_EMPTY,
    POWER_SUPPLY_PROP_CHARGE_NOW,
    POWER_SUPPLY_PROP_CHARGE_AVG,
    POWER_SUPPLY_PROP_CHARGE_COUNTER,
    POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT,
    POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX,
    POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE,
    POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE_MAX,
    POWER_SUPPLY_PROP_CHARGE_CONTROL_LIMIT,
    POWER_SUPPLY_PROP_CHARGE_CONTROL_LIMIT_MAX,
    POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT,
    POWER_SUPPLY_PROP_ENERGY_FULL_DESIGN,
    POWER_SUPPLY_PROP_ENERGY_EMPTY_DESIGN,
    POWER_SUPPLY_PROP_ENERGY_FULL,
    POWER_SUPPLY_PROP_ENERGY_EMPTY,
    POWER_SUPPLY_PROP_ENERGY_NOW,
    POWER_SUPPLY_PROP_ENERGY_AVG,
    POWER_SUPPLY_PROP_CAPACITY, /* in percents! */
    POWER_SUPPLY_PROP_CAPACITY_ALERT_MIN, /* in percents! */
    POWER_SUPPLY_PROP_CAPACITY_ALERT_MAX, /* in percents! */
    POWER_SUPPLY_PROP_CAPACITY_LEVEL,
    POWER_SUPPLY_PROP_TEMP,
    POWER_SUPPLY_PROP_TEMP_MAX,
    POWER_SUPPLY_PROP_TEMP_MIN,
    POWER_SUPPLY_PROP_TEMP_ALERT_MIN,
    POWER_SUPPLY_PROP_TEMP_ALERT_MAX,
    POWER_SUPPLY_PROP_TEMP_AMBIENT,
    POWER_SUPPLY_PROP_TEMP_AMBIENT_ALERT_MIN,
    POWER_SUPPLY_PROP_TEMP_AMBIENT_ALERT_MAX,
    POWER_SUPPLY_PROP_TIME_TO_EMPTY_NOW,
    POWER_SUPPLY_PROP_TIME_TO_EMPTY_AVG,
    POWER_SUPPLY_PROP_TIME_TO_FULL_NOW,
    POWER_SUPPLY_PROP_TIME_TO_FULL_AVG,
    POWER_SUPPLY_PROP_TYPE, /* use power_supply.type instead */
    POWER_SUPPLY_PROP_USB_TYPE,
    POWER_SUPPLY_PROP_SCOPE,
    POWER_SUPPLY_PROP_PRECHARGE_CURRENT,
    POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT,
    POWER_SUPPLY_PROP_CALIBRATE,
    /* Properties of type `const char *' */
    POWER_SUPPLY_PROP_MODEL_NAME,
    POWER_SUPPLY_PROP_MANUFACTURER,
    POWER_SUPPLY_PROP_SERIAL_NUMBER,
};
//电源类型
enum power_supply_type{
    POWER_SUPPLY_TYPE_UNKNOWN = 0,
    POWER_SUPPLY_TYPE_BATTERY,
    POWER_SUPPLY_TYPE_UPS,
    POWER_SUPPLY_TYPE_MAINS,
    POWER_SUPPLY_TYPE_USB, /* Standard Downstream Port */
    POWER_SUPPLY_TYPE_USB_DCP, /* Dedicated Charging Port */
    POWER_SUPPLY_TYPE_USB_CDP, /* Charging Downstream Port */
    POWER_SUPPLY_TYPE_USB_ACA, /* Accessory Charger Adapters */
    POWER_SUPPLY_TYPE_USB_TYPE_C, /* Type C Port */
    POWER_SUPPLY_TYPE_USB_PD, /* Power Delivery Port */
    POWER_SUPPLY_TYPE_USB_PD_DRP, /* PD Dual Role Port */
    POWER_SUPPLY_TYPE_APPLE_BRICK_ID, /* Apple Charging Method */
};
typedef struct ec_reg{
    u32	addr;	/* 地址为EC flash地址和ACPI命令 */
    u8	index;	/* 索引是ACPI命令的索引 */
    u16	val;	/* val为EC flash数据和EC空间值 */
    u8	flag;	/*不同的访问方法. */
}ec_reg;
#define	EC_IOC_MAGIC		'E'
#define	IOCTL_RDREG		_IOR(EC_IOC_MAGIC, 1, int)
#define	IOCTL_WRREG		_IOW(EC_IOC_MAGIC, 2, int)
int fd = 0;
char all = 0;
ec_reg ecreg = {
        .addr = 0x0,
        .index = 0x0,
        .val = 0x0,
        .flag = 0x0
};
void sig_handler(int sig)
{
    close(fd);
    system("sudo rmmod ./host_ec.ko");
    exit(0);
}
const char *exe = NULL;
int main(int argc, char **argv)
{
    system("sudo insmod ./host_ec.ko");
    fd = open("/dev/ec_misc", O_RDWR);
    if (fd < 0)
    {
        perror("ec_misc open error");
        system("sudo rmmod ./host_ec.ko");
        exit(fd);
    }
    printf("ec_misc fd = %d\n", fd);
    signal(SIGINT, sig_handler);
    int ret = 0;
    if (argc == 5)
    {
        printf("./run.out <addrs> <index> <val> <flag>\n");
        printf("%s will write base addrs\n%s is Offset\n%s is write val\n%s is 1: noindex 0:index\n", argv[1], argv[2], argv[3], argv[4]);
        exe = argv[0];
        ecreg.addr = atoi(argv[1]);
        ecreg.index = atoi(argv[2]);
        ecreg.val = atoi(argv[3]);
        ecreg.flag = atoi(argv[4]);
        ret = ioctl(fd, IOCTL_WRREG, &ecreg);
        if (ret == -1)
        {
            printf("ioctl: %s\n", strerror(errno));
            close(fd);
            system("sudo rmmod ./host_ec.ko");
            exit(ret);
        }
        ecreg.val = 0;
        ret = ioctl(fd, IOCTL_RDREG, &ecreg);
        if (ret == -1)
        {
            printf("ioctl: %s\n", strerror(errno));
            close(fd);
            system("sudo rmmod ./host_ec.ko");
            exit(ret);
        }
        printf("IOCTL WRITE REG32 \naddrs=%#x\tval=%#x\n", (ecreg.addr + ecreg.index), ecreg.val);
    }
    else if (argc == 4)
    {
        printf("./run.out <addrs> <index> <flag>\n");
        printf("%s will wrte base addrs\n%s is Offset\n%s is 1: noindex 0:index\n", argv[1], argv[2], argv[3]);
        exe = argv[0];
        ecreg.addr = atoi(argv[1]);
        ecreg.index = atoi(argv[2]);
        ecreg.val = 0;
        ecreg.flag = atoi(argv[3]);
        ret = ioctl(fd, IOCTL_RDREG, &ecreg);
        if (ret == -1)
        {
            printf("ioctl: %s\n", strerror(errno));
            close(fd);
            system("sudo rmmod ./host_ec.ko");
            exit(ret);
        }
        printf("IOCTL READ REG32 \naddrs=%#x\tval=%#x\n", (ecreg.addr + ecreg.index), ecreg.val);
    }
    else
    {
        printf("./run.out [<addrs> <index>  [val]<flag>]\n");
        printf("it's read all\n");
        all = 1;
    }
    if (all)
    {
        int ac_fd = open("/sys/class/power_supply/ecmisc-ac/uevent", O_RDONLY);
        if (ac_fd < 0)
        {
            perror("ac_fd open error");
            close(fd);
            system("sudo rmmod ./host_ec.ko");
            exit(ac_fd);
        }
        int bat_fd = open("/sys/class/power_supply/ecmisc-bat/uevent", O_RDONLY);
        if (bat_fd < 0)
        {
            perror("bat_fd open error");
            close(fd);
            close(ac_fd);
            system("sudo rmmod ./host_ec.ko");
            exit(bat_fd);
        }
        char ch[4096];
        do
        {
            ret = read(ac_fd, ch, sizeof(ch));
            write(STDOUT_FILENO, ch, ret);
        }
        while (ret == 4096);
        do
        {
            ret = read(bat_fd, ch, sizeof(ch));
            write(STDOUT_FILENO, ch, ret);
        }
        while (ret == 4096);
        close(ac_fd); close(bat_fd);
        ecreg.flag = 0;
        for (ecreg.addr = 0; ecreg.addr < 1000; ecreg.addr++)
        {
            ecreg.val = 0;
            ioctl(fd, IOCTL_RDREG, &ecreg);
            printf("%#x:%#x\n", ecreg.addr, ecreg.val);
        }
    }
    else
    {
        printf("%#x:%#x\n", ecreg.addr, ecreg.val);
    }
    close(fd);
    system("sudo rmmod ./host_ec.ko");
    return 0;
}