#!/bin/sh
clear
make compile -j4
rm AE10X/libLinuxAE10X.a 
riscv-nuclei-elf-gcc-ar -crsv  ./AE10X/libLinuxAE10X.a ./AE10X/*.o
make clean -j4
make release -j4
#mv ../release/AE_INIT.c ../release/AE10X/