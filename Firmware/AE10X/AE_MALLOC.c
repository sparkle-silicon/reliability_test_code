/*
 * @Author: Linyu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-11-15 18:08:25
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
 // #pragma GCC diagnostic push
 // #pragma GCC diagnostic ignored "-Warray-bounds"
 // // Your code
 // #pragma GCC diagnostic pop
#include "AE_MALLOC.H"
 /* Defined by the linker */
extern BYTE _heap_start[];
extern BYTE _heap_end;
#ifdef USER_AE10X_LIBC_A
static BYTEP heap_prt = NULL;
static size_t* heap_size = (size_t*)_heap_start;
static size_t* heap_info_size = (size_t*)(_heap_start + sizeof(size_t));
static BYTEP heap_info = _heap_start + 2 * sizeof(size_t);//0x26550
static BYTEP mem_start = NULL;
void* _sbrk(size_t nbyte) // 申请空间
{
	size_t bit, cnt;
	heap_prt = NULL;
	if (mem_start == NULL) // 初始化
	{
		*heap_size = ((&_heap_end) - (_heap_start)); // 获得堆栈大小
		*heap_info_size = *heap_size / 32;			  // 获得信息存储空间大小
		heap_info = (BYTEP)heap_info_size + sizeof(size_t);
		for (DWORD i = 0; i < *heap_info_size; i++)
			*(heap_info + i) = 0;									// 赋予内容
		mem_start = heap_info + *heap_info_size;					// 每1字节代表32个字节即为每1位代表4个字节
		mem_start = (BYTEP)(((DWORD)mem_start + 3) & (0xfffffffc)); // 4字节对齐
	}
	for (bit = 0, cnt = 0; bit < *heap_size / 4; bit++) // 查找合适位置
	{
		if ((*(heap_info + bit / 8)) & (1 << (bit % 8)))
			cnt = 0;
		else
			cnt++;
		if (cnt * 4 >= nbyte)
		{
			bit -= (cnt - 1);
			heap_prt = mem_start + bit * 4;
			break;
		}
	}
	if (heap_prt == NULL || (heap_prt + nbyte) >= &_heap_end) // 因为实际信息比计算信息空间更小(字节对齐，info也算在内等因素)
	{
		return NULL;
	}
	else
	{
		while (bit < bit + cnt)
		{
			(*(heap_info + bit / 8)) |= (1 << (bit % 8));
			bit++;
			cnt--;
		}
		*(size_t*)heap_prt = nbyte;
		heap_prt += sizeof(size_t);
		return (void*)heap_prt;
	}
}
void* malloc_r(size_t nbyte) // 数据处理
{
	size_t byte = (size_t)(((DWORD)nbyte + 3) & (0xfffffffc)); // 4字节对齐
	byte += sizeof(size_t);									   // 加入4字节存储申请空间大小
	if (byte == 0 || byte < nbyte)
		return NULL;
	if (byte < 8)
		nbyte = 8; // 至少申请8个字节
	return _sbrk(byte);
}
void* malloc(size_t nbytes) /* get a block */
{
	if (nbytes == 0)
		return NULL; // 防0
	return malloc_r(nbytes);
}
void free(void* aptr)
{
	size_t nbyte = *(size_t*)((BYTEP)aptr - sizeof(size_t)); // 获取信息
	size_t block = nbyte / 4;								  // 获取大小
	size_t cnt = 0;
	DWORD bit = ((DWORD)aptr - sizeof(size_t) - (DWORD)mem_start) / 4;
	while (cnt < block) // 释放
	{
		*(heap_info + bit / 8) &= (~(1 << (bit % 8)));
		cnt++;
		bit++;
	}
}
void* realloc(void* aptr, size_t nbytes) // 重新申请
{
	DWORD i = 0;
	void* new_mem = malloc(nbytes);												   // 申请新空间
	size_t nbyte = ((*(size_t*)((BYTEP)aptr - sizeof(size_t))) - sizeof(size_t)); // 获取大小
	while (i < nbyte)															   // 转移
	{
		*((BYTEP)new_mem + i) = *((BYTEP)aptr + i);
		i++;
	}
	free(aptr); // 释放空间
	return new_mem;
}
void* calloc(size_t num, size_t size) // 初始化malloc
{
	void* new_mem = malloc(num * size);
	size_t cnt;
	for (cnt = 0; cnt < num * size; cnt++)
	{
		*((BYTEP)new_mem + cnt) = 0;
	}
	return new_mem;
}
int memcmp(const void* mem1, const void* mem2, size_t cnt)
{
	const int8_t* s1 = (const int8_t*)mem1;
	const int8_t* s2 = (const int8_t*)mem2;
	for (size_t i = 0; i < cnt; i++)
	{
		if (*s1 != *s2)
		{
			return (int)(*s1 - *s2);
		}
		s1++;
		s2++;
	}
	return 0;
}
void* memset(void* str, int c, size_t n)
{
	c &= 0xff;
	for (size_t i = 0; i < n; i++)
		*((BYTEP)str + i) = c;
	return str;
}
void* memcpy(void* str1, const void* str2, size_t num)
{
	void* ret = str1;
	BYTEP dest = (BYTEP)str1;
	BYTEP src = (BYTEP)str2;
	if (dest == NULL || src == NULL)
		return ((void*)-1);
	while (num--)
	{
		*dest = *src;
		dest++;
		src++;
	}
	return ret;
}
#if 1
void* memchr(const void* str, int c, size_t n)
{
	char* mem = (char*)str;
	size_t i = 0;
	while (*(int*)(mem + i) != c)
	{
		if (i == n)
			return NULL;
		i++;
	}
	return (void*)(mem + i);
}
void* memmove(void* dst, const void* src, size_t len)
{
	char* d = dst;
	const char* s = src;
	/* The overlap case is allegedly rare - with this implementation
	   it will have a high penalty on the GR6.  */
	if (s < d && d < s + len)
	{
		s += len;
		d += len;
		while (len--)
			*--d = *--s;
		return dst;
	}
	return memcpy(dst, src, len);
}
#endif
#elif defined(USER_RISCV_LIBC_A)
#include <errno.h>
USED void*
_sbrk(int incr)
{
	static BYTE* heap_end = NULL; /* Previous end of heap or 0 if none */
	BYTE* prev_heap_end;
	if (NULL == heap_end)
	{
		heap_end = _heap_start; /* Initialize first time round */
	}
	prev_heap_end = heap_end;
	heap_end += incr;
	// check
	if ((BYTEP)(heap_end + incr) < (BYTEP)(&_heap_end))
	{
	}
	else
	{
		errno = ENOMEM;
		return (char*)-1;
	}
	return (void*)prev_heap_end;
} /* _sbrk () */
USED int _close(int __fildes)
{
	return 0;
}
USED int _fstat(int __fd, struct stat* __sbuf)
{
	return 0;
}
USED _off_t _lseek(int __fildes, _off_t __offset, int __whence)
{
	return 0;
}
#endif
#if SUPPORT_MEM_MALLOC
// 一下为其他事项方式，相互冲突
static unsigned int sum = 0;
#define MEM_START _heap_start
#define MEM_END &_heap_end
#define MEM_SIZE (&_heap_end - _heap_start)
#define BLK_SIZE sizeof(mem_block)
int mem_malloc(unsigned int msize)
{
	unsigned int all_size = msize + sizeof(mem_block);
	mem_block tmp_blk;
	if (msize == 0)
		return 0;
	if (sum)
	{
		mem_block* ptr_blk = (mem_block*)(MEM_START + BLK_SIZE * (sum - 1));
		int free_blk = (BYTEP)ptr_blk->mem_ptr - (MEM_START + BLK_SIZE * sum);
		if (all_size <= free_blk)
		{
			tmp_blk.mem_ptr = ptr_blk->mem_ptr - msize;
			tmp_blk.mem_size = msize;
			tmp_blk.mem_index = ptr_blk->mem_index + 1;
			memcpy(MEM_START + BLK_SIZE * sum, &tmp_blk, BLK_SIZE);
			sum = sum + 1;
			return tmp_blk.mem_index;
		}
	}
	else
	{
		if (all_size <= MEM_SIZE)
		{
			tmp_blk.mem_ptr = MEM_END - msize;
			tmp_blk.mem_size = msize;
			tmp_blk.mem_index = 1;
			memcpy(MEM_START, &tmp_blk, BLK_SIZE);
			sum = 1;
			return 1;
		}
	}
	return 0;
}
int mem_realloc(int id, unsigned int msize)
{
	for (int i = 0; i < sum; i++)
	{
		mem_block* ptr_blk = (mem_block*)(MEM_START + BLK_SIZE * i);
		if (id == ptr_blk->mem_index)
		{
			int free_blk = (BYTEP)ptr_blk->mem_ptr - (MEM_START + BLK_SIZE * sum);
			int old_size = ptr_blk->mem_size;
			int offset = msize - old_size;
			int move_size = 0;
			int n = sum - i;
			mem_block* ptr_tmp;
			if (offset == 0)
			{
				return 0;
			}
			else if (offset < 0)
			{
				offset = old_size - msize;
				for (int j = 1; j < n; j++)
				{
					ptr_tmp = (mem_block*)(MEM_START + BLK_SIZE * (i + j));
					move_size += ptr_tmp->mem_size;
				}
				if (n == 1)
				{
					ptr_tmp = (mem_block*)(MEM_START + BLK_SIZE * i);
				}
				move_size += msize;
				char* dst_addr = ptr_tmp->mem_ptr + move_size + offset - 1;
				char* src_addr = ptr_tmp->mem_ptr + move_size - 1;
				for (int j = move_size; j > 0; j--)
				{
					*dst_addr-- = *src_addr--;
				}
				memset(src_addr, 0, offset + 1);
				for (int j = 0; j < n; j++)
				{
					ptr_tmp = (mem_block*)(MEM_START + BLK_SIZE * (i + j));
					ptr_tmp->mem_ptr += offset;
					if (j == 0)
					{
						ptr_tmp->mem_size = msize;
					}
				}
				return 1;
			}
			else
			{
				if (offset <= free_blk)
				{
					for (int j = 1; j < n; j++)
					{
						ptr_tmp = (mem_block*)(MEM_START + BLK_SIZE * (i + j));
						move_size += ptr_tmp->mem_size;
					}
					if (n == 1)
					{
						ptr_tmp = (mem_block*)(MEM_START + BLK_SIZE * i);
					}
					move_size += old_size;
					char* dst_addr = ptr_tmp->mem_ptr - offset;
					char* src_addr = ptr_tmp->mem_ptr;
					for (int j = 0; j < move_size; j++)
					{
						*dst_addr++ = *src_addr++;
					}
					for (int j = 0; j < n; j++)
					{
						ptr_tmp = (mem_block*)(MEM_START + BLK_SIZE * (i + j));
						ptr_tmp->mem_ptr -= offset;
						if (j == 0)
						{
							ptr_tmp->mem_size = msize;
						}
					}
					return 1;
				}
			}
		}
	}
	return 0;
}
void* mem_buffer(int id)
{
	for (int i = 0; i < sum; i++)
	{
		mem_block* ptr_blk = (mem_block*)(MEM_START + BLK_SIZE * i);
		if (id == ptr_blk->mem_index)
		{
			return ptr_blk->mem_ptr;
		}
	}
	return NULL;
}
int mem_free(int id)
{
	for (int i = 0; i < sum; i++)
	{
		mem_block* ptr_blk = (mem_block*)(MEM_START + BLK_SIZE * i);
		if (id == ptr_blk->mem_index)
		{
			mem_block* ptr_old;
			if (i != (sum - 1))
			{
				int offset = ptr_blk->mem_size;
				int move_size = 0;
				int n = sum - i;
				mem_block* ptr_tmp;
				for (int j = 1; j < n; j++)
				{
					ptr_tmp = (mem_block*)(MEM_START + BLK_SIZE * (i + j));
					move_size += ptr_tmp->mem_size;
				}
				// memmove();
				char* dst_addr = ptr_tmp->mem_ptr + move_size + offset - 1;
				char* src_addr = ptr_tmp->mem_ptr + move_size - 1;
				for (int j = move_size; j > 0; j--)
				{
					*dst_addr-- = *src_addr--;
				}
				memset(src_addr, 0, offset + 1);
				for (int j = 0; j < (n - 1); j++)
				{
					ptr_tmp = (mem_block*)(MEM_START + BLK_SIZE * (i + j));
					ptr_old = (mem_block*)(MEM_START + BLK_SIZE * (i + j + 1));
					memcpy(ptr_tmp, ptr_old, BLK_SIZE);
					ptr_tmp->mem_ptr += offset;
				}
			}
			else
			{
				ptr_old = (mem_block*)(MEM_START + BLK_SIZE * i);
				memset(ptr_old->mem_ptr, 0, ptr_old->mem_size);
			}
			memset(ptr_old, 0, BLK_SIZE);
			sum = sum - 1;
			return 1;
		}
	}
	return 0;
}
#endif
