/*
 * AnalogCompConfig.h
 *
 *  Created on:17/10/2021
 *  Author: Mohamed_EL_Gallad
 *  Description : This file will contain all the required configurations Macros to configure the analog comparator to operate as desired
 *  this file will control the following properties of the analog comparator :
 *  	1- Enable or Disable the negative input multiplexing by setting this macro ANALOG_COMPARATOR_MULTIPLEXER to either ENABLE or DISABLE
 *  	2- Enable or Disable the use of bandgap voltage reference as the positive input by  setting this macro ANALOG_COMPARATOR_BANDGAP_SELECT to either ENABLE or DISABLE
 *  	3- Enable or Disable the use of the timeer_1 ICU by setting this macro ANALOG_COMPARATOR_INPUTCAPTURE to either ENABLE or DISABLE
 *
 */

#ifndef ANALOGCOMPCONFIG_H_
#define ANALOGCOMPCONFIG_H_


/**-------------------------------------------------------------------------------------------------------------
 *                               ANALOG COMPARATOR NEGATIVE INPUT CONFIGURATION
 *-------------------------------------------------------------------------------------------------------------*/
// change between either ENABLE or DISABLE Macros to enable or disable the negative input multiplexing
#define ANALOG_COMPARATOR_MULTIPLEXER      DISABLE

/***************************************************************************************************************/


/**-------------------------------------------------------------------------------------------------------------
 *                               ANALOG COMPARATOR POSITIVE INPUT CONFIGURATION
 *-------------------------------------------------------------------------------------------------------------*/
// change between either ENABLE or DISABLE Macros to either use the bandgap voltage as the positive input or not
#define ANALOG_COMPARATOR_BANDGAP_SELECT   DISABLE

/***************************************************************************************************************/


/**-------------------------------------------------------------------------------------------------------------
 *                               THE USE OF THE INPUT CAPTURE FUNCTIONALITY
 *-------------------------------------------------------------------------------------------------------------*/
// change between either ENABLE or DISABLE Macros to enable or disable the use of the Timer1 ICU
//NOTE: check the ICU section in Timers module "Timers_Interface.h and TimersConfig.h"for the proper configuration
#define ANALOG_COMPARATOR_INPUTCAPTURE     DISABLE

/***************************************************************************************************************/


/**-------------------------------------------------------------------------------------------------------------
 *                               ANALOG COMPARATOR INTERRPUT CONFIGURATIONS
 *-------------------------------------------------------------------------------------------------------------*/
//Analog Comparator Interrupt Mode Select
#define INTERRUPT_ON_TOGGLE                1
#define INTERRUPT_ON_FALLING_EDGE          2
#define INTERRUPT_ON_RISING_EDGE           3
#define INTERRUPT_DISABLE                  4

#define ANALOG_COMPARATOR_INTERRUPT_OPMODE  INTERRUPT_DISABLE

#endif /* ANALOGCOMPCONFIG_H_ */
