#ifndef _MCAL_SPI_INTERFACE_H
#define _MCAL_SPI_INTERFACE_H

void SPI_vidInit(void);

u8 SPI_u8SendReceive(u8 copy_Data);

void SPI_vidSendString(u8 *copy_ptr);

#endif