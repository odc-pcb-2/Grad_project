#ifndef _MCAL_DIO_INTERFACE_H_ 
#define _MCAL_DIO_INTERFACE_H_
/* define ports */
typedef enum
{
    GPIOA = 0,
    GPIOB,
    GPIOC,
    GPIOD 
}_enuGPIOx;
/* define pins */
typedef enum
{
    PIN0 = 0,
    PIN1,
    PIN2,
    PIN3,
    PIN4,
    PIN5,
    PIN6,
    PIN7 
}_enuPINx;
/* define status */
typedef enum
{
    INPUT = 0,
    OUTPUT
}_enuIOx;
/* define pin value */
typedef enum
{
    LOW = 0,
    HIGH
}_enuPINVALUE;
/* define nibble */
typedef enum
{
    LOW_NIBBLE = 0,
    HIGH_NIBBLE 
}_enuNIBBLEx;

/* pin function */

void DIO_vidSetPinMode(_enuGPIOx enuPortNum, _enuPINx enuPinNum, _enuIOx enuPinDirection);

void DIO_vidWritePin(_enuGPIOx enuPortNum, _enuPINx enuPinNum, _enuPINVALUE enuPinValue);

_enuPINVALUE DIO_enuReadPin(_enuGPIOx enuPortNum, _enuPINx enuPinNum);

void DIO_vidTogglePin(_enuGPIOx enuPortNum, _enuPINx enuPinNum);

/* Port function */

void DIO_vidSetPortMode(_enuGPIOx enuPortNum, _enuIOx enuPinDirection);

void DIO_vidWritePort(_enuGPIOx enuPortNum, u8 u8Value);

/* Four Bits Functions*/

void DIO_vidSetLowNibbleMode(_enuGPIOx enuPortNum, _enuIOx enuPinDirection);

void DIO_vidWriteLowNibble(_enuGPIOx enuPortNum, u8 u8Value);

void DIO_vidSetHighNibbleMode(_enuGPIOx enuPortNum, _enuIOx enuPinDirection);

void DIO_vidWriteHighNibble(_enuGPIOx enuPortNum, u8 u8Value);

#endif
