/*
 * WDT.h
 *
 *  Created on: 27/04/2021
 *  Author: Mohamed_EL_Gallad
 *  Brief : This fill will contain the interface functions for the watchdog timer
 */

#ifndef WDT_INTERFACE_H_
#define WDT_INTERFACE_H_

/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : This function will enable the watchdog timer and will set the reset interval depending on the predefined MACRO WDT_RESET_INTERVAL
 */
void Enable_WDT(void);


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : This function will reset the watchdog timer
 */
void Reset_WDT(void);


/**
 * RETUEN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : This function will disable the watchdog timer functionality
 */
void Disable_WDT(void);

#endif /* WDT_INTERFACE_H_ */
