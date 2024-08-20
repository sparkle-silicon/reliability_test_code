/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-14 16:03:35
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
#include "AE_UPDATE.H"
/****************************************************************************************************************/
/***********************************************************************/
#define flash_write(addr, value) ((*((volatile uint32_t *)(addr))) = (value))
#define flash_read(addr) (*((volatile uint32_t *)(addr)))
/***********************************************************************/
/***********************************************************************/
#define sms_write(addr, value) ((*((volatile uint8_t *)(addr))) = (value))
#define sms_read(addr) (*((volatile uint8_t *)(addr)))
VBYTE check[32];
FUNCT_PTR_V_V Cache2Ram_ptr = NULL;
// 更新主函数
uint32_t update_fifo = 0;
uint8_t  read_fifo[256];
int update_putchar(int ch) /**/
{
    if(update_fifo < UARTA_BASE_ADDR || ((update_fifo & 0x3ff) != 0) || update_fifo > UART1_BASE_ADDR)return -1;//此时判断异常偏移
    char str = (char)ch;
    int looplimit = (CHIP_CLOCK_INT_HIGH / 15) / (SYSCTL_CLKDIV_OSC80M + 1) / 300;//3.3ms，3200波特率最多一个字节
    if(str == '\n')
    {
        for(volatile uint32_t counter = looplimit; (!(UPDATE_LSR & UART_LSR_TEMP)) && counter; counter--);
        UPDATE_TX = '\r';
    }
    for(volatile uint32_t counter = looplimit; (!(UPDATE_LSR & UART_LSR_TEMP)) && counter; counter--);
         /*当此位为空发送fifo写入数据*/
    UPDATE_TX = str;
    return ch;
}
void EC_UART_Update(uint32_t checkflag)//默认更新内部flash
{
    SELECT_FLASH(INTERNAL_FLASH, QUAD_FLASH, NOPIN_FLASH, NOPIN_FLASH, HIGHT_CHIP_CLOCK);
#if WRITE_READ_CHECK
    UART_ReUpdate :
#endif
    update_putchar(0xff);// 表示进入flash 更新
    update_putchar('0');//初始化环境
    BYTE buff[(update_size)];
    u16 state = 0;  // 状态位(0:正常传输,1:最后一个包)
    u16 cnt = 0;    // 计数位,最高为255位
    u16 rx_cnt = 0; // 接收的数据位
    int i = 0;
    /***********数据擦写********/
    while(!(UPDATE_LSR & UART_LSR_TEMP))
        ;
    UPDATE_TX = '1'; // 开始擦写
    SPIF_Erase_FLASH(SPIF_COMMAND_CHIP_ERASE1, NULL, UINT_MAX);
    UPDATE_TX = '2'; // 结束擦写
    while(1)
    {
        /*****************************数据接收*****************/
        VBYTEP buf_p = buff;
        // 请求传包
        while((UPDATE_LSR & UART_LSR_DR))
            UPDATE_RX;
        while(!(UPDATE_LSR & UART_LSR_TEMP))
            ;
        UPDATE_TX = 'A'; // 本轮开始
        // 等待数据接收
        rx_cnt = 0;
        while(rx_cnt < update_size)
        {
            while(!(UPDATE_LSR & UART_LSR_DR))
                ;
            buff[rx_cnt] = UPDATE_RX;
            // UPDATE_TX = buff[rx_cnt];//模块测试
            rx_cnt++;
        }
        while(!(UPDATE_LSR & UART_LSR_TEMP))
            ;
        UPDATE_TX = 'B';       // 结束传输块
        state = *(u16 *)buf_p; // 记录状态位
        buf_p += update_state_size;
        // UPDATE_TX = state;//模块测试
        u16 data_cnt = *(u16 *)buf_p;
        buf_p += update_cnt_size;
        // UPDATE_TX = data_cnt;//模块测试
        if(cnt != data_cnt)
        {
            // UPDATE_TX = 0xfe;//报错计数
            while(!(UPDATE_LSR & UART_LSR_TEMP))
                ;
            UPDATE_TX = 'E';
            UPDATE_TX = cnt; // 报错误计数
            while(UPDATE_RX != 0x06)
                ;
            continue; // 退出本次循环
        }
        VBYTEP check_p = buf_p;
        buf_p += update_check_size;
        /*********************************校验码计算比较**************/
        while(!(UPDATE_LSR & UART_LSR_TEMP))
            ;
        UPDATE_TX = 'C'; // 正在校验
        if(checkflag)
        {
            VBYTEP data_p = buf_p;
            while(!(UPDATE_LSR & UART_LSR_TEMP))
                ;
            UPDATE_TX = 'C'; // 进入校验
            for(i = 0; i < (update_code_size >> 7); i++)
            {
                register u32 sum;
                sum = CHECK_SUM(data_p, 128) & 0xff;
                check[i << 1] = (u8)(sum);
                check[(i << 1) + 1] = ~(u8)(sum);
                data_p += 128;
            }
            // VBYTEP check = check_p; // 计算
            for(i = 0; i < update_check_size; WDT_CRR = 0x76, i++)
                if(*(check_p + i) != *(check + i))
                {
                    // UPDATE_TX = 0xff;//报错重读
                    while(!(UPDATE_LSR & UART_LSR_TEMP))
                        ;
                    UPDATE_TX = 'F';
                    while(UPDATE_RX != 0x06)
                        WDT_CRR = 0x76;
                    cnt--;
                    continue; // 退出本次循环
                } // 比较
        }
        /**********************数据写入*************************/
        /*************数据写入******/
        while(!(UPDATE_LSR & UART_LSR_TEMP))
            ;
        UPDATE_TX = 'D'; // 正在写入
        uint32_t timeout = (CHIP_CLOCK_INT_HIGH / 15) / (SYSCTL_CLKDIV_OSC80M + 1);
        for(uint32_t j = 0; j < 4; j++)
        {
            SPIF_Wirte_FLASH((void *)((u32)cnt * (u32)update_code_size + j * (u32)256), (void *)(buf_p + j * 256), 256, timeout);
            SPIF_Read_FLASH((void *)((u32)cnt * (u32)update_code_size + j * (u32)256), read_fifo, 256, timeout);
            for(uint32_t i = 0; i < 256; i++)
            {
                if(read_fifo[i] != *(buf_p + j * 256 + i))
                {
                    //error
                }
            }
        }
        /*************数据校验处理******/
        /**********************数据处理*************************/
        for(i = 0; i < update_size; i++)
        {
            buff[i] = 0;
        }
        cnt++; // 下一个包的数字
        if(state)
            break; // 判断继续/结束轮询
    }
    /********发送重启指令****/
    while(!(UPDATE_LSR & UART_LSR_TEMP))
        ;
    UPDATE_TX = 'Z'; // 结束//等待重启
    SYSCTL_RESERVED |= BIT3;
    goto * 0x80084UL;
}
void  EC_SMS_Update(void)
{
    SET_BIT(SYSCTL_RST1, 8);
    __nop;
    __nop;
    __nop;
    CLEAR_BIT(SYSCTL_RST1, 8); // 复位SPIF
    *((volatile uint8_t *)(SRAM_BASE_ADDR + 0X100)) = 0x00;
    u16 i, j;
    uint8_t Update_Addr0, Update_Addr1, Update_Addr2 = 0;
    uint32_t Update_Addr = 0;
    u8 update_buf[256];
    while(*((volatile uint8_t *)(SRAM_BASE_ADDR + 0X100)) != 0X11)
        WDT_CRR = 0x76;
    while(!(SPIF_READY & 0x1))
        WDT_CRR = 0x76;
    SPIF_FIFO_TOP = 0xc7;
    while(!(SPIF_READY & 0x1))
        WDT_CRR = 0x76;
    while(SPIF_STATUS & 0xf)
        WDT_CRR = 0x76;
    while(!(SPIF_READY & 0x1))
        WDT_CRR = 0x76;
    *((volatile uint8_t *)(SRAM_BASE_ADDR + 0X100)) = 0x22;
    while(1)
    {
        while(*((volatile uint8_t *)(SRAM_BASE_ADDR + 0x100)) != 0xAA)
            ;
        for(j = 0; j < 256; j++)
        {
            update_buf[j] = *((volatile u8 *)(SRAM_BASE_ADDR + 0x200 + j)); // 防止其他设备访问sharemem改变值
        }
        *((volatile uint8_t *)(SRAM_BASE_ADDR + 0x100)) = 0xBB;
        while(*((volatile uint8_t *)(SRAM_BASE_ADDR + 0x100)) != 0xCC)
            ;
        Update_Addr0 = *((volatile uint8_t *)(SRAM_BASE_ADDR + 0x101));
        Update_Addr1 = *((volatile uint8_t *)(SRAM_BASE_ADDR + 0x102));
        Update_Addr2 = *((volatile uint8_t *)(SRAM_BASE_ADDR + 0x103));
        Update_Addr = (uint32_t)(Update_Addr0 << 24) + (uint32_t)(Update_Addr1 << 16) + (uint32_t)(Update_Addr2 << 8) + (uint32_t)(0x2);
        *((volatile uint8_t *)(SRAM_BASE_ADDR + 0x100)) = 0xDD;
        while(!(SPIF_READY & 0x1))
            WDT_CRR = 0x76;
        SPIF_DBYTE = 0xff;
        while(!(SPIF_READY & 0x1))
            WDT_CRR = 0x76;
        SPIF_FIFO_TOP = Update_Addr;
        for(i = 0; i < 64; i++)
        {
            while((SPIF_FIFO_CNT & 0x3) != 0)
                WDT_CRR = 0x76;
            SPIF_FIFO_TOP = *((volatile uint32_t *)(update_buf + i * 4));
        }
        while(!(SPIF_READY & 0x1))
            WDT_CRR = 0x76;
        while(SPIF_STATUS & 0xf)
            WDT_CRR = 0x76;
        while(!(SPIF_READY & 0x1))
            WDT_CRR = 0x76;
        // 该位置设置为写使能
        while(*((volatile uint8_t *)(SRAM_BASE_ADDR + 0x100)) != 0xDD)
            ;
        if(*((volatile uint8_t *)(SRAM_BASE_ADDR + 0x100)) == 0xFF)
            break;
    }
    /********发送重启指令****/
    *((volatile uint8_t *)(SRAM_BASE_ADDR + 0x100)) = 'Z';
    WDT_TORR = 0xffff; // 设置最长延时
    WDT_CR |= 0x02;    // 进入wdt中断
    WDT_CRR = 0x76;    // 重启计数器
    SYSCTL_RESERVED |= BIT3;
    goto * 0x80084UL;
}
void  EC_IO_Update(void)
{
    _R7, _R8 = 0;
    uint32_t data[64];
    uint8_t Smf_Addr0, Smf_Addr1, Smf_Addr2, Smf_Data0, Smf_Data1, Smf_Data2, Smf_Data3;
    uint32_t Page_Write_Addr = 0;
    char check_flag = 1;
    while(!(SPIF_READY & 0x1))
        WDT_CRR = 0x76;
    SPIF_FIFO_TOP = 0xc7;
    while(!(SPIF_READY & 0x1))
        WDT_CRR = 0x76;
    while(SPIF_STATUS & 0x1)
        WDT_CRR = 0x76;
    while(!(SPIF_READY & 0x1))
        WDT_CRR = 0x76;
    PMC2_DOR = 0x5A; // response to erase flash ok
    while(PMC2_STR & 0x1)
        ; // 等主机读走
    while(1)
    {
        if(IS_MASK_CLEAR(PMC2_STR, BIT(0x01)))
            continue;
        ECU_Cmd = PMC2_DIR;
        if(ECU_Cmd == 0xB5) // 设置地址
        {
            ECU_Cmd = 0x0;
            // get address
            Smf_Addr0 = SMF_FADDR0;
            Smf_Addr1 = SMF_FADDR1;
            Smf_Addr2 = SMF_FADDR2;
            Page_Write_Addr = (uint32_t)(Smf_Addr0 << 24) + (uint32_t)(Smf_Addr1 << 16) + (uint32_t)(Smf_Addr2 << 8) + (uint32_t)(0x2);
            PMC2_DOR = 0x5B;
            while(PMC2_STR & 0x1)
                ;            // 等主机读走
        }
        else if(ECU_Cmd == 0xC5) // 获取数据+写入数据+校验
        {
            if(_R8 >= 64)
                _R8 = 0;
            ECU_Cmd = 0x0;
            Smf_Data0 = SMF_DR0;
            Smf_Data1 = SMF_DR1;
            Smf_Data2 = SMF_DR2;
            Smf_Data3 = SMF_DR3;
            data[_R8] = (uint32_t)(Smf_Data3 << 24) + (uint32_t)(Smf_Data2 << 16) + (uint32_t)(Smf_Data1 << 8) + (uint32_t)Smf_Data0;
            _R8++;
            if(_R8 >= 64) // 收完了256个字节开始往flash里面写256字节数据
            {
                while(!(SPIF_READY & 0x1))
                    WDT_CRR = 0x76;
                SPIF_DBYTE = 0xff;
                while(!(SPIF_READY & 0x1))
                    WDT_CRR = 0x76;
                SPIF_FIFO_TOP = Page_Write_Addr;
                for(_R7 = 0; _R7 < 64; _R7++)
                {
                    while((SPIF_FIFO_CNT & 0x3) != 0)
                        WDT_CRR = 0x76;
                    SPIF_FIFO_TOP = data[_R7];
                }
                while(!(SPIF_READY & 0x1))
                    WDT_CRR = 0x76;
                while(SPIF_STATUS & 0x1)
                    WDT_CRR = 0x76;
                while(!(SPIF_READY & 0x1))
                    WDT_CRR = 0x76;
                /*校验*/
                SPIF_DBYTE = 0xff;
                while(!(SPIF_READY & 0x1))
                    ;
                SPIF_FIFO_TOP = (Page_Write_Addr - 0x2 + 0x3);
                for(_R7 = 0; _R7 < 64; _R7++)
                {
                    while((SPIF_FIFO_CNT & 0x3) == 0)
                        WDT_CRR = 0x76;
                    if(data[_R7] != SPIF_FIFO_TOP) // 校验失败
                    {
                        check_flag = 0;
                    }
                    if(_R7 == 63)
                    {
                        if(check_flag == 0) // 校验失败处理
                        {
                            PMC2_DOR = 0xEE;
                            while(PMC2_STR & 0x1)
                                ;            // 等主机读走
                        }
                        else
                        {
                            check_flag = 1;
                        }
                    }
                }
                while(!(SPIF_READY & 0x1))
                    ;
                while(SPIF_STATUS & 0x1)
                    ;
                while(!(SPIF_READY & 0x1))
                    ;
                /*校验*/
            }
            if((check_flag == 0) && (_R8 >= 64)) // 校验失败处理
            {
                while(!(SPIF_READY & 0x1))
                    ;
                SPIF_FIFO_TOP = (Page_Write_Addr - 0x2 + 0x20);
                while(!(SPIF_READY & 0x1))
                    ;
                while(SPIF_STATUS & 0x1)
                    ;
            }
            else
            {
                PMC2_DOR = _R8;
                while(PMC2_STR & 0x1)
                    ; // 等主机读走
            }
            check_flag = 1;
        }
        else if(ECU_Cmd == 0xD5) // update_debug
        {
            // update_debug
            WDT_TORR = 0xffff; // 设置最长延时
            WDT_CR |= 0x02;    // 进入wdt中断
            WDT_CRR = 0x76;    // 重启计数器
            SYSCTL_RESERVED |= (1 << 3);
            goto * 0x80084UL;
            // update_debug
        }
    }
}
