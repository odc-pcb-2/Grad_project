		/************************************************************************/
		/*                         include library needed						*/
		/************************************************************************/
#include "Types.h" 
#include "BIT_MATH.h"
#include "EEPROM_interface.h"
#include "EEPROM_private.h"
#include "EEPROM_config.h"
		/************************************************************************/
		/*Function Name: EEPROM_vidWrite										*/
		/*Function Input: two input (address, data)								*/
		/*Function Description: void function take two input and return void
								write in EEPROM									*/
		/************************************************************************/
void EEPROM_vidWrite(u16 u16Address, u8 u8Data)
{
    EEARL = (u8) u16Address;
    EEARH = (u8)(u16Address >>8);
    EEDR = u8Data;
    EECR = 0x04; // Set EEMWE by 1
    EECR |= 0x02; // Set EEWE by 1
    while(GET_BIT(EECR, EEWE) == 1);
}
		/************************************************************************/
		/*Function Name: KPD_vidInit											*/
		/*Function Input: one input (address)									*/
		/*Function Description: u8 function take one input and return u8
								read from EEPROM								*/
		/************************************************************************/
u8 EEPROM_u8Read(u16 u16Address)
{
    EEARL = (u8) u16Address;
    EEARH = (u8)(u16Address >>8); 
    SET_BIT(EECR, EERE);
    return EEDR;
}