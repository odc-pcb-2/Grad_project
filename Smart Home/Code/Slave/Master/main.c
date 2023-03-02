/*
 * ODC_EXTI_test.c
 *
 * Created: 12/7/2022 8:15:59 PM
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
#include "smart_lock.h"
#include "Control_Rooms.h"
#include "UART_interface.h"
#include "LM35_interface.h"
#include "SERVO_interface.h"
#include "Bluetooth_Control.h"
#include "util/delay.h"
#include "EXTI_interface.h"
#include "LED_interface.h"
#include "SWT_interface.h"
		/************************************************************************/
		/*                       variable declaration                           */
		/************************************************************************/
#define F_CPU 8000000UL						// define CPU frequency to 8 MHZ
u8 u8ReturnedValue;							// store value returned from user pr admin
		/************************************************************************/
		/*                       main function		                            */
		/************************************************************************/
int main(void)
{
	LM35_vidInit(ADC_CH0);					// initialize LM35 (temp sensor)
	USART_vidInit(9600);					// initialize UART protocol with baud rate 9600
	LCD_vidInit();							// initialize LCD
	KPD_vidInit(GPIOC, HIGH_NIBBLE);		// initialize KPD
	Servo_Init();							// initialize servo motor
	LED_vidInit(GPIOD, PIN3);				// initialize LED at port D pin 3
	LED_vidInit(GPIOD, PIN4);				// initialize LED at port D pin 4
	LED_vidInit(GPIOB, PIN5);				// initialize LED at port B pin 5
	LED_vidInit(GPIOD, PIN2);				// initialize LED at port D pin 2
	LED_vidInit(GPIOD, PIN6);				// initialize LED at port D pin 6
	DIO_vidSetPinMode(GPIOD, PIN7, OUTPUT);	// initialize pin 7 at port D as output
    SWT_vidInti(GPIOB, PIN2);				// initialize switch on port B pin2
    EXTI_vidInit(INT2_PIN, RISING_EDGE);	// initialize external interrupt in interrupt pin2 with low level
    EXTI_vidSetCB(INT2_PIN, &fun);			// external interrupt set callback function that call function that control start
	start_control();						// start the system
	while (1)								// while loop still work if system used
    {
	}
}
		/************************************************************************/
		/*Function Name: fun													*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void
								start system with control in emergency			*/
		/************************************************************************/
void fun(void)			
{
	USART_vidSendByte('I');					// send data to slave
	USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
	start();								// call function that start system using keypad
}
		/************************************************************************/
		/*Function Name: start_control											*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void
								start system with control						*/
		/************************************************************************/
void start_control()
{
	LCD_vidMoveCursor(1,1);					// move LCD cursor
	LCD_vidDisplayString(" Welcome to ODC ");// display message on screen
	LCD_vidMoveCursor(2,1);					// move LCD cursor
	LCD_vidDisplayString("   smart home ");	// display message on screen
	LCD_vidClearScreen();					// clear LCD screen
	LCD_vidMoveCursor(1,2);					// move cursor
	LCD_vidDisplayString(" connected ");	// display message on LCD
	LCD_vidMoveCursor(2,2);					// move cursor
	LCD_vidDisplayString(" remotely ");		// display message on LCD
	LCD_vidMoveCursor(3,2);					// move cursor
	LCD_vidDisplayString(" Successfully ");	// display message on LCD
	_delay_ms(150);							// wait for 150 m second
	devices();								// call function that display devices status 
	USART_vidSendByte('R');					// send data to slave
	USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
	USART_vidSendByte('I');					// send data to slave
	USART_vidEnableTXInterrupt();			// enable TX interrupt (finish transmit)
	
	Bluetooth_Control();					// control system remotely 
}
		/************************************************************************/
		/*Function Name: devices                                                */
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void and return void                                                
								display devices status on LCD screen			*/
		/************************************************************************/
void devices(void)
{
	LCD_vidClearScreen();					// clear LCD screen
	LCD_vidMoveCursor(1,1);					// move cursor
	LCD_vidDisplayString("R1: OFF");		// display string on LCD
	LCD_vidMoveCursor(1,8);					// move cursor
	LCD_vidDisplayString(" R2: OFF");		// display string on LCD
	LCD_vidMoveCursor(2,1);					// move cursor
	LCD_vidDisplayString("R3: OFF");		// display string on LCD
	LCD_vidMoveCursor(2,8);					// move cursor
	LCD_vidDisplayString(" R4: OFF");		// display string on LCD
	LCD_vidMoveCursor(3,1);					// move cursor
	LCD_vidDisplayString("R5: OFF");		// display string on LCD
	LCD_vidMoveCursor(3,8);					// move cursor
	LCD_vidDisplayString(" R6: OFF");		// display string on LCD
	LCD_vidMoveCursor(4,1);					// move cursor
	LCD_vidDisplayString("Air:OFF ");		// display string on LCD
	LCD_vidMoveCursor(4,9);					// move cursor
	LCD_vidDisplayString("AL: OFF");		// display string on LCD
}