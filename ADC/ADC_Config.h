/*
 * 	ADC_Config.h
 *  Created on: 18/10/2021
 *  Author:Mohamed_EL_Gallad
 *	Description : This file will contain all the configurations that the user may need to operate the ADC module as required
 *		1-Prescaler Settings
 *		2-Data Adjustment "right or left adjustment"
 *		3-ADC Voltage reference "either AVCC or AREF or internal 2.56 volt"
 *		4-ADC Modes of operations and triggers sources
 *			4-1- Free running mode
 *			4-2- Analog Comparator
 *			4-3- A Conversion will be triggered when EXTERNAL_INT0 kicks in
 *			4-4- A Conversion will be triggered when TIMER0_COMP_MATCH occurs "timer0 compare match"
 *			4-5- A Conversion will be triggered when TIMER0_OVERFLOW occurs "timer0 overflow"
 *			4-6- A Conversion will be triggered when TIMER1_CHB_COMP_MATCH occurs "timer1 channel B compare match"
 *			4-7- A Conversion will be triggered when TIMER1_OVERFLOW occurs "timer1 overflow"
 *			4-8- A Conversion will be triggered when TIMER1_INPUT_CAPTURE event occurs
 *			4-9- SINGLE_CONVERSION
 *
 */

#ifndef ADC_CONFIG_H_
#define ADC_CONFIG_H_

/*--------------------------------------------------------------------------------------------------------------
 *                                      ADC PRESCALER
 *-------------------------------------------------------------------------------------------------------------*/
#define	ADC_CPU_CLK_DIV_BY2     1   // FCPU/2
#define	ADC_CPU_CLK_DIV_BY4     2   // FCPU/4
#define	ADC_CPU_CLK_DIV_BY8     3   // FCPU/8
#define	ADC_CPU_CLK_DIV_BY16    4   // FCPU/16
#define	ADC_CPU_CLK_DIV_BY32    5   // FCPU/32
#define	ADC_CPU_CLK_DIV_BY64    6   // FCPU/64
#define	ADC_CPU_CLK_DIV_BY128   7   // FCPU/128

#define ADC_PRESCLARE  ADC_CPU_CLK_DIV_BY128 //set the value of this MACRO to one of the predefined MACROS to set the ADC prescaler
/**************************************************************************************************************/


/*--------------------------------------------------------------------------------------------------------------
 *                                     DATA  ADJUSTMENT
 *-------------------------------------------------------------------------------------------------------------*/
#define RESULT_RIGHT_ADJUSTED  0
#define RESULT_LEFT_ADJUSTED   1

//set the value of this MACRO to either RESULT_RIGHT_ADJUSTED or RESULT_LEFT_ADJUSTED to select the data storage direction
#define RESULT_STORAGE_DIR   RESULT_RIGHT_ADJUSTED
/**************************************************************************************************************/


/*--------------------------------------------------------------------------------------------------------------
 *                                     VOLTAGE REFERENCE SELECTION
 *-------------------------------------------------------------------------------------------------------------*/
#define REF_AREF                       0   //AREF, Internal Vref turned off
#define REF_AVCC                       1   //AVCC with external capacitor at AREF pin
#define REF_INT_2V56                   2   //Internal 2.56V Voltage Reference with external capacitor at AREF pin

//set the value of this MACRO to either REF_AREF or REF_AVCC or REF_INT_2V56 to select the desired voltage reference
#define ADC_VOLTAGE_REF  REF_AVCC
/**************************************************************************************************************/


/*--------------------------------------------------------------------------------------------------------------
 *                                 ADC OPERATION MODES AND TRIGGER SOURCES
 *-------------------------------------------------------------------------------------------------------------*/
#define FREE_RUNNING_MODE           0  //free running mode
#define ANALOG_CPMPARATOR           1  //operate as analog comparator
#define EXTERNAL_INT0               2  //start a conversion when external interrupt 0 kicks in
#define TIMER0_COMP_MATCH           3  //start a conversion when a timer0 compare match occur
#define TIMER0_OVERFLOW             4  //start a conversion when timer0 overflows
#define TIMER1_CHB_COMP_MATCH       5  //start a conversion when timer1 channel B compare match occure
#define TIMER1_OVERFLOW             6  //start a conversion when timer1 overflows
#define TIMER1_INPUT_CAPTURE        7  //start a conversion when input capture event occurs
#define SINGLE_CONVERSION           8  //a single conversion will occure

//set the value of this MACRO to one of the predefined MACROS to select the operation mode and trigger source
#define ADC_OPERATION_MODE    FREE_RUNNING_MODE
/**************************************************************************************************************/

#endif /* ADC_CONFIG_H_ */
