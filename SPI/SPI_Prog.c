/*
 * SPI_Prog.c
 *
 *  Created on: 23/08/2021
 *  Author: Mohamed_EL_Gallad
 *  Brief: This file will contain the SPI module functions implementation
 */

#include "STD_types.h"
#include "REG_utils.h"
#include "Mega32_reg.h"
#include "DIO_interface.h"
#include "SPI_Interface.h"
#include "CircularBufferInterface.h"

#if SPI_OPERATION_MODE == SLAVE_NODE
	//the following flag in used to avoid writing on SPDR register while the previous frame hasn't been shifted out yet
	u8 volatile static DataCollisionAvoidanceFlag =TRUE;


	static CircularBuffer SPI_RX_Buffer; //define a buffer for the RX data
	static CircularBuffer SPI_TX_Buffer; //define a buffer for the TX data

	//define a variable with suitable size to use it's individual bits as a flag for the packets stored in the TX_Buffer
	//the flag will determine either to receive or drop the received packet from the master that corresponds to the shifted out packets
	#if   TX_RX_BUFFER_SIZE <= 8
	volatile static u8  RX_PacketSkipFlag=0;
  	#elif TX_RX_BUFFER_SIZE >8 && TX_RX_BUFFER_SIZE <=16
    volatile static u16 RX_PacketSkipFlag=0;
 	#elif TX_RX_BUFFER_SIZE >16 && TX_RX_BUFFER_SIZE <=32
    volatile static u32 RX_PacketSkipFlag=0;
  	#elif TX_RX_BUFFER_SIZE >32 && TX_RX_BUFFER_SIZE <=64
    volatile static u64 RX_PacketSkipFlag=0;
  	#endif

    //the DataSkipIndexer variable will be used to tell how many bytes stored in the salve's SPI_TX_Buffer
    //will be used to define how many bits used so far from the RX_PacketSkipFlag variable
    volatile static u8  DataSkipIndexer=0;
#endif



/**
 *  RETURN     : VOID
 *  PARAMETERS : VOID
 *  DESCRIPTION: This function is used to initiate the SPI module depending on the User's configuration
 *  the SPI_Init() function will do the following :
 *  	1- Enable the SPI functionality
 *  	2- Set the transmission data order either LSB or MSB to be shifted first
 *  	3- Set the node to operate in either MASTER or SLAVE modes
 */
void SPI_Init(void)
{
  //enable SPI functionality
  SetRegisterBit(SPCR,SPE);

  //frame related configuration , transmission data order
  #if   DATA_ORDER == TRANSMIT_LSB_FIRST
     SetRegisterBit(SPCR,DORD);   //transmit the LSB bit first
  #elif DATA_ORDER == TRANSMIT_MSB_FIRST
     ClearRegisterBit(SPCR,DORD); //transmit the MSB bit first
  #endif

  //operation mode select between either salve or master
  #if SPI_OPERATION_MODE == MASTER_NODE //node configured as Master
     SetRegisterBit(SPCR,MSTR); //node will be defined as master
     SetPinDIR(1, 5, 1); //define MOSI-PB5 pin as output
     SetPinDIR(1, 7, 1); //define  CLK-PB7 pin as output
     SetPinDIR(1, 4, 1); //define   SS-PB4 pin as output
     SetPinValue(1, 4, 1); // pull SS pin PB5 to high state

       //master CLK configuration
       #if   SPI_MASTER_FREQ == CPU_FREQ_DIV_BY4
          ClearRegisterBit(SPCR ,SPR0);  //clear SPR0
          ClearRegisterBit(SPCR ,SPR1);  //clear SPR1
       #elif SPI_MASTER_FREQ == CPU_FREQ_DIV_BY16
          SetRegisterBit(SPCR,SPR0);     // set  SPR0
          ClearRegisterBit(SPCR ,SPR1);  //clear SPR1
       #elif SPI_MASTER_FREQ == CPU_FREQ_DIV_BY64
          ClearRegisterBit(SPCR,SPR0);  //clear SPR0
	      SetRegisterBit(SPCR ,SPR1);   // set  SPR1
       #elif SPI_MASTER_FREQ == CPU_FREQ_DIV_BY128
		  SetRegisterBit(SPCR ,SPR0);   // set  SPR0
		  SetRegisterBit(SPCR ,SPR1);   // set  SPR1
	   #endif

	   //double speed mode configuration
       #if   MASTER_DOUBLE_SPEED_MODE == ENABLE
		  SetRegisterBit(SPSR ,SPI2X);   //set SPI2X bit to 1 to enable double speed mode
       #elif MASTER_DOUBLE_SPEED_MODE == DISABLE
		  ClearRegisterBit(SPSR ,SPI2X); //clear SPI2X bit disable double speed mode
       #endif

       //Master CLK phase and polarity configuration
       #if   SPI_MASTER_CLK_MODE == CLK_PHASE_POLARITY_MODE_0
		  ClearRegisterBit(SPCR,CPOL);  //Clear CPOL bit
		  ClearRegisterBit(SPCR,CPHA);  //Clear CPHA bit
       #elif SPI_MASTER_CLK_MODE == CLK_PHASE_POLARITY_MODE_1
	  	  ClearRegisterBit(SPCR,CPOL);  //clear CPOL bit
	  	  SetRegisterBit(SPCR,CPHA);    //Set   CPHA bit
       #elif SPI_MASTER_CLK_MODE == CLK_PHASE_POLARITY_MODE_2
		  SetRegisterBit(SPCR,CPOL);    //Set  CPOL bit
		  ClearRegisterBit(SPCR,,CPHA); //Clear CPHA bit
       #elif SPI_MASTER_CLK_MODE == CLK_PHASE_POLARITY_MODE_3
		  SetRegisterBit(SPCR,CPOL);    //Set  CPOL bit
		  SetRegisterBit(SPCR,CPHA);    //Set  CPHA bit
       #endif


  #elif SPI_OPERATION_MODE ==SLAVE_NODE //node configured as slave
     ClearRegisterBit(SPCR,MSTR); // node will be defined as slave
     SetPinDIR(1, 6, 1); //define MISO-BP6 pin as output
     SetRegisterBit(SPCR,SPIE); //SPI interrupt enable
     SetRegisterBit(SREG, 7);   //enable global interrupt
	 CBuffer_BufferInit(& SPI_TX_Buffer); //initiate the SPI_TX_Buffer
	 CBuffer_BufferInit(& SPI_RX_Buffer); //initiate the SPI_RX_Buffer
  #endif
}


/**
 * RETURN      : u8 variable that will contain the received data from the slave
 * PARAMETERS  : u8 variable "SendData" which will contain a copy of the data to be sent from the master to the slave
 * DESCRIPTION : This function is used ONLY when the node is configured as master , the function will send and receive one byte .
 * in case the Master needs to push data from a slave a dummy packet has to sent from the master
 * this MACRO "SPI_DUMMY_PACKET" represents a dummy packet that can be used for this purpose
 */
u8 SPI_MasterSendAndReceiveByte( u8 SendData)
{
	SetPinValue(1, 4, 0); // pull SS pin PB5 to low state
	SPDR =SendData; //load the SPDR register with data to be sent
	while(!GetRegisterBit(SPSR,SPIF)); // halt till the data is shifted out from the SPDR register
	SetPinValue(1, 4, 1); //pull SS pin PB5 to high state
	return SPDR; // return the received data from the slave
}


/**
 * RETURN      : u8 variable that will contain the SPI_TX_Buffer status it will have one of these values
 * 				 FAILED_OPERATION     : will be the return value if the function was called while the node wasn't configured as slave
 * 				 SUCCESSFUL_OPERATION : will be the return value if the data pushed successfully to the SPI_TX_Buffer
 * 				 ERROR_BUFFER_FULL	 : will be the return value if the SPI_TX_Buffer is full
 * PARAMETERS  : DropReceivedPacket is a u8 variable that is used as flag to either drop or receive the shifted in packet that corresponds to the sifted out packet
 * 				 it must have one of two predefined Macros
 * 				 RX_DROP_PACKET when passed as a parameter the received packet that corresponds to the shifted out packet will be dropped
 * 				 RX_RECEIVE_PACKET when passed as a parameter the received packed that corresponds to the shifted out packet will be saved into the SPI_RX_Buffer
 * DESCRIPTION : This function is used if the node is configured as a slave , when called it will store the passed data in either the SPDR directly if it's empty
 * 				 and ready to receive new data or store the data in the SPI_TX_Buffer if the previous byte hasn't been shifted out yet
 * 				 this function also gives the user the control over the received data from the master to either drop the received packet or store it in the SPI_RX_Buffer
 * 				 ex. if the shifted packet from the master is known to be a dummy packet then there is no need to save the packet
 * 				 if the user decided to save the received packet it will be automatically stored in the SPI_RX_Buffer
 * 				 to be read later by calling the SPI_SlaveReadByteFromRXBuffer function
 */
u8 SPI_SlaveSendAndReceiveByte(const u8 *SendData , u8 DropReceivedPacket)
{
	u8 TX_BufferStatus=FAILED_OPERATION; //set a TX_BufferStatus as a default value
#if SPI_OPERATION_MODE == SLAVE_NODE
	if(!CBuffer_IsTheBufferFull(& SPI_TX_Buffer)) //store flags only if SPI_TX_Buffer isn't full "No of stored flag must equal the SPI_TX_Buffer size"
	{
		if(DropReceivedPacket == RX_RECEIVE_PACKET)
		{
			//shift 1 to the bit location that corresponds to the current data slot in the SPI_TX_Buffer
			RX_PacketSkipFlag |=RX_RECEIVE_PACKET<<DataSkipIndexer;
		}
		else
		{
			//shift 0 to the bit location that corresponds to the current data slot in the SPI_TX_Buffer
			RX_PacketSkipFlag &=~(RX_RECEIVE_PACKET<<DataSkipIndexer);
		}
		DataSkipIndexer++; //increase the DataSkipIndexer value to point at the next bit location in the RX_PacketSkipFlag
	}

	//if the SPI_TX_Buffer is empty and ready ,load the data directly to the SPDR register to be ready to be shifted out when the master initiate the communication
	if(CBuffer_IsTheBufferEmpty(&SPI_TX_Buffer) && DataCollisionAvoidanceFlag)
	{
		//set DataCollisionAvoidanceFlag value to FALSE to avoid writing the SPDR register while it's content hasn't been shifted out yet
		//it's value will be set to TRUE when the transmit complete interrupt occurs
		DataCollisionAvoidanceFlag =FALSE;
		//load data to the SPDR register
		SPDR =*SendData;
		//set the return value to SUCCESSFUL_OPERATION
	    TX_BufferStatus =SUCCESSFUL_OPERATION;
	}

	else //in case the SPDR register isn't ready to receive new data , store the passed data into the SPI_TX_Buffer
	{
		if(CBuffer_IsTheBufferFull(& SPI_TX_Buffer)== TRUE)
		{
			//set the return value to ERROR_BUFFER_FULL as the SPI_TX_Buffer is full
			TX_BufferStatus =ERROR_BUFFER_FULL;
		}
		else
		{
			//store the passed data into the SPI_TX_Buffer
			CBuffer_PushData(&SPI_TX_Buffer, *SendData);
			//set the return value to SUCCESSFUL_OPERATION
			TX_BufferStatus =SUCCESSFUL_OPERATION;
		}
	}

#endif
	//return the value of TX_BufferStatus
	return TX_BufferStatus;
}


/**
 * RETURN      : u8 variable that will be contain the error code , it will be either
 *  			 ERROR_BUFFER_EMPTY in case the SPI_RX_Buffer is empty
 *  			 or SUCCESSFUL_OPERATION if a data byte popped successfully from the SPI_RX_Buffer
 * PARAMETER   : ReceiveData is a pointer to predefined u8 variable by the user where the received data will be stored in
 * DESCRIPTION : This function is used to read the received data from the Master that has been stored in the SPI_RX_Buffer
 * 				 whenever the master sends a data packet it will be stored automatically in the SPI_RX_Buffer
 * 				 to be read later using this function
 */
u8 SPI_SlaveReadByteFromRXBuffer(u8 *ReceiveData)
{
	u8 RX_BufferStatus=FAILED_OPERATION; //set a RX_BufferStatus as a default value
#if SPI_OPERATION_MODE == SLAVE_NODE
	if(CBuffer_IsTheBufferEmpty(&SPI_RX_Buffer))
	{
		RX_BufferStatus=ERROR_BUFFER_EMPTY; //set RX_BufferStatus value to ERROR_BUFFER_EMPTY if the SPI_RX_Buffer is empty
	}
	else
	{
		CBuffer_PopData(&SPI_RX_Buffer, ReceiveData); //pop one byte of data and store it in the variable whose address is stored in ReceiveData
		RX_BufferStatus=SUCCESSFUL_OPERATION; //set RX_BufferStatus value to SUCCESSFUL_OPERATION
	}
#endif
	//return the value of RX_BufferStatus
	return RX_BufferStatus;

}


/**
 * Transmit complete interrupt is used only in case the node is configured as slave
 */
#if SPI_OPERATION_MODE == SLAVE_NODE
void __vector_12 (void) __attribute__ ((signal,used));
void __vector_12 (void)
{

	u8 SPDR_Data; //temporary storage to store the received data
	if(DataSkipIndexer) //if DataSkipIndexer not equals zero means that there is new data to be sent from slave or data stored in the SPI_TX_Buffer hasn't been shifted yet
	{
		//check either to receive or drop the received packet from the master "ex. the received packet is known to be a dummy packet"
		if(RX_PacketSkipFlag &0x01) //receive the packet corresponding to the shifted out packet in case the flag bit = RX_RECEIVE_PACKET
		{
			SPDR_Data=SPDR; // read the received data from the SPDR register
			CBuffer_PushData(&SPI_RX_Buffer ,SPDR_Data); // store the received data in the SPI_RX_Buffer
		}
		else //drop the received packet corresponding to the shifted out packet in case the flag bit = RX_DROP_PACKET
		{
			//no op
		}
		RX_PacketSkipFlag>>=1; //shift the flag variable by 1 bit to indicate to the status of the next packet to be received from the master
		DataSkipIndexer--; //decrease the DataSkipIndexer to point at the previous bit location in the RX_PacketSkipFlag
	}
	else //in case the DataSkipIndexer equals zero means that the slave interrupt produced by a received packet from the master with no new data to be sent from the slave
	{
		SPDR_Data=SPDR; //read the received data from the SPDR register
		CBuffer_PushData(&SPI_RX_Buffer ,SPDR_Data); // store the received data in SPI_RX_Buffer
	}

	//if there is SPI_TX_Buffer isn't empty then pop 1 byte from the buffer and store it into the SPDR register to be shifted in the upcoming 8 clocks from the master
	if(!CBuffer_IsTheBufferEmpty(& SPI_TX_Buffer))
	{
		CBuffer_PopData(&SPI_TX_Buffer ,&SPDR_Data); //pop data from SPI_TX_Buffer and store it in SPDR_Data
		SPDR = SPDR_Data; // load the data to SPDR register
	}

	DataCollisionAvoidanceFlag=TRUE; //data shifted out of SPDR register and it's free to receive new data
}
#endif
