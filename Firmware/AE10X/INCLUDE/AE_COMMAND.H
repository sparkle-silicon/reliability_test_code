/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-09-15 18:13:19
 * @Description:
 *
 *
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 以下是中文及英文版权同步声明，编码为UTF-8。
 * Copyright has legal benefits, and violations must be investigated.
 * 版权具有法律效益，违法必究。
 *
 * Copyright ©2021-2025 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2025龙晶石半导体科技（苏州）有限公司
 */
#ifndef AE_COMMAND_H
#define AE_COMMAND_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#define CMD_BUFF_MAX 255
extern char CMD_UART_BUFF[CMD_BUFF_MAX + 1];
#ifndef NULL
#define NULL ((void *)0)
#endif
typedef BYTE uint8_t;
typedef struct cmd_tbl
{
	char *name;	 /* 命令名		*/
	int maxargs; /* 参数的最大数量	*/
	/* 功能函数	*/
	int (*cmd)(struct cmd_tbl *cmd, int flags, int argc,
			   char *const argv[]);
	char *usage;	  /* 使用信息	(short)	*/
	const char *help; /* 帮助信息	(long)	*/
} cmd_tbl;
extern void CMD_RUN(volatile char *cnt, char *buf);
#endif
