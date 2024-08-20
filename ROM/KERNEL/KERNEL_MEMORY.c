/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-07-19 11:56:37
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
//-----------------------------------------------------------------------------
// Common Header Include
//-----------------------------------------------------------------------------
#include "KERNEL_MEMORY.H"
VBYTE SCI_Event_Buffer[10];    // offset:HEX 0x00-0x09 SIZE:DEC 10
VBYTE SMI_Event_Buffer[10];    // offset:HEX 0x00-0x09 SIZE:DEC 10
VBYTE PD_SCI_Event_Buffer[10]; // offset:HEX 0x00-0x09 SIZE:DEC 10
VBYTE *KBDataPending = (VBYTE *)KBData_Buff_MemBase;//offset:HEX 0x00-0x03 SIZE:DEC 4
VBYTE *MSDataPending = (VBYTE *)MSData_Buff_MemBase;//offset:HEX 0x00-0x03 SIZE:DEC 4
VBYTE PS2_PortN_Data[3];
VBYTE *Tmp_code_pointer = NULL;
VBYTE *Tmp_XPntr;
VBYTE *Tmp_XPntr1;
void Clear_Specific_Mem(void)
{
        for(register u32 i = 0; i < 0x800; i += 4)
                (*(DWORDP)(DRAM_BASE_ADDR + i)) = 0;
}
