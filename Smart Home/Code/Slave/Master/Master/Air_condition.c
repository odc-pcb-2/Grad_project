/*
 * Air_condition.c
 *
 * Created: 2/4/2023 4:13:31 AM
 *  Author: Ahmed Emad
 */ 
		/************************************************************************/
		/*                     include library needed                           */
		/************************************************************************/
#include "Types.h"
#include "DIO_interface.h"
#include "LCD_interface.h"
#include "KPD_interface.h"
#include "LM35_interface.h"
#include "ADC_interface.h"
#include "admin_control.h"
#include "Control_Rooms.h"
#include "Air_condition.h"
#include "DC_Motor_interface.h"
#include "main.h"
#include "util/delay.h"
		/************************************************************************/
		/*                  declare variables needed							*/
		/************************************************************************/
u8 u8ReturnedValue;
u8 u8choice;
		/************************************************************************/
		/*Function Name: Air_condition                                          */
		/*Function Input: no input (void)										*/
		/*Function Description: void function that control air condition		*/
		/************************************************************************/
void Air_condition()
{
	DC_Motor_vidInit();												// initialize the DC Motor
	u8 temp,pre=1;													// declaration for two variable and initialize them with 1
	LCD_vidInit();													// initialize the LCD 	
	LM35_vidInit(ADC_CH0);											// initialize the temperature sensor with channel 0 	
	LCD_vidClearScreen();											// clear LCD screen 			
	while (1)														// infinite while loop
	{
		u8ReturnedValue = KPD_u8GetPressedKey(GPIOC, HIGH_NIBBLE);	// take choice from user
		if(u8ReturnedValue==NOT_PRESSED)							// if user still not enter any thing
		{
			temp = LM35_u8Read();									// read temperature from sensor 			
			if(temp != pre)											// if temperature is not change (equal to previous one)
			{
				LCD_vidMoveCursor(1,1);								// move LCD cursor to line one
				LCD_vidDisplayString("Temperature =");				// display message on LCD
				LCD_vidDisplayNumber(temp);							// display temperature on LCD
				LCD_vidDisplayString("C");							// display message on LCD
				pre = temp;											// set previous value with recent read value
				if(temp >= 28)										// if temperature is grater than or equal to 28
				{
					LED_vidTurnOn(GPIOD,PIN2);						// turn on air condition LED
					LCD_vidMoveCursor(2,1);							// move cursor to second line
					LCD_vidDisplayString("FAN ON ");				// display message on LCD
					DC_Motor_vidTurnOn();							// turn on the DC Motor (air condition)
					LCD_vidMoveCursor(3,1);							// move cursor to third line
					LCD_vidDisplayString("Press 0 to Back");		// display message on LCD
				}
				else if(temp <= 21 )								// if temperature is less than or equal to 21
				{
					LED_vidTurnOff(GPIOD,PIN2);						// turn off air condition LED
					LCD_vidMoveCursor(2,1);							// move cursor to second line
					LCD_vidDisplayString("FAN OFF ");				// display message on LCD
					DC_Motor_vidTurnOff();							// turn off the DC Motor (air condition)
					LCD_vidMoveCursor(3,1);							// move cursor to third line
					LCD_vidDisplayString("Press 0 to Back");		// display message on LCD
				}
			}
		}
		else														// if not 
		{
			back();													// back to previous menu
		}
	}
}
		/************************************************************************/
		/*Function Name: back		                                            */
		/*Function Input: no input (void)										*/
		/*Function Description: void function that return user to previous menu */
		/************************************************************************/
 void back()
{
	  if(u8ReturnedValue=='0')				// if user choice equal to 0
	  {
		  if(Isadmin()==1)					// if returned value of Isadmin function equal to 1
		  {
			  Home_control_Menue();			// call function that return to admin home control
		  }
		  else								// if not 
		  {
			  User_menue();					// call function that return to user home control
		  }
	  }
	  else								    // if not 
	  {
		 LCD_vidClearScreen();				// clear LCD screen
		 LCD_vidDisplayString("Wrong Choice");// display message on LCD screen
		 _delay_ms(250);					// wait for 250 m second
		 Air_condition();					// call function to reenter valid choice
	  }
}

