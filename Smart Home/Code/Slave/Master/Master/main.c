/*
 * Master.c
 *
 * Created: 2/14/2023 6:29:23 AM
 * Author : Ahmed Emad
 */ 
		/************************************************************************/
		/*                       include library needed                         */
		/************************************************************************/
#include "main.h"
#include "Types.h"
#include "BIT_MATH.h"
#include "EEPROM_interface.h"
#include "LCD_interface.h"
#include "KPD_interface.h"
#include "LED_interface.h"
#include "smart_lock.h"
#include "Control_Rooms.h"
#include "UART_interface.h"
#include "LM35_interface.h"
#include "SERVO_interface.h"
#include "Bluetooth_Control.h"
#define F_CPU 8000000UL
#include "util/delay.h"
		/************************************************************************/
		/*                       variable declaration                           */
		/************************************************************************/
u32 u32ReturnedValue;						// variable to store block of received input
u8 wrong_pass_count;						// variable to count number of wrong pass input
u8 pass[4];							// array to store entered pass
u8 EEPROM_pass[4];						// array to store pass which saved in EEPROM
u8 user[4];							// array to store entered user
u8 EEPROM_user[4];						// array to store user which saved in EEPROM
u8 u8ReturnedValue;						// store value returned from user pr admin
u8 pass_check;							// store value (1 or 0) which returned from PASS_address_check
u8 check;							// used to check if pass is correct or not
u8 check_user;							// used to check if user is correct or not
		/************************************************************************/
		/*                       main function                                  */
		/************************************************************************/
void fun(void);
int main(void)
{
	LM35_vidInit(ADC_CH0);					// initialize LM35 (temp sensor)
	USART_vidInit(9600);					// initialize UART protocol with baud rate 9600
	LCD_vidInit();						// initialize LCD
	KPD_vidInit(GPIOC, HIGH_NIBBLE);			// initialize KPD 
	Servo_Init();						// initialize servo motor
	LED_vidInit(GPIOD, PIN3);				// initialize LED at port D pin 3       
	LED_vidInit(GPIOD, PIN4);				// initialize LED at port D pin 4
	LED_vidInit(GPIOB, PIN5);				// initialize LED at port B pin 5
	LED_vidInit(GPIOD, PIN2);				// initialize LED at port D pin 2	
	LED_vidInit(GPIOD, PIN6);				// initialize LED at port D pin 6
	DIO_vidSetPinMode(GPIOD, PIN7, OUTPUT);			// initialize pin 7 at port D as output
	start_control();					// start the system
	while (1)						// while loop still work if system used
    {
		u8 wrong_pass_count = smart_lock();		// if pass wrong and exceed tries safe go on lock mode
		if (wrong_pass_count == 0 || wrong_pass_count == 3)// if wrong_pass_count equal to (0 when pass correct) or (3 when user exceed number of tries)
		{
			break;					// stop the loop
		}	
	}
}
		/************************************************************************/
		/*Function Name: start_control						*/
		/*Function Input: no input (void)					*/
		/*Function Description: void function take void and return void
					start system with control			*/
		/************************************************************************/
void start_control()
{
	LCD_vidMoveCursor(1,1);					// move LCD cursor
	LCD_vidDisplayString(" Welcome to ODC ");		// display message on screen
	LCD_vidMoveCursor(2,1);					// move LCD cursor
	LCD_vidDisplayString("   smart home ");			// display message on screen
	LCD_vidMoveCursor(3,1);					// move LCD cursor
	LCD_vidDisplayString("1.login remotely");		// display message on screen
	LCD_vidMoveCursor(4,1);					// move LCD cursor
	LCD_vidDisplayString("2.login by KPD ");		// display message on screen
	u8ReturnedValue = u8GetKeyPressed();			// take choice from user
	_delay_ms(150);						// wait for 150 m second
	if (u8ReturnedValue=='1')				// if choice equal to 1
	{  
		USART_vidSendByte('1');				// send data to slave
		USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
		LCD_vidClearScreen();				// clear LCD screen
		LCD_vidMoveCursor(1,2);				// move cursor
		LCD_vidDisplayString(" connected ");		// display message on LCD
		LCD_vidMoveCursor(2,2);				// move cursor
		LCD_vidDisplayString(" remotely "); 		// display message on LCD
		LCD_vidMoveCursor(3,2);				// move cursor
		LCD_vidDisplayString(" Successfully ");		// display message on LCD
		_delay_ms(150);					// wait for 150 m second
		devices();					// call function that display devices status
		Bluetooth_Control();				// control system remotely 
	}
	else if(u8ReturnedValue=='2')				// if choice equal to 2
	{
		USART_vidSendByte('2');				// send data to slave
		USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
		LCD_vidClearScreen();				// clear LCD screen
		start();					// start the system
	}
	else							// if not
	{
		LCD_vidClearScreen();				// clear LCD screen
		LCD_vidDisplayString("Wrong input");		// display message on LCD
		_delay_ms(150);					// wait for 150 m second
		start_control();				// call function that control start to enter valid choice
	}
}
		/************************************************************************/
		/*Function Name: devices                                                */
		/*Function Input: no input (void)					*/
		/*Function Description: void function take void and return void                                                
					display devices status on LCD screen		*/
		/************************************************************************/
void devices(void)
{
	LCD_vidClearScreen();					// clear LCD screen
	LCD_vidMoveCursor(1,1);					// move cursor
	LCD_vidDisplayString("R1: OFF");			// display string on LCD
	LCD_vidMoveCursor(1,8);					// move cursor
	LCD_vidDisplayString(" R2: OFF");			// display string on LCD
	LCD_vidMoveCursor(2,1);					// move cursor
	LCD_vidDisplayString("R3: OFF");			// display string on LCD
	LCD_vidMoveCursor(2,8);					// move cursor
	LCD_vidDisplayString(" R4: OFF");			// display string on LCD
	LCD_vidMoveCursor(3,1);					// move cursor
	LCD_vidDisplayString("R5: OFF");			// display string on LCD
	LCD_vidMoveCursor(3,8);					// move cursor
	LCD_vidDisplayString(" R6: OFF");			// display string on LCD
	LCD_vidMoveCursor(4,1);					// move cursor
	LCD_vidDisplayString("Air:OFF ");			// display string on LCD
	LCD_vidMoveCursor(4,9);					// move cursor
	LCD_vidDisplayString("AL: OFF");			// display string on LCD
}
