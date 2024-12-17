/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-12-17 15:40:53
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
#include "KERNEL_MAILBOX.H"
/****************************************************************************************************************/
/***********************************************************************/
#define flash_write(addr, value) ((*((volatile uint32_t *)(addr))) = (value))
#define flash_read(addr) (*((volatile uint32_t *)(addr)))
/***********************************************************************/
/***********************************************************************/
#define sms_write(addr, value) ((*((volatile uint8_t *)(addr))) = (value))
#define sms_read(addr) (*((volatile uint8_t *)(addr)))
char uart_crtpram_updatebuffer[12];
int uart_crypram_updateindex = 0;
char update_crypram_cmd[12] = { 0x64 ,0x72 ,0x61 ,0x6D ,0x30 ,0x2D ,0x75 ,0x70 ,0x64 ,0x61 ,0x74 ,0x65 };
char update_crypram_flag = 0;
VBYTE check[32];
FUNCT_PTR_V_V Cache2Ram_ptr = NULL;
// 更新主函数
void ALIGNED(4) OPTIMIZE0 SECTION(".update.uart") EC_UART_Update(void)
{
#if WRITE_READ_CHECK
    UART_ReUpdate :
#endif
    UPDATE_TX = 0xff; // 表示进入ram空间
    while(!(UPDATE_LSR & UART_LSR_TEMP));
    UPDATE_TX = '0'; // 进入传输
    BYTE buff[(update_size)];
    u16 state = 0;  // 状态位(0:正常传输,1:最后一个包)
    u16 cnt = 0;    // 计数位,最高为255位
    u16 rx_cnt = 0; // 接收的数据位
    u32 addrs = 0;
    int i = 0;
    /***********数据擦写********/
    while(!(UPDATE_LSR & UART_LSR_TEMP));
    UPDATE_TX = '1'; // 开始擦写
    while(!(SPIF_READY & 0x1));
    SPIF_FIFO_TOP = 0xc7;
    while(!(SPIF_READY & 0x1));
    while(SPIF_STATUS & 0xf);
    while(!(SPIF_READY & 0x1));
    while(!(UPDATE_LSR & UART_LSR_TEMP));
    UPDATE_TX = '2';    // 结束擦写
    //PRINTF_TX='a';
    while(1)
    {
        /*****************************数据接收*****************/
        VBYTEP buf_p = buff;
        // 请求传包
        while((UPDATE_LSR & UART_LSR_DR))
            UPDATE_RX;
        while(!(UPDATE_LSR & UART_LSR_TEMP));
        UPDATE_TX = 'A'; // 本轮开始
        //PRINTF_TX='a';
        // 等待数据接收
        rx_cnt = 0;
        while(rx_cnt < update_size)
        {
            while(!(UPDATE_LSR & UART_LSR_DR));
            buff[rx_cnt] = UPDATE_RX;
            // UPDATE_TX = buff[rx_cnt];//模块测试
            rx_cnt++;
        }
        while(!(UPDATE_LSR & UART_LSR_TEMP));
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
            while(!(UPDATE_LSR & UART_LSR_TEMP));
            UPDATE_TX = 'E';
            UPDATE_TX = cnt; // 报错误计数
            while(UPDATE_RX != 0x06);
            continue;           // 退出本次循环
        }
        VBYTEP check_p = buf_p;
        buf_p += update_check_size;
        /*********************************校验码计算比较**************/
        while(!(UPDATE_LSR & UART_LSR_TEMP));
        UPDATE_TX = 'C'; // 正在校验
        if(update_flag & 0x01)
        {
            VBYTEP data_p = buf_p;
            while(!(UPDATE_LSR & UART_LSR_TEMP));
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
                    while(!(UPDATE_LSR & UART_LSR_TEMP));
                    UPDATE_TX = 'F';
                    while(UPDATE_RX != 0x06)
                        WDT_CRR = 0x76;
                    cnt--;
                    continue; // 退出本次循环
                }             // 比较
        }
        /**********************数据写入*************************/
        /*************数据写入******/
        while(!(UPDATE_LSR & UART_LSR_TEMP));
        UPDATE_TX = 'D'; // 正在写入
        for(char j = 0; j < 4; j++)
        {
            u32 addr = (u32)cnt * (u32)update_code_size + j * (u32)256;
            addrs = ((addr & 0xFF) << 24) + ((addr & 0xFF00) << 8) + ((addr & 0XFF0000) >> 8) + 0x02; // 设置地址
            while(!(SPIF_READY & 0x1));
            SPIF_DBYTE = 0xff;
            while(!(SPIF_READY & 0x1));
            SPIF_FIFO_TOP = addrs; // 数据地址
            for(i = 0; i < 64; i++)
            {
                while((SPIF_FIFO_CNT & 0x3) != 0);
                SPIF_FIFO_TOP = *(u32 *)(buf_p + j * 256 + i * 4);
            }
            while(!(SPIF_READY & 0x1));
            while(SPIF_STATUS & 0xf);
            while(!(SPIF_READY & 0x1));
        #if WRITE_READ_CHECK
            UPDATE_TX = 'b';    // 正在读flash数据
            while(!(UPDATE_LSR & UART_LSR_DR))
                WDT_CRR = 0x76;
                // 读数据
            addrs = addrs & 0xffffff00 + 0x03; // 修改内容
            while(!(SPIF_READY & 0x1))
                WDT_CRR = 0x76; // 读忙
            SPIF_DBYTE = 0xff;
            while(!(SPIF_READY & 0x1))
                WDT_CRR = 0x76;    // 读忙
            SPIF_FIFO_TOP = addrs; // 数据地址
            register u32 checkdata, checksum1 = 0, checksum2 = 0;
            for(int i = 0; i < 64; i++)
            {
                while((SPIF_FIFO_CNT & 0x3) == 0)
                {
                    WDT_CRR = 0x76; // 读忙
                }
                checkdata = SPIF_FIFO_TOP;
                // while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                if(i < 32)
                    checksum1 += ((checkdata >> 0) & 0xff) + ((checkdata >> 8) & 0xff) + ((checkdata >> 16) & 0xff) + ((checkdata >> 24) & 0xff);
                else
                    checksum2 += ((checkdata >> 0) & 0xff) + ((checkdata >> 8) & 0xff) + ((checkdata >> 16) & 0xff) + ((checkdata >> 24) & 0xff);
                WDT_CRR = 0x76;
            }
            while(!(SPIF_READY & 0x1))
                WDT_CRR = 0x76; // 读忙
            while(SPIF_STATUS & 0xf)
                WDT_CRR = 0x76; // 直到写完
            while(!(SPIF_READY & 0x1))
                WDT_CRR = 0x76;
            UPDATE_TX = 'c'; // 正在校验flash数据
            // 判断是否错误，如果错误，一切从头开始
            checksum1 &= 0xFF;
            checksum2 &= 0xFF;
            if((check[j << 2] != (VBYTE)checksum1) || (check[(j << 2) + 2] != (VBYTE)checksum2))
            {
                UPDATE_TX = 'F';
                while(UPDATE_RX != 0x06)
                    WDT_CRR = 0x76;
                goto UART_ReUpdate; // 退出本次循环
            }
        #endif
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
    while(!(UPDATE_LSR & UART_LSR_TEMP));
    UPDATE_TX = 'Z'; // 结束//等待重启
    SYSCTL_RESERVED |= BIT3;
    goto * 0x80084UL;
}
void ALIGNED(4) OPTIMIZE0 SECTION(".update.sms")  EC_SMS_Update(void)
{
    SET_BIT(SYSCTL_RST1, 8);
    __nop;
    __nop;
    __nop;
    CLEAR_BIT(SYSCTL_RST1, 8); // 复位SPIF
    while(!(PRINTF_LSR & UART_LSR_TEMP));
    PRINTF_TX = '0';
    *((volatile uint8_t *)(SRAM_BASE_ADDR + 0X100)) = 0x00;
    u16 i, j;
    uint8_t Update_Addr0, Update_Addr1, Update_Addr2 = 0;
    uint32_t Update_Addr = 0;
    u8 update_buf[256];
    PRINTF_TX = '1';
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
    PRINTF_TX = '2';
    while(1)
    {
        PRINTF_TX = 'A';
        while(*((volatile uint8_t *)(SRAM_BASE_ADDR + 0x100)) != 0xAA);
        for(j = 0; j < 256; j++)
        {
            update_buf[j] = *((volatile u8 *)(SRAM_BASE_ADDR + 0x200 + j)); // 防止其他设备访问sharemem改变值
        }
        PRINTF_TX = 'B';
        *((volatile uint8_t *)(SRAM_BASE_ADDR + 0x100)) = 0xBB;
        PRINTF_TX = 'C';
        while(*((volatile uint8_t *)(SRAM_BASE_ADDR + 0x100)) != 0xCC);
        Update_Addr0 = *((volatile uint8_t *)(SRAM_BASE_ADDR + 0x101));
        Update_Addr1 = *((volatile uint8_t *)(SRAM_BASE_ADDR + 0x102));
        Update_Addr2 = *((volatile uint8_t *)(SRAM_BASE_ADDR + 0x103));
        Update_Addr = (uint32_t)(Update_Addr0 << 24) + (uint32_t)(Update_Addr1 << 16) + (uint32_t)(Update_Addr2 << 8) + (uint32_t)(0x2);
        PRINTF_TX = 'D';
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
        while(*((volatile uint8_t *)(SRAM_BASE_ADDR + 0x100)) != 0xDD);
        if(*((volatile uint8_t *)(SRAM_BASE_ADDR + 0x100)) == 0xFF)
            break;
    }
    /********发送重启指令****/
    while(!(PRINTF_LSR & UART_LSR_TEMP))
        WDT_CRR = 0x76; /*当此位为空发送fifo写入数据*/
    PRINTF_TX = 'Z';
    *((volatile uint8_t *)(SRAM_BASE_ADDR + 0x100)) = 'Z';
    WDT_TORR = 0xffff; // 设置最长延时
    WDT_CR |= 0x02;    // 进入wdt中断
    WDT_CRR = 0x76;    // 重启计数器
    SYSCTL_RESERVED |= BIT3;
    goto * 0x80084UL;
}
void ALIGNED(4) OPTIMIZE0 SECTION(".update.io")   EC_IO_Update(void)
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
    PRINTF_TX = 'a';//擦除完成打印
    PMC2_DOR = 0x5A; // response to erase flash ok
    while(PMC2_STR & 0x1);//等主机读走
    while(1)
    {
        if(IS_MASK_CLEAR(PMC2_STR, IBF2))
            continue;
        ECU_Cmd = PMC2_DIR;
        if(ECU_Cmd == 0xB5)//设置地址
        {
            ECU_Cmd = 0x0;
            // get address
            Smf_Addr0 = SMF_FADDR0;
            Smf_Addr1 = SMF_FADDR1;
            Smf_Addr2 = SMF_FADDR2;
            Page_Write_Addr = (uint32_t)(Smf_Addr0 << 24) + (uint32_t)(Smf_Addr1 << 16) + (uint32_t)(Smf_Addr2 << 8) + (uint32_t)(0x2);
            PMC2_DOR = 0x5B;
            while(PMC2_STR & 0x1);//等主机读走
            PRINTF_TX = 'b';//成功接收地址打印
        }
        else if(ECU_Cmd == 0xC5)//获取数据+写入数据+校验
        {
            if(_R8 >= 64)
                _R8 = 0;
            ECU_Cmd = 0x0;
            Smf_Data0 = SMF_DR0;
            Smf_Data1 = SMF_DR1;
            Smf_Data2 = SMF_DR2;
            Smf_Data3 = SMF_DR3;
            data[_R8] = (uint32_t)(Smf_Data3 << 24) + (uint32_t)(Smf_Data2 << 16) + (uint32_t)(Smf_Data1 << 8) + (uint32_t)Smf_Data0;
            PRINTF_TX = 'c';//成功获取数据打印
            _R8++;
            if(_R8 >= 64)//收完了256个字节开始往flash里面写256字节数据
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
                PRINTF_TX = 'w';//写入完成打印
                /*校验*/
                SPIF_DBYTE = 0xff;
                while(!(SPIF_READY & 0x1));
                SPIF_FIFO_TOP = (Page_Write_Addr - 0x2 + 0x3);
                for(_R7 = 0; _R7 < 64; _R7++)
                {
                    while((SPIF_FIFO_CNT & 0x3) == 0)
                        WDT_CRR = 0x76;
                    if(data[_R7] != SPIF_FIFO_TOP)//校验失败
                    {
                        check_flag = 0;
                    }
                    if(_R7 == 63)
                    {
                        if(check_flag == 0)//校验失败处理
                        {
                            PMC2_DOR = 0xEE;
                            while(PMC2_STR & 0x1);//等主机读走
                            PRINTF_TX = 'e';//校验出错打印
                        }
                        else
                        {
                            check_flag = 1;
                            PRINTF_TX = 'n';//校验无问题打印
                        }
                    }
                }
                while(!(SPIF_READY & 0x1));
                while(SPIF_STATUS & 0x1);
                while(!(SPIF_READY & 0x1));
                /*校验*/
            }
            if((check_flag == 0) && (_R8 >= 64))//校验失败处理
            {
                while(!(SPIF_READY & 0x1));
                SPIF_FIFO_TOP = (Page_Write_Addr - 0x2 + 0x20);
                while(!(SPIF_READY & 0x1));
                while(SPIF_STATUS & 0x1);
            }
            else
            {
                PMC2_DOR = _R8;
                while(PMC2_STR & 0x1);//等主机读走
            }
            check_flag = 1;
        }
        else if(ECU_Cmd == 0xD5)//update_debug
        {
            PRINTF_TX = 'h';
            //update_debug
            WDT_TORR = 0xffff; // 设置最长延时
            WDT_CR |= 0x02; // 进入wdt中断
            WDT_CRR = 0x76;    // 重启计数器
            SYSCTL_RESERVED |= (1 << 3);
            goto * 0x80084UL;
            //update_debug
        }
    }
}
/****************************
 * 函数名：加载funcvv到dram
 *****************************/
FUNCT_PTR_V_V SECTION(".update.load") Load_funVV_To_Dram(FUNCT_PTR_V_V funcpoint, const u16 malloc_size)
{
    // dprint("Load void Function(VOID) to DRAM!\n");
    Cache2Ram_ptr = malloc(malloc_size); // 申请dram空间
    if(!Cache2Ram_ptr)
        return NULL;
    // 判断函数偏移位置是否正确位置准确
    register u32 *ram_ptr = (u32 *)Cache2Ram_ptr;
    register u32 *func_ptr = (u32 *)(funcpoint);
    for(register u16 i = 0; i < ((malloc_size + 3) >> 2); i++)
    {
        *ram_ptr = *func_ptr;
        ram_ptr++;
        func_ptr++;
    }
    return Cache2Ram_ptr;
}
// flash更新程序
void SECTION(".update.function") Flash_Update_Function(void)
{
    BYTE mode = 0xff; // mode=0x0：串口模式，0x1：sharemem模式,0x2:firmware，0xff无效
    BYTE flag = 0x00; // 0x0无flag，0x1:开启校验
    if(UART_UPDATE && uart_updata_flag & 0x1)
    {
        uart_updata_flag &= ~0x1;//清除该标志防止重复进入
        update_reg_ptr &= 0xff00;
        mode = 0x0;
        if(uart_updata_flag & 0x2)
            flag |= 1;
        while(!(UPDATE_LSR & UART_LSR_TEMP)); /*当此位为空发送fifo写入数据*/
    #ifdef AE103
        void (*pFunc)(BYTE, DWORD);
        pFunc = (void (*)(BYTE, DWORD))(*((DWORDP)0x10200)); // ROM_UPDATE
        (*pFunc)(1, update_reg_ptr);
    #elif (defined(AE101) || defined(AE102))
        Cache2Ram_ptr = Load_Smfi_To_Dram(EC_UART_Update, 0x800); // 加载func到dram
    #endif
    }
#if 0
    else if(SHAREMEM_UPDATE && !strcmp((const char *)(SRAM_BASE_ADDR + 0x100), "update firmware\0") && (eFlash_Forbid_Flag == 0))
    #else
    else if(SHAREMEM_UPDATE && *((volatile uint8_t *)(SRAM_BASE_ADDR + 0x100)) == 0xAB && (eFlash_Forbid_Flag == 0))
    #endif
    {
    #if (GLE01 == 1)
        {
            printf("jump mailbx update\n");

            uint8_t Update_Addr0, Update_Addr1, Update_Addr2 = 0;
            uint32_t Update_Addr = 0;
            uint32_t FW_256byte_cnt = 0;
            Update_Addr = REG32((SRAM_BASE_ADDR + 0x108));
            FW_256byte_cnt = REG32((SRAM_BASE_ADDR + 0x104));
            printf("%x,%x,%x,%x,%d\n", Update_Addr0, Update_Addr1, Update_Addr2, Update_Addr, FW_256byte_cnt);
            __nop__;
            __nop__;
            // Transport_Update_To_iram1(Mailbox_4KSMS_UPDATE, 0x100);
            // FUNCT_PTR_B_D_D mailbox_update_jump = (FUNCT_PTR_B_D_D)0x34000;
            // (mailbox_update_jump)(0x3, FW_256byte_cnt * 256, Update_Addr); // 直接跳转到IRAM1里的Mailbox_SMS_UPDATE开始执行
            Mailbox_4KSMS_UPDATE(0x3, FW_256byte_cnt * 256, Update_Addr);
        }
#else
        {
            mode = 0x1;
            Load_funVV_To_Dram(EC_SMS_Update, 0x400); // 加载func到dram
        }
    #endif
    }
#if (GLE01 == 1)
    else if(SHAREMEM_UPDATE && (*((volatile uint8_t *)(SRAM_BASE_ADDR + 0x100)) == 0xBB) && (eFlash_Forbid_Flag == 0))
    {
        mode = 0x1;
        printf("SinglePage Write\n");
        //Transport_Func_To_iram1(EC_SinglePage_Update, 0x100); // 加载func到dram
        *(volatile uint8_t *)(SRAM_BASE_ADDR + 0x100) = 0xCC;
        // FUNCT_PTR_V_V mailbox_singlepage_jump = (FUNCT_PTR_V_V)0x34000;
        // (mailbox_singlepage_jump)(); // 直接跳转到IRAM1里的EC_SinglePage_Update开始执行
        EC_SinglePage_Update();
    }
#endif
    else if(IO_UPDATE && update_mode == 0xdc && (eFlash_Forbid_Flag == 0))
    {
        mode = 0x2;
        Load_funVV_To_Dram(EC_IO_Update, 0x600);
    }
    else
    {
    }
    if(mode != 0xff)
    {
        update_mode = mode;
        update_flag = flag;
        WDT_TORR = 0xffff; // 设置最长延时
        WDT_CR &= !0x02; // 不进入wdt中断
        WDT_CRR = 0x76;    // 重启计数器
        //清除中断
        clear_csr(mstatus, MSTATUS_MIE);
        __nop;
        __nop;
        __nop;
        // 进入更新EC_Update();
        if(Cache2Ram_ptr)
            (*Cache2Ram_ptr)();
        else
            dprint("RAM SPACE MALLOC ERROR\n");
    }
}

/***************************************************************************************************************/
#if (GLE01 == 1)
// void Mailbox_SMS_UPDATE(DWORD fw_size, DWORD start_addr)
// {
//     printf("c");
//     Remaining_size = fw_size; // 更新固件大小

//     if (ROM_COPY_CNT < Remaining_size)
//     {
//         // select_flash(EXTERNAL_FLASH);
//         // SPIF_Read_Interface(256, (DWORD)(0x0 + ROM_COPY_CNT), (BYTEP)(0x31000)); // 需要更新的数据从外部FLASH读入
//         E2CINFO0 = 0x10;                                  // 命令字
//         E2CINFO1 = ((DWORD)(0x3 << 24) | Remaining_size); // BYTE3:固件位置标志 BYTE0~2:固件大小
//         E2CINFO2 = start_addr + ROM_COPY_CNT;             // 更新起始地址
//         ROM_COPY_CNT += 256;
//         printf("d:%x,%x,%x\n", E2CINFO0, E2CINFO1, E2CINFO2);
//         E2CINT = 0x2; // 触发对应中断
//         // while ((C2EINFO1 != 0x1) || (C2EINFO1 != 0x2))
//         //     ; // 等待子系统更新回复
//     }
//     else
//     {
//         ROM_COPY_CNT = 0;
//     }
// }

void ALIGNED(4) EC_SinglePage_Update(void)
{
    BYTE Rewrite_cnt = 0;
    while(*((volatile uint8_t *)(SRAM_BASE_ADDR + 0x100)) != 0xDD);

    Rewrite_cnt = *((VBYTEP)(SRAM_BASE_ADDR + 0x107));

    // 将重写的数据及地址写入E2CINFO
    if(Rewrite_cnt != 0)
    {
        for(int i = 0; i <= (Rewrite_cnt / 2); i++)
        {
            *((VDWORDP)(0x32008 + i * 4)) = *((VDWORDP)(SRAM_BASE_ADDR + 0x108 + i * 4));
        }
    }
    E2CINFO0 = 0x11;
    E2CINFO1 = (DWORD)(*((VBYTEP)(SRAM_BASE_ADDR + 0x107)) << 24) + (DWORD)(*((VBYTEP)(SRAM_BASE_ADDR + 0x106)) << 16) + (DWORD)(*((VBYTEP)(SRAM_BASE_ADDR + 0x105)) << 8) + (DWORD)(*((VBYTEP)(SRAM_BASE_ADDR + 0x104)));
    E2CINT = 0x2; // 触发对应中断
    while(C2E_CMD != 0x11); // 等待子系统完毕回复
    return;
}

void ALIGNED(4) Mailbox_4KSMS_UPDATE(BYTE mode, DWORD fw_size, DWORD start_addr)
{
    // printf("c");
    PRINTF_TX = 'A';

    // if ((start_addr + fw_size) > 0x80000)
    // {
    //     // printf("更新固件大小超出安全区，请重新设置\n");
    //     PRINTF_TX = 'B';
    //     return;
    // }

    E2CINFO0 = 0x10;                            // 命令字
    E2CINFO1 = ((DWORD)(mode << 24) | fw_size); // BYTE3:固件位置标志 BYTE0~2:固件大小
    E2CINFO2 = start_addr;                      // 更新起始地址
    // printf("d:%x,%x,%x\n", E2CINFO0, E2CINFO1, E2CINFO2);
    PRINTF_TX = 'C';
    E2CINT = 0x2; // 触发对应中断
    while(C2E_CMD != 0x10); // 等待子系统更新完毕回复
    // WDT_CRR = 0x76;

    if(C2EINFO1 == 0x1)
    {
        // printf("更新完毕\n");
        PRINTF_TX = 'D';
        WDT_TORR = 0xffff; // 设置最长延时
        WDT_CR |= 0x02;    // 进入wdt中断
        WDT_CRR = 0x76;    // 重启计数器
        SYSCTL_RESERVED |= BIT3;
    }
    else if(C2EINFO1 == 0x2)
        // printf("更新失败\n");
        PRINTF_TX = 'E';
    else
        // printf("错误回复\n");
        PRINTF_TX = 'E';
}

// mailbox flash更新程序
void ALIGNED(4) Mailbox_Update_Function(BYTE mode, DWORD fwsize, DWORD update_addr)
{

    printf("a");
    BYTE mailbox_update_mode = 0x0; // mode=0x1：IRAM0模式，0x2：片外FLASH, 0x3:4K SHAREMEM，0x4~0x7:UART，0x8~0xD:I2C, 0x0/其他值无效
    // DWORD fw_size = fwsize;         // 测试初始化更新固件的大小 测试大小:32K
    // DWORD start_addr = update_addr; // 测试初始化更新起始地址   测试地址450K之后的32K
    // BYTE flag = 0x00;               // 0x0无flag，0x1:开启校验

    mailbox_update_mode = mode;

    // if ((start_addr + fw_size) > secure_area)
    // {
    //     printf("更新固件大小超出安全区，请重新设置\n");
    //     return;
    // }

    if(mailbox_update_mode == 0x1) // IRAM0模式更新
    {
    }
    else if(mailbox_update_mode == 0x2) // 片外FLASH模式更新（在rom里完成）
    {
    }
    else if(mailbox_update_mode == 0x3) // 4K SMS模式更新
    {
    }
    else if((mailbox_update_mode >= 0x4) && (mailbox_update_mode <= 0x7)) // UART模式更新
    {
        /* code */
    }
    else if((mailbox_update_mode >= 0x8) && (mailbox_update_mode <= 0xD)) // I2C模式更新
    {
        /* code */
    }
    else
    {
        printf("无效更新模式，请确认更新命令\n");
        C2EINFO1 = 0x2; // 固件更新不成功
        return;
    }
}

ALIGNED(4) void Go2Ram_WaitUpdate(void)
{
    while(1)
    {
        if(C2EINT == 0x2)
        {
            if(C2EINFO0 == 0x13)
            {
                PRINTF_TX = 'S';
                PRINTF_TX = 'U';
                PRINTF_TX = 'C';
                PRINTF_TX = 'C';
                PRINTF_TX = 'E';
                PRINTF_TX = 'S';
                SYSCTL_RST1 |= BIT(16);//chip reset
                __nop;
                SYSCTL_RST1 &= ~BIT(16);//release chip reset
                goto * 80084UL;
            }
        }
    }
}

void GLE01_Flash_Update_Function(void)
{
    uint32_t Update_Addr = 0;
    uint32_t FM_size = (REG8((SRAM_BASE_ADDR + 0x104))) | (REG8((SRAM_BASE_ADDR + 0x105)) << 8) | (REG8((SRAM_BASE_ADDR + 0x106)) << 16) | (REG8((SRAM_BASE_ADDR + 0x107)) << 24);
    uint8_t flashOption = REG8((SRAM_BASE_ADDR + 0x110));
    printf("FM_size:%d\n", FM_size);
    printf("flashOption:%x\n", flashOption);
    if(SYSCTL_RST1 & 0x00000100)
    {
        SYSCTL_RST1 &= 0xfffffeff; // 释放复位
    }
    sysctl_iomux_config(GPIOB, 17, 0x1);//wp
    sysctl_iomux_config(GPIOB, 20, 0x1);//mosi
    sysctl_iomux_config(GPIOB, 21, 0x1);//miso
    sysctl_iomux_config(GPIOB, 22, 0x1);//cs0
    sysctl_iomux_config(GPIOB, 23, 0x1);//clk
    sysctl_iomux_config(GPIOB, 30, 0x1);//hold
    /* 中断屏蔽 */
    Disable_Interrupt_Main_Switch();
    E2CINFO0 = 0x13;
    E2CINFO1 = flashOption;//0x0:内部flash，0x1:外部flash
    E2CINFO2 = FM_size;//更新大小
    E2CINFO3 = Update_Addr;//更新地址
    /* 进入ram跑代码 */
    Smf_Ptr = Load_Smfi_To_Dram(Go2Ram_WaitUpdate, 0x400);
    E2CINT = 0x2; // 触发对应中断
    (*Smf_Ptr)(); // Do Function at malloc address
}

void Reset_Crypto_Cpu(void)
{
    uint32_t Clk_Div = 0;
    SYSCTL_RST1 |= BIT(17);//crypto cpu reset
    __nop;
    SYSCTL_RST1 &= ~BIT(17);//release crypto cpu reset
    printf("crypto cpu reset\n");
    //修改时钟频率
    printf("modify clock frequency\n");
    Clk_Div = SYSCTL_CLKDIV_OSC96M;
    if(SYSCTL_ESTAT & BIT(24))
    {
        //由于fpga暂时跑不了
        //SYSCTL_CLKDIV_OSC96M=0;
        SYSCTL_CLKDIV_OSC96M = 3;
    }
    else
    {
        SYSCTL_CLKDIV_OSC96M = 3;
    }

    //等待子系统初始化完成
    printf("wait crypto bootctrl\n");
    while(SYSCTL_ESTAT_CRYPTO_BOOTCTRL != 1);

    //恢复时钟频率
    printf("modify clock frequency done\n");
    SYSCTL_CLKDIV_OSC96M = Clk_Div;
}

void WaitCrypUpdate(void)
{
    while(1)
    {
        if(C2EINT == 0x2)
        {
            do
            {
                C2EINT = C2EINT;
                nop;
            }
            while(C2EINT);
            if(C2EINFO0 == 0xaa)
            {
                PRINTF_TX = 'S';
                PRINTF_TX = 'U';
                PRINTF_TX = 'C';
                PRINTF_TX = 'C';
                PRINTF_TX = 'E';
                PRINTF_TX = 'S';
                PRINTF_TX = '\n';
                Reset_Crypto_Cpu();
                return;
            }
            else if(C2EINFO0 == 0xee)//失败
            {
                switch(C2EINFO1)
                {
                    case 0x11://crc32校验失败,回退备份成功
                        PRINTF_TX = 'C'; PRINTF_TX = 'R'; PRINTF_TX = 'C'; PRINTF_TX = '\n';
                        Reset_Crypto_Cpu();
                        return;
                    case 0x22://哈希校验失败,回退备份成功
                        PRINTF_TX = 'H'; PRINTF_TX = 'A'; PRINTF_TX = 'S'; PRINTF_TX = 'H'; PRINTF_TX = '\n';
                        Reset_Crypto_Cpu();
                        return;
                    case 0x33://哈希校验失败,回退备份失败
                        PRINTF_TX = 'H'; PRINTF_TX = 'A'; PRINTF_TX = 'S'; PRINTF_TX = 'H'; PRINTF_TX = 'E'; PRINTF_TX = 'R'; PRINTF_TX = '\n';
                        Reset_Crypto_Cpu();
                        return;
                    case 0x44://哈希校验失败,无备份代码
                        PRINTF_TX = 'H'; PRINTF_TX = 'A'; PRINTF_TX = 'S'; PRINTF_TX = 'H'; PRINTF_TX = 'N'; PRINTF_TX = '\n';
                        Reset_Crypto_Cpu();
                        return;
                    case 0x55://crc32校验失败,回退备份失败
                        PRINTF_TX = 'C'; PRINTF_TX = 'R'; PRINTF_TX = 'C'; PRINTF_TX = 'E'; PRINTF_TX = '\n';
                        Reset_Crypto_Cpu();
                        return;
                    case 0x66://crc32校验失败,无备份代码
                        PRINTF_TX = 'C'; PRINTF_TX = 'R'; PRINTF_TX = 'C'; PRINTF_TX = 'N'; PRINTF_TX = '\n';
                        Reset_Crypto_Cpu();
                        return;
                    default:
                        return;
                }
            }
        }
    }
}

void storeFirmwareSizeInMem(uint8_t TransType, uint8_t *buffer)
{
    uint16_t rx_cnt = 0;
    uint32_t rx_timeout = 0;
    if(TransType == 0x0)//uart update
    {
        while(rx_cnt < 256)
        {
            while(!(UARTA_LSR & UART_LSR_DR));
            buffer[rx_cnt] = UARTA_RX;
            rx_cnt++;
        }
    }

}

void reportDone(uint8_t TransType, uint8_t data)
{
    if(TransType == 0x0)//uart update
    {
        while(!(UARTA_LSR & UART_LSR_TEMP));
        UARTA_TX = data;
    }
}

void GLE01_Cryp_Update_Function(void)
{
    printf("GLE01_Cryp_Update_Function\n");
    /* 中断屏蔽 */
    Disable_Interrupt_Main_Switch();

    //通知子系统进入更新函数
    E2CINFO0 = 0x14;
    E2CINFO1 = 0x0;
    E2CINT = 0x2;
    while(C2EINT != 0x2);
    do
    {
        C2EINT = C2EINT;
        nop;
    }
    while(C2EINT);
    while(C2EINFO0 != 0xcc);
    while(C2EINFO1 != 0x00);//子系统回复处理完毕
    reportDone(0, 0xAA);//回复上位机芯片已经进入更新函数
    printf("send 0xaa to host\n");

    uint32_t i = 0;
    uint8_t Temp_buffer[256];
    sCryptoFlashInfo CrypDramCodeinfo;
    //先接收CrypDramCodeinfo并拷贝到(SRAM_BASE_ADDR+0x100)地址上
    storeFirmwareSizeInMem(0, Temp_buffer);
    memcpy((void *)&CrypDramCodeinfo, (void *)Temp_buffer, sizeof(sCryptoFlashInfo));
    printf("fm_size:%d CRC32:%08x\n", CrypDramCodeinfo.Crypto_CopySize, CrypDramCodeinfo.Crc32Data);
    printf("hash:");
    for(i = 0; i < 32; i++)
    {
        printf("%02x", CrypDramCodeinfo.HASH_AES_ECB_RTL_KEY[i]);
    }
    printf("\n");
    memcpy((void *)((uint8_t *)(SRAM_BASE_ADDR + 0x100)), (void *)Temp_buffer, sizeof(sCryptoFlashInfo));
    E2CINFO0 = 0xcc;
    E2CINFO1 = 0x1;//发送固件信息
    E2CINT = 0x2;//给子系统发送命令
    while(C2EINT != 0x2);
    do
    {
        C2EINT = C2EINT;
        nop;
    }
    while(C2EINT);
    while(C2EINFO0 != 0xcc);
    while(C2EINFO1 != 0x01);//子系统回复处理完毕
    E2CINFO0 = 0; E2CINFO1 = 0;//复位该寄存器值
    reportDone(0, 0xAA);//回复上位机获取固件信息命令处理完毕
    printf("send 0xaa to host\n");

    //接收加密数据并拷贝到(SRAM_BASE_ADDR+0x100)地址上
    for(i = 0; i < CrypDramCodeinfo.Crypto_CopySize; i += 256)
    {
        storeFirmwareSizeInMem(0, Temp_buffer);
        //打印接收到的数据
        // for (int i = 0; i < 256; i += 16) {
        // // 打印地址
        //     printf("%08X  ", i);

        //     // 打印数据
        //     for (int j = 0; j < 16; j++) {
        //         if (i + j < 256) {
        //             printf("%02X ", Temp_buffer[i + j]);
        //         } else {
        //             printf("   "); // 对于不足的地方，填充空格
        //         }
        //     }
        //     printf("\n");
        // }
        memcpy((void *)((uint8_t *)(SRAM_BASE_ADDR + 0x100)), (void *)Temp_buffer, 256);
        E2CINFO0 = 0xcc;
        E2CINFO1 = 0x2;//发送code密文
        E2CINT = 0x2;//给子系统发送命令
        while(C2EINT != 0x2);
        do
        {
            C2EINT = C2EINT;
            nop;
        }
        while(C2EINT);
        while(C2EINFO0 != 0xcc);
        while(C2EINFO1 != 0x2);//子系统回复处理完毕
        E2CINFO0 = 0; E2CINFO1 = 0;//复位该寄存器值
        reportDone(0, 0xAA);//回复上位机一笔数据处理完毕
        printf("send 0xaa to host\n");
        printf("i:%d CopySize:%d\n", i + 256, CrypDramCodeinfo.Crypto_CopySize);
    }
    /* 进入ram跑代码 */
    WaitCrypUpdate();
    Enable_Interrupt_Main_Switch();
}
#endif
