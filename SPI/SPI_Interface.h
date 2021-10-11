/*
 * SPI_Interface.h
 *
 *  Created on: 23/08/2021
 *  Author: Mohamed_EL_Gallad
 *  Description : This file will contain the SPI function's prototypes
 *  The user should configure the SPI settings and configurations in the SPI_Config.h file
 *
 *  1-To initiate the SPI module the user MUST call the SPI_Init() function first .
 *
 *  2-if the node is configured as a Master the data send and receive process will be based on polling algorithm
 *
 *  3-to send and receive data in case of a Master mode the user should use this function SPI_MasterSendAndReceiveByte().
 *
 *  4-if the node configured as a slave the data send a receive process will be based on the transmit complete interrupt .
 *
 *  5-to send and receive data in case of a Slave mode the user should use this function SPI_SlaveSendAndReceiveByte()
 *  by using this function the user will have control to either receive or drop the shifted in byte from the master that corresponds
 *  to the shifted out byte from the slave for ex. if the shifted in byte is known to be a dummy packet then there is no need to save it.
 *
 *  6- in case of slave mode the received data will be automatically save in the circular buffer and to read the received data the
 *  user will have to call this function SPI_SlaveReadByteFromRXBuffer().
 */

#ifndef SPI_INTERFACE_H_
#define SPI_INTERFACE_H_

#include "SPI_Config.h"
#include "SPI_Private.h"


/**
 *  RETURN     : VOID
 *  PARAMETERS : VOID
 *  DESCRIPTION: This function is used to initiate the SPI module depending on the User's configuration
 *  the SPI_Init() function will do the following :
 *  	1- Enable the SPI functionality
 *  	2- Set the transmission data order either LSB or MSB to be shifted first
 *  	3- Set the node to operate in either MASTER or SLAVE modes
 */
void SPI_Init(void);


/**
 * RETURN      : u8 variable that will contain the received data from the slave
 * PARAMETERS  : u8 variable "SendData" which will contain a copy of the data to be sent from the master to the slave
 * DESCRIPTION : This function is used ONLY when the node is configured as master , the function will send and receive one byte .
 * in case the Master needs to push data from a slave a dummy packet has to sent from the master
 * this MACRO "SPI_DUMMY_PACKET" represents a dummy packet that can be used for this purpose
 */
u8 SPI_MasterSendAndReceiveByte( u8 SendData);


/**
 * RETURN      : u8 variable that will contain the SPI_TX_Buffer status it will have one of these values
 * 				 FAILED_OPERATION     : will be the return value if the function was called while the node wasn't configured as slave
 * 				 SUCCESSFUL_OPERATION : will be the return value if the data pushed successfully to the SPI_TX_Buffer
 * 				 ERROR_BUFFER_FULL	 : will be the return value if the SPI_TX_Buffer is full
 * PARAMETERS  : DropReceivedPacket is a u8 variable that is used as flag to either drop or receive the shifted in packet that corresponds to the sifted out packet
 * 				 it must have one of two predefined Macros
 * 				 RX_DROP_PACKET when passed as a parameter the received packet that corresponds to the shifted out packet will be dropped
 * 				 RX_RECEIVE_PACKET when passed as a parameter the received packed that corresponds to the shifted out packet will be saved into the SPI_RX_Buffer
 * DESCRIPTION : This function is used if the node is configured as a slave , when called it will store the passed data in either the SPDR directly if it's empty
 * 				 and ready to receive new data or store the data in the SPI_TX_Buffer if the previous byte hasn't been shifted out yet
 * 				 this function also gives the user the control over the received data from the master to either drop the received packet or store it in the SPI_RX_Buffer
 * 				 ex. if the shifted packet from the master is known to be a dummy packet then there is no need to save the packet
 * 				 if the user decided to save the received packet it will be automatically stored in the SPI_RX_Buffer
 * 				 to be read later by calling the SPI_SlaveReadByteFromRXBuffer function
 */
u8 SPI_SlaveSendAndReceiveByte( const u8 *SendData , u8 DropReceivedPacket);


/**
 * RETURN      : u8 variable that will be contain the error code , it will be either
 *  			 ERROR_BUFFER_EMPTY in case the SPI_RX_Buffer is empty
 *  			 or SUCCESSFUL_OPERATION if a data byte popped successfully from the SPI_RX_Buffer
 * PARAMETER   : ReceiveData is a pointer to predefined u8 variable by the user where the received data will be stored in
 * DESCRIPTION : This function is used to read the received data from the Master that has been stored in the SPI_RX_Buffer
 * 				 whenever the master sends a data packet it will be stored automatically in the SPI_RX_Buffer
 * 				 to be read later using this function
 */
u8 SPI_SlaveReadByteFromRXBuffer(u8 *ReceiveData);


#endif /* SPI_INTERFACE_H_ */
