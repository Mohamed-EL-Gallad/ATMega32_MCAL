/*
 * UART_private.h
 *
 *  Created on: 30/6/2021
 *  Author: Mohamed EL-Gallad
 *
 */

#ifndef UART_PRIVATE_H_
#define UART_PRIVATE_H_



#define FRAME_ERROR     ((u8)0x15)
#define PARITY_ERROR    ((u8)0x16)
#define DATA_OVER_RUN   ((u8)0x17)

//UBRRH bits
#define URSEL ((u8)7)

//UCSRC bits0
#define UMSEL ((u8)6)
#define UPM1  ((u8)5)
#define UPM0  ((u8)4)
#define USBS  ((u8)3)
#define UCSZ1 ((u8)2)
#define UCSZ0 ((u8)1)
#define UCPOL ((u8)0)

//UCSRA bits
#define RXC  ((u8)7)
#define TXC  ((u8)6)
#define UDRE ((u8)5)
#define FE   ((u8)4)
#define DOR  ((u8)3)
#define PE   ((u8)2)
#define U2X  ((u8)1)
#define MPCM ((u8)0)

//UCSRB bits
#define RXCIE  ((u8)7)
#define TXCIE  ((u8)6)
#define UDRIE  ((u8)5)
#define RXEN   ((u8)4)
#define TXEN   ((u8)3)
#define UCSZ2  ((u8)2)
#define RXB8   ((u8)1)
#define TXB8   ((u8)0)

#endif /* UART_PRIVATE_H_ */
