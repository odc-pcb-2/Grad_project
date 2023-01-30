#include "Types.h"
#include "Bit_Math.h"

#include "I2C_interface.h"
#include "I2C_config.h"
#include "I2C_private.h"

void I2C_vidInitMaster(f64 SCL_frequency)
{
    TWBR = (u8)(((F_CPU/SCL_frequency)-16)/(2* PRESCALER));
	
    /* Adjust prescaler */ 
	#if   PRESCALER == 1
		TWSR = 0;
	#elif PRESCALER == 4
		TWSR = 1;
	#elif PRESCALER == 16
		TWSR = 2; 
	#else 
		TWSR = 3;
	#endif
}

void I2C_vidStart(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTA);   //1010 0100 /*SET_BIT(TWCR, TWINT); SET(TWCR , TWEN); SET_BIT(TWCR , TWSTA);*/
	//TWCR = 0b10100100
	while(GET_BIT(TWCR,TWINT)==0); // wait until start condition sent
	while((TWSR &0xF8) != 0x08);
}

void I2C_vidWriteAddress(u8 Address)
{
    TWDR = Address;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(GET_BIT(TWCR, TWINT) ==0); // wait to finish 
	while((TWSR &0xF8) != 0x18); // Address has been transmitted and ACK has been Received 
}

void I2C_vidWriteDate(u8 Data)
{
    TWDR = Data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(GET_BIT(TWCR, TWINT) ==0); // wait to finish 
	while((TWSR &0xF8) != 0x28); // Data has been Transmitted and ACK received   
}

void I2C_vidStop(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); 
}

void I2C_vidSetAddress(u8 Address)
{
    TWAR = Address;
}

u8 I2C_u8SalveRead(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while(GET_BIT(TWCR, TWINT) ==0);
	while((TWSR &0xF8) != 0x60); // Address Recived and ACK has been returned
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while(GET_BIT(TWCR, TWINT) ==0);
	while((TWSR &0xF8) != 0x80); // Data Recived and ACK has been returned
	return TWDR;
}

