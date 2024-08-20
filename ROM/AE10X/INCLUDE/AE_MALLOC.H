/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-09 10:01:18
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
#ifndef AE_MALLOC_H
#define AE_MALLOC_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
extern void *malloc(size_t nbytes); /* get a block */
extern void free(void *aptr);
extern  void *memset(void *str, int c, size_t n);
#if SUPPORT_MEM_MALLOC
#pragma pack(1)
typedef struct mem_block{
    void *mem_ptr;
    unsigned int mem_size;
    unsigned int mem_index;
} mem_block;
#pragma pack()
int mem_malloc(unsigned int msize);
int mem_realloc(int id, unsigned int msize);
void *mem_buffer(int id);
int mem_free(int id);
#endif
#endif