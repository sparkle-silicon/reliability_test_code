/*
 * @Author: Linyu
 * @LastEditors: daweslinyu
 * @LastEditTime: 2024-09-19 10:05:22
 * @Description:
 *
 *
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 以下是中文及英文版权同步声明，编码为UTF-8。
 * Copyright has legal effects and violations will be prosecuted.
 * 版权具有法律效力，违反必究。
 *
 * Copyright ©2021-2025 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2025龙晶石半导体科技（苏州）有限公司
 *
 * Portions of this firmware library utilize the ANX7447 driver, which is copyrighted by Analogix Semiconductor, Inc.
 * 本固件库的部分代码使用了 ANX7447 驱动程序，其版权归 Analogix Semiconductor, Inc. 所有。
 */
#include "AE_ANX7447_UCSI.H"
#include "AE_ANX7447_LIB.H"
#include "KERNEL_MEMORY.H"

#if SUPPORT_ANX7447

#define AUTO_RDO_ENABLE

#define PATCH_NOT_CALIBRATED_CHIP

#define _BIT0   0x01
#define _BIT1   0x02
#define _BIT2   0x04
#define _BIT3   0x08
#define _BIT4   0x10
#define _BIT5   0x20
#define _BIT6   0x40
#define _BIT7   0x80

/*Register definition of device address 0x58*/
#define TCPC_ROLE_CONTROL        0x1A
#define DRP_CONTROL              _BIT6
#define RP_VALUE                 (_BIT5|_BIT4)
#define CC2_CONTROL              (_BIT3|_BIT2)
#define CC1_CONTROL              (_BIT1|_BIT0)
#define DRP_EN                   _BIT6  // DRP_CONTROL

#define TCPC_COMMAND             0x23

#define ANALOG_CTRL_6            0xA6
#define ADC_CTRL_2               0xC0

#define ANALOG_CTRL_0            0xA0
#define DFP_OR_UFP               _BIT6

#define INTR_ALERT_0             0xCB
#define INTR_RECEIVED_MSG        _BIT7
#define INTR_SOFTWARE_INT        _BIT6

#define INTR_ALERT_1             0xCC
#define INTR_INTP_POW_ON         _BIT7  // digital powerup indicator
#define INTR_INTP_POW_OFF        _BIT6


#define TCPC_CTRL_2              0xCD
#define SOFT_INTP_1              _BIT1

#define  TCPC_CONTROL            0x19

#define  POWER_CONTROL           0x1C

#define TX_OBJ1_BYTE_0           0x54

#define VBUS_VOLTAGE_0           0x70
#define VBUS_VOLTAGE_BIT7_0      0xFF

#define VBUS_VOLTAGE_1           0x71
#define VBUS_VOLTAGE_BIT9_8      (_BIT0 | _BIT1)

#define  PD_1US_PERIOD           0x80
#define  PD_TX_BIT_PERIOD        0x86

#define ANALOG_CTRL_1            0xA1
#define R_TOGGLE_ENABLE          _BIT7
#define R_LATCH_TOGGLE_ENABLE    _BIT6
#define TOGGLE_CTRL_MODE         _BIT5
#define CC2_VRD_USB              _BIT2
#define CC2_VRD_1P5              _BIT1
#define CC2_VRD_3P0              _BIT0

#define  ANALOG_CTRL_9           0xA9

#define ANALOG_CTRL_10           0xAA
#define FRSWAP_CTRL              0xAB
#define FR_SWAP                  _BIT7
#define FR_SWAP_EN               _BIT6
#define R_FRSWAP_CONTROL_SELECT  _BIT3
#define R_SIGNAL_FRSWAP          _BIT2
#define TRANSMIT_FRSWAP_SIGNAL   _BIT1
#define FRSWAP_DETECT_ENABLE     _BIT0

#define RING_OSC_CTRL            0xD9
#define VBUS_OCP_0               0xE6
#define VBUS_OCP_1               0xE7
#define VBUS_OCP_BIT9_8          (_BIT0 | _BIT1)

/***************************************************************/
/*Register definition of device address 0x7a*/
#define TX_DATA_BYTE_30          0x00

/***************************************************************/
/*Register definition of device address 0x7e*/
#define R_RAM_LEN_H              0x03
#define FLASH_ADDR_EXTEND        _BIT7

#define R_RAM_CTRL               0x05
#define FLASH_DONE               _BIT7
#define BOOT_LOAD_DONE           _BIT6
#define LOAD_CRC_OK              _BIT5  // CRC_OK
#define LOAD_DONE                _BIT4

#define R_FLASH_ADDR_H           0x0c
#define R_FLASH_ADDR_L           0x0d

#define FLASH_WRITE_DATA_0       0xe
#define FLASH_READ_DATA_0        0x3c

#define R_FLASH_LEN_H            0x2e
#define R_FLASH_LEN_L            0x2f

#define R_FLASH_RW_CTRL          0x30
#define GENERAL_INSTRUCTION_EN   _BIT6
#define FLASH_ERASE_EN           _BIT5
#define WRITE_STATUS_EN          _BIT2
#define FLASH_READ               _BIT1
#define FLASH_WRITE              _BIT0

#define R_FLASH_STATUS_0         0x31

#define  FLASH_INSTRUCTION_TYPE  0x33
#define FLASH_ERASE_TYPE         0x34

#define R_FLASH_STATUS_REGISTER_READ_0  0x35
#define WIP                      _BIT0

#define R_I2C_0                  0x5C
#define read_Status_en           _BIT7

#define  OCM_CTRL_0              0x6e
#define OCM_RESET                _BIT6

#define ADDR_GPIO_CTRL_0         0x88
#define SPI_WP                   _BIT7
#define SPI_CLK_ENABLE           _BIT6
/*
* For SKIP highest voltage
* Maximum Voltage for Request Data Object
* 100mv units
*/
#define MAX_VOLTAGE              0xAC
/*
* For selection PDO
* Maximum Power for Request Data Object
* 500mW units
*/
#define MAX_POWER                0xAD
/*
* For mismatch
* Minimum Power for Request Data Object
* 500mW units
*/
#define MIN_POWER                0xAE
/*Show Maximum voltage of RDO*/
#define RDO_MAX_VOLTAGE          0xAF
/*Show Maximum Powe of RDO*/
#define RDO_MAX_POWER            0xB0
/*Show Maximum current of RDO*/
#define RDO_MAX_CURRENT          0xB1

#define FIRMWARE_CTRL                0xB2
#define disable_usb30                _BIT0
#define auto_pd_en                   _BIT1
#define trysrc_en                    _BIT2
#define trysnk_en                    _BIT3
#define support_goto_min_power       _BIT4
#define snk_remove_refer_cc          _BIT5
#define force_send_rdo               _BIT6
#define high_voltage_for_same_power  _BIT7

#define FW_STATE_MACHINE        0xB3

#define OCM_VERSION_REG         0xB4

#define INT_MASK                0xB6
/*same with 0x28 interrupt mask*/
#define CHANGE_INT              0xB7
#define OCM_BOOT_UP             _BIT0
#define OC_OV_EVENT             _BIT1 // OC/OV protection  LBT-399
#define VCONN_CHANGE            _BIT2
#define VBUS_CHANGE             _BIT3
#define CC_STATUS_CHANGE        _BIT4
#define DATA_ROLE_CHANGE        _BIT5
#define PR_CONSUMER_GOT_POWER   _BIT6
#define HPD_STATUS_CHANGE       _BIT7

#define SYSTEM_STSTUS           0xB8
/*0: SINK off; 1: SINK on */
#define SINK_STATUS             _BIT1
/*0: VCONN off; 1: VCONN on*/
#define VCONN_STATUS            _BIT2
/*0: vbus off; 1: vbus on*/
#define VBUS_STATUS             _BIT3
/*1: host; 0:device*/
#define S_DATA_ROLE             _BIT5
/*0: Chunking; 1: Unchunked*/
#define SUPPORT_UNCHUNKING      _BIT6
/*0: HPD low; 1: HPD high*/
#define HPD_STATUS              _BIT7

#define NEW_CC_STATUS           0xB9

// PD Revision configure
// 0: default, 1:PD_REV20, 2:PD_REV30
#define PD_REV_INIT             0xBA

#define PD_EXT_MSG_CTRL         0xBB
#define SRC_CAP_EXT_REPLY       _BIT0
#define MANUFACTURER_INFO_REPLY _BIT1
#define BATTERY_STS_REPLY       _BIT2
#define BATTERY_CAP_REPLY       _BIT3
#define ALERT_REPLY             _BIT4
#define STATUS_REPLY            _BIT5
#define PPS_STATUS_REPLY        _BIT6
#define SNK_CAP_EXT_REPLY       _BIT7

#define PD_MIN_POWER   0x02     /* 1W */

// Battery Status
struct battery_status_data_obj{
    u8 reserved;                // Reserved
    u8 battery_info;            // Battery Info
    u16 battery_pc;             // Battery PC
};

// Alert Message
struct alert_data_obj{
    u16 reserved : 16;            // Reserved
    u8 hot_swappable_batteries : 4;       // Battery Info
    u8 fixed_batteries : 4;       // Battery Info
    u8 type_of_alert : 8;         // Battery PC
};

// Source Capabilities Extended
struct source_cap_extended{
    u16 VID;
    u16 PID;
    u32 XID;
    u8 fw_version;
    u8 hw_version;
    u8 voltage_regulation;
    u8 holdup_time;
    u8 compliance;
    u8 touch_current;
    u16 peak_current1;
    u16 peak_current2;
    u16 peak_current3;
    u8 touch_temp;
    u8 source_inputs;
    u8 batteries;
    u8 source_PDP;
};

// Sink Capabilities Extended
struct sink_cap_extended{
    u16 VID;
    u16 PID;
    u32 XID;
    u8 fw_version;
    u8 hw_version;
    u8 Reserved[11];
    u8 sink_inputs;
    u8 batteries;
    u8 sink_PDP;
};


// Status Message
struct status_data{
    u8 internal_temp;
    u8 present_input;
    u8 present_battery_input;
    u8 event_flags;
    u8 temperature_status;
    //Fix JIRA LBT-564
    u8 power_status;
};

// Battery Cap
struct battery_cap{
    u16 VID;
    u16 PID;
    u16 battery_design_cap;
    u16 battery_last_full_charge_cap;
    u8 battery_type;
};

// Manufacturer Info
struct manufacturer_info{
    u8 manufacturer_info_target;
    u8 manufacturer_info_ref;
};

// Manufacturer Info Data
struct manufacturer_info_data{
    u16 VID;
    u16 PID;
    u8 manufacturer_string[22];
};

// PPS Status Message
struct pps_status_data{
    u16 output_voltage;
    u8 output_current;
    u8 real_time_flags;
};

struct ext_message_header{
    u16 data_size : 9;            // Data Size
    u16 reserved : 1;             // Reserved
    u16 request_chunk : 1;        // Request Chunk
    u16 chunk_number : 4;         // Chunk Number
    u16 chunked : 1;              // Chunked
};

#define BYTE_SWAP(word) do { (word) = ((word)<<8)|((word)>>8); } while(0)
//low byte + high byte
#define USB_PD_EXT_HEADER(sendbuf, size, request, number, chunk) \
    do { \
        ((struct ext_message_header *)sendbuf)->data_size = size; \
        ((struct ext_message_header *)sendbuf)->reserved = 0; \
        ((struct ext_message_header *)sendbuf)->request_chunk = request; \
        ((struct ext_message_header *)sendbuf)->chunk_number = number; \
        ((struct ext_message_header *)sendbuf)->chunked = chunk; \
     } while(0)


static u8 interface_send_ctr_msg();
static u8 interface_send_msg();
// static u8 send_pd_msg(PD_MSG_TYPE type, u8 *buf, u8 size, u8 type_sop);
#ifdef UCSI_USED_STRUCTURE_MEMORY
char ucsi_base_mem[256];
char anxlib_base_mem[512];

#define ucsi_ppm_state  *(static enum UCSI_PPM_STATE ANXXDATA *)(UCSI_baseaddr + 0x0)
#define ucsi_errno *(u8 ANXXDATA *)( UCSI_baseaddr + 0x1)
#define self_charging_error *(u8 ANXXDATA *)( UCSI_baseaddr + 0x5)
#define esd *(static struct error_status_data ANXXDATA *)( UCSI_baseaddr + 0x6)
#define connector_change_need_ack *(static u8 ANXXDATA *)( UCSI_baseaddr + 0x16)
#define error_need_ack *(static u8 ANXXDATA *)( UCSI_baseaddr + 0x17)
#define command_complete_need_ack *(static u8 ANXXDATA *)( UCSI_baseaddr + 0x18)
#define need_ack_command *(static u8 ANXXDATA *)( UCSI_baseaddr + 0x19)
#define random_data *(u8 ANXXDATA *)( UCSI_baseaddr + 0x1A)
#define registered_connector_index *(static u8 ANXXDATA *)( UCSI_baseaddr + 0x1B)
#define timer1msCount *(u8 ANXXDATA *)( UCSI_baseaddr + 0x1c)
#define ANX7447_i *(u8 ANXXDATA *)( UCSI_baseaddr + 0x1F)
#define ANX7447_j *(u8 ANXXDATA *)( UCSI_baseaddr + 0x20)
#define ANX7447_k *(u8 ANXXDATA *)( UCSI_baseaddr + 0x21)
#define ANX7447_l *(u8 ANXXDATA *)( UCSI_baseaddr + 0x22)
#define DataLength *(u8 ANXXDATA *)( UCSI_baseaddr + 0x23)
#define wait_for_ack_back *(u8 ANXXDATA *)( UCSI_baseaddr + 0x24)
/* reserved 0x25 ~ 0x27 */
#define current_pending_command_timeout *(u8 *)( UCSI_baseaddr + 0x28)
#define ucsi_async_checking_timer *(u16 *)( UCSI_baseaddr + 0x29)
#define ucsi_async_restore *(u8 *)( UCSI_baseaddr + 0x2B)
#define ucsi_command_process_delay *(u8 *)( UCSI_baseaddr + 0x2C)
#define ne *(struct notification_enable_status ANXXDATA *)( UCSI_baseaddr + 0x2F)
/* reserved 0x38 ~ 0x76 */
#define result *(u8 ANXXDATA *)( UCSI_baseaddr + 0x77)
#define port_index *(u8 ANXXDATA *)( UCSI_baseaddr + 0x78)
#define ucsi_received_command *(u8 ANXXDATA *)( UCSI_baseaddr + 0x79)
#define role *(u8 ANXXDATA *)( UCSI_baseaddr + 0x7a)
#define ANX7447_c *(u8 ANXXDATA *)( UCSI_baseaddr + 0x7b)
#define len *(u8 ANXXDATA *)( UCSI_baseaddr + 0x7c)
#define count *(u16 ANXXDATA *)( UCSI_baseaddr + 0x7d)
#define port_id *(u8 ANXXDATA *)( UCSI_baseaddr + 0x7f)

// u8 *InterfaceSendBuf[PD_MAX_INSTANCE] = (u8 *)(UCSI_baseaddr + 0x80);//InterfaceSendBuf[PD_MAX_INSTANCE][INTERFACE_SEND_BUF_SIZE]
// u8 *InterfaceRecvBuf[PD_MAX_INSTANCE] = (u8 *)(UCSI_baseaddr + 0xc0);//InterfaceRecvBuf[PD_MAX_INSTANCE][INTERFACE_RECV_BUF_SIZE]
#define pup *(struct ppm_ucsi_parameters ANXXDATA *)( UCSI_baseaddr + 0x1c0)

// u8 *context = (struct anx_context ANXXDATA *)(ANXLIB_baseaddr + 0x0);//context[PD_MAX_INSTANCE]
#define ppm2bios *(struct bios_share_msg ANXXDATA *)( ANXLIB_baseaddr + 0x1b2)
#define SWAP_BYTE *(u8 ANXXDATA *)( ANXLIB_baseaddr + 0x1e2)
#define supply_1500ma_flag *(u8 ANXXDATA *)( ANXLIB_baseaddr + 0x1e3)
#define supply_1500ma_port *(u8 ANXXDATA *)( ANXLIB_baseaddr + 0x1e4)
// u8 8request_src_caps_flag = (u8 ANXXDATA *)(ANXLIB_baseaddr + 0x1e5)//request_src_caps_flag[2] 
#define pd_fw_updated *(u8 ANXXDATA *)( ANXLIB_baseaddr + 0x1e7)
#define set_pdr_port_id *(u8 ANXXDATA *)( ANXLIB_baseaddr + 0x1e8)
#define set_uor_port_id *(u8 ANXXDATA *)( ANXLIB_baseaddr + 0x1e9)
#define change_int *(u8 ANXXDATA *)( ANXLIB_baseaddr + 0x1f0)
#define change_status *(u8 ANXXDATA *)( ANXLIB_baseaddr + 0x1f1)
#define intr_alert_0 *(u8 ANXXDATA *)( ANXLIB_baseaddr + 0x1f2)
#define intr_alert_1 *(u8 ANXXDATA *)( ANXLIB_baseaddr + 0x1f3)
#define pdo_h *(u16 ANXXDATA *)( ANXLIB_baseaddr + 0x1f4)
#define pdo_l *(u16 ANXXDATA *)( ANXLIB_baseaddr + 0x1f6)
#define pdo_max *(u32 ANXXDATA *)( ANXLIB_baseaddr + 0x1f8)
#define pdo_max_tmp *(u32 ANXXDATA *)( ANXLIB_baseaddr + 0x1fc)
#else
extern u8 InterfaceSendBuf[PD_MAX_INSTANCE][INTERFACE_SEND_BUF_SIZE];
extern u8 InterfaceRecvBuf[PD_MAX_INSTANCE][INTERFACE_RECV_BUF_SIZE];
extern struct anx_context ANXXDATA context[PD_MAX_INSTANCE];
extern u8 ANXXDATA ANX7447_i;
extern u8 ANXXDATA ANX7447_j;
extern u8 ANXXDATA ANX7447_k;
extern u8 ANXXDATA ANX7447_l;
extern struct ppm_ucsi_parameters ANXXDATA pup;
extern u8 ANXXDATA role;
extern u8 ANXXDATA ANX7447_c;
extern u8 ANXXDATA len;
extern u16 ANXXDATA count;
extern u8 ANXXDATA port_id;
extern u8 ANXXDATA change_int;
extern u8 ANXXDATA change_status;
extern u8 ANXXDATA intr_alert_0;
extern u8 ANXXDATA intr_alert_1;
extern u16 ANXXDATA pdo_h;
extern u16 ANXXDATA pdo_l;
extern u32 ANXXDATA pdo_max;
extern u32 ANXXDATA pdo_max_tmp;
extern u8 ANXXDATA supply_1500ma_flag;
extern u8 ANXXDATA supply_1500ma_port;
extern u8 ANXXDATA random_data;
extern u8 ANXXDATA timer1msCount;
extern u8 ANXXDATA self_charging_error;
extern u8 ANXXDATA request_src_caps_flag[];
#endif
/* Fix JIRA LBT-501 */
#define HW_REV       0x01
#define SI_REV       0x04
#define FW_MAJOR_REV 0x02
#define FW_MINOR_REV 0x01


#define BYTE_CONST_SWAP(word) ((unsigned short int)(((word)<<8)|((word)>>8)))
/***Define Global Variables***/
// Battery Cap - Invalid Battery
const struct battery_cap pd_battery_cap_invalid = {
    /* fix JIRA LBT-563 */
#ifdef EC_ANX_BIG_ENDIAN
    BYTE_CONST_SWAP(VENDOR_ID), // VID
    BYTE_CONST_SWAP(PRODUCT_ID),        // PID
#else
    VENDOR_ID,                  //VID
    PRODUCT_ID,                 //PID
#endif
    0xFFFF, 0xFFFF, 1
};

// Battery Cap
struct battery_cap pd_battery_cap;

//struct battery_cap recv_battery_cap;

// Battery Status - Invalid Battery
const struct battery_status_data_obj pd_battery_status_invalid = {
    0, 1, 0xFFFF                // Reserved, Invalid Battery reference, Battery'sSOC unknown
};

// Battery Status
struct battery_status_data_obj pd_battery_status = { 0, 0x01, 0xFFFF };

//struct battery_status_data_obj  recv_battery_status;

// Alert Message
struct alert_data_obj pd_alert_data_obj = {
    0,                          // Reserved
    0,                          // Hot Swappable Battery 0~3
    1,                          // Fixed Battery 0~3
    2                           // Battery Status Change
};

//struct alert_data_obj  recv_alert_data_obj;

// Get Battery Status
//unsigned char  pd_get_battery_status_ref = 4; // hot swap battery 0

// Source Capabilities Extended
const struct source_cap_extended pd_source_cap_extended = {
    /* fix JIRA LBT-563 */
#ifdef EC_ANX_BIG_ENDIAN
    BYTE_CONST_SWAP(VENDOR_ID), // VID
    BYTE_CONST_SWAP(PRODUCT_ID),        // PID
#else
    VENDOR_ID,                  //VID
    PRODUCT_ID,                 //PID
#endif
    0,                          // XID
    FW_MAJOR_REV,               // FW Ver
    HW_REV,                     // HW Ver
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0x0,                        //battery number
    CUST_PDP                    //PDP
};

//struct source_cap_extended  recv_source_cap_extended;

// Sink Capabilities Extended
const struct sink_cap_extended pd_sink_cap_extended = {
    /* fix JIRA LBT-563 */
#ifdef EC_ANX_BIG_ENDIAN
    BYTE_CONST_SWAP(VENDOR_ID), // VID
    BYTE_CONST_SWAP(PRODUCT_ID),        // PID
#else
    VENDOR_ID,                  //VID
    PRODUCT_ID,                 //PID
#endif
    0,                          // XID
    FW_MAJOR_REV,               // FW Ver
    HW_REV,                     // HW Ver
    // ....
};

//struct sink_cap_extended  recv_sink_cap_extended;

// Status Message
struct status_data pd_status_data = {
    0,                          // not supported
    2,                          // External DC power
    0,                          // no battery
    0,                          // Event Flags
    0,                          // not Supported
    0                           // power status, fix JIRA LBT-564
};

//struct status_data  recv_status_data;

// Get Battery Cap
//unsigned char  pd_get_battery_cap_ref = 4; // hot swap battery 0

// Manufacturer Info
const struct manufacturer_info pd_manufacturer_info = {
    0,                          // Port/Cable
    0                           // Not battery
};

// Manufacturer Info Data
struct manufacturer_info_data pd_manufacturer_info_data = {
    /* fix JIRA LBT-563 */
#ifdef EC_ANX_BIG_ENDIAN
    BYTE_CONST_SWAP(VENDOR_ID), // VID
    BYTE_CONST_SWAP(PRODUCT_ID),        // PID
#else
    VENDOR_ID,                  //VID
    PRODUCT_ID,                 //PID
#endif
    "Analogix(MCU)"             // String
};

//struct manufacturer_info_data  recv_manufacturer_info_data;
//Country code
const unsigned char pd_country_code_data[3 * 2 + 2] = {
    0x03, 0x00,                 // Number of country codes in the message
    'A', 'U',
    'B', 'R',
    'C', 'A',
};

//Country info
const unsigned char pd_country_code_info[8] = {  //for test
    'A', 'U',
    0x00, 0x00,                 //reserved
    0x00, 0x01,
    0x02, 0x02,
};

// PPS Status Message
const struct pps_status_data pd_pps_status_data = {
    0xFFFF,                     // not supported
    0xFF,                       // not supported
    0,                          // PTF: 00 ?V Not Supported, Constant Voltage mode
};

//struct pps_status_data  recv_pps_status_data;

const u32 dfp_caps = PDO_FIXED(PD_VOLTAGE_5V, PD_CURRENT_1500MA,
                              PDO_FIXED_DATA_SWAP | PDO_FIXED_EXTERNAL);
const u32 src_caps =
PDO_FIXED(PD_VOLTAGE_5V, PD_CURRENT_1500MA, PDO_FIXED_COMM_CAP);
const u32 dfp_caps_default_ma = PDO_FIXED(PD_VOLTAGE_5V, PD_CURRENT_1500MA,
                               PDO_FIXED_DATA_SWAP |
                               PDO_FIXED_EXTERNAL);
const u32 src_caps_default_ma =
PDO_FIXED(PD_VOLTAGE_5V, PD_CURRENT_1500MA, PDO_FIXED_COMM_CAP);
const u8 set_variable_1[] = { IF_VAR_fw_var_reg, 0x00, 0x02, 0x01, 0x0d };

const u8 src_pdo[] = { 0x5A, 0x90, 0x01, 0x2A, 0x96, 0x90, 0x01, 0x2A };

const u8 snk_identity[] = {
    /* JIRA LBT-501 */
    VENDOR_ID & 0xFF, (VENDOR_ID >> 8) & 0xFF, 0x00, 0x82,       /* snk_id_hdr */
    0x00, 0x00, 0x00, 0x00,                                      /* snk_cert */
    0x00, 0x00, PRODUCT_ID & 0xFF, (PRODUCT_ID >> 8) & 0xFF,     /* 5snk_ama */
};
const u8 enter_exit_svid[] = { 0xff, 0x01, 0x01, 0x00 };

/* Fix JIRA LBT-603 */
const u32 snk_cap[] = { PDO_FIXED(PD_VOLTAGE_5V, PD_CURRENT_900MA, PDO_FIXED_COMM_CAP) | PDO_FIXED_FAST_SWAP | PDO_FIXED_DATA_SWAP,    /*5V, 0.9A, Fixed */
                       PDO_VAR(PD_VOLTAGE_5V, PD_MAX_VOLTAGE_20V, PD_CURRENT_3A)   /*5V, 20V, 3A, variable */
};

//JIRA LBT-411
const u8 dp_source_dp_caps[4] = { 0xC6, 0x00, 0x00, 0x00 };

#define is_unchunked() (anx_read_reg(context[port_id].anx_spi, SYSTEM_STSTUS) & SUPPORT_UNCHUNKING)
#define EXT_HEADER_CHUNKED      1
#define EXT_HEADER_UNCHUNKED    0

s8 *result_to_str(u8 result_type)
{
    return (s8 *)((result_type == CMD_SUCCESS) ? "Accept" :
        (result_type == CMD_FAIL) ? "Fail" :
        (result_type == CMD_BUSY) ? "Busy" :
        (result_type == CMD_REJECT) ? "Reject" : "Unknown");
}

s8 *interface_to_str(u8 header_type)
{
#if 1
    s8 *str;
    switch (header_type)
    {
        case TYPE_PWR_SRC_CAP:
            str = (s8 *)"PWR_SRC_CAP";
            break;
        case TYPE_PWR_SNK_CAP:
            str = (s8 *)"PWR_SNK_CAP";
            break;
        case TYPE_PWR_OBJ_REQ:
            str = (s8 *)"PWR_OBJ_REQ";
            break;
        case TYPE_DP_SNK_IDENTITY:
            str = (s8 *)"DP_SNK_IDENTITY";
            break;
        case TYPE_SVID:
            str = (s8 *)"SVID";
            break;
        case TYPE_PSWAP_REQ:
            str = (s8 *)"PSWAP_REQ";
            break;
        case TYPE_DSWAP_REQ:
            str = (s8 *)"DSWAP_REQ";
            break;
        case TYPE_GOTO_MIN_REQ:
            str = (s8 *)"GOTO_MIN_REQ";
            break;
        case TYPE_DP_ALT_ENTER:
            str = (s8 *)"DPALT_ENTER";
            break;
        case TYPE_DP_ALT_EXIT:
            str = (s8 *)"DPALT_EXIT";
            break;
        case TYPE_GET_SRC_CAP:
            str = (s8 *)"GET_SRC_CAP";
            break;
        case TYPE_GET_RDO:
            str = (s8 *)"GET_RDO";
            break;
        case TYPE_GET_SNK_CAP:
            str = (s8 *)"GET_SNK_CAP";
            break;
        case TYPE_VCONN_SWAP_REQ:
            str = (s8 *)"VCONN_SWAP_REQ";
            break;
        case TYPE_GET_DP_SNK_CAP:
            str = (s8 *)"GET_DP_SINK_CAP";
            break;
        case TYPE_DP_SNK_CFG:
            str = (s8 *)"DP_SNK_CFG";
            break;
        case TYPE_SOFT_RST:
            str = (s8 *)"Software Reset";
            break;
        case TYPE_HARD_RST:
            str = (s8 *)"Hardware Reset";
            break;
        case TYPE_RESTART:
            str = (s8 *)"Restart";
            break;
        case TYPE_GET_PD_STATUS:
            str = (s8 *)"PD_STATUS_REQ";
            break;
        case TYPE_ACCEPT:
            str = (s8 *)"ACCEPT";
            break;
        case TYPE_REJECT:
            str = (s8 *)"REJECT";
            break;
        case TYPE_VDM:
            str = (s8 *)"VDM";
            break;
        case TYPE_RESPONSE_TO_REQ:
            str = (s8 *)"RESPONSE_TO_REQ";
            break;
        case TYPE_EXT_SRC_CAP_EXT:
            str = (s8 *)"PD3_SRC_CAP";
            break;
        case TYPE_EXT_STS:
            str = (s8 *)"PD3_STS";
            break;
        case TYPE_EXT_GET_BATT_CAP:
            str = (s8 *)"PD3_GET_BATT_CAP";
            break;
        case TYPE_EXT_GET_BATT_STS:
            str = (s8 *)"PD3_GET_BATT_STS";
            break;
        case TYPE_EXT_BATT_CAP:
            str = (s8 *)"PD3_BATT_CAP";
            break;
        case TYPE_EXT_GET_MFR_INFO:
            str = (s8 *)"PD3_GET_MFR_INFO";
            break;
        case TYPE_EXT_MFR_INFO:
            str = (s8 *)"PD3_MFR_INFO";
            break;
        case TYPE_EXT_PDFU_REQUEST:
            str = (s8 *)"PD3_PDFU_REQUEST";
            break;
        case TYPE_EXT_PDFU_RESPONSE:
            str = (s8 *)"PD3_PDFU_RESPONSE";
            break;
        case TYPE_BATT_STS:
            str = (s8 *)"PD3_BATT_STS";
            break;
        case TYPE_ALERT:
            str = (s8 *)"PD3_ALERT";
            break;
        case TYPE_NOT_SUPPORTED:
            str = (s8 *)"PD3_NOT_SUPPORTED";
            break;
        case TYPE_GET_SRC_CAP_EXT:
            str = (s8 *)"PD3_GET_SRC_CAP";
            break;
        case TYPE_GET_STS:
            str = (s8 *)"PD3_GET_SRC_STS";
            break;
        case TYPE_FR_SWAP:
            str = (s8 *)"PD3_FR_SWAP";
            break;
        case TYPE_GET_PPS_STS:
            str = (s8 *)"PD3_GET_PPS_STS";
            break;
        case TYPE_FR_SWAP_SIGNAL:
            str = (s8 *)"PD3_FR_SWAP_SIGNAL";
            break;
        case TYPE_GET_COUNTRY_INFO:
            str = (s8 *)"PD3_GET_COUNTRY_INFO";
            break;
        case TYPE_DP_DISCOVER_MODES_INFO:
            str = (s8 *)"TYPE_DP_DISCOVER_MODES_INFO";
            break;
        case TYPE_GET_DP_CONFIGURE:
            str = (s8 *)"TYPE_GET_DP_CONFIGURE";
            break;
        case TYPE_DP_CONFIGURE:
            str = (s8 *)"TYPE_DP_CONFIGURE";
            break;
        case TYPE_I2C_MASTER_WRITE:
            str = (s8 *)"TYPE_I2C_MASTER_WRITE";
            break;
        case TYPE_GET_COUNTRY_CODE:
            str = (s8 *)"PD3_GET_COUNTRY_CODE";
            break;
        case TYPE_GET_SINK_CAP_EXT:
            str = (s8 *)"PD3_GET_SINK_CAP_EXT";
            break;
        case TYPE_EXT_SINK_CAP_EXT:
            str = (s8 *)"PD3_EXT_SINK_CAP_EXT";
            break;
        case TYPE_EXT_PPS_STS:
            str = (s8 *)"PD3_EXT_PPS_CAP_STS";
            break;
        case TYPE_EXT_COUNTRY_INFO:
            str = (s8 *)"PD3_EXT_COUNTRY_INFO";
            break;
        case TYPE_EXT_COUNTRY_CODE:
            str = (s8 *)"PD3_EXT_COUNTRY_CODE";
            break;
        case TYPE_GET_VAR:
            str = (s8 *)"TYPE_GET_VAR";
            break;
        case TYPE_SET_VAR:
            str = (s8 *)"TYPE_SET_VAR";
            break;
        default:
            str = (s8 *)"Unknown";
            break;
    }
#endif
    return str;
}

u8 get_data_role()
{
    /*fetch the data role */
    ANX7447_k = anx_read_reg(context[port_id].anx_spi, SYSTEM_STSTUS);

    return (ANX7447_k & S_DATA_ROLE) != 0;

}


u8 get_power_role()
{
    /*fetch the power role */
    ANX7447_k = anx_read_reg(context[port_id].anx_spi, SYSTEM_STSTUS);

    return (ANX7447_k & VBUS_STATUS) != 0;
}

#define InterfaceSendBuf_Addr 0xC0
#define InterfaceRecvBuf_Addr 0xE0

#define recvd_msg_len()          ((context[port_id].InterfaceRecvBuf[0] & 0x1F) - 1)
#define recvd_msg_sop_type()     (context[port_id].InterfaceRecvBuf[0]  >> 6 )
#define recvd_msg_type()         (context[port_id].InterfaceRecvBuf[1])
#define recvd_msg_buf()          (&context[port_id].InterfaceRecvBuf[2])
#define send_msg_len()           (context[port_id].InterfaceSendBuf[0] & 0x1F)
#define RESPONSE_REQ_TYPE()      (context[port_id].InterfaceRecvBuf[2])
#define RESPONSE_REQ_RESULT()    (context[port_id].InterfaceRecvBuf[3])

/* define max request current 3A and voltage 5V */
#define MAX_REQUEST_VOLTAGE 20000
#define MAX_REQUEST_CURRENT 3000
#define set_rdo_value(v0, v1, v2, v3)   \
    do {                \
        context[port_id].pd_rdo[0] = (v0);  \
        context[port_id].pd_rdo[1] = (v1);  \
        context[port_id].pd_rdo[2] = (v2);  \
        context[port_id].pd_rdo[3] = (v3);  \
    } while (0)

#define plug_orientation(port) (anx_read_reg(context[port].anx_spi,NEW_CC_STATUS)& 0x0d)

/* default request max RDO */
void build_rdo_from_source_caps_new(u8 obj_cnt, u8 *buf)
{
    if (context[port_id].sel_voltage_pdo_index + 1 > obj_cnt)
    {
        for (ANX7447_i = 0; ANX7447_i < obj_cnt; ANX7447_i++)
        {
            {
                ((unsigned char *)&pdo_max_tmp)[0] = buf[4 * ANX7447_i + 3];
                ((unsigned char *)&pdo_max_tmp)[1] = buf[4 * ANX7447_i + 2];
                ((unsigned char *)&pdo_max_tmp)[2] = buf[4 * ANX7447_i + 1];
                ((unsigned char *)&pdo_max_tmp)[3] = buf[4 * ANX7447_i + 0];
                switch (GET_PDO_TYPE(pdo_max_tmp))
                {
                    case (PDO_TYPE_FIXED >> 30):
                        if ((GET_PDO_FIXED_VOLT(pdo_max_tmp)) > pdo_max)
                        {
                            pdo_max = GET_PDO_FIXED_VOLT(pdo_max_tmp);
                            context[port_id].sel_voltage_pdo_index = ANX7447_i;
                        }
                        break;
                    case (PDO_TYPE_VARIABLE >> 30):
                        if (GET_VAR_MAX_VOLT(pdo_max_tmp) > pdo_max)
                        {
                            pdo_max = GET_VAR_MAX_VOLT(pdo_max_tmp);
                            context[port_id].sel_voltage_pdo_index = ANX7447_i;
                        }
                        break;
                    case (PDO_TYPE_BATTERY >> 30):
                        if (GET_BATT_MAX_VOLT(pdo_max_tmp) > pdo_max)
                        {
                            pdo_max = GET_BATT_MAX_VOLT(pdo_max_tmp);
                            context[port_id].sel_voltage_pdo_index = ANX7447_i;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
    else
    {
        ((unsigned char *)&pdo_max_tmp)[0] =
            buf[context[port_id].sel_voltage_pdo_index * 4 + 3];
        ((unsigned char *)&pdo_max_tmp)[1] =
            buf[context[port_id].sel_voltage_pdo_index * 4 + 2];
        ((unsigned char *)&pdo_max_tmp)[2] =
            buf[context[port_id].sel_voltage_pdo_index * 4 + 1];
        ((unsigned char *)&pdo_max_tmp)[3] =
            buf[context[port_id].sel_voltage_pdo_index * 4 + 0];
        switch (GET_PDO_TYPE(pdo_max_tmp))
        {
            case (PDO_TYPE_FIXED >> 30):
                pdo_max = GET_PDO_FIXED_VOLT(pdo_max_tmp);
                break;
            case (PDO_TYPE_VARIABLE >> 30):
                pdo_max = GET_VAR_MAX_VOLT(pdo_max_tmp);
                break;
            case (PDO_TYPE_BATTERY >> 30):
                pdo_max = GET_BATT_MAX_VOLT(pdo_max_tmp);
                break;
            default:
                break;
        }
    }
    ((unsigned char *)&pdo_max_tmp)[0] =
        buf[context[port_id].sel_voltage_pdo_index * 4 + 3];
    ((unsigned char *)&pdo_max_tmp)[1] =
        buf[context[port_id].sel_voltage_pdo_index * 4 + 2];
    ((unsigned char *)&pdo_max_tmp)[2] =
        buf[context[port_id].sel_voltage_pdo_index * 4 + 1];
    ((unsigned char *)&pdo_max_tmp)[3] =
        buf[context[port_id].sel_voltage_pdo_index * 4 + 0];
    switch (GET_PDO_TYPE(pdo_max_tmp))
    {
        case (PDO_TYPE_FIXED >> 30):
        case (PDO_TYPE_VARIABLE >> 30):
            if (GET_PDO_FIXED_CURR(pdo_max_tmp) >= MAX_REQUEST_CURRENT)
            {
                context[port_id].ucsi_partner_rdo =
                    RDO_FIXED(context[port_id].sel_voltage_pdo_index + 1,
                              MAX_REQUEST_CURRENT, MAX_REQUEST_CURRENT, 0);
            }
            else
            {
                context[port_id].ucsi_partner_rdo =
                    RDO_FIXED(context[port_id].sel_voltage_pdo_index + 1,
                              GET_PDO_FIXED_CURR(pdo_max_tmp),
                              GET_PDO_FIXED_CURR(pdo_max_tmp),
                              RDO_CAP_MISMATCH);
            }
            break;
        case (PDO_TYPE_BATTERY >> 30):
            if ((GET_BATT_OP_POWER(pdo_max_tmp) >> 2) >=
                    (MAX_REQUEST_CURRENT * (MAX_REQUEST_VOLTAGE / 1000)) >> 2)
            {
                context[port_id].ucsi_partner_rdo =
                    RDO_BATT(context[port_id].sel_voltage_pdo_index + 1,
                             MAX_REQUEST_CURRENT * (MAX_REQUEST_VOLTAGE /
                                 1000),
                             MAX_REQUEST_CURRENT * (MAX_REQUEST_VOLTAGE /
                                 1000), 0);
            }
            else
            {
                context[port_id].ucsi_partner_rdo =
                    RDO_BATT(context[port_id].sel_voltage_pdo_index + 1,
                             (unsigned long)(GET_BATT_OP_POWER(pdo_max_tmp) >>
                                 2) * 4,
                             (unsigned long)(GET_BATT_OP_POWER(pdo_max_tmp) >>
                                 2) * 4, RDO_CAP_MISMATCH);
            }
            break;
        default:
            break;
    }


    //set_rdo_value(pd_rdo,sel_pdo & 0xff,(sel_pdo >> 8) & 0xff, (sel_pdo >> 16) & 0xff, (sel_pdo >> 24) & 0xff);

}

#ifndef AUTO_RDO_ENABLE
static u8 build_rdo_from_source_caps(u8 obj_cnt, u8 *buf)
{
    pdo_max = 0;
    obj_cnt &= 0x07;

    /* find the max voltage pdo */
    for (ANX7447_i = 0; ANX7447_i < obj_cnt; ANX7447_i++)
    {

/* get max voltage now */
        pdo_max_tmp =
            (u16)((((((((u16)buf[ANX7447_i * 4 + 3] << 8) | (buf[ANX7447_i * 4 + 2])) &
                0xf) << 6) | ((((u16)buf[ANX7447_i * 4 + 1] << 8) |
                    (buf[ANX7447_i * 4 + 0])) >> 10)) & 0x3ff) *
                   50);
        if (pdo_max_tmp > pdo_max)
        {
            pdo_max = pdo_max_tmp;
            pdo_l = (((u16)buf[ANX7447_i * 4 + 1] << 8) | (buf[ANX7447_i * 4 + 0]));
            pdo_h = (((u16)buf[ANX7447_i * 4 + 3] << 8) | (buf[ANX7447_i * 4 + 2]));
            context[port_id].sel_voltage_pdo_index = ANX7447_i;
        }
    }
    ucsi_debug("maxV=%d, cnt %d index %d\n", pdo_max_tmp, obj_cnt,
               context[port_id].sel_voltage_pdo_index);
    if ((pdo_h & (3 << 14)) != (PDO_TYPE_BATTERY >> 16))
    {
        ucsi_debug("maxMa %d\n", (u16)((pdo_l & 0x3ff) * 10));
        /* less than 900mA */
        if ((u16)((pdo_l & 0x3ff) * 10) < MAX_REQUEST_CURRENT)
        {
            pdo_max =
                RDO_FIXED(context[port_id].sel_voltage_pdo_index + 1,
                          (u16)((pdo_l & 0x3ff) * 10),
                          (u16)((pdo_l & 0x3ff) * 10), 0);
            pdo_max |= RDO_CAP_MISMATCH;
            set_rdo_value(pdo_max & 0xff, (pdo_max >> 8) & 0xff,
                          (pdo_max >> 16) & 0xff, (pdo_max >> 24) & 0xff);
            return 1;
        }
        else
        {
            pdo_max =
                RDO_FIXED(context[port_id].sel_voltage_pdo_index + 1,
                          MAX_REQUEST_CURRENT, MAX_REQUEST_CURRENT, 0);
            set_rdo_value(pdo_max & 0xff, (pdo_max >> 8) & 0xff,
                          (pdo_max >> 16) & 0xff, (pdo_max >> 24) & 0xff);

            return 1;
        }
    }
    else
    {
        pdo_max =
            RDO_FIXED(context[port_id].sel_voltage_pdo_index + 1,
                      MAX_REQUEST_CURRENT, MAX_REQUEST_CURRENT, 0);
        set_rdo_value(pdo_max & 0xff, (pdo_max >> 8) & 0xff,
                      (pdo_max >> 16) & 0xff, (pdo_max >> 24) & 0xff);
        return 1;
    }
    ucsi_debug("RDO Mismatch !!!\n");
    set_rdo_value(0x0A, 0x28, 0x00, 0x10);

    return 0;
}
#endif

static u32 change_bit_order(const u8 *pbuf)
{
    return ((u32)pbuf[3] << 24) | ((u32)pbuf[2] << 16)
        | ((u32)pbuf[1] << 8) | pbuf[0];
}

static u8 pd_check_requested_voltage()
{
    if (!(context[port_id].ucsi_partner_rdo >> 28)
            || (context[port_id].ucsi_partner_rdo >> 28) >
            context[port_id].pd_src_pdo_cnt)
    {
        ucsi_debug
        ("rdo = %x, Requested RDO is %d, Provided RDO number is %d\n",
         context[port_id].ucsi_partner_rdo,
         (u16)(context[port_id].ucsi_partner_rdo >> 28),
         (u8)context[port_id].pd_src_pdo_cnt);
        return 0;               /* Invalid index */
    }
    ucsi_debug("pdo_max = %x\n",
               (change_bit_order
               (context[port_id].pd_src_pdo +
                   (((context[port_id].ucsi_partner_rdo >> 28) -
                       1) * 4)) & 0x3ff));
        /* check current ... */
    if (((context[port_id].ucsi_partner_rdo >> 10) & 0x3FF) > (change_bit_order(context[port_id].pd_src_pdo + (((context[port_id].ucsi_partner_rdo >> 28) - 1) * 4)) & 0x3ff))  //Update to pass TD.PD.SRC.E12 Reject Request
        return 0;               /* too much op current */
    if ((context[port_id].ucsi_partner_rdo & 0x3FF) > (change_bit_order(context[port_id].pd_src_pdo + (((context[port_id].ucsi_partner_rdo >> 28) - 1) * 4)) & 0x3ff))  //Update to pass TD.PD.SRC.E12 Reject Request
        return 0;               /* too much max current */

    return 1;
}

void send_svid(void)
{
    u8 vdm_data[4];
    vdm_data[0] = 0;
    vdm_data[1] = 0;
    vdm_data[2] = CUST_SVID & 0xFF;
    vdm_data[3] = (CUST_SVID >> 8) & 0xFF;

    send_pd_msg(TYPE_SVID, vdm_data, 4, SOP_TYPE);
}

void send_source_capability(void)
{
#ifdef ANX_OHO_DFP_ONLY
    memcpy(context[port_id].pd_src_pdo, (u8 *)&dfp_caps,
           sizeof(dfp_caps));
#else
#ifdef ECR_CHECKING
    if ((supply_1500ma_port == port_id) && (supply_1500ma_flag == 1))
        memcpy(context[port_id].pd_src_pdo, (u8 *)&src_caps, 4);
    else
    #endif
        memcpy(context[port_id].pd_src_pdo, (u8 *)&src_caps_default_ma,
               4);
#endif
#ifdef EC_ANX_BIG_ENDIAN
    SWAP_DWORD(&context[port_id].pd_src_pdo);
#endif
    //Fix JIRA LBT-410
    if ((context[port_id].data_operation_mode_uor & OP_DRP) &&
            ((context[port_id].data_operation_mode_uom & OP_DRP)))
    {
        if ((context[port_id].power_operation_mode_pdr & PW_DRP) &&
                ((context[port_id].power_operation_mode_pdm & PW_DRP)))
            context[port_id].pd_src_pdo[3] |=
            PDO_FIXED_HIGH_BYTE_DATA_SWAP |
            PDO_FIXED_HIGH_BYTE_DUAL_ROLE;
        else
            context[port_id].pd_src_pdo[3] |=
            PDO_FIXED_HIGH_BYTE_DATA_SWAP;
    }
    else
    {
        if ((context[port_id].power_operation_mode_pdr & PW_DRP) &&
                ((context[port_id].power_operation_mode_pdm & PW_DRP)))
            context[port_id].pd_src_pdo[3] |=
            PDO_FIXED_HIGH_BYTE_DUAL_ROLE;
        else;                   //default is no drp, no dual role
    }
    context[port_id].pd_src_pdo_cnt = 1;
    send_pd_msg(TYPE_CMD_SRC_CAP_1, (u8 *)&context[port_id].pd_src_pdo,
                4, SOP_TYPE);
    ucsi_debug("Resend 1.5A source caps byte 3 is %#x.\n",
               context[port_id].pd_src_pdo[3]);
}

void send_sink_capability(void)
{
    memcpy(context[port_id].pd_snk_pdo, (u8 *)&snk_cap, 8);
#ifdef EC_ANX_BIG_ENDIAN
    SWAP_DWORD(&context[port_id].pd_snk_pdo);
    SWAP_DWORD((u8 *)&context[port_id].pd_snk_pdo + 4);
#endif
    //Fix JIRA LBT-410
    if ((context[port_id].data_operation_mode_uor & OP_DRP) &&
            ((context[port_id].data_operation_mode_uom & OP_DRP)))
    {
        if ((context[port_id].power_operation_mode_pdr & PW_DRP) &&
                ((context[port_id].power_operation_mode_pdm & PW_DRP)))
            context[port_id].pd_snk_pdo[3] |=
            PDO_FIXED_HIGH_BYTE_DATA_SWAP |
            PDO_FIXED_HIGH_BYTE_DUAL_ROLE;
        else
            context[port_id].pd_snk_pdo[3] |=
            PDO_FIXED_HIGH_BYTE_DATA_SWAP;
    }
    else
    {
        if ((context[port_id].power_operation_mode_pdr & PW_DRP) &&
                ((context[port_id].power_operation_mode_pdm & PW_DRP)))
            context[port_id].pd_snk_pdo[3] |=
            PDO_FIXED_HIGH_BYTE_DUAL_ROLE;
        else;                   //default is no drp, no dual role
    }
    ucsi_debug("Resend 1.5A sink caps, sink caps byte 3 is %#x.\n",
               context[port_id].pd_snk_pdo[3]);
    context[port_id].pd_snk_pdo_cnt = 2;
    send_pd_msg(TYPE_PWR_SNK_CAP, (u8 *)&context[port_id].pd_snk_pdo,
                8, SOP_TYPE);
}

void send_source_sink_capability(void)
{
    send_sink_capability();
    send_source_capability();
}

/*
 * @desc   : anx_get_batt_cap, EC call it to get partner's battery capability
 * @param  : battery_id -> battery id number
 * @return : none
 */
void anx_get_batt_cap(u8 battery_id)
{
    send_pd_msg(TYPE_EXT_GET_BATT_CAP, &battery_id, 1, 0);
}

/*
 * @desc   : anx_get_batt_sts, EC call it to get partner's battery status
 * @param  : battery_id -> battery id number
 * @return : none
 */
void anx_get_batt_sts(u8 battery_id)
{
    send_pd_msg(TYPE_EXT_GET_BATT_STS, &battery_id, 1, SOP_TYPE);
}

/*
 * @desc   : anx_get_batt_cap_response, EC call it to send battery capability to partner
 * @param  : battery_id 	-> battery id number
 * @param  : design_cap	-> design capacity
 * @param  : last_full_charge_cap -> last full charge capacity
 * @return : none
 */
void anx_get_batt_cap_response(u8 battery_id, u16 design_cap,
                               u16 last_full_charge_cap)
{
    if (battery_id == 0)
    {      //battery 0
//customer may need change the following code to match battery present status
        pd_battery_cap.VID = BYTE_CONST_SWAP(VENDOR_ID);
        pd_battery_cap.PID = BYTE_CONST_SWAP(PRODUCT_ID);
        pd_battery_cap.battery_design_cap = BYTE_CONST_SWAP(design_cap);
        pd_battery_cap.battery_last_full_charge_cap =
            BYTE_CONST_SWAP(last_full_charge_cap);
        pd_battery_cap.battery_type = 0;
        send_pd_msg(TYPE_EXT_BATT_CAP, (u8 *)&(pd_battery_cap), 9,
                    SOP_TYPE);
    }
    else
    {
        send_pd_msg(TYPE_EXT_BATT_CAP, (u8 *)&(pd_battery_cap_invalid),
                    9, SOP_TYPE);
    }
}

/*
 * @desc   : anx_get_batt_sts_response, EC call it to send battery status to partner
 * @param  : battery_id  	-> battery id number
 * @param  : battery_pc 	-> battery present capacity
 * @return : none
 */
void anx_get_batt_sts_response(u8 battery_id, u16 battery_pc)
{
    if (battery_id == 0)
    {
//customer may need change the following code to match battery present status
        if (context[port_id].anx_power_role == 0)
            pd_battery_status.battery_info = 2;
        else
            pd_battery_status.battery_info = 6;
        pd_battery_status.battery_pc = BYTE_CONST_SWAP(battery_pc);     //45w / 2
        pd_battery_status.reserved = 0;
        send_pd_msg(TYPE_BATT_STS, (u8 *)&(pd_battery_status), 4,
                    SOP_TYPE);
    }
    else
    {
        send_pd_msg(TYPE_BATT_STS, (u8 *)&(pd_battery_status_invalid), 4,
                    SOP_TYPE);
    }
}

/*
 * @desc   : anx_send_pd_battery_alert, EC call it to alert partner its battery status changed
 * @param  : battery_id -> battery id number (0, 1, 2, 3), other: invalid
 * @return : none
 */
void anx_send_pd_battery_alert(u8 battery_id)
{
    //customer may need change the following code to match battery present status
    pd_alert_data_obj.fixed_batteries = (1 << battery_id);
    pd_alert_data_obj.hot_swappable_batteries = 0;
    /* battery alert */
    pd_alert_data_obj.type_of_alert = 2;
    send_pd_msg(TYPE_ALERT, (u8 *)&pd_alert_data_obj, 4, 0);
}

void anx_get_sts_response(void)
{
    //customer may need change the following code to match battery present status
    pd_status_data.temperature_status = 0;
    pd_status_data.present_input = 2;
    pd_status_data.present_battery_input = 0;
    pd_status_data.power_status = 0;
    pd_status_data.internal_temp = 0;
    pd_status_data.event_flags = 0;
    send_pd_msg(TYPE_EXT_STS, (unsigned char *)&pd_status_data, 6,
                SOP_TYPE);
}

#ifdef ECR_CHECKING
static void Send_mismatch_source_caps(void)
{
    ucsi_debug("Send mismatch source caps, 1.5A source caps.\n");
    supply_1500ma_flag = 1;
    supply_1500ma_port = port_id;

    send_source_capability();
}
#endif

/*ChengLin: To integrate all recv_pd_xxxxxx_default_callback() functions to single one for reduce code size*/
static u8 Recv_PD_Commands_Default_Callback()
{
    u8 *pdo;
    u16  svid;//usb_vendor_id,usb_product_id,

    ANX7447_l = 1;                      // 0: Fail, 1: Pass

    if (TYPE_HARD_RST != recvd_msg_type())
        context[port_id].ct = CABLE_TYPE_C;

    /*ChengLin: Special Check for Specific Commands */
    if (recvd_msg_type() == TYPE_PWR_SRC_CAP)
    {
        if (recvd_msg_len() % 4 != 0)
            return 0;
    }
    else if (recvd_msg_type() == TYPE_PWR_SNK_CAP)
    {
    }
    else if (recvd_msg_type() == TYPE_PWR_OBJ_REQ)
    {
        if (recvd_msg_len() != 4)
            return 1;
    }

    switch (recvd_msg_type())
    {
        case TYPE_PWR_SRC_CAP:     //0x00
        #ifndef AUTO_RDO_ENABLE
            build_rdo_from_source_caps_new(recvd_msg_len() / 4,
                                           recvd_msg_buf());
            set_rdo_value(context[port_id].ucsi_partner_rdo & 0xff,
                          ((context[port_id].ucsi_partner_rdo >> 8) & 0xff),
                          ((context[port_id].ucsi_partner_rdo >> 16) & 0xff),
                          ((context[port_id].ucsi_partner_rdo >> 24) & 0xff))
                send_pd_msg(TYPE_PWR_OBJ_REQ, &context[port_id].pd_rdo, 4,
                            SOP_TYPE);
        #endif
            build_rdo_from_source_caps_new(recvd_msg_len() / 4,
                                           recvd_msg_buf());
            send_pd_msg(TYPE_GET_RDO, 0, 0, SOP_TYPE);
            send_svid();
            context[port_id].partner_pdo_length = recvd_msg_len();
            context[port_id].partner_pdo_sink_or_source = 1;        //partner as source
            context[port_id].anx_power_role = 0;
            /*voltage->0x7e:0xaf, current:0x7e:0xb1 */
            context[port_id].anx_current_in_50ma =
                anx_read_reg(context[port_id].anx_spi, 0xb1);
            context[port_id].anx_voltage_in_100mv =
                anx_read_reg(context[port_id].anx_spi, 0xaf);
            context[port_id].anx_power =
                (context[port_id].anx_current_in_50ma *
                 context[port_id].anx_voltage_in_100mv) / 200;
        #ifdef ReportNegotiatedPowerLevel
            context[port_id].csc.csc.NegotiatedPowerLevel = 1;
        #endif
        #ifdef ReportSupportedProviderCap
            context[port_id].csc.csc.SupportedProviderCap = 1;
        #endif
        #ifdef ReportExternalSupply
            context[port_id].csc.csc.ExternalSupply = 1;
        #endif
                /* LBT-865 */
            if (context[port_id].anx_power >= VERY_SLOW_CHARGING_POWER)
            {
                if (context[port_id].anx_power >= NORMAL_CHARGING_POWER)
                    context[port_id].BatteryCharging = BCS_NOMINAL_CHARGING;
                else if (context[port_id].anx_power >= SLOW_CHARGING_POWER)
                    context[port_id].BatteryCharging = BCS_SLOW_CHARGING;
                else
                    context[port_id].BatteryCharging = BCS_TRICKLE_CHARGING;

                context[port_id].csc.csc.BatteryChargingStatus = 1;
            }
            ucsi_debug("cur:%x, vol:%x, power:%x rdo :%lx, rdo length(%#x)\n",
                       (u16)context[port_id].anx_current_in_50ma,
                       (u16)context[port_id].anx_voltage_in_100mv,
                       context[port_id].anx_power,
                       context[port_id].ucsi_partner_rdo,
                       context[port_id].partner_pdo_length);
            ucsi_async_notify_raise_up(100);
            memcpy(context[port_id].partner_pdo, recvd_msg_buf(),
                   min(VDO_SIZE, recvd_msg_len()));
            anx_get_peer_source_pdo_event(port_id, recvd_msg_buf(),
                                             min(VDO_SIZE, recvd_msg_len()));
            break;
        case TYPE_PWR_SNK_CAP:     //0x01
            //received peer's sink caps, just store it in SINK_PDO array
            context[port_id].partner_pdo_length = recvd_msg_len();
            context[port_id].partner_pdo_sink_or_source = 0;
            memcpy(context[port_id].partner_pdo, recvd_msg_buf(),
                   min(VDO_SIZE, recvd_msg_len()));
            if (recvd_msg_len() > VDO_SIZE)
            {
                ANX7447_l = 0;
                break;
            }
            break;
        case TYPE_DP_SNK_IDENTITY: //0x02
            pdo = (u8 *)recvd_msg_buf();
            context[port_id].usb_vendor_id = (u16)pdo[1] << 8 | (u16)pdo[0];
            context[port_id].usb_product_id =
                (u16)pdo[11] << 8 | (u16)pdo[10];

            ucsi_debug("usb vendor id(0x%x), product id(0x%x).\n",
                       context[port_id].usb_vendor_id,
                       context[port_id].usb_product_id);
            context[port_id].usb_state = pdo[12] & 0x3;
            break;
        case TYPE_SVID:            //0x03
            break;
        case TYPE_ACCEPT:          //0x05
            break;
        case TYPE_REJECT:          //0x06
            break;
        case TYPE_PSWAP_REQ:       //0x10
            context[port_id].power_role_swap_flag = 1;
            context[port_id].anx_power_role = get_power_role();
            context[port_id].csc.csc.PowerDirection = 1;
            ucsi_async_notify_raise_up(50);
            break;
        case TYPE_DSWAP_REQ:       //0x11
            context[port_id].csc.csc.ConnectorPartner = 1;
            if (get_data_role() == 1)
                context[port_id].ucsi_connector_partner_type =
                CPT_UFP_ATTACHED;
            else
                context[port_id].ucsi_connector_partner_type =
                CPT_DFP_ATTACHED;
            ucsi_async_notify_raise_up(50);
            break;
        case TYPE_GOTO_MIN_REQ:    //0x12
            break;
        case TYPE_DP_SNK_CFG:
            break;
        case TYPE_DP_DISCOVER_MODES_INFO:
            /* fix JIRA LBT-591 */
            context[port_id].peer_svid_length += 4;
        #ifdef ReportSupportedCAM
            context[port_id].csc.csc.SupportedCAM = 1;
        #endif
            context[port_id].ac[0].svid = 0xFF01;
            memcpy(&context[port_id].ac[0].mid, recvd_msg_buf(), 4);
            break;
        case TYPE_DP_ALT_ENTER:    //0x19
            svid = (recvd_msg_buf()[3] << 8) | recvd_msg_buf()[2];
            ucsi_debug("enter mode :%x.\n", svid);
            if (svid == 0xFF01)
            {
                send_pd_msg(TYPE_GET_DP_CONFIGURE, 0, 0, SOP_TYPE);
                context[port_id].ConnectorPartner |= CP_ALT_MODE;
                context[port_id].csc.csc.ConnectorPartner = 1;
            #ifdef ReportSupportedCAM
                context[port_id].csc.csc.SupportedCAM = 1;
            #endif
                ucsi_async_notify_raise_up(50);
            }
            else
            {
                anx_ucsi_vdm_get_status(port_id);
            }
            break;
        case TYPE_DP_ALT_EXIT:     //0x1A
            //TODO....
            break;
        case TYPE_PWR_OBJ_REQ:     //0x16
            //pdo = (u8 *)para;

            if (recvd_msg_len() != 4)
                break;
            context[port_id].ucsi_partner_rdo =
                recvd_msg_buf()[0] | ((u32)recvd_msg_buf()[1] << 8) | ((u32)
                        recvd_msg_buf
                        ()[2]
                        << 16)
                | ((u32)recvd_msg_buf()[3] << 24);
        #ifdef ECR_CHECKING
            if ((context[port_id].anx_vbus_status == 1) &&
                    (recvd_msg_buf()[3] & _BIT2) && supply_1500ma_flag == 0)
                Send_mismatch_source_caps();
            ucsi_debug("RDO : %lx, vbus(%#x), supply_flag(%#x).\n",
                       context[port_id].ucsi_partner_rdo,
                       context[port_id].anx_vbus_status, supply_1500ma_flag);
        #endif
        #ifdef ReportNegotiatedPowerLevel
            context[port_id].csc.csc.NegotiatedPowerLevel = 1;
        #endif
        #ifdef ReportSupportedProviderCap
            context[port_id].csc.csc.SupportedProviderCap = 1;
        #endif
        #ifdef AUTO_RDO_ENABLE
                //ucsi_async_notify_raise_up(50);
            if (context[port_id].partner_pdo_sink_or_source)
                anx_get_rdo_event(port_id, context[port_id].ucsi_partner_rdo);

            break;
        #endif
            if (pd_check_requested_voltage())
            {
                ANX7447_l = send_pd_msg(TYPE_ACCEPT, NULL, 0, SOP_TYPE);
            }
            else
            {
                ANX7447_l = send_pd_msg(TYPE_REJECT, NULL, 0, SOP_TYPE);
            }
            context[port_id].downstream_pd_cap = 1;
            break;
        case TYPE_GET_PPS_STS:     //0x21
            break;
        case TYPE_BATT_STS:        //0x24
            break;
        case TYPE_ALERT:           //0x25
            /* Fix JIRA LBT-501 */
            ANX7447_l = recvd_msg_buf()[2];

            if (recvd_msg_buf()[3] & 0x02)
            {
                if (ANX7447_l & 0x1)
                {
                    ANX7447_k = 0x04;
                    send_pd_msg(TYPE_EXT_GET_BATT_STS, &ANX7447_k, 1, SOP_TYPE);
                }
                if (ANX7447_l & 0x2)
                {
                    ANX7447_k = 0x05;
                    send_pd_msg(TYPE_EXT_GET_BATT_STS, &ANX7447_k, 1, SOP_TYPE);
                }
                if (ANX7447_l & 0x4)
                {
                    ANX7447_k = 0x06;
                    send_pd_msg(TYPE_EXT_GET_BATT_STS, &ANX7447_k, 1, SOP_TYPE);
                }
                if (ANX7447_l & 0x8)
                {
                    ANX7447_k = 0x07;
                    send_pd_msg(TYPE_EXT_GET_BATT_STS, &ANX7447_k, 1, SOP_TYPE);
                }
                if (ANX7447_l & 0x10)
                {
                    ANX7447_k = 0x00;
                    send_pd_msg(TYPE_EXT_GET_BATT_STS, &ANX7447_k, 1, SOP_TYPE);
                }
                if (ANX7447_l & 0x20)
                {
                    ANX7447_k = 0x01;
                    send_pd_msg(TYPE_EXT_GET_BATT_STS, &ANX7447_k, 1, SOP_TYPE);
                }
                if (ANX7447_l & 0x40)
                {
                    ANX7447_k = 0x02;
                    send_pd_msg(TYPE_EXT_GET_BATT_STS, &ANX7447_k, 1, SOP_TYPE);
                }
                if (ANX7447_l & 0x80)
                {
                    ANX7447_k = 0x03;
                    send_pd_msg(TYPE_EXT_GET_BATT_STS, &ANX7447_k, 1, SOP_TYPE);
                }
            }
            else
            {
                send_pd_msg(TYPE_GET_STS, NULL, 0, SOP_TYPE);
            }
            break;
        case TYPE_DP_CONFIGURE:    //0x2a  redriver control
            /* LBT-540 */
            anx_ucsi_pin_assignment_event(port_id, recvd_msg_buf()[1]);
            break;
        case TYPE_GET_SRC_CAP_EXT: //0x1d
            //Fix JIRA LBT-501
            send_pd_msg(TYPE_EXT_SRC_CAP_EXT,
                        (unsigned char *)&pd_source_cap_extended, 24,
                        SOP_TYPE);
            break;
        case TYPE_GET_STS:         //0x1e
            /* Fix JIRA LBT-564 */
            anx_get_sts_response();
            break;
        case TYPE_EXT_SRC_CAP_EXT: //0x30
            break;
        case TYPE_GET_SINK_CAP_EXT:
            /* Fix JIRA LBT-501 */
            send_pd_msg(TYPE_EXT_SINK_CAP_EXT,
                        (unsigned char *)&pd_sink_cap_extended, 24, SOP_TYPE);
            break;
        case TYPE_EXT_STS:         //0x31
            break;
        case TYPE_EXT_GET_BATT_CAP:        //0x32
            /* Fix JIRA LBT-501 */
            anx_get_batt_cap_response(recvd_msg_buf()[0], 0x01c2, 0x01c2);
            break;
        case TYPE_EXT_GET_BATT_STS:        //0x33
            anx_get_batt_sts_response(recvd_msg_buf()[0], 0x00E2);
            break;
        case TYPE_EXT_BATT_CAP:    //0x34
            break;
        case TYPE_EXT_GET_MFR_INFO:        //0x35
            /* Fix JIRA LBT-501 *//* fix JIRA LBT-563 */
            if (recvd_msg_buf()[2] == 0)
                send_pd_msg(TYPE_EXT_MFR_INFO,
                            (unsigned char *)&pd_manufacturer_info_data, 26,
                            SOP_TYPE);
            else                    // 1:Battery, 2~255:Reserved
                send_pd_msg(TYPE_NOT_SUPPORTED, NULL, 0, SOP_TYPE);
            break;
        case TYPE_EXT_MFR_INFO:    //0x36
            break;
        case TYPE_GET_COUNTRY_INFO:        //0x38
            send_pd_msg(TYPE_EXT_COUNTRY_INFO,
                        (unsigned char *)&pd_country_code_info, 8, SOP_TYPE);
            break;
        case TYPE_GET_COUNTRY_CODE:        //0x39
            send_pd_msg(TYPE_EXT_COUNTRY_CODE,
                        (unsigned char *)&pd_country_code_data, 8, SOP_TYPE);
            break;
        case TYPE_EXT_COUNTRY_CODE:
            break;
        case TYPE_EXT_COUNTRY_INFO:
            break;
        case TYPE_EXT_PPS_STS:     //0x37
            break;
        case TYPE_EXT_SINK_CAP_EXT:        //0x3a
            break;
        case TYPE_NOT_SUPPORTED:   //0x1c
            break;
        case TYPE_SOFT_RST:
        #ifdef ReportPDResetComplete
            context[port_id].csc.csc.PDResetComplete = 1;
            ucsi_async_notify_raise_up(4);
        #endif
            break;
        case TYPE_HARD_RST:
        #ifdef ReportPDResetComplete
            ucsi_debug("received hardware reset event.\n");
            /* Fix LBT-541 */
            if ((recvd_msg_len() == 0) || recvd_msg_buf()[0] & _BIT1)
            {
                context[port_id].csc.csc.PDResetComplete = 1;
                ucsi_async_notify_raise_up(4);
                ucsi_debug("notify hardware reset event to ucsi.\n");
            }
        #endif
            break;
        case TYPE_FR_SWAP:         //0x1f
        default:
            break;
    }

    return ANX7447_l;

}

/* Recieve response message's callback function.
  * it can be rewritten by customer just reimmplement this function,
  * through register_default_pd_message_callbacku_func
  *  void *para : should be null
  *   para_len : 0
  * return:  0, fail;   1, success
  */
static u8 Recv_PD_Cmd_Rsp_Default_Callback()
{
    ucsi_debug("RESPONSE for %s is %s\n",
               interface_to_str(RESPONSE_REQ_TYPE()),
               result_to_str(RESPONSE_REQ_RESULT()));

    switch (RESPONSE_REQ_TYPE())
    {
        case TYPE_DSWAP_REQ:
            //need_notice_pd_cmd  =1;
            //usb_pd_cmd_status = RESPONSE_REQ_RESULT();
            //dswap_response_got = 1;

            context[port_id].csc.csc.ConnectorPartner = 1;
            break;

        case TYPE_PSWAP_REQ:
            context[port_id].power_role_swap_flag = 1;

            break;
        case TYPE_VCONN_SWAP_REQ:
            //vswap_response_got = 1;
            //need_notice_pd_cmd = 1;
            //usb_pd_cmd_status = RESPONSE_REQ_RESULT();
            break;
        case TYPE_GOTO_MIN_REQ:
            //gotomin_response_got = 1;
            //need_notice_pd_cmd = 1;
            //usb_pd_cmd_status = RESPONSE_REQ_RESULT();
            break;
        case TYPE_PWR_OBJ_REQ:
            //rdo_response_got = 1;
            //need_notice_pd_cmd = 1;
            //usb_pd_cmd_status = RESPONSE_REQ_RESULT();
            break;
        case TYPE_SOFT_RST:
        #ifdef ReportPDResetComplete
            context[port_id].csc.csc.PDResetComplete = 1;
        #endif
            break;
        case TYPE_HARD_RST:
        #ifdef ReportPDResetComplete
            context[port_id].csc.csc.PDResetComplete = 1;
        #endif
            break;

        default:
            break;
    }

    return 1;
}

/* Recieve Power Delivery Unstructured VDM message's callback function.
  * it can be rewritten by customer just reimmplement this function,
  * through register_default_pd_message_callbacku_func
  *  void *para : in this function it means PDO pointer
  *   para_len : means PDO length
  * return:  0, fail;   1, success
  */
static u8 Recv_PD_VDM_Defalut_Callback()
{
    u16 svid;
    if ((recvd_msg_sop_type() == SOP_TYPE)
            && (recvd_msg_type() == TYPE_VDM))
    {
        svid = (recvd_msg_buf()[3] << 8) | recvd_msg_buf()[2];

        if (svid == CUST_SVID)
            anx_ucsi_vdm_callback(port_id, recvd_msg_buf(), recvd_msg_len());
        else
            /* Fix JIRA LBT-501 return unsupport msg */
            send_pd_msg(TYPE_NOT_SUPPORTED, NULL, 0, SOP_TYPE);
        return 1;
    }

    if ((recvd_msg_sop_type() == SOP1_TYPE)
            && ((recvd_msg_buf()[0] & 0x0F) == 0x1))
    {
        if (recvd_msg_len() < 20)
        {
            ucsi_debug("no cable vdo.\n");
            return 1;
        }

        *(u32 *)&context[port_id].cv =
            ((u32)recvd_msg_buf()[19] << 24) |
            ((u32)recvd_msg_buf()[18] << 16) |
            ((u32)recvd_msg_buf()[17] << 8) |
            recvd_msg_buf()[16];
        return 1;
    }

    return 1;
}

static u8 Recv_Debug_Callback()
{
    switch (recvd_msg_type())
    {
        case TYPE_GET_VAR:
            if (recvd_msg_buf()[0] == 0)
            {  // idata
                ucsi_debug("read 0x%x idata = ",
                           ((u16)recvd_msg_buf()[1] << 8) +
                           recvd_msg_buf()[2]);
            }
            else if (recvd_msg_buf()[0] == 1)
            {   // xdata
                ucsi_debug("read 0x%x xdata = ",
                           ((u16)recvd_msg_buf()[1] << 8) +
                           recvd_msg_buf()[2]);
            }
            else if (recvd_msg_buf()[0] == IF_VAR_fw_var_reg)
            {   // REG_FW_VAR
                ucsi_debug("read REG_FW_VAR[0x%x] = ",
                           ((u16)recvd_msg_buf()[1] << 8) +
                           recvd_msg_buf()[2]);
            }
            else if (recvd_msg_buf()[0] == IF_VAR_pd_src_pdo)
            {
                ucsi_debug("read pd_src_pdo[0x%x] = ",
                           ((u16)recvd_msg_buf()[1] << 8) +
                           recvd_msg_buf()[2]);
            }
            else if (recvd_msg_buf()[0] == IF_VAR_pd_snk_pdo)
            {
                ucsi_debug("read pd_snk_pdo[0x%x] = ",
                           ((u16)recvd_msg_buf()[1] << 8) +
                           recvd_msg_buf()[2]);
            }
            else if (recvd_msg_buf()[0] == IF_VAR_pd_rdo_bak)
            {
                ucsi_debug("read pd_rdo_bak[0x%x] = ",
                           ((u16)recvd_msg_buf()[1] << 8) +
                           recvd_msg_buf()[2]);
            }
            else if (recvd_msg_buf()[0] == IF_VAR_pd_rdo)
            {
                ucsi_debug("read pd_rdo[0x%x] = ",
                           ((u16)recvd_msg_buf()[1] << 8) +
                           recvd_msg_buf()[2]);
            }
            else if (recvd_msg_buf()[0] == IF_VAR_DP_caps)
            {
                ucsi_debug("read DP_cap[0x%x] = ",
                           ((u16)recvd_msg_buf()[1] << 8) +
                           recvd_msg_buf()[2]);
            }
            else if (recvd_msg_buf()[0] == IF_VAR_configure_DP_caps)
            {    // REG_FW_VAR
                ucsi_debug("read configure_DP_caps[0x%x] = ",
                           ((u16)recvd_msg_buf()[1] << 8) +
                           recvd_msg_buf()[2]);
            }
            else if (recvd_msg_buf()[0] == IF_VAR_src_dp_status)
            { // REG_FW_VAR
                ucsi_debug("read src_dp_status[0x%x] = ",
                           ((u16)recvd_msg_buf()[1] << 8) +
                           recvd_msg_buf()[2]);
            }
            else if (recvd_msg_buf()[0] == IF_VAR_sink_svid_vdo)
            { // REG_FW_VAR
                ucsi_debug("read sink_svid_vdo[0x%x] = ",
                           ((u16)recvd_msg_buf()[1] << 8) +
                           recvd_msg_buf()[2]);
            }
            else if (recvd_msg_buf()[0] == IF_VAR_sink_identity)
            { // REG_FW_VAR
                ucsi_debug("read sink_identity[0x%x] = ",
                           ((u16)recvd_msg_buf()[1] << 8) +
                           recvd_msg_buf()[2]);
            }
            else
            {                // ?data
                ucsi_debug("read 0x%x ?data = ",
                           ((u16)recvd_msg_buf()[1] << 8) +
                           recvd_msg_buf()[2]);
            }
            break;
        default:
            break;
    }

    return 1;
}

/*ChengLin: To integrate all send_xxx DATA Messages to one function for reduce code size*/
static u8 Send_PD_Data_Messages(u8 type_msg, u8 *DataBuff, u8 size)
{
    if (NULL == DataBuff)
        return CMD_FAIL;

    if (type_msg == TYPE_DP_SNK_CFG)
    {
        memcpy(context[port_id].configure_DP_caps, DataBuff, 4);
        memcpy(context[port_id].InterfaceSendBuf + 2, DataBuff, 4);
        memset(context[port_id].InterfaceSendBuf + 2 + 4, 0, 4);
        context[port_id].InterfaceSendBuf[0] = 4 + 4 + 1;       // + cmd
    }
    else
    {
        if (type_msg == TYPE_DP_SNK_IDENTITY)
            memcpy(context[port_id].src_dp_caps, DataBuff, size);
        memcpy(context[port_id].InterfaceSendBuf + 2, DataBuff, size);
        context[port_id].InterfaceSendBuf[0] = size + 1;        // + cmd
    }

    context[port_id].InterfaceSendBuf[1] = type_msg;

    return interface_send_msg();
}

/* Fix JIRA LBT-501 */
static u8 Send_PD_Extend_Messages(u8 type_msg, u8 *buff_ref, u8 size,
                                  u8 type_sop)
{
    if (type_msg == TYPE_EXT_PDFU_REQUEST
            || type_msg == TYPE_EXT_COUNTRY_INFO
            || type_msg == TYPE_EXT_COUNTRY_CODE)
    {
        if (size > 26)
        {
            context[port_id].InterfaceSendBuf[0] = (26 + 2 + 1) | (type_sop << 6);      //        + ext_header + cmd
            memcpy(context[port_id].InterfaceSendBuf + 4, buff_ref, 26);

            if (type_msg == TYPE_EXT_COUNTRY_INFO
                    || type_msg == TYPE_EXT_COUNTRY_CODE)
                anx_write_block_reg(context[port_id].PD_EMTB_slave_id, 0,
                                    size - 26,
                                    (unsigned char *)(buff_ref + 26));
        }
        else
        {
            context[port_id].InterfaceSendBuf[0] = (size + 2 + 1) | (type_sop << 6);    //      + ext_header + cmd
            memcpy(context[port_id].InterfaceSendBuf + 4, buff_ref, size);
        }

    }
    else
    {
        context[port_id].InterfaceSendBuf[0] = (size + 2 + 1) | (type_sop << 6);        // + ext_header + cmd
        memcpy(context[port_id].InterfaceSendBuf + 4, buff_ref, size);
    }

    context[port_id].InterfaceSendBuf[1] = type_msg;
    USB_PD_EXT_HEADER((context[port_id].InterfaceSendBuf + 2), size, 0, 0,
                      0);

    SWAP_WORD((context[port_id].InterfaceSendBuf + 2));
    return interface_send_msg();
}


unsigned char Send_Interface_Messages(u8 type_msg, u8 *DataBuff, u8 size)
{
    memcpy(context[port_id].InterfaceSendBuf + 2, DataBuff, size);
    context[port_id].InterfaceSendBuf[0] = size + 1;    // + cmd
    context[port_id].InterfaceSendBuf[1] = type_msg;
    return interface_send_msg();
}

/**
 * @desc:   The Interface AP set the VDM packet to Ohio
 *
 * @param:  vdm:  object buffer pointer of VDM,
 *
 *
 *
 *                size: vdm packet size
 *
 * @return:  0: success 1: fail
 *
 */

/* Add VDM interface, LBT-437 */
u8 send_vdm(u8 type_sop, u8 *vdm, unsigned char size)
{
    if (NULL == vdm)
        return CMD_FAIL;
    if ((size < 4) || (size > 28) || (size % 4 != 0))
        return CMD_FAIL;

    context[port_id].InterfaceSendBuf[0] = (size + 1) | (type_sop << 6);        // + cmd
    context[port_id].InterfaceSendBuf[1] = TYPE_VDM;
    memcpy(context[port_id].InterfaceSendBuf + 2, vdm, size);
    return interface_send_msg();        //updated to fix OHO-423
}

/*ChengLin: To integrate all send_xxx_ control messages to one function for reduce code size*/
static u8 Send_PD_Control_Messages(u8 type_msg)
{
    context[port_id].InterfaceSendBuf[1] = type_msg;
    return interface_send_ctr_msg();
}

#define cac_checksum(buf, len) {\
    ANX7447_j = 0;\
    for (ANX7447_k = 0; ANX7447_k < len; ANX7447_k++)\
        ANX7447_j += *(buf + ANX7447_k);\
}


u8 send_pd_msg(PD_MSG_TYPE type, u8 *buf, u8 size, u8 type_sop)
{
    ucsi_debug("SendMSG ->%s: \n", interface_to_str(type));
#if 1
    switch (type)
    {
//Send Control Messages
        case TYPE_GET_DP_SNK_CAP:  //send 4
        case TYPE_ACCEPT:          //send 5
        case TYPE_REJECT:          //send 6
        case TYPE_GET_SRC_CAP:     //send 9
        case TYPE_GET_PD_STATUS:   //send 17
        case TYPE_GET_SNK_CAP:     //0x1B
        case TYPE_GET_SRC_CAP_EXT: //send 1d
        case TYPE_GET_STS:         //send 1e
        case TYPE_FR_SWAP_SIGNAL:  //send 20
        case TYPE_GET_PPS_STS:     // send 21
        case TYPE_GET_COUNTRY_CODE:        //send 22
        case TYPE_GET_SINK_CAP_EXT:        //send 23
        case TYPE_NOT_SUPPORTED:
        case TYPE_SOFT_RST:        //send f1
        case TYPE_HARD_RST:        //send f2
            Send_PD_Control_Messages(type);
            break;
        case TYPE_GET_RDO:
        case TYPE_GET_DP_CONFIGURE:
        case TYPE_PSWAP_REQ:       //send 10
        case TYPE_DSWAP_REQ:       //send 11
        case TYPE_GOTO_MIN_REQ:    //send 12
        case TYPE_VCONN_SWAP_REQ:  //send 13
            if (Send_PD_Control_Messages(type) == CMD_SUCCESS)
                ANX7447_l = CMD_SUCCESS;
            else
                ANX7447_l = CMD_FAIL;
            break;

        //Send Data Messages
        case TYPE_PWR_SRC_CAP:     //send 0
        case TYPE_PWR_SNK_CAP:     //send 1
        case TYPE_DP_SNK_IDENTITY: //send 2
        case TYPE_SVID:            //send 3
        case TYPE_SET_SNK_DP_CAP:  //send 8
        case TYPE_DP_SNK_CFG:      //send 15
        case TYPE_BATT_STS:        //send 24
        case TYPE_ALERT:           //send 25
        case TYPE_GET_COUNTRY_INFO:        //send 26
        case TYPE_GET_VAR:         //send 0xfc
        case TYPE_SET_VAR:         //send 0xfd
        case TYPE_CMD_SRC_CAP_1:
            ANX7447_l = Send_PD_Data_Messages(type, buf, size);
            break;
        case TYPE_PWR_OBJ_REQ:     //send 16
            if (Send_PD_Data_Messages(type, buf, size) == CMD_SUCCESS)
                ANX7447_l = CMD_SUCCESS;
            else
                ANX7447_l = CMD_FAIL;
            break;

        //Send VDM Messages
        case TYPE_VDM:             //send 14
            ANX7447_l = send_vdm(type_sop, buf, size);
            break;
        case TYPE_I2C_MASTER_WRITE:        //send 0x3e
            Send_Interface_Messages(type, buf, size);
            break;

        //Send Extend Messages
        case TYPE_EXT_SRC_CAP_EXT: //send 30 //Added by ChengLin
        case TYPE_EXT_STS:         //send 31 //Added by ChengLin
        case TYPE_EXT_SINK_CAP_EXT:        //send 3a //Added by ChengLin
        case TYPE_EXT_PPS_STS:     //send 37 //Added by ChengLin
        case TYPE_EXT_GET_BATT_CAP:        //send 32   size = 1
        case TYPE_EXT_GET_BATT_STS:        //send 33
        case TYPE_EXT_BATT_CAP:    //send 34
        case TYPE_EXT_GET_MFR_INFO:        //send 35
        case TYPE_EXT_MFR_INFO:    //send 36
        case TYPE_EXT_COUNTRY_INFO:        //send 38
        case TYPE_EXT_COUNTRY_CODE:        //send 39
        case TYPE_EXT_PDFU_REQUEST:        //send 3b
            Send_PD_Extend_Messages(type, buf, size, type_sop);
            break;
        default:
            ucsi_debug("unknown type %2X\n", type);
            ANX7447_l = CMD_FAIL;
            break;
    }
#endif
    return ANX7447_l;
}


/**
 * @desc:   The Interface that AP handle the specific USB PD command from Ohio
 *
 * @param:
 *      type: PD message type, define enum PD_MSG_TYPE.
 *      buf: the sepecific paramter pointer according to the message type:
 *                      eg: when AP update its source capability type=TYPE_PWR_SRC_CAP,
 *          "buf" contains the content of PDO object,its format USB PD spec
 *                      customer can easily packeted it through PDO_FIXED_XXX macro:
 *                     default5Vsafe 5V, 0.9A fixed --> PDO_FIXED(5000,900, PDO_FIXED_FLAGS)
 *                size: the paramter ponter's content length, if buf is null, it should be 0
 *
 * @return:  0: success 1: fail
 *
 */
static u8 dispatch_rcvd_pd_msg()
{
    switch (recvd_msg_type())
    {
        case TYPE_VDM:             //0x14
            ANX7447_l = Recv_PD_VDM_Defalut_Callback();
            break;
        case TYPE_EXT_PDFU_RESPONSE:       //0x3c
            break;
        case TYPE_RESPONSE_TO_REQ: //0xf0
            ANX7447_l = Recv_PD_Cmd_Rsp_Default_Callback();
            break;
        case TYPE_GET_VAR:         //0xfc
            ANX7447_l = Recv_Debug_Callback();
            break;
        default:
            ANX7447_l = Recv_PD_Commands_Default_Callback();
            break;
    }

    return ANX7447_l;
}


static void pd_cc_status_default_func(u8 cc_status)
{
    /* cc status */
    context[port_id].power_op_mode = PO_USB_DEFAULT;
    role = get_data_role();
    context[port_id].anx_data_role = role;

    if (role == 0)
        context[port_id].ucsi_connector_partner_type = CPT_DFP_ATTACHED;
    else
        context[port_id].ucsi_connector_partner_type = CPT_UFP_ATTACHED;

    switch ((cc_status & 0xF))
    {
        case 1:
            switch (((cc_status >> 4) & 0xF))
            {
                case 0:
                    // CC1 Rd
                    //UFP, CC2 Open
                    context[port_id].ucsi_connector_partner_type =
                        CPT_UFP_ATTACHED;
                    break;
                case 1:
                    //CC2 Rd, Debug
                    context[port_id].ucsi_connector_partner_type =
                        CPT_CAB_DEBUG_ACC;
                    break;
                case 2:
                    //CC2 Ra
                    context[port_id].ucsi_connector_partner_type =
                        CPT_CAB_UFP_ATTACHED;
                    break;
            }
            break;
        case 2:
            switch (((cc_status >> 4) & 0xF))
            {
                case 0:
                    //CC1 Ra
                    //UFP, CC2 Open
                    context[port_id].ucsi_connector_partner_type =
                        CPT_CAB_NO_UFP_ATTACHED;
                    break;
                case 1:
                    //CC2 Rd, Debug
                    context[port_id].ucsi_connector_partner_type =
                        CPT_CAB_UFP_ATTACHED;
                    break;
                case 2:
                    //CC2 Ra
                    context[port_id].ucsi_connector_partner_type = CPT_AUDIO_ACC;
                    break;
            }
            break;
        case 0:
            switch (((cc_status >> 4) & 0xF))
            {
                case 1:
                    //CC1 Open
                    //CC2 Rd
                    context[port_id].ucsi_connector_partner_type =
                        CPT_UFP_ATTACHED;
                    break;
                case 2:
                    //CC2 Ra
                    context[port_id].ucsi_connector_partner_type =
                        CPT_CAB_NO_UFP_ATTACHED;
                    break;
                case 0:
                    //context[port_id].ucsi_connector_partner_type = CPT_RESERVED;
                    break;
            }
            break;
        case 0x04:
            context[port_id].power_op_mode = PO_USB_DEFAULT;
            break;
        case 0x08:
            context[port_id].power_op_mode = PO_TYPE_C_1_5A;
            break;
        case 0x0c:
            context[port_id].power_op_mode = PO_TYPE_C_3A;
            break;
    }

    switch (((cc_status >> 4) & 0xF))
    {
        case 0x04:
            context[port_id].power_op_mode = PO_USB_DEFAULT;
            break;
        case 0x08:
            context[port_id].power_op_mode = PO_TYPE_C_1_5A;
            break;
        case 0x0c:
            context[port_id].power_op_mode = PO_TYPE_C_3A;
            break;
    }

    anx_ucsi_cc_status_event(port_id, cc_status, context[port_id].power_op_mode);

}

static u8 interface_send_msg()
{
    cac_checksum(context[port_id].InterfaceSendBuf, send_msg_len() + 1);
    context[port_id].InterfaceSendBuf[send_msg_len() + 1] = 0 - ANX7447_j;      //cmd + checksum
    len = send_msg_len() + 2;
    ANX7447_c = anx_read_reg(context[port_id].anx_spi, InterfaceSendBuf_Addr);
    // retry
    if (ANX7447_c)
    {
        count = 250;
        while (count)
        {
            mdelay(2);
            ANX7447_c = anx_read_reg(context[port_id].anx_spi, InterfaceSendBuf_Addr);
            if (ANX7447_c == 0)
                break;
            count--;
        }
    }
    if (ANX7447_c == 0)
    {
        anx_write_block_reg(context[port_id].anx_spi,
                            InterfaceSendBuf_Addr + 1, len - 1,
                            &context[port_id].InterfaceSendBuf[1]);
        anx_write_reg(context[port_id].anx_spi, InterfaceSendBuf_Addr,
                      context[port_id].InterfaceSendBuf[0]);
    }
    else
    {
        ucsi_debug("Tx Buf Full\n");
        return CMD_FAIL;
    }

    return CMD_SUCCESS;
}

static u8 interface_send_ctr_msg()
{
    context[port_id].InterfaceSendBuf[0] = 1;
    context[port_id].InterfaceSendBuf[2] = 0 - (context[port_id].InterfaceSendBuf[0] + context[port_id].InterfaceSendBuf[1]);   //cac_checksum(InterfaceSendBuf, 1 + 1); //cmd + checksum
    len = 3;
    ANX7447_c = anx_read_reg(context[port_id].anx_spi, InterfaceSendBuf_Addr);
    // retry
    if (ANX7447_c)
    {
        count = 250;
        while (count)
        {
            mdelay(1);
            ANX7447_c = anx_read_reg(context[port_id].anx_spi, InterfaceSendBuf_Addr);
            if (ANX7447_c == 0)
                break;
            count--;
        }
    }
    if (ANX7447_c == 0)
    {
        anx_write_block_reg(context[port_id].anx_spi,
                            InterfaceSendBuf_Addr + 1, len - 1,
                            &context[port_id].InterfaceSendBuf[1]);
        anx_write_reg(context[port_id].anx_spi, InterfaceSendBuf_Addr,
                      context[port_id].InterfaceSendBuf[0]);
    }
    else
    {
        ucsi_debug("Tx Buf Full\n");
        return CMD_FAIL;
    }

    return CMD_SUCCESS;
}

void dp_alt_enter_mode()
{
    Send_PD_Data_Messages(TYPE_DP_ALT_ENTER, (u8 *)enter_exit_svid, 4);
}

void dp_alt_exit_mode()
{
    Send_PD_Data_Messages(TYPE_DP_ALT_EXIT, (u8 *)enter_exit_svid, 4);
}

static u8 interface_recvd_msg()
{
#ifdef ANX_I2C_BUFF_FAST_READ
    anx_read_block_reg(context[port_id].anx_spi, InterfaceRecvBuf_Addr, 7,
                       (u8 *)context[port_id].InterfaceRecvBuf);
#else
    anx_read_block_reg(context[port_id].anx_spi, InterfaceRecvBuf_Addr, 32,
                       (u8 *)context[port_id].InterfaceRecvBuf);
#endif
    if (context[port_id].InterfaceRecvBuf[0] != 0)
    {
    #ifdef ANX_I2C_BUFF_FAST_READ
        if (recvd_msg_len() > 4)
            anx_read_block_reg(context[port_id].anx_spi,
                               InterfaceRecvBuf_Addr + 7,
                               recvd_msg_len() - 4,
                               context[port_id].InterfaceRecvBuf + 7);
    #endif
        anx_write_reg(context[port_id].anx_spi, InterfaceRecvBuf_Addr, 0);
        ANX7447_j = 0;
        for (ANX7447_i = 0; ANX7447_i < recvd_msg_len() + 2 + 1; ANX7447_i++)
        {
            ANX7447_j += context[port_id].InterfaceRecvBuf[ANX7447_i];
        }
        if (ANX7447_j == 0)
        {
            return CMD_SUCCESS;
        }
        else
        {
            ucsi_debug("checksum error: \n");
        }
    }
    return CMD_FAIL;
}

static void handle_intr_vector()
{
    if (interface_recvd_msg() == CMD_SUCCESS)
    {
        ucsi_debug("rev <- %s\n", interface_to_str((PD_MSG_TYPE)
            context[port_id].
            InterfaceRecvBuf[1]));
        dispatch_rcvd_pd_msg();
    }
}

#ifdef PATCH_NOT_CALIBRATED_CHIP
#define ANALOG_CTRL_6                           0xA6
#define ADC_CTRL_2                              0xC0
#define EFUSE_CTRL_10                           0x7C
#define EFUSE_CTRL_11                           0x7D
#define EFUSE_PROGRAM_DONE_CLR _BIT7
#define EFUSE_READ_DONE_CLR _BIT6
#define EFUSE_READ_DONE _BIT4
#define EFUSE_READ_DATA_0                       0x8D
void eFuseRead(unsigned char offset, unsigned char len, unsigned char *buf)
{
    anx_read_block_reg(context[port_id].anx_spi, EFUSE_READ_DATA_0 + offset,
                       len, buf);
}

unsigned char isCalibrated()
{
    unsigned char buf[3];
    eFuseRead(0, 3, buf);
    if (!buf[0] || !buf[1] || !buf[2])
        return 0;
    return 1;
}
#endif

void send_initialized_setting()
{
    switch (context[port_id].anx_initial_status)
    {
        case INIT_STAGE_1:
            //send_source_capability();
            context[port_id].anx_initial_status++;
            break;
        case INIT_STAGE_2:
            //send_sink_capability();
            context[port_id].anx_initial_status++;
            break;
        case INIT_STAGE_3:
        #ifdef ANX_NO_CUST_DEF_DATA
            ucsi_debug("Sending sink identity\n");

            /* send TYPE_DP_SNK_IDENTITY init setting */
            send_pd_msg(TYPE_DP_SNK_IDENTITY, (u8 *)snk_identity,
                        sizeof(snk_identity), SOP_TYPE);
        #endif
            context[port_id].anx_initial_status++;
            break;
        case INIT_STAGE_4:
            context[port_id].anx_initial_status++;
            break;
        case INIT_STAGE_5:
            //send_pd_msg(TYPE_SET_VAR,set_variable_1,5,0);
            //send_pd_msg(TYPE_SET_SNK_DP_CAP, dp_source_dp_caps, 4, SOP_TYPE);
            context[port_id].anx_initial_status++;
            break;
        case 55:
            context[port_id].anx_initial_status++;
            ucsi_debug("power on 550ms, data operation mode is %#x.\n",
                       context[port_id].data_operation_mode_uom);
        #if 1
            if ((context[port_id].data_operation_mode_uom == 0) ||
                    (context[port_id].data_operation_mode_uom & OP_DRP))
                break;
            switch (get_data_role())
            {
                case 1:                //DFP
                    role = OP_DFP;
                    break;
                case 0:                //UFP
                    role = OP_UFP;
                    break;
            }
            if (context[port_id].data_operation_mode_uom & role)
                break;
            //send_pd_msg(TYPE_DSWAP_REQ, 0, 0, SOP_TYPE);
            /* fix JIRA LBT-552 */
            chip_power_down(port_id);
            anx_initial_context(port_id);
        #endif
            break;
        case 56:
            context[port_id].anx_initial_status++;
            ucsi_debug("power on 560ms, power operation mode is %#x.\n",
                       context[port_id].power_operation_mode_pdm);
            if ((context[port_id].power_operation_mode_pdm == 0) ||
                    (context[port_id].power_operation_mode_pdm & PW_DRP))
                break;
            switch (get_power_role())
            {
                case 1:                //PW_PROVIDER
                    role = PW_PROVIDER;
                    break;
                case 0:                //PW_CONSUMER
                    role = PW_CONSUMER;
                    break;
            }
            if (context[port_id].power_operation_mode_pdm & role)
                break;
            send_pd_msg(TYPE_PSWAP_REQ, 0, 0, SOP_TYPE);
            break;
        case 80:
            context[port_id].anx_initial_status = 0;
            //Fix JIRA LBT-404
        #ifdef ANX_SEND_GET_SOURCE_CAPS
            if (request_src_caps_flag[port_id] == 1)
            {
                request_src_caps_flag[port_id] = 0;
                send_pd_msg(TYPE_GET_SRC_CAP, 0, 0, SOP_TYPE);
            }
        #endif
            context[port_id].enable_report_power_on = 1;
            break;
        default:
            context[port_id].anx_initial_status++;
            break;
        case 0:
            break;
    }

}

static void drp_toggle_enable()
{
    ANX7447_c = anx_read_reg(context[port_id].anx_spi, 0x95);
    if (context[port_id].data_operation_mode_uom & OP_DFP)
    {
        anx_write_reg(context[port_id].anx_addr1, TCPC_ROLE_CONTROL, 0x5);   //CC with  Rp + 1.5A
        ucsi_debug("set tcpc role as Rp(0x5), reg 0xaa(%#x)\n", ANX7447_c);
    }
    else if (context[port_id].data_operation_mode_uom & OP_UFP)
    {
        anx_write_reg(context[port_id].anx_addr1, TCPC_ROLE_CONTROL, 0xA);   //CC with  RD
        ucsi_debug("set tcpc role as Rd(0xa), reg 0xaa(%#x)\n", ANX7447_c);
    }
    else
    {
        anx_write_reg(context[port_id].anx_addr1, ANALOG_CTRL_10, 0x80);
        anx_write_reg(context[port_id].anx_addr1, TCPC_ROLE_CONTROL, 0x4A);  //CC with  RD
        ucsi_debug("set tcpc role as Rd + DRP en(0x4A), %#x chip\n", ANX7447_c);
        if (ANX7447_c >= 0xAC && ANX7447_c != 0xFF)
        {
            anx_write_reg(context[port_id].anx_addr1, TCPC_COMMAND, 0x99);   //DRP en
        }
        else
        {
            anx_write_reg(context[port_id].anx_addr1, ANALOG_CTRL_1,
                          (anx_read_reg
                          (context[port_id].anx_addr1,
                              ANALOG_CTRL_1) | TOGGLE_CTRL_MODE |
                              R_TOGGLE_ENABLE));
            anx_write_reg(context[port_id].anx_addr1, ANALOG_CTRL_1,
                          (anx_read_reg
                          (context[port_id].anx_addr1,
                              ANALOG_CTRL_1) | R_LATCH_TOGGLE_ENABLE));
        }
    }
}

// static void pd_ovp_set()
// {
//     /* disable VBUS protection */
//     anx_write_reg(context[port_id].anx_addr1, 0xcd, 0x80);
//     count = (PD_MAX_VOLTAGE * (100 / 25)) + 32;
//     anx_write_reg(context[port_id].anx_addr1, 0x76, count & 0xFF);
//     anx_write_reg(context[port_id].anx_addr1, 0x77, (count >> 8) & 0xFF);

//     /* clear interrupt */
//     anx_write_reg(context[port_id].anx_addr1, 0x10, 0x80);

//     /* enable VBUS protection */
//     anx_write_reg(context[port_id].anx_addr1, 0xcd, 0xA0);
// }

static void usb_pd_function_init(void)
{
#ifdef PATCH_NOT_CALIBRATED_CHIP
    ANX7447_c = anx_read_reg(context[port_id].anx_spi, 0x95);
    if (!isCalibrated())
    {
        ucsi_debug("Not calibrate chip.\n");
        anx_write_reg(context[port_id].anx_addr1, RING_OSC_CTRL, 0xb7);      //27M clock
        if (ANX7447_c >= 0xAC && ANX7447_c != 0xFF)
        {
            anx_write_reg(context[port_id].anx_addr1, ADC_CTRL_2, 0x26);     ////adc                AA: 0x0f    AC:0x26
        }
        else
        {
            anx_write_reg(context[port_id].anx_addr1, ADC_CTRL_2, 0x0f);     //adc
        }
        anx_write_reg(context[port_id].anx_addr1, ANALOG_CTRL_6, 0x11);      //5.1K
    }
#endif
    // configure OCM init use PD2.0
#ifdef USE_PD_20
    anx_write_reg(context[port_id].anx_spi, PD_REV_INIT, 0x01);
#else
    anx_write_reg(context[port_id].anx_spi, PD_REV_INIT, 0x00);
#endif

#ifdef ANX_OVP_RE_SET
    pd_ovp_set();
#endif
#ifdef ANX_NO_CUST_DEF_DATA
    anx_write_reg(context[port_id].anx_spi, MAX_VOLTAGE, PD_MAX_VOLTAGE);
    anx_write_reg(context[port_id].anx_spi, MAX_POWER, PD_MAX_POWER);

    /*Minimum Power in 500mW units */
    anx_write_reg(context[port_id].anx_spi, MIN_POWER, PD_MIN_POWER);
#endif
#if (PD_MAX_INSTANCE == 2)
    if (context[0].anx_power_status == 0 && context[1].anx_power_status == 0)
        random_data = (timer1msCount % 256);
    anx_write_reg(context[port_id].anx_addr1, 0xef, random_data);
#endif
#ifdef ANX_NO_CUST_DEF_DATA
    ANX7447_k = 0;
    /*
     * bit 0: enable unstructure VDM(fix self charging)
     * bit 1: dead battery mode, OCM maximum delay 200ms
     * bit 3 & 2 : 00:vsafe 0v, 01:vsafe 1v, 10:vsafe 2v, 11:vsafe 3v
     */
    ANX7447_k = ANX7447_k | 0x3;
    ANX7447_k = ANX7447_k | (0x1 << 2);
    if (context[port_id].pd_capability & PD_HAS_ANX_REDRIVER)
        ANX7447_k = ANX7447_k | _BIT4;
#ifdef ANX_FRS_EN_AT_I2C_ADR
    ANX7447_k = ANX7447_k | _BIT7;              /* Enable FRS_EN at pin I2C_ADR_1 */
#endif
    anx_write_reg(context[port_id].anx_addr1, 0xf0, ANX7447_k);
#endif

#ifdef ANX_NO_CUST_DEF_DATA
    ANX7447_k = anx_read_reg(context[port_id].anx_spi, FIRMWARE_CTRL);
    /* Default enable auto pd mode */
    ucsi_debug("enable auto pd function\n");
#ifdef AUTO_RDO_ENABLE
    ANX7447_k = ANX7447_k | auto_pd_en;
#endif
    //Fix JIRA LBT-366
    ANX7447_k = ANX7447_k | 0x40;
    // trySrc_trySnk_setting
    ANX7447_k = ANX7447_k & (~(trysrc_en | trysnk_en));

    anx_write_reg(context[port_id].anx_spi, FIRMWARE_CTRL, ANX7447_k);
#endif
    //Disable OVP
    anx_write_reg(context[port_id].anx_addr1, 0xdd, 0xff);
    anx_write_reg(context[port_id].anx_addr1, 0xde, 0x03);

    //fix Lenove dongle IOP issue
    anx_write_reg(context[port_id].anx_addr1, 0xf1,
                  anx_read_reg(context[port_id].anx_addr1, 0xf1) | 0x1);
    //interrupt mask initial
    mdelay(2);
    /* LBT-539 */
    anx_write_reg(context[port_id].anx_spi, INT_MASK,
                  (anx_read_reg(context[port_id].anx_spi, INT_MASK) &
                      (~CC_STATUS_CHANGE) & (~PR_CONSUMER_GOT_POWER) &
                      (~OCM_BOOT_UP) & (~DATA_ROLE_CHANGE)));

#ifdef ANX_DISABLE_DR_VCON_SWAP
    /* Disable automatically DR&VCONN swap */
    anx_write_reg(context[port_id].anx_addr1, 0xF0,
                  (anx_read_reg(context[port_id].anx_addr1, 0xF0) | 0x1));
#endif

    /* LBT-827 */
    anx_write_reg(context[port_id].anx_addr1, 0xF7,
                  (anx_read_reg(context[port_id].anx_addr1, 0xF7)
                      | _BIT7));
       // config OCM reply message
    anx_write_reg(context[port_id].anx_spi, PD_EXT_MSG_CTRL, 0xFF);
}

void chip_power_on(u8 port)
{
    anx_read_reg(context[port].anx_addr1, TCPC_COMMAND);
    RamDebug(0xE0 | port);
    ucsi_debug("power on port %#x.\n", port);
}

void chip_power_down(u8 port)
{
    RamDebug(0xD0 | port);
    anx_write_reg(context[port].anx_spi, OCM_CTRL_0,
                  (anx_read_reg(context[port].anx_spi, OCM_CTRL_0) | OCM_RESET));
    drp_toggle_enable();
    anx_write_reg(context[port].anx_addr1, TCPC_COMMAND, 0xff);  //lpower down
}

#ifdef ECR_CHECKING
static void anx_ecr_checking(void)
{
    if (((context[port_id].ucsi_partner_rdo >> 10) & 0x3ff)
            == (PD_CURRENT_1500MA / 10))
    {
        supply_1500ma_flag = 0;
        context[port_id].ucsi_partner_rdo = 0;
    }
}
#endif

static void anx_detect_voltage_and_current(u8 port)
{
    change_status =
        anx_read_reg(context[port].anx_spi, SYSTEM_STSTUS);
    change_int =
        anx_read_reg(context[port_id].anx_spi, CHANGE_INT);
    if (change_status & _BIT4)
    {
        self_charging_error = 1;
        return;
    }

    if (((change_status & VBUS_STATUS) == 0)
            || (change_int & PR_CONSUMER_GOT_POWER))
    {
        context[port].anx_power_role = 0;
        context[port].anx_current_in_50ma =
            anx_read_reg(context[port].anx_spi, 0xb1);
        context[port].anx_voltage_in_100mv =
            anx_read_reg(context[port].anx_spi, 0xaf);
        if ((context[port].anx_current_in_50ma == 0)
                || (context[port].anx_voltage_in_100mv == 0))
        {
            change_status =
                anx_read_reg(context[port].anx_spi, NEW_CC_STATUS);
            pd_cc_status_default_func(change_status);
            context[port].anx_voltage_in_100mv = 50;
            switch (context[port].power_op_mode)
            {
                default:
                case PO_USB_DEFAULT:
                    context[port].anx_current_in_50ma = 10;
                    break;
                case PO_TYPE_C_1_5A:
                    context[port].anx_current_in_50ma = 30;
                    break;
                case PO_TYPE_C_3A:
                    context[port].anx_current_in_50ma = 60;
                    break;
            }
        }
    }
    else
    {
        context[port].anx_power_role = 1;
    }
}

void anx_dead_battery_checking_2(u8 port)
{
    if (context[port].anx_power_status == 0)
    {
        change_status = anx_read_reg(context[port].anx_spi, NEW_CC_STATUS);
        if (change_status == 0)
            return;       //return if no cc connection
        context[port].anx_power_status = 1;
    }
    anx_detect_voltage_and_current(port);
}

void anx_dead_battery_checking_1()
{
    for (ANX7447_i = 0; ANX7447_i < PD_MAX_INSTANCE; ANX7447_i++)
    {
        if (context[ANX7447_i].anx_power_status == 0)
        {
            change_status =
                anx_read_reg(context[ANX7447_i].anx_spi, NEW_CC_STATUS);
            if (change_status != 0)
            {
                context[ANX7447_i].anx_power_status = 1;
                pd_cc_status_default_func(change_status);
                change_status =
                    anx_read_reg(context[ANX7447_i].anx_spi, SYSTEM_STSTUS);
                if (change_status & VBUS_CHANGE)
                    context[ANX7447_i].anx_power_role = 1;
                else
                    //debug only
                    context[ANX7447_i].anx_power_role = 0;
                if (change_status & _BIT4)
                {
                    self_charging_error = 1;
                    return;
                }

                if (context[ANX7447_i].anx_power_role == 0)
                {
                    context[ANX7447_i].anx_current_in_50ma =
                        anx_read_reg(context[port_id].anx_spi, 0xb1);
                    context[ANX7447_i].anx_voltage_in_100mv =
                        anx_read_reg(context[port_id].anx_spi, 0xaf);
                    if (context[ANX7447_i].anx_current_in_50ma == 0
                            || context[ANX7447_i].anx_voltage_in_100mv == 0)
                    {
                        context[ANX7447_i].anx_voltage_in_100mv = 50;
                        switch (context[port_id].power_op_mode)
                        {
                            default:
                            case PO_USB_DEFAULT:
                                context[ANX7447_i].anx_current_in_50ma = 10;
                                break;
                            case PO_TYPE_C_1_5A:
                                context[ANX7447_i].anx_current_in_50ma = 30;
                                break;
                            case PO_TYPE_C_3A:
                                context[ANX7447_i].anx_current_in_50ma = 60;
                                break;
                        }
                    }
                }
                ucsi_debug("voltage : %#x, current : %#x.\n",
                           context[ANX7447_i].anx_voltage_in_100mv,
                           context[ANX7447_i].anx_current_in_50ma);
            }
        }
    }
}

#if UCSI_SET_UOM_SUPPORT
static u8 anx_check_uom()
{
    switch (get_data_role())
    {
        case 1:                //DFP
            role = OP_DFP;
            break;
        case 0:                //UFP
            role = OP_UFP;
            break;
    }

    RamDebug(0x40 | role);
    ucsi_debug("check uom port(%#x) role(%#x), need role(%#x)\n",
               port_id, role, context[port_id].data_operation_mode_uom);

    if (context[port_id].data_operation_mode_uom == OP_DRP)
        return 0;

    if (context[port_id].data_operation_mode_uom & role)
        return 0;

    ucsi_debug("port(%#x) force power of, role(%#x), need role(%#x)\n",
               port_id, role, context[port_id].data_operation_mode_uom);

    chip_power_down(port_id);

    return 1;
}
#endif

void anx_alert_message_isr()
{
    if (0 == context[port_id].anx_power_status)
    {
        if (anx_read_reg(context[port_id].anx_spi, OCM_VERSION_REG)
                == 0x00)
        {
            anx_write_reg(context[port_id].anx_addr1, 0x10, 0xFF);
            anx_write_reg(context[port_id].anx_addr1, 0x11, 0xFF);
            return;
        }
    }

    /* Fix JIRA LBT-501 */
    anx_read_block_reg(context[port_id].anx_spi, CHANGE_INT, 2,
                       &change_int);
    //change_status = anx_read_reg(context[port_id].anx_spi, SYSTEM_STSTUS);
    anx_read_block_reg(context[port_id].anx_addr1, INTR_ALERT_0,
                       2, &intr_alert_0);
    //intr_alert_1 = anx_read_reg(context[port_id].anx_addr1, INTR_ALERT_1);
    //clear
    anx_write_reg(context[port_id].anx_spi, CHANGE_INT, 0x00);
    anx_write_block_reg(context[port_id].anx_addr1, INTR_ALERT_0,
                        2, &intr_alert_0);
    //anx_write_reg(context[port_id].anx_addr1, INTR_ALERT_1, intr_alert_1);

    /* power on process */
    if ((intr_alert_0 & INTR_SOFTWARE_INT) && (change_int & OCM_BOOT_UP))
    {
        usb_pd_function_init();
        retimer_power_ctrl(port_id, 1);
        ANX7447_j = anx_read_reg(context[port_id].anx_spi, OCM_VERSION_REG);
        ANX7447_k = anx_read_reg(context[port_id].anx_spi, OCM_VERSION_REG + 1);
        if (port_id == 0)
        {
            PDPort0MainVersion = ANX7447_j;
            PDPort0SubVersion = ANX7447_k;
        }
        else
        {
            PDPort1MainVersion = ANX7447_j;
            PDPort1SubVersion = ANX7447_k;
        }
        ucsi_debug("port(%#x) power on, ocm v%bx.%bx.%bx.\n",
                   port_id, (ANX7447_j >> 4) & 0xf, ANX7447_j & 0xf, ANX7447_k);

    #if UCSI_SET_UOM_SUPPORT
        if (anx_check_uom())
            return;
    #endif

            //send_initialized_setting();
        anx_initial_context(port_id);
        context[port_id].anx_power_status = 1;
        context[port_id].csc.csc.Connect = 1;
        context[port_id].ConnectorPartner = CP_USB;
        context[port_id].csc.csc.ConnectorPartner = 1;
        context[port_id].anx_initial_status = INIT_STAGE_1;
        //Fix JIRA LBT-215
        send_svid();
    #ifdef ANX_NO_CUST_DEF_DATA
        send_pd_msg(TYPE_SET_VAR, (u8 *)set_variable_1, 5, 0);
        //Fix JIRA LBT-411
        send_pd_msg(TYPE_SET_SNK_DP_CAP, (u8 *)dp_source_dp_caps, 4,
                    SOP_TYPE);
        //Fix JIRA LBT-414
        send_source_capability();
        send_sink_capability();
    #endif
        anx_ucsi_pin_assignment_event(port_id,
                                      SELECT_PIN_ASSIGMENT_U);
        //send_initialized_setting();
        context[port_id].csc.csc.BatteryChargingStatus = 1;
        context[port_id].csc.csc.PowerOperationMode = 1;
        /* LBT-865 */
        context[port_id].BatteryCharging = BCS_NOT_CHARGING;
        ucsi_async_notify_raise_up(150);
        anx_ucsi_attach_event(port_id);
        RamDebug(0xE8 | port_id);
    }

    if (intr_alert_1 & INTR_INTP_POW_OFF)
    {
        chip_power_down(port_id);
        ucsi_debug("port(%#x) power off, alert0(%#x), alert1(%#x).\n",
                   port_id, intr_alert_0, intr_alert_1);
        context[port_id].anx_power_status = 0;
        anx_initial_context(port_id);
        context[port_id].csc.csc.Connect = 1;
        ucsi_async_notify_raise_up(2);
        //anx_vbus_ctrl(context[port_id].vbus_gpio, 0);
    #ifdef ECR_CHECKING
        anx_ecr_checking();
    #endif
        context[port_id].anx_vbus_status = 0;
        anx_ucsi_detach_event(port_id);
        retimer_power_ctrl(port_id, 0);
        return;
    }

    /*Received interface message */
    if (intr_alert_0 & INTR_RECEIVED_MSG)
    {
        handle_intr_vector();
    }

    /*Received software interrupt */
    if (intr_alert_0 & INTR_SOFTWARE_INT)
    {

        if (change_int & HPD_STATUS_CHANGE)
        {
            context[port_id].hpd_status = change_status & HPD_STATUS;
            /* LBT-540 */
            anx_ucsi_hpd_event(port_id, context[port_id].hpd_status);
        }

        if (change_int & DATA_ROLE_CHANGE)
            anx_drole_change_event(port_id, get_data_role());

        if (change_int & VBUS_CHANGE)
        {
//context[port_id].enable_report_power_on = 1;
            context[port_id].csc.csc.PowerDirection = 1;
            context[port_id].power_sink =
                anx_read_reg(context[port_id].anx_spi,
                             SYSTEM_STSTUS) & SINK_STATUS;
            if (context[port_id].power_sink)
            {
            #ifdef ReportExternalSupply
                context[port_id].csc.csc.ExternalSupply = 1;
            #endif
            }
            anx_prole_change_event(port_id, !context[port_id].power_sink);

            ucsi_async_notify_raise_up(100);
            if (change_status & VBUS_CHANGE)
            {
                context[port_id].anx_vbus_status = 1;
                context[port_id].anx_power_role = 1;
                ucsi_debug("VBUS 5V OUT.\n");
                anx_vbus_event(port_id, 1);
            }
            else
            {
                anx_vbus_event(port_id, 0);
                //DISABLE_5V_VBUS_OUT();
                ucsi_debug("VBUS change to input.\n");
            #ifdef SUP_VBUS_CTL
                            //anx_vbus_ctrl(context[port_id].vbus_gpio, 0);
            #endif
                context[port_id].anx_power_role = 0;
                context[port_id].anx_vbus_status = 0;
            }
        }
        if (change_int & CC_STATUS_CHANGE)
        {
            change_status =
                anx_read_reg(context[port_id].anx_spi, NEW_CC_STATUS);
            if (plug_orientation(port_id))
                context[port_id].cc_orientation = CC1_CONNECTED;
            else
                context[port_id].cc_orientation = CC2_CONNECTED;
            /* LBT-540  */
            pd_cc_status_default_func(change_status);
        }
    }

    anx_dead_battery_checking_2(port_id);
}

static void anx7447_context_initial()
{
    //Addr select as : 0 0
    context[port_id].PD_standalone_slave_id1 = Liberty_i2c_address[port_id][0];
    context[port_id].PD_SPI_slave_id = Liberty_i2c_address[port_id][1];
    context[port_id].PD_EMTB_slave_id = Liberty_i2c_address[port_id][2];
    context[port_id].PD_EMRB_slave_id = Liberty_i2c_address[port_id][3];
    //context[port_id].alert_gpio = 1;
#ifdef ANX_OHO_DFP_ONLY
    context[port_id].dfp_only = 1;
#else
    context[port_id].dfp_only = 0;
#endif
    context[port_id].pd_src_pdo_cnt = 2;
    context[port_id].anx_power_status = 0;
    context[port_id].sel_voltage_pdo_index = 0x2;
    context[port_id].pd_capability = PD_HAS_RETURN_STATUS | PD_HAS_CABLE_VDO;
    context[port_id].error_count = 0;
}

static void anx7447_pup_initial()
{
    pup.alt_mode[0].SVID0 = 0xFF01;
    pup.alt_mode[0].MID0 = 0x00000405;
    pup.alt_mode_support_num = 1;
    pup.alt_mode_support_bitmap = 0;
    pup.current_alt_mode = 0;
}

/*
 * @desc   : ANX7447 ucsi module initialize all data structure.
 * @param  : none
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
void anx7447_ucsi_init()
{
    anx7447_pup_initial();
    for (ANX7447_i = 0; ANX7447_i < PD_MAX_INSTANCE; ANX7447_i++)
    {
        port_id = ANX7447_i;
        anx7447_context_initial();
        context[ANX7447_i].InterfaceRecvBuf = (u8 *)&InterfaceRecvBuf[ANX7447_i];
        context[ANX7447_i].InterfaceSendBuf = (u8 *)&InterfaceSendBuf[ANX7447_i];
        context[ANX7447_i].connector_index = ANX7447_i + 1;
    }

#ifdef PORT0_HAS_ANX_RETIMER
    /* LBT-800 */
    context[0].pd_capability |= PD_HAS_ANX_REDRIVER;
#endif
#ifdef PORT1_HAS_ANX_RETIMER
    /* LBT-800 */
    context[1].pd_capability |= PD_HAS_ANX_REDRIVER;
#endif
#ifdef PORT1_HAS_PARADE_REDRIVER
    context[1].pd_capability |= PD_HAS_PARADE_REDRIVER;
#endif
#if defined(PORT0_HAS_ANX_RETIMER) || defined(PORT1_HAS_ANX_RETIMER)
    anx_mht_retimer_gpio_init();
#endif
}

/* LBT-850 */
#include "CUSTOM_ANX7447_PORT.H"
#endif