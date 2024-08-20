#!/bin/sh
###
 # @Author: daweslinyu daowes.ly@qq.com
 # @Date: 2023-09-04 23:51:55
 # @LastEditors: daweslinyu daowes.ly@qq.com
 # @LastEditTime: 2024-06-12 10:54:22
 # @FilePath: /work_git/Firmware/rom.sh
 # @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
### 
clear
make compile -j4
##sed 's/cc.*/& eeeee/g' ./ec_main.bin
gcc ./TOOLS/ROM/main.c -o rom.out
./rom.out
rm ./rom.out
cp ./rom_main.bin /home/Share
cp ./ec_main.bin /home/Share
cp ./ec_main.bin /mnt/hgfs/Share
cp ./ec_main.hex /home/Share
cp ./ec_main.s19 /home/Share