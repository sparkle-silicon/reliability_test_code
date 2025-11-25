/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-10-20 17:41:33
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
#ifndef __KERNEL_SPIFE_H_
#define __KERNEL_SPIFE_H_
#include "AE_GLOBAL.H"

extern FUNCT_PTR_V_V Smf_Ptr;
extern FUNCT_PTR_V_D_BP_L Spif_Ptr;
extern FUNCT_PTR_V_D_BP ECU_Ptr;
extern FUNCT_PTR_V_D_BP_L Spif_Ptr;
extern BYTE Write_buff[256];
extern BYTE Read_buff[256];

/*-----------------------------------------------------------------------------
 * FLASH Commands
 *---------------------------------------------------------------------------*/
#define FLASH_ID_CMD 0x90
#define FLASH_SECT_ERASE_CMD 0x20
#define FLASH_PAGE_PROGRAM_CMD 0x02
#define FLASH_WRIT_CMD 0x06
#define FLASH_READ_CMD 0x03
/*-----------------------------------------------------------------------------
 * SPIF Status
 *---------------------------------------------------------------------------*/
#define SPIF_RDY_READY       0x1
#define SPIFE_Write_Status   0x1
#define SPIFE_Sleep_State    0x2
/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
extern void SPIFE_Init(void);
extern uint32_t SPIFE_READ_ID(void);
extern void ExtFlash_Deep_Power_Down(void);
extern void ExtFlash_Exit_Power_Down(void);
extern FUNCT_PTR_V_D_BP_L Load_Fla_If_To_Ram(FUNCT_PTR_V_D_BP_L funcpoint, const int malloc_size);
extern void Transport_Update_To_iram1(FUNCT_PTR_B_D_D funcpoint, const int malloc_size);
extern void Transport_Func_To_iram1(FUNCT_PTR_V_V funcpoint, const int malloc_size);
extern FUNCT_PTR_V_V Load_Smfi_To_Dram(FUNCT_PTR_V_V func, const int malloc_size);
extern ALIGNED(4) void SPIFE_Write(DWORD addr, BYTEP write_buff, WORD length);
extern ALIGNED(4) void SPIFE_Read(DWORD addr, BYTEP read_buff, WORD length);
extern void RunSPIFE_WriteFromRAM(DWORD addr, BYTEP write_buff, WORD lentgh);
extern void RunSPIFE_ReadFromRAM(DWORD addr, BYTEP read_buff, WORD lentgh);
extern void Smfi_Ram_Code(void);
#endif
