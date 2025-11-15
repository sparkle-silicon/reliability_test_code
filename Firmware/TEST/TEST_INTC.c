/*
 * @Author: Maple
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2025-10-17 15:17:35
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
#include "TEST_INTC.H"
#if TEST
extern uint32_t *Lpc_Mon_Ptr;
void intr0_gpio_a0(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[32]++;
#endif
    if (GPIO0_INTSTATUS0 & (0x1 << 0)) // if int happened
    {
        // BRAM_EC_Read();
        // BRAM_EC_Write();
        GPIO0_EOI0 |= (0x1 << 0); // clear int
    }
}
void intr0_gpio_a1(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[33]++;
#endif
}
void intr0_gpio_a2(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[34]++;
#endif
}
void intr0_gpio_a3(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[35]++;
#endif
}
void intr0_gpio_a4(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[36]++;
#endif
}
void intr0_gpio_a5(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[37]++;
#endif
}
void intr0_gpio_a6(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[38]++;
#endif
}
void intr0_gpio_a7(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[39]++;
#endif
}
void intr0_gpio_a8(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[40]++;
#endif
}
void intr0_gpio_a9(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[41]++;
#endif
}
void intr0_gpio_a10(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[42]++;
#endif
}
void intr0_gpio_a11(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[43]++;
#endif
}
void intr0_gpio_a12(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[44]++;
#endif
}
void intr0_gpio_a13(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[45]++;
#endif
}
void intr0_gpio_a14(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[46]++;
#endif
}
void intr0_gpio_a15(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[47]++;
#endif
}
void intr0_gpio_a16(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[48]++;
#endif
}
void intr0_gpio_a17(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[49]++;
#endif
}
void intr0_gpio_a18(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[50]++;
#endif
}
void intr0_gpio_a19(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[51]++;
#endif
}
void intr0_gpio_a20(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[52]++;
#endif
}
void intr0_gpio_a21(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[53]++;
#endif
}
void intr0_gpio_a22(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[54]++;
#endif
}
void intr0_gpio_a23(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[55]++;
#endif
}
void intr0_gpio_a24(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[56]++;
#endif
}
void intr0_gpio_a25(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[57]++;
#endif
}
void intr0_gpio_a26(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[58]++;
#endif
}
void intr0_gpio_a27(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[59]++;
#endif
}
void intr0_gpio_a28(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[60]++;
#endif
}
void intr0_gpio_a29(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[61]++;
#endif
}
void intr0_gpio_a30(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[62]++;
#endif
}
void intr0_gpio_a31(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[63]++;
#endif
}
void intr0_gpio_b0(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[64]++;
#endif
}
void intr0_gpio_b1(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[65]++;
#endif
}
void intr0_gpio_b2(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[66]++;
#endif
}
void intr0_gpio_b3(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[67]++;
#endif
}
void intr0_gpio_b4(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[68]++;
#endif
}
void intr0_gpio_b5(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[69]++;
#endif
}
void intr0_gpio_b6(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[70]++;
#endif
}
void intr0_gpio_b7(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[71]++;
#endif
}
void intr0_gpio_b8(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[72]++;
#endif
}
void intr0_gpio_b9(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[73]++;
#endif
}
void intr0_gpio_b10(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[74]++;
#endif
}
void intr0_gpio_b11(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[75]++;
#endif
}
void intr0_gpio_b12(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[76]++;
#endif
}
void intr0_gpio_b13(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[77]++;
#endif
}
void intr0_gpio_b14(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[78]++;
#endif
}
void intr0_gpio_b15(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[79]++;
#endif
}
void intr0_gpio_b16(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[80]++;
#endif
}
void intr0_gpio_b17(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[81]++;
#endif
}
void intr0_gpio_b18(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[82]++;
#endif
}
void intr0_gpio_b19(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[83]++;
#endif
}
void intr0_gpio_b20(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[84]++;
#endif
}
void intr0_gpio_b21(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[85]++;
#endif
}
void intr0_gpio_b22(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[86]++;
#endif
}
void intr0_gpio_b23(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[87]++;
#endif
}
void intr0_gpio_b24(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[88]++;
#endif
}
void intr0_gpio_b25(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[89]++;
#endif
}
void intr0_gpio_b26(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[90]++;
#endif
}
#ifdef AE103
void intr0_gpio_b27(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[91]++;
#endif
}
void intr0_gpio_b28(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[92]++;
#endif
}
void intr0_gpio_b29(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[93]++;
#endif
}
void intr0_gpio_b30(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[94]++;
#endif
}
void intr0_gpio_b31(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[95]++;
#endif
}
#endif
void intr1_gpio_c0(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[91]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[96]++;
#endif
#endif
}
void intr1_gpio_c1(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[92]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[97]++;
#endif
#endif
}
void intr1_gpio_c2(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[93]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[98]++;
#endif
#endif
}
void intr1_gpio_c3(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[94]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[99]++;
#endif
#endif
}
void intr1_gpio_c4(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[95]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[100]++;
#endif
#endif
}
void intr1_gpio_c5(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[96]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[101]++;
#endif
#endif
}
void intr1_gpio_c6(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[97]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[102]++;
#endif
#endif
}
void intr1_gpio_c7(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[98]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[103]++;
#endif
#endif
}
void intr1_gpio_c8(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[99]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[104]++;
#endif
#endif
}
void intr1_gpio_c9(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[100]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[105]++;
#endif
#endif
}
void intr1_gpio_c10(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[101]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[106]++;
#endif
#endif
}
void intr1_gpio_c11(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[102]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[107]++;
#endif
#endif
}
void intr1_gpio_c12(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[103]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[108]++;
#endif
#endif
}
void intr1_gpio_c13(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[104]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[109]++;
#endif
#endif
}
#if (defined(AE103))
void intr1_gpio_c14(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[110]++;
#endif
}
void intr1_gpio_c15(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[111]++;
#endif
}
#endif
void intr1_gpio_e0(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[105]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[112]++;
#endif
#endif
}
void intr1_gpio_e1(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[106]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[113]++;
#endif
#endif
}
void intr1_gpio_e2(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[107]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[114]++;
#endif
#endif
}
void intr1_gpio_e3(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[108]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[115]++;
#endif
#endif
}
void intr1_gpio_e4(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[109]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[116]++;
#endif
#endif
}
void intr1_gpio_e5(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[110]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[117]++;
#endif
#endif
}
void intr1_gpio_e6(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[111]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[118]++;
#endif
#endif
}
void intr1_gpio_e7(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[112]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[119]++;
#endif
#endif
}
#if (defined(AE101) || defined(AE102))
void intr1_gpio_e8(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[113]++;
#endif
}
void intr1_gpio_e9(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[114]++;
#endif
}
void intr1_gpio_e10(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[115]++;
#endif
}
void intr1_gpio_e11(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[116]++;
#endif
}
void intr1_gpio_e12(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[117]++;
#endif
}
void intr1_gpio_e13(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[118]++;
#endif
}
void intr1_gpio_e14(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[119]++;
#endif
}
void intr1_gpio_e15(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[120]++;
#endif
}
void intr1_gpio_e16(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[121]++;
#endif
}
void intr1_gpio_e17(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[122]++;
#endif
}
void intr1_gpio_e18(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[123]++;
#endif
}
void intr1_gpio_e19(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[124]++;
#endif
}
void intr1_gpio_e20(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[125]++;
#endif
}
void intr1_gpio_e21(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[126]++;
#endif
}
void intr1_gpio_e22(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[127]++;
#endif
}
void intr1_gpio_e23(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[128]++;
#endif
}
#elif defined(AE103)
void intr1_sm2(void) // 24
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[120]++;
#endif
}
void intr1_sm3(void) // 25
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[121]++;
#endif
}
void intr1_sm4(void) // 26
{
    if ((SM4_STATUS & SM4_STATUS_DONE))
    {
        SM4_INT |= SM4_INT_INT;
    }
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[122]++;
#endif
}
void intr1_smbus4(void) // 27
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[123]++;
#endif
    WORD baseaddr = 0x3D00;
    DWORD ic_stat, int_stat;
    ic_stat = I2c_Readb(I2C_STATUS_OFFSET, baseaddr);
    int_stat = i2c_dw_read_clear_intrbits(baseaddr); // clear interrupts
    dprint("int stat is %d \n", int_stat);
    if (int_stat & I2C_INTR_TX_ABRT)
    {
        dprint("iic send erro !!! \n");
        i2c_dw_tx_abrt(baseaddr);
    }
    if (int_stat & I2C_INTR_RX_FULL) // start req judge (set flags)
    {
    }
    if (int_stat & I2C_INTR_TX_EMPTY) // sFIFO almost empty
    {
    }
    if (int_stat & I2C_INTR_RD_REQ) // read request only once
    {
    }
    if (!(ic_stat & I2C_SLV_ACTIVITY))
    {
        // dprint("the iic of slave is dead! \n");
        // I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
    }
    if (!(ic_stat & I2C_MST_ACTIVITY))
    {
        // dprint("the iic of master is dead! \n");
        // I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
    }
}
void intr1_smbus5(void) // 28
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[124]++;
#endif
    WORD baseaddr = 0x3E00;
    DWORD ic_stat, int_stat;
    ic_stat = I2c_Readb(I2C_STATUS_OFFSET, baseaddr);
    int_stat = i2c_dw_read_clear_intrbits(baseaddr); // clear interrupts
    if (int_stat & I2C_INTR_TX_ABRT)
    {
        dprint("iic send erro !!! \n");
        i2c_dw_tx_abrt(baseaddr);
    }
    if (int_stat & I2C_INTR_RX_FULL) // start req judge (set flags)
    {
    }
    if (int_stat & I2C_INTR_TX_EMPTY) // sFIFO almost empty
    {
    }
    if (int_stat & I2C_INTR_RD_REQ) // read request only once
    {
    }
    if (!(ic_stat & I2C_SLV_ACTIVITY))
    {
        // dprint("the iic of slave is dead! \n");
        // I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
    }
    if (!(ic_stat & I2C_MST_ACTIVITY))
    {
        // dprint("the iic of master is dead! \n");
        // I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
    }
}
void intr1_null29(void) // 29
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[125]++;
#endif
}
void intr1_cec(void) // 30
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[126]++;
#endif
    DWORD stat, int_stat;
    stat = CEC_Read(CEC_SR_OFFSET);
    if ((stat & 0xF0) != 0)
    {
        dprint("CEC statu : \n", stat);
        dprint("statu erro ! \n");
        CEC_Write(0, CEC_CTRL_OFFSET); // disable the CEC
    }
    int_stat = CEC_Read(CEC_ISR_OFFSET);
#if CEC0_MODE_SWITCH // initiator
    if (int_stat & CEC_intStatue_sbis)
    {
    }
    if (int_stat & CEC_intStatue_sfis)
    {
    }
#else               // follower
    if (int_stat & CEC_intStatue_rhis)
    {
    }
    if (int_stat & CEC_intStatue_fbis)
    {
    }
    if (int_stat & CEC_intStatue_rfis)
    {
    }
#endif
    CEC_Write(int_stat, CEC_ISR_OFFSET); // clear the CEC_INT
}
void intr1_peci(void) // 31
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[127]++;
#endif
}
void intr1_null32(void) // 32
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[128]++;
#endif
    irqprint("null 32\n");
}
void intr1_null33(void) // 33
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[129]++;
#endif
    irqprint("null 33\n");
}
void intr1_null34(void) // 34
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[130]++;
#endif
    irqprint("null 34\n");
}
void intr1_null35(void) // 35
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[131]++;
#endif
    irqprint("null 35\n");
}
void intr1_null36(void) // 36
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[132]++;
#endif
    irqprint("null 36\n");
}
void intr1_null37(void) // 37
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[133]++;
#endif
    irqprint("null 37\n");
}
void intr1_pmc2_ibf_ec(void) // 38
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[134]++;
#endif
    PMC2_CTL = 0x0;
    F_Service_PCI3 = 1;
}
void intr1_pmc2_obe_ec(void) // 39
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[135]++;
#endif
}
#endif
void intr1_pmc3_ibf_ec(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[129]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[136]++;
#endif
#endif
    PMC3_CTL = 0x0;
    F_Service_PCI4 = 1;
}
void intr1_pmc3_obe_ec(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[130]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[137]++;
#endif
#endif
}
void intr1_pmc4_ibf_ec(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[131]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[138]++;
#endif
#endif
    PMC4_CTL = 0x0;
    F_Service_PCI5 = 1;
}
void intr1_pmc4_obe_ec(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[132]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[139]++;
#endif
#endif
}
void intr1_pmc5_ibf_ec(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[133]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[140]++;
#endif
#endif
    PMC5_CTL = 0x0;
    F_Service_PCI6 = 1;
}
void intr1_pmc5_obe_ec(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[134]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[141]++;
#endif
#endif
}
void intr1_ps2_1(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[135]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[142]++;
#endif
#endif
    if (MS_Main_CHN == 2)
        Handle_Mouse(MS_Main_CHN - 1);
    else if (KB_Main_CHN == 2)
        Handle_Kbd(KB_Main_CHN - 1);
}
#if defined(AE101)
void intr1_null45(void) // 45 47
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[136]++;
#endif
    irqprint("null 45\n");
}
void intr1_null46(void) // 46 48
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[137]++;
#endif
    irqprint("null 46\n");
}
void intr1_null47(void) // 47 49
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[138]++;
#endif
    irqprint("null 47\n");
}
#elif (defined(AE102) || defined(AE103))
void intr1_uart1(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[136]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[143]++;
#endif
#endif
#if ENABLE_DEBUGGER_SUPPORT
    Debugger_Cmd_IRQ(DEBUGGER_UART_REG(UART_RBR_OFFSET));
#endif
}
void intr1_uart2(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[137]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[144]++;
#endif
#endif
#if ENABLE_DEBUGGER_SUPPORT
    Debugger_Cmd_IRQ(DEBUGGER_UART_REG(UART_RBR_OFFSET));
#endif
}
void intr1_uart3(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[138]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[145]++;
#endif
#endif
#if ENABLE_DEBUGGER_SUPPORT
    Debugger_Cmd_IRQ(DEBUGGER_UART_REG(UART_RBR_OFFSET));
#endif
}
#endif
void intr1_spim(void)
{
    uint8_t data = 0;
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[139]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[146]++;
#endif
#endif
    if ((SPIM_ISR & 0X10)) // fifo接收满中断
    {
        if (SPI_Read_Start == 2) // 读命令完成准备读出
        {
            while (!(SPIM_SR & 0x4))
                ;
            data = SPIM_DA0;
            if (data == 0xff)
            {
                // dprint("data is %#x \n",data);
            }
            else
            {
                dprint("data is %#x \n", data);
                SPIM_DA0 = 0xff;
            }
        }
    }
    if (SPIM_ISR & 0X8) // fifo接收溢出中断
    {
        SPIM_ISR |= 0x8; // 清除中断
    }
    if (SPIM_ISR & 0x2) // 发送fifo满中断
    {
    }
    if ((SPIM_ISR & 0x1) && (!(SPIM_IMSR & 0x1))) // 发送fifo空中断
    {
        SPIM_Erase_FLASH_Data(0x0, 0);                     // 擦除
        SPIM_Write_FLASH_Data(0, 256, SPI_Write_Buff, 0); // 写入
        SPIM_Inturrupt_Mask(SPIM_IMSR_TXEIM, DISABLE);
        SPI_Read_Start = 1; // 写入完成判断可以读
    }
}
void intr1_smbus0(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[140]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[147]++;
#endif
#endif
    DWORD int_stat, ic_stat;
    WORD baseaddr = 0x3800;
    // BYTE mask = I2c_Readb(I2C_INTR_MASK_OFFSET,baseaddr);
    // BYTE raw_int = I2c_Readb(I2C_RAW_INTR_STAT_OFFSET,baseaddr);
    ic_stat = I2c_Readb(I2C_STATUS_OFFSET, baseaddr);
    int_stat = i2c_dw_read_clear_intrbits(baseaddr); // clear interrupts
    // dprint("int stat is %d \n",int_stat);
    // dprint("int raw_stat is %d \n",raw_int);
    // dprint("int mask is %d \n",mask);
    if (int_stat & I2C_INTR_TX_ABRT)
    {
        dprint("iic send erro !!! \n");
        i2c_dw_tx_abrt(baseaddr);
    }
    if (int_stat & I2C_INTR_RX_FULL) // start req judge (set flags)
    {
    #if ENABLE_DEBUGGER_SUPPORT
        Debugger_Cmd_IRQ(0);
    #endif
    }
    if (int_stat & I2C_INTR_TX_EMPTY) // sFIFO almost empty
    {
    #if ENABLE_DEBUGGER_SUPPORT
        Debugger_I2c_Send(0);
    #endif
    }
    if (int_stat & I2C_INTR_RD_REQ) // read request only once
    {
    #if ENABLE_DEBUGGER_SUPPORT
        Debugger_I2c_Req(0);
    #endif
    }
    if (!(ic_stat & I2C_SLV_ACTIVITY))
    {
        // dprint("the iic of slave is dead! \n");
        // I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
    }
    if (!(ic_stat & I2C_MST_ACTIVITY))
    {
        // dprint("the iic of master is dead! \n");
        // I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
    }
}
void intr1_smbus1(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[141]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[148]++;
#endif
#endif
    WORD baseaddr = 0x3900;
    DWORD ic_stat, int_stat;
    ic_stat = I2c_Readb(I2C_STATUS_OFFSET, baseaddr);
    int_stat = i2c_dw_read_clear_intrbits(baseaddr); // clear interrupts
    if (int_stat & I2C_INTR_TX_ABRT)
    {
        dprint("iic action erro !!! \n");
        i2c_dw_tx_abrt(baseaddr);
        I2c_Writeb(0, I2C_INTR_MASK_OFFSET, baseaddr);
    }
    if (int_stat & I2C_INTR_RX_FULL)
    {
    }
    if (int_stat & I2C_INTR_TX_EMPTY) // sFIFO almost empty
    {
    }
    if (int_stat & I2C_INTR_RD_REQ) // read request only once
    {
    }
    if (!(ic_stat & I2C_SLV_ACTIVITY))
    {
        // dprint("the iic of slave is dead! \n");
        // I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
    }
    if (!(ic_stat & I2C_MST_ACTIVITY))
    {
        // dprint("the iic of master is dead! \n");
        // I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
    }
}
void intr1_smbus2(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[142]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[149]++;
#endif
#endif
    WORD baseaddr = 0x3a00;
    DWORD ic_stat, int_stat;
    ic_stat = I2c_Readb(I2C_STATUS_OFFSET, baseaddr);
    int_stat = i2c_dw_read_clear_intrbits(baseaddr); // clear interrupts
    if (int_stat & I2C_INTR_TX_ABRT)
    {
        dprint("iic action erro !!! \n");
        i2c_dw_tx_abrt(baseaddr);
    }
    if (int_stat & I2C_INTR_RX_FULL)
    {
    }
    if (int_stat & I2C_INTR_TX_EMPTY) // sFIFO almost empty
    {
    }
    if (int_stat & I2C_INTR_RD_REQ) // read request only once
    {
    }
    if (!(ic_stat & I2C_SLV_ACTIVITY))
    {
        // dprint("the iic of slave is dead! \n");
        // I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
    }
    if (!(ic_stat & I2C_MST_ACTIVITY))
    {
        // dprint("the iic of master is dead! \n");
        // I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
    }
}
void intr1_smbus3(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[143]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[150]++;
#endif
#endif
    WORD baseaddr = 0x3b00;
    DWORD ic_stat, int_stat;
    ic_stat = I2c_Readb(I2C_STATUS_OFFSET, baseaddr);
    int_stat = i2c_dw_read_clear_intrbits(baseaddr); // clear interrupts
    if (int_stat & I2C_INTR_TX_ABRT)
    {
        dprint("iic action erro !!! \n");
        i2c_dw_tx_abrt(baseaddr);
    }
    if (int_stat & I2C_INTR_RX_FULL)
    {
    }
    if (int_stat & I2C_INTR_TX_EMPTY) // sFIFO almost empty
    {
    }
    if (int_stat & I2C_INTR_RD_REQ) // read request only once
    {
    }
    if (!(ic_stat & I2C_SLV_ACTIVITY))
    {
        // dprint("the iic of slave is dead! \n");
        // I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
    }
    if (!(ic_stat & I2C_MST_ACTIVITY))
    {
        // dprint("the iic of master is dead! \n");
        // I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
    }
}
#if defined(AE101)
void intr1_null53(void) // 53 55
{}
void intr1_null54(void) // 54 56
{}
void intr1_null55(void) // 55 57
{}
void intr1_null56(void) // 56 58
{}
void intr1_null57(void) // 57 59
{}
#elif (defined(AE102) || defined(AE103))
void intr1_can0(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[144]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[151]++;
#endif
#endif
#if CAN0_Receive
    if (CAN0_IR & 0x1)
    {
        dprint("RMC:%d\n", CAN0_RMC);
        CAN0_ReceiveData(CAN0_ReadBuff);
        CAN0_ReadFlag = 1;
        CAN0_CMR = (0x04); // 释放接收fifo
    }
#endif
}
void intr1_can1(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[145]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[152]++;
#endif
#endif
#if CAN1_Receive
    if (CAN1_IR & 0x1)
    {
        dprint("RMC:%d\n", CAN1_RMC);
        CAN1_ReceiveData(CAN1_ReadBuff);
        CAN1_ReadFlag = 1;
        CAN1_CMR = (0x04); // 释放接收fifo
    }
#endif
}
void intr1_can2(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[146]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[153]++;
#endif
#endif
#if CAN2_Receive
    if (CAN2_IR & 0x1)
    {
        dprint("RMC:%d\n", CAN2_RMC);
        CAN2_ReceiveData(CAN2_ReadBuff);
        CAN2_ReadFlag = 1;
        CAN2_CMR = (0x04); // 释放接收fifo
    }
#endif
}
void intr1_can3(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[147]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[154]++;
#endif
#endif
#if CAN3_Receive
    if (CAN3_IR & 0x1)
    {
        dprint("RMC:%d\n", CAN3_RMC);
        CAN3_ReceiveData(CAN3_ReadBuff);
        CAN3_ReadFlag = 1;
        CAN3_CMR = (0x04); // 释放接收fifo
    }
#endif
}
void intr1_pport(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[148]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[155]++;
#endif
#endif
}
#endif
void intr1_por_int(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[149]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[156]++;
#endif
#endif
    printf("SYSCTL_PORINT的值:%#x\n", SYSCTL_PORINT);
    SYSCTL_PORINT |= (0x1 << 2);
}
void intr1_lpc_mon(void)
{
#if (defined(AE101) || defined(AE102))
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[150]++;
#endif
#endif
#ifdef AE103
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[157]++;
#endif
#endif
    //*((volatile uint8_t *)(0x30810)) = 0x1f;
    *(Lpc_Mon_Ptr + LPC_MON_CNT) = *((volatile uint32_t *)(0x30814)); // when data full region 4byte
    dprint("LPC_MON Data is 0x%lx\n", *(Lpc_Mon_Ptr + LPC_MON_CNT));
    // dprint("INTR_RAW is 0x%lx\n", INTR_RAW);
    if ((*(Lpc_Mon_Ptr + LPC_MON_CNT)) & (0x40404040))
    {
        LPC_SOF = 1;
    }
    if ((*(Lpc_Mon_Ptr + LPC_MON_CNT)) & (0x20202020))
    {
        LPC_EOF = 1;
        // dprint("LPC_EOF=%d\n",LPC_EOF);
    }
    LPC_Monitor();
    LPC_MON_CNT++;
    if (LPC_MON_CNT > 100)
    {
        LPC_MON_CNT = 0;
    }
}
#ifdef AE103
void intr1_espi(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[158]++;
#endif
}
void intr1_TRNG(void)
{
#if ENABLE_DEBUGGER_SUPPORT
    Intr_num[159]++;
#endif
}
#endif
const FUNCT_PTR_V_V intr0_service[] =
{
    intr0_gpio_a0,  // 0
    intr0_gpio_a1,  // 1
    intr0_gpio_a2,  // 2
    intr0_gpio_a3,  // 3
    intr0_gpio_a4,  // 4
    intr0_gpio_a5,  // 5
    intr0_gpio_a6,  // 6
    intr0_gpio_a7,  // 7
    intr0_gpio_a8,  // 8
    intr0_gpio_a9,  // 9
    intr0_gpio_a10, // 10
    intr0_gpio_a11, // 11
    intr0_gpio_a12, // 12
    intr0_gpio_a13, // 13
    intr0_gpio_a14, // 14
    intr0_gpio_a15, // 15
    intr0_gpio_a16, // 16
    intr0_gpio_a17, // 17
    intr0_gpio_a18, // 18
    intr0_gpio_a19, // 19
    intr0_gpio_a20, // 20
    intr0_gpio_a21, // 21
    intr0_gpio_a22, // 22
    intr0_gpio_a23, // 23
    intr0_gpio_a24, // 24
    intr0_gpio_a25, // 25
    intr0_gpio_a26, // 26
    intr0_gpio_a27, // 27
    intr0_gpio_a28, // 28
    intr0_gpio_a29, // 29
    intr0_gpio_a30, // 30
    intr0_gpio_a31, // 31
    intr0_gpio_b0,  // 32
    intr0_gpio_b1,  // 33
    intr0_gpio_b2,  // 34
    intr0_gpio_b3,  // 35
    intr0_gpio_b4,  // 36
    intr0_gpio_b5,  // 37
    intr0_gpio_b6,  // 38
    intr0_gpio_b7,  // 39
    intr0_gpio_b8,  // 40
    intr0_gpio_b9,  // 41
    intr0_gpio_b10, // 42
    intr0_gpio_b11, // 43
    intr0_gpio_b12, // 44
    intr0_gpio_b13, // 45
    intr0_gpio_b14, // 46
    intr0_gpio_b15, // 47
    intr0_gpio_b16, // 48
    intr0_gpio_b17, // 49
    intr0_gpio_b18, // 50
    intr0_gpio_b19, // 51
    intr0_gpio_b20, // 52
    intr0_gpio_b21, // 53
    intr0_gpio_b22, // 54
    intr0_gpio_b23, // 55
    intr0_gpio_b24, // 56
    intr0_gpio_b25, // 57
    intr0_gpio_b26, // 58
#if (defined(AE103))
        intr0_gpio_b27, // 59
        intr0_gpio_b28, // 60
        intr0_gpio_b29, // 61
        intr0_gpio_b30, // 62
        intr0_gpio_b31, // 63
#endif
};
const FUNCT_PTR_V_V intr1_service[] =
{
    //                102 103
    intr1_gpio_c0,  // 0
    intr1_gpio_c1,  // 1
    intr1_gpio_c2,  // 2
    intr1_gpio_c3,  // 3
    intr1_gpio_c4,  // 4
    intr1_gpio_c5,  // 5
    intr1_gpio_c6,  // 6
    intr1_gpio_c7,  // 7
    intr1_gpio_c8,  // 8
    intr1_gpio_c9,  // 9
    intr1_gpio_c10, // 10
    intr1_gpio_c11, // 11
    intr1_gpio_c12, // 12
    intr1_gpio_c13, // 13
#if (defined(AE103))
        intr1_gpio_c14, //     14
        intr1_gpio_c15, //     15
#endif
#if (defined(AE101) || defined(AE102))
        intr1_gpio_e0,  // 14
        intr1_gpio_e1,  // 15
        intr1_gpio_e2,  // 16
        intr1_gpio_e3,  // 17
        intr1_gpio_e4,  // 18
        intr1_gpio_e5,  // 19
        intr1_gpio_e6,  // 20
        intr1_gpio_e7,  // 21
        intr1_gpio_e8,  // 22
        intr1_gpio_e9,  // 23
        intr1_gpio_e10, // 24
        intr1_gpio_e11, // 25
        intr1_gpio_e12, // 26
        intr1_gpio_e13, // 27
        intr1_gpio_e14, // 28
        intr1_gpio_e15, // 29
#endif
        intr1_gpio_e16, // 30  16
        intr1_gpio_e17, // 31  17
        intr1_gpio_e18, // 32  18
        intr1_gpio_e19, // 33  19
        intr1_gpio_e20, // 34  20
        intr1_gpio_e21, // 35  21
        intr1_gpio_e22, // 36  22
        intr1_gpio_e23, // 37  23
#if (defined(AE103))
        intr1_sm2,         //  24
        intr1_sm3,         //  25
        intr1_sm4,         //  26
        intr1_smbus4,      //  27
        intr1_smbus5,      //  28
        intr1_null29,         //  29
        intr1_cec,         //  30
        intr1_peci,        //  31
        intr1_null32,      //  32
        intr1_null33,      //  33
        intr1_null34,      //  34
        intr1_null35,      //  35
        intr1_null36,      //  36
        intr1_null37,      //  37
        intr1_pmc2_ibf_ec, //  38
        intr1_pmc2_obe_ec, //  39
#endif
        intr1_pmc3_ibf_ec, // 38
        intr1_pmc3_obe_ec,
        intr1_pmc4_ibf_ec,
        intr1_pmc4_obe_ec,
        intr1_pmc5_ibf_ec,
        intr1_pmc5_obe_ec,
        intr1_ps2_1, // 44
#if defined(AE101)
        intr1_null45, // 45 47
        intr1_null46, // 46 48
        intr1_null47, // 47 49
#elif (defined(AE102) || defined(AE103))
        intr1_uart1,
        intr1_uart2,
        intr1_uart3,
#endif
        intr1_spim,
        intr1_smbus0,
        intr1_smbus1,
        intr1_smbus2,
        intr1_smbus3,
#if defined(AE101)
        intr1_null53, // 53 55
        intr1_null54, // 54 56
        intr1_null55, // 55 57
        intr1_null56, // 56 58
        intr1_null57, // 57 59
#elif (defined(AE102) || defined(AE103))
        intr1_can0,
        intr1_can1,
        intr1_can2,
        intr1_can3,
        intr1_pport,
#endif
        intr1_por_int,
        intr1_lpc_mon,
#if (defined(AE103))
        intr1_espi, // 62
        intr1_TRNG, // 63
#endif
};
#endif