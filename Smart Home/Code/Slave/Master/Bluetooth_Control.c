		/************************************************************************/
		/*                     include library needed                           */
		/************************************************************************/
#include "Types.h"
#include "BIT_MATH.h"
#include "DIO_interface.h"
#include "UART_interface.h"
#include "LCD_interface.h"
#include "LED_interface.h"
#include "EEPROM_interface.h"
#include "DC_Motor_interface.h"
#include "LM35_interface.h"
#include "SERVO_interface.h"
#include "Bluetooth_Control.h"
#include "smart_lock.h"
		/************************************************************************/
		/*                  declare variables needed							*/
		/************************************************************************/
#define PASS_address_check 0x50							// address check if pass is already set before or not store only (0 or 1)
#define angle 90										// define angle for servo 90
u8 LogInFlag;											// declare variable that store login flag (who login admin or user)
u8 u8ReturnedValue;										// declare variable that store returned value from KPD
u32 u32ReturnedValue;									// declare variable that store received block data
u8 wrong_pass_count;									// declare variable that store wrong pass counter
u8 pass[4];												// array to store entered pass
u8 EEPROM_pass[4];										// array to store pass which saved in EEPROM
u8 user[4];												// array to store entered username
u8 EEPROM_user[4];										// array to store username which saved in EEPROM
u8 u8ReturnedValue;										// store value returned from KPD
u8 pass_check;											// store value (1 or 0) which returned from PASS_address_check
u8 check;												// used to check if pass is correct or not
u8 check_user;											// used to check if username is correct or not
// u8 UserId;												// declare variable to store user id
		/************************************************************************/
		/*Function Name: Bluetooth_Control										*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void
								control system via bluetooth (remoted)			*/
		/************************************************************************/
void Bluetooth_Control()
{
	pass_check = EEPROM_u8Read(PASS_address_check);		// check if pass set or not by reading value in PASS_address_check
	if (pass_check != 0)
	{
		USART_vidSendString("--- Please, Set Admin ---");// send message to serial terminal
		USART_vidEnableTXInterrupt();					// enable TX interrupt (finish transmit)
		USART_vidSendString("Set UserName: ");			// send message to serial terminal
		USART_vidEnableTXInterrupt();					// enable TX interrupt (finish transmit)
		for (int i=0;i<4;i++)
		{
			u8RecieveByte();							// receive data
			user[i]=u8ReturnedValue;					// store data in array
		}
		USART_vidSendString("Set Password: ");			// send message to serial terminal
		USART_vidEnableTXInterrupt();					// enable TX interrupt (finish transmit)
		for (int i=0;i<4;i++)
		{
			u8RecieveByte();							// receive data
			pass[i]=u8ReturnedValue;					// store data in array
		}
		write_EEPROM(0x0);								// call function that write username and pass in EEPROM		
		USART_vidSendString("Admin Set Successfully ");	// send message to serial terminal
		USART_vidEnableTXInterrupt();					// enable TX interrupt (finish transmit)
		EEPROM_vidWrite(PASS_address_check, 0);			// change value in PASS_address_check to zero (as pass is set)
		pass_check = EEPROM_u8Read(PASS_address_check);	// check if pass is set or not by reading value in PASS_address_check
		Bluetooth_Control();							// call function that control system via bluetooth 
	}
	else												// if not
	{
		USART_vidSendString("Press A for Admin Login & U for user login");// send message to serial terminal
		USART_vidEnableTXInterrupt();					// enable TX interrupt (finish transmit)
		u8ReturnedValue = USART_u8RecieveByte();		// receive data and store it in variable
		USART_vidEnableRXInterrupt();					// enable TX interrupt (finish receiving)
		if (u8ReturnedValue=='A')						// if choice equal to A
		{
			LogInFlag=1;								// set login flag to 1
			Isadmin_Remoted();							// call function that check who is login (admin or user)
			Admin_Remoted_Login();						// call function that scan username & pass from admin
		}
		else if (u8ReturnedValue=='U')					// if choice equal to U
		{
			LogInFlag=0;								// set login flag to 0
			Isadmin_Remoted();							// call function that check who is login (admin or user)
			User_Remoted_Login();						// call function that scan username & pass from user
		}
		else											// if not 
		{
			USART_vidSendString("Wrong Choice");		// send message to serial terminal
			USART_vidEnableTXInterrupt();				// enable TX interrupt (finish transmit)
			Bluetooth_Control();						// call function that control system remoted
		}
	}
}
		/************************************************************************/
		/*Function Name: User_Remoted_Login										*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void
								scan username & pass from user then check it (remoted)*/
		/************************************************************************/
void User_Remoted_Login()
{
	u8 UserId;											// declare user id variable
	USART_vidSendString("Enter User Id ");				// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	u8RecieveByte();									// receive data
	UserId=u8ReturnedValue;								// set user id to entered id
	USART_vidSendString("Enter UserName ");				// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	for (int i=0;i<4;i++)
	{
		u8RecieveByte();								// receive data
		user[i]=u8ReturnedValue;						// store data in array
	}
	USART_vidSendString("Enter Password ");				// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	for (int i=0;i<4;i++)
	{
		u8RecieveByte();								// receive data
		pass[i]=u8ReturnedValue;						// store data in array
	}
	read_EEPROM(UserId);								// read user from EEPROM
	check_user=0;										// initialize check variable to 0
	for (u8 i=0;i<4;i++)								// loop to check if pass entered is correct or not (same saved in EEPROM)
	{
		if (user[i] == EEPROM_user[i])					// check if two value are equal or not
		{
			check_user++;								// if that increase the check variable by one
		}
		else											// if not
		{
		}
	}
	check=0;											// initialize check variable to 0
	for (u8 i=0;i<4;i++)								// loop to check if pass entered is correct or not (same saved in EEPROM)
	{
		if (pass[i] == EEPROM_pass[i])					// check if two value are equal or not
		{
			check++;									// if that increase the check variable by one
		}
		else											// if not
		{
		}
	}
	if(check == 4 && check_user == 4)					// check if check variable equal 4 or not (correct pass)
	{
		USART_vidSendString("Correct Password ");		// send message to serial terminal
		USART_vidEnableTXInterrupt();					// enable TX interrupt (finish transmit)
		LED_vidTurnOn(GPIOD,PIN4);						// turn on the led
		LED_vidTurnOff(GPIOD,PIN3);						// turn off the led
		LED_vidTurnOff(GPIOB,PIN5);						// turn off the led
		wrong_pass_count = 0;							// set wrong pass counter to 0
		User_Remoted_Menue();							// reset wrong_pass_count variable by reinitialize by 0
	}
	else												// if not
	{ 
		wrong_pass_count++;								// increase wrong_pass_count variable by one
		USART_vidSendString("Password Wrong ");			// send message to serial terminal
		USART_vidEnableTXInterrupt();					// enable TX interrupt (finish transmit)
		if (wrong_pass_count == 1)						// if wrong pass counter equal to 1
		USART_vidSendString(" 2 ");						// send message to serial terminal
		else if (wrong_pass_count == 2)					// if wrong pass counter equal to 2
		USART_vidSendString(" 1 ");						// send message to serial terminal
		else if (wrong_pass_count == 3)					// if wrong pass counter equal to 3
		USART_vidSendString(" 0 ");						// send message to serial terminal
		USART_vidSendString(" tries remains ");			// send message to serial terminal
		USART_vidEnableTXInterrupt();					// enable TX interrupt (finish transmit)
		if (wrong_pass_count == 3)						// if wrong pass counter equal to 3
		{
			wrong_pass_count=0;							// set wrong pass counter to 0
			USART_vidSendString("Lock mode!! ");		// send message to serial terminal
			USART_vidEnableTXInterrupt();				// enable TX interrupt (finish transmit)
			USART_vidSendString("try again after 20 second ");// send message to serial terminal
			USART_vidEnableTXInterrupt();				// enable TX interrupt (finish transmit)

			LED_vidTurnOn(GPIOB, PIN5);					// turn on the led
			LED_vidTurnOff(GPIOD, PIN3);				// turn off led
			LED_vidTurnOff(GPIOD, PIN4);				// turn off led
			DIO_vidWritePin(GPIOD, PIN7, HIGH);			// output high on pin (turn on alarm)
			LCD_vidMoveCursor(4,10);
			LCD_vidDisplayString("AL: ON ");			// display string on LCD
			_delay_ms(20000);							// wait for 20 second
			DIO_vidWritePin(GPIOD, PIN7, LOW);			// output low on pin (turn off alarm)
			LCD_vidMoveCursor(4,10);
			LCD_vidDisplayString("AL: OFF");			// display string on LCD
		}
		User_Remoted_Login();							// call function that control user login
	}
}
		/************************************************************************/
		/*Function Name: Admin_Remoted_Login									*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void
								scan username & pass from admin then check it (remoted)*/
		/************************************************************************/
void Admin_Remoted_Login()
{
	USART_vidSendString("Enter UserName: ");			// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	for (int i=0;i<4;i++)
	{
		u8RecieveByte();								// receive data
		user[i]=u8ReturnedValue;						// store data in array
	}
	USART_vidSendString("Enter Password ");				// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)	
	for (int i=0;i<4;i++)
	{
		u8RecieveByte();								// receive data
		pass[i]=u8ReturnedValue;						// store data in array
	}
	check_admin_pass_remoted();							// call function that check username & pass for admin
}
		/************************************************************************/
		/*Function Name: check_admin_pass_remoted								*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void
								check username & pass for admin (remoted)		*/
		/************************************************************************/
void check_admin_pass_remoted()
{
	read_EEPROM(0x0);									// read EEPROM
	check_user=0;										// initialize check variable to 0
	for (u8 i=0;i<4;i++)								// loop to check if pass entered is correct or not (same saved in EEPROM)
	{
		if (user[i] == EEPROM_user[i])					// check if two value are equal or not
		{
			check_user++;								// if that increase the check variable by one
		}
		else											// if not
		{
		}
	}
	check=0;											// initialize check variable to 0
	for (u8 i=0;i<4;i++)								// loop to check if pass entered is correct or not (same saved in EEPROM)
	{
		if (pass[i] == EEPROM_pass[i])					// check if two value are equal or not
		{
			check++;									// if that increase the check variable by one
		}
		else											// if not
		{
		}
	}
	if(check == 4 && check_user == 4)					// check if check variable equal 4 or not (correct pass)
	{
		USART_vidSendString("Correct Password ");// send message to serial terminal
		USART_vidEnableTXInterrupt();					// enable TX interrupt (finish transmit)
		LED_vidTurnOn(GPIOD,PIN3);						// turn on led
		LED_vidTurnOff(GPIOD,PIN4);						// turn off led
		LED_vidTurnOff(GPIOB,PIN5);						// turn off led
		wrong_pass_count = 0;							// reset wrong_pass_count variable by reinitialize by 0	
		Admin_Remoted_Menue();							// call function that control admin menu
	}
	else												// if not
	{
		wrong_pass_count++;								// increase wrong_pass_count variable by one
		USART_vidSendString("Password Wrong ");			// send message to serial terminal
		USART_vidEnableTXInterrupt();					// enable TX interrupt (finish transmit)
		if (wrong_pass_count == 1)						// if wrong pass counter equal to 1
		USART_vidSendString(" 2 ");						// send message to serial terminal
		else if (wrong_pass_count == 2)					// if wrong pass counter equal to 2
		USART_vidSendString(" 1 ");						// send message to serial terminal
		else if (wrong_pass_count == 3)					// if wrong pass counter equal to 3
		USART_vidSendString(" 0 ");						// send message to serial terminal
		USART_vidSendString("tries remains ");// send message to serial terminal
		USART_vidEnableTXInterrupt();					// enable TX interrupt (finish transmit)
		if (wrong_pass_count == 3)						// if wrong pass counter equal to 3
		{
			wrong_pass_count=0;							// set wrong pass counter to 0
			USART_vidSendString("Lock mode!! ");		// send message to serial terminal
			USART_vidEnableTXInterrupt();				// enable TX interrupt (finish transmit)
			USART_vidSendString("try again after 20 second ");// send message to serial terminal
			USART_vidEnableTXInterrupt();				// enable TX interrupt (finish transmit)
			
			LED_vidTurnOn(GPIOB, PIN5);					// turn on led
			LED_vidTurnOff(GPIOD, PIN3);				// turn off led
			LED_vidTurnOff(GPIOD, PIN4);				// turn of led
			DIO_vidWritePin(GPIOD, PIN7, HIGH);			// output high on pin (turn on alarm)
			LCD_vidMoveCursor(4,9);
			LCD_vidDisplayString("AL: ON ");			// display string on LCD
			_delay_ms(20000);							// wait for 20 second
			DIO_vidWritePin(GPIOD, PIN7, LOW);			// output low on pin (turn off alarm)
			LCD_vidMoveCursor(4,9);
			LCD_vidDisplayString("AL: OFF");			// display string on LCD
			Bluetooth_Control();
		}
		Admin_Remoted_Login();							// call function that control admin login
	}
}
		/************************************************************************/
		/*Function Name: Admin_Remoted_Menue	                                */
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void
								display admin control menu (remoted)			*/
		/************************************************************************/
void Admin_Remoted_Menue()
{
	USART_vidSendString("1.Control Home       ");		// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	USART_vidSendString("2.Add user           ");		// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	USART_vidSendString("3.Remove user        ");		// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	USART_vidSendString("4.Log out            ");		// send message to serial terminal
	u8ReturnedValue=USART_u8RecieveByte();				// receive data
	USART_vidEnableRXInterrupt();						// enable RX interrupt (finish receiving)
	u8ReturnedValue=USART_u8RecieveByte();				// receive data
	USART_vidEnableRXInterrupt();						// enable RX interrupt (finish receiving)		
	u8ReturnedValue=USART_u8RecieveByte();				// receive data
	USART_vidEnableRXInterrupt();						// enable RX interrupt (finish receiving)
	switch(u8ReturnedValue)								// switch choice
	{
		case '1':										// if choice equal 1
			control_home_remoted();						// call function that control admin menu
			break;										// break
		case '2':										// if choice equal 2
			Add_Remoted_User();							// call function that control user menu
			break;										// break
		case '3':										// if choice equal 3
			Remove_Remoted_User();						// call function that control user menu
			break;										// break
		case '4':										// if choice equal 4
			LogOut_Remotly();							// call function that logout from system
			break;										// break
	}
}
		/************************************************************************/
		/*Function Name: control_home_remoted									*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void
								admin control (remoted)							*/
		/************************************************************************/
void control_home_remoted()
{
	USART_vidSendString("1.Opening Door  ");			// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	USART_vidSendString("2.Room Lighting ");			// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	USART_vidSendString("3.Air condition ");			// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	USART_vidSendString("4.Return        ");			// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	u8ReturnedValue=USART_u8RecieveByte();				// receive data
	USART_vidEnableRXInterrupt();						// enable RX interrupt (finish receiving)
	u8ReturnedValue=USART_u8RecieveByte();				// receive data
	USART_vidEnableRXInterrupt();						// enable RX interrupt (finish receiving)
	u8ReturnedValue=USART_u8RecieveByte();				// receive data
	USART_vidEnableRXInterrupt();						// enable RX interrupt (finish receiving)
	switch(u8ReturnedValue)								// switch choice
	{
		case '1':										// if choice equal 1
			open_door();								// call function that control door 
			break;										// break
		case '2':										// if choice equal 2
			control_Remoted_Rooms();					// call function that control rooms 
			break;										// break
		case '3':										// if choice equal 1
			Air_Condition_Remoted();					// call function that control air condition
			break;										// break
		case '4':										// if choice equal 1
			Admin_Remoted_Menue();						// call function that display admin control menu
			break;										// break
		default:										// default
			control_home_remoted();						// call function that return admin menu
	}
}
		/************************************************************************/
		/*Function Name: u8RecieveByte											*/
		/*Function Input: no input (void)										*/
		/*Function Description: function from type u8 take void and return u8
								receive byte from user							*/
		/************************************************************************/
u8 u8RecieveByte()
{
	u8ReturnedValue = USART_u8RecieveByte();			// receive data
	USART_vidEnableRXInterrupt();						// enable RX interrupt (finish receiving)
	u8ReturnedValue = USART_u8RecieveByte();			// receive data
	USART_vidEnableRXInterrupt();						// enable RX interrupt (finish receiving)
	u8ReturnedValue = USART_u8RecieveByte();			// receive data
	USART_vidEnableRXInterrupt();						// enable RX interrupt (finish receiving)
	return u8ReturnedValue;								// return data receive
}
		/************************************************************************/
		/*Function Name: control_Remoted_Rooms									*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void
								control rooms lightning (remoted)				*/
		/************************************************************************/
void control_Remoted_Rooms()
{
	USART_vidSendString("R1: Z & z   R2: B & b   R3: F & f   R4: D & d   R5: E & e R6: G & g   0.To Return");// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	while(u8ReturnedValue!='0')							// while loop work if choice not equal 0
	{
		u8ReturnedValue = USART_u8RecieveByte();		// receive data
		USART_vidEnableRXInterrupt();					// enable RX interrupt (finish receiving)
		switch(u8ReturnedValue)							// switch choice
		{
			case 'Z' :									// if choice equal to Z
				LCD_vidMoveCursor(1,1);					// move LCD cursor
				LCD_vidDisplayString("R1: ON ");		// display string on LCD
				USART_vidSendByte('Z');					// send message to serial terminal
				USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
				break;									// break
			case 'z' :									// if choice equal to z
				LCD_vidMoveCursor(1,1);					// move LCD cursor
				LCD_vidDisplayString("R1: OFF");		// display string on LCD
				USART_vidSendByte('z');					// send message to serial terminal
				USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
				break;									// break
			case 'B' :									// if choice equal to B
				USART_vidSendByte('B');					// send message to serial terminal
				USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
				LCD_vidMoveCursor(1,8);					// move LCD cursor
				LCD_vidDisplayString(" R2: ON ");		// display string on LCD
				break;									// break
			case 'b' :									// if choice equal to b
				USART_vidSendByte('b');					// send message to serial terminal
				USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
				LCD_vidMoveCursor(1,8);					// move LCD cursor
				LCD_vidDisplayString(" R2: OFF");		// display string on LCD
				break;									// break 
			case 'F' :									// if choice equal to F
				USART_vidSendByte('F');					// send message to serial terminal
				USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
				LCD_vidMoveCursor(2,1);					// move LCD cursor
				LCD_vidDisplayString("R3: ON ");		// display string on LCD
				break;									// break
			case 'f' :									// if choice equal to f
				USART_vidSendByte('f');					// send message to serial terminal
				USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
				LCD_vidMoveCursor(2,1);					// move LCD cursor
				LCD_vidDisplayString("R3: OFF");		// display string on LCD
				break;									// break
			case 'D' :									// if choice equal to D
				USART_vidSendByte('D');					// send message to serial terminal
				USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
				LCD_vidMoveCursor(2,8);					// move LCD cursor
				LCD_vidDisplayString(" R4: ON ");		// display string on LCD		
				break;									// break
			case 'd' :									// if choice equal to d
				USART_vidSendByte('d');					// send message to serial terminal
				USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
				LCD_vidMoveCursor(2,8);					// move LCD cursor
				LCD_vidDisplayString(" R4: OFF");		// display string on LCD
				break;									// break
			case 'E' :									// if choice equal to E
				USART_vidSendByte('E');					// send message to serial terminal
				USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
				LCD_vidMoveCursor(3,1);					// move LCD cursor
				LCD_vidDisplayString("R5: ON ");		// display string on LCD
				break;									// break
			case 'e' :									// if choice equal to e
				USART_vidSendByte('e');					// send message to serial terminal
				USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
				LCD_vidMoveCursor(3,1);					// move LCD cursor
				LCD_vidDisplayString("R5: OFF");		// display string on LCD
				break;									// break
			case 'G' :									// if choice equal to G
				USART_vidSendByte('G');					// send message to serial terminal
				USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
				LCD_vidMoveCursor(3,8);					// move LCD cursor
				LCD_vidDisplayString(" R6: ON ");		// display string on LCD
				break;									// break
			case 'g' :									// if choice equal to g
				USART_vidSendByte('g');					// send message to serial terminal
				USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
				LCD_vidMoveCursor(3,8);					// move LCD cursor
				LCD_vidDisplayString(" R6: OFF");		// display string on LCD
				break;									// break
		}
	}
	if (Isadmin_Remoted()==1)							// if is admin equal to 1
	{
		Admin_Remoted_Menue();							// call function that control admin login
	}
	else if(Isadmin_Remoted()==0)						// if not
	{
		User_Remoted_Menue();							// call function that control user login
	}
}
		/************************************************************************/
		/*Function Name: Air_Condition_Remoted									*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void
								control air condition	(remoted)				*/
		/************************************************************************/
void Air_Condition_Remoted()
{
	DC_Motor_vidInit();									// initialize DC Motor
	u8 temp,pre=1;										// declare two variable & initialize both with 1
	LM35_vidInit(ADC_CH0);								// initialize temperature sensor
	while (1)											// infinite while loop
	{
		temp = LM35_u8Read();							// read temperature
		if(temp != pre)									// if temperature not equal to previous one
		{
			pre = temp;									// set previous temperature to recent one
			if(temp >= 28)								// if temperature greater than or equal to 28
			{
				USART_vidSendString("Air condition ON                                                  ");// send message to serial terminal
				USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
				LCD_vidMoveCursor(4,1);					// move LCD cursor
				LCD_vidDisplayString("Air:ON ");		// display string on LCD
				LED_vidTurnOn(GPIOD, PIN2);				// turn on led
				DC_Motor_vidTurnOn();					// turn on DC Motor
			}
			else if(temp <= 21 )
			{
				USART_vidSendString("Air condition off ");// send message to serial terminal
				USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
				LCD_vidMoveCursor(4,1);					// move LCD cursor
				LCD_vidDisplayString("Air:OFF ");		// display string on LCD
				LED_vidTurnOff(GPIOD, PIN2);			// turn off led
				DC_Motor_vidTurnOff();					// turn off DC Motor
			}
		if (Isadmin_Remoted()==1)						// if is admin equal to 1
		{
			Admin_Remoted_Menue();						// call function that control admin login
		}
		else if(Isadmin_Remoted()==0)					// if not
		{
			User_Remoted_Menue();						// call function that control user login
		}
		}
	}
}
		/************************************************************************/
		/*Function Name: Add_Remoted_User                                       */
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void
								add user to the system	(remoted)				*/
		/************************************************************************/
void Add_Remoted_User()
{	
	u8 UserId;											// declare variable to store user id
	USART_vidSendString("Enter User Id ");				// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	u8RecieveByte();									// receive data
	UserId=u8ReturnedValue;								// store data in variable
	USART_vidSendString("Enter UserName ");				// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	for (int i=0;i<4;i++)
	{
		u8RecieveByte();								// receive data
		user[i]=u8ReturnedValue;						// store data in array
	}
	USART_vidSendString("Enter Password");				// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	for (int i=0;i<4;i++)
	{
		u8RecieveByte();								// receive data
		pass[i]=u8ReturnedValue;						// store data in array
	}
	write_EEPROM(UserId);								// write data in EEPROM
	read_EEPROM(UserId);								// read data from EEPROM
	USART_vidSendString("User Added Successfully ");	// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	Admin_Remoted_Menue();								// call function that display admin control menu
}
		/************************************************************************/
		/*Function Name: Remove_Remoted_User									*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void
								remove user to the system	(remoted)			*/
		/************************************************************************/
void Remove_Remoted_User()
{
	u8 UserId;											// declare variable to store user id
	USART_vidSendString("Enter User Id ");				// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	UserId = u8RecieveByte();							// receive data
	for (int i=0;i<4;i++)
	{
		user[i]='F';									// store data in array
	}
	for (int i=0;i<4;i++)
	{
		pass[i]='F';									// store data in array
	}
	write_EEPROM(UserId);								// write data in EEPROM
	read_EEPROM(UserId);								// read data from EEPROM
	USART_vidSendString("User Remove Successfully ");	// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	Admin_Remoted_Menue();								// call function that display admin control menu
}
		/************************************************************************/
		/*Function Name: User_Remoted_Menue										*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void
								display user control menu (remoted)				*/
		/************************************************************************/
void User_Remoted_Menue()
{
	USART_vidSendString("1.Room Lighting   2.Air condition   3.LogOut");// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	u8RecieveByte();									// receive data
	USART_vidEnableRXInterrupt();						// enable RX interrupt (finish receiving)
	switch(u8ReturnedValue)								// switch choice
	{
		case '1' :										// if choice equal 1
			control_Remoted_Rooms();					// call function that control rooms
			break;										// break
		case '2' :										// if choice equal 2
			Air_Condition_Remoted();					// call function that control air condition
			break;										// break
		case '3' :										// if choice equal 3
			LogOut_Remotly();							// call function that logout from system
			break;										// break
		default:										// default
			User_Remoted_Menue();						// call function that display user control menu
			break;										// break
	}
}
		/************************************************************************/
		/*Function Name: open_door												*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void
								control door opening & closing (remoted)		*/
		/************************************************************************/
void open_door()
{	
	USART_vidSendString("1. open The door    2.Close the door    0.Return");// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)
	u8RecieveByte();									// receive data
	if (u8ReturnedValue == '1')							// if choice equal 1
	{
		Servo_Clockwise(angle);							// turn on servo clock wise
		USART_vidSendString("The door is open                                                               ");// send message to serial terminal
		USART_vidEnableRXInterrupt();					// enable RX interrupt (finish receiving)
	}
	else if(u8ReturnedValue == '2')						// if choice equal 2
	{
		Servo_Counterclockwise(angle);					// turn on servo anti clock wise
		USART_vidSendString("The door is closed                                                            ");// send message to serial terminal
		USART_vidEnableRXInterrupt();					// enable RX interrupt (finish receiving)
	}
	else if (u8ReturnedValue == '0')					// if choice equal 0
	{
		Admin_Remoted_Menue();							// call function that display admin control menu
	}
	else												// if not
	{
		open_door();									// call function that control door
	}
	Admin_Remoted_Menue();								// call function that display admin control menu
}
		/************************************************************************/
		/*Function Name: LogOut_Remotly											*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void
								logout from the system and return to start screen (remoted)*/
		/************************************************************************/
void LogOut_Remotly()
{
	USART_vidSendString("press A for Admin Login & U for User login");// send message to serial terminal
	USART_vidEnableTXInterrupt();						// enable TX interrupt (finish transmit)					
	u8RecieveByte();									// receive data
	USART_vidEnableRXInterrupt();						// enable RX interrupt (finish receiving)
	if (u8ReturnedValue=='A')							// if choice equal A
	{
		LogInFlag=1;									// set login flag equal to 1
		Isadmin_Remoted();								// call function that know who is login
		Admin_Remoted_Login();							// call function that control admin login
	}
	else if (u8ReturnedValue=='U')						// if choice equal to U
	{
		LogInFlag=0;									// set login flag equal to 0
		Isadmin_Remoted();								// call function that know who is login
		User_Remoted_Login();							// call function that control user login
	}
	else												// if not
	{	
		LogOut_Remotly();								// call function that logout from system
	}
}
		/************************************************************************/
		/*Function Name: Isadmin_Remoted                                        */
		/*Function Input: no input (void)										*/
		/*Function Description: function from type u8 take void and return u8
					check who login user or admin & return only 0 or 1 (remoted)*/
		/************************************************************************/
u8 Isadmin_Remoted()
{	
	if (LogInFlag==1)									// if login flag equal to 1
	{
		return 1;										// return 1
	}
	else												// if not
	{
		return 0;										// return 0
	}
}



