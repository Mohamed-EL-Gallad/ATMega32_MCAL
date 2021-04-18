/*
 * TimersConfig.h
 *
 *  Created on: 15/03/2021
 *  Author: Mohamed_EL_Gallad
 *  Description:This file will contain all the setting required to properly set Timer0 , Timer1 and Timer2 to
 *  operate as desired
 *
 *  TIMERS GENERAL SETTING section is used to define the MCU operating frequency in HZ
 *
 *  TIMER0 CONFIGURATIONS section is used to select 1-Timer0's Prescaler
 *                                                  2-OC0 operating mode
 *
 *  TIMER1 CONFIGURATIONS section is used to select 1-Timer1's Prescaler
 *                                                  2-Phase Correct PWM modes selection
 *                                                  3-Fast PWM modes selection
 *                                                  4-CTC modes selection
 *                                                  5-TIMER1- CHANNEL_A COMPARE MATCH OUTPUT SETTINGS
 *                                                  6-TIMER1- CHANNEL_B COMPARE MATCH OUTPUT SETTINGS
 *                                                  7-Input Capture Settings
 *
 *  TIMER2 CONFIGURATIONS section is used to select 1-Timer2's Prescaler
 *                                                  2-OC2 operating mode
 */

#ifndef TIMERSCONFIG_H_
#define TIMERSCONFIG_H_


/**-----------------------------------------------------------------------------------------------------------*/
/*                                     TIMERS GENERAL SETTING                                                 */
/**-----------------------------------------------------------------------------------------------------------*/
#define CPU_FREQ 12000000  //user has to define the CPU operating frequency

/***************************************************************************************************************************************/


/**-----------------------------------------------------------------------------------------------------------*/
/*                                      TIMER0 CONFIGURATIONS                                                 */
/**-----------------------------------------------------------------------------------------------------------*/

                               /*----------------------------*/
                               /** TIMER0 PRESCALER SELECT  **/
                               /*----------------------------*/
#define	T0_NO_PRESCALER        1   // FCPU/1
#define	T0_CLK_DIV_BY8         2   // FCPU/8
#define	T0_CLK_DIV_BY64        3   // FCPU/64
#define	T0_CLK_DIV_BY265       4   // FCPU/265
#define	T0_CLK_DIV_BY1024      5   // FCPU/1024
#define	T0_EXT_CLK_FALLING     6   //external clock on T0,Pin PB0 ,trigger falling edge
#define	T0_EXT_CLK_RISSING     7   //external clock on T0,Pin PB0 ,trigger rising edge

#define TIMER0_PRESCALER	T0_NO_PRESCALER  //select your preferred prescaler

                            /*----------------------------------------*/
                            /** TIMER0 COMPARE MATCH OUTPUT SETTINGS **/
                            /*----------------------------------------*/
/*----------------*----------------*----------------*--------------------------*---------------------------*
  | OPERATION      |OC0 MODE0       |OC0 MODE1       |OC0 MODE2                 |OC0 MODE3                  |
  * ---------------*----------------*----------------*--------------------------*---------------------------*
  | NORMAL   MODE  |PIN DICONNECTED |TOGGLE ON MATCH |CLEAR ON MATCH            |SET ON MATCH               |
  * ---------------*----------------*----------------*--------------------------*---------------------------*
  *                |PIN DICONNECTED |RESERVED        |CLEAR ON MATCH UPCOUNTING |SET ON MATCH UPCOUNTING    |
  | PHASE CORRECT  |                |                |SET ON MATCH DOWNCOUNTING |CLEAR ON MATCH DOWNCOUNTING|
  |                |                |                |   NON INVERTED MODE      |      INVERTED MODE        |
  * ---------------*----------------*----------------*--------------------------*---------------------------*
  | CTC            |PIN DICONNECTED |TOGGLE ON MATCH |CLEAR ON MATCH            |SET ON MATCH               |
  * ---------------*----------------*----------------*--------------------------*---------------------------*
  | FAST PWM       |PIN DICONNECTED |RESERVED        |CLEAR ON MATCH,SET AT TOP |SET ON MATCH,CLEAR AT TOP  |
  |                |                |                |   NON INVERTED MODE      |       INVERTED MODE       |
  * ---------------*----------------*----------------*--------------------------*---------------------------*/
#define OC0_MODE0   0
#define OC0_MODE1   1
#define OC0_MODE2   2
#define OC0_MODE3   3

#define OC0_OPMODE OC0_MODE2 //select your preferred OC0 operation mode


/***************************************************************************************************************************************/


/**-----------------------------------------------------------------------------------------------------------*/
/*                                      TIMER1 CONFIGURATIONS                                                 */
/**-----------------------------------------------------------------------------------------------------------*/

                              /*---------------------------*/
                              /** TIMER1 PRESCALER SELECT **/
                              /*---------------------------*/
#define	T1_NO_PRESCALER        1   // FCPU/1
#define	T1_CLK_DIV_BY8         2   // FCPU/8
#define	T1_CLK_DIV_BY64        3   // FCPU/64
#define	T1_CLK_DIV_BY265       4   // FCPU/265
#define	T1_CLK_DIV_BY1024      5   // FCPU/1024
#define	T1_EXT_CLK_FALLING     6   //external clock on T1,Pin PB1 ,trigger falling edge
#define	T1_EXT_CLK_RISSING     7   //external clock on T1,Pin PB1 ,trigger rising edge

#define TIMER1_PRESCALER    T1_NO_PRESCALER //select your preferred prescaler

                        /*--------------------------------------------------------*/
                        /** PHASE CORRECT - PHASE AND FREQUENCY CORRECT PWM MODES**/
                        /*--------------------------------------------------------*/
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
#define T1_PHASECORR_DISABLED       0
#define T1_PHASECORR_MODE1          1
#define T1_PHASECORR_MODE2          2
#define T1_PHASECORR_MODE3          3
#define T1_PHASECORR_MODE4          4
#define T1_PHASECORR_MODE5          5
#define T1_PHASE_FREQ_CORR_MODE1    6
#define T1_PHASE_FREQ_CORR_MODE2    7

//IN CASE PHASE CORRECT MODES ARE NOT USED THE FOLLOWING MACRO SHOULD BE SET TO T1_PHASECORR_DISABLED
#define TI_PWM_PHASECORR_OPMODE   T1_PHASECORR_DISABLED //select your preferred operation mode

                                     /*------------------*/
                                     /** FAST PWM MODES **/
                                     /*------------------*/
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
#define T1_FASTPWM_DISABLED  0
#define T1_FASTPWM_MODE1     1
#define T1_FASTPWM_MODE2     2
#define T1_FASTPWM_MODE3     3
#define T1_FASTPWM_MODE4     4
#define T1_FASTPWM_MODE5     5

//IN CASE FAST PWM MODES ARE NOT USED THE FOLLOWING MACRO SHOULD BE SET TO T1_FASTPWM_DISABLED
#define T1_FASTPWM_OPMODE  T1_FASTPWM_DISABLED //select your preferred operation mode

                                     /*-----------------------*/
                                     /** COMPARE MATCH MODES **/
                                     /*-----------------------*/
/*--------------------*-----------*
 |        MODE        | TOP VALUE |
 * -------------------*-----------*
 | T1_COMPMATCH_MODE1 |  OCR1A    |
 * -------------------*-----------*
 | TI_COMPMATCH_MODE2 |  ICR1     |
 * -------------------*-----------*/
#define T1_COMPMATCH_MODE1   0
#define T1_COMPMATCH_MODE2   1

#define T1_COMPMATCH_OPMODE  T1_COMPMATCH_MODE2 //select your preferred operation mode

                            /*---------------------------------------------------*/
                            /** TIMER1- CHANNEL_A COMPARE MATCH OUTPUT SETTINGS **/
                            /*---------------------------------------------------*/
 /*----------------*----------------*-------------------------*---------------------------*----------------------------*
  | OPERATION      |OC1A MODE0      |OC1A MODE1               |OC1A MODE2                 |OC1A MODE3                  |
  * ---------------*----------------*-------------------------*---------------------------*----------------------------*
  | NORMAL   MODE  |PIN DICONNECTED |TOGGLE ON MATCH          |CLEAR ON MATCH             |SET ON MATCH                |
  * ---------------*----------------*-------------------------*---------------------------*----------------------------*
  | PHASE CORRECT  |PIN DICONNECTED |TOGGLE ON MATCH          |CLEAR ON MATCH UPCOUNTING  |SET ON MATCH UPCOUNTING     |
  | FREQ  CORRECT  |                |                         |SET ON MATCH DOUWNCOUNTING |CLEAR ON MATCH DOUWNCOUNTING|
  * ---------------*----------------*-------------------------*---------------------------*----------------------------*
  | CTC            |PIN DICONNECTED |TOGGLE ON MATCH          |CLEAR ON MATCH             |SET ON MATCH                |
  * ---------------*----------------*-------------------------*---------------------------*----------------------------*
  |                |                |      TOGGLE ON MATCH    |                           |                            |
  |                |                |       ONLY IN MODES     |                           |                            |
  |                |                |T1_PHASECORR_MODE5       |                           |                            |
  |                |                |T1_PHASE_FREQ_CORR_MODE2 |                           |                            |
  | FAST PWM       |PIN DICONNECTED |T1_FASTPWM_MODE5         ||CLEAR ON MATCH, SET AT TOP|SET ON MATCH,CLEAR AT TOP   |
  |                |                |IN WHICH OC1A CLK=FREQ/2 |     NON INVERTED MODE     |        INVERTED MODE       |
  |                |                |& DUTY CYCLE 50%         |                           |                            |
  |                |                |  DISCONNECTED IN        |                           |                            |
  |                |                |  ALL THE REST MODES     |                           |                            |
  * ---------------*----------------*-------------------------*---------------------------*---------------------------*/
#define OC1A_MODE0  0
#define OC1A_MODE1  1
#define OC1A_MODE2  2
#define OC1A_MODE3  3

#define OC1A_OPMODE OC1A_MODE2 //select your preferred OC1A operation mode

                            /*---------------------------------------------------*/
                            /** TIMER1- CHANNEL_B COMPARE MATCH OUTPUT SETTINGS **/
                            /*---------------------------------------------------*/
 /*----------------*----------------*----------------*--------------------------*----------------------------*
  | OPERATION      |OC1B MODE0      |OC1B MODE1      |OC1B MODE2                |OC1B MODE3                  |
  * ---------------*----------------*----------------*--------------------------*----------------------------*
  | NORMAL   MODE  |PIN DICONNECTED |TOGGLE ON MATCH |CLEAR ON MATCH            |SET ON MATCH                |
  * ---------------*----------------*----------------*--------------------------*----------------------------*
  * PHASE CORRECT  |PIN DICONNECTED |PIN DICONNECTED |CLEAR ON MATCH UPCOUNTING |SET ON MATCH UPCOUNTING     |
  | FREQ  CORRECT  |                |                |SET ON MATCH DOUWNCOUNTING|CLEAR ON MATCH DOUWNCOUNTING|
  * ---------------*----------------*----------------*--------------------------*----------------------------*
  | CTC            |PIN DICONNECTED |TOGGLE ON MATCH |CLEAR ON MATCH            |SET ON MATCH                |
  * ---------------*----------------*----------------*--------------------------*----------------------------*
  | FAST PWM       |PIN DICONNECTED |PIN DICONNECTED |CLEAR ON MATCH,SET AT TOP |SET ON MATCH,CLEAR AT TOP   |
  * ---------------*----------------*----------------*--------------------------*----------------------------*/
#define OC1B_MODE0  0
#define OC1B_MODE1  1
#define OC1B_MODE2  2
#define OC1B_MODE3  3

#define OC1B_OPMODE OC1B_MODE2 //select your preferred OC1A operation mode

                               /*---------------------------------*/
                               /** TIMER_1 INPUT CAPTURE SETTING **/
                               /*---------------------------------*/
#define ENABLE                   1 // DON'T CHANGE THE DESIGNATED VALUE
#define DISABLE                  0 // DON'T CHANGE THE DESIGNATED VALUE

#define TRIGGER_SRC_ICP1         0
#define TRIGGER_SRC_ACO          1

//choose the trigger source to be either TRIGGER_SRC_ICP1 or TRIGGER_SRC_ACO
#define TRIGGER_SOURCE TRIGGER_SRC_ICP1

//change to DISABLE to disable the noise cancellation property
#define NOISE_CANCELLATION  ENABLE

/***************************************************************************************************************************************/

/**-----------------------------------------------------------------------------------------------------------*/
/*                                      TIMER2 CONFIGURATIONS                                                 */
/**-----------------------------------------------------------------------------------------------------------*/

                              /*---------------------------*/
                              /** TIMER2 PRESCALER SELECT **/
                              /*---------------------------*/
#define	T2_NO_PRESCALER        1   // FCPU/1
#define	T2_CLK_DIV_BY8         2   // FCPU/8
#define	T2_CLK_DIV_BY32        3   // FCPU/32
#define	T2_CLK_DIV_BY64        4   // FCPU/64
#define	T2_CLK_DIV_BY128       5   // FCPU/128
#define	T2_CLK_DIV_BY256       6   // FCPU/256
#define	T2_CLK_DIV_BY1024      7   // FCPU/1024

#define TIMER2_PRESCALER	T2_NO_PRESCALER //select your preferred prescaler


                            /*----------------------------------------*/
                            /** TIMER0 COMPARE MATCH OUTPUT SETTINGS **/
                            /*----------------------------------------*/
 /*----------------*----------------*----------------*--------------------------*---------------------------*
  | OPERATION      |OC2 MODE0       |OC2 MODE1       |OC2 MODE2                 |OC2 MODE3                  |
  * ---------------*----------------*----------------*--------------------------*---------------------------*
  | NORMAL   MODE  |PIN DICONNECTED |TOGGLE ON MATCH |CLEAR ON MATCH            |SET ON MATCH               |
  * ---------------*----------------*----------------*--------------------------*---------------------------*
  *                |PIN DICONNECTED |RESERVED        |CLEAR ON MATCH UPCOUNTING |SET ON MATCH UPCOUNTING    |
  | PHASE CORRECT  |                |                |SET ON MATCH DOWNCOUNTING |CLEAR ON MATCH DOWNCOUNTING|
  |                |                |                |   NON INVERTED MODE      |      INVERTED MODE        |
  * ---------------*----------------*----------------*--------------------------*---------------------------*
  | CTC            |PIN DICONNECTED |TOGGLE ON MATCH |CLEAR ON MATCH            |SET ON MATCH               |
  * ---------------*----------------*----------------*--------------------------*---------------------------*
  | FAST PWM       |PIN DICONNECTED |RESERVED        |CLEAR ON MATCH,SET AT TOP |SET ON MATCH,CLEAR AT TOP  |
  |                |                |                |   NON INVERTED MODE      |       INVERTED MODE       |
  * ---------------*----------------*----------------*--------------------------*---------------------------*/
#define OC2_MODE0   0
#define OC2_MODE1   1
#define OC2_MODE2   2
#define OC2_MODE3   3

#define OC2_OPMODE OC2_MODE2 //select your preferred OC2 operation mode


#endif /* TIMERSCONFIG_H_ */
