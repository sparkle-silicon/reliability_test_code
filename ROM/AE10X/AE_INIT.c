/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-19 18:52:02
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

void ROM_PATCH(void)
{
    uint32_t PATCH_PTR = ROM_PATCH_ADDR + PATCH0_OFESET;
    volatile spatch_typdef *PATCH_INFO = FLASH_DYNAMIC_INFO_PTR->PATCH;
    for (size_t count = 0; count < 8; count++)
    {
        REG8(PATCH_PTR + PATCH_ADDRL_OFFSET) = PATCH_INFO[count].addrl;
        REG8(PATCH_PTR + PATCH_ADDRH_OFFSET) = PATCH_INFO[count].addrh;
        REG8(PATCH_PTR + PATCH_DATA0_OFFSET) = PATCH_INFO[count].data.byte[0];
        REG8(PATCH_PTR + PATCH_DATA1_OFFSET) = PATCH_INFO[count].data.byte[1];
        REG8(PATCH_PTR + PATCH_DATA2_OFFSET) = PATCH_INFO[count].data.byte[2];
        REG8(PATCH_PTR + PATCH_DATA3_OFFSET) = PATCH_INFO[count].data.byte[3];
        if (PATCH_INFO[count].last)
        {
            PATCH_PTR += 6;
        }
        else
            break;
    }
    return;
}

void MODULE_INIT(uint32_t *looplimit)
{
    uint32_t uart_channel = 0;
    // 1.config clock
    // if(FLASH_FIX_INFO_PTR->RESERVED_EXTCLOCK_Disable == 0) // 打开外部时钟
    // {
    //     // IOMUX
    //     // NONE CODE
    //     if(FLASH_FIX_INFO_PTR->RESERVED_EXTClock_Switch == 0) // 选择外部(因为只能切外部,不能切回内部)
    //     {
    //         // switch external low clock
    //         SYSCTL_SWITCH_PLL &= ~BIT0;
    //     }
    // }
    SYSCTL_CLKDIV_OSC80M = (FLASH_FIX_INFO_PTR->MainFrequency - 1); // 配置主频
    nop;
    nop;
    nop;
    *looplimit = (CHIP_CLOCK_INT_HIGH / 15) / (SYSCTL_CLKDIV_OSC80M + 1); // 重新配置超时机制
    // 2. Initialize APB CLOCK BUS ENABLE
    apb_MoudleClock_EN;
    // 4. Initialize DEBUG PRINTF
    if (FLASH_FIX_INFO_PTR->DEBUG_PRINTF_Enable && FLASH_FIX_INFO_PTR->UART_Enable)
    {
        uart_channel = FLASH_FIX_INFO_PTR->Uartn_Print_SWitch; // ^ 0b10;
        if (!((uart_channel == UART1_CHANNEL) && ((SYSCTL_PIO5_CFG & (0b1111U << 28)) == (0b1111U << 28))))
        {
            u_int32_t baud = (FLASH_FIX_INFO_PTR->DEBUG_BAUD_RATE + 1) * 3200;
            u_int32_t lcr = (FLASH_FIX_INFO_PTR->DEBUG_PRINTF_DLS);
            lcr |= (FLASH_FIX_INFO_PTR->DEBUG_PRINTF_STOP << 2);
            lcr |= (FLASH_FIX_INFO_PTR->DEBUG_PRINTF_PE << 3);
            lcr |= (FLASH_FIX_INFO_PTR->DEBUG_PRINTF_EPE << 4);
            lcr &= 0x1f;
            PRINTF_FIFO = UARTA_BASE_ADDR + (0x400 * (uart_channel ^ 0b10));
            baud = UART_Init(uart_channel, baud, lcr);
            printf("UART%x Init done\n", ((uart_channel & 0b10) ? uart_channel + 8 : uart_channel));
            if (!FLASH_FIX_INFO_PTR->DEBUG_LEVEL)
            {
                printf("BAUD = %d\n", baud);
            }
        }
        else
        {
            PRINTF_FIFO = *(volatile uint32_t *)NULL;
        }
        if (!FLASH_FIX_INFO_PTR->DEBUG_LEVEL)
        {
            printf("CHIP FREQ %d Hz\n", CHIP_CLOCK_INT_HIGH / (SYSCTL_CLKDIV_OSC80M + 1));
        }
    }
    // 5. Initialize DEBUGGER FUNCTION
    if (FLASH_FIX_INFO_PTR->DEBUGGER_Enable)
    {
        if (FLASH_FIX_INFO_PTR->DEBUGGER_UART_Enable && (SYSCTL_PIO5_CFG & (0b1111U << 28)) != (0b1111U << 28))
        {
            u_int32_t baud = (FLASH_FIX_INFO_PTR->DEBUG_BAUD_RATE + 1) * 3200;
            u_int32_t lcr = (FLASH_FIX_INFO_PTR->DEBUGGER_DLS);
            lcr |= (FLASH_FIX_INFO_PTR->DEBUGGER_STOP << 2);
            lcr |= (FLASH_FIX_INFO_PTR->DEBUGGER_PE << 3);
            lcr |= (FLASH_FIX_INFO_PTR->DEBUGGER_EPE << 4);
            lcr &= 0x1f;
            if (PRINTF_FIFO != UART1_CHANNEL)
            {
                DEBUGGER_FIFO = UART1_BASE_ADDR;
                baud = UART_Init(UART1_CHANNEL, baud, lcr);
                printf("UART1 Init done\n");
            }
            else
            {
                DEBUGGER_FIFO = PRINTF_FIFO;
                printf("This port will be used together because of the debugger and printf redirection conflict\n");
            }
            if (!FLASH_FIX_INFO_PTR->DEBUG_LEVEL)
            {
                printf("BAUD = %d\n", baud);
            }
        }
        if (FLASH_FIX_INFO_PTR->DEBUGGER_SMBUS_Enable)
        {
            // code
            printf("DEBUGEER SMBUS NO USED,READ CLOCK %dk\n", FLASH_FIX_INFO_PTR->SMBUS_CLOCK_Switch ? 100 : 400);
        }
    }
    return;
}
void Init(void)
{
    // //方案二，C跳转
    // nop;
    // if(SYSCTL_ESTAT & BIT4)
    // {
    //     // SELECT_FLASH(EXTERNAL_FLASH, DUAL_FLASH, NOPIN_FLASH, NOPIN_FLASH);
    //     SYSCTL_PIO3_CFG = (SYSCTL_PIO3_CFG & 0xffff00ff) | 0x00005500;// GPIOB[23:19](SPIF MOSI/MISO/CSN/SCK)--PIO3[15:8]
    //     SYSCTL_PIO4_UDCFG |= 0x00000002; // sysclt 0x3047c bit1=1
    //     SYSCTL_RST1 |= 0x00000100; // 复位
    //     nop;
    //     SYSCTL_RST1 &= 0xfffffeff; // 清除复位
    //     nop;
    //     goto * 0x80084;
    // }
    // nop;

    uint32_t looplimit = (CHIP_CLOCK_INT_HIGH / 15) / (SYSCTL_CLKDIV_OSC80M + 1); // 根据实际测试80M用20M频率需要64sec，因此需要除以16才能达到80M为1sec，20M为4sec，最高32sec
#if GLE01
    PRINTF_FIFO = UART0_BASE_ADDR;
    UART_Init(0, 115200, 3);
    printf("UART0 Init done\n");
    // SELECT_FLASH(INTERNAL_FLASH, QUAD_FLASH, NOPIN_FLASH, NOPIN_FLASH, looplimit);
    SELECT_FLASH(INTERNAL_FLASH, DUAL_FLASH, NOPIN_FLASH, NOPIN_FLASH, looplimit);
    goto * 0x80084;
#else
    { // 初始化信息
        register uint64_t mcycle0, mcycle1;
        mcycle0 = READ_CSR_MCYCLE_VAR; // 保存时钟周期起始值
        nop;                           // 等待时钟周期
        mcycle1 = READ_CSR_MCYCLE_VAR; // 判断是否出现时钟周期变化
        if (mcycle0 == mcycle1)
        {
            asm volatile("csrci 0x320, 0x5\n");
        } // 正常来说不可能，因此再开一次，无论成功与否都不再管理（假设该模块损坏）
        ROM_CSR_MCYCLE = mcycle0;
        // mcycle0 = READ_CSR_MCYCLE_VAR;
        // mcycle1 = READ_CSR_MCYCLE_VAR;
        ROM_CSR_MCYCLE_OVERHEAD = mcycle1 - mcycle0;
        ROM_ERROR = 0;                                                       // error = 0
        looplimit = (CHIP_CLOCK_INT_HIGH / 15) / (SYSCTL_CLKDIV_OSC80M + 1); // 根据实际测试80M用20M频率需要64sec，因此需要除以16才能达到80M为1sec，20M为4sec，最高32sec
        ROM_CNT = ROM_CNT_ROMREADY;                                          // cnt = 1
        DEBUGGER_FIFO = PRINTF_FIFO = (volatile uint32_t)NULL;
#if ROM_DEBUG
        { // debug临时
          // UART0 IOMUX
            SYSCTL_PIO1_CFG = (SYSCTL_PIO1_CFG & (0b1111 << 16)) | (0b1010 << 16);
            // BAUD RATE
            uint32_t uart0baudRate = 115200;

            PRINTF_FIFO = UART0_BASE_ADDR;
            register DWORD divisor;
            register DWORD freq;
            freq = CHIP_CLOCK_INT_HIGH / (SYSCTL_CLKDIV_OSC80M + 1) / (SYSCTL_CLKDIV_UART + 1);
            divisor = freq / uart0baudRate + 0b100; // 27
            // Set DLAB to 1 in LCR
            UART0_LCR = UART_LCR_DLAB | UART_LCR_DLS_8bit;
            divisor >>= 3;
            UART0_DLL = divisor;
            UART0_DLH = (divisor >> 8);
            // Set Line Control Register
            UART0_FCR = UART_FCR_TFTL_8BYTE | UART_FCR_FIFOEN | UART_FCR_TFR | UART_FCR_RFR;
            UART0_LCR &= ~UART_LCR_DLAB;
            UART0_IER = 0; // 不接收中断
            printf("SPK32AE103 ROM TestDebug Baud Rate = %d\n", freq / (divisor << 3));
            // printf("SPK32AE103 ROM MCYCLE OVERHEAD = %Ld\n", ROM_CSR_MCYCLE_OVERHEAD);
            // 校准timeout用
            {
                // uint64_t mcycle1 = READ_CSR_MCYCLE_VAR;
                // for(volatile uint32_t counter = ROM_TIMEOUT; ((SPIF_READY & 0x1)) && counter; counter--);
                // uint64_t mcycle2 = READ_CSR_MCYCLE_VAR;
                // uint64_t mcycle3 = READ_CSR_MCYCLE_VAR;
                // printf("mcycle counter = %d\n", (mcycle2 - mcycle1 - (mcycle3 - mcycle2)));
            }
            // 测试flash写读
            {
                // SELECT_FLASH(EXTERNAL_FLASH, QUAD_FLASH, PIN5571_FLASH, PIN5571_FLASH);
                // void *flash_addr = NULL;
                // char readbuff[256];
                // char space[256];
                // for(size_t i = 1; i < 26; i++)
                // {
                //     SPIF_Read_FLASH(flash_addr, space, 256);
                //     SPIF_Erase_FLASH(SPIF_COMMAND_4KSECTIOR_ERASE, flash_addr);
                //     SPIF_Read_FLASH(flash_addr, readbuff, 256);
                //     // printf("Clear extern flash %#x:\n", flash_addr);
                //     for(size_t j = 1; j < 26; j++)
                //     {
                //         // printf("%#02hhx ", readbuff[j]);
                //     }
                //     // printf("\n");
                //     SPIF_Wirte_FLASH(flash_addr, space, i);
                //     SPIF_Read_FLASH(flash_addr, readbuff, i);
                //     // printf("write extern flash %#x:\n", flash_addr);
                //     for(size_t j = 1; j < 26; j++)
                //     {
                //         // printf("%#02hhx ", readbuff[j]);
                //     }
                // }
            }
        }
#endif
    }
    { // 第一步 获取flash信息
#if ROM_DEBUG
        printf("Read Flash_Info Start\n");
#endif
        {
            ROM_ERROR = READ_FLASH_INFO(looplimit);
            if (ROM_ERROR == ROM_CNT_READ_INFO_ERROR)
            {
#if ROM_DEBUG
                {
                    printf("Read FLASH INFO ERROR\n");
                }
#endif
                exit(ROM_ERROR); // goto debug
            }
        }
        ROM_CNT = ROM_CNT_READ_INFO; // cnt = 2
    }
    { // 第二步 模块功能使能
#if ROM_DEBUG
        printf("Module Init Start\n");
#endif
        if (EFUSE_DATA0 & BIT(16) && (!FLASH_FIX_INFO_PTR->PATCH_Disable) && (FLASH_DYNAMIC_INFO_PTR != NULL))
        {
            ROM_PATCH(); // debug
        }
        MODULE_INIT(&looplimit);
        ROM_CNT = ROM_CNT_MOD_FUNC_EN; // cnt = 3
    }
#endif
    return;
}