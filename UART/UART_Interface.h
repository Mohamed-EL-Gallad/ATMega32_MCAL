/*
 * UART.h
 * Created on: 02/05/2021
 * Author: Mohamed_EL_Gallad
 *
 */

#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_

//#include "UART_private.h"

void UART_init(void);

void UART_SendDataFrame(UARTData_t SendData,u8 *ErrorCode);

u8 UART_ReceiveDataFrame(UARTData_t ReceivedData);

void UART_TransmitterStop(void);

void UART_Transmitter_ReEnable(void);

void UART_ReceiverStop(void);

void UART_Receiver_ReEnable(void);


#endif /* UART_INTERFACE_H_ */
