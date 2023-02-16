#ifndef _MCAL_TIMER0_PRIVATE_H
#define _MCAL_TIMER0_PRIVATE_H

#define TCNT0    *((volatile u8*)0x52)
#define OCR0     *((volatile u8*)0x5C)
#define TCCR0    *((volatile u8*)0x53)
    #define CS00    0
    #define CS01    1
    #define CS02    2
    #define WGM01   3
    #define WGM00   6
#define TIMSK    *((volatile u8*)0x59)
    #define TOIE0   0
    #define OCIE0   1
#define TIFR     *((volatile u8*)0x58)

#endif