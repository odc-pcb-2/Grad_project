#ifndef _MCAL_TIMER0_PRIVATE_H
#define _MCAL_TIMER0_PRIVATE_H
/* define registers */ 
#define TCNT0    *((volatile u8*)0x52)
#define OCR0     *((volatile u8*)0x5C)
#define TCCR0    *((volatile u8*)0x53)
    #define CS00    0
    #define CS01    1
    #define CS02    2
    #define WGM01   3
    #define WGM00   6
	#define FOC0    7
#define TIMSK    *((volatile u8*)0x59)
    #define TOIE0   0
    #define OCIE0   1
#define TIFR     *((volatile u8*)0x58)
	#define TOV0    0
	#define OCF0    1
#define SREG     *((volatile u8*)0x5F)
#define DDRB     *((volatile u8*)0x37)

#endif