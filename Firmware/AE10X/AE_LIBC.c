/*
 * @Author: Linyu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2024-07-31 14:45:54
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
#include <AE_LIBC.H>
#include <AE_INIT.H>
#if defined(USER_AE10X_LIBC_A)
void __assert_func(const char *file,
                   int line,
                   const char *func,
                   const char *failedexpr)
{
    dprint("assertion \"%s\" failed: file \"%s\", line %d%s%s\n", failedexpr, file, line, func ? ", function: " : "", func ? func : "");
    abort();
    /* NOTREACHED */
}
char *setlocale(int category, const char *locale)
{
    UNUSED_VAR(category);
    if(locale)
    {
        if(strcmp(locale, "POSIX") && strcmp(locale, "C") && strcmp(locale, ""))
            return NULL;
    }
    return "C";
}
#ifdef _SETJMP_H_
void longjmp(jmp_buf __jmpb, int __retval)
{
    exit(0);
    while(1)
        ;
}
int setjmp(jmp_buf __jmpb)
{
    return 0;
}
#endif
struct lconv *localeconv(void)
{
    return NULL;
}
void abort(void)
{
    exit(SIGABRT);
    while(1)
        ;
}
#ifdef _SIGNAL_H_
_sig_func_ptr signal(int sig, _sig_func_ptr sig_func)
{}
int raise(int sig)
{}
#endif
#ifdef _STDLIB_H_ 
static  int rand_seed = RAND_MAX;
int rand(void)
{
    rand_seed = (1103515245 * rand_seed + read_csr(0xBDA)) & RAND_MAX;  //0xbda:mtime 32k,0xb80_0xb00 mcycle 24M,0xb82_0xb02 instructions reired counter
    return rand_seed;
}
void srand(DWORD seed)
{
    rand_seed = (int)seed;
}
int abs(int num)
{
    if(num < 0)
        return -num;
    else
        return num;
}
long labs(long x)
{
    return abs(x);
}
time_t time(time_t *t)
{
    time_t tm = (((VDWORD)timer_1s_count) + ((VDWORD)timer_1min_count) * 60 + ((VDWORD)timer_1hours_count) * 3600);
    if(t != NULL)*t = tm;
    return tm;
}
ldiv_t ldiv(long num, long denom)
{
    ldiv_t div_n;
    div_n.quot = num / denom;
    div_n.rem = num % denom;
    return div_n;
}
div_t div(int num, int denom)
{
    div_t div_n;
    div_n.quot = num / denom;
    div_n.rem = num % denom;
    return div_n;
}
#endif
int *__errno(void)
{
    return (int *)-1; // 直接出错
}
//////////////////////////////////////////////////////////////////////libgcc2//////////////////////////////////////////////////////////////////////
// DWtype SECTION(".text.riscv_operation")
// __negdi2(DWtype u)//取反
// {
//     const DWunion uu = { .ll = u };
//     const DWunion w = { {.low = -uu.s.low,
//                  .high = -uu.s.high - ((UWtype)-uu.s.low > 0) } };

//     return w.ll;
// }
// Wtype
// __addvSI3(Wtype a, Wtype b)//SItype
// {
//     Wtype w;

//     if(__builtin_add_overflow(a, b, &w))
//         abort();

//     return w;
// }
// SItype
// __addvsi3(SItype a, SItype b)
// {
//   SItype w;

//   if(__builtin_add_overflow(a, b, &w))
//     abort();

//   return w;
// }
// DWtype
// __addvDI3 (DWtype a, DWtype b)
// {
//   DWtype w;

//   if (__builtin_add_overflow (a, b, &w))
//     abort ();

//   return w;
// }
// Wtype
// __subvSI3 (Wtype a, Wtype b)//SItype
// {
//   Wtype w;

//   if (__builtin_sub_overflow (a, b, &w))
//     abort ();

//   return w;
// }
// SItype
// __subvsi3 (SItype a, SItype b)
// {
//   SItype w;

//   if (__builtin_sub_overflow (a, b, &w))
//     abort ();

//   return w;
// }
// DWtype
// __subvDI3(DWtype a, DWtype b)
// {
//     DWtype w;

//     if(__builtin_sub_overflow(a, b, &w))
//         abort();

//     return w;
// }
// Wtype
// __mulvSI3(Wtype a, Wtype b)
// {
//     Wtype w;

//     if(__builtin_mul_overflow(a, b, &w))
//         abort();

//     return w;
// }
// SItype
// __mulvsi3(SItype a, SItype b)
// {
//     SItype w;

//     if(__builtin_mul_overflow(a, b, &w))
//         abort();

//     return w;
// }
// Wtype
// __negvSI2(Wtype a)
// {
//     Wtype w;

//     if(__builtin_sub_overflow(0, a, &w))
//         abort();

//     return w;
// }
// SItype
// __negvsi2(SItype a)
// {
//     SItype w;

//     if(__builtin_sub_overflow(0, a, &w))
//         abort();

//     return w;
// }
// DWtype
// __negvDI2(DWtype a)
// {
//     DWtype w;

//     if(__builtin_sub_overflow(0, a, &w))
//         abort();

//     return w;
// }

// Wtype
// __absvSI2(Wtype a)
// {
//     const Wtype v = 0 - (a < 0);
//     Wtype w;

//     if(__builtin_add_overflow(a, v, &w))
//         abort();

//     return v ^ w;
// }
// SItype
// __absvsi2(SItype a)
// {
//   const SItype v = 0 - (a < 0);
//   SItype w;

//   if(__builtin_add_overflow(a, v, &w))
//     abort();

//   return v ^ w;
// }
// DWtype
// __absvDI2(DWtype a)
// {
//     const DWtype v = 0 - (a < 0);
//     DWtype w;

//     if(__builtin_add_overflow(a, v, &w))
//         abort();

//     return v ^ w;
// }

// DWtype
// __mulvDI3 (DWtype u, DWtype v)
// {
//   /* The unchecked multiplication needs 3 Wtype x Wtype multiplications,
//      but the checked multiplication needs only two.  */
//   const DWunion uu = {.ll = u};
//   const DWunion vv = {.ll = v};

//   if (__builtin_expect (uu.s.high == uu.s.low >> (W_TYPE_SIZE - 1), 1))
//     {
//       /* u fits in a single Wtype.  */
//       if (__builtin_expect (vv.s.high == vv.s.low >> (W_TYPE_SIZE - 1), 1))
// 	{
// 	  /* v fits in a single Wtype as well.  */
// 	  /* A single multiplication.  No overflow risk.  */
// 	  return (DWtype) uu.s.low * (DWtype) vv.s.low;
// 	}
//       else
// 	{
// 	  /* Two multiplications.  */
// 	  DWunion w0 = {.ll = (UDWtype) (UWtype) uu.s.low
// 			* (UDWtype) (UWtype) vv.s.low};
// 	  DWunion w1 = {.ll = (UDWtype) (UWtype) uu.s.low
// 			* (UDWtype) (UWtype) vv.s.high};

// 	  if (vv.s.high < 0)
// 	    w1.s.high -= uu.s.low;
// 	  if (uu.s.low < 0)
// 	    w1.ll -= vv.ll;
// 	  w1.ll += (UWtype) w0.s.high;
// 	  if (__builtin_expect (w1.s.high == w1.s.low >> (W_TYPE_SIZE - 1), 1))
// 	    {
// 	      w0.s.high = w1.s.low;
// 	      return w0.ll;
// 	    }
// 	}
//     }
//   else
//     {
//       if (__builtin_expect (vv.s.high == vv.s.low >> (W_TYPE_SIZE - 1), 1))
// 	{
// 	  /* v fits into a single Wtype.  */
// 	  /* Two multiplications.  */
// 	  DWunion w0 = {.ll = (UDWtype) (UWtype) uu.s.low
// 			* (UDWtype) (UWtype) vv.s.low};
// 	  DWunion w1 = {.ll = (UDWtype) (UWtype) uu.s.high
// 			* (UDWtype) (UWtype) vv.s.low};

// 	  if (uu.s.high < 0)
// 	    w1.s.high -= vv.s.low;
// 	  if (vv.s.low < 0)
// 	    w1.ll -= uu.ll;
// 	  w1.ll += (UWtype) w0.s.high;
// 	  if (__builtin_expect (w1.s.high == w1.s.low >> (W_TYPE_SIZE - 1), 1))
// 	    {
// 	      w0.s.high = w1.s.low;
// 	      return w0.ll;
// 	    }
// 	}
//       else
// 	{
// 	  /* A few sign checks and a single multiplication.  */
// 	  if (uu.s.high >= 0)
// 	    {
// 	      if (vv.s.high >= 0)
// 		{
// 		  if (uu.s.high == 0 && vv.s.high == 0)
// 		    {
// 		      const DWtype w = (UDWtype) (UWtype) uu.s.low
// 			* (UDWtype) (UWtype) vv.s.low;
// 		      if (__builtin_expect (w >= 0, 1))
// 			return w;
// 		    }
// 		}
// 	      else
// 		{
// 		  if (uu.s.high == 0 && vv.s.high == (Wtype) -1)
// 		    {
// 		      DWunion ww = {.ll = (UDWtype) (UWtype) uu.s.low
// 				    * (UDWtype) (UWtype) vv.s.low};

// 		      ww.s.high -= uu.s.low;
// 		      if (__builtin_expect (ww.s.high < 0, 1))
// 			return ww.ll;
// 		    }
// 		}
// 	    }
// 	  else
// 	    {
// 	      if (vv.s.high >= 0)
// 		{
// 		  if (uu.s.high == (Wtype) -1 && vv.s.high == 0)
// 		    {
// 		      DWunion ww = {.ll = (UDWtype) (UWtype) uu.s.low
// 				    * (UDWtype) (UWtype) vv.s.low};

// 		      ww.s.high -= vv.s.low;
// 		      if (__builtin_expect (ww.s.high < 0, 1))
// 			return ww.ll;
// 		    }
// 		}
// 	      else
// 		{
// 		  if ((uu.s.high & vv.s.high) == (Wtype) -1
// 		      && (uu.s.low | vv.s.low) != 0)
// 		    {
// 		      DWunion ww = {.ll = (UDWtype) (UWtype) uu.s.low
// 				    * (UDWtype) (UWtype) vv.s.low};

// 		      ww.s.high -= uu.s.low;
// 		      ww.s.high -= vv.s.low;
// 		      if (__builtin_expect (ww.s.high >= 0, 1))
// 			return ww.ll;
// 		    }
// 		}
// 	    }
// 	}
//     }

//   /* Overflow.  */
//   abort ();
// }


DWtype SECTION(".text.riscv_operation") lshrdi3(DWtype u, shift_count_type b)
{
    if(b == 0)
        return u;

    const DWunion uu = { .ll = u };
    const shift_count_type bm = W_TYPE_SIZE - b;
    DWunion w;

    if(bm <= 0)
    {
        w.s.high = 0;
        w.s.low = (UWtype)uu.s.high >> -bm;
    }
    else
    {
        const UWtype carries = (UWtype)uu.s.high << bm;

        w.s.high = (UWtype)uu.s.high >> b;
        w.s.low = ((UWtype)uu.s.low >> b) | carries;
    }

    return w.ll;
}
DWtype SECTION(".text.riscv_operation") ashldi3(DWtype u, shift_count_type b)
{
    if(b == 0)
        return u;

    const DWunion uu = { .ll = u };
    const shift_count_type bm = W_TYPE_SIZE - b;
    DWunion w;

    if(bm <= 0)
    {
        w.s.low = 0;
        w.s.high = (UWtype)uu.s.low << -bm;
    }
    else
    {
        const UWtype carries = (UWtype)uu.s.low >> bm;

        w.s.low = (UWtype)uu.s.low << b;
        w.s.high = ((UWtype)uu.s.high << b) | carries;
    }

    return w.ll;
}
DWtype SECTION(".text.riscv_operation") ashrdi3(DWtype u, shift_count_type b)
{
    if(b == 0)
        return u;

    const DWunion uu = { .ll = u };
    const shift_count_type bm = W_TYPE_SIZE - b;
    DWunion w;

    if(bm <= 0)
    {
      /* w.s.high = 1..1 or 0..0 */
        w.s.high = uu.s.high >> (W_TYPE_SIZE - 1);
        w.s.low = uu.s.high >> -bm;
    }
    else
    {
        const UWtype carries = (UWtype)uu.s.high << bm;

        w.s.high = uu.s.high >> b;
        w.s.low = ((UWtype)uu.s.low >> b) | carries;
    }

    return w.ll;
}

// SItype
// __bswapsi2(SItype u)
// {
//     return ((((u) & 0xff000000u) >> 24)
//         | (((u) & 0x00ff0000u) >> 8)
//         | (((u) & 0x0000ff00u) << 8)
//         | (((u) & 0x000000ffu) << 24));
// }
// DItype
// __bswapdi2(DItype u)
// {
//     return ((((u) & 0xff00000000000000ull) >> 56)
//         | (((u) & 0x00ff000000000000ull) >> 40)
//         | (((u) & 0x0000ff0000000000ull) >> 24)
//         | (((u) & 0x000000ff00000000ull) >> 8)
//         | (((u) & 0x00000000ff000000ull) << 8)
//         | (((u) & 0x0000000000ff0000ull) << 24)
//         | (((u) & 0x000000000000ff00ull) << 40)
//         | (((u) & 0x00000000000000ffull) << 56));
// }

// int
// __ffsSI2(UWtype u)
// {
//     UWtype count;

//     if(u == 0)
//         return 0;

//     count_trailing_zeros(count, u);
//     return count + 1;
// }
DWtype SECTION(".text.riscv_operation")
muldi3(DWtype u, DWtype v)
{
    const DWunion uu = { .ll = u };
    const DWunion vv = { .ll = v };
    DWunion w;
    // DWunion w = { .ll = __umulsidi3(uu.s.low,vv.s.low) };//使用宏會跳轉回來，因為強轉成8字節計算了
    USItype __x0, __x1, __x2, __x3;
    USItype __ul, __vl, __uh, __vh;
    __ul = ((USItype)(uu.s.low) & 0xffff);
    __uh = ((USItype)(uu.s.low) >> 16);
    __vl = ((USItype)(vv.s.low) & 0xffff);
    __vh = ((USItype)(vv.s.low) >> 16);
    __x0 = (USItype)__ul * __vl;
    __x1 = (USItype)__ul * __vh;
    __x2 = (USItype)__uh * __vl;
    __x3 = (USItype)__uh * __vh;
    //x1=x1+x2+x0h//16
    __x1 += ((USItype)(__x0) >> 16);//因為最大0xffff*0xffff+0xffff＝0xfffeffff，所以不用判斷進位
    __x1 += __x2;
    if(__x1 < __x2)//進位,因为如正常来说更大或相等（+0），如果更小了说明溢出了最大溢出位，最大0x1 FFFD 0000,所以+1即可
        __x3 += ((USItype)1 << 16);
    (w.s.high) = __x3 + ((USItype)(__x1) >> 16);//高位相加
    (w.s.low) = ((USItype)(__x1) << 16) + ((USItype)(__x0) & 0xffff);//低位相加

    w.s.high += ((UWtype)uu.s.low * (UWtype)vv.s.high
             + (UWtype)uu.s.high * (UWtype)vv.s.low);

    return w.ll;
}
/* If sdiv_qrnnd doesn't exist, define dummy __udiv_w_sdiv.  */
// UWtype
// __udiv_w_sdiv(UWtype *rp __attribute__((__unused__)),
//          UWtype a1 __attribute__((__unused__)),
//          UWtype a0 __attribute__((__unused__)),
//          UWtype d __attribute__((__unused__)))
// {
//   return 0;
// }
// #if (defined (L_udivdi3) || defined (L_divdi3) || defined (L_umoddi3) || defined (L_moddi3))
// static inline __attribute__ ((__always_inline__))
// #endif
// UWtype
// __udiv_w_sdiv (UWtype *rp, UWtype a1, UWtype a0, UWtype d)
// {
//   UWtype q, r;
//   UWtype c0, c1, b1;

//   if ((Wtype) d >= 0)
//     {
//       if (a1 < d - a1 - (a0 >> (W_TYPE_SIZE - 1)))
// 	{
// 	  /* Dividend, divisor, and quotient are nonnegative.  */
// 	  sdiv_qrnnd (q, r, a1, a0, d);
// 	}
//       else
// 	{
// 	  /* Compute c1*2^32 + c0 = a1*2^32 + a0 - 2^31*d.  */
// 	  sub_ddmmss (c1, c0, a1, a0, d >> 1, d << (W_TYPE_SIZE - 1));
// 	  /* Divide (c1*2^32 + c0) by d.  */
// 	  sdiv_qrnnd (q, r, c1, c0, d);
// 	  /* Add 2^31 to quotient.  */
// 	  q += (UWtype) 1 << (W_TYPE_SIZE - 1);
// 	}
//     }
//   else
//     {
//       b1 = d >> 1;			/* d/2, between 2^30 and 2^31 - 1 */
//       c1 = a1 >> 1;			/* A/2 */
//       c0 = (a1 << (W_TYPE_SIZE - 1)) + (a0 >> 1);

//       if (a1 < b1)			/* A < 2^32*b1, so A/2 < 2^31*b1 */
// 	{
// 	  sdiv_qrnnd (q, r, c1, c0, b1); /* (A/2) / (d/2) */

// 	  r = 2*r + (a0 & 1);		/* Remainder from A/(2*b1) */
// 	  if ((d & 1) != 0)
// 	    {
// 	      if (r >= q)
// 		r = r - q;
// 	      else if (q - r <= d)
// 		{
// 		  r = r - q + d;
// 		  q--;
// 		}
// 	      else
// 		{
// 		  r = r - q + 2*d;
// 		  q -= 2;
// 		}
// 	    }
// 	}
//       else if (c1 < b1)			/* So 2^31 <= (A/2)/b1 < 2^32 */
// 	{
// 	  c1 = (b1 - 1) - c1;
// 	  c0 = ~c0;			/* logical NOT */

// 	  sdiv_qrnnd (q, r, c1, c0, b1); /* (A/2) / (d/2) */

// 	  q = ~q;			/* (A/2)/b1 */
// 	  r = (b1 - 1) - r;

// 	  r = 2*r + (a0 & 1);		/* A/(2*b1) */

// 	  if ((d & 1) != 0)
// 	    {
// 	      if (r >= q)
// 		r = r - q;
// 	      else if (q - r <= d)
// 		{
// 		  r = r - q + d;
// 		  q--;
// 		}
// 	      else
// 		{
// 		  r = r - q + 2*d;
// 		  q -= 2;
// 		}
// 	    }
// 	}
//       else				/* Implies c1 = b1 */
// 	{				/* Hence a1 = d - 1 = 2*b1 - 1 */
// 	  if (a0 >= -d)
// 	    {
// 	      q = -1;
// 	      r = a0 + d;
// 	    }
// 	  else
// 	    {
// 	      q = -2;
// 	      r = a0 + 2*d;
// 	    }
// 	}
//     }

//   *rp = r;
//   return q;
// }

// const UQItype __clz_tab[256] =
// {
//   0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
//   6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
//   7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
//   7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
//   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
//   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
//   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
//   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
// };

//计算前面的0有多少位(返回最大值)
// int
// __clzSI2(UWtype x)
// {
//   Wtype ret;

//   count_leading_zeros(ret, x);

//   return ret;
// }

int SECTION(".text.riscv_operation") clzsi2(int val)//Count Leading Zeros single integer 
{
    register int i = 32;
    register int j = 16;
    register int temp;
    for(; j; j >>= 1)//j有值，每轮结束j/2，即，16,8,4,2,1,0
    {
        temp = val >> j;//判断val右移j位还有值吗，有值则判断j是否到1，，到1则返回i-2，否则则令val等于右移的结果，i减去j值
        if(temp)
        {
            if(j == 1)
            {
                return (i - 2);
            }
            else
            {
                i -= j;
                val = temp;
            }
        }
    }
    return (i - val);
}
// int
// __clzDI2(UDWtype x)
// {
//     const DWunion uu = { .ll = x };
//     UWtype word;
//     Wtype ret, add;

//     if(uu.s.high)
//         word = uu.s.high, add = 0;
//     else
//         word = uu.s.low, add = W_TYPE_SIZE;

//     count_leading_zeros(ret, word);
//     return ret + add;
// }
int SECTION(".text.riscv_operation") clzdi2(long long val)
{
    if(val >> 32)
    {
        return clzsi2(val >> 32);
    }
    else
    {
        return clzsi2(val) + 32;
    }
}

// int
// __ctzSI2(UWtype x)
// {
//   Wtype ret;

//   count_trailing_zeros(ret, x);

//   return ret;
// }

// int
// __ctzDI2(UDWtype x)
// {
//     const DWunion uu = { .ll = x };
//     UWtype word;
//     Wtype ret, add;

//     if(uu.s.low)
//         word = uu.s.low, add = 0;
//     else
//         word = uu.s.high, add = W_TYPE_SIZE;

//     count_trailing_zeros(ret, word);
//     return ret + add;
// }
// int
// __clrsbSI2(Wtype x)
// {
//     Wtype ret;

//     if(x < 0)
//         x = ~x;
//     if(x == 0)
//         return W_TYPE_SIZE - 1;
//     count_leading_zeros(ret, x);
//     return ret - 1;
// }
// int
// __clrsbDI2(DWtype x)
// {
//     const DWunion uu = { .ll = x };
//     UWtype word;
//     Wtype ret, add;

//     if(uu.s.high == 0)
//         word = uu.s.low, add = W_TYPE_SIZE;
//     else if(uu.s.high == -1)
//         word = ~uu.s.low, add = W_TYPE_SIZE;
//     else if(uu.s.high >= 0)
//         word = uu.s.high, add = 0;
//     else
//         word = ~uu.s.high, add = 0;

//     if(word == 0)
//         ret = W_TYPE_SIZE;
//     else
//         count_leading_zeros(ret, word);

//     return ret + add - 1;
// }
// const UQItype __popcount_tab[256] =
// {
//     0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
//     1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
//     1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
//     2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
//     1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
//     2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
//     2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
//     3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8
// };

// #define POPCOUNTCST2(x) (((UWtype) x << __CHAR_BIT__) | x)
// #define POPCOUNTCST4(x) (((UWtype) x << (2 * __CHAR_BIT__)) | x)
// #define POPCOUNTCST8(x) (((UWtype) x << (4 * __CHAR_BIT__)) | x)
// #if W_TYPE_SIZE == __CHAR_BIT__
// #define POPCOUNTCST(x) x
// #elif W_TYPE_SIZE == 2 * __CHAR_BIT__
// #define POPCOUNTCST(x) POPCOUNTCST2 (x)
// #elif W_TYPE_SIZE == 4 * __CHAR_BIT__
// #define POPCOUNTCST(x) POPCOUNTCST4 (POPCOUNTCST2 (x))
// #elif W_TYPE_SIZE == 8 * __CHAR_BIT__
// #define POPCOUNTCST(x) POPCOUNTCST8 (POPCOUNTCST4 (POPCOUNTCST2 (x)))
// #endif
// int
// __popcountSI2(UWtype x)
// {
//   /* Force table lookup on targets like AVR and RL78 which only
//      pretend they have LIBGCC2_UNITS_PER_WORD 4, but actually
//      have 1, and other small word targets.  */
// #if __SIZEOF_INT__ > 2 && defined (POPCOUNTCST) && __CHAR_BIT__ == 8
//     x = x - ((x >> 1) & POPCOUNTCST(0x55));
//     x = (x & POPCOUNTCST(0x33)) + ((x >> 2) & POPCOUNTCST(0x33));
//     x = (x + (x >> 4)) & POPCOUNTCST(0x0F);
//     return (x * POPCOUNTCST(0x01)) >> (W_TYPE_SIZE - __CHAR_BIT__);
// #else
//     int i, ret = 0;

//     for(i = 0; i < W_TYPE_SIZE; i += 8)
//         ret += __popcount_tab[(x >> i) & 0xff];

//     return ret;
// #endif
// }
// int
// __popcountDI2(UDWtype x)
// {
//   /* Force table lookup on targets like AVR and RL78 which only
//      pretend they have LIBGCC2_UNITS_PER_WORD 4, but actually
//      have 1, and other small word targets.  */
// #if __SIZEOF_INT__ > 2 && defined (POPCOUNTCST) && __CHAR_BIT__ == 8
//     const DWunion uu = { .ll = x };
//     UWtype x1 = uu.s.low, x2 = uu.s.high;
//     x1 = x1 - ((x1 >> 1) & POPCOUNTCST(0x55));
//     x2 = x2 - ((x2 >> 1) & POPCOUNTCST(0x55));
//     x1 = (x1 & POPCOUNTCST(0x33)) + ((x1 >> 2) & POPCOUNTCST(0x33));
//     x2 = (x2 & POPCOUNTCST(0x33)) + ((x2 >> 2) & POPCOUNTCST(0x33));
//     x1 = (x1 + (x1 >> 4)) & POPCOUNTCST(0x0F);
//     x2 = (x2 + (x2 >> 4)) & POPCOUNTCST(0x0F);
//     x1 += x2;
//     return (x1 * POPCOUNTCST(0x01)) >> (W_TYPE_SIZE - __CHAR_BIT__);
// #else
//     int i, ret = 0;

//     for(i = 0; i < 2 * W_TYPE_SIZE; i += 8)
//         ret += __popcount_tab[(x >> i) & 0xff];

//     return ret;
// #endif
// }
// int
// __paritySI2(UWtype x)
// {
// #if W_TYPE_SIZE > 64
// # error "fill out the table"
// #endif
// #if W_TYPE_SIZE > 32
//     x ^= x >> 32;
// #endif
// #if W_TYPE_SIZE > 16
//     x ^= x >> 16;
// #endif
//     x ^= x >> 8;
//     x ^= x >> 4;
//     x &= 0xf;
//     return (0x6996 >> x) & 1;
// }
// int
// __parityDI2(UDWtype x)
// {
//     const DWunion uu = { .ll = x };
//     UWtype nx = uu.s.low ^ uu.s.high;

// #if W_TYPE_SIZE > 64
// # error "fill out the table"
// #endif
// #if W_TYPE_SIZE > 32
//     nx ^= nx >> 32;
// #endif
// #if W_TYPE_SIZE > 16
//     nx ^= nx >> 16;
// #endif
//     nx ^= nx >> 8;
//     nx ^= nx >> 4;
//     nx &= 0xf;
//     return (0x6996 >> nx) & 1;
// }


#if (defined (L_udivdi3) || defined (L_divdi3) || \
     defined (L_umoddi3) || defined (L_moddi3) || \
     defined (L_divmoddi4))
static inline __attribute__((__always_inline__))
#else 
SECTION(".text.riscv_operation")
#endif
// #ifdef TARGET_HAS_NO_HW_DIVIDE

UDWtype
__udivmoddi4(UDWtype n, UDWtype d, UDWtype *rp)
{
    UDWtype q = 0, r = n, y = d;
    UWtype lz1, lz2, i, k;

    /* Implements align divisor shift dividend method. This algorithm
       aligns the divisor under the dividend and then perform number of
       test-subtract iterations which shift the dividend left. Number of
       iterations is k + 1 where k is the number of bit positions the
       divisor must be shifted left to align it under the dividend.
       quotient bits can be saved in the rightmost positions of the dividend
       as it shifts left on each test-subtract iteration. */
    if(y <= r)
    {
        lz1 = __builtin_clzll(d);
        lz2 = __builtin_clzll(n);
        k = lz1 - lz2;
        y = (y << k);
        /* Dividend can exceed 2 ^ (width - 1) - 1 but still be less than the
       aligned divisor. Normal iteration can drops the high order bit
       of the dividend. Therefore, first test-subtract iteration is a
       special case, saving its quotient bit in a separate location and
       not shifting the dividend. */
        if(r >= y)
        {
            r = r - y;
            q = (1ULL << k);
        }
        if(k > 0)
        {
            y = y >> 1;

            /* k additional iterations where k regular test subtract shift
              dividend iterations are done.  */
            i = k;
            do
            {
                if(r >= y)
                    r = ((r - y) << 1) + 1;
                else
                    r = (r << 1);
                i = i - 1;
            }
            while(i != 0);
      /* First quotient bit is combined with the quotient bits resulting
         from the k regular iterations.  */
            q = q + r;
            r = r >> k;
            q = q - (r << k);
        }
    }
    if(rp)
        *rp = r;
    return q;
}
// #else
// #if (defined (L_udivdi3) || defined (L_divdi3) || defined (L_umoddi3) || defined (L_moddi3) || defined (L_divmoddi4))
// static inline __attribute__((__always_inline__))
// #endif
// UDWtype
// __udivmoddi4(UDWtype n, UDWtype d, UDWtype *rp)
// {
//   const DWunion nn = { .ll = n };
//   const DWunion dd = { .ll = d };
//   DWunion rr;
//   UWtype d0, d1, n0, n1, n2;
//   UWtype q0, q1;
//   UWtype b, bm;

//   d0 = dd.s.low;
//   d1 = dd.s.high;
//   n0 = nn.s.low;
//   n1 = nn.s.high;

// #if !UDIV_NEEDS_NORMALIZATION
//   if(d1 == 0)
//   {
//     if(d0 > n1)
//     {
//       /* 0q = nn / 0D */

//       udiv_qrnnd(q0, n0, n1, n0, d0);
//       q1 = 0;

//       /* Remainder in n0.  */
//     }
//     else
//     {
//       /* qq = NN / 0d */

//       if(d0 == 0)
//         d0 = 1 / d0;	/* Divide intentionally by zero.  */

//       udiv_qrnnd(q1, n1, 0, n1, d0);
//       udiv_qrnnd(q0, n0, n1, n0, d0);

//       /* Remainder in n0.  */
//     }

//     if(rp != 0)
//     {
//       rr.s.low = n0;
//       rr.s.high = 0;
//       *rp = rr.ll;
//     }
//   }

// #else /* UDIV_NEEDS_NORMALIZATION */

//   if(d1 == 0)
//   {
//     if(d0 > n1)
//     {
//       /* 0q = nn / 0D */

//       count_leading_zeros(bm, d0);

//       if(bm != 0)
//       {
//         /* Normalize, i.e. make the most significant bit of the
//      denominator set.  */

//         d0 = d0 << bm;
//         n1 = (n1 << bm) | (n0 >> (W_TYPE_SIZE - bm));
//         n0 = n0 << bm;
//       }

//       udiv_qrnnd(q0, n0, n1, n0, d0);
//       q1 = 0;

//       /* Remainder in n0 >> bm.  */
//     }
//     else
//     {
//       /* qq = NN / 0d */

//       if(d0 == 0)
//         d0 = 1 / d0;	/* Divide intentionally by zero.  */

//       count_leading_zeros(bm, d0);

//       if(bm == 0)
//       {
//         /* From (n1 >= d0) /\ (the most significant bit of d0 is set),
//      conclude (the most significant bit of n1 is set) /\ (the
//      leading quotient digit q1 = 1).

//      This special case is necessary, not an optimization.
//      (Shifts counts of W_TYPE_SIZE are undefined.)  */

//         n1 -= d0;
//         q1 = 1;
//       }
//       else
//       {
//         /* Normalize.  */

//         b = W_TYPE_SIZE - bm;

//         d0 = d0 << bm;
//         n2 = n1 >> b;
//         n1 = (n1 << bm) | (n0 >> b);
//         n0 = n0 << bm;

//         udiv_qrnnd(q1, n1, n2, n1, d0);
//       }

//     /* n1 != d0...  */

//       udiv_qrnnd(q0, n0, n1, n0, d0);

//       /* Remainder in n0 >> bm.  */
//     }

//     if(rp != 0)
//     {
//       rr.s.low = n0 >> bm;
//       rr.s.high = 0;
//       *rp = rr.ll;
//     }
//   }
// #endif /* UDIV_NEEDS_NORMALIZATION */

//   else
//   {
//     if(d1 > n1)
//     {
//       /* 00 = nn / DD */

//       q0 = 0;
//       q1 = 0;

//       /* Remainder in n1n0.  */
//       if(rp != 0)
//       {
//         rr.s.low = n0;
//         rr.s.high = n1;
//         *rp = rr.ll;
//       }
//     }
//     else
//     {
//       /* 0q = NN / dd */

//       count_leading_zeros(bm, d1);
//       if(bm == 0)
//       {
//         /* From (n1 >= d1) /\ (the most significant bit of d1 is set),
//      conclude (the most significant bit of n1 is set) /\ (the
//      quotient digit q0 = 0 or 1).

//      This special case is necessary, not an optimization.  */

//         /* The condition on the next line takes advantage of that
//      n1 >= d1 (true due to program flow).  */
//         if(n1 > d1 || n0 >= d0)
//         {
//           q0 = 1;
//           sub_ddmmss(n1, n0, n1, n0, d1, d0);
//         }
//         else
//           q0 = 0;

//         q1 = 0;

//         if(rp != 0)
//         {
//           rr.s.low = n0;
//           rr.s.high = n1;
//           *rp = rr.ll;
//         }
//       }
//       else
//       {
//         UWtype m1, m0;
//         /* Normalize.  */

//         b = W_TYPE_SIZE - bm;

//         d1 = (d1 << bm) | (d0 >> b);
//         d0 = d0 << bm;
//         n2 = n1 >> b;
//         n1 = (n1 << bm) | (n0 >> b);
//         n0 = n0 << bm;

//         udiv_qrnnd(q0, n1, n2, n1, d1);
//         umul_ppmm(m1, m0, q0, d0);

//         if(m1 > n1 || (m1 == n1 && m0 > n0))
//         {
//           q0--;
//           sub_ddmmss(m1, m0, m1, m0, d1, d0);
//         }

//         q1 = 0;

//         /* Remainder in (n1n0 - m1m0) >> bm.  */
//         if(rp != 0)
//         {
//           sub_ddmmss(n1, n0, n1, n0, m1, m0);
//           rr.s.low = (n1 << b) | (n0 >> bm);
//           rr.s.high = n1 >> bm;
//           *rp = rr.ll;
//         }
//       }
//     }
//   }

//   const DWunion ww = { {.low = q0, .high = q1} };
//   return ww.ll;
// }
// #endif

DWtype
divdi3(DWtype u, DWtype v)
{
    Wtype c = 0;
    DWunion uu = { .ll = u };
    DWunion vv = { .ll = v };
    DWtype w;

    if(uu.s.high < 0)
        c = ~c,
        uu.ll = -uu.ll;
    if(vv.s.high < 0)
        c = ~c,
        vv.ll = -vv.ll;

    w = __udivmoddi4(uu.ll, vv.ll, (UDWtype *)0);
    if(c)
        w = -w;

    return w;
}
DWtype
moddi3(DWtype u, DWtype v)
{
    Wtype c = 0;
    DWunion uu = { .ll = u };
    DWunion vv = { .ll = v };
    DWtype w;

    if(uu.s.high < 0)
        c = ~c,
        uu.ll = -uu.ll;
    if(vv.s.high < 0)
        vv.ll = -vv.ll;

    (void)__udivmoddi4(uu.ll, vv.ll, (UDWtype *)&w);
    if(c)
        w = -w;

    return w;
}
DWtype SECTION(".text.riscv_operation")
divmoddi4(DWtype u, DWtype v, DWtype *rp)
{
    Wtype c1 = 0, c2 = 0;
    DWunion uu = { .ll = u };
    DWunion vv = { .ll = v };
    DWtype w;
    DWtype r;

    if(uu.s.high < 0)
        c1 = ~c1, c2 = ~c2,
        uu.ll = -uu.ll;
    if(vv.s.high < 0)
        c1 = ~c1,
        vv.ll = -vv.ll;

    w = __udivmoddi4(uu.ll, vv.ll, (UDWtype *)&r);
    if(c1)
        w = -w;
    if(c2)
        r = -r;

    *rp = r;
    return w;
}
UDWtype SECTION(".text.riscv_operation")
umoddi3(UDWtype u, UDWtype v)
{
    UDWtype w;
    (void)__udivmoddi4(u, v, &w);
    return w;
}
UDWtype SECTION(".text.riscv_operation")
udivdi3(UDWtype n, UDWtype d)
{
    return __udivmoddi4(n, d, (UDWtype *)0);
}


// __cmpdi2(DWtype a, DWtype b)
// {
//     return (a > b) - (a < b) + 1;
// }
// cmp_return_type
// __ucmpdi2(UDWtype a, UDWtype b)
// {
//     return (a > b) - (a < b) + 1;
// }

// UDWtype
// __fixunstfDI(TFtype a)
// {
//     if(a < 0)
//         return 0;

//       /* Compute high word of result, as a flonum.  */
//     const TFtype b = (a / Wtype_MAXp1_F);
//     /* Convert that to fixed (but not to DWtype!),
//        and shift it into the high word.  */
//     UDWtype v = (UWtype)b;
//     v <<= W_TYPE_SIZE;
//     /* Remove high part from the TFtype, leaving the low part as flonum.  */
//     a -= (TFtype)v;
//     /* Convert that to fixed (but not to DWtype!) and add it in.
//        Sometimes A comes out negative.  This is significant, since
//        A has more bits than a long int does.  */
//     if(a < 0)
//         v -= (UWtype)(-a);
//     else
//         v += (UWtype)a;
//     return v;
// }
// DWtype
// __fixtfdi(TFtype a)
// {
//     if(a < 0)
//         return -__fixunstfDI(-a);
//     return __fixunstfDI(a);
// }

// UDWtype
// __fixunsdfDI(DFtype a)
// {
//   /* Get high part of result.  The division here will just moves the radix
//      point and will not cause any rounding.  Then the conversion to integral
//      type chops result as desired.  */
//     const UWtype hi = a / Wtype_MAXp1_F;

//     /* Get low part of result.  Convert `hi' to floating type and scale it back,
//        then subtract this from the number being converted.  This leaves the low
//        part.  Convert that to integral type.  */
//     const UWtype lo = a - (DFtype)hi * Wtype_MAXp1_F;

//     /* Assemble result from the two parts.  */
//     return ((UDWtype)hi << W_TYPE_SIZE) | lo;
// }
// DWtype
// __fixdfdi(DFtype a)
// {
//     if(a < 0)
//         return -__fixunsdfDI(-a);
//     return __fixunsdfDI(a);
// }
// UDWtype
// __fixunssfDI(SFtype a)
// {
// #if LIBGCC2_HAS_DF_MODE
//   /* Convert the SFtype to a DFtype, because that is surely not going
//      to lose any bits.  Some day someone else can write a faster version
//      that avoids converting to DFtype, and verify it really works right.  */
//   const DFtype dfa = a;

//   /* Get high part of result.  The division here will just moves the radix
//      point and will not cause any rounding.  Then the conversion to integral
//      type chops result as desired.  */
//   const UWtype hi = dfa / Wtype_MAXp1_F;

//   /* Get low part of result.  Convert `hi' to floating type and scale it back,
//      then subtract this from the number being converted.  This leaves the low
//      part.  Convert that to integral type.  */
//   const UWtype lo = dfa - (DFtype)hi * Wtype_MAXp1_F;

//   /* Assemble result from the two parts.  */
//   return ((UDWtype)hi << W_TYPE_SIZE) | lo;
// #elif FLT_MANT_DIG < W_TYPE_SIZE
//   if(a < 1)
//     return 0;
//   if(a < Wtype_MAXp1_F)
//     return (UWtype)a;
//   if(a < Wtype_MAXp1_F * Wtype_MAXp1_F)
//   {
//     /* Since we know that there are fewer significant bits in the SFmode
//  quantity than in a word, we know that we can convert out all the
//  significant bits in one step, and thus avoid losing bits.  */

//     /* ??? This following loop essentially performs frexpf.  If we could
//  use the real libm function, or poke at the actual bits of the fp
//  format, it would be significantly faster.  */

//     UWtype shift = 0, counter;
//     SFtype msb;

//     a /= Wtype_MAXp1_F;
//     for(counter = W_TYPE_SIZE / 2; counter != 0; counter >>= 1)
//     {
//       SFtype counterf = (UWtype)1 << counter;
//       if(a >= counterf)
//       {
//         shift |= counter;
//         a /= counterf;
//       }
//     }

//         /* Rescale into the range of one word, extract the bits of that
//      one word, and shift the result into position.  */
//     a *= Wtype_MAXp1_F;
//     counter = a;
//     return (DWtype)counter << shift;
//   }
//   return -1;
// #else
// # error
// #endif
// }

// TFtype
// __floatditf(DWtype u)
// {
// #if W_TYPE_SIZE > __LIBGCC_TF_MANT_DIG__
// # error
// #endif
//     TFtype d = (Wtype)(u >> W_TYPE_SIZE);
//     d *= Wtype_MAXp1_F;
//     d += (UWtype)u;
//     return d;
// }
// TFtype
// __floatunditf(UDWtype u)
// {
// #if W_TYPE_SIZE > __LIBGCC_TF_MANT_DIG__
// # error
// #endif
//     TFtype d = (UWtype)(u >> W_TYPE_SIZE);
//     d *= Wtype_MAXp1_F;
//     d += (UWtype)u;
//     return d;
// }

// #define DI_SIZE (W_TYPE_SIZE * 2)
// #define F_MODE_OK(SIZE) (SIZE < DI_SIZE	&& SIZE > (DI_SIZE - SIZE + FSSIZE)	&& !AVOID_FP_TYPE_CONVERSION(SIZE))
// #if defined(L_floatdisf)
// #define FUNC __floatdisf
// #define FSTYPE SFtype
// #define FSSIZE __LIBGCC_SF_MANT_DIG__
// #else
// #define FUNC __floatdidf
// #define FSTYPE DFtype
// #define FSSIZE __LIBGCC_DF_MANT_DIG__
// #endif

// FSTYPE
// FUNC(DWtype u)
// {
// #if FSSIZE >= W_TYPE_SIZE
//   /* When the word size is small, we never get any rounding error.  */
//   FSTYPE f = (Wtype)(u >> W_TYPE_SIZE);
//   f *= Wtype_MAXp1_F;
//   f += (UWtype)u;
//   return f;
// #elif (LIBGCC2_HAS_DF_MODE && F_MODE_OK (__LIBGCC_DF_MANT_DIG__)) || (LIBGCC2_HAS_XF_MODE && F_MODE_OK (__LIBGCC_XF_MANT_DIG__))	      || (LIBGCC2_HAS_TF_MODE && F_MODE_OK (__LIBGCC_TF_MANT_DIG__))

// #if (LIBGCC2_HAS_DF_MODE && F_MODE_OK (__LIBGCC_DF_MANT_DIG__))
// # define FSIZE __LIBGCC_DF_MANT_DIG__
// # define FTYPE DFtype
// #elif (LIBGCC2_HAS_XF_MODE && F_MODE_OK (__LIBGCC_XF_MANT_DIG__))
// # define FSIZE __LIBGCC_XF_MANT_DIG__
// # define FTYPE XFtype
// #elif (LIBGCC2_HAS_TF_MODE && F_MODE_OK (__LIBGCC_TF_MANT_DIG__))
// # define FSIZE __LIBGCC_TF_MANT_DIG__
// # define FTYPE TFtype
// #else
// # error
// #endif

// #define REP_BIT ((UDWtype) 1 << (DI_SIZE - FSIZE))

//   /* Protect against double-rounding error.
//      Represent any low-order bits, that might be truncated by a bit that
//      won't be lost.  The bit can go in anywhere below the rounding position
//      of the FSTYPE.  A fixed mask and bit position handles all usual
//      configurations.  */
//   if(!(-((DWtype)1 << FSIZE) < u
//     && u < ((DWtype)1 << FSIZE)))
//   {
//     if((UDWtype)u & (REP_BIT - 1))
//     {
//       u &= ~(REP_BIT - 1);
//       u |= REP_BIT;
//     }
//   }

// /* Do the calculation in a wider type so that we don't lose any of
//    the precision of the high word while multiplying it.  */
//   FTYPE f = (Wtype)(u >> W_TYPE_SIZE);
//   f *= Wtype_MAXp1_F;
//   f += (UWtype)u;
//   return (FSTYPE)f;
// #else
// #if FSSIZE >= W_TYPE_SIZE - 2
// # error
// #endif
//   /* Finally, the word size is larger than the number of bits in the
//      required FSTYPE, and we've got no suitable wider type.  The only
//      way to avoid double rounding is to special case the
//      extraction.  */

//   /* If there are no high bits set, fall back to one conversion.  */
//   if((Wtype)u == u)
//     return (FSTYPE)(Wtype)u;

//   /* Otherwise, find the power of two.  */
//   Wtype hi = u >> W_TYPE_SIZE;
//   if(hi < 0)
//     hi = -(UWtype)hi;

//   UWtype count, shift;
// #if !defined (COUNT_LEADING_ZEROS_0) || COUNT_LEADING_ZEROS_0 != W_TYPE_SIZE
//   if(hi == 0)
//     count = W_TYPE_SIZE;
//   else
//   #endif
//     count_leading_zeros(count, hi);

//     /* No leading bits means u == minimum.  */
//   if(count == 0)
//     return Wtype_MAXp1_F * (FSTYPE)(hi | ((UWtype)u != 0));

//   shift = 1 + W_TYPE_SIZE - count;

//   /* Shift down the most significant bits.  */
//   hi = u >> shift;

//   /* If we lost any nonzero bits, set the lsb to ensure correct rounding.  */
//   if((UWtype)u << (W_TYPE_SIZE - shift))
//     hi |= 1;

//   /* Convert the one word of data, and rescale.  */
//   FSTYPE f = hi, e;
//   if(shift == W_TYPE_SIZE)
//     e = Wtype_MAXp1_F;
//   /* The following two cases could be merged if we knew that the target
//      supported a native unsigned->float conversion.  More often, we only
//      have a signed conversion, and have to add extra fixup code.  */
//   else if(shift == W_TYPE_SIZE - 1)
//     e = Wtype_MAXp1_F / 2;
//   else
//     e = (Wtype)1 << shift;
//   return f * e;
// #endif
// }
// #define DI_SIZE (W_TYPE_SIZE * 2)
// #define F_MODE_OK(SIZE)    (SIZE < DI_SIZE							    && SIZE > (DI_SIZE - SIZE + FSSIZE)					    && !AVOID_FP_TYPE_CONVERSION(SIZE))
// #if defined(L_floatundisf)
// #define FUNC __floatundisf
// #define FSTYPE SFtype
// #define FSSIZE __LIBGCC_SF_MANT_DIG__
// #else
// #define FUNC __floatundidf
// #define FSTYPE DFtype
// #define FSSIZE __LIBGCC_DF_MANT_DIG__
// #endif

// FSTYPE
// FUNC(UDWtype u)
// {
// #if FSSIZE >= W_TYPE_SIZE
//   /* When the word size is small, we never get any rounding error.  */
//   FSTYPE f = (UWtype)(u >> W_TYPE_SIZE);
//   f *= Wtype_MAXp1_F;
//   f += (UWtype)u;
//   return f;
// #elif (LIBGCC2_HAS_DF_MODE && F_MODE_OK (__LIBGCC_DF_MANT_DIG__))	      || (LIBGCC2_HAS_XF_MODE && F_MODE_OK (__LIBGCC_XF_MANT_DIG__))	      || (LIBGCC2_HAS_TF_MODE && F_MODE_OK (__LIBGCC_TF_MANT_DIG__))

// #if (LIBGCC2_HAS_DF_MODE && F_MODE_OK (__LIBGCC_DF_MANT_DIG__))
// # define FSIZE __LIBGCC_DF_MANT_DIG__
// # define FTYPE DFtype
// #elif (LIBGCC2_HAS_XF_MODE && F_MODE_OK (__LIBGCC_XF_MANT_DIG__))
// # define FSIZE __LIBGCC_XF_MANT_DIG__
// # define FTYPE XFtype
// #elif (LIBGCC2_HAS_TF_MODE && F_MODE_OK (__LIBGCC_TF_MANT_DIG__))
// # define FSIZE __LIBGCC_TF_MANT_DIG__
// # define FTYPE TFtype
// #else
// # error
// #endif

// #define REP_BIT ((UDWtype) 1 << (DI_SIZE - FSIZE))

//   /* Protect against double-rounding error.
//      Represent any low-order bits, that might be truncated by a bit that
//      won't be lost.  The bit can go in anywhere below the rounding position
//      of the FSTYPE.  A fixed mask and bit position handles all usual
//      configurations.  */
//   if(u >= ((UDWtype)1 << FSIZE))
//   {
//     if((UDWtype)u & (REP_BIT - 1))
//     {
//       u &= ~(REP_BIT - 1);
//       u |= REP_BIT;
//     }
//   }

// /* Do the calculation in a wider type so that we don't lose any of
//    the precision of the high word while multiplying it.  */
//   FTYPE f = (UWtype)(u >> W_TYPE_SIZE);
//   f *= Wtype_MAXp1_F;
//   f += (UWtype)u;
//   return (FSTYPE)f;
// #else
// #if FSSIZE == W_TYPE_SIZE - 1
// # error
// #endif
//   /* Finally, the word size is larger than the number of bits in the
//      required FSTYPE, and we've got no suitable wider type.  The only
//      way to avoid double rounding is to special case the
//      extraction.  */

//   /* If there are no high bits set, fall back to one conversion.  */
//   if((UWtype)u == u)
//     return (FSTYPE)(UWtype)u;

//   /* Otherwise, find the power of two.  */
//   UWtype hi = u >> W_TYPE_SIZE;

//   UWtype count, shift;
//   count_leading_zeros(count, hi);

//   shift = W_TYPE_SIZE - count;

//   /* Shift down the most significant bits.  */
//   hi = u >> shift;

//   /* If we lost any nonzero bits, set the lsb to ensure correct rounding.  */
//   if((UWtype)u << (W_TYPE_SIZE - shift))
//     hi |= 1;

//   /* Convert the one word of data, and rescale.  */
//   FSTYPE f = hi, e;
//   if(shift == W_TYPE_SIZE)
//     e = Wtype_MAXp1_F;
//   /* The following two cases could be merged if we knew that the target
//      supported a native unsigned->float conversion.  More often, we only
//      have a signed conversion, and have to add extra fixup code.  */
//   else if(shift == W_TYPE_SIZE - 1)
//     e = Wtype_MAXp1_F / 2;
//   else
//     e = (Wtype)1 << shift;
//   return f * e;
// #endif
// }

// UWtype
// __fixunsdfSI(DFtype a)
// {
//     if(a >= -(DFtype)Wtype_MIN)
//         return (Wtype)(a + Wtype_MIN) - Wtype_MIN;
//     return (Wtype)a;
// }
// UWtype
// __fixunssfSI(SFtype a)
// {
//   if(a >= -(SFtype)Wtype_MIN)
//     return (Wtype)(a + Wtype_MIN) - Wtype_MIN;
//   return (Wtype)a;
// }
// # if defined(L_powisf2)
// #  define TYPE SFtype
// #  define NAME __powisf2
// # elif defined(L_powidf2)
// #  define TYPE DFtype
// #  define NAME __powidf2
// # elif defined(L_powixf2)
// #  define TYPE XFtype
// #  define NAME __powixf2
// # elif defined(L_powitf2)
// #  define TYPE TFtype
// #  define NAME __powitf2
// # endif

// #undef int
// #undef unsigned
// TYPE
// NAME(TYPE x, int m)
// {
//   unsigned int n = m < 0 ? -(unsigned int)m : (unsigned int)m;
//   TYPE y = n % 2 ? x : 1;
//   while(n >>= 1)
//   {
//     x = x * x;
//     if(n % 2)
//       y = y * x;
//   }
//   return m < 0 ? 1 / y : y;
// }

// #if defined(L_mulhc3) || defined(L_divhc3)
// # define MTYPE	HFtype
// # define CTYPE	HCtype
// # define AMTYPE SFtype
// # define MODE	hc
// # define CEXT	__LIBGCC_HF_FUNC_EXT__
// # define NOTRUNC (!__LIBGCC_HF_EXCESS_PRECISION__)
// #elif defined(L_mulsc3) || defined(L_divsc3)
// # define MTYPE	SFtype
// # define CTYPE	SCtype
// # define AMTYPE DFtype
// # define MODE	sc
// # define CEXT	__LIBGCC_SF_FUNC_EXT__
// # define NOTRUNC (!__LIBGCC_SF_EXCESS_PRECISION__)
// # define RBIG	(__LIBGCC_SF_MAX__ / 2)
// # define RMIN	(__LIBGCC_SF_MIN__)
// # define RMIN2	(__LIBGCC_SF_EPSILON__)
// # define RMINSCAL (1 / __LIBGCC_SF_EPSILON__)
// # define RMAX2	(RBIG * RMIN2)
// #elif defined(L_muldc3) || defined(L_divdc3)
// # define MTYPE	DFtype
// # define CTYPE	DCtype
// # define MODE	dc
// # define CEXT	__LIBGCC_DF_FUNC_EXT__
// # define NOTRUNC (!__LIBGCC_DF_EXCESS_PRECISION__)
// # define RBIG	(__LIBGCC_DF_MAX__ / 2)
// # define RMIN	(__LIBGCC_DF_MIN__)
// # define RMIN2	(__LIBGCC_DF_EPSILON__)
// # define RMINSCAL (1 / __LIBGCC_DF_EPSILON__)
// # define RMAX2  (RBIG * RMIN2)
// #elif defined(L_mulxc3) || defined(L_divxc3)
// # define MTYPE	XFtype
// # define CTYPE	XCtype
// # define MODE	xc
// # define CEXT	__LIBGCC_XF_FUNC_EXT__
// # define NOTRUNC (!__LIBGCC_XF_EXCESS_PRECISION__)
// # define RBIG	(__LIBGCC_XF_MAX__ / 2)
// # define RMIN	(__LIBGCC_XF_MIN__)
// # define RMIN2	(__LIBGCC_XF_EPSILON__)
// # define RMINSCAL (1 / __LIBGCC_XF_EPSILON__)
// # define RMAX2	(RBIG * RMIN2)
// #elif defined(L_multc3) || defined(L_divtc3)
// # define MTYPE	TFtype
// # define CTYPE	TCtype
// # define MODE	tc
// # define CEXT	__LIBGCC_TF_FUNC_EXT__
// # define NOTRUNC (!__LIBGCC_TF_EXCESS_PRECISION__)
// # if __LIBGCC_TF_MANT_DIG__ == 106
// #  define RBIG	(__LIBGCC_DF_MAX__ / 2)
// #  define RMIN	(__LIBGCC_DF_MIN__)
// #  define RMIN2  (__LIBGCC_DF_EPSILON__)
// #  define RMINSCAL (1 / __LIBGCC_DF_EPSILON__)
// # else
// #  define RBIG	(__LIBGCC_TF_MAX__ / 2)
// #  define RMIN	(__LIBGCC_TF_MIN__)
// #  define RMIN2	(__LIBGCC_TF_EPSILON__)
// #  define RMINSCAL (1 / __LIBGCC_TF_EPSILON__)
// # endif
// # define RMAX2	(RBIG * RMIN2)
// #else
// # error
// #endif

// #define CONCAT3(A,B,C)	_CONCAT3(A,B,C)
// #define _CONCAT3(A,B,C)	A##B##C

// #define CONCAT2(A,B)	_CONCAT2(A,B)
// #define _CONCAT2(A,B)	A##B

// #define isnan(x)	__builtin_isnan (x)
// #define isfinite(x)	__builtin_isfinite (x)
// #define isinf(x)	__builtin_isinf (x)

// #define INFINITY	CONCAT2(__builtin_huge_val, CEXT) ()
// #define I		1i

// /* Helpers to make the following code slightly less gross.  */
// #define COPYSIGN	CONCAT2(__builtin_copysign, CEXT)
// #define FABS		CONCAT2(__builtin_fabs, CEXT)

// /* Verify that MTYPE matches up with CEXT.  */
// extern void *compile_type_assert[sizeof(INFINITY) == sizeof(MTYPE) ? 1 : -1];

// /* Ensure that we've lost any extra precision.  */
// #if NOTRUNC
// # define TRUNC(x)
// #else
// # define TRUNC(x)	__asm__ ("" : "=m"(x) : "m"(x))
// #endif

// #if defined(L_mulhc3) || defined(L_mulsc3) || defined(L_muldc3)      || defined(L_mulxc3) || defined(L_multc3)

// CTYPE
// CONCAT3(__mul, MODE, 3) (MTYPE a, MTYPE b, MTYPE c, MTYPE d)
// {
//   MTYPE ac, bd, ad, bc, x, y;
//   CTYPE res;

//   ac = a * c;
//   bd = b * d;
//   ad = a * d;
//   bc = b * c;

//   TRUNC(ac);
//   TRUNC(bd);
//   TRUNC(ad);
//   TRUNC(bc);

//   x = ac - bd;
//   y = ad + bc;

//   if(isnan(x) && isnan(y))
//   {
//     /* Recover infinities that computed as NaN + iNaN.  */
//     _Bool recalc = 0;
//     if(isinf(a) || isinf(b))
//     {
//       /* z is infinite.  "Box" the infinity and change NaNs in
//          the other factor to 0.  */
//       a = COPYSIGN(isinf(a) ? 1 : 0, a);
//       b = COPYSIGN(isinf(b) ? 1 : 0, b);
//       if(isnan(c)) c = COPYSIGN(0, c);
//       if(isnan(d)) d = COPYSIGN(0, d);
//       recalc = 1;
//     }
//     if(isinf(c) || isinf(d))
//     {
//       /* w is infinite.  "Box" the infinity and change NaNs in
//          the other factor to 0.  */
//       c = COPYSIGN(isinf(c) ? 1 : 0, c);
//       d = COPYSIGN(isinf(d) ? 1 : 0, d);
//       if(isnan(a)) a = COPYSIGN(0, a);
//       if(isnan(b)) b = COPYSIGN(0, b);
//       recalc = 1;
//     }
//     if(!recalc
//    && (isinf(ac) || isinf(bd)
//      || isinf(ad) || isinf(bc)))
//     {
//       /* Recover infinities from overflow by changing NaNs to 0.  */
//       if(isnan(a)) a = COPYSIGN(0, a);
//       if(isnan(b)) b = COPYSIGN(0, b);
//       if(isnan(c)) c = COPYSIGN(0, c);
//       if(isnan(d)) d = COPYSIGN(0, d);
//       recalc = 1;
//     }
//     if(recalc)
//     {
//       x = INFINITY * (a * c - b * d);
//       y = INFINITY * (a * d + b * c);
//     }
//   }

//   __real__ res = x;
//   __imag__ res = y;
//   return res;
// }
// #endif /* complex multiply */

// #if defined(L_divhc3) || defined(L_divsc3) || defined(L_divdc3)      || defined(L_divxc3) || defined(L_divtc3)

// CTYPE
// CONCAT3(__div, MODE, 3) (MTYPE a, MTYPE b, MTYPE c, MTYPE d)
// {
// #if defined(L_divhc3)						   || (defined(L_divsc3) && defined(__LIBGCC_HAVE_HWDBL__) )

//   /* Half precision is handled with float precision.
//      float is handled with double precision when double precision
//      hardware is available.
//      Due to the additional precision, the simple complex divide
//      method (without Smith's method) is sufficient to get accurate
//      answers and runs slightly faster than Smith's method.  */

//   AMTYPE aa, bb, cc, dd;
//   AMTYPE denom;
//   MTYPE x, y;
//   CTYPE res;
//   aa = a;
//   bb = b;
//   cc = c;
//   dd = d;

//   denom = (cc * cc) + (dd * dd);
//   x = ((aa * cc) + (bb * dd)) / denom;
//   y = ((bb * cc) - (aa * dd)) / denom;

// #else
//   MTYPE denom, ratio, x, y;
//   CTYPE res;

//   /* double, extended, long double have significant potential
//      underflow/overflow errors that can be greatly reduced with
//      a limited number of tests and adjustments.  float is handled
//      the same way when no HW double is available.
//   */

//   /* Scale by max(c,d) to reduce chances of denominator overflowing.  */
//   if(FABS(c) < FABS(d))
//   {
//     /* Prevent underflow when denominator is near max representable.  */
//     if(FABS(d) >= RBIG)
//     {
//       a = a / 2;
//       b = b / 2;
//       c = c / 2;
//       d = d / 2;
//     }
//         /* Avoid overflow/underflow issues when c and d are small.
//      Scaling up helps avoid some underflows.
//      No new overflow possible since c&d < RMIN2.  */
//     if(FABS(d) < RMIN2)
//     {
//       a = a * RMINSCAL;
//       b = b * RMINSCAL;
//       c = c * RMINSCAL;
//       d = d * RMINSCAL;
//     }
//     else
//     {
//       if(((FABS(a) < RMIN) && (FABS(b) < RMAX2) && (FABS(d) < RMAX2))
//           || ((FABS(b) < RMIN) && (FABS(a) < RMAX2)
//             && (FABS(d) < RMAX2)))
//       {
//         a = a * RMINSCAL;
//         b = b * RMINSCAL;
//         c = c * RMINSCAL;
//         d = d * RMINSCAL;
//       }
//     }
//     ratio = c / d;
//     denom = (c * ratio) + d;
//     /* Choose alternate order of computation if ratio is subnormal.  */
//     if(FABS(ratio) > RMIN)
//     {
//       x = ((a * ratio) + b) / denom;
//       y = ((b * ratio) - a) / denom;
//     }
//     else
//     {
//       x = ((c * (a / d)) + b) / denom;
//       y = ((c * (b / d)) - a) / denom;
//     }
//   }
//   else
//   {
//     /* Prevent underflow when denominator is near max representable.  */
//     if(FABS(c) >= RBIG)
//     {
//       a = a / 2;
//       b = b / 2;
//       c = c / 2;
//       d = d / 2;
//     }
//         /* Avoid overflow/underflow issues when both c and d are small.
//      Scaling up helps avoid some underflows.
//      No new overflow possible since both c&d are less than RMIN2.  */
//     if(FABS(c) < RMIN2)
//     {
//       a = a * RMINSCAL;
//       b = b * RMINSCAL;
//       c = c * RMINSCAL;
//       d = d * RMINSCAL;
//     }
//     else
//     {
//       if(((FABS(a) < RMIN) && (FABS(b) < RMAX2) && (FABS(c) < RMAX2))
//           || ((FABS(b) < RMIN) && (FABS(a) < RMAX2)
//             && (FABS(c) < RMAX2)))
//       {
//         a = a * RMINSCAL;
//         b = b * RMINSCAL;
//         c = c * RMINSCAL;
//         d = d * RMINSCAL;
//       }
//     }
//     ratio = d / c;
//     denom = (d * ratio) + c;
//     /* Choose alternate order of computation if ratio is subnormal.  */
//     if(FABS(ratio) > RMIN)
//     {
//       x = ((b * ratio) + a) / denom;
//       y = (b - (a * ratio)) / denom;
//     }
//     else
//     {
//       x = (a + (d * (b / c))) / denom;
//       y = (b - (d * (a / c))) / denom;
//     }
//   }
// #endif

//   /* Recover infinities and zeros that computed as NaN+iNaN; the only
//      cases are nonzero/zero, infinite/finite, and finite/infinite.  */
//   if(isnan(x) && isnan(y))
//   {
//     if(c == 0.0 && d == 0.0 && (!isnan(a) || !isnan(b)))
//     {
//       x = COPYSIGN(INFINITY, c) * a;
//       y = COPYSIGN(INFINITY, c) * b;
//     }
//     else if((isinf(a) || isinf(b)) && isfinite(c) && isfinite(d))
//     {
//       a = COPYSIGN(isinf(a) ? 1 : 0, a);
//       b = COPYSIGN(isinf(b) ? 1 : 0, b);
//       x = INFINITY * (a * c + b * d);
//       y = INFINITY * (b * c - a * d);
//     }
//     else if((isinf(c) || isinf(d)) && isfinite(a) && isfinite(b))
//     {
//       c = COPYSIGN(isinf(c) ? 1 : 0, c);
//       d = COPYSIGN(isinf(d) ? 1 : 0, d);
//       x = 0.0 * (a * c + b * d);
//       y = 0.0 * (b * c - a * d);
//     }
//   }

//   __real__ res = x;
//   __imag__ res = y;
//   return res;
// }
// #endif /* complex divide */

/* Like bcmp except the sign is meaningful.
   Result is negative if S1 is less than S2,
   positive if S1 is greater, 0 if S1 and S2 are equal.  */

// int
// __gcc_bcmp(const unsigned char *s1, const unsigned char *s2, size_t size)
// {
//     while(size > 0)
//     {
//         const unsigned char c1 = *s1++, c2 = *s2++;
//         if(c1 != c2)
//             return c1 - c2;
//         size--;
//     }
//     return 0;
// }

// void
// __eprintf(const char *string, const char *expression,
//      unsigned int line, const char *filename)
// {
//   fprintf(stderr, string, expression, line, filename);
//   fflush(stderr);
//   abort();
// }

// void
// __clear_cache(void *beg __attribute__((__unused__)),
//          void *end __attribute__((__unused__)))
// {
// #ifdef CLEAR_INSN_CACHE
//   /* Cast the void* pointers to char* as some implementations
//      of the macro assume the pointers can be subtracted from
//      one another.  */
//   CLEAR_INSN_CACHE((char *)beg, (char *)end);
// #endif /* CLEAR_INSN_CACHE */
// }



UDWtype SECTION(".text.riscv_operation")
umulsidi3(UWtype u, UWtype v)
{

    UDWtype w = 0;
    USItype __x0, __x1, __x2, __x3;
    USItype __ul, __vl, __uh, __vh;
    __ul = ((USItype)(u) & 0xffff);
    __uh = (USItype)(u) >> 16;
    __vl = ((USItype)(v) & 0xffff);
    __vh = (USItype)(v) >> 16;

    __x0 = (USItype)__ul * __vl;//0
    __x1 = (USItype)__ul * __vh;//<<16
    __x2 = (USItype)__uh * __vl;//<<16
    __x3 = (USItype)__uh * __vh;//<<32
    w += (((UDWtype)__x0) << 0);
    w += (((UDWtype)__x1) << 16);
    w += (((UDWtype)__x2) << 16);
    w += (((UDWtype)__x3) << 32);
    return w;
}



#endif
