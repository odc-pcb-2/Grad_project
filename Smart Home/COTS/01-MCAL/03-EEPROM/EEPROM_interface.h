#ifndef _MCAL_EEPROM_INTERFACE_H
#define _MCAL_EEPROM_INTERFACE_H

void EEPROM_vidWrite(u16 u16Address, u8 u8Data);

u8 EEPROM_u8Read(u16 u16Address);

#endif