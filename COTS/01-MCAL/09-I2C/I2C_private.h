#ifndef _MCAL_I2C_PRIVATE_H
#define _MCAL_I2C_PRIVATE_H

#define TWDR        *((volatile u8*)0x23)

#define TWAR       *((volatile u8*)0x22)

#define TWSR        *((volatile u8*)0x21)

#define TWCR        *((volatile u8*)0x56)
    #define TWINT   7   
    #define TWEA    6   
    #define TWSTA   5   
    #define TWSTO   4   
    #define TWWC    3   
    #define TWEN    2   
    #define TWIE    0
       
#endif