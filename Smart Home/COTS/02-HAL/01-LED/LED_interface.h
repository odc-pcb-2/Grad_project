#ifndef _HAL_LED_INTERFACE_H_
#define _HAL_LED_INTERFACE_H_
/* include library needed */
#include "DIO_interface.h"

void LED_vidInit(_enuGPIOx enuPortNum, _enuPINx enuPinNum);
void LED_vidTurnOn(_enuGPIOx enuPortNum, _enuPINx enuPinNum);
void LED_vidTurnOff(_enuGPIOx enuPortNum, _enuPINx enuPinNum);
void LED_vidToggle(_enuGPIOx enuPortNum, _enuPINx enuPinNum);
u8 LED_u8ReadStatus(u8 portname,u8 pinnumber);

#endif /* end if */