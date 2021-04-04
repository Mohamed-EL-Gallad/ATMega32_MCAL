/*
 * TimersConfig.h
 *
 *  Created on: 15/03/2021
 *  Author: Mohamed_EL_Gallad
 *  Description:
 */

#ifndef TIMERSCONFIG_H_
#define TIMERSCONFIG_H_

/***************************** TIMER0 & TIMER1 PRESCALER VALUE **********************************************
 * These MACROS define both Timer0 and Timer 1 prscaler values
 * user has to set TIMER0_PRESCALER & TIMER1_PRESCALER to the desired prescaler
 *----------------------------------------------------------------------------------------------------------*/
#define	NO_PRESCALER        (u8)1   // FCPU/1
#define	CLK_DIV_BY8         (u8)2   // FCPU/8
#define	CLK_DIV_BY64        (u8)3   // FCPU/64
#define	CLK_DIV_BY265       (u8)4   // FCPU/265
#define	CLK_DIV_BY1024      (u8)5   // FCPU/1024
#define	EXT_CLK_FALLING     (u8)6   //external clock on T0,Pin B0 ,trigger falling edge
#define	EXT_CLK_RISSING     (u8)7   //external clock on T0,Pin B0 ,trigger rising edge

#define TIMER0_PRESCALER	CLK_DIV_BY265
#define TIMER1_PRESCALER    CLK_DIV_BY64

/************************************************************************************************************/



/*************************************** OC0 PIN OPERATION MODE *********************************************
 * these MACROS define the operation modes for the OC0 PIN according the below table
 * user has to set the value of OC0_OPMODE MACRO to the required operation Mode regarding Timer0 functionality
 -----------------------------------------------------------------------------------------------------------*/

 /*----------------*----------------*----------------*--------------------------*---------------------------*
  | OPERATION      |OC0 MODE0       |OC0 MODE1       |OC0 MODE2                 |OC0 MODE3                  |
  * ---------------*----------------*----------------*--------------------------*---------------------------*
  | NORMAL   MODE  |PIN DICONNECTED |TOGGLE ON MATCH |CLEAR ON MATCH            |SET ON MATCH               |
  * ---------------*----------------*----------------*--------------------------*---------------------------*
  * PHASE CORRECT  |PIN DICONNECTED |RESERVED        |CLEAR ON MATCH UPCOUNTING |SET ON MATCH UPCOUNTING    |
  |                |                |                |SET ON MATCH DOWNCOUNTING |CLEAR ON MATCH DOWNCOUNTING|
  * ---------------*----------------*----------------*--------------------------*---------------------------*
  | CTC            |PIN DICONNECTED |TOGGLE ON MATCH |CLEAR ON MATCH            |SET ON MATCH               |
  * ---------------*----------------*----------------*--------------------------*---------------------------*
  | FAST PWM       |PIN DICONNECTED |RESERVED        |CLEAR ON MATCH,SET AT TOP |SET ON MATCH,CLEAR AT TOP  |
  * ---------------*----------------*----------------*--------------------------*---------------------------*/
#define OC0_MODE0   0
#define OC0_MODE1   1
#define OC0_MODE2   2
#define OC0_MODE3   3

#define OC0_OPMODE OC0_MODE1
/**************************************************************************************************************/



/*
*in phase and freq correction the update of top value"ICR1 or OCR1A"  will occur when the counter reaches the bottom value
*while in the other modes their values will be updated when the counter reaches top value
*/
/*--------------------------*----------------*
 |         MODE             |  TOP VALUE     |
 * -------------------------*----------------*
 | T1_PHASECORR_MODE1       | 8-BIT, 0x00FF  |
 * -------------------------*----------------*
 | T1_PHASECORR_MODE2       | 9-BIT, 0x01FF  |
 * -------------------------*----------------*
 | T1_PHASECORR_MODE3       | 10-BIT, 0x03FF |
 * -------------------------*----------------*
 | T1_PHASECORR_MODE4       | ICR1           |
 * -------------------------*----------------*
 | T1_PHASECORR_MODE5       | OCR1A          |
 * -------------------------*----------------*
 | T1_PHASE_FREQ_CORR_MODE1 | ICR1           |
 * -------------------------*----------------*
 | T1_PHASE_FREQ_CORR_MODE2 | OCR1A          |
 * -------------------------*----------------*/
#define T1_PHASECORR_MODE1        (u8)1
#define T1_PHASECORR_MODE2        (u8)2
#define T1_PHASECORR_MODE3        (u8)3
#define T1_PHASECORR_MODE4        (u8)4
#define T1_PHASECORR_MODE5        (u8)5
#define T1_PHASE_FREQ_CORR_MODE1  (u8)6
#define T1_PHASE_FREQ_CORR_MODE2  (u8)7

//default phase correct PWM operation mode
#define TI_PWM_PHASECORR_OPMODE   TI_PHASE_FREQ_CORR_MODE1


/*-------------------*------*---------*
 |         MODE      |  TOP VALUE     |
 * ------------------*----------------*
 | T1_FASTPWM_MODE1  | 8-BIT, 0x00FF  |
 * ------------------*----------------*
 | T1_FASTPWM_MODE2  | 9-BIT, 0x01FF  |
 * ------------------*----------------*
 | T1_FASTPWM_MODE3  | 10-BIT, 0x03FF |
 * ------------------*----------------*
 | T1_FASTPWM_MODE4  | ICR1           |
 * ------------------*----------------*
 | T1_FASTPWM_MODE5  | OCR1A          |
 * ------------------*----------------*/
#define T1_FASTPWM_MODE1   (u8)1
#define T1_FASTPWM_MODE2   (u8)2
#define T1_FASTPWM_MODE3   (u8)3
#define T1_FASTPWM_MODE4   (u8)4
#define T1_FASTPWM_MODE5   (u8)5

//default fast PWM operation mode
#define T1_FASTPWM_OPMODE  T1_FASTPWM_MODE4

/***********************************COMPARE MATCH MODE SETTING**************************************/

 /*----------------*----------------*----------------*--------------------------*----------------------------*
  | OPERATION      |OC1A MODE0      |OC1A MODE1      |OC1A MODE2                |OC1A MODE3                  |
  * ---------------*----------------*----------------*--------------------------*----------------------------*
  | NORMAL   MODE  |PIN DICONNECTED |TOGGLE ON MATCH |CLEAR ON MATCH            |SET ON MATCH                |
  * ---------------*----------------*----------------*--------------------------*----------------------------*
  | PHASE CORRECT  |PIN DICONNECTED |TOGGLE ON MATCH |CLEAR ON MATCH UPCOUNTING |SET ON MATCH UPCOUNTING     |
  | FREQ  CORRECT  |                |                |SET ON MATCH DOUWNCOUNTING|CLEAR ON MATCH DOUWNCOUNTING|                                                  |
  * ---------------*----------------*----------------*--------------------------*----------------------------*
  | CTC            |PIN DICONNECTED |TOGGLE ON MATCH |CLEAR ON MATCH            |SET ON MATCH                |
  * ---------------*----------------*----------------*--------------------------*----------------------------*
  | FAST PWM       |PIN DICONNECTED |TOGGLE ON MATCH |CLEAR ON MATCH,SET AT TOP |SET ON MATCH,CLEAR AT TOP   |
  * ---------------*----------------*----------------*--------------------------*----------------------------*/
#define OC1A_MODE0  0
#define OC1A_MODE1  1
#define OC1A_MODE2  2
#define OC1A_MODE3  3

#define OC1A_OPMODE OC1A_MODE1

 /*----------------*----------------*----------------*--------------------------*----------------------------*
  | OPERATION      |OC1B MODE0      |OC1B MODE1      |OC1B MODE2                |OC1B MODE3                  |
  * ---------------*----------------*----------------*--------------------------*----------------------------*
  | NORMAL   MODE  |PIN DICONNECTED |TOGGLE ON MATCH |CLEAR ON MATCH            |SET ON MATCH                |
  * ---------------*----------------*----------------*--------------------------*----------------------------*
  * PHASE CORRECT  |PIN DICONNECTED |PIN DICONNECTED |CLEAR ON MATCH UPCOUNTING |SET ON MATCH UPCOUNTING     |
  | FREQ  CORRECT  |                |                |SET ON MATCH DOUWNCOUNTING|CLEAR ON MATCH DOUWNCOUNTING|                                                  |
  * ---------------*----------------*----------------*--------------------------*----------------------------*
  | CTC            |PIN DICONNECTED |TOGGLE ON MATCH |CLEAR ON MATCH            |SET ON MATCH                |
  * ---------------*----------------*----------------*--------------------------*----------------------------*
  | FAST PWM       |PIN DICONNECTED |PIN DICONNECTED  |CLEAR ON MATCH,SET AT TOP |SET ON MATCH,CLEAR AT TOP   |
  * ---------------*----------------*----------------*--------------------------*----------------------------*/
#define OC1B_MODE0  0
#define OC1B_MODE1  1
#define OC1B_MODE2  2
#define OC1B_MODE3  3

#define OC1B_OPMODE OC1B_MODE1

/*--------------------*-----------*
 |        MODE        | TOP VALUE |
 * -------------------*-----------*
 | T1_COMPMATCH_MODE1 |  OCR1A    |
 * -------------------*-----------*
 | TI_COMPMATCH_MODE2 |  ICR1     |
 * -------------------*-----------*/

#define T1_COMPMATCH_MODE1   0
#define T1_COMPMATCH_MODE2   1

#define T1_COMPMATCH_OPMODE  T1_COMPMATCH_MODE2



/*************************** TIMER_1 INPUT CAPTURE SETTING *****************************************************************************/
#define ENABLE                   1 // DON'T CHANGE THE DESIGNATED VALUE
#define DISABLE                  0 // DON'T CHANGE THE DESIGNATED VALUE

#define TRIGGER_SRC_ICP1         0
#define TRIGGER_SRC_ACO          1

#define TRIGGER_SOURCE TRIGGER_SRC_ICP1

//user has to define the timer1 operating frequency either internal or external clock is being used
#define Timer1_OP_CLK     187500 // F_CPU/ Prescaler OR the value of the external CLK on T1

//change to DISABLE to disable the noise cancellation property
#define NOISE_CANCELLATION  ENABLE

/***************************************************************************************************************************************/


#endif /* TIMERSCONFIG_H_ */
