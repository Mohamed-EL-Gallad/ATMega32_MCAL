/*
 *  ADC_Interface.h
 *  Created on: 18/10/2021
 *  Author: Mohamed_EL_Gallad
 *  Description: This file will contain the interface functions for the ADC module
 *  to enable the ADC functionality the following sequence MUST be followed
 *  	1- Initiate the ADC by calling ADC_Init()
 *  	2- Select the desired input channel and gain by calling ADC_SelectChanelAndGain()
 *  	3- Enable the ADC by calling ADC_Enable()
 *  	4- If either single conversion or free running modes is being used a conversion has to be started by calling ADC_StartConversion()
 *  	in case single conversion mode is being used the ADC_StartConversion() function has to be called each time a new ADC result is required
 *  	5- to get the conversion result the ADC_GetConvResult() function has to be called
 */

#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_

#include "ADC_Config.h"
#include "ADC_Private.h"

/**
 * RETURN     : VOID
 * PARAMETERS : VOID
 * DESCRIPTION: This function is used to initiate the ADC module and will set the following configurations
 * 			1-ADC Voltage reference
 * 			2-ADC Conversion result adjustment "right or left adjustment"
 * 			3-ADC mode of operation and trigger source
 * 			4-ADC prescaler value
 */
void ADC_Init (void);


/**
 * RETURN     : VOID
 * PARAMETERS : VOID
 * DESCRIPTION: This function is used to enable the ADC module and the ADC conversion complete interrupt
 */
void ADC_Enable(void);


/**
 * RETURN     : VOID
 * PARAMETERS : VOID
 * DESCRIPTION: This function is used to disable the ADC module and will also disable the ADC conversion complete interrupt
 */
void ADC_Disable(void);


/**
 * RETURN     : VOID
 * PARAMETERS : VOID
 * DESCRIPTION: This function is used to start the ADC conversion and it has to be used in the following cases:
 *		1- in case a single mode is used , it has to be called every time a conversion is needed and before the call of ADC_GetConvResult() function
 *		2- in case free running mode ,it must be called once to start conversion and start the free running mode
 */
void ADC_StartConversion(void);


/**
 * RETURN     : VOID
 * PARAMETERS : A u8 variable equals to a predefined MACROS represent the different combinations of channels and gains
 * DESCRIPTION: this function is used to select the input channel and gain depending on the passed MACRO by the user and it has to be one of the following:
 * 	   	 MUX0_SE_ADC0                   //Single ended input on ADC0
 *	 	 MUX1_SE_ADC1                   //Single ended input on ADC1
 *	 	 MUX2_SE_ADC2                   //Single ended input on ADC2
 *		 MUX3_SE_ADC3                   //Single ended input on ADC3
 *		 MUX4_SE_ADC4                   //Single ended input on ADC4
 *		 MUX5_SE_ADC5                   //Single ended input on ADC5
 *   	 MUX6_SE_ADC6                   //Single ended input on ADC6
 *	 	 MUX7_SE_ADC7                   //Single ended input on ADC7
 *		 MUX8_DIFF_PADC0_NADC0_G10X     //Differential ,Positive input on ADC0 , Negative input on ADC0 , Gain 10x
 *	 	 MUX9_DIFF_PADC1_NADC0_G10X     //Differential ,Positive input on ADC1 , Negative input on ADC0 , Gain 10x
 *		 MUX10_DIFF_PADC0_NADC0_G200X   //Differential ,Positive input on ADC0 , Negative input on ADC0 , Gain 200x
 *	 	 MUX11_DIFF_PADC1_NADC0_G200X   //Differential ,Positive input on ADC1 , Negative input on ADC0 , Gain 200x
 *	 	 MUX12_DIFF_PADC2_NADC2_G10X    //Differential ,Positive input on ADC2 , Negative input on ADC2 , Gain 10x
 *	     MUX13_DIFF_PADC3_NADC2_G10X    //Differential ,Positive input on ADC3 , Negative input on ADC2 , Gain 10x
 *	     MUX14_DIFF_PADC2_NADC2_G200X   //Differential ,Positive input on ADC2 , Negative input on ADC2 , Gain 200x
 *	 	 MUX15_DIFF_PADC3_NADC2_G200X   //Differential ,Positive input on ADC3 , Negative input on ADC2 , Gain 200x
 *	 	 MUX16_DIFF_PADC0_NADC1_G1X     //Differential ,Positive input on ADC0 , Negative input on ADC1 , Gain 1x
 *	 	 MUX17_DIFF_PADC1_NADC1_G1X     //Differential ,Positive input on ADC1 , Negative input on ADC1 , Gain 1x
 *	 	 MUX18_DIFF_PADC2_NADC1_G1X     //Differential ,Positive input on ADC2 , Negative input on ADC1 , Gain 1x
 *	 	 MUX19_DIFF_PADC3_NADC1_G1X     //Differential ,Positive input on ADC3 , Negative input on ADC1 , Gain 1x
 *	 	 MUX20_DIFF_PADC4_NADC1_G1X     //Differential ,Positive input on ADC4 , Negative input on ADC1 , Gain 1x
 *	 	 MUX21_DIFF_PADC5_NADC1_G1X     //Differential ,Positive input on ADC5 , Negative input on ADC1 , Gain 1x
 *	 	 MUX22_DIFF_PADC6_NADC1_G1X     //Differential ,Positive input on ADC6 , Negative input on ADC1 , Gain 1x
 *	 	 MUX23_DIFF_PADC7_NADC1_G1X     //Differential ,Positive input on ADC7 , Negative input on ADC1 , Gain 1x
 *	 	 MUX24_DIFF_PADC0_NADC2_G1X     //Differential ,Positive input on ADC0 , Negative input on ADC2 , Gain 1x
 *	 	 MUX25_DIFF_PADC1_NADC2_G1X     //Differential ,Positive input on ADC1 , Negative input on ADC2 , Gain 1x
 *	 	 MUX26_DIFF_PADC2_NADC2_G1X     //Differential ,Positive input on ADC2 , Negative input on ADC2 , Gain 1x
 *	 	 MUX27_DIFF_PADC3_NADC2_G1X     //Differential ,Positive input on ADC3 , Negative input on ADC2 , Gain 1x
 *	 	 MUX28_DIFF_PADC4_NADC2_G1X     //Differential ,Positive input on ADC4 , Negative input on ADC2 , Gain 1x
 *		 MUX29_DIFF_PADC5_NADC2_G1X     //Differential ,Positive input on ADC5 , Negative input on ADC2 , Gain 1x
 *	 	 MUX30_VBG_1V22                 //Single ended input = Vbandgap 1.22V
 *	 	 MUX31_GND                      //Single ended input =GND
 */
void ADC_SelectChanelAndGain(u8 MuxValue);


/**
 * RETURN     : A u16 variable that will contain the result of the ADC conversion
 * PARAMETERS : VOID
 * DESCRIPTION: The call of this function will return the result of the ADC conversion
 */
u16  ADC_GetConvResult(void);


/**
 * RETURN     : VOID
 * PARAMETERS : A pointer to the user's function witch must have a void return and void parameter
 * DESCRIPTION: This function is used to mount the user's function on the ADC conversion complete interrupt to be executed when it occurs
 */
void ADC_ExecuteUserFnOnInterrupt(void (*ADC_ConvComp)(void));


/**
 * RETURN     : VOID
 * PARAMETERS : VOID
 * DESCRIPTION: This function is used to unmount the user's function from the ADC conversion complete interrupt
 */
void ADC_DisableUserFN(void);


#endif /* ADC_INTERFACE_H_ */
