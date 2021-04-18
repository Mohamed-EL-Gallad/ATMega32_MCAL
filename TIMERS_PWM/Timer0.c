/*
 * Timer0.c
 *
 * Created on: 20/03/2021
 * Author: Mohamed_EL_Gallad
 * this C file contains the timer0 functions implementation
 */
#include "STD_types.h"
#include "Mega32_reg.h"
#include "TimersConfig.h"
#include "REG_utils.h"
#include "Timers_Interface.h"

#define T0_PRESCALER_MASK 0xF8 //exclude the prescaler bits
static void T0_OVFcounterFunc(void);

static u8 T0_OVF_Counter;
static void (*Timer0_OverFlowIntFunc)(); //pointer to a function that points to the function that shall be executed in the case of timer0 overflows
static void (*Timer0_CompMatchIntFunc)();//pointer to a function that points to the function that shall be executed in the case of timer0 compare match occurrence


/**
 *  RETURN     :VOID.
 *  PARAMETER  :VOID
 *  DESCRIPTION:This function will set timer0 functionality to operate in Normal mode and set OC0 pin operation
 *  mode according to the predefined OC0_OPMODE macro
 *  Timer0_Enable() will be required to start the timer0 module and Timer0_Stop() to disable it
 */
void Timer0_NormalModeInit(void)
{
	TCCR0 &=~(1<<3 | 1<<6); //enable normal mode operation WGM01=0 WGM00=0

	//set the OC0 pin functionality
    #if OC0_OPMODE==OC0_MODE0 //OC0 pin is disconnected
	TCCR0 &=~(1<<4 | 1<<5); //COM01=00 COM00=00

    #elif OC0_OPMODE==OC0_MODE1 //Toggle OC0 pin on overflow
	TCCR0 &=~(1<<5); //COM01=00
	TCCR0 |=(1<<4);  //COM00=01
	SetPinDIR(1, 3, 1); //set the OC0 pin data direction as output

    #endif
}


/**
 *  RETURN      :VOID
 *  PARAMETER   :OVF_UserFunction,is pointer to a user defined function that has void return and input parameters
 *  DESCRIPTION :This function will mount a user defined function to be executed on overflow interrupt
 */
void Timer0_ExecuteOnOverFlow(void (*OVF_UserFunction)(void))
{
	if(OVF_UserFunction!=NULL)
	{
	  SREG |=(1<<7); //make sure that the global interrupt is enabled
	  TIMSK |=(1<<0); //enable timer0 overflow interrupt
	  Timer0_OverFlowIntFunc=OVF_UserFunction; // assign user's function to the overflow interrupt

	}
	else
	{
	  TIMSK &=~(1<<0); //make sure that the overflow interrupt has been disabled
	}
}


/**
 * RETURN      :VOID
 * PARAMETER   :VOID
 * DESCRIPTION :this function will disable the execution of the user defined function when overflow interrupt occurs
 * it will also disable the overflow interrupt
 */
void Timer0_OVF_UserFncDisable(void)
{
		TIMSK &=~(1<<0); //disable overflow interrupt
		Timer0_OverFlowIntFunc=NULL; //set the overflow function pointer to NULL
}


/**
 *  RETURN      :VOID
 *  PARAMETER   :OverFLowsNo is a u16 variable which will contain a number of overflow to be counted down
 *  DESCRIPTION :This function can be used to halt the normal program execution for a given number of Timer0 overflows
 *  that is defined by the user
 *  the user Must set Timer0 to operate in normal mode -Timer0_NormalModeInit()- first before using this function
 */
void Timer0_CountDownOverFlows(u16 OverFLowsNo)
{
	while (OverFLowsNo)
	{
		while(!(TIFR & 0x01)); //halt till the timer0 overflow flag flips to 1

		TIFR |=(1<<0);
		OverFLowsNo--;
	}
}


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : this function is used to initiate timer0 as counter , it will count numbers of external falling or rising edges
 * applied on pin B0 , the edge trigger type can be selected by setting the TIMER0_PRESCALER MACRO to
 * either EXT_CLK_FALLING "for falling edge trigger" or  EXT_CLK_RISSING "for rising edge trigger"
 *  Timer0_Enable() will be required to start the timer0 module and Timer0_Stop() to disable it
 * CAUTION     :this function will force timer0 to operate in normal mode and it will also dictate timer0 overflow interrupt
 */
void Timer0_CounterInit(void)
{
	Timer0_NormalModeInit(); //force timer0 to operate in normal mode
	TCCR0 &=~(1<<4 | 1<<5); //COM01=00 COM00=00 disconnect OC0 pin , normal port operation
	SetPinDIR(1, 0, 0); //define pin B0 "T0"as input
	SetPinValue(1, 0, 1); //enable internal pull up resistor of pin B0
	Timer0_ExecuteOnOverFlow(&T0_OVFcounterFunc); //mount the overflow counter function to overflow interrupt
}


/**
 * RETURN      : u16 variable represents the counter value
 * PARAMETER   : VOID
 * DESCRIPTION : this function will return a value represents the number of counts of the signal applied on pin B0
 */
u16 Timer0_GetCounterValue(void)
{
	u16 NoOfCounts;
	NoOfCounts=TCNT0+(T0_OVF_Counter*256); //calculate the number of counts and taking in consideration the number of overflows
    return NoOfCounts;
}


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : this function is used to reset the counter's value as it will reset the value stored in TCNT0
 * and will also reset the number of overflows
 */
void Timer0_ResetCounter(void)
{
	TCNT0=0; //reset TCNT0 register
	T0_OVF_Counter=0; //reset the number of overflows
}


/**
 *  RETURN     :VOID
 *  PARAMETER  :VOID
 *  DESCRIPTION:This function will set timer0 functionality to operate in compare match and set OC0 pin operation
 *  mode according to the predefined OC0_OPMODE macro
 *  Timer0_Enable() will be required to start the timer0 module and Timer0_Stop() to disable it
 */
void Timer0_CTCModeInit(void)
{

	//enable CTC mode WGM01=1 WGM00=0
	TCCR0 |=(1<<3);  //WGM01=1
	TCCR0 &=~(1<<5); //WGM00=0

	//set the OC0 pin functionality
    #if OC0_OPMODE==OC0_MODE0 || OC0_OPMODE==OC0_MODE2 || OC0_OPMODE==OC0_MODE3//OC0 pin is disconnected
	TCCR0 &=~(1<<4 | 1<<5); //COM01=00 COM00=00

    #elif OC0_OPMODE==OC0_MODE1 //Toggle OC0 pin on compare match occurrence
	TCCR0 &=~(1<<5); //COM01=00
	TCCR0 |=(1<<4);  //COM00=01
	SetPinDIR(1, 3, 1); //set the OC0 pin data direction as output

    #endif
}


/**
 * RETURN      :VOID
 * PARAMETER   :CompMatch_UserFunction is pointer to a user defined function that has void return and input parameters
 * DESCRIPTION :This function will mount a user defined function to be executed on compare match interrupt
 */
void Timer0_ExecuteOnCompMatch(void (*CompMatch_UserFunction)(void))
{
	if(CompMatch_UserFunction!=NULL)
		{
		  SREG |=(1<<7); //make sure that the global interrupt is enabled
		  TIMSK |=(1<<1); //enable timer0 CompareMatch interrupt
		  Timer0_CompMatchIntFunc=CompMatch_UserFunction; // assign user's function to the CompareMatch interrupt

		}
		else
		{
		  TIMSK &=~(1<<1); //make sure that the CompareMatch interrupt has been disabled
		}
}


/**
 * RETURN      :VOID
 * PARAMETER   :VOID
 * DESCRIPTION :this function will disable the execution of the user defined function when compare match interrupt occurs
 * it will also disable the compare match interrupt
 */
void Timer0_CompMatch_UserFncDisable(void)
{
	TIMSK &=~(1<<1); //disable compare match interrupt
	Timer0_CompMatchIntFunc=NULL; //set compare match pointer function to NULL
}


/**
 *  RETURN     :VOID
 *  PARAMETER  :CompareMatchValue is a U8 variable , used to set the compare match value
 *  DESCRIPTION:This function will set the OCR0 register value to the defined value by the user
 *  the user Must set Timer0 to operate in CTC mode -Timer0_CTCModeInit()- first before using this function
 */
void Timer0_SetCompValue(u8 CompareMatchValue)
{
	OCR0=CompareMatchValue; //set a compare match value
}


/**
 *  RETURN     :VOID
 *  PARAMETER  :CycleInitValue is a U8 value , used to set a start value for a timer0 current cycle
 *  DESCRIPTION:this function is used to set a start value equal to CycleInitValue that timer0 will start count from for the current cycle
 *  this function can be used in any non PWM Mode operation mode
 */
void Timer0_CycleStartCountAt(u8 CycleInitValue)
{
	TCNT0=CycleInitValue; //set the counting start value for the current timer cycle
}


/**
 *  RETURN     :VOID
 *  PARAMETER  :VOID
 *  DESCRIPTION:This function will set timer0 functionality to operate in Fast PWM and set OC0 pin operation
 *  mode according to the predefined OC0_OPMODE macro
 *  Timer0_Enable() will be required to start the timer0 module and Timer0_Stop() to disable it
 */
void Timer0_FastPWMInit(void)
{
	TCCR0 &=~(1<<7); //set FOC0 to zero
	TCCR0 |=(1<<6 | 1<<3); //configure timer0 for FastPWM functionality WGM00=1 WGM01=1

    #if OC0_OPMODE==OC0_MODE0 || OC0_OPMODE==OC0_MODE1 //OC0 pin is disconnected
    TCCR0 &=~(1<<4 | 1<<5); //COM01=00 COM00=00

    #elif OC0_OPMODE==OC0_MODE2 //non inverted mode ,clear on compare match set on top
    TCCR0 |= (1<<5); //COM01=1
    TCCR0 &=~(1<<4); //COM00=0
    SetPinDIR(1, 3, 1); //set the OC0 pin data direction as output

    #elif OC0_OPMODE==OC0_MODE3 //inverted mode  , set on compare match clear on top
    TCCR0 |=(1<<5 | 1<<4); //COM01=1 , COM00=1
    SetPinDIR(1, 3, 1); //set the OC0 pin data direction as output

    #endif
}


/**
 *  RETURN     :VOID
 *  PARAMETER  :VOID
 *  DESCRIPTION:This function will set timer0 functionality to operate in Phase Correct PWM and set OC0 pin operation
 *  mode according to the predefined OC0_OPMODE macro
 *  Timer0_Enable() will be required to start the timer0 module and Timer0_Stop() to disable it
 */
void Timer0_PhaseCorrPWMInit(void)
{
	TCCR0 &=~(1<<7); //set FOC0 to zero
	///configure timer0 for Phase correct functionality WGM00=1 WGM01=0
	TCCR0 |=(1<<6) ; //WGM00=1
	TCCR0 &=~(1<<5); //WGM01=0

    #if OC0_OPMODE==OC0_MODE0 || OC0_OPMODE==OC0_MODE1 //OC0 pin is disconnected
    TCCR0 &=~(1<<4 | 1<<5); //COM01=00 COM00=00

    #elif OC0_OPMODE==OC0_MODE2 //non inverted mode ,clear on uprising compare match  & set on count down compare match
    TCCR0 |= (1<<5); //COM01=1
    TCCR0 &=~(1<<4); //COM00=0
    SetPinDIR(1, 3, 1); //set the OC0 pin data direction as output

    #elif OC0_OPMODE==OC0_MODE3 //inverted mode  , ,set on uprising compare match  & clear on countdown compare match
    TCCR0 |=(1<<5 | 1<<4); //COM01=1 , COM00=1
    SetPinDIR(1, 3, 1); //set the OC0 pin data direction as output

    #endif
}


/**
 * RETURN     :VOID
 * PARAMETER  :DutyCyclePercentage is a f32 variable represents the required duty cycle will have value from 0 to 100
 * DESCRIPTION:this function will be used to set the PWM duty cycle by a user defined vale equal to DutyCyclePercentage
 * this function can be used in either fast PWM or phase correct PWM.
 * either Timer0_FastPWMInit() or Timer0_PhaseCorrPWMInit() must be used before the use of this function
 */
void Timer0_SetPWM_DutyCycle(f32 DutyCyclePercentage)
{
	//first define either OC0 pin configured for inverted or non inverted mode
       #if OC0_OPMODE== OC0_MODE2  //OC0 was configured as non inverting mode
			OCR0=(255*((f32)DutyCyclePercentage/100));
       #elif OC0_OPMODE == OC0_MODE3 //OC0 was configured as inverted mode
			OCR0=255-(255*((f32)DutyCyclePercentage/100));
       #endif
}


/**
 * RETURN      :VOID
 * PARAMETER   :VOID
 * DESCRIPTION :this function will enable the timer0 module by setting the prescaler value that is defined
 * by the TIMER0_PRESCALER Macro , the use of this function is required to start the initiated timer0 functionality
 */
void Timer0_Enable(void)
{
	TCCR0|=(TCCR0 & T0_PRESCALER_MASK)|TIMER0_PRESCALER; //enable timer0 by setting a non zero prescaler value
}


/**
 * RETURN      :VOID
 * PARAMETER   :VOID
 * DESCRIPTION :this function will disable timer0 by setting the prescaler value to zero
 * it will also reset the value of TCNT0 register
 * it's a good practice to disable the timer if it's no longer needed to save power
 */
void Timer0_Stop(void)
{
	TCCR0 &=T0_PRESCALER_MASK; //set the prescaler value to zero
	TCNT0 =0x0; //reset the timer counter to zero
	Timer0_OVF_UserFncDisable(); //disable the execution of user defined function on overflow
	Timer0_CompMatch_UserFncDisable(); //disable the execution of user defined function on compare match
}


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIBTION : static function used to count the number of timer0's overflows
 */
static void T0_OVFcounterFunc(void)
{
	T0_OVF_Counter++;
}


/**
 * DESCRIPTION: the following is the overflow interrupt routine in which a user defined function will be executed
 */
void __vector_11 (void) __attribute__ ((signal,used));//disable optimization for this function
void __vector_11 (void) //timer0 overflow interrupt vector no
{
	Timer0_OverFlowIntFunc(); //execute the user's function when the overflow interrupt occurs
}

/**
 * DESCRIPTION: the following is the compare match interrupt routine in which a user defined function will be executed
 */
void __vector_10 (void) __attribute__ ((signal,used));//disable optimization for this function
void __vector_10 (void) //timer0 compare match interrupt vector no
{
	Timer0_CompMatchIntFunc(); //execute the user's function when the compare match interrupt occurs
}
