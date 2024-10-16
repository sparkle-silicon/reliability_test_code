#!/bin/bash
###
 # @Author: daweslinyu daowes.ly@qq.com
 # @Date: 2024-05-31 14:37:40
 # @LastEditors: daweslinyu daowes.ly@qq.com
 # @LastEditTime: 2024-06-27 15:09:13
 # @FilePath: /SPK32AE103/Firmware/make.sh
 # @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
### 
folder0="/home/Share"
folder1="/mnt/d"
folder2="/mnt/hgfs/Share"
folder3="/mnt/hgfs/share"
binname="./ec_main.bin"
hexname="./ec_main.hex"
s19name="./ec_main.s19"
# clear
make clean -j4
make compile -j4 DOWNLOAD=crypto
##sed 's/cc.*/& eeeee/g' ./ec_main.bin
if [ -d "$folder0" ] ; then #存在目录/home/Share
    if [ -f "$binname" ]; then cp "$binname" "$folder0";fi
    if [ -f "$hexname" ]; then cp "$hexname" "$folder0";fi
    if [ -f "$s19name" ]; then cp "$s19name" "$folder0";fi
elif [ -d "$folder1" ] ; then
    if [ -f "$binname" ]; then cp "$binname" "$folder1";fi
    if [ -f "$hexname" ]; then cp "$hexname" "$folder1";fi
    if [ -f "$s19name" ]; then cp "$s19name" "$folder1";fi
elif [ -d "$folder2" ] ; then
    if [ -f "$binname" ]; then cp "$binname" "$folder2";fi
    if [ -f "$hexname" ]; then cp "$hexname" "$folder2";fi
    if [ -f "$s19name" ]; then cp "$s19name" "$folder2";fi
elif [ -d "$folder3" ] ; then
    if [ -f "$binname" ]; then cp "$binname" "$folder3";fi
    if [ -f "$hexname" ]; then cp "$hexname" "$folder3";fi
    if [ -f "$s19name" ]; then cp "$s19name" "$folder3";fi
else
    echo "NOT COPY FIRMWARE FINAL PROJECT FILE TO SHARE"
fi
