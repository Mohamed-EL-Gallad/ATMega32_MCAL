/*
 * AnalogCompPrivate.h
 *
 *  Created on:17/10/2021
 *  Author: Mohamed_EL_Gallad
 *  Description: This file will contain the Analog Comparator private Macros "DON'T CHANGE ANY VALUE EXISTS IN THIS FILE"
 */

#ifndef ANALOGCOMPPRIVATE_H_
#define ANALOGCOMPPRIVATE_H_

//SFIOR Register Bits
#define ACME   ((u8)3)


//ACSR Register Bits
#define ACD    ((u8)7)
#define ACBG   ((u8)6)
#define ACO    ((u8)5)
#define ACI    ((u8)4)
#define ACIE   ((u8)3)
#define ACIC   ((u8)2)
#define ACIS1  ((u8)1)
#define ACIS0  ((u8)0)

//ADCSRA Register Bits
#ifndef ADEN
#define ADEN   ((u8)7)
#endif

//negative input options
#define MULTIPLEXED_INPUT_ADC0      ((u8)0)
#define MULTIPLEXED_INPUT_ADC1      ((u8)1)
#define MULTIPLEXED_INPUT_ADC2      ((u8)2)
#define MULTIPLEXED_INPUT_ADC3      ((u8)3)
#define MULTIPLEXED_INPUT_ADC4      ((u8)4)
#define MULTIPLEXED_INPUT_ADC5      ((u8)5)
#define MULTIPLEXED_INPUT_ADC6      ((u8)6)
#define MULTIPLEXED_INPUT_ADC7      ((u8)7)
#define NEGATIVE_INPUT_ON_AIN1      ((u8)8)


//ADMUX pins , limited to the used ones in the analog comparator lib.
#ifndef MUX0
#define MUX0   ((u8)0)
#endif

#ifndef MUX1
#define MUX1   ((u8)1)
#endif

#ifndef MUX2
#define MUX2   ((u8)2)
#endif

#endif /* ANALOGCOMPPRIVATE_H_ */
