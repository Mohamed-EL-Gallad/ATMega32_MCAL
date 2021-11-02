/*
 * ADC_Interface.h
 *
 *  Created on: 18/10/2021
 *  Author: Mohamed_EL_Gallad
 */

#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_


#include "ADC_Config.h"
#include "ADC_Private.h"


void ADC_Init (void); //
void ADC_Enable(void); //
void ADC_Disable(void); //
void ADC_StartConversion(void); //
void ADC_SelectChanelAndGain(u8 MuxValue); //
u16  ADC_GetConvResult(void);//

void ADC_ExecuteUserFnOnInterrupt(void (*ADC_ConvComp)(void)); //
void ADC_DisableUserFN(void); //


#endif /* ADC_INTERFACE_H_ */
