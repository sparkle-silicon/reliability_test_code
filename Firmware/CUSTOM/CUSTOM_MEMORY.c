/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:24:10
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
#include "CUSTOM_MEMORY.H"
/*****************************************************************************/
// FUNCTION CALL AREA
/*****************************************************************************/
void Clear_RAM_Range(volatile unsigned long nAddr, WORD nSize)
{
    WORD i;
    Tmp_XPntr = (BYTEP)nAddr;
    for(i = 0; i <= nSize; i++)
    {
        *Tmp_XPntr = 0;
        Tmp_XPntr++;
    }
}
//-----------------------------------------------------------------------------
