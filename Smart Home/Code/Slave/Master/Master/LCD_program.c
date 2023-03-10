		/************************************************************************/
		/*                         include library needed						*/
		/************************************************************************/
#include "Types.h"
#include "BIT_MATH.h"
#include "DIO_interface.h"
#include "LCD_interface.h"
#include "LCD_private.h"
#include "LCD_config.h"
#include "stdio.h"
#define F_CPU  8000000UL
#include "util/delay.h"
		/************************************************************************/
		/*Function Name: LCD_vidInit											*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void input and return void
								initialize LCD									*/
		/************************************************************************/
void LCD_vidInit(void)
{
    _delay_ms(200);
    DIO_vidSetPinMode(LCD_RS_PORT, LCD_RS_PIN, OUTPUT);
    DIO_vidSetPinMode(LCD_RW_PORT, LCD_RW_PIN, OUTPUT);
    DIO_vidSetPinMode(LCD_EN_PORT, LCD_EN_PIN, OUTPUT);
    #if   LCD_MODE == EIGHT_BIT
        DIO_vidSetPortMode(LCD_DATA_PORT, OUTPUT);
        LCD_vidSendCmd(LCD_FUNCTION_8BIT_2LINES);
        _delay_ms(1);
    #else
        #if  NIBBLE_MODE  == LOW_NIBBLE
            DIO_vidSetLowNibbleMode(LCD_DATA_PORT, OUTPUT);
        #else
            DIO_vidSetHighNibbleMode(LCD_DATA_PORT, OUTPUT);
        #endif 
       LCD_vidSendCmd(LCD_RETURN_HOME);
        _delay_ms(15);
		LCD_vidSendCmd(LCD_FUNCTION_4BIT_2LINES);
        _delay_ms(1);
    #endif
    LCD_vidSendCmd(LCD_DISP_ON_CURSOR_ON);
    _delay_ms(1);
    LCD_vidSendCmd(LCD_CLEAR_SCREEN);
    _delay_ms(10);
    LCD_vidSendCmd(LCD_ENTRY_MODE);
}
		/************************************************************************/
		/*Function Name: LCD_vidSendCmd											*/
		/*Function Input: one input (CMD)										*/
		/*Function Description: void function take one input and return void
								send command to LCD								*/
		/************************************************************************/
static void LCD_vidSendCmd(u8 u8Cmd)
{
    DIO_vidWritePin(LCD_RW_PORT, LCD_RW_PIN, LOW); // Set Write Operation
    DIO_vidWritePin(LCD_RS_PORT, LCD_RS_PIN, LOW); // Select Command Register
    #if  LCD_MODE ==EIGHT_BIT
        DIO_vidWritePort(LCD_DATA_PORT, u8Cmd);
        LCD_vidSendEnablePulse();
    #else
        #if  NIBBLE_MODE  == LOW_NIBBLE
            DIO_vidWriteLowNibble(LCD_DATA_PORT, (u8Cmd>>4));
            LCD_vidSendEnablePulse();
			_delay_ms(2);
            DIO_vidWriteLowNibble(LCD_DATA_PORT, (u8Cmd));
            LCD_vidSendEnablePulse();
        #else
            DIO_vidWriteHighNibble(LCD_DATA_PORT, (u8Cmd));
            LCD_vidSendEnablePulse();
            DIO_vidWriteHighNibble(LCD_DATA_PORT, (u8Cmd<<4));
            LCD_vidSendEnablePulse();
        #endif
    #endif
}
		/************************************************************************/
		/*Function Name: LCD_vidSendEnablePulse									*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void input and return void
								send enable pulse to  LCD						*/
		/************************************************************************/
static void LCD_vidSendEnablePulse(void)
{
    DIO_vidWritePin(LCD_EN_PORT, LCD_EN_PIN, HIGH);
    _delay_ms(2);
    DIO_vidWritePin(LCD_EN_PORT, LCD_EN_PIN, LOW);
}
		/************************************************************************/
		/*Function Name: LCD_vidDisplayCharacter								*/
		/*Function Input: one input (char)										*/
		/*Function Description: void function take one input and return void
								display character on LCD						*/
		/************************************************************************/
void LCD_vidDisplayCharacter(u8 u8Char)
{
    DIO_vidWritePin(LCD_RW_PORT, LCD_RW_PIN, LOW); // Set Write Operation
    DIO_vidWritePin(LCD_RS_PORT, LCD_RS_PIN, HIGH); // Select Data Register
    #if  LCD_MODE ==EIGHT_BIT
        DIO_vidWritePort(LCD_DATA_PORT, u8Char);
        LCD_vidSendEnablePulse();
    #else
        #if  NIBBLE_MODE  == LOW_NIBBLE
            DIO_vidWriteLowNibble(LCD_DATA_PORT, (u8Char>>4));
            LCD_vidSendEnablePulse();
            DIO_vidWriteLowNibble(LCD_DATA_PORT, (u8Char));
            LCD_vidSendEnablePulse();
        #else
            DIO_vidWriteHighNibble(LCD_DATA_PORT, (u8Char));
            LCD_vidSendEnablePulse();
            DIO_vidWriteHighNibble(LCD_DATA_PORT, (u8Char<<4));
            LCD_vidSendEnablePulse();
        #endif
    #endif
}
		/************************************************************************/
		/*Function Name: LCD_vidDisplayString									*/
		/*Function Input: one input (pointer of string)							*/
		/*Function Description: void function take one input and return void
								display string on LCD							*/
		/************************************************************************/
void LCD_vidDisplayString(u8 *u8PtrString)
{
    while(*u8PtrString != '\0')
    {
        LCD_vidDisplayCharacter(*u8PtrString);
        u8PtrString++;
    }
}
		/************************************************************************/
		/*Function Name: LCD_vidDisplayNumber									*/
		/*Function Input: one input (number)									*/
		/*Function Description: void function take one input and return void
								Display Number on LCD							*/
		/************************************************************************/
void LCD_vidDisplayNumber(u32 u32Number)
{
	u8 u8arr[7];
	sprintf(u8arr, "%d", u32Number); 
	LCD_vidDisplayString(u8arr);
}
		/************************************************************************/
		/*Function Name: LCD_vidClearScreen										*/
		/*Function Input: no input (void)										*/
		/*Function Description: void function take void input and return void
								clear LCD										*/
		/************************************************************************/
void LCD_vidClearScreen(void)
{
	LCD_vidSendCmd(LCD_CLEAR_SCREEN);
	_delay_ms(10);
}
		/************************************************************************/
		/*Function Name: LCD_vidMoveCursor										*/
		/*Function Input: two input (line, position)							*/
		/*Function Description: void function take void input and return void
								move LCD cursor									*/
		/************************************************************************/
void LCD_vidMoveCursor(u8 u8Line, u8 u8Position)
{
	u8 u8PositionCmd;
	if(u8Line == 1)
	{
		u8PositionCmd = 0x80 + (u8Position-1);
	}
	else if(u8Line == 2)
	{
		u8PositionCmd = 0xC0 + (u8Position-1);	
	}
	else if(u8Line == 3)
	{
		u8PositionCmd = 0x90 + (u8Position-1);	
	}
	else 
	{
		u8PositionCmd = 0xD0 + (u8Position-1);
	}
	LCD_vidSendCmd(u8PositionCmd);
}
		/************************************************************************/
		/*Function Name: LCD_vidDisplayRealNumber								*/
		/*Function Input: one input (number)									*/
		/*Function Description: void function take one input and return void
								Display Real Number on LCD						*/
		/************************************************************************/
void LCD_vidDisplayRealNumber(f64 u64Number)
{
	u8 str [16];
	u8 *tmpsign = (u64Number < 0)? "-" : "";
	f16 tmpval  = (u64Number < 0)? -u64Number : u64Number;
	u16 tmpint1 = (u16)tmpval;
	f16 tmpfrac = tmpval - (f16)tmpint1;
	u16 tmpint2 = tmpfrac * 10000;
	sprintf(str,"%s%d.%04d", tmpsign, tmpint1, tmpint2);
	LCD_vidDisplayString(str);
}