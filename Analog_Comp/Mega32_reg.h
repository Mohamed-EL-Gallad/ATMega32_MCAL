/*
 * Mega32_reg.h
 *
 *  Created on: ٢٧‏/١٢‏/٢٠٢٠
 *      Author: Mohamed_EL_Gallad
 */
#include "STD_types.h"

#ifndef MEGA32_REG_H_
#define MEGA32_REG_H_
/*----------------------------------------------------------------------
           DIO registers addresses
----------------------------------------------------------------------*/
#define PORTA (*((volatile u8*)0x3B))
#define DDRA  (*((volatile u8*)0x3A))
#define PINA  (*((volatile u8*)0x39))

#define PORTB (*((volatile u8*)0x38))
#define DDRB  (*((volatile u8*)0x37))
#define PINB  (*((volatile u8*)0x36))

#define PORTC (*((volatile u8*)0x35))
#define DDRC  (*((volatile u8*)0x34))
#define PINC  (*((volatile u8*)0x33))

#define PORTD (*((volatile u8*)0x32))
#define DDRD  (*((volatile u8*)0x31))
#define PIND  (*((volatile u8*)0x30))

/*----------------------------------------------------------------------
                 Timer zero registers
----------------------------------------------------------------------*/
#define TCCR0 (*((volatile u8*)0x53))
#define TCNT0 (*((volatile u8*)0x52))
#define OCR0  (*((volatile u8*)0x5C))


/*----------------------------------------------------------------------
 *               Timer1 Registers
 ---------------------------------------------------------------------*/
#define SFIOR   (*((volatile u8*)0x50))
#define TCCR1A  (*((volatile u8*)0x4F))
#define TCCR1B  (*((volatile u8*)0x4E))
#define TCNT1H  (*((volatile u8*)0x4D))
#define TCNT1L  (*((volatile u8*)0x4C))
#define OCR1AH  (*((volatile u8*)0x4B))
#define OCR1AL  (*((volatile u8*)0x4A))
#define OCR1BH  (*((volatile u8*)0x49))
#define OCR1BL  (*((volatile u8*)0x48))
#define ICR1H   (*((volatile u8*)0x47))
#define ICR1L   (*((volatile u8*)0x46))


/*----------------------------------------------------------------------
 *               Timer2 Registers
 ---------------------------------------------------------------------*/
#define TCCR2   (*((volatile u8*)0x45))
#define TCNT2   (*((volatile u8*)0x44))
#define OCR2    (*((volatile u8*)0x43))
#define ASSR    (*((volatile u8*)0x42))


/*----------------------------------------------------------------------
 *            General Timers Registers
 ---------------------------------------------------------------------*/
#define TIMSK (*((volatile u8*)0x59))
#define TIFR  (*((volatile u8*)0x58))


/*----------------------------------------------------------------------
             global interrupt register
----------------------------------------------------------------------*/
#define SREG (*((volatile u8*)0x5F))


/*----------------------------------------------------------------------
 *             Analog comparator
 ---------------------------------------------------------------------*/
#define ACSR    (*((volatile u8*)0x28))


/*----------------------------------------------------------------------
                  WatchDog Timer
----------------------------------------------------------------------*/
#define WDTCR   (*((volatile u8*)0x41))


/*----------------------------------------------------------------------
                 EEPROM registers
----------------------------------------------------------------------*/
#define EEARH   (*((volatile u8*)0x3F))
#define EEARL   (*((volatile u8*)0x3E))
#define EEDR    (*((volatile u8*)0x3D))
#define EECR    (*((volatile u8*)0x3C))


/*----------------------------------------------------------------------
                   UART registers
----------------------------------------------------------------------*/
#define UBRRH    (*((volatile u8*)0x40))
#define UCSRC    (*((volatile u8*)0x40))
#define UDR      (*((volatile u8*)0x2C))
#define UCSRA    (*((volatile u8*)0x2B))
#define UCSRB    (*((volatile u8*)0x2A))
#define UBRRL    (*((volatile u8*)0x29))


/*----------------------------------------------------------------------
 *               SPI registers
 ---------------------------------------------------------------------*/
#define SPCR     (*((volatile u8*)0x2D))
#define SPSR     (*((volatile u8*)0x2E))
#define SPDR     (*((volatile u8*)0x2F))


/*----------------------------------------------------------------------
                   ADC registers
----------------------------------------------------------------------*/
#define ADCSRA    (*((volatile u8*)0x26))
#define ADMUX     (*((volatile u8*)0x27))


#endif /* MEGA32_REG_H_ */
