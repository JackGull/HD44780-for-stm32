/* =================================================================================
	HD44780 Driver v1.0 for stm32f1x processors
	
	This is a driver for HD44780 compatible displays, written for and tested with
	the STM32VL Discovery board. Should not be hard to port it on other STM32 devices.
	
	History:
	*V1.0:
		First release. Only 4 bit interface is supported and only specific pins are used.
		PA1 - RS
		PA2 - R/W
		PA3 - E
		PC0 = D4
		PC1 = D5
		PC2 = D6
		PC3 = D7
	
================================================================================= */

#ifndef HD44780_DRIVER_H
#define HD44780_DRIVER_H

#include "stm32f10x.h"

/* Pinout */
#define PIN_RS	GPIO_Pin_1
#define PIN_RW	GPIO_Pin_2
#define PIN_E		GPIO_Pin_3
#define PIN_D0	GPIO_Pin_0
#define PIN_D1	GPIO_Pin_1
#define PIN_D2	GPIO_Pin_2
#define PIN_D3	GPIO_Pin_3
#define PIN_D4	GPIO_Pin_0
#define PIN_D5	GPIO_Pin_1
#define PIN_D6	GPIO_Pin_2
#define PIN_D7	GPIO_Pin_3

/* Commands */
#define CLEAR_DISPLAY						0x01
#define RETURN_HOME							0x02
#define SET_MOVE_CURSOR_RIGHT		0x06
#define SET_MOVE_CURSOR_LEFT		0x04
#define SET_SHIFT_DISPLAY_OFF		0x06
#define SET_SHIFT_DISPLAY_RIGHT	0x07
#define SET_SHIFT_DISPLAY_LEFT	0x05
#define DISPLAY_ON							0x0C
#define DISPLAY_OFF							0x08
#define CURSOR_ON								0x0E
#define CURSOR_OFF							0x0C
#define CURSOR_BLINKING_ON			0x0F
#define CURSOR_BLINKING_OFF			0x0E
#define MOVE_CURSOR_RIGHT				0x16
#define MOVE_CURSOR_LEFT				0x10
#define SHIFT_DISPLAY_RIGHT			0x1C
#define SHIFT_DISPLAY_LEFT			0x18
#define SET_4B_1LINE_5X8				0x20
#define SET_4B_1LINE_5X10				0x26
#define SET_4B_2LINES_5X8				0x28
#define SET_4B_2LINES_5X10			0x2C
#define SET_8B_1LINE_5X8				0x30
#define SET_8B_1LINE_5X10				0x36
#define SET_8B_2LINES_5X8				0x38
#define SET_8B_2LINES_5X10			0x3C

void HD44780_4b_GPIOInit(void);
void HD44780_4b_sendByte(uint8_t byteToSend);
uint16_t HD44780_4b_checkBusy(void);
void delay_us(uint16_t delay);

void HD44780_4b_sendCommand(uint8_t command);
void HD44780_4b_setCp(uint8_t row, uint8_t coloumn);
void HD44780_4b_write_char_cp(char character);
void HD44780_4b_write_cp(char *astring);
void HD44780_4b_write_char(uint8_t row, uint8_t coloumn, char character);
void HD44780_4b_write(uint8_t row, uint8_t coloumn, char *astring);

void HD44780_4b_init(void);

#endif
