/*
 * LCD_interface.h
 *
 *  Created on: ??�/??�/????
 *      Author: spider
 */

#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_

#define LCD_MODE 8  //comment this line in case of 4bit mode of operation
//#define MODE 4 //comment this line in case of 8 bit mode of operation

#define LCD_ON_PORT 0 //select port number 0->3 "based on mega32/16"

#define CURSOR_ON 1 //1 show cursor 0 for disable cursor
#define CURSOR_BLINK 1 //1 for blinking the cursor 0 for constant cursor

#define RSonPORT 1 //sets the port for the RS pin
#define RSonPIN 0  // sets the pin for RS

#define RWonPORT 1
#define RWonPIN 1

#define ENABLE_PORT 1
#define ENABLE_PIN 2

void LCD_vidWriteChar(u8 sympol);
void LCD_vidLCDClear(void);
void LCD_vidLCDInit (void);
void LCDPort(void);
void LCD_vidPrintString(c8* letter);
void LCD_vidPrintInteger(u64 integer);
void LCD_GoTo_xy(u8 x,u8 y);
void LCD_PrintFloat(f64 num);

#endif /* LCD_INTERFACE_H_ */
