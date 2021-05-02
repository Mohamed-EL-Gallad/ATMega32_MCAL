/*
 * AVR_EEPROM.c
 *
 *  Created on: 01/05/2021
 *  Author: Mohamed_EL_Gallad
 */
#include "STD_types.h"
#include "Mega32_reg.h"
#include "AVR_EEPROM.h"

#define EEPROM_MAX_ADDRESSES 1024 //EEPROM available addresses from 0 to 1023


/**
 * RETURN      : VOID
 * PARAMETERS  : DataPTR , A void Pointer to a SRAM variable which point to the start of the required data to be stored in EEPROM
 *             : NBytes , u16 variable represent the number of bytes that required to be copied from SRAM to EEPROM
 *             : EEPROM_Addr,the start address in which the first byte of the data will be stored in the EEPROM "available addresses from 0 to 1023"
 * Description : This function is used to copy number of bytes or even a single byte of any data type from SRAM to EEPROM
 */
void EEPROM_WriteNbytes(void *DataPTR ,u16 NBytes, u16 EEPROM_Addr)
{
  u16 BytesCounter;
   for(BytesCounter=0 ; BytesCounter<NBytes && EEPROM_Addr<EEPROM_MAX_ADDRESSES ; BytesCounter++)
   {
	   while (EECR & 0x02); //wait till EEWE bit is being cleared means no current write operation in process
	   EEDR =((u8*)DataPTR)[BytesCounter]; //store the required data byte by byte in EEDR register
       EEARH =((EEPROM_Addr & 0x0300)>>8); //store the high byte of the passed address in EEARH
       EEARL =(EEPROM_Addr & 0x00FF); //store the low byte of the passed address in EEARL
	   EEPROM_Addr++; //increase EEPROM address one step after each iteration
       EECR |= (1<<2); //EEPROM Master Write Enable
       EECR |= (1<<1); //Start data writing in EEPROM
   }
}


/**
 * RETURN      : u8 variable represents the data stored in the given EEPROM address
 * PARAMETER   : EEPROM_Addr, is a u16 variable which represent the EEPROM address where the required data is being stored in "from 0 to 1023"
 * Description : This function will return the required data "1 byte" that is being stored in a given address EEPROM_Addr
 */
u8 EEPROM_GetByte(u16 EEPROM_Addr)
{
	while (EECR & 0x02); //wait till EEWE bit is being cleared means no current write operation in process
    EEARH =((EEPROM_Addr & 0x0300)>>8); //store the high byte of the passed address in EEARH
    EEARL =(EEPROM_Addr & 0x00FF); //store the low byte of the passed address in EEARL
    EECR |= (1<<0);//Start data reading from EEPROM
    return EEDR;
}


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * Description : This function will clear all EEPROM cells "0xFF will be stored in each byte"
 */
void EEPROM_Clear(void)
{
	u8 ClearValue=0xFF; //the default value for EEPROM cells
	u16 EEPROM_CellCounter;
	for(EEPROM_CellCounter=0 ; EEPROM_CellCounter<EEPROM_MAX_ADDRESSES ; EEPROM_CellCounter++)
	{
	EEPROM_WriteNbytes(&ClearValue, 1, EEPROM_CellCounter); //store the default value in all EEPROM cells
	}
}
