#include "Types.h"
#include "Bit_Math.h"

#include "DIO_interface.h"

#include "ADC_interface.h"
#include "ADC_private.h"
#include "ADC_config.h"

void ADC_vidInit(_enuADCChannels enuADCChannel)
{
    ADMUX &= 0xE0;
    switch (enuADCChannel)
    {
    case ADC_CH0:
        DIO_vidSetPinMode(GPIOA, PIN0, INPUT);
        ADMUX |= ADC_CH0;
        break;
    case ADC_CH1:
        DIO_vidSetPinMode(GPIOA, PIN1, INPUT);
        ADMUX |= ADC_CH1;
        break;
    case ADC_CH2:
        DIO_vidSetPinMode(GPIOA, PIN2, INPUT);
        ADMUX |= ADC_CH2;
        break;
    case ADC_CH3:
        DIO_vidSetPinMode(GPIOA, PIN3, INPUT);
        ADMUX |= ADC_CH3;
        break;
    case ADC_CH4:
        DIO_vidSetPinMode(GPIOA, PIN4, INPUT);
        ADMUX |= ADC_CH4;
        break;
    case ADC_CH5:
        DIO_vidSetPinMode(GPIOA, PIN5, INPUT);
        ADMUX |= ADC_CH5;
        break;
    case ADC_CH6:
        DIO_vidSetPinMode(GPIOA, PIN6, INPUT);
        ADMUX |= ADC_CH6;
        break;
    case ADC_CH7:
        DIO_vidSetPinMode(GPIOA, PIN7, INPUT);
        ADMUX |= ADC_CH7;
        break;
    default:
        break;
    }
    ADCSRA &= 0xF8;
    ADCSRA |= ADC_PSC;
    /*#if     ADC_PSC == 0          if i need to use set & clear 
            ADCSR = 
    #elif   ADC_PSC == 1
    #elif   ADC_PSC == 2
    #elif   ADC_PSC == 3
    #elif   ADC_PSC == 4
    #elif   ADC_PSC == 5
    #else
    #endif*/
    ADMUX  &= 0xF8;
    ADMUX  |= (ADC_VREF << 6);

    #if ADC_STORE == 0
        CLR_BIT(ADMUX, ADLAR);
    #else
        SET_BIT(ADMUX, ADLAR);
    #endif

    SET_BIT(ADCSRA, ADATE);
    SET_BIT(ADCSRA, ADEN);
}

u16 ADC_u16Read(void)
{
    u16 u16ReturnedValue;
    SET_BIT(ADCSRA, ADSC);
    while(GET_BIT(ADSRA, ADIF) == 0);
    SET_BIT(ADCSRA, ADIF);
    u16ReturnedValue  = (u16)ADCL;
    u16ReturnedValue |= (u16)(ADCL<<8);
    return u16ReturnedValue;
}

