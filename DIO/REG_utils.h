/*
 * REG_utils.h
 *
 *  Created on: ٢٨‏/١٢‏/٢٠٢٠
 *      Author: Mohamed_EL_Gallad
 */

#ifndef REG_UTILS_H_
#define REG_UTILS_H_

void SetPortDIR(u8 PortNum, u8 DIR) ;


u8 GetPortValue(u8 PortNum);


void SetPortValue(u8 PortNum,u8 Value);


u8 GetPortValue(u8 PortNum);


void SetPinDIR(u8 PortNum,u8 PIN_Num,u8 DIR);


void SetPinValue(u8 PortNum,u8 PIN_Num,u8 PIN_Value);


u8 GetPinValue(u8 PortNum, u8 PIN_Num);


void TogglePin(u8 PortNum , u8 PinNum);
#endif /* REG_UTILS_H_ */
