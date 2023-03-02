/* include library needed */
#include "Types.h"
#include "Bit_Math.h"

#include "KPD_interface.h"
#include "KPD_private.h"
#include "KPD_config.h"
/* function that initialize kpd */
void KPD_vidInit(_enuGPIOx enuPortNum, _enuNIBBLEx enuInputNibble)
{
    if (enuInputNibble == LOW_NIBBLE)
    {
        DIO_vidSetLowNibbleMode(enuPortNum, INPUT);     // set column input
        DIO_vidSetHighNibbleMode(enuPortNum, OUTPUT);   // set Row output
        DIO_vidWriteLowNibble(enuPortNum, 0x0F);        // Enable internal pull up 
    }
    else
    {
        DIO_vidSetLowNibbleMode(enuPortNum, OUTPUT);   // set Row output
        DIO_vidSetHighNibbleMode(enuPortNum, INPUT);   // set column input
        DIO_vidWriteHighNibble(enuPortNum, 0xF0);      // Enable internal pull up  
    }
}
/* function that return pressed key from kpd s*/
u8 KPD_u8GetPressedKey(_enuGPIOx enuPortNum, _enuNIBBLEx enuInputNibble)
{
    u8 KeyArr[4][4]={{'7', '8', '9', '/'}, {'4', '5', '6', '*'}, {'1', '2', '3', '-'}, {'A', '0', '=', '+'}};
    u8 u8Rows;
    u8 u8Column;
    _enuPINVALUE enuReadValue;
    u8 u8ReternedValue = NOT_PRESSED;
    if (enuInputNibble == LOW_NIBBLE )
    {
        for(u8Rows =0; u8Rows <= 3; u8Rows++)
        {
            DIO_vidWriteHighNibble(enuPortNum, 0xF0);
            DIO_vidWritePin(enuPortNum, (u8Rows + 4), LOW);
            for ( u8Column = 0; u8Column <= 3; u8Column++)
            {
                enuReadValue = DIO_enuReadPin(enuPortNum, u8Column);
                if (LOW == enuReadValue)
                {
                    break;
                }
                else
                {
                }
            }
            if (LOW == enuReadValue)
                {
                    u8ReternedValue = KeyArr[u8Rows][u8Column];
                    break;
                }
            else
                {
                }
        }
    }
    else
    {
        for(u8Rows =0; u8Rows <= 3; u8Rows++)
        {
            DIO_vidWriteLowNibble(enuPortNum, 0x0F);
            DIO_vidWritePin(enuPortNum, (u8Rows), LOW);  
            for ( u8Column = 0; u8Column <= 3; u8Column++)
            {
                enuReadValue = DIO_enuReadPin(enuPortNum, u8Column);
                if (LOW == enuReadValue)
                {
                    break;
                }
                else
                {
                }
            }
            if (LOW == enuReadValue)
                {
                    u8ReternedValue = KeyArr[u8Rows][u8Column];
                    break;
                }
            else
                {
                }
        }
    }
    return u8ReternedValue;
}
