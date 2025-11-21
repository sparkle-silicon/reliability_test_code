/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:21:31
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
#ifndef CUSTOM_PMC4_H
#define CUSTOM_PMC4_H
#include "AE_GLOBAL.H"

/*PMC4 Register bit definition*/
#define OBF4            BIT(0)
#define IBF4            BIT(1)
#define DBG_En4         BIT(2)
#define C_D4            BIT(3)
/*--------------------------------------------------------------------------*/
extern void Service_PCI5(void);
#endif
