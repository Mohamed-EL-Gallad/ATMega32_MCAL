/*
 * Timers.h
 * Created on: 01/04/2021
 * Author: Mohamed_EL_Gallad.
 * Description: this is an interface header for timer0, timer1 and timer2 modules.
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "STD_types.h"
#include "TimersConfig.h"

/**-----------------------------------------------------------------------------------------------------------*/
/*                               TIMER0 INTERFACE FUNCTIONS                                                   */
/**-----------------------------------------------------------------------------------------------------------*/
/**
 *  RETURN     :VOID.
 *  PARAMETER  :VOID
 *  DESCRIPTION:This function will set timer0 functionality to operate in Normal mode and set OC0 pin operation
 *  mode according to the predefined OC0_OPMODE macro
 *  Timer0_Enable() will be required to start the timer0 module and Timer0_Stop() to disable it
 */

void Timer0_NormalModeInit(void);


/**
 *  RETURN      :VOID.
 *  PARAMETER   :OVF_UserFunction,is pointer to a user defined function that has void return and input parameters.
 *  DESCRIPTION :This function will mount a user defined function to be executed on overflow interrupt.
 */
void Timer0_ExecuteOnOverFlow(void (*OVF_UserFunction)(void));


/**
 * RETURN      :VOID.
 * PARAMETER   :VOID.
 * DESCRIPTION :this function will disable the execution of the user defined function when overflow interrupt occurs
 * it will also disable the overflow interrupt.
 */
void Timer0_OVF_UserFncDisable(void);


/**
 *  RETURN      :VOID.
 *  PARAMETER   :OverFLowsNo is a U16 variable the will have the number of overflows.
 *  DESCRIPTION :This function can be used to halt the normal program execution for a given number of Timer0 overflows
 *  that is defined by the user
 *  the user Must set Timer0 to operate in normal mode -Timer0_NormalModeInit()- first before using this function.
 */
void Timer0_CountDownOverFlows(u16 OverFLowsNo);


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : this function is used to initiate timer0 as counter , it will count numbers of external falling or rising edges
 * applied on pin B0 , the edge trigger type can be selected by setting the TIMER0_PRESCALER MACRO to
 * either EXT_CLK_FALLING "for falling edge trigger" or  EXT_CLK_RISSING "for rising edge trigger"
 *  Timer0_Enable() will be required to start the timer0 module and Timer0_Stop() to disable it
 * CAUTION     :this function will force timer0 to operate in normal mode and it will also dictate timer0 overflow interrupt
 */
void Timer0_CounterInit(void);


/**
 * RETURN      : u16 variable represents the counter value
 * PARAMETER   : VOID
 * DESCRIPTION : this function will return a value represents the number of counts of the signal applied on pin B0
 */
u16 Timer0_GetCounterValue(void);


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : this function is used to reset the counter's value as it will reset the value stored in TCNT0
 * and will also reset the number of overflows
 */
void Timer0_ResetCounter(void);


/**
 *  RETURN     :VOID.
 *  PARAMETER  :VOID.
 *  DESCRIPTION:This function will set timer0 functionality to operate in compare match and set OC0 pin operation
 *  mode according to the predefined OC0_OPMODE macro
 *  Timer0_Enable() will be required to start the timer0 module and Timer0_Stop() to disable it.
 */
void Timer0_CTCModeInit(void);


/**
 * RETURN      :VOID.
 * PARAMETER   :CompMatch_UserFunction is pointer to a user defined function that has void return and input parameters.
 * DESCRIPTION :This function will mount a user defined function to be executed on compare match interrupt.
 */
void Timer0_ExecuteOnCompMatch(void (*CompMatch_UserFunction)(void));


/**
 * RETURN      :VOID.
 * PARAMETER   :VOID.
 * DESCRIPTION :this function will disable the execution of the user defined function when compare match interrupt occurs
 * it will also disable the compare match interrupt.
 */
void Timer0_CompMatch_UserFncDisable(void);


/**
 *  RETURN     :VOID.
 *  PARAMETER  :CompareMatchValue is a U8 variable , used to set the compare match value.
 *  DESCRIPTION:This function will set the OCR0 register value to the defined value by the user
 *  the user Must set Timer0 to operate in CTC mode -Timer0_CTCModeInit()- first before using this function.
 */
void Timer0_SetCompValue(u8 CompareMatchValue);


/**
 *  RETURN     :VOID.
 *  PARAMETER  :CycleInitValue is a U8 value , used to set a start value for a timer0 current cycle.
 *  DESCRIPTION:this function is used to set a start value equal to CycleInitValue that timer0 will start count from for the current cycle
 *  this function can be used in any non PWM Mode operation mode.
 */
void Timer0_CycleStartCountAt(u8 CycleInitValue);


/**
 *  RETURN     :VOID.
 *  PARAMETER  :VOID.
 *  DESCRIPTION:This function will set timer0 functionality to operate in Fast PWM and set OC0 pin operation
 *  mode according to the predefined OC0_OPMODE macro
 *  Timer0_Enable() will be required to start the timer0 module and Timer0_Stop() to disable it.
 */
void Timer0_FastPWMInit(void);


/**
 *  RETURN     :VOID.
 *  PARAMETER  :VOID.
 *  DESCRIPTION:This function will set timer0 functionality to operate in Phase Correct PWM and set OC0 pin operation
 *  mode according to the predefined OC0_OPMODE macro
 *  Timer0_Enable() will be required to start the timer0 module and Timer0_Stop() to disable it.
 */
void Timer0_PhaseCorrPWMInit(void);


/**
 * RETURN     :VOID.
 * PARAMETER  :DutyCyclePercentage is a f32 variable represents the required duty cycle will have value from 0.0 to 100.0.
 * DESCRIPTION:this function will be used to set the PWM duty cycle by a user defined vale equal to DutyCyclePercentage
 * this function can be used in either fast PWM or phase correct PWM.
 * either Timer0_FastPWMInit() or Timer0_PhaseCorrPWMInit() must be used before the use of this function.
 */
void Timer0_SetPWM_DutyCycle(f32 DutyCyclePercentage);


/**
 * RETURN      :VOID.
 * PARAMETER   :VOID.
 * DESCRIPTION :this function will enable the timer0 module by setting the prescaler value that is defined
 * by the TIMER0_PRESCALER Macro , the use of this function is required to start the initiated timer0 functionality.
 */
void Timer0_Enable(void);


/**
 * RETURN      :VOID.
 * PARAMETER   :VOID.
 * DESCRIPTION :this function will disable timer0 by setting the prescaler value to zero
 * it will also reset the value of TCNT0 register
 * it's a good practice to disable the timer if it's no longer needed to save power.
 */
void Timer0_Stop(void);


/**-----------------------------------------------------------------------------------------------------------*/
/*                               TIMER1 INTERFACE FUNCTIONS                                                   */
/**-----------------------------------------------------------------------------------------------------------*/

/**
 * RETURN   : VOID
 * PARAMETER: VOID
 * DESCRIPTION:This function will set timer1 functionality to operate in Normal mode and set OC1A & OC1B pins operation
 * mode according to the predefined OC1A_OPMODE & OC1B_OPMODE MACROS.
 * Timer1_Enable() will be required to start the timer1 module and Timer1_Stop() to disable it
 */
void Timer1_NormalModeInit(void);


/**
 *  RETURN      :VOID.
 *  PARAMETER   :T1_OVFUserFunction,is pointer to a user defined function that has void return and input parameters.
 *  DESCRIPTION :This function will mount a user defined function to be executed on overflow interrupt.
 */
void Timer1_ExecuteOnOverFlow(void (*T1_OVFUserFunction)(void));


/**
 * RETURN      :VOID.
 * PARAMETER   :VOID.
 * DESCRIPTION :this function will disable the execution of the user defined function when overflow interrupt occurs
 * it will also disable the overflow interrupt.
 */
void Timer1_OVF_UserFnDisable(void);


/**
 *  RETURN      :VOID.
 *  PARAMETER   :T1_OverFLowsNo is a U32 variable the will have the number of overflows.
 *  DESCRIPTION :This function can be used to halt the normal program execution for a given number of Timer1 overflows
 *  that is defined by the user
 *  the user Must set Timer1 to operate in normal mode -Timer1_NormalModeInit()- first before using this function.
 */
void Timer1_CountDownOverFlows(u32 T1_OverFLowsNo);


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : this function is used to initiate timer1 as counter , it will count numbers of external falling or rising edges
 * applied on pin B1 , the edge trigger type can be selected by setting the TIMER1_PRESCALER MACRO to
 * either EXT_CLK_FALLING "for falling edge trigger" OR EXT_CLK_RISSING "for rising edge trigger"
 *  Timer1_Enable() will be required to start the timer1 module and Timer1_Stop() to disable it
 * CAUTION     :this function will force timer1 to operate in normal mode and it will also dictate timer1 overflow interrupt
 */
void Timer1_CounterInit(void);


/**
 * RETURN      : u32 variable represents the counter value
 * PARAMETER   : VOID
 * DESCRIPTION : this function will return a value represents the number of counts of the signal applied on pin B1
 */
u32 Timer1_GetCounterValue(void);


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : this function is used to reset the counter's value as it will reset the value stored in TCNT1H & TCNT1L
 * and will also reset the number of overflows
 */
void Timer1_ResetCounter(void);


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
void Timer1_CTCModeInit(void);


/**
 * RETURN      :VOID.
 * PARAMETER   :T1_CHA_CompMatch_UserFunction is pointer to a user defined function that has void return and input parameters.
 * DESCRIPTION :This function will mount a user defined function to be executed on Timer1 channel A compare match interrupt.
 */
void Timer1_CHA_ExecuteOnCompMatch(void (*T1_CHA_CompMatch_UserFunction)(void));


/**
 * RETURN      :VOID.
 * PARAMETER   :VOID.
 * DESCRIPTION :this function will disable the execution of the user defined function when Timer1 channel A compare match interrupt occurs
 * it will also disable the compare match interrupt.
 */
void Timer1_CHACompMatch_UserFnDisable(void);


/**
 * RETURN      :VOID.
 * PARAMETER   :T1_T1_CHB_CompMatch_UserFunction is pointer to a user defined function that has void return and input parameters.
 * DESCRIPTION :This function will mount a user defined function to be executed on Timer1 channel A compare match interrupt.
 */
void Timer1_CHB_ExecuteOnCompMatch(void (*T1_CHB_CompMatch_UserFunction)(void));


/**
 * RETURN      :VOID.
 * PARAMETER   :VOID.
 * DESCRIPTION :this function will disable the execution of the user defined function when Timer1 channel A compare match interrupt occurs
 * it will also disable the compare match interrupt.
 */
void Timer1_CHBCompMatch_UserFnDisable(void);


/**
 * RETURN     : VOID
 * PARAMETER  : T1_CTC_TopValue is a U16 variable is being used to set the timer's top value in compare match mode
 * DESCRIPTION: this function will be used to set the top value for timer1 in compare match mode
 * depending on the CTC mode being choosed using the macro T1_COMPMATCH_OPMODE ,either OCR1A or ICR1 register can be used to set the timer's top value
 * if COMPMATCH_OPMODE =T1_COMPMATCH_MODE1 ,OCR1A will be used to set the top value
 * if COMPMATCH_OPMODE =T1_COMPMATCH_MODE2 , ICR1 will be used to set the top value
 */
void Timer1_SetCompMatchTopVal(u16 T1_CTC_TopValue);


/**
 * RETURN      :VOID.
 * PARAMETER   :T1_CHA_CompareMatchValue this U16 variable will set the compare match value for timer1 channel A.
 * DESCRIPTION :this function is used to set the compare match value for timer1 channel A "OCR1A=T1_CHA_CompareMatchValue" if TI_COMPMATCH_MODE2 is being used as timer1 compare match mode
 * CAUTOION    :this function will have no effect if OCR1A is used to set timer1's top value "T1_COMPMATCH_OPMODE=T1_COMPMATCH_MODE2"
 */
void Timer1_CHA_SetCompValue(u16 T1_CHA_CompareMatchValue);


/**
 * RETURN      :VOID.
 * PARAMETER   :T1_CHB_CompareMatchValue this U16 variable will set the compare match value for timer1 channel B.
 * DESCRIPTION :this function is used to set the compare match value for timer1 channel B "OCR1B=T1_CHB_CompareMatchValue"
 * CAUTOION    :T1_CHB_CompareMatchValue Must have a value less than or equal OCR1A in case it's being used to set the timer's top value "T1_COMPMATCH_OPMODE=T1_COMPMATCH_MODE2"
 * OR a value less than or equal ICR1 in case it's being used to set the timer's top value "T1_COMPMATCH_OPMODE=T1_COMPMATCH_MODE1" otherwise no compare match will occur
 */
void Timer1_CHB_SetCompValue(u16 T1_CHB_CompareMatchValue);


/**
 *  RETURN     :VOID.
 *  PARAMETER  :T1_CycleInitValue is a U16 value , used to set a start value for a timer1 current cycle.
 *  DESCRIPTION:this function is used to set a start value equal to T1_CycleInitValue that timer1 will start count from for the current cycle
 *  this function can be used in any non PWM Mode operation mode.
 */
void Timer1_CycleStartCountAt(u16 T1_CycleInitValue);


/**
 * RETURN      :VOID
 * PARAMETER   :VOID
 * DESCRIPTION :this function will initiate the input capture functionality
 * CAUTION     :this function will dictate timer1 operation and force it to work in normal mode, disconnect OC1A & OC1B pins and override any function mounted on overflow interrupt
 * so in case the input capture functionality is being initiated user MUST avoid using timer1 in any other application.
 */
void Timer1_InputCaptureInit(void);


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
 *
 */
void Timer1_ICUGetEventData(f32 *TonTime, f32 *DutyCycle, u16 *Freq);


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION :  This function will set timer1 functionality to operate in one of the 5 Fast PWM modes that are available
 * depending on the predefined macro T1_FASTPWM_OPMODE and it will also set OC1A and OCA1B  pins operation
 *  mode depending on the predefined OC1A_OPMODE & OC1B_OPMODE MACROS
 *  Timer1_Enable() will be required to start the timer1 module and Timer1_Stop() to disable it.
 */
void Timer1_FastPWMInit(void);


/**
 * RETURN     :VOID.
 * PARAMETER  :VOID.
 * DESCRIPTION:This function will set timer1 functionality to operate in one of 7 available modes of either Phase Correct PWM or phase and frequency correct
 * depending on the predefined macro TI_PWM_PHASECORR_OPMODE and it will also set OC1A and OCA1B  pins operation
 *  mode depending on the predefined OC1A_OPMODE & OC1B_OPMODE MACROS
 *  Timer1_Enable() will be required to start the timer1 module and Timer0_Stop() to disable it.
 */
void Timer1_PhaseCorrPWMInit(void);


/**
 * RETURN      : VOID
 * PARAMETER   : CHA_DutyCyclePercentage is f32 variable that represents the required duty cycle for channel A and must have a value between 0.0 -> 100.0
 * DESCRIPTION : this function is used to set the output's duty cycle of channel A in either fast PWM modes or Phase correct PWM modes
 */
void Timer1_CHA_SetPWM_DutyCycle(f32 CHA_DutyCyclePercentage);


/**
 * RETURN      : VOID
 * PARAMETER   : CHB_DutyCyclePercentage is f32 variable that represents the required duty cycle for channel B and must have a value between 0.0 -> 100.0
 * DESCRIPTION : this function is used to set the output's duty cycle of channel B in either fast PWM modes or Phase correct PWM modes
 */
void Timer1_CHB_SetPWM_DutyCycle(f32 CHB_DutyCyclePercentage);


/**
 * RETURN      : VOID
 * PARAMETER   : Frequency is u16 variable that represents the required frequency for channel A and channel B
 * DESCRIPTION : this function is used to set the required frequency for both channel A and channel B in any of the following modes:
 * T1_PHASECORR_MODE4
 * T1_PHASECORR_MODE5 ,OC1A will toggle with freq=Frequency/2 and duty cycle=50% when OC1A_OPMODE=OC1A_MODE1
 * T1_PHASE_FREQ_CORR_MODE1
 * T1_PHASE_FREQ_CORR_MODE2 ,OC1A will toggle with freq=Frequency/2 and duty cycle=50% when OC1A_OPMODE=OC1A_MODE1
 * T1_FASTPWM_MODE4
 * T1_FASTPWM_MODE5 ,OC1A will toggle with freq=Frequency/2 and duty cycle=50% when OC1A_OPMODE=OC1A_MODE1
 * and will have no effect if being used with any other PWM modes
 * NOTE        :changing the frequency from a value to another during the operation will adjust the duty cycle automatically to match the predefined value before changing the frequency
 */
void Timer1_SetPWN_Freq(u16 Frequency);


/**
 * RETURN      :VOID.
 * PARAMETER   :VOID.
 * DESCRIPTION :this function will enable the timer1 module by setting the prescaler value that is defined
 * by the TIMER1_PRESCALER Macro , the use of this function is required to start the initiated timer1 functionality.+
 * CAUTION     :this function must not be used while the input capture mode is being initiated ,
 * the timer will be enabled when Timer1_ICUGetEventData() is being provoked and will be disabled after the function's execution
 */
void Timer1_Enable(void);


/**
 * RETURN      :VOID.
 * PARAMETER   :VOID.
 * DESCRIPTION :this function will disable timer1 by setting the prescaler value to zero
 * it will also reset the value of TCNT1 register
 * it's a good practice to disable the timer if it's no longer needed to save power.
 * CAUTION     :this function must not be used while the input capture mode is being initiated ,
 * the timer will be enabled when Timer1_ICUGetEventData() is being provoked and will be disabled after the function's execution
 */
void Timer1_Stop(void);



#endif /* TIMERS_H_ */
