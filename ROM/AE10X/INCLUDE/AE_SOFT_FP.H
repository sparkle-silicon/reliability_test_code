/*
 * @Author: daweslinyu daowes.ly@qq.com
 * @Date: 2024-04-12 19:00:37
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-04-22 15:33:35
 * @FilePath: /gcc-13.2.0/home/riscv/work_git/DEBUGGER/Slave/AE10X/INCLUDE/AE_SOFT_FP.H
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef AE_SOFT_FP_H
#define AE_SOFT_FP_H
#include "RISCV.H"
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"

/******************************************riscv sfp-machine.h start******************************************/
#if __riscv_xlen == 32

#define _FP_W_TYPE_SIZE		32
#define _FP_W_TYPE		unsigned long
#define _FP_WS_TYPE		signed long
#define _FP_I_TYPE		long

#define _FP_MUL_MEAT_S(R,X,Y) _FP_MUL_MEAT_1_wide(_FP_WFRACBITS_S,R,X,Y,umul_ppmm)
#define _FP_MUL_MEAT_D(R,X,Y) _FP_MUL_MEAT_2_wide(_FP_WFRACBITS_D,R,X,Y,umul_ppmm)
#define _FP_MUL_MEAT_Q(R,X,Y) _FP_MUL_MEAT_4_wide(_FP_WFRACBITS_Q,R,X,Y,umul_ppmm)

#define _FP_DIV_MEAT_S(R,X,Y) _FP_DIV_MEAT_1_udiv_norm(S,R,X,Y)
#define _FP_DIV_MEAT_D(R,X,Y) _FP_DIV_MEAT_2_udiv(D,R,X,Y)
#define _FP_DIV_MEAT_Q(R,X,Y) _FP_DIV_MEAT_4_udiv(Q,R,X,Y)

#define _FP_NANFRAC_H		_FP_QNANBIT_H
#define _FP_NANFRAC_S		_FP_QNANBIT_S
#define _FP_NANFRAC_D		_FP_QNANBIT_D, 0
#define _FP_NANFRAC_Q		_FP_QNANBIT_Q, 0, 0, 0
#endif

/* The type of the result of a floating point comparison.  This must
   match __libgcc_cmp_return__ in GCC for the target.  */
typedef int __gcc_CMPtype __attribute__((mode(__libgcc_cmp_return__)));
#define CMPtype __gcc_CMPtype

#define _FP_NANSIGN_H		0
#define _FP_NANSIGN_S		0
#define _FP_NANSIGN_D		0
#define _FP_NANSIGN_Q		0

#define _FP_KEEPNANFRACP 0
#define _FP_QNANNEGATEDP 0

#define _FP_CHOOSENAN(fs, wc, R, X, Y, OP)	\
  do {						\
    R##_s = _FP_NANSIGN_##fs;			\
    _FP_FRAC_SET_##wc(R,_FP_NANFRAC_##fs);	\
    R##_c = FP_CLS_NAN;				\
  } while (0)

#define _FP_DECL_EX		int _frm __attribute__ ((unused));
#define FP_ROUNDMODE		_frm

#define FP_RND_NEAREST		0x0
#define FP_RND_ZERO		0x1
#define FP_RND_PINF		0x3
#define FP_RND_MINF		0x2

#define FP_EX_INVALID		0x10
#define FP_EX_OVERFLOW		0x04
#define FP_EX_UNDERFLOW		0x02
#define FP_EX_DIVZERO		0x08
#define FP_EX_INEXACT		0x01

#define _FP_TININESS_AFTER_ROUNDING 1

#ifdef __riscv_flen/*编译器正常编译无该宏，但显示可能显示，因此添加保护阅读机制*/
#undef __riscv_flen
#error  "__riscv_flen defined"
#endif

#ifdef __riscv_flen
#define FP_INIT_ROUNDMODE			\
do {						\
  __asm__ volatile ("frrm %0" : "=r" (_frm));	\
} while (0)

#define FP_HANDLE_EXCEPTIONS					\
do {								\
  if (__builtin_expect (_fex, 0))				\
    __asm__ volatile ("csrs fflags, %0" : : "rK" (_fex));	\
} while (0)
#else
#define FP_INIT_ROUNDMODE	_frm = FP_RND_NEAREST
#endif

#if defined(__BYTE_ORDER__) 
#define __BYTE_ORDER __BYTE_ORDER__
#else
#define __BYTE_ORDER __ORDER_LITTLE_ENDIAN__
#endif


/* Define ALIASNAME as a strong alias for NAME.  */
# define _strong_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((alias (#name)));
# define strong_alias(name, aliasname) _strong_alias(name, aliasname)
/******************************************riscv sfp-machine.h end******************************************/

/******************************************libgcc soft-fp.h start******************************************/

/* For unreachable default cases in switch statements over bitwise OR
   of FP_CLS_* values.  */
#if (defined __GNUC__							\
     && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)))
# define _FP_UNREACHABLE	__builtin_unreachable ()
#else
# define _FP_UNREACHABLE	abort ()
#endif

#if ((defined __GNUC__							\
      && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)))	\
     || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 201112L))
# define _FP_STATIC_ASSERT(expr, msg)		\
  _Static_assert ((expr), msg)
#else
# define _FP_STATIC_ASSERT(expr, msg)					\
  extern int (*__Static_assert_function (void))				\
    [!!sizeof (struct { int __error_if_negative: (expr) ? 2 : -1; })]
#endif

/* In the Linux kernel, some architectures have a single function that
   uses different kinds of unpacking and packing depending on the
   instruction being emulated, meaning it is not readily visible to
   the compiler that variables from _FP_DECL and _FP_FRAC_DECL_*
   macros are only used in cases where they were initialized.  */
#ifdef __KERNEL__
# define _FP_ZERO_INIT		= 0
#else
# define _FP_ZERO_INIT
#endif

#define _FP_WORKBITS		3
#define _FP_WORK_LSB		((_FP_W_TYPE) 1 << 3)
#define _FP_WORK_ROUND		((_FP_W_TYPE) 1 << 2)
#define _FP_WORK_GUARD		((_FP_W_TYPE) 1 << 1)
#define _FP_WORK_STICKY		((_FP_W_TYPE) 1 << 0)


/* By default don't care about exceptions.  */
#ifndef FP_EX_DENORM
# define FP_EX_DENORM		0
#endif

/* Sub-exceptions of "invalid".  */
/* Signaling NaN operand.  */
#ifndef FP_EX_INVALID_SNAN
# define FP_EX_INVALID_SNAN	0
#endif
/* Inf * 0.  */
#ifndef FP_EX_INVALID_IMZ
# define FP_EX_INVALID_IMZ	0
#endif
/* fma (Inf, 0, c).  */
#ifndef FP_EX_INVALID_IMZ_FMA
# define FP_EX_INVALID_IMZ_FMA	0
#endif
/* Inf - Inf.  */
#ifndef FP_EX_INVALID_ISI
# define FP_EX_INVALID_ISI	0
#endif
/* 0 / 0.  */
#ifndef FP_EX_INVALID_ZDZ
# define FP_EX_INVALID_ZDZ	0
#endif
/* Inf / Inf.  */
#ifndef FP_EX_INVALID_IDI
# define FP_EX_INVALID_IDI	0
#endif
/* sqrt (negative).  */
#ifndef FP_EX_INVALID_SQRT
# define FP_EX_INVALID_SQRT	0
#endif
/* Invalid conversion to integer.  */
#ifndef FP_EX_INVALID_CVI
# define FP_EX_INVALID_CVI	0
#endif
/* Invalid comparison.  */
#ifndef FP_EX_INVALID_VC
# define FP_EX_INVALID_VC	0
#endif

/* _FP_STRUCT_LAYOUT may be defined as an attribute to determine the
   struct layout variant used for structures where bit-fields are used
   to access specific parts of binary floating-point numbers.  This is
   required for systems where the default ABI uses struct layout with
   differences in how consecutive bit-fields are laid out from the
   default expected by soft-fp.  */
#ifndef _FP_STRUCT_LAYOUT
# define _FP_STRUCT_LAYOUT
#endif

# define FP_DECL_EX					\
  int _fex = 0;	/*_fex fflags(floating-point flags) exception*/\
_FP_DECL_EX	/*_frm frounding(floating-point rounding) mode*/

/* Initialize any machine-specific state used in
   FP_TRAPPING_EXCEPTIONS or FP_HANDLE_EXCEPTIONS.  */
#ifndef FP_INIT_TRAPPING_EXCEPTIONS
# define FP_INIT_TRAPPING_EXCEPTIONS FP_INIT_ROUNDMODE
#endif

/* Initialize any machine-specific state used in
   FP_HANDLE_EXCEPTIONS.  */
#ifndef FP_INIT_EXCEPTIONS
# define FP_INIT_EXCEPTIONS FP_INIT_TRAPPING_EXCEPTIONS
#endif

#ifndef FP_HANDLE_EXCEPTIONS
# define FP_HANDLE_EXCEPTIONS do {} while (0)
#endif

/* Whether to flush subnormal inputs to zero with the same sign.  */
#ifndef FP_DENORM_ZERO
# define FP_DENORM_ZERO 0
#endif

#ifndef FP_INHIBIT_RESULTS
/* By default we write the results always.
   sfp-machine may override this and e.g.
   check if some exceptions are unmasked
   and inhibit it in such a case.  */
# define FP_INHIBIT_RESULTS 0
#endif

#define FP_SET_EXCEPTION(ex)				\
  _fex |= (ex)

#define FP_CUR_EXCEPTIONS				\
  (_fex)

#ifndef FP_TRAPPING_EXCEPTIONS
# define FP_TRAPPING_EXCEPTIONS 0
#endif

/* A file using soft-fp may define FP_NO_EXCEPTIONS before including
   soft-fp.h to indicate that, although a macro used there could raise
   exceptions, or do rounding and potentially thereby raise
   exceptions, for some arguments, for the particular arguments used
   in that file no exceptions or rounding can occur.  Such a file
   should not itself use macros relating to handling exceptions and
   rounding modes; this is only for indirect uses (in particular, in
   _FP_FROM_INT and the macros it calls).  */
#ifdef FP_NO_EXCEPTIONS

# undef FP_SET_EXCEPTION
# define FP_SET_EXCEPTION(ex) do {} while (0)

# undef FP_CUR_EXCEPTIONS
# define FP_CUR_EXCEPTIONS 0

# undef FP_TRAPPING_EXCEPTIONS
# define FP_TRAPPING_EXCEPTIONS 0

# undef FP_ROUNDMODE
# define FP_ROUNDMODE FP_RND_ZERO

# undef _FP_TININESS_AFTER_ROUNDING
# define _FP_TININESS_AFTER_ROUNDING 0

#endif

/* A file using soft-fp may define FP_NO_EXACT_UNDERFLOW before
   including soft-fp.h to indicate that, although a macro used there
   could allow for the case of exact underflow requiring the underflow
   exception to be raised if traps are enabled, for the particular
   arguments used in that file no exact underflow can occur.  */
#ifdef FP_NO_EXACT_UNDERFLOW
# undef FP_TRAPPING_EXCEPTIONS
# define FP_TRAPPING_EXCEPTIONS 0
#endif

#define _FP_ROUND_NEAREST(wc, X)				\
  do								\
    {								\
      if ((_FP_FRAC_LOW_##wc (X) & 15) != _FP_WORK_ROUND)	\
	_FP_FRAC_ADDI_##wc (X, _FP_WORK_ROUND);			\
    }								\
  while (0)

#define _FP_ROUND_ZERO(wc, X)		(void) 0

#define _FP_ROUND_PINF(wc, X)				\
  do							\
    {							\
      if (!X##_s && (_FP_FRAC_LOW_##wc (X) & 7))	\
	_FP_FRAC_ADDI_##wc (X, _FP_WORK_LSB);		\
    }							\
  while (0)

#define _FP_ROUND_MINF(wc, X)			\
  do						\
    {						\
      if (X##_s && (_FP_FRAC_LOW_##wc (X) & 7))	\
	_FP_FRAC_ADDI_##wc (X, _FP_WORK_LSB);	\
    }						\
  while (0)

#define _FP_ROUND(wc, X)			\
  do						\
    {						\
      if (_FP_FRAC_LOW_##wc (X) & 7)		\
	{					\
	  FP_SET_EXCEPTION (FP_EX_INEXACT);	\
	  switch (FP_ROUNDMODE)			\
	    {					\
	    case FP_RND_NEAREST:		\
	      _FP_ROUND_NEAREST (wc, X);	\
	      break;				\
	    case FP_RND_ZERO:			\
	      _FP_ROUND_ZERO (wc, X);		\
	      break;				\
	    case FP_RND_PINF:			\
	      _FP_ROUND_PINF (wc, X);		\
	      break;				\
	    case FP_RND_MINF:			\
	      _FP_ROUND_MINF (wc, X);		\
	      break;				\
	    }					\
	}					\
    }						\
  while (0)

#define FP_CLS_NORMAL		0
#define FP_CLS_ZERO		1
#define FP_CLS_INF		2
#define FP_CLS_NAN		3

#define _FP_CLS_COMBINE(x, y)	(((x) << 2) | (y))

/******************************************libgcc op-1.h start******************************************/
#define _FP_FRAC_DECL_1(X)	_FP_W_TYPE X##_f _FP_ZERO_INIT
#define _FP_FRAC_COPY_1(D, S)	(D##_f = S##_f)
#define _FP_FRAC_SET_1(X, I)	(X##_f = I)
#define _FP_FRAC_HIGH_1(X)	(X##_f)
#define _FP_FRAC_LOW_1(X)	(X##_f)
#define _FP_FRAC_WORD_1(X, w)	(X##_f)

#define _FP_FRAC_ADDI_1(X, I)	(X##_f += I)
#define _FP_FRAC_SLL_1(X, N)			\
  do						\
    {						\
      if (__builtin_constant_p (N) && (N) == 1)	\
	X##_f += X##_f;				\
      else					\
	X##_f <<= (N);				\
    }						\
  while (0)
#define _FP_FRAC_SRL_1(X, N)	(X##_f >>= N)

/* Right shift with sticky-lsb.  */
#define _FP_FRAC_SRST_1(X, S, N, sz)	__FP_FRAC_SRST_1 (X##_f, S, (N), (sz))
#define _FP_FRAC_SRS_1(X, N, sz)	__FP_FRAC_SRS_1 (X##_f, (N), (sz))

#define __FP_FRAC_SRST_1(X, S, N, sz)			\
  do							\
    {							\
      S = (__builtin_constant_p (N) && (N) == 1		\
	   ? X & 1					\
	   : (X << (_FP_W_TYPE_SIZE - (N))) != 0);	\
      X = X >> (N);					\
    }							\
  while (0)

#define __FP_FRAC_SRS_1(X, N, sz)				\
  (X = (X >> (N) | (__builtin_constant_p (N) && (N) == 1	\
		    ? X & 1					\
		    : (X << (_FP_W_TYPE_SIZE - (N))) != 0)))

#define _FP_FRAC_ADD_1(R, X, Y)	(R##_f = X##_f + Y##_f)
#define _FP_FRAC_SUB_1(R, X, Y)	(R##_f = X##_f - Y##_f)
#define _FP_FRAC_DEC_1(X, Y)	(X##_f -= Y##_f)
#define _FP_FRAC_CLZ_1(z, X)	__FP_CLZ ((z), X##_f)

/* Predicates.  */
#define _FP_FRAC_NEGP_1(X)	((_FP_WS_TYPE) X##_f < 0)
#define _FP_FRAC_ZEROP_1(X)	(X##_f == 0)
#define _FP_FRAC_OVERP_1(fs, X)	(X##_f & _FP_OVERFLOW_##fs)
#define _FP_FRAC_CLEAR_OVERP_1(fs, X)	(X##_f &= ~_FP_OVERFLOW_##fs)
#define _FP_FRAC_HIGHBIT_DW_1(fs, X)	(X##_f & _FP_HIGHBIT_DW_##fs)
#define _FP_FRAC_EQ_1(X, Y)	(X##_f == Y##_f)
#define _FP_FRAC_GE_1(X, Y)	(X##_f >= Y##_f)
#define _FP_FRAC_GT_1(X, Y)	(X##_f > Y##_f)

#define _FP_ZEROFRAC_1		0
#define _FP_MINFRAC_1		1
#define _FP_MAXFRAC_1		(~(_FP_WS_TYPE) 0)

/* Unpack the raw bits of a native fp value.  Do not classify or
   normalize the data.  */

#define _FP_UNPACK_RAW_1(fs, X, val)			\
  do							\
    {							\
      union _FP_UNION_##fs _FP_UNPACK_RAW_1_flo;	\
      _FP_UNPACK_RAW_1_flo.flt = (val);			\
							\
      X##_f = _FP_UNPACK_RAW_1_flo.bits.frac;		\
      X##_e = _FP_UNPACK_RAW_1_flo.bits.exp;		\
      X##_s = _FP_UNPACK_RAW_1_flo.bits.sign;		\
    }							\
  while (0)

#define _FP_UNPACK_RAW_1_P(fs, X, val)			\
  do							\
    {							\
      union _FP_UNION_##fs *_FP_UNPACK_RAW_1_P_flo	\
	= (union _FP_UNION_##fs *) (val);		\
							\
      X##_f = _FP_UNPACK_RAW_1_P_flo->bits.frac;	\
      X##_e = _FP_UNPACK_RAW_1_P_flo->bits.exp;		\
      X##_s = _FP_UNPACK_RAW_1_P_flo->bits.sign;	\
    }							\
  while (0)

/* Repack the raw bits of a native fp value.  */

#define _FP_PACK_RAW_1(fs, val, X)		\
  do						\
    {						\
      union _FP_UNION_##fs _FP_PACK_RAW_1_flo;	\
						\
      _FP_PACK_RAW_1_flo.bits.frac = X##_f;	\
      _FP_PACK_RAW_1_flo.bits.exp  = X##_e;	\
      _FP_PACK_RAW_1_flo.bits.sign = X##_s;	\
						\
      (val) = _FP_PACK_RAW_1_flo.flt;		\
    }						\
  while (0)

#define _FP_PACK_RAW_1_P(fs, val, X)			\
  do							\
    {							\
      union _FP_UNION_##fs *_FP_PACK_RAW_1_P_flo	\
	= (union _FP_UNION_##fs *) (val);		\
							\
      _FP_PACK_RAW_1_P_flo->bits.frac = X##_f;		\
      _FP_PACK_RAW_1_P_flo->bits.exp  = X##_e;		\
      _FP_PACK_RAW_1_P_flo->bits.sign = X##_s;		\
    }							\
  while (0)


/* Multiplication algorithms: */

/* Basic.  Assuming the host word size is >= 2*FRACBITS, we can do the
   multiplication immediately.  */

#define _FP_MUL_MEAT_DW_1_imm(wfracbits, R, X, Y)	\
  do							\
    {							\
      R##_f = X##_f * Y##_f;				\
    }							\
  while (0)

#define _FP_MUL_MEAT_1_imm(wfracbits, R, X, Y)				\
  do									\
    {									\
      _FP_MUL_MEAT_DW_1_imm ((wfracbits), R, X, Y);			\
      /* Normalize since we know where the msb of the multiplicands	\
	 were (bit B), we know that the msb of the of the product is	\
	 at either 2B or 2B-1.  */					\
      _FP_FRAC_SRS_1 (R, (wfracbits)-1, 2*(wfracbits));			\
    }									\
  while (0)

/* Given a 1W * 1W => 2W primitive, do the extended multiplication.  */

#define _FP_MUL_MEAT_DW_1_wide(wfracbits, R, X, Y, doit)	\
  do								\
    {								\
      doit (R##_f1, R##_f0, X##_f, Y##_f);			\
    }								\
  while (0)

#define _FP_MUL_MEAT_1_wide(wfracbits, R, X, Y, doit)			\
  do									\
    {									\
      _FP_FRAC_DECL_2 (_FP_MUL_MEAT_1_wide_Z);				\
      _FP_MUL_MEAT_DW_1_wide ((wfracbits), _FP_MUL_MEAT_1_wide_Z,	\
			      X, Y, doit);				\
      /* Normalize since we know where the msb of the multiplicands	\
	 were (bit B), we know that the msb of the of the product is	\
	 at either 2B or 2B-1.  */					\
      _FP_FRAC_SRS_2 (_FP_MUL_MEAT_1_wide_Z, (wfracbits)-1,		\
		      2*(wfracbits));					\
      R##_f = _FP_MUL_MEAT_1_wide_Z_f0;					\
    }									\
  while (0)

/* Finally, a simple widening multiply algorithm.  What fun!  */

#define _FP_MUL_MEAT_DW_1_hard(wfracbits, R, X, Y)			\
  do									\
    {									\
      _FP_W_TYPE _FP_MUL_MEAT_DW_1_hard_xh, _FP_MUL_MEAT_DW_1_hard_xl;	\
      _FP_W_TYPE _FP_MUL_MEAT_DW_1_hard_yh, _FP_MUL_MEAT_DW_1_hard_yl;	\
      _FP_FRAC_DECL_2 (_FP_MUL_MEAT_DW_1_hard_a);			\
									\
      /* Split the words in half.  */					\
      _FP_MUL_MEAT_DW_1_hard_xh = X##_f >> (_FP_W_TYPE_SIZE/2);		\
      _FP_MUL_MEAT_DW_1_hard_xl						\
	= X##_f & (((_FP_W_TYPE) 1 << (_FP_W_TYPE_SIZE/2)) - 1);	\
      _FP_MUL_MEAT_DW_1_hard_yh = Y##_f >> (_FP_W_TYPE_SIZE/2);		\
      _FP_MUL_MEAT_DW_1_hard_yl						\
	= Y##_f & (((_FP_W_TYPE) 1 << (_FP_W_TYPE_SIZE/2)) - 1);	\
									\
      /* Multiply the pieces.  */					\
      R##_f0 = _FP_MUL_MEAT_DW_1_hard_xl * _FP_MUL_MEAT_DW_1_hard_yl;	\
      _FP_MUL_MEAT_DW_1_hard_a_f0					\
	= _FP_MUL_MEAT_DW_1_hard_xh * _FP_MUL_MEAT_DW_1_hard_yl;	\
      _FP_MUL_MEAT_DW_1_hard_a_f1					\
	= _FP_MUL_MEAT_DW_1_hard_xl * _FP_MUL_MEAT_DW_1_hard_yh;	\
      R##_f1 = _FP_MUL_MEAT_DW_1_hard_xh * _FP_MUL_MEAT_DW_1_hard_yh;	\
									\
      /* Reassemble into two full words.  */				\
      if ((_FP_MUL_MEAT_DW_1_hard_a_f0 += _FP_MUL_MEAT_DW_1_hard_a_f1)	\
	  < _FP_MUL_MEAT_DW_1_hard_a_f1)				\
	R##_f1 += (_FP_W_TYPE) 1 << (_FP_W_TYPE_SIZE/2);		\
      _FP_MUL_MEAT_DW_1_hard_a_f1					\
	= _FP_MUL_MEAT_DW_1_hard_a_f0 >> (_FP_W_TYPE_SIZE/2);		\
      _FP_MUL_MEAT_DW_1_hard_a_f0					\
	= _FP_MUL_MEAT_DW_1_hard_a_f0 << (_FP_W_TYPE_SIZE/2);		\
      _FP_FRAC_ADD_2 (R, R, _FP_MUL_MEAT_DW_1_hard_a);			\
    }									\
  while (0)

#define _FP_MUL_MEAT_1_hard(wfracbits, R, X, Y)			\
  do								\
    {								\
      _FP_FRAC_DECL_2 (_FP_MUL_MEAT_1_hard_z);			\
      _FP_MUL_MEAT_DW_1_hard ((wfracbits),			\
			      _FP_MUL_MEAT_1_hard_z, X, Y);	\
								\
      /* Normalize.  */						\
      _FP_FRAC_SRS_2 (_FP_MUL_MEAT_1_hard_z,			\
		      (wfracbits) - 1, 2*(wfracbits));		\
      R##_f = _FP_MUL_MEAT_1_hard_z_f0;				\
    }								\
  while (0)


/* Division algorithms: */

/* Basic.  Assuming the host word size is >= 2*FRACBITS, we can do the
   division immediately.  Give this macro either _FP_DIV_HELP_imm for
   C primitives or _FP_DIV_HELP_ldiv for the ISO function.  Which you
   choose will depend on what the compiler does with divrem4.  */

#define _FP_DIV_MEAT_1_imm(fs, R, X, Y, doit)				\
  do									\
    {									\
      _FP_W_TYPE _FP_DIV_MEAT_1_imm_q, _FP_DIV_MEAT_1_imm_r;		\
      X##_f <<= (X##_f < Y##_f						\
		 ? R##_e--, _FP_WFRACBITS_##fs				\
		 : _FP_WFRACBITS_##fs - 1);				\
      doit (_FP_DIV_MEAT_1_imm_q, _FP_DIV_MEAT_1_imm_r, X##_f, Y##_f);	\
      R##_f = _FP_DIV_MEAT_1_imm_q | (_FP_DIV_MEAT_1_imm_r != 0);	\
    }									\
  while (0)

/* GCC's longlong.h defines a 2W / 1W => (1W,1W) primitive udiv_qrnnd
   that may be useful in this situation.  This first is for a primitive
   that requires normalization, the second for one that does not.  Look
   for UDIV_NEEDS_NORMALIZATION to tell which your machine needs.  */

#define _FP_DIV_MEAT_1_udiv_norm(fs, R, X, Y)				\
  do									\
    {									\
      _FP_W_TYPE _FP_DIV_MEAT_1_udiv_norm_nh;				\
      _FP_W_TYPE _FP_DIV_MEAT_1_udiv_norm_nl;				\
      _FP_W_TYPE _FP_DIV_MEAT_1_udiv_norm_q;				\
      _FP_W_TYPE _FP_DIV_MEAT_1_udiv_norm_r;				\
      _FP_W_TYPE _FP_DIV_MEAT_1_udiv_norm_y;				\
									\
      /* Normalize Y -- i.e. make the most significant bit set.  */	\
      _FP_DIV_MEAT_1_udiv_norm_y = Y##_f << _FP_WFRACXBITS_##fs;	\
									\
      /* Shift X op correspondingly high, that is, up one full word.  */ \
      if (X##_f < Y##_f)						\
	{								\
	  R##_e--;							\
	  _FP_DIV_MEAT_1_udiv_norm_nl = 0;				\
	  _FP_DIV_MEAT_1_udiv_norm_nh = X##_f;				\
	}								\
      else								\
	{								\
	  _FP_DIV_MEAT_1_udiv_norm_nl = X##_f << (_FP_W_TYPE_SIZE - 1);	\
	  _FP_DIV_MEAT_1_udiv_norm_nh = X##_f >> 1;			\
	}								\
									\
      udiv_qrnnd (_FP_DIV_MEAT_1_udiv_norm_q,				\
		  _FP_DIV_MEAT_1_udiv_norm_r,				\
		  _FP_DIV_MEAT_1_udiv_norm_nh,				\
		  _FP_DIV_MEAT_1_udiv_norm_nl,				\
		  _FP_DIV_MEAT_1_udiv_norm_y);				\
      R##_f = (_FP_DIV_MEAT_1_udiv_norm_q				\
	       | (_FP_DIV_MEAT_1_udiv_norm_r != 0));			\
    }									\
  while (0)

#define _FP_DIV_MEAT_1_udiv(fs, R, X, Y)				\
  do									\
    {									\
      _FP_W_TYPE _FP_DIV_MEAT_1_udiv_nh, _FP_DIV_MEAT_1_udiv_nl;	\
      _FP_W_TYPE _FP_DIV_MEAT_1_udiv_q, _FP_DIV_MEAT_1_udiv_r;		\
      if (X##_f < Y##_f)						\
	{								\
	  R##_e--;							\
	  _FP_DIV_MEAT_1_udiv_nl = X##_f << _FP_WFRACBITS_##fs;		\
	  _FP_DIV_MEAT_1_udiv_nh = X##_f >> _FP_WFRACXBITS_##fs;	\
	}								\
      else								\
	{								\
	  _FP_DIV_MEAT_1_udiv_nl = X##_f << (_FP_WFRACBITS_##fs - 1);	\
	  _FP_DIV_MEAT_1_udiv_nh = X##_f >> (_FP_WFRACXBITS_##fs + 1);	\
	}								\
      udiv_qrnnd (_FP_DIV_MEAT_1_udiv_q, _FP_DIV_MEAT_1_udiv_r,		\
		  _FP_DIV_MEAT_1_udiv_nh, _FP_DIV_MEAT_1_udiv_nl,	\
		  Y##_f);						\
      R##_f = _FP_DIV_MEAT_1_udiv_q | (_FP_DIV_MEAT_1_udiv_r != 0);	\
    }									\
  while (0)


/* Square root algorithms:
   We have just one right now, maybe Newton approximation
   should be added for those machines where division is fast.  */

#define _FP_SQRT_MEAT_1(R, S, T, X, q)		\
  do						\
    {						\
      while ((q) != _FP_WORK_ROUND)		\
	{					\
	  T##_f = S##_f + (q);			\
	  if (T##_f <= X##_f)			\
	    {					\
	      S##_f = T##_f + (q);		\
	      X##_f -= T##_f;			\
	      R##_f += (q);			\
	    }					\
	  _FP_FRAC_SLL_1 (X, 1);		\
	  (q) >>= 1;				\
	}					\
      if (X##_f)				\
	{					\
	  if (S##_f < X##_f)			\
	    R##_f |= _FP_WORK_ROUND;		\
	  R##_f |= _FP_WORK_STICKY;		\
	}					\
    }						\
  while (0)

/* Assembly/disassembly for converting to/from integral types.
   No shifting or overflow handled here.  */

#define _FP_FRAC_ASSEMBLE_1(r, X, rsize)	((r) = X##_f)
#define _FP_FRAC_DISASSEMBLE_1(X, r, rsize)	(X##_f = (r))


/* Convert FP values between word sizes.  */

#define _FP_FRAC_COPY_1_1(D, S)		(D##_f = S##_f)
/******************************************libgcc op-1.h end******************************************/
/******************************************libgcc op-2.h start******************************************/
#define _FP_FRAC_DECL_2(X)				\
  _FP_W_TYPE X##_f0 _FP_ZERO_INIT, X##_f1 _FP_ZERO_INIT
#define _FP_FRAC_COPY_2(D, S)	(D##_f0 = S##_f0, D##_f1 = S##_f1)
#define _FP_FRAC_SET_2(X, I)	__FP_FRAC_SET_2 (X, I)
#define _FP_FRAC_HIGH_2(X)	(X##_f1)
#define _FP_FRAC_LOW_2(X)	(X##_f0)
#define _FP_FRAC_WORD_2(X, w)	(X##_f##w)

#define _FP_FRAC_SLL_2(X, N)						\
  (void) (((N) < _FP_W_TYPE_SIZE)					\
	  ? ({								\
	      if (__builtin_constant_p (N) && (N) == 1)			\
		{							\
		  X##_f1 = X##_f1 + X##_f1 + (((_FP_WS_TYPE) (X##_f0)) < 0); \
		  X##_f0 += X##_f0;					\
		}							\
	      else							\
		{							\
		  X##_f1 = X##_f1 << (N) | X##_f0 >> (_FP_W_TYPE_SIZE - (N)); \
		  X##_f0 <<= (N);					\
		}							\
	      0;							\
	    })								\
	  : ({								\
	      X##_f1 = X##_f0 << ((N) - _FP_W_TYPE_SIZE);		\
	      X##_f0 = 0;						\
	    }))


#define _FP_FRAC_SRL_2(X, N)						\
  (void) (((N) < _FP_W_TYPE_SIZE)					\
	  ? ({								\
	      X##_f0 = X##_f0 >> (N) | X##_f1 << (_FP_W_TYPE_SIZE - (N)); \
	      X##_f1 >>= (N);						\
	    })								\
	  : ({								\
	      X##_f0 = X##_f1 >> ((N) - _FP_W_TYPE_SIZE);		\
	      X##_f1 = 0;						\
	    }))

/* Right shift with sticky-lsb.  */
#define _FP_FRAC_SRST_2(X, S, N, sz)					\
  (void) (((N) < _FP_W_TYPE_SIZE)					\
	  ? ({								\
	      S = (__builtin_constant_p (N) && (N) == 1			\
		   ? X##_f0 & 1						\
		   : (X##_f0 << (_FP_W_TYPE_SIZE - (N))) != 0);		\
	      X##_f0 = (X##_f1 << (_FP_W_TYPE_SIZE - (N)) | X##_f0 >> (N)); \
	      X##_f1 >>= (N);						\
	    })								\
	  : ({								\
	      S = ((((N) == _FP_W_TYPE_SIZE				\
		     ? 0						\
		     : (X##_f1 << (2*_FP_W_TYPE_SIZE - (N))))		\
		    | X##_f0) != 0);					\
	      X##_f0 = (X##_f1 >> ((N) - _FP_W_TYPE_SIZE));		\
	      X##_f1 = 0;						\
	    }))

#define _FP_FRAC_SRS_2(X, N, sz)					\
  (void) (((N) < _FP_W_TYPE_SIZE)					\
	  ? ({								\
	      X##_f0 = (X##_f1 << (_FP_W_TYPE_SIZE - (N)) | X##_f0 >> (N) \
			| (__builtin_constant_p (N) && (N) == 1		\
			   ? X##_f0 & 1					\
			   : (X##_f0 << (_FP_W_TYPE_SIZE - (N))) != 0)); \
	      X##_f1 >>= (N);						\
	    })								\
	  : ({								\
	      X##_f0 = (X##_f1 >> ((N) - _FP_W_TYPE_SIZE)		\
			| ((((N) == _FP_W_TYPE_SIZE			\
			     ? 0					\
			     : (X##_f1 << (2*_FP_W_TYPE_SIZE - (N))))	\
			    | X##_f0) != 0));				\
	      X##_f1 = 0;						\
	    }))

#define _FP_FRAC_ADDI_2(X, I)	\
  __FP_FRAC_ADDI_2 (X##_f1, X##_f0, I)

#define _FP_FRAC_ADD_2(R, X, Y)	\
  __FP_FRAC_ADD_2 (R##_f1, R##_f0, X##_f1, X##_f0, Y##_f1, Y##_f0)

#define _FP_FRAC_SUB_2(R, X, Y)	\
  __FP_FRAC_SUB_2 (R##_f1, R##_f0, X##_f1, X##_f0, Y##_f1, Y##_f0)

#define _FP_FRAC_DEC_2(X, Y)	\
  __FP_FRAC_DEC_2 (X##_f1, X##_f0, Y##_f1, Y##_f0)

#define _FP_FRAC_CLZ_2(R, X)			\
  do						\
    {						\
      if (X##_f1)				\
	__FP_CLZ ((R), X##_f1);			\
      else					\
	{					\
	  __FP_CLZ ((R), X##_f0);		\
	  (R) += _FP_W_TYPE_SIZE;		\
	}					\
    }						\
  while (0)

/* Predicates.  */
#define _FP_FRAC_NEGP_2(X)	((_FP_WS_TYPE) X##_f1 < 0)
#define _FP_FRAC_ZEROP_2(X)	((X##_f1 | X##_f0) == 0)
#define _FP_FRAC_OVERP_2(fs, X)	(_FP_FRAC_HIGH_##fs (X) & _FP_OVERFLOW_##fs)
#define _FP_FRAC_CLEAR_OVERP_2(fs, X)	(_FP_FRAC_HIGH_##fs (X) &= ~_FP_OVERFLOW_##fs)
#define _FP_FRAC_HIGHBIT_DW_2(fs, X)	\
  (_FP_FRAC_HIGH_DW_##fs (X) & _FP_HIGHBIT_DW_##fs)
#define _FP_FRAC_EQ_2(X, Y)	(X##_f1 == Y##_f1 && X##_f0 == Y##_f0)
#define _FP_FRAC_GT_2(X, Y)	\
  (X##_f1 > Y##_f1 || (X##_f1 == Y##_f1 && X##_f0 > Y##_f0))
#define _FP_FRAC_GE_2(X, Y)	\
  (X##_f1 > Y##_f1 || (X##_f1 == Y##_f1 && X##_f0 >= Y##_f0))

#define _FP_ZEROFRAC_2		0, 0
#define _FP_MINFRAC_2		0, 1
#define _FP_MAXFRAC_2		(~(_FP_WS_TYPE) 0), (~(_FP_WS_TYPE) 0)

/* Internals.  */

#define __FP_FRAC_SET_2(X, I1, I0)	(X##_f0 = I0, X##_f1 = I1)

#define __FP_CLZ_2(R, xh, xl)			\
  do						\
    {						\
      if (xh)					\
	__FP_CLZ ((R), xh);			\
      else					\
	{					\
	  __FP_CLZ ((R), xl);			\
	  (R) += _FP_W_TYPE_SIZE;		\
	}					\
    }						\
  while (0)

#if 0

# ifndef __FP_FRAC_ADDI_2
#  define __FP_FRAC_ADDI_2(xh, xl, i)	\
  (xh += ((xl += i) < i))
# endif
# ifndef __FP_FRAC_ADD_2
#  define __FP_FRAC_ADD_2(rh, rl, xh, xl, yh, yl)	\
  (rh = xh + yh + ((rl = xl + yl) < xl))
# endif
# ifndef __FP_FRAC_SUB_2
#  define __FP_FRAC_SUB_2(rh, rl, xh, xl, yh, yl)	\
  (rh = xh - yh - ((rl = xl - yl) > xl))
# endif
# ifndef __FP_FRAC_DEC_2
#  define __FP_FRAC_DEC_2(xh, xl, yh, yl)		\
  do							\
    {							\
      UWtype __FP_FRAC_DEC_2_t = xl;			\
      xh -= yh + ((xl -= yl) > __FP_FRAC_DEC_2_t);	\
    }							\
  while (0)
# endif

#else

# undef __FP_FRAC_ADDI_2
# define __FP_FRAC_ADDI_2(xh, xl, i)	add_ssaaaa (xh, xl, xh, xl, 0, i)
# undef __FP_FRAC_ADD_2
# define __FP_FRAC_ADD_2		add_ssaaaa
# undef __FP_FRAC_SUB_2
# define __FP_FRAC_SUB_2		sub_ddmmss
# undef __FP_FRAC_DEC_2
# define __FP_FRAC_DEC_2(xh, xl, yh, yl)	\
  sub_ddmmss (xh, xl, xh, xl, yh, yl)

#endif

/* Unpack the raw bits of a native fp value.  Do not classify or
   normalize the data.  */

#define _FP_UNPACK_RAW_2(fs, X, val)			\
  do							\
    {							\
      union _FP_UNION_##fs _FP_UNPACK_RAW_2_flo;	\
      _FP_UNPACK_RAW_2_flo.flt = (val);			\
							\
      X##_f0 = _FP_UNPACK_RAW_2_flo.bits.frac0;		\
      X##_f1 = _FP_UNPACK_RAW_2_flo.bits.frac1;		\
      X##_e  = _FP_UNPACK_RAW_2_flo.bits.exp;		\
      X##_s  = _FP_UNPACK_RAW_2_flo.bits.sign;		\
    }							\
  while (0)

#define _FP_UNPACK_RAW_2_P(fs, X, val)			\
  do							\
    {							\
      union _FP_UNION_##fs *_FP_UNPACK_RAW_2_P_flo	\
	= (union _FP_UNION_##fs *) (val);		\
							\
      X##_f0 = _FP_UNPACK_RAW_2_P_flo->bits.frac0;	\
      X##_f1 = _FP_UNPACK_RAW_2_P_flo->bits.frac1;	\
      X##_e  = _FP_UNPACK_RAW_2_P_flo->bits.exp;	\
      X##_s  = _FP_UNPACK_RAW_2_P_flo->bits.sign;	\
    }							\
  while (0)


/* Repack the raw bits of a native fp value.  */

#define _FP_PACK_RAW_2(fs, val, X)		\
  do						\
    {						\
      union _FP_UNION_##fs _FP_PACK_RAW_2_flo;	\
						\
      _FP_PACK_RAW_2_flo.bits.frac0 = X##_f0;	\
      _FP_PACK_RAW_2_flo.bits.frac1 = X##_f1;	\
      _FP_PACK_RAW_2_flo.bits.exp   = X##_e;	\
      _FP_PACK_RAW_2_flo.bits.sign  = X##_s;	\
						\
      (val) = _FP_PACK_RAW_2_flo.flt;		\
    }						\
  while (0)

#define _FP_PACK_RAW_2_P(fs, val, X)			\
  do							\
    {							\
      union _FP_UNION_##fs *_FP_PACK_RAW_2_P_flo	\
	= (union _FP_UNION_##fs *) (val);		\
							\
      _FP_PACK_RAW_2_P_flo->bits.frac0 = X##_f0;	\
      _FP_PACK_RAW_2_P_flo->bits.frac1 = X##_f1;	\
      _FP_PACK_RAW_2_P_flo->bits.exp   = X##_e;		\
      _FP_PACK_RAW_2_P_flo->bits.sign  = X##_s;		\
    }							\
  while (0)


/* Multiplication algorithms: */

/* Given a 1W * 1W => 2W primitive, do the extended multiplication.  */

#define _FP_MUL_MEAT_DW_2_wide(wfracbits, R, X, Y, doit)		\
  do									\
    {									\
      _FP_FRAC_DECL_2 (_FP_MUL_MEAT_DW_2_wide_b);			\
      _FP_FRAC_DECL_2 (_FP_MUL_MEAT_DW_2_wide_c);			\
									\
      doit (_FP_FRAC_WORD_4 (R, 1), _FP_FRAC_WORD_4 (R, 0),		\
	    X##_f0, Y##_f0);						\
      doit (_FP_MUL_MEAT_DW_2_wide_b_f1, _FP_MUL_MEAT_DW_2_wide_b_f0,	\
	    X##_f0, Y##_f1);						\
      doit (_FP_MUL_MEAT_DW_2_wide_c_f1, _FP_MUL_MEAT_DW_2_wide_c_f0,	\
	    X##_f1, Y##_f0);						\
      doit (_FP_FRAC_WORD_4 (R, 3), _FP_FRAC_WORD_4 (R, 2),		\
	    X##_f1, Y##_f1);						\
									\
      __FP_FRAC_ADD_3 (_FP_FRAC_WORD_4 (R, 3), _FP_FRAC_WORD_4 (R, 2),	\
		       _FP_FRAC_WORD_4 (R, 1), 0,			\
		       _FP_MUL_MEAT_DW_2_wide_b_f1,			\
		       _FP_MUL_MEAT_DW_2_wide_b_f0,			\
		       _FP_FRAC_WORD_4 (R, 3), _FP_FRAC_WORD_4 (R, 2),	\
		       _FP_FRAC_WORD_4 (R, 1));				\
      __FP_FRAC_ADD_3 (_FP_FRAC_WORD_4 (R, 3), _FP_FRAC_WORD_4 (R, 2),	\
		       _FP_FRAC_WORD_4 (R, 1), 0,			\
		       _FP_MUL_MEAT_DW_2_wide_c_f1,			\
		       _FP_MUL_MEAT_DW_2_wide_c_f0,			\
		       _FP_FRAC_WORD_4 (R, 3), _FP_FRAC_WORD_4 (R, 2),	\
		       _FP_FRAC_WORD_4 (R, 1));				\
    }									\
  while (0)

#define _FP_MUL_MEAT_2_wide(wfracbits, R, X, Y, doit)			\
  do									\
    {									\
      _FP_FRAC_DECL_4 (_FP_MUL_MEAT_2_wide_z);				\
									\
      _FP_MUL_MEAT_DW_2_wide ((wfracbits), _FP_MUL_MEAT_2_wide_z,	\
			      X, Y, doit);				\
									\
      /* Normalize since we know where the msb of the multiplicands	\
	 were (bit B), we know that the msb of the of the product is	\
	 at either 2B or 2B-1.  */					\
      _FP_FRAC_SRS_4 (_FP_MUL_MEAT_2_wide_z, (wfracbits)-1,		\
		      2*(wfracbits));					\
      R##_f0 = _FP_FRAC_WORD_4 (_FP_MUL_MEAT_2_wide_z, 0);		\
      R##_f1 = _FP_FRAC_WORD_4 (_FP_MUL_MEAT_2_wide_z, 1);		\
    }									\
  while (0)

/* Given a 1W * 1W => 2W primitive, do the extended multiplication.
   Do only 3 multiplications instead of four. This one is for machines
   where multiplication is much more expensive than subtraction.  */

#define _FP_MUL_MEAT_DW_2_wide_3mul(wfracbits, R, X, Y, doit)		\
  do									\
    {									\
      _FP_FRAC_DECL_2 (_FP_MUL_MEAT_DW_2_wide_3mul_b);			\
      _FP_FRAC_DECL_2 (_FP_MUL_MEAT_DW_2_wide_3mul_c);			\
      _FP_W_TYPE _FP_MUL_MEAT_DW_2_wide_3mul_d;				\
      int _FP_MUL_MEAT_DW_2_wide_3mul_c1;				\
      int _FP_MUL_MEAT_DW_2_wide_3mul_c2;				\
									\
      _FP_MUL_MEAT_DW_2_wide_3mul_b_f0 = X##_f0 + X##_f1;		\
      _FP_MUL_MEAT_DW_2_wide_3mul_c1					\
	= _FP_MUL_MEAT_DW_2_wide_3mul_b_f0 < X##_f0;			\
      _FP_MUL_MEAT_DW_2_wide_3mul_b_f1 = Y##_f0 + Y##_f1;		\
      _FP_MUL_MEAT_DW_2_wide_3mul_c2					\
	= _FP_MUL_MEAT_DW_2_wide_3mul_b_f1 < Y##_f0;			\
      doit (_FP_MUL_MEAT_DW_2_wide_3mul_d, _FP_FRAC_WORD_4 (R, 0),	\
	    X##_f0, Y##_f0);						\
      doit (_FP_FRAC_WORD_4 (R, 2), _FP_FRAC_WORD_4 (R, 1),		\
	    _FP_MUL_MEAT_DW_2_wide_3mul_b_f0,				\
	    _FP_MUL_MEAT_DW_2_wide_3mul_b_f1);				\
      doit (_FP_MUL_MEAT_DW_2_wide_3mul_c_f1,				\
	    _FP_MUL_MEAT_DW_2_wide_3mul_c_f0, X##_f1, Y##_f1);		\
									\
      _FP_MUL_MEAT_DW_2_wide_3mul_b_f0					\
	&= -_FP_MUL_MEAT_DW_2_wide_3mul_c2;				\
      _FP_MUL_MEAT_DW_2_wide_3mul_b_f1					\
	&= -_FP_MUL_MEAT_DW_2_wide_3mul_c1;				\
      __FP_FRAC_ADD_3 (_FP_FRAC_WORD_4 (R, 3), _FP_FRAC_WORD_4 (R, 2),	\
		       _FP_FRAC_WORD_4 (R, 1),				\
		       (_FP_MUL_MEAT_DW_2_wide_3mul_c1			\
			& _FP_MUL_MEAT_DW_2_wide_3mul_c2), 0,		\
		       _FP_MUL_MEAT_DW_2_wide_3mul_d,			\
		       0, _FP_FRAC_WORD_4 (R, 2), _FP_FRAC_WORD_4 (R, 1)); \
      __FP_FRAC_ADDI_2 (_FP_FRAC_WORD_4 (R, 3), _FP_FRAC_WORD_4 (R, 2),	\
			_FP_MUL_MEAT_DW_2_wide_3mul_b_f0);		\
      __FP_FRAC_ADDI_2 (_FP_FRAC_WORD_4 (R, 3), _FP_FRAC_WORD_4 (R, 2),	\
			_FP_MUL_MEAT_DW_2_wide_3mul_b_f1);		\
      __FP_FRAC_DEC_3 (_FP_FRAC_WORD_4 (R, 3), _FP_FRAC_WORD_4 (R, 2),	\
		       _FP_FRAC_WORD_4 (R, 1),				\
		       0, _FP_MUL_MEAT_DW_2_wide_3mul_d,		\
		       _FP_FRAC_WORD_4 (R, 0));				\
      __FP_FRAC_DEC_3 (_FP_FRAC_WORD_4 (R, 3), _FP_FRAC_WORD_4 (R, 2),	\
		       _FP_FRAC_WORD_4 (R, 1), 0,			\
		       _FP_MUL_MEAT_DW_2_wide_3mul_c_f1,		\
		       _FP_MUL_MEAT_DW_2_wide_3mul_c_f0);		\
      __FP_FRAC_ADD_2 (_FP_FRAC_WORD_4 (R, 3), _FP_FRAC_WORD_4 (R, 2),	\
		       _FP_MUL_MEAT_DW_2_wide_3mul_c_f1,		\
		       _FP_MUL_MEAT_DW_2_wide_3mul_c_f0,		\
		       _FP_FRAC_WORD_4 (R, 3), _FP_FRAC_WORD_4 (R, 2));	\
    }									\
  while (0)

#define _FP_MUL_MEAT_2_wide_3mul(wfracbits, R, X, Y, doit)		\
  do									\
    {									\
      _FP_FRAC_DECL_4 (_FP_MUL_MEAT_2_wide_3mul_z);			\
									\
      _FP_MUL_MEAT_DW_2_wide_3mul ((wfracbits),				\
				   _FP_MUL_MEAT_2_wide_3mul_z,		\
				   X, Y, doit);				\
									\
      /* Normalize since we know where the msb of the multiplicands	\
	 were (bit B), we know that the msb of the of the product is	\
	 at either 2B or 2B-1.  */					\
      _FP_FRAC_SRS_4 (_FP_MUL_MEAT_2_wide_3mul_z,			\
		      (wfracbits)-1, 2*(wfracbits));			\
      R##_f0 = _FP_FRAC_WORD_4 (_FP_MUL_MEAT_2_wide_3mul_z, 0);		\
      R##_f1 = _FP_FRAC_WORD_4 (_FP_MUL_MEAT_2_wide_3mul_z, 1);		\
    }									\
  while (0)

#define _FP_MUL_MEAT_DW_2_gmp(wfracbits, R, X, Y)	\
  do							\
    {							\
      _FP_W_TYPE _FP_MUL_MEAT_DW_2_gmp_x[2];		\
      _FP_W_TYPE _FP_MUL_MEAT_DW_2_gmp_y[2];		\
      _FP_MUL_MEAT_DW_2_gmp_x[0] = X##_f0;		\
      _FP_MUL_MEAT_DW_2_gmp_x[1] = X##_f1;		\
      _FP_MUL_MEAT_DW_2_gmp_y[0] = Y##_f0;		\
      _FP_MUL_MEAT_DW_2_gmp_y[1] = Y##_f1;		\
							\
      mpn_mul_n (R##_f, _FP_MUL_MEAT_DW_2_gmp_x,	\
		 _FP_MUL_MEAT_DW_2_gmp_y, 2);		\
    }							\
  while (0)

#define _FP_MUL_MEAT_2_gmp(wfracbits, R, X, Y)				\
  do									\
    {									\
      _FP_FRAC_DECL_4 (_FP_MUL_MEAT_2_gmp_z);				\
									\
      _FP_MUL_MEAT_DW_2_gmp ((wfracbits), _FP_MUL_MEAT_2_gmp_z, X, Y);	\
									\
      /* Normalize since we know where the msb of the multiplicands	\
	 were (bit B), we know that the msb of the of the product is	\
	 at either 2B or 2B-1.  */					\
      _FP_FRAC_SRS_4 (_FP_MUL_MEAT_2_gmp_z, (wfracbits)-1,		\
		      2*(wfracbits));					\
      R##_f0 = _FP_MUL_MEAT_2_gmp_z_f[0];				\
      R##_f1 = _FP_MUL_MEAT_2_gmp_z_f[1];				\
    }									\
  while (0)

/* Do at most 120x120=240 bits multiplication using double floating
   point multiplication.  This is useful if floating point
   multiplication has much bigger throughput than integer multiply.
   It is supposed to work for _FP_W_TYPE_SIZE 64 and wfracbits
   between 106 and 120 only.
   Caller guarantees that X and Y has (1LLL << (wfracbits - 1)) set.
   SETFETZ is a macro which will disable all FPU exceptions and set rounding
   towards zero,  RESETFE should optionally reset it back.  */

#define _FP_MUL_MEAT_2_120_240_double(wfracbits, R, X, Y, setfetz, resetfe) \
  do									\
    {									\
      static const double _const[] =					\
	{								\
	  /* 2^-24 */ 5.9604644775390625e-08,				\
	  /* 2^-48 */ 3.5527136788005009e-15,				\
	  /* 2^-72 */ 2.1175823681357508e-22,				\
	  /* 2^-96 */ 1.2621774483536189e-29,				\
	  /* 2^28 */ 2.68435456e+08,					\
	  /* 2^4 */ 1.600000e+01,					\
	  /* 2^-20 */ 9.5367431640625e-07,				\
	  /* 2^-44 */ 5.6843418860808015e-14,				\
	  /* 2^-68 */ 3.3881317890172014e-21,				\
	  /* 2^-92 */ 2.0194839173657902e-28,				\
	  /* 2^-116 */ 1.2037062152420224e-35				\
	};								\
      double _a240, _b240, _c240, _d240, _e240, _f240,			\
	_g240, _h240, _i240, _j240, _k240;				\
      union { double d; UDItype i; } _l240, _m240, _n240, _o240,	\
				       _p240, _q240, _r240, _s240;	\
      UDItype _t240, _u240, _v240, _w240, _x240, _y240 = 0;		\
									\
      _FP_STATIC_ASSERT ((wfracbits) >= 106 && (wfracbits) <= 120,	\
			 "wfracbits out of range");			\
									\
      setfetz;								\
									\
      _e240 = (double) (long) (X##_f0 & 0xffffff);			\
      _j240 = (double) (long) (Y##_f0 & 0xffffff);			\
      _d240 = (double) (long) ((X##_f0 >> 24) & 0xffffff);		\
      _i240 = (double) (long) ((Y##_f0 >> 24) & 0xffffff);		\
      _c240 = (double) (long) (((X##_f1 << 16) & 0xffffff) | (X##_f0 >> 48)); \
      _h240 = (double) (long) (((Y##_f1 << 16) & 0xffffff) | (Y##_f0 >> 48)); \
      _b240 = (double) (long) ((X##_f1 >> 8) & 0xffffff);		\
      _g240 = (double) (long) ((Y##_f1 >> 8) & 0xffffff);		\
      _a240 = (double) (long) (X##_f1 >> 32);				\
      _f240 = (double) (long) (Y##_f1 >> 32);				\
      _e240 *= _const[3];						\
      _j240 *= _const[3];						\
      _d240 *= _const[2];						\
      _i240 *= _const[2];						\
      _c240 *= _const[1];						\
      _h240 *= _const[1];						\
      _b240 *= _const[0];						\
      _g240 *= _const[0];						\
      _s240.d =							      _e240*_j240; \
      _r240.d =						_d240*_j240 + _e240*_i240; \
      _q240.d =				  _c240*_j240 + _d240*_i240 + _e240*_h240; \
      _p240.d =		    _b240*_j240 + _c240*_i240 + _d240*_h240 + _e240*_g240; \
      _o240.d = _a240*_j240 + _b240*_i240 + _c240*_h240 + _d240*_g240 + _e240*_f240; \
      _n240.d = _a240*_i240 + _b240*_h240 + _c240*_g240 + _d240*_f240;	\
      _m240.d = _a240*_h240 + _b240*_g240 + _c240*_f240;		\
      _l240.d = _a240*_g240 + _b240*_f240;				\
      _k240 =   _a240*_f240;						\
      _r240.d += _s240.d;						\
      _q240.d += _r240.d;						\
      _p240.d += _q240.d;						\
      _o240.d += _p240.d;						\
      _n240.d += _o240.d;						\
      _m240.d += _n240.d;						\
      _l240.d += _m240.d;						\
      _k240 += _l240.d;							\
      _s240.d -= ((_const[10]+_s240.d)-_const[10]);			\
      _r240.d -= ((_const[9]+_r240.d)-_const[9]);			\
      _q240.d -= ((_const[8]+_q240.d)-_const[8]);			\
      _p240.d -= ((_const[7]+_p240.d)-_const[7]);			\
      _o240.d += _const[7];						\
      _n240.d += _const[6];						\
      _m240.d += _const[5];						\
      _l240.d += _const[4];						\
      if (_s240.d != 0.0)						\
	_y240 = 1;							\
      if (_r240.d != 0.0)						\
	_y240 = 1;							\
      if (_q240.d != 0.0)						\
	_y240 = 1;							\
      if (_p240.d != 0.0)						\
	_y240 = 1;							\
      _t240 = (DItype) _k240;						\
      _u240 = _l240.i;							\
      _v240 = _m240.i;							\
      _w240 = _n240.i;							\
      _x240 = _o240.i;							\
      R##_f1 = ((_t240 << (128 - (wfracbits - 1)))			\
		| ((_u240 & 0xffffff) >> ((wfracbits - 1) - 104)));	\
      R##_f0 = (((_u240 & 0xffffff) << (168 - (wfracbits - 1)))		\
		| ((_v240 & 0xffffff) << (144 - (wfracbits - 1)))	\
		| ((_w240 & 0xffffff) << (120 - (wfracbits - 1)))	\
		| ((_x240 & 0xffffff) >> ((wfracbits - 1) - 96))	\
		| _y240);						\
      resetfe;								\
    }									\
  while (0)

/* Division algorithms: */

#define _FP_DIV_MEAT_2_udiv(fs, R, X, Y)				\
  do									\
    {									\
      _FP_W_TYPE _FP_DIV_MEAT_2_udiv_n_f2;				\
      _FP_W_TYPE _FP_DIV_MEAT_2_udiv_n_f1;				\
      _FP_W_TYPE _FP_DIV_MEAT_2_udiv_n_f0;				\
      _FP_W_TYPE _FP_DIV_MEAT_2_udiv_r_f1;				\
      _FP_W_TYPE _FP_DIV_MEAT_2_udiv_r_f0;				\
      _FP_W_TYPE _FP_DIV_MEAT_2_udiv_m_f1;				\
      _FP_W_TYPE _FP_DIV_MEAT_2_udiv_m_f0;				\
      if (_FP_FRAC_GE_2 (X, Y))						\
	{								\
	  _FP_DIV_MEAT_2_udiv_n_f2 = X##_f1 >> 1;			\
	  _FP_DIV_MEAT_2_udiv_n_f1					\
	    = X##_f1 << (_FP_W_TYPE_SIZE - 1) | X##_f0 >> 1;		\
	  _FP_DIV_MEAT_2_udiv_n_f0					\
	    = X##_f0 << (_FP_W_TYPE_SIZE - 1);				\
	}								\
      else								\
	{								\
	  R##_e--;							\
	  _FP_DIV_MEAT_2_udiv_n_f2 = X##_f1;				\
	  _FP_DIV_MEAT_2_udiv_n_f1 = X##_f0;				\
	  _FP_DIV_MEAT_2_udiv_n_f0 = 0;					\
	}								\
									\
      /* Normalize, i.e. make the most significant bit of the		\
	 denominator set.  */						\
      _FP_FRAC_SLL_2 (Y, _FP_WFRACXBITS_##fs);				\
									\
      udiv_qrnnd (R##_f1, _FP_DIV_MEAT_2_udiv_r_f1,			\
		  _FP_DIV_MEAT_2_udiv_n_f2, _FP_DIV_MEAT_2_udiv_n_f1,	\
		  Y##_f1);						\
      umul_ppmm (_FP_DIV_MEAT_2_udiv_m_f1, _FP_DIV_MEAT_2_udiv_m_f0,	\
		 R##_f1, Y##_f0);					\
      _FP_DIV_MEAT_2_udiv_r_f0 = _FP_DIV_MEAT_2_udiv_n_f0;		\
      if (_FP_FRAC_GT_2 (_FP_DIV_MEAT_2_udiv_m, _FP_DIV_MEAT_2_udiv_r))	\
	{								\
	  R##_f1--;							\
	  _FP_FRAC_ADD_2 (_FP_DIV_MEAT_2_udiv_r, Y,			\
			  _FP_DIV_MEAT_2_udiv_r);			\
	  if (_FP_FRAC_GE_2 (_FP_DIV_MEAT_2_udiv_r, Y)			\
	      && _FP_FRAC_GT_2 (_FP_DIV_MEAT_2_udiv_m,			\
				_FP_DIV_MEAT_2_udiv_r))			\
	    {								\
	      R##_f1--;							\
	      _FP_FRAC_ADD_2 (_FP_DIV_MEAT_2_udiv_r, Y,			\
			      _FP_DIV_MEAT_2_udiv_r);			\
	    }								\
	}								\
      _FP_FRAC_DEC_2 (_FP_DIV_MEAT_2_udiv_r, _FP_DIV_MEAT_2_udiv_m);	\
									\
      if (_FP_DIV_MEAT_2_udiv_r_f1 == Y##_f1)				\
	{								\
	  /* This is a special case, not an optimization		\
	     (_FP_DIV_MEAT_2_udiv_r/Y##_f1 would not fit into UWtype).	\
	     As _FP_DIV_MEAT_2_udiv_r is guaranteed to be < Y,		\
	     R##_f0 can be either (UWtype)-1 or (UWtype)-2.  But as we	\
	     know what kind of bits it is (sticky, guard, round),	\
	     we don't care.  We also don't care what the reminder is,	\
	     because the guard bit will be set anyway.  -jj */		\
	  R##_f0 = -1;							\
	}								\
      else								\
	{								\
	  udiv_qrnnd (R##_f0, _FP_DIV_MEAT_2_udiv_r_f1,			\
		      _FP_DIV_MEAT_2_udiv_r_f1,				\
		      _FP_DIV_MEAT_2_udiv_r_f0, Y##_f1);		\
	  umul_ppmm (_FP_DIV_MEAT_2_udiv_m_f1,				\
		     _FP_DIV_MEAT_2_udiv_m_f0, R##_f0, Y##_f0);		\
	  _FP_DIV_MEAT_2_udiv_r_f0 = 0;					\
	  if (_FP_FRAC_GT_2 (_FP_DIV_MEAT_2_udiv_m,			\
			     _FP_DIV_MEAT_2_udiv_r))			\
	    {								\
	      R##_f0--;							\
	      _FP_FRAC_ADD_2 (_FP_DIV_MEAT_2_udiv_r, Y,			\
			      _FP_DIV_MEAT_2_udiv_r);			\
	      if (_FP_FRAC_GE_2 (_FP_DIV_MEAT_2_udiv_r, Y)		\
		  && _FP_FRAC_GT_2 (_FP_DIV_MEAT_2_udiv_m,		\
				    _FP_DIV_MEAT_2_udiv_r))		\
		{							\
		  R##_f0--;						\
		  _FP_FRAC_ADD_2 (_FP_DIV_MEAT_2_udiv_r, Y,		\
				  _FP_DIV_MEAT_2_udiv_r);		\
		}							\
	    }								\
	  if (!_FP_FRAC_EQ_2 (_FP_DIV_MEAT_2_udiv_r,			\
			      _FP_DIV_MEAT_2_udiv_m))			\
	    R##_f0 |= _FP_WORK_STICKY;					\
	}								\
    }									\
  while (0)


/* Square root algorithms:
   We have just one right now, maybe Newton approximation
   should be added for those machines where division is fast.  */

#define _FP_SQRT_MEAT_2(R, S, T, X, q)				\
  do								\
    {								\
      while (q)							\
	{							\
	  T##_f1 = S##_f1 + (q);				\
	  if (T##_f1 <= X##_f1)					\
	    {							\
	      S##_f1 = T##_f1 + (q);				\
	      X##_f1 -= T##_f1;					\
	      R##_f1 += (q);					\
	    }							\
	  _FP_FRAC_SLL_2 (X, 1);				\
	  (q) >>= 1;						\
	}							\
      (q) = (_FP_W_TYPE) 1 << (_FP_W_TYPE_SIZE - 1);		\
      while ((q) != _FP_WORK_ROUND)				\
	{							\
	  T##_f0 = S##_f0 + (q);				\
	  T##_f1 = S##_f1;					\
	  if (T##_f1 < X##_f1					\
	      || (T##_f1 == X##_f1 && T##_f0 <= X##_f0))	\
	    {							\
	      S##_f0 = T##_f0 + (q);				\
	      S##_f1 += (T##_f0 > S##_f0);			\
	      _FP_FRAC_DEC_2 (X, T);				\
	      R##_f0 += (q);					\
	    }							\
	  _FP_FRAC_SLL_2 (X, 1);				\
	  (q) >>= 1;						\
	}							\
      if (X##_f0 | X##_f1)					\
	{							\
	  if (S##_f1 < X##_f1					\
	      || (S##_f1 == X##_f1 && S##_f0 < X##_f0))		\
	    R##_f0 |= _FP_WORK_ROUND;				\
	  R##_f0 |= _FP_WORK_STICKY;				\
	}							\
    }								\
  while (0)


/* Assembly/disassembly for converting to/from integral types.
   No shifting or overflow handled here.  */

#define _FP_FRAC_ASSEMBLE_2(r, X, rsize)	\
  (void) (((rsize) <= _FP_W_TYPE_SIZE)		\
	  ? ({ (r) = X##_f0; })			\
	  : ({					\
	      (r) = X##_f1;			\
	      (r) <<= _FP_W_TYPE_SIZE;		\
	      (r) += X##_f0;			\
	    }))

#define _FP_FRAC_DISASSEMBLE_2(X, r, rsize)	\
  do						\
    {						\
      X##_f0 = (r);				\
      X##_f1 = ((rsize) <= _FP_W_TYPE_SIZE	\
		? 0				\
		: (r) >> _FP_W_TYPE_SIZE);	\
    }						\
  while (0)

/* Convert FP values between word sizes.  */

#define _FP_FRAC_COPY_1_2(D, S)		(D##_f = S##_f0)

#define _FP_FRAC_COPY_2_1(D, S)		((D##_f0 = S##_f), (D##_f1 = 0))

#define _FP_FRAC_COPY_2_2(D, S)		_FP_FRAC_COPY_2 (D, S)


/******************************************libgcc op-2.h end******************************************/
/******************************************libgcc op-4.h start******************************************/
#define _FP_FRAC_DECL_4(X)	_FP_W_TYPE X##_f[4]
#define _FP_FRAC_COPY_4(D, S)			\
  (D##_f[0] = S##_f[0], D##_f[1] = S##_f[1],	\
   D##_f[2] = S##_f[2], D##_f[3] = S##_f[3])
#define _FP_FRAC_SET_4(X, I)	__FP_FRAC_SET_4 (X, I)
#define _FP_FRAC_HIGH_4(X)	(X##_f[3])
#define _FP_FRAC_LOW_4(X)	(X##_f[0])
#define _FP_FRAC_WORD_4(X, w)	(X##_f[w])

#define _FP_FRAC_SLL_4(X, N)						\
  do									\
    {									\
      _FP_I_TYPE _FP_FRAC_SLL_4_up, _FP_FRAC_SLL_4_down;		\
      _FP_I_TYPE _FP_FRAC_SLL_4_skip, _FP_FRAC_SLL_4_i;			\
      _FP_FRAC_SLL_4_skip = (N) / _FP_W_TYPE_SIZE;			\
      _FP_FRAC_SLL_4_up = (N) % _FP_W_TYPE_SIZE;			\
      _FP_FRAC_SLL_4_down = _FP_W_TYPE_SIZE - _FP_FRAC_SLL_4_up;	\
      if (!_FP_FRAC_SLL_4_up)						\
	for (_FP_FRAC_SLL_4_i = 3;					\
	     _FP_FRAC_SLL_4_i >= _FP_FRAC_SLL_4_skip;			\
	     --_FP_FRAC_SLL_4_i)					\
	  X##_f[_FP_FRAC_SLL_4_i]					\
	    = X##_f[_FP_FRAC_SLL_4_i-_FP_FRAC_SLL_4_skip];		\
      else								\
	{								\
	  for (_FP_FRAC_SLL_4_i = 3;					\
	       _FP_FRAC_SLL_4_i > _FP_FRAC_SLL_4_skip;			\
	       --_FP_FRAC_SLL_4_i)					\
	    X##_f[_FP_FRAC_SLL_4_i]					\
	      = ((X##_f[_FP_FRAC_SLL_4_i-_FP_FRAC_SLL_4_skip]		\
		  << _FP_FRAC_SLL_4_up)					\
		 | (X##_f[_FP_FRAC_SLL_4_i-_FP_FRAC_SLL_4_skip-1]	\
		    >> _FP_FRAC_SLL_4_down));				\
	  X##_f[_FP_FRAC_SLL_4_i--] = X##_f[0] << _FP_FRAC_SLL_4_up;	\
	}								\
      for (; _FP_FRAC_SLL_4_i >= 0; --_FP_FRAC_SLL_4_i)			\
	X##_f[_FP_FRAC_SLL_4_i] = 0;					\
    }									\
  while (0)

/* This one was broken too.  */
#define _FP_FRAC_SRL_4(X, N)						\
  do									\
    {									\
      _FP_I_TYPE _FP_FRAC_SRL_4_up, _FP_FRAC_SRL_4_down;		\
      _FP_I_TYPE _FP_FRAC_SRL_4_skip, _FP_FRAC_SRL_4_i;			\
      _FP_FRAC_SRL_4_skip = (N) / _FP_W_TYPE_SIZE;			\
      _FP_FRAC_SRL_4_down = (N) % _FP_W_TYPE_SIZE;			\
      _FP_FRAC_SRL_4_up = _FP_W_TYPE_SIZE - _FP_FRAC_SRL_4_down;	\
      if (!_FP_FRAC_SRL_4_down)						\
	for (_FP_FRAC_SRL_4_i = 0;					\
	     _FP_FRAC_SRL_4_i <= 3-_FP_FRAC_SRL_4_skip;			\
	     ++_FP_FRAC_SRL_4_i)					\
	  X##_f[_FP_FRAC_SRL_4_i]					\
	    = X##_f[_FP_FRAC_SRL_4_i+_FP_FRAC_SRL_4_skip];		\
      else								\
	{								\
	  for (_FP_FRAC_SRL_4_i = 0;					\
	       _FP_FRAC_SRL_4_i < 3-_FP_FRAC_SRL_4_skip;		\
	       ++_FP_FRAC_SRL_4_i)					\
	    X##_f[_FP_FRAC_SRL_4_i]					\
	      = ((X##_f[_FP_FRAC_SRL_4_i+_FP_FRAC_SRL_4_skip]		\
		  >> _FP_FRAC_SRL_4_down)				\
		 | (X##_f[_FP_FRAC_SRL_4_i+_FP_FRAC_SRL_4_skip+1]	\
		    << _FP_FRAC_SRL_4_up));				\
	  X##_f[_FP_FRAC_SRL_4_i++] = X##_f[3] >> _FP_FRAC_SRL_4_down;	\
	}								\
      for (; _FP_FRAC_SRL_4_i < 4; ++_FP_FRAC_SRL_4_i)			\
	X##_f[_FP_FRAC_SRL_4_i] = 0;					\
    }									\
  while (0)


/* Right shift with sticky-lsb.
   What this actually means is that we do a standard right-shift,
   but that if any of the bits that fall off the right hand side
   were one then we always set the LSbit.  */
#define _FP_FRAC_SRST_4(X, S, N, size)					\
  do									\
    {									\
      _FP_I_TYPE _FP_FRAC_SRST_4_up, _FP_FRAC_SRST_4_down;		\
      _FP_I_TYPE _FP_FRAC_SRST_4_skip, _FP_FRAC_SRST_4_i;		\
      _FP_W_TYPE _FP_FRAC_SRST_4_s;					\
      _FP_FRAC_SRST_4_skip = (N) / _FP_W_TYPE_SIZE;			\
      _FP_FRAC_SRST_4_down = (N) % _FP_W_TYPE_SIZE;			\
      _FP_FRAC_SRST_4_up = _FP_W_TYPE_SIZE - _FP_FRAC_SRST_4_down;	\
      for (_FP_FRAC_SRST_4_s = _FP_FRAC_SRST_4_i = 0;			\
	   _FP_FRAC_SRST_4_i < _FP_FRAC_SRST_4_skip;			\
	   ++_FP_FRAC_SRST_4_i)						\
	_FP_FRAC_SRST_4_s |= X##_f[_FP_FRAC_SRST_4_i];			\
      if (!_FP_FRAC_SRST_4_down)					\
	for (_FP_FRAC_SRST_4_i = 0;					\
	     _FP_FRAC_SRST_4_i <= 3-_FP_FRAC_SRST_4_skip;		\
	     ++_FP_FRAC_SRST_4_i)					\
	  X##_f[_FP_FRAC_SRST_4_i]					\
	    = X##_f[_FP_FRAC_SRST_4_i+_FP_FRAC_SRST_4_skip];		\
      else								\
	{								\
	  _FP_FRAC_SRST_4_s						\
	    |= X##_f[_FP_FRAC_SRST_4_i] << _FP_FRAC_SRST_4_up;		\
	  for (_FP_FRAC_SRST_4_i = 0;					\
	       _FP_FRAC_SRST_4_i < 3-_FP_FRAC_SRST_4_skip;		\
	       ++_FP_FRAC_SRST_4_i)					\
	    X##_f[_FP_FRAC_SRST_4_i]					\
	      = ((X##_f[_FP_FRAC_SRST_4_i+_FP_FRAC_SRST_4_skip]		\
		  >> _FP_FRAC_SRST_4_down)				\
		 | (X##_f[_FP_FRAC_SRST_4_i+_FP_FRAC_SRST_4_skip+1]	\
		    << _FP_FRAC_SRST_4_up));				\
	  X##_f[_FP_FRAC_SRST_4_i++]					\
	    = X##_f[3] >> _FP_FRAC_SRST_4_down;				\
	}								\
      for (; _FP_FRAC_SRST_4_i < 4; ++_FP_FRAC_SRST_4_i)		\
	X##_f[_FP_FRAC_SRST_4_i] = 0;					\
      S = (_FP_FRAC_SRST_4_s != 0);					\
    }									\
  while (0)

#define _FP_FRAC_SRS_4(X, N, size)				\
  do								\
    {								\
      int _FP_FRAC_SRS_4_sticky;				\
      _FP_FRAC_SRST_4 (X, _FP_FRAC_SRS_4_sticky, (N), (size));	\
      X##_f[0] |= _FP_FRAC_SRS_4_sticky;			\
    }								\
  while (0)

#define _FP_FRAC_ADD_4(R, X, Y)					\
  __FP_FRAC_ADD_4 (R##_f[3], R##_f[2], R##_f[1], R##_f[0],	\
		   X##_f[3], X##_f[2], X##_f[1], X##_f[0],	\
		   Y##_f[3], Y##_f[2], Y##_f[1], Y##_f[0])

#define _FP_FRAC_SUB_4(R, X, Y)					\
  __FP_FRAC_SUB_4 (R##_f[3], R##_f[2], R##_f[1], R##_f[0],	\
		   X##_f[3], X##_f[2], X##_f[1], X##_f[0],	\
		   Y##_f[3], Y##_f[2], Y##_f[1], Y##_f[0])

#define _FP_FRAC_DEC_4(X, Y)					\
  __FP_FRAC_DEC_4 (X##_f[3], X##_f[2], X##_f[1], X##_f[0],	\
		   Y##_f[3], Y##_f[2], Y##_f[1], Y##_f[0])

#define _FP_FRAC_ADDI_4(X, I)					\
  __FP_FRAC_ADDI_4 (X##_f[3], X##_f[2], X##_f[1], X##_f[0], I)

#define _FP_ZEROFRAC_4  0, 0, 0, 0
#define _FP_MINFRAC_4   0, 0, 0, 1
#define _FP_MAXFRAC_4	(~(_FP_WS_TYPE) 0), (~(_FP_WS_TYPE) 0), (~(_FP_WS_TYPE) 0), (~(_FP_WS_TYPE) 0)

#define _FP_FRAC_ZEROP_4(X)     ((X##_f[0] | X##_f[1] | X##_f[2] | X##_f[3]) == 0)
#define _FP_FRAC_NEGP_4(X)      ((_FP_WS_TYPE) X##_f[3] < 0)
#define _FP_FRAC_OVERP_4(fs, X)  (_FP_FRAC_HIGH_##fs (X) & _FP_OVERFLOW_##fs)
#define _FP_FRAC_HIGHBIT_DW_4(fs, X)	\
  (_FP_FRAC_HIGH_DW_##fs (X) & _FP_HIGHBIT_DW_##fs)
#define _FP_FRAC_CLEAR_OVERP_4(fs, X)  (_FP_FRAC_HIGH_##fs (X) &= ~_FP_OVERFLOW_##fs)

#define _FP_FRAC_EQ_4(X, Y)				\
  (X##_f[0] == Y##_f[0] && X##_f[1] == Y##_f[1]		\
   && X##_f[2] == Y##_f[2] && X##_f[3] == Y##_f[3])

#define _FP_FRAC_GT_4(X, Y)				\
  (X##_f[3] > Y##_f[3]					\
   || (X##_f[3] == Y##_f[3]				\
       && (X##_f[2] > Y##_f[2]				\
	   || (X##_f[2] == Y##_f[2]			\
	       && (X##_f[1] > Y##_f[1]			\
		   || (X##_f[1] == Y##_f[1]		\
		       && X##_f[0] > Y##_f[0]))))))

#define _FP_FRAC_GE_4(X, Y)				\
  (X##_f[3] > Y##_f[3]					\
   || (X##_f[3] == Y##_f[3]				\
       && (X##_f[2] > Y##_f[2]				\
	   || (X##_f[2] == Y##_f[2]			\
	       && (X##_f[1] > Y##_f[1]			\
		   || (X##_f[1] == Y##_f[1]		\
		       && X##_f[0] >= Y##_f[0]))))))


#define _FP_FRAC_CLZ_4(R, X)			\
  do						\
    {						\
      if (X##_f[3])				\
	__FP_CLZ ((R), X##_f[3]);		\
      else if (X##_f[2])			\
	{					\
	  __FP_CLZ ((R), X##_f[2]);		\
	  (R) += _FP_W_TYPE_SIZE;		\
	}					\
      else if (X##_f[1])			\
	{					\
	  __FP_CLZ ((R), X##_f[1]);		\
	  (R) += _FP_W_TYPE_SIZE*2;		\
	}					\
      else					\
	{					\
	  __FP_CLZ ((R), X##_f[0]);		\
	  (R) += _FP_W_TYPE_SIZE*3;		\
	}					\
    }						\
  while (0)


#define _FP_UNPACK_RAW_4(fs, X, val)			\
  do							\
    {							\
      union _FP_UNION_##fs _FP_UNPACK_RAW_4_flo;	\
      _FP_UNPACK_RAW_4_flo.flt = (val);			\
      X##_f[0] = _FP_UNPACK_RAW_4_flo.bits.frac0;	\
      X##_f[1] = _FP_UNPACK_RAW_4_flo.bits.frac1;	\
      X##_f[2] = _FP_UNPACK_RAW_4_flo.bits.frac2;	\
      X##_f[3] = _FP_UNPACK_RAW_4_flo.bits.frac3;	\
      X##_e  = _FP_UNPACK_RAW_4_flo.bits.exp;		\
      X##_s  = _FP_UNPACK_RAW_4_flo.bits.sign;		\
    }							\
  while (0)

#define _FP_UNPACK_RAW_4_P(fs, X, val)			\
  do							\
    {							\
      union _FP_UNION_##fs *_FP_UNPACK_RAW_4_P_flo	\
	= (union _FP_UNION_##fs *) (val);		\
							\
      X##_f[0] = _FP_UNPACK_RAW_4_P_flo->bits.frac0;	\
      X##_f[1] = _FP_UNPACK_RAW_4_P_flo->bits.frac1;	\
      X##_f[2] = _FP_UNPACK_RAW_4_P_flo->bits.frac2;	\
      X##_f[3] = _FP_UNPACK_RAW_4_P_flo->bits.frac3;	\
      X##_e  = _FP_UNPACK_RAW_4_P_flo->bits.exp;	\
      X##_s  = _FP_UNPACK_RAW_4_P_flo->bits.sign;	\
    }							\
  while (0)

#define _FP_PACK_RAW_4(fs, val, X)		\
  do						\
    {						\
      union _FP_UNION_##fs _FP_PACK_RAW_4_flo;	\
      _FP_PACK_RAW_4_flo.bits.frac0 = X##_f[0];	\
      _FP_PACK_RAW_4_flo.bits.frac1 = X##_f[1];	\
      _FP_PACK_RAW_4_flo.bits.frac2 = X##_f[2];	\
      _FP_PACK_RAW_4_flo.bits.frac3 = X##_f[3];	\
      _FP_PACK_RAW_4_flo.bits.exp   = X##_e;	\
      _FP_PACK_RAW_4_flo.bits.sign  = X##_s;	\
      (val) = _FP_PACK_RAW_4_flo.flt;		\
    }						\
  while (0)

#define _FP_PACK_RAW_4_P(fs, val, X)			\
  do							\
    {							\
      union _FP_UNION_##fs *_FP_PACK_RAW_4_P_flo	\
	= (union _FP_UNION_##fs *) (val);		\
							\
      _FP_PACK_RAW_4_P_flo->bits.frac0 = X##_f[0];	\
      _FP_PACK_RAW_4_P_flo->bits.frac1 = X##_f[1];	\
      _FP_PACK_RAW_4_P_flo->bits.frac2 = X##_f[2];	\
      _FP_PACK_RAW_4_P_flo->bits.frac3 = X##_f[3];	\
      _FP_PACK_RAW_4_P_flo->bits.exp   = X##_e;		\
      _FP_PACK_RAW_4_P_flo->bits.sign  = X##_s;		\
    }							\
  while (0)

/* Multiplication algorithms: */

/* Given a 1W * 1W => 2W primitive, do the extended multiplication.  */

#define _FP_MUL_MEAT_DW_4_wide(wfracbits, R, X, Y, doit)		\
  do									\
    {									\
      _FP_FRAC_DECL_2 (_FP_MUL_MEAT_DW_4_wide_b);			\
      _FP_FRAC_DECL_2 (_FP_MUL_MEAT_DW_4_wide_c);			\
      _FP_FRAC_DECL_2 (_FP_MUL_MEAT_DW_4_wide_d);			\
      _FP_FRAC_DECL_2 (_FP_MUL_MEAT_DW_4_wide_e);			\
      _FP_FRAC_DECL_2 (_FP_MUL_MEAT_DW_4_wide_f);			\
									\
      doit (_FP_FRAC_WORD_8 (R, 1), _FP_FRAC_WORD_8 (R, 0),		\
	    X##_f[0], Y##_f[0]);					\
      doit (_FP_MUL_MEAT_DW_4_wide_b_f1, _FP_MUL_MEAT_DW_4_wide_b_f0,	\
	    X##_f[0], Y##_f[1]);					\
      doit (_FP_MUL_MEAT_DW_4_wide_c_f1, _FP_MUL_MEAT_DW_4_wide_c_f0,	\
	    X##_f[1], Y##_f[0]);					\
      doit (_FP_MUL_MEAT_DW_4_wide_d_f1, _FP_MUL_MEAT_DW_4_wide_d_f0,	\
	    X##_f[1], Y##_f[1]);					\
      doit (_FP_MUL_MEAT_DW_4_wide_e_f1, _FP_MUL_MEAT_DW_4_wide_e_f0,	\
	    X##_f[0], Y##_f[2]);					\
      doit (_FP_MUL_MEAT_DW_4_wide_f_f1, _FP_MUL_MEAT_DW_4_wide_f_f0,	\
	    X##_f[2], Y##_f[0]);					\
      __FP_FRAC_ADD_3 (_FP_FRAC_WORD_8 (R, 3), _FP_FRAC_WORD_8 (R, 2),	\
		       _FP_FRAC_WORD_8 (R, 1), 0,			\
		       _FP_MUL_MEAT_DW_4_wide_b_f1,			\
		       _FP_MUL_MEAT_DW_4_wide_b_f0,			\
		       0, 0, _FP_FRAC_WORD_8 (R, 1));			\
      __FP_FRAC_ADD_3 (_FP_FRAC_WORD_8 (R, 3), _FP_FRAC_WORD_8 (R, 2),	\
		       _FP_FRAC_WORD_8 (R, 1), 0,			\
		       _FP_MUL_MEAT_DW_4_wide_c_f1,			\
		       _FP_MUL_MEAT_DW_4_wide_c_f0,			\
		       _FP_FRAC_WORD_8 (R, 3), _FP_FRAC_WORD_8 (R, 2),	\
		       _FP_FRAC_WORD_8 (R, 1));				\
      __FP_FRAC_ADD_3 (_FP_FRAC_WORD_8 (R, 4), _FP_FRAC_WORD_8 (R, 3),	\
		       _FP_FRAC_WORD_8 (R, 2), 0,			\
		       _FP_MUL_MEAT_DW_4_wide_d_f1,			\
		       _FP_MUL_MEAT_DW_4_wide_d_f0,			\
		       0, _FP_FRAC_WORD_8 (R, 3), _FP_FRAC_WORD_8 (R, 2)); \
      __FP_FRAC_ADD_3 (_FP_FRAC_WORD_8 (R, 4), _FP_FRAC_WORD_8 (R, 3),	\
		       _FP_FRAC_WORD_8 (R, 2), 0,			\
		       _FP_MUL_MEAT_DW_4_wide_e_f1,			\
		       _FP_MUL_MEAT_DW_4_wide_e_f0,			\
		       _FP_FRAC_WORD_8 (R, 4), _FP_FRAC_WORD_8 (R, 3),	\
		       _FP_FRAC_WORD_8 (R, 2));				\
      __FP_FRAC_ADD_3 (_FP_FRAC_WORD_8 (R, 4), _FP_FRAC_WORD_8 (R, 3),	\
		       _FP_FRAC_WORD_8 (R, 2), 0,			\
		       _FP_MUL_MEAT_DW_4_wide_f_f1,			\
		       _FP_MUL_MEAT_DW_4_wide_f_f0,			\
		       _FP_FRAC_WORD_8 (R, 4), _FP_FRAC_WORD_8 (R, 3),	\
		       _FP_FRAC_WORD_8 (R, 2));				\
      doit (_FP_MUL_MEAT_DW_4_wide_b_f1,				\
	    _FP_MUL_MEAT_DW_4_wide_b_f0, X##_f[0], Y##_f[3]);		\
      doit (_FP_MUL_MEAT_DW_4_wide_c_f1,				\
	    _FP_MUL_MEAT_DW_4_wide_c_f0, X##_f[3], Y##_f[0]);		\
      doit (_FP_MUL_MEAT_DW_4_wide_d_f1, _FP_MUL_MEAT_DW_4_wide_d_f0,	\
	    X##_f[1], Y##_f[2]);					\
      doit (_FP_MUL_MEAT_DW_4_wide_e_f1, _FP_MUL_MEAT_DW_4_wide_e_f0,	\
	    X##_f[2], Y##_f[1]);					\
      __FP_FRAC_ADD_3 (_FP_FRAC_WORD_8 (R, 5), _FP_FRAC_WORD_8 (R, 4),	\
		       _FP_FRAC_WORD_8 (R, 3), 0,			\
		       _FP_MUL_MEAT_DW_4_wide_b_f1,			\
		       _FP_MUL_MEAT_DW_4_wide_b_f0,			\
		       0, _FP_FRAC_WORD_8 (R, 4), _FP_FRAC_WORD_8 (R, 3)); \
      __FP_FRAC_ADD_3 (_FP_FRAC_WORD_8 (R, 5), _FP_FRAC_WORD_8 (R, 4),	\
		       _FP_FRAC_WORD_8 (R, 3), 0,			\
		       _FP_MUL_MEAT_DW_4_wide_c_f1,			\
		       _FP_MUL_MEAT_DW_4_wide_c_f0,			\
		       _FP_FRAC_WORD_8 (R, 5), _FP_FRAC_WORD_8 (R, 4),	\
		       _FP_FRAC_WORD_8 (R, 3));				\
      __FP_FRAC_ADD_3 (_FP_FRAC_WORD_8 (R, 5), _FP_FRAC_WORD_8 (R, 4),	\
		       _FP_FRAC_WORD_8 (R, 3), 0,			\
		       _FP_MUL_MEAT_DW_4_wide_d_f1,			\
		       _FP_MUL_MEAT_DW_4_wide_d_f0,			\
		       _FP_FRAC_WORD_8 (R, 5), _FP_FRAC_WORD_8 (R, 4),	\
		       _FP_FRAC_WORD_8 (R, 3));				\
      __FP_FRAC_ADD_3 (_FP_FRAC_WORD_8 (R, 5), _FP_FRAC_WORD_8 (R, 4),	\
		       _FP_FRAC_WORD_8 (R, 3), 0,			\
		       _FP_MUL_MEAT_DW_4_wide_e_f1,			\
		       _FP_MUL_MEAT_DW_4_wide_e_f0,			\
		       _FP_FRAC_WORD_8 (R, 5), _FP_FRAC_WORD_8 (R, 4),	\
		       _FP_FRAC_WORD_8 (R, 3));				\
      doit (_FP_MUL_MEAT_DW_4_wide_b_f1, _FP_MUL_MEAT_DW_4_wide_b_f0,	\
	    X##_f[2], Y##_f[2]);					\
      doit (_FP_MUL_MEAT_DW_4_wide_c_f1, _FP_MUL_MEAT_DW_4_wide_c_f0,	\
	    X##_f[1], Y##_f[3]);					\
      doit (_FP_MUL_MEAT_DW_4_wide_d_f1, _FP_MUL_MEAT_DW_4_wide_d_f0,	\
	    X##_f[3], Y##_f[1]);					\
      doit (_FP_MUL_MEAT_DW_4_wide_e_f1, _FP_MUL_MEAT_DW_4_wide_e_f0,	\
	    X##_f[2], Y##_f[3]);					\
      doit (_FP_MUL_MEAT_DW_4_wide_f_f1, _FP_MUL_MEAT_DW_4_wide_f_f0,	\
	    X##_f[3], Y##_f[2]);					\
      __FP_FRAC_ADD_3 (_FP_FRAC_WORD_8 (R, 6), _FP_FRAC_WORD_8 (R, 5),	\
		       _FP_FRAC_WORD_8 (R, 4), 0,			\
		       _FP_MUL_MEAT_DW_4_wide_b_f1,			\
		       _FP_MUL_MEAT_DW_4_wide_b_f0,			\
		       0, _FP_FRAC_WORD_8 (R, 5), _FP_FRAC_WORD_8 (R, 4)); \
      __FP_FRAC_ADD_3 (_FP_FRAC_WORD_8 (R, 6), _FP_FRAC_WORD_8 (R, 5),	\
		       _FP_FRAC_WORD_8 (R, 4), 0,			\
		       _FP_MUL_MEAT_DW_4_wide_c_f1,			\
		       _FP_MUL_MEAT_DW_4_wide_c_f0,			\
		       _FP_FRAC_WORD_8 (R, 6), _FP_FRAC_WORD_8 (R, 5),	\
		       _FP_FRAC_WORD_8 (R, 4));				\
      __FP_FRAC_ADD_3 (_FP_FRAC_WORD_8 (R, 6), _FP_FRAC_WORD_8 (R, 5),	\
		       _FP_FRAC_WORD_8 (R, 4), 0,			\
		       _FP_MUL_MEAT_DW_4_wide_d_f1,			\
		       _FP_MUL_MEAT_DW_4_wide_d_f0,			\
		       _FP_FRAC_WORD_8 (R, 6), _FP_FRAC_WORD_8 (R, 5),	\
		       _FP_FRAC_WORD_8 (R, 4));				\
      __FP_FRAC_ADD_3 (_FP_FRAC_WORD_8 (R, 7), _FP_FRAC_WORD_8 (R, 6),	\
		       _FP_FRAC_WORD_8 (R, 5), 0,			\
		       _FP_MUL_MEAT_DW_4_wide_e_f1,			\
		       _FP_MUL_MEAT_DW_4_wide_e_f0,			\
		       0, _FP_FRAC_WORD_8 (R, 6), _FP_FRAC_WORD_8 (R, 5)); \
      __FP_FRAC_ADD_3 (_FP_FRAC_WORD_8 (R, 7), _FP_FRAC_WORD_8 (R, 6),	\
		       _FP_FRAC_WORD_8 (R, 5), 0,			\
		       _FP_MUL_MEAT_DW_4_wide_f_f1,			\
		       _FP_MUL_MEAT_DW_4_wide_f_f0,			\
		       _FP_FRAC_WORD_8 (R, 7), _FP_FRAC_WORD_8 (R, 6),	\
		       _FP_FRAC_WORD_8 (R, 5));				\
      doit (_FP_MUL_MEAT_DW_4_wide_b_f1, _FP_MUL_MEAT_DW_4_wide_b_f0,	\
	    X##_f[3], Y##_f[3]);					\
      __FP_FRAC_ADD_2 (_FP_FRAC_WORD_8 (R, 7), _FP_FRAC_WORD_8 (R, 6),	\
		       _FP_MUL_MEAT_DW_4_wide_b_f1,			\
		       _FP_MUL_MEAT_DW_4_wide_b_f0,			\
		       _FP_FRAC_WORD_8 (R, 7), _FP_FRAC_WORD_8 (R, 6));	\
    }									\
  while (0)

#define _FP_MUL_MEAT_4_wide(wfracbits, R, X, Y, doit)			\
  do									\
    {									\
      _FP_FRAC_DECL_8 (_FP_MUL_MEAT_4_wide_z);				\
									\
      _FP_MUL_MEAT_DW_4_wide ((wfracbits), _FP_MUL_MEAT_4_wide_z,	\
			      X, Y, doit);				\
									\
      /* Normalize since we know where the msb of the multiplicands	\
	 were (bit B), we know that the msb of the of the product is	\
	 at either 2B or 2B-1.  */					\
      _FP_FRAC_SRS_8 (_FP_MUL_MEAT_4_wide_z, (wfracbits)-1,		\
		      2*(wfracbits));					\
      __FP_FRAC_SET_4 (R, _FP_FRAC_WORD_8 (_FP_MUL_MEAT_4_wide_z, 3),	\
		       _FP_FRAC_WORD_8 (_FP_MUL_MEAT_4_wide_z, 2),	\
		       _FP_FRAC_WORD_8 (_FP_MUL_MEAT_4_wide_z, 1),	\
		       _FP_FRAC_WORD_8 (_FP_MUL_MEAT_4_wide_z, 0));	\
    }									\
  while (0)

#define _FP_MUL_MEAT_DW_4_gmp(wfracbits, R, X, Y)	\
  do							\
    {							\
      mpn_mul_n (R##_f, _x_f, _y_f, 4);			\
    }							\
  while (0)

#define _FP_MUL_MEAT_4_gmp(wfracbits, R, X, Y)				\
  do									\
    {									\
      _FP_FRAC_DECL_8 (_FP_MUL_MEAT_4_gmp_z);				\
									\
      _FP_MUL_MEAT_DW_4_gmp ((wfracbits), _FP_MUL_MEAT_4_gmp_z, X, Y);	\
									\
      /* Normalize since we know where the msb of the multiplicands	\
	 were (bit B), we know that the msb of the of the product is	\
	 at either 2B or 2B-1.  */					\
      _FP_FRAC_SRS_8 (_FP_MUL_MEAT_4_gmp_z, (wfracbits)-1,		\
		      2*(wfracbits));					\
      __FP_FRAC_SET_4 (R, _FP_FRAC_WORD_8 (_FP_MUL_MEAT_4_gmp_z, 3),	\
		       _FP_FRAC_WORD_8 (_FP_MUL_MEAT_4_gmp_z, 2),	\
		       _FP_FRAC_WORD_8 (_FP_MUL_MEAT_4_gmp_z, 1),	\
		       _FP_FRAC_WORD_8 (_FP_MUL_MEAT_4_gmp_z, 0));	\
    }									\
  while (0)

/* Helper utility for _FP_DIV_MEAT_4_udiv:
 * pppp = m * nnn.  */
#define umul_ppppmnnn(p3, p2, p1, p0, m, n2, n1, n0)	\
  do							\
    {							\
      UWtype umul_ppppmnnn_t;				\
      umul_ppmm (p1, p0, m, n0);			\
      umul_ppmm (p2, umul_ppppmnnn_t, m, n1);		\
      __FP_FRAC_ADDI_2 (p2, p1, umul_ppppmnnn_t);	\
      umul_ppmm (p3, umul_ppppmnnn_t, m, n2);		\
      __FP_FRAC_ADDI_2 (p3, p2, umul_ppppmnnn_t);	\
    }							\
  while (0)

/* Division algorithms: */

#define _FP_DIV_MEAT_4_udiv(fs, R, X, Y)				\
  do									\
    {									\
      int _FP_DIV_MEAT_4_udiv_i;					\
      _FP_FRAC_DECL_4 (_FP_DIV_MEAT_4_udiv_n);				\
      _FP_FRAC_DECL_4 (_FP_DIV_MEAT_4_udiv_m);				\
      _FP_FRAC_SET_4 (_FP_DIV_MEAT_4_udiv_n, _FP_ZEROFRAC_4);		\
      if (_FP_FRAC_GE_4 (X, Y))						\
	{								\
	  _FP_DIV_MEAT_4_udiv_n_f[3]					\
	    = X##_f[0] << (_FP_W_TYPE_SIZE - 1);			\
	  _FP_FRAC_SRL_4 (X, 1);					\
	}								\
      else								\
	R##_e--;							\
									\
      /* Normalize, i.e. make the most significant bit of the		\
	 denominator set.  */						\
      _FP_FRAC_SLL_4 (Y, _FP_WFRACXBITS_##fs);				\
									\
      for (_FP_DIV_MEAT_4_udiv_i = 3; ; _FP_DIV_MEAT_4_udiv_i--)	\
	{								\
	  if (X##_f[3] == Y##_f[3])					\
	    {								\
	      /* This is a special case, not an optimization		\
		 (X##_f[3]/Y##_f[3] would not fit into UWtype).		\
		 As X## is guaranteed to be < Y,			\
		 R##_f[_FP_DIV_MEAT_4_udiv_i] can be either		\
		 (UWtype)-1 or (UWtype)-2.  */				\
	      R##_f[_FP_DIV_MEAT_4_udiv_i] = -1;			\
	      if (!_FP_DIV_MEAT_4_udiv_i)				\
		break;							\
	      __FP_FRAC_SUB_4 (X##_f[3], X##_f[2], X##_f[1], X##_f[0],	\
			       Y##_f[2], Y##_f[1], Y##_f[0], 0,		\
			       X##_f[2], X##_f[1], X##_f[0],		\
			       _FP_DIV_MEAT_4_udiv_n_f[_FP_DIV_MEAT_4_udiv_i]); \
	      _FP_FRAC_SUB_4 (X, Y, X);					\
	      if (X##_f[3] > Y##_f[3])					\
		{							\
		  R##_f[_FP_DIV_MEAT_4_udiv_i] = -2;			\
		  _FP_FRAC_ADD_4 (X, Y, X);				\
		}							\
	    }								\
	  else								\
	    {								\
	      udiv_qrnnd (R##_f[_FP_DIV_MEAT_4_udiv_i],			\
			  X##_f[3], X##_f[3], X##_f[2], Y##_f[3]);	\
	      umul_ppppmnnn (_FP_DIV_MEAT_4_udiv_m_f[3],		\
			     _FP_DIV_MEAT_4_udiv_m_f[2],		\
			     _FP_DIV_MEAT_4_udiv_m_f[1],		\
			     _FP_DIV_MEAT_4_udiv_m_f[0],		\
			     R##_f[_FP_DIV_MEAT_4_udiv_i],		\
			     Y##_f[2], Y##_f[1], Y##_f[0]);		\
	      X##_f[2] = X##_f[1];					\
	      X##_f[1] = X##_f[0];					\
	      X##_f[0]							\
		= _FP_DIV_MEAT_4_udiv_n_f[_FP_DIV_MEAT_4_udiv_i];	\
	      if (_FP_FRAC_GT_4 (_FP_DIV_MEAT_4_udiv_m, X))		\
		{							\
		  R##_f[_FP_DIV_MEAT_4_udiv_i]--;			\
		  _FP_FRAC_ADD_4 (X, Y, X);				\
		  if (_FP_FRAC_GE_4 (X, Y)				\
		      && _FP_FRAC_GT_4 (_FP_DIV_MEAT_4_udiv_m, X))	\
		    {							\
		      R##_f[_FP_DIV_MEAT_4_udiv_i]--;			\
		      _FP_FRAC_ADD_4 (X, Y, X);				\
		    }							\
		}							\
	      _FP_FRAC_DEC_4 (X, _FP_DIV_MEAT_4_udiv_m);		\
	      if (!_FP_DIV_MEAT_4_udiv_i)				\
		{							\
		  if (!_FP_FRAC_EQ_4 (X, _FP_DIV_MEAT_4_udiv_m))	\
		    R##_f[0] |= _FP_WORK_STICKY;			\
		  break;						\
		}							\
	    }								\
	}								\
    }									\
  while (0)


/* Square root algorithms:
   We have just one right now, maybe Newton approximation
   should be added for those machines where division is fast.  */

#define _FP_SQRT_MEAT_4(R, S, T, X, q)					\
  do									\
    {									\
      while (q)								\
	{								\
	  T##_f[3] = S##_f[3] + (q);					\
	  if (T##_f[3] <= X##_f[3])					\
	    {								\
	      S##_f[3] = T##_f[3] + (q);				\
	      X##_f[3] -= T##_f[3];					\
	      R##_f[3] += (q);						\
	    }								\
	  _FP_FRAC_SLL_4 (X, 1);					\
	  (q) >>= 1;							\
	}								\
      (q) = (_FP_W_TYPE) 1 << (_FP_W_TYPE_SIZE - 1);			\
      while (q)								\
	{								\
	  T##_f[2] = S##_f[2] + (q);					\
	  T##_f[3] = S##_f[3];						\
	  if (T##_f[3] < X##_f[3]					\
	      || (T##_f[3] == X##_f[3] && T##_f[2] <= X##_f[2]))	\
	    {								\
	      S##_f[2] = T##_f[2] + (q);				\
	      S##_f[3] += (T##_f[2] > S##_f[2]);			\
	      __FP_FRAC_DEC_2 (X##_f[3], X##_f[2],			\
			       T##_f[3], T##_f[2]);			\
	      R##_f[2] += (q);						\
	    }								\
	  _FP_FRAC_SLL_4 (X, 1);					\
	  (q) >>= 1;							\
	}								\
      (q) = (_FP_W_TYPE) 1 << (_FP_W_TYPE_SIZE - 1);			\
      while (q)								\
	{								\
	  T##_f[1] = S##_f[1] + (q);					\
	  T##_f[2] = S##_f[2];						\
	  T##_f[3] = S##_f[3];						\
	  if (T##_f[3] < X##_f[3]					\
	      || (T##_f[3] == X##_f[3]					\
		  && (T##_f[2] < X##_f[2]				\
		      || (T##_f[2] == X##_f[2]				\
			  && T##_f[1] <= X##_f[1]))))			\
	    {								\
	      S##_f[1] = T##_f[1] + (q);				\
	      S##_f[2] += (T##_f[1] > S##_f[1]);			\
	      S##_f[3] += (T##_f[2] > S##_f[2]);			\
	      __FP_FRAC_DEC_3 (X##_f[3], X##_f[2], X##_f[1],		\
			       T##_f[3], T##_f[2], T##_f[1]);		\
	      R##_f[1] += (q);						\
	    }								\
	  _FP_FRAC_SLL_4 (X, 1);					\
	  (q) >>= 1;							\
	}								\
      (q) = (_FP_W_TYPE) 1 << (_FP_W_TYPE_SIZE - 1);			\
      while ((q) != _FP_WORK_ROUND)					\
	{								\
	  T##_f[0] = S##_f[0] + (q);					\
	  T##_f[1] = S##_f[1];						\
	  T##_f[2] = S##_f[2];						\
	  T##_f[3] = S##_f[3];						\
	  if (_FP_FRAC_GE_4 (X, T))					\
	    {								\
	      S##_f[0] = T##_f[0] + (q);				\
	      S##_f[1] += (T##_f[0] > S##_f[0]);			\
	      S##_f[2] += (T##_f[1] > S##_f[1]);			\
	      S##_f[3] += (T##_f[2] > S##_f[2]);			\
	      _FP_FRAC_DEC_4 (X, T);					\
	      R##_f[0] += (q);						\
	    }								\
	  _FP_FRAC_SLL_4 (X, 1);					\
	  (q) >>= 1;							\
	}								\
      if (!_FP_FRAC_ZEROP_4 (X))					\
	{								\
	  if (_FP_FRAC_GT_4 (X, S))					\
	    R##_f[0] |= _FP_WORK_ROUND;					\
	  R##_f[0] |= _FP_WORK_STICKY;					\
	}								\
    }									\
  while (0)


/* Internals.  */

#define __FP_FRAC_SET_4(X, I3, I2, I1, I0)			\
  (X##_f[3] = I3, X##_f[2] = I2, X##_f[1] = I1, X##_f[0] = I0)

#ifndef __FP_FRAC_ADD_3
# define __FP_FRAC_ADD_3(r2, r1, r0, x2, x1, x0, y2, y1, y0)	\
  do								\
    {								\
      _FP_W_TYPE __FP_FRAC_ADD_3_c1, __FP_FRAC_ADD_3_c2;	\
      r0 = x0 + y0;						\
      __FP_FRAC_ADD_3_c1 = r0 < x0;				\
      r1 = x1 + y1;						\
      __FP_FRAC_ADD_3_c2 = r1 < x1;				\
      r1 += __FP_FRAC_ADD_3_c1;					\
      __FP_FRAC_ADD_3_c2 |= r1 < __FP_FRAC_ADD_3_c1;		\
      r2 = x2 + y2 + __FP_FRAC_ADD_3_c2;			\
    }								\
  while (0)
#endif

#ifndef __FP_FRAC_ADD_4
# define __FP_FRAC_ADD_4(r3, r2, r1, r0, x3, x2, x1, x0, y3, y2, y1, y0) \
  do									\
    {									\
      _FP_W_TYPE __FP_FRAC_ADD_4_c1, __FP_FRAC_ADD_4_c2;		\
      _FP_W_TYPE __FP_FRAC_ADD_4_c3;					\
      r0 = x0 + y0;							\
      __FP_FRAC_ADD_4_c1 = r0 < x0;					\
      r1 = x1 + y1;							\
      __FP_FRAC_ADD_4_c2 = r1 < x1;					\
      r1 += __FP_FRAC_ADD_4_c1;						\
      __FP_FRAC_ADD_4_c2 |= r1 < __FP_FRAC_ADD_4_c1;			\
      r2 = x2 + y2;							\
      __FP_FRAC_ADD_4_c3 = r2 < x2;					\
      r2 += __FP_FRAC_ADD_4_c2;						\
      __FP_FRAC_ADD_4_c3 |= r2 < __FP_FRAC_ADD_4_c2;			\
      r3 = x3 + y3 + __FP_FRAC_ADD_4_c3;				\
    }									\
  while (0)
#endif

#ifndef __FP_FRAC_SUB_3
# define __FP_FRAC_SUB_3(r2, r1, r0, x2, x1, x0, y2, y1, y0)    \
  do                                                            \
    {                                                           \
      _FP_W_TYPE __FP_FRAC_SUB_3_tmp[2];                        \
      _FP_W_TYPE __FP_FRAC_SUB_3_c1, __FP_FRAC_SUB_3_c2;        \
      __FP_FRAC_SUB_3_tmp[0] = x0 - y0;                         \
      __FP_FRAC_SUB_3_c1 = __FP_FRAC_SUB_3_tmp[0] > x0;         \
      __FP_FRAC_SUB_3_tmp[1] = x1 - y1;                         \
      __FP_FRAC_SUB_3_c2 = __FP_FRAC_SUB_3_tmp[1] > x1;         \
      __FP_FRAC_SUB_3_tmp[1] -= __FP_FRAC_SUB_3_c1;             \
      __FP_FRAC_SUB_3_c2 |= __FP_FRAC_SUB_3_c1 && (y1 == x1);   \
      r2 = x2 - y2 - __FP_FRAC_SUB_3_c2;                        \
      r1 = __FP_FRAC_SUB_3_tmp[1];                              \
      r0 = __FP_FRAC_SUB_3_tmp[0];                              \
    }                                                           \
  while (0)
#endif

#ifndef __FP_FRAC_SUB_4
# define __FP_FRAC_SUB_4(r3, r2, r1, r0, x3, x2, x1, x0, y3, y2, y1, y0) \
  do                                                                     \
    {                                                                    \
      _FP_W_TYPE __FP_FRAC_SUB_4_tmp[3];                                 \
      _FP_W_TYPE __FP_FRAC_SUB_4_c1, __FP_FRAC_SUB_4_c2;                 \
      _FP_W_TYPE __FP_FRAC_SUB_4_c3;                                     \
      __FP_FRAC_SUB_4_tmp[0] = x0 - y0;                                  \
      __FP_FRAC_SUB_4_c1 = __FP_FRAC_SUB_4_tmp[0] > x0;                  \
      __FP_FRAC_SUB_4_tmp[1] = x1 - y1;                                  \
      __FP_FRAC_SUB_4_c2 = __FP_FRAC_SUB_4_tmp[1] > x1;                  \
      __FP_FRAC_SUB_4_tmp[1] -= __FP_FRAC_SUB_4_c1;                      \
      __FP_FRAC_SUB_4_c2 |= __FP_FRAC_SUB_4_c1 && (y1 == x1);            \
      __FP_FRAC_SUB_4_tmp[2] = x2 - y2;                                  \
      __FP_FRAC_SUB_4_c3 = __FP_FRAC_SUB_4_tmp[2] > x2;                  \
      __FP_FRAC_SUB_4_tmp[2] -= __FP_FRAC_SUB_4_c2;                      \
      __FP_FRAC_SUB_4_c3 |= __FP_FRAC_SUB_4_c2 && (y2 == x2);            \
      r3 = x3 - y3 - __FP_FRAC_SUB_4_c3;                                 \
      r2 = __FP_FRAC_SUB_4_tmp[2];                                       \
      r1 = __FP_FRAC_SUB_4_tmp[1];                                       \
      r0 = __FP_FRAC_SUB_4_tmp[0];                                       \
    }                                                                    \
  while (0)
#endif

#ifndef __FP_FRAC_DEC_3
# define __FP_FRAC_DEC_3(x2, x1, x0, y2, y1, y0)		\
  do								\
    {								\
      UWtype __FP_FRAC_DEC_3_t0, __FP_FRAC_DEC_3_t1;		\
      UWtype __FP_FRAC_DEC_3_t2;				\
      __FP_FRAC_DEC_3_t0 = x0;					\
      __FP_FRAC_DEC_3_t1 = x1;					\
      __FP_FRAC_DEC_3_t2 = x2;					\
      __FP_FRAC_SUB_3 (x2, x1, x0, __FP_FRAC_DEC_3_t2,		\
		       __FP_FRAC_DEC_3_t1, __FP_FRAC_DEC_3_t0,	\
		       y2, y1, y0);				\
    }								\
  while (0)
#endif

#ifndef __FP_FRAC_DEC_4
# define __FP_FRAC_DEC_4(x3, x2, x1, x0, y3, y2, y1, y0)	\
  do								\
    {								\
      UWtype __FP_FRAC_DEC_4_t0, __FP_FRAC_DEC_4_t1;		\
      UWtype __FP_FRAC_DEC_4_t2, __FP_FRAC_DEC_4_t3;		\
      __FP_FRAC_DEC_4_t0 = x0;					\
      __FP_FRAC_DEC_4_t1 = x1;					\
      __FP_FRAC_DEC_4_t2 = x2;					\
      __FP_FRAC_DEC_4_t3 = x3;					\
      __FP_FRAC_SUB_4 (x3, x2, x1, x0, __FP_FRAC_DEC_4_t3,	\
		       __FP_FRAC_DEC_4_t2, __FP_FRAC_DEC_4_t1,	\
		       __FP_FRAC_DEC_4_t0, y3, y2, y1, y0);	\
    }								\
  while (0)
#endif

#ifndef __FP_FRAC_ADDI_4
# define __FP_FRAC_ADDI_4(x3, x2, x1, x0, i)		\
  do							\
    {							\
      UWtype __FP_FRAC_ADDI_4_t;			\
      __FP_FRAC_ADDI_4_t = ((x0 += i) < i);		\
      x1 += __FP_FRAC_ADDI_4_t;				\
      __FP_FRAC_ADDI_4_t = (x1 < __FP_FRAC_ADDI_4_t);	\
      x2 += __FP_FRAC_ADDI_4_t;				\
      __FP_FRAC_ADDI_4_t = (x2 < __FP_FRAC_ADDI_4_t);	\
      x3 += __FP_FRAC_ADDI_4_t;				\
    }							\
  while (0)
#endif

/* Convert FP values between word sizes. This appears to be more
   complicated than I'd have expected it to be, so these might be
   wrong... These macros are in any case somewhat bogus because they
   use information about what various FRAC_n variables look like
   internally [eg, that 2 word vars are X_f0 and x_f1]. But so do
   the ones in op-2.h and op-1.h.  */
#define _FP_FRAC_COPY_1_4(D, S)		(D##_f = S##_f[0])

#define _FP_FRAC_COPY_2_4(D, S)			\
  do						\
    {						\
      D##_f0 = S##_f[0];			\
      D##_f1 = S##_f[1];			\
    }						\
  while (0)

/* Assembly/disassembly for converting to/from integral types.
   No shifting or overflow handled here.  */
/* Put the FP value X into r, which is an integer of size rsize.  */
#define _FP_FRAC_ASSEMBLE_4(r, X, rsize)				\
  do									\
    {									\
      if ((rsize) <= _FP_W_TYPE_SIZE)					\
	(r) = X##_f[0];							\
	else if ((rsize) <= 2*_FP_W_TYPE_SIZE)				\
	{								\
	  (r) = X##_f[1];						\
	  (r) = ((rsize) <= _FP_W_TYPE_SIZE				\
		 ? 0							\
		 : (r) << _FP_W_TYPE_SIZE);				\
	  (r) += X##_f[0];						\
	}								\
      else								\
	{								\
	  /* I'm feeling lazy so we deal with int == 3words		\
	     (implausible) and int == 4words as a single case.  */	\
	  (r) = X##_f[3];						\
	  (r) = ((rsize) <= _FP_W_TYPE_SIZE				\
		 ? 0							\
		 : (r) << _FP_W_TYPE_SIZE);				\
	  (r) += X##_f[2];						\
	  (r) = ((rsize) <= _FP_W_TYPE_SIZE				\
		 ? 0							\
		 : (r) << _FP_W_TYPE_SIZE);				\
	  (r) += X##_f[1];						\
	  (r) = ((rsize) <= _FP_W_TYPE_SIZE				\
		 ? 0							\
		 : (r) << _FP_W_TYPE_SIZE);				\
	  (r) += X##_f[0];						\
	}								\
    }									\
  while (0)

/* "No disassemble Number Five!" */
/* Move an integer of size rsize into X's fractional part. We rely on
   the _f[] array consisting of words of size _FP_W_TYPE_SIZE to avoid
   having to mask the values we store into it.  */
#define _FP_FRAC_DISASSEMBLE_4(X, r, rsize)	\
  do						\
    {						\
      X##_f[0] = (r);				\
      X##_f[1] = ((rsize) <= _FP_W_TYPE_SIZE	\
		  ? 0				\
		  : (r) >> _FP_W_TYPE_SIZE);	\
      X##_f[2] = ((rsize) <= 2*_FP_W_TYPE_SIZE	\
		  ? 0				\
		  : (r) >> 2*_FP_W_TYPE_SIZE);	\
      X##_f[3] = ((rsize) <= 3*_FP_W_TYPE_SIZE	\
		  ? 0				\
		  : (r) >> 3*_FP_W_TYPE_SIZE);	\
    }						\
  while (0)

#define _FP_FRAC_COPY_4_1(D, S)			\
  do						\
    {						\
      D##_f[0] = S##_f;				\
      D##_f[1] = D##_f[2] = D##_f[3] = 0;	\
    }						\
  while (0)

#define _FP_FRAC_COPY_4_2(D, S)			\
  do						\
    {						\
      D##_f[0] = S##_f0;			\
      D##_f[1] = S##_f1;			\
      D##_f[2] = D##_f[3] = 0;			\
    }						\
  while (0)

#define _FP_FRAC_COPY_4_4(D, S)	_FP_FRAC_COPY_4 (D, S)

/******************************************libgcc op-4.h end******************************************/
/******************************************libgcc op-8.h start******************************************/
/* We need just a few things from here for op-4, if we ever need some
   other macros, they can be added.  */
#define _FP_FRAC_DECL_8(X)	_FP_W_TYPE X##_f[8]
#define _FP_FRAC_SET_8(X, I)    __FP_FRAC_SET_8 (X, I)
#define _FP_FRAC_HIGH_8(X)	(X##_f[7])
#define _FP_FRAC_LOW_8(X)	(X##_f[0])
#define _FP_FRAC_WORD_8(X, w)	(X##_f[w])

#define _FP_FRAC_SLL_8(X, N)						\
  do									\
    {									\
      _FP_I_TYPE _FP_FRAC_SLL_8_up, _FP_FRAC_SLL_8_down;		\
      _FP_I_TYPE _FP_FRAC_SLL_8_skip, _FP_FRAC_SLL_8_i;			\
      _FP_FRAC_SLL_8_skip = (N) / _FP_W_TYPE_SIZE;			\
      _FP_FRAC_SLL_8_up = (N) % _FP_W_TYPE_SIZE;			\
      _FP_FRAC_SLL_8_down = _FP_W_TYPE_SIZE - _FP_FRAC_SLL_8_up;	\
      if (!_FP_FRAC_SLL_8_up)						\
	for (_FP_FRAC_SLL_8_i = 7;					\
	     _FP_FRAC_SLL_8_i >= _FP_FRAC_SLL_8_skip;			\
	     --_FP_FRAC_SLL_8_i)					\
	  X##_f[_FP_FRAC_SLL_8_i]					\
	    = X##_f[_FP_FRAC_SLL_8_i-_FP_FRAC_SLL_8_skip];		\
      else								\
	{								\
	  for (_FP_FRAC_SLL_8_i = 7;					\
	       _FP_FRAC_SLL_8_i > _FP_FRAC_SLL_8_skip;			\
	       --_FP_FRAC_SLL_8_i)					\
	    X##_f[_FP_FRAC_SLL_8_i]					\
	      = ((X##_f[_FP_FRAC_SLL_8_i-_FP_FRAC_SLL_8_skip]		\
		  << _FP_FRAC_SLL_8_up)					\
		 | (X##_f[_FP_FRAC_SLL_8_i-_FP_FRAC_SLL_8_skip-1]	\
		    >> _FP_FRAC_SLL_8_down));				\
	  X##_f[_FP_FRAC_SLL_8_i--] = X##_f[0] << _FP_FRAC_SLL_8_up;	\
	}								\
      for (; _FP_FRAC_SLL_8_i >= 0; --_FP_FRAC_SLL_8_i)			\
	X##_f[_FP_FRAC_SLL_8_i] = 0;					\
    }									\
  while (0)

#define _FP_FRAC_SRL_8(X, N)						\
  do									\
    {									\
      _FP_I_TYPE _FP_FRAC_SRL_8_up, _FP_FRAC_SRL_8_down;		\
      _FP_I_TYPE _FP_FRAC_SRL_8_skip, _FP_FRAC_SRL_8_i;			\
      _FP_FRAC_SRL_8_skip = (N) / _FP_W_TYPE_SIZE;			\
      _FP_FRAC_SRL_8_down = (N) % _FP_W_TYPE_SIZE;			\
      _FP_FRAC_SRL_8_up = _FP_W_TYPE_SIZE - _FP_FRAC_SRL_8_down;	\
      if (!_FP_FRAC_SRL_8_down)						\
	for (_FP_FRAC_SRL_8_i = 0;					\
	     _FP_FRAC_SRL_8_i <= 7-_FP_FRAC_SRL_8_skip;			\
	     ++_FP_FRAC_SRL_8_i)					\
	  X##_f[_FP_FRAC_SRL_8_i]					\
	    = X##_f[_FP_FRAC_SRL_8_i+_FP_FRAC_SRL_8_skip];		\
      else								\
	{								\
	  for (_FP_FRAC_SRL_8_i = 0;					\
	       _FP_FRAC_SRL_8_i < 7-_FP_FRAC_SRL_8_skip;		\
	       ++_FP_FRAC_SRL_8_i)					\
	    X##_f[_FP_FRAC_SRL_8_i]					\
	      = ((X##_f[_FP_FRAC_SRL_8_i+_FP_FRAC_SRL_8_skip]		\
		  >> _FP_FRAC_SRL_8_down)				\
		 | (X##_f[_FP_FRAC_SRL_8_i+_FP_FRAC_SRL_8_skip+1]	\
		    << _FP_FRAC_SRL_8_up));				\
	  X##_f[_FP_FRAC_SRL_8_i++] = X##_f[7] >> _FP_FRAC_SRL_8_down;	\
	}								\
      for (; _FP_FRAC_SRL_8_i < 8; ++_FP_FRAC_SRL_8_i)			\
	X##_f[_FP_FRAC_SRL_8_i] = 0;					\
    }									\
  while (0)


/* Right shift with sticky-lsb.
   What this actually means is that we do a standard right-shift,
   but that if any of the bits that fall off the right hand side
   were one then we always set the LSbit.  */
#define _FP_FRAC_SRS_8(X, N, size)					\
  do									\
    {									\
      _FP_I_TYPE _FP_FRAC_SRS_8_up, _FP_FRAC_SRS_8_down;		\
      _FP_I_TYPE _FP_FRAC_SRS_8_skip, _FP_FRAC_SRS_8_i;			\
      _FP_W_TYPE _FP_FRAC_SRS_8_s;					\
      _FP_FRAC_SRS_8_skip = (N) / _FP_W_TYPE_SIZE;			\
      _FP_FRAC_SRS_8_down = (N) % _FP_W_TYPE_SIZE;			\
      _FP_FRAC_SRS_8_up = _FP_W_TYPE_SIZE - _FP_FRAC_SRS_8_down;	\
      for (_FP_FRAC_SRS_8_s = _FP_FRAC_SRS_8_i = 0;			\
	   _FP_FRAC_SRS_8_i < _FP_FRAC_SRS_8_skip;			\
	   ++_FP_FRAC_SRS_8_i)						\
	_FP_FRAC_SRS_8_s |= X##_f[_FP_FRAC_SRS_8_i];			\
      if (!_FP_FRAC_SRS_8_down)						\
	for (_FP_FRAC_SRS_8_i = 0;					\
	     _FP_FRAC_SRS_8_i <= 7-_FP_FRAC_SRS_8_skip;			\
	     ++_FP_FRAC_SRS_8_i)					\
	  X##_f[_FP_FRAC_SRS_8_i]					\
	    = X##_f[_FP_FRAC_SRS_8_i+_FP_FRAC_SRS_8_skip];		\
      else								\
	{								\
	  _FP_FRAC_SRS_8_s						\
	    |= X##_f[_FP_FRAC_SRS_8_i] << _FP_FRAC_SRS_8_up;		\
	  for (_FP_FRAC_SRS_8_i = 0;					\
	       _FP_FRAC_SRS_8_i < 7-_FP_FRAC_SRS_8_skip;		\
	       ++_FP_FRAC_SRS_8_i)					\
	    X##_f[_FP_FRAC_SRS_8_i]					\
	      = ((X##_f[_FP_FRAC_SRS_8_i+_FP_FRAC_SRS_8_skip]		\
		  >> _FP_FRAC_SRS_8_down)				\
		 | (X##_f[_FP_FRAC_SRS_8_i+_FP_FRAC_SRS_8_skip+1]	\
		    << _FP_FRAC_SRS_8_up));				\
	  X##_f[_FP_FRAC_SRS_8_i++] = X##_f[7] >> _FP_FRAC_SRS_8_down;	\
	}								\
      for (; _FP_FRAC_SRS_8_i < 8; ++_FP_FRAC_SRS_8_i)			\
	X##_f[_FP_FRAC_SRS_8_i] = 0;					\
      /* Don't fix the LSB until the very end when we're sure f[0] is	\
	 stable.  */							\
      X##_f[0] |= (_FP_FRAC_SRS_8_s != 0);				\
    }									\
  while (0)

#define _FP_FRAC_ADD_8(R, X, Y)                                             \
  do                                                                        \
    {                                                                       \
      _FP_W_TYPE _FP_FRAC_ADD_8_c = 0;                                      \
      _FP_I_TYPE _FP_FRAC_ADD_8_i;                                          \
      for (_FP_FRAC_ADD_8_i = 0; _FP_FRAC_ADD_8_i < 8; ++_FP_FRAC_ADD_8_i)  \
        {                                                                   \
          R##_f[_FP_FRAC_ADD_8_i]                                           \
            = (X##_f[_FP_FRAC_ADD_8_i] + Y##_f[_FP_FRAC_ADD_8_i]            \
               + _FP_FRAC_ADD_8_c);                                         \
          _FP_FRAC_ADD_8_c                                                  \
            = (_FP_FRAC_ADD_8_c                                             \
               ? R##_f[_FP_FRAC_ADD_8_i] <= X##_f[_FP_FRAC_ADD_8_i]         \
               : R##_f[_FP_FRAC_ADD_8_i] < X##_f[_FP_FRAC_ADD_8_i]);        \
        }                                                                   \
    }                                                                       \
  while (0)

#define _FP_FRAC_SUB_8(R, X, Y)                                             \
  do                                                                        \
    {                                                                       \
      _FP_W_TYPE _FP_FRAC_SUB_8_tmp[8];                                     \
      _FP_W_TYPE _FP_FRAC_SUB_8_c = 0;                                      \
      _FP_I_TYPE _FP_FRAC_SUB_8_i;                                          \
      for (_FP_FRAC_SUB_8_i = 0; _FP_FRAC_SUB_8_i < 8; ++_FP_FRAC_SUB_8_i)  \
        {                                                                   \
          _FP_FRAC_SUB_8_tmp[_FP_FRAC_SUB_8_i]                              \
            = (X##_f[_FP_FRAC_SUB_8_i] - Y##_f[_FP_FRAC_SUB_8_i]            \
               - _FP_FRAC_SUB_8_c);                                         \
          _FP_FRAC_SUB_8_c                                                  \
            = (_FP_FRAC_SUB_8_c                                             \
               ? (_FP_FRAC_SUB_8_tmp[_FP_FRAC_SUB_8_i]                      \
                  >= X##_f[_FP_FRAC_SUB_8_i])                               \
               : (_FP_FRAC_SUB_8_tmp[_FP_FRAC_SUB_8_i]                      \
                  > X##_f[_FP_FRAC_SUB_8_i]));                              \
        }                                                                   \
      for (_FP_FRAC_SUB_8_i = 0; _FP_FRAC_SUB_8_i < 8; ++_FP_FRAC_SUB_8_i)  \
        R##_f[_FP_FRAC_SUB_8_i] = _FP_FRAC_SUB_8_tmp[_FP_FRAC_SUB_8_i];     \
    }                                                                       \
  while (0)

#define _FP_FRAC_CLZ_8(R, X)                                                \
  do                                                                        \
    {                                                                       \
      _FP_I_TYPE _FP_FRAC_CLZ_8_i;                                          \
      for (_FP_FRAC_CLZ_8_i = 7; _FP_FRAC_CLZ_8_i > 0; _FP_FRAC_CLZ_8_i--)  \
        if (X##_f[_FP_FRAC_CLZ_8_i])                                        \
          break;                                                            \
      __FP_CLZ ((R), X##_f[_FP_FRAC_CLZ_8_i]);                              \
      (R) += _FP_W_TYPE_SIZE * (7 - _FP_FRAC_CLZ_8_i);                      \
    }                                                                       \
  while (0)

#define _FP_MINFRAC_8   0, 0, 0, 0, 0, 0, 0, 1

#define _FP_FRAC_NEGP_8(X)      ((_FP_WS_TYPE) X##_f[7] < 0)
#define _FP_FRAC_ZEROP_8(X)                                             \
  ((X##_f[0] | X##_f[1] | X##_f[2] | X##_f[3]                           \
    | X##_f[4] | X##_f[5] | X##_f[6] | X##_f[7]) == 0)
#define _FP_FRAC_HIGHBIT_DW_8(fs, X)                                    \
  (_FP_FRAC_HIGH_DW_##fs (X) & _FP_HIGHBIT_DW_##fs)

#define _FP_FRAC_COPY_4_8(D, S)                           \
  do                                                      \
    {                                                     \
      D##_f[0] = S##_f[0];                                \
      D##_f[1] = S##_f[1];                                \
      D##_f[2] = S##_f[2];                                \
      D##_f[3] = S##_f[3];                                \
    }                                                     \
  while (0)

#define _FP_FRAC_COPY_8_4(D, S)                           \
  do                                                      \
    {                                                     \
      D##_f[0] = S##_f[0];                                \
      D##_f[1] = S##_f[1];                                \
      D##_f[2] = S##_f[2];                                \
      D##_f[3] = S##_f[3];                                \
      D##_f[4] = D##_f[5] = D##_f[6] = D##_f[7]= 0;       \
    }                                                     \
  while (0)

#define __FP_FRAC_SET_8(X, I7, I6, I5, I4, I3, I2, I1, I0)             \
  (X##_f[7] = I7, X##_f[6] = I6, X##_f[5] = I5, X##_f[4] = I4,         \
   X##_f[3] = I3, X##_f[2] = I2, X##_f[1] = I1, X##_f[0] = I0)

/******************************************libgcc op-8.h end******************************************/
/******************************************libgcc op-common.h start******************************************/
#define _FP_DECL(wc, X)						\
  _FP_I_TYPE X##_c __attribute__ ((unused)) _FP_ZERO_INIT;	\
  _FP_I_TYPE X##_s __attribute__ ((unused)) _FP_ZERO_INIT;	\
  _FP_I_TYPE X##_e __attribute__ ((unused)) _FP_ZERO_INIT;	\
  _FP_FRAC_DECL_##wc (X)

/* Test whether the qNaN bit denotes a signaling NaN.  */
#define _FP_FRAC_SNANP(fs, X)				\
  ((_FP_QNANNEGATEDP)					\
   ? (_FP_FRAC_HIGH_RAW_##fs (X) & _FP_QNANBIT_##fs)	\
   : !(_FP_FRAC_HIGH_RAW_##fs (X) & _FP_QNANBIT_##fs))
#define _FP_FRAC_SNANP_SEMIRAW(fs, X)			\
  ((_FP_QNANNEGATEDP)					\
   ? (_FP_FRAC_HIGH_##fs (X) & _FP_QNANBIT_SH_##fs)	\
   : !(_FP_FRAC_HIGH_##fs (X) & _FP_QNANBIT_SH_##fs))

/* Finish truly unpacking a native fp value by classifying the kind
   of fp value and normalizing both the exponent and the fraction.  */

#define _FP_UNPACK_CANONICAL(fs, wc, X)				\
  do								\
    {								\
      switch (X##_e)						\
	{							\
	default:						\
	  _FP_FRAC_HIGH_RAW_##fs (X) |= _FP_IMPLBIT_##fs;	\
	  _FP_FRAC_SLL_##wc (X, _FP_WORKBITS);			\
	  X##_e -= _FP_EXPBIAS_##fs;				\
	  X##_c = FP_CLS_NORMAL;				\
	  break;						\
								\
	case 0:							\
	  if (_FP_FRAC_ZEROP_##wc (X))				\
	    X##_c = FP_CLS_ZERO;				\
	  else if (FP_DENORM_ZERO)				\
	    {							\
	      X##_c = FP_CLS_ZERO;				\
	      _FP_FRAC_SET_##wc (X, _FP_ZEROFRAC_##wc);		\
	      FP_SET_EXCEPTION (FP_EX_DENORM);			\
	    }							\
	  else							\
	    {							\
	      /* A denormalized number.  */			\
	      _FP_I_TYPE _FP_UNPACK_CANONICAL_shift;		\
	      _FP_FRAC_CLZ_##wc (_FP_UNPACK_CANONICAL_shift,	\
				 X);				\
	      _FP_UNPACK_CANONICAL_shift -= _FP_FRACXBITS_##fs;	\
	      _FP_FRAC_SLL_##wc (X, (_FP_UNPACK_CANONICAL_shift \
				     + _FP_WORKBITS));		\
	      X##_e -= (_FP_EXPBIAS_##fs - 1			\
			+ _FP_UNPACK_CANONICAL_shift);		\
	      X##_c = FP_CLS_NORMAL;				\
	      FP_SET_EXCEPTION (FP_EX_DENORM);			\
	    }							\
	  break;						\
								\
	case _FP_EXPMAX_##fs:					\
	  if (_FP_FRAC_ZEROP_##wc (X))				\
	    X##_c = FP_CLS_INF;					\
	  else							\
	    {							\
	      X##_c = FP_CLS_NAN;				\
	      /* Check for signaling NaN.  */			\
	      if (_FP_FRAC_SNANP (fs, X))			\
		FP_SET_EXCEPTION (FP_EX_INVALID			\
				  | FP_EX_INVALID_SNAN);	\
	    }							\
	  break;						\
	}							\
    }								\
  while (0)

/* Finish unpacking an fp value in semi-raw mode: the mantissa is
   shifted by _FP_WORKBITS but the implicit MSB is not inserted and
   other classification is not done.  */
#define _FP_UNPACK_SEMIRAW(fs, wc, X)	_FP_FRAC_SLL_##wc (X, _FP_WORKBITS)

/* Check whether a raw or semi-raw input value should be flushed to
   zero, and flush it to zero if so.  */
#define _FP_CHECK_FLUSH_ZERO(fs, wc, X)			\
  do							\
    {							\
      if (FP_DENORM_ZERO				\
	  && X##_e == 0					\
	  && !_FP_FRAC_ZEROP_##wc (X))			\
	{						\
	  _FP_FRAC_SET_##wc (X, _FP_ZEROFRAC_##wc);	\
	  FP_SET_EXCEPTION (FP_EX_DENORM);		\
	}						\
    }							\
  while (0)

/* A semi-raw value has overflowed to infinity.  Adjust the mantissa
   and exponent appropriately.  */
#define _FP_OVERFLOW_SEMIRAW(fs, wc, X)			\
  do							\
    {							\
      if (FP_ROUNDMODE == FP_RND_NEAREST		\
	  || (FP_ROUNDMODE == FP_RND_PINF && !X##_s)	\
	  || (FP_ROUNDMODE == FP_RND_MINF && X##_s))	\
	{						\
	  X##_e = _FP_EXPMAX_##fs;			\
	  _FP_FRAC_SET_##wc (X, _FP_ZEROFRAC_##wc);	\
	}						\
      else						\
	{						\
	  X##_e = _FP_EXPMAX_##fs - 1;			\
	  _FP_FRAC_SET_##wc (X, _FP_MAXFRAC_##wc);	\
	}						\
      FP_SET_EXCEPTION (FP_EX_INEXACT);			\
      FP_SET_EXCEPTION (FP_EX_OVERFLOW);		\
    }							\
  while (0)

/* Check for a semi-raw value being a signaling NaN and raise the
   invalid exception if so.  */
#define _FP_CHECK_SIGNAN_SEMIRAW(fs, wc, X)			\
  do								\
    {								\
      if (X##_e == _FP_EXPMAX_##fs				\
	  && !_FP_FRAC_ZEROP_##wc (X)				\
	  && _FP_FRAC_SNANP_SEMIRAW (fs, X))			\
	FP_SET_EXCEPTION (FP_EX_INVALID | FP_EX_INVALID_SNAN);	\
    }								\
  while (0)

/* Choose a NaN result from an operation on two semi-raw NaN
   values.  */
#define _FP_CHOOSENAN_SEMIRAW(fs, wc, R, X, Y, OP)			\
  do									\
    {									\
      /* _FP_CHOOSENAN expects raw values, so shift as required.  */	\
      _FP_FRAC_SRL_##wc (X, _FP_WORKBITS);				\
      _FP_FRAC_SRL_##wc (Y, _FP_WORKBITS);				\
      _FP_CHOOSENAN (fs, wc, R, X, Y, OP);				\
      _FP_FRAC_SLL_##wc (R, _FP_WORKBITS);				\
    }									\
  while (0)

/* Make the fractional part a quiet NaN, preserving the payload
   if possible, otherwise make it the canonical quiet NaN and set
   the sign bit accordingly.  */
#define _FP_SETQNAN(fs, wc, X)					\
  do								\
    {								\
      if (_FP_QNANNEGATEDP)					\
	{							\
	  _FP_FRAC_HIGH_RAW_##fs (X) &= _FP_QNANBIT_##fs - 1;	\
	  if (_FP_FRAC_ZEROP_##wc (X))				\
	    {							\
	      X##_s = _FP_NANSIGN_##fs;				\
	      _FP_FRAC_SET_##wc (X, _FP_NANFRAC_##fs);		\
	    }							\
	}							\
      else							\
	_FP_FRAC_HIGH_RAW_##fs (X) |= _FP_QNANBIT_##fs;		\
    }								\
  while (0)
#define _FP_SETQNAN_SEMIRAW(fs, wc, X)				\
  do								\
    {								\
      if (_FP_QNANNEGATEDP)					\
	{							\
	  _FP_FRAC_HIGH_##fs (X) &= _FP_QNANBIT_SH_##fs - 1;	\
	  if (_FP_FRAC_ZEROP_##wc (X))				\
	    {							\
	      X##_s = _FP_NANSIGN_##fs;				\
	      _FP_FRAC_SET_##wc (X, _FP_NANFRAC_##fs);		\
	      _FP_FRAC_SLL_##wc (X, _FP_WORKBITS);		\
	    }							\
	}							\
      else							\
	_FP_FRAC_HIGH_##fs (X) |= _FP_QNANBIT_SH_##fs;		\
    }								\
  while (0)

/* Test whether a biased exponent is normal (not zero or maximum).  */
#define _FP_EXP_NORMAL(fs, wc, X)	(((X##_e + 1) & _FP_EXPMAX_##fs) > 1)

/* Prepare to pack an fp value in semi-raw mode: the mantissa is
   rounded and shifted right, with the rounding possibly increasing
   the exponent (including changing a finite value to infinity).  */
#define _FP_PACK_SEMIRAW(fs, wc, X)				\
  do								\
    {								\
      int _FP_PACK_SEMIRAW_is_tiny				\
	= X##_e == 0 && !_FP_FRAC_ZEROP_##wc (X);		\
      if (_FP_TININESS_AFTER_ROUNDING				\
	  && _FP_PACK_SEMIRAW_is_tiny)				\
	{							\
	  FP_DECL_##fs (_FP_PACK_SEMIRAW_T);			\
	  _FP_FRAC_COPY_##wc (_FP_PACK_SEMIRAW_T, X);		\
	  _FP_PACK_SEMIRAW_T##_s = X##_s;			\
	  _FP_PACK_SEMIRAW_T##_e = X##_e;			\
	  _FP_FRAC_SLL_##wc (_FP_PACK_SEMIRAW_T, 1);		\
	  _FP_ROUND (wc, _FP_PACK_SEMIRAW_T);			\
	  if (_FP_FRAC_OVERP_##wc (fs, _FP_PACK_SEMIRAW_T))	\
	    _FP_PACK_SEMIRAW_is_tiny = 0;			\
	}							\
      _FP_ROUND (wc, X);					\
      if (_FP_PACK_SEMIRAW_is_tiny)				\
	{							\
	  if ((FP_CUR_EXCEPTIONS & FP_EX_INEXACT)		\
	      || (FP_TRAPPING_EXCEPTIONS & FP_EX_UNDERFLOW))	\
	    FP_SET_EXCEPTION (FP_EX_UNDERFLOW);			\
	}							\
      if (_FP_FRAC_HIGH_##fs (X)				\
	  & (_FP_OVERFLOW_##fs >> 1))				\
	{							\
	  _FP_FRAC_HIGH_##fs (X) &= ~(_FP_OVERFLOW_##fs >> 1);	\
	  X##_e++;						\
	  if (X##_e == _FP_EXPMAX_##fs)				\
	    _FP_OVERFLOW_SEMIRAW (fs, wc, X);			\
	}							\
      _FP_FRAC_SRL_##wc (X, _FP_WORKBITS);			\
      if (X##_e == _FP_EXPMAX_##fs && !_FP_FRAC_ZEROP_##wc (X))	\
	{							\
	  if (!_FP_KEEPNANFRACP)				\
	    {							\
	      _FP_FRAC_SET_##wc (X, _FP_NANFRAC_##fs);		\
	      X##_s = _FP_NANSIGN_##fs;				\
	    }							\
	  else							\
	    _FP_SETQNAN (fs, wc, X);				\
	}							\
    }								\
  while (0)

/* Before packing the bits back into the native fp result, take care
   of such mundane things as rounding and overflow.  Also, for some
   kinds of fp values, the original parts may not have been fully
   extracted -- but that is ok, we can regenerate them now.  */

#define _FP_PACK_CANONICAL(fs, wc, X)					\
  do									\
    {									\
      switch (X##_c)							\
	{								\
	case FP_CLS_NORMAL:						\
	  X##_e += _FP_EXPBIAS_##fs;					\
	  if (X##_e > 0)						\
	    {								\
	      _FP_ROUND (wc, X);					\
	      if (_FP_FRAC_OVERP_##wc (fs, X))				\
		{							\
		  _FP_FRAC_CLEAR_OVERP_##wc (fs, X);			\
		  X##_e++;						\
		}							\
	      _FP_FRAC_SRL_##wc (X, _FP_WORKBITS);			\
	      if (X##_e >= _FP_EXPMAX_##fs)				\
		{							\
		  /* Overflow.  */					\
		  switch (FP_ROUNDMODE)					\
		    {							\
		    case FP_RND_NEAREST:				\
		      X##_c = FP_CLS_INF;				\
		      break;						\
		    case FP_RND_PINF:					\
		      if (!X##_s)					\
			X##_c = FP_CLS_INF;				\
		      break;						\
		    case FP_RND_MINF:					\
		      if (X##_s)					\
			X##_c = FP_CLS_INF;				\
		      break;						\
		    }							\
		  if (X##_c == FP_CLS_INF)				\
		    {							\
		      /* Overflow to infinity.  */			\
		      X##_e = _FP_EXPMAX_##fs;				\
		      _FP_FRAC_SET_##wc (X, _FP_ZEROFRAC_##wc);		\
		    }							\
		  else							\
		    {							\
		      /* Overflow to maximum normal.  */		\
		      X##_e = _FP_EXPMAX_##fs - 1;			\
		      _FP_FRAC_SET_##wc (X, _FP_MAXFRAC_##wc);		\
		    }							\
		  FP_SET_EXCEPTION (FP_EX_OVERFLOW);			\
		  FP_SET_EXCEPTION (FP_EX_INEXACT);			\
		}							\
	    }								\
	  else								\
	    {								\
	      /* We've got a denormalized number.  */			\
	      int _FP_PACK_CANONICAL_is_tiny = 1;			\
	      if (_FP_TININESS_AFTER_ROUNDING && X##_e == 0)		\
		{							\
		  FP_DECL_##fs (_FP_PACK_CANONICAL_T);			\
		  _FP_FRAC_COPY_##wc (_FP_PACK_CANONICAL_T, X);		\
		  _FP_PACK_CANONICAL_T##_s = X##_s;			\
		  _FP_PACK_CANONICAL_T##_e = X##_e;			\
		  _FP_ROUND (wc, _FP_PACK_CANONICAL_T);			\
		  if (_FP_FRAC_OVERP_##wc (fs, _FP_PACK_CANONICAL_T))	\
		    _FP_PACK_CANONICAL_is_tiny = 0;			\
		}							\
	      X##_e = -X##_e + 1;					\
	      if (X##_e <= _FP_WFRACBITS_##fs)				\
		{							\
		  _FP_FRAC_SRS_##wc (X, X##_e, _FP_WFRACBITS_##fs);	\
		  _FP_ROUND (wc, X);					\
		  if (_FP_FRAC_HIGH_##fs (X)				\
		      & (_FP_OVERFLOW_##fs >> 1))			\
		    {							\
		      X##_e = 1;					\
		      _FP_FRAC_SET_##wc (X, _FP_ZEROFRAC_##wc);		\
		      FP_SET_EXCEPTION (FP_EX_INEXACT);			\
		    }							\
		  else							\
		    {							\
		      X##_e = 0;					\
		      _FP_FRAC_SRL_##wc (X, _FP_WORKBITS);		\
		    }							\
		  if (_FP_PACK_CANONICAL_is_tiny			\
		      && ((FP_CUR_EXCEPTIONS & FP_EX_INEXACT)		\
			  || (FP_TRAPPING_EXCEPTIONS			\
			      & FP_EX_UNDERFLOW)))			\
		    FP_SET_EXCEPTION (FP_EX_UNDERFLOW);			\
		}							\
	      else							\
		{							\
		  /* Underflow to zero.  */				\
		  X##_e = 0;						\
		  if (!_FP_FRAC_ZEROP_##wc (X))				\
		    {							\
		      _FP_FRAC_SET_##wc (X, _FP_MINFRAC_##wc);		\
		      _FP_ROUND (wc, X);				\
		      _FP_FRAC_LOW_##wc (X) >>= (_FP_WORKBITS);		\
		    }							\
		  FP_SET_EXCEPTION (FP_EX_UNDERFLOW);			\
		}							\
	    }								\
	  break;							\
									\
	case FP_CLS_ZERO:						\
	  X##_e = 0;							\
	  _FP_FRAC_SET_##wc (X, _FP_ZEROFRAC_##wc);			\
	  break;							\
									\
	case FP_CLS_INF:						\
	  X##_e = _FP_EXPMAX_##fs;					\
	  _FP_FRAC_SET_##wc (X, _FP_ZEROFRAC_##wc);			\
	  break;							\
									\
	case FP_CLS_NAN:						\
	  X##_e = _FP_EXPMAX_##fs;					\
	  if (!_FP_KEEPNANFRACP)					\
	    {								\
	      _FP_FRAC_SET_##wc (X, _FP_NANFRAC_##fs);			\
	      X##_s = _FP_NANSIGN_##fs;					\
	    }								\
	  else								\
	    _FP_SETQNAN (fs, wc, X);					\
	  break;							\
	}								\
    }									\
  while (0)

/* This one accepts raw argument and not cooked,  returns
   1 if X is a signaling NaN.  */
#define _FP_ISSIGNAN(fs, wc, X)			\
  ({						\
    int _FP_ISSIGNAN_ret = 0;			\
    if (X##_e == _FP_EXPMAX_##fs)		\
      {						\
	if (!_FP_FRAC_ZEROP_##wc (X)		\
	    && _FP_FRAC_SNANP (fs, X))		\
	  _FP_ISSIGNAN_ret = 1;			\
      }						\
    _FP_ISSIGNAN_ret;				\
  })





/* Addition on semi-raw values.  */
#define _FP_ADD_INTERNAL(fs, wc, R, X, Y, OP)				\
  do									\
    {									\
      _FP_CHECK_FLUSH_ZERO (fs, wc, X);					\
      _FP_CHECK_FLUSH_ZERO (fs, wc, Y);					\
      if (X##_s == Y##_s)						\
	{								\
	  /* Addition.  */						\
	  __label__ add1, add2, add3, add_done;				\
	  R##_s = X##_s;						\
	  int _FP_ADD_INTERNAL_ediff = X##_e - Y##_e;			\
	  if (_FP_ADD_INTERNAL_ediff > 0)				\
	    {								\
	      R##_e = X##_e;						\
	      if (Y##_e == 0)						\
		{							\
		  /* Y is zero or denormalized.  */			\
		  if (_FP_FRAC_ZEROP_##wc (Y))				\
		    {							\
		      _FP_CHECK_SIGNAN_SEMIRAW (fs, wc, X);		\
		      _FP_FRAC_COPY_##wc (R, X);			\
		      goto add_done;					\
		    }							\
		  else							\
		    {							\
		      FP_SET_EXCEPTION (FP_EX_DENORM);			\
		      _FP_ADD_INTERNAL_ediff--;				\
		      if (_FP_ADD_INTERNAL_ediff == 0)			\
			{						\
			  _FP_FRAC_ADD_##wc (R, X, Y);			\
			  goto add3;					\
			}						\
		      if (X##_e == _FP_EXPMAX_##fs)			\
			{						\
			  _FP_CHECK_SIGNAN_SEMIRAW (fs, wc, X);		\
			  _FP_FRAC_COPY_##wc (R, X);			\
			  goto add_done;				\
			}						\
		      goto add1;					\
		    }							\
		}							\
	      else if (X##_e == _FP_EXPMAX_##fs)			\
		{							\
		  /* X is NaN or Inf, Y is normal.  */			\
		  _FP_CHECK_SIGNAN_SEMIRAW (fs, wc, X);			\
		  _FP_FRAC_COPY_##wc (R, X);				\
		  goto add_done;					\
		}							\
									\
	      /* Insert implicit MSB of Y.  */				\
	      _FP_FRAC_HIGH_##fs (Y) |= _FP_IMPLBIT_SH_##fs;		\
									\
	    add1:							\
	      /* Shift the mantissa of Y to the right			\
		 _FP_ADD_INTERNAL_EDIFF steps; remember to account	\
		 later for the implicit MSB of X.  */			\
	      if (_FP_ADD_INTERNAL_ediff <= _FP_WFRACBITS_##fs)		\
		_FP_FRAC_SRS_##wc (Y, _FP_ADD_INTERNAL_ediff,		\
				   _FP_WFRACBITS_##fs);			\
	      else if (!_FP_FRAC_ZEROP_##wc (Y))			\
		_FP_FRAC_SET_##wc (Y, _FP_MINFRAC_##wc);		\
	      _FP_FRAC_ADD_##wc (R, X, Y);				\
	    }								\
	  else if (_FP_ADD_INTERNAL_ediff < 0)				\
	    {								\
	      _FP_ADD_INTERNAL_ediff = -_FP_ADD_INTERNAL_ediff;		\
	      R##_e = Y##_e;						\
	      if (X##_e == 0)						\
		{							\
		  /* X is zero or denormalized.  */			\
		  if (_FP_FRAC_ZEROP_##wc (X))				\
		    {							\
		      _FP_CHECK_SIGNAN_SEMIRAW (fs, wc, Y);		\
		      _FP_FRAC_COPY_##wc (R, Y);			\
		      goto add_done;					\
		    }							\
		  else							\
		    {							\
		      FP_SET_EXCEPTION (FP_EX_DENORM);			\
		      _FP_ADD_INTERNAL_ediff--;				\
		      if (_FP_ADD_INTERNAL_ediff == 0)			\
			{						\
			  _FP_FRAC_ADD_##wc (R, Y, X);			\
			  goto add3;					\
			}						\
		      if (Y##_e == _FP_EXPMAX_##fs)			\
			{						\
			  _FP_CHECK_SIGNAN_SEMIRAW (fs, wc, Y);		\
			  _FP_FRAC_COPY_##wc (R, Y);			\
			  goto add_done;				\
			}						\
		      goto add2;					\
		    }							\
		}							\
	      else if (Y##_e == _FP_EXPMAX_##fs)			\
		{							\
		  /* Y is NaN or Inf, X is normal.  */			\
		  _FP_CHECK_SIGNAN_SEMIRAW (fs, wc, Y);			\
		  _FP_FRAC_COPY_##wc (R, Y);				\
		  goto add_done;					\
		}							\
									\
	      /* Insert implicit MSB of X.  */				\
	      _FP_FRAC_HIGH_##fs (X) |= _FP_IMPLBIT_SH_##fs;		\
									\
	    add2:							\
	      /* Shift the mantissa of X to the right			\
		 _FP_ADD_INTERNAL_EDIFF steps; remember to account	\
		 later for the implicit MSB of Y.  */			\
	      if (_FP_ADD_INTERNAL_ediff <= _FP_WFRACBITS_##fs)		\
		_FP_FRAC_SRS_##wc (X, _FP_ADD_INTERNAL_ediff,		\
				   _FP_WFRACBITS_##fs);			\
	      else if (!_FP_FRAC_ZEROP_##wc (X))			\
		_FP_FRAC_SET_##wc (X, _FP_MINFRAC_##wc);		\
	      _FP_FRAC_ADD_##wc (R, Y, X);				\
	    }								\
	  else								\
	    {								\
	      /* _FP_ADD_INTERNAL_ediff == 0.  */			\
	      if (!_FP_EXP_NORMAL (fs, wc, X))				\
		{							\
		  if (X##_e == 0)					\
		    {							\
		      /* X and Y are zero or denormalized.  */		\
		      R##_e = 0;					\
		      if (_FP_FRAC_ZEROP_##wc (X))			\
			{						\
			  if (!_FP_FRAC_ZEROP_##wc (Y))			\
			    FP_SET_EXCEPTION (FP_EX_DENORM);		\
			  _FP_FRAC_COPY_##wc (R, Y);			\
			  goto add_done;				\
			}						\
		      else if (_FP_FRAC_ZEROP_##wc (Y))			\
			{						\
			  FP_SET_EXCEPTION (FP_EX_DENORM);		\
			  _FP_FRAC_COPY_##wc (R, X);			\
			  goto add_done;				\
			}						\
		      else						\
			{						\
			  FP_SET_EXCEPTION (FP_EX_DENORM);		\
			  _FP_FRAC_ADD_##wc (R, X, Y);			\
			  if (_FP_FRAC_HIGH_##fs (R) & _FP_IMPLBIT_SH_##fs) \
			    {						\
			      /* Normalized result.  */			\
			      _FP_FRAC_HIGH_##fs (R)			\
				&= ~(_FP_W_TYPE) _FP_IMPLBIT_SH_##fs;	\
			      R##_e = 1;				\
			    }						\
			  goto add_done;				\
			}						\
		    }							\
		  else							\
		    {							\
		      /* X and Y are NaN or Inf.  */			\
		      _FP_CHECK_SIGNAN_SEMIRAW (fs, wc, X);		\
		      _FP_CHECK_SIGNAN_SEMIRAW (fs, wc, Y);		\
		      R##_e = _FP_EXPMAX_##fs;				\
		      if (_FP_FRAC_ZEROP_##wc (X))			\
			_FP_FRAC_COPY_##wc (R, Y);			\
		      else if (_FP_FRAC_ZEROP_##wc (Y))			\
			_FP_FRAC_COPY_##wc (R, X);			\
		      else						\
			_FP_CHOOSENAN_SEMIRAW (fs, wc, R, X, Y, OP);	\
		      goto add_done;					\
		    }							\
		}							\
	      /* The exponents of X and Y, both normal, are equal.  The	\
		 implicit MSBs will always add to increase the		\
		 exponent.  */						\
	      _FP_FRAC_ADD_##wc (R, X, Y);				\
	      R##_e = X##_e + 1;					\
	      _FP_FRAC_SRS_##wc (R, 1, _FP_WFRACBITS_##fs);		\
	      if (R##_e == _FP_EXPMAX_##fs)				\
		/* Overflow to infinity (depending on rounding mode).  */ \
		_FP_OVERFLOW_SEMIRAW (fs, wc, R);			\
	      goto add_done;						\
	    }								\
	add3:								\
	  if (_FP_FRAC_HIGH_##fs (R) & _FP_IMPLBIT_SH_##fs)		\
	    {								\
	      /* Overflow.  */						\
	      _FP_FRAC_HIGH_##fs (R) &= ~(_FP_W_TYPE) _FP_IMPLBIT_SH_##fs; \
	      R##_e++;							\
	      _FP_FRAC_SRS_##wc (R, 1, _FP_WFRACBITS_##fs);		\
	      if (R##_e == _FP_EXPMAX_##fs)				\
		/* Overflow to infinity (depending on rounding mode).  */ \
		_FP_OVERFLOW_SEMIRAW (fs, wc, R);			\
	    }								\
	add_done: ;							\
	}								\
      else								\
	{								\
	  /* Subtraction.  */						\
	  __label__ sub1, sub2, sub3, norm, sub_done;			\
	  int _FP_ADD_INTERNAL_ediff = X##_e - Y##_e;			\
	  if (_FP_ADD_INTERNAL_ediff > 0)				\
	    {								\
	      R##_e = X##_e;						\
	      R##_s = X##_s;						\
	      if (Y##_e == 0)						\
		{							\
		  /* Y is zero or denormalized.  */			\
		  if (_FP_FRAC_ZEROP_##wc (Y))				\
		    {							\
		      _FP_CHECK_SIGNAN_SEMIRAW (fs, wc, X);		\
		      _FP_FRAC_COPY_##wc (R, X);			\
		      goto sub_done;					\
		    }							\
		  else							\
		    {							\
		      FP_SET_EXCEPTION (FP_EX_DENORM);			\
		      _FP_ADD_INTERNAL_ediff--;				\
		      if (_FP_ADD_INTERNAL_ediff == 0)			\
			{						\
			  _FP_FRAC_SUB_##wc (R, X, Y);			\
			  goto sub3;					\
			}						\
		      if (X##_e == _FP_EXPMAX_##fs)			\
			{						\
			  _FP_CHECK_SIGNAN_SEMIRAW (fs, wc, X);		\
			  _FP_FRAC_COPY_##wc (R, X);			\
			  goto sub_done;				\
			}						\
		      goto sub1;					\
		    }							\
		}							\
	      else if (X##_e == _FP_EXPMAX_##fs)			\
		{							\
		  /* X is NaN or Inf, Y is normal.  */			\
		  _FP_CHECK_SIGNAN_SEMIRAW (fs, wc, X);			\
		  _FP_FRAC_COPY_##wc (R, X);				\
		  goto sub_done;					\
		}							\
									\
	      /* Insert implicit MSB of Y.  */				\
	      _FP_FRAC_HIGH_##fs (Y) |= _FP_IMPLBIT_SH_##fs;		\
									\
	    sub1:							\
	      /* Shift the mantissa of Y to the right			\
		 _FP_ADD_INTERNAL_EDIFF steps; remember to account	\
		 later for the implicit MSB of X.  */			\
	      if (_FP_ADD_INTERNAL_ediff <= _FP_WFRACBITS_##fs)		\
		_FP_FRAC_SRS_##wc (Y, _FP_ADD_INTERNAL_ediff,		\
				   _FP_WFRACBITS_##fs);			\
	      else if (!_FP_FRAC_ZEROP_##wc (Y))			\
		_FP_FRAC_SET_##wc (Y, _FP_MINFRAC_##wc);		\
	      _FP_FRAC_SUB_##wc (R, X, Y);				\
	    }								\
	  else if (_FP_ADD_INTERNAL_ediff < 0)				\
	    {								\
	      _FP_ADD_INTERNAL_ediff = -_FP_ADD_INTERNAL_ediff;		\
	      R##_e = Y##_e;						\
	      R##_s = Y##_s;						\
	      if (X##_e == 0)						\
		{							\
		  /* X is zero or denormalized.  */			\
		  if (_FP_FRAC_ZEROP_##wc (X))				\
		    {							\
		      _FP_CHECK_SIGNAN_SEMIRAW (fs, wc, Y);		\
		      _FP_FRAC_COPY_##wc (R, Y);			\
		      goto sub_done;					\
		    }							\
		  else							\
		    {							\
		      FP_SET_EXCEPTION (FP_EX_DENORM);			\
		      _FP_ADD_INTERNAL_ediff--;				\
		      if (_FP_ADD_INTERNAL_ediff == 0)			\
			{						\
			  _FP_FRAC_SUB_##wc (R, Y, X);			\
			  goto sub3;					\
			}						\
		      if (Y##_e == _FP_EXPMAX_##fs)			\
			{						\
			  _FP_CHECK_SIGNAN_SEMIRAW (fs, wc, Y);		\
			  _FP_FRAC_COPY_##wc (R, Y);			\
			  goto sub_done;				\
			}						\
		      goto sub2;					\
		    }							\
		}							\
	      else if (Y##_e == _FP_EXPMAX_##fs)			\
		{							\
		  /* Y is NaN or Inf, X is normal.  */			\
		  _FP_CHECK_SIGNAN_SEMIRAW (fs, wc, Y);			\
		  _FP_FRAC_COPY_##wc (R, Y);				\
		  goto sub_done;					\
		}							\
									\
	      /* Insert implicit MSB of X.  */				\
	      _FP_FRAC_HIGH_##fs (X) |= _FP_IMPLBIT_SH_##fs;		\
									\
	    sub2:							\
	      /* Shift the mantissa of X to the right			\
		 _FP_ADD_INTERNAL_EDIFF steps; remember to account	\
		 later for the implicit MSB of Y.  */			\
	      if (_FP_ADD_INTERNAL_ediff <= _FP_WFRACBITS_##fs)		\
		_FP_FRAC_SRS_##wc (X, _FP_ADD_INTERNAL_ediff,		\
				   _FP_WFRACBITS_##fs);			\
	      else if (!_FP_FRAC_ZEROP_##wc (X))			\
		_FP_FRAC_SET_##wc (X, _FP_MINFRAC_##wc);		\
	      _FP_FRAC_SUB_##wc (R, Y, X);				\
	    }								\
	  else								\
	    {								\
	      /* ediff == 0.  */					\
	      if (!_FP_EXP_NORMAL (fs, wc, X))				\
		{							\
		  if (X##_e == 0)					\
		    {							\
		      /* X and Y are zero or denormalized.  */		\
		      R##_e = 0;					\
		      if (_FP_FRAC_ZEROP_##wc (X))			\
			{						\
			  _FP_FRAC_COPY_##wc (R, Y);			\
			  if (_FP_FRAC_ZEROP_##wc (Y))			\
			    R##_s = (FP_ROUNDMODE == FP_RND_MINF);	\
			  else						\
			    {						\
			      FP_SET_EXCEPTION (FP_EX_DENORM);		\
			      R##_s = Y##_s;				\
			    }						\
			  goto sub_done;				\
			}						\
		      else if (_FP_FRAC_ZEROP_##wc (Y))			\
			{						\
			  FP_SET_EXCEPTION (FP_EX_DENORM);		\
			  _FP_FRAC_COPY_##wc (R, X);			\
			  R##_s = X##_s;				\
			  goto sub_done;				\
			}						\
		      else						\
			{						\
			  FP_SET_EXCEPTION (FP_EX_DENORM);		\
			  _FP_FRAC_SUB_##wc (R, X, Y);			\
			  R##_s = X##_s;				\
			  if (_FP_FRAC_HIGH_##fs (R) & _FP_IMPLBIT_SH_##fs) \
			    {						\
			      /* |X| < |Y|, negate result.  */		\
			      _FP_FRAC_SUB_##wc (R, Y, X);		\
			      R##_s = Y##_s;				\
			    }						\
			  else if (_FP_FRAC_ZEROP_##wc (R))		\
			    R##_s = (FP_ROUNDMODE == FP_RND_MINF);	\
			  goto sub_done;				\
			}						\
		    }							\
		  else							\
		    {							\
		      /* X and Y are NaN or Inf, of opposite signs.  */	\
		      _FP_CHECK_SIGNAN_SEMIRAW (fs, wc, X);		\
		      _FP_CHECK_SIGNAN_SEMIRAW (fs, wc, Y);		\
		      R##_e = _FP_EXPMAX_##fs;				\
		      if (_FP_FRAC_ZEROP_##wc (X))			\
			{						\
			  if (_FP_FRAC_ZEROP_##wc (Y))			\
			    {						\
			      /* Inf - Inf.  */				\
			      R##_s = _FP_NANSIGN_##fs;			\
			      _FP_FRAC_SET_##wc (R, _FP_NANFRAC_##fs);	\
			      _FP_FRAC_SLL_##wc (R, _FP_WORKBITS);	\
			      FP_SET_EXCEPTION (FP_EX_INVALID		\
						| FP_EX_INVALID_ISI);	\
			    }						\
			  else						\
			    {						\
			      /* Inf - NaN.  */				\
			      R##_s = Y##_s;				\
			      _FP_FRAC_COPY_##wc (R, Y);		\
			    }						\
			}						\
		      else						\
			{						\
			  if (_FP_FRAC_ZEROP_##wc (Y))			\
			    {						\
			      /* NaN - Inf.  */				\
			      R##_s = X##_s;				\
			      _FP_FRAC_COPY_##wc (R, X);		\
			    }						\
			  else						\
			    {						\
			      /* NaN - NaN.  */				\
			      _FP_CHOOSENAN_SEMIRAW (fs, wc, R, X, Y, OP); \
			    }						\
			}						\
		      goto sub_done;					\
		    }							\
		}							\
	      /* The exponents of X and Y, both normal, are equal.  The	\
		 implicit MSBs cancel.  */				\
	      R##_e = X##_e;						\
	      _FP_FRAC_SUB_##wc (R, X, Y);				\
	      R##_s = X##_s;						\
	      if (_FP_FRAC_HIGH_##fs (R) & _FP_IMPLBIT_SH_##fs)		\
		{							\
		  /* |X| < |Y|, negate result.  */			\
		  _FP_FRAC_SUB_##wc (R, Y, X);				\
		  R##_s = Y##_s;					\
		}							\
	      else if (_FP_FRAC_ZEROP_##wc (R))				\
		{							\
		  R##_e = 0;						\
		  R##_s = (FP_ROUNDMODE == FP_RND_MINF);		\
		  goto sub_done;					\
		}							\
	      goto norm;						\
	    }								\
	sub3:								\
	  if (_FP_FRAC_HIGH_##fs (R) & _FP_IMPLBIT_SH_##fs)		\
	    {								\
	      int _FP_ADD_INTERNAL_diff;				\
	      /* Carry into most significant bit of larger one of X and Y, \
		 canceling it; renormalize.  */				\
	      _FP_FRAC_HIGH_##fs (R) &= _FP_IMPLBIT_SH_##fs - 1;	\
	    norm:							\
	      _FP_FRAC_CLZ_##wc (_FP_ADD_INTERNAL_diff, R);		\
	      _FP_ADD_INTERNAL_diff -= _FP_WFRACXBITS_##fs;		\
	      _FP_FRAC_SLL_##wc (R, _FP_ADD_INTERNAL_diff);		\
	      if (R##_e <= _FP_ADD_INTERNAL_diff)			\
		{							\
		  /* R is denormalized.  */				\
		  _FP_ADD_INTERNAL_diff					\
		    = _FP_ADD_INTERNAL_diff - R##_e + 1;		\
		  _FP_FRAC_SRS_##wc (R, _FP_ADD_INTERNAL_diff,		\
				     _FP_WFRACBITS_##fs);		\
		  R##_e = 0;						\
		}							\
	      else							\
		{							\
		  R##_e -= _FP_ADD_INTERNAL_diff;			\
		  _FP_FRAC_HIGH_##fs (R) &= ~(_FP_W_TYPE) _FP_IMPLBIT_SH_##fs; \
		}							\
	    }								\
	sub_done: ;							\
	}								\
    }									\
  while (0)

#define _FP_ADD(fs, wc, R, X, Y) _FP_ADD_INTERNAL (fs, wc, R, X, Y, '+')
#define _FP_SUB(fs, wc, R, X, Y)					\
  do									\
    {									\
      if (!(Y##_e == _FP_EXPMAX_##fs && !_FP_FRAC_ZEROP_##wc (Y)))	\
	Y##_s ^= 1;							\
      _FP_ADD_INTERNAL (fs, wc, R, X, Y, '-');				\
    }									\
  while (0)


/* Main negation routine.  The input value is raw.  */

#define _FP_NEG(fs, wc, R, X)			\
  do						\
    {						\
      _FP_FRAC_COPY_##wc (R, X);		\
      R##_e = X##_e;				\
      R##_s = 1 ^ X##_s;			\
    }						\
  while (0)


/* Main multiplication routine.  The input values should be cooked.  */

#define _FP_MUL(fs, wc, R, X, Y)				\
  do								\
    {								\
      R##_s = X##_s ^ Y##_s;					\
      R##_e = X##_e + Y##_e + 1;				\
      switch (_FP_CLS_COMBINE (X##_c, Y##_c))			\
	{							\
	case _FP_CLS_COMBINE (FP_CLS_NORMAL, FP_CLS_NORMAL):	\
	  R##_c = FP_CLS_NORMAL;				\
								\
	  _FP_MUL_MEAT_##fs (R, X, Y);				\
								\
	  if (_FP_FRAC_OVERP_##wc (fs, R))			\
	    _FP_FRAC_SRS_##wc (R, 1, _FP_WFRACBITS_##fs);	\
	  else							\
	    R##_e--;						\
	  break;						\
								\
	case _FP_CLS_COMBINE (FP_CLS_NAN, FP_CLS_NAN):		\
	  _FP_CHOOSENAN (fs, wc, R, X, Y, '*');			\
	  break;						\
								\
	case _FP_CLS_COMBINE (FP_CLS_NAN, FP_CLS_NORMAL):	\
	case _FP_CLS_COMBINE (FP_CLS_NAN, FP_CLS_INF):		\
	case _FP_CLS_COMBINE (FP_CLS_NAN, FP_CLS_ZERO):		\
	  R##_s = X##_s;					\
	  /* FALLTHRU */					\
								\
	case _FP_CLS_COMBINE (FP_CLS_INF, FP_CLS_INF):		\
	case _FP_CLS_COMBINE (FP_CLS_INF, FP_CLS_NORMAL):	\
	case _FP_CLS_COMBINE (FP_CLS_ZERO, FP_CLS_NORMAL):	\
	case _FP_CLS_COMBINE (FP_CLS_ZERO, FP_CLS_ZERO):	\
	  _FP_FRAC_COPY_##wc (R, X);				\
	  R##_c = X##_c;					\
	  break;						\
								\
	case _FP_CLS_COMBINE (FP_CLS_NORMAL, FP_CLS_NAN):	\
	case _FP_CLS_COMBINE (FP_CLS_INF, FP_CLS_NAN):		\
	case _FP_CLS_COMBINE (FP_CLS_ZERO, FP_CLS_NAN):		\
	  R##_s = Y##_s;					\
	  /* FALLTHRU */					\
								\
	case _FP_CLS_COMBINE (FP_CLS_NORMAL, FP_CLS_INF):	\
	case _FP_CLS_COMBINE (FP_CLS_NORMAL, FP_CLS_ZERO):	\
	  _FP_FRAC_COPY_##wc (R, Y);				\
	  R##_c = Y##_c;					\
	  break;						\
								\
	case _FP_CLS_COMBINE (FP_CLS_INF, FP_CLS_ZERO):		\
	case _FP_CLS_COMBINE (FP_CLS_ZERO, FP_CLS_INF):		\
	  R##_s = _FP_NANSIGN_##fs;				\
	  R##_c = FP_CLS_NAN;					\
	  _FP_FRAC_SET_##wc (R, _FP_NANFRAC_##fs);		\
	  FP_SET_EXCEPTION (FP_EX_INVALID | FP_EX_INVALID_IMZ);	\
	  break;						\
								\
	default:						\
	  _FP_UNREACHABLE;					\
	}							\
    }								\
  while (0)


/* Fused multiply-add.  The input values should be cooked.  */

#define _FP_FMA(fs, wc, dwc, R, X, Y, Z)				\
  do									\
    {									\
      __label__ done_fma;						\
      FP_DECL_##fs (_FP_FMA_T);						\
      _FP_FMA_T##_s = X##_s ^ Y##_s;					\
      _FP_FMA_T##_e = X##_e + Y##_e + 1;				\
      switch (_FP_CLS_COMBINE (X##_c, Y##_c))				\
	{								\
	case _FP_CLS_COMBINE (FP_CLS_NORMAL, FP_CLS_NORMAL):		\
	  switch (Z##_c)						\
	    {								\
	    case FP_CLS_INF:						\
	    case FP_CLS_NAN:						\
	      R##_s = Z##_s;						\
	      _FP_FRAC_COPY_##wc (R, Z);				\
	      R##_c = Z##_c;						\
	      break;							\
									\
	    case FP_CLS_ZERO:						\
	      R##_c = FP_CLS_NORMAL;					\
	      R##_s = _FP_FMA_T##_s;					\
	      R##_e = _FP_FMA_T##_e;					\
									\
	      _FP_MUL_MEAT_##fs (R, X, Y);				\
									\
	      if (_FP_FRAC_OVERP_##wc (fs, R))				\
		_FP_FRAC_SRS_##wc (R, 1, _FP_WFRACBITS_##fs);		\
	      else							\
		R##_e--;						\
	      break;							\
									\
	    case FP_CLS_NORMAL:;					\
	      _FP_FRAC_DECL_##dwc (_FP_FMA_TD);				\
	      _FP_FRAC_DECL_##dwc (_FP_FMA_ZD);				\
	      _FP_FRAC_DECL_##dwc (_FP_FMA_RD);				\
	      _FP_MUL_MEAT_DW_##fs (_FP_FMA_TD, X, Y);			\
	      R##_e = _FP_FMA_T##_e;					\
	      int _FP_FMA_tsh						\
		= _FP_FRAC_HIGHBIT_DW_##dwc (fs, _FP_FMA_TD) == 0;	\
	      _FP_FMA_T##_e -= _FP_FMA_tsh;				\
	      int _FP_FMA_ediff = _FP_FMA_T##_e - Z##_e;		\
	      if (_FP_FMA_ediff >= 0)					\
		{							\
		  int _FP_FMA_shift					\
		    = _FP_WFRACBITS_##fs - _FP_FMA_tsh - _FP_FMA_ediff;	\
		  if (_FP_FMA_shift <= -_FP_WFRACBITS_##fs)		\
		    _FP_FRAC_SET_##dwc (_FP_FMA_ZD, _FP_MINFRAC_##dwc);	\
		  else							\
		    {							\
		      _FP_FRAC_COPY_##dwc##_##wc (_FP_FMA_ZD, Z);	\
		      if (_FP_FMA_shift < 0)				\
			_FP_FRAC_SRS_##dwc (_FP_FMA_ZD, -_FP_FMA_shift,	\
					    _FP_WFRACBITS_DW_##fs);	\
		      else if (_FP_FMA_shift > 0)			\
			_FP_FRAC_SLL_##dwc (_FP_FMA_ZD, _FP_FMA_shift);	\
		    }							\
		  R##_s = _FP_FMA_T##_s;				\
		  if (_FP_FMA_T##_s == Z##_s)				\
		    _FP_FRAC_ADD_##dwc (_FP_FMA_RD, _FP_FMA_TD,		\
					_FP_FMA_ZD);			\
		  else							\
		    {							\
		      _FP_FRAC_SUB_##dwc (_FP_FMA_RD, _FP_FMA_TD,	\
					  _FP_FMA_ZD);			\
		      if (_FP_FRAC_NEGP_##dwc (_FP_FMA_RD))		\
			{						\
			  R##_s = Z##_s;				\
			  _FP_FRAC_SUB_##dwc (_FP_FMA_RD, _FP_FMA_ZD,	\
					      _FP_FMA_TD);		\
			}						\
		    }							\
		}							\
	      else							\
		{							\
		  R##_e = Z##_e;					\
		  R##_s = Z##_s;					\
		  _FP_FRAC_COPY_##dwc##_##wc (_FP_FMA_ZD, Z);		\
		  _FP_FRAC_SLL_##dwc (_FP_FMA_ZD, _FP_WFRACBITS_##fs);	\
		  int _FP_FMA_shift = -_FP_FMA_ediff - _FP_FMA_tsh;	\
		  if (_FP_FMA_shift >= _FP_WFRACBITS_DW_##fs)		\
		    _FP_FRAC_SET_##dwc (_FP_FMA_TD, _FP_MINFRAC_##dwc);	\
		  else if (_FP_FMA_shift > 0)				\
		    _FP_FRAC_SRS_##dwc (_FP_FMA_TD, _FP_FMA_shift,	\
					_FP_WFRACBITS_DW_##fs);		\
		  if (Z##_s == _FP_FMA_T##_s)				\
		    _FP_FRAC_ADD_##dwc (_FP_FMA_RD, _FP_FMA_ZD,		\
					_FP_FMA_TD);			\
		  else							\
		    _FP_FRAC_SUB_##dwc (_FP_FMA_RD, _FP_FMA_ZD,		\
					_FP_FMA_TD);			\
		}							\
	      if (_FP_FRAC_ZEROP_##dwc (_FP_FMA_RD))			\
		{							\
		  if (_FP_FMA_T##_s == Z##_s)				\
		    R##_s = Z##_s;					\
		  else							\
		    R##_s = (FP_ROUNDMODE == FP_RND_MINF);		\
		  _FP_FRAC_SET_##wc (R, _FP_ZEROFRAC_##wc);		\
		  R##_c = FP_CLS_ZERO;					\
		}							\
	      else							\
		{							\
		  int _FP_FMA_rlz;					\
		  _FP_FRAC_CLZ_##dwc (_FP_FMA_rlz, _FP_FMA_RD);		\
		  _FP_FMA_rlz -= _FP_WFRACXBITS_DW_##fs;		\
		  R##_e -= _FP_FMA_rlz;					\
		  int _FP_FMA_shift = _FP_WFRACBITS_##fs - _FP_FMA_rlz;	\
		  if (_FP_FMA_shift > 0)				\
		    _FP_FRAC_SRS_##dwc (_FP_FMA_RD, _FP_FMA_shift,	\
					_FP_WFRACBITS_DW_##fs);		\
		  else if (_FP_FMA_shift < 0)				\
		    _FP_FRAC_SLL_##dwc (_FP_FMA_RD, -_FP_FMA_shift);	\
		  _FP_FRAC_COPY_##wc##_##dwc (R, _FP_FMA_RD);		\
		  R##_c = FP_CLS_NORMAL;				\
		}							\
	      break;							\
	    }								\
	  goto done_fma;						\
									\
	case _FP_CLS_COMBINE (FP_CLS_NAN, FP_CLS_NAN):			\
	  _FP_CHOOSENAN (fs, wc, _FP_FMA_T, X, Y, '*');			\
	  break;							\
									\
	case _FP_CLS_COMBINE (FP_CLS_NAN, FP_CLS_NORMAL):		\
	case _FP_CLS_COMBINE (FP_CLS_NAN, FP_CLS_INF):			\
	case _FP_CLS_COMBINE (FP_CLS_NAN, FP_CLS_ZERO):			\
	  _FP_FMA_T##_s = X##_s;					\
	  /* FALLTHRU */						\
									\
	case _FP_CLS_COMBINE (FP_CLS_INF, FP_CLS_INF):			\
	case _FP_CLS_COMBINE (FP_CLS_INF, FP_CLS_NORMAL):		\
	case _FP_CLS_COMBINE (FP_CLS_ZERO, FP_CLS_NORMAL):		\
	case _FP_CLS_COMBINE (FP_CLS_ZERO, FP_CLS_ZERO):		\
	  _FP_FRAC_COPY_##wc (_FP_FMA_T, X);				\
	  _FP_FMA_T##_c = X##_c;					\
	  break;							\
									\
	case _FP_CLS_COMBINE (FP_CLS_NORMAL, FP_CLS_NAN):		\
	case _FP_CLS_COMBINE (FP_CLS_INF, FP_CLS_NAN):			\
	case _FP_CLS_COMBINE (FP_CLS_ZERO, FP_CLS_NAN):			\
	  _FP_FMA_T##_s = Y##_s;					\
	  /* FALLTHRU */						\
									\
	case _FP_CLS_COMBINE (FP_CLS_NORMAL, FP_CLS_INF):		\
	case _FP_CLS_COMBINE (FP_CLS_NORMAL, FP_CLS_ZERO):		\
	  _FP_FRAC_COPY_##wc (_FP_FMA_T, Y);				\
	  _FP_FMA_T##_c = Y##_c;					\
	  break;							\
									\
	case _FP_CLS_COMBINE (FP_CLS_INF, FP_CLS_ZERO):			\
	case _FP_CLS_COMBINE (FP_CLS_ZERO, FP_CLS_INF):			\
	  _FP_FMA_T##_s = _FP_NANSIGN_##fs;				\
	  _FP_FMA_T##_c = FP_CLS_NAN;					\
	  _FP_FRAC_SET_##wc (_FP_FMA_T, _FP_NANFRAC_##fs);		\
	  FP_SET_EXCEPTION (FP_EX_INVALID | FP_EX_INVALID_IMZ_FMA);	\
	  break;							\
									\
	default:							\
	  _FP_UNREACHABLE;						\
	}								\
									\
      /* T = X * Y is zero, infinity or NaN.  */			\
      switch (_FP_CLS_COMBINE (_FP_FMA_T##_c, Z##_c))			\
	{								\
	case _FP_CLS_COMBINE (FP_CLS_NAN, FP_CLS_NAN):			\
	  _FP_CHOOSENAN (fs, wc, R, _FP_FMA_T, Z, '+');			\
	  break;							\
									\
	case _FP_CLS_COMBINE (FP_CLS_NAN, FP_CLS_NORMAL):		\
	case _FP_CLS_COMBINE (FP_CLS_NAN, FP_CLS_INF):			\
	case _FP_CLS_COMBINE (FP_CLS_NAN, FP_CLS_ZERO):			\
	case _FP_CLS_COMBINE (FP_CLS_INF, FP_CLS_NORMAL):		\
	case _FP_CLS_COMBINE (FP_CLS_INF, FP_CLS_ZERO):			\
	  R##_s = _FP_FMA_T##_s;					\
	  _FP_FRAC_COPY_##wc (R, _FP_FMA_T);				\
	  R##_c = _FP_FMA_T##_c;					\
	  break;							\
									\
	case _FP_CLS_COMBINE (FP_CLS_INF, FP_CLS_NAN):			\
	case _FP_CLS_COMBINE (FP_CLS_ZERO, FP_CLS_NAN):			\
	case _FP_CLS_COMBINE (FP_CLS_ZERO, FP_CLS_NORMAL):		\
	case _FP_CLS_COMBINE (FP_CLS_ZERO, FP_CLS_INF):			\
	  R##_s = Z##_s;						\
	  _FP_FRAC_COPY_##wc (R, Z);					\
	  R##_c = Z##_c;						\
	  R##_e = Z##_e;						\
	  break;							\
									\
	case _FP_CLS_COMBINE (FP_CLS_INF, FP_CLS_INF):			\
	  if (_FP_FMA_T##_s == Z##_s)					\
	    {								\
	      R##_s = Z##_s;						\
	      _FP_FRAC_COPY_##wc (R, Z);				\
	      R##_c = Z##_c;						\
	    }								\
	  else								\
	    {								\
	      R##_s = _FP_NANSIGN_##fs;					\
	      R##_c = FP_CLS_NAN;					\
	      _FP_FRAC_SET_##wc (R, _FP_NANFRAC_##fs);			\
	      FP_SET_EXCEPTION (FP_EX_INVALID | FP_EX_INVALID_ISI);	\
	    }								\
	  break;							\
									\
	case _FP_CLS_COMBINE (FP_CLS_ZERO, FP_CLS_ZERO):		\
	  if (_FP_FMA_T##_s == Z##_s)					\
	    R##_s = Z##_s;						\
	  else								\
	    R##_s = (FP_ROUNDMODE == FP_RND_MINF);			\
	  _FP_FRAC_COPY_##wc (R, Z);					\
	  R##_c = Z##_c;						\
	  break;							\
									\
	default:							\
	  _FP_UNREACHABLE;						\
	}								\
    done_fma: ;								\
    }									\
  while (0)


/* Main division routine.  The input values should be cooked.  */

#define _FP_DIV(fs, wc, R, X, Y)				\
  do								\
    {								\
      R##_s = X##_s ^ Y##_s;					\
      R##_e = X##_e - Y##_e;					\
      switch (_FP_CLS_COMBINE (X##_c, Y##_c))			\
	{							\
	case _FP_CLS_COMBINE (FP_CLS_NORMAL, FP_CLS_NORMAL):	\
	  R##_c = FP_CLS_NORMAL;				\
								\
	  _FP_DIV_MEAT_##fs (R, X, Y);				\
	  break;						\
								\
	case _FP_CLS_COMBINE (FP_CLS_NAN, FP_CLS_NAN):		\
	  _FP_CHOOSENAN (fs, wc, R, X, Y, '/');			\
	  break;						\
								\
	case _FP_CLS_COMBINE (FP_CLS_NAN, FP_CLS_NORMAL):	\
	case _FP_CLS_COMBINE (FP_CLS_NAN, FP_CLS_INF):		\
	case _FP_CLS_COMBINE (FP_CLS_NAN, FP_CLS_ZERO):		\
	  R##_s = X##_s;					\
	  _FP_FRAC_COPY_##wc (R, X);				\
	  R##_c = X##_c;					\
	  break;						\
								\
	case _FP_CLS_COMBINE (FP_CLS_NORMAL, FP_CLS_NAN):	\
	case _FP_CLS_COMBINE (FP_CLS_INF, FP_CLS_NAN):		\
	case _FP_CLS_COMBINE (FP_CLS_ZERO, FP_CLS_NAN):		\
	  R##_s = Y##_s;					\
	  _FP_FRAC_COPY_##wc (R, Y);				\
	  R##_c = Y##_c;					\
	  break;						\
								\
	case _FP_CLS_COMBINE (FP_CLS_NORMAL, FP_CLS_INF):	\
	case _FP_CLS_COMBINE (FP_CLS_ZERO, FP_CLS_INF):		\
	case _FP_CLS_COMBINE (FP_CLS_ZERO, FP_CLS_NORMAL):	\
	  R##_c = FP_CLS_ZERO;					\
	  break;						\
								\
	case _FP_CLS_COMBINE (FP_CLS_NORMAL, FP_CLS_ZERO):	\
	  FP_SET_EXCEPTION (FP_EX_DIVZERO);			\
	  /* FALLTHRU */					\
	case _FP_CLS_COMBINE (FP_CLS_INF, FP_CLS_ZERO):		\
	case _FP_CLS_COMBINE (FP_CLS_INF, FP_CLS_NORMAL):	\
	  R##_c = FP_CLS_INF;					\
	  break;						\
								\
	case _FP_CLS_COMBINE (FP_CLS_INF, FP_CLS_INF):		\
	case _FP_CLS_COMBINE (FP_CLS_ZERO, FP_CLS_ZERO):	\
	  R##_s = _FP_NANSIGN_##fs;				\
	  R##_c = FP_CLS_NAN;					\
	  _FP_FRAC_SET_##wc (R, _FP_NANFRAC_##fs);		\
	  FP_SET_EXCEPTION (FP_EX_INVALID			\
			    | (X##_c == FP_CLS_INF		\
			       ? FP_EX_INVALID_IDI		\
			       : FP_EX_INVALID_ZDZ));		\
	  break;						\
								\
	default:						\
	  _FP_UNREACHABLE;					\
	}							\
    }								\
  while (0)


/* Helper for comparisons.  EX is 0 not to raise exceptions, 1 to
   raise exceptions for signaling NaN operands, 2 to raise exceptions
   for all NaN operands.  Conditionals are organized to allow the
   compiler to optimize away code based on the value of EX.  */

#define _FP_CMP_CHECK_NAN(fs, wc, X, Y, ex)				\
  do									\
    {									\
      /* The arguments are unordered, which may or may not result in	\
	 an exception.  */						\
      if (ex)								\
	{								\
	  /* At least some cases of unordered arguments result in	\
	     exceptions; check whether this is one.  */			\
	  if (FP_EX_INVALID_SNAN || FP_EX_INVALID_VC)			\
	    {								\
	      /* Check separately for each case of "invalid"		\
		 exceptions.  */					\
	      if ((ex) == 2)						\
		FP_SET_EXCEPTION (FP_EX_INVALID | FP_EX_INVALID_VC);	\
	      if (_FP_ISSIGNAN (fs, wc, X)				\
		  || _FP_ISSIGNAN (fs, wc, Y))				\
		FP_SET_EXCEPTION (FP_EX_INVALID | FP_EX_INVALID_SNAN);	\
	    }								\
	  /* Otherwise, we only need to check whether to raise an	\
	     exception, not which case or cases it is.  */		\
	  else if ((ex) == 2						\
		   || _FP_ISSIGNAN (fs, wc, X)				\
		   || _FP_ISSIGNAN (fs, wc, Y))				\
	    FP_SET_EXCEPTION (FP_EX_INVALID);				\
	}								\
    }									\
  while (0)

/* Helper for comparisons.  If denormal operands would raise an
   exception, check for them, and flush to zero as appropriate
   (otherwise, we need only check and flush to zero if it might affect
   the result, which is done later with _FP_CMP_CHECK_FLUSH_ZERO).  */
#define _FP_CMP_CHECK_DENORM(fs, wc, X, Y)				\
  do									\
    {									\
      if (FP_EX_DENORM != 0)						\
	{								\
	  /* We must ensure the correct exceptions are raised for	\
	     denormal operands, even though this may not affect the	\
	     result of the comparison.  */				\
	  if (FP_DENORM_ZERO)						\
	    {								\
	      _FP_CHECK_FLUSH_ZERO (fs, wc, X);				\
	      _FP_CHECK_FLUSH_ZERO (fs, wc, Y);				\
	    }								\
	  else								\
	    {								\
	      if ((X##_e == 0 && !_FP_FRAC_ZEROP_##wc (X))		\
		  || (Y##_e == 0 && !_FP_FRAC_ZEROP_##wc (Y)))		\
		FP_SET_EXCEPTION (FP_EX_DENORM);			\
	    }								\
	}								\
    }									\
  while (0)

/* Helper for comparisons.  Check for flushing denormals for zero if
   we didn't need to check earlier for any denormal operands.  */
#define _FP_CMP_CHECK_FLUSH_ZERO(fs, wc, X, Y)	\
  do						\
    {						\
      if (FP_EX_DENORM == 0)			\
	{					\
	  _FP_CHECK_FLUSH_ZERO (fs, wc, X);	\
	  _FP_CHECK_FLUSH_ZERO (fs, wc, Y);	\
	}					\
    }						\
  while (0)

/* Main differential comparison routine.  The inputs should be raw not
   cooked.  The return is -1, 0, 1 for normal values, UN
   otherwise.  */

#define _FP_CMP(fs, wc, ret, X, Y, un, ex)				\
  do									\
    {									\
      _FP_CMP_CHECK_DENORM (fs, wc, X, Y);				\
      /* NANs are unordered.  */					\
      if ((X##_e == _FP_EXPMAX_##fs && !_FP_FRAC_ZEROP_##wc (X))	\
	  || (Y##_e == _FP_EXPMAX_##fs && !_FP_FRAC_ZEROP_##wc (Y)))	\
	{								\
	  (ret) = (un);							\
	  _FP_CMP_CHECK_NAN (fs, wc, X, Y, (ex));			\
	}								\
      else								\
	{								\
	  int _FP_CMP_is_zero_x;					\
	  int _FP_CMP_is_zero_y;					\
									\
	  _FP_CMP_CHECK_FLUSH_ZERO (fs, wc, X, Y);			\
									\
	  _FP_CMP_is_zero_x						\
	    = (!X##_e && _FP_FRAC_ZEROP_##wc (X)) ? 1 : 0;		\
	  _FP_CMP_is_zero_y						\
	    = (!Y##_e && _FP_FRAC_ZEROP_##wc (Y)) ? 1 : 0;		\
									\
	  if (_FP_CMP_is_zero_x && _FP_CMP_is_zero_y)			\
	    (ret) = 0;							\
	  else if (_FP_CMP_is_zero_x)					\
	    (ret) = Y##_s ? 1 : -1;					\
	  else if (_FP_CMP_is_zero_y)					\
	    (ret) = X##_s ? -1 : 1;					\
	  else if (X##_s != Y##_s)					\
	    (ret) = X##_s ? -1 : 1;					\
	  else if (X##_e > Y##_e)					\
	    (ret) = X##_s ? -1 : 1;					\
	  else if (X##_e < Y##_e)					\
	    (ret) = X##_s ? 1 : -1;					\
	  else if (_FP_FRAC_GT_##wc (X, Y))				\
	    (ret) = X##_s ? -1 : 1;					\
	  else if (_FP_FRAC_GT_##wc (Y, X))				\
	    (ret) = X##_s ? 1 : -1;					\
	  else								\
	    (ret) = 0;							\
	}								\
    }									\
  while (0)


/* Simplification for strict equality.  */

#define _FP_CMP_EQ(fs, wc, ret, X, Y, ex)				\
  do									\
    {									\
      _FP_CMP_CHECK_DENORM (fs, wc, X, Y);				\
      /* NANs are unordered.  */					\
      if ((X##_e == _FP_EXPMAX_##fs && !_FP_FRAC_ZEROP_##wc (X))	\
	  || (Y##_e == _FP_EXPMAX_##fs && !_FP_FRAC_ZEROP_##wc (Y)))	\
	{								\
	  (ret) = 1;							\
	  _FP_CMP_CHECK_NAN (fs, wc, X, Y, (ex));			\
	}								\
      else								\
	{								\
	  _FP_CMP_CHECK_FLUSH_ZERO (fs, wc, X, Y);			\
									\
	  (ret) = !(X##_e == Y##_e					\
		    && _FP_FRAC_EQ_##wc (X, Y)				\
		    && (X##_s == Y##_s					\
			|| (!X##_e && _FP_FRAC_ZEROP_##wc (X))));	\
	}								\
    }									\
  while (0)

/* Version to test unordered.  */

#define _FP_CMP_UNORD(fs, wc, ret, X, Y, ex)				\
  do									\
    {									\
      _FP_CMP_CHECK_DENORM (fs, wc, X, Y);				\
      (ret) = ((X##_e == _FP_EXPMAX_##fs && !_FP_FRAC_ZEROP_##wc (X))	\
	       || (Y##_e == _FP_EXPMAX_##fs && !_FP_FRAC_ZEROP_##wc (Y))); \
      if (ret)								\
	_FP_CMP_CHECK_NAN (fs, wc, X, Y, (ex));				\
    }									\
  while (0)

/* Main square root routine.  The input value should be cooked.  */

#define _FP_SQRT(fs, wc, R, X)						\
  do									\
    {									\
      _FP_FRAC_DECL_##wc (_FP_SQRT_T);					\
      _FP_FRAC_DECL_##wc (_FP_SQRT_S);					\
      _FP_W_TYPE _FP_SQRT_q;						\
      switch (X##_c)							\
	{								\
	case FP_CLS_NAN:						\
	  _FP_FRAC_COPY_##wc (R, X);					\
	  R##_s = X##_s;						\
	  R##_c = FP_CLS_NAN;						\
	  break;							\
	case FP_CLS_INF:						\
	  if (X##_s)							\
	    {								\
	      R##_s = _FP_NANSIGN_##fs;					\
	      R##_c = FP_CLS_NAN; /* NAN */				\
	      _FP_FRAC_SET_##wc (R, _FP_NANFRAC_##fs);			\
	      FP_SET_EXCEPTION (FP_EX_INVALID | FP_EX_INVALID_SQRT);	\
	    }								\
	  else								\
	    {								\
	      R##_s = 0;						\
	      R##_c = FP_CLS_INF; /* sqrt(+inf) = +inf */		\
	    }								\
	  break;							\
	case FP_CLS_ZERO:						\
	  R##_s = X##_s;						\
	  R##_c = FP_CLS_ZERO; /* sqrt(+-0) = +-0 */			\
	  break;							\
	case FP_CLS_NORMAL:						\
	  R##_s = 0;							\
	  if (X##_s)							\
	    {								\
	      R##_c = FP_CLS_NAN; /* NAN */				\
	      R##_s = _FP_NANSIGN_##fs;					\
	      _FP_FRAC_SET_##wc (R, _FP_NANFRAC_##fs);			\
	      FP_SET_EXCEPTION (FP_EX_INVALID | FP_EX_INVALID_SQRT);	\
	      break;							\
	    }								\
	  R##_c = FP_CLS_NORMAL;					\
	  if (X##_e & 1)						\
	    _FP_FRAC_SLL_##wc (X, 1);					\
	  R##_e = X##_e >> 1;						\
	  _FP_FRAC_SET_##wc (_FP_SQRT_S, _FP_ZEROFRAC_##wc);		\
	  _FP_FRAC_SET_##wc (R, _FP_ZEROFRAC_##wc);			\
	  _FP_SQRT_q = _FP_OVERFLOW_##fs >> 1;				\
	  _FP_SQRT_MEAT_##wc (R, _FP_SQRT_S, _FP_SQRT_T, X,		\
			      _FP_SQRT_q);				\
	}								\
    }									\
  while (0)

/* Convert from FP to integer.  Input is raw.  */

/* RSIGNED can have following values:
   0:  the number is required to be 0..(2^rsize)-1, if not, NV is set plus
	   the result is either 0 or (2^rsize)-1 depending on the sign in such
	   case.
   1:  the number is required to be -(2^(rsize-1))..(2^(rsize-1))-1, if not,
	   NV is set plus the result is either -(2^(rsize-1)) or (2^(rsize-1))-1
	   depending on the sign in such case.
   2:  the number is required to be -(2^(rsize-1))..(2^(rsize-1))-1, if not,
	   NV is set plus the result is reduced modulo 2^rsize.
   -1: the number is required to be -(2^(rsize-1))..(2^rsize)-1, if not, NV is
	   set plus the result is either -(2^(rsize-1)) or (2^(rsize-1))-1
	   depending on the sign in such case.  */
#define _FP_TO_INT(fs, wc, r, X, rsize, rsigned)			\
  do									\
    {									\
      if (X##_e < _FP_EXPBIAS_##fs)					\
	{								\
	  (r) = 0;							\
	  if (X##_e == 0)						\
	    {								\
	      if (!_FP_FRAC_ZEROP_##wc (X))				\
		{							\
		  if (!FP_DENORM_ZERO)					\
		    FP_SET_EXCEPTION (FP_EX_INEXACT);			\
		  FP_SET_EXCEPTION (FP_EX_DENORM);			\
		}							\
	    }								\
	  else								\
	    FP_SET_EXCEPTION (FP_EX_INEXACT);				\
	}								\
      else if ((rsigned) == 2						\
	       && (X##_e						\
		   >= ((_FP_EXPMAX_##fs					\
			< _FP_EXPBIAS_##fs + _FP_FRACBITS_##fs + (rsize) - 1) \
		       ? _FP_EXPMAX_##fs				\
		       : _FP_EXPBIAS_##fs + _FP_FRACBITS_##fs + (rsize) - 1))) \
	{								\
	  /* Overflow resulting in 0.  */				\
	  (r) = 0;							\
	  FP_SET_EXCEPTION (FP_EX_INVALID				\
			    | FP_EX_INVALID_CVI				\
			    | ((FP_EX_INVALID_SNAN			\
				&& _FP_ISSIGNAN (fs, wc, X))		\
			       ? FP_EX_INVALID_SNAN			\
			       : 0));					\
	}								\
      else if ((rsigned) != 2						\
	       && (X##_e >= (_FP_EXPMAX_##fs < _FP_EXPBIAS_##fs + (rsize) \
			     ? _FP_EXPMAX_##fs				\
			     : (_FP_EXPBIAS_##fs + (rsize)		\
				- ((rsigned) > 0 || X##_s)))		\
		   || (!(rsigned) && X##_s)))				\
	{								\
	  /* Overflow or converting to the most negative integer.  */	\
	  if (rsigned)							\
	    {								\
	      (r) = 1;							\
	      (r) <<= (rsize) - 1;					\
	      (r) -= 1 - X##_s;						\
	    }								\
	  else								\
	    {								\
	      (r) = 0;							\
	      if (!X##_s)						\
		(r) = ~(r);						\
	    }								\
									\
	  if (_FP_EXPBIAS_##fs + (rsize) - 1 < _FP_EXPMAX_##fs		\
	      && (rsigned)						\
	      && X##_s							\
	      && X##_e == _FP_EXPBIAS_##fs + (rsize) - 1)		\
	    {								\
	      /* Possibly converting to most negative integer; check the \
		 mantissa.  */						\
	      int _FP_TO_INT_inexact = 0;				\
	      (void) ((_FP_FRACBITS_##fs > (rsize))			\
		      ? ({						\
			  _FP_FRAC_SRST_##wc (X, _FP_TO_INT_inexact,	\
					      _FP_FRACBITS_##fs - (rsize), \
					      _FP_FRACBITS_##fs);	\
			  0;						\
			})						\
		      : 0);						\
	      if (!_FP_FRAC_ZEROP_##wc (X))				\
		FP_SET_EXCEPTION (FP_EX_INVALID | FP_EX_INVALID_CVI);	\
	      else if (_FP_TO_INT_inexact)				\
		FP_SET_EXCEPTION (FP_EX_INEXACT);			\
	    }								\
	  else								\
	    FP_SET_EXCEPTION (FP_EX_INVALID				\
			      | FP_EX_INVALID_CVI			\
			      | ((FP_EX_INVALID_SNAN			\
				  && _FP_ISSIGNAN (fs, wc, X))		\
				 ? FP_EX_INVALID_SNAN			\
				 : 0));					\
	}								\
      else								\
	{								\
	  int _FP_TO_INT_inexact = 0;					\
	  _FP_FRAC_HIGH_RAW_##fs (X) |= _FP_IMPLBIT_##fs;		\
	  if (X##_e >= _FP_EXPBIAS_##fs + _FP_FRACBITS_##fs - 1)	\
	    {								\
	      _FP_FRAC_ASSEMBLE_##wc ((r), X, (rsize));			\
	      (r) <<= X##_e - _FP_EXPBIAS_##fs - _FP_FRACBITS_##fs + 1; \
	    }								\
	  else								\
	    {								\
	      _FP_FRAC_SRST_##wc (X, _FP_TO_INT_inexact,		\
				  (_FP_FRACBITS_##fs + _FP_EXPBIAS_##fs - 1 \
				   - X##_e),				\
				  _FP_FRACBITS_##fs);			\
	      _FP_FRAC_ASSEMBLE_##wc ((r), X, (rsize));			\
	    }								\
	  if ((rsigned) && X##_s)					\
	    (r) = -(r);							\
	  if ((rsigned) == 2 && X##_e >= _FP_EXPBIAS_##fs + (rsize) - 1) \
	    {								\
	      /* Overflow or converting to the most negative integer.  */ \
	      if (X##_e > _FP_EXPBIAS_##fs + (rsize) - 1		\
		  || !X##_s						\
		  || (r) != (((typeof (r)) 1) << ((rsize) - 1)))	\
		{							\
		  _FP_TO_INT_inexact = 0;				\
		  FP_SET_EXCEPTION (FP_EX_INVALID | FP_EX_INVALID_CVI);	\
		}							\
	    }								\
	  if (_FP_TO_INT_inexact)					\
	    FP_SET_EXCEPTION (FP_EX_INEXACT);				\
	}								\
    }									\
  while (0)

/* Convert from floating point to integer, rounding according to the
   current rounding direction.  Input is raw.  RSIGNED is as for
   _FP_TO_INT.  */
#define _FP_TO_INT_ROUND(fs, wc, r, X, rsize, rsigned)			\
  do									\
    {									\
      __label__ _FP_TO_INT_ROUND_done;					\
      if (X##_e < _FP_EXPBIAS_##fs)					\
	{								\
	  int _FP_TO_INT_ROUND_rounds_away = 0;				\
	  if (X##_e == 0)						\
	    {								\
	      if (_FP_FRAC_ZEROP_##wc (X))				\
		{							\
		  (r) = 0;						\
		  goto _FP_TO_INT_ROUND_done;				\
		}							\
	      else							\
		{							\
		  FP_SET_EXCEPTION (FP_EX_DENORM);			\
		  if (FP_DENORM_ZERO)					\
		    {							\
		      (r) = 0;						\
		      goto _FP_TO_INT_ROUND_done;			\
		    }							\
		}							\
	    }								\
	  /* The result is 0, 1 or -1 depending on the rounding mode;	\
	     -1 may cause overflow in the unsigned case.  */		\
	  switch (FP_ROUNDMODE)						\
	    {								\
	    case FP_RND_NEAREST:					\
	      _FP_TO_INT_ROUND_rounds_away				\
		= (X##_e == _FP_EXPBIAS_##fs - 1			\
		   && !_FP_FRAC_ZEROP_##wc (X));			\
	      break;							\
	    case FP_RND_ZERO:						\
	      /* _FP_TO_INT_ROUND_rounds_away is already 0.  */		\
	      break;							\
	    case FP_RND_PINF:						\
	      _FP_TO_INT_ROUND_rounds_away = !X##_s;			\
	      break;							\
	    case FP_RND_MINF:						\
	      _FP_TO_INT_ROUND_rounds_away = X##_s;			\
	      break;							\
	    }								\
	  if ((rsigned) == 0 && _FP_TO_INT_ROUND_rounds_away && X##_s)	\
	    {								\
	      /* Result of -1 for an unsigned conversion.  */		\
	      (r) = 0;							\
	      FP_SET_EXCEPTION (FP_EX_INVALID | FP_EX_INVALID_CVI);	\
	    }								\
	  else if ((rsize) == 1 && (rsigned) > 0			\
		   && _FP_TO_INT_ROUND_rounds_away && !X##_s)		\
	    {								\
	      /* Converting to a 1-bit signed bit-field, which cannot	\
		 represent +1.  */					\
	      (r) = ((rsigned) == 2 ? -1 : 0);				\
	      FP_SET_EXCEPTION (FP_EX_INVALID | FP_EX_INVALID_CVI);	\
	    }								\
	  else								\
	    {								\
	      (r) = (_FP_TO_INT_ROUND_rounds_away			\
		     ? (X##_s ? -1 : 1)					\
		     : 0);						\
	      FP_SET_EXCEPTION (FP_EX_INEXACT);				\
	    }								\
	}								\
      else if ((rsigned) == 2						\
	       && (X##_e						\
		   >= ((_FP_EXPMAX_##fs					\
			< _FP_EXPBIAS_##fs + _FP_FRACBITS_##fs + (rsize) - 1) \
		       ? _FP_EXPMAX_##fs				\
		       : _FP_EXPBIAS_##fs + _FP_FRACBITS_##fs + (rsize) - 1))) \
	{								\
	  /* Overflow resulting in 0.  */				\
	  (r) = 0;							\
	  FP_SET_EXCEPTION (FP_EX_INVALID				\
			    | FP_EX_INVALID_CVI				\
			    | ((FP_EX_INVALID_SNAN			\
				&& _FP_ISSIGNAN (fs, wc, X))		\
			       ? FP_EX_INVALID_SNAN			\
			       : 0));					\
	}								\
      else if ((rsigned) != 2						\
	       && (X##_e >= (_FP_EXPMAX_##fs < _FP_EXPBIAS_##fs + (rsize) \
			     ? _FP_EXPMAX_##fs				\
			     : (_FP_EXPBIAS_##fs + (rsize)		\
				- ((rsigned) > 0 && !X##_s)))		\
		   || ((rsigned) == 0 && X##_s)))			\
	{								\
	  /* Definite overflow (does not require rounding to tell).  */	\
	  if ((rsigned) != 0)						\
	    {								\
	      (r) = 1;							\
	      (r) <<= (rsize) - 1;					\
	      (r) -= 1 - X##_s;						\
	    }								\
	  else								\
	    {								\
	      (r) = 0;							\
	      if (!X##_s)						\
		(r) = ~(r);						\
	    }								\
									\
	  FP_SET_EXCEPTION (FP_EX_INVALID				\
			    | FP_EX_INVALID_CVI				\
			    | ((FP_EX_INVALID_SNAN			\
				&& _FP_ISSIGNAN (fs, wc, X))		\
			       ? FP_EX_INVALID_SNAN			\
			       : 0));					\
	}								\
      else								\
	{								\
	  /* The value is finite, with magnitude at least 1.  If	\
	     the conversion is unsigned, the value is positive.		\
	     If RSIGNED is not 2, the value does not definitely		\
	     overflow by virtue of its exponent, but may still turn	\
	     out to overflow after rounding; if RSIGNED is 2, the	\
	     exponent may be such that the value definitely overflows,	\
	     but at least one mantissa bit will not be shifted out.  */ \
	  int _FP_TO_INT_ROUND_inexact = 0;				\
	  _FP_FRAC_HIGH_RAW_##fs (X) |= _FP_IMPLBIT_##fs;		\
	  if (X##_e >= _FP_EXPBIAS_##fs + _FP_FRACBITS_##fs - 1)	\
	    {								\
	      /* The value is an integer, no rounding needed.  */	\
	      _FP_FRAC_ASSEMBLE_##wc ((r), X, (rsize));			\
	      (r) <<= X##_e - _FP_EXPBIAS_##fs - _FP_FRACBITS_##fs + 1; \
	    }								\
	  else								\
	    {								\
	      /* May need to shift in order to round (unless there	\
		 are exactly _FP_WORKBITS fractional bits already).  */	\
	      int _FP_TO_INT_ROUND_rshift				\
		= (_FP_FRACBITS_##fs + _FP_EXPBIAS_##fs			\
		   - 1 - _FP_WORKBITS - X##_e);				\
	      if (_FP_TO_INT_ROUND_rshift > 0)				\
		_FP_FRAC_SRS_##wc (X, _FP_TO_INT_ROUND_rshift,		\
				   _FP_WFRACBITS_##fs);			\
	      else if (_FP_TO_INT_ROUND_rshift < 0)			\
		_FP_FRAC_SLL_##wc (X, -_FP_TO_INT_ROUND_rshift);	\
	      /* Round like _FP_ROUND, but setting			\
		 _FP_TO_INT_ROUND_inexact instead of directly setting	\
		 the "inexact" exception, since it may turn out we	\
		 should set "invalid" instead.  */			\
	      if (_FP_FRAC_LOW_##wc (X) & 7)				\
		{							\
		  _FP_TO_INT_ROUND_inexact = 1;				\
		  switch (FP_ROUNDMODE)					\
		    {							\
		    case FP_RND_NEAREST:				\
		      _FP_ROUND_NEAREST (wc, X);			\
		      break;						\
		    case FP_RND_ZERO:					\
		      _FP_ROUND_ZERO (wc, X);				\
		      break;						\
		    case FP_RND_PINF:					\
		      _FP_ROUND_PINF (wc, X);				\
		      break;						\
		    case FP_RND_MINF:					\
		      _FP_ROUND_MINF (wc, X);				\
		      break;						\
		    }							\
		}							\
	      _FP_FRAC_SRL_##wc (X, _FP_WORKBITS);			\
	      _FP_FRAC_ASSEMBLE_##wc ((r), X, (rsize));			\
	    }								\
	  if ((rsigned) != 0 && X##_s)					\
	    (r) = -(r);							\
	  /* An exponent of RSIZE - 1 always needs testing for		\
	     overflow (either directly overflowing, or overflowing	\
	     when rounding up results in 2^RSIZE).  An exponent of	\
	     RSIZE - 2 can overflow for positive values when rounding	\
	     up to 2^(RSIZE-1), but cannot overflow for negative	\
	     values.  Smaller exponents cannot overflow.  */		\
	  if (X##_e >= (_FP_EXPBIAS_##fs + (rsize) - 1			\
			- ((rsigned) > 0 && !X##_s)))			\
	    {								\
	      if (X##_e > _FP_EXPBIAS_##fs + (rsize) - 1		\
		  || (X##_e == _FP_EXPBIAS_##fs + (rsize) - 1		\
		      && (X##_s						\
			  ? (r) != (((typeof (r)) 1) << ((rsize) - 1))	\
			  : ((rsigned) > 0 || (r) == 0)))		\
		  || ((rsigned) > 0					\
		      && !X##_s						\
		      && X##_e == _FP_EXPBIAS_##fs + (rsize) - 2	\
		      && (r) == (((typeof (r)) 1) << ((rsize) - 1))))	\
		{							\
		  if ((rsigned) != 2)					\
		    {							\
		      if ((rsigned) != 0)				\
			{						\
			  (r) = 1;					\
			  (r) <<= (rsize) - 1;				\
			  (r) -= 1 - X##_s;				\
			}						\
		      else						\
			{						\
			  (r) = 0;					\
			  (r) = ~(r);					\
			}						\
		    }							\
		  _FP_TO_INT_ROUND_inexact = 0;				\
		  FP_SET_EXCEPTION (FP_EX_INVALID | FP_EX_INVALID_CVI);	\
		}							\
	    }								\
	  if (_FP_TO_INT_ROUND_inexact)					\
	    FP_SET_EXCEPTION (FP_EX_INEXACT);				\
	}								\
    _FP_TO_INT_ROUND_done: ;						\
    }									\
  while (0)

/* Convert integer to fp.  Output is raw.  RTYPE is unsigned even if
   input is signed.  */
#define _FP_FROM_INT(fs, wc, X, r, rsize, rtype)			\
  do									\
    {									\
      __label__ pack_semiraw;						\
      if (r)								\
	{								\
	  rtype _FP_FROM_INT_ur = (r);					\
									\
	  if ((X##_s = ((r) < 0)))					\
	    _FP_FROM_INT_ur = -_FP_FROM_INT_ur;				\
									\
	  _FP_STATIC_ASSERT ((rsize) <= 2 * _FP_W_TYPE_SIZE,		\
			     "rsize too large");			\
	  (void) (((rsize) <= _FP_W_TYPE_SIZE)				\
		  ? ({							\
		      int _FP_FROM_INT_lz;				\
		      __FP_CLZ (_FP_FROM_INT_lz,			\
				(_FP_W_TYPE) _FP_FROM_INT_ur);		\
		      X##_e = (_FP_EXPBIAS_##fs + _FP_W_TYPE_SIZE - 1	\
			       - _FP_FROM_INT_lz);			\
		    })							\
		  : ({						\
		      int _FP_FROM_INT_lz;				\
		      __FP_CLZ_2 (_FP_FROM_INT_lz,			\
				  (_FP_W_TYPE) (_FP_FROM_INT_ur		\
						>> _FP_W_TYPE_SIZE),	\
				  (_FP_W_TYPE) _FP_FROM_INT_ur);	\
		      X##_e = (_FP_EXPBIAS_##fs + 2 * _FP_W_TYPE_SIZE - 1 \
			       - _FP_FROM_INT_lz);			\
		    }));						\
									\
	  if ((rsize) - 1 + _FP_EXPBIAS_##fs >= _FP_EXPMAX_##fs		\
	      && X##_e >= _FP_EXPMAX_##fs)				\
	    {								\
	      /* Exponent too big; overflow to infinity.  (May also	\
		 happen after rounding below.)  */			\
	      _FP_OVERFLOW_SEMIRAW (fs, wc, X);				\
	      goto pack_semiraw;					\
	    }								\
									\
	  if ((rsize) <= _FP_FRACBITS_##fs				\
	      || X##_e < _FP_EXPBIAS_##fs + _FP_FRACBITS_##fs)		\
	    {								\
	      /* Exactly representable; shift left.  */			\
	      _FP_FRAC_DISASSEMBLE_##wc (X, _FP_FROM_INT_ur, (rsize));	\
	      if (_FP_EXPBIAS_##fs + _FP_FRACBITS_##fs - 1 - X##_e > 0)	\
		_FP_FRAC_SLL_##wc (X, (_FP_EXPBIAS_##fs			\
				       + _FP_FRACBITS_##fs - 1 - X##_e)); \
	    }								\
	  else								\
	    {								\
	      /* More bits in integer than in floating type; need to	\
		 round.  */						\
	      if (_FP_EXPBIAS_##fs + _FP_WFRACBITS_##fs - 1 < X##_e)	\
		_FP_FROM_INT_ur						\
		  = ((_FP_FROM_INT_ur >> (X##_e - _FP_EXPBIAS_##fs	\
					  - _FP_WFRACBITS_##fs + 1))	\
		     | ((_FP_FROM_INT_ur				\
			 << ((rsize) - (X##_e - _FP_EXPBIAS_##fs	\
					- _FP_WFRACBITS_##fs + 1)))	\
			!= 0));						\
	      _FP_FRAC_DISASSEMBLE_##wc (X, _FP_FROM_INT_ur, (rsize));	\
	      if ((_FP_EXPBIAS_##fs + _FP_WFRACBITS_##fs - 1 - X##_e) > 0) \
		_FP_FRAC_SLL_##wc (X, (_FP_EXPBIAS_##fs			\
				       + _FP_WFRACBITS_##fs - 1 - X##_e)); \
	      _FP_FRAC_HIGH_##fs (X) &= ~(_FP_W_TYPE) _FP_IMPLBIT_SH_##fs; \
	    pack_semiraw:						\
	      _FP_PACK_SEMIRAW (fs, wc, X);				\
	    }								\
	}								\
      else								\
	{								\
	  X##_s = 0;							\
	  X##_e = 0;							\
	  _FP_FRAC_SET_##wc (X, _FP_ZEROFRAC_##wc);			\
	}								\
    }									\
  while (0)


/* Extend from a narrower floating-point format to a wider one.  Input
   and output are raw.  If CHECK_NAN, then signaling NaNs are
   converted to quiet with the "invalid" exception raised; otherwise
   signaling NaNs remain signaling with no exception.  */
#define _FP_EXTEND_CNAN(dfs, sfs, dwc, swc, D, S, check_nan)		\
  do									\
    {									\
      _FP_STATIC_ASSERT (_FP_FRACBITS_##dfs >= _FP_FRACBITS_##sfs,	\
			 "destination mantissa narrower than source");	\
      _FP_STATIC_ASSERT ((_FP_EXPMAX_##dfs - _FP_EXPBIAS_##dfs		\
			  >= _FP_EXPMAX_##sfs - _FP_EXPBIAS_##sfs),	\
			 "destination max exponent smaller"		\
			 " than source");				\
      _FP_STATIC_ASSERT (((_FP_EXPBIAS_##dfs				\
			   >= (_FP_EXPBIAS_##sfs			\
			       + _FP_FRACBITS_##sfs - 1))		\
			  || (_FP_EXPBIAS_##dfs == _FP_EXPBIAS_##sfs)), \
			 "source subnormals do not all become normal,"	\
			 " but bias not the same");			\
      D##_s = S##_s;							\
      _FP_FRAC_COPY_##dwc##_##swc (D, S);				\
      if (_FP_EXP_NORMAL (sfs, swc, S))					\
	{								\
	  D##_e = S##_e + _FP_EXPBIAS_##dfs - _FP_EXPBIAS_##sfs;	\
	  _FP_FRAC_SLL_##dwc (D, (_FP_FRACBITS_##dfs - _FP_FRACBITS_##sfs)); \
	}								\
      else								\
	{								\
	  if (S##_e == 0)						\
	    {								\
	      _FP_CHECK_FLUSH_ZERO (sfs, swc, S);			\
	      if (_FP_FRAC_ZEROP_##swc (S))				\
		D##_e = 0;						\
	      else if (_FP_EXPBIAS_##dfs				\
		       < _FP_EXPBIAS_##sfs + _FP_FRACBITS_##sfs - 1)	\
		{							\
		  FP_SET_EXCEPTION (FP_EX_DENORM);			\
		  _FP_FRAC_SLL_##dwc (D, (_FP_FRACBITS_##dfs		\
					  - _FP_FRACBITS_##sfs));	\
		  D##_e = 0;						\
		  if (FP_TRAPPING_EXCEPTIONS & FP_EX_UNDERFLOW)		\
		    FP_SET_EXCEPTION (FP_EX_UNDERFLOW);			\
		}							\
	      else							\
		{							\
		  int FP_EXTEND_lz;					\
		  FP_SET_EXCEPTION (FP_EX_DENORM);			\
		  _FP_FRAC_CLZ_##swc (FP_EXTEND_lz, S);			\
		  _FP_FRAC_SLL_##dwc (D,				\
				      FP_EXTEND_lz + _FP_FRACBITS_##dfs	\
				      - _FP_FRACTBITS_##sfs);		\
		  D##_e = (_FP_EXPBIAS_##dfs - _FP_EXPBIAS_##sfs + 1	\
			   + _FP_FRACXBITS_##sfs - FP_EXTEND_lz);	\
		}							\
	    }								\
	  else								\
	    {								\
	      D##_e = _FP_EXPMAX_##dfs;					\
	      if (!_FP_FRAC_ZEROP_##swc (S))				\
		{							\
		  if (check_nan && _FP_FRAC_SNANP (sfs, S))		\
		    FP_SET_EXCEPTION (FP_EX_INVALID			\
				      | FP_EX_INVALID_SNAN);		\
		  _FP_FRAC_SLL_##dwc (D, (_FP_FRACBITS_##dfs		\
					  - _FP_FRACBITS_##sfs));	\
		  if (check_nan)					\
		    _FP_SETQNAN (dfs, dwc, D);				\
		}							\
	    }								\
	}								\
    }									\
  while (0)

#define FP_EXTEND(dfs, sfs, dwc, swc, D, S)		\
    _FP_EXTEND_CNAN (dfs, sfs, dwc, swc, D, S, 1)

/* Truncate from a wider floating-point format to a narrower one.
   Input and output are semi-raw.  */
#define FP_TRUNC(dfs, sfs, dwc, swc, D, S)				\
  do									\
    {									\
      _FP_STATIC_ASSERT (_FP_FRACBITS_##sfs >= _FP_FRACBITS_##dfs,	\
			 "destination mantissa wider than source");	\
      _FP_STATIC_ASSERT (((_FP_EXPBIAS_##sfs				\
			   >= (_FP_EXPBIAS_##dfs			\
			       + _FP_FRACBITS_##dfs - 1))		\
			  || _FP_EXPBIAS_##sfs == _FP_EXPBIAS_##dfs),	\
			 "source subnormals do not all become same,"	\
			 " but bias not the same");			\
      D##_s = S##_s;							\
      if (_FP_EXP_NORMAL (sfs, swc, S))					\
	{								\
	  D##_e = S##_e + _FP_EXPBIAS_##dfs - _FP_EXPBIAS_##sfs;	\
	  if (D##_e >= _FP_EXPMAX_##dfs)				\
	    _FP_OVERFLOW_SEMIRAW (dfs, dwc, D);				\
	  else								\
	    {								\
	      if (D##_e <= 0)						\
		{							\
		  if (D##_e < 1 - _FP_FRACBITS_##dfs)			\
		    {							\
		      _FP_FRAC_SET_##swc (S, _FP_ZEROFRAC_##swc);	\
		      _FP_FRAC_LOW_##swc (S) |= 1;			\
		    }							\
		  else							\
		    {							\
		      _FP_FRAC_HIGH_##sfs (S) |= _FP_IMPLBIT_SH_##sfs;	\
		      _FP_FRAC_SRS_##swc (S, (_FP_WFRACBITS_##sfs	\
					      - _FP_WFRACBITS_##dfs	\
					      + 1 - D##_e),		\
					  _FP_WFRACBITS_##sfs);		\
		    }							\
		  D##_e = 0;						\
		}							\
	      else							\
		_FP_FRAC_SRS_##swc (S, (_FP_WFRACBITS_##sfs		\
					- _FP_WFRACBITS_##dfs),		\
				    _FP_WFRACBITS_##sfs);		\
	      _FP_FRAC_COPY_##dwc##_##swc (D, S);			\
	    }								\
	}								\
      else								\
	{								\
	  if (S##_e == 0)						\
	    {								\
	      _FP_CHECK_FLUSH_ZERO (sfs, swc, S);			\
	      D##_e = 0;						\
	      if (_FP_FRAC_ZEROP_##swc (S))				\
		_FP_FRAC_SET_##dwc (D, _FP_ZEROFRAC_##dwc);		\
	      else							\
		{							\
		  FP_SET_EXCEPTION (FP_EX_DENORM);			\
		  if (_FP_EXPBIAS_##sfs					\
		      < _FP_EXPBIAS_##dfs + _FP_FRACBITS_##dfs - 1)	\
		    {							\
		      _FP_FRAC_SRS_##swc (S, (_FP_WFRACBITS_##sfs	\
					      - _FP_WFRACBITS_##dfs),	\
					  _FP_WFRACBITS_##sfs);		\
		      _FP_FRAC_COPY_##dwc##_##swc (D, S);		\
		    }							\
		  else							\
		    {							\
		      _FP_FRAC_SET_##dwc (D, _FP_ZEROFRAC_##dwc);	\
		      _FP_FRAC_LOW_##dwc (D) |= 1;			\
		    }							\
		}							\
	    }								\
	  else								\
	    {								\
	      D##_e = _FP_EXPMAX_##dfs;					\
	      if (_FP_FRAC_ZEROP_##swc (S))				\
		_FP_FRAC_SET_##dwc (D, _FP_ZEROFRAC_##dwc);		\
	      else							\
		{							\
		  _FP_CHECK_SIGNAN_SEMIRAW (sfs, swc, S);		\
		  _FP_FRAC_SRL_##swc (S, (_FP_WFRACBITS_##sfs		\
					  - _FP_WFRACBITS_##dfs));	\
		  _FP_FRAC_COPY_##dwc##_##swc (D, S);			\
		  /* Semi-raw NaN must have all workbits cleared.  */	\
		  _FP_FRAC_LOW_##dwc (D)				\
		    &= ~(_FP_W_TYPE) ((1 << _FP_WORKBITS) - 1);		\
		  _FP_SETQNAN_SEMIRAW (dfs, dwc, D);			\
		}							\
	    }								\
	}								\
    }									\
  while (0)

/* Truncate from a wider floating-point format to a narrower one.
   Input and output are cooked.  */
#define FP_TRUNC_COOKED(dfs, sfs, dwc, swc, D, S)			\
  do									\
    {									\
      _FP_STATIC_ASSERT (_FP_FRACBITS_##sfs >= _FP_FRACBITS_##dfs,	\
			 "destination mantissa wider than source");	\
      if (S##_c == FP_CLS_NAN)						\
	_FP_FRAC_SRL_##swc (S, (_FP_WFRACBITS_##sfs			\
				- _FP_WFRACBITS_##dfs));		\
      else								\
	_FP_FRAC_SRS_##swc (S, (_FP_WFRACBITS_##sfs			\
				- _FP_WFRACBITS_##dfs),			\
			    _FP_WFRACBITS_##sfs);			\
      _FP_FRAC_COPY_##dwc##_##swc (D, S);				\
      D##_e = S##_e;							\
      D##_c = S##_c;							\
      D##_s = S##_s;							\
    }									\
  while (0)

/* Helper primitives.  */

/* Count leading zeros in a word.  */

#ifndef __FP_CLZ
/* GCC 3.4 and later provide the builtins for us.  */
# define __FP_CLZ(r, x)							\
  do									\
    {									\
      _FP_STATIC_ASSERT ((sizeof (_FP_W_TYPE) == sizeof (unsigned int)	\
			  || (sizeof (_FP_W_TYPE)			\
			      == sizeof (unsigned long))		\
			  || (sizeof (_FP_W_TYPE)			\
			      == sizeof (unsigned long long))),		\
			 "_FP_W_TYPE size unsupported for clz");	\
      if (sizeof (_FP_W_TYPE) == sizeof (unsigned int))			\
	(r) = __builtin_clz (x);					\
      else if (sizeof (_FP_W_TYPE) == sizeof (unsigned long))		\
	(r) = __builtin_clzl (x);					\
      else /* sizeof (_FP_W_TYPE) == sizeof (unsigned long long).  */	\
	(r) = __builtin_clzll (x);					\
    }									\
  while (0)
#endif /* ndef __FP_CLZ */

#define _FP_DIV_HELP_imm(q, r, n, d)		\
  do						\
    {						\
      (q) = (n) / (d), (r) = (n) % (d);		\
    }						\
  while (0)


/* A restoring bit-by-bit division primitive.  */

#define _FP_DIV_MEAT_N_loop(fs, wc, R, X, Y)				\
  do									\
    {									\
      int _FP_DIV_MEAT_N_loop_count = _FP_WFRACBITS_##fs;		\
      _FP_FRAC_DECL_##wc (_FP_DIV_MEAT_N_loop_u);			\
      _FP_FRAC_DECL_##wc (_FP_DIV_MEAT_N_loop_v);			\
      _FP_FRAC_COPY_##wc (_FP_DIV_MEAT_N_loop_u, X);			\
      _FP_FRAC_COPY_##wc (_FP_DIV_MEAT_N_loop_v, Y);			\
      _FP_FRAC_SET_##wc (R, _FP_ZEROFRAC_##wc);				\
      /* Normalize _FP_DIV_MEAT_N_LOOP_U and _FP_DIV_MEAT_N_LOOP_V.  */	\
      _FP_FRAC_SLL_##wc (_FP_DIV_MEAT_N_loop_u, _FP_WFRACXBITS_##fs);	\
      _FP_FRAC_SLL_##wc (_FP_DIV_MEAT_N_loop_v, _FP_WFRACXBITS_##fs);	\
      /* First round.  Since the operands are normalized, either the	\
	 first or second bit will be set in the fraction.  Produce a	\
	 normalized result by checking which and adjusting the loop	\
	 count and exponent accordingly.  */				\
      if (_FP_FRAC_GE_1 (_FP_DIV_MEAT_N_loop_u, _FP_DIV_MEAT_N_loop_v))	\
	{								\
	  _FP_FRAC_SUB_##wc (_FP_DIV_MEAT_N_loop_u,			\
			     _FP_DIV_MEAT_N_loop_u,			\
			     _FP_DIV_MEAT_N_loop_v);			\
	  _FP_FRAC_LOW_##wc (R) |= 1;					\
	  _FP_DIV_MEAT_N_loop_count--;					\
	}								\
      else								\
	R##_e--;							\
      /* Subsequent rounds.  */						\
      do								\
	{								\
	  int _FP_DIV_MEAT_N_loop_msb					\
	    = (_FP_WS_TYPE) _FP_FRAC_HIGH_##wc (_FP_DIV_MEAT_N_loop_u) < 0; \
	  _FP_FRAC_SLL_##wc (_FP_DIV_MEAT_N_loop_u, 1);			\
	  _FP_FRAC_SLL_##wc (R, 1);					\
	  if (_FP_DIV_MEAT_N_loop_msb					\
	      || _FP_FRAC_GE_1 (_FP_DIV_MEAT_N_loop_u,			\
				_FP_DIV_MEAT_N_loop_v))			\
	    {								\
	      _FP_FRAC_SUB_##wc (_FP_DIV_MEAT_N_loop_u,			\
				 _FP_DIV_MEAT_N_loop_u,			\
				 _FP_DIV_MEAT_N_loop_v);		\
	      _FP_FRAC_LOW_##wc (R) |= 1;				\
	    }								\
	}								\
      while (--_FP_DIV_MEAT_N_loop_count > 0);				\
      /* If there's anything left in _FP_DIV_MEAT_N_LOOP_U, the result	\
	 is inexact.  */						\
      _FP_FRAC_LOW_##wc (R)						\
	|= !_FP_FRAC_ZEROP_##wc (_FP_DIV_MEAT_N_loop_u);		\
    }									\
  while (0)

#define _FP_DIV_MEAT_1_loop(fs, R, X, Y)  _FP_DIV_MEAT_N_loop (fs, 1, R, X, Y)
#define _FP_DIV_MEAT_2_loop(fs, R, X, Y)  _FP_DIV_MEAT_N_loop (fs, 2, R, X, Y)
#define _FP_DIV_MEAT_4_loop(fs, R, X, Y)  _FP_DIV_MEAT_N_loop (fs, 4, R, X, Y)

/******************************************libgcc op-common.h end******************************************/

/* Sigh.  Silly things longlong.h needs.  */
#define UWtype		_FP_W_TYPE
#define W_TYPE_SIZE	_FP_W_TYPE_SIZE

typedef int QItype __attribute__((mode(QI)));
typedef int SItype __attribute__((mode(SI)));
typedef int DItype __attribute__((mode(DI)));
typedef unsigned int UQItype __attribute__((mode(QI)));
typedef unsigned int USItype __attribute__((mode(SI)));
typedef unsigned int UDItype __attribute__((mode(DI)));
#if _FP_W_TYPE_SIZE == 32
typedef unsigned int UHWtype __attribute__((mode(HI)));
#elif _FP_W_TYPE_SIZE == 64
typedef USItype UHWtype;
#endif

#ifndef CMPtype
# define CMPtype	int
#endif

#define SI_BITS		(__CHAR_BIT__ * (int) sizeof (SItype))
#define DI_BITS		(__CHAR_BIT__ * (int) sizeof (DItype))

#ifndef umul_ppmm
/******************************************libgcc longlong.h start******************************************/
#define __BITS4 (W_TYPE_SIZE / 4)
#define __ll_B ((UWtype) 1 << (W_TYPE_SIZE / 2))
#define __ll_lowpart(t) ((UWtype) (t) & (__ll_B - 1))
#define __ll_highpart(t) ((UWtype) (t) >> (W_TYPE_SIZE / 2))

#ifndef W_TYPE_SIZE
#define W_TYPE_SIZE	32
#define UWtype		USItype
#define UHWtype		USItype
#define UDWtype		UDItype
#endif

/* Used in glibc only.  */
#ifndef attribute_hidden
#define attribute_hidden
#endif

extern const UQItype __clz_tab[256] attribute_hidden;



#if defined (__GNUC__) && !defined (NO_ASM)

/* We sometimes need to clobber "cc" with gcc2, but that would not be
   understood by gcc1.  Use cpp to avoid major code duplication.  */
#if __GNUC__ < 2
#define __CLOBBER_CC
#define __AND_CLOBBER_CC
#else /* __GNUC__ >= 2 */
#define __CLOBBER_CC : "cc"
#define __AND_CLOBBER_CC , "cc"
#endif /* __GNUC__ < 2 */

#if defined(__riscv)
#ifdef __riscv_mul
#define __umulsidi3(u,v) ((UDWtype)(UWtype)(u) * (UWtype)(v))
#define __muluw3(a, b) ((UWtype)(a) * (UWtype)(b))
#else
#if __riscv_xlen == 32
#define MULUW3 "call __mulsi3"
#elif __riscv_xlen == 64
#define MULUW3 "call __muldi3"
#else
#error unsupport xlen
#endif /* __riscv_xlen */
/* We rely on the fact that MULUW3 doesn't clobber the t-registers.
   It can get better register allocation result.  */
#define __muluw3(a, b) \
  ({ \
    register UWtype __op0 asm ("a0") = a; \
    register UWtype __op1 asm ("a1") = b; \
    asm volatile (MULUW3 \
                  : "+r" (__op0), "+r" (__op1) \
                  : \
                  : "ra", "a2", "a3"); \
    __op0; \
  })
#endif /* __riscv_mul */
#define umul_ppmm(w1, w0, u, v) \
  do { \
    UWtype __x0, __x1, __x2, __x3; \
    UHWtype __ul, __vl, __uh, __vh; \
 \
    __ul = __ll_lowpart (u); \
    __uh = __ll_highpart (u); \
    __vl = __ll_lowpart (v); \
    __vh = __ll_highpart (v); \
 \
    __x0 = __muluw3 (__ul, __vl); \
    __x1 = __muluw3 (__ul, __vh); \
    __x2 = __muluw3 (__uh, __vl); \
    __x3 = __muluw3 (__uh, __vh); \
 \
    __x1 += __ll_highpart (__x0);/* this can't give carry */ \
    __x1 += __x2; /* but this indeed can */ \
    if (__x1 < __x2) /* did we get it? */ \
      __x3 += __ll_B; /* yes, add it in the proper pos.  */ \
 \
    (w1) = __x3 + __ll_highpart (__x1); \
    (w0) = __ll_lowpart (__x1) * __ll_B + __ll_lowpart (__x0); \
  } while (0)
#endif /* __riscv */


#endif /* __GNUC__ */

/* If this machine has no inline assembler, use C macros.  */

#if !defined (add_ssaaaa)
#define add_ssaaaa(sh, sl, ah, al, bh, bl) \
  do {									\
    UWtype __x;								\
    __x = (al) + (bl);							\
    (sh) = (ah) + (bh) + (__x < (al));					\
    (sl) = __x;								\
  } while (0)
#endif

#if !defined (sub_ddmmss)
#define sub_ddmmss(sh, sl, ah, al, bh, bl) \
  do {									\
    UWtype __x;								\
    __x = (al) - (bl);							\
    (sh) = (ah) - (bh) - (__x > (al));					\
    (sl) = __x;								\
  } while (0)
#endif


/* Define this unconditionally, so it can be used for debugging.  */
#define __udiv_qrnnd_c(q, r, n1, n0, d) \
  do {									\
    UWtype __d1, __d0, __q1, __q0;					\
    UWtype __r1, __r0, __m;						\
    __d1 = __ll_highpart (d);						\
    __d0 = __ll_lowpart (d);						\
									\
    __r1 = (n1) % __d1;							\
    __q1 = (n1) / __d1;							\
    __m = (UWtype) __q1 * __d0;						\
    __r1 = __r1 * __ll_B | __ll_highpart (n0);				\
    if (__r1 < __m)							\
      {									\
	__q1--, __r1 += (d);						\
	if(__r1 >= (d)) /* i.e. we didn't get carry when adding to __r1 */\
	if(__r1 < __m)						\
	__q1--, __r1 += (d);					\
	}									\
	__r1 -= __m;							\
	\
	__r0 = __r1 % __d1;							\
	__q0 = __r1 / __d1;							\
	__m = (UWtype)__q0 * __d0;						\
	__r0 = __r0 * __ll_B | __ll_lowpart(n0);				\
	if(__r0 < __m)							\
	{									\
	__q0--, __r0 += (d);						\
	if(__r0 >= (d))						\
		if(__r0 < __m)						\
			__q0--, __r0 += (d);					\
	}									\
	__r0 -= __m;							\
			\
			(q) = (UWtype)__q1 * __ll_B | __q0;				\
			(r) = __r0;								\
	} while(0)

  /* If the processor has no udiv_qrnnd but sdiv_qrnnd, go through
	 __udiv_w_sdiv (defined in libgcc or elsewhere).  */
#if !defined (udiv_qrnnd) && defined (sdiv_qrnnd)
#define udiv_qrnnd(q, r, nh, nl, d) \
  do {									\
    extern UWtype __udiv_w_sdiv (UWtype *, UWtype, UWtype, UWtype);	\
    UWtype __r;								\
    (q) = __udiv_w_sdiv (&__r, nh, nl, d);				\
    (r) = __r;								\
  } while (0)
#endif

/* If udiv_qrnnd was not defined for this processor, use __udiv_qrnnd_c.  */
#if !defined (udiv_qrnnd)
#define UDIV_NEEDS_NORMALIZATION 1
#define udiv_qrnnd __udiv_qrnnd_c
#endif

#if !defined (count_leading_zeros)
#define count_leading_zeros(count, x) \
  do {									\
    UWtype __xr = (x);							\
    UWtype __a;								\
									\
    if (W_TYPE_SIZE <= 32)						\
      {									\
	__a = __xr < ((UWtype)1<<2*__BITS4)				\
	  ? (__xr < ((UWtype)1<<__BITS4) ? 0 : __BITS4)			\
	  : (__xr < ((UWtype)1<<3*__BITS4) ?  2*__BITS4 : 3*__BITS4);	\
      }									\
    else								\
      {									\
	for (__a = W_TYPE_SIZE - 8; __a > 0; __a -= 8)			\
	  if (((__xr >> __a) & 0xff) != 0)				\
	    break;							\
      }									\
									\
    (count) = W_TYPE_SIZE - (__clz_tab[__xr >> __a] + __a);		\
  } while (0)
#define COUNT_LEADING_ZEROS_0 W_TYPE_SIZE
#endif

#if !defined (count_trailing_zeros)
/* Define count_trailing_zeros using count_leading_zeros.  The latter might be
   defined in asm, but if it is not, the C version above is good enough.  */
#define count_trailing_zeros(count, x) \
  do {									\
    UWtype __ctz_x = (x);						\
    UWtype __ctz_c;							\
    count_leading_zeros (__ctz_c, __ctz_x & -__ctz_x);			\
    (count) = W_TYPE_SIZE - 1 - __ctz_c;				\
  } while (0)
#endif

#ifndef UDIV_NEEDS_NORMALIZATION
#define UDIV_NEEDS_NORMALIZATION 0
#endif
/******************************************libgcc longlong.h end******************************************/
#endif
/******************************************libgcc soft-fp.h end******************************************/
/******************************************libgcc single.h start******************************************/
#if _FP_W_TYPE_SIZE < 32
# error "Here's a nickel kid.  Go buy yourself a real computer."
#endif

#define _FP_FRACTBITS_S		_FP_W_TYPE_SIZE

#if _FP_W_TYPE_SIZE < 64
# define _FP_FRACTBITS_DW_S	(2 * _FP_W_TYPE_SIZE)
#else
# define _FP_FRACTBITS_DW_S	_FP_W_TYPE_SIZE
#endif

#define _FP_FRACBITS_S		24
#define _FP_FRACXBITS_S		(_FP_FRACTBITS_S - _FP_FRACBITS_S)
#define _FP_WFRACBITS_S		(_FP_WORKBITS + _FP_FRACBITS_S)
#define _FP_WFRACXBITS_S	(_FP_FRACTBITS_S - _FP_WFRACBITS_S)
#define _FP_EXPBITS_S		8
#define _FP_EXPBIAS_S		127
#define _FP_EXPMAX_S		255
#define _FP_QNANBIT_S		((_FP_W_TYPE) 1 << (_FP_FRACBITS_S-2))
#define _FP_QNANBIT_SH_S	((_FP_W_TYPE) 1 << (_FP_FRACBITS_S-2+_FP_WORKBITS))
#define _FP_IMPLBIT_S		((_FP_W_TYPE) 1 << (_FP_FRACBITS_S-1))
#define _FP_IMPLBIT_SH_S	((_FP_W_TYPE) 1 << (_FP_FRACBITS_S-1+_FP_WORKBITS))
#define _FP_OVERFLOW_S		((_FP_W_TYPE) 1 << (_FP_WFRACBITS_S))

#define _FP_WFRACBITS_DW_S	(2 * _FP_WFRACBITS_S)
#define _FP_WFRACXBITS_DW_S	(_FP_FRACTBITS_DW_S - _FP_WFRACBITS_DW_S)
#define _FP_HIGHBIT_DW_S	\
  ((_FP_W_TYPE) 1 << (_FP_WFRACBITS_DW_S - 1) % _FP_W_TYPE_SIZE)

/* The implementation of _FP_MUL_MEAT_S and _FP_DIV_MEAT_S should be
   chosen by the target machine.  */

typedef float SFtype __attribute__((mode(SF)));

union _FP_UNION_S
{
	SFtype flt;
	uint32_t num;
	struct _FP_STRUCT_LAYOUT
	{
		unsigned frac : _FP_FRACBITS_S - (_FP_IMPLBIT_S != 0);
		unsigned exp : _FP_EXPBITS_S;
		unsigned sign : 1;
	} bits;
};

#define FP_DECL_S(X)		_FP_DECL (1, X)
#define FP_UNPACK_RAW_S(X, val)	_FP_UNPACK_RAW_1 (S, X, (val))
#define FP_UNPACK_RAW_SP(X, val)	_FP_UNPACK_RAW_1_P (S, X, (val))
#define FP_PACK_RAW_S(val, X)	_FP_PACK_RAW_1 (S, (val), X)
#define FP_PACK_RAW_SP(val, X)			\
  do						\
    {						\
      if (!FP_INHIBIT_RESULTS)			\
	_FP_PACK_RAW_1_P (S, (val), X);		\
    }						\
  while (0)

#define FP_UNPACK_S(X, val)			\
  do						\
    {						\
      _FP_UNPACK_RAW_1 (S, X, (val));		\
      _FP_UNPACK_CANONICAL (S, 1, X);		\
    }						\
  while (0)

#define FP_UNPACK_SP(X, val)			\
  do						\
    {						\
      _FP_UNPACK_RAW_1_P (S, X, (val));		\
      _FP_UNPACK_CANONICAL (S, 1, X);		\
    }						\
  while (0)

#define FP_UNPACK_SEMIRAW_S(X, val)		\
  do						\
    {						\
      _FP_UNPACK_RAW_1 (S, X, (val));		\
      _FP_UNPACK_SEMIRAW (S, 1, X);		\
    }						\
  while (0)

#define FP_UNPACK_SEMIRAW_SP(X, val)		\
  do						\
    {						\
      _FP_UNPACK_RAW_1_P (S, X, (val));		\
      _FP_UNPACK_SEMIRAW (S, 1, X);		\
    }						\
  while (0)

#define FP_PACK_S(val, X)			\
  do						\
    {						\
      _FP_PACK_CANONICAL (S, 1, X);		\
      _FP_PACK_RAW_1 (S, (val), X);		\
    }						\
  while (0)

#define FP_PACK_SP(val, X)			\
  do						\
    {						\
      _FP_PACK_CANONICAL (S, 1, X);		\
      if (!FP_INHIBIT_RESULTS)			\
	_FP_PACK_RAW_1_P (S, (val), X);		\
    }						\
  while (0)

#define FP_PACK_SEMIRAW_S(val, X)		\
  do						\
    {						\
      _FP_PACK_SEMIRAW (S, 1, X);		\
      _FP_PACK_RAW_1 (S, (val), X);		\
    }						\
  while (0)

#define FP_PACK_SEMIRAW_SP(val, X)		\
  do						\
    {						\
      _FP_PACK_SEMIRAW (S, 1, X);		\
      if (!FP_INHIBIT_RESULTS)			\
	_FP_PACK_RAW_1_P (S, (val), X);		\
    }						\
  while (0)

#define FP_ISSIGNAN_S(X)		_FP_ISSIGNAN (S, 1, X)
#define FP_NEG_S(R, X)			_FP_NEG (S, 1, R, X)
#define FP_ADD_S(R, X, Y)		_FP_ADD (S, 1, R, X, Y)
#define FP_SUB_S(R, X, Y)		_FP_SUB (S, 1, R, X, Y)
#define FP_MUL_S(R, X, Y)		_FP_MUL (S, 1, R, X, Y)
#define FP_DIV_S(R, X, Y)		_FP_DIV (S, 1, R, X, Y)
#define FP_SQRT_S(R, X)			_FP_SQRT (S, 1, R, X)
#define _FP_SQRT_MEAT_S(R, S, T, X, Q)	_FP_SQRT_MEAT_1 (R, S, T, X, (Q))

#if _FP_W_TYPE_SIZE < 64
# define FP_FMA_S(R, X, Y, Z)	_FP_FMA (S, 1, 2, R, X, Y, Z)
#else
# define FP_FMA_S(R, X, Y, Z)	_FP_FMA (S, 1, 1, R, X, Y, Z)
#endif

#define FP_CMP_S(r, X, Y, un, ex)	_FP_CMP (S, 1, (r), X, Y, (un), (ex))
#define FP_CMP_EQ_S(r, X, Y, ex)	_FP_CMP_EQ (S, 1, (r), X, Y, (ex))
#define FP_CMP_UNORD_S(r, X, Y, ex)	_FP_CMP_UNORD (S, 1, (r), X, Y, (ex))

#define FP_TO_INT_S(r, X, rsz, rsg)	_FP_TO_INT (S, 1, (r), X, (rsz), (rsg))
#define FP_TO_INT_ROUND_S(r, X, rsz, rsg)	\
  _FP_TO_INT_ROUND (S, 1, (r), X, (rsz), (rsg))
#define FP_FROM_INT_S(X, r, rs, rt)	_FP_FROM_INT (S, 1, X, (r), (rs), rt)

#define _FP_FRAC_HIGH_S(X)	_FP_FRAC_HIGH_1 (X)
#define _FP_FRAC_HIGH_RAW_S(X)	_FP_FRAC_HIGH_1 (X)

#if _FP_W_TYPE_SIZE < 64
# define _FP_FRAC_HIGH_DW_S(X)	_FP_FRAC_HIGH_2 (X)
#else
# define _FP_FRAC_HIGH_DW_S(X)	_FP_FRAC_HIGH_1 (X)
#endif

/******************************************libgcc single.h end******************************************/
/******************************************libgcc double.h start******************************************/
#if _FP_W_TYPE_SIZE < 32
# error "Here's a nickel kid.  Go buy yourself a real computer."
#endif

#if _FP_W_TYPE_SIZE < 64
# define _FP_FRACTBITS_D	(2 * _FP_W_TYPE_SIZE)
# define _FP_FRACTBITS_DW_D	(4 * _FP_W_TYPE_SIZE)
#else
# define _FP_FRACTBITS_D	_FP_W_TYPE_SIZE
# define _FP_FRACTBITS_DW_D	(2 * _FP_W_TYPE_SIZE)
#endif

#define _FP_FRACBITS_D		53
#define _FP_FRACXBITS_D		(_FP_FRACTBITS_D - _FP_FRACBITS_D)
#define _FP_WFRACBITS_D		(_FP_WORKBITS + _FP_FRACBITS_D)
#define _FP_WFRACXBITS_D	(_FP_FRACTBITS_D - _FP_WFRACBITS_D)
#define _FP_EXPBITS_D		11
#define _FP_EXPBIAS_D		1023
#define _FP_EXPMAX_D		2047

#define _FP_QNANBIT_D		\
	((_FP_W_TYPE) 1 << (_FP_FRACBITS_D-2) % _FP_W_TYPE_SIZE)
#define _FP_QNANBIT_SH_D		\
	((_FP_W_TYPE) 1 << (_FP_FRACBITS_D-2+_FP_WORKBITS) % _FP_W_TYPE_SIZE)
#define _FP_IMPLBIT_D		\
	((_FP_W_TYPE) 1 << (_FP_FRACBITS_D-1) % _FP_W_TYPE_SIZE)
#define _FP_IMPLBIT_SH_D		\
	((_FP_W_TYPE) 1 << (_FP_FRACBITS_D-1+_FP_WORKBITS) % _FP_W_TYPE_SIZE)
#define _FP_OVERFLOW_D		\
	((_FP_W_TYPE) 1 << _FP_WFRACBITS_D % _FP_W_TYPE_SIZE)

#define _FP_WFRACBITS_DW_D	(2 * _FP_WFRACBITS_D)
#define _FP_WFRACXBITS_DW_D	(_FP_FRACTBITS_DW_D - _FP_WFRACBITS_DW_D)
#define _FP_HIGHBIT_DW_D	\
  ((_FP_W_TYPE) 1 << (_FP_WFRACBITS_DW_D - 1) % _FP_W_TYPE_SIZE)

typedef float DFtype __attribute__((mode(DF)));
// typedef float XFtype __attribute__ ((mode (XF)));
typedef float TFtype __attribute__((mode(TF)));

#if _FP_W_TYPE_SIZE < 64

union _FP_UNION_D
{
	DFtype flt;
	uint64_t num;
	struct _FP_STRUCT_LAYOUT
	{
		unsigned frac0 : _FP_W_TYPE_SIZE;
		unsigned frac1 : _FP_FRACBITS_D - (_FP_IMPLBIT_D != 0) - _FP_W_TYPE_SIZE;
		unsigned exp : _FP_EXPBITS_D;
		unsigned sign : 1;
	} bits;
};

# define FP_DECL_D(X)		_FP_DECL (2, X)
# define FP_UNPACK_RAW_D(X, val)	_FP_UNPACK_RAW_2 (D, X, (val))
# define FP_UNPACK_RAW_DP(X, val)	_FP_UNPACK_RAW_2_P (D, X, (val))
# define FP_PACK_RAW_D(val, X)	_FP_PACK_RAW_2 (D, (val), X)
# define FP_PACK_RAW_DP(val, X)			\
  do						\
    {						\
      if (!FP_INHIBIT_RESULTS)			\
	_FP_PACK_RAW_2_P (D, (val), X);		\
    }						\
  while (0)

# define FP_UNPACK_D(X, val)			\
  do						\
    {						\
      _FP_UNPACK_RAW_2 (D, X, (val));		\
      _FP_UNPACK_CANONICAL (D, 2, X);		\
    }						\
  while (0)

# define FP_UNPACK_DP(X, val)			\
  do						\
    {						\
_FP_UNPACK_RAW_2_P(D, X, (val));		\
_FP_UNPACK_CANONICAL(D, 2, X);		\
  }						\
while(0)

# define FP_UNPACK_SEMIRAW_D(X, val)		\
do						\
{						\
_FP_UNPACK_RAW_2(D, X, (val));		\
_FP_UNPACK_SEMIRAW(D, 2, X);		\
}						\
while(0)

# define FP_UNPACK_SEMIRAW_DP(X, val)		\
  do						\
    {						\
      _FP_UNPACK_RAW_2_P (D, X, (val));		\
      _FP_UNPACK_SEMIRAW (D, 2, X);		\
    }						\
  while (0)

# define FP_PACK_D(val, X)			\
  do						\
    {						\
      _FP_PACK_CANONICAL (D, 2, X);		\
      _FP_PACK_RAW_2 (D, (val), X);		\
    }						\
  while (0)

# define FP_PACK_DP(val, X)			\
  do						\
    {						\
      _FP_PACK_CANONICAL (D, 2, X);		\
      if (!FP_INHIBIT_RESULTS)			\
	_FP_PACK_RAW_2_P (D, (val), X);		\
    }						\
  while (0)

# define FP_PACK_SEMIRAW_D(val, X)		\
  do						\
    {						\
      _FP_PACK_SEMIRAW (D, 2, X);		\
      _FP_PACK_RAW_2 (D, (val), X);		\
    }						\
  while (0)

# define FP_PACK_SEMIRAW_DP(val, X)		\
  do						\
    {						\
      _FP_PACK_SEMIRAW (D, 2, X);		\
      if (!FP_INHIBIT_RESULTS)			\
	_FP_PACK_RAW_2_P (D, (val), X);		\
    }						\
  while (0)

# define FP_ISSIGNAN_D(X)		_FP_ISSIGNAN (D, 2, X)
# define FP_NEG_D(R, X)			_FP_NEG (D, 2, R, X)
# define FP_ADD_D(R, X, Y)		_FP_ADD (D, 2, R, X, Y)
# define FP_SUB_D(R, X, Y)		_FP_SUB (D, 2, R, X, Y)
# define FP_MUL_D(R, X, Y)		_FP_MUL (D, 2, R, X, Y)
# define FP_DIV_D(R, X, Y)		_FP_DIV (D, 2, R, X, Y)
# define FP_SQRT_D(R, X)		_FP_SQRT (D, 2, R, X)
# define _FP_SQRT_MEAT_D(R, S, T, X, Q)	_FP_SQRT_MEAT_2 (R, S, T, X, (Q))
# define FP_FMA_D(R, X, Y, Z)		_FP_FMA (D, 2, 4, R, X, Y, Z)

# define FP_CMP_D(r, X, Y, un, ex)	_FP_CMP (D, 2, (r), X, Y, (un), (ex))
# define FP_CMP_EQ_D(r, X, Y, ex)	_FP_CMP_EQ (D, 2, (r), X, Y, (ex))
# define FP_CMP_UNORD_D(r, X, Y, ex)	_FP_CMP_UNORD (D, 2, (r), X, Y, (ex))

# define FP_TO_INT_D(r, X, rsz, rsg)	_FP_TO_INT (D, 2, (r), X, (rsz), (rsg))
# define FP_TO_INT_ROUND_D(r, X, rsz, rsg)	\
  _FP_TO_INT_ROUND (D, 2, (r), X, (rsz), (rsg))
# define FP_FROM_INT_D(X, r, rs, rt)	_FP_FROM_INT (D, 2, X, (r), (rs), rt)

# define _FP_FRAC_HIGH_D(X)	_FP_FRAC_HIGH_2 (X)
# define _FP_FRAC_HIGH_RAW_D(X)	_FP_FRAC_HIGH_2 (X)

# define _FP_FRAC_HIGH_DW_D(X)	_FP_FRAC_HIGH_4 (X)
#endif /* W_TYPE_SIZE < 64 */

/******************************************libgcc double.h end******************************************/
typedef int cmp_return_type __attribute__((mode(__libgcc_cmp_return__)));
typedef int shift_count_type __attribute__((mode(__libgcc_shift_count__)));
struct DWstruct
{
	SItype low, high;
};
// #define W_TYPE_SIZE (4 * __CHAR_BIT__)
#define Wtype	SItype
// #define UWtype	USItype
#define HWtype	SItype
#define UHWtype	USItype
#define DWtype	int64_t
#define UDWtype	u_int64_t
// #define DWtype	//DItype
// #define UDWtype	UDItype
# define Wtype_MAXp1_F	0x1p32f
typedef union
{
	struct DWstruct s;
	DWtype ll;
} DWunion;
#endif