/*
 * WDT_Config.h
 *
 *  Created on: 27/04/2021
 *  Author: Mohamed_EL_Gallad
 *  Brief : This file will contain the required setting to configure the WDT to reset the MCU depending on the chosen reset interval
 *  the intervals can be selected by setting the WDT_RESET_INTERVAL MACRO to one of the predefined reset interval MACROS
 */

/*
 *  The following MACROS represent the typical reset time intervals at VCC = 5.0V
 */

#ifndef WDT_CONFIG_H_
#define WDT_CONFIG_H_

#define WDT_RESET_INTERVAL_16_3    0 //WDT reset interval is 16.3 ms
#define WDT_RESET_INTERVAL_32_5    1 //WDT reset interval is 32.5 ms
#define WDT_RESET_INTERVAL_65      2 //WDT reset interval is 65   ms
#define WDT_RESET_INTERVAL_130     3 //WDT reset interval is 130  ms
#define WDT_RESET_INTERVAL_260     4 //WDT reset interval is 260  ms
#define WDT_RESET_INTERVAL_520     5 //WDT reset interval is 520  ms
#define WDT_RESET_INTERVAL_1000    6 //WDT reset interval is 1000 ms
#define WDT_RESET_INTERVAL_2100    7 //WDT reset interval is 2100 ms


#define WDT_RESET_INTERVAL  WDT_RESET_INTERVAL_1000

#endif /* WDT_CONFIG_H_ */
