/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2023-12-29 19:18:09
 * @Description: Read temperature support
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
#include "CUSTOM_TEMPERATURE.H"
#include "AE_FUNC.H"

char get_temperature(WORD i2c_channel)
{
    if (i2c_channel == 0)
    {
#if !(SMBUS0_CLOCK_EN)
        dprint("SMBUS0 CLOCK NOT ENABLE\n");
        return 0;
#endif
    }
    else if (i2c_channel == 1)
    {
#if !(SMBUS1_CLOCK_EN)
        dprint("SMBUS1 CLOCK NOT ENABLE\n");
        return 0;
#endif
    }
    else if (i2c_channel == 2)
    {
#if !(SMBUS2_CLOCK_EN)
        dprint("SMBUS2 CLOCK NOT ENABLE\n");
        return 0;
#endif
    }
    else if (i2c_channel == 3)
    {
#if !(SMBUS3_CLOCK_EN)
        dprint("SMBUS3 CLOCK NOT ENABLE\n");
        return 0;
#endif
    }
    /* Channel 1 of I2C is used to get the temperature */
    char temperature[3];
    /* read temperaturn only when Debugger not working and chip set as Master*/
    if (Slave_flag == 0)
    {
        /* read temperature */
        temperature[0] = I2c_Master_Read_Byte(0x0, i2c_channel);
        temperature[1] = I2c_Master_Read_Byte(0x1, i2c_channel);
        temperature[2] = I2c_Master_Read_Byte(0x10, i2c_channel);
        /* Write temp into ECSPACE_BASE_ADDR */
        // CPU_TEMP = temperature[0];
        SYSTEM_TEMP = temperature[1];
        dprint("I2C%d temperature=%d temp[1]=%d.%d\n", i2c_channel, temperature[0], temperature[1], ((((BYTE)temperature[2]) >> 5) & 0x7) * 125);
    }
    return temperature[1];
}
