#ifndef _MCAL_EEPROM_PRIVATE_H
#define _MCAL_EEPROM_PRIVATE_H
/* define registers */
#define EEARH   *((volatile u8*)0X3F)

#define EEARL   *((volatile u8*)0X3E)

#define EEDR    *((volatile u8*)0X3D)

#define EECR    *((volatile u8*)0X3C)
    #define EERE        1
    #define EEWE        2
    #define EEMWE       3
    #define EERIE       4



#endif