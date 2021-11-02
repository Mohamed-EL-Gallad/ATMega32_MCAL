/*
 * ADC_Config.h
 *
 *  Created on: 18/10/2021
 *  Author:Mohamed_EL_Gallad
 */

#ifndef ADC_CONFIG_H_
#define ADC_CONFIG_H_


// ADC Prescaler
#define	ADC_CPU_CLK_DIV_BY2     1   // FCPU/2
#define	ADC_CPU_CLK_DIV_BY4     2   // FCPU/4
#define	ADC_CPU_CLK_DIV_BY8     3   // FCPU/8
#define	ADC_CPU_CLK_DIV_BY16    4   // FCPU/16
#define	ADC_CPU_CLK_DIV_BY32    5   // FCPU/32
#define	ADC_CPU_CLK_DIV_BY64    6   // FCPU/64
#define	ADC_CPU_CLK_DIV_BY128   7   // FCPU/128

#define ADC_PRESCLARE  ADC_CPU_CLK_DIV_BY64

//Data Adjustment
#define RESULT_RIGHT_ADJUSTED  0
#define RESULT_LEFT_ADJUSTED   1

#define RESULT_STORAGE_DIR   RESULT_RIGHT_ADJUSTED


//Voltage Reference Selection
#define REF_AREF                       0   //AREF, Internal Vref turned off
#define REF_AVCC                       1   //AVCC with external capacitor at AREF pin
#define REF_INT_2V56                   2   //Internal 2.56V Voltage Reference with external capacitor at AREF pin

#define ADC_VOLTAGE_REF  REF_AVCC


//ADC Operation Mode
#define FREE_RUNNING_MODE           0
#define ANALOG_CPMPARATOR           1
#define EXTERNAL_INT0               2
#define TIMER0_COMP_MATCH           3
#define TIMER0_OVERFLOW             4
#define TIMER1_CHB_COMP_MATCH       5
#define TIMER1_OVERFLOW             6
#define TIMER1_INPUT_CAPTURE        7
#define SINGLE_CONVERSION           8

#define ADC_OPERATION_MODE    FREE_RUNNING_MODE

#endif /* ADC_CONFIG_H_ */
