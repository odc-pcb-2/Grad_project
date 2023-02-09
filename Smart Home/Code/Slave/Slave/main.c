/*
 * U2.c
 *
 * Created: 1/31/2023 9:54:49 PM
 * Author : Ahmed Emad
 */ 
		/************************************************************************/
		/*                        include library needed						*/
		/************************************************************************/
#include "Types.h"
#include "Bit_Math.h"
#include "DIO_interface.h"
#include "UART_interface.h"
#include "LED_interface.h"
#include "LCD_interface.h"
#include "SERVO_interface.h"
#include "main.h"
		/************************************************************************/
		/*                          define variable needed						*/
		/************************************************************************/
#define angle	90									// define angle for servo motor
u8 request;											// declare variable to store receive data
		/************************************************************************/
		/*                         Slave main function							*/
		/************************************************************************/
int main(void)
{	
	USART_vidInit(9600);							// initialize UART protocol with baud rate 9600
	LED_vidInit(GPIOC,PIN0);						// initialize led in port C & pin0
	LED_vidInit(GPIOC,PIN1);						// initialize led in port C & pin1
	LED_vidInit(GPIOC,PIN2);						// initialize led in port C & pin2
	LED_vidInit(GPIOC,PIN3);						// initialize led in port C & pin3
	LED_vidInit(GPIOC,PIN4);						// initialize led in port C & pin4
	DIO_vidSetPinMode(GPIOC, PIN5, OUTPUT);			// initialize pin5 in port C as output
	u8 response ;									// declare variable to store receive data
	u8 k;
	request = USART_u8RecieveByte();				// receive data
	USART_vidEnableRXInterrupt();					// enable RX interrupt (finish reserving)
	while(1)										// while loop for keeping system on while using it 
	{
		if (response == '1')
		{
			Remoted_Rooms();						// function to control rooms remotely
		}
		else 
		{
			request = USART_u8RecieveByte();		// receive data
			USART_vidEnableRXInterrupt();			// enable RX interrupt (finish reserving)
			switch (request)						// switch for receive data to know which action should take
			{
				case ROOM1_STATUS:					// if data receive is room1 
				if (DIO_enuReadPin(GPIOC,0)==0)		// check if pin is low (device off)  
				{
					response = OFF_STATUS;			// set response variable to off status
				}
				else if (DIO_enuReadPin(GPIOC,0)==1)// check if pin is High (device on)
				{
					response = ON_STATUS;			// set response variable to on status
				}
				else
				{
				}
				USART_vidSendByte(response);		// send updated data 
				USART_vidEnableTXInterrupt();		// enable TX interrupt
				break;								// break switch
				case ROOM2_STATUS:					// if data receive is room2
				if (DIO_enuReadPin(GPIOC,1)==0)		// check if pin is low (device off)  
				{
					response = OFF_STATUS;			// set response variable to off status
				}
				else if (DIO_enuReadPin(GPIOC,1)==1)// check if pin is High (device on)
				{
					response = ON_STATUS;			// set response variable to on status
				}
				else
				{
				}
				USART_vidSendByte(response);		// send updated data 
				USART_vidEnableTXInterrupt();		// enable TX interrupt
				break;								// break switch
				case ROOM3_STATUS:					// if data receive is room3
				if (DIO_enuReadPin(GPIOC,2)==0)		// check if pin is low (device off)
				{
					response = OFF_STATUS;			// set response variable to off status
				}
				else if (DIO_enuReadPin(GPIOC,2)==1)// check if pin is High (device on)
				{
					response = ON_STATUS;			// set response variable to on status
				}
				else
				{
				}
				USART_vidSendByte(response);		// send updated data
				USART_vidEnableTXInterrupt();		// enable TX interrupt
				break;								// break switch
			
				case ROOM4_STATUS:					// if data receive is room4
				if (DIO_enuReadPin(GPIOC,3)==0)		// check if pin is low (device off)
				{
					response = OFF_STATUS;			// set response variable to off status
				}
				else if (DIO_enuReadPin(GPIOC,3)==1)// check if pin is High (device on)
				{
					response = ON_STATUS;			// set response variable to on status
				}
				else
				{
				}
				USART_vidSendByte(response);		// send updated data
				USART_vidEnableTXInterrupt();		// enable TX interrupt
				break;								// break switch
				case ROOM5_STATUS:					// if data receive is room5
				if (DIO_enuReadPin(GPIOC,4)==0)		// check if pin is low (device off)
				{
					response = OFF_STATUS;			// set response variable to off status	
				}
				else if (DIO_enuReadPin(GPIOC,4)==1)// check if pin is High (device on)
				{
					response = ON_STATUS;			// set response variable to on status
				}
				else
				{
				}
				USART_vidSendByte(response);		// send updated data
				USART_vidEnableTXInterrupt();		// enable TX interrupt
				break;								// break switch
				case ROOM6_STATUS:					// if data receive is room6
				if (DIO_enuReadPin(GPIOC,5)==0)		// check if pin is low (device off)
				{
					response = OFF_STATUS;			// set response variable to off status
				}
				else if (DIO_enuReadPin(GPIOC,5)==1)// check if pin is High (device on)
				{
					response = ON_STATUS;			// set response variable to on status
				}
				else
				{
				}
				USART_vidSendByte(response);		// send updated data
				USART_vidEnableTXInterrupt();		// enable TX interrupt
				break;								// break switch
			
				case ROOM1_TURN_ON:					// case room1 turn on 
					LED_vidTurnOn(GPIOC,0);			// turn on led
				break;								// break switch
				case ROOM2_TURN_ON:					// case room2 turn on
					LED_vidTurnOn(GPIOC,1);			// turn on led
				break;								// break switch
				case ROOM3_TURN_ON:					// case room3 turn on
					LED_vidTurnOn(GPIOC,2);			// turn on led
				break;								// break switch
				case ROOM4_TURN_ON:					// case room4 turn on
					LED_vidTurnOn(GPIOC,3);			// turn on led
				break;								// break switch
				case ROOM5_TURN_ON:					// case room5 turn on
					LED_vidTurnOn(GPIOC,4);			// turn on led
				break;								// break switch
				case ROOM6_TURN_ON:					// case room6 turn on
					DIO_vidWritePin(GPIOC, PIN5, HIGH);// output high at pin
				break;								// break switch
			
				case ROOM1_TURN_OFF:				// case room2 turn off
					LED_vidTurnOff(GPIOC,0);		// turn off led
				break;								// break switch
				case ROOM2_TURN_OFF:				// case room1 turn off
					LED_vidTurnOff(GPIOC,1);		// turn off led
				break;								// break switch
				case ROOM3_TURN_OFF:				// case room3 turn off
					LED_vidTurnOff(GPIOC,2);		// turn off led
				break;								// break switch
				case ROOM4_TURN_OFF:				// case room4 turn off
					LED_vidTurnOff(GPIOC,3);		// turn off led
				break;								// break switch
				case ROOM5_TURN_OFF:				// case room5 turn off
					LED_vidTurnOff(GPIOC,4);		// turn off led
				break;								// break switch
				case ROOM6_TURN_OFF:				// case room6 turn off
					DIO_vidWritePin(GPIOC, PIN5, LOW);// output low at pin
				break;								// break switch
			}
		}
	}
}
		/************************************************************************/
		/* function name: Remoted_Rooms											*/
		/* input: void (no input)												*/
		/* description: function control rooms lighting using received command	*/
		/************************************************************************/
void Remoted_Rooms()
{
	while(1)									// while loop to still control rooms     
	{
		request = USART_u8RecieveByte();		// receive data from user
		USART_vidEnableRXInterrupt();			// enable TX interrupt
		switch(request)							// switch for data input
		{
			case 'Z' :							// if data equal Z
			LED_vidTurnOn(GPIOC,0);				// turn on led
			break;								// break switch
			case 'z' :							// if data equal z
			LED_vidTurnOff(GPIOC,0);			// turn off led
			break;								// break switch
			case 'B' :							// if data equal B
			LED_vidTurnOn(GPIOC,1);				// turn on led
			break;								// break switch
			case 'b' :							// if data equal b
			LED_vidTurnOff(GPIOC,1);			// turn off led
			break;								// break switch
			case 'F' :							// if data equal F
			LED_vidTurnOn(GPIOC,2);				// turn on led
			break;								// break switch
			case 'f' :							// if data equal f
			LED_vidTurnOff(GPIOC,2);			// turn off led
			break;								// break switch
			case 'D' :							// if data equal D
			LED_vidTurnOn(GPIOC,3);				// turn on led
			break;								// break switch
			case 'd' :							// if data equal d
			LED_vidTurnOff(GPIOC,3);			// turn off led
			break;								// break switch
			case 'E' :							// if data equal E
			LED_vidTurnOn(GPIOC,4);				// turn on led
			break;								// break switch
			case 'e' :							// if data equal e
			LED_vidTurnOff(GPIOC,4);			// turn off led
			break;								// break switch
			case 'G' :							// if data equal G
			LED_vidTurnOn(GPIOC,5);				// turn on led
			break;								// break switch
			case 'g' :							// if data equal g
			LED_vidTurnOff(GPIOC,5);			// turn off led
			break;								// break switch
		}
	}
}