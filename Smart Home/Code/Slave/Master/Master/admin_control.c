/*
 * admin_control.c
 *
 * Created: 2/4/2023 3:46:00 AM
 *  Author: Ahmed Nashaat
 */ 
		/************************************************************************/
		/*                     include library needed                           */
		/************************************************************************/
#include "BIT_MATH.h"
#include "Types.h"
#include "DIO_interface.h"
#include "Control_Rooms.h"
#include "smart_lock.h"
#include "Air_condition.h"
#include "admin_control.h"
#include "Control_Rooms.h"
#include "smart_lock.h"
#include "main.h"
		/************************************************************************/
		/*                  declare variables needed							*/
		/************************************************************************/
u8 u8choice;
		/************************************************************************/
		/*Function Name: Home_control_Menue										*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function display admin control menu		*/
		/************************************************************************/
void Home_control_Menue()
{
	LCD_vidClearScreen();						// clear LCD screen
	LCD_vidDisplayString("1.Open door");		// display message on LCD
	LCD_vidMoveCursor(2,1);						// move LCD cursor to line 2
	LCD_vidDisplayString("2.Room Lighting");	// display message on LCD
	LCD_vidMoveCursor(3,1);						// move LCD cursor to line 3
	LCD_vidDisplayString("3.Air condition");	// display message on LCD
	LCD_vidMoveCursor(4,1);						// move LCD cursor to line 4
	LCD_vidDisplayString("4.Return");			// display message on LCD
	u8choice = u8GetKeyPressed();				// take choice and store it u8choice variable
	switch(u8choice)							// switch for u8choice variable
	{
		case '1':								// if input is equal '1'
			Open_Door();						// call function that open the door
		break;									// break case
		case '2' :								// if input is equal '2'
			Print();							// call function that control rooms	
		break;									// break case
		case '3':								// if input is equal '3'
			Air_condition();					// call function that control air condition
		break;									// break case
		case '4':								// if input is equal '4'
			admin_menue();						// call function that return to home menu
		break;									// break case
		default:								// default case if not one of previous cases
			LCD_vidClearScreen();				// clear LCD screen
			LCD_vidDisplayString("Wrong Choice");// display message on LCD
			Home_control_Menue();				// call the same function to reenter choice
	}	
}

