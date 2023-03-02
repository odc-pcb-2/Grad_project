#ifndef _HAL_SERVO_INTERFACE_H_
#define _HAL_SERVO_INTERFACE_H_
		/************************************************************************/
		/*                         include library needed						*/
		/************************************************************************/
#include "avr/io.h"
#define F_CPU 8000000UL								/* Define CPU Frequency e.g. here its 8MHz */
#include "util/delay.h"
#include "Types.h"
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
#define Max_Angle 90
#define Min_Angle 90
#define Position0 0
#define Position_OCR1A 182
		/************************************************************************/
		/*                       declare functions needed	                    */
		/************************************************************************/
void Servo_Init(void);
void Servo_Clockwise(u8 angle);
void Servo_Counterclockwise(u8 angle);

#endif