/*
Date: 12/06/2021
Author: Mohamed EL-Gallad
Description : This header file will contain interface functions for the CircularBuffer library
*/

#ifndef _CIRCULARBUFFER_H_
#define _CIRCULARBUFFER_H_


/*******************************************************************************************************
CircularBuffer : is a struct that is being used to create a CircularBuffer instance .
NOTE : all the created Buffers will have the size equals to BUFFER_SIZE MACROS
*******************************************************************************************************/
typedef struct {
    volatile BuffData_t data[BUFFER_SIZE];
    volatile BuffData_t *headptr;
    volatile BuffData_t *tailptr;
    u8 AvailablePosition;
}CircularBuffer;


/*******************************************************************************************************
RETURN     : VOID
PARAMETERS : BufferPtr is A pointer to the Circular Buffer instance
DESCRIPTION: This function is used to initiate the circular buffer and it must be called before any opertion
performed on a curcular buffer object
*******************************************************************************************************/
void CBuffer_BufferInit(CircularBuffer *BufferPtr);


/*******************************************************************************************************
RETURN     : unsigned char value represents the error codes ERROR_BUFFER_FULL if the buffer is full or
SUCCESSFUL_OPERATION if the CBuffer_PushData has been executed successfully.                                                    
PARAMETERS :Buffer ,is A pointer to the Circular Buffer instance
           :data , is copy of the data that is required to be stored in the buffer
DESCRIPTION:this function is used to push data to the circular buffer if there is an available space in the buffer
*******************************************************************************************************/
u8 CBuffer_PushData(CircularBuffer* Buffer ,BuffData_t data);


/*******************************************************************************************************
RETURN     : unsigned char value represents the error codes ERROR_BUFFER_EMPTY if the buffer is empty or
SUCCESSFUL_OPERATION if the CBuffer_PopData has been executed successfully.                                                    
PARAMETERS :Buffer ,is A pointer to the Circular Buffer instance
           :DataPtr is a pointer to BuffData_t varialbe that will contain the popped value out of the buffer 
DESCRIPTION:this function is used to pop data out of the buffer and store it in a variable that DataPtr pointer points at.
*******************************************************************************************************/
u8 CBuffer_PopData(CircularBuffer* Buffer, BuffData_t *DataPtr);


/*******************************************************************************************************
RETURN     : VOID
PARAMETERS : BufferAddress is A pointer to the Circular Buffer instance
DESCRIPTION: This function is used to clear the circular buffer instance
after the execution headptr and tailptr will point at the first element in the buffer and AvailablePosition
will equal BUFFER_SIZE
*******************************************************************************************************/
void CBuffer_BufferReset(CircularBuffer *BufferAddress);


/*******************************************************************************************************
RETURN     : unsigned char value either TRUE if the buffer is empty or FALSE if the buffer isn't empty
PARAMETERS : Buffer is A pointer to the Circular Buffer instance
DESCRIPTION: This function is used to check if either the buffer is empty or not 
*******************************************************************************************************/
u8 CBuffer_IsTheBufferEmpty(CircularBuffer *Buffer);


/*******************************************************************************************************
RETURN     : unsigned char value either TRUE if the buffer is full or FALSE if the buffer isn't full
PARAMETERS : Buffer is A pointer to the Circular Buffer instance
DESCRIPTION: This function is used to check if either the buffer is full or not
*******************************************************************************************************/
u8 CBuffer_IsTheBufferFull(CircularBuffer *Buffer);

#endif
