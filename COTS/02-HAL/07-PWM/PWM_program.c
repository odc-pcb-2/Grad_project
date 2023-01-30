#include "Types.h"
#include "Bit_Math.h"

#include "TIMER0_interface.h"

#include "DIO_interface.h"

#include "PWM_interface.h"
#include "PWM_private.h"
#include "PWM_config.h"

void PWM_vidGenerate(u8 u8DutyCucle)
{
	// Make OC0 output pin
	DIO_vidSetPinMode(GPIOB, PIN3, OUTPUT);
	u8 u8OCRValue;
	u8OCRValue = (u8)(((256 * u8DutyCucle)/100)-1);
	_strTimmer0Configuration strTimmer0Configuration = {FAST_PWM, NON_INVERTING_PWM, TOV_INTERRUPT_DISABLE};
	TMR0_vidInit(&strTimmer0Configuration);
	TMR0_vidSetOCR0Value(u8OCRValue);
	TMR0_vidStart(CLK_1024);
}

