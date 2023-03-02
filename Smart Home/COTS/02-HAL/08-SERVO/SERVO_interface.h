#ifndef _HAL_SERVO_INTERFACE_H_
#define _HAL_SERVO_INTERFACE_H_
/* include library needed */
#include "Types.h"

#define F_CPU 8000000UL								/* Define CPU Frequency e.g. here its 8MHz */
#include <util/delay.h>

/*Min angle=-90
Max angle=90
Postition0=0, OCR1A=182*/
#define Max_Angle 90
#define Min_Angle 90
#define Position0 0
#define Position_OCR1A 182

void Servo_Init(void);

void Servo_Clockwise(u8 angle);

void Servo_Counterclockwise(u8 angle);

void SERVO_vidInit(void);

void SERVO_vidRotateDegree0(void);

void SERVO_vidRotateDegree90(void);

void SERVO_vidRotateDegree180(void);

#endif