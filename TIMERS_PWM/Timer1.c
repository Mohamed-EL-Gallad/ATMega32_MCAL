/*
 * Timer1.c
 *
 *  Created on: ٣١‏/٠١‏/٢٠٢١
 *      Author: Mohamed_EL_Gallad
 */

#include "STD_types.h"
#include "Mega32_reg.h"
#include "TimersConfig.h"
#include "REG_utils.h"
#include "Timers_Interface.h"


#define T1_PRESCALER_MASK 0xF8

volatile static u32 ICU_TonTicks=0,ICU_ToffTicks=0;
volatile u16 T1_OVF_Counter;
volatile static u8 ICU_EdgeFlag=0;


static void      (*Timer1_OverFlowIntFunc)();
static void (*Timer1_CHA_CompMatchIntFunc)();
static void (*Timer1_CHB_CompMatchIntFunc)();
//hikle
//556
//9989569
static void T1_OC1AB_NonPWMCtrl(u8 T1_OC1A, u8 T1_OC1B);


void Timer1_Enable(void){
	TCCR1B |=(TCCR1B & T1_PRESCALER_MASK)|TIMER1_PRESCALER; //set the operational prescaler to initiate the timer
}

void Timer1_Stop(void)
{
	TCCR1B &=T1_PRESCALER_MASK; //set the prescaler value to zero, stop the timer
	TCNT1H=0x0; //set timer1 counter high byte to zero
	TCNT1L=0x0; //set timer1 counter low byte to zero
}

void Timer1_CycleStartCountAt(u16 T1_CycleInitValue)
{
	TCNT1H =((u8)(T1_CycleInitValue & 0xFF00)); //store the high byte of T1_CycleInitValue in TCNT1H register
	TCNT1L =((u8)(T1_CycleInitValue & 0x00FF)); //store teh low byte of T1_CycleInitValue in TCNT1L register
}

void Timer1_NormalModeInit(u8 OC1A_OPModeNormal,u8 OC1B_OPModeNormal)
{
	TCCR1A &=~((1<<0) | (1<<1)); // WGM10=0 WGM11=0
	TCCR1B &=~((1<<3) | (1<<4)); // WGM12=0 WGM13=0
	T1_OC1AB_NonPWMCtrl(OC1A_OPModeNormal,OC1B_OPModeNormal);

}

void Timer1_CountDownOverFlows(u32 T1_OverFLowsNo)
{
	while (T1_OverFLowsNo)
	{
		while(!(TIFR & 0x04)); //halt till the timer1 overflow flag flips to 1
		TIFR |=(1<<2);
		T1_OverFLowsNo--;
	}
}

void Timer1_CTCModeInit(u8 OC1A_OPModeCTC, u8 OC1B_OPModeCTC)
{
	switch(T1_COMPMATCH_OPMODE)
	{
	case 0: //top value is OCR1A
		TCCR1B |= (1<<3); //WGM12=1
		TCCR1B &=~(1<<4); //WGM13=0
		TCCR1A &=~((1<<0) |(1<<1)); //WGM10=0 WGM11=0
		break;
	case 1: //top value is ICR1
		TCCR1B |= (1<<3) | (1<<4); //WGM12=1 WGM13=1
		TCCR1A &=~((1<<0) |(1<<1)); //WGM10=0 WGM11=0
		break;
	default:
		//noop
		break;
	}
	T1_OC1AB_NonPWMCtrl(OC1A_OPModeCTC,OC1B_OPModeCTC);

}
static void T1_OVFcounterFunc(void)
{
	T1_OVF_Counter++;
}


static void T1_OC1AB_NonPWMCtrl(u8 T1_OC1A, u8 T1_OC1B)
{
	switch(T1_OC1A) //define the operation mode for OC1A pin
		{
		 	 case 0: //disconnected , normal port operation
		 		TCCR1A &=~((1<<6) | (1<<7));  //COM1A1=0 COM1A0=0
		 		break;
		 	 case 1: //TOGGLE ON MATCH
		 		TCCR1A |=(1<<6);   //COM1A0=1
				TCCR1A &=~(1<<7);  //COM1A1=0
				SetPinDIR(3,5,1); //define PD5 pin as output
				break;
		 	 case 3: //CLEAR ON MATCH
		 		TCCR1A &=~(1<<6);   //COM1A0=0
		 		TCCR1A |=(1<<7);    //COM1A1=1
		 		SetPinDIR(3,5,1); //define PD5 pin as output
		 		break;
		 	 case 4: //SET ON MATCH
		 		TCCR1A |=((1<<6) | (1<<7)); //COM1A1=1 COM1A0=1
		 		SetPinDIR(3,5,1); //define PD5 pin as output
		 		break;
		 	 default:
		 		 //noop
		 		 break;
		}

		switch(T1_OC1B) //define the operation mode for OC1B pin
			{
			 case 0: //disconnected , normal port operation
				TCCR1A &=~((1<<4) | (1<<5));  //COM1B1=0 COM1B0=0
				break;
			 case 1: //TOGGLE ON MATCH
				TCCR1A |=(1<<4);   //COM1B0=1
				TCCR1A &=~(1<<5);  //COM1B1=0
				SetPinDIR(3,4,1); //define PD4 pin as output
				break;
			 case 3: //CLEAR ON MATCH
				TCCR1A &=~(1<<4);   //COM1B0=0
				TCCR1A |=(1<<5);    //COM1B1=1
				SetPinDIR(3,4,1); //define PD4 pin as output
				break;
			 case 4: //SET ON MATCH
				TCCR1A |=((1<<4) | (1<<5)); //COM1B1=1 COM1B0=1
				SetPinDIR(3,4,1); //define PD4 pin as output
				break;
			 default:
				 //noop
				 break;
			}
}
void Timer1_CHA_SetCompValue(u16 T1_CHA_CompareMatchValue)
{
#if T1_COMPMATCH_OPMODE==T1_COMPMATCH_MODE2 //in case of OCR1A is used to set the TCNT1 top value this function will have no effect if used
	    OCR1AH =((T1_CHA_CompareMatchValue & 0xFF00)>>8);
		OCR1AL=(T1_CHA_CompareMatchValue & 0x00FF);
#else
#warning "the use of this function will have no effect as OCR1A used to set the timer's top value"
#endif
}

void Timer1_SetCompMatchTopVal (u16 T1_CTC_TopValue)
{
	switch(T1_COMPMATCH_OPMODE)
	{
	case 0://top value is OCR1A
		OCR1AH =((T1_CTC_TopValue & 0xFF00)>>8);
		OCR1AL=(T1_CTC_TopValue & 0x00FF);
		break;
	case 1://top value is ICR1
		ICR1H =((T1_CTC_TopValue & 0xFF00)>>8);
		ICR1L=(T1_CTC_TopValue & 0x00FF);
		break;
	default:
		//noop
		break;
	}
}


void Timer1_CHB_SetCompValue(u16 T1_CHB_CompareMatchValue)
{
	OCR1BH =((T1_CHB_CompareMatchValue & 0xFF00)>>8);
	OCR1BL=(T1_CHB_CompareMatchValue & 0x00FF);

}

void Timer1_ExecuteOnOverFlow(void (*T1_OVFUserFunction)(void))
{
	if(T1_OVFUserFunction!=NULL)
			{
			  SREG  |=(1<<7); //make sure that the global interrupt is enabled
			  TIMSK |=(1<<2); //enable timer1 overflow interrupt
			  Timer1_OverFlowIntFunc=T1_OVFUserFunction;
			}
	else
			{
			 TIMSK &=~(1<<2); //disable timer1 overflow interrupt
			}
}


void Timer1_ICUGetEventData(f32 *TonTime , f32 *DutyCycle ,u16 *Freq)
{


	TIFR |=(1<<5); //reset input capture flag to avoid accidental interrupt

	Timer1_ExecuteOnOverFlow(&T1_OVFcounterFunc); //mount overflows counter function
    Timer1_Enable(); //enable timer1


	SREG  |=(1<<7); //make sure that the global interrupt is enabled
	TIMSK |=(1<<5); //enable timer1 input capture event interrupt

	while(ICU_TonTicks==0 || ICU_ToffTicks==0); //wait until both ton and toff ticks are being calculated

	TIMSK &=~(1<<5); //disable timer1 input capture event interrupt
	Timer1_Stop();
	Timer1_OVF_UserFnDisable();


	*TonTime=ICU_TonTicks*((f32)1/Timer1_OP_CLK)*1000000; //calculate TonTime in microseconds

    if((ICU_TonTicks+ICU_ToffTicks)!=0) //avoid division by zero
	{
		*DutyCycle=((float)ICU_TonTicks/(ICU_TonTicks+ICU_ToffTicks))*100;
	}

	else
	{
		*DutyCycle=0;

	}

	*Freq=Timer1_OP_CLK/(ICU_ToffTicks+ICU_TonTicks);



	ICU_TonTicks=0; //reset the Ton ticks of the last captured event
	ICU_ToffTicks=0; //reset the Toff ticks of the last captured event
	ICU_EdgeFlag=0;  //reset the edge flag to ensure the ICU ISR to be executed according to the required rhythm
	TCCR1B &=~(1<<6); //set the ICU trigger edge to default value 0


}



void Timer1_CHA_ExecuteOnCompMatch(void (*T1_CHA_CompMatch_UserFunction)(void))
{
	if(T1_CHA_CompMatch_UserFunction!=NULL)
			{
			  SREG  |=(1<<7); //make sure that the global interrupt is enabled
			  TIMSK |=(1<<4); //enable Channel A compare match interrupt
			  Timer1_CHA_CompMatchIntFunc=T1_CHA_CompMatch_UserFunction;
			}
	else
			{
			 TIMSK &=~(1<<4); //disable Channel A compare match interrupt
			}
}

void Timer1_CHB_ExecuteOnCompMatch(void (*T1_CHB_CompMatch_UserFunction)(void))
{
	if(T1_CHB_CompMatch_UserFunction!=NULL)
			{
			  SREG  |=(1<<7); //make sure that the global interrupt is enabled
			  TIMSK |=(1<<3); //enable channel B compare match interrupt
			  Timer1_CHB_CompMatchIntFunc=T1_CHB_CompMatch_UserFunction;
			}
	else
			{
			 TIMSK &=~(1<<3); //disable channel B compare match interrupt
			}

}

void Timer1_OVF_UserFnDisable(void)
{
	TIMSK &=~(1<<2); //disable Channel A compare match interrupt
	Timer1_OverFlowIntFunc=NULL;
}


void Timer1_CHACompMatch_UserFnDisable(void)
{
	TIMSK &=~(1<<4); //disable Channel A compare match interrupt
	Timer1_CHA_CompMatchIntFunc=NULL;
}

void Timer1_CHBCompMatch_UserFnDisable(void)
{
	TIMSK &=~(1<<3); //disable channel B compare match interrupt
	Timer1_CHB_CompMatchIntFunc=NULL;
}

void Timer1_InputCaptureInit(u8 TrigSRC)
{


	Timer1_NormalModeInit(OC1A_MODE0, OC1B_MODE0); //force timer1 to run in normal mode
	switch(NOISE_CANCELLATION)
	{
		case ENABLE:
		TCCR1B |=(1<<7); //set ICNC1 to 1 , enable noise cancellation
		break;
		case DISABLE:
		TCCR1B &=~(1<<7);//clear ICNC1 , disable noise cancellation
		break;
		default:
		//noop
		break;
	}

	switch(TrigSRC)
	{

		case TRIGGER_SRC_ICP1: //trigger source is ICP1 pin
		ACSR &=~(1<<2); //set trigger source to be ICP pin
		TIFR |=(1<<5); //clear the input capture flag to avoid accidental trigger after changing the trigger source
		break;

		case TRIGGER_SRC_ACO: //trigger source is ASIC pin "analog comparator"
		ACSR |=(1<<2);
		TIFR |=(1<<5); //clear the input capture flag to avoid accidental trigger after changing the trigger source
		break;

		default:
		//noop
		break;
	}


}

void __vector_6 (void) __attribute__ ((signal,used));//disable optimization for this function
void __vector_6 (void) //timer1 input capture  interrupt vector no
{

	 static volatile u16 RissingValue=0,FallingValue=0;
	 volatile u16 ICU_CapturedValue;

	 ICU_CapturedValue =ICR1L; //store the value of the low byte of ICR1 register
	 ICU_CapturedValue|=(ICR1H <<8); //store the value of the high byte of ICR1 register
	 ICU_EdgeFlag++;

	/*
	  * when ICU_EdgeFlag =1 means it's the very first reading ,any calculation for either Ton ticks or toff ticks should execlude this reading
	  * when ICU_EdgeFlag =2 means it's the second consecutive edge to be registered now either
	  */

	    if (ICU_EdgeFlag==2) // execute on signal's rising edge
	    {
		 TCCR1B &=~(1<<6); //change trigger edge to be falling edge
		 RissingValue=ICU_CapturedValue;

		if(T1_OVF_Counter>0)
		{
			ICU_ToffTicks=(65536-FallingValue)+RissingValue+((T1_OVF_Counter-1)*65536);
		}
		else{
			ICU_ToffTicks=RissingValue-FallingValue;
		}

		T1_OVF_Counter=0;
         }



	 else if(ICU_EdgeFlag==3) //execute on signal's falling edge
	 {
		TCCR1B |=(1<<6); //change trigger edge to be  rising edge
		FallingValue=ICU_CapturedValue;
		if(T1_OVF_Counter>0)
		{
			ICU_TonTicks=(65536-RissingValue)+FallingValue+((T1_OVF_Counter-1)*65536);
		}
		else{
			ICU_TonTicks=FallingValue-RissingValue;
		}

		T1_OVF_Counter=0;
		ICU_EdgeFlag-=2;

	 }


	else //the very first  being picked on first falling edge
	{
		FallingValue=ICU_CapturedValue;
		T1_OVF_Counter=0;
		TCCR1B |=(1<<6); //change trigger edge to be rising edge

	}



}

void __vector_7 (void) __attribute__ ((signal,used));//disable optimization for this function
void __vector_7 (void) //timer1  channel A compare match interrupt vector no
{
	Timer1_CHA_CompMatchIntFunc(); //execute the user's function when the channel A compare match interrupt occurs
}


void __vector_8 (void) __attribute__ ((signal,used));//disable optimization for this function
void __vector_8 (void) //timer1  channel B compare match interrupt vector no
{
	Timer1_CHB_CompMatchIntFunc(); //execute the user's function when the channel B compare match interrupt occurs
}


void __vector_9 (void) __attribute__ ((signal,used));//disable optimization for this function
void __vector_9 (void) //timer1 overflow interrupt vector no
{
	Timer1_OverFlowIntFunc(); //execute the user's function when the overflow interrupt occurs
}
