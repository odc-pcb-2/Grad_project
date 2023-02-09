#ifndef _HAL_SWT_INTERFACE_H_
#define _HAL_SWT_INTERFACE_H_

#include "DIO_interface.h"

void SWT_vidInti(_enuGPIOx enuPortNum, _enuPINx enuPinNum);

_enuPINVALUE SWT_enuRead(_enuGPIOx enuPortNum, _enuPINx enuPinNum);

#endif