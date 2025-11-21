/*
 * @Author: Iversu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2024-02-07 15:57:07
 * @Description:
 *
 *
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 以下是中文及英文版权同步声明，编码为UTF-8。
 * Copyright has legal effects and violations will be prosecuted.
 * 版权具有法律效力，违反必究。
 *
 * Copyright ©2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2023龙晶石半导体科技（苏州）有限公司
 */
#ifndef KERNEL_ESPI_H
#define KERNEL_ESPI_H
#include "AE_GLOBAL.H" //全局变量

#define F_CLR_PLTRST_STEP 0
#define F_PLTRST_HI_LEVEL 1
#define F_PLTRST_HI_TO_LO 2
#define F_PLTRST_LO_TO_HI 3
#define F_PLTRST_DETECTED 3

 //-----------------------------------------------------------------------------
 // (31xxh) eSPI slave Interface ()
 //-----------------------------------------------------------------------------
 /* Device Identification 00h-03h */
#define eSPI_Version_ID REG_3103 // REG_3103
/* General Capabilities and Configurations     04h-07h */
#define F_CRC_Checking_Enable BIT(7) // ESPI_GCC0
#define F_Response_Modifier_Enable BIT(6)
#define F_ALERT_MODE BIT(4) // 0b: EIO1 (I/O[1] ) pin is used to
                            //     signal the Alert event

#define _eSPI_MAX_FREQ_20MHZ 0x00 // ESPI_GCC1
#define _eSPI_MAX_FREQ_25MHZ 0x01
#define _eSPI_MAX_FREQ_33MHZ 0x02
#define _eSPI_MAX_FREQ_50MHZ 0x03
#define _eSPI_MAX_FREQ_66MHZ 0x04
#define _eSPI_OP_FREQ_20MHZ 0x00
#define _eSPI_OP_FREQ_25MHZ 0x10
#define _eSPI_OP_FREQ_33MHZ 0x20
#define _eSPI_OP_FREQ_50MHZ 0x30
#define _eSPI_OP_FREQ_66MHZ 0x40

#define F_Flash_Access_Channel BIT(3) // ESPI_GCC3
#define F_OOB_Message_Channel BIT(2)
#define F_Virtual_Wire_Channel BIT(1)
#define F_Peripheral_Channel BIT(0)

/* Channel 0 Capabilities and Configurations   08h-0Bh */
#define F_Bus_Master_Enable BIT(2) // ESPI_CH0_GCC3
#define F_Peripheral_Channel_Ready BIT(1)
#define F_PeripheralChannelEnable BIT(0)

/* Channel 1 Capabilities and Configurations   0Ch-0Fh */
#define F_VW_CHN_READY BIT(1) // ESPI_CH1_GCC3
#define F_VW_CHN_ENABLE BIT(0)

/* Channel 2 Capabilities and Configurations   10h-13h */
#define F_OOB_CHN_READY BIT(1) // ESPI_CH2_GCC3
#define F_OOB_CHN_ENABLE BIT(0)

/* Channel 3 Capabilities and Configurations   14h-17h */
/* ESPI_CH3_GCC1: Flash Sharing Capability Supported
   This field indicates the flash sharing capability supported by the slave.
   00b: MAFS supported.
   01b: MAFS supported.
   10b: SAFS supported.
   11b: MAFS/SAFS supported.
*/
#define _SUPPORT_MAFS 0x00
#define _SUPPORT_MAFS1 0x01
#define _SUPPORT_SAFS 0x02
#define _SUPPORT_MAFS_SAFS 0x03

#define F_FLASH_CHN_READY BIT(1) // ESPI_CH3_GCC3
#define F_FLASH_CHN_ENABLE BIT(0)

#define SET_FAS_CHANNEL_READY BIT(3)
#define SET_OOB_CHANNEL_READY BIT(2)
#define SET_VW_CHANNEL_READY BIT(1)

/* Channel 3 Capabilities and Configurations 2 18h-1Bh */
#define F_PUT_PC_STATUS BIT(7)  // PUT_PC Status

#define F_eSPI_INT_ENABLE BIT(7) // ESPI_GCTRL1

#define F_ESPIQCSE BIT(7) // ESPI_GCTRL2
#define F_ESPIIPG BIT(6)
#define F_eSPI_TO_WUC_ENABLE BIT(4)

#define F_SESPIPC BIT(0)   // ESPI_GCTRL3      // Suspend eSPI Peripheral Channel
#define CH0_not_RDY BIT(0) // Suspend eSPI Peripheral Channel
//REG32(0x330B0)
#define Upstream_EN 0x80 // ESPI_UCTRL0
#define Upstream_GO 0x40
#define Upstream_INT_EN 0x20
#define Upstream_Req_Authority 0x10
#define Upstream_Read_Authority_EN 0x08
#define Upstream_Channe_Disable 0x04
#define Upstream_Done 0x02
#define Upstream_Busy 0x01

// Upstream cycle Type
#define OOB_Message 0x07 // ESPI_UCTRL1
#define OOB_Flash_Read 0x08
#define OOB_Flash_Write 0x09
#define OOB_Flash_Erase 0x0A
#define Peri_Message 0x05

#define UpstreamLength ESPI_UCTRL3 // ESPI_UCTRL3

// PUT_FLASH_C cycle Type
#define Successful_Completion_Without_Data 0x00 // ESPI_UCTRL6
#define Successful_Completion_With_Data_the_only_Completion 0x04
#define Unsuccessful_Completion_Without_Data_the_last_Completion 0x05
#define Unsuccessful_Completion_Without_Data_the_only_Completion 0x06
#define Invalid_Cycle_Type 0x0F

//REG32(0x330C0)
#define PUT_OOB_STATUS 0x80 // ESPI_OCTRL0
#define eRPMCSTS 0x10

#define F_PUT_OOB_INT_ENABLE BIT(7) // ESPI_OCTRL1

#define PUT_FLASH_NP_STATUS BIT(7) // ESPI_SAFSC1

//-----------------------------------------------------------------------------
// (32xxh) eSPI VW Interface ()
//-----------------------------------------------------------------------------
/* System Event Virtual Wires for Index=2
   Virtual Wire Group: System Event
   Reset: eSPI Reset#
   Direction: Master to Slave
*/
#define F_IDX2_BIT7_RESERVED BIT(7) // ESPI_VWIDX2
#define F_IDX2_SLP_S5_VALID BIT(6)
#define F_IDX2_SLP_S4_VALID BIT(5)
#define F_IDX2_SLP_S3_VALID BIT(4)
#define F_IDX2_BIT3_RESERVED BIT(3)
#define F_IDX2_SLP_S5 BIT(2)
#define F_IDX2_SLP_S4 BIT(1)
#define F_IDX2_SLP_S3 BIT(0)

#define IS_eSPI_SLP_S5_VALID() IS_MASK_SET(ESPI_VWIDX2, F_IDX2_SLP_S5_VALID)
#define IS_eSPI_SLP_S4_VALID() IS_MASK_SET(ESPI_VWIDX2, F_IDX2_SLP_S4_VALID)
#define IS_eSPI_SLP_S3_VALID() IS_MASK_SET(ESPI_VWIDX2, F_IDX2_SLP_S3_VALID)

#define IS_eSPI_SLP_S5_HI() IS_MASK_SET(ESPI_VWIDX2, F_IDX2_SLP_S5)
#define IS_eSPI_SLP_S5_LO() IS_MASK_CLEAR(ESPI_VWIDX2, F_IDX2_SLP_S5)

#define IS_eSPI_SLP_S4_HI() IS_MASK_SET(ESPI_VWIDX2, F_IDX2_SLP_S4)
#define IS_eSPI_SLP_S4_LO() IS_MASK_CLEAR(ESPI_VWIDX2, F_IDX2_SLP_S4)

#define IS_eSPI_SLP_S3_HI() IS_MASK_SET(ESPI_VWIDX2, F_IDX2_SLP_S3)
#define IS_eSPI_SLP_S3_LO() IS_MASK_CLEAR(ESPI_VWIDX2, F_IDX2_SLP_S3)

/* System Event Virtual Wires for Index=3
   Virtual Wire Group: System Event
   Reset: eSPI Reset#
   Direction: Master to Slave
*/
#define F_IDX3_BIT7_RESERVED BIT(7) // ESPI_VWIDX3
#define F_IDX3_OOB_RST_WARN_VALID BIT(6)
#define F_IDX3_PLTRST_VALID BIT(5)
#define F_IDX3_SUS_STAT_VALID BIT(4)
#define F_IDX3_BIT3_RESERVED BIT(3)
#define F_IDX3_OOB_RST_WARN BIT(2)
#define F_IDX3_PLTRST BIT(1)
#define F_IDX3_SUS_STAT BIT(0)

/* System Event Virtual Wires for Index=4
   Virtual Wire Group: System Event
   Reset: eSPI Reset#
   Direction: Slave to Master
*/
#define F_IDX4_PME_VALID BIT(7) // ESPI_VWIDX4
#define F_IDX4_WAKE_VALID BIT(6)
#define F_IDX4_BIT5_RESERVED BIT(5)
#define F_IDX4_OOB_RST_ACK_VALID BIT(4)
#define F_IDX4_PME BIT(3)
#define F_IDX4_WAKE BIT(2)
#define F_IDX4_BIT1_RESERVED BIT(1)
#define F_IDX4_OOB_RST_ACK BIT(0)

/* System Event Virtual Wires for Index=5
   Virtual Wire Group: System Event
   Reset: eSPI Reset#
   Direction: Slave to Master
*/
#define F_IDX5_SLAVE_BOOT_LOAD_STATUS_VALID BIT(7) // ESPI_VWIDX5
#define F_IDX5_ERROR_NONFATAL_VALID BIT(6)
#define F_IDX5_ERROR_FATAL_VALID BIT(5)
#define F_IDX5_SLAVE_BOOT_LOAD_DONE_VALID BIT(4)
#define F_IDX5_SLAVE_BOOT_LOAD_STATUS BIT(3)
#define F_IDX5_ERROR_NONFATAL BIT(2)
#define F_IDX5_ERROR_FATAL BIT(1)
#define F_IDX5_SLAVE_BOOT_LOAD_DONE BIT(0)

/* System Event Virtual Wires for Index=6
   Virtual Wire Group: System Event
   Reset: PLTRST#
   Direction: Slave to Master
*/
#define F_IDX6_HOST_RST_ACK_VALID BIT(7) // ESPI_VWIDX6
#define F_IDX6_RCIN_VALID BIT(6)
#define F_IDX6_SMI_VALID BIT(5)
#define F_IDX6_SCI_VALID BIT(4)
#define F_IDX6_HOST_RST_ACK BIT(3)
#define F_IDX6_RCIN BIT(2)
#define F_IDX6_SMI BIT(1)
#define F_IDX6_SCI BIT(0)

/* System Event Virtual Wires for Index=7
   Virtual Wire Group: System Event
   Reset: PLTRST#
   Direction: Master to Slave
*/
#define F_IDX7_BIT7_RESERVED BIT(7) // ESPI_VWIDX7
#define F_IDX7_NMIOUT_VALID BIT(6)
#define F_IDX7_SMIOUT_VALID BIT(5)
#define F_IDX7_HOST_RST_WARN_VALID BIT(4)
#define F_IDX7_BIT3_RESERVED BIT(3)
#define F_IDX7_NMIOUT BIT(2)
#define F_IDX7_SMIOUT BIT(1)
#define F_IDX7_HOST_RST_WARN BIT(0)

#define F_INTR1_SMIOUT BIT(7)    //ESPI_VWISTA0
#define F_INTR0_HOST_RST_WARN BIT(6)
#define F_INTR0_OOB_RST_WARN BIT(5)
#define F_INTR0_PLT_RST_WARN BIT(4)
#define F_INTR0_SUS_STAT BIT(3)
#define F_INTR0_SLP_S5 BIT(2)
#define F_INTR0_SLP_S4 BIT(1)
#define F_INTR0_SLP_S3 BIT(0)

#define F_INTR1_SLP_WLAN BIT(6)    //ESPI_VWISTA1
#define F_INTR1_SLP_LAN BIT(5) 
#define F_INTR1_SLP_A BIT(4) 
#define F_INTR1_SUS_PWRDN_ACK BIT(3) 
#define F_INTR1_SUS_WARN BIT(2) 
#define F_INTR1_HOST_C10 BIT(1) 
#define F_INTR1_NMIOUT BIT(0)

#define F_INTR1_GENERIC7 BIT(7)    //ESPI_VWISTA2
#define F_INTR1_GENERIC6 BIT(6)
#define F_INTR1_GENERIC5 BIT(5)
#define F_INTR1_GENERIC4 BIT(4)
#define F_INTR1_GENERIC3 BIT(3)
#define F_INTR1_GENERIC2 BIT(2)
#define F_INTR1_GENERIC1 BIT(1)
#define F_INTR1_GENERIC0 BIT(0)

#define F_IDX40_SUSACK BIT(0) // ESPI_VWIDX40
#define F_IDX40_VALID BIT(4)

#define F_IDX41_SUS_WARN BIT(0) // ESPI_VWIDX41
#define F_IDX41_VALID BIT(4)

#define F_VW_INT_ENABLE BIT(7)  // ESPI_VWCTRL0   // VW Interrupt Enable
#define F_VW_UPDATE_FLAG_SEL BIT(2)
#define F_ASVWSIE_ENABLE BIT(1) // ESPI_VWCTRL0   // Auto Send VW SUS_ACK# Interrupt Enable
#define F_ASVWBIE_ENABLE BIT(0) // ESPI_VWCTRL0   // Auto Send VW Boot_Load_Done/Status Interrupt Enable

#define F_ESPI_VWIDX47_UPDATED BIT(7) // ESPI_VWCTRL1
#define F_ESPI_VWIDX44_UPDATED BIT(6)
#define F_ESPI_VWIDX43_UPDATED BIT(5)
#define F_ESPI_VWIDX42_UPDATED BIT(4)
#define F_ESPI_VWIDX41_UPDATED BIT(3)
#define F_ESPI_VWIDX7_UPDATED BIT(2)
#define F_ESPI_VWIDX3_UPDATED BIT(1)
#define F_ESPI_VWIDX2_UPDATED BIT(0)

#define F_DSW_PWROK_SOURCE BIT(7) // ESPI_VWCTRL2
#define F_SW_DSW_PWROK BIT(6)
#define F_PLATFORM_RESET_CONTROL BIT(5)

#define F_VW_INDEX_46_RESEND BIT(5) // ESPI_VWCTRL3
#define F_VW_INDEX_45_RESEND BIT(4)
#define F_VW_INDEX_40_RESEND BIT(3)
#define F_VW_INDEX_6_RESEND BIT(2)
#define F_VW_INDEX_5_RESEND BIT(1)
#define F_VW_INDEX_4_RESEND BIT(0)

#define F_VWSS BIT(4)    // ESPI_VWCTRL5   // VW SUS_WARN# Select
#define EN_ASVWSE BIT(1) // ESPI_VWCTRL5   // Auto Send VW SUS_ACK# Enable
#define EN_ASVWBE BIT(0) // ESPI_VWCTRL5   // Auto Send VW Boot_Load_Done/Status Enable

#define F_ASVWSF BIT(1) // ESPI_VWCTRL6   // Auto Send VW SUS_ACK# Flag
#define F_ASVWBF BIT(0) // ESPI_VWCTRL6   // Auto Send VW Boot_Load_Done/Status Flag

#define F_ASVWSS BIT(0) // ESPI_VWCTRL7   // Auto Send VW SUS_ACK# Select
// 0b: If ASVWSE is 1, EC sends SUS_ACK# =0.
// 1b: If ASVWSE is 1, EC sends SUS_ACK# =1.
//-----------------------------------------------------------------------------
// RPMC-OOB Message&Response Struct define
//-----------------------------------------------------------------------------
typedef struct
{
    // Byte 0
    uint8_t eSPI_Cycle_Type : 8;   // 0: eSPI Cycle Type: OOB Message = 21h
    // Byte 1
    uint8_t Length_High : 4;        // 1: Length[11:8] = 0h
    uint8_t Tag : 4;                // 1: Tag[3:0] = 0h
    // Byte 2
    uint8_t Length_Low : 8;         // 2: Length[7:0] = 48h
    // Byte 3
    uint8_t Reserved1 : 1;           // 3: Bit 0 reserved
    uint8_t Dest_Slave_Addr : 7;    // 3: Dest Slave Addr[7:1] = 07h
    // Byte 4
    uint8_t Command_Code : 8;        // 4: Command Code = 0Fh
    // Byte 5
    uint8_t Byte_Count : 8;          // 5: Byte Count = 45h
    // Byte 6
    uint8_t Reserved2 : 1;           // 6: Bit 0 reserved
    uint8_t Source_Slave_Addr : 7;   // 6: Source Slave Addr[7:1] = 08h
    // Byte 7
    uint8_t Header_Version : 4;      // 7: Header Version = 0h
    uint8_t MCTP : 4;       		 // 7: MCTP Reserved = 0h
    // Byte 8
    uint8_t Destination_Endpoint_ID : 8; // 8: Destination Endpoint ID
    // Byte 9
    uint8_t Source_Endpoint_ID : 8;  // 9: Source Endpoint ID
    // Byte 10
    uint8_t Message_Tag : 3;         // 10: Message Tag = 000b
    uint8_t TO : 1;                  // 10: TO = 0b
    uint8_t Packet_Seq : 2;          // 10: Packet Seq # = 00b
    uint8_t EOM : 1;                 // 10: EOM = 0b
    uint8_t SOM : 1;                 // 10: SOM = 1b
    // Byte 11
    uint8_t Message_Type : 7;        // 11: Message Type = 7Dh
    uint8_t IC : 1;                  // 11: IC
    // Byte 12
    uint8_t RPMC_Device : 8;         // 12: Reserved = 0h
    // Byte 13
    uint8_t Opcode : 8;              // 13: Opcode = 9Bh
    // Byte 14
    uint8_t Cmd_Type : 8;            // 14: Cmd Type = 00h
    // Byte 15
    uint8_t Counter_Addr : 8;        // 15: Counter Addr[7:0]
    // Byte 16
    uint8_t Rsvd : 8;        // 16: Counter Data[7:0]
    // Byte 17-48
    uint8_t Root_Key[32];            // 根密钥
    // Byte 49-72
    uint8_t TruncatedSignature2_27[26];   // 截断签名
} eSPI_OOB_WriteRootKet_MESSAGE1;

typedef struct {
    // Byte 0
    uint8_t eSPI_Cycle_Type : 8;               // Byte 0: eSPI Cycle Type: OOB Message = 21h
    // Byte 1
    uint8_t Length_High : 4;                   // 1: Length[11:8] = 0h
    uint8_t Tag : 4;                           // 1: Tag[3:0] = 0h
    // Byte 2
    uint8_t Length_Low : 8;                    // Length[7:0]
    // Byte 3
    uint8_t Reserved1 : 1;                      // Bit 0 reserved
    uint8_t Dest_Slave_Addr : 7;               // Dest Slave Addr[7:1] = 07h
    // Byte 4
    uint8_t Command_Code : 8;                   // Command Code = 0Fh
    // Byte 5
    uint8_t Byte_Count : 8;                     // Byte Count = 08h
    // Byte 6
    uint8_t Reserved2 : 1;                      // Bit 0 reserved
    uint8_t Source_Slave_Address : 7;           // Source Slave Address[7:1] = 08h
    // Byte 7
    uint8_t Header_Version : 4;                 // 7: Header Version = 0h
    uint8_t MCTP : 4;                           // 7: MCTP Reserved = 0h
    // Byte 8
    uint8_t Destination_Endpoint_ID : 8;        // Destination Endpoint ID
    // Byte 9
    uint8_t Source_Endpoint_ID : 8;             // Source Endpoint ID
    // Byte 10
    uint8_t Message_Tag : 3;                    // 10: Message Tag = 000b
    uint8_t TO : 1;                             // 10: TO = 0b
    uint8_t Packet_Seq : 2;                     // 10: Packet Seq # = 01b
    uint8_t EOM : 1;                            // 10: EOM = 1b
    uint8_t SOM : 1;                            // 10: SOM = 0b
    // Byte 11
    uint8_t Message_Type : 7;                   // 11: Message Type = 7Dh
    uint8_t IC : 1;                             // 11: IC
    // Byte 12-13
    uint8_t TruncatedSignature0_1[2];           // 截断签名     

} eSPI_OOB_WriteRootKet_MESSAGE2;

typedef struct {
    // Byte 0
    uint8_t eSPI_Cycle_Type : 8;               // Byte 0: eSPI Cycle Type: OOB Message = 21h
    // Byte 1
    uint8_t Length_High : 4;                   // 1: Length[11:8] = 0h
    uint8_t Tag : 4;                           // 1: Tag[3:0] = 0h
    // Byte 2
    uint8_t Length_Low : 8;                    // Length[7:0]
    // Byte 3
    uint8_t Reserved1 : 1;                      // Bit 0 reserved
    uint8_t Dest_Slave_Addr : 7;               // Dest Slave Addr[7:1] = 07h
    // Byte 4
    uint8_t Command_Code : 8;                   // Command Code = 0Fh
    // Byte 5
    uint8_t Byte_Count : 8;                     // Byte Count = 08h
    // Byte 6
    uint8_t Reserved2 : 1;                      // Bit 0 reserved
    uint8_t Source_Slave_Address : 7;           // Source Slave Address[7:1] = 08h
    // Byte 7
    uint8_t Header_Version : 4;                 // 7: Header Version = 0h
    uint8_t MCTP : 4;                           // 7: MCTP Reserved = 0h
    // Byte 8
    uint8_t Destination_Endpoint_ID : 8;        // Destination Endpoint ID
    // Byte 9
    uint8_t Source_Endpoint_ID : 8;             // Source Endpoint ID
    // Byte 10
    uint8_t Message_Tag : 3;                    // 10: Message Tag = 000b
    uint8_t TO : 1;                             // 10: TO = 0b
    uint8_t Packet_Seq : 2;                     // 10: Packet Seq # = 01b
    uint8_t EOM : 1;                            // 10: EOM = 1b
    uint8_t SOM : 1;                            // 10: SOM = 0b
    // Byte 11
    uint8_t Message_Type : 7;                   // 11: Message Type = 7Dh
    uint8_t IC : 1;                             // 11: IC
    // Byte 12
    uint8_t RPMC_Device : 8;        // Truncated Signature High (Byte 12)
    // Byte 13
    uint8_t Counter_Addr : 8;         // Truncated Signature Low (Byte 13)
    // Byte 14
    uint8_t Extended_Status : 8;         // Counter Data[7:0]

} eSPI_OOB_WriteRootKet_RESPONSE;

typedef struct
{
    // Byte 0
    uint8_t eSPI_Cycle_Type : 8;   // 0: eSPI Cycle Type: OOB Message = 21h
    // Byte 1
    uint8_t Length_High : 4;        // 1: Length[11:8] = 0h
    uint8_t Tag : 4;                // 1: Tag[3:0] = 0h
    // Byte 2
    uint8_t Length_Low : 8;         // 2: Length[7:0] = 48h
    // Byte 3
    uint8_t Reserved1 : 1;           // 3: Bit 0 reserved
    uint8_t Dest_Slave_Addr : 7;    // 3: Dest Slave Addr[7:1] = 07h
    // Byte 4
    uint8_t Command_Code : 8;        // 4: Command Code = 0Fh
    // Byte 5
    uint8_t Byte_Count : 8;          // 5: Byte Count = 45h
    // Byte 6
    uint8_t Reserved2 : 1;           // 6: Bit 0 reserved
    uint8_t Source_Slave_Addr : 7;   // 6: Source Slave Addr[7:1] = 08h
    // Byte 7
    uint8_t Header_Version : 4;      // 7: Header Version = 0h
    uint8_t MCTP : 4;       		 // 7: MCTP Reserved = 0h
    // Byte 8
    uint8_t Destination_Endpoint_ID : 8; // 8: Destination Endpoint ID
    // Byte 9
    uint8_t Source_Endpoint_ID : 8;  // 9: Source Endpoint ID
    // Byte 10
    uint8_t Message_Tag : 3;         // 10: Message Tag = 000b
    uint8_t TO : 1;                  // 10: TO = 0b
    uint8_t Packet_Seq : 2;          // 10: Packet Seq # = 00b
    uint8_t EOM : 1;                 // 10: EOM = 0b
    uint8_t SOM : 1;                 // 10: SOM = 1b
    // Byte 11
    uint8_t Message_Type : 7;        // 11: Message Type = 7Dh
    uint8_t IC : 1;                  // 11: IC
    // Byte 12
    uint8_t RPMC_Device : 8;         // 12: Reserved = 0h
    // Byte 13
    uint8_t Opcode : 8;              // 13: Opcode = 9Bh
    // Byte 14
    uint8_t Cmd_Type : 8;            // 14: Cmd Type = 00h
    // Byte 15
    uint8_t Counter_Addr : 8;        // 15: Counter Addr[7:0]
    // Byte 16
    uint8_t Rsvd : 8;        // 16: Counter Data[7:0]
    // Byte 17-20
    uint8_t Key_Data[4];
    // Byte 21-52
    uint8_t Signature[32];
} eSPI_OOB_UpdateHMACKey;

typedef struct {
    // Byte 0
    uint8_t eSPI_Cycle_Type : 8;               // Byte 0: eSPI Cycle Type: OOB Message = 21h
    // Byte 1
    uint8_t Length_High : 4;                   // 1: Length[11:8] = 0h
    uint8_t Tag : 4;                           // 1: Tag[3:0] = 0h
    // Byte 2
    uint8_t Length_Low : 8;                    // Length[7:0]
    // Byte 3
    uint8_t Reserved1 : 1;                      // Bit 0 reserved
    uint8_t Dest_Slave_Addr : 7;               // Dest Slave Addr[7:1] = 07h
    // Byte 4
    uint8_t Command_Code : 8;                   // Command Code = 0Fh
    // Byte 5
    uint8_t Byte_Count : 8;                     // Byte Count = 08h
    // Byte 6
    uint8_t Reserved2 : 1;                      // Bit 0 reserved
    uint8_t Source_Slave_Address : 7;           // Source Slave Address[7:1] = 08h
    // Byte 7
    uint8_t Header_Version : 4;                 // 7: Header Version = 0h
    uint8_t MCTP : 4;                           // 7: MCTP Reserved = 0h
    // Byte 8
    uint8_t Destination_Endpoint_ID : 8;        // Destination Endpoint ID
    // Byte 9
    uint8_t Source_Endpoint_ID : 8;             // Source Endpoint ID
    // Byte 10
    uint8_t Message_Tag : 3;                    // 10: Message Tag = 000b
    uint8_t TO : 1;                             // 10: TO = 0b
    uint8_t Packet_Seq : 2;                     // 10: Packet Seq # = 01b
    uint8_t EOM : 1;                            // 10: EOM = 1b
    uint8_t SOM : 1;                            // 10: SOM = 0b
    // Byte 11
    uint8_t Message_Type : 7;                   // 11: Message Type = 7Dh
    uint8_t IC : 1;                             // 11: IC
    // Byte 12
    uint8_t RPMC_Device : 8;        // Truncated Signature High (Byte 12)
    // Byte 13
    uint8_t Counter_Addr : 8;         // Truncated Signature Low (Byte 13)
    // Byte 14
    uint8_t Extended_Status : 8;         // Counter Data[7:0]

} eSPI_OOB_UpdateHMACKey_RESPONSE;

typedef struct
{
    // Byte 0
    uint8_t eSPI_Cycle_Type : 8;   // 0: eSPI Cycle Type: OOB Message = 21h
    // Byte 1
    uint8_t Length_High : 4;        // 1: Length[11:8] = 0h
    uint8_t Tag : 4;                // 1: Tag[3:0] = 0h
    // Byte 2
    uint8_t Length_Low : 8;         // 2: Length[7:0] = 48h
    // Byte 3
    uint8_t Reserved1 : 1;           // 3: Bit 0 reserved
    uint8_t Dest_Slave_Addr : 7;    // 3: Dest Slave Addr[7:1] = 07h
    // Byte 4
    uint8_t Command_Code : 8;        // 4: Command Code = 0Fh
    // Byte 5
    uint8_t Byte_Count : 8;          // 5: Byte Count = 45h
    // Byte 6
    uint8_t Reserved2 : 1;           // 6: Bit 0 reserved
    uint8_t Source_Slave_Addr : 7;   // 6: Source Slave Addr[7:1] = 08h
    // Byte 7
    uint8_t Header_Version : 4;      // 7: Header Version = 0h
    uint8_t MCTP : 4;       		 // 7: MCTP Reserved = 0h
    // Byte 8
    uint8_t Destination_Endpoint_ID : 8; // 8: Destination Endpoint ID
    // Byte 9
    uint8_t Source_Endpoint_ID : 8;  // 9: Source Endpoint ID
    // Byte 10
    uint8_t Message_Tag : 3;         // 10: Message Tag = 000b
    uint8_t TO : 1;                  // 10: TO = 0b
    uint8_t Packet_Seq : 2;          // 10: Packet Seq # = 00b
    uint8_t EOM : 1;                 // 10: EOM = 0b
    uint8_t SOM : 1;                 // 10: SOM = 1b
    // Byte 11
    uint8_t Message_Type : 7;        // 11: Message Type = 7Dh
    uint8_t IC : 1;                  // 11: IC
    // Byte 12
    uint8_t RPMC_Device : 8;         // 12: Reserved = 0h
    // Byte 13
    uint8_t Opcode : 8;              // 13: Opcode = 9Bh
    // Byte 14
    uint8_t Cmd_Type : 8;            // 14: Cmd Type = 00h
    // Byte 15
    uint8_t Counter_Addr : 8;        // 15: Counter Addr[7:0]
    // Byte 16
    uint8_t Rsvd : 8;        // 16: Counter Data[7:0]
    // Byte 17-20
    uint8_t Counter_Data[4];
    // Byte 21-52
    uint8_t Signature[32];
} eSPI_OOB_IncrementCounter;

typedef struct {
    // Byte 0
    uint8_t eSPI_Cycle_Type : 8;               // Byte 0: eSPI Cycle Type: OOB Message = 21h
    // Byte 1
    uint8_t Length_High : 4;                   // 1: Length[11:8] = 0h
    uint8_t Tag : 4;                           // 1: Tag[3:0] = 0h
    // Byte 2
    uint8_t Length_Low : 8;                    // Length[7:0]
    // Byte 3
    uint8_t Reserved1 : 1;                      // Bit 0 reserved
    uint8_t Dest_Slave_Addr : 7;               // Dest Slave Addr[7:1] = 07h
    // Byte 4
    uint8_t Command_Code : 8;                   // Command Code = 0Fh
    // Byte 5
    uint8_t Byte_Count : 8;                     // Byte Count = 08h
    // Byte 6
    uint8_t Reserved2 : 1;                      // Bit 0 reserved
    uint8_t Source_Slave_Address : 7;           // Source Slave Address[7:1] = 08h
    // Byte 7
    uint8_t Header_Version : 4;                 // 7: Header Version = 0h
    uint8_t MCTP : 4;                           // 7: MCTP Reserved = 0h
    // Byte 8
    uint8_t Destination_Endpoint_ID : 8;        // Destination Endpoint ID
    // Byte 9
    uint8_t Source_Endpoint_ID : 8;             // Source Endpoint ID
    // Byte 10
    uint8_t Message_Tag : 3;                    // 10: Message Tag = 000b
    uint8_t TO : 1;                             // 10: TO = 0b
    uint8_t Packet_Seq : 2;                     // 10: Packet Seq # = 01b
    uint8_t EOM : 1;                            // 10: EOM = 1b
    uint8_t SOM : 1;                            // 10: SOM = 0b
    // Byte 11
    uint8_t Message_Type : 7;                   // 11: Message Type = 7Dh
    uint8_t IC : 1;                             // 11: IC
    // Byte 12
    uint8_t RPMC_Device : 8;        // Truncated Signature High (Byte 12)
    // Byte 13
    uint8_t Counter_Addr : 8;         // Truncated Signature Low (Byte 13)
    // Byte 14
    uint8_t Extended_Status : 8;         // Counter Data[7:0]

} eSPI_OOB_IncrementCounter_RESPONSE;

typedef struct
{
    // Byte 0
    uint8_t eSPI_Cycle_Type : 8;   // 0: eSPI Cycle Type: OOB Message = 21h
    // Byte 1
    uint8_t Length_High : 4;        // 1: Length[11:8] = 0h
    uint8_t Tag : 4;                // 1: Tag[3:0] = 0h
    // Byte 2
    uint8_t Length_Low : 8;         // 2: Length[7:0] = 48h
    // Byte 3
    uint8_t Reserved1 : 1;           // 3: Bit 0 reserved
    uint8_t Dest_Slave_Addr : 7;    // 3: Dest Slave Addr[7:1] = 07h
    // Byte 4
    uint8_t Command_Code : 8;        // 4: Command Code = 0Fh
    // Byte 5
    uint8_t Byte_Count : 8;          // 5: Byte Count = 45h
    // Byte 6
    uint8_t Reserved2 : 1;           // 6: Bit 0 reserved
    uint8_t Source_Slave_Addr : 7;   // 6: Source Slave Addr[7:1] = 08h
    // Byte 7
    uint8_t Header_Version : 4;      // 7: Header Version = 0h
    uint8_t MCTP : 4;       		 // 7: MCTP Reserved = 0h
    // Byte 8
    uint8_t Destination_Endpoint_ID : 8; // 8: Destination Endpoint ID
    // Byte 9
    uint8_t Source_Endpoint_ID : 8;  // 9: Source Endpoint ID
    // Byte 10
    uint8_t Message_Tag : 3;         // 10: Message Tag = 000b
    uint8_t TO : 1;                  // 10: TO = 0b
    uint8_t Packet_Seq : 2;          // 10: Packet Seq # = 00b
    uint8_t EOM : 1;                 // 10: EOM = 0b
    uint8_t SOM : 1;                 // 10: SOM = 1b
    // Byte 11
    uint8_t Message_Type : 7;        // 11: Message Type = 7Dh
    uint8_t IC : 1;                  // 11: IC
    // Byte 12
    uint8_t RPMC_Device : 8;         // 12: Reserved = 0h
    // Byte 13
    uint8_t Opcode : 8;              // 13: Opcode = 9Bh
    // Byte 14
    uint8_t Cmd_Type : 8;            // 14: Cmd Type = 00h
    // Byte 15
    uint8_t Counter_Addr : 8;        // 15: Counter Addr[7:0]
    // Byte 16
    uint8_t Rsvd : 8;        // 16: Counter Data[7:0]
    // Byte 17-28
    uint8_t Tag_Arr[12];
    // Byte 21-52
    uint8_t Signature[32];
} eSPI_OOB_RequestCounter;

typedef struct {
    // Byte 0
    uint8_t eSPI_Cycle_Type : 8;               // Byte 0: eSPI Cycle Type: OOB Message = 21h
    // Byte 1
    uint8_t Length_High : 4;                   // 1: Length[11:8] = 0h
    uint8_t Tag : 4;                           // 1: Tag[3:0] = 0h
    // Byte 2
    uint8_t Length_Low : 8;                    // Length[7:0]
    // Byte 3
    uint8_t Reserved1 : 1;                      // Bit 0 reserved
    uint8_t Dest_Slave_Addr : 7;               // Dest Slave Addr[7:1] = 07h
    // Byte 4
    uint8_t Command_Code : 8;                   // Command Code = 0Fh
    // Byte 5
    uint8_t Byte_Count : 8;                     // Byte Count = 08h
    // Byte 6
    uint8_t Reserved2 : 1;                      // Bit 0 reserved
    uint8_t Source_Slave_Address : 7;           // Source Slave Address[7:1] = 08h
    // Byte 7
    uint8_t Header_Version : 4;                 // 7: Header Version = 0h
    uint8_t MCTP : 4;                           // 7: MCTP Reserved = 0h
    // Byte 8
    uint8_t Destination_Endpoint_ID : 8;        // Destination Endpoint ID
    // Byte 9
    uint8_t Source_Endpoint_ID : 8;             // Source Endpoint ID
    // Byte 10
    uint8_t Message_Tag : 3;                    // 10: Message Tag = 000b
    uint8_t TO : 1;                             // 10: TO = 0b
    uint8_t Packet_Seq : 2;                     // 10: Packet Seq # = 01b
    uint8_t EOM : 1;                            // 10: EOM = 1b
    uint8_t SOM : 1;                            // 10: SOM = 0b
    // Byte 11
    uint8_t Message_Type : 7;                   // 11: Message Type = 7Dh
    uint8_t IC : 1;                             // 11: IC
    // Byte 12
    uint8_t RPMC_Device : 8;        // Truncated Signature High (Byte 12)
    // Byte 13
    uint8_t Counter_Addr : 8;         // Truncated Signature Low (Byte 13)
    // Byte 14
    uint8_t Extended_Status : 8;         // Counter Data[7:0]
    // Byte 15-26
    uint8_t Tag_Arr[12];
    // Byte 27-30
    uint8_t CounterReadData[4];
    // Byte 31-62
    uint8_t Signature[32];
} eSPI_OOB_RequestCounter_RESPONSE;

typedef struct {
    // Byte 0
    uint8_t eSPI_Cycle_Type : 8;               // Byte 0: eSPI Cycle Type: OOB Message = 21h
    // Byte 1
    uint8_t Length_High : 4;                   // 1: Length[11:8] = 0h
    uint8_t Tag : 4;                           // 1: Tag[3:0] = 0h
    // Byte 2
    uint8_t Length_Low : 8;                    // Length[7:0]
    // Byte 3
    uint8_t Reserved1 : 1;                      // Bit 0 reserved
    uint8_t Dest_Slave_Addr : 7;               // Dest Slave Addr[7:1] = 07h
    // Byte 4
    uint8_t Command_Code : 8;                   // Command Code = 0Fh
    // Byte 5
    uint8_t Byte_Count : 8;                     // Byte Count = 08h
    // Byte 6
    uint8_t Reserved2 : 1;                      // Bit 0 reserved
    uint8_t Source_Slave_Address : 7;           // Source Slave Address[7:1] = 08h
    // Byte 7
    uint8_t Header_Version : 4;                 // 7: Header Version = 0h
    uint8_t MCTP : 4;                           // 7: MCTP Reserved = 0h
    // Byte 8
    uint8_t Destination_Endpoint_ID : 8;        // Destination Endpoint ID
    // Byte 9
    uint8_t Source_Endpoint_ID : 8;             // Source Endpoint ID
    // Byte 10
    uint8_t Message_Tag : 3;                    // 10: Message Tag = 000b
    uint8_t TO : 1;                             // 10: TO = 0b
    uint8_t Packet_Seq : 2;                     // 10: Packet Seq # = 01b
    uint8_t EOM : 1;                            // 10: EOM = 1b
    uint8_t SOM : 1;                            // 10: SOM = 0b
    // Byte 11
    uint8_t Message_Type : 7;                   // 11: Message Type = 7Dh
    uint8_t IC : 1;                             // 11: IC
    // Byte 12
    uint8_t RPMC_Device : 8;                    // 12: Reserved = 0h
    // Byte 13
    uint8_t Opcode : 8;                         // 13: Opcode = 9Bh

} eSPI_OOB_ReadParameters;

typedef struct {
    // Byte 0
    uint8_t eSPI_Cycle_Type : 8;               // Byte 0: eSPI Cycle Type: OOB Message = 21h
    // Byte 1
    uint8_t Length_High : 4;                   // 1: Length[11:8] = 0h
    uint8_t Tag : 4;                           // 1: Tag[3:0] = 0h
    // Byte 2
    uint8_t Length_Low : 8;                    // Length[7:0]
    // Byte 3
    uint8_t Reserved1 : 1;                      // Bit 0 reserved
    uint8_t Dest_Slave_Addr : 7;               // Dest Slave Addr[7:1] = 07h
    // Byte 4
    uint8_t Command_Code : 8;                   // Command Code = 0Fh
    // Byte 5
    uint8_t Byte_Count : 8;                     // Byte Count = 08h
    // Byte 6
    uint8_t Reserved2 : 1;                      // Bit 0 reserved
    uint8_t Source_Slave_Address : 7;           // Source Slave Address[7:1] = 08h
    // Byte 7
    uint8_t Header_Version : 4;                 // 7: Header Version = 0h
    uint8_t MCTP : 4;                           // 7: MCTP Reserved = 0h
    // Byte 8
    uint8_t Destination_Endpoint_ID : 8;        // Destination Endpoint ID
    // Byte 9
    uint8_t Source_Endpoint_ID : 8;             // Source Endpoint ID
    // Byte 10
    uint8_t Message_Tag : 3;                    // 10: Message Tag = 000b
    uint8_t TO : 1;                             // 10: TO = 0b
    uint8_t Packet_Seq : 2;                     // 10: Packet Seq # = 01b
    uint8_t EOM : 1;                            // 10: EOM = 1b
    uint8_t SOM : 1;                            // 10: SOM = 0b
    // Byte 11
    uint8_t Message_Type : 7;                   // 11: Message Type = 7Dh
    uint8_t IC : 1;                             // 11: IC
    // Byte 12
    uint8_t Extended_Status : 8;        //  (Byte 12)
    // Byte 13
    uint8_t __padding_for_align[3];     // bytes 13-15: 确保下面的 uint32_t 从偏移 16 开始
    // Byte 16-19
    uint32_t RPMC_ParameterTable;         //
    // Byte 20-23
    uint32_t RPMC_Parameters_Device0;     //

} eSPI_OOB_ReadParameters_RESPONSE;

extern eSPI_OOB_WriteRootKet_MESSAGE1 eRPMC_WriteRootKey_m1;
extern eSPI_OOB_WriteRootKet_MESSAGE2 eRPMC_WriteRootKey_m2;
extern eSPI_OOB_WriteRootKet_RESPONSE eRPMC_WriteRootKey_data;
extern eSPI_OOB_UpdateHMACKey eRPMC_UpdateHMACKey;
extern eSPI_OOB_UpdateHMACKey_RESPONSE eRPMC_UpdateHMACKey_data;
extern eSPI_OOB_IncrementCounter eRPMC_IncrementCounter;
extern eSPI_OOB_IncrementCounter_RESPONSE eRPMC_IncrementCounter_data;
extern eSPI_OOB_RequestCounter eRPMC_RequestCounter;
extern eSPI_OOB_RequestCounter_RESPONSE eRPMC_RequestCounter_data;
extern eSPI_OOB_ReadParameters eRPMC_ReadParameters;
extern eSPI_OOB_ReadParameters_RESPONSE eRPMC_ReadParameters_data;
extern BYTE RPMC_OOB_TempArr[80];
//-----------------------------------------------------------------------------
// (33xxh) EC Interface Registers, eSI Queue 0 - PUT_PC DATA , PUT_OOB DATA
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// (34xxh) EC Interface Registers, eSI Queue 1 - UPSTEAM DATA
//-----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
extern void Service_OOB_Message(void);
extern void Service_Peripheral_Message(void);
extern BYTE Process_eSPI_OOB_Message(void);
extern void Process_eSPI_OOB_CrashLog(void);
extern void Get_OOB_RTC_Time(void);
extern void Get_OOB_PCH_Temperature(void);
extern void Get_OOB_PMC_CrashLog(void);

extern BYTE eSPI_Flash_Read(BYTE add3, BYTE add2, BYTE add1, BYTE add0,
    BYTE length, VBYTE* bufferindex);
extern BYTE eSPI_Flash_Erase(BYTE addr3, BYTE addr2, BYTE addr1, BYTE addr0,
    BYTE mode);
extern BYTE eSPI_Flash_Write(BYTE addr3, BYTE addr2, BYTE addr1, BYTE addr0,
    BYTE length, VBYTE* bufferindex);

extern BYTE OOB_PECI_RdPkgConfig(BYTE addr, BYTE* ReadData, BYTE Domain,
    BYTE Retry, BYTE Index, BYTE LSB,
    BYTE MSB, BYTE ReadLen, BYTE WriteLen);
extern BYTE OOB_PECI_WrPkgConfig(BYTE addr, BYTE* WriteData, BYTE Domain,
    BYTE Retry, BYTE Index, BYTE LSB,
    BYTE MSB, BYTE ReadLen, BYTE WriteLen);

extern void OOB_PECI_ReadPowerUnit(void);
extern void OOB_PECI_ReadPowerLimit1(void);
extern void OOB_PECI_WritePowerLimit1(BYTE SettingWatts, BYTE SettingTimer);
extern void OOB_PECI_ReadPowerLimit2(void);
extern void OOB_PECI_WritePowerLimit2(BYTE SettingWatts);
extern void OOB_PECI_ReadPowerLimit3(void);
extern void OOB_PECI_WritePowerLimit3(BYTE SettingWatts);
extern void OOB_PECI_ReadPowerLimit4(void);
extern void OOB_PECI_WritePowerLimit4(BYTE SettingWatts);
extern void Service_eSPI(void);
extern BYTE EC_ACK_eSPI_Boot_Ready(void);
extern void ESPI_Init(void);
extern void EC_ACK_eSPI_SUS_WARN(void);
extern BYTE Process_Peripheral_Message_Send(void);
extern BYTE Process_Peripheral_Memory_Read32(void);
extern BYTE eSPI_OOBRPMC_Handler(void);
extern BYTE eSPI_OOB_Receive(BYTE* OOB_Meg_Table);
extern void eRPMC_WriteRootKey_Response(void);
extern void eRPMC_UpdateHMACKey_Response(void);
extern void eRPMC_IncrementCounter_Response(void);
extern void eRPMC_RequestCounter_Response(void);
extern void eRPMC_ReadParameter_Response(void);
extern BYTE eSPI_OOB_Send(BYTE* OOB_Meg_Table);
#endif