/* include library needed */
#include "Types.h"
#include "Bit_Math.h"
#include "LED_config.h"
#include "LED_interface.h"
#include "LED_private.h"

/* function that initialize the led */
void LED_vidInit(_enuGPIOx enuPortNum, _enuPINx enuPinNum)
{
    DIO_vidSetPinMode(enuPortNum, enuPinNum, OUTPUT);   
}
/* function that turn on the led */
void LED_vidTurnOn(_enuGPIOx enuPortNum, _enuPINx enuPinNum)
{
    DIO_vidWritePin(enuPortNum, enuPinNum, HIGH);
}
/* function that turn off the led */
void LED_vidTurnOff(_enuGPIOx enuPortNum, _enuPINx enuPinNum)
{
    DIO_vidWritePin(enuPortNum, enuPinNum, LOW);
}
/* function that toggle the led */
void LED_vidToggle(_enuGPIOx enuPortNum, _enuPINx enuPinNum)
{
    DIO_vidTogglePin(enuPortNum, enuPinNum);
}
/* function that read status of the led */
u8 LED_u8ReadStatus(u8 portname,u8 pinnumber)
{
	return DIO_u8read(portname,pinnumber);//return the current status of the given pin
}

