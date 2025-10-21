/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-21 16:38:41
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
#include "AE_INCLUDE.H"
#include "KERNEL_UART.H"
int serial_base(BYTE uart_num)
{
    uart_num &= 0b111;//保护机制
    switch(uart_num)
    {
        case UART0_CHANNEL:
            g_uart_base = UART0_BASE_ADDR;
            break;
        case UART1_CHANNEL:
            g_uart_base = UART1_BASE_ADDR;
            break;
        case UARTA_CHANNEL:
            g_uart_base = UARTA_BASE_ADDR;
            break;
        case UARTB_CHANNEL:
            g_uart_base = UARTB_BASE_ADDR;
            break;
        default:
            g_uart_base = 0;
            break;
    }
    return g_uart_base;
}
int serial_config(BYTE uart_num, DWORD baudrate)
{
    uart_num &= 0b111;//保护机制
    VBYTEP uart_lcr = (VBYTEP)REG_ADDR(g_uart_base, UART_LCR_OFFSET);
    VBYTEP uart_divisor_lsb = (VBYTEP)REG_ADDR(g_uart_base, UART_DLL_OFFSET);
    VBYTEP uart_divisor_msb = (VBYTEP)REG_ADDR(g_uart_base, UART_DLH_OFFSET);
    VBYTEP uart_fcr = (VBYTEP)REG_ADDR(g_uart_base, UART_FCR_OFFSET);
    VBYTEP uart_ier = (VBYTEP)REG_ADDR(g_uart_base, UART_IER_OFFSET);
    register DWORD divisor;
    register DWORD freq;
    freq = UART_CLOCK;
    divisor = freq / baudrate + (0b100 << (uart_num >> 1)); // 27
    // Set DLAB to 1 in LCR
    *uart_lcr = UART_LCR_DLAB | UART_LCR_DLS_8bit;
    divisor >>= (3 + (uart_num >> 1));
    *uart_divisor_lsb = divisor;
    *uart_divisor_msb = (divisor >> 8);
    // Set Line Control Register
    *uart_fcr = UART_FCR_TFTL_8BYTE | UART_FCR_FIFOEN | UART_FCR_TFR | UART_FCR_RFR;
    *uart_lcr &= ~UART_LCR_DLAB;
    *uart_ier |= UART_IER_RDAIE | UART_IER_PTIME | UART_IER_RLSIE; // recived data int enable
    baudrate = freq / (divisor << (3 + (uart_num >> 2)));
    return  baudrate;
}
int serial_init(DWORD uart_num, DWORD baudrate)
{
    if(serial_base(uart_num) == 0)return -1;
    return serial_config(uart_num, baudrate);
}
int serial_deinit(DWORD uart_num)
{
    uart_num &= 0b111;//保护机制
    u32 reset_val;
    serial_base(uart_num);
    VBYTEP uart_lcr = (VBYTEP)REG_ADDR(g_uart_base, UART_LCR_OFFSET);
    VBYTEP uart_ier = (VBYTEP)REG_ADDR(g_uart_base, UART_IER_OFFSET);
    VBYTEP uart_lsr = (VBYTEP)REG_ADDR(g_uart_base, UART_LSR_OFFSET);
    *uart_lcr &= ~UART_LCR_DLAB;
    *uart_ier = 0;//先屏蔽中断
    while(!(*uart_lsr & UART_LSR_TEMP))//等待最后发送完成
        ;
    //reset uart 
    switch(uart_num)
    {
        case UART0_CHANNEL:
            reset_val = UART0_EN;
            break;
        case UART1_CHANNEL:
            reset_val = UART1_EN;
            break;
        case UARTA_CHANNEL:
            reset_val = UARTA_EN;
            break;
        case UARTB_CHANNEL:
            reset_val = UARTB_EN;
            break;
        default:
            reset_val = UARTA_EN;
            break;
    }
    SYSCTL_RST0 |= reset_val;
    SYSCTL_RST0 &= ~reset_val;
    return 0;
}
int serial2gpio(DWORD uart_num)
{
    uart_num &= 0b111;//保护机制
    u32 val = 0;
    u8 ier;
    serial_base(uart_num);
    VBYTEP uart_ier = (VBYTEP)REG_ADDR(g_uart_base, UART_IER_OFFSET);
    VBYTEP uart_lsr = (VBYTEP)REG_ADDR(g_uart_base, UART_LSR_OFFSET);
    ier = *uart_ier;
    *uart_ier = 0;//先屏蔽中断
    while(!(*uart_lsr & UART_LSR_TEMP))//等待最后发送完成
        ;
    // set pin to gpio
    switch(uart_num)
    {
        case UART0_CHANNEL:
            sysctl_iomux_disable_uart0();
            val |= UART0_EN;
            break;
        case UART1_CHANNEL:
            sysctl_iomux_disable_uart1(UART1_TX_SEL, UART1_RX_SEL);
            val |= UART1_EN;
            break;
        case UARTB_CHANNEL:
            sysctl_iomux_disable_uartb();
            val |= UARTB_EN;
            break;
        case UARTA_CHANNEL:
        default:
            sysctl_iomux_disable_uarta(UARTA_TX_SEL, UARTA_RX_SEL);
            val |= UARTA_EN;
            break;
    }
    *uart_ier = ier;
    SYSCTL_MODEN0 &= ~val;//disable uart
    return 0;
}
int gpio2serial(DWORD uart_num)
{
    uart_num &= 0b111;//保护机制
    u32 val = 0;
    serial_base(uart_num);
    VBYTEP uart_lcr = (VBYTEP)REG_ADDR(g_uart_base, UART_LCR_OFFSET);
    *uart_lcr &= ~UART_LCR_DLAB;
    //set pin 2 uart
    switch(uart_num)
    {
        case UART0_CHANNEL:
            sysctl_iomux_uart0();
            val |= UART0_EN;
            break;
        case UART1_CHANNEL:
            sysctl_iomux_uart1(UART1_TX_SEL, UART1_RX_SEL);
            val |= UART1_EN;
            break;
        case UARTB_CHANNEL:
            sysctl_iomux_uartb();
            val |= UARTB_EN;
            break;
        case UARTA_CHANNEL:
        default:
            sysctl_iomux_uarta(UARTA_TX_SEL, UARTA_RX_SEL);
            val |= UARTA_EN;
            break;
    }
    SYSCTL_MODEN0 |= val;//enable uart
    return 0;
}
BYTE Uart_Int_Enable(BYTE channel, BYTE type)
{
    switch(channel)
    {
        case UART0_CHANNEL:
            UART0_IER |= (0x1 << type);
            break;
        case UART1_CHANNEL:
            UART1_IER |= (0x1 << type);
            break;
        case UARTA_CHANNEL:
            UARTA_IER |= (0x1 << type);
            break;
        case UARTB_CHANNEL:
            UARTB_IER |= (0x1 << type);
            break;
        default:
            assert_print();
            return -1;
    }
    return 0;
}
BYTE Uart_Int_Disable(BYTE channel, BYTE type)
{
    switch(channel)
    {
        case UART0_CHANNEL:
            UART0_IER &= ~(0x1 << type);
            break;
        case UART1_CHANNEL:
            UART1_IER &= ~(0x1 << type);
            break;
        case UARTA_CHANNEL:
            UARTA_IER &= ~(0x1 << type);
            break;
        case UARTB_CHANNEL:
            UARTB_IER &= ~(0x1 << type);
            break;
        default:
            assert_print();
            return -1;
    }
    return 0;
}
BYTE Uart_Int_Enable_Read(BYTE channel, BYTE type)
{
    switch(channel)
    {
        case UART0_CHANNEL:
            return ((UART0_IER & (0x1 << type)) != 0);
        case UART1_CHANNEL:
            return ((UART1_IER & (0x1 << type)) != 0);
        case UARTA_CHANNEL:
            return ((UARTA_IER & (0x1 << type)) != 0);
        case UARTB_CHANNEL:
            return ((UARTB_IER & (0x1 << type)) != 0);
        default:
            return -1;
    }
}
BYTE Uart_Int_Status(BYTE channel, BYTE type)
{
    switch(channel)
    {
        case UART0_CHANNEL:
            return ((UART0_IIR & (0x1 << type)) != 0);
        case UART1_CHANNEL:
            return ((UART1_IIR & (0x1 << type)) != 0);
        case UARTA_CHANNEL:
            return ((UARTA_IIR & (0x1 << type)) != 0);
        case UARTB_CHANNEL:
            return ((UARTB_IIR & (0x1 << type)) != 0);
        default:
            return -1;
    }
}
#if SUPPORT_REAL_OR_DELAY_PRINTF
//----------------------------------------------------------------------------
// FUNCTION: Service_PUTC
// UART putchar support function
//----------------------------------------------------------------------------
void Service_PUTC(void)
{
    if(F_Service_PUTC && (!print_number))
        return;
      // if(print_number&&(PRINTF_LSR & UART_LSR_TEMP) )
    while(print_number && (!(PRINTF_LSR & UART_LSR_THRE)))
    {
        PRINTF_TX = print_buff[PRINT_SERVICE_CNT];
        // print_buff[PRINT_SERVICE_CNT]='\0';
        PRINT_SERVICE_CNT++;
        print_number--;
        if(PRINT_SERVICE_CNT >= PRINT_MAX_SIZE)
            PRINT_SERVICE_CNT = 0;
    }
}
#endif
/**
 * @brief 字符输出
 *
 * @param    ch       输出字符
 *
 * @return   输出字符
 */
WEAK int putchar(int ch) /**/
{
#if PRINTF_UART_SWITCH < 6
    char str = (char)ch;
#if SUPPORT_REAL_OR_DELAY_PRINTF
    if(F_Service_PUTC)
    {
        if(str == '\n')
        {
            while(!(PRINTF_LSR & UART_LSR_TEMP))
                ;
            PRINTF_TX = '\r';
        }
        while(!(PRINTF_LSR & UART_LSR_TEMP))
            ; /*当此位为空发送fifo写入数据*/
        PRINTF_TX = str;
    }
    else
    {
        if(str == '\n')
        {
            // while (print_buff[print_cnt]!='\0')WDT_REG(0xC) =0x76;
            print_buff[print_cnt] = '\r';
            print_cnt++;
            print_number++;
            if(print_cnt >= PRINT_MAX_SIZE)
                print_cnt = 0;
        }
        // while (print_buff[print_cnt]!='\0')WDT_REG(0xC) =0x76;
        print_buff[print_cnt] = str;
        print_cnt++;
        print_number++;
        if(print_cnt >= PRINT_MAX_SIZE)
            print_cnt = 0;
    }
#else
    if(str == '\n')
    {
        while(!(PRINTF_LSR & UART_LSR_TEMP))
            ;
        PRINTF_TX = '\r';
        }
    while(!(PRINTF_LSR & UART_LSR_TEMP))
        ; /*当此位为空发送fifo写入数据*/
    PRINTF_TX = str;
#endif
    return ch;
#endif
    }
void DEBUGER_putchar(char ch)
{
#if ENABLE_DEBUGGER_SUPPORT
#if (DEBUGGER_OUTPUT_SWITCH == 0)
#if DEBUG_UART_SWITCH < 6
    while(!(REG8(DEBUGGER_UART + UART_LSR_OFFSET) & 0x60));
    /*FIFO check*/
    REG8(DEBUGGER_UART + UART_THR_OFFSET) = ch; // send by uart
#endif
#elif (DEBUGGER_OUTPUT_SWITCH == 1)
#elif (DEBUGGER_OUTPUT_SWITCH == 2)
    while(PMC3_STR & 0x1);
    PMC3_DOR = ch;
#endif
#endif
}
#if(defined(USER_RISCV_LIBC_A))
USED ssize_t _write(int fd, const void *ptr, size_t len)
{
    const BYTEP str = (const BYTEP)ptr;
    if(isatty(fd))
    {
        for(size_t j = 0; j < len; j++)
        {
            if(*str == '\n')
            {
                while(!(PRINTF_LSR & UART_LSR_TEMP))
                    ;
                PRINTF_TX = '\r';
            }
            while(!(PRINTF_LSR & UART_LSR_TEMP))
                ; /*当此位为空发送fifo写入数据*/
            PRINTF_TX = *str;
            str++;
        }
        return len;
    }
    return -1;
}
//.gloable
USED int _read(int fd, void *ptr, size_t len)
{
    BYTEP str = (BYTEP)ptr;
    if(isatty(fd))
    {
        for(size_t j = 0; j < len; j++)
        {
            while(!(PRINTF_LSR & UART_LSR_DR))
                ; /*当此位为空发送fifo写入数据*/
            *str = PRINTF_RX;
        }
        return len;
    }
    return -1;
}
#endif