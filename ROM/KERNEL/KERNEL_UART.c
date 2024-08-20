/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-19 18:50:33
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
volatile uint32_t debugger_flag = 0;
volatile uint32_t cmd_flag = 0;
uint32_t UART_Init(uint32_t chanel, uint32_t bauld, uint32_t lcr)
{
    chanel &= (0b11U);
    switch(chanel)
    {
        case UART0_CHANNEL:
            SYSCTL_PIO1_CFG &= ~(0b1111U << 16); /* tx[19:18] rx[17:16] */
            SYSCTL_PIO1_CFG |= (0b1010U << 16);	/* [19:18] [17:16] function 2*/
            break;
        case UART1_CHANNEL:
            if((SYSCTL_PIO5_CFG & (0b1111U << 28)) != (0b1111U << 28)) // debugger未置位
            {
                SYSCTL_PIO2_CFG &= ~((0b11U << 6) | (0b11U << 2)); /* [7:6] [3:2] */
                SYSCTL_PIO2_CFG |= (0b01U << 6) | (0b01U << 2);	 /*[7:6] [3:2] function 1*/
            }
            break;
        case UARTA_CHANNEL:
            SYSCTL_PIO0_CFG &= ~(0b1111U << 16); /* [19:18] [17:16] */
            SYSCTL_PIO0_CFG |= (0b1010U << 16);	/* [19:18] [17:16] function 2*/
            break;
        case UARTB_CHANNEL:
            UARTB_SEL;
            SYSCTL_PIO3_CFG &= ~(0b1111U << 18); /* [21:20] [19:18] */
            SYSCTL_PIO3_CFG |= (0b1010U << 18);	/* [21:20] [19:18] function 2*/
            break;
        default:
            break;
    };
    volatile u_int32_t uart_ptr = UARTA_BASE_ADDR + (0x400 * (chanel ^ 0b10));
    // Set DLAB to 1 in LCR
    REG8(uart_ptr + UART_LCR_OFFSET) = UART_LCR_DLAB | UART_LCR_DLS_8bit;

    register DWORD freq = CHIP_CLOCK_INT_HIGH / (SYSCTL_CLKDIV_OSC80M + 1) / (SYSCTL_CLKDIV_UART + 1);
    register DWORD divisor = (freq / bauld + (0b100 << (chanel >> 1))) >> (3 + (chanel >> 1));
    REG8(uart_ptr + UART_DLL_OFFSET) = divisor;
    REG8(uart_ptr + UART_DLH_OFFSET) = (divisor >> 8);
    bauld = freq / (divisor << (3 + (chanel >> 1)));
    // Set Line Control Register
    REG8(uart_ptr + UART_FCR_OFFSET) = UART_FCR_TFTL_8BYTE | UART_FCR_FIFOEN | UART_FCR_TFR | UART_FCR_RFR;
    REG8(uart_ptr + UART_LCR_OFFSET) &= ~UART_LCR_DLAB;
    REG8(uart_ptr + UART_LCR_OFFSET) |= (lcr & 0x1f);
    REG8(uart_ptr + UART_IER_OFFSET) = 0; // 不接收中断
    return bauld;
}

static unsigned char shell_fifo[128] = { '\0' };
static unsigned char shell_cnt = 0;
static unsigned char debugger_fifo[128] = { '\0' };
static unsigned char debugger_cnt = 0;
static unsigned char debugger_comcmnt = 1;
/*return :0 本次没有执行内容，1：本次执行了代码 */
int32_t GET_DEBUGGER_CMD(void)
{

    if(DEBUGGER_FIFO != (uint32_t)NULL && (DEBUGGER_LSR & UART_LSR_DR))
    {
        debugger_fifo[debugger_cnt] = DEBUGER_getchar();
    #if ROM_DEBUG
        printf("debugger_fifo[%#02x]=%#02x(%c)\n", debugger_cnt, debugger_fifo[debugger_cnt], debugger_fifo[debugger_cnt]);
    #endif
        if(debugger_cnt == 0)
        {
            switch(debugger_fifo[debugger_cnt])
            {
                case DEBUGGER_CMD_READ_REQUEST://读
                    debugger_comcmnt = 4;
                    break;
                case DEBUGGER_CMD_WRITE_REQUEST://写
                    debugger_comcmnt = 7;
                    break;
                case DEBUGGER_CMD_RESET_CHIP://复位
                case DISCONNECTION_REQUEST://关闭握手，如果在rom会退出调试器功能继续执行流程
                    debugger_comcmnt = 0;
                    break;
                case DEBUGGER_CMD_HANDSHAKE:
                    debugger_comcmnt = 3;
                    break;
                case DEBUGGER_CMD_UPDATE:
                default:
                    debugger_comcmnt = 0xff;
                    break;
            }

        }
        if(debugger_cnt >= debugger_comcmnt)
        {
            debugger_cnt = 0;
            debugger_comcmnt = 1;
        #if ROM_DEBUG
            printf("goto debugger\n");
        #endif
            return Deubgger_Cmd_Parsing(debugger_fifo);
        }
        else if(debugger_comcmnt == 0xff)
        {
            if(debugger_fifo[debugger_cnt] == '\r')debugger_fifo[debugger_cnt] = '\0';
            if(debugger_fifo[debugger_cnt] == '\n' || debugger_cnt >= 127)
            {
                debugger_fifo[debugger_cnt--] = '\0';
                if(debugger_fifo[debugger_cnt] == ' ')debugger_fifo[debugger_cnt--] = '\0';
                debugger_cnt = 0;
                debugger_comcmnt = 1;
            #if ROM_DEBUG
                printf("goto debugger cmd\n");
            #endif
                return Deubgger_Cmd_Parsing(debugger_fifo);
            }
        }
        debugger_cnt++;
    }
    else if(1 && PRINTF_FIFO != (uint32_t)NULL && (PRINTF_LSR & UART_LSR_DR))//命令行作为辅助功能不受影响（同时也防止除了问题）
    {

        shell_fifo[shell_cnt] = (u_char)getchar();
    #if ROM_DEBUG
        printf("debugger_fifo[%#02x]=%#02x(%c)\n", shell_cnt, shell_fifo[shell_cnt], shell_fifo[shell_cnt]);
    #endif
        if(shell_fifo[shell_cnt] == '\r')shell_fifo[shell_cnt] = '\0';
        if(shell_fifo[shell_cnt] == '\n' || shell_cnt >= 127)
        {
            cmd_flag = 1;//不需要握手机制，直接强行绑定,但要注意循环次数给足了，不然还没抓完退出了（相当于一个命令类似握手机制）
            shell_fifo[shell_cnt--] = '\0';
            if(shell_fifo[shell_cnt] == ' ')shell_fifo[shell_cnt--] = '\0';
        #if ROM_DEBUG
            printf("goto printf cmd\n");
        #endif
            CMD_RUN(shell_fifo);
            shell_cnt = 0;
            return 1;
        }
        else { shell_cnt++; }
    }
    return 0;
}
int getchar(void)
{
    if(PRINTF_FIFO < UARTA_BASE_ADDR || ((PRINTF_FIFO & 0x3ff) != 0) || PRINTF_FIFO > UART1_BASE_ADDR)return -1;//此时判断异常偏移
    while(!(PRINTF_LSR & UART_LSR_DR))//强制卡死的获取机制
        ;
    return  (int)((u_int32_t)((u_int8_t)PRINTF_RX));
}
u_char DEBUGER_getchar(void)
{
    if(DEBUGGER_FIFO < UARTA_BASE_ADDR || ((DEBUGGER_FIFO & 0x3ff) != 0) || DEBUGGER_FIFO > UART1_BASE_ADDR)return 0xFE;//此时判断异常偏移
    while(!(DEBUGGER_LSR & UART_LSR_DR))//强制卡死的获取机制
        ;
    return  (u_char)DEBUGGER_RX;
}
/**
 * @brief 字符输出
 *
 * @param    ch       输出字符
 *
 * @return   输出字符
 */
int putchar(int ch) /**/
{
    if(PRINTF_FIFO < UARTA_BASE_ADDR || ((PRINTF_FIFO & 0x3ff) != 0) || PRINTF_FIFO > UART1_BASE_ADDR)return -1;//此时判断异常偏移
    char str = (char)ch;
    int looplimit = (CHIP_CLOCK_INT_HIGH / 15) / (SYSCTL_CLKDIV_OSC80M + 1) / 300;//3.3ms，3200波特率最多一个字节
    if(str == '\n')
    {
        for(volatile uint32_t counter = looplimit; (!(PRINTF_LSR & UART_LSR_TEMP)) && counter; counter--);
        PRINTF_TX = '\r';
    }
    for(volatile uint32_t counter = looplimit; (!(PRINTF_LSR & UART_LSR_TEMP)) && counter; counter--);
         /*当此位为空发送fifo写入数据*/
    PRINTF_TX = str;
    return ch;
}
void DEBUGER_putchar(char ch)
{
#if ROM_DEBUG&&0
    printf("%#02hhx ", ((u_char)ch));//转成十六进制输出
#else

    if(DEBUGGER_FIFO < UARTA_BASE_ADDR || ((DEBUGGER_FIFO & 0x3ff) != 0) || DEBUGGER_FIFO > UART1_BASE_ADDR)return;//此时判断异常偏移
    int looplimit = (CHIP_CLOCK_INT_HIGH / 15) / (SYSCTL_CLKDIV_OSC80M + 1) / 300;//3ms，3200波特率最多一个字节
    for(volatile uint32_t counter = looplimit; (!(DEBUGGER_LSR & UART_LSR_TEMP)) && counter; counter--);
        /*当此位为空发送fifo写入数据*/
    DEBUGGER_TX = ch;
    // printf("%#x\t", ch);
#endif
}