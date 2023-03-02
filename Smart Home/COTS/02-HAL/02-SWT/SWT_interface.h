#ifndef _HAL_SWT_INTERFACE_H_
#define _HAL_SWT_INTERFACE_H_
/* include library needed */
#include "DIO_interface.h"

void SWT_vidInit(_enuGPIOx enuPortNum, _enuPINx enuPinNum);

_enuPINVALUE SWT_enuRead(_enuGPIOx enuPortNum, _enuPINx enuPinNum);

#endif