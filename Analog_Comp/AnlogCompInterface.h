/*
 * AnlogCompInterface.h
 *
 *  Created on:17/10/2021
 *  Author: Mohamed_EL_Gallad
 *  Description : This file will contain the interface function for the Analog comparator module
 *   1- to initiate the Analog comparator functionality the user has to call AnalogComp_Init(); first to configure the analog comparator
 *   based on the the configurations that has been chosen by the user in the  AnalogCompConfig.h file.
 *
 *   2- to enable the Analog Comparator the user should call AnalogComp_Enable(); otherwise the module will be in disabled mode.
 *
 *   3- at anytime the user can disable the module by calling AnalogComp_Disable(); and can re-enable it at anytime.
 *
 *   4- to get the comparison result between the negative and positive input the user should call  AnalogComp_GetCompareResult();.
 *
 *   5- the user can define a function to be executed when a pre-configured trigger for the analog comparator interrupt is a achieved
 *   this can be done using this function AnalogComp_ExecuteUserFnOnInterrupt(pointer_to_the_user's_function);
 *   the call of this function will enable the analog comparator interrupt even if ANALOG_COMPARATOR_INTERRUPT_OPMODE macros is equal INTERRUPT_DISABLE.
 *
 *   6- at anytime the user can disable the execution of the user's defined function from being executed and disable the analog comparator interrupt
 *   by calling this function AnalogComp_DisableUserFN();.
 */

#ifndef ANLOGCOMPINTERFACE_H_
#define ANLOGCOMPINTERFACE_H_

#include "AnalogCompPrivate.h"
#include "AnalogCompConfig.h"

/**
 * RETURN     : VOID
 * PARAMETER  : VOID
 * DESCRIPTION: This function must be called at first before the use of the Analog Comparator module
 * the call of this function will initiate the module depending on the user's configuration in AnalogCompConfig.h
 * this function will set the following properties based on the configured values in AnalogCompConfig.h
 * 		1- enable or disable the analog comparator multiplexing for the analog comparator negative input
 * 		2- enable or disable the analog comparator bandgap to be used as a voltage reference on analog comparator positive input
 * 		3- enable or disable the use of timer_1 ICU
 * 		4- enable or disable the analog comparator interrupt and define the trigger type "rising edge, falling edge or toggle"
 */
void AnalogComp_Init(void);


/**
 * RETURN     : VOID
 * PARAMETER  : u8 variable that will represent the negative input of the analog comparator it's value has to be one of the following Macros
 *    if the ANALOG_COMPARATOR_MULTIPLEXER Macro is set to ENABLE the NegativeInputOption variable should equal one one of following MACROS
 * 		 MULTIPLEXED_INPUT_ADC0 , the negative input will be on ADCO - PA0 PIN
 *		 MULTIPLEXED_INPUT_ADC1 , the negative input will be on ADC1 - PA1 PIN
 *		 MULTIPLEXED_INPUT_ADC2 , the negative input will be on ADC2 - PA2 PIN
 * 		 MULTIPLEXED_INPUT_ADC3 , the negative input will be on ADC3 - PA3 PIN
 *		 MULTIPLEXED_INPUT_ADC4 , the negative input will be on ADC4 - PA4 PIN
 *		 MULTIPLEXED_INPUT_ADC5 , the negative input will be on ADC5 - PA5 PIN
 *		 MULTIPLEXED_INPUT_ADC6 , the negative input will be on ADC6 - PA6 PIN
 *		 MULTIPLEXED_INPUT_ADC7 , the negative input will be on ADC7 - PA7 PIN
 *	  if the ANALOG_COMPARATOR_MULTIPLEXER Macro is set to DISABLE the NegativeInputOption variable should equal the following
 *		 NEGATIVE_INPUT_ON_AIN1 , the negative input will be on AIN1 - PB3 PIN
 * DESCRIPTION: This function is used to enable the analog comparator functionality
 */
void AnalogComp_Enable(u8 NegativeInputOption);


/**
 * RETURN     : VOID
 * PARAMETER  : VOID
 * DESCRIPTION: The call of this function will disable the Analog Comparator functionality to save power
 * 	at anytime the user can re-enable the analog comparator module by calling AnalogComp_Enable();
 */
void AnalogComp_Disable(void);


/**
 * RETURN     : u8 variable represents the result of the comparison it's value will be
 * 			    ONE  if the Positive input is  greater than the negative input
 * 			    ZERO if the Positive input is  less    than the negative input
 * PARAMETER  : VOID
 * DESCRIPTION: This function will return the result of the comparison between the negative and positive inputs of the analog comparator
 */
u8 AnalogComp_GetCompareResult(void);


/**
 * RETURN     : VOID
 * PARAMETER  : AnalogComp_UserFn is a pointer to a user defined function that has a void parameter and void return
 * 			    this function will be executed when the analog comparator interrupt is triggered .
 * DESCRIPTION: this function is used to mount a user defined function to be executed when the analog comparator interrupt is triggered
 * 	the call of this function will enable the analog comparator interrupt even if ANALOG_COMPARATOR_INTERRUPT_OPMODE is set to INTERRUPT_DISABLE
 */
void AnalogComp_ExecuteUserFnOnInterrupt(void (*AnalogComp_UserFn)(void));


/**
 * RETURN     : VOID
 * PARAMETER  : VOID
 * DESCRIPTION: This function will disable the execution of the user's defined function and also will disable the analog comparator interrupt
 */
void AnalogComp_DisableUserFN(void);

#endif /* ANLOGCOMPINTERFACE_H_ */
