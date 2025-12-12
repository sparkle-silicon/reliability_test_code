/*
 * @Author: Linyu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2024-04-22 15:42:54
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
#ifndef AE_LIBC_H
#define AE_LIBC_H
#include <AE_GLOBAL.H>
#include <AE_CONFIG.H>
#if defined(USER_AE10X_LIBC_A) || 1
#include <AE_MALLOC.H>
#include <AE_PRINTF.H>
#include <AE_SOFT_FP.H>
// signed.h
#ifndef _SIGNAL_H_
#define SIGINT 2   /* Interactive attention signal.  */
#define SIGILL 4   /* Illegal instruction.  */
#define SIGABRT 6  /* Abnormal termination.  */
#define SIGFPE 8   /* Erroneous arithmetic operation.  */
#define SIGSEGV 11 /* Invalid access to storage.  */
#define SIGTERM 15 /* Termination request.  */
/* Historical signals specified by POSIX. */
#define SIGHUP 1   /* Hangup.  */
#define SIGQUIT 3  /* Quit.  */
#define SIGTRAP 5  /* Trace/breakpoint trap.  */
#define SIGKILL 9  /* Killed.  */
#define SIGPIPE 13 /* Broken pipe.  */
#define SIGALRM 14 /* Alarm clock.  */
/* Archaic names for compatibility.  */
#define SIGIO SIGPOLL  /* I/O now possible (4.2 BSD).  */
#define SIGIOT SIGABRT /* IOT instruction, abort() on a PDP-11.  */
#define SIGCLD SIGCHLD /* Old System V name */
// assert.h
#ifndef assert
#define assert(x) ((void)0)
#endif
#endif
// errno.h
#undef errno
#define errno (*__errno())
extern int *__errno(void);
#ifndef RAND_MAX
#define RAND_MAX INT_MAX
#endif
#endif
#define Wtype_MAX ((Wtype)(((UWtype)1 << (W_TYPE_SIZE - 1)) - 1))
#define Wtype_MIN (- Wtype_MAX - 1)
#endif