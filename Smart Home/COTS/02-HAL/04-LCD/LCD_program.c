/* inlude library needed */
#include "Types.h"
#include "Bit_Math.h"

#include "LCD_interface.h"
#include "LCD_private.h"
#include "LCD_config.h"

#define F_CPU 8000000UL
#include "util/delay.h"
/* function that initialize lcd */
void LCD_vidInit(void)
{
    _delay_ms(200);
    DIO_vidSetPinMode(LCD_RS_PORT, LCD_RS_PIN, OUTPUT);
    DIO_vidSetPinMode(LCD_RW_PORT, LCD_RW_PIN, OUTPUT);
    DIO_vidSetPinMode(LCD_EN_PORT, LCD_EN_PIN, OUTPUT);

    #if LCD_MODE == EIGHT_BIT
        DIO_vidSetPortMode(LCD_DATA_PORT, OUTPUT);
        LCD_vidSendCmd(LCD_FUNCTION_8BIT_2LINES);
        _delay_ms(1);
    #else
        #if NIBBLE_MODE == LOW_NIBBLE
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
/* function that send command for rs & rw of lcd */
static void LCD_vidSendCmd(u8 u8Cmd)
{
    DIO_vidWritePin(LCD_RW_PORT, LCD_RW_PIN, LOW);
    DIO_vidWritePin(LCD_RS_PORT, LCD_RS_PIN, LOW);
    #if LCD_MODE == EIGHT_BIT
        DIO_vidWritePort(LCD_DATA_PORT, u8Cmd);
        LCD_vidSendEnablePulse();
    #else
        #if NIBBLE_MODE == LOW_NIBBLE
            DIO_vidWriteLowNibble(LCD_DATA_PORT, (u8Cmd>>4));
            LCD_vidSendEnablePulse();
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
/* function that send pulse for enable pin */
static void LCD_vidSendEnablePulse(void)
{
    DIO_vidWritePin(LCD_EN_PORT, LCD_EN_PIN, HIGH);
    _delay_ms(2);
    DIO_vidWritePin(LCD_EN_PORT, LCD_EN_PIN, LOW);
}
/* function that display character on lcd */
void LCD_vidDisplayCharacter(u8 u8Char)
{
    DIO_vidWritePin(LCD_RW_PORT, LCD_RW_PIN, LOW);
    DIO_vidWritePin(LCD_RS_PORT, LCD_RS_PIN, HIGH);
    #if LCD_MODE == EIGHT_BIT
        DIO_vidWritePort(LCD_DATA_PORT, u8Char);
        LCD_vidSendEnablePulse();
    #else
        #if NIBBLE_MODE == LOW_NIBBLE
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
/* function that display string on lcd */
void LCD_vidDisplayString(u8 *u8PtrString)
{
    while (*u8PtrString != '\0')
    {
        LCD_vidDisplayCharacter(*u8PtrString);
        u8PtrString++;
    }    
}
/* function that display number on lcd */
void LCD_vidDisplayNumber(u32 u32Number)
{
    u8 u8arr[7];
    sprintf(u8arr, "%d", u32Number);
    LCD_vidDisplayString(u8arr);
}
/* function that clear lcd screen */
void LCD_vidClearScreen(void)
{
    LCD_vidSendCmd(LCD_CLEAR_SCREEN);
    _delay_ms(10);
}
/* function that move lcd cursor */
void LCD_vidMoveCursor(u8 u8Line, u8 u8Position)
{
    u8 u8PositionCmd;
    if (u8Line == 1)
    {
        u8PositionCmd = 0x80 + (u8Position - 1);
    }
    else
    {
        u8PositionCmd = 0xC0 + (u8Position - 1);
    }
    LCD_vidSendCmd(u8PositionCmd);
}
/* function that display real number on lcd */
void LCD_vidDisplayRealNumber(double Number)
{
    u8 Str[16];
    u8 *tmpsign = (Number < 0)? "-" : "";
    float tmpVal = (Number < 0)? -Number : Number;
    u16 tmpint1 = (u16)tmpVal;
    float tmpfrac = tmpVal - (float)tmpint1;
    u16 tmpint2 = tmpfrac * 10000;
    sprintf(Str, "%s%d.%0.4d", tmpsign, tmpint1, tmpint2);
    LCD_vidDisplayString(Str);
}
