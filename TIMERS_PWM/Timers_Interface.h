/*
 * Timers.h
 * Created on: ٠٨‏/٠١‏/٢٠٢١.
 * Author: Mohamed_EL_Gallad.
 * Description: this is an interface header for timer0, timer1 and timer2 modules.
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "STD_types.h"
#include "TimersConfig.h"

/***************************** TIMER0 INTERFACE FUNCTIONS *****************************************************/

/**
 *  RETURN     :VOID.
 *  PARAMETER  :VOID
 *  DESCRIPTION:This function will set timer0 functionality to operate in Normal mode and set OC0 pin operation
 *     mode according to the predefined OC0_OPMODE macro
 *     Timer0_Enable() will be required to start the timer0 module and Timer0_Stop() to disable it
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
 *    it will also disable the overflow interrupt.
 */
void Timer0_OVF_UserFncDisable(void);


/**
 *  RETURN      :VOID.
 *  PARAMETER   :OverFLowsNo is a U16 variable the will have a number of overflow.
 *  DESCRIPTION :This function can be used to halt the normal program execution for a given number of Timer0 overflows
 *     that is defined by the user
 *     the user Must set Timer0 to operate in normal mode -Timer0_NormalModeInit()- first before using this function.
 */
void Timer0_CountDownOverFlows(u16 OverFLowsNo);


/**
 *  RETURN     :VOID.
 *  PARAMETER  :VOID.
 *  DESCRIPTION:This function will set timer0 functionality to operate in compare match and set OC0 pin operation
 *     mode according to the predefined OC0_OPMODE macro
 *     Timer0_Enable() will be required to start the timer0 module and Timer0_Stop() to disable it.
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
 *    it will also disable the compare match interrupt.
 */
void Timer0_CompMatch_UserFncDisable(void);


/**
 *  RETURN     :VOID.
 *  PARAMETER  :CompareMatchValue is a U8 variable , used to set the compare match value.
 *  DESCRIPTION:This function will set the OCR0 register value to the defined value by the user
 *     the user Must set Timer0 to operate in CTC mode -Timer0_CTCModeInit()- first before using this function.
 */
void Timer0_SetCompValue(u8 CompareMatchValue);


/**
 *  RETURN     :VOID.
 *  PARAMETER  :CycleInitValue is a U8 value , used to set a start value for a timer0 current cycle.
 *  DESCRIPTION:this function is used to set a start value equal to CycleInitValue that timer0 will start count from for the current cycle
 *     this function can be used in any non PWM Mode operation mode.
 */
void Timer0_CycleStartCountAt(u8 CycleInitValue);


/**
 *  RETURN     :VOID.
 *  PARAMETER  :VOID.
 *  DESCRIPTION:This function will set timer0 functionality to operate in Fast PWM and set OC0 pin operation
 *     mode according to the predefined OC0_OPMODE macro
 *     Timer0_Enable() will be required to start the timer0 module and Timer0_Stop() to disable it.
 */
void Timer0_FastPWMInit(void);


/**
 *  RETURN     :VOID.
 *  PARAMETER  :VOID.
 *  DESCRIPTION:This function will set timer0 functionality to operate in Phase Correct PWM and set OC0 pin operation
 *     mode according to the predefined OC0_OPMODE macro
 *     Timer0_Enable() will be required to start the timer0 module and Timer0_Stop() to disable it.
 */
void Timer0_PhaseCorrPWMInit(void);


/**
 * RETURN     :VOID.
 * PARAMETER  :DutyCyclePercentage is a U8 variable represents the required duty cycle will have value from 0 to 100.
 * DESCRIPTION:this function will be used to set the PWM duty cycle by a user defined vale equal to DutyCyclePercentage
 *    this function can be used in either fast PWM or phase correct PWM.
 *    either Timer0_FastPWMInit() or Timer0_PhaseCorrPWMInit() must be used before the use of this function.
 */
void Timer0_SetPWM_DutyCycle(u8 DutyCyclePercentage);


/**
 * RETURN      :VOID.
 * PARAMETER   :VOID.
 * DESCRIPTION :this function will enable the timer0 module by setting the prescaler value that is defined
 *    by the TIMER0_PRESCALER Macro , the use of this function is required to start the initiated timer0 functionality.
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



/***************************** TIMER1 INTERFACE FUNCTIONS *****************************************************/
/**
 * @details d practice to disable the timer if it's no longer needed to save power.
 * @param OC1A_OPModeNormal
 * @param OC1B_OPModeNormal
 */
void Timer1_NormalModeInit(u8 OC1A_OPModeNormal, u8 OC1B_OPModeNormal);

void Timer1_CountDownOverFlows(u32 T1_OverFLowsNo);

void Timer1_CTCModeInit(u8 OC1A_OPModeCTC, u8 OC1B_OPModeCTC);

void Timer1_SetCompMatchTopVal(u16 T1_CTC_TopValue);

void Timer1_CHA_SetCompValue(u16 T1_CHA_CompareMatchValue);

void Timer1_CHB_SetCompValue(u16 T1_CHB_CompareMatchValue);

void Timer1_CycleStartCountAt(u16 T1_CycleInitValue);

void Timer1_InputCaptureInit(u8 TrigSrc);

void Timer1_ICUGetEventData(f32 *TonTime, f32 *DutyCycle, u16 *Freq);

void Timer1_ExecuteOnOverFlow(void (*T1_OVFUserFunction)(void));

void Timer1_OVF_UserFnDisable(void);

void Timer1_CHA_ExecuteOnCompMatch(void (*T1_CHA_CompMatch_UserFunction)(void));

void Timer1_CHACompMatch_UserFnDisable(void);

void Timer1_CHB_ExecuteOnCompMatch(void (*T1_CHB_CompMatch_UserFunction)(void));

void Timer1_CHBCompMatch_UserFnDisable(void);

void Timer1_Enable(void);

void Timer1_Stop(void);

#endif /* TIMERS_H_ */
