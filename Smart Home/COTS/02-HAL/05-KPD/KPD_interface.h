#ifndef _HAL_KPD_INTERFACE_H_
#define _HAL_KPD_INTERFACE_H_
/* include library needed */
#include "DIO_interface.h"

#define NOT_PRESSED     0xFF

void KPD_vidInit(_enuGPIOx enuPortNum, _enuNIBBLEx enuInputNibble);

u8 KPD_u8GetPressedKey(_enuGPIOx enuPortNum, _enuNIBBLEx enuInputNibble);

#endif