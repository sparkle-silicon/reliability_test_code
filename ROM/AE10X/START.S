/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-19 18:45:24
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
#include <RISCV.H>
/*********************************INTERRUPT_VECTOR_TABLE.S*********************************/
	.section .vtable
    .globl vector_base
    //定义中断向量表
vector_base:
    .word  0x10084//0
    .word  main//4
    .word  0//8
    .word  0//C
    .word  0//10
    .word  0//14
    .word  0//18
    .word  0//1C
    .word  0//20
    .word  0//24
    .word  0//28
    .word  0//2C
    .word  0//30
    .word  0//34
    .word  0//38
    .word  0//3C
    .word  0//40
    .word  0//44
    .word  0//48
    .word  0//4C
    .word  0//50
    .word  0//54
    .word  0//58
    .word  0//5C
    .word  0//60
    .word  0//64
    .word  0//68
    .word  0//6C
    .word  0//70
    .word  0//74
    .word  0//78
    .word  0//7C
    .word  0//80
/*********************************INTERRUPT_VECTOR_TABLE.S*********************************/
/*********************************START.S*********************************/
/****************************************************************************
* SPKAE10X start FLOW :
* 1. Hardware Initialization
* 2. Hardware Configuration CPU Program Counter(PC)
* 3. Software Turns Off CPU Counts And CPU Global Interrupts
* 4. Software Configuration Stack Pointers And Global Pointers
* 5. Software Copies SECTION .data To RAM Space
* 6. Software Initializes SECTION .bss To RAM Space
* 7. Software Initializes EC SPACE Values 0 To RAM Space
* 8. SPKAE10X Init Flow(ODM Custom)
* 9. SPKAE10X Main Flow(ODM Custom)
* 10.SPKAE10X Exit (while(1))
****************************************************************************/
    .align 2
	.section ._start//声明section段名.scatterload
	.globl _start//全局标签_start
	.type _start,@function//定义为函数
_start://标签位置
/*方案1汇编跳转   保护*/
//判断是否置位EFUSE data19（1：直接使用外部flash，0：正常流程）（同步sysctl 0x3041C bit4位）
    lui	a1,0x30
    # la a1, 0x3041C # SYSCTL_ESTAT
    # lw a0, 0(a1) # 从a1指向的地址加载数据到a0
    lw a0, 1052(a1) # 将地址0x3041C加载到a1
    andi a0, a0, 0x10 # 将a0与 bit4 按位与，检查特定位
    beqz a0, rom_start # 如果a0为零，跳转到rom_start，否则执行后续流程
//选择引脚
    # la a1,0x30460 #SYSCTL_PIO3_CFG
    # lw a0,0(a1) #读取寄存器
    lw a0,1120(a1) #读取寄存器0x30460
    li a2,0xffff00ff#加载立即数到a2（因为超过12位了）
    and a0,a0,a2 #按位与iomux清除piob20-23
    li a2,0x00005500#加载立即数到a2
    or a0,a0,a2 #按位或iomux切换piob20-23为spif四线
    # sw a0,0(a1) #写入寄存器
    sw a0,1120(a1) #写入寄存器0x30460
//选择外部flash
    # la a1,0x3047C #SYSCTL_PIO4_UDCFG
    # lw a0,0(a1) #读取寄存器
    lw a0,1148(a1) #读取寄存器0x3047C
    ori a0,a0,0x00000002 #按位或置位bit1
    # sw a0,0(a1) #写入寄存器
    sw a0,1148(a1) #写入寄存器0x3047C
//复位spif
    # la a1,0x3042C #SYSCTL_RST1
    # lw a0,0(a1) #读取寄存器
    lw a0,1068(a1) #读取寄存器0x3042C
    ori a0,a0,0x00000100 #按位或置位spif rst
    sw a0,0(a1) #写入寄存器
    nop#等待一个时钟
    andi a0,a0,0xfffffeff #按位与关闭spif rst
    # sw a0,0(a1) #写入寄存器
    sw a0,1068(a1) #写入寄存器0x3042C
    nop#等待一个时钟
//跳转
    j 0x80084 # 将地址0x80084加载到a1
rom_start:        
/*方案1汇编跳转   保护*/
    csrc CSR_MSTATUS, MSTATUS_MIE//中断全局关闭
    csrci CSR_MCOUNTINHIBIT, 0x5//开启CSR周期计数器
    //注意：标签_sp在链接脚本中定义，参见链接脚本的_sp标签
    .option push
    .option norelax
    //设置全局指针
	la gp, __global_pointer$//将标签__global_pointer$所处的地址赋值给gp寄存器
	//注意：标签__global_pointer在链接脚本中定义参见链接脚本的__global_pointer$标签
    .option pop
     //设置堆栈指针 
    la sp, _sp //将标签_sp所处的地址赋值给sp寄存器
2:
	la a0, _data_lma
	la a1, _data
	la a2, _edata
	bgeu a1, a2, 2f
1:
	lw t0, (a0)
	sw t0, (a1)
	addi a0, a0, 4
	addi a1, a1, 4
	bltu a1, a2, 1b
2:
	//BSS段是链接器预留的未初始化变量所处的地址段，引导程序必须对其初始化为0
	//此处通过一个循环来初始化BSS段
	la a0, __bss_start
	la a1, _end
	bgeu a0, a1, 2f
1:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
2:
	call Init//调用Init
	call main //调用main函数，开始执行main函数
	tail exit//如果完成了main函数后，调用exit函数（Newlib桩函数之一，参见第2.3.1
            //节了解Newlib桩函数的更多信息）
1:
	j 1b //最后的死循环，程序理论上不可能执行到此处
/*********************************START.S*********************************/
/*********************************CSR_CONFIG.S*********************************/
	.section .text.csr_config//声明section段名

	.global disable_mcycle_minstret
disable_mcycle_minstret: //用于控制计数器的关闭(低功耗考虑) 
        csrsi CSR_MCOUNTINHIBIT, 0x5
	ret
	.global enable_mcycle_minstret
enable_mcycle_minstret: //用于控制计数器的开启    
        csrci CSR_MCOUNTINHIBIT, 0x5
	ret
	.global core_wfe
core_wfe: //休眠时，Wait for Event
    	csrc CSR_MSTATUS, MSTATUS_MIE 
    	csrs CSR_WFE, 0x1
    	wfi   //WFI的休眠模式
    	csrc CSR_WFE, 0x1
    	csrs CSR_MSTATUS, MSTATUS_MIE
	ret
/*********************************CSR_CONFIG.S*********************************/
