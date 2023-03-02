		/************************************************************************/
		/*                         include library needed						*/
		/************************************************************************/
#include "Types.h"
#include "Bit_Math.h"
#include "SERVO_interface.h"
#include "SERVO_config.h"
#include "SERVO_private.h"
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
void Servo_Init(void)
{
	DDRD |= (1<<PD5);								/* Initialise OC1A pin as output */
	TCNT1 = 0;										/* Set timer1 count zero */
	ICR1 = 2499;									/* Set TOP count for timer1 in ICR1 register */
	TCCR1A = (1<<WGM11)|(1<<COM1A1);				/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, prescaler=64, timer=125kHz */
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
}
void Servo_Clockwise(u8 angle)
{
	if(angle>Max_Angle)
	{
		angle=Max_Angle;
	}
	else
	{
		OCR1A = (1.2912*angle)+Position_OCR1A;  //Servo rotates in clockwise direction, min angle=0 max angle=90
	}

}

void Servo_Counterclockwise(u8 angle)
{
	if(angle>Max_Angle)
	{
		angle=Max_Angle;
	}
	else
	{
		OCR1A = (-1.2912*angle)+Position_OCR1A;  //Servo rotates in counterclockwise direction, min angle=-90 max angle=0
	}

}
