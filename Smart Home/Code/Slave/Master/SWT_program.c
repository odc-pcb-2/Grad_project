		/************************************************************************/
		/*                       include library needed		                    */
		/************************************************************************/
#include "Types.h"
#include "Bit_Math.h"

#include "SWT_interface.h"
#include "SWT_config.h"
#include "SWT_private.h"
		/************************************************************************/
		/*																	    */
		/************************************************************************/
void SWT_vidInti(_enuGPIOx enuPortNum, _enuPINx enuPinNum)
{
    DIO_vidSetPinMode(enuPortNum, enuPinNum, INPUT);
    /* Method 1*/
    #if INTERNAL_PULLUP == ENABLE
        DIO_vidWritePin(enuPortNum, enuPinNum, HIGH);
    #endif
}
		/************************************************************************/
		/*																		*/
		/************************************************************************/
_enuPINVALUE SWT_enuRead(_enuGPIOx enuPortNum, _enuPINx enuPinNum)
{
    _enuPINVALUE enuReturnedValue;
    enuReturnedValue = DIO_enuReadPin(enuPortNum, enuPinNum);
    return enuReturnedValue;
}