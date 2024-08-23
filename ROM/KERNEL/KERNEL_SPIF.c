#include <KERNEL_SPIF.H>

// uint32_t EC_4kdata_Upgram(SPIF_INT_EXT selectflsh, void *flashaddr, void *memaddr)
// {
//    // SELECT_FLASH();
//    // SPIF_Erase_FLASH();
// }
// uint32_t EC_Mirror_Update(void)
// {}

SPIF_MODE READ_FLASH_QUAD_ENABLE(const uint32_t looplimit) // 获取flash四路/二路状态
{
   uint32_t flash_status2;
   for (volatile uint32_t counter = looplimit; (!(SPIF_READY & 0x1)) && counter; counter--)
      ;
   for (volatile uint32_t counter = looplimit; (SPIF_STATUS & 0xf) && counter; counter--)
      ;
   SPIF_DBYTE = 0x0;
   SPIF_FIFO_TOP = SPIF_COMMAND_READ_STATUS_REG_2; // 读状态寄存器2
   for (volatile uint32_t counter = looplimit; (!(SPIF_READY & 0x1)) && counter; counter--)
      ;
   for (volatile uint32_t counter = looplimit; (!(SPIF_FIFO_CNT & 0x3)) && counter; counter--)
      ;
   flash_status2 = SPIF_FIFO_TOP;
   flash_status2 >>= 24;
   uint32_t status = (flash_status2 & BIT1) >> 1; // QE BIT
#if ROM_DEBUG
   printf("FLASH QUAD MODE %s\n", status ? "ENABLE" : "DISABLE");
#endif
   return status ? QUAD_FLASH : DUAL_FLASH;
}
SPIF_MODE SELECT_DUAL_QUAD(const SPIF_MODE mode, const uint32_t looplimit) // 选择四路/二路
{
   if (mode == QUAD_FLASH) // 四线
   {
      if (READ_FLASH_QUAD_ENABLE(looplimit) == DUAL_FLASH) // flash侧没有Quad Enable 则打开QE位
      {
         for (volatile uint32_t counter = looplimit; (!(SPIF_READY & 0x1)) && counter; counter--)
            ; // 读忙
         SPIF_DBYTE = 0x1;
         for (volatile uint32_t counter = looplimit; (!(SPIF_READY & 0x1)) && counter; counter--)
            ;                        // 读忙
         SPIF_FIFO_TOP = 0x02020001; // 写状态寄存器
         for (volatile uint32_t counter = looplimit; (!(SPIF_READY & 0x1)) && counter; counter--)
            ; // 读忙
         for (volatile uint32_t counter = looplimit; (SPIF_STATUS & 0xf) && counter; counter--)
            ; // 判断写完
         for (volatile uint32_t counter = looplimit; (!(SPIF_READY & 0x1)) && counter; counter--)
            ; // 读忙
      }
      for (volatile uint32_t counter = looplimit; (!(SPIF_STATUS & BIT4)) && counter; counter--)
         READ_FLASH_QUAD_ENABLE(looplimit); // 判断写完
      if (SPIF_STATUS & BIT4)               // 0:2线，1:4线
      {
         SPIF_CTRL0 |= BIT1; // 成功则使用4线模式
      }
      else
      {
         SPIF_CTRL0 &= ~BIT1; // 失败使用二线模式
      }
   }
   else
   {
      SPIF_CTRL0 &= ~BIT1; // 使用二线模式
   }
#if ROM_DEBUG
   printf("SPIF MODULE IS %s MODE\n", (SPIF_CTRL0 & BIT1) ? "QUAD" : "DUAL");
#endif
   return (SPIF_CTRL0 & BIT1) ? QUAD_FLASH : DUAL_FLASH; // 返回状态
}
void SELECT_FLASH(SPIF_INT_EXT selectflsh, SPIF_MODE mode, SPIF_PIN WP, SPIF_PIN HOLD, const uint32_t looplimit)
{
   // GPIOA16(  5571SPIF_HOLD)--PIO1[1:0]
   SYSCTL_PIO1_CFG &= 0xfffffffc;
   // GPIOB17(  8528SPIF_WP)--PIO3[3:2]
   // GPIOB19(  8528SPIF_HOLD)--PIO3[7:6]
   // GPIOB[23:19]( MOSI/MISO/CSN/SCK)--PIO3[15:8]
   SYSCTL_PIO3_CFG &= 0xffff0033;
   // GPIOD8(  5571SPIF_WP)--PIO5[17:16]
   SYSCTL_PIO5_CFG &= 0xfffcffff;
   if (selectflsh == INTERNAL_FLASH)
   {
      // internal
      SYSCTL_PIO4_UDCFG &= 0xfffffffd; // sysclt 0x3047c bit1=0;
   }
   else if (selectflsh == EXTERNAL_FLASH) // set extern now
   {
      // external flash iomux
      // sysclt 0x30460 G7-4==func1;
      SYSCTL_PIO3_CFG |= 0x00005500; // GPIOB[23:19](SPIF MOSI/MISO/CSN/SCK)--PIO3[15:8]
      if (mode == QUAD_FLASH)
      {
         if (WP == PIN5571_FLASH) // select 5571 GPIOD8 1
         {
            SYSCTL_PIO5_CFG |= 0x00010000; // GPIOD8(5571SPIF_WP)--PIO5[17:16]
         }
         else if (WP == PIN8528_FLASH) // default 8528 GPIOB17 0
         {
            SYSCTL_PIO3_CFG |= 0x00000004; // GPIOB17(8528SPIF_WP)--PIO3[3:2]
         }
         if (HOLD == PIN5571_FLASH) // select 5571 GPIOA16 1
         {
            SYSCTL_PIO1_CFG |= 0x00000002; // GPIOA16(5571SPIF_HOLD)--PIO1[1:0]
         }
         else if (HOLD == PIN8528_FLASH) // default 8528 GPIOB19 0
         {
            SYSCTL_PIO3_CFG |= 0x00000040; // GPIOB19(  8528SPIF_HOLD)--PIO3[7:6]
         }
      }
      // external
      SYSCTL_PIO4_UDCFG |= 0x00000002; // sysclt 0x3047c bit1=1;
   }
   SYSCTL_RST1 |= 0x00000100; // 复位
   SYSCTL_RST1 &= 0xfffffeff; // 清除复位
#if ROM_DEBUG
   printf("USED %s FLASH\n", (SYSCTL_PIO4_UDCFG & BIT1) ? "EXTERNAL" : "INTERNAL");
#endif
   SELECT_DUAL_QUAD(mode, looplimit);
}

void *SPIF_Read_FLASH(void *flashaddr, void *memaddr, size_t size, const uint32_t looplimit)
{
   if (size > 256)
      size = 256; // 最多不超过256字节
   // size += 0b100 - (size & 0b11);//对齐
   if (!size || memaddr == NULL)
      return NULL;
   u_int32_t addr = (u_int32_t)flashaddr;
   u_int32_t buff = (u_int32_t)memaddr;
   // 3
   for (volatile uint32_t counter = looplimit; (!(SPIF_READY & 0x1)) && counter; counter--)
      ; // 读忙
   for (volatile uint32_t counter = looplimit; (SPIF_STATUS & 0xf) && counter; counter--)
      ; // 直到写完
   for (volatile uint32_t counter = looplimit; (!(SPIF_READY & 0x1)) && counter; counter--)
      ; // 读忙
   SPIF_DBYTE = (size - 1) & 0xff;
   for (volatile uint32_t counter = looplimit; (!(SPIF_READY & 0x1)) && counter; counter--)
      ; // 读忙
   u_int32_t command;
   if (SPIF_CTRL0 & BIT1)
      command = SPIF_COMMAND_FASTREAD_QUAD_OUTPUT /*SPIF_COMMAND_FASTREAD_QUAD_IO*/; // 考虑兼容性问题，高新更模式和回绕问题不使用eb，由于软件操作，实际影响相对小（受指令时序影响）
   else
      command = SPIF_COMMAND_FASTREAD_DUAL_OUTPUT /*SPIF_COMMAND_FASTREAD_DUAL_IO*/; // 考虑兼容性问题，高新更模式和回绕问题不使用bb，由于软件操作，实际影响相对小（受指令时序影响）
                                                                                     // #if ROM_DEBUG
                                                                                     //    printf("READ CMD = %x\n", command);
                                                                                     // #endif
                                                                                     // if(buff & 0b11)//4字节//会影响实际执行
                                                                                     // {
                                                                                     // 	buff += (0b100 - (buff & 0b11));
                                                                                     // }
   SPIF_FIFO_TOP = (((addr & 0xFF) << 24) + ((addr & 0xFF00) << 8) + ((addr & 0xFF0000) >> 8) + command);
   for (uint32_t j = 0; j < (size >> 2); j++) /*如果有必要，此处采用汇编*/
   {
      for (volatile uint32_t counter = looplimit; (!(SPIF_FIFO_CNT & 0x3)) && counter; counter--)
         ;
      (*(uint32_t *)buff) = SPIF_FIFO_TOP;
      // #if ROM_DEBUG
      // 	printf("read flash buff %#08x\n", (*(uint32_t *)buff));
      // #endif
      buff += 4;
   }
   if (size & 0b11)
   {
      size &= 0b11;
      for (volatile uint32_t counter = looplimit; (!(SPIF_FIFO_CNT & 0x3)) && counter; counter--)
         ;

      // (*(uint32_t *)buff) = (SPIF_FIFO_TOP >> (8 * (4 - size)));
      (*(uint32_t *)buff) = ((SPIF_FIFO_TOP >> (8 * (4 - size))) | ((*(uint32_t *)buff) & (0xffffffff << (8 * size)))); // 防止元数据被写入
      // #if ROM_DEBUG
      //    printf("read flash data %#08x\n", (*(uint32_t *)buff) & (0xffffffff >> (8 * (4 - size))));
      // #endif
      //    uint32_t data = SPIF_FIFO_TOP;
      //   // #if ROM_DEBUG
      //   // 	printf("read flash data %#08x\n", data);
      //   // #endif
      //    while(size--)
      //    {
      //       *(uint8_t *)(buff + size) = (uint8_t)((data & 0xff000000) >> 24);
      //       data <<= 8;
      //    }
   }
   return memaddr;
}
void *SPIF_Wirte_FLASH(void *flashaddr, void *memaddr, size_t size, const uint32_t looplimit)
{
   if (size > 256)
      size = 256; // 最多不超过256字节
   // size += 0b100 - (size & 0b11);//对齐
   if (!size || memaddr == NULL)
      return NULL;
   u_int32_t addr = (u_int32_t)flashaddr;
   u_int32_t buff = (u_int32_t)memaddr;
   // 3
   for (volatile uint32_t counter = looplimit; (!(SPIF_READY & 0x1)) && counter; counter--)
      ; // 读忙
   for (volatile uint32_t counter = looplimit; (SPIF_STATUS & 0xf) && counter; counter--)
      ; // 直到写完
   for (volatile uint32_t counter = looplimit; (!(SPIF_READY & 0x1)) && counter; counter--)
      ; // 读忙
   SPIF_DBYTE = (size - 1) & 0xff;
   for (volatile uint32_t counter = looplimit; (!(SPIF_READY & 0x1)) && counter; counter--)
      ; // 读忙
   u_int32_t command;
   if (SPIF_CTRL0 & BIT1)
      command = SPIF_COMMAND_QUAD_PAGE_PROGRAM /*SPIF_COMMAND_SECTIOR_ERASE*/; //
   else
      command = SPIF_COMMAND_PAGE_PROGRAM /*SPIF_COMMAND_DUAD_PAGE_PROGRAM*/; //
                                                                              // #if ROM_DEBUG
                                                                              //    printf("Write CMD = %x\n", command);
                                                                              // #endif
   // if(buff & 0b11)//4字节//会影响实际执行
   // {
   // 	buff += (0b100 - (buff & 0b11));
   // }
   SPIF_FIFO_TOP = (((addr & 0xFF) << 24) + ((addr & 0xFF00) << 8) + ((addr & 0xFF0000) >> 8) + command);
   for (uint32_t j = 0; j < (size >> 2); j++) /*如果有必要，此处采用汇编*/
   {
      for (volatile uint32_t counter = looplimit; ((SPIF_FIFO_CNT & 0x3) >= 2) && counter; counter--)
         ;
      SPIF_FIFO_TOP = (*(uint32_t *)buff);
      // #if ROM_DEBUG
      // 	printf("write flash buff %#08x\n", (*(uint32_t *)buff));
      // #endif
      buff += 4;
   }
   if (size & 0b11)
   {
      size &= 0b11;

      for (volatile uint32_t counter = looplimit; (!(SPIF_FIFO_CNT & 0x3)) && counter; counter--)
         ;
      // SPIF_FIFO_TOP = (*(uint32_t *)buff);
      SPIF_FIFO_TOP = (*(uint32_t *)buff) & (0xffffffff >> (8 * (4 - size)));
      // #if ROM_DEBUG
      //    printf("write flash data %#08x\n", (*(uint32_t *)buff) & (0xffffffff >> (8 * (4 - size))));
      // #endif
   }
   return memaddr;
}
uint32_t SPIF_Erase_FLASH(uint8_t command, void *flash_addr, const uint32_t looplimit)
{
   uint32_t fifo = 0;
   uint32_t addr = 0;
   switch (command)
   {
   case SPIF_COMMAND_4KSECTIOR_ERASE: // 4k//0x1000
      addr = (uint32_t)flash_addr & (0x00fff000);
      break;
   case SPIF_COMMAND_32KBLOCK_ERASE: // 32k//0x8000
      addr = (uint32_t)flash_addr & (0x00ff8000);
      break;
   case SPIF_COMMAND_64KBLOCK_ERASE: // 64k//0x10000
      addr = (uint32_t)flash_addr & 0x00ff0000;
      break;
   case SPIF_COMMAND_CHIP_ERASE1: // chip
   case SPIF_COMMAND_CHIP_ERASE2: // chip
      addr = 0;
      break;
   default: // 不是擦除命令，退出
      return -1;
   }
   fifo = (((addr & 0xFF) << 24) + ((addr & 0xFF00) << 8) + ((addr & 0xFF0000) >> 8)) | command;

   for (volatile uint32_t counter = looplimit; (!(SPIF_READY & 0x1)) && counter; counter--)
      ; // 读忙
   SPIF_FIFO_TOP = fifo;
   for (volatile uint32_t counter = looplimit; (!(SPIF_READY & 0x1)) && counter; counter--)
      ; // 读忙
   for (volatile uint32_t counter = looplimit; (SPIF_STATUS & 0xf) && counter; counter--)
      ; // 直到写完
   for (volatile uint32_t counter = looplimit; (!(SPIF_READY & 0x1)) && counter; counter--)
      ; // 读忙
#if ROM_DEBUG && 0
   printf("EC ERASE(CMD:%#x) FLASH ADDR %#x\n", command, addr);
#endif
   return 0;
}
uint32_t SPIF_Read_ID(const uint32_t looplimit)
{
   SPIF_CTRL0 = 0x1;
   // 5ms延迟
   for (volatile uint32_t counter = looplimit / 200; (!(SPIF_READY & 0x1)) && counter; counter--)
      ; // 读忙
   SPIF_DBYTE = 0x1;
   // 5ms延迟
   for (volatile uint32_t counter = looplimit / 200; (!(SPIF_READY & 0x1)) && counter; counter--)
      ; // 读忙
   if (SPIF_CTRL0 & BIT1)
      SPIF_FIFO_TOP = SPIF_COMMAND_QUADIO_MID_DID;
   else
      SPIF_FIFO_TOP = SPIF_COMMAND_DUALIO_MID_DID;
   // 5ms延迟
   for (volatile uint32_t counter = looplimit / 200; (!(SPIF_READY & 0x1)) && counter; counter--)
      ; // 读忙
   return (SPIF_FIFO_TOP >> 16);
}

uint32_t READ_FLASH_INFO(const uint32_t looplimit)
{
   uint32_t error;
   uint64_t mcycle0 = READ_CSR_MCYCLE_VAR;
   error = 0;
   internal_flash_state = external_flash_state = ROM_NOFirmware; // 目前没有flash信息
repeat:
{ // 读内部FLASH信息（无论是否读到都继续走）
   // 切换内部FLASH-4线（内部默认4线模式）
   SELECT_FLASH(INTERNAL_FLASH, QUAD_FLASH, NOPIN_FLASH, NOPIN_FLASH, looplimit);
   // 获取内部flash的识别字符存在内部FLASH的字符就读取内部FLASH，不存在
   SPIF_Read_FLASH((void *)flash_info_addr, (void *)&INT_FLASH_FIX_INFO, sizeof(sFixedFlashInfo), looplimit);
   if (!strcmp((const char *)INT_FLASH_FIX_INFO.Firmware_ID, (const char *)FIX_FIRMWARE_ID))
   {
      internal_flash_state = ROM_Firmware_Reader; // 存在内部FLASH信息，先清空状态
      // 获取ROM信息写入的
      register uint32_t DynamicFlashInfo = INT_FLASH_FIX_INFO.DynamicFlashInfo;

      // FLASH DynamicFlashInfo
      if (INT_FLASH_FIX_INFO.EXTERNAL_FLASH_CTRL.SPACE_OFFSET == 1) // 如果是EC映射地址相对值或者隐式flash信息
      {
         DynamicFlashInfo -= FLASH_BASE_ADDR; // 减去本身映射地址
      }
      if (DynamicFlashInfo > 0x100000) // 假设它是flash地址的绝对值
      {
         DynamicFlashInfo -= INT_FLASH_FIX_INFO.Mirror_Addr;
      }
#if ROM_DEBUG
      printf("INTERNAL FLASH DYNAMIC INFO FIRMWARE ADDR: %#lx\n", DynamicFlashInfo);
#endif
      // 获取尾部FLASH的信息状态
      SPIF_Read_FLASH((void *)(DynamicFlashInfo), (void *)&INT_FLASH_DYNAMIC_INFO, sizeof(sDynamicFlashInfo), looplimit);

      if (strcmp((const char *)INT_FLASH_DYNAMIC_INFO.Firmware_ID, (const char *)DYN_FIRMWARE_ID))
         internal_flash_state = ROM_NODynamic;
   }
#if ROM_DEBUG
   else
   {
      printf("INTERNAL FLASH NO FIRMWARE\n");
   }
#endif
}
#if ROM_DEBUG
   printf("INTERNAL FLASH FIX INFO FIRMWARE ID: %s\n", INT_FLASH_FIX_INFO.Firmware_ID);
   printf("INTERNAL FLASH DYNAMIC INFO FIRMWARE ID: %s\n", INT_FLASH_DYNAMIC_INFO.Firmware_ID);
#endif
   // 由于内外部FLASH的特殊权限性质，因此该部分硬件复位时候必须由内部FLASH决定或者默认值，只有软复位的时候才可以通过flash程序导入（预留软件干涉通道）
   if (SYSCTL_RAM_CTRL) // if ram ctrl used(SYSCTL_RESERVER BIT15)
   {
      FLASH_CTRL = SYSCTL_RESERVER & 0xff;
   }
   else if (internal_flash_state != ROM_NOFirmware) // 如果内部FLASH读到了固件
   {
      FLASH_CTRL = INT_FLASH_FIX_INFO.EXTERNAL_FLASH_CTRL.SYSCTL_RESERVERD & 0xff;
   }
   else // 内部FLASH没固件
   {
      FLASH_CTRL = 0x7f; // 允许外部FLASH使用，全16M4k为间隔查找[23:12个地址],
   }
#if ROM_DEBUG
   printf("%s INFO FLASH CTRL:%#lx\n", (SYSCTL_RAM_CTRL ? "SYSCTL" : (internal_flash_state != ROM_NOFirmware ? "FLASH" : "DEFAULT")), FLASH_CTRL);
#endif
   if (EXTERNAL_FLASH_ENABLE(FLASH_CTRL)) // 如果使用外部flash
   {                                      // 读外部fLASH信息（无论任何配置）
      // 为防止更换外部FLASH切换外部FLASH-2线（外部默认2线模式）
      SELECT_FLASH(EXTERNAL_FLASH, DUAL_FLASH, NOPIN_FLASH, NOPIN_FLASH, looplimit);
      // 读取FLASH ID
      FLASH_ID = SPIF_Read_ID(looplimit);
#if ROM_DEBUG
      printf("EXTERNAL FLASH ID:%#lx\n", FLASH_ID);
#endif
      if ((FLASH_ID & 0xf0ff) == (SPIF_COMMAND_MID_DID << 8)) // 命令发送不出去(防止设备id号最高位为9的情况，同时防止其他读命令的代码)
      {
         external_flash_state = ROM_NOFLASH; // 没有flash
      }
      else
      {
         if (internal_flash_state == ROM_Firmware_Reader && (INT_FLASH_DYNAMIC_INFO.Info.FixedFlashInfo_Addr != 0xffffff)) // 如果内部FLASH存在信息则先读取相关位置信息
         {
            register uint32_t flash_start;
            flash_start = (INT_FLASH_DYNAMIC_INFO.Info.FixedFlashInfo_Addr & (~0xfff)) + flash_info_addr; // 上次地址优先查找（保存绝对值）
            SPIF_Read_FLASH((void *)flash_start, (void *)&EXT_FLASH_FIX_INFO, sizeof(sFixedFlashInfo), looplimit);
            if (!strcmp((const char *)EXT_FLASH_FIX_INFO.Firmware_ID, (const char *)FIX_FIRMWARE_ID))
               external_flash_state = ROM_Firmware_Reader; // 固件纸存在
#if ROM_DEBUG
            printf("READ EXTERNAL FLASH LAST ADDR = %#lx\n", flash_start);
#endif
         }
         // 开始轮询查找
         if (external_flash_state != ROM_Firmware_Reader && FLASH_LowAddr_OFFSET(FLASH_CTRL) != 0) // 没有找到并且开启轮询查找
         {

            register uint32_t offset = 0;
            register uint32_t flash_start = 0;
            register uint32_t flash_end = 0;
            if (FLASH_LAST8M(FLASH_CTRL) == 1)
            {
               flash_start = 0;
            }
            else if (FLASH_LAST8M(FLASH_CTRL) == 0)
            {
               flash_start = (8 * 1024 * 1024); // 从8M开始
            }
            offset = ((256 * 1024) >> ((FLASH_LowAddr_OFFSET(FLASH_CTRL)) << 1));
            if (FLASH_HighAddr_SPACE(FLASH_CTRL) == 7)
            {
               flash_end = 0x1000000; // 16M
            }
            else
            {
               flash_end = flash_start + (1 << (17 + FLASH_HighAddr_SPACE(FLASH_CTRL)));
            }
#if ROM_DEBUG
            printf("FLASH FIND INFO START %#lx\n", flash_start);
            printf("FLASH FIND INFO OFFSET %#lx\n", offset);
            printf("FLASH FIND INFO END %#lx\n", flash_end);
#endif
            do
            {
               // #if ROM_DEBUG
               //    printf("FLASH NOW FIND  ADDR %#lx\n", (flash_start + flash_info_addr));
               // #endif
               SPIF_Read_FLASH((void *)(flash_start + flash_info_addr), (void *)&EXT_FLASH_FIX_INFO, sizeof(uint32_t), looplimit);
               if ((*(uint32_t *)&EXT_FLASH_FIX_INFO.Firmware_ID[0]) == (*(uint32_t *)&FIX_FIRMWARE_ID[0]))
               {
                  SPIF_Read_FLASH((void *)(flash_start + flash_info_addr), (void *)&EXT_FLASH_FIX_INFO, sizeof(sFixedFlashInfo), looplimit);
                  if (!strcmp((const char *)EXT_FLASH_FIX_INFO.Firmware_ID, (const char *)FIX_FIRMWARE_ID))
                  {
                     external_flash_state = ROM_Firmware_Reader; // 固件纸存在
                     break;
                  }
               }
               // #if ROM_DEBUG
               //    printf("FLASH NOW FIND  uint32 (%#08x)\n", *(uint32_t *)&EXT_FLASH_FIX_INFO.Firmware_ID[0]);
               // #endif
               flash_start += offset;
            } while (flash_start < flash_end);
         }
         if (external_flash_state == ROM_Firmware_Reader) // 有数值
         {
            // 获取ROM信息写入的
            register uint32_t DynamicFlashInfo = (EXT_FLASH_FIX_INFO.DynamicFlashInfo + EXT_FLASH_FIX_INFO.Mirror_Addr);
            // FLASH DynamicFlashInfo
            if (EXT_FLASH_FIX_INFO.EXTERNAL_FLASH_CTRL.SPACE_OFFSET == 1) // 如果是EC映射地址相对值或者隐式flash信息
            {
               DynamicFlashInfo -= FLASH_BASE_ADDR; // 减去本身映射地址
            }
            // 获取尾部FLASH的信息状态
            SPIF_Read_FLASH((void *)(DynamicFlashInfo), (void *)&EXT_FLASH_DYNAMIC_INFO, sizeof(sDynamicFlashInfo), looplimit);
            if (strcmp((const char *)EXT_FLASH_DYNAMIC_INFO.Firmware_ID, (const char *)DYN_FIRMWARE_ID))
               internal_flash_state = ROM_NODynamic;
         }
#if ROM_DEBUG
         printf("EXTERNAL FLASH FIX INFO FIRMWARE ID:%s\n", EXT_FLASH_FIX_INFO.Firmware_ID);
         printf("EXTERNAL FLASH DYNAMIC INFO FIRMWARE ID:%s\n", EXT_FLASH_DYNAMIC_INFO.Firmware_ID);
#endif
      }
   }
   // 对状态位判断
   if ((external_flash_state == ROM_Firmware_Reader || external_flash_state == ROM_NODynamic) && strcmp((const char *)EXT_FLASH_FIX_INFO.Compiler_Version, (const char *)INT_FLASH_FIX_INFO.Compiler_Version))
   { // 存在外部的fixed INFO且不一致
      external_flash_used = 1;
      SPIF_MODE WP = NOPIN_FLASH, HOLD = NOPIN_FLASH;
      SPIF_MODE mode = DUAL_FLASH;
      if (EXT_FLASH_FIX_INFO.EXTERNAL_FLASH_CTRL.SPI_Switch == 0)
      {
         mode = QUAD_FLASH;
         if (EXT_FLASH_FIX_INFO.EXTERNAL_FLASH_CTRL.WP_Switch)
            WP = PIN5571_FLASH;
         else
            WP = PIN8528_FLASH;
         if (EXT_FLASH_FIX_INFO.EXTERNAL_FLASH_CTRL.HOLD_Switch)
            HOLD = PIN5571_FLASH;
         else
            HOLD = PIN8528_FLASH;
      }
      SELECT_FLASH(EXTERNAL_FLASH, mode, WP, HOLD, looplimit);
      FLASH_FIX_INFO_PTR = &EXT_FLASH_FIX_INFO;
      if (external_flash_state != ROM_NODynamic)
         FLASH_DYNAMIC_INFO_PTR = &INT_FLASH_DYNAMIC_INFO;
      else // 如果没有开安全和patch功能该情况是允许的，此时会按默认情况配置
         FLASH_DYNAMIC_INFO_PTR = NULL;
   }
   else if ((internal_flash_state == ROM_Firmware_Reader || internal_flash_state == ROM_NODynamic))
   { // 只有内部FLASH
      external_flash_used = 0;
      // 采用内部模式
      SELECT_FLASH(INTERNAL_FLASH, QUAD_FLASH, NOPIN_FLASH, NOPIN_FLASH, looplimit);
      FLASH_FIX_INFO_PTR = &INT_FLASH_FIX_INFO;
      if (internal_flash_state != ROM_NODynamic)
         FLASH_DYNAMIC_INFO_PTR = &INT_FLASH_DYNAMIC_INFO;
      else // 如果没有开安全和patch功能该情况是允许的，此时会按默认情况配置
         FLASH_DYNAMIC_INFO_PTR = NULL;
   }
   else
   { // 不存在
      // 如果没有固件
      if (!error) // 如果内部都没有
      {
         error++;
         goto repeat; // 再去读一次内部flash
      }
      else
      {
         register uint64_t mcycle1 = READ_CSR_MCYCLE_VAR;
         READ_FLASH_INFO_TIME = mcycle1 - mcycle0;
         return ROM_CNT_READ_INFO_ERROR;
      }
   }
   register uint64_t mcycle1 = READ_CSR_MCYCLE_VAR;
   READ_FLASH_INFO_TIME = mcycle1 - mcycle0;
   return 0;
}

void ALIGNED(4) Mailbox_EFLASH_Mirror_Trigger(BYTE mode, DWORD fw_size, DWORD start_addr)
{
   printf("mailbox Mirror eflash\n");

   __nop__;
   __nop__;

   E2CINFO0 = 0x10;                            // 命令字
   E2CINFO1 = ((DWORD)(mode << 24) | fw_size); // BYTE3:固件位置标志 BYTE0~2:固件大小
   E2CINFO2 = start_addr;                      // 更新起始地址
   E2CINT = 0x2;

   printf("等待mirror完毕\n");

   while (C2EINFO0 != 0x10)
      ; // 等待子系统更新完毕回复

   if (C2EINFO1 == 0x1)
   {
      printf("更新完毕\n");
      PRINTF_TX = 'D';
      WDT_TORR = 0xffff; // 设置最长延时
      WDT_CR |= 0x02;    // 进入wdt中断
      WDT_CRR = 0x76;    // 重启计数器
      // SYSCTL_RESERVED |= BIT3;
   }
   else if (C2EINFO1 == 0x2)
   {
      printf("更新失败:%x\n", C2EINFO1);
      PRINTF_TX = 'E';
      WDT_TORR = 0xffff; // 设置最长延时
      WDT_CR |= 0x02;    // 进入wdt中断
      WDT_CRR = 0x76;    // 重启计数器
   }
   else
   {
      printf("错误回复:%x\n", C2EINFO1);
      PRINTF_TX = 'E';
      WDT_TORR = 0xffff; // 设置最长延时
      WDT_CR |= 0x02;    // 进入wdt中断
      WDT_CRR = 0x76;    // 重启计数器
   }
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