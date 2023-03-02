/* include library needed */
/* COMMON Include*/
#include "Types.h"
#include "Bit_Math.h"

/* MCAL Include*/
#include "DIO_interface.h"
#include "DIO_private.h"
#include "DIO_config.h"

/* function that define mode for pin (input or output) */
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
/* function that generate (high or low) on pin */
void DIO_vidWritePin(_enuGPIOx enuPortNum, _enuPINx enuPinNum, _enuPINVALUE enuPinValue)
{
    switch(enuPortNum)
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
/* function that return pin stuts (high or low) */
_enuPINVALUE DIO_enuReadPin(_enuGPIOx enuPortNum, _enuPINx enuPinNum)
{
    _enuPINVALUE enuPinReturnedValue;
    switch(enuPortNum)
    {
        case GPIOA:
            enuPinReturnedValue = GET_BIT(PINA,enuPinNum);
            break;
        case GPIOB:
            enuPinReturnedValue = GET_BIT(PINB,enuPinNum);
            break;
        case GPIOC:
            enuPinReturnedValue = GET_BIT(PINC,enuPinNum);
            break;
        case GPIOD:
            enuPinReturnedValue = GET_BIT(PIND,enuPinNum);
            break;
        default:
            break;
    }
    return enuPinReturnedValue;
}
/* function that toogle pin status */
void DIO_vidTogglePin(_enuGPIOx enuPortNum, _enuPINx enuPinNum)
{
    switch (enuPortNum)
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
/* function that define mode for port (input or output) */
void DIO_vidSetPortMode(_enuGPIOx enuPortNum, _enuIOx enuPinDirection)
{
    switch (enuPortNum)
    {
    case GPIOA:
        if(OUTPUT == enuPinDirection)
        {
            DDRA = 0xFF;
        }
        else
        {
            DDRA = 0x00;
        }
        break;
    case GPIOB:
        if(OUTPUT == enuPinDirection)
        {
            DDRB = 0xFF;
        }
        else
        {
            DDRB = 0x00;
        }
        break;
    case GPIOC:
        if(OUTPUT == enuPinDirection)
        {
            DDRC = 0xFF;
        }
        else
        {
            DDRC = 0x00;
        }
        break;
    case GPIOD:
        if(OUTPUT == enuPinDirection)
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
/* function that generate (high or low) on port */
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
/* function that define low nibble mode for pin (input or output) */
void DIO_vidSetLowNibbleMode(_enuGPIOx enuPortNum, _enuIOx enuPinDirection)
{
    u8 u8Counter =0;
    for (u8Counter = 0; u8Counter <= 3; u8Counter++)
    {
        DIO_vidSetPinMode(enuPortNum, u8Counter, enuPinDirection);
    }
}
/* function that generate (high or low) on pin in low nibble mode */
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
/* function that define high nibble mode for pin (input or output) */
void DIO_vidSetHighNibbleMode(_enuGPIOx enuPortNum, _enuIOx enuPinDirection)
{
    u8 u8Counter = 4;
    for (u8Counter = 4; u8Counter <= 7; u8Counter++)
    {
        DIO_vidSetPinMode(enuPortNum, u8Counter, enuPinDirection);
    }    
}
/* function that generate (high or low) on pin in high nibble mode */
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

