/*
 * EXT_InterruptsInterface.h
 *
 *  Created on: 20/04/2021
 *  Author: Mohamed_EL_Gallad
 *  Brief: This fill will contain the interface for the external interrupts INT0, INT1, and INT2
 */

#ifndef EXT_INTERRUPTSINTERFACE_H_
#define EXT_INTERRUPTSINTERFACE_H_

#include "EXT_InterruptsConfig.h"

/**
 * RETURN      : VOID
 * PARAMETER   : INT0_UserFN , a pointer to a user defined function to be executed when INT0 is triggered
 * DESCRIPTION : This function is used to enable INT0 and set it's external trigger type depending on
 * the predefined MACRO INT0_TRIG_TYPE , it will also mount a user defined function on INT0 ISR to be executed whenever INT0 is being triggered
 */
void Enable_INT0(void(*INT0_UserFN)(void));


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : This function is used to disable INT0 and it will also unmount the user defined function from INT0 ISR
 */
void Disable_INT0(void);


/**
 * RETURN      : VOID
 * PARAMETER   : INT1_UserFN , a pointer to a user defined function to be executed when INT1 is triggered
 * DESCRIPTION : This function is used to enable INT1 and set it's external trigger type depending on
 * the predefined MACRO INT1_TRIG_TYPE , it will also mount a user defined function on INT1 ISR to be executed whenever INT1 is being triggered
 */
void Enable_INT1(void(*INT0_UserFN)(void));


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : This function is used to disable INT1 and it will also unmount the user defined function from INT1 ISR
 */
void Disable_INT1(void);


/**
 * RETURN      : VOID
 * PARAMETER   : INT2_UserFN , a pointer to a user defined function to be executed when INT2 is triggered
 * DESCRIPTION : This function is used to enable INT2 and set it's external trigger type depending on
 * the predefined MACRO INT2_TRIG_TYPE , it will also mount a user defined function on INT2 ISR to be executed whenever INT2 is being triggered
 */
void Enable_INT2(void(*INT0_UserFN)(void));


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : This function is used to disable INT2 and it will also unmount the user defined function from INT2 ISR
 */
void Disable_INT2(void);

#endif /* EXT_INTERRUPTSINTERFACE_H_ */
