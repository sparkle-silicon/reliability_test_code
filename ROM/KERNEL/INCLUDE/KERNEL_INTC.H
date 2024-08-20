/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-11-06 17:02:52
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
#ifndef _KERNEL_INTC_H
#define _KERNEL_INTC_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
/*--------------------------------------------------------------------------*/
/*--------------------INTC1 Register Address definition---------------------*/
/*--------------------------------------------------------------------------*/
/*IRQC relevant functions*/
extern void disable_intc(int index, int num);
extern void enable_intc(int index, int num);
extern void intc_init();
/*----------------------------------LINE--------------------------------*/
#define INTC_ICTL_INTEN_L 0x00       // IRQ interrupt source enable register (low)
#define INTC_ICTL_INTEN_H 0x04       // IRQ interrupt source enable register (high)
#define INTC_ICTL_INTMASK_L 0x08     // IRQ interrupt source mask register (low).
#define INTC_ICTL_INTMASK_H 0x0c     // IRQ interrupt source mask register (high).
#define INTC_ICTL_INTFORCE_L 0x14    // IRQ interrupt force register
#define INTC_ICTL_INTFORCE_H 0x14    //
#define INTC_ICTL_RAWSTATUS_L 0x18   // IRQ raw status register
#define INTC_ICTL_RAWSTATUS_H 0x1c   //
#define INTC_ICTL_STATUS_L 0x20      // IRQ status register
#define INTC_ICTL_STATUS_H 0x24      //
#define INTC_ICTL_MASKSTATUS_L 0x28  // IRQ interrupt mask status register
#define INTC_ICTL_MASKSTATUS_H 0x2c  //
#define INTC_ICTL_FINALSTATUS_L 0x30 // IRQ interrupt final status
#define INTC_ICTL_FINALSTATUS_H 0x34
#define FINTC_ICTL_INTEN 0xc0       // Fast interrupt enable register
#define FINTC_ICTL_INTMASK 0xc4     // Fast interrupt mask register
#define FINTC_ICTL_INTFORCE 0xc8    // Fast interrupt force register
#define FINTC_ICTL_RAWSTATUS 0xcc   // Fast interrupt source raw status register
#define FINTC_ICTL_STATUS 0xd0      // Fast interrupt status register
#define FINTC_ICTL_FINALSTATUS 0xd4 // Fast interrupt final status register
#define INTC_ICTL_PLEVEL 0xd8       // IRQ System Priority Level Register
#define intc_write(index, addr, val) *(VBYTEP)(INTC0_BASE_ADDR + (index * 0x400) + addr) = val
#define intc_read(index, addr) *(VBYTEP)(INTC0_BASE_ADDR + (index * 0x400) + addr)
#define set_ictl_reg(index, addr, val) intc_write(index, addr, ((val) | intc_read(index, addr)))
#define clear_ictl_reg(index, addr, val) intc_write(index, addr, (~(val) & intc_read(index, addr)))
#define Pport_Int_Enable (PP_CTL |= (0x1 << 4))
#define Pport_Int_Disable (PP_CTL &= ~(0x1 << 4))
#define Pport_Int_Enable_Read ((PP_CTL & (0x1 << 4)) != 0)
#define Pport_Int_Status ((PP_STA & (0x1 << 2)) != 0)
#define MON_INTR_MASK INTR_MASK
#define MON_INTR_STS INTR_STS
#define Mon_Int_Mask(type) (MON_INTR_MASK |= (0x1 << (type)))
#define Mon_Int_Unmask(type) (MON_INTR_MASK &= ~(0x1 << (type)))
#define Mon_Int_Mask_Read(type) ((MON_INTR_MASK & (0x1 << (type))) != 0)
#define Mon_Int_Status(type) ((MON_INTR_STS & (0x1 << (type))) != 0)
extern const FUNCT_PTR_V_V intr0_service[];
extern const FUNCT_PTR_V_V intr1_service[];
void Handle_Kbd(BYTE channel);
extern BYTE Int_Control0_Enable(BYTE int_number);
extern BYTE Int_Control1_Enable(BYTE int_num);
extern BYTE Int_Control0_Disable(BYTE int_num);
extern BYTE Int_Control1_Disable(BYTE int_num);
extern BYTE Int_Control0_Mask(BYTE int_num);
extern BYTE Int_Control1_Mask(BYTE int_num);
extern BYTE Int_Control0_Unmask(BYTE int_num);
extern BYTE Int_Control1_Unmask(BYTE int_num);
extern BYTE Int_Control0_Status(BYTE int_num);
extern BYTE Int_Control1_Status(BYTE int_num);
extern BYTE Int_Control0_Enable_Read(BYTE int_num);
extern BYTE Int_Control1_Enable_Read(BYTE int_num);
extern BYTE Int_Control0_Mask_Read(BYTE int_num);
extern BYTE Int_Control1_Mask_Read(BYTE int_num);
extern void CPU_Int_Enable(BYTE type);
extern void CPU_Int_Disable(BYTE type);
extern BYTE CPU_Int_Enable_Read(BYTE type);
extern void CPU_Int_Type_Edge(BYTE type);
extern void CPU_Int_Type_Level(BYTE type);
extern BYTE CPU_Int_Type_Read(BYTE type);
extern void intr1_uart1(void);
extern void intr1_uart2(void);
extern void intr1_uart3(void);
#endif
