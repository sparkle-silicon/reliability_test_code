  /*
  * @Author: Linyu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-10-20 19:26:02
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
  /******************************************h file********************************************/
#include "AE_PRINTF.H"
#ifdef USER_AE10X_LIBC_A

const BYTE _ctype[] = {
HEX08, HEX08, HEX08, HEX08, HEX08, HEX08, HEX08, HEX08,                                                                 /* 0-7 */
HEX08, HEX08 | HEX20, HEX08 | HEX20, HEX08 | HEX20, HEX08 | HEX20, HEX08 | HEX20, HEX08, HEX08,                         /* 8-15 */
HEX08, HEX08, HEX08, HEX08, HEX08, HEX08, HEX08, HEX08,                                                                 /* 16-23 */
HEX08, HEX08, HEX08, HEX08, HEX08, HEX08, HEX08, HEX08,                                                                 /* 24-31 */
HEX20 | HEX80, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10,                                                         /* 32-39 */
HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10,                                                                 /* 40-47 */
HEX04, HEX04, HEX04, HEX04, HEX04, HEX04, HEX04, HEX04,                                                                 /* 48-55 */
HEX04, HEX04, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10,                                                                 /* 56-63 */
HEX10, HEX01 | HEX40, HEX01 | HEX40, HEX01 | HEX40, HEX01 | HEX40, HEX01 | HEX40, HEX01 | HEX40, HEX01,                 /* 64-71 */
HEX01, HEX01, HEX01, HEX01, HEX01, HEX01, HEX01, HEX01,                                                                 /* 72-79 */
HEX01, HEX01, HEX01, HEX01, HEX01, HEX01, HEX01, HEX01,                                                                 /* 80-87 */
HEX01, HEX01, HEX01, HEX10, HEX10, HEX10, HEX10, HEX10,                                                                 /* 88-95 */
HEX10, HEX02 | HEX40, HEX02 | HEX40, HEX02 | HEX40, HEX02 | HEX40, HEX02 | HEX40, HEX02 | HEX40, HEX02,                 /* 96-103 */
HEX02, HEX02, HEX02, HEX02, HEX02, HEX02, HEX02, HEX02,                                                                 /* 104-111 */
HEX02, HEX02, HEX02, HEX02, HEX02, HEX02, HEX02, HEX02,                                                                 /* 112-119 */
HEX02, HEX02, HEX02, HEX10, HEX10, HEX10, HEX10, HEX08,                                                                 /* 120-127 */
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                                                                         /* 128-143 */
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                                                                         /* 144-159 */
HEX20 | HEX80, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, /* 160-175 */
HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10, HEX10,         /* 176-191 */
HEX01, HEX01, HEX01, HEX01, HEX01, HEX01, HEX01, HEX01, HEX01, HEX01, HEX01, HEX01, HEX01, HEX01, HEX01, HEX01,         /* 192-207 */
HEX01, HEX01, HEX01, HEX01, HEX01, HEX01, HEX01, HEX10, HEX01, HEX01, HEX01, HEX01, HEX01, HEX01, HEX01, HEX02,         /* 208-223 */
HEX02, HEX02, HEX02, HEX02, HEX02, HEX02, HEX02, HEX02, HEX02, HEX02, HEX02, HEX02, HEX02, HEX02, HEX02, HEX02,         /* 224-239 */
HEX02, HEX02, HEX02, HEX02, HEX02, HEX02, HEX02, HEX10, HEX02, HEX02, HEX02, HEX02, HEX02, HEX02, HEX02, HEX02 };        /* 240-255 */
/******************************************prirntf.c*************************************************************/
VBYTE print_buff[PRINT_MAX_SIZE] = { 0 };
char *strstr(const char *s1, const char *s2)
{
  char *str1 = (char *)s1;
  if (!*s2)
    return str1;
  char *str2 = (char *)s2;
  while (*str1)
  {
    str2 = (char *)s2;
    while (*str1 && *str1 != *str2)
      str1++;
    while (*str2 && !(*str1 - *str2))
      str1++, str2++;
    if (!*str2)
      return (str1);
    str1++;
  }
  return (NULL);
}
int strcmp(const char *str1, const char *str2)
{
  DWORD i = 0;
  while (*(str1 + i) == *(str2 + i))
  {
    if (*(str1 + i) == '\0')
      return 0;
    i++;
  }
  return ++i;
}
size_t strlen(const char *str)
{
  DWORD i = 0;
  while (*(str + i))
  {
    i++;
  }
  return i;
}
char *strcat(char *dest, const char *src)
{
  while (*dest != '\0')
    dest++;
  while (*src != '\0')
  {
    *dest++ = *src++;
  }
  return dest;
}
char *strchr(const char *str, int c)
{
  while (*str != c && *str != '\0')
  {
    str++;
  }
  if (*str == '\0')
    return NULL;
  return (char *)str;
}
int puts(const char *str) /*字符串输出*/
{
  const char *start = str;
  while (*str)
    putchar(*str++);
  return (int)(str - start);
}
int _isatty(int fd)
{
  if (fd == STDOUT_FILENO || fd == STDERR_FILENO)
    return 1;
  return 0;
}
ssize_t _write(int fd, const void *ptr, size_t len)
{
  const uint8_t *current = (const uint8_t *)ptr;
  if (_isatty(fd))
  {
    for (size_t jj = 0; jj < len; jj++)
    {
      putchar(current[jj]);
    }
    return len;
  }
  return -1;
}
static int skip_atoi(const char **s) // 字符串转数字
{
  int i = 0;
  while (is_digit(**s))
    i = i * 10 + *((*s)++) - '0';
  return i;
}
static size_t strnlens(const char *s, size_t count)
{
  const char *sc = s;
  while (*sc && count)
  {
    sc++;
    count--;
  }
  return sc - s;
}
static void string(const char *s, s32 field_width,
s32 precision, int flags)
{
  int len, i;
  if (s == NULL)
    s = "<NULL>";
  len = strnlens(s, precision);

  if (!(flags & LEFT))
    while (len < field_width--)
      putchar(' ');
  for (i = 0; i < len; s++, ++i)
  {
    putchar(*s);
  }
  while (len < field_width--)
    putchar(' ');
}
static NOINLINE void float_point(union _FP_UNION_D *dfp, s32 field_width, s32 precision, int flags)
{
  char sign;
  char c;
  char buff[64];
  u_int64_t frac, frac_max;
  u_int16_t exp, exp_max, frac_bits;
  union _FP_UNION_S sfp = { .num = 0 };
  short i = 0;
  if (flags & LEFT)//如果靠左则前面没0
    flags &= (~ZEROPAD);
  c = (flags & ZEROPAD) ? '0' : ' ';//填充
  if (flags & DOUBLE_FLOAT)
  {
    sign = dfp->bits.sign;
    exp = dfp->bits.exp;
    frac = (((u_int64_t)dfp->bits.frac0) | (((u_int64_t)dfp->bits.frac1) << 32));
    exp_max = _FP_EXPMAX_D;
    frac_max = (((u_int64_t)1 << _FP_FRACBITS_D) - 1);
    frac_bits = _FP_FRACBITS_D;

  }
  else if (flags & SIGLE_FLOAT)
  {
    sfp.flt = (float)dfp->flt;
    sign = sfp.bits.sign;
    exp = sfp.bits.exp;
    frac = (u_int64_t)sfp.bits.frac;
    exp_max = _FP_EXPMAX_S;
    frac_max = ((1 << _FP_FRACBITS_S) - 1);
    frac_bits = _FP_FRACBITS_S;

  }
  int bits = (frac_bits & 1) + (frac_bits >> 2) + 1;

  if (flags & AUTO_FMT_FLOAT)//%g
  {
    if ((dfp->bits.exp >= 6 && dfp->bits.exp < (0x7ff - 5)) || (frac & (frac_max >> (frac_bits >> 1))))
      flags |= EXP_FLOAT;
  }
  if (sign)//必有符号
  {
    sign = '-';
    dfp->flt = -dfp->flt;
    sfp.flt = -sfp.flt;
    field_width--;
  }
  else if (flags & PLUS)
  {
    sign = '+';
    field_width--;
  }
  else if (flags & SPACE)
  {
    sign = ' ';
    field_width--;
  }
  i = 0;
  if (exp == 0)//先排除特殊情况
  {
    if (frac == 0)//number = 0.0
    {
      if (!(flags & PLUS))
        sign = 0;//因为是0了所以不输出符号
      buff[i++] = '0';
    }
    else// 非规格化数的情况Denormalized
    {
      if (flags & EXP_FLOAT)
      {
        buff[i++] = '0';
        buff[i++] = '+';
        buff[i++] = ('E' | SMALL);//因为不正常，所以仅输出胡
      }
      int cnt = 0;
      int mul = 10;
      while (cnt < bits)
      {
        mul *= 10;
        cnt++;
      }
      frac = (frac * 1024) / (((frac_max + 1) * 1024) / (mul));
      if ((frac % 10) >= 5)frac += 10;
      frac /= 10;
      if (frac)
      {
        while ((!(frac % 10)) && bits)
        {
          frac /= 10;
          bits--;
        }
        if (bits)
          while (bits)
          {
            buff[i++] = (frac % 10) + '0';
            frac = (frac / 10);
            if (precision > 0)precision--;
            bits--;
          }
        else
        {
          buff[i++] = '0';
          if (precision > 0)precision--;

        }
      }
      else buff[i++] = '0';
      buff[i++] = '.';
      buff[i++] = '0';
    }
  }
  else if (exp == exp_max)//无穷大或者NaN
  {
    if (frac == 0)//無窮大
    {
      if (sign)putchar(sign);
      //else putchar('+');
    #if 1
      puts("Inf");//±Infinity
    #else
      puts("Infinity");
    #endif
    }
    else
    {
      puts("NaN");//Not a Number
    }
    return;
  }
  else if (flags & EXP_FLOAT)//科学计数法%e/%a
  {
    char exponent_sign = '+';
    int32_t exponent = 0;//10进制指数
    u_int64_t fraction = 0;//小数部分
    u_int64_t integer = 0;//整数部分
    if (flags & DOUBLE_FLOAT)//双精度
    {
      double flt = dfp->flt;
      if (flt < 0)flt = -flt;
      while (flt >= 10.0)
      {
        flt /= 10.0;
        exponent++;
      }
      while (flt < 1.0)
      {
        flt *= 10.0;
        exponent--;
      }
      integer = flt;
      fraction = (u_int64_t)((flt - (double)integer) * (double)10000000000000000.0);
      if (fraction % 10 >= 5)fraction += 10;
      fraction /= 10;

    }
    else if (flags & SIGLE_FLOAT)//单精度
    {
      float flt = sfp.flt;
      if (flt < 0)flt = -flt;
      while (flt >= 10.0)
      {
        flt /= 10.0;
        exponent++;
      }
      while (flt < 1.0)
      {
        flt *= 10.0;
        exponent--;
      }
      integer = (u_int64_t)flt;
      fraction = (u_int64_t)((flt - (float)integer) * (float)100000000.0);
    }
          //指数位
    if (exponent < 0)
    {
      exponent_sign = '-';
      exponent = -exponent;
    }
    if (exponent)
    {
      while (exponent)
      {
        buff[i++] = (exponent % (int32_t)10) + '0';
        exponent = (exponent / (int32_t)10);
      }
    }
    else
    {
      buff[i++] = '0';
    }
     //符号位
    buff[i++] = exponent_sign;
    buff[i++] = ('E' | SMALL);//十六进制
    // //小数位
    if (fraction % 10 >= 5)fraction += 10;
    fraction /= 10;
    while (!(fraction % 10) && bits)//排除最低位为0的情况
    {
      fraction /= 10;
      bits--;
    }
    if (bits)
      while (bits)
      {
        buff[i++] = (fraction % 10) + '0';
        fraction = (fraction / 10);
        if (precision > 0)precision--;
        bits--;
      }
    else
    {
      buff[i++] = '0';
      if (precision > 0)precision--;

    }
    //整数位
    buff[i++] = '.';
    buff[i++] = (integer % 10) + '0';
  }
  else//小数表示%f
  {
    u_int64_t fraction = 0;//小数部分
    u_int64_t integer = 0;//整数部分
    if (flags & DOUBLE_FLOAT)//双精度
    {
      double flt = dfp->flt;
      if (flt < 0)flt = -flt;
      integer = (u_int64_t)flt;
      fraction = (u_int64_t)((flt - (double)integer) * (double)10000000000000000.0);

    }
    else if (flags & SIGLE_FLOAT)//单精度
    {
      float flt = sfp.flt;
      if (flt < 0)flt = -flt;
      integer = (u_int64_t)flt;
      fraction = (u_int64_t)((flt - (double)integer) * (double)100000000.0);

    }
        // //小数位
    if (fraction % 10 >= 5)fraction += 10;
    fraction /= 10;
    while (!(fraction % 10) && bits)//排除最低位为0的情况
    {
      fraction /= 10;
      bits--;
    }
    if (bits)
      while (bits)
      {
        buff[i++] = (fraction % 10) + '0';
        fraction = (fraction / 10);
        if (precision > 0)precision--;
        bits--;
      }
    else
    {
      buff[i++] = '0';
      if (precision > 0)precision--;

    }
    buff[i++] = '.';
  //整数位
    if (integer)
    {
      while (integer)
      {
        buff[i++] = (integer % 10) + '0';
        integer = (integer / 10);
      }
    }
    else
    {
      buff[i++] = '0';
    }
  }
  if (i > precision)
    precision = i;
  field_width -= precision;
  if (!(flags & (ZEROPAD | LEFT)))
    while (field_width-- > 0)
      putchar(' ');
  if (sign)
    putchar(sign);
  if (!(flags & LEFT))
    while (field_width-- > 0)
      putchar(c);
  while (i < precision--)
    putchar('0');
  while (i-- > 0)
    putchar(buff[i]);
  while (field_width-- > 0)
    putchar(' ');

}
static void number(int64_t num, int base, int size, s32 precision, int type)
{
/* we are called with base 8, 10 or 16, only, thus don't need "G..."  */
  static const char digits[] = "0123456789ABCDEF"; // 转换字符
  char sign;
  char c;
  char locase;
  char buff[64];
  short i = 0;
  if (base < 2 || base > 16)//防止异常
    return;
  /* locase = 0 or 0x20. ORing digits or letters with 'locase'
  * produces same digits or (maybe lowercased) letters */
  locase = (type & SMALL);//小写

  if (type & LEFT)//如果靠左则前面没0
    type &= (~ZEROPAD);
  c = (type & ZEROPAD) ? '0' : ' ';//填充

  sign = 0;//查看符号
  if (type & SIGN)
  {
    if (num < 0)
    {
      sign = '-';
      num = -num;
      size--;
    }
    else if (type & PLUS)
    {
      sign = '+';
      size--;
    }
    else if (type & SPACE)
    {
      sign = ' ';
      size--;
    }
  }
  if (type & SPECIAL)
  {
    // if((base == 16) || (base == 2))
    //   size -= 2;
    // else
    if (base == 8)
      size--;
  }
  // u64 number = (u64)num;
  i = 0;
  if (num == 0)
    buff[i++] = '0';
  else
  {
    while (num)
    {
      buff[i++] = (digits[(((u_int64_t)num) % (u_int64_t)base)] | locase);
      num = (((u_int64_t)num) / (u_int64_t)base);
    }
  }
  if (i > precision)
    precision = i;
  size -= precision;
  if (!(type & (ZEROPAD | LEFT)))
    while (size-- > 0)
      putchar(' ');
  if (sign)
    putchar(sign);
  if (type & SPECIAL)
  {
    if (base == 8)
      putchar('0');
    else if (base == 2)
    {
      putchar('0');
      putchar('B' | locase);
    }

    else if (base == 16)
    {
      putchar('0');
      putchar('X' | locase);
    }
  }
  if (!(type & LEFT))
    while (size-- > 0)
      putchar(c);
  while (i < precision--)
    putchar('0');
  while (i-- > 0)
    putchar(buff[i]);
  while (size-- > 0)
    putchar(' ');

}
static void pointer(void *ptr,
s32 field_width, s32 precision, int flags)
{
  unsigned long   num = (unsigned long)ptr;
  if (field_width == -1)
  {
    field_width = 2 * sizeof(void *);
    flags |= ZEROPAD;
  }
  number(num, 16, field_width, precision, flags);
}
static int vprintf_internal(const char *fmt, va_list args)
{
  unsigned long long num; // 数值
  int base;      // 进制
  const char *str;     // 指针
  int flags;     // 标志 flags to number()
  s32 field_width; // 长度 width of output field
  s32 precision;   // 精度 min. # of digits for integers; max number of chars for from string
  s32 qualifier;   // 类型 'h', 'l', or 'L' for integer fields
  union _FP_UNION_D dfp;//浮点数

  for (str = (const char *)fmt; *str; ++str)
  {
    if (*str != '%')
    {
      putchar(*str);
      continue;
    }
    /* process flags */
    flags = 0;
  repeat:
    ++str;/* this also skips first '%' */
    switch (*str)
    {
      case '-': flags |= LEFT;     goto repeat;
      case '+': flags |= PLUS;     goto repeat;
      case ' ': flags |= SPACE;    goto repeat;
      case '#': flags |= SPECIAL;  goto repeat;
      case '0': flags |= ZEROPAD;  goto repeat;
    }
    /* get field width */
    field_width = -1;
    if (is_digit(*str))
      field_width = skip_atoi((const char **)&str);
    else if (*str == '*')
    {
      ++str;
      /* it's the next argument */
      field_width = va_arg(args, int);
      if (field_width < 0)
      {
        field_width = -field_width;
        flags |= LEFT;
      }
    }
    /* get the precision */
    precision = -1;
    if (*str == '.')
    {
      ++str;
      if (is_digit(*str))
        precision = skip_atoi((const char **)&str);
      else if (*str == '*')
      {
        ++str;
        /* it's the next argument */
        precision = va_arg(args, int);
      }
      if (precision < 0)
        precision = 0;
    }
    /* get the conversion qualifier */
    qualifier = -1;
    if (*str == 'h' || *str == 'H' || *str == 'l' || *str == 'L')
    {
      qualifier = *str;
      ++str;
      if (qualifier == 'l' && *str == 'l')
      {
        qualifier = 'L';
        ++str;
      }
      else if (qualifier == 'h' && *str == 'h')
      {
        qualifier = 'H';
        ++str;
      }
    } /* nothing */;
    /* default base */
    base = 10;
    switch (*str)
    {
      case 'c':
        if (!(flags & LEFT))
        {
          while (--field_width > 0)
            putchar(' ');
        }
        putchar((BYTE)va_arg(args, int));
        while (--field_width > 0)
          putchar(' ');
        continue;
      case 's':
        string(va_arg(args, char *), field_width, precision, flags);
        continue;
      case 'p':
        pointer(va_arg(args, void *), field_width, precision, flags);
        while (isalnum(str[1]))
          str++;
        continue;
        //
      case 'n':
        if (qualifier == 'l')
        {
          long *ip = va_arg(args, long *);
          *ip = (str - fmt);
        }
        else
        {
          int *ip = va_arg(args, int *);
          *ip = (str - fmt);
        }
        continue;
      case '%':
        putchar('%');
        continue;
        /* integer number formats - set up the flags and "break" */
      case 'b':
        flags |= SMALL;
        // fall through
      case 'B':
        base = 2;
        break;
      case 'o':
        base = 8;
        break;
      case 'x':
        flags |= SMALL;
        // fall through
      case 'X':
        base = 16;
        break;
      case 'f':
      case 'e'://scientific notation
      case 'g'://auto format specifier
        flags |= SMALL;
      case 'F':
      case 'E':
      case 'G':
      #if 0
        if (qualifier == 'l')flags |= DOUBLE_FLOAT;
        else if (qualifier == 'L')flags |= QUAD_FLOAT;
        else if (qualifier == 'h')flags |= HALF_FLOAT;
      #else 
        if (qualifier == 'l' || qualifier == 'L') flags |= DOUBLE_FLOAT;
      #endif
        else flags |= SIGLE_FLOAT;
        if (*str == 'e' || *str == 'E')flags |= EXP_FLOAT;
        else if (*str == 'g' || *str == 'G')flags |= AUTO_FMT_FLOAT;
        dfp.flt = va_arg(args, double);//默认都是double位
        float_point(&dfp, field_width, precision, flags);
        continue;
      case 'd':
      case 'i':
        flags |= SIGN;
        // fall through
      case 'u':
        break;
        /* nothing */;
      default:
        putchar('%');
        if (*str)
          putchar(*str);
        else
          --str;
        continue;
    }
    if (qualifier == 'L') /* "quad" for 64 bit variables */
    {
      num = va_arg(args, unsigned long long);
      if (flags & SIGN)
        num = (signed long long)num;
    }
    else if (qualifier == 'l')
    {
      num = va_arg(args, unsigned long);
      if (flags & SIGN)
        num = (signed long long)((signed long)num);
    }
    else
    {
      num = va_arg(args, unsigned int);
      if (flags & SIGN)
      {
        if (qualifier == 'H')
          num = (signed long long)((s8)num);
        else if (qualifier == 'h')
          num = (signed long long)((s16)num);
        else  num = (signed long long)((s32)num);
      }
      else
      {
        if (qualifier == 'H')
          num = (u8)(num & 0xff);
        else if (qualifier == 'h')
          num = (u16)(num & 0xffff);
      }
    }
    number(num, base, field_width, precision, flags);
  }
  /* the trailing null byte doesn't count towards the total */
  return str - fmt;

}
  //printf% 
  //%
  //+(大于0有加号，小于0没有加号),-(靠左), (添加空格) #(添加前缀)，0（添加0）
  //数值（填充宽度）*（由参数决定，eg："%*d"，len,data)
  //h/l/L short,long , longlong 
  //c (char),s (string) p (pointer),n(*addr=字符相对位置)，%（%），b/B（二进制），o，x/X，d/i。u
  //
int printf(const char *fmt, ...)
{
  int printed;
  va_list args;        // 创建形参表
  va_start(args, fmt); // 初值
  printed = vprintf_internal(fmt, args);
  va_end(args); // 结束
  return printed;
}
#if SUPPORT_REAL_OR_DELAY_PRINTF
int printr(const char *fmt, ...)//实时输出
{
  int i;
  BYTE cache = F_Service_PUTC;
  F_Service_PUTC = 1; // 打开实时输出
  va_list args;        // 创建形参表
  va_start(args, fmt); // 初值
  i = vprintf_internal(fmt, args);
  F_Service_PUTC = cache; // 返回原值
  va_end(args); // 结束
  return i;
}
int printd(const char *fmt, ...)//延时输出
{
  int i;
  BYTE cache = F_Service_PUTC;
  F_Service_PUTC = 0; // 打开延时输出
  va_list args;        // 创建形参表
  va_start(args, fmt); // 初值
  i = vprintf_internal(fmt, args);
  F_Service_PUTC = cache; // 返回原值
  va_end(args); // 结束
  return i;
}
#endif
#elif (defined(USER_RISCV_LIBC_A))
USED int _isatty(int fd)
{
  if (fd == STDOUT_FILENO || fd == STDERR_FILENO)
    return 1;
  return 0;
}
int strcoll(const char *str1, const char *str2) { return strcmp(str1, str2); }
char *strcpy(char *__restrict dst0, const char *__restrict src0)
{
  return (char *)memcpy(dst0, src0, strlen(src0));
}
size_t strcspn(const char *s1, const char *s2)
{
  const char *s = s1;
  const char *c;
  while (*s1)
  {
    for (c = s2; *c; c++)
    {
      if (*s1 == *c)
        break;
    }
    if (*c)
      break;
    s1++;
  }
  return s1 - s;
}
size_t strlen(const char *str)
{
  DWORD i = 0;
  while (*(str + i))
  {
    i++;
  }
  return i;
}
char *strpbrk(const char *s1, const char *s2)
{
  const char *c = s2;
  if (!*s1)
    return (char *)NULL;
  while (*s1)
  {
    for (c = s2; *c; c++)
    {
      if (*s1 == *c)
        break;
    }
    if (*c)
      break;
    s1++;
  }
  if (*c == '\0')
    s1 = NULL;
  return (char *)s1;
}
char *strrchr(const char *s, int i)
{
  const char *last = NULL;
  if (i)
  {
    while ((s = strchr(s, i)))
    {
      last = s;
      s++;
    }
  }
  else
  {
    last = strchr(s, i);
  }
  return (char *)last;
}
size_t strspn(const char *s1, const char *s2)
{
  const char *s = s1;
  const char *c;
  while (*s1)
  {
    for (c = s2; *c; c++)
    {
      if (*s1 == *c)
        break;
    }
    if (*c == '\0')
      break;
    s1++;
  }
  return s1 - s;
}
#endif