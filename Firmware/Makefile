 # @Author: Iversu
 # @LastEditors: daweslinyu
 # @LastEditTime: 2025-07-03 18:10:26
 # @Description: Platform battery & battery charger control code
 #
 #
 # The following is the Chinese and English copyright notice, encoded as UTF-8.
 # 以下是中文及英文版权同步声明,编码为UTF-8。
 # Copyright has legal effects and violations will be prosecuted.
 # 版权具有法律效力，违反必究。
 #
 # Copyright ©2021-2025 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 # 版权所有 ©2021-2025龙晶石半导体科技(苏州)有限公司

#############################################################
# Global Variables
#############################################################
#SYSTEM GLOBAL VARIABLES
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
ifeq ($(findstring /sh,$(SHELL_TYPES)),/sh)
SHELL_TYPE := $(shell readlink -f $(SHELL_TYPES))
else 
SHELL_TYPE := $(SHELL_TYPES)
endif
export OS BUILD_DATE BUILD_TIME SHELL_TYPE

#CHIP GLOBAL VARIABLES
DOWNLOAD ?= flash
RISCV_ARCH?=rv32ec#按照RISC-V架构命名规则，以下指令子集的组合可表示为RV32EC
#RV32架构：riscv-32架构通用寄存器宽度32位
#E：支持16个通用寄存器
#C：支持编码长度为16位的压缩指令，提高代码密度
RISCV_ABI?=ilp32e#int long pointer 都是32bit,不使用硬件浮点数,支持rv32e
export DOWNLOAD RISCV_ARCH RISCV_ABI

#CHIP SPECIFIC GLOBAL VARIABLES
CHIP?=GLE01 # x替换当前芯片型号
VERSION ?= 0.0.0# 定义版本号
PROGRAM ?= ec_main#project Object name
export CHIP VERSION PROGRAM
#############################################################
# Configuration
#############################################################
# Compile flags
DUAL_BOOT_FLAG?=0#双启动开关
SAVE_TEMPS_FILE ?= 0#保留编译文件
USER_RISCV_LIBC_A?=0#0 USER_AE10X_LIBC_A 1 USER_RISCV_LIBC_A
export DUAL_BOOT_FLAG SAVE_TEMPS_FILE USER_RISCV_LIBC_A

#Security and efuse flags
PEM_FILE ?= $(TOPDIR)/TOOLS/Security/private_key4096.pem
EFUSE_SECVER_FLAG?=1#efuse security open or close 
HASH_4kSECTORSIZE?=63#0:4k*HASH_4kSECTORSIZE sector size
EXTERNAL_FLASH_ADDR?=0x000000#外部flash地址
ifeq ($(EFUSE_SECVER_FLAG),1)
EFUSE_SECVER_FLAG_STR := -efuse
else
EFUSE_SECVER_FLAG_STR := -no_efuse
endif
#############################################################
# Toolchain
#############################################################
# Compile Toolchain paths
RISCV_GCC     := riscv-nuclei-elf-gcc
RISCV_GXX     := riscv-nuclei-elf-g++
RISCV_OBJDUMP := riscv-nuclei-elf-objdump
RISCV_OBJCOPY := riscv-nuclei-elf-objcopy
RISCV_GDB     := riscv-nuclei-elf-gdb
RISCV_READELF := riscv-nuclei-elf-readelf
RISCV_AR      := riscv-nuclei-elf-ar
RISCV_SIZE    := riscv-nuclei-elf-size
export RISCV_GCC RISCV_GXX RISCV_OBJDUMP RISCV_OBJCOPY RISCV_GDB RISCV_READELF RISCV_AR RISCV_SIZE

# Shell command
MAKE          	:= make
GCC           	:= gcc
ECHO     		:= echo
ifeq ($(OS),Linux)
COPY     := cp
RM       := rm
RMDIR    := rm -rf
CLEAR    := clear
DD       := dd
else
COPY     := copy
RM       := del
RMDIR    := rmdir
MKDIR    := mkdir
CLEAR    := cls
DD       := dd
XCOPY    := xcopy
endif
export DD COPY RM ECHO

#############################################################
# Directories
#############################################################
#############################################################
# Main Directories
TOPDIR 			:= .
export TOPDIR 
# Sub Directories
AE10X_DIR 		= $(TOPDIR)/AE10X
KERNEL_DIR 		= $(TOPDIR)/KERNEL
CUSTOM_DIR  	= $(TOPDIR)/CUSTOM
export AE10X_DIR KERNEL_DIR CUSTOM_DIR
ifeq ($(VERSION),0.0.0)#未发布版本
TEST_DIR  		= $(TOPDIR)/TEST
export TEST_DIR
endif

# Environment
ENV_DIR 		= $(AE10X_DIR)/ENV
export  ENV_DIR
# Security and efuse
SECURITY_DIR 	= $(TOPDIR)/TOOLS/Security

#includes Directory
CINCLUDE 			+= -I $(AE10X_DIR)/INCLUDE
CINCLUDE 			+= -I $(KERNEL_DIR)/INCLUDE
CINCLUDE 			+= -I $(CUSTOM_DIR)/INCLUDE
ifeq ($(VERSION),0.0.0)#未发布版本
CINCLUDE 			+= -I $(TEST_DIR)/INCLUDE
endif
export CINCLUDE
##############################################################
# Compile Options
#############################################################
PROGRAM_FILE ?= $(PROGRAM)_$(BUILD_DATE)$(BUILD_TIME).elf
PROGRAM_ELF = $(TOPDIR)/$(PROGRAM_FILE)
export PROGRAM_FILE PROGRAM_ELF
ifeq ($(DUAL_BOOT_FLAG),1)#双启动
PROGRAM_BOOT1 ?= $(PROGRAM)_1
PROGRAM_BOOT2 ?= $(PROGRAM)_2
PROGRAM_FILE_BOOT1 ?=$(PROGRAM_BOOT1)_$(BUILD_DATE)$(BUILD_TIME).elf
PROGRAM_FILE_BOOT2 ?= $(PROGRAM_BOOT2)_$(BUILD_DATE)$(BUILD_TIME).elf
PROGRAM_ELF1 = $(TOPDIR)/$(PROGRAM_FILE_BOOT1)
PROGRAM_ELF2 = $(TOPDIR)/$(PROGRAM_FILE_BOOT2)
export PROGRAM_BOOT1 PROGRAM_BOOT2 PROGRAM_FILE_BOOT1 PROGRAM_FILE_BOOT2 PROGRAM_ELF1 PROGRAM_ELF2

#合并命令
DD_ELF_BOOT:= if=/dev/zero ibs=4k count=64 | tr "\000" "\377"
DD_ELF_BOOT1:= if=./$(PROGRAM_BOOT1).bin  of=$(PROGRAM).bin seek=0   conv=notrunc
DD_ELF_BOOT2:= if=./$(PROGRAM_BOOT2).bin  of=$(PROGRAM).bin seek=256 conv=notrunc
endif

#Security and efuse Tools
ifeq ($(OS),Linux)
SIGN_NAME := sign
else
SIGN_NAME := sign.exe
endif
SIGN_SL = $(SECURITY_DIR)/$(SIGN_NAME)
SIGN = $(TOPDIR)/$(SIGN_NAME)

compile: software 
ifeq ($(OS),Linux) #Linux
#	 @$(GCC) $(SECURITY_DIR)/sign.c $(SECURITY_DIR)/sign.h -D$(CHIP) -I/usr/include/openssl/ -L /usr/lib/x86_64-linux-gnu/ -lcrypto  -o $(SIGN_NAME) #-D_DEBUG
#	 @$(COPY) $(SIGN) $(SECURITY_DIR)
	@if [ -f $(SIGN_SL) ]; then \
		$(COPY)  $(SIGN_SL) $(TOPDIR); \
		$(SIGN) -f $(PROGRAM).bin -s 4096 -h $(HASH_4kSECTORSIZE) -sign_key $(PEM_FILE) -addr $(EXTERNAL_FLASH_ADDR)  $(EFUSE_SECVER_FLAG_STR); \
		$(RM) $(SIGN); \
	fi
else #Windows or other commands
	@if exist $(SIGN_SL) ( \
		$(COPY)  $(SIGN_SL) $(TOPDIR) && \
		$(SIGN) -f $(PROGRAM).bin -s 4096 -h $(HASH_4kSECTORSIZE) -sign_key $(PEM_FILE) -addr $(EXTERNAL_FLASH_ADDR)  $(EFUSE_SECVER_FLAG_STR) && \
		$(RM) $(SIGN); \
		)
endif
	@$(ECHO) CHIP $(CHIP) CODE Compiled successfully.
.PHONY: software
software: copyright_notice

	@$(ECHO) SHELL = $(SHELL_TYPE)
	@$(ECHO) make -f $(TOPDIR)/common.mk
	@$(MAKE) -f $(TOPDIR)/common.mk --no-print-directory 

ifeq ($(DUAL_BOOT_FLAG),0)
#riscv-nuclei-elf-objdump -m riscv:rv32 -D -b binary *.bin > *.dump
	@$(RISCV_OBJDUMP) -D $(PROGRAM_ELF) > $(PROGRAM).dump
	@$(ECHO) OBJDUMP $(PROGRAM).dump
	@$(RISCV_OBJCOPY) $(PROGRAM_ELF) -O verilog $(PROGRAM).verilog
#	@$(RISCV_OBJCOPY) $(PROGRAM_ELF) -O verilog $(PROGRAM).vh
	@$(ECHO) OBJCOPY $(PROGRAM).verilog
	@$(RISCV_OBJCOPY) -O binary $(PROGRAM_ELF) $(PROGRAM).bin
	@$(ECHO) OBJCOPY $(PROGRAM).bin
	@$(RISCV_OBJCOPY) -I binary -O ihex $(PROGRAM).bin $(PROGRAM).hex
# 	@$(RISCV_OBJCOPY) -O ihex $(PROGRAM_ELF) $(PROGRAM)_0x80000.hex
	@$(ECHO) OBJCOPY $(PROGRAM).hex
	@$(RISCV_OBJCOPY) -I binary -O srec $(PROGRAM).bin $(PROGRAM).s19
# 	@$(RISCV_OBJCOPY) -O srec $(PROGRAM_ELF) $(PROGRAM)_0x80000.s19
	@$(ECHO) OBJCOPY $(PROGRAM).s19
	@$(RISCV_READELF)  -a $(PROGRAM_ELF) > $(PROGRAM).readelf
	@$(ECHO) READELF $(PROGRAM).readelf
else ifeq ($(DUAL_BOOT_FLAG),1)#Dual start
	@$(RISCV_OBJDUMP) -D $(PROGRAM_ELF1) > $(PROGRAM_BOOT1).dump
	@$(ECHO) OBJDUMP $(PROGRAM_BOOT1).dump
	@$(RISCV_OBJCOPY) $(PROGRAM_ELF1) -O verilog $(PROGRAM_BOOT1).verilog
	@$(ECHO) OBJCOPY $(PROGRAM_BOOT1).verilog
	@$(RISCV_OBJCOPY) -O binary $(PROGRAM_ELF1) $(PROGRAM_BOOT1).bin
	@$(ECHO) OBJCOPY $(PROGRAM_BOOT1).bin
	@$(RISCV_OBJDUMP) -D $(PROGRAM_ELF2) > $(PROGRAM_BOOT2).dump
	@$(ECHO) OBJDUMP $(PROGRAM_BOOT2).dump
	@$(RISCV_OBJCOPY) $(PROGRAM_ELF2) -O verilog $(PROGRAM_BOOT2).verilog
	@$(ECHO) OBJCOPY $(PROGRAM_BOOT2).verilog
	@$(RISCV_OBJCOPY) -O binary $(PROGRAM_ELF2) $(PROGRAM_BOOT2).bin
	@$(ECHO) OBJCOPY $(PROGRAM_BOOT2).bin
	@$(DD) $(DD_ELF_BOOT)>$(PROGRAM).bin
	@$(DD) $(DD_ELF_BOOT1)
	@$(DD) $(DD_ELF_BOOT2)
endif
.PHONY: copyright_notice
copyright_notice:
	-git config --global core.ignorecase false
ifeq ($(OS),Linux) #Linux
	@$(CLEAR)
	@$(CLEAR)
	@$(ECHO) "************************************************************************************"
	@$(ECHO) "*                    Embedded Controller  SPK32 Series Firmware                    *"
	@$(ECHO) "* Copyright ©2021-2025 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved. *"
	@$(ECHO) "************************************************************************************"
else #Windows or other commands
#-powershell -Command "Start-Process 'fsutil' -ArgumentList 'file setCaseSensitiveInfo . enable' -Verb RunAs" 
#-fsutil file setCaseSensitiveInfo $(TOPDIR) enable
	@$(CLEAR)
	@$(CLEAR)
	@$(ECHO) ************************************************************************************
	@$(ECHO) *                    Embedded Controller  SPK32 Series Firmware                    *
	@$(ECHO) *Copyright (c)2021-2025 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.*
	@$(ECHO) ************************************************************************************
endif
release_dir := release_v$(VERSION)_$(BUILD_DATE)
.PHONY: release
release:
ifeq ($(OS),Linux) #Linux
	@$(COPY) ./ ../$(release_dir) -r
	@$(RM) -f ../$(release_dir)/AE10X/*.c ../$(release_dir)/AE10X/*.S ../$(release_dir)/AE10X/*.o ../$(release_dir)/AE10X/*.d ../$(release_dir)/AE10X/*.i ../$(release_dir)/AE10X/*.asm ../$(release_dir)/AE10X/*.s
	@$(COPY) ./AE10X/AE_FLASHINFO.c ../$(release_dir)/AE10X/
	@$(RMDIR) ../$(release_dir)/TEST
	@$(RMDIR) ../$(release_dir)/release.sh ../$(release_dir)/rom.sh
	@$(RMDIR) ../$(release_dir)/TOOLS/app ../$(release_dir)/TOOLS/auto_test ../$(release_dir)/TOOLS/drive ../$(release_dir)/TOOLS/classic_benchmarks
	@$(RMDIR) ../$(release_dir)/.v* ../$(release_dir)/.s*  ../$(release_dir)/.c* ../$(release_dir)/.g* ../$(release_dir)/.p* 
else #Windows or other commands
	@if not exist ..\$(release_dir) $(MKDIR) ..\$(release_dir)
	@xcopy **\* ..\$(release_dir) /s /e /i /y
	@$(RM) /q /f /s ..\$(release_dir)\AE10X\*.c
	@$(XCOPY) .\AE10X\AE_FLASHINFO.c  ..\$(release_dir)\AE10X
	@$(RM) /q /f /s ..\$(release_dir)\AE10X\*.S
	@$(RMDIR) /s /q ..\$(release_dir)\TEST
	@$(RM) /q /f /s ..\$(release_dir)\*.sh
	@$(RMDIR) /s /q ..\$(release_dir)\TOOLS\app
	@$(RMDIR) /s /q ..\$(release_dir)\TOOLS\auto_test
	@$(RMDIR) /s /q ..\$(release_dir)\TOOLS\drive
	@$(RMDIR) /s /q ..\$(release_dir)\TOOLS\classic_benchmarks
	@$(RM) /q /f /s ..\$(release_dir)\TOOLS\check_and_concatenate.sh
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
	$(ECHO) "Successfully uploaded '$(PROGRAM)' "
#############################################################
# Prints help message
#############################################################
.PHONY: help
help: copyright_notice
	@$(ECHO)   Makefile targets:
	@$(ECHO) 
	@$(ECHO)      compile :
	@$(ECHO)      Build a software program to load with the debugger.
	@$(ECHO) 
	@$(ECHO)  	upload :
	@$(ECHO)   Launch OpenOCD to flash your program to the on-board Flash.
	@$(ECHO) 
	@$(ECHO)  	openocd :
	@$(ECHO)  	gdb :
	@$(ECHO)   Launch OpenOCD or GDB seperately. Allows Ctrl-C to halt running programs.
	@$(ECHO) 
	@$(ECHO)  	clean :
	@$(ECHO)   clean compile build software program file.
.PHONY:clean
clean: copyright_notice
	@$(MAKE) -f $(TOPDIR)/common.mk --no-print-directory clean
ifeq ($(OS),Linux) #Linux
	@$(RM) -f $(shell find -name "*.verilog")
	@$(ECHO) RM -F *.VERILOG
	@$(RM) -f $(shell find -name "*.readelf")
	@$(ECHO) RM -F *.READELF
	@$(RM) -f $(shell find -name "*.dump")
	@$(ECHO) RM -F *.DUMP
	@$(RM) -f $(shell find -name "*.bin")
	@$(ECHO) RM -F *.BIN
	@$(RM) -f $(shell find -name "*.hex")
	@$(ECHO) RM -F *.HEX
	@$(RM) -f $(shell find -name "*.s19")
	@$(ECHO) RM -F *.S19
	@$(RM) -f $(shell find -name "*.map")
	@$(ECHO) RM -F *.MAP
	@$(RM) -f $(shell find -name "*.elf")
	@$(ECHO) RM -F *.ELF
	@$(RM) -f $(shell find -name "*.res")
	@$(ECHO) RM -F *.RES
	@$(RM) -f $(shell find -name "*.tags")
	@$(ECHO) RM -F *.TAGS
	@$(RM) -f $(shell find -name "*.d")
	@$(RM) -f $(shell find -name "*.i")
	@$(RM) -f $(shell find -name "*.s")
	@$(ECHO) RM -F *.[IDS]
	@$(RM) -f $(shell find -name "*.asm")
	@$(ECHO) RM -F *.ASM
else #Windows or other commands
	@$(RM) /q /f /s *.verilog
	@$(RM) /q /f /s *.readelf
	@$(RM) /q /f /s *.dump
	@$(RM) /q /f /s *.bin
	@$(RM) /q /f /s *.hex
	@$(RM) /q /f /s *.s19
	@$(RM) /q /f /s *.map
	@$(RM) /q /f /s *.elf
	@$(RM) /q /f /s  *.res
	@$(RM) /q /f /s  *.d
	@$(RM) /q /f /s  *.i
	@$(RM) /q /f /s  *.s
	@$(RM) /q /f /s  *.asm
endif
		