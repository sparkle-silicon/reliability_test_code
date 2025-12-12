/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-10-20 17:43:20
 * @Description: This file is used for SPI Flash Interface
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
#include "KERNEL_SPIF.H"
#include "KERNEL_TIMER.H"
#include "KERNEL_ACPI.H"
#include "KERNEL_MEMORY.H"
#pragma pack(4)
FUNCT_PTR_V_V Smf_Ptr;
FUNCT_PTR_V_D_BP_L Spif_Ptr;
BYTE Write_buff[256] = { 0, 1, 2, 3, 4, 5, 6 };
BYTE Read_buff[256] = { 0 };
uint32_t EXTERNAL_FLASH_ID = 0;
void SPIFE_Init(void)
{
#if (!SPIFE_CLOCK_EN)
   return;
#endif
   SPIFE_CTL0 |= 0x1; // 擦写时允许挂起
   if (SYSCTL_PIO_CFG & BIT1)//使用外部FLASH
   {
   }
   else//内部SPIFE
   {
      EXTERNAL_FLASH_ID = SPIFE_READ_ID();
      dprint("EXTERNAL FLASH ID is %#lx.\n", EXTERNAL_FLASH_ID);
   }

}
// 读取id
uint32_t SPIFE_READ_ID(void)
{
#if (!SPIFE_CLOCK_EN)
   return;
#endif
   while (!(SPIFE_RDY & SPIF_RDY_READY));
   SPIFE_DBYTE = 0x2;
   while (!(SPIFE_RDY & SPIF_RDY_READY));
   // SPIFE_FTOP = FLASH_ID_CMD;
   SPIFE_FTOP = 0x9f;
   // while(!(SPIFE_RDY & SPIF_RDY_READY));
   while (((SPIFE_FCNT & 0x3) == 0));
   uint32_t id = SPIFE_FTOP;
   return id;
}
VBYTEP OPTIMIZE0 USED SPIFE_Read_Interface(register DWORD size, register DWORD addr, BYTEP read_buff)
{
   BYTE SPIFE_READ_COMMAND;
   DWORD SPIFE_J;
#define temp_addrs SPIFE_addrs
#define temp_data SPIFE_data
   if (size & 0b11)
   {
      size += 0b100;
      size &= ~0b11;
   }
   // PRINTF_TX('M');
   while (!(SPIFE_RDY & SPIF_RDY_READY))
      ;
   // PRINTF_TX('N');
   // rom_wdt_feed();
   while (SPIFE_STA & 0xf)
      ;
   // PRINTF_TX('W');
   // rom_wdt_feed(); // 直到写完
   while (!(SPIFE_RDY & SPIF_RDY_READY))
      ;
   // rom_wdt_feed(); // 读忙
   SPIFE_DBYTE = (size - 1) & 0xff;
   while (!(SPIFE_RDY & SPIF_RDY_READY))
      ;
   // rom_wdt_feed();
   if (SPIFE_CTL0 & BIT1)
      SPIFE_READ_COMMAND = 0x6b;
   else
      SPIFE_READ_COMMAND = 0x3b;
   SPIFE_FTOP = (((addr & 0xFF) << 24) + ((addr & 0xFF00) << 8) + ((addr & 0xFF0000) >> 8) + SPIFE_READ_COMMAND);
   for (SPIFE_J = 0; SPIFE_J < (size >> 2); SPIFE_J++)
   {
      while ((SPIFE_FCNT & 0x3) == 0)
         ;
      // rom_wdt_feed();
      // dprint("TOP:%x", SPIFE_FTOP);
      (*((DWORDP)(&(read_buff[SPIFE_J << 2])))) = SPIFE_FTOP;
   }
   return read_buff;
#undef temp_addrs
#undef temp_data
}
/**
 * @brief 写入flash操作
 *
 * @param addr          要进行页写入的地址
 * @param write_buff    传入一个数组，数组中的内容是要页写入的值
 * @param length        要写入的字节数，必须小于或等于页的大小(256字节)。
 *
 * @note
 * - 该函数建议不要直接使用，在RunSPIFE_WriteFromRAM函数中调用
 * @return
 */
ALIGNED(4) void SPIFE_Write(DWORD addr, BYTEP write_buff, WORD length)
{
#if (!SPIFE_CLOCK_EN)
   return;
#endif
   DWORD i, j, write_data = 0;
   uint32_t temp_addrs = ((addr & 0xFF) << 24) + ((addr & 0xFF00) << 8) + ((addr & 0xFF0000) >> 8);//设置地址
   BYTE temp_status = 0;
   BYTE _4ByteCunt = 0;
   BYTE temp_length = 0;
   PRINTF_TX = 'a';
   while (!(SPIFE_RDY & SPIF_RDY_READY));
   SPIFE_FTOP = temp_addrs + FLASH_SECT_ERASE_CMD;   //Sector Erase
   while (!(SPIFE_RDY & SPIF_RDY_READY));
   while (SPIFE_STA & SPIFE_Write_Status);
   while (!(SPIFE_RDY & SPIF_RDY_READY));
   PRINTF_TX = 'b';
   _4ByteCunt = length / 4;
   temp_length = length % 4;
   //写
   SPIFE_DBYTE = length - 1;                 //准备写256字节
   while (!(SPIFE_RDY & SPIF_RDY_READY));
   SPIFE_FTOP = (temp_addrs + FLASH_PAGE_PROGRAM_CMD);  //Page Program
   if (_4ByteCunt != 0)
   {
      for (i = 0; i < (length / 4); i++)
      {
         j = i * 4;
         write_data = write_buff[j] | (write_buff[j + 1] << 8) | (write_buff[j + 2] << 16) | (write_buff[j + 3] << 24);
         temp_status = SPIFE_FCNT;
         while ((temp_status & 0x3) >= 2)
         {
            temp_status = SPIFE_FCNT;
         }
         SPIFE_FTOP = write_data;
      }
      switch (temp_length)
      {
         case 1:
            write_data = write_buff[j + 4];
            temp_status = SPIFE_FCNT;
            while ((temp_status & 0x3) >= 2)
            {
               temp_status = SPIFE_FCNT;
            }
            SPIFE_FTOP = write_data;
            break;
         case 2:
            write_data = write_buff[j + 4] | (write_buff[j + 5] << 8);
            temp_status = SPIFE_FCNT;
            while ((temp_status & 0x3) >= 2)
            {
               temp_status = SPIFE_FCNT;
            }
            SPIFE_FTOP = write_data;
            break;
         case 3:
            write_data = write_buff[j + 4] | (write_buff[j + 5] << 8) | (write_buff[j + 6] << 16);
            temp_status = SPIFE_FCNT;
            while ((temp_status & 0x3) >= 2)
            {
               temp_status = SPIFE_FCNT;
            }
            SPIFE_FTOP = write_data;
            break;
      }
   }
   else
   {
      switch (temp_length)
      {
         case 1:
            write_data = write_buff[0];
            temp_status = SPIFE_FCNT;
            while ((temp_status & 0x3) >= 2)
            {
               temp_status = SPIFE_FCNT;
            }
            SPIFE_FTOP = write_data;
            break;
         case 2:
            write_data = write_buff[0] | (write_buff[1] << 8);
            temp_status = SPIFE_FCNT;
            while ((temp_status & 0x3) >= 2)
            {
               temp_status = SPIFE_FCNT;
            }
            SPIFE_FTOP = write_data;
            break;
         case 3:
            write_data = write_buff[0] | (write_buff[1] << 8) | (write_buff[2] << 16);
            temp_status = SPIFE_FCNT;
            while ((temp_status & 0x3) >= 2)
            {
               temp_status = SPIFE_FCNT;
            }
            SPIFE_FTOP = write_data;
            break;
      }
   }
   while (!(SPIFE_RDY & SPIF_RDY_READY));
   while (SPIFE_STA & SPIFE_Write_Status);
   while (!(SPIFE_RDY & SPIF_RDY_READY));
   PRINTF_TX = 'c';
   while (!(SPIFE_RDY & SPIF_RDY_READY));
}

/**
 * @brief 读取flash操作。
 *
 * @param addr          要进行页写入的起始地址，必须对齐到页大小的边界。
 * @param read_buff    传入一个数组，数组中的内容是要页写入的值。
 * @param length        要读取的字节数，必须小于或等于页的大小(256字节)。
 *
 * @return
 *
 * @note
 * - 该函数建议不要直接使用，在RunSPIFE_ReadFromRAM函数中调用
 */
ALIGNED(4) void SPIFE_Read(DWORD addr, BYTEP read_buff, WORD length)
{
#if (!SPIFE_CLOCK_EN)
   return;
#endif
   uint32_t temp_addrs = ((addr & 0xFF) << 24) + ((addr & 0xFF00) << 8) + ((addr & 0xFF0000) >> 8);//设置地址
   int i, j = 0;
   BYTE temp_status = 0;
   uint32_t temp_data[64];
   length = (length + 3) & ~3;
   while (!(SPIFE_RDY & SPIF_RDY_READY));
   SPIFE_DBYTE = length - 1;
   while (!(SPIFE_RDY & SPIF_RDY_READY));
   SPIFE_FTOP = (temp_addrs + FLASH_READ_CMD);
   for (j = 0; j < (length / 4); j++)
   {
      i = j * 4;
      temp_status = SPIFE_FCNT;
      while ((temp_status & 0x3) == 0)
      {
         temp_status = SPIFE_FCNT;
      }
      temp_data[j] = SPIFE_FTOP;
      read_buff[i] = (temp_data[j] & 0xff);
      read_buff[i + 1] = (temp_data[j] & 0xff00) >> 8;
      read_buff[i + 2] = (temp_data[j] & 0xff0000) >> 16;
      read_buff[i + 3] = (temp_data[j] & 0xff000000) >> 24;
   }
   while (!(SPIFE_RDY & SPIF_RDY_READY));
   while (SPIFE_STA & SPIFE_Write_Status);
   while (!(SPIFE_RDY & SPIF_RDY_READY));
   PRINTF_TX = 'd';
}

void ExtFlash_Deep_Power_Down(void)
{
#if (!SPIFE_CLOCK_EN)
   dprint("SPIFE_CLOCK is not enable\n");
   return;
#endif
   while (!(SPIFE_RDY & SPIF_RDY_READY));
   while (SPIFE_STA & 0xf);
   SPIFE_FTOP = 0xb9;
   for (int i = 0; i < 1000; i++)
   {
      nop;
   }
   while (!(SPIFE_RDY & SPIF_RDY_READY));
   while (SPIFE_STA & 0xb);
   while (!(SPIFE_RDY & SPIF_RDY_READY));
}

void ExtFlash_Exit_Power_Down(void)
{
#if (!SPIFE_CLOCK_EN)
   dprint("SPIFE_CLOCK is not enable\n");
   return;
#endif
   while (!(SPIFE_RDY & SPIF_RDY_READY));
   SPIFE_FTOP = 0xab;
   for (int i = 0; i < 1000; i++)
   {
      nop;
   }
   while (!(SPIFE_RDY & SPIF_RDY_READY));
   while (SPIFE_STA & 0xb);
   while (!(SPIFE_RDY & SPIF_RDY_READY));
}

void Smfi_Ram_Code(void)
{
   int i, j = 0;
   uint32_t temp_status, temp_data;
   uint8_t SmfiCmd, Smf_Addr0, Smf_Addr1, Smf_Addr2, Smf_Data0, Smf_Data1, Smf_Data2, Smf_Data3, Smf_Sram_Base;
   uint32_t Smfi_Write_Addr, Smfi_Write_Data, Smfi_Custom_Addr;
   // Disable_Interrupt_Main_Switch();
   // get command addr data
   SmfiCmd = SMF_CMD;
   Smf_Addr0 = SMF_FADDR0;
   Smf_Addr1 = SMF_FADDR1;
   Smf_Addr2 = SMF_FADDR2;
   Smf_Data0 = SMF_DR0;
   Smf_Data1 = SMF_DR1;
   Smf_Data2 = SMF_DR2;
   Smf_Data3 = SMF_DR3;
   // compute flash address&data
   Smfi_Write_Addr = (uint32_t)(Smf_Addr2 << 24) + (uint32_t)(Smf_Addr1 << 16) + (uint32_t)(Smf_Addr0 << 8) + (uint32_t)SmfiCmd;
   Smfi_Write_Data = (uint32_t)(Smf_Data3 << 24) + (uint32_t)(Smf_Data2 << 16) + (uint32_t)(Smf_Data1 << 8) + (uint32_t)Smf_Data0;
   PRINTF_TX = 'a';
   switch (SmfiCmd)
   {
      case 0x9f: // read id
         {
            // SPIFE_READ_ID();
         }
         break;
      case 0xc7: // chip erase
         {
         }
         break;
      case 0x20: // sector erase
         {
            temp_status = SPIFE_RDY;
            while (!(temp_status & 0x1))
            {
               temp_status = SPIFE_RDY;
            }
               //fla_if_write(FLA_FIFO_TOP_ADDR, 0x00c80020);
            SPIFE_FTOP = Smfi_Write_Addr;
            while (!(SPIFE_RDY & SPIF_RDY_READY));
            temp_status = SPIFE_STA;
            while (temp_status & 0x1)
            {
               temp_status = SPIFE_STA;
            }
            while (!(SPIFE_RDY & SPIF_RDY_READY));
            PRINTF_TX = 'b';
         }
         break;
      case 0x52: // block erase 32kb
         {
         }
         break;
      case 0xd8: // block erase 64kb
         {
         }
         break;
      case 0x02: // page program
         {
            SPIFE_DBYTE = 0xff;
            temp_status = SPIFE_RDY;
            while (!(temp_status & 0x1))
            {
               temp_status = SPIFE_RDY;
            }
            // fla_if_write(FLA_FIFO_TOP_ADDR, 0x00c80002);
            SPIFE_FTOP = Smfi_Write_Addr;
            for (i = 0; i < 64; i++)
            {
               temp_status = SPIFE_FCNT;
               while ((temp_status & 0x3) >= 2)
               {
                  temp_status = SPIFE_FCNT;
               }
               // fla_if_write(FLA_FIFO_TOP_ADDR, ((i<<24)|(i<<16)|(i<<8)|(i)));
               SPIFE_FTOP = Smfi_Write_Data;
            }
            temp_status = SPIFE_RDY;
            while (!(temp_status & 0x1))
            {
               temp_status = SPIFE_RDY;
            }
            temp_status = SPIFE_STA;
            while (temp_status & 0x1)
            {
               temp_status = SPIFE_STA;
            }
            temp_status = SPIFE_RDY;
            while (!(temp_status & 0x1))
            {
               temp_status = SPIFE_RDY;
            }
            PRINTF_TX = 'c';
         }
         break;
      case 0x03: // read data
         {
            Smf_Sram_Base = 0x00; // set rewrite sram addr[11:4]
            temp_status = SPIFE_RDY;
            while (!(temp_status & 0x1))
            {
               temp_status = SPIFE_RDY;
            }
            SPIFE_DBYTE = 0xff;
            // fla_if_write(FLA_DBYTE_ADDR, Smf_Num-1);
            temp_status = SPIFE_RDY;
            while (!(temp_status & 0x1))
            {
               temp_status = SPIFE_RDY;
            }
            // fla_if_write(FLA_FIFO_TOP_ADDR, 0x00c80003);
            SPIFE_FTOP = Smfi_Write_Addr;
            // if((Smf_Num%4) != 0)
            // Read_Num = Smf_Num/4 + 1;
            // else Read_Num = Smf_Num/4;
            PRINTF_TX = 's';
            for (j = 0; j < 64; j++)
            // for(j=0;j<Read_Num;j++)
            {
               temp_status = SPIFE_FCNT;
               while ((temp_status & 0x3) == 0)
               {
                  temp_status = SPIFE_FCNT;
               }
               // data[j] = SPIFE_FTOP;
               *((volatile uint32_t *)(SRAM_BASE_ADDR + Smf_Sram_Base + j * 4)) = SPIFE_FTOP;
            }
            PRINTF_TX = 'x';
            temp_status = SPIFE_RDY;
            while (!(temp_status & 0x1))
            {
               temp_status = SPIFE_RDY;
            }
            temp_status = SPIFE_STA;
            while (temp_status & 0x1)
            {
               temp_status = SPIFE_STA;
            }
            temp_status = SPIFE_RDY;
            while (!(temp_status & 0x1))
            {
               temp_status = SPIFE_RDY;
            }
            PRINTF_TX = 'd';
         }
         break;
      case 0x12: // firmware cycle write
         {
            Smfi_Custom_Addr = (uint32_t)(Smf_Addr0 << 24) + (uint32_t)(Smf_Addr1 << 16) + (uint32_t)(Smf_Addr2 << 8) + 0x02;
            SPIFE_DBYTE = 0x0;
            temp_status = SPIFE_RDY;
            while (!(temp_status & 0x1))
            {
               temp_status = SPIFE_RDY;
            }
            // fla_if_write(FLA_FIFO_TOP_ADDR, 0x00c80002);
            SPIFE_FTOP = Smfi_Custom_Addr;
            temp_status = SPIFE_FCNT;
            while ((temp_status & 0x3) >= 2)
            {
               temp_status = SPIFE_FCNT;
            }
            // fla_if_write(FLA_FIFO_TOP_ADDR, ((i<<24)|(i<<16)|(i<<8)|(i)));
            SPIFE_FTOP = Smfi_Write_Data;
            temp_status = SPIFE_RDY;
            while (!(temp_status & 0x1))
            {
               temp_status = SPIFE_RDY;
            }
            temp_status = SPIFE_STA;
            while (temp_status & 0x1)
            {
               temp_status = SPIFE_STA;
            }
            temp_status = SPIFE_RDY;
            while (!(temp_status & 0x1))
            {
               temp_status = SPIFE_RDY;
            }
            PRINTF_TX = 'e';
         }
         break;
      case 0x13: // firmware cycle read
         {
            Smfi_Custom_Addr = (uint32_t)(Smf_Addr0 << 24) + (uint32_t)(Smf_Addr1 << 16) + (uint32_t)(Smf_Addr2 << 8) + 0x03;
            temp_status = SPIFE_RDY;
            while (!(temp_status & 0x1))
            {
               temp_status = SPIFE_RDY;
            }
            SPIFE_DBYTE = 0x0;
            temp_status = SPIFE_RDY;
            while (!(temp_status & 0x1))
            {
               temp_status = SPIFE_RDY;
            }
            // fla_if_write(FLA_FIFO_TOP_ADDR, 0x00c80003);
            SPIFE_FTOP = Smfi_Custom_Addr;
            temp_status = SPIFE_FCNT;
            while ((temp_status & 0x3) == 0)
            {
               temp_status = SPIFE_FCNT;
            }
            temp_data = SPIFE_FTOP;
            temp_status = SPIFE_RDY;
            while (!(temp_status & 0x1))
            {
               temp_status = SPIFE_RDY;
            }
            temp_status = SPIFE_STA;
            while (temp_status & 0x1)
            {
               temp_status = SPIFE_STA;
            }
            temp_status = SPIFE_RDY;
            while (!(temp_status & 0x1))
            {
               temp_status = SPIFE_RDY;
            }
            Smf_Data0 = (uint8_t)temp_data;
            Smf_Data1 = (uint8_t)(temp_data >> 8);
            Smf_Data2 = (uint8_t)(temp_data >> 16);
            Smf_Data3 = (uint8_t)(temp_data >> 24);
            SMF_SR |= 0xa; // set serirq interrupt status
            PRINTF_TX = 'f';
         }
         break;
      default:
         break;
   }
   PRINTF_TX = 'g';
   // main();
}
FUNCT_PTR_V_D_BP_L Load_Fla_If_To_Ram(FUNCT_PTR_V_D_BP_L funcpoint, const int malloc_size)
{
   int i;
   FUNCT_PTR_V_D_BP_L Smft_Ptr;
   Smft_Ptr = malloc(malloc_size);
   dprint("Smft_Ptr:0x%p\n", Smft_Ptr);
   if (!Smft_Ptr)
   {
      dprint("attention! malloc failed!\n");
      return 0;
   }
   Tmp_XPntr = (VBYTE *)Smft_Ptr;
   Tmp_code_pointer = (VBYTE *)funcpoint;
   for (i = 0; i < malloc_size; i++)
   {
      *Tmp_XPntr = *Tmp_code_pointer;
      Tmp_XPntr++;
      Tmp_code_pointer++;
   }
   dprint("Load Function to DRAM finish!\n");
   return Smft_Ptr;
}

void Transport_Func_To_iram1(FUNCT_PTR_V_V funcpoint, const int malloc_size)
{
   int i;
   Tmp_XPntr = (VBYTE *)0x34000;
   Tmp_code_pointer = (VBYTE *)funcpoint;
   for (i = 0; i < malloc_size; i++)
   {
      *Tmp_XPntr = *Tmp_code_pointer;
      Tmp_XPntr++;
      Tmp_code_pointer++;
   }
   dprint("Load Function to DRAM finish!\n");
}

void Transport_Update_To_iram1(FUNCT_PTR_B_D_D funcpoint, const int malloc_size)
{
   int i;
   Tmp_XPntr = (VBYTE *)0x34000;
   Tmp_code_pointer = (VBYTE *)funcpoint;
   for (i = 0; i < malloc_size; i++)
   {
      *Tmp_XPntr = *Tmp_code_pointer;
      Tmp_XPntr++;
      Tmp_code_pointer++;
   }
   dprint("Load Function to DRAM finish!\n");
}

FUNCT_PTR_V_V Load_Smfi_To_Dram(FUNCT_PTR_V_V func, const int malloc_size)
{
   int i;
   VDWORD malloc_cnt = 0;
   FUNCT_PTR_V_V func_ptr;
   func_ptr = malloc(malloc_size);
   //dprint("func_ptr value is 0x%x\n", (unsigned int)func_ptr);

   if (!func_ptr)
      dprint("attention! malloc failed!\n");
   VDWORD *Tmp_XPntr = (VDWORD *)func_ptr;
   VDWORD *Tmp_code_pointer = (VDWORD *)func;
   if ((malloc_size % 4 != 0))
   {
      malloc_cnt = (malloc_size / 4) + 1;
   }
   else
   {
      malloc_cnt = (malloc_size / 4);
   }
   for (i = 0; i < malloc_cnt; i++)
   {
      *Tmp_XPntr = *Tmp_code_pointer;
      Tmp_XPntr++;
      Tmp_code_pointer++;
   }
   //dprint("Load Smfi Function to DRAM finish!\n");
   return func_ptr;
}
/**
 * @brief 写入flash操作
 *
 * @param addr          要进行页写入的地址
 * @param write_buff    传入一个数组，数组中的内容是要页写入的值
 * @param length        要写入的字节数，必须小于或等于页的大小(256字节)。
 *
 * @return
 */
void RunSPIFE_WriteFromRAM(DWORD addr, BYTEP write_buff, WORD lentgh)
{
   Disable_Interrupt_Main_Switch();                  // Disable All Interrupt
   WDT_TORR0 = 0xff; WDT_TORR1 = 0xff;                               // 设置最长延时
   WDT_CRR = 0x76;                                   // 重启计数器
   Spif_Ptr = Load_Fla_If_To_Ram(SPIFE_Write, 0x600); // Load Fla_If Code to DRAM
   (*Spif_Ptr)(addr, write_buff, lentgh);                    // Do Function at 0x21000,this 0x21000 just for test,maybe cause some problem
   Enable_Interrupt_Main_Switch();
   free(Spif_Ptr);  // 释放通过 malloc 分配的内存空间
   Spif_Ptr = NULL; // 将指针设置为 NULL，以避免悬空指针问题
}
/**
 * @brief 读取flash操作。
 *
 * @param addr          要进行页写入的起始地址，必须对齐到页大小的边界。
 * @param read_buff    传入一个数组，数组中的内容是要页写入的值。
 * @param length        要读取的字节数，必须小于或等于页的大小(256字节)。
 *
 * @return
 *
 * @note
 * - 该函数建议不要直接使用，在RunSPIFE_ReadFromRAM函数中调用
 */
void RunSPIFE_ReadFromRAM(DWORD addr, BYTEP read_buff, WORD lentgh)
{
   Disable_Interrupt_Main_Switch();                  // Disable All Interrupt
   WDT_TORR0 = 0xff; WDT_TORR1 = 0xff;                           // 设置最长延时
   WDT_CRR = 0x76;                                   // 重启计数器
   Spif_Ptr = Load_Fla_If_To_Ram(SPIFE_Read, 0x800); // Load Fla_If Code to DRAM
   (*Spif_Ptr)(addr, read_buff, lentgh);                     // Do Function at 0x21000,this 0x21000 just for test,maybe cause some problem
   Enable_Interrupt_Main_Switch();
   free(Spif_Ptr);  // 释放通过 malloc 分配的内存空间
   Spif_Ptr = NULL; // 将指针设置为 NULL，以避免悬空指针问题
   for (short i = 0; i < lentgh; i++)
   {
      dprint("Read_buff:%d\n", read_buff[i]);
   }
}
