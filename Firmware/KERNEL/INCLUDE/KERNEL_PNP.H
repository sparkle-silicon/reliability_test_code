/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2023-09-20 18:23:10
 * @Description: This file is used for setting logical device relevant configuration
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
#ifndef KERNEL_PNP_H
#define KERNEL_PNP_H
#include "AE_GLOBAL.H"

#define PNP_Timeout 5000
/*-----------------------------------------------------------------------------
 * PNP definition
 *---------------------------------------------------------------------------*/
#define E2PNP_RDY       0x1
#define E2PNP_REQ       0x2
#define EFIR_2E         0x2e000000
#define EFDR_2F         0x002f0000
#define E2PNP_ReadREQ   0x00000200 
#define E2PNP_WriteREQ  0x00000100
#define E2PNP_RC        0x00000800 //读完成
#define E2PNP_WC        0x00000400 //写完成
#define PNPCNST_SETEN   BIT(27)
/*-----------------------------------------------------------------------------
 * PNP Register Address definition
 *---------------------------------------------------------------------------*/
#define PNP_CHIPID_VER_OFFSET HEX04
#define PNP_LD_EN_OFFSET HEX08
#define PNP_CONFIG_OFFSET HEX0C
#define PNP_CTRL0_OFFSET HEX10
#define PNP_CTRL1_OFFSET HEX14
#define PNP_REG_ADDR(offset) REG_ADDR(SYSCTL_BASE_ADDR, offset)
#define PNP_CHIPID_VER_ADDR PNP_REG_ADDR(PNP_CHIPID_VER_OFFSET)
#define PNP_LD_EN_ADDR PNP_REG_ADDR(PNP_LD_EN_OFFSET)
#define PNP_CONFIG_ADDR PNP_REG_ADDR(PNP_CONFIG_OFFSET)
#define PNP_CTRL0_ADDR PNP_REG_ADDR(PNP_CTRL0_OFFSET)
#define PNP_CTRL1_ADDR PNP_REG_ADDR(PNP_CTRL1_OFFSET)
#define PNP_CHIPID_VER REG32(PNP_CHIPID_VER_ADDR)
#define PNP_LD_EN REG32(PNP_LD_EN_ADDR)
#define PNP_CONFIG REG32(PNP_CONFIG_ADDR)
#define PNP_CTRL0 REG32(PNP_CTRL0_ADDR)
#define PNP_CTRL1 REG32(PNP_CTRL1_ADDR)
/*-----------------------------------------------------------------------------
 * PNP Enable definition
 *---------------------------------------------------------------------------*/
#define HOST_UARTA_EN BIT0
#define HOST_UARTB_EN BIT1
#define HOST_UART0_EN BIT2
#define HOST_UART1_EN BIT3
#define HOST_UART2_EN BIT4
#define HOST_UART3_EN BIT5
#define HOST_UARTx_EN (HOST_UARTA_EN | HOST_UARTB_EN | HOST_UART0_EN | HOST_UART1_EN | HOST_UART2_EN | HOST_UART3_EN)
#define HOST_PMC1_EN BIT6
#define HOST_PMC2_EN BIT7
#define HOST_PMC3_EN BIT(8)
#define HOST_PMC4_EN BIT(9)
#define HOST_PMC5_EN BIT(10)
#define HOST_PMCx_EN (HOST_PMC1_EN | HOST_PMC2_EN | HOST_PMC3_EN | HOST_PMC4_EN | HOST_PMC5_EN)
#define HOST_WUC_EN BIT(12)
#define HOST_KBM_EN BIT(13)
#define HOST_KBC_EN BIT(14)
#define HOST_KB_WU_EN (HOST_WUC_EN | HOST_KBM_EN | HOST_KBC_EN)
#define HOST_KBC_PMC_EN (HOST_PMCx_EN | HOST_KB_WU_EN)
#define HOST_PP_EN BIT(16)
#define HOST_BRAM_EN BIT(18)
#define HOST_P80_EN BIT(21)
#define HOST_BRAM_P80_EN (HOST_BRAM_EN | HOST_P80_EN)
#define HOST_SMFI_EN BIT(22)
#define HOST_SSPI_EN BIT(24)
#define HOST_SMFI_SSPI_EN (HOST_SMFI_EN | HOST_SSPI_EN)
#define HOST_CAN0_EN BIT(26)
#define HOST_CAN1_EN BIT(27)
#define HOST_CAN2_EN BIT(28)
#define HOST_CAN3_EN BIT(29)
#define HOST_CANx_EN (HOST_CAN0_EN | HOST_CAN1_EN | HOST_CAN2_EN | HOST_CAN3_EN)
/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
extern void Config_PNP_Access_Request(void);
extern void Config_PNP_Access_Release(void);
extern void Config_PNP_Write(BYTE idx, BYTE ldn, BYTE data);
extern void Config_PNP_Read(BYTE idx, BYTE ldn);
extern void Clear_Logical_Device_Default_IRQ_Num(void);
extern void LogicalDevice_PNP_Config(void);
extern BYTE LD_PNP_Config(void);
extern void ShareMem_Config(void);
#endif
