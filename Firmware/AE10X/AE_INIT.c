/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-20 15:49:20
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
#include "AE_INIT.H"

/*
 * @brief 双启动流程
 */
void SECTION(".init.dbinit") DoubleBoot_Init(void)
{
#if 1
  // 1.get OEM Set Double Boot Addr and Code Space ID
  register FUNCT_PTR_V_V func_ptr = Get_DoubleBoot_ADDR();
  if(func_ptr == NULL)
    return;
  // 2.Separate ID and address
  register u16 id = (u8)((((u32)func_ptr) >> 20) & 0xfff);
  func_ptr = (FUNCT_PTR_V_V)(((u32)func_ptr) & 0xfffff);
  // 3.Determine the current boot ID and perform the corresponding operation
  switch(id) // 选择ID
  {
    case 0xFF0: // 直接跳转
      (*func_ptr)();
      break;
    case 0xFF1:                  // 启动片区1和2的选择
      if(SYSCTL_ESTAT & BIT(3)) // 判断是否为看门狗复位
      {
        if(func_ptr != (FUNCT_PTR_V_V)0x0) // 是否地址
          (*func_ptr)();
      }
      break;
    case 0xFF2:                  // 启动片区1和2的选择
      if(SYSCTL_ESTAT & BIT(6)) // 判断是否为PWRSW复位
      {
        if(func_ptr != (FUNCT_PTR_V_V)0x0) // 是否地址
          (*func_ptr)();
      }
      break;
    case 0xFF3://切换FLASH后跳转选择
    default:
      break;
  }
  return;
#endif
}
/************************************Init*************************************
- SPKAE10X Init FLOW :
- 1. DoubleBoot (Custom Configuration Double Boot Addr，Function Is Get_DoubleBoot_ADDR() In File CUSTOM_INIT.c)
- 2. Default Config(Custom Configuration All ,Function Is Default_Config() In File CUSTOM_INIT.c)
- 3. Modlue Init (Custom Configuration All ,Function Is Module_init() In File KERNEL_SOC_FUNC.c)
- 4. Interrupt Configuration (Custom Configuration All , Function Is Irqc_init() In File KERNEL_IRQ.c)
- 5. Device Initialization (Custom Configuration All , Function Is Device_init() In File CUSTOM_INIT.c)
- 6. Specific memory space Initialization (Custom Configuration All , Function Is Specific_Mem_init() In File KERNEL_MEMORY.c)
- 7. return start and goto main() function
************************************Init*************************************/
void SECTION(".init.flow") Init()
{

  // 0. check update over init
  AE_UPDATE_NO_INIT_FUNCTION;
  // 1.double boot
  DoubleBoot_Init();
  // 2.default Set Operating Environment
  Default_Config();
  // 3.Module Initialization
  Module_init();
  // 4.Interrupt Initialization
  Irqc_init();
  // 5.Device Initialization
  Device_init();
  // 6.Specific memory space Initialization
  Specific_Mem_init();
  dprint("Init Done.\n");
  return;
}
