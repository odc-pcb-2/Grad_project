/*
 * EEPROM.c
 *
 * Created: 12/11/2022 7:07:51 PM
 * Author : Ahmed Emad
 */ 

#include "Types.h"
#include "BIT_MATH.h"

#include "EEPROM_interface.h"
#include "LCD_interface.h"
#include "KPD_interface.h"

#include "smart_lock.h"

#include "util/delay.h"

//#define F_CPU 8000000UL

int main(void)
{
	LCD_vidInit();											// initialize LCD
	KPD_vidInit(GPIOC, HIGH_NIBBLE);						// initialize KPD 
	LCD_vidMoveCursor(1,5);
	LCD_vidDisplayString("Welcome");						// display message on screen
	LCD_vidMoveCursor(2,2);
	LCD_vidDisplayString("to my home");						// display message on screen
	_delay_ms(1000);											// delay 250m second
	LCD_vidClearScreen();									// clear LCD screen
	start();												// sett pass if not set or read pass if already set 
    while (1) 
    {
		u8 wrong_pass_count = smart_lock();					// if pass wrong and exceed tries safe go on lock mode
		if (wrong_pass_count == 0 || wrong_pass_count == 3)	// if wrong_pass_count equal to (0 when pass correct) or (3 when user exceed number of tries)
		{
			break;											// stop the loop
		}
	}
}
