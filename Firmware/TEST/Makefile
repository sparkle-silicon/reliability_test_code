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

## Add ASM Source Files:
## ASM_SRCS += $(TEST_DIR)/xxx.S
## Add C Source Files:
## C_SRCS += $(TEST_DIR)/xxx.c
## Add O Source Files:
## O_SRCS += $(TEST_DIR)/xxx.o
## Add H Source Files:
## HEADERS += $(TEST_DIR)/INCLUDE/xxx.H
##################################### C and S Source Files ##############################
ifeq ($(VERSION),0.0.0)#未发布版本
C_SRCS += $(TEST_DIR)/TEST.c
C_SRCS += $(TEST_DIR)/TEST_AUTOMATIC.c
C_SRCS += $(TEST_DIR)/TEST_MAIN.c
C_SRCS += $(TEST_DIR)/TEST_INTC.c
C_SRCS += $(TEST_DIR)/TEST_IRQ.c
C_SRCS += $(TEST_DIR)/TEST_TIMEEVENT.c
C_SRCS += $(TEST_DIR)/TEST_SOC_FUNC.c
HEADERS += $(TEST_DIR)/INCLUDE/TEST.H
HEADERS += $(TEST_DIR)/INCLUDE/TEST_AUTOMATIC.H
HEADERS += $(TEST_DIR)/INCLUDE/TEST_MAIN.H
HEADERS += $(TEST_DIR)/INCLUDE/TEST_INTC.H
HEADERS += $(TEST_DIR)/INCLUDE/TEST_IRQ.H
HEADERS += $(TEST_DIR)/INCLUDE/TEST_TIMEEVENT.H
HEADERS += $(TEST_DIR)/INCLUDE/TEST_CONFIG.H
HEADERS += $(TEST_DIR)/INCLUDE/TEST_SOC_FUNC.H
endif
