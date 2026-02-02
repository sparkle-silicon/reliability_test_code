#ifndef __KERNEL_RELIABILITY_H
#define __KERNEL_RELIABILITY_H
#include "AE_GLOBAL.H"
#include "KERNEL_GPIO.H"

#define RELIABILITY_BUFF_SIZE 64
#define ENABLE_RELIABILITY_TEST 1
extern uint8_t MCU_ID;
extern uint8_t Reliability_Rx_Buffer[RELIABILITY_BUFF_SIZE];
extern uint8_t Reliability_Tx_Buffer[RELIABILITY_BUFF_SIZE];
extern uint8_t Reliability_Complete_Flag;
extern uint16_t Reliability_Rx_Cnt;
extern uint8_t Reliability_Expected_Len;

extern void Reply_Voltage_Query(void);
extern void Get_Flash_MCU_ID(void);
extern uint16_t CRC16_Generate(uint8_t *pData, uint16_t Length);
extern void SendResonse(uint8_t command, uint16_t length, uint8_t *buf);
#endif
