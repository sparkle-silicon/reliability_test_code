/*
 * @Author: Linyu
 * @LastEditors: Linyu
 * @LastEditTime: 2023-04-02 21:17:53
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
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#define MAP_SIZE	0x2000000
#define EC_REG_BASE     0x1C000000
int dev_fd = 0;
unsigned long addr;
unsigned long value;
char *ptr;
unsigned long base_addr;
unsigned long other_addr;
int main(int argc, char **argv)
{
	dev_fd = open("/dev/mem", O_RDWR | O_NDELAY);
	addr = strtoul(argv[1], &ptr, 16);
	base_addr = addr & 0xff000000;
	other_addr = addr & 0xffffff;
//	printf("base_addr is %#x ,other_addr is %#x \n",base_addr,other_addr);
	if (dev_fd < 0)
	{
		printf("Open /dev/mem failed \n");
		return 0;
	}
	unsigned char *map_base = (unsigned char *)mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, dev_fd, base_addr);
	if (argv[2] == NULL)
	{
		unsigned char var = *(volatile unsigned char *)(map_base + other_addr);
		printf("%#x\n", var);
	}
	else
	{
		value = strtoul(argv[2], &ptr, 16);
		*(volatile unsigned char *)(map_base + other_addr) = value;
	}
	if (dev_fd)
		close(dev_fd);
	munmap(map_base, MAP_SIZE);
}
