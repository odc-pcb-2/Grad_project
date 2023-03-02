		/************************************************************************/
		/*                         include library needed						*/
		/************************************************************************/
#include "Types.h"
#include "Bit_Math.h"
#include "LED_config.h"
#include "LED_interface.h"
#include "LED_private.h"
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
void LED_vidInit(_enuGPIOx enuPortNum, _enuPINx enuPinNum)
{
    DIO_vidSetPinMode(enuPortNum, enuPinNum, OUTPUT);   
}
		/************************************************************************/
		/*													                    */
		/************************************************************************/
void LED_vidTurnOn(_enuGPIOx enuPortNum, _enuPINx enuPinNum)
{
    DIO_vidWritePin(enuPortNum, enuPinNum, HIGH);
}
		/************************************************************************/
		/*													                    */
		/************************************************************************/
void LED_vidTurnOff(_enuGPIOx enuPortNum, _enuPINx enuPinNum)
{
    DIO_vidWritePin(enuPortNum, enuPinNum, LOW);
}
		/************************************************************************/
		/*													                    */
		/************************************************************************/
void LED_vidToggle(_enuGPIOx enuPortNum, _enuPINx enuPinNum)
{
    DIO_vidTogglePin(enuPortNum, enuPinNum);
}

