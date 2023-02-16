		/************************************************************************/
		/*                         include library needed						*/
		/************************************************************************/	
#include "Bit_Math.h"
#include "Types.h"
#include "DIO_interface.h"
#include "DC_Motor_config.h"
#include "DC_Motor_private.h"
#include "DC_Motor_interface.h"
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
void DC_Motor_vidInit()
{
DIO_vidSetPinMode(DC_MOTOR_PORT,DC_MOTOR_PIN0,OUTPUT);
DIO_vidSetPinMode(DC_MOTOR_PORT,DC_MOTOR_PIN1,OUTPUT);
}
void DC_Motor_vidTurnOn()
{
    DIO_vidWritePin(DC_MOTOR_PORT,DC_MOTOR_PIN0,HIGH);
    DIO_vidWritePin(DC_MOTOR_PORT,DC_MOTOR_PIN1,LOW);
}
void DC_Motor_vidTurnOff()
{
    DIO_vidWritePin(DC_MOTOR_PORT,DC_MOTOR_PIN0,LOW);
    DIO_vidWritePin(DC_MOTOR_PORT,DC_MOTOR_PIN1,LOW);
}