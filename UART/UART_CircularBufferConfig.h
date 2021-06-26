/** THIS CONFIGURATION FILE WILL BE CONTROLLED BY UART_COFIG.H NO NEED TO CHANGE ANYTHING IN THIS FILE **/
/*
Date: 12/06/2021
Author: Mohamed EL-Gallad
Description : This header file will contain the main Circular Buffer configuration "Buffer size and buffer data type"
*/
#ifndef _CIRCULARBUFFERCONFIG_H_
#define _CIRCULARBUFFERCONFIG_H_



/*
  The BUFFER_SIZE MACRO will define the buffer size 
  all the buffers that being created will have the same size defined by this MACRO  
*/
#define BUFFER_SIZE   TX_RX_BUFFER_SIZE

/*
  BuffData_t MACRO will define the data type of the circular buffer elements "depending on the frame size"
  if frame size is < 9bits then BuffData_t will be defined as u8  variable
  if frame size is = 9bits then BuffData_t will be defined as u16 variable
*/
#if FRAME_SIZE == _9_BITS_FRAME
#define BuffData_t   u16
#else
#define BuffData_t   u8
#endif



#endif
