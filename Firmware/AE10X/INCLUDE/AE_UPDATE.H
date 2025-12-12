/*
 * @Author: Linyu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2025-06-04 16:03:50
 * @Description:
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
#ifndef __AE_UPDATA_H_
#define __AE_UPDATA_H_
#include "AE_CONFIG.H"
extern char uart_crtpram_updatebuffer[16];
extern char update_extflash_cmd[16];
extern int uart_crypram_updateindex;
extern char update_crypram_cmd[12];
extern uint8_t update_crypram_flag;
extern uint8_t update_intflash_flag;
// update config
#define UART_UPDATE 1
#define SHAREMEM_UPDATE 1 & SUPPORT_SHAREMEM_PNP
#define IO_UPDATE 0
// update size
#define update_code_size 1024 // code_size =1000byte of code +16byte of size +8byte of cnt
#define update_cnt_size 2
#define update_state_size 2
#define update_check_size 16 // check_size 16*8=128bit
#define update_size (update_code_size + update_check_size + update_state_size + update_cnt_size)
#define flash_page_write 0x02
#define update_reg_ptr g_uart_base
#define UPDATE_REG(addr) REG8(g_uart_base + addr)
#define UPDATE_RX UPDATE_REG(UART_RBR_OFFSET)
#define UPDATE_TX UPDATE_REG(UART_THR_OFFSET)
#define UPDATE_DLL UPDATE_REG(UART_DLL_OFFSET)
#define UPDATE_DLH UPDATE_REG(UART_DLH_OFFSET)
#define UPDATE_IER UPDATE_REG(UART_IER_OFFSET)
#define UPDATE_IIR UPDATE_REG(UART_IIR_OFFSET)
#define UPDATE_FCR UPDATE_REG(UART_FCR_OFFSET)
#define UPDATE_LCR UPDATE_REG(UART_LCR_OFFSET)
#define UPDATE_MICR UPDATE_REG(UART_MICR_OFFSET)
#define UPDATE_LSR UPDATE_REG(UART_LSR_OFFSET)
#define UPDATE_MSR UPDATE_REG(UART_MSR_OFFSET)
#define UPDATE_USR UPDATE_REG(UART_USR_OFFSET)
#define CHECK_SUM(data_ptr, data_size) ({\
unsigned long checksum,data_offset;\
unsigned char *data_array=(unsigned char *) data_ptr;\
for(data_offset = 0, checksum = 0; data_offset < data_size; data_offset++)\
{\
checksum += data_array[data_offset]; \
}\
checksum; })
extern FUNCT_PTR_V_V Load_funVV_To_Dram(FUNCT_PTR_V_V, const u16);
extern void Flash_Update_Function(void);
extern void ALIGNED(4) EC_IO_Update(void);
extern void ALIGNED(4) EC_SMS_Update(void);
extern void Mailbox_4KSMS_UPDATE(BYTE mode, DWORD fw_size, DWORD start_addr);
extern void Mailbox_Update_Function(BYTE mode, DWORD fwsize, DWORD update_addr);
extern void ALIGNED(4) EC_SinglePage_Update(void);
extern void Cryp_Update_Function(void);
char ExtFlash_Update_Function(void);
#define WRITE_READ_CHECK 0
#endif