/*
Date: 12/06/2021
Author: Mohamed EL-Gallad
Description : This file will contain the errors MACROS .
*/
#ifndef _CIRCULARBUFFERPRIVATE_H_
#define _CIRCULARBUFFERPRIVATE_H_


//if the buffer is full the CBuffer_PushData function should return the following MACROS 
#define ERROR_BUFFER_FULL     (u8)0x15

//if the buffer is empty the CBuffer_PopData function should return the following MACROS 
#define ERROR_BUFFER_EMPTY    (u8)0x16

//when a successful operation is performed by either CBuffer_PushData or CBuffer_PopData they should return the following MACROS
#define SUCCESSFUL_OPERATION  (u8)0x17

//the following MACROS is used as boolean return values for CBuffer_IsTheBufferFull and CBuffer_IsTheBufferEmpty



#endif
