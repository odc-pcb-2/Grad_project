		/************************************************************************/
		/*                     include library needed                           */
		/************************************************************************/
#include "Types.h"
#include "DIO_interface.h"
#include "LCD_interface.h"
#include "KPD_interface.h"
#include "smart_lock.h"
#include "Control_Rooms.h"
#include "main.h"
#include "admin_control.h"
#include "Air_condition.h"
#define F_CPU 8000000UL
#include "util/delay.h"
		/************************************************************************/
		/*                  define variables needed								*/
		/************************************************************************/
#define angle	90					// define angle for servo motor
#define PASS_address 0x00			// address of stored pass in EEPROM
#define USER_address 0x60			// address of stored pass in EEPROM
#define KEY 0xAB					// KEY used to make encrypt to pass 
#define PASS_address_check 0x50		// address check if pass is already set before or not store only (0 or 1)
		/************************************************************************/
		/*                  declare variables needed							*/
		/************************************************************************/
u8 recive;							// declare variable to store recive data
u8 logInFlag;						// declare variable to store if who login
u8 choice;							// declare variable to store user choice
u8 UserId;							// declare variable to store user id
u8 pass[4];							// array to store entered pass
u8 EEPROM_pass[4];					// array to store pass which saved in EEPROM
u8 user[4];							// array to store entered username
u8 EEPROM_user[4];					// array to store username which saved in EEPROM
u8 u8ReturnedValue;					// store value returned from KPD
u8 pass_check;						// store value (1 or 0) which returned from PASS_address_check  
u8 check;							// used to check if pass is correct or not 
u8 check_user;						// used to check if pass is correct or not
u8 wrong_pass_count = 0;			// used to check tries remain 
u8 u8Status = 0;					// declare variable that will store status and initialize it with 0
u8 u8TurnOn  = 0;					// declare variable that will store turn on status and initialize it with 0
u8 u8TurnOff = 0;					// declare variable that will store turn off status and initialize it with 0
u8 response;						// declare variable to store response
u8 door = 0;						// declare variable that will store door status and initialize it with 0
		/************************************************************************/
		/*Function Name: Scan_pass												*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void
								scan pass from user								*/
		/************************************************************************/
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
			_delay_ms(150);								// delay 250m second for user see what number enter 
			LCD_vidMoveCursor(4, i+3);					// move LCD cursor to position of value entered 
			LCD_vidDisplayCharacter('*');				// display symbol * instead of entered number 
			pass[i] = u8ReturnedValue;					// store value in array called pass
	}
}
		/************************************************************************/
		/*Function Name: Scan_user												*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void
								scan user from user								*/
		/************************************************************************/
void Scan_user()
	{
		for (u8 i=0;i<4;i++)							// loop to take pass from user
		{
			do											// loop to take value of pressed key working until key pressed
			{
				u8ReturnedValue = KPD_u8GetPressedKey(GPIOC, HIGH_NIBBLE);// store value of pressed key in u8ReturnedValue
				_delay_ms(200);							// delay 200m second to can detect change
			} while (u8ReturnedValue == NOT_PRESSED);	// condition to break the loop
			LCD_vidDisplayCharacter(u8ReturnedValue);	// display pressed value on LCD
			_delay_ms(150);								// delay 250m second for user see what number enter
			user[i] = u8ReturnedValue;					// store value in array called pass
		}
}
		/************************************************************************/
		/*Function Name: write_EEPROM                                           */
		/*Function Input: take one value (from type u8)							*/
		/*Function Description: void function take void and return void                                                 
								write pass in EEPROM							*/
		/************************************************************************/
void write_EEPROM(u8 u8UserId)
{
	u8 User_Address = USER_address + (u8UserId * 0x8);	// calculate the address of user for store his username
	for (u8 i=0;i<4;i++)								// loop to take write pass in EEPROM
	{
		user[i]^=KEY;									// encrypt user entered before store in EEPROM by(XOR pass with KEY)
		EEPROM_vidWrite(User_Address + i, user[i]);		// save username in EEPROM
	}
	u8 Pass_Address = PASS_address +(u8UserId * 0x4);	// calculate the address of user for store his pass
	for (u8 i=0;i<4;i++)								// loop to take write pass in EEPROM 
	{
		pass[i]^=KEY;									// encrypt pass entered before store in EEPROM by(XOR pass with KEY) 
		EEPROM_vidWrite(Pass_Address + i, pass[i]);		// save pass in EEPROM
	}
}
		/************************************************************************/
		/*Function Name: read_EEPROM                                            */
		/*Function Input: take one value (from type u8)							*/
		/*Function Description: void function take void and return void                                                 
								read pass from EEPROM							*/
		/************************************************************************/
void read_EEPROM(u8 u8UserId)
{
	u8 User_Address = USER_address + (u8UserId * 0x8 );	// calculate the address of user for store his username
	for (u8 i=0;i<4;i++)								// loop to take write pass in EEPROM
	{
		EEPROM_user[i] = EEPROM_u8Read(User_Address + i)^KEY;// read pass stored in EEPROM and decrypting it by (XOR with KEY) then store in array
	}
	u8 Pass_Address = PASS_address + (u8UserId * 0x4 );	// calculate the address of user for store his pass
	for (u8 i=0;i<4;i++)								// loop to take write pass in EEPROM 
	{
		EEPROM_pass[i] = EEPROM_u8Read(Pass_Address+ i)^KEY;// read pass stored in EEPROM and decrypting it by (XOR with KEY) then store in array 
	}
}
		/************************************************************************/
		/*Function Name:                                                        */
		/*Function Input: no input (void)		                                */
		/*Function Description: void function take void and return void
		                        set pass if not set or read pass if already set	*/
		/************************************************************************/
void start()
{
	pass_check = EEPROM_u8Read(PASS_address_check);		// check if pass set or not by reading value in PASS_address_check
	if (pass_check != 0)								// check if value of pass_check equal to 0 or not 
	{
		LCD_vidDisplayString("Set Admin Name:");		// display message on LCD
		LCD_vidMoveCursor(2,1);							// move cursor of LCD to second line
		LCD_vidDisplayString(">>");						// display message on LCD
		Scan_user();									// call function that take username from user
		LCD_vidMoveCursor(3,1);							// move cursor of LCD third line
		LCD_vidDisplayString("Please,Set Pass:");		// display message on LCD
		LCD_vidMoveCursor(4,1);							// move LCD cursor to fourth line
		LCD_vidDisplayString(">>");						// display message on LCD screen
		Scan_pass();									// call function that take pass from user
		write_EEPROM(0x0);								// call function that write username and pass in EEPROM
		LCD_vidClearScreen();							// clear LCD screen 
		LCD_vidDisplayString("Admin Set ");				// display message on LCD screen
		LCD_vidMoveCursor(2,1);							// move cursor of LCD to second line
		LCD_vidDisplayString("Successfully");			// display message on LCD screen
		_delay_ms(200);									// wait for 200 m second
		EEPROM_vidWrite(PASS_address_check, 0);			// change value in PASS_address_check to zero (as pass is set)
		pass_check = EEPROM_u8Read(PASS_address_check);	// check if pass is set or not by reading value in PASS_address_check 
		LCD_vidClearScreen();							// clear LCD screen
		LCD_vidDisplayString("1.Admin Login ");			// display message on LCD screen
		LCD_vidMoveCursor(2,1);							// move cursor of LCD to second line
		LCD_vidDisplayString("2.User Login ");			// display message on LCD screen
		LCD_vidMoveCursor(3,1);							// move cursor of LCD to third line
		LCD_vidDisplayString(">>");						// display message on LCD screen
		choice = u8GetKeyPressed();						// take choice from user
		_delay_ms(150);									// wait for 150 m second
		logInFlag=choice;								// store choice in login flag variable
	}
	else												// if not
	{  
		LCD_vidClearScreen();							// clear LCD screen
		LCD_vidDisplayString("1.Admin Login ");			// display message on LCD screen
		LCD_vidMoveCursor(2,1);							// move cursor of LCD to second line
		LCD_vidDisplayString("2.User Login ");			// display message on LCD screen
		LCD_vidMoveCursor(3,1);							// move cursor of LCD to third line
		LCD_vidDisplayString(">>");						// display message on LCD screen
		choice = u8GetKeyPressed();						// take choice from user
		_delay_ms(150);									// wait for 150 m second
		logInFlag=choice;								// store choice in login flag variable
		if (choice == '1')
		{
			check_admin_pass();
		}
		else if (choice == '2')
		{
			check_user_pass();
		}
		else
		{
			LCD_vidClearScreen();
			LCD_vidDisplayString("wrong input");
			_delay_ms(150);
			start();
		}
	}
}
		/************************************************************************/
		/*Function Name: check_admin_pass										*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void                                                
								check pass is true or not						*/
		/************************************************************************/
void check_admin_pass()
{    
		    read_EEPROM(0x0);							// call function that read EEPROM 
			LCD_vidClearScreen();						// move LCD cursor to first line
			LCD_vidDisplayString("Enter UserName:");	// display message on LCD			
			LCD_vidMoveCursor(2, 1);					// move LCD cursor to next line
			LCD_vidDisplayString(">>");					// display message on LCD
			Scan_user();								// take pass from user
			check_user=0;								// initialize check variable to 0
			for (u8 i=0;i<4;i++)						// loop to check if pass entered is correct or not (same saved in EEPROM)
			{
				if (user[i] == EEPROM_user[i])			// check if two value are equal or not
				{
					check_user++;						// if that increase the check variable by one
				}
				else									// if not	 
				{
				}
			}
		LCD_vidMoveCursor(3, 1);						// move LCD cursor to third line
		LCD_vidDisplayString("Enter Password:");		// display message on LCD
		LCD_vidMoveCursor(4, 1);						// move LCD cursor to fourth line
		LCD_vidDisplayString(">>");						// display message on LCD
		Scan_pass();									// take pass from user
		check=0;										// initialize check variable to 0 
		for (u8 i=0;i<4;i++)							// loop to check if pass entered is correct or not (same saved in EEPROM)	
		{		
			if (pass[i] == EEPROM_pass[i])				// check if two value are equal or not 
			{
				check++;								// if that increase the check variable by one
			}
			else										// if not
			{
			}
		}
		LCD_vidClearScreen();							// clear LCD screen
		if(check == 4 && check_user == 4)				// check if check variable equal 4 or not (correct pass)
		{
			LED_vidTurnOn(GPIOD, PIN3);					// turn on admin mode led 
			LED_vidTurnOff(GPIOD,PIN4);					// turn off user mode led
			LED_vidTurnOff(GPIOB,PIN5);					// turn off block mode led
			LCD_vidDisplayString("Correct Password");	// display message on LCD
			LCD_vidMoveCursor(2,1);
			LCD_vidDisplayString("Login ");				// display message on screen
			LCD_vidMoveCursor(3,1);
			LCD_vidDisplayString("Successfully");		// display message on screen
			_delay_ms(200);								// delay 200m second to see message
			wrong_pass_count = 0;						// reset wrong_pass_count variable by reinitialize by 0
			admin_menue();
		}
		else											// if not
		{
			wrong_pass_count++;							// increase wrong_pass_count variable by one 
			LCD_vidDisplayString("Wrong password");		// display message on LCD
			LCD_vidMoveCursor(2, 1);					// move LCD cursor
			LCD_vidDisplayString("or user");			// display message on LCD
			LCD_vidMoveCursor(3, 1);					// move LCD cursor
			LCD_vidDisplayString("Rem times:");			// display message on LCD 
			LCD_vidDisplayNumber(3-wrong_pass_count);	// display number of reaming tries
		}
		_delay_ms(100);									// delay 100 m second
}
		/************************************************************************/
		/*Function Name: smart_lock												*/
		/*Function Input: no input (void)										*/
		/*Function Description: u8 type function take void and return variable form u8 type                                                 
								if pass wrong and exceed tries safe go on lock mode */
		/************************************************************************/
u8 smart_lock()
{
	if (choice=='1')									// if choice equal to 1
	{
		logInFlag=0;									// set login flag to 0
		check_admin_pass();								// call function that check admin username & pass
		Isadmin();										// call function that check login flag
	}
	else if (choice=='2')								// if choice equal to 2
	{
		logInFlag=1;									// set login flag to 1
		check_user_pass();								// call function that check user username & pass
		Isadmin();										// call function that check login flag
	}
	else												// if not 
	{
		LCD_vidClearScreen();							// clear LCD screen
		LCD_vidDisplayString("Wrong Choice");			// display message on LCD
		start();										// call function that start the system
	}
	if (wrong_pass_count == 3)							// check if user exceed maximum number of tries or not
	{
		wrong_pass_count = 0;							// set wrong pass counter to 0
		LCD_vidClearScreen();							// clear LCD screen
		LCD_vidDisplayString("Locked Mode!!");			// display message on LCD
		LED_vidTurnOn(GPIOB, PIN5);						// turn on block mode led
		LED_vidTurnOff(GPIOD,PIN4);						// turn off user mode led
		LED_vidTurnOff(GPIOD,PIN3);						// turn off admin mode led
		DIO_vidWritePin(GPIOD,PIN7,HIGH);				// output hight on pin to turn on buzzer
		_delay_ms(20000);								// wait for 20 second
		DIO_vidWritePin(GPIOD,PIN7,LOW);				// output hight on pin to turn off buzzer
		start();										// call function that start the system
	}
	return wrong_pass_count;							// return value of wrong_pass_count variable
}
		/************************************************************************/
		/*Function Name: Print                                                  */
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void                                                
								display rooms menu & control it					*/
		/************************************************************************/
void Print()
{
	u8 show_menu = MAIN_MENU;							// set variable to main menu	
	while(1)											//while loop to Show the menu
	{
		u8ReturnedValue = NOT_PRESSED;					//Set the key pressed by the user to its default value
		switch (show_menu)								// switch form menus
		{
			case MAIN_MENU:								// if main menu
			do											// do while loop
			{
				LCD_vidClearScreen();					// clear LCD screen
				LCD_vidDisplayString("1:Room1 2:Room2 ");// display message on LCD screen
				LCD_vidMoveCursor(2,1);					// move LCD cursor to second line
				LCD_vidDisplayString("3:Room3 4:Room4 ");// display message on LCD screen
				LCD_vidMoveCursor(3,1);					// move LCD cursor to third line
				LCD_vidDisplayString("5:Room5 6:Room6");// display message on LCD screen
				LCD_vidMoveCursor(4,1);					// move LCD cursor to fourth line
				LCD_vidDisplayString("0:Return >> ");	// display message on LCD screen
				u8ReturnedValue = u8GetKeyPressed();	// take choice from user 
				_delay_ms(200);							// wait for 200 m second
				if (u8ReturnedValue == '1')				// if choice equal 1
				{
					show_menu = ROOM1_MENU;				// display room 1 menu
				}
				else if (u8ReturnedValue == '2')		// if choice equal 2
				{
					show_menu = ROOM2_MENU;				// display room 2 menu
				}
				else if (u8ReturnedValue == '3')		// if choice equal 3
				{
					show_menu = ROOM3_MENU;				// display room 3 menu
				}
				else if (u8ReturnedValue == '4')		// if choice equal 4
				{
					show_menu = ROOM4_MENU;				// display room 4 menu
				}
				else if (u8ReturnedValue == '5')		// if choice equal 5
				{
					show_menu = ROOM5_MENU;				// display room 5 menu
				}
				else if (u8ReturnedValue == '6')		// if choice equal 6
				{
					show_menu = ROOM6_MENU;				// display room 6 menu
				}
				else if (u8ReturnedValue=='0')			// if choice equal 0
				{
					u8 login = Isadmin();
					if(login == 1)					// if returned value of Isadmin function equal to 1
					{
						Home_control_Menue();			// call function that return to admin home control
					}
					else if(login == 0) 				// if not
					{
						User_menue();					// call function that return to user home control
					}
				}
				else if(u8ReturnedValue != NOT_PRESSED)	// if user enter any thing else 
				{
					LCD_vidClearScreen();				// clear LCD screen
					LCD_vidDisplayString("Wrong input");// display message on LCD screen
					_delay_ms(200);						// wait for 200m second
				}
			} while  ((u8ReturnedValue < '0') || (u8ReturnedValue > '6') );// loop still work if choice less than 0 or greater than 6
			break;										// stop case
			case ROOM1_MENU:							// if room 1 menu
			control_rooms(ROOM1_MENU);					// call function that control room 1 lightning
			show_menu = MAIN_MENU;						// set menu to main menu
			break;										// stop case
			case ROOM2_MENU:							// if room 2 menu
			control_rooms(ROOM2_MENU);					// call function that control room 2 lightning
			show_menu = MAIN_MENU;						// set menu to main menu
			break;										// stop case
			case ROOM3_MENU:							// if room 3 menu
			control_rooms(ROOM3_MENU);					// call function that control room 3 lightning
			show_menu = MAIN_MENU;						// set menu to main menu
			break;										// stop case
			case ROOM4_MENU:							// if room 4 menu
			control_rooms(ROOM4_MENU);					// call function that control room 4 lightning
			show_menu = MAIN_MENU;						// set menu to main menu
			break;										// stop case
			case ROOM5_MENU:							// if room 5 menu
			control_rooms(ROOM5_MENU);					// call function that control room 5 lightning
			show_menu = MAIN_MENU;						// set menu to main menu
			break;										// stop case
			case ROOM6_MENU:							// if room 6 menu
			control_rooms(ROOM6_MENU);					// call function that control room 6 lightning
			show_menu = MAIN_MENU;						// set menu to main menu
			break;										// stop case
			u8ReturnedValue = u8GetKeyPressed();		// take choice from user
			_delay_ms(200);								// wait for 200 m second
			if ((u8ReturnedValue <'0' || u8ReturnedValue >'6'))// if choice less than 0 or greater than 6
			{
				LCD_vidClearScreen();					// clear LCD screen
				LCD_vidDisplayString("Wrong input");	// display message on LCD
				_delay_ms(150);							// wait for 150 m second
				continue;								// continue
			}
			break;										// break
		}
	}
}
		/************************************************************************/
		/*Function Name: check_user_pass                                        */
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void                                                
								check username & pass for user if correct or not*/
		/************************************************************************/
void check_user_pass()
{	
	LCD_vidClearScreen();								// move LCD cursor to first line
	LCD_vidDisplayString("  User LOGIN ");				// display message on LCD
	LCD_vidMoveCursor(2,1);								// move LCD cursor to second line
	LCD_vidDisplayString("ID from (0-9) ");				// display message on LCD
	LCD_vidMoveCursor(3,1);								// move LCD cursor to third line
	LCD_vidDisplayString("Enter User ID:");				// display message on LCD
	LCD_vidMoveCursor(4,1);								// move LCD cursor to fourth line
	LCD_vidDisplayString(">> ");						// display message on LCD
	UserId = u8GetKeyPressed();							// take user id from user
	read_EEPROM(UserId);								// read this user id from EEPROM
	_delay_ms(200);										// wait 200 m second
	LCD_vidClearScreen();								// move LCD cursor to next line
	LCD_vidDisplayString("Enter UserName:");			// display message on LCD
	LCD_vidMoveCursor(2,1);								// move LCD cursor to second line
	LCD_vidDisplayString(">>");							// display message on LCD
	Scan_user();										// take username & pass from user
	check_user=0;										// initialize check variable to 0
	for (u8 i=0;i<4;i++)								// loop to check if pass entered is correct or not (same saved in EEPROM)
	{
		if (user[i] == EEPROM_user[i])					// check if two value are equal or not
		{
			check_user++;								// if that increase the check variable by one
		}
		else											//if not
		{
		}
	}
	LCD_vidMoveCursor(3,1);								// move LCD cursor to first line
	LCD_vidDisplayString("Enter Password:");			// display message on LCD
	LCD_vidMoveCursor(4, 1);							// move LCD cursor to next line
	LCD_vidDisplayString(">>");							// display message on LCD
	Scan_pass();										// take pass from user
	check=0;											// initialize check variable to 0
	for (u8 i=0;i<4;i++)								// loop to check if pass entered is correct or not (same saved in EEPROM)
	{
		if (pass[i] == EEPROM_pass[i])					// check if two value are equal or not
		{
			check++;									// if that increase the check variable by one
		}
		else                                            // if not
		{
		}
	}
	LCD_vidClearScreen();								// clear LCD screen
	if(check == 4 && check_user == 4)					// check if check variable equal 4 or not (correct pass)
	{		
		LED_vidTurnOn(GPIOD, PIN4);						// turn on user mode led
		LED_vidTurnOff(GPIOD,PIN3);						// turn off admin mode led
		LED_vidTurnOff(GPIOB,PIN5);						// turn off block mode led
		LCD_vidDisplayString("Correct Password");		// display message on LCD
		LCD_vidMoveCursor(2,1);
		LCD_vidDisplayString("Login ");					// display message on screen
		LCD_vidMoveCursor(3,1);
		LCD_vidDisplayString("Successfully");			// display message on screen
		_delay_ms(250);									// delay 250m second to see message
		wrong_pass_count = 0;							// reset wrong_pass_count variable by reinitialize by 0
		User_menue(); 									// call function that display user control menu
	}
	else												// if not
	{
		wrong_pass_count++;								// increase wrong_pass_count variable by one
		LCD_vidDisplayString("Wrong password");			// display message on LCD
		LCD_vidMoveCursor(2, 1);						// move LCD cursor to second line
		LCD_vidDisplayString("or user");				// display message on LCD
		LCD_vidMoveCursor(3, 1);						// move LCD cursor to next line
		LCD_vidDisplayString("Rem times:");				// display message on LCD
		LCD_vidDisplayNumber(3-wrong_pass_count);		// display number of reaming tries
	}
		_delay_ms(100);									// wait for 100 m second
}
		/************************************************************************/
		/*Function Name: admin_menue                                            */
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void                                                
								display admin control menu						*/
		/************************************************************************/
void admin_menue()
{
	LCD_vidClearScreen();								// clear LCD screen
	LCD_vidDisplayString("1.Control Home");				// display message on LCD
	LCD_vidMoveCursor(2,1);								// move LCD cursor to second line
	LCD_vidDisplayString("2.Add user");					// display message on LCD
	LCD_vidMoveCursor(3,1);								// move LCD cursor to third line
	LCD_vidDisplayString("3.LOG Out");					// display message on LCD
	LCD_vidMoveCursor(4,1);								// move LCD cursor to fourth line
	LCD_vidDisplayString(">> ");						// display message on LCD
	choice = u8GetKeyPressed();							// take choice from user
	_delay_ms(150);										// wait for 150 m second
	switch(choice)										// switch for choice
	{
		case '1' :										// if choice equal 1
			Home_control_Menue();						// call function that display admin control menu
		break;											// break
		case '2' :										// if choice equal 2
			add_user();									// call function that add new user
		break;											// break
		case '3':										// if choice equal 3
			LogOut();									// call function that logout from the system
		break;											// break
		default:										// default case
			LCD_vidClearScreen();						// clear LCD screen
			LCD_vidDisplayString("Wrong Choice");		// display message on LCD
			admin_menue();								// call function that display admin menu		
	}
}
		/************************************************************************/
		/*Function Name: add_user                                               */
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void                                                
								add user to the system							*/
		/************************************************************************/
void add_user()
{
	LCD_vidClearScreen();								// clear LCD screen
	LCD_vidDisplayString("-- Add User --");					// display message on LCD
	LCD_vidMoveCursor(3,1);								// move LCD cursor to third line
	LCD_vidDisplayString("Set user ID:");				// display message on LCD
	LCD_vidMoveCursor(4,1);								// move LCD cursor to fourth line
	LCD_vidDisplayString(">>");							// display message on LCD
	UserId = u8GetKeyPressed();							// take choice from user
	_delay_ms(200);										// wait for 200 m second
	LCD_vidClearScreen();								// clear LCD screen
	LCD_vidDisplayString("Set UserName:");				// display message on LCD
	LCD_vidMoveCursor(2,1);								// move LCD cursor to second line
	LCD_vidDisplayString(">>");							// display message on LCD
	Scan_user();										// take username & pass from user
	LCD_vidMoveCursor(3,1);								// move LCD cursor to third line
	LCD_vidDisplayString("Please,Set Pass:");			// display message on LCD
	LCD_vidMoveCursor(4,1);								// move LCD cursor to next line
	LCD_vidDisplayString(">>");							// display message on LCD
	Scan_pass();										// take username & pass from user
	write_EEPROM(UserId);								// write this id in EEPROM
	LCD_vidClearScreen();								// clear LCD screen
	LCD_vidDisplayString("User Added ");				// display message on LCD
	LCD_vidMoveCursor(2,1);								// move LCD cursor to second line
	LCD_vidDisplayString("Successfully");				// display message on LCD
	_delay_ms(250);										// wait for 250 m second
	admin_menue();										// call function that display admin control menu
}
		/************************************************************************/
		/*Function Name: User_menue                                             */
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void                                                
								display user control menu						*/
		/************************************************************************/
void User_menue()
{
	LCD_vidClearScreen();								// clear LCD screen
	LCD_vidDisplayString("1.Room Lighting");			// display message on LCD
	LCD_vidMoveCursor(2,1);								// move LCD cursor to second line
	LCD_vidDisplayString("2.Air Condition");			// display message on LCD
	LCD_vidMoveCursor(3,1);								// move LCD cursor to third line
	LCD_vidDisplayString("3.Log Out");					// display message on LCD
	LCD_vidMoveCursor(4,1);								// move LCD cursor to fourth line
	LCD_vidDisplayString(">> ");					// display message on LCD
	choice = u8GetKeyPressed();							// take choice from the user
	_delay_ms(150);										// wait for 150 m second
	switch(choice)										// switch for choice
	{
		case '1':										// if choice equal 1
			Print();									// call function that display rooms control
		break;											// break
		case '2' :										// if choice equal 2
			Air_condition();							// call function that control air condition
		break;											// break
		case '3' :										// if choice equal 3
			LogOut();									// call function that logout from the system
		break;											// break
		default:										// default case
			LCD_vidClearScreen();						// clear LCD screen
			LCD_vidDisplayString("Wrong input");		// display message on LCD
			_delay_ms(250);								// wait for 250 m second
			User_menue();								// call function that display user control menu
	}	
}
		/************************************************************************/
		/*Function Name: Isadmin                                                */
		/*Function Input: no input (void)										*/
		/*Function Description: u8 function take void and return u8                                                
								check who login user or admin & return only 0 or 1*/
		/************************************************************************/
u8 Isadmin()
{
	if (logInFlag==0)									// if login flag = 0
	{
		return 1;										// return 1
	}
	else												// if not 
	{
		return 0;										// return 0
	}
}
		/************************************************************************/
		/*Function Name: LogOut													*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void                                                
								logout from the system and return to start screen*/
		/************************************************************************/
void LogOut()
{	
	LCD_vidClearScreen();								// clear LCD screen
	LCD_vidMoveCursor(2,1);								// move LCD cursor to second line 
	LCD_vidDisplayString(" Logged Out ");				// display message on LCD
	LCD_vidMoveCursor(3,1);								// move LCD cursor to third line
	LCD_vidDisplayString(" Successfully");				// display message on LCD
	_delay_ms(500);										// wait for 500 m second
	LCD_vidClearScreen();								// clear LCD screen
	LCD_vidDisplayString("1.Admin Login");				// display message on LCD
	LCD_vidMoveCursor(2,1);								// move LCD cursor to second line
	LCD_vidDisplayString("2.User Login");				// display message on LCD
	LCD_vidMoveCursor(3,1);								// move LCD cursor to third line
	LCD_vidDisplayString(">>");							// display message on LCD 
	u8ReturnedValue = u8GetKeyPressed();				// take choice from user
	_delay_ms(150);										// wait for 150 m second
	if(u8ReturnedValue=='1')							// if choice equal 1
	{
		check_admin_pass();								// call function that check username & pass for admin
	}
	else if(u8ReturnedValue=='2')						// if choice equal 2
	{
		check_user_pass();								// call function that check username & pass for user
	}
	else												// if not	
	{
		LCD_vidClearScreen();							// clear LCD screen
		LCD_vidDisplayString("Wrong Choice");			// display message on LCD
//		LogOut();										// call function that logout from the system
		start();
	}
}
		/************************************************************************/
		/*Function Name: Open_Door												*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void                                                
								control door opening & closing					*/
		/************************************************************************/
void Open_Door()
{
	LCD_vidClearScreen();								// clear LCD screen
	LCD_vidDisplayString("DOOR IS: ");					// display message on LCD
	if(door == 1)										// if door equal to 1
	{
		LCD_vidDisplayString("OPEN");					// display message on LCD
	}
	else if(door == 0)									// if door equal to 0	
	{
		LCD_vidDisplayString("CLOSE");					// display message on LCD
	}	
	LCD_vidMoveCursor(2,1);								// move cursor to second line	
	LCD_vidDisplayString(">> 1-open door ");			// display message on LCD
	LCD_vidMoveCursor(3,1);								// move cursor to third line
	LCD_vidDisplayString(">> 2-close door ");			// display message on LCD
	LCD_vidMoveCursor(4,1);								// move cursor to fourth line
	LCD_vidDisplayString(">> 0-RET ");					// display message on LCD
//	_delay_ms(250);										// wait for 250 m second
	u8ReturnedValue = u8GetKeyPressed();				// take choice from user
//	_delay_ms(200);										// wait for 200 m second
	if (u8ReturnedValue == '1')							// if choice equal 1
	{
		LCD_vidClearScreen();							// clear LCD screen
		LCD_vidDisplayString("door opening----");		// display message on LCD
		Servo_Clockwise(angle);							// turn on the servo on clock wise
		_delay_ms(1000);								// wait for one second
		door= 1;										// set door equal 1
	}
	else if(u8ReturnedValue == '2')						// if choice equal 2
	{
		LCD_vidClearScreen();							// clear LCD screen
		LCD_vidDisplayString("door closing----");		// display message on LCD
		Servo_Counterclockwise(angle);					// turn on the servo on anti clock wise
		_delay_ms(1000);								// wait for one second
		door = 0;										// set door equal 0
	}
	else if (u8ReturnedValue == '0')					// if choice equal 0
	{
		Home_control_Menue();							// call function that display admin control menu
	}
	else												// if not 
	{
		LCD_vidClearScreen();							// clear LCD screen
		LCD_vidDisplayString("wrong input");			// display message on LCD
		Open_Door();									// call function that control door opening
	}
	Home_control_Menue();								// call function that display admin control menu 
}