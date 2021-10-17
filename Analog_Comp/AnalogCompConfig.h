/*
 * AnalogCompConfig.h
 *
 *  Created on:17/10/2021
 *  Author: Mohamed_EL_Gallad
 */

#ifndef ANALOGCOMPCONFIG_H_
#define ANALOGCOMPCONFIG_H_



#define ANALOG_COMPARATOR_MULTIPLEXER      ENABLE
#define ANALOG_COMPARATOR_BANDGAP_SELECT   ENABLE
#define ANALOG_COMPARATOR_INPUTCAPTURE     ENABLE

//Analog Comparator Interrupt Mode Select
#define INTERRUPT_ON_TOGGLE                1
#define INTERRUPT_ON_FALLING_EDGE          2
#define INTERRUPT_ON_RISING_EDGE           3
#define INTERRUPT_DISABLE                  4

#define ANALOG_COMPARATOR_INTERRUPT_OPMODE  INTERRUPT_ON_RISING_EDGE

#endif /* ANALOGCOMPCONFIG_H_ */
