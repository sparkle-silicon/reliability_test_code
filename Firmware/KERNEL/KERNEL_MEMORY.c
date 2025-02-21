/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-05-11 15:36:09
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
#include "CUSTOM_POWER.H"
VBYTE SCI_Event_Buffer[10];                          // offset:HEX 0x00-0x09 SIZE:DEC 10
VBYTE SMI_Event_Buffer[10];                          // offset:HEX 0x00-0x09 SIZE:DEC 10
VBYTE PD_SCI_Event_Buffer[10];                       // offset:HEX 0x00-0x09 SIZE:DEC 10
VBYTE *KBDataPending = (VBYTE *)KBData_Buff_MemBase; // offset:HEX 0x00-0x03 SIZE:DEC 4
VBYTE *MSDataPending = (VBYTE *)MSData_Buff_MemBase; // offset:HEX 0x00-0x03 SIZE:DEC 4
VBYTE PS2_PortN_Data[3];
VBYTE *Tmp_code_pointer = NULL;
VBYTE *Tmp_XPntr;
VBYTE *Tmp_XPntr1;
void Clear_Specific_Mem(void)
{
        for (register u32 i = 0; i < 0x800; i += 4)
                (*(DWORDP)(DRAM_BASE_ADDR + i)) = 0;
}
void SECTION(".init.mem") Specific_Mem_init(void)
{
#ifdef AE101
        CORE_Version = 101;
#elif defined(AE102)
        CORE_Version = 102;
#elif defined(AE103)
        CORE_Version = 103;
#endif
        for (int i = 0; i <= 8; i++)
        {
                KBDataPending[i] = 0;
                MSDataPending[i] = 0;
        }
        Debugger_PowerSequence_Step = 1;
        KBPendingTXCount = 0;
        KBPendingRXCount = 0;
        TP_ACK_CUNT = 0;
        NumLockKey = 1;
        Host_Flag = 0x0;
        Host_Flag_INTR_KEY = 1;
        Host_Flag_INTR_AUX = 1;
        PowerSequence_Step = 1;
        System_PowerState = SYSTEM_S5;
        KB_Scan_Flag = 0;
        TP_ACK_Timeout = 40000;
        PowerSequence_WaitTime = Wait_Time;
        ROM_COPY_CNT = 0;

        for (int i = 0; i < 4 * 1024; i++)
        {
                *((VBYTEP)(SRAM_BASE_ADDR + i)) = 0x0;
        }
}
