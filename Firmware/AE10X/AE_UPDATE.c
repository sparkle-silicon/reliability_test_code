/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-20 16:46:57
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
char uart_crtpram_updatebuffer[16];
int uart_crypram_updateindex = 0;
char update_crypram_cmd[12] = { 0x64 ,0x72 ,0x61 ,0x6D ,0x30 ,0x2D ,0x75 ,0x70 ,0x64 ,0x61 ,0x74 ,0x65 };
char update_extflash_cmd[16] = { 0x75 ,0x70 ,0x64 ,0x61 ,0x74 ,0x65 ,0x20 ,0x66 ,0x69 ,0x6d ,0x77 ,0x61 ,0x72 ,0x65 ,0x2D ,0x65 };
uint8_t update_crypram_flag = 0;
uint8_t update_intflash_flag = 0;
VBYTE check[32];
FUNCT_PTR_V_V Cache2Ram_ptr = NULL;
void reportDone(uint8_t TransType, uint8_t data);
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
    while(!(SPIFE_RDY & SPIF_RDY_READY));
    SPIFE_FTOP = 0xc7;
    while(!(SPIFE_RDY & SPIF_RDY_READY));
    while(SPIFE_STA & 0xf);
    while(!(SPIFE_RDY & SPIF_RDY_READY));
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
            while(!(SPIFE_RDY & SPIF_RDY_READY));
            SPIFE_DBYTE = 0xff;
            while(!(SPIFE_RDY & SPIF_RDY_READY));
            SPIFE_FTOP = addrs; // 数据地址
            for(i = 0; i < 64; i++)
            {
                while((SPIFE_FCNT & 0x3) != 0);
                SPIFE_FTOP = *(u32 *)(buf_p + j * 256 + i * 4);
            }
            while(!(SPIFE_RDY & SPIF_RDY_READY));
            while(SPIFE_STA & 0xf);
            while(!(SPIFE_RDY & SPIF_RDY_READY));
        #if WRITE_READ_CHECK
            UPDATE_TX = 'b';    // 正在读flash数据
            while(!(UPDATE_LSR & UART_LSR_DR))
                WDT_CRR = 0x76;
                // 读数据
            addrs = addrs & 0xffffff00 + 0x03; // 修改内容
            while(!(SPIFE_RDY & SPIF_RDY_READY))
                WDT_CRR = 0x76; // 读忙
            SPIFE_DBYTE = 0xff;
            while(!(SPIFE_RDY & SPIF_RDY_READY))
                WDT_CRR = 0x76;    // 读忙
            SPIFE_FTOP = addrs; // 数据地址
            register u32 checkdata, checksum1 = 0, checksum2 = 0;
            for(int i = 0; i < 64; i++)
            {
                while((SPIFE_FCNT & 0x3) == 0)
                {
                    WDT_CRR = 0x76; // 读忙
                }
                checkdata = SPIFE_FTOP;
                // while(!(SPIFE_RDY&SPIF_RDY_READY))WDT_CRR =0x76;//读忙
                if(i < 32)
                    checksum1 += ((checkdata >> 0) & 0xff) + ((checkdata >> 8) & 0xff) + ((checkdata >> 16) & 0xff) + ((checkdata >> 24) & 0xff);
                else
                    checksum2 += ((checkdata >> 0) & 0xff) + ((checkdata >> 8) & 0xff) + ((checkdata >> 16) & 0xff) + ((checkdata >> 24) & 0xff);
                WDT_CRR = 0x76;
            }
            while(!(SPIFE_RDY & SPIF_RDY_READY))
                WDT_CRR = 0x76; // 读忙
            while(SPIFE_STA & 0xf)
                WDT_CRR = 0x76; // 直到写完
            while(!(SPIFE_RDY & SPIF_RDY_READY))
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
    SYSCTL_CFG |= BIT3;
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
    while(!(SPIFE_RDY & SPIF_RDY_READY))
        WDT_CRR = 0x76;
    SPIFE_FTOP = 0xc7;
    while(!(SPIFE_RDY & SPIF_RDY_READY))
        WDT_CRR = 0x76;
    while(SPIFE_STA & 0xf)
        WDT_CRR = 0x76;
    while(!(SPIFE_RDY & SPIF_RDY_READY))
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
        while(!(SPIFE_RDY & SPIF_RDY_READY))
            WDT_CRR = 0x76;
        SPIFE_DBYTE = 0xff;
        while(!(SPIFE_RDY & SPIF_RDY_READY))
            WDT_CRR = 0x76;
        SPIFE_FTOP = Update_Addr;
        for(i = 0; i < 64; i++)
        {
            while((SPIFE_FCNT & 0x3) != 0)
                WDT_CRR = 0x76;
            SPIFE_FTOP = *((volatile uint32_t *)(update_buf + i * 4));
        }
        while(!(SPIFE_RDY & SPIF_RDY_READY))
            WDT_CRR = 0x76;
        while(SPIFE_STA & 0xf)
            WDT_CRR = 0x76;
        while(!(SPIFE_RDY & SPIF_RDY_READY))
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
    WDT_TORR0 = 0xff; WDT_TORR1 = 0xff;// 设置最长延时
    WDT_CR |= 0x02;    // 进入wdt中断
    WDT_CRR = 0x76;    // 重启计数器
    SYSCTL_CFG |= BIT3;
    goto * 0x80084UL;
}
void ALIGNED(4) OPTIMIZE0 SECTION(".update.io")   EC_IO_Update(void)
{
    _R7, _R8 = 0;
    uint32_t data[64];
    uint8_t Smf_Addr0, Smf_Addr1, Smf_Addr2, Smf_Data0, Smf_Data1, Smf_Data2, Smf_Data3;
    uint32_t Page_Write_Addr = 0;
    char check_flag = 1;
    while(!(SPIFE_RDY & SPIF_RDY_READY))
        WDT_CRR = 0x76;
    SPIFE_FTOP = 0xc7;
    while(!(SPIFE_RDY & SPIF_RDY_READY))
        WDT_CRR = 0x76;
    while(SPIFE_STA & 0x1)
        WDT_CRR = 0x76;
    while(!(SPIFE_RDY & SPIF_RDY_READY))
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
                while(!(SPIFE_RDY & SPIF_RDY_READY))
                    WDT_CRR = 0x76;
                SPIFE_DBYTE = 0xff;
                while(!(SPIFE_RDY & SPIF_RDY_READY))
                    WDT_CRR = 0x76;
                SPIFE_FTOP = Page_Write_Addr;
                for(_R7 = 0; _R7 < 64; _R7++)
                {
                    while((SPIFE_FCNT & 0x3) != 0)
                        WDT_CRR = 0x76;
                    SPIFE_FTOP = data[_R7];
                }
                while(!(SPIFE_RDY & SPIF_RDY_READY))
                    WDT_CRR = 0x76;
                while(SPIFE_STA & 0x1)
                    WDT_CRR = 0x76;
                while(!(SPIFE_RDY & SPIF_RDY_READY))
                    WDT_CRR = 0x76;
                PRINTF_TX = 'w';//写入完成打印
                /*校验*/
                SPIFE_DBYTE = 0xff;
                while(!(SPIFE_RDY & SPIF_RDY_READY));
                SPIFE_FTOP = (Page_Write_Addr - 0x2 + 0x3);
                for(_R7 = 0; _R7 < 64; _R7++)
                {
                    while((SPIFE_FCNT & 0x3) == 0)
                        WDT_CRR = 0x76;
                    if(data[_R7] != SPIFE_FTOP)//校验失败
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
                while(!(SPIFE_RDY & SPIF_RDY_READY));
                while(SPIFE_STA & 0x1);
                while(!(SPIFE_RDY & SPIF_RDY_READY));
                /*校验*/
            }
            if((check_flag == 0) && (_R8 >= 64))//校验失败处理
            {
                while(!(SPIFE_RDY & SPIF_RDY_READY));
                SPIFE_FTOP = (Page_Write_Addr - 0x2 + 0x20);
                while(!(SPIFE_RDY & SPIF_RDY_READY));
                while(SPIFE_STA & 0x1);
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
            WDT_TORR0 = 0xff; WDT_TORR1 = 0xff; // 设置最长延时
            WDT_CR |= 0x02; // 进入wdt中断
            WDT_CRR = 0x76;    // 重启计数器
            SYSCTL_CFG |= BIT3;
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
        Cache2Ram_ptr = Load_Smfi_To_Dram(EC_UART_Update, 0x800); // 加载func到dram
    }
#if 0
    else if(SHAREMEM_UPDATE && !strcmp((const char *)(SRAM_BASE_ADDR + 0x100), "update firmware\0") && (eFlash_Forbid_Flag == 0))
    #else
    else if(SHAREMEM_UPDATE && *((volatile uint8_t *)(SRAM_BASE_ADDR + 0x100)) == 0xAB && (eFlash_Forbid_Flag == 0))
    #endif
    {
        {
            dprint("jump mailbx update\n");

            uint8_t Update_Addr0, Update_Addr1, Update_Addr2 = 0;
            uint32_t Update_Addr = 0;
            uint32_t FW_256byte_cnt = 0;
            Update_Addr = REG32((SRAM_BASE_ADDR + 0x108));
            FW_256byte_cnt = REG32((SRAM_BASE_ADDR + 0x104));
            dprint("%x,%x,%x,%x,%d\n", Update_Addr0, Update_Addr1, Update_Addr2, Update_Addr, FW_256byte_cnt);
            __nop__;
            __nop__;
            // Transport_Update_To_iram1(Mailbox_4KSMS_UPDATE, 0x100);
            // FUNCT_PTR_B_D_D mailbox_update_jump = (FUNCT_PTR_B_D_D)0x34000;
            // (mailbox_update_jump)(0x3, FW_256byte_cnt * 256, Update_Addr); // 直接跳转到IRAM1里的Mailbox_SMS_UPDATE开始执行
            Mailbox_4KSMS_UPDATE(0x3, FW_256byte_cnt * 256, Update_Addr);
        }
    }
    else if(SHAREMEM_UPDATE && (*((volatile uint8_t *)(SRAM_BASE_ADDR + 0x100)) == 0xBB) && (eFlash_Forbid_Flag == 0))
    {
        mode = 0x1;
        dprint("SinglePage Write\n");
        //Transport_Func_To_iram1(EC_SinglePage_Update, 0x100); // 加载func到dram
        *(volatile uint8_t *)(SRAM_BASE_ADDR + 0x100) = 0xCC;
        // FUNCT_PTR_V_V mailbox_singlepage_jump = (FUNCT_PTR_V_V)0x34000;
        // (mailbox_singlepage_jump)(); // 直接跳转到IRAM1里的EC_SinglePage_Update开始执行
        EC_SinglePage_Update();
    }
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
        WDT_TORR0 = 0xff; WDT_TORR1 = 0xff; // 设置最长延时
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
// void Mailbox_SMS_UPDATE(DWORD fw_size, DWORD start_addr)
// {
//     printf("c");
//     Remaining_size = fw_size; // 更新固件大小

//     if (ROM_COPY_CNT < Remaining_size)
//     {
//         // select_flash(EXTERNAL_FLASH);
//         // SPIFE_Read_Interface(256, (DWORD)(0x0 + ROM_COPY_CNT), (BYTEP)(0x31000)); // 需要更新的数据从外部FLASH读入
//         MAILBOX_E2CINFO0 = 0x10;                                  // 命令字
//         MAILBOX_E2CINFO1 = ((DWORD)(0x3 << 24) | Remaining_size); // BYTE3:固件位置标志 BYTE0~2:固件大小
//         MAILBOX_E2CINFO2 = start_addr + ROM_COPY_CNT;             // 更新起始地址
//         ROM_COPY_CNT += 256;
//         printf("d:%x,%x,%x\n", MAILBOX_E2CINFO0, MAILBOX_E2CINFO1, MAILBOX_E2CINFO2);
//         MAILBOX_E2CINT = 0x2; // 触发对应中断
//         // while ((MAILBOX_C2EINFO1 != 0x1) || (MAILBOX_C2EINFO1 != 0x2))
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

    // 将重写的数据及地址写入MAILBOX_E2CINFO
    if(Rewrite_cnt != 0)
    {
        for(int i = 0; i <= (Rewrite_cnt / 2); i++)
        {
            *((VDWORDP)(0x32008 + i * 4)) = *((VDWORDP)(SRAM_BASE_ADDR + 0x108 + i * 4));
        }
    }
    MAILBOX_E2CINFO0 = 0x11;
    MAILBOX_E2CINFO1 = (DWORD)(*((VBYTEP)(SRAM_BASE_ADDR + 0x107)) << 24) + (DWORD)(*((VBYTEP)(SRAM_BASE_ADDR + 0x106)) << 16) + (DWORD)(*((VBYTEP)(SRAM_BASE_ADDR + 0x105)) << 8) + (DWORD)(*((VBYTEP)(SRAM_BASE_ADDR + 0x104)));
    MAILBOX_E2CINT = 0x2; // 触发对应中断
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

    MAILBOX_E2CINFO0 = 0x10;                            // 命令字
    MAILBOX_E2CINFO1 = ((DWORD)(mode << 24) | fw_size); // BYTE3:固件位置标志 BYTE0~2:固件大小
    MAILBOX_E2CINFO2 = start_addr;                      // 更新起始地址
    // printf("d:%x,%x,%x\n", MAILBOX_E2CINFO0, MAILBOX_E2CINFO1, MAILBOX_E2CINFO2);
    PRINTF_TX = 'C';
    MAILBOX_E2CINT = 0x2; // 触发对应中断
    while(C2E_CMD != 0x10); // 等待子系统更新完毕回复
    // WDT_CRR = 0x76;

    if(MAILBOX_C2EINFO1 == 0x1)
    {
        // printf("更新完毕\n");
        PRINTF_TX = 'D';
        WDT_TORR0 = 0xff; WDT_TORR1 = 0xff; // 设置最长延时
        WDT_CR |= 0x02;    // 进入wdt中断
        WDT_CRR = 0x76;    // 重启计数器
        SYSCTL_CFG |= BIT3;
    }
    else if(MAILBOX_C2EINFO1 == 0x2)
        // printf("更新失败\n");
        PRINTF_TX = 'E';
    else
        // printf("错误回复\n");
        PRINTF_TX = 'E';
}

// mailbox flash更新程序
void ALIGNED(4) Mailbox_Update_Function(BYTE mode, DWORD fwsize, DWORD update_addr)
{

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
        dprint("无效更新模式，请确认更新命令\n");
        MAILBOX_C2EINFO1 = 0x2; // 固件更新不成功
        return;
    }
}
void Reset_Crypto_Cpu(void)
{
    uint32_t Clk_Div = 0;
    SYSCTL_RST1 |= BIT(17);//crypto cpu reset
    __nop;
    SYSCTL_RST1 &= ~BIT(17);//release crypto cpu reset
    dprint("crypto cpu reset\n");
    //修改时钟频率
    dprint("modify clock frequency\n");
    Clk_Div = SYSCTL_CLKDIV_OSC96M;
    if(SYSCTL_ESTAT & BIT(24))
    {
        SYSCTL_CLKDIV_OSC96M = 0;
    }
    else
    {
        SYSCTL_CLKDIV_OSC96M = 3;
    }

    //等待子系统初始化完成
    dprint("wait crypto bootctrl\n");
    while(SYSCTL_ESTAT_CRYPTO_BOOTCTRL != 1);

    //恢复时钟频率
    dprint("modify clock frequency done\n");
    SYSCTL_CLKDIV_OSC96M = Clk_Div;
}

void WaitCrypUpdate(void)
{
    while(1)
    {
        if(MAILBOX_C2EINT == 0x2)
        {
            do
            {
                MAILBOX_C2EINT = MAILBOX_C2EINT;
                nop;
            }
            while(MAILBOX_C2EINT);
            while((MAILBOX_C2EINFO0 != 0x14));
            if(MAILBOX_C2EINFO1 == 0x1)
            {
                PRINTF_TX = 'S';
                PRINTF_TX = 'U';
                PRINTF_TX = 'C';
                PRINTF_TX = 'C';
                PRINTF_TX = 'E';
                PRINTF_TX = 'S';
                PRINTF_TX = '\n';
                reportDone(0, 0xAA);//回复上位机一笔数据处理完毕
                Reset_Crypto_Cpu();
                return;
            }
            else//失败
            {
                switch((MAILBOX_C2EINFO1 >> 8) & 0xff)
                {
                    case 0x01://crc32校验失败,回退备份成功
                        reportDone(0, 0x01);//回复上位机一笔数据处理完毕
                        PRINTF_TX = 'C'; PRINTF_TX = 'R'; PRINTF_TX = 'C'; PRINTF_TX = '\n';
                        Reset_Crypto_Cpu();
                        return;
                    case 0x02://哈希校验失败,回退备份成功
                        reportDone(0, 0x02);//回复上位机一笔数据处理完毕
                        PRINTF_TX = 'H'; PRINTF_TX = 'A'; PRINTF_TX = 'S'; PRINTF_TX = 'H'; PRINTF_TX = '\n';
                        Reset_Crypto_Cpu();
                        return;
                    case 0x03://哈希校验失败,回退备份失败
                        reportDone(0, 0x03);//回复上位机一笔数据处理完毕
                        PRINTF_TX = 'H'; PRINTF_TX = 'A'; PRINTF_TX = 'S'; PRINTF_TX = 'H'; PRINTF_TX = 'E'; PRINTF_TX = 'R'; PRINTF_TX = '\n';
                        Reset_Crypto_Cpu();
                        return;
                    case 0x04://哈希校验失败,无备份代码
                        reportDone(0, 0x04);//回复上位机一笔数据处理完毕
                        PRINTF_TX = 'H'; PRINTF_TX = 'A'; PRINTF_TX = 'S'; PRINTF_TX = 'H'; PRINTF_TX = 'N'; PRINTF_TX = '\n';
                        Reset_Crypto_Cpu();
                        return;
                    case 0x05://crc32校验失败,回退备份失败
                        reportDone(0, 0x05);//回复上位机一笔数据处理完毕
                        PRINTF_TX = 'C'; PRINTF_TX = 'R'; PRINTF_TX = 'C'; PRINTF_TX = 'E'; PRINTF_TX = '\n';
                        Reset_Crypto_Cpu();
                        return;
                    case 0x06://crc32校验失败,无备份代码
                        reportDone(0, 0x06);//回复上位机一笔数据处理完毕
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
    if(TransType == 0x0)//uart update
    {
        while(rx_cnt < 257)
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

// CRC-8常用参数：
// 多项式 (x^8 + x^2 + x + 1) => 0x07
// 初始值 0x00
// 输出 XOR 值 0x00
// 数据输入是字节（8位）
uint8_t CRC8(const uint8_t *data, size_t length)
{
    uint8_t crc = 0x00;  // 初始值
    uint8_t polynomial = 0x07;  // CRC-8多项式：x^8 + x^2 + x + 1 (0x07)

    for(size_t i = 0; i < length; i++)
    {
        crc ^= data[i];  // 将当前字节与 CRC 值异或

        // 对每一位执行 CRC 计算
        for(int j = 0; j < 8; j++)
        {
            if(crc & 0x80)
            {  // 如果 CRC 的最高位为1
                crc = (crc << 1) ^ polynomial;  // 左移并异或多项式
            }
            else
            {
                crc <<= 1;  // 否则仅左移
            }
        }
    }
    return crc;  // 返回最终的 CRC 值
}

void Cryp_Update_Function(void)
{
    dprint("Cryp_Update_Function\n");
    /* 中断屏蔽 */
    Disable_Interrupt_Main_Switch();

    //通知子系统进入更新函数
    MAILBOX_E2CINFO0 = 0x14;
    MAILBOX_E2CINFO1 = 0x0;
    MAILBOX_E2CINT = 0x2;
    while(MAILBOX_C2EINT != 0x2);
    do
    {
        MAILBOX_C2EINT = MAILBOX_C2EINT;
        nop;
    }
    while(MAILBOX_C2EINT);
    while(MAILBOX_C2EINFO0 != 0x14);
    while(MAILBOX_C2EINFO1 != 0x00);//子系统回复处理完毕
    reportDone(0, 0xAA);//回复上位机芯片已经进入更新函数
    dprint("send 0xaa to host\n");

    uint32_t i = 0;
    uint8_t Temp_buffer[257];
    sCryptoFlashInfo CrypDramCodeinfo;
    //先接收CrypDramCodeinfo并拷贝到(SRAM_BASE_ADDR+0x100)地址上
    for(int i = 0; i < 10; i++)
    {
        storeFirmwareSizeInMem(0, Temp_buffer);
        if(CRC8(Temp_buffer, 256) != Temp_buffer[256])
        {
            reportDone(0, 0xEE);//回复上位机接收错误
            dprint("header crc error send 0xee to host\n");
            while((UARTA_LSR & UART_LSR_DR))//清空接收缓存
            {
                UARTA_RX;
            }
            reportDone(0, 0xAA);//下位机准备完成可以接收数据
        }
        else
        {
            while((UARTA_LSR & UART_LSR_DR))//清空接收缓存
            {
                UARTA_RX;
            }
            break;
        }
    }

    memcpy((void *)&CrypDramCodeinfo, (void *)Temp_buffer, sizeof(sCryptoFlashInfo));
    dprint("fm_size:%d CRC32:%08x\n", CrypDramCodeinfo.Crypto_CopySize, CrypDramCodeinfo.Crc32Data);
    dprint("hash:");
    for(i = 0; i < 32; i++)
    {
        dprint("%02x", CrypDramCodeinfo.HASH_AES_ECB_RTL_KEY[i]);
    }
    dprint("\n");
    memcpy((void *)((uint8_t *)(SRAM_BASE_ADDR + 0x100)), (void *)Temp_buffer, sizeof(sCryptoFlashInfo));
    MAILBOX_E2CINFO0 = 0x14;
    MAILBOX_E2CINFO1 = 0x1;//发送固件信息
    MAILBOX_E2CINT = 0x2;//给子系统发送命令
    while(MAILBOX_C2EINT != 0x2);
    do
    {
        MAILBOX_C2EINT = MAILBOX_C2EINT;
        nop;
    }
    while(MAILBOX_C2EINT);
    while(MAILBOX_C2EINFO0 != 0x14);
    while(MAILBOX_C2EINFO1 != 0x01);//子系统回复处理完毕
    MAILBOX_E2CINFO0 = 0; MAILBOX_E2CINFO1 = 0;//复位该寄存器值
    reportDone(0, 0xAA);//回复上位机获取固件信息命令处理完毕
    dprint("send 0xaa to host\n");

    //接收加密数据并拷贝到(SRAM_BASE_ADDR+0x100)地址上
    for(i = 0; i < CrypDramCodeinfo.Crypto_CopySize; i += 256)
    {
        for(int i = 0; i < 10; i++)
        {
            storeFirmwareSizeInMem(0, Temp_buffer);
            uint8_t crc = CRC8(Temp_buffer, 256);
            if(crc != Temp_buffer[256])
            {
                reportDone(0, 0xEE);//回复上位机接收错误
                //打印接收到的数据
                for(int i = 0; i < 256; i += 16)
                {
// 打印地址
                    dprint("%08X  ", i);

                    // 打印数据
                    for(int j = 0; j < 16; j++)
                    {
                        if(i + j < 256)
                        {
                            dprint("%02X ", Temp_buffer[i + j]);
                        }
                        else
                        {
                            dprint("   "); // 对于不足的地方，填充空格
                        }
                    }
                    dprint("\n");
                }
                dprint("c crc:%02x  r crc:%02x  send 0xee to host\n", crc, Temp_buffer[256]);
                while((UARTA_LSR & UART_LSR_DR))//清空接收缓存
                {
                    UARTA_RX;
                }
                reportDone(0, 0xAA);//下位机准备完成可以接收数据
            }
            else
            {
                while((UARTA_LSR & UART_LSR_DR))//清空接收缓存
                {
                    UARTA_RX;
                }
                break;
            }
        }
        memcpy((void *)((uint8_t *)(SRAM_BASE_ADDR + 0x100)), (void *)Temp_buffer, 256);
        MAILBOX_E2CINFO0 = 0x14;
        MAILBOX_E2CINFO1 = 0x2;//发送code密文
        MAILBOX_E2CINT = 0x2;//给子系统发送命令
        while(MAILBOX_C2EINT != 0x2);
        do
        {
            MAILBOX_C2EINT = MAILBOX_C2EINT;
            nop;
        }
        while(MAILBOX_C2EINT);
        while(MAILBOX_C2EINFO0 != 0x14);
        while(MAILBOX_C2EINFO1 != 0x2);//子系统回复处理完毕
        MAILBOX_E2CINFO0 = 0; MAILBOX_E2CINFO1 = 0;//复位该寄存器值
        reportDone(0, 0xAA);//回复上位机一笔数据处理完毕
        dprint("send 0xaa to host\n");
        dprint("i:%d CopySize:%d\n", i + 256, CrypDramCodeinfo.Crypto_CopySize);
    }
    /* 进入ram跑代码 */
    WaitCrypUpdate();
    Enable_Interrupt_Main_Switch();
}


char ExtFlash_Update_Function(void)
{
    dprint("ExtFlash_Update_Function\n");
    Disable_Interrupt_Main_Switch();
    uint8_t Crc_fail_flag = 0;
    uint8_t Crc8_fail_cnt = 0;
    uint32_t looplimit = (CHIP_CLOCK_INT_HIGH / 15) / (SYSCTL_CLKDIV_OSC96M + 1);
    uint32_t Fw_Size = 0;
    uint32_t Sram_idx = 0;
    uint32_t Copy_Addr = 0;
    uint32_t Backup_Addr = 0;
    uint32_t Backup_Fw_Size = 0;
    uint32_t Fw_crc32 = 0;
    uint8_t Backup_En = 0, Rest_Flag = 0;
    uint8_t Temp_buffer[257];
    uint8_t Err_Sta = 0, Ack_Sta = 0;
    reportDone(0, 0xAA);//回复上位机进入更新函数
    dprint("chip enter update func send 0xaa to host\n");
    //先接收更新地址与文件大小信息
    for(int i = 0; i < 10; i++)
    {
        storeFirmwareSizeInMem(0, Temp_buffer);
        if(CRC8(Temp_buffer, 256) != Temp_buffer[256])
        {
            dprint("fifo crc8 error again\n");
            reportDone(0, 0xEE);
            //清空接收缓存
            while(UARTA_LSR & UART_LSR_DR)
            {
                UARTA_RX;
            }
            reportDone(0, 0xAA);
            Crc8_fail_cnt++;
            if(Crc8_fail_cnt >= 10)
            {
                Crc8_fail_cnt = 0;
                Crc_fail_flag = 1;
            }
        }
        else
        {
            Crc8_fail_cnt = 0;
            //清空接收缓存
            while(UARTA_LSR & UART_LSR_DR)
            {
                UARTA_RX;
            }
            break;

        }
    }
    Fw_Size = (uint32_t)(Temp_buffer[0] | (Temp_buffer[1] << 8) | (Temp_buffer[2] << 16) | (Temp_buffer[3] << 24));
    Copy_Addr = (uint32_t)(Temp_buffer[4] | (Temp_buffer[5] << 8) | (Temp_buffer[6] << 16) | (Temp_buffer[7] << 24));
    Fw_crc32 = (uint32_t)(Temp_buffer[8] | (Temp_buffer[9] << 8) | (Temp_buffer[10] << 16) | (Temp_buffer[11] << 24));
    Backup_En = Temp_buffer[12]; Rest_Flag = Temp_buffer[13];

    //向上4k对齐
    if(Fw_Size % 4096 != 0)
    {
        Fw_Size = Fw_Size + (4096 - (Fw_Size % 4096));
    }
    dprint("Fw_Size:%d Copy_Addr:%08x Backup_Addr:%08x Backup_Fw_Size:%d Backup_En:%d Rest_Flag:%d\n", Fw_Size, Copy_Addr, Backup_Addr, Backup_Fw_Size, Backup_En, Rest_Flag);

    //将头信息传递给子系统
    MAILBOX_E2CINFO1 = 0x0;				//选择外部flash
    MAILBOX_E2CINFO2 = Copy_Addr;     	//更新flash的起始地址
    MAILBOX_E2CINFO3 = Fw_Size;       	//更新flash的大小
    MAILBOX_E2CINFO4 = Backup_Addr;     	//备份区flash的起始地址
    MAILBOX_E2CINFO5 = Backup_Fw_Size;  	//备份程序的大小
    MAILBOX_E2CINFO6 = Backup_En;       	//备份使能状态
    MAILBOX_E2CINFO7 = Fw_crc32;			//固件内容crc校验码
    MAILBOX_E2CINFO0 = 0x13;				//通知子系统更新命令
    MAILBOX_E2CINT = 0x2;					//通知子系统更新命令
    //等待子系统收到数据后回应
    while(MAILBOX_C2EINT != 0x2);
    do
    {
        MAILBOX_C2EINT = MAILBOX_C2EINT;
        nop;
    }
    while(MAILBOX_C2EINT);
    while((MAILBOX_C2EINFO0 != 0x13));
    Ack_Sta = (MAILBOX_C2EINFO1 & 0xff);
    Err_Sta = (MAILBOX_C2EINFO1 >> 8) & 0xff;
    if(Ack_Sta == 0x2)
    {
        if(Err_Sta < 4)
        {
            reportDone(0, 0xCC);
            dprint("Parameter input does not match the actual situation Err_Sta:0x%x\n", Err_Sta);
            return 1;
        }
    }
    else
        reportDone(0, 0xAA);//回复上位机获取更新地址信息命令处理完毕
    dprint("s 0xaa\n");

    //接收固件数据并写入flash
    for(uint32_t i = 0; i < Fw_Size; i += 256)
    {
        for(int j = 0; j < 10; j++)
        {
            storeFirmwareSizeInMem(0, Temp_buffer);
            if(CRC8(Temp_buffer, 256) != Temp_buffer[256])
            {
            #if ROM_DEBUG
                dprint("%d crc8 error again\n", i / 256);
            #endif
                reportDone(0, 0xEE);
                //清空接收缓存
                while(UARTA_LSR & UART_LSR_DR)
                {
                    UARTA_RX;
                }
                reportDone(0, 0xAA);
                Crc8_fail_cnt++;
                if(Crc8_fail_cnt >= 10)
                {
                    Crc8_fail_cnt = 0;
                    Crc_fail_flag = 1;
                }
            }
            else
            {
                Crc8_fail_cnt = 0;
                //清空接收缓存
                while(UARTA_LSR & UART_LSR_DR)
                {
                    UARTA_RX;
                }
                break;

            }
        }
    #if ROM_DEBUG
    //printf("spif write block:%d addr:%08x\n",i/256,Copy_Addr+i);
    //printf("s 0xaa\n");
    #endif
        memcpy((void *)(SRAM_BASE_ADDR + Sram_idx), Temp_buffer, 256);
        Sram_idx += 256;
        if(Sram_idx >= 4096)
        {
            Sram_idx = 0;
            //通知子系统进行烧录
            MAILBOX_E2CINFO1 = 0x2;
            MAILBOX_E2CINFO0 = 0x13;
            MAILBOX_E2CINT = 0x2;
            //等待子系统烧录完成
        #if ROM_DEBUG
        //printf("wait cry write over\n");
        #endif
            while(MAILBOX_C2EINT != 0x2);
            do
            {
                MAILBOX_C2EINT = MAILBOX_C2EINT;
                nop;
            }
            while(MAILBOX_C2EINT);
            while((MAILBOX_C2EINFO0 != 0x13) || (MAILBOX_C2EINFO1 != 0x03));
        }
        reportDone(0, 0xAA);//回复上位机一笔数据处理完毕
        nop; nop; nop; nop; nop;
    }
    //回复子系统上位机到EC数据校验结果
    MAILBOX_E2CINFO1 = Crc_fail_flag;     //通知子系统校验结果
    MAILBOX_E2CINFO0 = 0x13;				//通知子系统更新命令
    MAILBOX_E2CINT = 0x2;					//通知子系统更新命令
    //等待子系统回复更新结果
    while(MAILBOX_C2EINT != 0x2);
    do
    {
        MAILBOX_C2EINT = MAILBOX_C2EINT;
        nop;
    }
    while(MAILBOX_C2EINT);
    while((MAILBOX_C2EINFO0 != 0x13));
    Ack_Sta = (MAILBOX_C2EINFO1 & 0xff);
    Err_Sta = (MAILBOX_C2EINFO1 >> 8) & 0xff;
    if(Ack_Sta == 0x2)
        if(Err_Sta == 0x4)
            Crc_fail_flag = 1;

    //主系统校验处理
    if(Crc_fail_flag)
    {
        dprint("update fail\n");
        return 1;
    }
    else
    {
        for(volatile uint32_t counter = looplimit; (!(UARTA_LSR & UART_LSR_TEMP)) && counter; counter--);
        dprint("update success\n");
        //默认不复位CPU
        if(Rest_Flag)
            Reset_Crypto_Cpu();
    }
    Enable_Interrupt_Main_Switch();
    return 1;
}
