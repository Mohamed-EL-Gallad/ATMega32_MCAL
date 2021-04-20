/*
 * Timer1.c
 *
 *  Created on: 15/03/2021
 *  Author: Mohamed_EL_Gallad
 */
#define F_CPU 12000000
#include "util\delay.h"

#include "STD_types.h"
#include "Mega32_reg.h"
#include "TimersConfig.h"
#include "REG_utils.h"
#include "Timers_Interface.h"

#define T1_PRESCALER_MASK 0xF8
#define HIGH_BYTE_MASK    0xFF00
#define  LOW_BYTE_MASK    0x00FF


volatile static u32 ICU_TonTicks=0,ICU_ToffTicks=0;
volatile static u16 T1_OVF_Counter;
volatile static u8 ICU_EdgeFlag=0;
         static f32 T1_CHA_DutyCycle=0;
         static f32 T1_CHB_DutyCycle=0;

static void      (*Timer1_OverFlowIntFunc)();
static void (*Timer1_CHA_CompMatchIntFunc)();
static void (*Timer1_CHB_CompMatchIntFunc)();

static void T1_OC1A_OC1B_OutputCTRL(void);
static void T1_OVFcounterFunc      (void);
static u16  T1_PrescalerDivisor    (void);


/**
 * RETURN   : VOID
 * PARAMETER: VOID
 * DESCRIPTION:This function will set timer1 functionality to operate in Normal mode and set OC1A & OC1B pins operation
 * mode according to the predefined OC1A_OPMODE & OC1B_OPMODE MACROS.
 * Timer1_Enable() will be required to start the timer1 module and Timer1_Stop() to disable it
 */
void Timer1_NormalModeInit(void)
{
	TCCR1A &=~((1<<0) | (1<<1)); // WGM10=0 WGM11=0
	TCCR1B &=~((1<<3) | (1<<4)); // WGM12=0 WGM13=0
	T1_OC1A_OC1B_OutputCTRL();
}


/**
 *  RETURN      :VOID.
 *  PARAMETER   :T1_OVFUserFunction,is pointer to a user defined function that has void return and input parameters.
 *  DESCRIPTION :This function will mount a user defined function to be executed on overflow interrupt.
 */
void Timer1_ExecuteOnOverFlow(void (*T1_OVFUserFunction)(void))
{
	if(T1_OVFUserFunction!=NULL)
			{
			  SREG  |=(1<<7); //make sure that the global interrupt is enabled
			  TIMSK |=(1<<2); //enable timer1 overflow interrupt
			  Timer1_OverFlowIntFunc=T1_OVFUserFunction;
			}
	else
			{
			 TIMSK &=~(1<<2); //disable timer1 overflow interrupt
			}
}


/**
 * RETURN      :VOID.
 * PARAMETER   :VOID.
 * DESCRIPTION :this function will disable the execution of the user defined function when overflow interrupt occurs
 * it will also disable the overflow interrupt.
 */
void Timer1_OVF_UserFnDisable(void)
{
	TIMSK &=~(1<<2); //disable Channel A compare match interrupt
	Timer1_OverFlowIntFunc=NULL;
}


/**
 *  RETURN      :VOID.
 *  PARAMETER   :T1_OverFLowsNo is a U32 variable the will have the number of overflows.
 *  DESCRIPTION :This function can be used to halt the normal program execution for a given number of Timer1 overflows
 *  that is defined by the user
 *  the user Must set Timer1 to operate in normal mode -Timer1_NormalModeInit()- first before using this function.
 */
void Timer1_CountDownOverFlows(u32 T1_OverFLowsNo)
{
	while (T1_OverFLowsNo)
	{
		while(!(TIFR & 0x04)); //halt till the timer1 overflow flag flips to 1
		TIFR |=(1<<2);
		T1_OverFLowsNo--;
	}
}


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : this function is used to initiate timer1 as counter , it will count numbers of external falling or rising edges
 * applied on pin B1 , the edge trigger type can be selected by setting the TIMER1_PRESCALER MACRO to
 * either EXT_CLK_FALLING "for falling edge trigger" OR EXT_CLK_RISSING "for rising edge trigger"
 *  Timer1_Enable() will be required to start the timer1 module and Timer1_Stop() to disable it
 * CAUTION     :this function will force timer1 to operate in normal mode and it will also dictate timer1 overflow interrupt
 */
void Timer1_CounterInit(void)
{
	Timer1_NormalModeInit(); //force timer1 to operate in normal mode
	 TCCR1A &=~((1<<6) | (1<<7)); //COM1A1=0 COM1A0=0 ,define OC1A pin to be disconnected normal pin operation
	 TCCR1A &=~((1<<4) | (1<<5));  //COM1B1=0 COM1B0=0 ,define OC1B pin to be disconnected normal pin operation
	 SetPinDIR(1, 1, 0); //define pin B1 "T1" as input
	 SetPinValue(1, 1, 1); //enable internal pull up resistor of pin B0
	 Timer1_ExecuteOnOverFlow(&T1_OVFcounterFunc); //mount the overflow counter function to overflow interrupt
}


/**
 * RETURN      : u32 variable represents the counter value
 * PARAMETER   : VOID
 * DESCRIPTION : this function will return a value represents the number of counts of the signal applied on pin B1
 */
u32 Timer1_GetCounterValue(void){
	u32 NoOfCounts;
	NoOfCounts =(u8)TCNT1L; //read the counter low byte
	NoOfCounts|=(u16)(TCNT1H<<8); //read the counter high byte
	NoOfCounts+=(T1_OVF_Counter*65536); //calculate the number of counts and taking in consideration the number of overflows
	return NoOfCounts;
}


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : this function is used to reset the counter's value as it will reset the value stored in TCNT1H & TCNT1L
 * and will also reset the number of overflows
 */
void Timer1_ResetCounter(void)
{
	TCNT1H=0x0; //set timer1 counter high byte to zero
	TCNT1L=0x0; //set timer1 counter low byte to zero
	T1_OVF_Counter=0; //reset the number of overflows
}


/**
 *  RETURN     :VOID.
 *  PARAMETER  :VOID.
 *  DESCRIPTION:This function will set timer1 functionality to operate in one of a two compare match modes that can be defined using the MACRO T1_COMPMATCH_OPMODE.
 *  if T1_COMPMATCH_OPMODE=T1_COMPMATCH_MODE1 OCR1A will be used as the timer's top value
 *  if T1_COMPMATCH_OPMODE=T1_COMPMATCH_MODE2  ICR1 will be used as the timer's top value
 *
 *  this function will also set OC1A & OC1B pins operation mode according to the predefined OC1A_OPMODE & OC1B_OPMODE MACROS.
 *  Timer1_Enable() will be required to start the timer1 module and Timer1_Stop() to disable it.
 */
void Timer1_CTCModeInit(void)
{
	switch(T1_COMPMATCH_OPMODE)
	{
	case 0: //top value is OCR1A
		TCCR1B |= (1<<3); //WGM12=1
		TCCR1B &=~(1<<4); //WGM13=0
		TCCR1A &=~((1<<0) |(1<<1)); //WGM10=0 WGM11=0
		break;
	case 1: //top value is ICR1
		TCCR1B |= (1<<3) | (1<<4); //WGM12=1 WGM13=1
		TCCR1A &=~((1<<0) |(1<<1)); //WGM10=0 WGM11=0
		break;
	default:
		//noop
		break;
	}
	T1_OC1A_OC1B_OutputCTRL(); //set the operation mode of OC1A & OC1B pins
}


/**
 * RETURN      :VOID.
 * PARAMETER   :T1_CHA_CompMatch_UserFunction is pointer to a user defined function that has void return and input parameters.
 * DESCRIPTION :This function will mount a user defined function to be executed on Timer1 channel A compare match interrupt.
 */
void Timer1_CHA_ExecuteOnCompMatch(void (*T1_CHA_CompMatch_UserFunction)(void))
{
	if(T1_CHA_CompMatch_UserFunction!=NULL)
			{
			  SREG  |=(1<<7); //make sure that the global interrupt is enabled
			  TIMSK |=(1<<4); //enable Channel A compare match interrupt
			  Timer1_CHA_CompMatchIntFunc=T1_CHA_CompMatch_UserFunction;
			}
	else
			{
			 TIMSK &=~(1<<4); //disable Channel A compare match interrupt
			}
}


/**
 * RETURN      :VOID.
 * PARAMETER   :VOID.
 * DESCRIPTION :this function will disable the execution of the user defined function when Timer1 channel A compare match interrupt occurs
 * it will also disable the compare match interrupt.
 */
void Timer1_CHACompMatch_UserFnDisable(void)
{
	TIMSK &=~(1<<4); //disable Channel A compare match interrupt
	Timer1_CHA_CompMatchIntFunc=NULL;
}


/**
 * RETURN      :VOID.
 * PARAMETER   :T1_T1_CHB_CompMatch_UserFunction is pointer to a user defined function that has void return and input parameters.
 * DESCRIPTION :This function will mount a user defined function to be executed on Timer1 channel A compare match interrupt.
 */
void Timer1_CHB_ExecuteOnCompMatch(void (*T1_CHB_CompMatch_UserFunction)(void))
{
	if(T1_CHB_CompMatch_UserFunction!=NULL)
			{
			  SREG  |=(1<<7); //make sure that the global interrupt is enabled
			  TIMSK |=(1<<3); //enable channel B compare match interrupt
			  Timer1_CHB_CompMatchIntFunc=T1_CHB_CompMatch_UserFunction;
			}
	else
			{
			 TIMSK &=~(1<<3); //disable channel B compare match interrupt
			}
}


/**
 * RETURN      :VOID.
 * PARAMETER   :VOID.
 * DESCRIPTION :this function will disable the execution of the user defined function when Timer1 channel A compare match interrupt occurs
 * it will also disable the compare match interrupt.
 */
void Timer1_CHBCompMatch_UserFnDisable(void)
{
	TIMSK &=~(1<<3); //disable channel B compare match interrupt
	Timer1_CHB_CompMatchIntFunc=NULL;
}


/**
 * RETURN     : VOID
 * PARAMETER  : T1_CTC_TopValue is a U16 variable is being used to set the timer's top value in compare match mode
 * DESCRIPTION: this function will be used to set the top value for timer1 in compare match mode
 * depending on the CTC mode being choosed using the macro T1_COMPMATCH_OPMODE ,either OCR1A or ICR1 register can be used to set the timer's top value
 * if COMPMATCH_OPMODE =T1_COMPMATCH_MODE1 ,OCR1A will be used to set the top value
 * if COMPMATCH_OPMODE =T1_COMPMATCH_MODE2 , ICR1 will be used to set the top value
 */
void Timer1_SetCompMatchTopVal (u16 T1_CTC_TopValue)
{
        #if T1_COMPMATCH_OPMODE == T1_COMPMATCH_MODE1 //top value is OCR1A
		OCR1AH =((T1_CTC_TopValue & HIGH_BYTE_MASK)>>8); //store the high byte
		OCR1AL=(T1_CTC_TopValue & LOW_BYTE_MASK);  //store the low byte

        #elif T1_COMPMATCH_OPMODE ==T1_COMPMATCH_MODE2//top value is ICR1
		ICR1H =((T1_CTC_TopValue & HIGH_BYTE_MASK)>>8);  //store the high byte
		ICR1L=(T1_CTC_TopValue & LOW_BYTE_MASK); //store the low byte
        #endif
}


/**
 * RETURN      :VOID.
 * PARAMETER   :T1_CHA_CompareMatchValue this U16 variable will set the compare match value for timer1 channel A.
 * DESCRIPTION :this function is used to set the compare match value for timer1 channel A "OCR1A=T1_CHA_CompareMatchValue"
 * if TI_COMPMATCH_MODE2 is being used as timer1 compare match mode
 * CAUTOION    :this function will have no effect if OCR1A is used to set timer1's top value "T1_COMPMATCH_OPMODE=T1_COMPMATCH_MODE2"
 */
void Timer1_CHA_SetCompValue(u16 T1_CHA_CompareMatchValue)
{
#if T1_COMPMATCH_OPMODE==T1_COMPMATCH_MODE2 //in case of OCR1A is used to set the TCNT1 top value this function will have no effect if used
	    OCR1AH =((T1_CHA_CompareMatchValue & HIGH_BYTE_MASK)>>8); //store the passed variable's high byte in OCR1AH
		OCR1AL=(T1_CHA_CompareMatchValue & LOW_BYTE_MASK);  //store the passed variable's low byte in OCR1AH
#else
#warning "the use of this function will have no effect as OCR1A used to set the timer's top value"
#endif
}


/**
 * RETURN      :VOID.
 * PARAMETER   :T1_CHB_CompareMatchValue this U16 variable will set the compare match value for timer1 channel B.
 * DESCRIPTION :this function is used to set the compare match value for timer1 channel B "OCR1B=T1_CHB_CompareMatchValue"
 * CAUTOION    :T1_CHB_CompareMatchValue Must have a value less than or equal OCR1A in case it's being used to set the timer's top value
 * where "T1_COMPMATCH_OPMODE=T1_COMPMATCH_MODE2"
 * OR a value less than or equal ICR1 in case it's being used to set the timer's top value "T1_COMPMATCH_OPMODE=T1_COMPMATCH_MODE1"
 * otherwise no compare match will occur
 */
void Timer1_CHB_SetCompValue(u16 T1_CHB_CompareMatchValue)
{
	OCR1BH =((T1_CHB_CompareMatchValue & HIGH_BYTE_MASK)>>8); //store the passed variable's high byte in OCR1BH
	OCR1BL=(T1_CHB_CompareMatchValue & LOW_BYTE_MASK); //store the passed variable's low byte in OCR1BH

}


/**
 *  RETURN     :VOID.
 *  PARAMETER  :T1_CycleInitValue is a U16 value , used to set a start value for a timer1 current cycle.
 *  DESCRIPTION:this function is used to set a start value equal to T1_CycleInitValue that timer1 will start count from for the current cycle
 *  this function can be used in any non PWM Mode operation mode.
 */
void Timer1_CycleStartCountAt(u16 T1_CycleInitValue)
{
	TCNT1H =((u8)(T1_CycleInitValue & HIGH_BYTE_MASK)); //store the high byte of T1_CycleInitValue in TCNT1H register
	TCNT1L =((u8)(T1_CycleInitValue & LOW_BYTE_MASK)); //store the low byte of T1_CycleInitValue in TCNT1L register
}


/**
 * RETURN      :VOID
 * PARAMETER   :VOID
 * DESCRIPTION :this function will initiate the input capture functionality
 * CAUTION     :this function will dictate timer1 operation and force it to work in normal mode, disconnect OC1A & OC1B pins and override any function mounted on overflow interrupt
 * so in case the input capture functionality is being initiated user MUST avoid using timer1 in any other application.
 */
void Timer1_InputCaptureInit(void)
{
    	Timer1_NormalModeInit(); //force timer1 to run in normal mode

     	//disconnect both OC1A & OC1B pins
       #if OC1A_OPMODE !=OC1A_MODE0 || OC1B_OPMODE !=OC1B_MODE0
		TCCR1A &=~((1<<4) | (1<<5));  //COM1B1=0 COM1B0=0 ,ensure that OC1B pin is disconnected .
		TCCR1A &=~((1<<6) | (1<<7));  //COM1A1=0 COM1A0=0 ,ensure that OC1A pin is disconnected .
       #endif


	    //noise cancellation enable\disable
        #if NOISE_CANCELLATION == ENABLE
	    TCCR1B |=(1<<7); //set ICNC1 to 1 , enable noise cancellation

        #elif NOISE_CANCELLATION == DISABLE
	    TCCR1B &=~(1<<7);//clear ICNC1 , disable noise cancellation
        #endif


	    //trigger source selection
        #if TRIGGER_SOURCE == TRIGGER_SRC_ICP1  //trigger source is ICP1 pin
		ACSR &=~(1<<2); //set trigger source to be ICP pin
		TIFR |=(1<<5); //clear the input capture flag to avoid accidental trigger after changing the trigger source

        #elif TRIGGER_SOURCE == TRIGGER_SRC_ACO //trigger source is ASIC pin "analog comparator"
		ACSR |=(1<<2);
		TIFR |=(1<<5); //clear the input capture flag to avoid accidental trigger after changing the trigger source

        #endif
}


/**
 * RETURN    : VOID
 * PARAMETER : TonTime is a pointer to F32 variable that will contain the measured TON time in microseconds
 *           DutyCycle is a pointer to F32 variable that will contain the measured duty cycle
 *                Freq is a pointer to U16 variable that will contain the measured frequency in HZ
 * DESCRIPTION:this function is used to measure the TonTime , DutyCycle and the frequency of an external signal on ICP1 pin
 * the execution of this function will return the data of single sample of the external signal,
 * if "X" samples are needed then revoking this function "X" times will be required
 *
 * this function handles the use of Timer1_Enble() and Timer1_Stop() by it's own ,
 * DON'T USE THESE FUNCTIONS WITH THE ICU FUNCTIONALITY
 */

void Timer1_ICUGetEventData(f32 *TonTime , f32 *DutyCycle ,u16 *Freq)
{
	u32 Timer1_OP_CLK;
	Timer1_OP_CLK=CPU_FREQ/T1_PrescalerDivisor();
	TIFR |=(1<<5); //reset input capture flag to avoid accidental interrupt

	Timer1_ExecuteOnOverFlow(&T1_OVFcounterFunc); //mount overflows counter function
    Timer1_Enable(); //enable timer1


	SREG  |=(1<<7); //make sure that the global interrupt is enabled
	TIMSK |=(1<<5); //enable timer1 input capture event interrupt

	while(ICU_TonTicks==0 || ICU_ToffTicks==0); //wait until both ton and toff ticks are being calculated

	TIMSK &=~(1<<5); //disable timer1 input capture event interrupt
	Timer1_Stop();
	Timer1_OVF_UserFnDisable();


	*TonTime=ICU_TonTicks*((f32)1/Timer1_OP_CLK)*1000000; //calculate TonTime in microseconds

    if((ICU_TonTicks+ICU_ToffTicks)!=0) //avoid division by zero
	{
		*DutyCycle=((float)ICU_TonTicks/(ICU_TonTicks+ICU_ToffTicks))*100;
	}

	else
	{
		*DutyCycle=0;
	}

	*Freq=Timer1_OP_CLK/(ICU_ToffTicks+ICU_TonTicks);

	ICU_TonTicks=0; //reset the Ton ticks of the last captured event
	ICU_ToffTicks=0; //reset the Toff ticks of the last captured event
	ICU_EdgeFlag=0;  //reset the edge flag to ensure the ICU ISR to be executed according to the required rhythm
	TCCR1B &=~(1<<6); //set the ICU trigger edge to default value 0
}


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION :  This function will set timer1 functionality to operate in one of the 5 Fast PWM modes that are available
 * depending on the predefined macro T1_FASTPWM_OPMODE and it will also set OC1A and OCA1B  pins operation
 *  mode depending on the predefined OC1A_OPMODE & OC1B_OPMODE MACROS
 *  Timer1_Enable() will be required to start the timer1 module and Timer1_Stop() to disable it.
 */
void Timer1_FastPWMInit(void){
	//confirm that either T1_FASTPWM_OPMODE or TI_PWM_PHASECORR_OPMODE MACROS is set to an operating mode and the other one set to a disabled macro
#if T1_FASTPWM_OPMODE!=T1_FASTPWM_DISABLED && TI_PWM_PHASECORR_OPMODE!=T1_PHASECORR_DISABLED
#error "both T1_FASTPWM_OPMODE & TI_PWM_PHASECORR_OPMODE MACROS are defined to an operating mode , the output will be undefined"
#endif

	TCCR1A &=~((1<<3) | (1<<2)); //reset Force Output Compare for Channel A & Channel B

	//setting the desired fast PWM operation mode
    #if   T1_FASTPWM_OPMODE ==T1_FASTPWM_MODE1 //8bits mode ,top value is 0x00FF
	TCCR1A |= (1<<0); //WGM10=1
	TCCR1A &=~(1<<1); //WGM11=0
	TCCR1B |= (1<<3); //WGM12=1
	TCCR1B &=~(1<<4); //WGM13=0

    #elif T1_FASTPWM_OPMODE ==T1_FASTPWM_MODE2 //9bits mode ,top value is 0x01FF
	TCCR1A &=~(1<<0); //WGM10=0
	TCCR1A |= (1<<1); //WGM11=1
	TCCR1B |= (1<<3); //WGM12=1
	TCCR1B &=~(1<<4); //WGM13=0

    #elif T1_FASTPWM_OPMODE ==T1_FASTPWM_MODE3 //10bits mode,top value is 0x03FF
	TCCR1A |= (1<<0)|(1<<1); //WGM10=1 WGM11=1
	TCCR1B |= (1<<3);        //WGM12=1
	TCCR1B &=~(1<<4);        //WGM13=0

    #elif T1_FASTPWM_OPMODE ==T1_FASTPWM_MODE4 //top value is set by ICR1 register
    TCCR1A &=~(1<<0);        //WGM10=0
	TCCR1A |= (1<<1);        //WGM11=1
	TCCR1B |= (1<<3)|(1<<4); //WGM12=1 WGM13=1

    #elif T1_FASTPWM_OPMODE ==T1_FASTPWM_MODE5 //top value is set by OCR1A register
	TCCR1A=(1<<0)|(1<<1); //WGM10=1 WGM11=1
	TCCR1B=(1<<3)|(1<<4); //WGM12=1 WGM13=1
    #endif

	T1_OC1A_OC1B_OutputCTRL(); //define the operation mode of OC1A & OC1B according to the predefined MACROS OC1A_OPMODE & OC1B_OPMODE
}


/**
 * RETURN     :VOID.
 * PARAMETER  :VOID.
 * DESCRIPTION:This function will set timer1 functionality to operate in one of 7 available modes of either Phase Correct PWM or phase and frequency correct
 * depending on the predefined macro TI_PWM_PHASECORR_OPMODE and it will also set OC1A and OCA1B  pins operation
 *  mode depending on the predefined OC1A_OPMODE & OC1B_OPMODE MACROS
 *  Timer1_Enable() will be required to start the timer1 module and Timer0_Stop() to disable it.
 */
void Timer1_PhaseCorrPWMInit(void)
{
	//confirm that either T1_FASTPWM_OPMODE or TI_PWM_PHASECORR_OPMODE MACROS is set to an operating mode and the other one set to a disabled macro
#if T1_FASTPWM_OPMODE!=T1_FASTPWM_DISABLED && TI_PWM_PHASECORR_OPMODE!=T1_PHASECORR_DISABLED
#error "both T1_FASTPWM_OPMODE & TI_PWM_PHASECORR_OPMODE MACROS is defined to an operating mode , the output will be undefined"
#endif

	TCCR1A &=~((1<<3) | (1<<2)); //reset Force Output Compare for Channel A & Channel B

    #if   TI_PWM_PHASECORR_OPMODE == T1_PHASECORR_MODE1 //phase correct PWM ,top value is 8bits
	     TCCR1A |= (1<<0);  //WGM10=1
	     TCCR1A &=~(1<<1);  //WGM11=0
	     TCCR1B &=~((1<<3) | (1<<4)); //WGM12=0 WGM13=0

    #elif TI_PWM_PHASECORR_OPMODE == T1_PHASECORR_MODE2 //phase correct PWM ,top value is 9bits
	     TCCR1A &=~(1<<0);  //WGM10=0
	     TCCR1A |=(1<<1);  //WGM11=1
	     TCCR1B &=~((1<<3) | (1<<4)); //WGM12=0 WGM13=0

    #elif TI_PWM_PHASECORR_OPMODE == T1_PHASECORR_MODE3 //phase correct PWM ,top value is 10bits
	     TCCR1A |=(1<<1) | (1<<0);  //WGM10=1 WGM11=1
	     TCCR1B &=~((1<<3) | (1<<4)); //WGM12=0 WGM13=0

    #elif TI_PWM_PHASECORR_OPMODE == T1_PHASECORR_MODE4 //phase correct PWM ,top value is defined by ICR1 register
	     TCCR1A &=~(1<<0);  //WGM10=0
		 TCCR1A |=(1<<1);   //WGM11=1
	   	 TCCR1B &=~(1<<3);  //WGM12=0
	   	 TCCR1B |=(1<<4);   //WGM13=1

    #elif TI_PWM_PHASECORR_OPMODE == T1_PHASECORR_MODE5 //phase correct PWM ,top value is defined by OCR1A register
	   	 TCCR1A |=(1<<0) | (1<<1); //WGM10=1 WGM11=1
	   	 TCCR1B &=~(1<<3);  //WGM12=0
	   	 TCCR1B |=(1<<4);   //WGM13=1

    #elif TI_PWM_PHASECORR_OPMODE == T1_PHASE_FREQ_CORR_MODE1 //phase and frequency correct top value is defined by ICR1
	     TCCR1A &=~((1<<0) | (1<<1)); //WGM10=0 WGM11=0
	   	 TCCR1B &=~(1<<3); //WGM12=0
	   	 TCCR1B |= (1<<4); //WGM13=1

    #elif TI_PWM_PHASECORR_OPMODE == T1_PHASE_FREQ_CORR_MODE2 //phase and frequency correct top value is defined by OCR1A
	     TCCR1A |=(1<<0);  //WGM10=1
		 TCCR1A &=~(1<<1); //WGM11=0
	   	 TCCR1B &=~(1<<3); //WGM12=0
	   	 TCCR1B |=(1<<4);  //WGM13=1

    #endif

	   	T1_OC1A_OC1B_OutputCTRL(); //define the operation mode of OC1A & OC1B according to the predefined MACROS OC1A_OPMODE & OC1B_OPMODE
}


/**
 * RETURN      : VOID
 * PARAMETER   : CHA_DutyCyclePercentage is f32 variable that represents the required duty cycle for channel A and must have a value between 0.0 -> 100.0
 * DESCRIPTION : this function is used to set the output's duty cycle of channel A in either fast PWM modes or Phase correct PWM modes
 */
void Timer1_CHA_SetPWM_DutyCycle(f32 CHA_DutyCyclePercentage)
{

 T1_CHA_DutyCycle=CHA_DutyCyclePercentage;

#if   OC1A_OPMODE== OC1A_MODE2 //non inverting mode

    #if T1_FASTPWM_OPMODE==T1_FASTPWM_MODE1  || TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE1//8bits mode , MAX counts in this mode is 256 counts "0->255"
         OCR1AH=0; //store zero in the OCRA high byte
         OCR1AL=(255*((f32)CHA_DutyCyclePercentage/100)); // store the value that will achieve the required duty cycle

    #elif T1_FASTPWM_OPMODE==T1_FASTPWM_MODE2 || TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE2//9bits mode, MAX counts in this mode is 512 counts "0->511"
         u16 CHA_RequiredCounts;
         CHA_RequiredCounts=(511*((f32)CHA_DutyCyclePercentage/100)); // store the value that will achieve the required duty cycle
         OCR1AH = (CHA_RequiredCounts & HIGH_BYTE_MASK)>>8;
         OCR1AL = (CHA_RequiredCounts & LOW_BYTE_MASK);

    #elif T1_FASTPWM_OPMODE==T1_FASTPWM_MODE3 || TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE3//10bits mode , MAX counts in this mode is 1024 counts "0->1023"
         u16 CHA_RequiredCounts;
         CHA_RequiredCounts=(1023*((f32)CHA_DutyCyclePercentage/100)); // store the value that will achieve the required duty cycle
         OCR1AH = (CHA_RequiredCounts & HIGH_BYTE_MASK)>>8; //store the high byte
         OCR1AL = (CHA_RequiredCounts & LOW_BYTE_MASK); //store the low byte

    #elif T1_FASTPWM_OPMODE==T1_FASTPWM_MODE4 || TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE4 || TI_PWM_PHASECORR_OPMODE==T1_PHASE_FREQ_CORR_MODE1//Top value is defined by ICR1 register
          u16 CHA_TopValue,CHA_RequiredCounts;
          CHA_TopValue =ICR1L;  //read the low byte of the ICR1L
          CHA_TopValue|=ICR1H <<8; //read the high byte of the ICR1H
          CHA_RequiredCounts=((CHA_TopValue-1)*((f32)CHA_DutyCyclePercentage/100));// store the value that will achieve the required duty cycle
          OCR1AH = (CHA_RequiredCounts & HIGH_BYTE_MASK)>>8; //store the high byte
          OCR1AL = (CHA_RequiredCounts & LOW_BYTE_MASK); //store the low byte

          #endif

#elif OC1A_OPMODE== OC1A_MODE3 //inverting mode

    #if T1_FASTPWM_OPMODE==T1_FASTPWM_MODE1  || TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE1 //8bits mode , MAX counts in this mode is 256 counts "0->255"
        OCR1AH=0; //store zero in the OCRA high byte
        OCR1AL=255-(255*((f32)CHA_DutyCyclePercentage/100)); // store the value that will achieve the required duty cycle

    #elif T1_FASTPWM_OPMODE==T1_FASTPWM_MODE2 || TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE2 //9bits mode, MAX counts in this mode is 512 counts "0->511"
         u16 CHA_RequiredCounts;
         CHA_RequiredCounts=511-(511*((f32)CHA_DutyCyclePercentage/100)); // store the value that will achieve the required duty cycle
         OCR1AH = (CHA_RequiredCounts & HIGH_BYTE_MASK)>>8;
         OCR1AL = (CHA_RequiredCounts & LOW_BYTE_MASK);

    #elif T1_FASTPWM_OPMODE==T1_FASTPWM_MODE3 || TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE3//10bits mode , MAX counts in this mode is 1024 counts "0->1023"
         u16 CHA_RequiredCounts;
         CHA_RequiredCounts=1023-(1023*((f32)CHA_DutyCyclePercentage/100)); // store the value that will achieve the required duty cycle
         OCR1AH = (CHA_RequiredCounts & HIGH_BYTE_MASK)>>8; //store the high byte
         OCR1AL = (CHA_RequiredCounts & LOW_BYTE_MASK); //store the low byte.

    #elif T1_FASTPWM_OPMODE==T1_FASTPWM_MODE4 || TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE4 || TI_PWM_PHASECORR_OPMODE==T1_PHASE_FREQ_CORR_MODE1 //Top value is defined by ICR1 register
         u16 CHA_TopValue,CHA_RequiredCounts;
         CHA_TopValue =ICR1L;  //read the low byte of the ICR1L
         CHA_TopValue|=ICR1H <<8; //read the high byte of the ICR1H
         CHA_RequiredCounts=(CHA_TopValue-1)-((CHA_TopValue-1)*((f32)CHA_DutyCyclePercentage/100));// store the value that will achieve the required duty cycle
         OCR1AH = (CHA_RequiredCounts & HIGH_BYTE_MASK)>>8; //store the high byte
         OCR1AL = (CHA_RequiredCounts & LOW_BYTE_MASK); //store the low byte

         #endif //inner #if
#endif //outer #if
}


/**
 * RETURN      : VOID
 * PARAMETER   : CHB_DutyCyclePercentage is f32 variable that represents the required duty cycle for channel B and must have a value between 0.0 -> 100.0
 * DESCRIPTION : this function is used to set the output's duty cycle of channel B in either fast PWM modes or Phase correct PWM modes
 */
void Timer1_CHB_SetPWM_DutyCycle(f32 CHB_DutyCyclePercentage)
{

 T1_CHB_DutyCycle=CHB_DutyCyclePercentage;

#if   OC1B_OPMODE== OC1B_MODE2 //non inverting mode

    #if T1_FASTPWM_OPMODE==T1_FASTPWM_MODE1  || TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE1  //8bits mode , MAX counts in this mode is 256 counts "0->255"
         OCR1BH=0; //store zero in the OCRA high byte
         OCR1BL=(255*((f32)CHB_DutyCyclePercentage/100)); // store the value that will achieve the required duty cycle

    #elif T1_FASTPWM_OPMODE==T1_FASTPWM_MODE2 || TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE2  //9bits mode, MAX counts in this mode is 512 counts "0->511"
         u16 CHB_RequiredCounts;
         CHB_RequiredCounts=(511*((f32)CHB_DutyCyclePercentage/100)); // store the value that will achieve the required duty cycle
         OCR1BH = (CHB_RequiredCounts & HIGH_BYTE_MASK)>>8;
         OCR1BL = (CHB_RequiredCounts & LOW_BYTE_MASK);

    #elif T1_FASTPWM_OPMODE==T1_FASTPWM_MODE3 || TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE3//10bits mode , MAX counts in this mode is 1024 counts "0->1023"
         u16 CHB_RequiredCounts;
         CHB_RequiredCounts=(1023*((f32)CHB_DutyCyclePercentage/100)); // store the value that will achieve the required duty cycle
         OCR1BH = (CHB_RequiredCounts & HIGH_BYTE_MASK)>>8; //store the high byte
         OCR1BL = (CHB_RequiredCounts & LOW_BYTE_MASK); //store the low byte

    #elif T1_FASTPWM_OPMODE==T1_FASTPWM_MODE4 || TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE4 || TI_PWM_PHASECORR_OPMODE==T1_PHASE_FREQ_CORR_MODE1  //Top value is defined by ICR1 register
         u16 CHB_TopValue,CHB_RequiredCounts;
         CHB_TopValue =ICR1L;  //read the low byte of the ICR1L
         CHB_TopValue |=ICR1H <<8; //read the high byte of the ICR1H
         CHB_RequiredCounts=((CHB_TopValue-1)*((f32)CHB_DutyCyclePercentage/100));// store the value that will achieve the required duty cycle
         OCR1BH = (CHB_RequiredCounts & HIGH_BYTE_MASK)>>8; //store the high byte
         OCR1BL = (CHB_RequiredCounts & LOW_BYTE_MASK); //store the low byte

    #elif T1_FASTPWM_OPMODE==T1_FASTPWM_MODE5 || TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE5 || TI_PWM_PHASECORR_OPMODE==T1_PHASE_FREQ_CORR_MODE2 //Top value is defined by OCR1A register
         u16 CHB_TopValue,CHB_RequiredCounts;
         CHB_TopValue =OCR1AL;  //read the low byte of the ICR1L
         CHB_TopValue |=OCR1AH <<8; //read the high byte of the ICR1H
         CHB_RequiredCounts=((CHB_TopValue-1)*((f32)CHB_DutyCyclePercentage/100));// store the value that will achieve the required duty cycle
         OCR1BH = (CHB_RequiredCounts & HIGH_BYTE_MASK)>>8; //store the high byte
         OCR1BL = (CHB_RequiredCounts & LOW_BYTE_MASK); //store the low byte

         #endif //inner #if


#elif OC1A_OPMODE== OC1A_MODE3 //inverting mode

    #if T1_FASTPWM_OPMODE==T1_FASTPWM_MODE1  || TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE1   //8bits mode , MAX counts in this mode is 256 counts "0->255"
        OCR1BH=0; //store zero in the OCRA high byte
        OCR1BL=255-(255*((f32)CHB_DutyCyclePercentage/100)); // store the value that will achieve the required duty cycle

    #elif T1_FASTPWM_OPMODE==T1_FASTPWM_MODE2 || TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE2   //9bits mode, MAX counts in this mode is 512 counts "0->511"
         u16 CHB_RequiredCounts;
         CHB_RequiredCounts=511-(511*((f32)CHB_DutyCyclePercentage/100)); // store the value that will achieve the required duty cycle
         OCR1BH = (CHB_RequiredCounts & HIGH_BYTE_MASK)>>8;
         OCR1BL = (CHB_RequiredCounts & LOW_BYTE_MASK);

    #elif T1_FASTPWM_OPMODE==T1_FASTPWM_MODE3 || TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE3//10bits mode , MAX counts in this mode is 1024 counts "0->1023"
         u16 CHB_RequiredCounts;
         CHB_RequiredCounts=1023-(1023*((f32)CHB_DutyCyclePercentage/100)); // store the value that will achieve the required duty cycle
         OCR1BH = (CHB_RequiredCounts & HIGH_BYTE_MASK)>>8; //store the high byte
         OCR1BL = (CHB_RequiredCounts & LOW_BYTE_MASK); //store the low byte

    #elif T1_FASTPWM_OPMODE==T1_FASTPWM_MODE4 || TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE4 || TI_PWM_PHASECORR_OPMODE==T1_PHASE_FREQ_CORR_MODE1 //Top value is defined by ICR1 register
         u16 CHB_TopValue,CHB_RequiredCounts;
         CHB_TopValue =ICR1L;  //read the low byte of the ICR1L
         CHB_TopValue |=ICR1H <<8; //read the high byte of the ICR1H
         CHB_RequiredCounts=(CHB_TopValue-1)-((CHB_TopValue-1)*((f32)CHB_DutyCyclePercentage/100));// store the value that will achieve the required duty cycle
         OCR1BH = (CHB_RequiredCounts & HIGH_BYTE_MASK)>>8; //store the high byte
         OCR1BL = (CHB_RequiredCounts & LOW_BYTE_MASK); //store the low byte

    #elif T1_FASTPWM_OPMODE==T1_FASTPWM_MODE5  || TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE5 || TI_PWM_PHASECORR_OPMODE==T1_PHASE_FREQ_CORR_MODE2//Top value is defined by OCR1A register
         u16 CHB_TopValue,CHB_RequiredCounts;
         CHB_TopValue =OCR1AL;  //read the low byte of the ICR1L
         CHB_TopValue |=OCR1AH <<8; //read the high byte of the ICR1H
         CHB_RequiredCounts=(CHB_TopValue-1)-((CHB_TopValue-1)*((f32)CHB_DutyCyclePercentage/100));// store the value that will achieve the required duty cycle
         OCR1BH = (CHB_RequiredCounts & HIGH_BYTE_MASK)>>8; //store the high byte
         OCR1BL = (CHB_RequiredCounts & LOW_BYTE_MASK); //store the low byte

         #endif //inner #if

#endif //outer #if
}


/**
 * RETURN      : VOID
 * PARAMETER   : Frequency is u16 variable that represents the required frequency for channel A and channel B
 * DESCRIPTION : this function is used to set the required frequency for both channel A and channel B in any of the following modes:
 * T1_PHASECORR_MODE4
 * T1_PHASECORR_MODE5
 * T1_PHASE_FREQ_CORR_MODE1
 * T1_PHASE_FREQ_CORR_MODE2
 * T1_FASTPWM_MODE4
 * T1_FASTPWM_MODE5
 * and will have no effect if being used with any other PWM modes
 * NOTE        :changing the frequency from a value to another during the operation will adjust the duty cycle automatically to match the predefined value before changing the frequency
 */
void Timer1_SetPWN_Freq(u16 Frequency){
	u16 RequiredCounts=0,PrescalerDIV=0;
	PrescalerDIV=T1_PrescalerDivisor();

#if   T1_FASTPWM_OPMODE ==T1_FASTPWM_MODE4 //timer's top value defined by ICR1
	   RequiredCounts=(CPU_FREQ/(((u32)PrescalerDIV) * Frequency))-1; //calculate the required top value to be stored in ICR1 to achieve the required frequency
	   ICR1H= (RequiredCounts & HIGH_BYTE_MASK)>>8; //store the value of the high byte
	   ICR1L= (RequiredCounts & LOW_BYTE_MASK); //store the value of the low byte

#elif T1_FASTPWM_OPMODE	== T1_FASTPWM_MODE5 //timer's top value defined by OCR1A
	   RequiredCounts=(CPU_FREQ/(((u32)PrescalerDIV) * Frequency))-1; //calculate the required top value to be stored OCR1A to achieve the required frequency
	   OCR1AH = (RequiredCounts & HIGH_BYTE_MASK)>>8;//store the value of the high byte
	   OCR1AL = (RequiredCounts & LOW_BYTE_MASK);//store the value of the low byte

#elif TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE4 || TI_PWM_PHASECORR_OPMODE==T1_PHASE_FREQ_CORR_MODE1
	   RequiredCounts=(CPU_FREQ/(((u32)PrescalerDIV) * Frequency * 2))-1; //calculate the required top value to be stored in ICR1 to achieve the required frequency
	   ICR1H = (RequiredCounts & HIGH_BYTE_MASK)>>8;//store the value of the high byte
	   ICR1L = (RequiredCounts & LOW_BYTE_MASK);//store the value of the low byte

#elif TI_PWM_PHASECORR_OPMODE==T1_PHASECORR_MODE5 || TI_PWM_PHASECORR_OPMODE==T1_PHASE_FREQ_CORR_MODE2
	   RequiredCounts=(CPU_FREQ/(((u32)PrescalerDIV) * Frequency * 2))-1; //calculate the required top value to be stored OCR1A to achieve the required frequency
	   OCR1AH = (RequiredCounts & HIGH_BYTE_MASK)>>8; //store the value of the high byte
	   OCR1AL = (RequiredCounts & LOW_BYTE_MASK); //store the value of the low byte

#endif

	Timer1_CHB_SetPWM_DutyCycle(T1_CHB_DutyCycle); //re-calculate the duty cycle for Channel B after the frequency being changed
	Timer1_CHA_SetPWM_DutyCycle(T1_CHA_DutyCycle); //re-calculate the duty cycle for Channel A after the frequency being changed
}


/**
 * RETURN      :VOID.
 * PARAMETER   :VOID.
 * DESCRIPTION :this function will enable the timer1 module by setting the prescaler value that is defined
 * by the TIMER1_PRESCALER Macro , the use of this function is required to start the initiated timer1 functionality.+
 * CAUTION     :this function must not be used while the input capture mode is being initiated ,
 * the timer will be enabled when Timer1_ICUGetEventData() is being provoked and will be disabled after the function's execution
 */
void Timer1_Enable(void){
	TCCR1B |=(TCCR1B & T1_PRESCALER_MASK)|TIMER1_PRESCALER; //set the operational prescaler to initiate the timer
}


/**
 * RETURN      :VOID.
 * PARAMETER   :VOID.
 * DESCRIPTION :this function will disable timer1 by setting the prescaler value to zero
 * it will also reset the value of TCNT1 register
 * it's a good practice to disable the timer if it's no longer needed to save power.
 * CAUTION     :this function must not be used while the input capture mode is being initiated ,
 * the timer will be enabled when Timer1_ICUGetEventData() is being provoked and will be disabled after the function's execution
 */
void Timer1_Stop(void)
{
	TCCR1B &=T1_PRESCALER_MASK; //set the prescaler value to zero, stop the timer
	TCNT1H=0x0; //set timer1 counter high byte to zero
	TCNT1L=0x0; //set timer1 counter low byte to zero
	Timer1_OVF_UserFnDisable();
}




/**
 * RETURN     : VOID
 * PARAMETER  : VOID
 * DESCRIPTION: static function to calculate the number of timer1's overflows
 */
static void T1_OVFcounterFunc(void)
{
	T1_OVF_Counter++;
}


/**
 * RETURN : u16 represents the prescaler  value
 * PARAMETER: VOID
 * DESCRIPTION : static function that is being used to calculate the prescaler value
 */
static u16 T1_PrescalerDivisor(void)
{
	u16 Divisor=0;
#if TIMER1_PRESCALER==T1_NO_PRESCALER
	Divisor =1;
#elif TIMER1_PRESCALER==T1_CLK_DIV_BY8
	Divisor =8;
#elif TIMER1_PRESCALER==T1_CLK_DIV_BY64
	Divisor =64;
#elif TIMER1_PRESCALER==T1_CLK_DIV_BY265
	Divisor =265;
#elif TIMER1_PRESCALER==T1_CLK_DIV_BY1024
	Divisor =1024;
#endif

	return Divisor;
}

/**
 * RETURN     : VOID
 * PARAMETER  : VOID
 * DESCRIPTION: static function to set the operation mode of OC1A and OC1B pins according to predefined MACROS
 * OC1A_OPMODE & OC1B_OPMODE
 */
static void T1_OC1A_OC1B_OutputCTRL(void)
{
	     //define the operation mode for OC1A pin
		   #if OC1A_OPMODE==OC1A_MODE0    //OC1A_MODE0
		 	   TCCR1A &=~((1<<6) | (1<<7));  //COM1A1=0 COM1A0=0

           #elif OC1A_OPMODE==OC1A_MODE1  //OC1A_MODE1
		        TCCR1A |=(1<<6);   //COM1A0=1
			    TCCR1A &=~(1<<7);  //COM1A1=0
			    SetPinDIR(3,5,1); //define PD5 pin as output

           #elif OC1A_OPMODE==OC1A_MODE2 //OC1A_MODE2
		 	   	TCCR1A &=~(1<<6);   //COM1A0=0
		 		TCCR1A |=(1<<7);    //COM1A1=1
		 		SetPinDIR(3,5,1); //define PD5 pin as output

           #else  //OC1A_MODE3
		 		TCCR1A |=((1<<6) | (1<<7)); //COM1A1=1 COM1A0=1
		 		SetPinDIR(3,5,1); //define PD5 pin as output
           #endif


		   //define the operation mode for OC1B pin
           #if OC1B_OPMODE==OC1B_MODE0  //OC1B_MODE0
				TCCR1A &=~((1<<4) | (1<<5));  //COM1B1=0 COM1B0=0

           #elif OC1B_OPMODE==OC1B_MODE1//OC1B_MODE1
				TCCR1A |=(1<<4);   //COM1B0=1
				TCCR1A &=~(1<<5);  //COM1B1=0
				SetPinDIR(3,4,1); //define PD4 pin as output

           #elif OC1B_OPMODE==OC1B_MODE2 //OC1B_MODE2
				TCCR1A &=~(1<<4);   //COM1B0=0
				TCCR1A |=(1<<5);    //COM1B1=1
				SetPinDIR(3,4,1); //define PD4 pin as output

           #else //OC1B_MODE3
				TCCR1A |=((1<<4) | (1<<5)); //COM1B1=1 COM1B0=1
				SetPinDIR(3,4,1); //define PD4 pin as output
           #endif
}





/**
 * input capture ISR
 */
void __vector_6 (void) __attribute__ ((signal,used));//disable optimization for this function
void __vector_6 (void) //timer1 input capture  interrupt vector no
{

	 static volatile u16 RissingValue=0,FallingValue=0;
	 volatile u16 ICU_CapturedValue;

	 ICU_CapturedValue =ICR1L; //store the value of the low byte of ICR1 register
	 ICU_CapturedValue|=(ICR1H <<8); //store the value of the high byte of ICR1 register
	 ICU_EdgeFlag++;

	  /*
	  * when ICU_EdgeFlag =1 means it's the very first captured reading on the first falling edge
	  *  any calculation regarding this reading should be avoided.
	  *
	  * when ICU_EdgeFlag =2 means it's the second consecutive edge to be registered "rising edge"
	  * now ICU_ToffTicks can be calculated.
	  *
	  * when ICU_EdgeFlag =3 means it's the third consecutive edge to be registered "falling edge"
	  * now ICU_TonTicks can be calculated.
	  */

	    if (ICU_EdgeFlag==2) // execute on signal's rising edge "second edge to be captured"
	    {
		 TCCR1B &=~(1<<6); //change trigger edge to be falling edge
		 RissingValue=ICU_CapturedValue;

		if(T1_OVF_Counter>0) //check if overflow occurs before the value being registered
		{
			ICU_ToffTicks=(65536-FallingValue)+RissingValue+((T1_OVF_Counter-1)*65536);
		}
		else{ //signal registered in the same timer cycle
			ICU_ToffTicks=RissingValue-FallingValue;
		}

		T1_OVF_Counter=0;
         }



	   else if(ICU_EdgeFlag==3) //execute on signal's falling edge ,third edge to be captured
	    {
		TCCR1B |=(1<<6); //change trigger edge to be  rising edge
		FallingValue=ICU_CapturedValue;
	     	if(T1_OVF_Counter>0) //check if overflow occurs before the value being registered
		       {
			     ICU_TonTicks=(65536-RissingValue)+FallingValue+((T1_OVF_Counter-1)*65536);
		       }
		  else{//signal registered in the same timer cycle
			  	 ICU_TonTicks=FallingValue-RissingValue;
		       }

		T1_OVF_Counter=0;
		ICU_EdgeFlag-=2;

	    }


	   else //the very first reading  being picked on first falling edge
	    {
		FallingValue=ICU_CapturedValue;
		T1_OVF_Counter=0;
		TCCR1B |=(1<<6); //change trigger edge to be rising edge
	    }
}


/**
 * Channel A compare match ISR
 */
void __vector_7 (void) __attribute__ ((signal,used));//disable optimization for this function
void __vector_7 (void) //timer1  channel A compare match interrupt vector no
{
	Timer1_CHA_CompMatchIntFunc(); //execute the user's function when the channel A compare match interrupt occurs
}


/**
 * Channel B compare match ISR
 */
void __vector_8 (void) __attribute__ ((signal,used));//disable optimization for this function
void __vector_8 (void) //timer1  channel B compare match interrupt vector no
{
	Timer1_CHB_CompMatchIntFunc(); //execute the user's function when the channel B compare match interrupt occurs
}


/**
 * Timner1 overflow ISR
 */
void __vector_9 (void) __attribute__ ((signal,used));//disable optimization for this function
void __vector_9 (void) //timer1 overflow interrupt vector no
{
	Timer1_OverFlowIntFunc(); //execute the user's function when the overflow interrupt occurs
}
