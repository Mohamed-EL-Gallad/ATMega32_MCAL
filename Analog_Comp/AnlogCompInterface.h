/*
 * AnlogCompInterface.h
 *
 *  Created on:17/10/2021
 *  Author: Mohamed_EL_Gallad
 */

#ifndef ANLOGCOMPINTERFACE_H_
#define ANLOGCOMPINTERFACE_H_

#include "AnalogCompPrivate.h"
#include "AnalogCompConfig.h"

void AnalogComp_Init(void);
void AnalogComp_Enable(u8 NegativeInputOption);
void AnalogComp_Disable(void);
void AnalogComp_ExecuteUserFnOnInterrupt(void (*AnalogComp_UserFn)(void));
void AnalogComp_DisableUserFN(void);

#endif /* ANLOGCOMPINTERFACE_H_ */
