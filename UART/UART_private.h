/*
 * UART_private.h
 *
 *  Created on: 30/6/2021
 *  Author: Mohamed EL-Gallad
 *
 */

#ifndef UART_PRIVATE_H_
#define UART_PRIVATE_H_



#define FRAME_ERROR     ((u8)0x04)
#define PARITY_ERROR    ((u8)0x02)
#define DATA_OVER_RUN   ((u8)0x03)


//UBRRH bits
#define URSEL (7)

//UCSRC bits
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



/*
  UARTData_t MACRO will define the data type of the data to be sent and received  "depending on the frame size"
  if frame size is < 9bits then BuffData_t will be defined as u8  variable
  if frame size is = 9bits then BuffData_t will be defined as u16 variable
*/
#if FRAME_SIZE == _9_BITS_FRAME
#define UARTData_t   u16
#else
#define UARTData_t   u8
#endif

#endif /* UART_PRIVATE_H_ */
