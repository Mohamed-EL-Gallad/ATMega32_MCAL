/*
 *  ADC_Prog.c
 *  Created on:18/10/2021
 *  Author:Mohamed_EL_Gallad
 *  Description : this file will contain the functions implementation for the ADC module
 */
#include "STD_types.h"
#include "Mega32_reg.h"
#include "REG_utils.h"
#include "ADC_Interface.h"

#define ADMUX_CHANNEL_BITS_MASK  ((u8)0XE0) //this Macro is used to clear the ADC channel selection bits

static void (*ADC_ConvCompISR_PTR)(void)=NULL; //static pointer to store the address of the user's function to be executed on ADC interrupt
volatile static u16 ADC_ConversionRes=0; // a variable to store the ADC conversion result
volatile static u8 NewDataStoredFlg=FALSE; // a flag that will indicate that a new conversion result is ready to be read


/**
 * RETURN     : VOID
 * PARAMETERS : VOID
 * DESCRIPTION: This function is used to initiate the ADC module and will set the following configurations
 * 			1-ADC Voltage reference
 * 			2-ADC Conversion result adjustment "right or left adjustment"
 * 			3-ADC mode of operation and trigger source
 * 			4-ADC prescaler value
 */
void ADC_Init (void)
{
	//ADC voltage reference selection
	#if ADC_VOLTAGE_REF == REF_AREF       //voltage reference on AREF pin
		ClearRegisterBit(ADMUX , REFS0);  //REFS0 =0
		ClearRegisterBit(ADMUX , REFS1);  //REFS1 =0
	#elif ADC_VOLTAGE_REF == REF_AVCC     //voltage reference on AVCC pin
		SetRegisterBit  (ADMUX , REFS0);  //REFS0=1
		ClearRegisterBit(ADMUX , REFS1);  //REFS1=0
	#elif ADC_VOLTAGE_REF == REF_INT_2V56 //voltage reference = internal 2.56V
		SetRegisterBit (ADMUX , REFS0);   //REFS0=1
		SetRegisterBit (ADMUX , REFS1);   //REFS1=1
	#endif

	//ADC Conversion result adjustment
	#if RESULT_STORAGE_DIR == RESULT_RIGHT_ADJUSTED
		ClearRegisterBit(ADMUX , ADLAR);
	#elif RESULT_STORAGE_DIR ==RESULT_LEFT_ADJUSTED
		SetRegisterBit (ADMUX , ADLAR);
	#endif

	//ADC Operation Mode
	#if ADC_OPERATION_MODE == FREE_RUNNING_MODE //Free Running mode
		SetRegisterBit(ADCSRA , ADATE);   //ADATE=1 , Enable ADC Auto Trigger Source
		ClearRegisterBit(SFIOR , ADTS0);  //ADTS0=0
		ClearRegisterBit(SFIOR , ADTS1);  //ADTS1=0
		ClearRegisterBit(SFIOR , ADTS2);  //ADTS2=0
	#elif ADC_OPERATION_MODE == ANALOG_CPMPARATOR //Analog Comparator
		SetRegisterBit(ADCSRA , ADATE);  //ADATE=1 , Enable ADC Auto Trigger Source
		SetRegisterBit  (SFIOR , ADTS0); //ADTS0=1
		ClearRegisterBit(SFIOR , ADTS1); //ADTS1=0
		ClearRegisterBit(SFIOR , ADTS2); //ADTS2=0
	#elif ADC_OPERATION_MODE == EXTERNAL_INT0 //External Interrupt Request 0
		SetRegisterBit(ADCSRA , ADATE);  //ADATE=1 , Enable ADC Auto Trigger Source
		ClearRegisterBit(SFIOR , ADTS0); //ADTS0=0
		SetRegisterBit  (SFIOR , ADTS1); //ADTS1=1
		ClearRegisterBit(SFIOR , ADTS2); //ADTS2=0
	#elif ADC_OPERATION_MODE == TIMER0_COMP_MATCH //Timer/Counter0 Compare Match
		SetRegisterBit(ADCSRA , ADATE);  //ADATE=1 , Enable ADC Auto Trigger Source
		SetRegisterBit  (SFIOR , ADTS0); //ADTS0=1
		SetRegisterBit  (SFIOR , ADTS1); //ADTS1=1
		ClearRegisterBit(SFIOR , ADTS2); //ADTS2=0
	#elif ADC_OPERATION_MODE == TIMER0_OVERFLOW //Timer/Counter0 Overflow
		SetRegisterBit(ADCSRA , ADATE);  //ADATE=1 , Enable ADC Auto Trigger Source
		ClearRegisterBit(SFIOR , ADTS0); //ADTS0=0
		ClearRegisterBit(SFIOR , ADTS1); //ADTS1=0
		SetRegisterBit  (SFIOR , ADTS2); //ADTS2=1
	#elif ADC_OPERATION_MODE == TIMER1_CHB_COMP_MATCH //Timer/Counter Compare Match B
		SetRegisterBit(ADCSRA , ADATE);  //ADATE=1 , Enable ADC Auto Trigger Source
		SetRegisterBit  (SFIOR , ADTS0); //ADTS0=1
		ClearRegisterBit(SFIOR , ADTS1); //ADTS1=0
		SetRegisterBit  (SFIOR , ADTS2); //ADTS2=1
	#elif ADC_OPERATION_MODE == TIMER1_OVERFLOW //Timer/Counter1 Overflow
		SetRegisterBit(ADCSRA , ADATE);  //ADATE=1 , Enable ADC Auto Trigger Source
		ClearRegisterBit(SFIOR , ADTS0); //ADTS0=0
		SetRegisterBit  (SFIOR , ADTS1); //ADTS1=1
		SetRegisterBit  (SFIOR , ADTS2); //ADTS2=1
	#elif ADC_OPERATION_MODE == TIMER1_INPUT_CAPTURE //Timer/Counter1 Capture Event
		SetRegisterBit(ADCSRA , ADATE);  //ADATE=1 , Enable ADC Auto Trigger Source
		SetRegisterBit(SFIOR , ADTS0);   //ADTS0=1
		SetRegisterBit(SFIOR , ADTS1);   //ADTS1=1
		SetRegisterBit(SFIOR , ADTS2);   //ADTS2=1
	#elif ADC_OPERATION_MODE == SINGLE_CONVERSION //Single Conversion
		ClearRegisterBit(ADCSRA , ADATE);  //ADATE=0 , Disable ADC Auto Trigger Source
	#endif

	//ADC Prescaler Selection
	#if ADC_PRESCLARE == ADC_CPU_CLK_DIV_BY2     //ADC freq = CPU_FREQ/2 ADCSRA
		SetRegisterBit  (ADCSRA , ADPS0); //ADPS0=1
		ClearRegisterBit(ADCSRA , ADPS1); //ADPS1=0
		ClearRegisterBit(ADCSRA , ADPS2); //ADPS2=0
	#elif ADC_PRESCLARE == ADC_CPU_CLK_DIV_BY4   //ADC freq = CPU_FREQ/4
		ClearRegisterBit(ADCSRA , ADPS0); //ADPS0=0
		SetRegisterBit  (ADCSRA , ADPS1); //ADPS1=1
		ClearRegisterBit(ADCSRA , ADPS2); //ADPS2=0
	#elif ADC_PRESCLARE == ADC_CPU_CLK_DIV_BY8   //ADC freq = CPU_FREQ/8
		SetRegisterBit  (ADCSRA , ADPS0); //ADPS0=1
		SetRegisterBit  (ADCSRA , ADPS1); //ADPS1=1
		ClearRegisterBit(ADCSRA , ADPS2); //ADPS2=0
	#elif ADC_PRESCLARE == ADC_CPU_CLK_DIV_BY16  //ADC freq = CPU_FREQ/16
		ClearRegisterBit(ADCSRA , ADPS0); //ADPS0=0
		ClearRegisterBit(ADCSRA , ADPS1); //ADPS1=0
		SetRegisterBit  (ADCSRA , ADPS2); //ADPS2=1
	#elif ADC_PRESCLARE == ADC_CPU_CLK_DIV_BY32  //ADC freq = CPU_FREQ/32
		SetRegisterBit  (ADCSRA , ADPS0); //ADPS0=1
		ClearRegisterBit(ADCSRA , ADPS1); //ADPS1=0
		SetRegisterBit  (ADCSRA , ADPS2); //ADPS2=1
	#elif ADC_PRESCLARE == ADC_CPU_CLK_DIV_BY64  //ADC freq = CPU_FREQ/64
		ClearRegisterBit(ADCSRA , ADPS0); //ADPS0=0
		SetRegisterBit  (ADCSRA , ADPS1); //ADPS1=1
		SetRegisterBit  (ADCSRA , ADPS2); //ADPS2=1
	#elif ADC_PRESCLARE == ADC_CPU_CLK_DIV_BY128 //ADC freq = CPU_FREQ/128
		SetRegisterBit(ADCSRA , ADPS0);   //ADPS0=1
		SetRegisterBit(ADCSRA , ADPS1);   //ADPS1=1
		SetRegisterBit(ADCSRA , ADPS2);   //ADPS2=1
	#endif
}


/**
 * RETURN     : VOID
 * PARAMETERS : VOID
 * DESCRIPTION: This function is used to enable the ADC module and the ADC conversion complete interrupt
 */
void ADC_Enable(void)
{
	SetRegisterBit(SREG,7); //ensure that global interrupt is enabled
	SetRegisterBit(ADCSRA , ADIE); //Enable ADC conversion complete ISR
	SetRegisterBit(ADCSRA , ADEN); // ADEN=1 , Enable ADC
}


/**
 * RETURN     : VOID
 * PARAMETERS : VOID
 * DESCRIPTION: This function is used to disable the ADC module and will also disable the ADC conversion complete interrupt
 */
void ADC_Disable(void)
{
	ClearRegisterBit(ADCSRA , ADEN); // ADEN=0 , Disable ADC
	ClearRegisterBit(ADCSRA , ADIE); //Disable ADC conversion complete ISR
}


/**
 * RETURN     : VOID
 * PARAMETERS : VOID
 * DESCRIPTION: This function is used to start the ADC conversion and it has to be used in the following cases:
 *		1- in case a single mode is used , it has to be called every time a conversion is needed and before the call of ADC_GetConvResult() function
 *		2- in case free running mode ,it must be called once to start conversion and start the free running mode
 */
void ADC_StartConversion(void)
{
	SetRegisterBit(ADCSRA , ADSC); // ADSC=1 , ADC Start Conversion
}


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
 *
 */
void ADC_SelectChanelAndGain(u8 MuxValue)
{
	/*To change the ADC channel in a safe manner and ensure that the upcoming conversion will be related to the selected channel:
	 * 1-Disable the ADC if it was Enabled
	 * 2-clear the last conversion value
	 * 3-change the ADC channel
	 * 4-return the ADC to it's previous state
	 * 5-If the ADC mode was free running mode then call ADC_StartConversion after Re-Enable the ADC*/

	u8 ADC_RunStateFlg=FALSE; //a flag to indicate the ADC running state before calling ADC_SelectChanelAndGain function
	if(GetRegisterBit(ADCSRA , ADEN))
	{
	ADC_Disable(); //Disable ADC
	ADC_RunStateFlg=TRUE;
	}

	ADC_ConversionRes=0; //Clear the last stored conversion of the previous channel
	NewDataStoredFlg=FALSE; //clear The NewDataStoredFlg

	switch (MuxValue)
	{
	case MUX0_SE_ADC0: //Single ended input on ADC0
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX0_SE_ADC0 ;
		break;

	case MUX1_SE_ADC1: //Single ended input on ADC1
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX1_SE_ADC1 ;
		break;

	case MUX2_SE_ADC2: //Single ended input on ADC2
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX2_SE_ADC2 ;
		break;

	case MUX3_SE_ADC3: //Single ended input on ADC3
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX3_SE_ADC3 ;
		break;

	case MUX4_SE_ADC4: //Single ended input on ADC4
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX4_SE_ADC4 ;
		break;

	case MUX5_SE_ADC5: //Single ended input on ADC5
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX5_SE_ADC5 ;
		break;

	case MUX6_SE_ADC6: //Single ended input on ADC6
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX6_SE_ADC6 ;
		break;

	case MUX7_SE_ADC7: //Single ended input on ADC7
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX7_SE_ADC7 ;
		break;

	case MUX8_DIFF_PADC0_NADC0_G10X: //Differential ,Positive input on ADC0 , Negative input on ADC0 , Gain 10x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX8_DIFF_PADC0_NADC0_G10X ;
		break;

	case MUX9_DIFF_PADC1_NADC0_G10X:  //Differential ,Positive input on ADC1 , Negative input on ADC0 , Gain 10x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX9_DIFF_PADC1_NADC0_G10X ;
		break;

	case MUX10_DIFF_PADC0_NADC0_G200X: //Differential ,Positive input on ADC0 , Negative input on ADC0 , Gain 200x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX10_DIFF_PADC0_NADC0_G200X ;
		break;

	case MUX11_DIFF_PADC1_NADC0_G200X: //Differential ,Positive input on ADC1 , Negative input on ADC0 , Gain 200x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX11_DIFF_PADC1_NADC0_G200X ;
		break;
	case MUX12_DIFF_PADC2_NADC2_G10X: //Differential ,Positive input on ADC2 , Negative input on ADC2 , Gain 10x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX12_DIFF_PADC2_NADC2_G10X ;
		break;

	case MUX13_DIFF_PADC3_NADC2_G10X: //Differential ,Positive input on ADC3 , Negative input on ADC2 , Gain 10x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX13_DIFF_PADC3_NADC2_G10X ;
		break;

	case MUX14_DIFF_PADC2_NADC2_G200X: //Differential ,Positive input on ADC2 , Negative input on ADC2 , Gain 200x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX14_DIFF_PADC2_NADC2_G200X ;
		break;

	case MUX15_DIFF_PADC3_NADC2_G200X:  //Differential ,Positive input on ADC3 , Negative input on ADC2 , Gain 200x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX15_DIFF_PADC3_NADC2_G200X ;
		break;

	case MUX16_DIFF_PADC0_NADC1_G1X: //Differential ,Positive input on ADC0 , Negative input on ADC1 , Gain 1x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX16_DIFF_PADC0_NADC1_G1X ;
		break;

	case MUX17_DIFF_PADC1_NADC1_G1X: //Differential ,Positive input on ADC1 , Negative input on ADC1 , Gain 1x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX17_DIFF_PADC1_NADC1_G1X ;
		break;

	case MUX18_DIFF_PADC2_NADC1_G1X: //Differential ,Positive input on ADC2 , Negative input on ADC1 , Gain 1x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX18_DIFF_PADC2_NADC1_G1X ;
		break;

	case MUX19_DIFF_PADC3_NADC1_G1X: //Differential ,Positive input on ADC3 , Negative input on ADC1 , Gain 1x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX19_DIFF_PADC3_NADC1_G1X ;
		break;

	case MUX20_DIFF_PADC4_NADC1_G1X: //Differential ,Positive input on ADC4 , Negative input on ADC1 , Gain 1x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX20_DIFF_PADC4_NADC1_G1X ;
		break;

	case MUX21_DIFF_PADC5_NADC1_G1X: //Differential ,Positive input on ADC5 , Negative input on ADC1 , Gain 1x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX21_DIFF_PADC5_NADC1_G1X ;
		break;

	case MUX22_DIFF_PADC6_NADC1_G1X: //Differential ,Positive input on ADC6 , Negative input on ADC1 , Gain 1x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX22_DIFF_PADC6_NADC1_G1X ;
		break;

	case MUX23_DIFF_PADC7_NADC1_G1X: //Differential ,Positive input on ADC7 , Negative input on ADC1 , Gain 1x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX23_DIFF_PADC7_NADC1_G1X ;
		break;

	case MUX24_DIFF_PADC0_NADC2_G1X: //Differential ,Positive input on ADC0 , Negative input on ADC2 , Gain 1x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX24_DIFF_PADC0_NADC2_G1X ;
		break;

	case MUX25_DIFF_PADC1_NADC2_G1X: //Differential ,Positive input on ADC1 , Negative input on ADC2 , Gain 1x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX25_DIFF_PADC1_NADC2_G1X ;
		break;

	case MUX26_DIFF_PADC2_NADC2_G1X: //Differential ,Positive input on ADC2 , Negative input on ADC2 , Gain 1x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX26_DIFF_PADC2_NADC2_G1X ;
		break;

	case MUX27_DIFF_PADC3_NADC2_G1X: //Differential ,Positive input on ADC3 , Negative input on ADC2 , Gain 1x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX27_DIFF_PADC3_NADC2_G1X ;
		break;

	case MUX28_DIFF_PADC4_NADC2_G1X: //Differential ,Positive input on ADC4 , Negative input on ADC2 , Gain 1x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX28_DIFF_PADC4_NADC2_G1X ;
		break;

	case MUX29_DIFF_PADC5_NADC2_G1X: //Differential ,Positive input on ADC5 , Negative input on ADC2 , Gain 1x
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX29_DIFF_PADC5_NADC2_G1X ;
		break;

	case MUX30_VBG_1V22: //Single ended input = Vbandgap 1.22V
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX30_VBG_1V22 ;
		break;

	case MUX31_GND: //Single ended input =GND
		ADMUX =(ADMUX & ADMUX_CHANNEL_BITS_MASK) | MUX31_GND ;
		break;

	default:
		break;
	}


	if(ADC_RunStateFlg) //Re-Enable the ADC if it was Enabled before calling this function
	{
	ADC_Enable(); //Re-Enable ADC
	#if ADC_OPERATION_MODE == FREE_RUNNING_MODE
	ADC_StartConversion(); //in case free running mode is being used , re-initiate the conversion .
	#endif
	}

}


/**
 * RETURN     : A u16 variable that will contain the result of the ADC conversion
 * PARAMETERS : VOID
 * DESCRIPTION: The call of this function will return the result of the ADC conversion
 */
u16 ADC_GetConvResult(void)
{
	while(!NewDataStoredFlg); //wait till the conversion result is stored in ADC_ConversionRes variable
	NewDataStoredFlg=FALSE;   //set the flag value to FALSE to indicate that there is no new conversion result stored.
	return ADC_ConversionRes; //return the ADC conversion result
}


/**
 * RETURN     : VOID
 * PARAMETERS : A pointer to the user's function witch must have a void return and void parameter
 * DESCRIPTION: This function is used to mount the user's function on the ADC conversion complete interrupt to be executed when it occurs
 */
void ADC_ExecuteUserFnOnInterrupt(void (*ADC_ConvComp)(void))
{
	if(ADC_ConvComp)
	{
		ADC_ConvCompISR_PTR = ADC_ConvComp; //mount the user's function
	}
}


/**
 * RETURN     : VOID
 * PARAMETERS : VOID
 * DESCRIPTION: This function is used to unmount the user's function from the ADC conversion complete interrupt
 */
void ADC_DisableUserFN(void)
{
	ADC_ConvCompISR_PTR= NULL;  //unmount the user's function by setting the ADC_ConvCompISR_PTR to NULL
}


/**
 *  ADC conversion complete interrupt
 */
void __vector_16 (void) __attribute__ ((signal,used));
void __vector_16 (void)
{
	#if RESULT_STORAGE_DIR == RESULT_LEFT_ADJUSTED
	ADC_ConversionRes = (ADCL >>6); //read the first 2bits stored in ADCL
	ADC_ConversionRes |=(ADCH <<2); //read the rest  8bits stored in ADCH
	#elif RESULT_STORAGE_DIR == RESULT_RIGHT_ADJUSTED
	ADC_ConversionRes = ADCL ;    //read the first 8bits from ADCL
	ADC_ConversionRes |=(ADCH<<8);//read the reset 2bits from ADCH
	#endif

	NewDataStoredFlg=TRUE; //set the flag value to TRUE to indicate that a new conversion result is ready to be read

	if(ADC_ConvCompISR_PTR)
	{
		ADC_ConvCompISR_PTR(); //execute user's function when ADC conversion complete interrupt occurs
	}
}

