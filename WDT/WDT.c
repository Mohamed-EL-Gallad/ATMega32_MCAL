/*
 * WDT.c
 *
 *  Created on: 27/04/2021
 *  Author: Mohamed_EL_Gallad
 *  Brief : This file will contain the implementation for the watchdog timer functions
 */
#include "STD_types.h"
#include "WDT_Interface.h"
#include "WDT_Config.h"
#include "Mega32_reg.h"


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : This function will enable the watchdog timer and will set the reset interval depending on the predefined MACRO WDT_RESET_INTERVAL
 */
void Enable_WDT(void)
{
	WDTCR |=(1<<3); //WDE=1 enable WDT
	WDTCR |=((WDTCR & 0xF8) | ((u8)WDT_RESET_INTERVAL)); //define the required reset interval
}


/**
 * RETURN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : This function will reset the watchdog timer
 */
void Reset_WDT(void)
{
	//disabling the WDT will reset the timer
	Disable_WDT();
	Enable_WDT();
}


/**
 * RETUEN      : VOID
 * PARAMETER   : VOID
 * DESCRIPTION : This function will disable the watchdog timer functionality
 */
void Disable_WDT(void)
{
	//WDT disable sequence
	WDTCR |=(1<<3 | 1<<4); //WDTOE=1 and WDE=1
	WDTCR &=~(1<<3); //WDE=0
}

