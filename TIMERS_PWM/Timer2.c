/*
 * Timer2.c
 *
 *  Created on: 17/04/2021
 *  Author: Mohamed_EL_Gallad
 *  Brief :this C file contains the timer2 functions implementation
 */
#include "STD_types.h"
#include "Mega32_reg.h"
#include "TimersConfig.h"
#include "REG_utils.h"
#include "Timers_Interface.h"

#define T2_PRESCALER_MASK 0xF8 //exclude the prescaler bits

static void (*Timer2_OverFlowIntFunc)(void)=NULL; //pointer to a function that points to the function that shall be executed in the case of timer0 overflows
static void (*Timer2_CompMatchIntFunc)(void)=NULL;//pointer to a function that points to the function that shall be executed in the case of timer0 compare match occurrence

/**
 *  RETURN     :VOID.
 *  PARAMETER  :VOID
 *  DESCRIPTION:This function will set timer2 functionality to operate in Normal mode and set OC2 pin operation
 *  mode according to the predefined OC2_OPMODE MACRO
 *  Timer2_Enable() will be required to start the timer2 module and Timer2_Stop() to disable it
 */
void Timer2_NormalModeInit(void)
{
	TCCR2 &=~(1<<3 | 1<<6); //WGM20=0 WGM21=0.
	        #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
	        while(ASSR & 0xF1);
            #endif

	 //set the OC2 operation to the required mode depending on the predefined MACRO OC2_OPMODE
     #if   OC2_OPMODE==OC2_MODE0  //OC2 PIN DICONNECTED
	 TCCR2 &=~(1<<4 | 1<<5); //COM20=0 COM21=0
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif

     #elif OC2_OPMODE==OC2_MODE1  //TOGGLE ON MATCH
	 TCCR2 |= (1<<4); //COM20=1
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
	 TCCR2 &=~(1<<5); //COM21=0
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
	 SetPinDIR(3, 7, 1); //set OC2-PB7 pin data direction as output

     #elif OC2_OPMODE==OC2_MODE2  //CLEAR ON MATCH
	 TCCR2 &=~(1<<4); //COM20=0
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
	 TCCR2 |= (1<<5); //COM21=1
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
	 SetPinDIR(3, 7, 1); //set OC2-PB7 pin data direction as output

     #elif OC2_OPMODE==OC2_MODE3  //SET ON MATCH
	 TCCR2 |=(1<<4 | 1<<5); //COM20=1 COM21=1
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
	 SetPinDIR(3, 7, 1); //set OC2-PB7 pin data direction as output

     #endif
}


/**
 *  RETURN      :VOID
 *  PARAMETER   :OVF_UserFunction,is pointer to a user defined function that has void return and input parameters
 *  DESCRIPTION :This function will mount a user defined function to be executed on overflow interrupt
 */
void Timer2_ExecuteOnOverFlow(void (*OVF_UserFunction)(void))
{
	if(OVF_UserFunction!=NULL)
	{
	  SREG  |=(1<<7); //make sure that the global interrupt is enabled
	  TIMSK |=(1<<6); //enable timer2 overflow interrupt
	  Timer2_OverFlowIntFunc=OVF_UserFunction; // assign user's function to the overflow interrupt

	}
	else
	{
	  TIMSK &=~(1<<6); //make sure that the overflow interrupt has been disabled
	}
}


/**
 * RETURN      :VOID
 * PARAMETER   :VOID
 * DESCRIPTION :this function will disable the execution of the user defined function when overflow interrupt occurs
 * it will also disable the overflow interrupt
 */
void Timer2_OVF_UserFncDisable(void)
{
	TIMSK &=~(1<<6); //disable overflow interrupt
	Timer2_OverFlowIntFunc=NULL; //set the overflow function pointer to NULL
}


/**
 *  RETURN      :VOID
 *  PARAMETER   :OverFLowsNo is a u16 variable which will contain a number of overflow to be counted down
 *  DESCRIPTION :This function can be used to halt the normal program execution for a given number of Timer2's overflows
 *  that is defined by the user
 *  the user Must set Timer2 to operate in normal mode -Timer2_NormalModeInit()- first before using this function
 */
void Timer2_CountDownOverFlows(u16 OverFLowsNo)
{
	while (OverFLowsNo)
	{
		while(!(TIFR & 0x40)); //halt till the timer2 overflow flag flips to 1

		TIFR |=(1<<6);
		OverFLowsNo--;
	}
}


/**
 *  RETURN     :VOID
 *  PARAMETER  :VOID
 *  DESCRIPTION:This function will set timer2 functionality to operate in compare match and set OC2 pin operation
 *  mode according to the predefined OC2_OPMODE MACRO
 *  Timer2_Enable() will be required to start the timer2 module and Timer2_Stop() to disable it
 */
void Timer2_CTCModeInit(void)
{
	//enable CTC mode WGM20=0  WGM21=1
	TCCR2 &=~(1<<6);  //WGM20=0
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
	TCCR2 |= (1<<3);  //WGM21=1
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif

	//set the OC2 pin functionality
    #if   OC2_OPMODE==OC2_MODE0  //OC2 PIN DICONNECTED
    TCCR2 &=~(1<<4 | 1<<5); //COM20=0 COM21=0
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif

    #elif OC2_OPMODE==OC2_MODE1  //TOGGLE ON MATCH
    TCCR2 |= (1<<4); //COM20=1
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
    TCCR2 &=~(1<<5); //COM21=0
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
    SetPinDIR(3, 7, 1); //set OC2-PB7 pin data direction as output

    #elif OC2_OPMODE==OC2_MODE2  //CLEAR ON MATCH
    TCCR2 &=~(1<<4); //COM20=0
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
    TCCR2 |= (1<<5); //COM21=1
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
    SetPinDIR(3, 7, 1); //set OC2-PB7 pin data direction as output

    #elif OC2_OPMODE==OC2_MODE3  //SET ON MATCH
    TCCR2 |=(1<<4 | 1<<5); //COM20=1 COM21=1
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
    SetPinDIR(3, 7, 1); //set OC2-PB7 pin data direction as output

    #endif
}


/**
 * RETURN      :VOID
 * PARAMETER   :CompMatch_UserFunction is pointer to a user defined function that has void return and input parameters
 * DESCRIPTION :This function will mount a user defined function to be executed on compare match interrupt
 */
void Timer2_ExecuteOnCompMatch(void (*CompMatch_UserFunction)(void))
{
	if(CompMatch_UserFunction!=NULL)
		{
		  SREG |=(1<<7); //make sure that the global interrupt is enabled
		  TIMSK |=(1<<7); //enable timer2 CompareMatch interrupt
		  Timer2_CompMatchIntFunc=CompMatch_UserFunction; // assign user's function to the CompareMatch interrupt

		}
		else
		{
		  TIMSK &=~(1<<7); //make sure that the CompareMatch interrupt has been disabled
		}
}


/**
 *  RETURN      :VOID
 *  PARAMETER   :VOID
 *  DESCRIPTION :this function will disable the execution of the user defined function when compare match interrupt occurs
 *  it will also disable the compare match interrupt
 */
void Timer2_CompMatch_UserFncDisable(void)
{
	TIMSK &=~(1<<7); //disable compare match interrupt
	Timer2_CompMatchIntFunc=NULL; //set compare match pointer function to NULL
}


/**
 *  RETURN     :VOID
 *  PARAMETER  :CompareMatchValue is a u8 variable , used to set the compare match register value
 *  DESCRIPTION:This function will set the OCR2 register value to the defined value by the user
 *  the user Must set Timer2 to operate in CTC mode -Timer2_CTCModeInit()- first before using this function
 */
void Timer2_SetCompValue(u8 CompareMatchValue)
{
	OCR2=CompareMatchValue; //set a compare match value
    #ifdef ASYNCHRONOUS_CLK //in case of asynchronous clk ,wait till OCR2 busy flag is cleared
	while (ASSR & 0xF2);
    #endif
}


/**
 *  RETURN     :VOID
 *  PARAMETER  :CycleInitValue is a u8 value , used to set a start value for a timer2 current cycle
 *  DESCRIPTION:this function is used to set a start value equal to CycleInitValue that timer2 will start count from for the current cycle
 *  this function can be used in any non PWM Mode operation mode
 */
void Timer2_CycleStartCountAt(u8 CycleInitValue)
{
	TCNT2=CycleInitValue; //set the counting start value for the current timer cycle
    #ifdef ASYNCHRONOUS_CLK //in case of asynchronous clk ,wait till TCNT2 busy flag is cleared
    while (ASSR & 0xF4);
    #endif
}


/**
 *  RETURN     :VOID
 *  PARAMETER  :VOID
 *  DESCRIPTION:This function will set timer2 functionality to operate in Fast PWM and set OC2 pin operation
 *  mode according to the predefined OC2_OPMODE MACRO
 *  Timer2_Enable() will be required to start the timer2 module and Timer2_Stop() to disable it
 */
void Timer2_FastPWMInit(void)
{
	//enable fast PWM mode WGM20=1  WGM21=1
	TCCR2 |= (1<<6 | 1<<3);  //WGM20=0 WGM21=1
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
	//set the OC2 pin functionality
    #if   OC2_OPMODE==OC2_MODE0  //OC2 PIN DICONNECTED
    TCCR2 &=~(1<<4 | 1<<5); //COM20=0 COM21=0
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif

    #elif OC2_OPMODE==OC2_MODE1  //reserved
    TCCR2 |= (1<<4); //COM20=1
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
    TCCR2 &=~(1<<5); //COM21=0
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif

    #elif OC2_OPMODE==OC2_MODE2  //Clear OC2 on compare match, set OC2 at TOP
    TCCR2 &=~(1<<4); //COM20=0
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
    TCCR2 |= (1<<5); //COM21=1.
           #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
           while(ASSR & 0xF1);
           #endif
    SetPinDIR(3, 7, 1); //set OC2-PB7 pin data direction as output

    #elif OC2_OPMODE==OC2_MODE3  //Set OC2 on compare match, clear OC2 at TOP
    TCCR2 |=(1<<4 | 1<<5); //COM20=1 COM21=1
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
    SetPinDIR(3, 7, 1); //set OC2-PB7 pin data direction as output

    #endif
}


/**
 *  RETURN     :VOID
 *  PARAMETER  :VOID
 *  DESCRIPTION:This function will set timer2 functionality to operate in Phase Correct PWM and set OC2 pin operation
 *  mode according to the predefined OC2_OPMODE MACRO
 *  Timer2_Enable() will be required to start the timer2 module and Timer2_Stop() to disable it
 */
void Timer2_PhaseCorrPWMInit(void)
{
	//enable phase correct PWM mode WGM20=1  WGM21=0
	TCCR2 |= (1<<6);  //WGM20=0
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
	TCCR2 &=~(1<<3);  //WGM21=1
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif

	//set the OC2 pin functionality
    #if   OC2_OPMODE==OC2_MODE0  //OC2 PIN DICONNECTED
    TCCR2 &=~(1<<4 | 1<<5); //COM20=0 COM21=0
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif

    #elif OC2_OPMODE==OC2_MODE1  //reserved
    TCCR2 |= (1<<4); //COM20=1
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
    TCCR2 &=~(1<<5); //COM21=0
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif

    #elif OC2_OPMODE==OC2_MODE2  //Clear OC2 on compare match when up-counting. Set OC2 on compare match when downcounting.
    TCCR2 &=~(1<<4); //COM20=0
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
    TCCR2 |= (1<<5); //COM21=1
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
    SetPinDIR(3, 7, 1); //set OC2-PB7 pin data direction as output

    #elif OC2_OPMODE==OC2_MODE3  //Set OC2 on compare match when up-counting. Clear OC2 on compare match when downcounting.
    TCCR2 |=(1<<4 | 1<<5); //COM20=1 COM21=1
            #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
            while(ASSR & 0xF1);
            #endif
    SetPinDIR(3, 7, 1); //set OC2-PB7 pin data direction as output

    #endif
}


/**
 * RETURN     :VOID
 * PARAMETER  :DutyCyclePercentage is a f32 variable represents the required duty cycle will have value from 0.0 to 100.0
 * DESCRIPTION:this function will be used to set the PWM duty cycle by a user defined vale equal to DutyCyclePercentage
 * this function can be used in either fast PWM or phase correct PWM.
 * either Timer2_FastPWMInit() or Timer2_PhaseCorrPWMInit() must be used before the use of this function
 */
void Timer2_SetPWM_DutyCycle(f32 DutyCyclePercentage)
{
	//first define either OC2 pin configured for inverted or non inverted mode
       #if OC2_OPMODE== OC2_MODE2  //OC0 was configured as non inverting mode
			OCR2=(255*((f32)DutyCyclePercentage/100));
            #ifdef ASYNCHRONOUS_CLK //in case of asynchronous clk ,wait till OCR2 busy flag is cleared
            while (ASSR & 0xF2);
            #endif

       #elif OC2_OPMODE == OC2_MODE3 //OC0 was configured as inverted mode
			OCR2=255-(255*((f32)DutyCyclePercentage/100));
            #ifdef ASYNCHRONOUS_CLK //in case of asynchronous clk ,wait till OCR2 busy flag is cleared
            while (ASSR & 0xF2);
            #endif

       #endif
}


/**
 * RETURN      :VOID
 * PARAMETER   :VOID
 * DESCRIPTION :this function will enable the timer2 module by setting the prescaler value that is defined
 * by the TIMER2_PRESCALER MACRO , the use of this function is required to start the initiated timer2 functionality
 */
void Timer2_Enable(void)
{
    #ifdef ASYNCHRONOUS_CLK //if timer2 clocked by external watch crystal
	ASSR |=(1<<3); //set AS2 bit
    #endif

	TCCR2 |=(TCCR2 & T2_PRESCALER_MASK)|TIMER2_PRESCALER; //set the prescaler value depending on the predefined MACRO TIMER2_PRESCALER
}


/**
 * RETURN      :VOID
 * PARAMETER   :VOID
 * DESCRIPTION :this function will disable timer2 by setting the prescaler value to zero
 * it will also reset the value of TCNT2 register
 * it's a good practice to disable the timer if it's no longer needed to save power
 */
void Timer2_Stop(void){
	TCCR2 &=T2_PRESCALER_MASK; //set the prescaler value to zero
             #ifdef ASYNCHRONOUS_CLK //in case of Asynchronous CLK wait till TCCR2 busy flag cleared
             while(ASSR & 0xF1);
             #endif

	TCNT2 =0x0; //reset the timer counter to zero
            #ifdef ASYNCHRONOUS_CLK //in case of asynchronous clk ,wait till TCNT2 busy flag is cleared
            while (ASSR & 0xF4);
            #endif
}

/**
 * DESCRIPTION: the following is the overflow interrupt routine in which a user defined function will be executed
 */
void __vector_5 (void) __attribute__ ((signal,used));//disable optimization for this function
void __vector_5 (void) //timer2 overflow interrupt vector no
{
	Timer2_OverFlowIntFunc(); //execute the user's function when the overflow interrupt occurs
}

/**
 * DESCRIPTION: the following is the compare match interrupt routine in which a user defined function will be executed
 */
void __vector_4 (void) __attribute__ ((signal,used));//disable optimization for this function
void __vector_4 (void) //timer2 compare match interrupt vector no
{
	Timer2_CompMatchIntFunc(); //execute the user's function when the compare match interrupt occurs
}
