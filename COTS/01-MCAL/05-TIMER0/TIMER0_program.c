#include "Types.h"
#include "Bit_Math.h"

#include "TIMER0_interface.h"
#include "TIMER0_private.h"
#include "TIMER0_config.h"

#include "AVR/interrupt.h"

void (*LOCPtr_TOV0CB)(void);
void (*LOCPtr_OCF0CB)(void);

void TMR0_vidInit(_strTimmer0Configuration *ptr_strtimerConfig)
{
    switch (ptr_strtimerConfig->enuTimer0Mode)
    {
    case NORMALL_MODE:
        CLR_BIT(TCCR0, WGM00);
        CLR_BIT(TCCR0, WGM01);
        break;
    case PHASE_CORRECT_MODE:
        SET_BIT(TCCR0, WGM00);
        CLR_BIT(TCCR0, WGM01);
        TCCR0 &= 0xCF;
        TCCR0 |= ptr_strtimerConfig->enuTimer0PWMMode;
        break;
    case CTC_MODE:
        CLR_BIT(TCCR0, WGM00);
        SET_BIT(TCCR0, WGM01);
        break;
    case FAST_PWM:
        SET_BIT(TCCR0, WGM00);
        SET_BIT(TCCR0, WGM01);
        TCCR0 &= 0xCF;
        TCCR0 |= ptr_strtimerConfig->enuTimer0PWMMode;
        break;
    default:
        break;
    }
    if(ptr_strtimerConfig->enuTimer0TOVInterrupt == TOV_INTERRUPT_ENABLE)
    {
        SET_BIT(TIMSK, TOIE0);
    }
    else
    {
        CLR_BIT(TIMSK, TOIE0);
    }
    if(ptr_strtimerConfig->enuTimer0OCFInterrupt == OCF_INTERRUPT_ENABLE)
    {
        SET_BIT(TIMSK, OCIE0);
    }
    else
    {
        CLR_BIT(TIMSK, OCIE0);
    }
}

void TMR0_vidStart(_enuTimer0CLKSource enuClkSource)
{
    TCCR0 &= 0xF8;          // To Clear CS00, CS01 and CS02
    TCCR0 |= enuClkSource;  // To write Clock value In CS00, CS01 and CS02
}

void TMR0_vidSetOCR0Value(u8 u8Value)
{
    OCR0 = u8Value;
}

void TMR0_vidSetTOV0CB(void (*ptr_TOV0CB)(void))
{
    LOCPtr_TOV0CB = ptr_TOV0CB;
}

void TMR0_vidSetOCF0CB(void (*ptr_OCF0CB)(void))
{
    LOCPtr_OCF0CB = ptr_OCF0CB;
}

ISR(TIMER0_OV_vect)
{
    (*LOCPtr_TOV0CB)();
}

ISR(TIMER0_COMP_vect)
{
    (*LOCPtr_OCF0CB)();
}

