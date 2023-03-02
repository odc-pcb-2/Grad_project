#ifndef _HAL_LM35_INTERFACE_H_
#define _HAL_LM35_INTERFACE_H_
		/************************************************************************/
		/*                       include library needed		                    */
		/************************************************************************/
#include "ADC_interface.h"
		/************************************************************************/
		/*                       declare functions needed	                    */
		/************************************************************************/
void LM35_vidInit(_enuADCChannels enuADCChannel);

u8 LM35_u8Read(void);

#endif