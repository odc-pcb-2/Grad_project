#ifndef _MCAL_TIMER2_PRIVATE_H
#define _MCAL_TIMER2_PRIVATE_H
/* define registers */ 
#define TCNT2    *((volatile u8*)0x52)
#define OCR2     *((volatile u8*)0x5C)
#define TCCR2    *((volatile u8*)0x53)
    #define CS20    0
    #define CS21    1
    #define CS22    2
    #define WGM21   3
    #define WGM20   6
	#define FOC2    7
#define TIMSK    *((volatile u8*)0x59)
    #define TOIE2   0
    #define OCIE2   1
#define TIFR     *((volatile u8*)0x58)
    #define TOV2    6
	#define OCF2    7
// Global Interrupt Register
#define SREG     *((volatile u8*)0x5F)
// OC0 Data Direction Register
#define DDRD     *((volatile u8*)0x31)

#define ASSR     *((volatile u8*)0x42)
	#define AS2     3
#endif