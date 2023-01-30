#ifndef _MCAL_I2C_INTERFACE_H
#define _MCAL_I2C_INTERFACE_H

void I2C_vidInitMaster(f64 SCL_frequency);

void I2C_vidStart(void);

void I2C_vidWriteAddress(u8 Address);

void I2C_vidWriteDate(u8 Data);

void I2C_vidStop(void);

void I2C_vidSetAddress(u8 Address);

u8 I2C_u8SalveRead(void);

#endif