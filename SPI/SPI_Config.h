/*
 *  SPI_Config.h
 *
 *  CAUTION : IF THIS MODULE IS USED ALONGSIDE THE UART MODULE THE USER MUST UNCOMMENT THE INCLUSION OF THE SPI_Config.h HEADER FILE
 *  IN THE CircularBufferConfig.h HEADER FILE
 *
 *  Created on: 23/08/2021
 *  Author: Mohamed_EL_Gallad
 *  Description :This file will contain all the configuration related to the SPI module
 *  	1- Node operation mode either Master or Slave
 *  	   1-1- If the node configured as Master the user has to set the following configurations
 *  	        A- Master frequency by setting the SPI_MASTER_FREQ to one of the predefined Macros
 *  	        B- Select to either enable or disable the double speed mode
 *  	        C- The selection of the clock phase and polarity
 *  	        D- The value of the Dummy packet by setting the macro DUMMY_PACKET to the desired value
 *  	        E- The order of data transmission to be either LSB or MSB first
 *
 *  	   1-2- If the node configured as Slave the user has to set the following configurations
 *  	   		A- The size of the TX and RX buffers by setting the value of the macro TX_RX_BUFFER_SIZE
 *  	   		NOTE: this macro is shared with the UART module so if both module is used at the same time
 *  	   		the value of that macro must be the same in both modules
 */

#ifndef SPI_CONFIG_H_
#define SPI_CONFIG_H_

/*-------------------------------------------------------------------------------------------------------------
 *                                     OPERATION MODE SELECTION
 *-------------------------------------------------------------------------------------------------------------*/
#define SLAVE_NODE     0
#define MASTER_NODE    1

#define SPI_OPERATION_MODE  MASTER_NODE //Select either SLAVE_NODE or MASTER_NODE

/**************************************************************************************************************/


/*-------------------------------------------------------------------------------------------------------------
 *                                     MASTER MODE CONFIGURATIONS
 *-------------------------------------------------------------------------------------------------------------*/

                                  /*--------------------------------*
                                   *        MASTER FREQUENCY       *
                                   *-------------------------------*/
#define CPU_FREQ_DIV_BY4      0 //if used while MASTER_DOUBLE_SPEED_MODE is enabled Master Freq will be = CPU_FREQ/2
#define CPU_FREQ_DIV_BY16     2 //if used while MASTER_DOUBLE_SPEED_MODE is enabled Master Freq will be = CPU_FREQ/8
#define CPU_FREQ_DIV_BY64     4 //if used while MASTER_DOUBLE_SPEED_MODE is enabled Master Freq will be = CPU_FREQ/32
#define CPU_FREQ_DIV_BY128    5 //if used while MASTER_DOUBLE_SPEED_MODE is enabled Master Freq will be = CPU_FREQ/64

#define SPI_MASTER_FREQ   CPU_FREQ_DIV_BY64


                                  /*-------------------------------*
                                   *    MASTER DOUBLE SPEED MODE   *
                                   *-------------------------------*/
#define MASTER_DOUBLE_SPEED_MODE  DISABLE // Set to either DISABLE or ENABLE Macros


                                  /*-------------------------------*
                                   * MASTER CLK PHASE AND POLARITY *
                                   *-------------------------------*/
#define CLK_PHASE_POLARITY_MODE_0    0 //CLK idle state=Low  || Data sampling edge=Rising  || Data setup edge=Falling
#define CLK_PHASE_POLARITY_MODE_1    1 //CLK idle state=Low  || Data sampling edge=Rising  || Data setup edge=Falling
#define CLK_PHASE_POLARITY_MODE_3    2 //CLK idle state=High || Data sampling edge=Falling || Data setup edge=Rising
#define CLK_PHASE_POLARITY_MODE_4    3 //CLK idle state=High || Data sampling edge=Falling || Data setup edge=Rising

#define SPI_MASTER_CLK_MODE  CLK_PHASE_POLARITY_MODE_0


                                 /*-------------------------------*
                                  *      MASTER DUMMY PACKET      *
                                  *-------------------------------*/
//this macro is used to define a define a value to be used by the master as a dummy packet
#define DUMMY_PACKET       ((u8)0XC3)


                                 /*-------------------------------*
                                  *    TRANSMISSION DATA ORDER    *
                                  *-------------------------------*/

#define TRANSMIT_LSB_FIRST   0
#define TRANSMIT_MSB_FIRST   1

#define DATA_ORDER  TRANSMIT_MSB_FIRST

/**************************************************************************************************************/


/*-------------------------------------------------------------------------------------------------------------
 *                                     SLAVE MODE CONFIGURATIONS
 *-------------------------------------------------------------------------------------------------------------*/

                                 /*-------------------------------*
                                  *   SLAVE TX & RX BUFFER SIZE   *
                                  *-------------------------------*/

#ifndef TX_RX_BUFFER_SIZE
#define TX_RX_BUFFER_SIZE  (7)
#endif
/**************************************************************************************************************/

#endif /* SPI_CONFIG_H_ */
