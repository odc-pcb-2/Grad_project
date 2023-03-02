/* include library needed */
#include "Types.h"
#include "Bit_Math.h"

#include "TIMER1_interface.h"
#include "TIMER1_config.h"
#include "TIMER1_private.h"
/* defined values needed */
#define NULLPTR   ((void*)0)
/* define functions needed */
static void (*Timer1_OVF_Fptr) (void)=NULLPTR;
static void (*Timer1_OCA_Fptr) (void)=NULLPTR;
static void (*Timer1_OCB_Fptr) (void)=NULLPTR;
static void (*Timer1_ICU_Fptr) (void)=NULLPTR;

/* function that initialize timer1 */
void Timer1_vidInit(u8 Timer1_Mode, u8 Timer1_Scaler, u8 Timer1_OCR1AMode, u8 Timer1_OCR1BMode)
{
	switch (Timer1_Mode)
	{
		case TIMER1_NORMAL_MODE:
			CLR_BIT(TCCR1A,WGM10);
			CLR_BIT(TCCR1A,WGM11);
			CLR_BIT(TCCR1B,WGM12);
			CLR_BIT(TCCR1B,WGM13);
			break;
		case TIMER1_CTC_ICR_TOP_MODE:
			CLR_BIT(TCCR1A,WGM10);
			CLR_BIT(TCCR1A,WGM11);
			SET_BIT(TCCR1B,WGM12);
			SET_BIT(TCCR1B,WGM13);
			break;
		
		case TIMER1_CTC_OCRA_TOP_MODE:
			CLR_BIT(TCCR1A,WGM10);
			CLR_BIT(TCCR1A,WGM11);
			SET_BIT(TCCR1B,WGM12);
			CLR_BIT(TCCR1B,WGM13);
			break;
		case TIMER1_FASTPWM_ICR_TOP_MODE:
			CLR_BIT(TCCR1A,WGM10);
			SET_BIT(TCCR1A,WGM11);
			SET_BIT(TCCR1B,WGM12);
			SET_BIT(TCCR1B,WGM13);
			break;
		case TIMER1_FASTPWM_OCRA_TOP_MODE:
			SET_BIT(TCCR1A,WGM10);
			SET_BIT(TCCR1A,WGM11);
			SET_BIT(TCCR1B,WGM12);
			SET_BIT(TCCR1B,WGM13);
			break;
	}
	switch (Timer1_OCR1AMode)
	{
		case OCRA_DISCONNECTED:
			CLR_BIT(TCCR1A,COM1A0);
			CLR_BIT(TCCR1A,COM1A1);
			break;
		case OCRA_TOGGLE:
			SET_BIT(TCCR1A,COM1A0);
			CLR_BIT(TCCR1A,COM1A1);
			break;
		case OCRA_NON_INVERTING:
			CLR_BIT(TCCR1A,COM1A0);
			SET_BIT(TCCR1A,COM1A1);
			break;
		case OCRA_INVERTING:
			SET_BIT(TCCR1A,COM1A0);
			SET_BIT(TCCR1A,COM1A1);
			break;
	}
	switch (Timer1_OCR1BMode)
	{
		case OCRB_DISCONNECTED:
			CLR_BIT(TCCR1A,COM1B0);
			CLR_BIT(TCCR1A,COM1B1);
			break;
		case OCRB_TOGGLE:
			SET_BIT(TCCR1A,COM1B0);
			CLR_BIT(TCCR1A,COM1B1);
			break;
		case OCRB_NON_INVERTING:
			CLR_BIT(TCCR1A,COM1B0);
			SET_BIT(TCCR1A,COM1B1);
			break;
		case OCRB_INVERTING:
			SET_BIT(TCCR1A,COM1B0);
			SET_BIT(TCCR1A,COM1B1);
			break;
	}
	TCCR1B&=0XF8;
	TCCR1B|=Timer1_Scaler;
}
/* function that control input status */
void Timer1_vidInputCaptureEdge(u8 ICU_Edge)
{
	if(ICU_Edge==ICU_RISING)
	{
		SET_BIT(TCCR1B,ICES1);
	}
	else if(ICU_Edge==ICU_FALLING)
	{
		CLR_BIT(TCCR1B,ICES1);
	}
	
}
/* function that return icr flag */
u16 Timer1_u8ReturnICU(void)
{
	return ICR1;
}
/* function that enable ovf interrupt */
void Timer1_vidOVFInterruptEnable(void)
{
	SET_BIT(TIMSK,TOIE1);
}
/* function that disable ovf interrupt */
void Timer1_vidOVFInterruptDisable(void)
{
	CLR_BIT(TIMSK,TOIE1);
}
/* function that enable ocra interrupt */
void Timer1_vidOCR1AInterruptEnable(void)
{
	SET_BIT(TIMSK,OCIE1A);
}
/* function that disable ocra interrupt */
void Timer1_vidOCR1AInterruptDisable(void)
{
	CLR_BIT(TIMSK,OCIE1A);
}
/* function that enable ocrb interrupt */
void Timer1_vidOCR1BInterruptEnable(void)
{
	SET_BIT(TIMSK,OCIE1B);
}
/* function that disable ocrb interrupt */
void Timer1_vidOCR1BInterruptDisable(void)
{
	CLR_BIT(TIMSK,OCIE1B);
}
/* function that enable icu interrupt */
void Timer1_vidICUInterruptEnable(void)
{
	SET_BIT(TIMSK,TICIE1);
}
/* function that disable icu interrupt */
void Timer1_vidICUInterruptDisable(void)
{
	CLR_BIT(TIMSK,TICIE1);
}
/* function that set call back for ovf */
void Timer1_vidOVFSetCallBack(void(*LocalFptr)(void))
{
	Timer1_OVF_Fptr=LocalFptr;
}
/* function that set call back for ocra */
void Timer1_vidOCR1ASetCallBack(void(*LocalFptr)(void))
{
	Timer1_OCA_Fptr=LocalFptr;
}
/* function that set call back for ocrb */
void Timer1_vidOCR1BSetCallBack(void(*LocalFptr)(void))
{
	Timer1_OCB_Fptr=LocalFptr;
}
/* function that set call back for icu */
void Timer1_vidICUSetCallBack(void(*LocalFptr)(void))
{
	Timer1_ICU_Fptr=LocalFptr;
}

/*********************************Timer 1 ISR functions*********************************************/
/* function call when ovf set */
ISR(TIMER1_OVF_vect)
{
	if(Timer1_OVF_Fptr!=NULLPTR)
	{
		Timer1_OVF_Fptr();
	}
}
/* function call when ocra set */
ISR(TIMER1_OCA_vect)
{
	if(Timer1_OCA_Fptr!=NULLPTR)
	{
		Timer1_OCA_Fptr();
	}
}
/* function call when ocrb set */
ISR(TIMER1_OCB_vect)
{
	if(Timer1_OCB_Fptr!=NULLPTR)
	{
		Timer1_OCB_Fptr();
	}
}
/* function call when icu set */
ISR(TIMER1_ICU_vect)
{
	if(Timer1_ICU_Fptr!=NULLPTR)
	{
		Timer1_ICU_Fptr();
	}
}
