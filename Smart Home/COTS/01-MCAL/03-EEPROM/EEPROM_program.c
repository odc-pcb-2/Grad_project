/*include library needed */
#include "Types.h"
#include "Bit_Math.h"

#include "EEPROM_interface.h"
#include "EEPROM_private.h"
#include "EEPROM_config.h"
/* function that write data in EEPROM */
void EEPROM_vidWrite(u16 u16Address, u8 u8Data)
{
    EEARL = (u8) u16Address;
    EEARH = (u8) (u16Address >> 8);
    EEDR  = u8Data;
    EECR  = 0x04;
    EECR |= 0x02;
    while (GET_BIT(EECR, EEWE) == 1);
}
/* function that read data in EEPROM */
u8 EEPROM_u8Read(u16 u16Address)
{
    EEARL = (u8) u16Address;            // SET EEMWE by 1
    EEARH = (u8) (u16Address >> 8);     // SET EEWE by 1
    SET_BIT(EECR, EERE);
    return EEDR;
}

