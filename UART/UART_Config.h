/*
 * UART_Config.h
 *
 * Created on: 08/05/2021
 * Author: Mohamed_EL_Gallad
 */

#ifndef UART_CONFIG_H_
#define UART_CONFIG_H_



#define CPU_FREQ  ((u32)12000000)  //user has to define the CPU operating frequency
#define BAUD_RATE ((u32)9600) //baud rate value

#define       NORMAL_ASYNCHRONOUS_UART   0
#define DOUBLE_SPEED_ASYNCHRONOUS_UART   1
#define        MASTER_SYNCHRONOUS_UART   2
#define         SLAVE_SYNCHRONOUS_UART   3

#define OPERATION_MODE   NORMAL_ASYNCHRONOUS_UART



#define TX_Rising__RX_FALLING  0
#define TX_FALLING__RX_RISING  1

#define SYNCHRONOUS_CLK_POLARITY  TX_Rising__RX_FALLING


#define _5_BITS_FRAME  0
#define _6_BITS_FRAME  1
#define _7_BITS_FRAME  2
#define _8_BITS_FRAME  3
#define _9_BITS_FRAME  4

#define FRAME_SIZE   _8_BITS_FRAME



#define TX_RX_BUFFER_SIZE  ((u8)10)
#define NODE_TRANSMIT_DATA ENABLE
#define NODE_RECEIVE_DATA  DISABLE


#define SINGLE_STOP_BIT 0
#define TWO_STOP_BITS 1

#define STOP_BITS_MODE  SINGLE_STOP_BIT


#define NO_PARITY_CHK   0
#define    ODD_PARITY   2
#define   EVEN_PARITY   3

#define PARITY_MODE   NO_PARITY_CHK


#define MPCM_MASTER_NODE
#define  MPCM_SLAVE_NODE

#define MPCM_NODE_MODE  MPCM_MASTER_NODE


#endif /* UART_CONFIG_H_ */
