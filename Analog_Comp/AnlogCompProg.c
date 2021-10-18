/*
 * AnlogCompProg.c
 *
 *  Created on:17/10/2021
 *  Author: Mohamed_EL_Gallad
 *  Description : this file will contain the functions implementation for the analog comparator module
 */
#include "STD_types.h"
#include "Mega32_reg.h"
#include "REG_utils.h"
#include "DIO_interface.h"
#include "AnlogCompInterface.h"

static void (*AalogCompUserFnPtr)(void) = NULL ; //pointer to a function the will hold the address of the user's defined function

/**
 * RETURN     : VOID
 * PARAMETER  : VOID
 * DESCRIPTION: This function must be called at first before the use of the Analog Comparator module
 * the call of this function will initiate the module depending on the user's configuration in AnalogCompConfig.h
 * this function will set the following
 * 		1- enable or disable the analog comparator multiplexing for the analog comparator negative input
 * 		2- enable or disable the analog comparator bandgap to be used as a voltage reference on analog comparator positive input
 * 		3- enable or disable the use of timer_1 ICU
 * 		4- enable or disable the analog comparator interrupt and define the trigger type "rising edge, falling edge or toggle"
 */
void AnalogComp_Init(void)
{
	//negative input selection
	#if(ANALOG_COMPARATOR_MULTIPLEXER == ENABLE)
		ClearRegisterBit(ADCSRA , ADEN); //disable the ADC
		SetRegisterBit(ACSR, ACME); //enable the Analog Comparator Multiplexer
	#else
		ClearRegisterBit(ACSR, ACME);//disable the Analog Comparator Multiplexer
		SetPinDIR(1, 3, 0); //configure PB3 - AIN1 as input
		SetPinValue(1, 3, 0); //disable pull up for PB3 - AIN1
	#endif

	//positive input voltage reference selection
	#if(ANALOG_COMPARATOR_BANDGAP_SELECT == ENABLE)
	SetRegisterBit(ACSR, ACBG); // fixed bandgap reference voltage replaces the positive input
	#else
	ClearRegisterBit(ACSR, ACBG); //AIN0 is applied to the positive input
	SetPinDIR(1, 2, 0); //configure PB2 - AIN0 as input
	SetPinValue(1, 3, 0); //disable pull up for PB2 - AIN0
	#endif

	//Analog Comparator Input Capture Enable/Disable
	#if (ANALOG_COMPARATOR_INPUTCAPTURE == ENABLE )
		SetRegisterBit(ACSR, ACIC); //connect the input capture unit to the analog comparator
	#else
		ClearRegisterBit(ACSR, ACIC); //disconnect the input capture unit to the analog comparator
	#endif

	//Analog Comparator interrupt mode select
	#if  (ANALOG_COMPARATOR_INTERRUPT_OPMODE == INTERRUPT_ON_TOGGLE)
		SetRegisterBit(ACSR, ACIE);  //enable analog comparator interrupt
		ClearRegisterBit(ACSR, ACIS0); //clear ACIS0
		ClearRegisterBit(ACSR, ACIS1); //clear ACIS1

	#elif(ANALOG_COMPARATOR_INTERRUPT_OPMODE == INTERRUPT_ON_FALLING_EDGE)
		SetRegisterBit(ACSR, ACIE);  //enable analog comparator interrupt
		ClearRegisterBit(ACSR, ACIS0); //clear ACIS0
		SetRegisterBit(ACSR , ACIS1);  //set ACIS1

	#elif(ANALOG_COMPARATOR_INTERRUPT_OPMODE == INTERRUPT_ON_RISING_EDGE)
		SetRegisterBit(ACSR, ACIE);  //enable analog comparator interrupt
		SetRegisterBit(ACSR , ACIS0);  //set ACIS0
		SetRegisterBit(ACSR , ACIS1);  //set ACIS1

	#elif(ANALOG_COMPARATOR_INTERRUPT_OPMODE == INTERRUPT_DISABLE)
		ClearRegisterBit(ACSR, ACIE); //disable analog comparator interrupt
	#endif
}


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
 * DESCRIPTION: This function is used to enable the analog comparaotr functionality
 */
void AnalogComp_Enable(u8 NegativeInputOption)
{
	ClearRegisterBit(ACSR, ACIE); //disable analog comparator interrupt temporary to avoid accidental triggering
	ClearRegisterBit(ACSR, ACD);   //Analog Comparator Enable
	if (AalogCompUserFnPtr != NULL)
	{
		SetRegisterBit(ACSR, ACIE); //re-enable the analog comparator interrupt in case a user function is defined
	}

	if(NegativeInputOption == NEGATIVE_INPUT_ON_AIN1)
	{
		ClearRegisterBit(ACSR, ACME); //disable the Analog Comparator Multiplexer , negative input will be on AIN1
	}
	#if(ANALOG_COMPARATOR_MULTIPLEXER == ENABLE)
	else if(ANALOG_COMPARATOR_MULTIPLEXER == MULTIPLEXED_INPUT_ADC0)
	{
		// MUX0=0 MUX1=0 MUX2=0 , ADC0 Selection
		ClearRegisterBit (ADMUX , MUX0);
		ClearRegisterBit (ADMUX , MUX1);
		ClearRegisterBit (ADMUX , MUX2);
	}
	else if(ANALOG_COMPARATOR_MULTIPLEXER == MULTIPLEXED_INPUT_ADC1)
	{
		// MUX0=1 MUX1=0 MUX2=0 , ADC1 Selection
		SetRegisterBit   (ADMUX , MUX0);
		ClearRegisterBit (ADMUX , MUX1);
		ClearRegisterBit (ADMUX , MUX2);
	}
	else if(ANALOG_COMPARATOR_MULTIPLEXER == MULTIPLEXED_INPUT_ADC2)
	{
		// MUX0=0 MUX1=1 MUX2=0 , ADC2 Selection
		ClearRegisterBit (ADMUX , MUX0);
		SetRegisterBit   (ADMUX , MUX1);
		ClearRegisterBit (ADMUX , MUX2);
	}
	else if(ANALOG_COMPARATOR_MULTIPLEXER == MULTIPLEXED_INPUT_ADC3)
	{
		// MUX0=1 MUX1=1 MUX2=0 , ADC3 Selection
		SetRegisterBit  (ADMUX , MUX0);
		SetRegisterBit  (ADMUX , MUX1);
		ClearRegisterBit(ADMUX , MUX2);
	}
	else if(ANALOG_COMPARATOR_MULTIPLEXER == MULTIPLEXED_INPUT_ADC4)
	{
		// MUX0=0 MUX1=0 MUX2=1 , ADC4 Selection
		ClearRegisterBit (ADMUX , MUX0);
		ClearRegisterBit (ADMUX , MUX1);
		SetRegisterBit   (ADMUX , MUX2);
	}
	else if(ANALOG_COMPARATOR_MULTIPLEXER == MULTIPLEXED_INPUT_ADC5)
	{
		// MUX0=1 MUX1=0 MUX2=1 , ADC5 Selection
		SetRegisterBit  (ADMUX , MUX0);
		ClearRegisterBit(ADMUX , MUX1);
		SetRegisterBit  (ADMUX , MUX2);
	}
	else if(ANALOG_COMPARATOR_MULTIPLEXER == MULTIPLEXED_INPUT_ADC6)
	{
		// MUX0=0 MUX1=1 MUX2=1 , ADC6 Selection
		ClearRegisterBit(ADMUX , MUX0);
		SetRegisterBit  (ADMUX , MUX1);
		SetRegisterBit  (ADMUX , MUX2);
	}
	else if(ANALOG_COMPARATOR_MULTIPLEXER == MULTIPLEXED_INPUT_ADC7)
	{
		// MUX0=1 MUX1=1 MUX2=1 , ADC7 Selection
		SetRegisterBit (ADMUX , MUX0);
		SetRegisterBit (ADMUX , MUX1);
		SetRegisterBit (ADMUX , MUX2);
	}

	#endif

}


/**
 * RETURN     : VOID
 * PARAMETER  : VOID
 * DESCRIPTION: The call of this function will disable the Analog Comparator functionality to save power
 * 	at anytime the user can re-enable the analog comparator module by calling AnalogComp_Enable();
 */
void AnalogComp_Disable(void)
{
	ClearRegisterBit(ACSR, ACIE); //disable analog comparator interrupt temporary to avoid accidental triggering
	SetRegisterBit(ACSR, ACD); //Analog Comparator Disable
}


/**
 * RETURN     : u8 variable represents the result of the comparison it's value will be
 * 			    ONE  if the Positive input is  greater than the negative input
 * 			    ZERO if the Positive input is  less    than the negative input
 * PARAMETER  : VOID
 * DESCRIPTION: This function will return the result of the comparison between the negative and positive inputs of the analog comparator
 */
u8 AnalogComp_GetCompareResult(void)
{
	return GetRegisterBit(ACSR , ACO); //Get the value of the analog comparator ACO
}


/**
 * RETURN     : VOID
 * PARAMETER  : AnalogComp_UserFn is a pointer to a user defined function that has a void parameter and void return
 * 			    this function will be executed when the analog comparator interrupt is triggered .
 * DESCRIPTION: this function is used to mount a user defined function to be executed when the analog comparator interrupt is triggered
 * 	the call of this function will enable the analog comparator interrupt even if ANALOG_COMPARATOR_INTERRUPT_OPMODE is set to INTERRUPT_DISABLE
 */
void AnalogComp_ExecuteUserFnOnInterrupt(void (*AnalogComp_UserFn)(void))
{
	if (AnalogComp_UserFn != NULL)
	{
		AalogCompUserFnPtr = AnalogComp_UserFn; //mount the user defined function address to AalogCompUserFnPtr
		SetRegisterBit(SREG ,7); //ensure that the global interrupt is enabled
		SetRegisterBit(ACSR, ACIE); //enable the analog comparator interrupt
	}
}


/**
 * RETURN     : VOID
 * PARAMETER  : VOID
 * DESCRIPTION: This function will disable the execution of the user's defined function and also will disable the analog comparator interrupt
 */
void AnalogComp_DisableUserFN(void)
{
	AalogCompUserFnPtr = NULL;    //clear the AalogCompUserFnPtr pointer
	ClearRegisterBit(ACSR, ACIE); //disable the analog comparator interrupt
}


/**
 *  Analog Comparator ISR
 */
void __vector_18(void) __attribute__ ((signal,used));
void __vector_18(void)
{
	if (AalogCompUserFnPtr != NULL)
	{
	AalogCompUserFnPtr();
	}
}

