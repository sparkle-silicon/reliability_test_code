#!/bin/bash
###
 # @Author: daweslinyu daowes.ly@qq.com
 # @Date: 2024-05-31 14:37:40
 # @LastEditors: daweslinyu daowes.ly@qq.com
 # @LastEditTime: 2024-10-08 10:57:59
 # @FilePath: /SPK32AE103/Firmware/make.sh
 # @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
### 
folder0="/home/Share"
folder1="/mnt/d"
folder2="/mnt/hgfs/Share"
folder3="/mnt/hgfs/share"
binname="./main.bin"
# clear
rm -rf ./*.bin ./*.tags #删除
cd Firmware
# make clean -j4
make compile -j4 DOWNLOAD=crypto #flash
cp ./ec_main.bin ../
cd ..
# sleep 2
cd crypto_sys
make clean -j4
make compile -j4 DOWNLOAD=iram0
cp ./encrypt_iram0.bin ../
# sleep 2
make clean -j4
make compile -j4 DOWNLOAD=rom
cp ./encrypt_rom.bin ../
cd ..
# sleep 2
cd ROM
# make clean -j4
make compile -j4 DOWNLOAD=rom
cp ./rom.bin ../
cd ..
# sleep 2

dd if=/dev/zero ibs=512k count=1 | tr "\000" "\377">main.bin
dd if=ec_main.bin  of=main.bin bs=1k seek=0   conv=notrunc
# 0x40000(0xC0000)
dd if=encrypt_iram0.bin  of=main.bin bs=1k seek=256  conv=notrunc
dd if=encrypt_rom.bin  of=main.bin bs=1k seek=288  conv=notrunc
# 0x60000(0xE0000)
dd if=rom.bin  of=main.bin bs=1k seek=384  conv=notrunc
#  dd if=./main.bin  of=main2.bin bs=512k seek=31  conv=notrunc #16M位置檢索
# cp ./main2.bin /mnt/hgfs/Share/
##sed 's/cc.*/& eeeee/g' ./ec_main.bin#全局查找cc**** 并替换成cc**** eeeee
if [ -d "$folder0" ] ; then #存在目录/home/Share
    if [ -f "$binname" ]; then cp "$binname" "$folder0";fi
elif [ -d "$folder1" ] ; then
    if [ -f "$binname" ]; then cp "$binname" "$folder1";fi
elif [ -d "$folder2" ] ; then
    if [ -f "$binname" ]; then cp "$binname" "$folder2";fi
elif [ -d "$folder3" ] ; then
    if [ -f "$binname" ]; then cp "$binname" "$folder3";fi
else
    echo "NOT COPY FIRMWARE FINAL PROJECT FILE TO SHARE"
fi
