#############################################################
# Configuration
#############################################################
#SHELL := $(shell which bash)
OS ?= $(shell uname)
ifeq ($(OS),Linux)
BUILD_DATE := $(shell date +'%y%m%d')#'%Y%m%d'
BUILD_TIME := $(shell date +'%H%M%S')
SHELL_TYPES := $(shell echo $$0)
else #Windows_NT
BUILD_DATE := $(shell powershell Get-Date -Format '%y%m%d')#'%Y%m%d'
BUILD_TIME := $(shell powershell Get-Date -Format '%H%M%S')
SHELL_TYPES := $(SHELL)
endif
CHIP?=GLE01 # x替换当前芯片型号
VERSION ?= 0.0.0# 定义版本号

PROGRAM ?= ec_main#project Object name
PROGRAM_FILE ?= $(PROGRAM)_$(BUILD_DATE)$(BUILD_TIME).elf



DUAL_BOOT_FLAG?=0#双启动开关
SAVE_TEMPS_FILE ?= 0#保留编译文件
USER_RISCV_LIBC_A?=0#0 USER_AE10X_LIBC_A 1 USER_RISCV_LIBC_A
#安全相关
PEM_FILE ?= $(TOPDIR)/TOOLS/SMx/private_key.pem
EFUSE_SECVER_FLAG?=1#efuse security open or close 
HASH_4kSECTORSIZE?=0#0:4k*HASH_4kSECTORSIZE sector size
EXTERNAL_FLASH_ADDR?=0x000000#外部flash地址
ifeq ($(findstring /sh,$(SHELL_TYPES)),/sh)
SHELL_TYPE := $(shell readlink -f $(SHELL_TYPES))
else 
SHELL_TYPE := $(SHELL_TYPES)
endif

ifeq ($(EFUSE_SECVER_FLAG),1)
EFUSE_SECVER_FLAG_STR := -efuse
else
EFUSE_SECVER_FLAG_STR := -no_efuse
endif



# default the program is downloaded to ilm
DOWNLOAD ?= flash
RISCV_ARCH?=rv32ec#按照RISC-V架构命名规则，以下指令子集的组合可表示为RV32EC
#RV32架构：riscv-32架构通用寄存器宽度32位
#E：支持16个通用寄存器
#C：支持编码长度为16位的压缩指令，提高代码密度
RISCV_ABI?=ilp32e#int long pointer 都是32bit,不使用硬件浮点数,支持rv32e
# Default target
# Pointers to various important tools in the toolchain.
RISCV_GCC     := riscv-nuclei-elf-gcc
RISCV_GXX     := riscv-nuclei-elf-g++
RISCV_OBJDUMP := riscv-nuclei-elf-objdump
RISCV_OBJCOPY := riscv-nuclei-elf-objcopy
RISCV_GDB     := riscv-nuclei-elf-gdb
RISCV_READELF := riscv-nuclei-elf-readelf
RISCV_AR      := riscv-nuclei-elf-ar
RISCV_SIZE    := riscv-nuclei-elf-size
DD			  := dd
#############################################################
# This Section is for Software Compilation
#############################################################
#############################################################
TOPDIR 			:= .
KERNEL_DIR 		= $(TOPDIR)/KERNEL
AE10X_DIR 		= $(TOPDIR)/AE10X
CUSTOM_DIR  	= $(TOPDIR)/CUSTOM
HARDWARE_DIR  	= $(TOPDIR)/HARDWARE
TEST_DIR  		= $(TOPDIR)/TEST
ENV_DIR 		= $(AE10X_DIR)/ENV
CINCLUDE 			+= -I $(KERNEL_DIR)/INCLUDE
CINCLUDE 			+= -I $(AE10X_DIR)/INCLUDE
CINCLUDE 			+= -I $(CUSTOM_DIR)/INCLUDE
CINCLUDE 			+= -I $(HARDWARE_DIR)/INCLUDE
CINCLUDE 			+= -I $(TEST_DIR)/INCLUDE
CFLAGS 				+= $(CINCLUDE)
export TOPDIR KERNEL_DIR AE10X_DIR CUSTOM_DIR HARDWARE_DIR TEST_DIR ENV_DIR CFLAGS
##############################################################
#编译
#############################################################
PROGRAM_ELF = $(TOPDIR)/$(PROGRAM_FILE)
PROGRAM_BOOT1 ?= $(PROGRAM)_1
PROGRAM_BOOT2 ?= $(PROGRAM)_2
PROGRAM_FILE_BOOT1 ?=$(PROGRAM_BOOT1)_$(BUILD_DATE)$(BUILD_TIME).elf
PROGRAM_FILE_BOOT2 ?= $(PROGRAM_BOOT2)_$(BUILD_DATE)$(BUILD_TIME).elf
PROGRAM_ELF_BOOT1 = $(TOPDIR)/$(PROGRAM_FILE_BOOT1)
PROGRAM_ELF_BOOT2 = $(TOPDIR)/$(PROGRAM_FILE_BOOT2)
DD_ELF_BOOT:= if=/dev/zero ibs=4k count=64 | tr "\000" "\377"
DD_ELF_BOOT1:= if=./$(PROGRAM_BOOT1).bin  of=$(PROGRAM).bin seek=0   conv=notrunc
DD_ELF_BOOT2:= if=./$(PROGRAM_BOOT2).bin  of=$(PROGRAM).bin seek=256 conv=notrunc

compile: software 
ifeq ($(OS),Linux) #Linux
#	 @gcc ./TOOLS/SMx/gle01_sign.c ./TOOLS/SMx/gle01_sign.h -D$(CHIP) -I/usr/include/openssl/ -L /usr/lib/x86_64-linux-gnu/ -lcrypto  -o gle01_sign #-D_DEBUG
#	 @cp ./gle01_sign ./TOOLS/SMx/
ifeq ($(findstring bash,$(SHELL_TYPE)),bash)
# 如果是bash
	@if [ $(CHIP) == GLE01 ];then cp  ./TOOLS/SMx/gle01_sign ./;./gle01_sign -f $(PROGRAM).bin -s 4096 -h $(HASH_4kSECTORSIZE) -sign_key $(PEM_FILE) -addr $(EXTERNAL_FLASH_ADDR)  $(EFUSE_SECVER_FLAG_STR);rm ./gle01_sign;fi;
else
ifeq ($(findstring dash,$(SHELL_TYPE)),dash)
# 如果是dash
	@if [ $(CHIP) = GLE01 ];then cp  ./TOOLS/SMx/gle01_sign ./;./gle01_sign -f $(PROGRAM).bin -s 4096 -h $(HASH_4kSECTORSIZE) -sign_key $(PEM_FILE) -addr $(EXTERNAL_FLASH_ADDR) $(EFUSE_SECVER_FLAG_STR);rm ./gle01_sign;fi;
else
# 默认情况
	@if [ $(CHIP) == GLE01 ];then cp  ./TOOLS/SMx/gle01_sign ./;./gle01_sign -f $(PROGRAM).bin -s 4096 -h $(HASH_4kSECTORSIZE) -sign_key $(PEM_FILE) -addr $(EXTERNAL_FLASH_ADDR) $(EFUSE_SECVER_FLAG_STR);rm ./gle01_sign;fi;
endif
endif
	@if [ -e SM3_hash.txt ];then mv SM3_hash.txt ./TOOLS/SMx/;fi;
	@if [ -e SM2_sig.txt ];then mv SM2_sig.txt ./TOOLS/SMx/;fi;
	@if [ -e SM2_user_key.txt ];then mv SM2_user_key.txt ./TOOLS/SMx/;fi;
else #Windows or other commands
endif
	@echo CHIP $(CHIP) CODE Compiled successfully.
.PHONY: software
software: copyright_notice

	@echo SHELL = $(SHELL_TYPE)
	@echo make -f $(TOPDIR)/common.mk
	@$(MAKE) -f $(TOPDIR)/common.mk --no-print-directory CHIP=$(CHIP) USER_RISCV_LIBC_A=$(USER_RISCV_LIBC_A)  DOWNLOAD=$(DOWNLOAD) SAVE_TEMPS_FILE=$(SAVE_TEMPS_FILE) DUAL_BOOT_FLAG=$(DUAL_BOOT_FLAG)  PROGRAM=$(PROGRAM) TARGET=$(PROGRAM_FILE)

ifeq ($(DUAL_BOOT_FLAG),0)
#riscv-nuclei-elf-objdump -m riscv:rv32 -D -b binary *.bin > *.dump
	@$(RISCV_OBJDUMP) -D $(PROGRAM_ELF) > $(PROGRAM).dump
	@echo OBJDUMP $(PROGRAM).dump
	@$(RISCV_OBJCOPY) $(PROGRAM_ELF) -O verilog $(PROGRAM).verilog
#	@$(RISCV_OBJCOPY) $(PROGRAM_ELF) -O verilog $(PROGRAM).vh
	@echo OBJCOPY $(PROGRAM).verilog
	@$(RISCV_OBJCOPY) -O binary $(PROGRAM_ELF) $(PROGRAM).bin
	@echo OBJCOPY $(PROGRAM).bin
	@$(RISCV_OBJCOPY) -I binary -O ihex $(PROGRAM).bin $(PROGRAM).hex
# 	@$(RISCV_OBJCOPY) -O ihex $(PROGRAM_ELF) $(PROGRAM)_0x80000.hex
	@echo OBJCOPY $(PROGRAM).hex
	@$(RISCV_OBJCOPY) -I binary -O srec $(PROGRAM).bin $(PROGRAM).s19
# 	@$(RISCV_OBJCOPY) -O srec $(PROGRAM_ELF) $(PROGRAM)_0x80000.s19
	@echo OBJCOPY $(PROGRAM).s19
	@$(RISCV_READELF)  -a $(PROGRAM_ELF) > $(PROGRAM).readelf
	@echo READELF $(PROGRAM).readelf
else ifeq ($(DUAL_BOOT_FLAG),1)#Dual start
	@$(RISCV_OBJDUMP) -D $(PROGRAM_ELF_BOOT1) > $(PROGRAM_BOOT1).dump
	@echo OBJDUMP $(PROGRAM_BOOT1).dump
	@$(RISCV_OBJCOPY) $(PROGRAM_ELF_BOOT1) -O verilog $(PROGRAM_BOOT1).verilog
	@echo OBJCOPY $(PROGRAM_BOOT1).verilog
	@$(RISCV_OBJCOPY) -O binary $(PROGRAM_ELF_BOOT1) $(PROGRAM_BOOT1).bin
	@echo OBJCOPY $(PROGRAM_BOOT1).bin
	@$(RISCV_OBJDUMP) -D $(PROGRAM_ELF_BOOT2) > $(PROGRAM_BOOT2).dump
	@echo OBJDUMP $(PROGRAM_BOOT2).dump
	@$(RISCV_OBJCOPY) $(PROGRAM_ELF_BOOT2) -O verilog $(PROGRAM_BOOT2).verilog
	@echo OBJCOPY $(PROGRAM_BOOT2).verilog
	@$(RISCV_OBJCOPY) -O binary $(PROGRAM_ELF_BOOT2) $(PROGRAM_BOOT2).bin
	@echo OBJCOPY $(PROGRAM_BOOT2).bin
	@$(DD) $(DD_ELF_BOOT)>$(PROGRAM).bin
	@$(DD) $(DD_ELF_BOOT1)
	@$(DD) $(DD_ELF_BOOT2)
endif
.PHONY: copyright_notice
copyright_notice:
	-git config --global core.ignorecase false
ifeq ($(OS),Linux) #Linux
	@clear
	@clear
	@echo "************************************************************************************"
	@echo "*                    Embedded Controller  SPK32 Series Firmware                    *"
	@echo "* Copyright ©2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved. *"
	@echo "************************************************************************************"
else #Windows or other commands
#-powershell -Command "Start-Process 'fsutil' -ArgumentList 'file setCaseSensitiveInfo . enable' -Verb RunAs" 
#-fsutil file setCaseSensitiveInfo $(TOPDIR) enable
	@cls
	@cls
	@echo ************************************************************************************
	@echo *                    Embedded Controller  SPK32 Series Firmware                    *
	@echo *Copyright (c)2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.*
	@echo ************************************************************************************
endif
release_dir := release_v$(VERSION)_$(BUILD_DATE)
.PHONY: release
release:
ifeq ($(OS),Linux) #Linux
	@cp ./ ../$(release_dir) -r
	@rm -f ../$(release_dir)/AE10X/*.c ../$(release_dir)/AE10X/*.S
	@rm -rf ../$(release_dir)/HARDWARE ../$(release_dir)/TEST
	@rm -rf ../$(release_dir)/$(release_dir).sh ../$(release_dir)/rom.sh
	@rm -rf ../$(release_dir)/TOOLS/AUTO ../$(release_dir)/TOOLS/drive ../$(release_dir)/TOOLS/ROM
	@rm -rf ../$(release_dir)/.v* ../$(release_dir)/.s*  ../$(release_dir)/.c* ../$(release_dir)/.g* ../$(release_dir)/.p* 
else #Windows or other commands
	@if not exist ..\$(release_dir) mkdir ..\$(release_dir)
	@xcopy **\* ..\$(release_dir) /s /e /i /y
	@del /q /f /s ..\$(release_dir)\AE10X\*.c
	@del /q /f /s ..\$(release_dir)\AE10X\*.S
	@del /q /f /s ..\$(release_dir)\HARDWARE
	@del /q /f /s ..\$(release_dir)\TEST
	@del /q /f /s ..\$(release_dir)\$(release_dir).sh
	@del /q /f /s ..\$(release_dir)\rom.sh
	@del /q /f /s ..\$(release_dir)\TOOLS\AUTO
	@del /q /f /s ..\$(release_dir)\TOOLS\drive
	@del /q /f /s ..\$(release_dir)\TOOLS\ROM
	@del /q /f /s ..\$(release_dir)\**\.* 
endif
##############################################################
#############################################################
# This Section is for uploading a program to SPI Flash
#############################################################
RISCV_OPENOCD ?= openocd 
ifeq ($(DOWNLOAD),ilm)
OPENOCDCFG ?= $(AE10X_DIR)/ENV/openocd_hbird_ilm.cfg
else
OPENOCDCFG ?= $(AE10X_DIR)/ENV/openocd_hbird.cfg
endif
OPENOCDARGS += -f $(OPENOCDCFG)
GDB_PORT ?= 3333
#############################################################
# This Section is for launching the debugger
#############################################################
openocd: copyright_notice
	$(RISCV_OPENOCD) $(OPENOCDARGS)
GDBCMDS += -ex "set remotetimeout 240"
GDBCMDS += -ex "set architecture riscv:rv32ec"
GDBCMDS += -ex "target extended-remote localhost:$(GDB_PORT)"
gdb: copyright_notice
	$(RISCV_GDB) $(PROGRAM_FILE)  $(GDBCMDS)
GDB_UPLOAD_ARGS ?= --batch
GDB_UPLOAD_CMDS += $(GDBCMDS)
GDB_UPLOAD_CMDS += -ex "monitor reset halt"
GDB_UPLOAD_CMDS += -ex "monitor flash protect 0 0 last off"
GDB_UPLOAD_CMDS += -ex "load"
GDB_UPLOAD_CMDS += -ex "monitor resume"
GDB_UPLOAD_CMDS += -ex "monitor shutdown"
GDB_UPLOAD_CMDS += -ex "quit"
upload: copyright_notice
	$(RISCV_OPENOCD) $(OPENOCDARGS) & \
	$(RISCV_GDB)  $(PROGRAM_FILE) $(GDB_UPLOAD_ARGS) $(GDB_UPLOAD_CMDS) && \
	echo "Successfully uploaded '$(PROGRAM)' "
#############################################################
# Prints help message
#############################################################
.PHONY: help
help: copyright_notice
	@echo   Makefile targets:
	@echo 
	@echo      compile :
	@echo      Build a software program to load with the debugger.
	@echo 
	@echo  	upload :
	@echo   Launch OpenOCD to flash your program to the on-board Flash.
	@echo 
	@echo  	openocd :
	@echo  	gdb :
	@echo   Launch OpenOCD or GDB seperately. Allows Ctrl-C to halt running programs.
	@echo 
	@echo  	clean :
	@echo   clean compile build software program file.
.PHONY:clean
clean: copyright_notice
ifeq ($(OS),Linux) #Linux
	@$(MAKE) -f $(TOPDIR)/common.mk --no-print-directory clean OS=$(OS)
	@rm -f $(shell find -name "*.verilog")
	@echo RM -F *.VERILOG
	@rm -f $(shell find -name "*.readelf")
	@echo RM -F *.READELF
	@rm -f $(shell find -name "*.dump")
	@echo RM -F *.DUMP
	@rm -f $(shell find -name "*.bin")
	@echo RM -F *.BIN
	@rm -f $(shell find -name "*.hex")
	@echo RM -F *.HEX
	@rm -f $(shell find -name "*.s19")
	@echo RM -F *.S19
	@rm -f $(shell find -name "*.map")
	@echo RM -F *.MAP
	@rm -f $(shell find -name "*.elf")
	@echo RM -F *.ELF
	@rm -f $(shell find -name "*.o")
	@rm -f $(shell find -name "*.d")
	@rm -f $(shell find -name "*.i")
	@rm -f $(shell find -name "*.s")
	@echo RM -F *.[IODS]
	@rm -f $(shell find -name "*.res")
	@echo RM -F *.RES
	@rm -f $(shell find -name "*.tags")
	@echo RM -F *.TAGS
	@rm -f $(shell find -name "*.asm")
	@echo RM -F *.ASM
else #Windows or other commands
	@del /q /f /s *.verilog
	@del /q /f /s *.readelf
	@del /q /f /s *.dump
	@del /q /f /s *.bin
	@del /q /f /s *.hex
	@del /q /f /s *.s19
	@del /q /f /s *.map
	@del /q /f /s *.elf
	@del /q /f /s  *.o
	@del /q /f /s  *.d
	@del /q /f /s  *.i
	@del /q /f /s  *_*.s
	@del /q /f /s  *.res
	@del /q /f /s  *.asm
endif
		