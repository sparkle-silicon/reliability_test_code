/*
 * @Author: Linyu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2024-03-07 15:23:47
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
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"

#include "AE_ANX7447_UCSI.H"
#include "AE_ANX7447_LIB.H"
#include "CUSTOM_ANX7447_PORT.H"

#if SUPPORT_ANX7447

const u8 Liberty_i2c_address[PD_MAX_INSTANCE][4] = {
    PORT0_I2C_ADDR_MAP,
    PORT1_I2C_ADDR_MAP
};

u8  xEC_UCSIMajorVersion;
u8  xEC_UCSIMinorVersion;
u8    xEC_UCSIRevVersion;

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
u8 ANXXDATA ANX7447_i;
u8 ANXXDATA ANX7447_j;
u8 ANXXDATA ANX7447_k;
u8 ANXXDATA ANX7447_l;
extern struct ppm_ucsi_parameters ANXXDATA pup;
extern u8 ANXXDATA role;
u8 ANXXDATA ANX7447_c;
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

// void ucsi_debug(char *fmt, ...)
// {
// #ifdef ANXDEBUG
//     va_list ap;

//     va_start(ap, fmt);
//     vprintf(fmt, ap);
//     va_end(ap);
// #endif

//     return;
// }
void i2c_set_address(BYTE addr)
{
    // setup Charger TAR
    I2c_Master_Set_Tar(addr, I2C_REGADDR_7BIT, ANX7447_CHANNEL);

}
void ucsi_i2c_start(BYTE addr)
{
    i2c_set_address(addr);
    ucsi_debug("\n\tANX7447 UCSI ");
}
void ucsi_i2c_READ_DEBUG(void)
{
    ucsi_debug("READ\n");
}
void ucsi_i2c_WRITE_DEBUG(void)
{
    ucsi_debug("WRITE\n");
}
void ucsi_i2c_debug(BYTE addr, BYTE reg, BYTE length, BYTE *data)
{
    ucsi_debug("ADDRESS REGISTER OFFSET DATA\n");
    for(BYTE cnt = 0; cnt < length; cnt++)
        ucsi_debug(" %#2x     %#2x    %#2x  %#2x\n", addr & 0xff, reg & 0xff, cnt & 0xff, (*(data + cnt)) & 0xff);
}

BYTE i2c_read_reg(BYTE i2c_addr, BYTE reg)
{
    BYTE datal = 0;
    ucsi_i2c_start(i2c_addr);
    ucsi_i2c_READ_DEBUG();
    datal = (I2c_Master_Read_Byte(reg, ANX7447_CHANNEL));
    ucsi_i2c_debug(((i2c_addr << 1) | 0b1), reg, 1, &datal);
    return datal;
}
void i2c_read_block_reg(BYTE i2c_addr, BYTE reg, BYTE length, BYTE *buf)
{
    ucsi_i2c_start(i2c_addr);//占位符，后续可能有用
    ucsi_i2c_READ_DEBUG();
    I2c_Master_Read_Block(buf, length, reg, ANX7447_CHANNEL);
    ucsi_i2c_debug(((i2c_addr << 1) | 0b1), reg, length, buf);
}
void i2c_write_reg(BYTE i2c_addr, BYTE reg, BYTE data1)
{
    ucsi_i2c_start(i2c_addr);
    ucsi_i2c_WRITE_DEBUG();
    I2c_Master_Write_Byte(data1, reg, ANX7447_CHANNEL);
    ucsi_i2c_debug(((i2c_addr << 1) | 0b0), reg, 1, &data1);
}
void i2c_write_block_reg_b(BYTE i2c_addr, BYTE reg, BYTE length, BYTE *buf)
{
    ucsi_i2c_start(i2c_addr);//占位符，后续可能有用
    ucsi_i2c_WRITE_DEBUG();
    I2c_Master_Write_Block(buf, length, reg, ANX7447_CHANNEL);
    ucsi_i2c_debug(((i2c_addr << 1) | 0b0), reg, length, buf);
}

/*The following lists the BIOS notification handler that the EC defines.*/
void ucsi_ppm_notify_opm(void)
{
// Notify BIOS that we have message.
// Transfer shared ppm2bios to BIOS.
}


/* LBT-540 */
/*
 * @desc   : anx_ucsi_cc_status_event, EC may need add cc status event processing
 * @param  : port_id     -> port id number (0 or 1)
 *           cc_status   ->  cc status
 *           typec_mode  ->  cc status: PO_USB_DEFAULT or PO_TYPE_C_1_5A or PO_TYPE_C_3A
 * @return : none
 */
void anx_ucsi_cc_status_event(u8 port_id, u8 cc_status, u8 typec_mode)
{
    if(cc_status == 0)
        return;

    //customer add interrupt processing.
    //cc direction -> context[port_id].cc_orientation
    //  value :   CC1_CONNECTED or CC2_CONNECTED
}

/*
 * @desc   : anx_ucsi_pin_assignment_event, EC may add pin assignment event processing
 * @param  : port_id -> port id number (0 or 1)
 *           pin_assignment      ->  pin assignment
 *                                   SELECT_PIN_ASSIGMENT_C, SELECT_PIN_ASSIGMENT_D
 *                                   SELECT_PIN_ASSIGMENT_E,  SELECT_PIN_ASSIGMENT_U
 * @return : none
 */
void anx_ucsi_pin_assignment_event(u8 port_id, u8 pin_assignment)
{
    //customer add interrupt processing.
    switch(pin_assignment)
    {
        case SELECT_PIN_ASSIGMENT_U:
            //default 4 line USB 3.1
            break;
        case SELECT_PIN_ASSIGMENT_C:
        case SELECT_PIN_ASSIGMENT_E:
            // 4 line DP
            break;
        case SELECT_PIN_ASSIGMENT_D:
            // 2 line DP, 2 line USB
            break;
    }

    retimer_pin_assignment_set(port_id, pin_assignment);
}

/*
 * @desc   : anx_usb_state_get , EC call this to get USB state
 *
 * @param  : port_id -> port id number (0 or 1)
 * @return : USB2 / USB31_GEN1_USB2 / USB31_GEN1_GEN2_USB2 / USB2_BILLBOARD
 */
u8 anx_usb_state_get(u8 port_id)
{
    if(context[port_id].anx_power_status)
        return context[port_id].usb_state;

    return USB_INVALID;
}

/*
 * @desc   : anx_ucsi_hpd_event, EC may add HPD event processing
 * @param  : port_id -> port id number (0 or 1)
 *                 hpd      -> 1:HPD high, 0 HPD low
 * @return : none
 */
void anx_ucsi_hpd_event(u8 port_id, u8 hpd)
{
    //customer may add event processing.
}

/*
 * @desc   : anx_ucsi_detach_event, EC may add detach event processing
 * @param  : port_id -> port id number (0 or 1)
 * @return : none
 */
void anx_ucsi_detach_event(u8 port_id)
{
    // customer may add detach event processing
}

/*
 * @desc   : anx_ucsi_attach_event, EC may add attach event processing
 * @param  : port_id -> port id number (0 or 1)
 * @return : none
 */
void anx_ucsi_attach_event(u8 port_id)
{
    // customer may add attach event processing
}

/*
 * @desc   : anx_ucsi_renotify_OPM, EC may call it at reboot stage
 * @param  : port_id -> port id number (0 or 1)
 * @return : none
 */
void anx_ucsi_renotify_OPM(u8 port_id)
{
    // customer may add detach event processing, include reset MUX/HPD
    if(context[port_id].anx_power_status == 1)
    {
        ucsi_debug("re-notify OPM at reboot stage.\n");
        context[port_id].csc.csc.Connect = 1;
        context[port_id].csc.csc.ConnectorPartner = 1;
        context[port_id].csc.csc.BatteryChargingStatus = 1;
        if(context[port_id].ucsi_partner_rdo)
        {
        #ifdef ReportNegotiatedPowerLevel
            context[port_id].csc.csc.NegotiatedPowerLevel = 1;
        #endif
        #ifdef ReportSupportedProviderCap
            context[port_id].csc.csc.SupportedProviderCap = 1;
        #endif
        }
        ucsi_async_notify_raise_up(250);
    }
}

/*
 * @desc   : anx_ucsi_vdm_get_status, automaticly call it to send
 *           "get status" VDM command after into enter mode.
 * @param  : port_id -> port id number (0 or 1)
 * @return : none
 */
void anx_ucsi_vdm_get_status(u8 port_id)
{
    u8 vdm_data[4];

    vdm_data[0] = 0x10;
    vdm_data[1] = 0xa0;
    vdm_data[2] = CUST_SVID & 0xFF;
    vdm_data[3] = (CUST_SVID >> 8) & 0xFF;

    send_pd_msg(TYPE_VDM, vdm_data, 4, SOP_TYPE);
}

/*
 * @desc   : anx_ucsi_vdm_callback, this callback receive all customer
 *           defined VDM message.
 * @param  : port_id -> port id number (0 or 1)
 *         : buf     -> VDM message(include VDM header)
 *         : len     -> buffer length
 * @return : none
 */
void anx_ucsi_vdm_callback(u8 port_id, u8 *buf, u8 len)
{
    u8 cmd;
    u16 svid = (buf[3] << 8) | buf[2];
    ucsi_debug("VDM header: %#x:%#x:%#x:%#x.\n", buf[0], buf[1],
               buf[2], buf[3]);
    cmd = buf[0] & VDM_CMD_AND_ACK_MASK;
    switch(cmd)
    {
        case (VDM_ACK | VDM_CMD_GET_STS):
            /* VDM return status, command 0x10 */
            //VDO1 at context[port_id].InterfaceRecvBuf[6 ~ 9]
            ucsi_debug("VDO1 %#x:%#x:%#x:%#x.\n", buf[4], buf[5], buf[6],
                       buf[7]);
            //VDO2 at context[port_id].InterfaceRecvBuf[10 ~ 13]
            ucsi_debug("VDO2 %#x:%#x:%#x:%#x.\n", buf[8], buf[9], buf[10],
                       buf[11]);
            break;
        case (VDM_CMD_ATTENTION):
            ucsi_debug("Attention\n");
            break;
        case (VDM_ACK | VDM_CMD_DIS_MODE):
            context[port_id].peer_svid_length += 4;
            context[port_id].ac[1].svid = svid;
            memcpy(&context[port_id].ac[1].mid, &buf[4], 4);
            break;
        default:
            break;
    }
}

/*
 * @desc   : anx_request_voltage_in_100mv, EC should call this to check the
 *           voltage while Type-C connector work as power sink.
 * @param  : port_id -> port id number (0 or 1)
 * @return :
 *           0 : Type-C connector work as power source.
 *           other : voltage, unit is 100mv.
 */
u8 anx_request_voltage_in_100mv(u8 port_id)
{
    u8 vol = 0;

    if(self_charging_error == 1)
        return vol;
    if(context[port_id].anx_power_status == 0
            || context[port_id].anx_power_role == 1)
        return 0;

    vol = (context[port_id].anx_voltage_in_100mv == 0) ? 50 :
        context[port_id].anx_voltage_in_100mv;

    return vol;
}

/*
 * @desc   : anx_request_current_in_50ma, EC should call this to check the
 *           current while Type-C connector work as power sink.
 * @param  : port_id -> port id number (0 or 1)
 * @return :
 *           0 : Type-C connector work as power source.
 *           other : current, unit is 50ma.
 */
u8 anx_request_current_in_50ma(u8 port_id)
{
    u8 cur = 0;

    if(self_charging_error == 1)
        return cur;
    if(context[port_id].anx_power_status == 0
            || context[port_id].anx_power_role == 1)
        return cur;

    cur = context[port_id].anx_current_in_50ma;

    return cur;
}

/*
 * @desc   : anx_connector_power_status, EC should call this to check whether
 *           the PD chip is power provider or consumer
 * @param  : port_id -> port id number (0 or 1)
 * @return :
 *           0 : power provider or PD standby
 *           other : power consumer
 */
u8 anx_connector_power_status(u8 port_id)
{
    /* LBT-800 */
    return ((context[port_id].anx_power_status == 1) &&
            (context[port_id].anx_power_role == 0)
            && context[port_id].power_sink);
}

/*
 * @desc   : anx_vbus_event, EC may need to known VBUS on/off event
 * @param  : port_id -> port id number (0 or 1)
 *           on      ->  vbus status, 0: off, other: on
 * @return : none
 */
void anx_vbus_event(u8 port_id, u8 on)
{
    /* Add customer vbus detect code*/
}

/*
 * @desc   : anx_get_peer_source_pdo_event, EC may need to known partner source PDO
 * @param  : port_id -> port id number (0 or 1)
 *           buf     -> source pdo buffer
 *           buf_len -> source pdo buffer length
 * @return : none
 */
void anx_get_peer_source_pdo_event(u8 port_id, u8 *buf, u8 buf_len)
{}

/*
 * @desc   : anx_get_rdo_event, EC may need to known RDO
 * @param  : port_id -> port id number (0 or 1)
 *           rdo     -> RDO
 * @return : none
 */
void anx_get_rdo_event(u8 port_id, u32 rdo)
{}

/*
 * @desc   : anx_drole_change_event, EC may need to known data role change event
 * @param  : port_id -> port id number (0 or 1)
 *           drole   -> data role, 0: UFP, 1: DFP
 * @return : none
 */
void anx_drole_change_event(u8 port_id, u8 drole)
{}

/*
 * @desc   : anx_prole_change_event, EC may need to known power role change event
 * @param  : port_id -> port id number (0 or 1)
 *           prole   -> power role, 0: sink, 1: source
 * @return : none
 */
void anx_prole_change_event(u8 port_id, u8 prole)
{}

/*
 * @desc   : anx_mht_retimer_gpio_init, EC should call this to
 *           initial MHT retimer control GPIO
 * @param  : none
 * @return : none
 */
void anx_mht_retimer_gpio_init()
{
    /* LBT-800 */
    // char buf[1];
    ucsi_debug("init redriver/sink_ctrl gpio\n");
    /* PORT 0 REDIRVER GPIO */
    PORT0_POWER_CTRL(0);            /* power control */
    PORT0_SLINK_CTRL(0);            /* sink control */
    /* PORT 1 REDRIVER GPIO */
    PORT1_POWER_CTRL(0);            /* power control */
    PORT1_SLINK_CTRL(0);            /* sink control */
    /* PORT 0 ROLE SEL */
    /* PORT 1 ROLE SEL */
    PORT0_ROLE_SEL(0);
    PORT1_ROLE_SEL(0);

    PORT_ROLE_SEL(1);  /* port 1 default DRP */

    for(ANX7447_i = 0; ANX7447_i < PD_MAX_INSTANCE; ANX7447_i++)
    {
        ucsi_debug("port(%d) power down\n", ANX7447_i);
        chip_power_down(ANX7447_i);

        ucsi_debug("port(%d) power on\n", ANX7447_i);
        chip_power_on(ANX7447_i);
    }
}

void anx_mht_retimer_power_ctrl(u8 port, u8 on)
{
    if(port == 1)
    {
        if(on)
        {
            PORT0_POWER_CTRL(1);
            ucsi_debug("port(%d) power on redriver\n", port);
        }
        else
        {
            PORT0_POWER_CTRL(0);
            ucsi_debug("port(%d) power off redriver\n", port);
        }
    }
    else
    {
        if(on)
        {
            PORT1_POWER_CTRL(1);
            ucsi_debug("port(%d) power on redriver\n", port);
        }
        else
        {
            PORT1_POWER_CTRL(0);
            ucsi_debug("port(%d) power off redriver\n", port);
        }
    }
}

/* parade redriver control */
static void parade_redriver_mux_control(unsigned char mode, unsigned char cc_direction)
{
    unsigned char buf[3];
    buf[0] = REDRIVER_ADDRESS;  //address
    buf[1] = REDRIVER_OFFSET;   //offset
    if(cc_direction == CC1_CONNECTED)
    {
        if(mode == USB3_1_DP_2LANES)
        {
            buf[2] = 0xf8;      //data
        }
        else if(mode == DP_ALT_4LANES)
        {
            buf[2] = 0xe8;      //data
        }
        else if(mode == USB3_1_CONNECTED)
        {
            buf[2] = 0xb8;      //data
        }
    }
    else
    {
        if(mode == USB3_1_DP_2LANES)
        {
            buf[2] = 0xfc;      //data
        }
        else if(mode == DP_ALT_4LANES)
        {
            buf[2] = 0xec;      //data
        }
        else if(mode == USB3_1_CONNECTED)
        {
            buf[2] = 0xbc;      //data
        }
    }
    send_pd_msg((PD_MSG_TYPE)TYPE_I2C_MASTER_WRITE, buf, 3, 0);
}

/* parade redriver control */
static void parade_redriver_ctrl(u8 port_id, unsigned char mode)
{
    unsigned char mux_mode;
    //Fix JIRA LBT-302
    if((mode == SELECT_PIN_ASSIGMENT_C)
            || (mode == SELECT_PIN_ASSIGMENT_E))
        mux_mode = DP_ALT_4LANES;
    else if(mode == SELECT_PIN_ASSIGMENT_D)
        mux_mode = USB3_1_DP_2LANES;
    else
        mux_mode = USB3_1_CONNECTED;

    if(context[port_id].mux_mode != mux_mode)
    {
        parade_redriver_mux_control(mux_mode,
                                    context[port_id].cc_orientation);
        context[port_id].mux_mode = mux_mode;
    }
}

/*
 * @desc   : retimer_power_ctrl, control retimer power on and off
 * @param  : port_id -> port id number (0 or 1)
 *           retimer_power      ->  1: power on, 0: power off
 * @return : none
 */
void retimer_power_ctrl(u8 port_id, u8 retimer_power)
{
    if(context[port_id].pd_capability & PD_HAS_ANX_REDRIVER)
        anx_mht_retimer_power_ctrl(port_id, retimer_power);

    /* Add customer defined retimer power control */
}

/*
 * @desc   : retimer_pin_assignment_set, EC may add pin assignment event processing
 * @param  : port_id -> port id number (0 or 1)
 *           pin_assignment      ->  pin assignment
 *                                   SELECT_PIN_ASSIGMENT_C, SELECT_PIN_ASSIGMENT_D
 *                                   SELECT_PIN_ASSIGMENT_E,  SELECT_PIN_ASSIGMENT_U
 * @return : none
 */
void retimer_pin_assignment_set(u8 port_id, u8 mux)
{
    if(context[port_id].pd_capability & PD_HAS_PARADE_REDRIVER)
        parade_redriver_ctrl(port_id, mux);

    /* Add customer defined retimer control */
}
#endif