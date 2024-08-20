/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-03-07 15:21:54
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
 *
 * Portions of this firmware library utilize the ANX7447 driver, which is copyrighted by Analogix Semiconductor, Inc.
 * 本固件库的部分代码使用了 ANX7447 驱动程序，其版权归 Analogix Semiconductor, Inc. 所有。
 */
#ifndef _CUSTOM_ANX7447_PORT_H_
#define _CUSTOM_ANX7447_PORT_H_

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"

#define ANX_UCSI_VERSION_MAJOR       0x1
#define ANX_UCSI_VERSION_MINOR       0x2
#define ANX_UCSI_VERSION_REV         0x4

// #define EC_ANX_BIG_ENDIAN

#define CUST_SVID            0xFF01
/* PDP default 15W */
#define CUST_PDP             15
#define ANX_NO_CUST_DEF_DATA
#define UCSI_TIMEOUT_NOTIFY

#define ANX7447_CHANNEL //I2C_CHANNEL_0
#define IS_BATTERY1_OUT()  //(Read_BATT_PRS_1()?1:0)
#define IS_BATTERY1_IN()  //(Read_BATT_PRS_1()?0:1)

//#define INTEL_GLK_MRD

#ifdef INTEL_GLK_MRD

/* PORT 0 status define */
#define HAS_PORT0
#define PORT0_DETECT_INTERRUPT       IS_USBPD1_INT_N_LO()
#define READ_USBPD1_SOURCE_CTRL             
#define READ_USBPD1_SINK_CTRL  
#define PORT0_VBUS_INPUT             USBPD1_VBUS_INPUT_ON()//USBPD1_VBUS_OUTPUT_OFF()
#define PORT0_VBUS_OUTPUT            USBPD1_VBUS_OUTPUT_ON()//USBPD1_VBUS_INPUT_OFF()
#define PORT0_I2C_ADDR_MAP           {0x58, 0x7e, 0x7a, 0x84}

/* PORT 1 status define */
#define HAS_PORT1
#define PORT1_I2C_ADDR_MAP           {0x54, 0x64, 0x68, 0x6c}
#define PORT1_DETECT_INTERRUPT       IS_USBPD2_INT_N_LO()
#define READ_USBPD2_SOURCE_CTRL             
#define READ_USBPD2_SINK_CTRL   
#define PORT1_VBUS_INPUT             USBPD2_VBUS_INPUT_ON()//USBPD2_VBUS_OUTPUT_OFF()
#define PORT1_VBUS_OUTPUT            USBPD2_VBUS_OUTPUT_ON()//USBPD2_VBUS_INPUT_OFF()
#define PORT1_HAS_PARADE_REDRIVER

#else
#define PORT_ROLE_SEL(x)


/* PORT 0 status define */
#define HAS_PORT0
#define PORT0_DETECT_INTERRUPT       IS_USBPD2_INT_N_LO()
#define READ_USBPD1_SOURCE_CTRL             
#define READ_USBPD1_SINK_CTRL  
#define PORT0_VBUS_INPUT             USBPD2_VBUS_INPUT_ON()//USBPD1_VBUS_OUTPUT_OFF()
#define PORT0_VBUS_OUTPUT            USBPD2_VBUS_OUTPUT_ON()//USBPD1_VBUS_INPUT_OFF()
#define PORT0_POWER_CTRL(x)             
#define PORT0_SLINK_CTRL(x)            
#define PORT0_ROLE_SEL(x)  
#define PORT0_I2C_ADDR_MAP           //{0x58, 0x7e, 0x7a, 0x84}
#define PORT0_HAS_ANX_RETIMER

/* PORT 1 status define */
#define HAS_PORT1
#define PORT1_I2C_ADDR_MAP           //{0x54, 0x64, 0x68, 0x6c}
#define PORT1_DETECT_INTERRUPT       IS_USBPD1_INT_N_LO()
#define READ_USBPD2_SOURCE_CTRL             
#define READ_USBPD2_SINK_CTRL   
#define PORT1_VBUS_INPUT             USBPD1_VBUS_INPUT_ON()//USBPD2_VBUS_OUTPUT_OFF()
#define PORT1_VBUS_OUTPUT            USBPD1_VBUS_OUTPUT_ON()//USBPD2_VBUS_INPUT_OFF()
#define PORT1_POWER_CTRL(x)             
#define PORT1_SLINK_CTRL(x)            
#define PORT1_ROLE_SEL(x)

#define PORT1_HAS_ANX_RETIMER
#endif

#if defined(HAS_PORT0) && defined(HAS_PORT1)
#define PD_MAX_INSTANCE    2
#else
#define PD_MAX_INSTANCE    1
#endif

#ifndef PORT1_I2C_ADDR_MAP
#define PORT1_I2C_ADDR_MAP
#endif

#define ANXDEBUG ANX7447_UCSI_DEBUG
/*enable ANX debug mode*/
#if ANXDEBUG
#define ucsi_debug dprint
#define RamDebug(x) ucsi_debug("RAM DEBUG(%#2x)\n",(u8)x)
#else
#define ucsi_debug 
#define RamDebug(x) //PORT80_SETBRAM(x)
#endif

#define VENDOR_ID    0x1F29
#define PRODUCT_ID   0x7447

#define PD_MAX_VOLTAGE 0xc8     /*20V, 100mv per unit */
#define PD_MAX_POWER   0x82     /*65W, 500mW per unit */

/* Default use PD 2.0, or PD 3.0 */
#define USE_PD_20

// #define UCSI_USED_STRUCTURE_MEMORY 
#ifdef UCSI_USED_STRUCTURE_MEMORY
/* Define 256 bytes UCSI data structure memory */
extern char ucsi_base_mem[256];
#define UCSI_baseaddr (&ucsi_base_mem[0])
/* Define 512 bytes Liberty data structure memory */
extern char anxlib_base_mem[512];
#define ANXLIB_baseaddr (&anxlib_base_mem[0])
#endif
#define NORMAL_CHARGING_POWER           45
#define SLOW_CHARGING_POWER             27
#define VERY_SLOW_CHARGING_POWER        15

#define VDM_ACK              0x40
#define VDM_CMD_RES          0x00
#define VDM_CMD_DIS_ID       0x01
#define VDM_CMD_DIS_SVID     0x02
#define VDM_CMD_DIS_MODE     0x03
#define VDM_CMD_ENTER_MODE   0x04
#define VDM_CMD_EXIT_MODE    0x05
#define VDM_CMD_ATTENTION    0x06
#define VDM_CMD_GET_STS      0x10
#define VDM_CMD_AND_ACK_MASK 0x5F

#define UCSI_VERSION        xUCSI_DS_VERSION
#define UCSI_DS_CCI0        xUCSI_DS_CCI0
#define UCSI_DS_CCI1        xUCSI_DS_CCI1
#define UCSI_DS_CCI2        xUCSI_DS_CCI2
#define UCSI_DS_CCI3        xUCSI_DS_CCI3
#define UCSI_COMMAND        xUCSI_COMMAND
#define UCSI_DS_MGI         xUCSI_DS_MGI
extern u8  xEC_UCSIMajorVersion;
extern u8  xEC_UCSIMinorVersion;
extern u8    xEC_UCSIRevVersion;
#define EC_UCSIMajorVersion xEC_UCSIMajorVersion
#define EC_UCSIMinorVersion xEC_UCSIMinorVersion
#define EC_UCSIRevVersion   xEC_UCSIRevVersion
#define PDPort0MainVersion  xEC_PDPort0MainVersion
#define PDPort0SubVersion   xEC_PDPort0SubVersion
#define PDPort1MainVersion  xEC_PDPort1MainVersion
#define PDPort1SubVersion   xEC_PDPort1SubVersion

#define ANX_FRS_EN_AT_I2C_ADR

#define ReportNegotiatedPowerLevel
#define ReportSupportedProviderCap
#define ReportPDResetComplete
#define ReportSupportedCAM
#define ReportExternalSupply

#define CMD_OPTIONAL_SUPPORT    \
        UCSI_SET_UOM_SUPPORT    | \
        UCSI_SET_PDM_SUPPORT    | \
        UCSI_ALT_SUPPORT        | \
        UCSI_ALT_OR_SUPPORT     | \
        UCSI_PDOS_SUPPORT       | \
        UCSI_CABLE_SUPPORT      | \
        UCSI_EXTERNAL_SUPPORT   | \
        UCSI_PD_RESET_SUPPORT


/* millisecond delay */
#define DelayXms vDelayXms
#define mdelay DelayXms

/*
 * @desc   : I2C read one register
 * @param  :
 *           i2c_addr -> i2c slave address
 *           reg -> register offset
 * @return :
 *           return register value if success
 */
extern BYTE i2c_read_reg(BYTE i2c_addr, BYTE reg);
//u8 anx_read_reg(u8 i2c_addr, u8 reg);
#define anx_read_reg i2c_read_reg
/*
 * @desc   : I2C write one register
 * @param  :
 *           i2c_addr -> i2c slave address
 *           reg -> register offset
 *           data -> value which will be wrote
 * @return : none
 */
void i2c_write_reg(u8 i2c_addr, u8 reg, u8 data1);
//void anx_write_reg(u8 i2c_addr, u8 reg, u8 data1);
#define anx_write_reg i2c_write_reg

/*
 * @desc   : I2C read block registers
 * @param  :
 *           i2c_addr -> i2c slave address
 *           reg -> register offset
 *           length -> the amount of read register
 *           buf -> value stored to
 * @return : none
 */
void i2c_read_block_reg(u8 i2c_addr, u8 reg, u8 length, u8 *buf);
//void anx_read_block_reg(u8 i2c_addr, u8 reg, u8 length, u8 *buf);
#define anx_read_block_reg i2c_read_block_reg

/*
 * @desc   : I2C write block registers
 * @param  :
 *           i2c_addr -> i2c slave address
 *           reg -> register offset
 *           length -> the amount of read register
 *           buf -> value stored to
 * @return : none
 */
void i2c_write_block_reg_b(u8 i2c_addr, u8 reg, u8 length, u8 *buf);
//void anx_write_block_reg(u8 i2c_addr, u8 reg, u8 length, u8 *buf);
#define anx_write_block_reg i2c_write_block_reg_b


extern u8 pd_fw_updated;
void ucsi_async_notify_raise_up(u8 ms);
void cancel_all_timer();

/*
 * @desc   : anx_request_voltage_in_100mv, EC should call this to check the
 *           voltage while Type-C connector work as power sink.
 * @param  : port_id -> port id number (0 or 1)
 * @return :
 *           0 : Type-C connector work as power source.
 *           other : voltage, unit is 100mv.
 */
u8 anx_request_voltage_in_100mv(u8 port_id);
/*
 * @desc   : anx_request_current_in_50ma, EC should call this to check the
 *           current while Type-C connector work as power sink.
 * @param  : port_id -> port id number (0 or 1)
 * @return :
 *           0 : Type-C connector work as power source.
 *           other : current, unit is 50ma.
 */
u8 anx_request_current_in_50ma(u8 port_id);
/*
 * @desc   : anx_connector_power_status, EC should call this to check whether
 *           the PD chip is power on or power off
 * @param  : port_id -> port id number (0 or 1)
 * @return :
 *           0 : power off
 *           other : power on
 */
u8 anx_connector_power_status(u8 port_id);
/*
 * @desc   : anx_vbus_event, EC may need to known VBUS on/off event
 * @param  : port_id -> port id number (0 or 1)
 *           on      ->  vbus status, 0: off, other: on
 * @return : none
 */
void anx_vbus_event(u8 port_id, u8 on);
/*
 * @desc   : anx_get_peer_source_pdo_event, EC may need to known partner source PDO
 * @param  : port_id -> port id number (0 or 1)
 *           buf     -> source pdo buffer
 *           buf_len -> source pdo buffer length
 * @return : none
 */
void anx_get_peer_source_pdo_event(u8 port_id, u8 *buf, u8 buf_len);
/*
 * @desc   : anx_get_rdo_event, EC may need to known RDO
 * @param  : port_id -> port id number (0 or 1)
 *           rdo     -> RDO
 * @return : none
 */
void anx_get_rdo_event(u8 port_id, u32 rdo);
/*
 * @desc   : anx_drole_change_event, EC may need to known data role change event
 * @param  : port_id -> port id number (0 or 1)
 *           drole   -> data role, 0: UFP, 1: DFP
 * @return : none
 */
void anx_drole_change_event(u8 port_id, u8 drole);
/*
 * @desc   : anx_prole_change_event, EC may need to known power role change event
 * @param  : port_id -> port id number (0 or 1)
 *           prole   -> power role, 0: sink, 1: source
 * @return : none
 */
void anx_prole_change_event(u8 port_id, u8 prole);
/*
 * @desc   : anx_ucsi_cc_status_event, EC may need add cc status event processing
 * @param  : port_id     -> port id number (0 or 1)
 *           cc_status   ->  cc status
 *           typec_mode  ->  cc status: PO_USB_DEFAULT or PO_TYPE_C_1_5A or PO_TYPE_C_3A
 * @return : none
 */
void anx_ucsi_cc_status_event(u8 port_id, u8 cc_status, u8 typec_mode);
/*
 * @desc   : anx_ucsi_pin_assignment_event, EC may add pin assignment event processing
 * @param  : port_id -> port id number (0 or 1)
 *                 pin_assignment      ->  pin assignment
                                     SELECT_PIN_ASSIGMENT_C, SELECT_PIN_ASSIGMENT_D
                                     SELECT_PIN_ASSIGMENT_E,  SELECT_PIN_ASSIGMENT_U
 * @return : none
 */
void anx_ucsi_pin_assignment_event(u8 port_id, u8 pin_assignment);

/*
 * @desc   : anx_usb_state_get , EC call this to get USB state
 *
 * @param  : port_id -> port id number (0 or 1)
 * @return : USB2 / USB31_GEN1_USB2 / USB31_GEN1_GEN2_USB2 / USB2_BILLBOARD
 */
u8 anx_usb_state_get(u8 port_id);

/*
 * @desc   : anx_ucsi_hpd_event, EC may add HPD event processing
 * @param  : port_id -> port id number (0 or 1)
 *                 hpd      -> 1:HPD high, 0 HPD low
 * @return : none
 */
void anx_ucsi_hpd_event(u8 port_id, u8 hpd);

/*
 * @desc   : anx_ucsi_detach_event, EC may add detach event processing
 * @param  : port_id -> port id number (0 or 1)
 * @return : none
 */
void anx_ucsi_detach_event(u8 port_id);

/*
 * @desc   : anx_ucsi_attach_event, EC may add attach event processing
 * @param  : port_id -> port id number (0 or 1)
 * @return : none
 */
void anx_ucsi_attach_event(u8 port_id);

/*
 * @desc   : anx_ucsi_vdm_get_status, automaticly call it to send
 *           "get status" VDM command after into enter mode.
 * @param  : port_id -> port id number (0 or 1)
 * @return : none
 */
void anx_ucsi_vdm_get_status(u8 port_id);

/*
 * @desc   : anx_ucsi_vdm_callback, this callback receive all customer
 *           defined VDM message.
 * @param  : port_id -> port id number (0 or 1)
 *         : buf     -> VDM message(include VDM header)
 *         : len     -> buffer length
 * @return : none
 */
void anx_ucsi_vdm_callback(u8 port_id, u8 *buf, u8 len);

/*
 * @desc   : anx_mht_retimer_gpio_init, EC should call this to
 *           initial MHT retimer control GPIO
 * @param  : none
 * @return : none
 */
void anx_mht_retimer_gpio_init();

/*
 * @desc   : retimer_pin_assignment_set, EC may add pin assignment event processing
 * @param  : port_id -> port id number (0 or 1)
 *           pin_assignment      ->  pin assignment
 *                                   SELECT_PIN_ASSIGMENT_C, SELECT_PIN_ASSIGMENT_D
 *                                   SELECT_PIN_ASSIGMENT_E,  SELECT_PIN_ASSIGMENT_U
 * @return : none
 */
void retimer_pin_assignment_set(u8 port_id, u8 mux);

/*
 * @desc   : retimer_power_ctrl, control retimer power on and off
 * @param  : port_id -> port id number (0 or 1)
 *           retimer_power      ->  1: power on, 0: power off
 * @return : none
 */
void retimer_power_ctrl(u8 port_id, u8 retimer_power);

extern const u8 Liberty_i2c_address[PD_MAX_INSTANCE][4];

/* EC call this interface while received command from OPM */
void anx_ucsi_command_received(void);

/* 10ms timer, process UCSI command */
void ANX_HOOK_10ms(void);

/* 1ms timer, polling port interrupt */
void ANX_HOOK_1ms(void);

/*The following lists the BIOS notification handler that the EC defines.*/
void ucsi_ppm_notify_opm(void);

#endif