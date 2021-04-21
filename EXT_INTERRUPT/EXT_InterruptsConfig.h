/*
 * EXT_InterruptsConfig.h
 *
 *  Created on: 20/04/2021
 *  Author: Mohamed_EL_Gallad
 *  Brief :This file will contain the logical level configurations required to trigger the external interrupts INT0 , INT1 and INT3
 */

#ifndef EXT_INTERRUPTSCONFIG_H_
#define EXT_INTERRUPTSCONFIG_H_

/**-----------------------------------------------------------------------------------------------------------*/
/*                                      INT0 & INT1 TRIGGER CONFIGURATION                                     */
/**-----------------------------------------------------------------------------------------------------------*/

#define LOW_LEVEL             0   //low level trigger
#define ANY_LOGICAL_CHANGE    1   //any logical change trigger
#define FALLING_EDGE          2   //falling edge trigger
#define RISING_EDGE           3   //rising edge trigger

#define INT0_TRIG_TYPE   RISING_EDGE //this macro will define the logical trigger required to trigger INT0

#define INT1_TRIG_TYPE   RISING_EDGE //this macro will define the logical trigger required to trigger INT1

/**************************************************************************************************************/

/**-----------------------------------------------------------------------------------------------------------*/
/*                                     INT2 TRIGGER CONFIGURATION                                     */
/**-----------------------------------------------------------------------------------------------------------*/

#define INT2_FALLING_EDGE     0
#define  INT2_RISING_EDGE     1

#define INT2_TRIG_TYPE   INT2_RISING_EDGE //this macro will define the logical trigger required to trigger INT2




#endif /* EXT_INTERRUPTSCONFIG_H_ */
