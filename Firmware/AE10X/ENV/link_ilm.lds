/*
 * @Author: daweslinyu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-07-03 18:10:26
 * @Description: Platform battery & battery charger control code
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

OUTPUT_ARCH( "riscv" )
ENTRY( _start )
MEMORY
{
  ilm (rxai!w) : ORIGIN = 0x80000, LENGTH = 256K
  ram (wxa!ri) : ORIGIN = 0x20000, LENGTH = 32K
}
SECTIONS
{
  __stack_size = DEFINED(__stack_size) ? __stack_size : 2K;
  .init           :
  {
    *(.vtable_ilm)
    KEEP (*(SORT_NONE(.init)))
  } >ilm AT>ilm 
  .ilalign         :
  {
    . = ALIGN(4);
    PROVIDE( _ilm_lma = . );
  } >ilm AT>ilm 
  .ialign         :
  {
    PROVIDE( _ilm = . );
  } >ilm AT>ilm 
  .text           :
  {
    *(.text.unlikely .text.unlikely.*)
    *(.text.startup .text.startup.*)
    *(.text .text.*)
    *(.gnu.linkonce.t.*)
  } >ilm AT>ilm 
  .fini           :
  {
    KEEP (*(SORT_NONE(.fini)))
  } >ilm AT>ilm 
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
  } >ilm AT>ilm 
  .init_array     :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >ilm AT>ilm 
  .fini_array     :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >ilm AT>ilm 
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
  } >ilm AT>ilm 
  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >ilm AT>ilm 
  .lalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data_lma = . );
  } >ilm AT>ilm 
  .dalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data = . );
  } >ram AT>ilm 
  .data          :
  {
    *(.rdata)
    *(.rodata .rodata.*)
    *(.gnu.linkonce.r.*)
    *(.data .data.*)
    *(.gnu.linkonce.d.*)
    . = ALIGN(8);
    PROVIDE( __global_pointer$ = . + 0x800 );
    *(.sdata .sdata.* .sdata*)
    *(.gnu.linkonce.s.*)
    . = ALIGN(8);
    *(.srodata.cst16)
    *(.srodata.cst8)
    *(.srodata.cst4)
    *(.srodata.cst2)
    *(.srodata .srodata.*)
  } >ram AT>ilm 
  . = ALIGN(4);
  PROVIDE( _edata = . );
  PROVIDE( edata = . );
  PROVIDE( _fbss = . );
  PROVIDE( __bss_start = . );
  .bss            :
  {
    *(.sbss*)
    *(.gnu.linkonce.sb.*)
    *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(4);
  } >ram AT>ram 
  . = ALIGN(8);
  PROVIDE( _end = . );
  PROVIDE( end = . );
  PROVIDE( __heap_start = . );
  PROVIDE( _heap_start = . );
  .stack ORIGIN(ram) + LENGTH(ram) - __stack_size :
  {
    PROVIDE( _heap_end = . );
    . = __stack_size;
    PROVIDE( _sp = . );
  } >ram AT>ram 
  .magic_number 				:/*留1k给flash 验签*/
  {
	. = ORIGIN(ilm)+LENGTH(ilm)-0x100;/*+0x1FF00;*/
  PROVIDE( _MAGIC_NUMBER = . );
	*(.FlashInfo.Dynamic)
  } >ilm AT>ilm =0xff
    .flash_over         :
  {
    . = ORIGIN(ilm)+LENGTH(ilm)-1;
    *(.flash_over)
    PROVIDE( flash_over = . );
  } >ilm AT>ilm =0xff
}
