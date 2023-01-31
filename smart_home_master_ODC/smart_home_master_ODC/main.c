/*
 * smart_home_master_ODC.c
 *
 * Created: 1/30/2023 4:38:26 AM
 * Author : Ahmed Emad
 */ 
#include "Types.h"
#include "DIO_interface.h"
#include "LCD_interface.h"
#include "LM35_interface.h"

int main(void)
{
	u8 temp,pre=1;
	DIO_vidSetPinMode(GPIOB,PIN0,OUTPUT);
	LCD_vidInit();
	LM35_vidInit(ADC_CH0);
    LCD_vidDisplayString("1.Door Lock");
	LCD_vidMoveCursor(2,1);
    LCD_vidDisplayString("2.Air off");
	LCD_vidMoveCursor(3,1);
    LCD_vidDisplayString("3.buzzer off");
    LCD_vidMoveCursor(4,1);
    LCD_vidDisplayString("4.Temp off");
	LCD_vidClearScreen();
	while (1) 
    {
	    temp = LM35_u8Read();
		if(temp != pre)
		{
			LCD_vidMoveCursor(1,1);
			LCD_vidDisplayString("Temperature =");
		    LCD_vidDisplayNumber(temp);
			LCD_vidDisplayString("C");
			pre = temp;
			if(temp >= 25)
			{
				LCD_vidMoveCursor(2,1);
				LCD_vidDisplayString("FAN ON ");
				DIO_vidWritePin(GPIOB,PIN0,HIGH);
			}
			else if(temp <= 21 )
			{
				LCD_vidMoveCursor(2,1);
				LCD_vidDisplayString("FAN OFF ");
				DIO_vidWritePin(GPIOB,PIN0,LOW);
			}
		}
    }
}

