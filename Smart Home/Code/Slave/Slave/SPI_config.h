#ifndef _MCAL_SPI_CONFIG_H
#define _MCAL_SPI_CONFIG_H

/* Options: Master or Slave */
#define SPI_MODE  SLAVE

/* Options: MSB or LSB */
#define SPI_DATAORDER  MSB

/* Options: IDEL_LOW or IDEL_HIGH */
#define SPI_OPERATINGLEVEL  IDEL_HIGH

/* Options: RISING or FALLING */
#define SPI_CLOCKPHASE  RISING

/* Options: NORMAL_SPEED or FALLING */
#define SPI_SPEEDMODE  NORMAL_SPEED

/* Options: FN_4, FN_16, FN_64, FN_128 for Normal Speed
            FD_2, FD_8,  FD_32, FD_64  for Double Speed 
 */
#define SPI_FREQUENCY_PRESCALER  FN_16

#endif