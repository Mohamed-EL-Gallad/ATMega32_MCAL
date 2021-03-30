/*
 * REG_utils.c
 *
 *  Created on: ٢٨‏/١٢‏/٢٠٢٠
 *      Author: Mohamed_EL_Gallad
 */
#include "DIO_reg.h"
#include "REG_utils.h"


void SetPortDIR(u8 PortNum, u8 DIR)
{

	switch(PortNum)
	{
	case 0:
		DDRA=DIR;
		break;
	case 1:
		DDRB=DIR;
		break;
	case 2:
		DDRC=DIR;
		break;
	case 3:
		DDRD=DIR;
		break;
	default:
		break;
	};
}



void SetPortValue(u8 PortNum,u8 Value)
{
	switch (PortNum)
	{
	case 0:
		PORTA=Value;
		break;
	case 1:
		PORTB=Value;
		break;
	case 2:
		PORTC=Value;
		break;
	case 3:
		PORTD=Value;
		break;
	default:
		break;
	};
}



u8 GetPortValue(u8 PortNum)
{
	u8 PortValue=0;
	switch(PortNum)
	{
	case 0:
		PortValue=PINA;
		break;
	case 1:
		PortValue=PINB;
		break;
	case 2:
		PortValue=PINC;
		break;
	case 3:
		PortValue=PIND;
		break;
	default:
		break;
	};
	return PortValue;
}



void SetPinDIR(u8 PortNum,u8 PIN_Num,u8 DIR)
{
	switch (PortNum)
	{
	case 0:
		   if(DIR==0)
		   {
			   DDRA &=~(1<<PIN_Num);
		   }
		   else if(DIR==1)
		   {
			   DDRA |=(1<<PIN_Num);
		   }
		else{
			   //no op
		   }
		    break;


	 case 1:
			if(DIR==0)
			{
				DDRB &=~(1<<PIN_Num);
			}
			else if(DIR==1)
			{
				DDRB |=(1<<PIN_Num);
			}
			else{
				//no op
			}
			break;


	 case 2:
			if(DIR==0)
			{
				DDRC &=~(1<<PIN_Num);
			}
			else if(DIR==1)
			{
				DDRC |=(1<<PIN_Num);
			}
			else{
				//no op
			}
			break;


	 case 3:
			if(DIR==0)
			{
				DDRD &=~(1<<PIN_Num);
			}
			else if(DIR==1)
			{
				DDRD |=(1<<PIN_Num);
			}
			else{
				//no op
			}
			break;


	 default:
	 	 break;

	};

}



void SetPinValue(u8 PortNum,u8 PIN_Num,u8 PIN_Value)
{
	switch (PortNum)
	{
	case 0:
		   if(PIN_Value==0)
		   {
			   PORTA &=~(1<<PIN_Num);
		   }
		   else if(PIN_Value==1)
		   {
			   PORTA |=(1<<PIN_Num);
		   }
		else{
			   //no op
		   }
		    break;


	 case 1:
			if(PIN_Value==0)
			{
				PORTB &=~(1<<PIN_Num);
			}
			else if(PIN_Value==1)
			{
				PORTB |=(1<<PIN_Num);
			}
			else{
				//no op
			}
			break;


	 case 2:
			if(PIN_Value==0)
			{
				PORTC &=~(1<<PIN_Num);
			}
			else if(PIN_Value==1)
			{
				PORTC |=(1<<PIN_Num);
			}
			else{
				//no op
			}
			break;


	 case 3:
			if(PIN_Value==0)
			{
				PORTD &=~(1<<PIN_Num);
			}
			else if(PIN_Value==1)
			{
				PORTD |=(1<<PIN_Num);
			}
			else{
				//no op
			}
			break;


	 default:
	 	 break;

	};

}


u8 GetPinValue(u8 PortNum, u8 PIN_Num)
{
	u8 PIN_Value=0;
	switch(PortNum)
	{

	case 0:
		PIN_Value=((u8)(((u8)PINA)&(1<<PIN_Num)))>>PIN_Num;
		break;
	case 1:
		PIN_Value=((u8)(((u8)PINB)&(1<<PIN_Num)))>>PIN_Num;
		break;
	case 2:
		PIN_Value=((u8)(((u8)PINC)&(1<<PIN_Num)))>>PIN_Num;
		break;
	case 3:
		PIN_Value=((u8)(((u8)PIND)&(1<<PIN_Num)))>>PIN_Num;
		break;
	default:
		break;
	};
	return PIN_Value;

}

void TogglePin(u8 PortNum , u8 PinNum)
{
	switch(PortNum)
	{
	case 0:
		PORTA ^=(1<<PinNum);
		break;
	case 1:
		PORTB ^=(1<<PinNum);
		break;
	case 2:
		PORTC ^=(1<<PinNum);
		break;
	case 3:
		PORTD ^=(1<<PinNum);
		break;
	default:
		break;

	}
}

