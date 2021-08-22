/*
 * UART_Interface.h
 * Created on: 02/05/2021
 * Author: Mohamed_EL_Gallad
 *
 * Brief: This file will contain the UART module interface functions
 * the UART interface is based on TX,RX interrupts and circular buffer for both TX and RX
 */

#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_
#include "UART_Config.h"
#include "UART_private.h"


/**
 * RETURN      : void
 * PARAMETERS  : void
 * DESCRIPTION : This function is used to initiate the UART module depending on the user's configuration
 * this function will set the following
 *     1- Baud Rate
 *     2- UART operation mode
 *           2-1- Normal Asynchronous Mode
 *           2-2- Double Speed Asynchronous Mode
 *           2-3- Synchronous Master Mode
 *           2-4- Synchronous Slave Mode
 *     3- Number of stop Bits "1 stop bit , 2 stop bits"
 *     4- Parity Mode
 *     5- Frame size configuration "5 , 6 , 7, 8 ,9 Bits frame modes"
 *     6- Enable the Transmitter and the receiver and their buffers
 */
void UART_init(void);


/**
 * RETURN      : void
 * PARAMETERS  :SendData has a type of UARTData_t which will be resolved automatically to either u8 or u16 data type
 *               depending on the frame size
 *               the user can declare the variable with the type UARTData_t and it will be automatically resolved to the correct data size
 *               or declare the variable as a u16 variable in case of 9bits frame size is being used
 *               or declare the variable as a u8 variable in case of 6,7,8bits frame size is being used
 *             :ErrorCode is a pointer to a u8 variable that has to be defined by the user before calling this function
 *               this variable will store the error code that may be produced when the data is being pushed to the TX_Buffer
 *               it will have one of the following values
 *               ERROR_BUFFER_FULL    0x15 which means that the TX_Buffer is full and no room for more data to be pushed
 *               SUCCESSFUL_OPERATION 0x17 which means that the user data has been pushed successfully to the TX_Buffer and to be transmitted
 * DESCRIPTION :this function is used to store the data frame that is required to be sent to the TX_Buffer to be transmitted
 */
void UART_SendDataFrame(UARTData_t SendData,u8 *ErrorCode);


/**
 * RETURN      : u8 variable that will store the error code related to received frame
 *               return value = ZERO means that the frame received with no errors
 *               return value = ONE  means that the frame received with PARITY ERROR
 *               return value = TWO  means that the frame received with DATA OVER RUN ERROR
 *               return value = FOUR means that the frame received with FRAME ERROR
 *               return value = ERROR_BUFFER_EMPTY in case the RX_Buffer is empty and no new data received
 * PARAMETERS  : ReceivedData is pointer to variable where the date will be stored in "the user has to declare the variable and pass it's address"
 *               the variable size has to be u16 in case of 9bits frames is being used and u8 in case of any other frame sizes
 *               the user can use the UARTData_t data type to declare the variable regardless the frame size and it will be automatically resolved to the proper size
 * DESCRIPTION : This function is used to get the received data frame from the RX_Buffer and also to get the error value that corresponds to the received data frame
 */
u8 UART_ReceiveDataFrame(UARTData_t *ReceivedData);


/**
 * RETURN      : void
 * PARAMETERS  : void
 * DESCRIPTION : this function is used to stop the transmitter
 * CAUTION     : the use of this function will clear all the unsent data store in the TX_Buffer
 */
void UART_TransmitterStop(void);


/**
 * RETURN      : void
 * PARAMETERS  : void
 * DESCRIPTION : This function is used to re-enable the transmitter functionality after it's being disabled
 */
void UART_Transmitter_ReEnable(void);


/**
 * RETURN      : void
 * PARAMETERS  : void
 * DESCRIPTION : this function is used to disable the receiver
 * CAUTION     : the use of this function will clear all the unread data stored in the RX_Buffer
 */
void UART_ReceiverStop(void);


/**
 * RETURN      : void
 * PARAMETERS  : void
 * DESCRIPTION : This function is used to re-enable the receiver functionality after it's being disabled
 */
void UART_Receiver_ReEnable(void);


#endif /* UART_INTERFACE_H_ */
