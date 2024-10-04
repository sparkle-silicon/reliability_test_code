/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:28:51
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
#ifndef __KERNEL_SPIF_H_
#define __KERNEL_SPIF_H_
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "AE_MALLOC.H"
#include "KERNEL_MEMORY.H"

/*FLASH COMMANDS*/
#define FLASH_ID_CMD 0x90
#define FLASH_SECT_ERASE_CMD 0x20
#define FLASH_PAGE_PROGRAM_CMD 0x02
#define FLASH_WRIT_CMD 0x06
#define FLASH_READ_CMD 0x03

#define SPIF_RDY            0x1
#define SPIF_Write_Status   0x1
#define SPIF_Sleep_State    0x2
#define Spif_Int_Mask (SPIF_INT_MASK &= ~(0x1))
#define Spif_Int_Unmask (SPIF_INT_MASK |= 0x1)
#define Spif_Int_Mask_Read ((SPIF_INT_MASK & 0x1) != 0)
extern FUNCT_PTR_V_D_BP_L Spif_Ptr;
extern BYTE Write_buff[256];
extern BYTE Read_buff[256];
extern VBYTE *Tmp_code_pointer;
extern VBYTE *Tmp_XPntr;
extern void SPIF_Init(void);
extern void SPIF_READ_ID(void);
extern FUNCT_PTR_V_D_BP_L Load_Fla_If_To_Ram(FUNCT_PTR_V_D_BP_L funcpoint, const int malloc_size);
extern void Transport_Update_To_iram1(FUNCT_PTR_B_D_D funcpoint, const int malloc_size);
extern void Transport_Func_To_iram1(FUNCT_PTR_V_V funcpoint, const int malloc_size);
extern FUNCT_PTR_V_V Load_Smfi_To_Dram(FUNCT_PTR_V_V func, const int malloc_size);
extern void Dram_Part_Init(void);
extern void Dram_Read(void);
extern ALIGNED(4) void SPIF_Write(DWORD addr, BYTEP write_buff, WORD length);
extern ALIGNED(4) void SPIF_Read(DWORD addr, BYTEP read_buff, WORD length);
extern void RunSPIF_WriteFromRAM(DWORD addr, BYTEP write_buff, WORD lentgh);
extern void RunSPIF_ReadFromRAM(DWORD addr, BYTEP read_buff, WORD lentgh);
extern void Smfi_Ram_Code(void);
extern void Flash_Update_Utility(void);
#endif
