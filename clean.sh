#!/bin/sh
###
 # @Author: daweslinyu daowes.ly@qq.com
 # @Date: 2024-07-19 15:14:28
 # @LastEditors: daweslinyu daowes.ly@qq.com
 # @LastEditTime: 2024-07-19 15:21:34
 # @FilePath: /SPK32AE103/clean.sh
 # @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
### 
clear
cd ROM
make clean -j4
cd ../Firmware
make clean -j4
rm -f $(find -name "*.tags")