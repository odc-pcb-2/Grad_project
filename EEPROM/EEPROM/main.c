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





	// save input in array
	/*
		do
		{
			u8ReturnedValue = KPD_u8GetPressedKey(GPIOC, HIGH_NIBBLE);
			_delay_ms(200);
		} while (u8ReturnedValue == NOT_PRESSED);
		LCD_vidDisplayCharacter(u8ReturnedValue);
		u8arr[u8counter] = u8ReturnedValue;
		u8counter++;
	*/
		//Scan_pass();	
	//	if (u8ReturnedValue == '=')
		//{
			// write in EEPROM
		/*
			for (u8 i=0;i<4;i++)
			{
				EEPROM_vidWrite(PASS_address + i, u8arr[i]);
			}
		*/
			/*
			write_EEPROM();
			LCD_vidClearScreen();
			LCD_vidDisplayString("enter pass");
			LCD_vidMoveCursor(2, 1);
			read_EEPROM();
			*/	
			// read for check	
			/*
			for (u8 i=0;i<4;i++)
			{
				re[i] = EEPROM_u8Read(PASS_address + i);
				LCD_vidDisplayCharacter(re[i]);
			}
			*/
			//re = EEPROM_u8Read(u8arr);
			//LCD_vidDisplayNumber(re);			
		//}

/*
#define PASS_address 20

#define PASS_address_check 50

u8 pass[4];
u8 EEPROM_pass[4];

u8 u8ReturnedValue;
u8 pass_check;
u8 check;
u8 wrong_pass_count = 0;

void Scan_pass()
{
	for (u8 i=0;i<4;i++)
	{
		do
		{
			u8ReturnedValue = KPD_u8GetPressedKey(GPIOC, HIGH_NIBBLE);
			_delay_ms(200);
		} while (u8ReturnedValue == NOT_PRESSED);
			LCD_vidDisplayCharacter(u8ReturnedValue);
			pass[i] = u8ReturnedValue;	
	}
}
void write_EEPROM()
{
	for (u8 i=0;i<4;i++)
	{
		EEPROM_vidWrite(PASS_address + i, pass[i]);
	}
}
void read_EEPROM()
{
	for (u8 i=0;i<4;i++)
	{
		EEPROM_pass[i] = EEPROM_u8Read(PASS_address + i);
	}
}

void start()
{
	pass_check = EEPROM_u8Read(PASS_address_check);
	if (pass_check != 0)
	{
		LCD_vidDisplayString("enter pass set:");
		LCD_vidMoveCursor(2,1);
		Scan_pass();
		write_EEPROM();
		EEPROM_vidWrite(PASS_address_check, 0);
	}
	else
	{
		read_EEPROM();
	}
	pass_check = EEPROM_u8Read(PASS_address_check);
}

void check_pass()
{
		LCD_vidMoveCursor(1, 1);
		LCD_vidDisplayString("Enter Password:");
		LCD_vidMoveCursor(2, 1);
		Scan_pass();
		check=0;
		for (u8 i=0;i<4;i++)
		{
			if (pass[i] == EEPROM_pass[i])
			{
				check++;
			}
		}
		LCD_vidClearScreen();
		if(check == 4)
		{
			LCD_vidDisplayString("Correct Password");
			wrong_pass_count = 0;
		}
		else
		{
			wrong_pass_count++;
			LCD_vidDisplayString("Wrong password");
			LCD_vidMoveCursor(2, 1);
			LCD_vidDisplayString("Rem times:");
			LCD_vidDisplayNumber(3-wrong_pass_count);
		}
		_delay_ms(1000);
}
*/

// 		LCD_vidClearScreen();
// 				if (pass_check == 0)
// 				{
// 					check_pass();
// 					if (wrong_pass_count == 0)
// 					{
// 						break;
// 					}
// 				}
// 				if (wrong_pass_count == 3)
// 				{
// 					LCD_vidClearScreen();
// 					LCD_vidDisplayString("Locked!!");
// 					break;
//}
