#include "Types.h"
#include "DIO_interface.h"
#include "LCD_interface.h"
#include "KPD_interface.h"
#include "smart_lock.h"
#define F_CPU 8000000UL
#include "util/delay.h"
#define PASS_address 0x20			// address of stored pass in EEPROM
#define USER_address 0x60			// address of stored pass in EEPROM
#define KEY 0xAB					// KEY used to make encrypt to pass 
#define PASS_address_check 0x50		// address check if pass is already set before or not store only (0 or 1)

u8 pass[4];					// array to store entered pass
u8 EEPROM_pass[4];			// array to store pass which saved in EEPROM

u8 user[8];					// array to store entered user
u8 EEPROM_user[8];			// array to store user which saved in EEPROM

u8 u8ReturnedValue;			// store value returned from KPD
u8 pass_check;				// store value (1 or 0) which returned from PASS_address_check  
u8 check;					// used to check if pass is correct or not 
u8 check_user;					// used to check if pass is correct or not
u8 wrong_pass_count = 0;	// used to check tries remain 

	/* scan pass from user */
	/* void function take void and return void */
void Scan_pass()
{
	for (u8 i=0;i<4;i++)		// loop to take pass from user 
	{
		do						// loop to take value of pressed key working until key pressed
		{
			u8ReturnedValue = KPD_u8GetPressedKey(GPIOC, HIGH_NIBBLE);	// store value of pressed key in u8ReturnedValue
			_delay_ms(200);								// delay 200m second to can detect change
		} while (u8ReturnedValue == NOT_PRESSED);		// condition to break the loop
			LCD_vidDisplayCharacter(u8ReturnedValue);	// display pressed value on LCD
			_delay_ms(250);								// delay 250m second for user see what number enter 
			LCD_vidMoveCursor(4, i+1);					// move LCD cursor to position of value entered 
			LCD_vidDisplayCharacter('*');				// display symbol * instead of entered number 
			pass[i] = u8ReturnedValue;					// store value in array called pass
	}
}
	/* scan user from user */
	/* void function take void and return void */
	void Scan_user()
	{
		for (u8 i=0;i<8;i++)		// loop to take pass from user
		{
			do						// loop to take value of pressed key working until key pressed
			{
				u8ReturnedValue = KPD_u8GetPressedKey(GPIOC, HIGH_NIBBLE);	// store value of pressed key in u8ReturnedValue
				_delay_ms(200);								// delay 200m second to can detect change
			} while (u8ReturnedValue == NOT_PRESSED);		// condition to break the loop
			LCD_vidDisplayCharacter(u8ReturnedValue);	// display pressed value on LCD
			_delay_ms(250);								// delay 250m second for user see what number enter
			LCD_vidMoveCursor(2, i+1);					// move LCD cursor to position of value entered
			LCD_vidDisplayCharacter('*');				// display symbol * instead of entered number
			user[i] = u8ReturnedValue;					// store value in array called pass
		}
	}
	/* write pass in EEPROM */
	/* void function take void and return void */
	
void write_EEPROM()
{
	for (u8 i=0;i<8;i++)								// loop to take write pass in EEPROM
	{
		user[i]^=KEY;									// encrypt user entered before store in EEPROM by(XOR pass with KEY)
		EEPROM_vidWrite(USER_address + i, user[i]);		// save user in EEPROM
	}
	for (u8 i=0;i<4;i++)								// loop to take write pass in EEPROM 
	{
		pass[i]^=KEY;									// encrypt pass entered before store in EEPROM by(XOR pass with KEY) 
		EEPROM_vidWrite(PASS_address + i, pass[i]);		// save pass in EEPROM
	}
}
	/* read pass from EEPROM */
	/* void function take void and return void */
	
void read_EEPROM()
{
	for (u8 i=0;i<8;i++)										// loop to take write pass in EEPROM
	{
		EEPROM_user[i] = EEPROM_u8Read(USER_address + i)^KEY;	// read pass stored in EEPROM and decrypting it by (XOR with KEY) then store in array
	}
	for (u8 i=0;i<4;i++)										// loop to take write pass in EEPROM 
	{
		EEPROM_pass[i] = EEPROM_u8Read(PASS_address + i)^KEY;	// read pass stored in EEPROM and decrypting it by (XOR with KEY) then store in array 
	}
}
	/* sett pass if not set or read pass if already set */
	/* void function take void and return void */	
void start()
{
	pass_check = EEPROM_u8Read(PASS_address_check);		// check if pass set or not by reading value in PASS_address_check
	if (pass_check != 0)								// check if value of pass_check equal to 0 or not 
	{
		LCD_vidDisplayString("please,set user:");		// display message on LCD
		LCD_vidMoveCursor(2,1);
		Scan_user();
		LCD_vidMoveCursor(3,1);
		LCD_vidDisplayString("please,set pass:");		// display message on LCD
		LCD_vidMoveCursor(4,1);							// move LCD cursor to next line
		Scan_pass();									// take pass from user
		write_EEPROM();
		LCD_vidClearScreen();
		LCD_vidDisplayString("user added ");		// display message on LCD
		LCD_vidMoveCursor(2,1);
		LCD_vidDisplayString("Successfully");
		_delay_ms(250);
		EEPROM_vidWrite(PASS_address_check, 0);			// change value in PASS_address_check to zero (as pass is set)
		pass_check = EEPROM_u8Read(PASS_address_check);	// check if pass is set or not by reading value in PASS_address_check 
		read_EEPROM();									// read value from EEPROM
	}
	else
	{
		read_EEPROM();									// read value from EEPROM
	}
}
	/* check pass is true or not */
	/* void function take void and return void */
void check_pass()
{
			LCD_vidMoveCursor(1, 1);					// move LCD cursor to first line
			LCD_vidDisplayString("Enter User:");	// display message on LCD
			LCD_vidMoveCursor(2, 1);					// move LCD cursor to next line
			Scan_user();								// take pass from user
			check_user=0;									// initialize check variable to 0
			for (u8 i=0;i<8;i++)						// loop to check if pass entered is correct or not (same saved in EEPROM)
			{
				if (user[i] == EEPROM_user[i])			// check if two value are equal or not
				{
					check_user++;							// if that increase the check variable by one
				}
				else
				{
				}
			}
		LCD_vidMoveCursor(3, 1);					// move LCD cursor to first line
		LCD_vidDisplayString("Enter Password:");	// display message on LCD
		LCD_vidMoveCursor(4, 1);					// move LCD cursor to next line
		Scan_pass();								// take pass from user
		check=0;									// initialize check variable to 0 
		for (u8 i=0;i<4;i++)						// loop to check if pass entered is correct or not (same saved in EEPROM)	
		{		
			if (pass[i] == EEPROM_pass[i])			// check if two value are equal or not 
			{
				check++;							// if that increase the check variable by one
			}
			else
			{
			}
		}
		LCD_vidClearScreen();						// clear LCD screen
		if(check == 4 && check_user == 8)								// check if check variable equal 4 or not (correct pass)
		{
			LCD_vidDisplayString("Correct Password");	// display message on LCD
			_delay_ms(250);								// delay 250m second to see message
			wrong_pass_count = 0;						// reset wrong_pass_count variable by reinitialize by 0
			LCD_vidClearScreen();						// clear LCD screen	
			LCD_vidDisplayString("Lock Opened");		// display message on screen
		}
		else
		{
			wrong_pass_count++;							// increase wrong_pass_count variable by one 
			LCD_vidDisplayString("Wrong password");		// display message on LCD
			LCD_vidMoveCursor(2, 1);					// move LCD cursor
			LCD_vidDisplayString("or user");		// display message on LCD
			LCD_vidMoveCursor(3, 1);					// move LCD cursor
			LCD_vidDisplayString("Rem times:");			// display message on LCD 
			LCD_vidDisplayNumber(3-wrong_pass_count);	// display number of reaming tries
		}
		_delay_ms(1000);								// delay one second
}
	/* if pass wrong and exceed tries safe go on lock mode */
	/* u8 type function take void and return variable form u8 type */
u8 smart_lock()
{
	LCD_vidClearScreen();					// clear LCD screen
	if (pass_check == 0)					// check if pass is set or not
	{
		check_pass();						// check if pass is correct or not 
	}
	if (wrong_pass_count == 3)				// check if user exceed maximum number of tries or not
	{
		LCD_vidClearScreen();				// clear LCD screen
		LCD_vidDisplayString("Locked Mode!!");	// display message on LCD
		DIO_vidWritePin(GPIOD,PIN7,HIGH);
		_delay_ms(15000);
		DIO_vidWritePin(GPIOD,PIN7,LOW);
	}
	return wrong_pass_count;				// return value of wrong_pass_count variable
}
