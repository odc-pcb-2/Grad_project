#include "Types.h"
#include "Bit_Math.h"

#include "TIMER2_interface.h"
#include "TIMER2_private.h"
#include "TIMER2_config.h"

#include "AVR/interrupt.h"

void (*LOCPtr_TOV2CB)(void);
void (*LOCPtr_OCF2CB)(void);

void TMR2_vidInit(_strTimmer2Configuration *ptr_strtimerConfig)
{
    switch (ptr_strtimerConfig->enuTimer2Mode)
    {
    case NORMALL_MODE:
        CLR_BIT(TCCR2, WGM20);
        CLR_BIT(TCCR2, WGM21);
        break;
    case PHASE_CORRECT_MODE:
        SET_BIT(TCCR2, WGM20);
        CLR_BIT(TCCR2, WGM21);
        TCCR2 &= 0xCF;
        TCCR2 |= ptr_strtimerConfig->enuTimer2PWMMode;
        break;
    case CTC_MODE:
        CLR_BIT(TCCR2, WGM20);
        SET_BIT(TCCR2, WGM21);
        break;
    case FAST_PWM:
        SET_BIT(TCCR2, WGM20);
        SET_BIT(TCCR2, WGM21);
        TCCR2 &= 0xCF;
        TCCR2 |= ptr_strtimerConfig->enuTimer2PWMMode;
        break;
    default:
        break;
    }
    if(ptr_strtimerConfig->enuRTC == NO_RTC)
    {
        CLR_BIT(ASSR, AS2);
    }
    else
    {
        SET_BIT(ASSR, AS2);
    }
    if(ptr_strtimerConfig->enuTimer2TOVInterrupt == TOV_INTERRUPT_ENABLE)
    {
        SET_BIT(TIMSK, TOIE2);
    }
    else
    {
        CLR_BIT(TIMSK, TOIE2);
    }
    if(ptr_strtimerConfig->enuTimer2OCFInterrupt == OCF_INTERRUPT_ENABLE)
    {
        SET_BIT(TIMSK, OCIE2);
    }
    else
    {
        CLR_BIT(TIMSK, OCIE2);
    }
}

void TMR2_vidStart(_enuTimer2CLKSource enuClkSource)
{
    TCCR2 &= 0xF8;          // To Clear CS00, CS01 and CS02
    TCCR2 |= enuClkSource;  // To write Clock value In CS00, CS01 and CS02
}

void TMR2_vidSetOCR0Value(u8 u8Value)
{
    OCR2 = u8Value;
}

void TMR2_vidSetTOV2CB(void (*ptr_TOV2CB)(void))
{
    LOCPtr_TOV2CB = ptr_TOV2CB;
}

void TMR2_vidSetOCF2CB(void (*ptr_OCF2CB)(void))
{
    LOCPtr_OCF2CB = ptr_OCF2CB;
}

ISR(TIMER2_OV_vect)
{
    (*LOCPtr_TOV2CB)();
}

ISR(TIMER2_COMP_vect)
{
    (*LOCPtr_OCF2CB)();
}

