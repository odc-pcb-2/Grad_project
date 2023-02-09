/*
 * Control_Rooms.c
 *
 * Created: 12/11/2022 7:07:51 PM
 * Author : Ahmed Emad
 */ 
		/************************************************************************/
		/*                     include library needed                           */
		/************************************************************************/
#include "Types.h"
#include "BIT_MATH.h"
#include "DIO_interface.h"
#include "LED_interface.h"
#include "LCD_interface.h"
#include "Control_Rooms.h"
#include "KPD_interface.h"
#include "util\delay.h"
#include "main.h"
		/************************************************************************/
		/*                  declare variables needed							*/
		/************************************************************************/
u8 u8ReturnedValue;							// store value returned from KPD
		/************************************************************************/
		/*Function Name: control_rooms											*/
		/*Function Input: take one input (constant variable form u8 type)		*/
		/*Function Description: void function that control rooms lightning		*/
		/************************************************************************/
void control_rooms(const u8 u8SelectedRoom)
{
	u8 u8Status = 0;						// declare variable that will store status and initialize it with 0
	u8 u8TurnOn  = 0;						// declare variable that will store turn on status and initialize it with 0
	u8 u8TurnOff = 0;						// declare variable that will store turn off status and initialize it with 0
	u8 response;
	do										// do while loop 
	{
		LCD_vidClearScreen();				// clear LCD screen
		switch(u8SelectedRoom)				// switch for selection room variable
		{
			case ROOM1_MENU:				// if selection is room1
			u8Status = ROOM1_STATUS;		// set status to room1 status
			u8TurnOn = ROOM1_TURN_ON;		// set turn on status to room1 turn on status
			u8TurnOff = ROOM1_TURN_OFF;		// set turn off status to room1 turn off status
			LCD_vidDisplayString("Room1 Status:");// display message on LCD screen
			break;							// break case
			case ROOM2_MENU:				// if selection is room2
			u8Status = ROOM2_STATUS;		// set status to room2 status
			u8TurnOn = ROOM2_TURN_ON;		// set turn on status to room2 turn on status
			u8TurnOff = ROOM2_TURN_OFF;		// set turn off status to room2 turn off status
			LCD_vidDisplayString("Room2 Status:");// display message on LCD screen
			break;							// break case
			case ROOM3_MENU:				// if selection is room3
			u8Status = ROOM3_STATUS;		// set status to room3 status
			u8TurnOn = ROOM3_TURN_ON;		// set turn on status to room3 turn on status
			u8TurnOff = ROOM3_TURN_OFF;		// set turn off status to room3 turn off status
			LCD_vidDisplayString("Room3 Status:");// display message on LCD screen
			break;							// break case
			case ROOM4_MENU:				// if selection is room4
			u8Status = ROOM4_STATUS;		// set status to room4 status
			u8TurnOn = ROOM4_TURN_ON;		// set turn on status to room4 turn on status
			u8TurnOff = ROOM4_TURN_OFF;		// set turn off status to room4 turn off status
			LCD_vidDisplayString("Room4 Status:");// display message on LCD screen
			break;							// break case
			case ROOM5_MENU:				// if selection is room5
			u8Status = ROOM5_STATUS;		// set status to room5 status
			u8TurnOn = ROOM5_TURN_ON;		// set turn on status to room5 turn on status
			u8TurnOff = ROOM5_TURN_OFF;		// set turn off status to room5 turn off status
			LCD_vidDisplayString("Room5 Status:");// display message on LCD screen
			break;							// break case
			case ROOM6_MENU:				// if selection is room6
			u8Status = ROOM6_STATUS;		// set status to room6 status
			u8TurnOn = ROOM6_TURN_ON;		// set turn on status to room6 turn on status
			u8TurnOff = ROOM6_TURN_OFF;		// set turn off status to room6 turn off status
			LCD_vidDisplayString("Room6 Status:");// display message on LCD screen
			break;							// break case
		}	
		USART_vidSendByte(u8Status);		// send status to slave
		USART_vidEnableTXInterrupt();		// enable TX interrupt (finish transmitting)
		_delay_ms(100);						// wait 100 m second
		response = USART_u8RecieveByte();	// receive response from slave
		USART_vidEnableRXInterrupt();		// enable RX interrupt (finish receiving)
		if(response == ON_STATUS)			// if response equal to on state
		{
			LCD_vidDisplayString("ON");		// display message on LCD screen
		}
		else								// if not 
		{
			LCD_vidDisplayString("OFF");	// display message on LCD screen
		}
		LCD_vidMoveCursor(2,1);				// move LCD cursor to second line
		LCD_vidDisplayString(">> 1-On ");	// display message on LCD screen
		LCD_vidMoveCursor(3,1);				// move LCD cursor to third line
		LCD_vidDisplayString(">> 2-Off ");	// display message on LCD screen
		LCD_vidMoveCursor(4,1);				// move LCD cursor to fourth line
		LCD_vidDisplayString(">> 0-RET ");	// display message on LCD screen
		u8ReturnedValue = u8GetKeyPressed();// take choice from user            
		_delay_ms(200);                     // wait for 200 m second
		if (u8ReturnedValue == '1')			// if choice equal to one
		{
			USART_vidSendByte(u8TurnOn);	// send turn on status to slave
			USART_vidEnableTXInterrupt();	// enable TX interrupt (finish transmitting)
		}
		else if (u8ReturnedValue == '2')	// if choice equal to two
		{
		  USART_vidSendByte(u8TurnOff);		// send turn off status to slave
		  USART_vidEnableTXInterrupt();		// enable TX interrupt (finish transmitting)
		}
		else								// if not 
		{
			LCD_vidClearScreen();			// clear LCD screen
			LCD_vidDisplayString("Wrong input");// display message on LCD screen
			_delay_ms(1000);				// wait for one second
		}
	}while ( (u8ReturnedValue < '0') || (u8ReturnedValue > '2') );// loop repeated if choice less than 0 or grater than 2
}
		/************************************************************************/
		/*Function Name: u8GetKeyPressed										*/
		/*Function Input: no input (void)										*/
		/*Function Description: function from type u8 that return keypad pressed key value*/
		/************************************************************************/
u8 u8GetKeyPressed()
{
	do										// do while loop                         	
	{
		_delay_ms(350);						// wait for 350 m second
		u8ReturnedValue = KPD_u8GetPressedKey(GPIOC, HIGH_NIBBLE);// return keypad pressed value	
		_delay_ms(150);						//	wait for 150 m second                   	
	} while (u8ReturnedValue == NOT_PRESSED);// loop repeated if not pressed any key
	return u8ReturnedValue;					// return pressed value
}
