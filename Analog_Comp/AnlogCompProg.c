/*
 * AnlogCompProg.c
 *
 *  Created on:17/10/2021
 *  Author: Mohamed_EL_Gallad
 */
#include "STD_types.h"
#include "Mega32_reg.h"
#include "REG_utils.h"
#include "DIO_interface.h"
#include "AnlogCompInterface.h"

static void (*AalogCompUserFnPtr)(void) = NULL;

void AnalogComp_Init(void) {
	//negative input selection
	#if(ANALOG_COMPARATOR_MULTIPLEXER == ENABLE)
		ClearRegisterBit(ADCSRA , ADEN); //disable the ADC
		SetRegisterBit(ACSR, ACME); //enable the Analog Comparator Multiplexer
	#else
		ClearRegisterBit(ACSR, ACME);//disable the Analog Comparator Multiplexer
	#endif

	//positive input voltage reference selection
	#if(ANALOG_COMPARATOR_BANDGAP_SELECT == ENABLE)
		SetRegisterBit(ACSR, ACBG);
	#else
	ClearRegisterBit(ACSR, ACBG);
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

void AnalogComp_Enable(u8 NegativeInputOption) {
	ClearRegisterBit(ACSR, ACIE); //disable analog comparator interrupt temporary to avoid accidental triggering
	ClearRegisterBit(ACSR, ACD);   //Analog Comparator Enable
	if (AalogCompUserFnPtr != NULL)
	{
		SetRegisterBit(ACSR, ACIE); //re-enable the analog comparator interrupt in case a user function is defined
	}

	if(NegativeInputOption == NEGATIVE_INPUT_ON_AIN1)
	{
		ClearRegisterBit(ACSR, ACME); //disable the Analog Comparator Multiplexer
	}
	#if(ANALOG_COMPARATOR_MULTIPLEXER == ENABLE)
	else if(ANALOG_COMPARATOR_MULTIPLEXER == MULTIPLEXED_INPUT_ADC0)
	{
		ClearRegisterBit (ADMUX , MUX0);
		ClearRegisterBit (ADMUX , MUX1);
		ClearRegisterBit (ADMUX , MUX2);
	}
	else if(ANALOG_COMPARATOR_MULTIPLEXER == MULTIPLEXED_INPUT_ADC1)
	{
		SetRegisterBit   (ADMUX , MUX0);
		ClearRegisterBit (ADMUX , MUX1);
		ClearRegisterBit (ADMUX , MUX2);
	}
	else if(ANALOG_COMPARATOR_MULTIPLEXER == MULTIPLEXED_INPUT_ADC2)
	{
		ClearRegisterBit (ADMUX , MUX0);
		SetRegisterBit   (ADMUX , MUX1);
		ClearRegisterBit (ADMUX , MUX2);
	}
	else if(ANALOG_COMPARATOR_MULTIPLEXER == MULTIPLEXED_INPUT_ADC3)
	{
		SetRegisterBit  (ADMUX , MUX0);
		SetRegisterBit  (ADMUX , MUX1);
		ClearRegisterBit(ADMUX , MUX2);
	}
	else if(ANALOG_COMPARATOR_MULTIPLEXER == MULTIPLEXED_INPUT_ADC4)
	{
		ClearRegisterBit (ADMUX , MUX0);
		ClearRegisterBit (ADMUX , MUX1);
		SetRegisterBit   (ADMUX , MUX2);
	}
	else if(ANALOG_COMPARATOR_MULTIPLEXER == MULTIPLEXED_INPUT_ADC5)
	{
		SetRegisterBit  (ADMUX , MUX0);
		ClearRegisterBit(ADMUX , MUX1);
		SetRegisterBit  (ADMUX , MUX2);
	}
	else if(ANALOG_COMPARATOR_MULTIPLEXER == MULTIPLEXED_INPUT_ADC6)
	{
		ClearRegisterBit(ADMUX , MUX0);
		SetRegisterBit  (ADMUX , MUX1);
		SetRegisterBit  (ADMUX , MUX2);
	}
	else if(ANALOG_COMPARATOR_MULTIPLEXER == MULTIPLEXED_INPUT_ADC7)
	{
		SetRegisterBit (ADMUX , MUX0);
		SetRegisterBit (ADMUX , MUX1);
		SetRegisterBit (ADMUX , MUX2);
	}

	#endif

}

void AnalogComp_Disable(void) {
	ClearRegisterBit(ACSR, ACIE); //disable analog comparator interrupt temporary to avoid accidental triggering
	SetRegisterBit(ACSR, ACD); //Analog Comparator Disable

}

void AnalogComp_ExecuteUserFnOnInterrupt(void (*AnalogComp_UserFn)(void)) {
	if (AnalogComp_UserFn != NULL)
	{
		AalogCompUserFnPtr = AnalogComp_UserFn; //mount the user defined function address to AalogCompUserFnPtr
		SetRegisterBit(SREG ,7); //ensure that the global interrupt is enabled
		SetRegisterBit(ACSR, ACIE); //enable the analog comparator interrupt
	}
}

void AnalogComp_DisableUserFN(void) {
	AalogCompUserFnPtr = NULL;    //clear the AalogCompUserFnPtr pointer
	ClearRegisterBit(ACSR, ACIE); //disable the analog comparator interrupt
}

void __vector_18(void) __attribute__ ((signal,used));
void __vector_18(void) {
	AalogCompUserFnPtr();
}

