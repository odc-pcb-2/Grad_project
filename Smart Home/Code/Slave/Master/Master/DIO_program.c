		/************************************************************************/
		/*				     	 include library needed							*/
		/************************************************************************/
/* Common Includes*/
#include "Types.h"
#include "BIT_MATH.h"
/* MCAL Include */
#include "DIO_interface.h"
#include "DIO_private.h"
#include "DIO_config.h"
		/************************************************************************/
		/*Function Name: DIO_vidSetPinMode										*/
		/*Function Input: three input (port number, pin number, pin direction)	*/
		/*Function Description: void function take three input and return void
								set pin mode									*/
		/************************************************************************/
void DIO_vidSetPinMode(_enuGPIOx enuPortNum, _enuPINx enuPinNum, _enuIOx enuPinDirection)
{
    switch (enuPortNum)
    {
    case GPIOA:
        if(OUTPUT == enuPinDirection)
        {
            SET_BIT(DDRA,enuPinNum);
        }
        else
        {
            CLR_BIT(DDRA,enuPinNum);
        }
        break;
    case GPIOB:
        if(OUTPUT == enuPinDirection)
        {
            SET_BIT(DDRB,enuPinNum);
        }
        else
        {
            CLR_BIT(DDRB,enuPinNum);
        }
        break;
    case GPIOC:
        if(OUTPUT == enuPinDirection)
        {
            SET_BIT(DDRC,enuPinNum);
        }
        else
        {
            CLR_BIT(DDRC,enuPinNum);
        }
        break;
    case GPIOD:
        if(OUTPUT == enuPinDirection)
        {
            SET_BIT(DDRD,enuPinNum);
        }
        else
        {
            CLR_BIT(DDRD,enuPinNum);
        }
        break;
    default:
        break;
    }
}
		/************************************************************************/
		/*Function Name: DIO_vidWritePin										*/
		/*Function Input: three input (port number, pin number, pin value)		*/
		/*Function Description: void function take three input and return void
								write to pin (high or low)						*/
		/************************************************************************/
void DIO_vidWritePin(_enuGPIOx enuPortNum, _enuPINx enuPinNum, _enuPINVALUE enuPinValue)
{
    switch (enuPortNum)
    {
    case GPIOA:
        if(HIGH == enuPinValue)
        {
            SET_BIT(PORTA,enuPinNum);
        }
        else
        {
            CLR_BIT(PORTA,enuPinNum);
        }
        break;
    case GPIOB:
        if(HIGH == enuPinValue)
        {
            SET_BIT(PORTB,enuPinNum);
        }
        else
        {
            CLR_BIT(PORTB,enuPinNum);
        }
        break;
    case GPIOC:
        if(HIGH == enuPinValue)
        {
            SET_BIT(PORTC,enuPinNum);
        }
        else
        {
            CLR_BIT(PORTC,enuPinNum);
        }
        break;
    case GPIOD:
        if(HIGH == enuPinValue)
        {
            SET_BIT(PORTD,enuPinNum);
        }
        else
        {
            CLR_BIT(PORTD,enuPinNum);
        }
        break;
    default:
        break;
    }
}
		/************************************************************************/
		/*Function Name: DIO_enuReadPin										*/
		/*Function Input: two input (address, data)								*/
		/*Function Description: void function take two input and return void
								write in EEPROM									*/
		/************************************************************************/
_enuPINVALUE DIO_enuReadPin(_enuGPIOx enuPortNum, _enuPINx enuPinNum)
{
    _enuPINVALUE enuPinReturnedValue;
    switch(enuPortNum)
    {
        case GPIOA:
            enuPinReturnedValue = GET_BIT(PINA, enuPinNum);
            break;
        case GPIOB:
            enuPinReturnedValue = GET_BIT(PINB, enuPinNum);
            break;
        case GPIOC:
            enuPinReturnedValue = GET_BIT(PINC, enuPinNum);
            break;
        case GPIOD:
            enuPinReturnedValue = GET_BIT(PIND, enuPinNum);
            break;
        default:
            break;
    }
    return enuPinReturnedValue;
}
		/************************************************************************/
		/*Function Name: DIO_vidTogglePin										*/
		/*Function Input: two input (address, data)								*/
		/*Function Description: void function take two input and return void
								write in EEPROM									*/
		/************************************************************************/
void DIO_vidTogglePin(_enuGPIOx enuPortNum, _enuPINx enuPinNum)
{
   switch(enuPortNum)
    {
        case GPIOA:
            TOG_BIT(PORTA, enuPinNum);
            break;
        case GPIOB:
            TOG_BIT(PORTB, enuPinNum);
            break;
        case GPIOC:
            TOG_BIT(PORTC, enuPinNum);
            break;
        case GPIOD:
            TOG_BIT(PORTD, enuPinNum);
            break;
        default:
            break;
    } 
}
		/************************************************************************/
		/*Function Name: DIO_vidSetPortMode										*/
		/*Function Input: two input (address, data)								*/
		/*Function Description: void function take two input and return void
								write in EEPROM									*/
		/************************************************************************/
void DIO_vidSetPortMode(_enuGPIOx enuPortNum,  _enuIOx enuPinDirection)
{
    switch (enuPortNum)
    {
    case GPIOA:
        if(enuPinDirection == OUTPUT)
        {
            DDRA = 0xFF;
        }
        else
        {
            DDRA = 0x00;
        }
        break;
    case GPIOB:
        if(enuPinDirection == OUTPUT)
        {
            DDRB = 0xFF;
        }
        else
        {
            DDRB = 0x00;
        }
        break;
    case GPIOC:
        if(enuPinDirection == OUTPUT)
        {
            DDRC = 0xFF;
        }
        else
        {
            DDRC = 0x00;
        }
        break;
    case GPIOD:
        if(enuPinDirection == OUTPUT)
        {
            DDRD = 0xFF;
        }
        else
        {
            DDRD = 0x00;
        }
        break;
    default:
        break;
    }
}
		/************************************************************************/
		/*Function Name: DIO_vidWritePort										*/
		/*Function Input: two input (address, data)								*/
		/*Function Description: void function take two input and return void
								write in EEPROM									*/
		/************************************************************************/
void DIO_vidWritePort(_enuGPIOx enuPortNum, u8 u8Value)  
{
    switch (enuPortNum)
    {
    case GPIOA:
        PORTA = u8Value;
        break;
    case GPIOB:
        PORTB = u8Value;
        break;
    case GPIOC:
        PORTC = u8Value;
        break;
    case GPIOD:
        PORTD = u8Value;
        break;    
    default:
        break;
    }
}
		/************************************************************************/
		/*Function Name: DIO_vidSetLowNibbleMode								*/
		/*Function Input: two input (address, data)								*/
		/*Function Description: void function take two input and return void
								write in EEPROM									*/
		/************************************************************************/
void DIO_vidSetLowNibbleMode(_enuGPIOx enuPortNum , _enuIOx enuPinDirection)
{
    u8 u8Counter = 0;
    for (u8Counter = 0; u8Counter <= 3; u8Counter++)
    {
        DIO_vidSetPinMode(enuPortNum, u8Counter, enuPinDirection);
    }
    
}
		/************************************************************************/
		/*Function Name: DIO_vidWriteLowNibble									*/
		/*Function Input: two input (address, data)								*/
		/*Function Description: void function take two input and return void
								write in EEPROM									*/
		/************************************************************************/
void DIO_vidWriteLowNibble(_enuGPIOx enuPortNum, u8 u8Value)
{
    u8Value &= 0x0F;
    switch (enuPortNum)
    {
    case GPIOA:
        PORTA &= 0xF0;
        PORTA |= u8Value;
        break;
    case GPIOB:
        PORTB &= 0xF0;
        PORTB |= u8Value;
        break;
    case GPIOC:
        PORTC &= 0xF0;
        PORTC |= u8Value;
        break;
    case GPIOD:
        PORTD &= 0xF0;
        PORTD |= u8Value;
        break;
    default:
        break;
    }
}
		/************************************************************************/
		/*Function Name: DIO_vidSetHighNibbleMode								*/
		/*Function Input: two input (address, data)								*/
		/*Function Description: void function take two input and return void
								write in EEPROM									*/
		/************************************************************************/
void DIO_vidSetHighNibbleMode(_enuGPIOx enuPortNum , _enuIOx enuPinDirection)
{
    u8 u8Counter = 4;
    for (u8Counter = 4; u8Counter <= 7; u8Counter++)
    {
        DIO_vidSetPinMode(enuPortNum, u8Counter, enuPinDirection);
    }
}
		/************************************************************************/
		/*Function Name: DIO_vidWriteHighNibble									*/
		/*Function Input: two input (address, data)								*/
		/*Function Description: void function take two input and return void
								write in EEPROM									*/
		/************************************************************************/
void DIO_vidWriteHighNibble(_enuGPIOx enuPortNum, u8 u8Value)
{
    u8Value &= 0xF0;
    switch (enuPortNum)
    {
    case GPIOA:
        PORTA &= 0x0F;
        PORTA |= u8Value;
        break;
    case GPIOB:
        PORTB &= 0x0F;
        PORTB |= u8Value;
        break;
    case GPIOC:
        PORTC &= 0x0F;
        PORTC |= u8Value;
        break;
    case GPIOD:
        PORTD &= 0x0F;
        PORTD |= u8Value;
        break;
    default:
        break;
    }
}