#include "main_config.h"
#include "LCD_interface.h"
#include "EEPROM_interface.h"
// #include "timer_driver.h"
#include "KPD_interface.h"
#include "SPI_interface.h"
#include "LED_interface.h"
#include "menu.h"
#include <avr/io.h>

// volatile uint16 session_counter = 0;//indicate session time
// uint8 timeout_flag = FALSE;//stores if the session is still valid or outdated

int main(void)
{
	u8 temperature = 0;//The average temperature of the room
	u8 temp_ones = NOT_SELECTED;//The entered right number of the temperature
	u8 temp_tens = NOT_SELECTED;//The entered left number of the temperature
	
	u8 login_mode = NO_MODE; //Store the current user mode admin or guest or not logged in
	u8 block_mode_flag = FALSE;//is true if the login is blocked or false if not blocked
	u8 key_pressed = NOT_PRESSED;//
	/*****************  INITIALIZE  ***********************/
	LED_vidInit(ADMIN_LED_PORT,ADMIN_LED_PIN);//initializes the led of admin
	LED_vidInit(GUEST_LED_PORT,GUEST_LED_PIN);//initializes the led of guest
	LED_vidInit(BLOCK_LED_PORT,BLOCK_LED_PIN);//initializes the led of block
	LCD_vidInit();//initializes the LCD screen
	KPD_vidInit();//initializes the keypad
	SPI_vidInit();//initializes the communication protocol of SPI
	/******************************************************/
	/* Printing Welcome screen */
	LCD_vidDisplayString("Welcome to smart");
	LCD_vidMoveCursor(2,1);
	LCD_vidDisplayString("home system");
	_delay_ms(1000);//Halt the system for the given time in (ms)
	LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
	/***************************/
	/*Setting Admin and Guest passwords if not set */
	//read the state of the the passwords of the admin and guest if both are set or not set
	if ( (EEPROM_u8Read(ADMIN_PASS_STATUS_ADDRESS)!=PASS_SET) || (EEPROM_u8Read(GUEST_PASS_STATUS_ADDRESS)!=PASS_SET) )
	{
		LCD_vidDisplayString("Login for");//printing login menu
		LCD_vidMoveCursor(2,1);//move the cursor to the second line
		LCD_vidDisplayString("first time");
		_delay_ms(1000);//Halt the system for the given time in (ms)
		LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
		LCD_vidDisplayString("Set Admin pass");//printing the set admin password menu
		LCD_vidMoveCursor(2,1);
		LCD_vidDisplayString("Admin pass:");

		/********************************* setting Admin password **********************************************/
		u8 pass_counter=0;//the counter of the characters of the password
		u8 pass[PASS_SIZE]={NOT_STORED,NOT_STORED,NOT_STORED,NOT_STORED};//the array where it stored the password
		while (pass_counter<PASS_SIZE)//loop till the user finish inserting the pass
		{
			key_pressed = NOT_PRESSED;//return the variable that holds the pressed key from keypad to its initial value
			while (key_pressed == NOT_PRESSED)//repeat till the user press any key
			{
				key_pressed = KPD_u8GetPressedKey();//if the user pressed any button in keypad save the value in key_pressed
			}

			pass[pass_counter]=key_pressed;//add the entered character to the pass array
			LCD_vidDisplayCharacter(key_pressed);//print the entered character
			_delay_ms(CHARACTER_PREVIEW_TIME);//Halt the system for the given time in (ms)
			LCD_vidMoveCursor(2,12+pass_counter);//move the lcd cursor to the previous location to write the password symbol over the character
			LCD_vidDisplayCharacter(PASSWORD_SYMBOL); // to display (Password sign *)
			_delay_ms(100);//Halt the system for the given time in (ms)
			pass_counter++;//increase the characters count
		}
		EEPROM_vidWrite(EEPROM_ADMIN_ADDRESS,pass,PASS_SIZE);//save the entire password as a block to the EEPROM
		EEPROM_vidWrite(ADMIN_PASS_STATUS_ADDRESS,PASS_SET);//write the status of pass as it is set
		LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
		LCD_vidDisplayString("Pass Saved");// show pass saved message
		_delay_ms(500);//Halt the system for the given time in (ms)
		LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row

		/********************************* setting guest password **********************************************/
		pass_counter=0;//reset password counter which count the characters of the pass
		LCD_vidDisplayString("Set Guest Pass");//printing the set admin password menu
		LCD_vidMoveCursor(2,1);
		LCD_vidDisplayString("Guest Pass:");
		while (pass_counter<PASS_SIZE)//loop till the user finish inserting the pass
		{
			key_pressed = NOT_PRESSED;//return the variable that holds the pressed key from keypad to its initial value
			while (key_pressed == NOT_PRESSED)//repeat till the user press any key
			{
				key_pressed = KPD_u8GetPressedKey();//if the user pressed any button in keypad save the value in key_pressed
			}

			pass[pass_counter]=key_pressed;//add the entered character to the pass array
			LCD_vidDisplayCharacter(key_pressed);//print the entered character
			_delay_ms(CHARACTER_PREVIEW_TIME);//Halt the system for the given time in (ms)
			LCD_vidMoveCursor(2,12+pass_counter);//move the lcd cursor to the previous location to write the password symbol over the character
			LCD_vidDisplayCharacter(PASSWORD_SYMBOL); // to display (Password sign *)
			_delay_ms(100);//Halt the system for the given time in (ms)
			pass_counter++;//increase the characters count
		}
		EEPROM_vidWrite(EEPROM_GUEST_ADDRESS,pass,PASS_SIZE);//save the entire password as a block to the EEPROM
		EEPROM_vidWrite(GUEST_PASS_STATUS_ADDRESS,PASS_SET);//write the status of pass as it is set
		LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
		LCD_vidDisplayString("Pass Saved");//move the lcd cursor to the previous location
		_delay_ms(500);//Halt the system for the given time in (ms)
		LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
		EEPROM_vidWrite(LOGIN_BLOCKED_ADDRESS,FALSE);
	}//The end of if admin and guest password is set
	else//this code of else run only if the system is not running for the first time (ADMIN and GUEST passwords are set)
	{
		block_mode_flag = EEPROM_u8Read(LOGIN_BLOCKED_ADDRESS); //read the blocked location from EEPROM
	}
	while (1)//The start of the periodic code
	{
		key_pressed = NOT_PRESSED;//return the variable that holds the pressed key from keypad to its initial value
		u8 pass_tries_count=0;//stores how many times the user tried to log in to the system and failed
		
		// if ( timeout_flag==TRUE )//check for timeout
		{//if timeout flag was raised
			timer0_stop();//stop the timer that increase the session counter
		//	session_counter = 0;//clear session counter
		//	timeout_flag=FALSE;//clear time out flag
			login_mode=NO_MODE;//log the user out
			key_pressed = NOT_PRESSED;//clear the key_pressed to avoid unwanted selection in the menu switch
			LED_vidTurnOff(GUEST_LED_PORT,GUEST_LED_PIN);//turnoff the led of the guest
			LED_vidTurnOff(ADMIN_LED_PORT,ADMIN_LED_PIN);//turnoff the led of the admin
			LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
			LCD_vidDisplayString("Session Timeout");//print session timeout message
			_delay_ms(1000);//Halt the system for the given time in (ms)
		}
		while (login_mode==NO_MODE)//The user can only leave the loop only in case of he was logged in as guest or admin
		{
			if(block_mode_flag==TRUE)//if the login process was blocked wait till the end of the block period
			{
				LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
				LCD_vidDisplayString("Login blocked");
				LCD_vidMoveCursor(2,1);
				LCD_vidDisplayString("wait 20 seconds");
				LED_vidTurnOn(BLOCK_LED_PORT,BLOCK_LED_PIN);//Turn on the led of Blocked
				_delay_ms(BLOCK_MODE_TIME);//Halt the system for the given time in (ms)
				pass_tries_count = 0; //Clear the count on number of wrong tries
				block_mode_flag = FALSE;//Disable block of runtime
				LED_vidTurnOff(BLOCK_LED_PORT,BLOCK_LED_PIN);//Turn off the led of Blocked
				EEPROM_vidWrite(LOGIN_BLOCKED_ADDRESS,FALSE);//write false at blocked location in EEPROM
			}
			LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
			LCD_vidDisplayString("Select mode :");
			LCD_vidMoveCursor(2,1);
			LCD_vidDisplayString("0:Admin 1:Guest");
			while(key_pressed==NOT_PRESSED)//wait for the selection of the mode
			{
				key_pressed = KPD_u8GetPressedKey();//if the user pressed any button in keypad save the value in key_pressed
			}
			if ( key_pressed!=CHECK_ADMIN_MODE && key_pressed!=CHECK_GUEST_MODE )
			{
				LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
				LCD_vidDisplayString("Wrong input.");//Prints error message on the LCD
				key_pressed = NOT_PRESSED;//return the variable that holds the pressed key from keypad to its initial value
				_delay_ms(1000);//Halt the system for the given time in (ms)
				continue;//return to the loop of login #while (login_mode==NO_MODE)# line 128
			}

			u8 pass_counter=0;//counts the entered key of the password from the keypad
			u8 pass[PASS_SIZE]={NOT_STORED,NOT_STORED,NOT_STORED,NOT_STORED};//temporarily hold the entire password that will be entered by the user to be check
			u8 stored_pass[PASS_SIZE]={NOT_STORED,NOT_STORED,NOT_STORED,NOT_STORED};//temporarily hold the entire stored password that is written by the user for the first time
			
			switch(key_pressed)
			{
				/********************************* Admin login **********************************************/
				case CHECK_ADMIN_MODE:
				while(login_mode!=ADMIN)//this loop is to repeat the login for admin in case of wrong password
				{
					key_pressed = NOT_PRESSED;//return the variable that holds the pressed key from keypad to its initial value
					LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
					LCD_vidDisplayString("Admin mode");
					LCD_vidMoveCursor(2,1);
					LCD_vidDisplayString("Enter Pass:");
					_delay_ms(200);//Halt the system for the given time in (ms)
					pass_counter=0;//counts the number of entered characters
					while(pass_counter<PASS_SIZE)
					{
						while (key_pressed == NOT_PRESSED)//repeat till the user press any key
						{
							key_pressed = KPD_u8GetPressedKey();//if the user pressed any button in keypad save the value in key_pressed
						}
						pass[pass_counter]=key_pressed;//add the entered character to the pass array
						LCD_vidDisplayCharacter(key_pressed);//print the entered character
						_delay_ms(CHARACTER_PREVIEW_TIME);//Halt the system for the given time in (ms)
						LCD_vidMoveCursor(2,12+pass_counter);//move the cursor of the lcd screen to the previous location
						LCD_vidDisplayCharacter(PASSWORD_SYMBOL);// to display (Password sign *)
						_delay_ms(100);//Halt the system for the given time in (ms)
						pass_counter++;//increase the password counter that count the characters of the pass
						key_pressed = NOT_PRESSED;//return the variable that holds the pressed key from keypad to its initial value
					}
					EEPROM_vReadBlockFromAddress(EEPROM_ADMIN_ADDRESS,stored_pass,PASS_SIZE);//read the stored pass from the EEPROM

					/*compare passwords*/
					if ((ui8ComparePass(pass,stored_pass,PASS_SIZE)) == TRUE)//in case of right password
					{
						login_mode = ADMIN;//set the login mode to admin mode
						pass_tries_count=0;//clear the counter of wrong tries
						LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
						LCD_vidDisplayString("Right pass");
						LCD_vidMoveCursor(2,1);
						LCD_vidDisplayString("Admin mode");
						_delay_ms(500);//Halt the system for the given time in (ms)
						LED_vidTurnOn(ADMIN_LED_PORT,ADMIN_LED_PIN);//turn on the led of admin
					//	timer0_initializeCTC();//start the timer that counts the session time
						LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
					}
					else//in case of wrong password
					{
						pass_tries_count++;//increase the number of wrong tries to block login if it exceeds the allowed tries
						login_mode = NO_MODE;//set the mode as not logged in
						LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
						LCD_vidDisplayString("Wrong Pass");
						LCD_vidMoveCursor(2,1);
						LCD_vidDisplayString("Tries left:");
						LCD_vidDisplayCharacter(TRIES_ALLOWED-pass_tries_count+ASCII_ZERO);//print the number of tries left before block mode to be activated
						_delay_ms(1000);//Halt the system for the given time in (ms)
						LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
						if (pass_tries_count>=TRIES_ALLOWED)//if the condition of the block mode is true
						{
							EEPROM_vidWrite(LOGIN_BLOCKED_ADDRESS,TRUE);//write to the EEPROM TRUE to the the block mode address
							block_mode_flag = TRUE;//turn on block mode
							break;//break the loop of admin login #while(login_mode!=ADMIN)# at line 169
						}
					}
				}
				break;//bREAK SWITCH case
				/********************************* Guest login **********************************************/
				case  CHECK_GUEST_MODE:
				while(login_mode != GUEST)
				{
					key_pressed = NOT_PRESSED;//return the variable that holds the pressed key from keypad to its initial value
					LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
					LCD_vidDisplayString("Guest mode");
					LCD_vidMoveCursor(2,1);
					LCD_vidDisplayString("Enter pass:");
					_delay_ms(200);//Halt the system for the given time in (ms)
					pass_counter=0;//counts the number of entered characters
					while(pass_counter<PASS_SIZE)
					{
						while (key_pressed == NOT_PRESSED)//repeat till the user press any key
						{
							key_pressed = KPD_u8GetPressedKey();//if the user pressed any button in keypad save the value in key_pressed
						}
						pass[pass_counter]=key_pressed;//add the pressed key to the password string
						LCD_vidDisplayCharacter(key_pressed);//print the entered character
						_delay_ms(CHARACTER_PREVIEW_TIME);//Halt the system for the given time in (ms)
						LCD_vidMoveCursor(2,12+pass_counter);//return the cursor to the location of the previous character to replace it with the password sign
						LCD_vidDisplayCharacter(PASSWORD_SYMBOL); // to display (Password sign *)
						_delay_ms(100);//Halt the system for the given time in (ms)
						pass_counter++;//increase the password counter that count the characters of the pass
						key_pressed = NOT_PRESSED;//return the variable that holds the pressed key from keypad to its initial value
					}
					EEPROM_vReadBlockFromAddress(EEPROM_GUEST_ADDRESS,stored_pass,PASS_SIZE);//Save the entire password in the EEPROM

					/*compare passwords*/
					if (ui8ComparePass(pass,stored_pass,PASS_SIZE)==TRUE)//in case of right password
					{
						login_mode = GUEST;
						pass_tries_count=0;//clear the counter of wrong tries
						LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
						LCD_vidDisplayString("Right pass");
						LCD_vidMoveCursor(2,1);
						LCD_vidDisplayString("Guest mode");
						_delay_ms(500);//Halt the system for the given time in (ms)
						LED_vidTurnOn(GUEST_LED_PORT,GUEST_LED_PIN);//turn the led of gust mode that is connected to the master micro controller on
					//	timer0_initializeCTC();//start the counter of the session
						LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
					}
					else//in case of wrong password
					{
						pass_tries_count++;//increase the number of wrong tries to block login if it exceeds the allowed tries
						login_mode = NO_MODE;//set the mode as not logged in
						LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
						LCD_vidDisplayString("Wrong pass");
						LCD_vidMoveCursor(2,1);
						LCD_vidDisplayString("Tries left:");
						LCD_vidDisplayCharacter(TRIES_ALLOWED-pass_tries_count+ASCII_ZERO);//print the number of tries left before block mode to be activated
						_delay_ms(1000);//Halt the system for the given time in (ms)
						LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
						if (pass_tries_count>=TRIES_ALLOWED)//if the condition of the block mode is true
						{
							EEPROM_vidWrite(LOGIN_BLOCKED_ADDRESS,TRUE);//write to the EEPROM TRUE to the the block mode address
							block_mode_flag = TRUE;//turn on block mode
							break;//breaks the loop of insert guest password #while(login_mode != GUEST)# line 228
						}//end of if that check if the number of tries exceeds the maximum tries allowed
					}//end of the case of wrong password
				}//end of loop of guest login
				break;//break for CHECK_GUEST_MODE switch case
			}//end of switch
			
		}
		
		/*************************************************************************************************/
		u8 show_menu = MAIN_MENU;
		
		
	//	while(timeout_flag!=TRUE)//Show the menu in case of the time is not out
		{
			key_pressed = NOT_PRESSED;//Set the key pressed by the user to its default value
			switch (show_menu)
			{
				case MAIN_MENU:
				do
				{
					/******************** print main Menu ******************/
					LCD_vidClearScreen();
					LCD_vidDisplayString("1:Room1 2:Room2");
					LCD_vidMoveCursor(2,1);
					if(login_mode==ADMIN)//check login mode
					{
						LCD_vidDisplayString("3:Room3 4:More ");//this menu options only printed if the logged in user is an admin
					}
					else if(login_mode==GUEST)//check login mode
					{
						LCD_vidDisplayString("3:Room3 4:Room4");//this menu options only printed if the logged in user is a guest
					}
					/*******************************************************/
					
					key_pressed = u8GetKeyPressed(login_mode);//wait for the user till key is pressed or the time is out
					_delay_ms(100);//to avoid the duplication of the pressed key
					
					if (key_pressed == SELECT_ROOM1)//If key pressed is 1
					{
						show_menu = ROOM1_MENU;//Set the next menu to be shown to room1 menu
					}
					else if (key_pressed == SELECT_ROOM2)//If key pressed is 2
					{
						show_menu = ROOM2_MENU;//Set the next menu to be shown to room2 menu
					}
					else if (key_pressed == SELECT_ROOM3)//If key pressed is 3
					{
						show_menu = ROOM3_MENU;//Set the next menu to be shown to room3 menu
					}
					else if (key_pressed == SELECT_ROOM4 && login_mode == GUEST)//If key pressed is 4 and the logged in user is guest
					{
						show_menu = ROOM4_MENU;//Set the next menu to be shown to room4 menu
					}
					else if (key_pressed == ADMIN_MORE_OPTION && login_mode == ADMIN)//If key pressed is 4 and the logged in user is admin
					{
						show_menu = MORE_MENU;//Set the next menu to be shown to more menu
					}
					else if(key_pressed != NOT_PRESSED)//show wrong input message if the user pressed wrong key
					{
						LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
						LCD_vidDisplayString("Wrong input");//print error message
						_delay_ms(500);//Halt the system for the given time in (ms)
					}
				} while ( ((key_pressed < '1') || (key_pressed > '4') ) /*&& (timeout_flag == FALSE) */);//break the loop in case of valid key or time is out
				
				break;//End of main menu case
				
				case MORE_MENU:
				do
				{
					/******************** print more Menu ******************/
					LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
					LCD_vidDisplayString("1:Room4    2:TV   ");
					LCD_vidMoveCursor(2,1);
					LCD_vidDisplayString("3:Air Cond.4:RET");
					/*******************************************************/
					key_pressed = u8GetKeyPressed(login_mode);//wait for the user till key is pressed or the time is out
					_delay_ms(100);//to avoid the duplication of the pressed key
					
					if (key_pressed == SELECT_ROOM4_ADMIN)//If key pressed is 1
					{
						show_menu = ROOM4_MENU;//Set the next menu to be shown to room4 menu
					}
					else if (key_pressed == SELECT_TV)//If key pressed is 2
					{
						show_menu = TV_MENU;//Set the next menu to be shown to TV menu
					}
					else if (key_pressed == SELECT_AIR_CONDITIONING)//If key pressed is 3
					{ 
						show_menu = AIRCONDITIONING_MENU;//Set the next menu to be shown to Air conditioning menu
					}
					else if (key_pressed == ADMIN_RET_OPTION)//If key pressed is 4 (RET)
					{
						show_menu = MAIN_MENU;//Set the next menu to be shown to main menu
					}
					else if(key_pressed != NOT_PRESSED)//show wrong input message if the user pressed wrong key
					{
						LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
						LCD_vidDisplayString("Wrong input");//print error message
						_delay_ms(500);//Halt the system for the given time in (ms)
					}
				} while (( (key_pressed < '1') || (key_pressed > '4') ) /*&& (timeout_flag == FALSE)*/);//break the loop in case of valid key or time is out
				
				break;//End of more menu case
				
				case AIRCONDITIONING_MENU:
				do
				{
					/******************** print more Menu ******************/
					LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
					LCD_vidDisplayString("1:Set temperature ");
					LCD_vidMoveCursor(2,1);
					LCD_vidDisplayString("2:Control  0:RET");
					/*******************************************************/
					key_pressed = u8GetKeyPressed(login_mode);//wait for the user till key is pressed or the time is out
					_delay_ms(100);//to avoid the duplication of the pressed key
					
					if (key_pressed == SELECT_SET_TEMPERATURE)//If key pressed is 1
					{
						show_menu = TEMPERATURE_MENU;//Set the next menu to be shown to set temperature menu
					}
					else if (key_pressed == SELECT_AIR_COND_CTRL)//If key pressed is 2
					{
						show_menu = AIRCOND_CTRL_MENU;//Set the next menu to be shown to air conditioning control menu
					}
					else if (key_pressed == SELECT_AIR_COND_RET)//If key pressed is 0
					{
						show_menu = MORE_MENU;//Set the next menu to be shown to more menu
					}
					else if(key_pressed != NOT_PRESSED)//show wrong input message if the user pressed wrong key
					{
						LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
						LCD_vidDisplayString("Wrong input");//print error message
						_delay_ms(500);//Halt the system for the given time in (ms)
					}
				} while (( (key_pressed < '0') || (key_pressed > '2') )/* && (timeout_flag == FALSE)*/);//break the loop in case of valid key or time is out
				break;//End of air conditioning menu case
				
				case ROOM1_MENU:
				vMenuOption(ROOM1_MENU,login_mode);//call the function that show the menu of room 1
				show_menu = MAIN_MENU;//Set the next menu to be shown to main menu
				break;//End of room1 menu case
				
				case ROOM2_MENU:
				vMenuOption(ROOM2_MENU,login_mode);//call the function that show the menu of room 2
				show_menu = MAIN_MENU;//Set the next menu to be shown to main menu
				break;//End of room2 menu case
				
				case ROOM3_MENU:
				vMenuOption(ROOM3_MENU,login_mode);//call the function that show the menu of room 3
				show_menu = MAIN_MENU;//Set the next menu to be shown to main menu
				break;//End of room3 menu case
				
				case ROOM4_MENU:
				vMenuOption(ROOM4_MENU,login_mode);//call the function that show the menu of room 4
				if (login_mode == GUEST)//in case of guest is logged in
				{
					show_menu = MAIN_MENU;//Set the next menu to be shown to main menu
				}
				else//in case of admin is logged in
				{
					show_menu = MORE_MENU;//Set the next menu to be shown to more menu
				}
				break;//End of room4 menu case
				
				case TV_MENU:
				vMenuOption(TV_MENU,login_mode);//call the function that show the menu of tv
				show_menu = MORE_MENU;//Set the next menu to be shown to more menu
				break;//End of TV menu case
				
				case AIRCOND_CTRL_MENU:
				vMenuOption(AIRCOND_CTRL_MENU,login_mode);//call the function that show the menu of Air conditioning control
				show_menu = AIRCONDITIONING_MENU;//Set the next menu to be shown to air conditioning menu
				break;//End of air conditioning control menu case
				
				case TEMPERATURE_MENU:
				temperature = 0;//clear the value of temperature
				while (temperature==0 /*&& timeout_flag == FALSE*/)//start the loop that asks for the temperature from the user in case of the time is not out
				{
					key_pressed = NOT_PRESSED;//set the key pressed to the default value
					LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
					LCD_vidDisplayString("Set temp.:__ ");//print the format of inserting temperature
					LCD_vidDisplayCharacter(DEGREES_SYMBOL); // print the symbol of degree
					LCD_vidDisplayCharacter('C'); // print the C character
					LCD_vidMoveCursor(1,11);//move the cursor to the place to write the entered temperature
					_delay_ms(200);//Halt the system for the given time in (ms)
					/*******************************************************************************/
					key_pressed = u8GetKeyPressed(login_mode);//wait for the user till key is pressed or the time is out
					_delay_ms(250);//to avoid the duplication of the pressed key

					if (timeout_flag == TRUE) //in case of the time is out before the user press a key
					{
						break;//break the loop that ask for temperature
					}
					if (key_pressed <'0' || key_pressed >'9')//show wrong input message if the user entered non numeric value
					{
						LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
						LCD_vidDisplayString("Wrong input");//print error message
						_delay_ms(500);//Halt the system for the given time in (ms)
						continue;//return to #while (temperature==0)# line 672
					}
					else//if the value is valid
					{
						LCD_vidDisplayCharacter(key_pressed);//print the value on the lcd
						temp_tens = key_pressed-ASCII_ZERO;//save the entered value
						key_pressed = NOT_PRESSED;//set the key pressed to the default value
					}
					/*******************************************************************************/
					key_pressed = u8GetKeyPressed(login_mode);//wait for the user till key is pressed or the time is out
					_delay_ms(250);//to avoid the duplication of the pressed key
					
				/*	if (timeout_flag == TRUE)//if the user session is timeout
					{
						break;//break the loop that ask for temperature
					}
				*/	if ((key_pressed <'0' || key_pressed >'9'))//show wrong input message if the user entered non numeric value
					{
						LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
						LCD_vidDisplayString("Wrong input");//print error message
						_delay_ms(500);//Halt the system for the given time in (ms)
						continue;//repeat the loop that ask for the temperature
					}
					else//if the value is valid
					{
						LCD_vidDisplayCharacter(key_pressed);//print the value on the lcd
						temp_ones = key_pressed-ASCII_ZERO;//save the entered value
						key_pressed = NOT_PRESSED;//set the key pressed to the default value
					}
					temperature = temp_tens*10 + temp_ones;//set the value of the temperature from the given separated values
					SPI_u8SendReceive(SET_TEMPERATURE);//Send the code of set temperature
					_delay_ms(200);//Halt the system to prevent write collision
					SPI_u8SendReceive(temperature);//send the entered temperature
					LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
					LCD_vidDisplayString("Temperature Sent");//show the message
					_delay_ms(500);//Halt the system for the given time in (ms)
				}
				show_menu = AIRCONDITIONING_MENU;//Set the next menu to be shown to air conditioning menu
				break;//break from switch
			}//End of the switch
		}//End of while that repeats the menu after each successful action till session timeout
	}// end of the main while(1)
}//end of main function

ISR(TIMER0_COMP_vect)
{
//	session_counter++;//increase the indicator of session time for every tick
}