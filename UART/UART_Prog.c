/*
 * UART_Prog.c
 *
 *  Created on: ٢٧‏/٠٦‏/٢٠٢١
 *      Author: spider
 */
#include "STD_types.h"
#include "Mega32_reg.h"
#include "REG_utils.h"
#include "UART_Config.h"
#include "UART_private.h"
#include "UART_Interface.h"

#include "UART_CircularBufferConfig.h"
#include "UART_CircularBufferPrivate.h"
#include "UART_CircularBuffer.h"

#if NODE_RECEIVE_DATA==ENABLE
 CircularBuffer RX_Buffer; //create a receiver buffer
#endif

#if NODE_TRANSMIT_DATA==ENABLE
 CircularBuffer TX_Buffer; //create a transmitter buffer
#endif



void UART_init(void)
{
  u8 TempUBRRH=0;
  u8 TempUCSRC=0;
	//baud rate configuration
   u16 UBRR_Value;
   #if   OPERATION_MODE == NORMAL_ASYNCHRONOUS_UART
    UBRR_Value=((u16)((CPU_FREQ/(16*BAUD_RATE))-1));

   #elif OPERATION_MODE == DOUBLE_SPEED_ASYNCHRONOUS_UART
    UBRR_Value=((u16)((CPU_FREQ/(8*BAUD_RATE))-1));

   #elif OPERATION_MODE == MASTER_SYNCHRONOUS_UART
    UBRR_Value=((u16)((CPU_FREQ/(2*BAUD_RATE))-1));
   #endif

    UBRRL =((u8)(UBRR_Value & 0x00FF)); //store the UBRR low byte to achieve the required baud rate in UBRRL register
    ClearRegisterBit(TempUBRRH,URSEL); //select UBRRH register
    TempUBRRH =((u8)((UBRR_Value & 0x0F00)>>8)); //store the next 4bits of UBRR in the UBRRH register
    UBRRH = TempUBRRH;

/**********************************************************************************/

	//UART mode select
   #if   OPERATION_MODE == NORMAL_ASYNCHRONOUS_UART
    SetRegisterBit(TempUCSRC,URSEL); //select UCSRC register
    ClearRegisterBit(TempUCSRC,UMSEL); //UMSEL=0
    ClearRegisterBit(TempUCSRC,UCPOL); //UCPOL=0

   #elif OPERATION_MODE == DOUBLE_SPEED_ASYNCHRONOUS_UART
    ClearRegisterBit(TempUCSRC,UMSEL); //UMSEL=0
    ClearRegisterBit(TempUCSRC,UCPOL); //UCPOL=0

   #elif OPERATION_MODE == MASTER_SYNCHRONOUS_UART
    SetRegisterBit(TempUCSRC,UMSEL); // UMSEL=1
	SetPinDIR(1, 0, 1) ;  //define XCK_PB0 as output "Master is the clock source"
	   // define the sampling clock polarity
         #if   SYNCHRONOUS_CLK_POLARITY == TX_Rising__RX_FALLING //TX data change on rising edge , RX data change on falling edge
	       ClearRegisterBit(TempUCSRC,UCPOL); //UCPOL=0
         #elif SYNCHRONOUS_CLK_POLARITY == TX_FALLING__RX_RISING //TX data change on falling edge , RX data change on rising edge
	       SetRegisterBit(TempUCSRC,UCPOL);  //UCPOL=1
         #endif

   #elif OPERATION_MODE == SLAVE_SYNCHRONOUS_UART
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
	CBuffer_BufferInit(& TX_Buffer); //initiate the transmitter buffer
	SetRegisterBit(UCSRB,TXEN); //Transmitter Enable
   #endif

   #if NODE_RECEIVE_DATA==ENABLE
	CBuffer_BufferInit(& RX_Buffer); //initiate the receiver buffer
	SetRegisterBit(UCSRB,RXEN); //Receiver Enable
   #endif

}


void UART_SendDataFrame(UARTData_t SendData,u8 *ErrorCode)
{

	*ErrorCode = CBuffer_PushData(&TX_Buffer,SendData);
	if(*ErrorCode == SUCCESSFUL_OPERATION)
	{
	    SetRegisterBit(SREG, 7); //enable global interrupt
	    SetRegisterBit(UCSRB ,UDRIE); //USART Data Register Empty Interrupt Enable
	}

}


u8 UART_ReceiveDataFrame(UARTData_t ReceivedData)
{
	SetRegisterBit(SREG, 7); //enable global interrupt
	SetRegisterBit(UCSRB ,RXCIE); //RX Complete Interrupt Enable
}


void UART_TransmitterStop(void)
{
#if NODE_TRANSMIT_DATA==ENABLE
    ClearRegisterBit(UCSRB, TXEN);  //Transmitter Disable
    ClearRegisterBit(UCSRB, UDRIE); //Disable USART Data Register Empty Interrupt
    CBuffer_BufferReset(& TX_Buffer); //Clear the TX buffer
#endif
}


void UART_Transmitter_ReEnable(void)
{
#if NODE_TRANSMIT_DATA==ENABLE
	SetRegisterBit(SREG, 7); //enable global interrupt
	SetRegisterBit(UCSRB ,UDRIE); //Enable USART Data Register Empty Interrupt
	SetRegisterBit(UCSRB,TXEN);   //Enable Transmitter
#endif
}


void UART_ReceiverStop(void)
{
#if NODE_RECEIVE_DATA==ENABLE
	ClearRegisterBit(UCSRB,RXCIE);//Disable RX Complete Interrupt
	ClearRegisterBit(UCSRB,RXEN); //Disable Receiver
	CBuffer_BufferReset(& RX_Buffer); //Clear the RX buffer
#endif
}


void UART_Receiver_ReEnable(void)
{
#if NODE_RECEIVE_DATA==ENABLE
	SetRegisterBit(SREG, 7); //enable global interrupt
	SetRegisterBit(UCSRB ,RXCIE); //Enable RX Complete Interrupt
	SetRegisterBit(UCSRB,RXEN);   //Enable Receiver
#endif
}




#if NODE_TRANSMIT_DATA==ENABLE
void __vector_14 (void) __attribute__ ((signal,used));
void __vector_14 (void)
{
	UARTData_t TX_Data;

    if(!CBuffer_IsTheBufferEmpty(&TX_Buffer))
    {
    	CBuffer_PopData(&TX_Buffer, &TX_Data);
    	UDR=TX_Data;
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

}
#endif
