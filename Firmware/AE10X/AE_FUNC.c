/*
 * @Author: Linyu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2025-10-08 17:41:23
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
  return CPU_TIMER_FREQ;
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
#if 1
uint32_t NOINLINE measure_cpu_freq(size_t n)
{
#if 0//不走pll了
  enable_mcycle_minstret();
  register uint32_t start_mtime, delta_mtime;
  register uint32_t mtime_freq = get_timer_freq(); // get default freq（32k）
  // Don't start measuruing until we see an mtime tick
  register uint32_t tmp = mtime_lo(); // get Machine-mode timer register(计时器寄存器)//反映了 32 位计时器的实时数值。该计时器的时钟频率为系统的低速 32K 时钟。
  do
  {
    start_mtime = mtime_lo(); // 获取计时器改变后的值（用的32K的时钟，时间相对比较久，判断新的开始）
  } while (start_mtime == tmp);
  uint32_t start_mcycle = read_csr(mcycle); // get Lower 32 bits of Cycle counter（周期计数器）//用于反映处理器执行了多少个时钟周期。只要处理器处于执行状态时，此计数器便会不断自增计数
  do
  {
    delta_mtime = mtime_lo() - start_mtime; // 判断走了n个计时器时钟
  } while (delta_mtime < n);
  register uint32_t delta_mcycle = read_csr(mcycle) - start_mcycle; // 获取时钟周期差值
  disable_mcycle_minstret();
  // 24M时钟周期/32k时钟周期*预定32k的频率+24M时钟周期%32k时钟周期*预定32k频率/32K时钟频率
  return (delta_mcycle / delta_mtime) * mtime_freq + ((delta_mcycle % delta_mtime) * mtime_freq) / delta_mtime;
#else
  //后续如果有方案可以添加,目前暂时认为其为标准时钟
  UNUSED_VAR(n);
  return HIGH_CHIP_CLOCK;
#endif
}
uint32_t get_cpu_freq()
{
  // warm up
  measure_cpu_freq(1);
  // measure for real
  CPU_FREQ = measure_cpu_freq(100);
  return CPU_FREQ;
}
#else
#define MIN_MEASUREMENT_CYCLES 1000000  // 最小测量周期数
#define CALIBRATION_SAMPLES 5           // 校准采样次数
#define MTIME_FREQ_NOMINAL LOW_CHIP_CLOCK      // 标称32.768kHz频率
#define CPU_FREQ_NOMINAL 96000000       // 标称96MHz频率

// 精确延时函数（使用循环计数）
static void precise_delay(uint32_t cycles)
{
  volatile uint32_t count = cycles;
  while (count--);
}

// 测量函数（核心）
static uint64_t measure_ratio(uint32_t n)
{
  enable_mcycle_minstret();

  uint32_t start_mtime, end_mtime;
  uint32_t start_mcycle, end_mcycle;

  // 等待mtime变化确保起点准确
  uint32_t tmp = mtime_lo();
  do
  {
    start_mtime = mtime_lo();
  } while (start_mtime == tmp);

  start_mcycle = read_csr(mcycle);

  // 等待足够的mtime计数
  uint32_t elapsed;
  do
  {
    end_mtime = mtime_lo();
    elapsed = (end_mtime >= start_mtime) ?
      (end_mtime - start_mtime) :
      (0xFFFFFFFF - start_mtime + end_mtime + 1);
  } while (elapsed < n);

  end_mcycle = read_csr(mcycle);
  disable_mcycle_minstret();

  // 计算mcycle差值（处理溢出）
  uint64_t delta_mcycle = (end_mcycle >= start_mcycle) ?
    (end_mcycle - start_mcycle) :
    (0xFFFFFFFF - start_mcycle + end_mcycle + 1);

  return (delta_mcycle << 32) | elapsed; // 返回64位结果
}

// 主测量函数
uint32_t NOINLINE measure_cpu_freq(void)
{
  // 预热测量系统
  measure_ratio(100);

  // 多次测量取平均
  uint64_t total_ratio = 0;
  for (int i = 0; i < CALIBRATION_SAMPLES; i++)
  {
    // 每次测量使用不同的时间基准
    uint32_t n = MIN_MEASUREMENT_CYCLES + (i * 10000);

    uint64_t result = measure_ratio(n);
    uint64_t delta_mcycle = result >> 32;
    uint32_t delta_mtime = result & 0xFFFFFFFF;

    // 计算比例因子
    uint64_t ratio = (delta_mcycle << 20) / delta_mtime;
    total_ratio += ratio;

    // 间隔延时，减少热影响
    precise_delay(1000);
  }

  // 计算平均比例因子
  uint64_t avg_ratio = total_ratio / CALIBRATION_SAMPLES;

  // 计算实际频率（使用标称值作为基准）
  uint64_t freq = (avg_ratio * MTIME_FREQ_NOMINAL) >> 20;

  // 应用温度补偿（如果有温度传感器）
#ifdef HAS_TEMP_SENSOR
  int temp = read_temperature();
  freq = apply_temp_compensation(freq, temp);
#endif

  return (uint32_t)freq;
}
uint32_t get_cpu_freq()
{
  // measure for real
  CPU_FREQ = measure_cpu_freq();
  return CPU_FREQ;
}
#endif

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
  if (value1 != 0 && value2 == 0)
  {
    return 0;
  }
  else if (value1 == 0 && value2 != 0)
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
  if (value1 != 0)
  {
    return 1;
  }
  else
  {
    if (value2 == 0)
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

