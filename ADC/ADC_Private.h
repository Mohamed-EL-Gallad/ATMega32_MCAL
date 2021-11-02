/*
 * ADC_Private.h
 *
 *  Created on: 18/10/2021
 *  Author: Mohamed_EL_Gallad
 */

#ifndef ADC_PRIVATE_H_
#define ADC_PRIVATE_H_

//ADCSRA Register Bits
#define ADPS0  ((u8)0)
#define ADPS1  ((u8)1)
#define ADPS2  ((u8)2)
#define ADIE   ((u8)3)
#define ADIF   ((u8)4)
#define ADATE  ((u8)5)
#define ADSC   ((u8)6)
#ifndef ADEN
#define ADEN   ((u8)7)
#endif


//ADMUX Register Bits
#ifndef MUX0
#define MUX0   ((u8)0)
#endif
#ifndef MUX1
#define MUX1   ((u8)1)
#endif
#ifndef MUX2
#define MUX2   ((u8)2)
#endif
#define MUX3   ((u8)3)
#define MUX4   ((u8)4)
#define ADLAR  ((u8)5)
#define REFS0  ((u8)6)
#define REFS1  ((u8)7)


//SFIOR Register Bits
#define ADTS0  ((u8)5)
#define ADTS1  ((u8)6)
#define ADTS2  ((u8)7)


//ADC Auto Trigger Source
#define FREE_RUNNING_MODE           0
#define ANALOG_CPMPARATOR           1
#define EXTERNAL_INT0               2
#define TIMER0_COMP_MATCH           3
#define TIMER0_OVERFLOW             4
#define TIMER1_CHB_COMP_MATCH       5
#define TIMER1_OVERFLOW             6
#define TIMER1_INPUT_CAPTURE        7


//Input Channel and Gain Selections
#define MUX0_SE_ADC0                        ((u8)0)   //Single ended input on ADC0
#define MUX1_SE_ADC1                        ((u8)1)   //Single ended input on ADC1
#define MUX2_SE_ADC2                        ((u8)2)   //Single ended input on ADC2
#define MUX3_SE_ADC3                        ((u8)3)   //Single ended input on ADC3
#define MUX4_SE_ADC4                        ((u8)4)   //Single ended input on ADC4
#define MUX5_SE_ADC5                        ((u8)5)   //Single ended input on ADC5
#define MUX6_SE_ADC6                        ((u8)6)   //Single ended input on ADC6
#define MUX7_SE_ADC7                        ((u8)7)   //Single ended input on ADC7
#define MUX8_DIFF_PADC0_NADC0_G10X          ((u8)8)   //Differential ,Positive input on ADC0 , Negative input on ADC0 , Gain 10x
#define MUX9_DIFF_PADC1_NADC0_G10X          ((u8)9)   //Differential ,Positive input on ADC1 , Negative input on ADC0 , Gain 10x
#define MUX10_DIFF_PADC0_NADC0_G200X        ((u8)10)  //Differential ,Positive input on ADC0 , Negative input on ADC0 , Gain 200x
#define MUX11_DIFF_PADC1_NADC0_G200X        ((u8)11)  //Differential ,Positive input on ADC1 , Negative input on ADC0 , Gain 200x
#define MUX12_DIFF_PADC2_NADC2_G10X         ((u8)12)  //Differential ,Positive input on ADC2 , Negative input on ADC2 , Gain 10x
#define MUX13_DIFF_PADC3_NADC2_G10X         ((u8)13)  //Differential ,Positive input on ADC3 , Negative input on ADC2 , Gain 10x
#define MUX14_DIFF_PADC2_NADC2_G200X        ((u8)14)  //Differential ,Positive input on ADC2 , Negative input on ADC2 , Gain 200x
#define MUX15_DIFF_PADC3_NADC2_G200X        ((u8)15)  //Differential ,Positive input on ADC3 , Negative input on ADC2 , Gain 200x
#define MUX16_DIFF_PADC0_NADC1_G1X          ((u8)16)  //Differential ,Positive input on ADC0 , Negative input on ADC1 , Gain 1x
#define MUX17_DIFF_PADC1_NADC1_G1X          ((u8)17)  //Differential ,Positive input on ADC1 , Negative input on ADC1 , Gain 1x
#define MUX18_DIFF_PADC2_NADC1_G1X          ((u8)18)  //Differential ,Positive input on ADC2 , Negative input on ADC1 , Gain 1x
#define MUX19_DIFF_PADC3_NADC1_G1X          ((u8)19)  //Differential ,Positive input on ADC3 , Negative input on ADC1 , Gain 1x
#define MUX20_DIFF_PADC4_NADC1_G1X          ((u8)20)  //Differential ,Positive input on ADC4 , Negative input on ADC1 , Gain 1x
#define MUX21_DIFF_PADC5_NADC1_G1X          ((u8)21)  //Differential ,Positive input on ADC5 , Negative input on ADC1 , Gain 1x
#define MUX22_DIFF_PADC6_NADC1_G1X          ((u8)22)  //Differential ,Positive input on ADC6 , Negative input on ADC1 , Gain 1x
#define MUX23_DIFF_PADC7_NADC1_G1X          ((u8)23)  //Differential ,Positive input on ADC7 , Negative input on ADC1 , Gain 1x
#define MUX24_DIFF_PADC0_NADC2_G1X          ((u8)24)  //Differential ,Positive input on ADC0 , Negative input on ADC2 , Gain 1x
#define MUX25_DIFF_PADC1_NADC2_G1X          ((u8)25)  //Differential ,Positive input on ADC1 , Negative input on ADC2 , Gain 1x
#define MUX26_DIFF_PADC2_NADC2_G1X          ((u8)26)  //Differential ,Positive input on ADC2 , Negative input on ADC2 , Gain 1x
#define MUX27_DIFF_PADC3_NADC2_G1X          ((u8)27)  //Differential ,Positive input on ADC3 , Negative input on ADC2 , Gain 1x
#define MUX28_DIFF_PADC4_NADC2_G1X          ((u8)28)  //Differential ,Positive input on ADC4 , Negative input on ADC2 , Gain 1x
#define MUX29_DIFF_PADC5_NADC2_G1X          ((u8)29)  //Differential ,Positive input on ADC5 , Negative input on ADC2 , Gain 1x
#define MUX30_VBG_1V22                      ((u8)30)  //Single ended input = Vbandgap 1.22V
#define MUX31_GND                           ((u8)31)  //Single ended input =GND




#endif /* ADC_PRIVATE_H_ */
