/* include library needed */
#include "Types.h"
#include "Bit_Math.h"

#include "DIO_interface.h"
#include "TIMER1_interface.h"

#include "SERVO_interface.h"
#include "SERVO_config.h"
#include "SERVO_private.h"
/* function that initialize servo */
void SERVO_vidInit(void)
{
    TCNT1 =0;
    Timer1_voidInit(TIMER1_FASTPWM_ICR_TOP_MODE,TIMER1_SCALER_64,OCRA_INVERTING,OCRB_INVERTING);
    ICR1 = 19999;
}
/* function that rotate servo with 0 dgree */
void SERVO_vidRotateDegree0(void)
{
    OCR1A = ICR1 - 2200;
}
/* function that rotate servo with 90 dgree */
void SERVO_vidRotateDegree90(void)
{
    OCR1A = ICR1 - 1500;
}
/* function that rotate servo with 180 dgree */
void SERVO_vidRotateDegree180(void)
{
    OCR1A = ICR1 - 800;
}
/* function that initialize servo */
void Servo_Init(void)
{
	DDRD |= (1<<PD5);								/* Initialise OC1A pin as output */
	TCNT1 = 0;										/* Set timer1 count zero */
	ICR1 = 2499;									/* Set TOP count for timer1 in ICR1 register */
	TCCR1A = (1<<WGM11)|(1<<COM1A1);				/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, prescaler=64, timer=125kHz */
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
}
/* function that rotate servo with clockwise */
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
/* function that rotate servo with anti clockwise */
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
