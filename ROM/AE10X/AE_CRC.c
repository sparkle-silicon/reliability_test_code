/*
 * @Author: daweslinyu daowes.ly@qq.com
 * @Date: 2024-08-09 15:16:54
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-14 15:47:22
 * @FilePath: /ROM/AE10X/AE_CRC.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include<AE_CRC.H>
uint32_t CRC32(const void *bytes, size_t len)
{
//byte：读入的1字节
    const uint8_t *byte = bytes;
    //init
    uint32_t crc = CRC_DEFAULT_INIT;
    for(size_t i = 0; i < len; ++i)
    {
        crc ^= byte[i];
        for(int j = 0; j < 8; j++)
        {
            if(crc & 1)
            {
                crc = (crc >> 1) ^ CRC32_REFPOLY;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return ~crc;
}
