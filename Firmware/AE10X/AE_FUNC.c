/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-12-14 17:22:10
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
#include <AE_FUNC.H>
// Configure PMP to make all the address space accesable and executable
void pmp_open_all_space()
{
  // Config entry0 addr to all 1s to make the range cover all space
  asm volatile("li x6, 0xffffffff" ::
                   : "x6");
  asm volatile("csrw pmpaddr0, x6" ::
                   :);
  // Config entry0 cfg to make it NAPOT address mode, and R/W/X okay
  asm volatile("li x6, 0x7f" ::
                   : "x6");
  asm volatile("csrw pmpcfg0, x6" ::
                   :);
}
void switch_m2u_mode()
{
  clear_csr(mstatus, MSTATUS_MPP);
  asm volatile("la x6, 1f    " ::
                   : "x6");
  asm volatile("csrw mepc, x6" ::
                   :);
  asm volatile("mret" ::
                   :);
  asm volatile("1:" ::
                   :);
}
uint32_t mtime_lo(void)
{
  return read_csr(0xBDA);
}
uint32_t get_timer_value()
{
  return mtime_lo();
}
uint32_t get_timer_freq()
{
  return TIMER_FREQ;
}
uint32_t get_instret_value()
{
  return read_csr(minstret);
  ;
}
uint32_t get_cycle_value()
{
  return read_csr(mcycle);
}
uint32_t NOINLINE measure_cpu_freq(register size_t n)
{
  enable_mcycle_minstret();
  register uint32_t start_mtime, delta_mtime;
  register uint32_t mtime_freq = get_timer_freq(); // get default freq（32k）
  // Don't start measuruing until we see an mtime tick
  register uint32_t tmp = mtime_lo(); // get Machine-mode timer register(计时器寄存器)//反映了 32 位计时器的实时数值。该计时器的时钟频率为系统的低速 32K 时钟。
  do
  {
    start_mtime = mtime_lo(); // 获取计时器改变后的值（用的32K的时钟，时间相对比较久，判断新的开始）
  }
  while(start_mtime == tmp);
  uint32_t start_mcycle = read_csr(mcycle); // get Lower 32 bits of Cycle counter（周期计数器）//用于反映处理器执行了多少个时钟周期。只要处理器处于执行状态时，此计数器便会不断自增计数
  do
  {
    delta_mtime = mtime_lo() - start_mtime; // 判断走了n个计时器时钟
  }
  while(delta_mtime < n);
  register uint32_t delta_mcycle = read_csr(mcycle) - start_mcycle; // 获取时钟周期差值
  disable_mcycle_minstret();
  // 24M时钟周期/32k时钟周期*预定32k的频率+24M时钟周期%32k时钟周期*预定32k频率/32K时钟频率
  return (delta_mcycle / delta_mtime) * mtime_freq + ((delta_mcycle % delta_mtime) * mtime_freq) / delta_mtime;
}
uint32_t get_cpu_freq()
{
  // warm up
  measure_cpu_freq(1);
  // measure for real
  CPU_FREQ = measure_cpu_freq(100);
  return CPU_FREQ;
}
// Debugger CPU Int Function
void CPU_Int_Enable(BYTE type)
{
  uint32_t value = read_csr(0xBD1);
  write_csr(0xBD1, (value |= (0x1 << type)));
}
void CPU_Int_Disable(BYTE type)
{
  uint32_t value = read_csr(0xBD1);
  write_csr(0xBD1, (value &= ~(0x1 << type)));
}
BYTE CPU_Int_Enable_Read(BYTE type)
{
  uint32_t value = read_csr(0xBD1);
  return ((value & (0x1 < type)) != 0);
}
void CPU_Int_Type_Edge(BYTE type)
{
  uint32_t value1, value2;
  value1 = read_csr(0xBD2);
  value2 = read_csr(0xBD3);
  write_csr(0xBD2, (value1 &= ~(0x1 << type)));
  write_csr(0xBD3, (value2 |= (0x1 << type)));
}
void CPU_Int_Type_Level(BYTE type)
{
  uint32_t value1, value2;
  value1 = read_csr(0xBD2);
  value2 = read_csr(0xBD3);
  write_csr(0xBD2, (value1 |= (0x1 << type)));
  write_csr(0xBD3, (value2 &= ~(0x1 << type)));
}
BYTE CPU_Int_Type_Read(BYTE type)
{
  uint32_t value1, value2;
  value1 = (read_csr(0xBD2) & (0x1 < type));
  value2 = (read_csr(0xBD3) & (0x1 < type));
  if(value1 != 0 && value2 == 0)
  {
    return 0;
  }
  else if(value1 == 0 && value2 != 0)
  {
    return 1;
  }
  else
  {
    return -1;
  }
}
BYTE CPU_Int_Polarity_Read(BYTE type)//0为低，1为高
{
  uint32_t value2, value1;
  value1 = (read_csr(0xBD2) & (0x1 << type));
  value2 = (read_csr(0xBD3) & (0x1 << type));
  if(value1 != 0)
  {
    return 1;
  }
  else
  {
    if(value2 == 0)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }

}

BYTE CPU_Int_Polarity_LOW(BYTE type)
{
  uint32_t value1;
  value1 = read_csr(0xBD3);
  write_csr(0xBD3, (value1 |= (0x1 << type)));
  return 0;
}
BYTE CPU_Int_Polarity_HIGH(BYTE type)
{
  uint32_t value1;
  value1 = read_csr(0xBD2);
  write_csr(0xBD3, (value1 &= ~(0x1 << type)));
  return 0;
}
WEAK DWORD SECTION(".interrupt.handle_trap") handle_trap(uintptr_t mcause, uintptr_t sp)
{
  return kernel_trap(mcause, sp);
}
//------------------------------------------------------------------------------
// The function of disable all interrupts
//------------------------------------------------------------------------------
void Disable_Interrupt_Main_Switch(void)
{
  clear_csr(mstatus, MSTATUS_MIE);
  __nop;
  __nop;
  __nop;
}
//------------------------------------------------------------------------------
// The function of enable all interrupts
//------------------------------------------------------------------------------
void Enable_Interrupt_Main_Switch(void)
{
  set_csr(mstatus, MSTATUS_MIE);
}

void ALIGNED(4) GLE01_RomCode_Transport_FlashToIRAM0(void)
{
  asm volatile("la a0, 0xC0000"); // 搬运的FLASH起始地址(0x80000+256K)
  asm volatile("la a1, 0xC8000"); // 搬运的FLASH终止地址(0x80000+256K+32K)
  asm volatile("la a2, 0x28000"); // 搬运的IRAM0目标地址

  asm volatile("1:");
  asm volatile("lw t0, (a0)");
  asm volatile("sw t0, (a2)");
  asm volatile("addi a0, a0, 4");
  asm volatile("addi a2, a2, 4");
  asm volatile("bltu a0, a1, 1b");

  return;
}

void GLE01_RomCode_Transport(void)
{
  Disable_Interrupt_Main_Switch();
  GLE01_RomCode_Transport_FlashToIRAM0();
  // GLE01_RomCode_Ptr = Load_Smfi_To_Dram(GLE01_RomCode_Transport_FlashToIRAM0, 0x200);
  // (*GLE01_RomCode_Ptr)(); // Do Function at malloc address
  Enable_Interrupt_Main_Switch();
  SYSCTL_CRYPTODBG_FLAG |= 1;
  dprint("GLE01 ROMCODE Transport Flash to IRAM0\n");
}

void ALIGNED(4) Vtable_Tansport_FlashToIVT(void)
{
  asm volatile("la a0, 0x80000"); // 搬运的Vector table起始地址
  asm volatile("la a1, 0x80084"); // 搬运的Vector table终止地址
  asm volatile("la a2, 0x30800"); // 搬运的IVT空间目标地址

  asm volatile("1:");
  asm volatile("lw t0, (a0)");
  asm volatile("sw t0, (a2)");
  asm volatile("addi a0, a0, 4");
  asm volatile("addi a2, a2, 4");
  asm volatile("bltu a0, a1, 1b");

  return;
}

void Vtable_Tansport(void)
{
  Disable_Interrupt_Main_Switch();
  IVT_Ptr = Load_Smfi_To_Dram(Vtable_Tansport_FlashToIVT, 0x200);
  (*IVT_Ptr)(); // Do Function at malloc address
  Enable_Interrupt_Main_Switch();
  // free(IVT_Ptr);
  dprint("vector table Transport to IVT Space\n");
}
