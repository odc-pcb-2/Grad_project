#ifndef _HAL_LCD_INTERFACE_H_
#define _HAL_LCD_INTERFACE_H_


void LCD_vidInit(void);
void LCD_vidDisplayCharacter(u8 u8Char);
void LCD_vidDisplayString(u8 *u8PtrString);
void LCD_vidDisplayNumber(u32 u32Number);
void LCD_vidClearScreen(void);
void LCD_vidMoveCursor(u8 u8Line, u8 u8Position);
void LCD_vidDisplayRealNumber(double Number);
#endif