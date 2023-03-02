#ifndef  _MCAL_ADC_PRIVATE_H_
#define _MCAL_ADC_PRIVATE_H_
/* define registers */ 
#define ADMUX      *((volatile u8*)0x27)
    #define MUX0     0
    #define MUX1     1
    #define MUX2     2
    #define MUX3     3
    #define MUX4     4
    #define ADLAR    5
    #define REFS0    6
    #define REFS1    7

#define ADCSRA     *((volatile u8*)0x26)
    #define ADPS0   0
    #define ADPS1   1
    #define ADPS2   2
    #define ADIE    3
    #define ADIF    4
    #define ADATE   5
    #define ADSC    6
    #define ADEN    7

#define ADCH       *((volatile u8*)0x25)
#define ADCL       *((volatile u8*)0x24)
#define DDRA       *((volatile u8*)0x3A)

#define SFIOR      *((volatile u8*)0x50) 
	#define ADTS2  7
	#define ADTS1  6
	#define ADTS0  5
#endif  