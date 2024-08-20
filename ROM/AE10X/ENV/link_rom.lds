/*
riscv lds链接参考：
Dawes LinYu:
https://www.cnblogs.com/wahahahehehe/p/15149960.html
https://zhuanlan.zhihu.com/p/529271196?utm_id=0
https://nucleisys.com/product/n100/6_sdk/?v=20210901401#233
data ： 2023.1.30
*/
/*采用riscv架构体系*/
OUTPUT_ARCH( "riscv" )
/*代码入口地址定位为_start(位于START.s文件夹内标注)*/
ENTRY( vector_base )
/*--------------------------配置RAM内存空间分配--------------------------------*/
/*DRAM总大小及起始地址*/
"DRAM_ADDR"=0x20000;
"DRAM_SIZE"=32K;
/**************DRAM的内存分配**************/
/*c代码中公共内存分配*/
"MEM_SIZE"=4K;/*2K的ec space不允许操作，2k的rom space用作rom空间*/
/*data段内存分配*/
"DATA_SIZE"=12K;
/*bss段内存分配*/
"BSS_SIZE"=12K;
"HEAP_SIZE"=2k;
/**************IRAM的内存分配**************/
"IRAM0_ADDR"=0x28000;
"IRAM0_SIZE"=32K;
"IRAM1_ADDR"=0x34000;
"IRAM1_SIZE"=8K;

/*栈stack分配，如果没有定义stack大小则默认2k*/
__stack_size = DEFINED(__stack_size) ? __stack_size : 2K;
"STACK_SIZE"=__stack_size;
/**************动态内存段（bss后）的内存分配**************/



/*地址计算*/
"MEM_ADDR"="DRAM_ADDR";
"DATA_ADDR"=("MEM_ADDR"+"MEM_SIZE");
"BSS_ADDR"=("DATA_ADDR"+"DATA_SIZE");
"HEAP_ADDR"=("BSS_ADDR"+"BSS_SIZE");/*实际上heap从bss结束就开始了*/
"STACK_ADDR"=("IRAM0_ADDR"+"IRAM0_SIZE"-"STACK_SIZE");
/*
    内存分布声明
    ROM   起始地址为 0x10000，长度为 32KB
    RAM   起始地址为 0x20000，长度为 32KB
    ------- 
 */
MEMORY
{
  rom (rx!aiw) : ORIGIN = 0x10000, LENGTH =32k
  ram (rwxai) : ORIGIN = 0x20000, LENGTH =64k
  dram(rwxai) : ORIGIN = DRAM_ADDR, LENGTH =DRAM_SIZE
    mem (rwx!ai) : ORIGIN = MEM_ADDR, LENGTH = MEM_SIZE
    data (rw!xai) : ORIGIN = DATA_ADDR, LENGTH = DATA_SIZE
    bss (rw!xai) : ORIGIN = BSS_ADDR, LENGTH = BSS_SIZE
    heap(rw!xai) : ORIGIN = HEAP_ADDR, LENGTH = HEAP_SIZE
    stack (rw!xai) : ORIGIN = STACK_ADDR, LENGTH = STACK_SIZE
  iram0(rwxai) : ORIGIN = IRAM0_ADDR, LENGTH =IRAM0_SIZE
  iram1(rwxai) : ORIGIN = IRAM1_ADDR, LENGTH =IRAM1_SIZE
}
/*段声明*/
SECTIONS
{
/*代码段*/
  .text           :
  {
    . = ALIGN(4);
    *(.vtable)
    *(._start)
    *(.text.unlikely .text.unlikely.*)
    *(.text.startup .text.startup.*)
    *(.text .text.*)
    *(.gnu.linkonce.t.*)
  } >rom AT>rom =0xff
  . = ALIGN(4);
  PROVIDE (__etext = .);
  PROVIDE (_etext = .);
  PROVIDE (etext = .);
  PROVIDE( _eilm = . );
  .preinit_array  :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >rom AT>rom =0xff
  .init_array     :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >rom AT>rom =0xff
  .fini_array     :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >rom AT>rom =0xff
  .ctors          :
  {
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } >rom AT>rom =0xff
  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >rom AT>rom =0xff
  .rodata         :
  {
    . = ALIGN(4);
    /*由于rom空间，因此该段内容固定，防止冲突*/
    *(.rdata)/*只读段*/
    *(.rodata .rodata.*)/*只读段*/
    *(.gnu.linkonce.r.*)/*只读段*/
    . = ALIGN(4);
    *(.srodata.cst16)/* 常量只读数据16字节对齐 */
    *(.srodata.cst8)/* 常量只读数据8字节对齐 */
    *(.srodata.cst4) /* 常量只读数据4字节对齐 */
    *(.srodata.cst2)/* 常量只读数据2字节对齐 */
    *(.srodata .srodata.*)/* 只读数据段 */
  } >rom AT>rom =0xff
    /*
        该段定义了全局变量_data_lma，
        此段位于前面各段末尾，用于存放data段中的保存在rom数据
        程序运行时会从该地址加载data段数据到RAM中
    */
  .lalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data_lma = . );
  } >rom AT>rom =0xff
  /*
        该段定义了全局变量 _data,
        因为从该段开始第一次声明 保存在RAM中的段，
        所以_data变量的地址为RAM的起始地址 0x20800
    */
  .dalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data = . );
  } >data AT>rom  =0xff
/*该段保存已初始化的数据段*/
  .data          :
  {
    /*(.rdata)/*只读段*/
    /*(.rodata .rodata.*)/*只读段*/
    /*(.gnu.linkonce.r.*)/*只读段*/
    *(.data .data.*)/*可写段*/
    *(.gnu.linkonce.d.*)/*GNU 链接一次可写数据段*/
    . = ALIGN(8);
    PROVIDE( __global_pointer$ = . + 0x800/*15k*/ );
    *(.sdata .sdata.* .sdata*)/*小数据段*/
    *(.gnu.linkonce.s.*)/*小数据段*/
     . = ALIGN(8);
    /*(.srodata.cst16)/* 常量只读数据16字节对齐 */
    /*(.srodata.cst8)/* 常量只读数据8字节对齐 */
    /*(.srodata.cst4) /* 常量只读数据4字节对齐 */
    /*(.srodata.cst2)/* 常量只读数据2字节对齐 */
    /*(.srodata .srodata.*)/* 只读数据段 */

  } >data AT>rom =0xff
  . = ALIGN(4);
  PROVIDE( _edata = . );
  PROVIDE( edata = . );
  .balign         :
  {
    . = ALIGN(4);
    PROVIDE( _fbss = . );
    PROVIDE( __bss_start = . );
  } >bss AT>bss =0xff
  /*该段保存未初始化的数据段*/
  .bss            :
  {
    *(.sbss*)
    *(.gnu.linkonce.sb.*)
    *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(4);
  } >bss AT>bss =0xff
/*从这里开始作为动态存储段，非stack部分皆为heap段*/
  . = ALIGN(8);
  PROVIDE( _end = . );
  PROVIDE( end = . );
  PROVIDE( __heap_start = . );
  PROVIDE( _heap_start = . );
  .stack  :
  {
    PROVIDE( __heap_end = . );
    PROVIDE( _heap_end = . );
    . = __stack_size;
    PROVIDE( _sp = . );
  } >stack AT>stack =0xff
}
