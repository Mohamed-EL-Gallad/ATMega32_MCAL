/*
 * SPI_Private.h
 *
 *  Created on: 23/08/2021
 *  Author: Mohamed_EL_Gallad
 *  Brief: This file will contain Macros for the SPI registers bits
 *  DON'T CHANGE ANYTHING IN THIS FILE
 */

#ifndef SPI_PRIVATE_H_
#define SPI_PRIVATE_H_

//SPSR Register bits
#define SPIF    ((u8)7)
#define WCOL    ((u8)6)
#define SPI2X   ((u8)0)

//SPCR Register bits
#define SPIE    ((u8)7)
#define SPE     ((u8)6)
#define DORD    ((u8)5)
#define MSTR    ((u8)4)
#define CPOL    ((u8)3)
#define CPHA    ((u8)2)
#define SPR1    ((u8)1)
#define SPR0    ((u8)0)

//this macro defines a flag to be used as a parameter in SPI_SlaveSendAndReceiveByte() in case the user willing to drop the received packet
#define RX_DROP_PACKET     ((u8)0x00)

//this macro defines a flag to be used as a parameter in SPI_SlaveSendAndReceiveByte() in case the user willing to save the received packet
#define RX_RECEIVE_PACKET  ((u8)0x01)



//this macro should be used as return value if the function execution failed
#ifndef FAILED_OPERATION
#define FAILED_OPERATION   ((u8)0x00)
#endif


#endif /* SPI_PRIVATE_H_ */
