#ifndef _HAL_SEVSEG_INTERFACE_H_
#define _HAL_SEVSEG_INTERFACE_H_

#include "DIO_interface.h"

void SEVSEG_vidInti(_enuGPIOx enuPortNum);

void SEVSEG_vidDisplay(_enuGPIOx enuPortNum, u8 u8Value);

void SEVSEG_vidInti4Bit(_enuGPIOx enuPortNum, _enuNIBBLEx enuNibbleNum);

void SEVSEG_vidDisplay4Bit(_enuGPIOx enuPortNum, _enuNIBBLEx enuNibbleNum, u8 u8Value);

#endif