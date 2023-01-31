#ifndef _MCAL_TIMER2_PRIVATE_H
#define _MCAL_TIMER2_PRIVATE_H

#define TCNT2    *((volatile u8*)0x52)
#define OCR2     *((volatile u8*)0x5C)
#define TCCR2    *((volatile u8*)0x53)
    #define CS20    0
    #define CS21    1
    #define CS22    2
    #define WGM21   3
    #define WGM20   6
#define TIMSK    *((volatile u8*)0x59)
    #define TOIE2   0
    #define OCIE2   1
#define TIFR     *((volatile u8*)0x58)

#endif