#include "Types.h"
#include "Bit_Math.h"

#include "SEVSEG_config.h"
#include "SEVSEG_interface.h"
#include "SEVSEG_private.h"


void SEVSEG_vidInti(_enuGPIOx enuPortNum)
{
    DIO_vidSetPortMode(enuPortNum, OUTPUT);
}

void SEVSEG_vidDisplay(_enuGPIOx enuPortNum, u8 u8Value)
{
    u8 arr[10] = {0x3F, 0x06, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    DIO_vidWritePort(enuPortNum, arr[u8Value]);
}

void SEVSEG_vidInti4Bit(_enuGPIOx enuPortNum, _enuNIBBLEx enuNibbleNum)
{
    if(LOW_NIBBLE == enuNibbleNum)
    {
        DIO_vidSetLowNibbleMode(enuPortNum, OUTPUT);
    }
    else
    {
        DIO_vidSetHighNibbleMode(enuPortNum, OUTPUT);
    }
}
void SEVSEG_vidDisplay4Bit(_enuGPIOx enuPortNum, _enuNIBBLEx enuNibbleNum, u8 u8Value)
{
    if (LOW_NIBBLE == enuNibbleNum)
    {
        DIO_vidWriteLowNibble(enuPortNum, u8Value);
    }
    else
    {
        DIO_vidWriteHighNibble(enuPortNum, u8Value);
    }
}
