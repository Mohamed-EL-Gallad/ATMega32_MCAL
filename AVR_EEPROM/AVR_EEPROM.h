/*
 * AVR_EEPROM.h
 *
 *  Created on: 01/05/2021
 *  Author: Mohamed_EL_Gallad
 */

#ifndef AVR_EEPROM_H_
#define AVR_EEPROM_H_
#include "STD_types.h"


/**
 *  Using this MACRO before a variable declaration will force the variable to be stored in the .eeprom section
 */
#define AVR_EEPROM __attribute__ ((section(".eeprom")))


/**
 * RETURN      : VOID
 * PARAMETERS  : DataPTR , A void Pointer to a SRAM variable which point to the start of the required data to be stored in EEPROM
 *             : NBytes , u16 variable represent the number of bytes that required to be copied from SRAM to EEPROM
 *             : EEPROM_Addr,the start address in which the first byte of the data will be stored in the EEPROM "available addresses from 0 to 1023"
 * Description : This function is used to copy number of bytes or even a single byte of any data type from SRAM to EEPROM
 */
void EEPROM_WriteNbytes(void *DataPTR ,u16 NBytes, u16 EEPROM_Addr);


/**
 * RETURN      : u8 variable represents the data stored in the given EEPROM address
 * PARAMETER   : EEPROM_Addr, is a u16 variable which represent the EEPROM address where the required data is being stored in "from 0 to 1023"
 * Description : This function will return the required data "1 byte" that is being stored in a given address EEPROM_Addr
 */
u8 EEPROM_GetByte(u16 EEPROM_Addr);


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * Description : This function will clear all EEPROM cells "0xFF will be stored in each byte"
 */
void EEPROM_Clear(void);

#endif /* AVR_EEPROM_H_ */
