/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-09 11:23:14
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
#ifndef ENTRY_S
#define ENTRY_S
#include "RISCV.H"
###############################################
###############################################
# Disable Interrupt
#
.macro DISABLE_MIE
	csrc CSR_MSTATUS, MSTATUS_MIE
.endm
/*
 * x0 zero: zero register
 * x1 ra:         return address register
 * x2 sp:         stack pointer register
 * x3 gp:         global pointer register
 * x4 tp:         thread pointer register
 * x5 t0:         temporary / alternate link register
 * x6~x7 t1~t2:   temporaries
 * x8 s0/fp:      saved register/ frame pointer
 * x9 s1:         saved register
 * x10~x11 a0~a1: function arguments/return values
 * x12~x17 a2~a7: function arguments
 * x18~x27 s2~s11:saved register
 * x28~x31 t3~t6: temporaries
*/
###############################################
###############################################
#Save caller registers
//该宏用于保存ABI定义的“调用者应保持的寄存器（Caller saved register）”进入堆栈
.macro SAVE_CONTEXT
//更改堆栈指针，分配20个单字（32位）的空间用于保存寄存器
	addi sp, sp, -20*(1 << 2)
	sw x1, 0*(1 << 2)(sp)
	sw x4, 1*(1 << 2)(sp)
	sw x5, 2*(1 << 2)(sp)
	sw x6, 3*(1 << 2)(sp)
	sw x7, 4*(1 << 2)(sp)
	sw x10, 5*(1 << 2)(sp)
	sw x11, 6*(1 << 2)(sp)
	sw x12, 7*(1 << 2)(sp)
	sw x13, 8*(1 << 2)(sp)
	sw x14, 9*(1 << 2)(sp)
	sw x15, 10*(1 << 2)(sp)
# #ifndef __riscv_32e
#   sw x16, 11*(1 << 2)(sp)
# 	sw x17, 12*(1 << 2)(sp)
# 	sw x28, 13*(1 << 2)(sp)
# 	sw x29, 14*(1 << 2)(sp)
# 	sw x30, 15*(1 << 2)(sp)
# 	sw x31, 16*(1 << 2)(sp)
# #endif
.endm
###############################################
###############################################
#restore caller registers
//该宏用于从堆栈中恢复ABI定义的“调用者应保存的寄存器（Caller saved register）”
.macro RESTORE_CONTEXT
	lw x1, 0*(1 << 2)(sp)
	lw x4, 1*(1 << 2)(sp)
	lw x5, 2*(1 << 2)(sp)
	lw x6, 3*(1 << 2)(sp)
	lw x7, 4*(1 << 2)(sp)
	lw x10, 5*(1 << 2)(sp)
	lw x11, 6*(1 << 2)(sp)
	lw x12, 7*(1 << 2)(sp)
	lw x13, 8*(1 << 2)(sp)
	lw x14, 9*(1 << 2)(sp)
	lw x15, 10*(1 << 2)(sp)
//#ifndef __riscv_32e
//  lw x16, 11*(1 << 2)(sp)
//	lw x17, 12*(1 << 2)(sp)
//	lw x28, 13*(1 << 2)(sp)
//	lw x29, 14*(1 << 2)(sp)
//	lw x30, 15*(1 << 2)(sp)
//	lw x31, 16*(1 << 2)(sp)
//#endif
//De-allocate the stack space
	addi sp, sp, 20*(1 << 2)
//恢复寄存器之后，更改堆栈指针，回收19个单字（32位）的空间
//#endif
.endm
###############################################
###############################################
#restore caller registers
# .macro RESTORE_CONTEXT_EXCPT_X5
# 	lw x1,  0*(1 << 2)(sp)
# 	lw x6,  2*(1 << 2)(sp)
# 	lw x7,  3*(1 << 2)(sp)
# 	lw x10, 4*(1 << 2)(sp)
# 	lw x11, 5*(1 << 2)(sp)
# 	lw x12, 6*(1 << 2)(sp)
# 	lw x13, 7*(1 << 2)(sp)
# 	lw x14, 8*(1 << 2)(sp)
# 	lw x15, 9*(1 << 2)(sp)
# #ifndef __riscv_32e
# 	lw x16, 10*(1 << 2)(sp)
# 	lw x17, 11*(1 << 2)(sp)
# 	lw x28, 12*(1 << 2)(sp)
# 	lw x29, 13*(1 << 2)(sp)
# 	lw x30, 14*(1 << 2)(sp)
# 	lw x31, 15*(1 << 2)(sp)
# #endif
# .endm
###############################################
###############################################
#restore caller registers
# .macro RESTORE_CONTEXT_ONLY_X5
# 	lw x5,  1*(1 << 2)(sp)
# .endm
###############################################
###############################################
# Save the mepc and mstatus
#
//该宏用于保存MEPC和MSTATUS寄存器进入堆栈
.macro SAVE_EPC_STATUS
	csrr x5, CSR_MEPC
	sw x5,  16*(1 << 2)(sp)
	csrr x5, CSR_MSTATUS
	sw x5,  17*(1 << 2)(sp)
	csrr x5, CSR_MCAUSE
	sw x5,  18*(1 << 2)(sp)
.endm
###############################################
###############################################
# Restore the mepc and mstatus
#
//该宏用于从堆栈中恢复MEPC和MSTATUS寄存器
.macro RESTORE_EPC_STATUS
	lw x5,  16*(1 << 2)(sp)
	csrw CSR_MEPC, x5
	lw x5,  17*(1 << 2)(sp)
	csrw CSR_MSTATUS, x5
	lw x5,  18*(1 << 2)(sp)
	csrw CSR_MCAUSE, x5
.endm
###############################################
###############################################
#endif
#ifdef USER_AE10X_LIBC_A

# 		.global  memset
# 	.type memset, @function
# memset:
# 	j _memset
	
	.section      .text.riscv_sr_x	
	.align 2

	.global __riscv_save_2
	 .type __riscv_save_2,@function
__riscv_save_2:
	.global __riscv_save_1
	 .type __riscv_save_1,@function
__riscv_save_1:
	.global __riscv_save_0
	.type __riscv_save_0,@function
__riscv_save_0:
	.cfi_startproc
	# __riscv_save_* routine use t0/x5 as return address
	.cfi_return_column 5
	addi	sp,sp,-12//加载sp现场保存
	.cfi_def_cfa_offset 12
	sw	s1,0(sp)//保存s1，存储寄存器，切换上下文保存
	.cfi_offset 9, -12
	sw	s0,4(sp)//保存s0
	.cfi_offset 8, -8
	sw	ra,8(sp)//保存返回寄存器的值（保存返回位置）
	.cfi_offset 1, 0
	jr	t0
	.cfi_endproc
	.size __riscv_save_2,.-__riscv_save_2
	.size __riscv_save_1,.-__riscv_save_1
	.size __riscv_save_0,.-__riscv_save_0

	.global __riscv_restore_2
	.type __riscv_restore_2,@function
__riscv_restore_2:
	.global __riscv_restore_1
	.type __riscv_restore_1,@function
__riscv_restore_1:
	.global __riscv_restore_0
	.type __riscv_restore_0,@function
__riscv_restore_0://恢复现场
	.cfi_startproc
	.cfi_def_cfa_offset 14
	lw	s1,0(sp)
	.cfi_restore 9
	lw	s0,4(sp)
	.cfi_restore 8
	lw	ra,8(sp)
	.cfi_restore 1
	addi	sp,sp,12
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
	.size __riscv_restore_2,.-__riscv_restore_2
	.size __riscv_restore_1,.-__riscv_restore_1
	.size __riscv_restore_0,.-__riscv_restore_0


	.section      .text.riscv_operation	
	.align 4
//乘法
	.global __mulsi3
	.type __mulsi3,@function//定义为函数乘法
	//逻辑：如果低位为0则不变反之则加上对应的值
__mulsi3://A*B=C//a0=A,A2=B
	mv	a2,a0//a2=A
	li	a0,0//a0=0=C
.M1:
	andi	a3,a1,1//a3=B&1
	beqz	a3,.M2//if(a3==0)srli	a1,a1,0x1;else add	a0,a0,a2//a0=a0+a2
	add	a0,a0,a2//a0=a0+a2//
.M2:
	srli	a1,a1,0x1//a1>>1//B/=2
	slli	a2,a2,0x1//a2<<1//A*=2
	bnez	a1,.M1//if(a1!=0)andi	a3,a1,1; else ret
	ret
	.size __mulsi3,.-__mulsi3

//除法,求模
	.global __divsi3
	.type __divsi3,@function//定义为函数
__divsi3://signed  A/B
	bltz	a0,.L10//if(a0<0)neg	a0,a0,else bltz	a1,__divsi3+0x16//A<0则取其补码负数是按补码存储的，所以转为负数才能运算
	bltz	a1,.L11//if(a1<0)neg	a1,a1,else bltz	a1,__divsi3+0x16//A<0则取其补码
	/* Since the quotient is positive, fall into __udivdi3.  */

	.global __udivsi3
	.type __udivsi3,@function//定义为函数
__udivsi3://A/B=C//A=a0,B=a1
	mv	a2,a1//a2=B
	mv	a1,a0//a1=A
	li	a0,-1//a0=-1//0xffffffff
	beqz	a2,.L5//if(a2==0)ret(return -1);else  li	a3,1//除数B为0报错
	li	a3,1//a3=1
	bgeu	a2,a1,.L2//if(a2>=a1) li	a0,0 else blez	a2,__udivsi3+0x1a//除数B>=被除数A时C=0
.L1:
	blez	a2,.L2//if (a2<=0)li	a0,0 else slli	a2,a2,0x1//此处判断B是否被分析完，如果B=0则退出C=0
	slli	a2,a2,0x1//a2<<=1;//B*=2
	slli	a3,a3,0x1//a3<<=1;//a3*=2
	bgtu	a1,a2,.L1//if(a2<a1)blez	a2,__udivsi3+0x1a else a0=0、、此处判断Ba2要×多少个2的n次方才能超过被除数a3作为计数
.L2:
	li	a0,0//C=0
.L3:
	bltu	a1,a2,.L4//if(a1<a2) srli	a3,a3,0x1 else sub	a1,a1,a2 //如果a1<a2,此时无法再减去了故而跳过处理a3
	sub	a1,a1,a2//a1=a1-a2(A=A-B)
	or	a0,a0,a3//a0=a0|a3,C=C|a3//这里类似于填充运算，相当于通过不同数据的填充获取最大减的值
.L4:
	srli	a3,a3,0x1//a3>>1//B
	srli	a2,a2,0x1//a2>>1//B/2,相当于从最大值开始加入内容
	bnez	a3,.L3//if(a3!=0)bltu	a1,a2,__udivsi3+0x24,else ret//此处判断a3处理完成与否，错误的状态此时a3为0，否则则会在a3的计数结束后退出
.L5:
	ret
	.size __udivsi3,.-__udivsi3
	.globl __hidden___udivsi3;
	__hidden___udivsi3=__udivsi3;
	.hidden __hidden___udivsi3

	.global __umodsi3
	.type __umodsi3,@function//定义为函数
__umodsi3://A%B//a0=a,a1=B
	/* Call __udivdi3(a0, a1), then return the remainder, which is in a1.  */
	move	t0,ra//把返回地址调入t0保存
	jal	__hidden___udivsi3//跳转进行除法运算//返回a0为除数，a1即为余数
	move	a0,a1//把a1作为返回值到a0
	jr	t0//返回地址
	.size __umodsi3,.-__umodsi3

	/* Handle negative arguments to __divdi3.  */
.L10:
	neg	a0,a0//a0取饭吗（A=-A）
/* Zero is handled as a negative so that the result will not be inverted.  */
	bgtz	a1,.L12//if(a1>=0)mv	t0,ra else neg	a1,a1//如果B》=0则跳转无符号获取后再取补即为-A/B=-C,结果不变，

	neg	a1,a1//a1取反(B=-B),此时a1,a0都是正数
	j	__hidden___udivsi3//进行无符号运算，得到正数相除结果，后通过_udivsi3直接返回ra的地址（此处之后不再运行）
.L11:
	neg	a1,a1//a1取补B=-B
.L12:
	move	t0,ra//保留返回值
	jal	__hidden___udivsi3//跳转进行计算
	neg	a0,a0//对结果进行取反，同理B=0时候结果为-1，此时因为a0可能为负数-1，所以对报错-1没有绝对的办法，默认无处理
	jr	t0
	.size __divsi3,.-__divsi3

	.global __modsi3
	.type __modsi3,@function//定义为函数
__modsi3://signed A%B
	move	t0,ra
	bltz	a1,.L31//if(a1<0)neg	a1,a1 else bltz	a0,__modsi3+0x18//B<0
	bltz	a0,.L32//if(a0<0)neg	a0,a0 else jal	__udivsi3//A<0
.L30:
	jal	__hidden___udivsi3
	move	a0,a1//a0=余数
	jr	t0
.L31:
	neg	a1,a1//B=-B
	bgez	a0,.L30//if(a0>=0)jal	__udivsi3 else neg	a0,a0,此时相当于B的正负不会影像到余数
.L32:
	neg	a0,a0//a0取反
	jal	__hidden___udivsi3//求余数
	neg	a0,a1//a0=-a1
	jr	t0
	.size __modsi3,.-__modsi3

	.global __umoddi3
	.type __umoddi3,@function//定义为函数
__umoddi3:
	j umoddi3

	.global __umulsidi3
	.type __umulsidi3,@function//定义为函数
__umulsidi3:
	j umulsidi3
	
	.global __udivdi3
	.type __udivdi3,@function//定义为函数
__udivdi3:
	j udivdi3
	.global __divdi3
	.type __divdi3,@function//定义为函数
__divdi3:
	j divdi3

	.global __moddi3
	.type __moddi3,@function//定义为函数
__moddi3:
	j moddi3

	.global __muldi3
	.type __muldi3,@function//定义为函数
__muldi3:
	j muldi3

	.global __lshrdi3
	.type __lshrdi3,@function//定义为函数
__lshrdi3:
	j lshrdi3

.global  __ashldi3
	.type __ashldi3, @function
__ashldi3:
	j ashldi3
	.global __clzsi2
	.type __clzsi2, @function
__clzsi2:
	j clzsi2
	.global __clzdi2
	.type __clzdi2, @function
__clzdi2:
	j clzdi2
	
	.section      .riscv_asm_fp	
	.align 4
#if SOFT_SFP_DFP_CODE_SELECT//C代码

	.global __fixsfdi
	.type __fixsfdi, @function
__fixsfdi:
	j fixsfdi
	.global __fixunssfdi
	.type __fixunssfdi, @function
__fixunssfdi:
	j fixunssfdi
	.global __fixdfdi
	.type __fixdfdi, @function
__fixdfdi:
	j fixdfdi
# 	.global __fixunsdfdi
# 	.type __fixunsdfdi, @function
# __fixunsdfdi:
# 	j fixunsdfdi

	.global __fixsfsi
	.type __fixsfsi, @function
__fixsfsi:
	j fixsfsi
	.global  __fixunssfsi
	.type __fixunssfsi, @function
__fixunssfsi:
	j fixunssfsi
	.global  __fixdfsi
	.type __fixdfsi, @function
__fixdfsi:
	j fixdfsi
	.global  __fixunsdfsi
	.type __fixunsdfsi, @function
__fixunsdfsi:
	j fixunsdfsi
	.global  __fixunsdfdi
	.type __fixunsdfdi, @function
__fixunsdfdi:
	j fixunsdfdi


	.global __floatdidf
	.type __floatdidf, @function
__floatdidf:
	j floatdidf
	.global __floatdisf
	.type __floatdisf, @function
__floatdisf:
	j floatdisf
	.global __floatundidf
	.type __floatundidf, @function
__floatundidf:
	j floatundidf
	.global __floatundisf
	.type __floatundisf, @function
__floatundisf:
	j floatundisf

	.global __floatsisf
	.type __floatsisf, @function
__floatsisf:
	j floatsisf
	.global  __floatunsisf
	.type __floatunsisf, @function
__floatunsisf:
	j floatunsisf
	.global  __floatsidf
	.type __floatsidf, @function
__floatsidf:
	j floatsidf
	.global  __floatunsidf
	.type __floatunsidf, @function
__floatunsidf:
	j floatunsidf
	.global __addsf3
	.type __addsf3, @function
__addsf3:
	j addsf3
	.global  __subsf3
	.type __subsf3, @function
__subsf3:
	j subsf3
	.global  __mulsf3
	.type __mulsf3, @function
__mulsf3:
	j mulsf3
	.global  __divsf3
	.type __divsf3, @function
__divsf3:
	j divsf3
	.global __extendsfdf2
	.type __extendsfdf2, @function
__extendsfdf2:
	j extendsfdf2
	.global  __truncdfsf2
	.type __truncdfsf2, @function
__truncdfsf2:
	j truncdfsf2
	.global __adddf3
	.type __adddf3, @function
__adddf3:
	j adddf3
	.global  __subdf3
	.type __subdf3, @function
__subdf3:
	j subdf3
	.global  __muldf3
	.type __muldf3, @function
__muldf3:
	j muldf3
	.global  __divdf3
	.type __divdf3, @function
__divdf3:
	j divdf3
	.global __gedf2
	.type __gedf2, @function
__gedf2:
	j gedf2
	.global  __gesf2
	.type __gesf2, @function
__gesf2:
	j gesf2
	.global  __ledf2
	.type __ledf2, @function
__ledf2:
	j ledf2
	.global  __lesf2
	.type __lesf2, @function
__lesf2:
	j lesf2

	.global  __ltdf2
	.type __ltdf2, @function
__ltdf2:
	j ltdf2
		.global  __ltsf2
	.type __ltsf2, @function
__ltsf2:
	j ltsf2
#endif
#endif
