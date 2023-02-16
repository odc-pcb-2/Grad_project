#include "Types.h"
#include "Bit_Math.h"

#include "ADC_interface.h"

#include "LM35_interface.h"
#include "LM35_private.h"
#include "LM35_config.h"


void LM35_vidInit(_enuADCChannels enuADCChannel)
{
    ADC_vidInit(enuADCChannel);
}

u8 LM35_u8Read(void)
{
    u8 u8Temperature;
    u16 u16Value;
    u16Value = ADC_u16Read();
    u8Temperature = (u8)((float)u16Value * 0.25f);
    return u8Temperature;
}

