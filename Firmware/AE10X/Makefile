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
 # Copyright ©2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 # 版权所有 ©2021-2023龙晶石半导体科技(苏州)有限公司

## Add ASM Source Files:
## ASM_SRCS += $(AE10X_DIR)/xxx.S
## Add C Source Files:
## C_SRCS += $(AE10X_DIR)/xxx.c
## Add O Source Files:
## O_SRCS += $(AE10X_DIR)/xxx.o
## Add H Source Files:
## HEADERS += $(AE10X_DIR)/INCLUDE/xxx.H
##################################### C and S Source Files ##############################
ifeq ($(VERSION),0.0.0)#未发布版本
# The start-up assembly program
ASM_SRCS += $(AE10X_DIR)/START.S
# The interrupt/exception/nmi entry program
ASM_SRCS += $(AE10X_DIR)/ENTRY.S
# The CPU Function program
C_SRCS += $(AE10X_DIR)/AE_FUNC.c
# The CHIP Initalization program
C_SRCS += $(AE10X_DIR)/AE_INIT.c
# The Flash Info program
C_SRCS += $(AE10X_DIR)/AE_FLASHINFO.c
# The Library program
C_SRCS += $(AE10X_DIR)/AE_PRINTF.c
C_SRCS += $(AE10X_DIR)/AE_MALLOC.c
C_SRCS += $(AE10X_DIR)/AE_LIBC.c
C_SRCS += $(AE10X_DIR)/AE_SOFT_FP.c
# The Update program
C_SRCS += $(AE10X_DIR)/AE_UPDATE.c
# The Debug program
C_SRCS += $(AE10X_DIR)/AE_COMMAND.c
C_SRCS += $(AE10X_DIR)/AE_DEBUGGER.c
# The ANX7447 Library program
C_SRCS += $(AE10X_DIR)/AE_ANX7447_LIB.c
C_SRCS += $(AE10X_DIR)/AE_ANX7447_UCSI.c
else #已发布版本
# The Flash Info program
C_SRCS += $(AE10X_DIR)/AE_FLASHINFO.c
# The Library program
ifeq ($(OS),Linux)
ifneq ($(wildcard $(AE10X_DIR)/libLinuxAE10X.a),)
LINK_LIBS += -L$(AE10X_DIR) -lLinuxAE10X
endif
else #Windows or other commands
ifneq ($(wildcard $(AE10X_DIR)/libWinAE10X.a),)
LINK_LIBS += -L $(AE10X_DIR) -lWinAE10X
endif
endif

endif
##################################### H Source Files ##############################
HEADERS += $(AE10X_DIR)/INCLUDE/RISCV.H
HEADERS += $(AE10X_DIR)/INCLUDE/AE_GLOBAL.H
HEADERS += $(AE10X_DIR)/INCLUDE/AE_CONFIG.H
HEADERS += $(AE10X_DIR)/INCLUDE/AE_REG.H
HEADERS += $(AE10X_DIR)/INCLUDE/AE_GLOBAL.H
HEADERS += $(AE10X_DIR)/INCLUDE/AE_LIBC.H
HEADERS += $(AE10X_DIR)/INCLUDE/AE_INIT.H
HEADERS += $(AE10X_DIR)/INCLUDE/AE_INCLUDE.H
HEADERS += $(AE10X_DIR)/INCLUDE/AE_DEBUGGER.H
HEADERS += $(AE10X_DIR)/INCLUDE/AE_COMMAND.H
HEADERS += $(AE10X_DIR)/INCLUDE/AE_FUNC.H
HEADERS += $(AE10X_DIR)/INCLUDE/AE_UPDATE.H
HEADERS += $(AE10X_DIR)/INCLUDE/AE_PRINTF.H
HEADERS += $(AE10X_DIR)/INCLUDE/AE_MALLOC.H
HEADERS += $(AE10X_DIR)/INCLUDE/AE_SOFT_FP.H
HEADERS += $(AE10X_DIR)/INCLUDE/AE_ANX7447_LIB.H
HEADERS += $(AE10X_DIR)/INCLUDE/AE_ANX7447_UCSI.H
HEADERS += $(AE10X_DIR)/INCLUDE/AE_FLASHINFO.H
