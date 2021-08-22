/*
 * UART_Config.h
 * Created on: 08/05/2021
 * Author: Mohamed_EL_Gallad
 *
 * Brief : This file contain the configuration required to control the behavior of the UART Module this configurations are:
 *  1- The baud rate value
 *     By setting the value of the BAUD_RATE Macros the user can set the desired baud rate , but before that
 *     the user MUST define the CPU operating frequency this can be done by setting the value of CPU_FREQ Macros
 *  2- The mode of operation
 *     the user can select between 4 modes represented by the following Macros:
 *        A- NORMAL_ASYNCHRONOUS_UART
 *        B- DOUBLE_SPEED_ASYNCHRONOUS_UART
 *        C- MASTER_SYNCHRONOUS_UART
 *        D- SLAVE_SYNCHRONOUS_UART
 *      the user can select the desired operation mode by setting the value of the OPERATION_MODE macro to one of the previously mentioned Macros
 *  3- Clock Polarity selection in case of Synchronous modes of operation
 *     the user can select on which edge the data sampling will occur
 *     by setting the value of the SYNCHRONOUS_CLK_POLARITY Macros to one of the following Macros
 *        A- TX_Rising__RX_FALLING "TX data will be sampled on the rising edge and RX data will be sampled on falling edge"
 *        B- TX_FALLING__RX_RISING "TX data will be sampled on the falling edge and RX data will be sampled on rising edge"
 *  4- Frame size selection
 *     the user can select between 5 different frame sizes by setting the value of FRAME_SIZE Macros to one of the following Macros
 *        A- _5_BITS_FRAME "frame size will be 5bits"
 *        B- _6_BITS_FRAME "frame size will be 6bits"
 *        C- _7_BITS_FRAME "frame size will be 7bits"
 *        D- _8_BITS_FRAME "frame size will be 8bits"
 *        E- _9_BITS_FRAME "frame size will be 9bits"
 *  5- TX & RX circular buffer size selection
 *     The user can set the available location for both TX_Buffer and RX_Buffer by setting the value of TX_RX_BUFFER_SIZE Macros
 *     CAUTION: if the frame size is between 5bits to 8bits another circular buffer will be automatically created which is RX_ErrorBuffer
 *     and will have the same No of available locations of the RX_Buffer and will contain the error values for each of the received frames so in this case while
 *     setting the TX_RX_BUFFER_SIZE Macros the user MUST be aware that 3buffers will be created with available locations =TX_RX_BUFFER_SIZE
 *     in case of 5bits to 8bits frames the size of each slot of the TX_Buffer , RX_Buffer and RX_ErrorBuffer will be 2bytes
 *     in case of 9bits frames the size of each slot of the TX_Buffer and RX_Buffer will be 1byte
 *  6- Node functionality selection to either receive or send data or both
 *     This can be achieved by setting the value of NODE_TRANSMIT_DATA and NODE_RECEIVE_DATA Macros to either ENABLE or DISABLE
 *  7- The selection of number of stop bits
 *     the user can select the number of stop bits that trail the data frame by setting the value of STOP_BITS_MODE to either
 *     SINGLE_STOP_BIT Macros for a single stop bit
 *     or TWO_STOP_BITS Macros for two stop bits
 *  8- The selection of a parity check , as the user can select to between odd parity , even parity or no parity by setting the value of
 *     PARITY_MODE Macros to one of the following
 *     NO_PARITY_CHK "to parity check is used"
 *     ODD_PARITY    "odd parity check is used"
 *     EVEN_PARITY   "even parity check is used"
 */

#ifndef UART_CONFIG_H_
#define UART_CONFIG_H_

/*-------------------------------------------------------------------------------------------------------------
 *                                     CPU FREQUENCY
 -------------------------------------------------------------------------------------------------------------*/
#ifndef CPU_FREQ
#define CPU_FREQ  ((u32)12000000)  //user has to define the CPU operating frequency
#endif
/**************************************************************************************************************/


/*--------------------------------------------------------------------------------------------------------------
 *                                     BAUD RATE VALUE
 -------------------------------------------------------------------------------------------------------------*/
#define BAUD_RATE ((u32)9600) //baud rate value
/**************************************************************************************************************/


/*--------------------------------------------------------------------------------------------------------------
 *                                  UART MODE OF OPERATION
 -------------------------------------------------------------------------------------------------------------*/
#define       NORMAL_ASYNCHRONOUS_UART   0
#define DOUBLE_SPEED_ASYNCHRONOUS_UART   1
#define        MASTER_SYNCHRONOUS_UART   2
#define         SLAVE_SYNCHRONOUS_UART   3

#define OPERATION_MODE   NORMAL_ASYNCHRONOUS_UART
/**************************************************************************************************************/


/*--------------------------------------------------------------------------------------------------------------
 *                               SYNCHRONOUS MODES CLOCK POLARITY
 -------------------------------------------------------------------------------------------------------------*/
#define TX_Rising__RX_FALLING  0
#define TX_FALLING__RX_RISING  1

#define SYNCHRONOUS_CLK_POLARITY  TX_Rising__RX_FALLING
/**************************************************************************************************************/


/*--------------------------------------------------------------------------------------------------------------
 *                                   FRAME SIZE SETTING
 -------------------------------------------------------------------------------------------------------------*/
#define _5_BITS_FRAME  0
#define _6_BITS_FRAME  1
#define _7_BITS_FRAME  2
#define _8_BITS_FRAME  3
#define _9_BITS_FRAME  4

#define FRAME_SIZE   _8_BITS_FRAME
/**************************************************************************************************************/


/*-------------------------------------------------------------------------------------------------------------
 *                               TX & RX FUNCTIONALITY CONTROL
 *                                TX & RX BUFFER SIZE SETTING
 -------------------------------------------------------------------------------------------------------------*/
#define TX_RX_BUFFER_SIZE  ((u8)10) //TX & RX buffer size control "Number of available locations in buffer"
#define NODE_TRANSMIT_DATA ENABLE   //change to DISABLE Macros to disable the transmitter functionality
#define NODE_RECEIVE_DATA  ENABLE   //change to DISABLE Macros to disable the receiver functionality
/**************************************************************************************************************/


/*--------------------------------------------------------------------------------------------------------------
 *                               NUMBER OF STOP BITS SETTING
 -------------------------------------------------------------------------------------------------------------*/
#define SINGLE_STOP_BIT 0
#define TWO_STOP_BITS 1

#define STOP_BITS_MODE  SINGLE_STOP_BIT
/**************************************************************************************************************/


/*--------------------------------------------------------------------------------------------------------------
 *                                PARITY MODE SELECTION
 -------------------------------------------------------------------------------------------------------------*/
#define NO_PARITY_CHK   0
#define    ODD_PARITY   2
#define   EVEN_PARITY   3

#define PARITY_MODE   NO_PARITY_CHK
/**************************************************************************************************************/




#endif /* UART_CONFIG_H_ */
