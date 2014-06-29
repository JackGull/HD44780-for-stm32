#include "hd44780_stm32.h"

void HD44780_4b_GPIOInit(void) {
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	/* Enable the GPIOC Clock */ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE); 
	
	/* Configure the GPIOC pins */ 
	GPIO_InitStructure.GPIO_Pin = PIN_D4; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = PIN_D5; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_D6; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_D7; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_RS; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_RW; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_E; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void HD44780_4b_sendByte(uint8_t byteToSend) {
	uint16_t gpioc_backup;
	uint8_t nibble;
	/* Enable signal Up */
	GPIO_WriteBit(GPIOA, PIN_E, (BitAction) 1);
	/* Retreive GPIOC */
	gpioc_backup = GPIO_ReadOutputData (GPIOC);
	/* Write Upper Nibble */
	nibble = (byteToSend >> 4) & 0x0F;
	gpioc_backup &= 0xFFF0;
	gpioc_backup |= nibble;
	GPIO_Write(GPIOC, gpioc_backup);
	/* Enable Signal Down */
	GPIO_WriteBit(GPIOA, PIN_E, (BitAction) 0);
	/* Enable signal Up */
	GPIO_WriteBit(GPIOA, PIN_E, (BitAction) 1);
	/* Retreive GPIOC */
	gpioc_backup = GPIO_ReadOutputData (GPIOC);
	/* Write Upper Nibble */
	nibble = byteToSend & 0x0F;
	gpioc_backup &= 0xFFF0;
	gpioc_backup |= nibble;
	GPIO_Write(GPIOC, gpioc_backup);
	/* Enable Signal Down */
	GPIO_WriteBit(GPIOA, PIN_E, (BitAction) 0);
	
	/* wait until no more busy */
	HD44780_4b_checkBusy();
}

uint16_t HD44780_4b_checkBusy(void) {
	GPIO_InitTypeDef GPIO_InitStructure; 
	uint8_t busy;
	
	/* Configure the GPIOC pins */ 
	GPIO_InitStructure.GPIO_Pin = PIN_D4 | PIN_D5 | PIN_D6 | PIN_D7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	/* RW signal Up */
	GPIO_WriteBit(GPIOA, PIN_RW, (BitAction) 1);
	
	do {
		/* Enable signal Up */
		GPIO_WriteBit(GPIOA, PIN_E, (BitAction) 1);
		/* Check for busy */
		if (GPIO_ReadInputDataBit(GPIOC, PIN_D7)) {
			busy = 1;
		} else busy = 0;
		/* Enable signal Down */
		GPIO_WriteBit(GPIOA, PIN_E, (BitAction) 0);
		/* Enable signal Up */
		GPIO_WriteBit(GPIOA, PIN_E, (BitAction) 1);
		/* Enable signal Down */
		GPIO_WriteBit(GPIOA, PIN_E, (BitAction) 0);
		delay_us(5);
	
	/* Wait until no more busy */
	} while (busy);
	
	/* RW signal Down */
	GPIO_WriteBit(GPIOA, PIN_RW, (BitAction) 0);
	
	/* Change Back D4-7 to outputs */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	/* Could be Return Address */
	return 0;
}

void HD44780_4b_sendCommand(uint8_t command) {
	/* RS Signal Down */
	GPIO_WriteBit(GPIOA, PIN_RS, (BitAction) 0);
	/* Send Data */
	HD44780_4b_sendByte(command);	
}


void HD44780_4b_setCp(uint8_t row, uint8_t coloumn) {
	uint8_t DDRAM_Addr = 0x80;
	
	/* RS Signal Down */
	GPIO_WriteBit(GPIOA, PIN_RS, (BitAction) 0);
	
	while (row > 1)
		row -=2;
	DDRAM_Addr += (row << 6);
	while (coloumn > 63)
		coloumn -= 64;
	DDRAM_Addr += coloumn;
	HD44780_4b_sendCommand(DDRAM_Addr);
}

void HD44780_4b_write_char_cp(char character) {
	/* RS Signal UP */
	GPIO_WriteBit(GPIOA, PIN_RS, (BitAction) 1);
	/* Write Character */
	HD44780_4b_sendByte((uint8_t) character);
}


void HD44780_4b_write_cp(char *astring) {
	uint16_t i = 0;
	while ((astring[i] != '\0') & (i < 80)) {
		/* Write Character */
		HD44780_4b_write_char_cp(astring[i]);
		//HD44780_4b_sendByte((uint8_t) astring[i]);
		i++;
	}
}


void HD44780_4b_write_char(uint8_t row, uint8_t coloumn, char character) {
	/* First, Set new CP */
	HD44780_4b_setCp(row,coloumn);
	/* Then write character */
	HD44780_4b_write_char_cp(character);
}

void HD44780_4b_write(uint8_t row, uint8_t coloumn, char *astring) {
	uint16_t i = 0;
	/* First, Set new CP */
	HD44780_4b_setCp(row,coloumn);
	while ((astring[i] != '\0') & (i < 128)) {
		/* Write Character */
		HD44780_4b_write_char_cp(astring[i]);
		//HD44780_4b_sendByte((uint8_t) astring[i]);
		i++;
	}
}

void HD44780_4b_init(void) {
	
	/* wait for 40ms */
	delay_us(40000);
	
	/* GPIO Init */
	HD44780_4b_GPIOInit();
	
	/* Init Sequence from Datasheet */
	/* RS Signal Down */
	GPIO_WriteBit(GPIOA, PIN_RS, (BitAction) 0);
	/* Funcion Set 4b */
	HD44780_4b_sendByte(0x02);
	/* wait for 4.1ms */
	delay_us(4100);
	/* Function Set 4b, 2 Lines, Small Font */
	HD44780_4b_sendCommand(SET_4B_2LINES_5X8);
	HD44780_4b_sendCommand(SET_4B_2LINES_5X8);
	/* Turn Display On */
	HD44780_4b_sendCommand(DISPLAY_ON);
	/* Entry mode set increase counter and shift cursor to right after each write */
	HD44780_4b_sendCommand(SET_MOVE_CURSOR_RIGHT);
	/* Clear Display */
	HD44780_4b_sendCommand(CLEAR_DISPLAY);
}

void delay_us(uint16_t delay) {
	uint16_t i,j,us = SystemCoreClock / 1000000;
	if (us == 0)
		us = 1;
	for(i=delay; i>0; i--) {
		for(j=us; j>0; j--);
	}
}
