/*
 *  UART_Prog.c
 *  Created on: 27/06/2021
 *  Author: Mohamed_EL_Gallad
 *
 *  Brief: This file will contain the UART module functions definition
 */
#include "STD_types.h"
#include "Mega32_reg.h"
#include "REG_utils.h"
#include "DIO_interface.h"
#include "CircularBufferInterface.h"
#include "UART_Interface.h"



#if NODE_RECEIVE_DATA==ENABLE
static CircularBuffer UART_RX_Buffer; //create a receiver buffer
  #if FRAME_SIZE != _9_BITS_FRAME
  static CircularBuffer RX_ErrorBuffer;  //create a buffer to store the error corresponding to each frame
  #endif
#endif

#if NODE_TRANSMIT_DATA==ENABLE
static CircularBuffer UART_TX_Buffer; //create a transmitter buffer
#endif


/**
 * RETURN      : void
 * PARAMETERS  : void
 * DESCRIPTION : This function is used to initiate the UART module depending on the user's configuration
 * this function will set the following
 *     1- Baud Rate
 *     2- UART operation mode
 *           2-1- Normal Asynchronous Mode
 *           2-2- Double Speed Asynchronous Mode
 *           2-3- Synchronous Master Mode
 *           2-4- Synchronous Slave Mode
 *     3- Number of stop Bits "1 stop bit , 2 stop bits"
 *     4- Parity Mode
 *     5- Frame size configuration "5 , 6 , 7, 8 ,9 Bits frame modes"
 *     6- Enable the Transmitter and the receiver and their buffers
 */
void UART_init(void)
{
  u8 TempUBRRH=0;
  u8 TempUCSRC=0;
	//baud rate configuration
   u16 UBRR_Value;
   #if   UART_OPERATION_MODE == NORMAL_ASYNCHRONOUS_UART //calculate the baud rate in case of Normal Asynchronous mode
    UBRR_Value=((u16)((CPU_FREQ/(16*BAUD_RATE))-1));

   #elif UART_OPERATION_MODE == DOUBLE_SPEED_ASYNCHRONOUS_UART // calculate the baud rate in case of double speed Asynchronous mode
    UBRR_Value=((u16)((CPU_FREQ/(8*BAUD_RATE))-1));

   #elif UART_OPERATION_MODE == MASTER_SYNCHRONOUS_UART || UART_OPERATION_MODE == SLAVE_SYNCHRONOUS_UART //calculate the baud rate in case of Synchronous modes
    UBRR_Value=((u16)((CPU_FREQ/(2*BAUD_RATE))-1));
   #endif

    UBRRL =((u8)(UBRR_Value & 0x00FF)); //store the UBRR low byte to achieve the required baud rate in UBRRL register
    ClearRegisterBit(TempUBRRH,URSEL); //select UBRRH register
    TempUBRRH =((u8)((UBRR_Value & 0x0F00)>>8)); //get the next 4bits of UBRR register that required to acheive the baud rate
    UBRRH = TempUBRRH; //store the next 4bits of UBRR in the UBRRH register


/**********************************************************************************/

	//UART mode select
    SetRegisterBit(TempUCSRC,URSEL); //select UCSRC register

   #if   UART_OPERATION_MODE == NORMAL_ASYNCHRONOUS_UART
    ClearRegisterBit(TempUCSRC,UMSEL); //UMSEL=0
    ClearRegisterBit(TempUCSRC,UCPOL); //UCPOL=0

   #elif UART_OPERATION_MODE == DOUBLE_SPEED_ASYNCHRONOUS_UART
    ClearRegisterBit(TempUCSRC,UMSEL); //UMSEL=0
    ClearRegisterBit(TempUCSRC,UCPOL); //UCPOL=0

   #elif UART_OPERATION_MODE == MASTER_SYNCHRONOUS_UART
    SetRegisterBit(TempUCSRC,UMSEL); // UMSEL=1
	SetPinDIR(1, 0, 1) ;  //define XCK_PB0 as output "Master is the clock source"
	   // define the sampling clock polarity
         #if   SYNCHRONOUS_CLK_POLARITY == TX_Rising__RX_FALLING //TX data change on rising edge , RX data change on falling edge
	       ClearRegisterBit(TempUCSRC,UCPOL); //UCPOL=0
         #elif SYNCHRONOUS_CLK_POLARITY == TX_FALLING__RX_RISING //TX data change on falling edge , RX data change on rising edge
	       SetRegisterBit(TempUCSRC,UCPOL);  //UCPOL=1
         #endif

   #elif UART_OPERATION_MODE == SLAVE_SYNCHRONOUS_UART
	SetRegisterBit(TempUCSRC,UMSEL); //UMSEL=1
	SetPinDIR(1, 0, 0); //define XCK_PB0 as input "Slave is the clock source"
	   // define the sampling clock polarity
         #if   SYNCHRONOUS_CLK_POLARITY == TX_Rising__RX_FALLING //TX data change on rising edge , RX data change on falling edge
	       ClearRegisterBit(TempUCSRC,UCPOL);  //UCPOL=0
         #elif SYNCHRONOUS_CLK_POLARITY == TX_FALLING__RX_RISING //TX data change on falling edge , RX data change on rising edge
	       SetRegisterBit(TempUCSRC,UCPOL);  //UCPOL=1
         #endif

   #endif
/**********************************************************************************/

	//stop bits mode select
   #if   STOP_BITS_MODE == SINGLE_STOP_BIT
	ClearRegisterBit(TempUCSRC,USBS); // USBS=0

   #elif STOP_BITS_MODE == TWO_STOP_BITS
	SetRegisterBit(TempUCSRC,USBS);   // USBS=1
   #endif

/**********************************************************************************/

	//parity mode select
   #if   PARITY_MODE == NO_PARITY_CHK
	ClearRegisterBit(TempUCSRC,UPM0); // UPM0=0
	ClearRegisterBit(TempUCSRC,UPM1); // UPM1=0

   #elif PARITY_MODE == ODD_PARITY
	SetRegisterBit(TempUCSRC,UPM0);   // UPM0=1
	SetRegisterBit(TempUCSRC,UPM1);   // UPM1=1

   #elif PARITY_MODE == EVEN_PARITY
	ClearRegisterBit(TempUCSRC,UPM0); // UPM0=0
	SetRegisterBit(TempUCSRC,UPM1);   // UPM1=1
   #endif


/**********************************************************************************/

    //frame size configuration
   #if   FRAME_SIZE == _5_BITS_FRAME
	ClearRegisterBit(UCSRB, UCSZ2); //UCSZ2=0
	ClearRegisterBit(TempUCSRC,UCSZ1); //UCSZ1=0
	ClearRegisterBit(TempUCSRC,UCSZ0); //UCSZ0=0

   #elif FRAME_SIZE == _6_BITS_FRAME
	ClearRegisterBit(UCSRB, UCSZ2); //UCSZ2=0
	ClearRegisterBit(TempUCSRC,UCSZ1); //UCSZ1=0
    SetRegisterBit(TempUCSRC, UCSZ0);   //UCSZ0=1


   #elif FRAME_SIZE == _8_BITS_FRAME
	ClearRegisterBit(UCSRB, UCSZ2); //UCSZ2=0
	SetRegisterBit(TempUCSRC, UCSZ1);   //UCSZ1=1
	SetRegisterBit(TempUCSRC, UCSZ0);   //UCSZ0=1

   #elif FRAME_SIZE == _9_BITS_FRAME
	SetRegisterBit(UCSRB, UCSZ2);   //UCSZ2=1
	SetRegisterBit(TempUCSRC, UCSZ1);   //UCSZ1=1
	SetRegisterBit(TempUCSRC, UCSZ0);   //UCSZ0=1
  #endif

	UCSRC=TempUCSRC;
/**********************************************************************************/

	//enable transmitter and receiver if enabled and create circular buffer for each
   #if NODE_TRANSMIT_DATA==ENABLE
	CBuffer_BufferInit(& UART_TX_Buffer); //initiate the transmitter buffer
	SetRegisterBit(UCSRB,TXEN); //Transmitter Enable
   #endif

   #if NODE_RECEIVE_DATA==ENABLE
	CBuffer_BufferInit(& UART_RX_Buffer); //initiate the receiver buffer
      #if FRAME_SIZE != _9_BITS_FRAME
	  CBuffer_BufferInit(&RX_ErrorBuffer); //initiate the receiver's error buffer in case the frame size is less than 9bits
      #endif
	SetRegisterBit(UCSRB,RXEN); //Receiver Enable
	SetRegisterBit(SREG, 7); //enable global interrupt
	SetRegisterBit(UCSRB ,RXCIE); //RX Complete Interrupt Enable
   #endif

}


/**
 * RETURN      : void
 * PARAMETERS  :SendData has a type of UARTData_t which will be resolved automatically to either u8 or u16 data type
 *               depending on the frame size
 *               the user can declare the variable with the type UARTData_t and it will be automatically resolved to the correct data size
 *               or declare the variable as a u16 variable in case of 9bits frame size is being used
 *               or declare the variable as a u8 variable in case of 6,7,8bits frame size is being used
 *             :ErrorCode is a pointer to a u8 variable that has to be defined by the user before calling this function
 *               this variable will store the error code that may be produced when the data is being pushed to the UART_TX_Buffer
 *               it will have one of the following values
 *               ERROR_BUFFER_FULL    0x15 which means that the UART_TX_Buffer is full and no room for more data to be pushed
 *               SUCCESSFUL_OPERATION 0x17 which means that the user data has been pushed successfully to the UART_TX_Buffer and to be transmitted
 * DESCRIPTION :this function is used to store the data frame that is required to be sent to the UART_TX_Buffer to be transmitted
 */
void UART_SendDataFrame(UARTData_t SendData,u8 *ErrorCode)
{
	*ErrorCode = CBuffer_PushData(&UART_TX_Buffer,SendData); //store the error code after the data push operation to be either SUCCESSFUL_OPERATION or ERROR_BUFFER_FULL
	if(*ErrorCode == SUCCESSFUL_OPERATION) //a successful push operation means that 1 byte of data already stored in the UART_TX_Buffer
	{
	    SetRegisterBit(SREG, 7); //enable global interrupt
	    SetRegisterBit(UCSRB ,UDRIE); //USART Data Register Empty Interrupt Enable , the TX-ISR will pop 1 byte of data at a time from UART_TX_Buffer to UDR reg. to be sent
	}
}


/**
 * RETURN      : u8 variable that will store the error code related to received frame
 *               return value = ZERO means that the frame received with no errors
 *               return value = ONE  means that the frame received with PARITY ERROR
 *               return value = TWO  means that the frame received with DATA OVER RUN ERROR
 *               return value = FOUR means that the frame received with FRAME ERROR
 *               return value = ERROR_BUFFER_EMPTY in case the UART_RX_Buffer is empty and no new data received
 * PARAMETERS  : ReceivedData is pointer to variable where the date will be stored in "the user has to declare the variable and pass it's address"
 *               the variable size has to be u16 in case of 9bits frames is being used and u8 in case of any other frame sizes
 *               the user can use the UARTData_t data type to declare the variable regardless the frame size and it will be automatically resolved to the proper size
 * DESCRIPTION : This function is used to get the received data frame from the UART_RX_Buffer and also to get the error value that corresponds to the received data frame
 */
u8 UART_ReceiveDataFrame(UARTData_t *ReceivedData)
{
  u8 ErrorValue=0; //error value temporary storage
  #if FRAME_SIZE == _9_BITS_FRAME //in 9bits frames each UART_RX_Buffer slot "2bytes" the RX-ISR will store the data at the first 9bits and the error value at the last 3bits
   u16 DataAndErrorFrame=0; //temporary storage to hold the data and error value
   if(!CBuffer_IsTheBufferEmpty(&UART_RX_Buffer))
    {
      CBuffer_PopData(&UART_RX_Buffer, &DataAndErrorFrame); //pop 1 slot of the UART_RX_Buffer and store it in DataAndErrorFrame variable
      *ReceivedData =DataAndErrorFrame & 0x01FF; // exclude the the data frame "9bits" from the DataAndErrorFrame variable
       ErrorValue = ((u8)((DataAndErrorFrame & 0xE000)>>13)); //exclude the error value from DataAndErrorFrame and store it in ErrorValue to be the return value
     }
   else
     {
	  ErrorValue = ERROR_BUFFER_EMPTY; // in case the UART_RX_Buffer is empty , the error value will equal ERROR_BUFFER_EMPTY
     }

  #else // in case of 8bit or less frames error value will be stored in a separate ErrorBuffer has same size as UART_RX_Buffer
   if(!CBuffer_IsTheBufferEmpty(&UART_RX_Buffer))
   {
      CBuffer_PopData(&UART_RX_Buffer, ReceivedData); //pop the data from UART_RX_Buffer and store it where ReceivedData points at
      CBuffer_PopData(&RX_ErrorBuffer,&ErrorValue);//pop the the error value corresponds to the relative data frame and store it in ErrorValue
   }
   else
   {
   	  ErrorValue = ERROR_BUFFER_EMPTY; // in case the UART_RX_Buffer is empty , the error value will equal ERROR_BUFFER_EMPTY
   }
  #endif

  return ErrorValue; //return the error value

}


/**
 * RETURN      : void
 * PARAMETERS  : void
 * DESCRIPTION : this function is used to stop the transmitter
 * CAUTION     : the use of this function will clear all the unsent data store in the UART_TX_Buffer
 */
void UART_TransmitterStop(void)
{
#if NODE_TRANSMIT_DATA==ENABLE
    ClearRegisterBit(UCSRB, TXEN);  //Transmitter Disable
    ClearRegisterBit(UCSRB, UDRIE); //Disable USART Data Register Empty Interrupt
    CBuffer_BufferReset(& UART_TX_Buffer); //Clear the TX buffer
#endif
}


/**
 * RETURN      : void
 * PARAMETERS  : void
 * DESCRIPTION : This function is used to re-enable the transmitter functionality after it's being disabled
 */
void UART_Transmitter_ReEnable(void)
{
#if NODE_TRANSMIT_DATA==ENABLE
	SetRegisterBit(SREG, 7); //enable global interrupt
	SetRegisterBit(UCSRB ,UDRIE); //Enable USART Data Register Empty Interrupt
	SetRegisterBit(UCSRB,TXEN);   //Enable Transmitter
#endif
}


/**
 * RETURN      : void
 * PARAMETERS  : void
 * DESCRIPTION : this function is used to disable the receiver
 * CAUTION     : the use of this function will clear all the unread data stored in the UART_RX_Buffer
 */
void UART_ReceiverStop(void)
{
#if NODE_RECEIVE_DATA==ENABLE
	ClearRegisterBit(UCSRB,RXCIE);//Disable RX Complete Interrupt
	ClearRegisterBit(UCSRB,RXEN); //Disable Receiver
	CBuffer_BufferReset(& UART_RX_Buffer); //Clear the RX buffer
    #if FRAME_SIZE != _9_BITS_FRAME
	CBuffer_BufferReset(&RX_ErrorBuffer); //Clear the RX error buffer
    #endif
#endif
}


/**
 * RETURN      : void
 * PARAMETERS  : void
 * DESCRIPTION : This function is used to re-enable the receiver functionality after it's being disabled
 */
void UART_Receiver_ReEnable(void)
{
#if NODE_RECEIVE_DATA==ENABLE
	SetRegisterBit(SREG, 7); //enable global interrupt
	SetRegisterBit(UCSRB ,RXCIE); //Enable RX Complete Interrupt
	SetRegisterBit(UCSRB,RXEN);   //Enable Receiver
#endif
}



/**
 *
 */
#if NODE_TRANSMIT_DATA==ENABLE
void __vector_14 (void) __attribute__ ((signal,used));
void __vector_14 (void)
{
	UARTData_t TX_Data;

    if(!CBuffer_IsTheBufferEmpty(&UART_TX_Buffer))
    {
    	CBuffer_PopData(&UART_TX_Buffer, &TX_Data);
       #if(FRAME_SIZE == _9_BITS_FRAME)
    	{
    	 if (TX_Data & 0x100 ) //check if the ninth bit in the TX_data=1 if so set TXB8 to 1
    	  {
    	  SetRegisterBit(UCSRB,TXB8); //set the TXB8 to 1
    	  }
    	 else
    	  {
    	  ClearRegisterBit(UCSRB,TXB8); //clear the ninth bit
    	  }
    	}
       #endif
    	UDR=TX_Data; //load data from UART_TX_Buffer to UDR register to initiate data sending
    }
    else
    {
    	ClearRegisterBit(UCSRB, UDRIE); //Disable USART Data Register Empty Interrupt
    }
}
#endif


#if NODE_RECEIVE_DATA==ENABLE
void __vector_13 (void) __attribute__ ((signal,used));
void __vector_13 (void)
{
	 u8 FrameErrorLog=0; //temporary storage for the error value
	 UARTData_t RX_Data=0; //data frame temporary storage,RX_Date size will depend on the frame size "2bytes in case of 9bits frame and 1byte for the rest"

     //get the PE,DOR and FE error values from UCSRA and store them in the first 3 bits of the FrameErrorLog variable
	 FrameErrorLog = ((GetRegisterBit(UCSRA,PE)<<PE) | (GetRegisterBit(UCSRA,DOR)<<DOR) | (GetRegisterBit(UCSRA,FE)<<FE)) >> 2;
  #if FRAME_SIZE == _9_BITS_FRAME
	RX_Data =GetRegisterBit(UCSRB,TXB8)<<8; //in case of a 9bits frame store the ninth bit in it's relative location in RX_Data variable
  #endif
	RX_Data = UDR; //store the data from UDR register to the RX_Data variable

  #if FRAME_SIZE ==_9_BITS_FRAME
	RX_Data |=FrameErrorLog <<13; //in case of a 9bits frame the error values of BE,DOR and FE will be stored in the last 3bits of RX_Data
  #else //in case of a NON 9bits frame the error values will be stored in a separated buffer it's size equals to UART_RX_Buffer
    CBuffer_PushData(&RX_ErrorBuffer, FrameErrorLog); //store the error value of the received data frame to the RX_ErrorBuffer
  #endif

  if(!CBuffer_IsTheBufferFull(&UART_RX_Buffer))
  {
   CBuffer_PushData(&UART_RX_Buffer, RX_Data); //store the data in the UART_RX_Buffer
  }
  else
  {
	  //the user has to read the data from the UART_RX_Buffer frequently to avoid filling the UART_RX_Buffer to it's full capacity
  }

}
#endif
