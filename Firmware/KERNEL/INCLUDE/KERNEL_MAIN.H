/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2025-10-24 11:12:22
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
#ifndef KERNEL_MAIN_H
#define KERNEL_MAIN_H

#include "AE_INCLUDE.H"
#include "KERNEL_INCLUDE.H"
#include "CUSTOM_INCLUDE.H"

 // #define HIGH_LEVEL_SERVICE_NUM_DIV  5//3:>90% 4:>95%,9:>98%
#define HIGH_LEVEL_SERVICE_NUM (3)/*((sizeof(service_table) / sizeof(FUNCT_PTR_V_V)) /HIGH_LEVEL_SERVICE_NUM_DIV)*/
// #define LOW_LEVEL_SERVICE_NUM_MUL  3//11/6:>90% 7/5:>95%,17/15:>98%
// #define LOW_LEVEL_SERVICE_NUM_DIV  2//
#define LOW_LEVEL_SERVICE_NUM (7)/*((sizeof(service_table) / sizeof(FUNCT_PTR_V_V))*LOW_LEVEL_SERVICE_NUM_MUL /LOW_LEVEL_SERVICE_NUM_DIV*/
//// #define MIDDLE_LEVEL_SERVICE_NUM_DIV  9//3:>90% 4:>95%,9:>98%
#define MIDDLE_LEVEL_SERVICE_NUM (4)/*((sizeof(service_table) / sizeof(FUNCT_PTR_V_V)) -HIGH_LEVEL_SERVICE_NUM - LOW_LEVEL_SERVICE_NUM)*//*((sizeof(service_table) / sizeof(FUNCT_PTR_V_V)) /MIDDLE_LEVEL_SERVICE_NUM_DIV)*/

/*SUPPORT_MAIN_SERVICE_LEVEL_POLL: 主程序轮询函数下的副功能,一般关闭即可,除非时序要求非常高,否则不建议开启,会影响正常轮询的执行效率
如果开启,则会穿插执行列表轮询:
    不同优先级下,越靠前优先级执行越频繁执行顺序:(高优先级)->(中优先级->高优先级)->(低优先级->高优先级->中优先级),
    一轮下来,相当于至少执行3次高优先级,两次中优先级,一次低优先级,如果比例更大,其执行效果会提高,相对开启后平均性能效率会降低
    同优先级下,越靠前优先级越容易执行多一轮次,但至少保证一轮次完整执行:同优先级0->同优先级1->...->同优先级(x-1)->...(重头开始循环,继续0->x)->优先级n(x为同优先级一轮总数,n为列表总数%该优先级一轮总数的值,n!=x)

    例如16个service,3个高优先级,5个中优先级,剩余低优先级
    相当于一次轮询会执行5.3轮高优先级,2.6轮中优先级,1轮低优先级,
    相当于高优先级第一个函数会多执行一遍,中优先级前三个函数会多执行一次,低优先级每个函数会执行1次,
    一轮次总共会执行16个高优先级+13个中优先级+8低优先级=37次,发改为原先的37/16=2.4轮,即原先会执行9个高优先级,12个中优先级,16个低优先级
    其一轮原轮询次数16*2 = 32,实际一轮轮询数量相当于32原数据+16一级增加轮询次数+13个一级轮询函数+ 13个二级轮询次数+8个二级轮询函数=82次,约为2.56倍
    其执行总效率相近,倍率相等.但高优先级增加7个轮询个数,中优先级减少1个轮询个数,低优先级增加1个轮询个数,其高优先级执行效率提升明显,

    设总数n,高优先级x,中优先级y,低优先级z,则抽象优先级计算量为:
        一轮执行为n/x个高优先级,(n-x)/y个中优先级,1个低优先级
        相当于高优先级前n%x个函数会多执行1遍,中优先级前(n-x)%y个函数会多执行1次,低优先级每个函数会执行1次,
        一轮次总共会执行n个高优先级+(n-x)个中优先级+z低优先级=2n+z-x=3n-2x-y次,即为原先的2+((z-x)/n) = 3-((2x+y)/n)轮,相当于执行1-3轮的量
        其一轮原轮询次数2n,实际一轮轮询数量相当于2n+n+(n-x)+ (n-x)+(n-x-y)=6n-3x-y=6n-(3x+y)=5n+(z-2x)次,约2.5+((z-2x)/2n) =3-((3x+y)/2n)倍,约1.5-3倍的次数
        理论上效率降低为2/3<=(3-((2x+y)/n))/(3-((3x+y)/2n))/ <=1,但一般0<x<n/2且0<=y<n/2,x+y<n,则实际效率为3/4-1之间,
        推荐  (1<=x <= n/5), x <= y <= n/3,z>=n/2其效率为原先89.5%-(3-3/n)/(3-2/n)之间,
        (其他为1,只改变一个值估算:90%效率:x<=46.14%n,y<=54.54%n,z>=45.46%n,x+y+z = n,95%:效率:x<=26.08%n,y<=28.57%n,z>=71.43%n,98%:效率:x<=11.32%n,y<=11.76%n,z>=88.24%n)
    其高优先级访问效率明显提高(其数量越小,效率提升越明显),中优先级访问效率和原来相近(其数量越小,效率提升越明显),其低优先级(其数量越小,越贴近原效率,同时整体效率越低)效率明显降低,
    */
#define SUPPORT_MAIN_SERVICE_LEVEL_POLL 0
    //-----------------------------------------------------------------------------
#endif
