/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-12-30 14:37:03
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
#include <RISCV.H>
/*********************************INTERRUPT_VECTOR_TABLE.S*********************************/
	.section .vtable
    .globl vector_base
    //定义中断向量表
vector_base:
    .word  CPUS_HANDLER//0//software error
    .word  CPUT_HANDLER//1//time error  
    .word  CPUR_HANDLER//2//reg error
    .word  SWUC_HANDLER//3//WU26
    .word  LPC_ESPI_RST_HANDLER//4
    .word  PWRSW_HANDLER//5
    .word  PS2_0_HANDLER//6//ps2 mouse
    .word  KBS_SDV_HANDLER//7
    .word  KBS_PRESS_HANDLER//8
    .word  TACH0_HANDLER//9
    .word  TACH1_HANDLER//10
    .word  TACH2_HANDLER//11
    .word  TACH3_HANDLER//12
    .word  KBC_IBF_HANDLER//13
    .word  KBC_OBE_HANDLER//14
    .word  PMC1_IBF_HANDLER//15
    .word  PMC1_OBE_HANDLER//16
    .word  WU42_HANDLER//17
    .word  RTC_HANDLER//18
    .word  WDT_HANDLER//19
    .word  ADC_HANDLER//20
    .word  UART0_HANDLER//21
    .word  UARTA_HANDLER//22
    .word  UARTB_HANDLER//23
    .word  SM_EC_HANDLER//24
    .word  SM_HOST_HANDLER//25
    .word  TIMER0_HANDLER//26
    .word  TIMER1_HANDLER//27
    .word  TIMER2_HANDLER//28
    .word  TIMER3_HANDLER//29
    .word  INTC0_HANDLER//30
    .word  INTC1_HANDLER//31
    j (trap_entry+0x80080-0x30880)//j trap_entry//跳转硬件错误处理函数//由于IVT导致此处偏移有问题；
	/*原计算公式：offset = trap_entr -0x80080
	需求计算公式：offset = trap_entr -0x30880
	因此推导出需求关系和原公式关系可用：offset =trap_entr-0x30880+0x80080-0x80080= （trap_entr+ 0x80080-0x30880）-0x80080
	*/
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
    csrc CSR_MSTATUS, MSTATUS_MIE//中断全局关闭
    csrsi CSR_MCOUNTINHIBIT, 0x5//关闭全局计数
#if 1//ROM可能没复位CACHE导致异常，所以使复位CACHE，保障正常启动,后续如果新版本ROM可以删除此行
	la a0,0xBFFFF//复位CACHE
	sb zero,0(a0)//复位CACHE
#endif
    la sp, _sp //将标签_sp所处的地址赋值给sp寄存器
    //注意：标签_sp在链接脚本中定义，参见链接脚本的_sp标签
    .option push
    .option norelax
    //设置全局指针
	la gp, __global_pointer$//将标签__global_pointer$所处的地址赋值给gp寄存器
	//注意：标签__global_pointer在链接脚本中定义参见链接脚本的__global_pointer$标签
    .option pop
    //设置堆栈指针
    
    call __scatterload_ram//将FLASH中指定的内容拷贝到RAM中
	call Init//调用Init
	call main //调用main函数，开始执行main函数
	tail exit//如果完成了main函数后，调用exit函数（Newlib桩函数之一，参见第2.3.1
            //节了解Newlib桩函数的更多信息）
1:
	j 1b //最后的死循环，程序理论上不可能执行到此处
/*********************************START.S*********************************/
/*********************************SCATTERLOAD.S*********************************/
    .align 2
	.section .scatterload//声明section段名.scatterload
	.globl __scatterload_ram//全局标签_start
	.type __scatterload_ram,@function//定义为函数
__scatterload_ram://将FLASH中指定的内容拷贝到RAM中
	 /* 使用与上述相同的原理，通过一个循环，将 数据从Flash中搬运到DLM中*/
    la a1,0x304C0
    lw a0,0(a1)
    andi a0,a0,0x8
    bnez a0,__scatterload_data  //若0x304C0的bit3为1，则表示为固件更新后的首次启动，不能清除EC SPACE和通用内存空间，需跳过此部分
__scatterload_globalmem://将EC_SPACE和通用内存空间的空间清零
2:	
    la a0, MEM_ADDR
	la a1, DATA_ADDR
	bgeu a0, a1, 2f
1:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
__scatterload_data: //将常量数据从flash搬运到ram中
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
__scatterload_bss://BSS段是链接器预留的未初始化变量所处的地址段，引导程序必须对其初始化为0
2:
	la a0, __bss_start
	la a1, _end
	bgeu a0, a1, 2f
1:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
__scatterload_shareram://将SRAM的空间清零
2:
    la a0, 0x31000
	la a1, 0x32000
	bgeu a0, a1, 2f
1:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
#if 1
__scatterload_ivt://赋值IVT的空间清零
2:
	la a0, vector_base
	la a1, 0x30800
	la a2, 0x30884
	bgeu a1, a2, 2f
1:
	lw t0, (a0)
	sw t0, (a1)
	addi a0, a0, 4
	addi a1, a1, 4
	bltu a1, a2, 1b
#endif
#if 0 //将FLASH中ROM代码拷贝到ROM中 ,暂时保留,测试代码,正式无效
#if (!defined(DUBLE_FIRMWARE2))   
__scatterload_rom:
2:
	la a0, 0xE0000//384k-416k作为rom代码段
    la a1, 0x10000//32k
	la a2, 0x18000
	bgeu a1, a2, 2f
1:
	lw t0, (a0)
	sw t0, (a1)
	addi a0, a0, 4
	addi a1, a1, 4
	bltu a1, a2, 1b
#endif
#endif
2:
    ret
/*********************************SCATTERLOAD.S*********************************/
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
