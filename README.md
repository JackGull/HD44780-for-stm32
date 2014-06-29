HD44780-for-stm32
=================

A driver for the HD44780 Display Controller (And compatibles) for the stm32 micropocessors.
Because i needed one and couldn't find any.

Please, feel free to use this library. If you modify or improve it, please notify me.

This is a driver for HD44780 compatible displays, written for and tested with 
the STM32VL Discovery board. Should not be hard to port it on other STM32 devices.

V1.0 (Current):
		First release. Only 4 bit interface is supported and only specific pins are used.
		PA1 - RS
		PA2 - R/W
		PA3 - E
		PC0 = D4
		PC1 = D5
		PC2 = D6
		PC3 = D7

Functions and defines in the .h file should be self explanatory. Will add a full readme in the future
