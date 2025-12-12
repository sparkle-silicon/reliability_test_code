/*
 * @Author: dejavuwdh
 * @LastEditors: daweslinyu
 * @LastEditTime: 2024-04-23 16:26:53
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
#include "AE_SOFT_FP.H"

#if SOFT_SFP_DFP_CODE_SELECT





//浮点型转整形fix
#define __fixsfdi SECTION(".riscv_ccode_fp") fixsfdi
#define __fixunssfdi SECTION(".riscv_ccode_fp") fixunssfdi
#define __fixdfdi SECTION(".riscv_ccode_fp") fixdfdi
#define __fixunsdfdi SECTION(".riscv_ccode_fp") fixunsdfdi
#define __fixsfsi SECTION(".riscv_ccode_fp") fixsfsi
#define __fixunssfsi SECTION(".riscv_ccode_fp") fixunssfsi
#define __fixdfsi SECTION(".riscv_ccode_fp") fixdfsi
#define __fixunsdfsi SECTION(".riscv_ccode_fp") fixunsdfsi

DItype  __fixsfdi(SFtype a)//单精度转长整形
{
  FP_DECL_EX;
  FP_DECL_S(A);
  UDItype r;
  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_S(A, a);
  FP_TO_INT_S(r, A, DI_BITS, 1);
  FP_HANDLE_EXCEPTIONS;
  return r;
}

UDItype  __fixunssfdi(SFtype a)//单精度转无符号长整形
{
  FP_DECL_EX;
  FP_DECL_S(A);
  UDItype r;
  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_S(A, a);
  FP_TO_INT_S(r, A, DI_BITS, 0);
  FP_HANDLE_EXCEPTIONS;
  return r;
}

DItype  __fixdfdi(DFtype a)//双精度转长整形
{
  FP_DECL_EX;
  FP_DECL_D(A);
  UDItype r;
  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_D(A, a);
  FP_TO_INT_D(r, A, DI_BITS, 1);
  FP_HANDLE_EXCEPTIONS;
  return r;
}

UDItype __fixunsdfdi(DFtype a)//双精度转无符号长整形
{
  FP_DECL_EX;
  FP_DECL_D(A);
  UDItype r;
  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_D(A, a);
  FP_TO_INT_D(r, A, DI_BITS, 0);
  FP_HANDLE_EXCEPTIONS;
  return r;
}

SItype __fixsfsi(SFtype a)//单精度浮点数转换为整数
{
  FP_DECL_EX;//错误标志变量和舍入模式变量
  FP_DECL_S(A);//拆分符号，指数，尾数
  USItype r;//结果
  FP_INIT_EXCEPTIONS;//初始化变量
  FP_UNPACK_RAW_S(A, a);//将浮点数a通过共用体拆解
  FP_TO_INT_S(r, A, SI_BITS, 1);//进行转换
  FP_HANDLE_EXCEPTIONS;
  return r;
}

USItype __fixunssfsi(SFtype a)//单精度浮点数转换为无符号整数
{
  FP_DECL_EX;
  FP_DECL_S(A);
  USItype r;
  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_S(A, a);
  FP_TO_INT_S(r, A, SI_BITS, 0);
  FP_HANDLE_EXCEPTIONS;
  return r;
}

SItype __fixdfsi(DFtype a) //双精度转整形
{
  FP_DECL_EX;
  FP_DECL_D(A);
  USItype r;
  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_D(A, a);
  FP_TO_INT_D(r, A, SI_BITS, 1);
  FP_HANDLE_EXCEPTIONS;
  return r;
}

USItype __fixunsdfsi(DFtype a)//双精度转无符号整形
{
  FP_DECL_EX;
  FP_DECL_D(A);
  USItype r;
  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_D(A, a);
  FP_TO_INT_D(r, A, SI_BITS, 0);
  FP_HANDLE_EXCEPTIONS;
  return r;
}

//整形转浮点型float
#define __floatdidf SECTION(".riscv_ccode_fp") floatdidf
#define __floatdisf SECTION(".riscv_ccode_fp") floatdisf
#define __floatundidf SECTION(".riscv_ccode_fp") floatundidf
#define __floatundisf SECTION(".riscv_ccode_fp") floatundisf
#define __floatsisf SECTION(".riscv_ccode_fp") floatsisf
#define __floatunsisf SECTION(".riscv_ccode_fp") floatunsisf
#define __floatsidf SECTION(".riscv_ccode_fp") floatsidf
#define __floatunsidf SECTION(".riscv_ccode_fp") floatunsidf

DFtype __floatdidf(DItype i)//长整形转双精度
{
  FP_DECL_EX;
  FP_DECL_D(A);
  DFtype a;
  FP_INIT_ROUNDMODE;
  FP_FROM_INT_D(A, i, DI_BITS, UDItype);
  FP_PACK_RAW_D(a, A);
  FP_HANDLE_EXCEPTIONS;
  return a;
}

SFtype __floatdisf(DItype i)//长整形转单精度
{
  FP_DECL_EX;
  FP_DECL_S(A);
  SFtype a;
  FP_INIT_ROUNDMODE;
  FP_FROM_INT_S(A, i, DI_BITS, UDItype);
  FP_PACK_RAW_S(a, A);
  FP_HANDLE_EXCEPTIONS;
  return a;
}

DFtype __floatundidf(UDItype i)
{
  FP_DECL_EX;
  FP_DECL_D(A);
  DFtype a;
  FP_INIT_ROUNDMODE;
  FP_FROM_INT_D(A, i, DI_BITS, UDItype);
  FP_PACK_RAW_D(a, A);
  FP_HANDLE_EXCEPTIONS;
  return a;
}

SFtype __floatundisf(UDItype i)
{
  FP_DECL_EX;
  FP_DECL_S(A);
  SFtype a;
  FP_INIT_ROUNDMODE;
  FP_FROM_INT_S(A, i, DI_BITS, UDItype);
  FP_PACK_RAW_S(a, A);
  FP_HANDLE_EXCEPTIONS;
  return a;
}



SFtype __floatsisf(SItype i)//整形转单精度
{
  FP_DECL_EX;
  FP_DECL_S(A);
  SFtype a;
  FP_INIT_ROUNDMODE;
  FP_FROM_INT_S(A, i, SI_BITS, USItype);
  FP_PACK_RAW_S(a, A);
  FP_HANDLE_EXCEPTIONS;
  return a;
}

SFtype __floatunsisf(USItype i)//无符号整形转单精度
{
  FP_DECL_EX;
  FP_DECL_S(A);
  SFtype a;
  FP_INIT_ROUNDMODE;
  FP_FROM_INT_S(A, i, SI_BITS, USItype);
  FP_PACK_RAW_S(a, A);
  FP_HANDLE_EXCEPTIONS;
  return a;
}

DFtype __floatsidf(SItype i)//整形转双精度
{
  FP_DECL_EX;
  FP_DECL_D(A);
  DFtype a;
  FP_FROM_INT_D(A, i, SI_BITS, USItype);
  FP_PACK_RAW_D(a, A);
  return a;
}

DFtype __floatunsidf(USItype i)//无符号整形转双精度
{
  FP_DECL_EX;
  FP_DECL_D(A);
  DFtype a;
  FP_FROM_INT_D(A, i, SI_BITS, USItype);
  FP_PACK_RAW_D(a, A);
  return a;
}

#define __addsf3 SECTION(".riscv_ccode_fp") addsf3
#define __subsf3 SECTION(".riscv_ccode_fp") subsf3
#define __mulsf3 SECTION(".riscv_ccode_fp") mulsf3
#define __divsf3 SECTION(".riscv_ccode_fp") divsf3
//单精度算法
SFtype __addsf3(SFtype a, SFtype b)//单精度加法
{
  FP_DECL_EX;
  FP_DECL_S(A);
  FP_DECL_S(B);
  FP_DECL_S(R);
  SFtype r;
  FP_INIT_ROUNDMODE;
  FP_UNPACK_SEMIRAW_S(A, a);
  FP_UNPACK_SEMIRAW_S(B, b);
  FP_ADD_S(R, A, B);
  FP_PACK_SEMIRAW_S(r, R);
  FP_HANDLE_EXCEPTIONS;
  return r;
}
SFtype __subsf3(SFtype a, SFtype b)//单精度减法
{
  FP_DECL_EX;
  FP_DECL_S(A);
  FP_DECL_S(B);
  FP_DECL_S(R);
  SFtype r;
  FP_INIT_ROUNDMODE;
  FP_UNPACK_SEMIRAW_S(A, a);
  FP_UNPACK_SEMIRAW_S(B, b);
  FP_SUB_S(R, A, B);
  FP_PACK_SEMIRAW_S(r, R);
  FP_HANDLE_EXCEPTIONS;
  return r;
}
SFtype __mulsf3(SFtype a, SFtype b)//单精度乘法
{
  FP_DECL_EX;
  FP_DECL_S(A);
  FP_DECL_S(B);
  FP_DECL_S(R);
  SFtype r;
  FP_INIT_ROUNDMODE;
  FP_UNPACK_S(A, a);
  FP_UNPACK_S(B, b);
  FP_MUL_S(R, A, B);
  FP_PACK_S(r, R);
  FP_HANDLE_EXCEPTIONS;
  return r;
}
SFtype __divsf3(SFtype a, SFtype b)//单精度除法 
{
  FP_DECL_EX;
  FP_DECL_S(A);
  FP_DECL_S(B);
  FP_DECL_S(R);
  SFtype r;
  FP_INIT_ROUNDMODE;
  FP_UNPACK_S(A, a);
  FP_UNPACK_S(B, b);
  FP_DIV_S(R, A, B);
  FP_PACK_S(r, R);
  FP_HANDLE_EXCEPTIONS;
  return r;
}

#define __extendsfdf2 SECTION(".riscv_ccode_fp") extendsfdf2
#define __truncdfsf2 SECTION(".riscv_ccode_fp") truncdfsf2

//精度转化
DFtype __extendsfdf2(SFtype a)//浮点数扩展为双精度浮点数
{
  FP_DECL_EX;
  FP_DECL_S(A);
  FP_DECL_D(R);
  DFtype r;
  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_S(A, a);
#if _FP_W_TYPE_SIZE < _FP_FRACBITS_D
  FP_EXTEND(D, S, 2, 1, R, A);
#else
  FP_EXTEND(D, S, 1, 1, R, A);
#endif
  FP_PACK_RAW_D(r, R);
  FP_HANDLE_EXCEPTIONS;
  return r;
}
SFtype __truncdfsf2(DFtype a)//双精度浮点数截断为单精度浮点数
{
  FP_DECL_EX;
  FP_DECL_D(A);
  FP_DECL_S(R);
  SFtype r;
  FP_INIT_ROUNDMODE;
  FP_UNPACK_SEMIRAW_D(A, a);
#if _FP_W_TYPE_SIZE < _FP_FRACBITS_D
  FP_TRUNC(S, D, 1, 2, R, A);
#else
  FP_TRUNC(S, D, 1, 1, R, A);
#endif
  FP_PACK_SEMIRAW_S(r, R);
  FP_HANDLE_EXCEPTIONS;
  return r;
}

#define __adddf3 SECTION(".riscv_ccode_fp") adddf3
#define __subdf3 SECTION(".riscv_ccode_fp") subdf3
#define __muldf3 SECTION(".riscv_ccode_fp") muldf3
#define __divdf3 SECTION(".riscv_ccode_fp") divdf3
//双精度算法
DFtype  __adddf3(DFtype a, DFtype b)//双精度加法
{
  FP_DECL_EX;
  FP_DECL_D(A);
  FP_DECL_D(B);
  FP_DECL_D(R);
  DFtype r;

  FP_INIT_ROUNDMODE;
  FP_UNPACK_SEMIRAW_D(A, a);
  FP_UNPACK_SEMIRAW_D(B, b);
  FP_ADD_D(R, A, B);
  FP_PACK_SEMIRAW_D(r, R);
  FP_HANDLE_EXCEPTIONS;

  return r;
}
DFtype __subdf3(DFtype a, DFtype b)//双精度减法
{
  FP_DECL_EX;
  FP_DECL_D(A);
  FP_DECL_D(B);
  FP_DECL_D(R);
  DFtype r;

  FP_INIT_ROUNDMODE;
  FP_UNPACK_SEMIRAW_D(A, a);
  FP_UNPACK_SEMIRAW_D(B, b);
  FP_SUB_D(R, A, B);
  FP_PACK_SEMIRAW_D(r, R);
  FP_HANDLE_EXCEPTIONS;

  return r;
}
DFtype __muldf3(DFtype a, DFtype b)//双精度乘法
{
  FP_DECL_EX;
  FP_DECL_D(A);
  FP_DECL_D(B);
  FP_DECL_D(R);
  DFtype r;
  FP_INIT_ROUNDMODE;
  FP_UNPACK_D(A, a);
  FP_UNPACK_D(B, b);
  FP_MUL_D(R, A, B);
  FP_PACK_D(r, R);
  FP_HANDLE_EXCEPTIONS;
  return r;
}
DFtype __divdf3(DFtype a, DFtype b)//双精度除法
{
  FP_DECL_EX;
  FP_DECL_D(A);
  FP_DECL_D(B);
  FP_DECL_D(R);
  DFtype r;
  FP_INIT_ROUNDMODE;
  FP_UNPACK_D(A, a);
  FP_UNPACK_D(B, b);
  FP_DIV_D(R, A, B);
  FP_PACK_D(r, R);
  FP_HANDLE_EXCEPTIONS;
  return r;
}

// //取反
// SFtype __negsf2(SFtype a)//单精度浮点数取反negative single float
// {
//   FP_DECL_S(A);
//   FP_DECL_S(R);
//   SFtype r;
//   FP_UNPACK_RAW_S(A, a);
//   FP_NEG_S(R, A);
//   FP_PACK_RAW_S(r, R);
//   return r;
// }
// DFtype __negdf2(DFtype a)//双精度浮点数取反negative double float
// {
//   FP_DECL_D(A);
//   FP_DECL_D(R);
//   DFtype r;
//   FP_UNPACK_RAW_D(A, a);
//   FP_NEG_D(R, A);
//   FP_PACK_RAW_D(r, R);
//   return r;
// }
// //判断无序
// CMPtype __unorddf2(DFtype a, DFtype b)//检查两个双精度浮点数是否无序（unordered）的函数
// {
//   FP_DECL_EX;
//   FP_DECL_D(A);
//   FP_DECL_D(B);
//   CMPtype r;
//   FP_INIT_EXCEPTIONS;
//   FP_UNPACK_RAW_D(A, a);
//   FP_UNPACK_RAW_D(B, b);
//   FP_CMP_UNORD_D(r, A, B, 1);
//   FP_HANDLE_EXCEPTIONS;
//   return r;
// }
// CMPtype __unordsf2(SFtype a, SFtype b)//检查两个单精度浮点数是否无序（unordered）的函数
// {
//   FP_DECL_EX;
//   FP_DECL_S(A);
//   FP_DECL_S(B);
//   CMPtype r;
//   FP_INIT_EXCEPTIONS;
//   FP_UNPACK_RAW_S(A, a);
//   FP_UNPACK_RAW_S(B, b);
//   FP_CMP_UNORD_S(r, A, B, 1);
//   FP_HANDLE_EXCEPTIONS;
//   return r;
// }
// //判断浮点数
// CMPtype __eqdf2(DFtype a, DFtype b)//比较两个双精度浮点数是否相等equal double float
// {
//   FP_DECL_EX;
//   FP_DECL_D(A);
//   FP_DECL_D(B);
//   CMPtype r;
//   FP_INIT_EXCEPTIONS;
//   FP_UNPACK_RAW_D(A, a);
//   FP_UNPACK_RAW_D(B, b);
//   FP_CMP_EQ_D(r, A, B, 1);
//   FP_HANDLE_EXCEPTIONS;
//   return r;
// }
// strong_alias(__eqdf2, __nedf2);
// CMPtype __eqsf2(SFtype a, SFtype b)//比较两个单精度浮点数是否相等equal single float
// {
//   FP_DECL_EX;
//   FP_DECL_S(A);
//   FP_DECL_S(B);
//   CMPtype r;
//   FP_INIT_EXCEPTIONS;
//   FP_UNPACK_RAW_S(A, a);
//   FP_UNPACK_RAW_S(B, b);
//   FP_CMP_EQ_S(r, A, B, 1);
//   FP_HANDLE_EXCEPTIONS;
//   return r;
// }
// strong_alias(__eqsf2, __nesf2);

#define __gedf2 SECTION(".riscv_ccode_fp") gedf2
#define __gesf2 SECTION(".riscv_ccode_fp") gesf2
#define __ledf2 SECTION(".riscv_ccode_fp") ledf2
#define __lesf2 SECTION(".riscv_ccode_fp") lesf2

CMPtype __gedf2(DFtype a, DFtype b)//比较两个双精度浮点数（大于）greater than or equal to double float
{
  FP_DECL_EX;
  FP_DECL_D(A);
  FP_DECL_D(B);
  CMPtype r;
  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_D(A, a);
  FP_UNPACK_RAW_D(B, b);
  FP_CMP_D(r, A, B, -2, 2);
  FP_HANDLE_EXCEPTIONS;
  return r;
}
strong_alias(gedf2, gtdf2);
CMPtype __gesf2(SFtype a, SFtype b)//比较两个单精度浮点数（大于）greater than or equal to single float
{
  FP_DECL_EX;
  FP_DECL_S(A);
  FP_DECL_S(B);
  CMPtype r;
  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_S(A, a);
  FP_UNPACK_RAW_S(B, b);
  FP_CMP_S(r, A, B, -2, 2);
  FP_HANDLE_EXCEPTIONS;
  return r;
}
strong_alias(gesf2, gtsf2);
CMPtype __ledf2(DFtype a, DFtype b)//比较两个双精度浮点数（小于）less than or equal to double float
{
  FP_DECL_EX;
  FP_DECL_D(A);
  FP_DECL_D(B);
  CMPtype r;
  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_D(A, a);
  FP_UNPACK_RAW_D(B, b);
  FP_CMP_D(r, A, B, 2, 2);
  FP_HANDLE_EXCEPTIONS;
  return r;
}
strong_alias(ledf2, ltdf2);
CMPtype __lesf2(SFtype a, SFtype b)//比较两个单精度浮点数（小于）less than or equal to single float
{
  FP_DECL_EX;
  FP_DECL_S(A);
  FP_DECL_S(B);
  CMPtype r;
  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_S(A, a);
  FP_UNPACK_RAW_S(B, b);
  FP_CMP_S(r, A, B, 2, 2);
  FP_HANDLE_EXCEPTIONS;
  return r;
}
strong_alias(lesf2, ltsf2);

#endif
