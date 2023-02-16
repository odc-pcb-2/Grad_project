		/************************************************************************/
		/*                         include library needed						*/
		/************************************************************************/
#include "Types.h"
#include "BIT_MATH.h"
#include "DIO_interface.h"
#include "KPD_interface.h"
#include "KPD_private.h"
#include "KPD_config.h"
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
void KPD_vidInit(_enuGPIOx enuPortNum, _enuNIBBLEx enuInputNibble)
{
    if(enuInputNibble == LOW_NIBBLE)
    {
        DIO_vidSetLowNibbleMode(enuPortNum, INPUT); // Set Column Input
        DIO_vidSetHighNibbleMode(enuPortNum, OUTPUT); // Set ROW as Output
        DIO_vidWriteLowNibble(enuPortNum, 0x0F); // Enable Internal Pull Up
    }
    else
    {
        DIO_vidSetHighNibbleMode(enuPortNum, INPUT); // Set Column Input
        DIO_vidSetLowNibbleMode(enuPortNum, OUTPUT); // Set ROW as Output
        DIO_vidWriteHighNibble(enuPortNum, 0xF0); // Enable Internal Pull Up
    }
}

u8 KPD_u8GetPressedKey(_enuGPIOx enuPortNum, _enuNIBBLEx enuInputNibble)
{
    u8 KeysArr[4][4]={{'7','8','9','A'},{'4','5','6','B'},{'1','2','3','C'},{'E','0','M','D'}};
    u8 u8Rows;
    u8 u8Column;
    _enuPINVALUE enuReadValue;
    u8 u8ReturnedValue = NOT_PRESSED;
    if(enuInputNibble == LOW_NIBBLE)
    {
        for(u8Rows = 0; u8Rows <= 3; u8Rows++)
        {
            DIO_vidWriteHighNibble(enuPortNum, 0xF0);
            DIO_vidWritePin(enuPortNum, (u8Rows+4), LOW);
            for(u8Column = 0; u8Column <= 3; u8Column++)
            {
                enuReadValue = DIO_enuReadPin(enuPortNum, u8Column);
                if(LOW == enuReadValue)
                {
                    break;
                }
                else
                {
                }
            }
            if (LOW == enuReadValue)
            {
                u8ReturnedValue = KeysArr[u8Rows][u8Column];
                break;
            }
            else
            {
            }
        }
    }
    else
    {
        for(u8Rows = 0; u8Rows <= 3; u8Rows++)
        {
            DIO_vidWriteLowNibble(enuPortNum, 0x0F);
            DIO_vidWritePin(enuPortNum, (u8Rows), LOW);
            for(u8Column = 0; u8Column <= 3; u8Column++)
            {
                enuReadValue = DIO_enuReadPin(enuPortNum, (u8Column+4));
                if(LOW == enuReadValue)
                {
                    break;
                }
                else
                {
                }
            }
            if (LOW == enuReadValue)
            {
                u8ReturnedValue = KeysArr[u8Rows][u8Column];
                break;
            }
            else
            {
            }
        }
    }
    return u8ReturnedValue;
}