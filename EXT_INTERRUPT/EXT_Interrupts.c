/*
 * EXT_Interrupt.c
 *
 *  Created on:20/04/2021
 *  Author: Mohamed_EL_Gallad
 *  Brief : This file will contain the implementation for the external interrupts functions
 */
#include "STD_types.h"
#include "Mega32_reg.h"
#include "REG_utils.h"
#include "EXT_InterruptsInterface.h"
#include "EXT_InterruptsConfig.h"

static void(*INT0_CallBackFN)(void)=NULL; //pointer to a user defined function to be executed when INT0 is being triggered
static void(*INT1_CallBackFN)(void)=NULL; //pointer to a user defined function to be executed when INT1 is being triggered
static void(*INT2_CallBackFN)(void)=NULL; //pointer to a user defined function to be executed when INT2 is being triggered


/**
 * RETURN      : VOID
 * PARAMETER   : INT0_UserFN , a pointer to a user defined function to be executed when INT0 is triggered
 * DESCRIPTION : This function is used to enable INT0 and set it's external trigger type depending on
 * the predefined MACRO INT0_TRIG_TYPE , it will also mount a user defined function on INT0 ISR to be executed whenever INT0 is being triggered
 */
void Enable_INT0(void(*INT0_UserFN)(void))
{
     #if   INT0_TRIG_TYPE==LOW_LEVEL  //configure the trigger type to be low level
	 MCUCR &=~(1<<0 | 1<<1); //ISC00=0 ISC11=10

     #elif INT0_TRIG_TYPE==ANY_LOGICAL_CHANGE //configure the trigger type to be any logical level
	 MCUCR |= (1<<0);  //ISC00=1
	 MCUCR &=~(1<<1);  //ISC01=0

     #elif INT0_TRIG_TYPE==FALLING_EDGE //configure the trigger type to be falling edge
	 MCUCR &=~(1<<0);  //ISC00=0
	 MCUCR |= (1<<1);  //ISC01=1

     #elif INT0_TRIG_TYPE==RISING_EDGE //configure the trigger type to be rising edge
     MCUCR |= (1<<0 | 1<<1);  //ISC00=1	 ISC01=1
     #endif

 	if(INT0_UserFN != NULL )
 	{
	SREG |=(1<<7); //make sure that the global interrupt is enabled
	GICR |=(1<<6); //enable INT0
	INT0_CallBackFN=INT0_UserFN;
 	}
 	else
 	{
 	GICR &=~(1<<6); // in case if INT0_UserFN is NULL , ensure that INT0 ISR is disabled
 	}
}


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : This function is used to disable INT0 and it will also unmount the user defined function from INT0 ISR
 */
void Disable_INT0(void)
{
	GICR &=~(1<<6); //INT0 ISR is disabled
	INT0_CallBackFN=NULL; //set the function callback pointer to NULL
}


/**
 * RETURN      : VOID
 * PARAMETER   : INT1_UserFN , a pointer to a user defined function to be executed when INT1 is triggered
 * DESCRIPTION : This function is used to enable INT1 and set it's external trigger type depending on
 * the predefined MACRO INT1_TRIG_TYPE , it will also mount a user defined function on INT1 ISR to be executed whenever INT1 is being triggered
 */
void Enable_INT1(void(*INT1_UserFN)(void))
{

     #if   INT1_TRIG_TYPE==LOW_LEVEL  //configure the trigger type to be low level
     MCUCR &=~(1<<2 | 1<<3); //ISC10=0 ISC11=0

     #elif INT1_TRIG_TYPE==ANY_LOGICAL_CHANGE //configure the trigger type to be any logical level
     MCUCR |= (1<<2);  //ISC10=1
     MCUCR &=~(1<<3);  //ISC11=0

     #elif INT1_TRIG_TYPE==FALLING_EDGE //configure the trigger type to be falling edge
     MCUCR &=~(1<<2);  //ISC10=0
     MCUCR |= (1<<3);  //ISC11=1

     #elif INT1_TRIG_TYPE==RISING_EDGE //configure the trigger type to be rising edge
     MCUCR |= (1<<3 | 1<<2);  //ISC10=1	 ISC11=1
     #endif

	if(INT1_UserFN != NULL )
	{
	SREG  |=(1<<7); //make sure that the global interrupt is enabled
	GICR |=(1<<7); //enable INT1
	INT1_CallBackFN=INT1_UserFN;
	}
	else
	{
	GICR &=~(1<<7); // in case if INT1_UserFN is NULL , ensure that INT1 ISR is disabled
	}
}


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : This function is used to disable INT1 and it will also unmount the user defined function from INT1 ISR
 */
void Disable_INT1(void)
{
	GICR &=~(1<<7); //INT1 ISR is disabled
	INT1_CallBackFN=NULL; //set the function callback pointer to NULL
}


/**
 * RETURN      : VOID
 * PARAMETER   : INT2_UserFN , a pointer to a user defined function to be executed when INT2 is triggered
 * DESCRIPTION : This function is used to enable INT2 and set it's external trigger type depending on
 * the predefined MACRO INT2_TRIG_TYPE , it will also mount a user defined function on INT2 ISR to be executed whenever INT2 is being triggered
 */
void Enable_INT2(void(*INT2_UserFN)(void))
{
	GICR &=~(1<<5); //Disabling INT2 temporary till selecting the edge trigger to avoid accidental trigger

     #if  INT2_TRIG_TYPE==INT2_FALLING_EDGE
	 MCUCSR &=~(1<<6); //ISC2=0 falling edge will trigger INT2

     #elif INT2_TRIG_TYPE==INT2_RISING_EDGE
	 MCUCSR |=(1<<6);
     #endif

	if(INT2_UserFN != NULL )
	{
	SREG  |=(1<<7); //make sure that the global interrupt is enabled
	GICR |=(1<<5); //enable INT2
	INT2_CallBackFN=INT2_UserFN;
	}
	else
	{
	GICR&=~(1<<5);// in case if INT2_UserFN is NULL , ensure that INT2 ISR is disabled
	}
}


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : This function is used to disable INT2 and it will also unmount the user defined function from INT2 ISR
 */
void Disable_INT2(void)
{
	GICR &=~(1<<5); //INT0 ISR is disabled
	INT2_CallBackFN=NULL; //set the function callback pointer to NULL
}


/**
 *  INT0 ISR
 */
void __vector_1 (void) __attribute__ ((signal,used,interrupt));//disable optimization for this function , define it as external interrupt
void __vector_1 (void) //INT0 interrupt vector no
{
	INT0_CallBackFN(); //execute the user's function when the INT0 triggers
}


/**
 *  INT1 ISR
 */
void __vector_2 (void) __attribute__ ((signal,used,interrupt));//disable optimization for this function, define it as external interrupt
void __vector_2 (void) //INT1 interrupt vector no
{
	INT1_CallBackFN(); //execute the user's function when the INT1 triggers
}


/**
 *  INT2 ISR
 */
void __vector_3 (void) __attribute__ ((signal,used,interrupt));//disable optimization for this function, define it as external interrupt
void __vector_3 (void) //INT2 interrupt vector no
{
	INT2_CallBackFN(); //execute the user's function when the INT2 triggers
}


