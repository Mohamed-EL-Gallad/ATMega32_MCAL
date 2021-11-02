/*
 * REG_utils.h
 *
 *  Created on: 18/10/2021
 *  Author: Mohamed_EL_Gallad
 */

#ifndef DIO_INTERFACE_H_
#define DIO_INTERFACE_H_
#include "STD_types.h"
void SetPortDIR(u8 PortNum, u8 DIR) ;


u8 GetPortValue(u8 PortNum);


void SetPortValue(u8 PortNum,u8 Value);


u8 GetPortValue(u8 PortNum);


void SetPinDIR(u8 PortNum,u8 PIN_Num,u8 DIR);


void SetPinValue(u8 PortNum,u8 PIN_Num,u8 PIN_Value);


u8 GetPinValue(u8 PortNum, u8 PIN_Num);


void TogglePin(u8 PortNum , u8 PinNum);
#endif /* DIO_INTERFACE_H_ */
