#ifndef _HAL_LED_INTERFACE_H_
#define _HAL_LED_INTERFACE_H_
		/************************************************************************/
		/*                         include library needed						*/
		/************************************************************************/
#include "DIO_interface.h"
		/************************************************************************/
		/*                       declare functions needed	                    */
		/************************************************************************/
void LED_vidInit(_enuGPIOx enuPortNum, _enuPINx enuPinNum);
void LED_vidTurnOn(_enuGPIOx enuPortNum, _enuPINx enuPinNum);
void LED_vidTurnOff(_enuGPIOx enuPortNum, _enuPINx enuPinNum);
void LED_vidToggle(_enuGPIOx enuPortNum, _enuPINx enuPinNum);

#endif