/*
 * UART.h
 * Created on: 02/05/2021
 * Author: Mohamed_EL_Gallad
 *
 */

#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_

#include "STD_types.h"


void UART_init(void);

void UART_SendDataFrame(u16 SentData);

u8 UART_ReceiveDataFrame(u16 ReceivedData);

void UART_TransmitterStop(void);

void UART_Transmitter_ReEnable(void);

void UART_ReceiverStop(void);

void UART_Receiver_ReEnable(void);


#endif /* UART_INTERFACE_H_ */
