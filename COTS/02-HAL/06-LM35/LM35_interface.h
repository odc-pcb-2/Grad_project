#ifndef _HAL_LM35_INTERFACE_H_
#define _HAL_LM35_INTERFACE_H_

void LM35_vidInit(_enuADCChannels enuADCChannel);

u8 LM35_u16Read(void);

#endif