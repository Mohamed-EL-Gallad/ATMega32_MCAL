/*
Date: 12/06/2021
Author: Mohamed EL-Gallad
Description : This file will contain the functions implementation for the circular buffer lib.
*/
#include "STD_types.h"
#include "CircularBufferInterface.h"



/*******************************************************************************************************
RETURN     : VOID
PARAMETERS : BufferPtr is A pointer to the Circular Buffer instance
DESCRIPTION: This function is used to initiate the circular buffer and it must be called before any opertion
performed on a curcular buffer object
*******************************************************************************************************/
void CBuffer_BufferInit(CircularBuffer *BufferPtr)
{
	//set the headptr pointer to the first ever location of the buffer 
    BufferPtr->headptr=BufferPtr->data;
	//set the tailptr pointer to the first ever location of the buffer
    BufferPtr->tailptr=BufferPtr->data;
	//make the AvailablePosition value equals the BUFFER_SIZE MACRO 
    BufferPtr->AvailablePosition =BUFFER_SIZE;
}


/*******************************************************************************************************
RETURN     : unsigned char value represents the error codes ERROR_BUFFER_FULL if the buffer is full or
SUCCESSFUL_OPERATION if the CBuffer_PushData has been executed successfully.                                                    
PARAMETERS :Buffer ,is A pointer to the Circular Buffer instance
           :data , is copy of the data that is required to be stored in the buffer
DESCRIPTION:this function is used to push data to the circular buffer if there is an available space in the buffer
*******************************************************************************************************/
u8 CBuffer_PushData(CircularBuffer* Buffer ,BuffData_t data)
{
	//PushDataError will hold the error value either ERROR_BUFFER_FULL or SUCCESSFUL_OPERATION
	u8 PushDataError;
	//check if there is an available space in the buffer or not
    if(Buffer->AvailablePosition>0)
    {
	//store the data at the available space 
	*(Buffer->headptr)=data;
	//set the error value to SUCCESSFUL_OPERATION as the data has been stored successfully 
	PushDataError=SUCCESSFUL_OPERATION;
	//decrease the available spaces in the buffer by 1
    Buffer->AvailablePosition--;
	//check if the headptr has the value of the last available space in the buffer if so set the headptr to point at the firt element of the buffer
	    if(Buffer->headptr == &Buffer->data[BUFFER_SIZE-1])
	     {
			 //set the headptr value to the first element of the buffer
		    Buffer->headptr =Buffer->data;
	     }
	     else
		 {
			 //if it doesn't contain the address of the last element of the buffer, advance the headptr by one step 
			 Buffer->headptr++;
		 }
      }
     else 
	{
		//if there is no available space in the buffer set PushDataError value to ERROR_BUFFER_FULL
		PushDataError=ERROR_BUFFER_FULL;
	}
	//return the error value
    return PushDataError;
}


/*******************************************************************************************************
RETURN     : unsigned char value represents the error codes ERROR_BUFFER_EMPTY if the buffer is empty or
SUCCESSFUL_OPERATION if the CBuffer_PopData has been executed successfully.                                                    
PARAMETERS :Buffer ,is A pointer to the Circular Buffer instance
           :DataPtr is a pointer to BuffData_t varialbe that will contain the popped value out of the buffer 
DESCRIPTION:this function is used to pop data out of the buffer and store it in a variable that DataPtr pointer points at.
*******************************************************************************************************/
u8 CBuffer_PopData(CircularBuffer* Buffer,  BuffData_t *DataPtr)
{
	//PopDataError will hold the error value either ERROR_BUFFER_EMPTY or SUCCESSFUL_OPERATION
	u8 PopDataError;
	//check if the buffer is empty or not
	if(Buffer->AvailablePosition < BUFFER_SIZE)
	{
	//pop the data where the tailptr points at
    *DataPtr=*(Buffer->tailptr);
	//set the error value to SUCCESSFUL_OPERATION as the data has been popped successfully 
	PopDataError =SUCCESSFUL_OPERATION;
	//increase the available positions in the circular buffer by 1
    Buffer->AvailablePosition++;
	//check if the tailptr has the value of the last available space in the buffer if so set the tailptr to point at the firt element of the buffer
	 if(Buffer->tailptr == &Buffer->data[BUFFER_SIZE-1])
	     {
			 //set the tailptr value to the first element of the buffer
		    Buffer->tailptr =Buffer->data;
	     }
	     else
		 {
			 //if it doesn't contain the address of the last element of the buffer, advance the tailptr by one step 
			 Buffer->tailptr++;
		 }
	}
	else
	{
	//if the buffer is empty set PopDataError value to ERROR_BUFFER_EMPTY
    PopDataError=ERROR_BUFFER_EMPTY;
	}
	//return the error value
	return PopDataError;
}


/*******************************************************************************************************
RETURN     : VOID
PARAMETERS : BufferAddress is A pointer to the Circular Buffer instance
DESCRIPTION: This function is used to clear the circular buffer instance
after the execution headptr and tailptr will point at the first element in the buffer and AvailablePosition
will equal BUFFER_SIZE
*******************************************************************************************************/
void CBuffer_BufferReset(CircularBuffer *BufferAddress)
{
	//calling CBuffer_BufferInit will reset the buffer to the initial values
	CBuffer_BufferInit(BufferAddress);
}


/*******************************************************************************************************
RETURN     : unsigned char value either TRUE if the buffer is empty or FALSE if the buffer isn't empty
PARAMETERS : Buffer is A pointer to the Circular Buffer instance
DESCRIPTION: This function is used to check if either the buffer is empty or not 
*******************************************************************************************************/
u8 CBuffer_IsTheBufferEmpty(CircularBuffer *Buffer)
{
	//a flag that will have a value of either TRUE or FALSE
	u8 EmptyFlag;
	//check if the AvailablePosition variable = BUFFER_SIZE "which means that the buffer is empty"
	if(Buffer->AvailablePosition==BUFFER_SIZE)
	{
		//set the flag value to TRUE as the buffer is empty
		EmptyFlag=TRUE;
	}
	else
	{
		//set the flag value to FALSE as the buffer isn't emnpty
		EmptyFlag=FALSE;
	}
	//return the flag value
	return EmptyFlag;
}


/*******************************************************************************************************
RETURN     : unsigned char value either TRUE if the buffer is full or FALSE if the buffer isn't full
PARAMETERS : Buffer is A pointer to the Circular Buffer instance
DESCRIPTION: This function is used to check if either the buffer is full or not
*******************************************************************************************************/
u8 CBuffer_IsTheBufferFull(CircularBuffer *Buffer)
{
	//a flag that will have a value of either TRUE or FALSE
	u8 FullFlag;
	//check if the AvailablePosition variable =0 "which means that the buffer is full"
	if(Buffer->AvailablePosition==0)
	{
		//set the flag value to TRUE as the buffer is full
		FullFlag=TRUE;
	}
	else
	{
		//set the flag value to TRUE as the buffer isn't full
		FullFlag=FALSE;
	}
	//return the flag value
	return FullFlag;
}
