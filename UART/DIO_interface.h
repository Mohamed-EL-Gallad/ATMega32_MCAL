/*
 * DIO.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: spider
 */
#ifndef DIO_H_
#define DIO_H_
#include "std_types.h"

void DIO_vidSetPortDirection(u8 u8PortCpy , u8 u8DirectionCpy);
void DIO_vidSetPortValue(u8 u8PortCpy , u8 u8ValueCpy);
u8 DIO_u8GetPortValue(u8 u8PortCpy);
void DIO_vidSetPinDirection(u8 u8PortCpy , u8 u8PinCpy , u8 u8DirCpy);
void DIO_vidSetPinValue(u8 u8PortCpy , u8 u8PinCpy , u8 u8ValueCpy);
u8 DIO_u8GetPinValue(u8 u8PortCpy,u8 u8PinCpy);

#endif /* DIO_H_ */
