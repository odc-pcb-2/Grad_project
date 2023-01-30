#ifndef _MCAL_TIMER0_INTERFACE_H_ 
#define _MCAL_TIMER0_INTERFACE_H_

typedef enum
{
    NO_CLK = 0,
    CLK_MC,
    CLK_8,
    CLK_64,
    CLK_256,
    CLK_1024,
    FALING_EDGE,
    RISING_EDGE
}_enuTimer0CLKSource;

typedef enum
{
    NORMALL_MODE = 0,
    PHASE_CORRECT_MODE,
    CTC_MODE,
    FAST_PWM
}_enuTimer0Mode;

typedef enum
{
    TOV_INTERRUPT_ENABLE = 0,
    TOV_INTERRUPT_DISABLE,
}_enuTimer0TOVInterrupt;

typedef enum
{
    OCF_INTERRUPT_ENABLE = 0,
    OCF_INTERRUPT_DISABLE,
}_enuTimer0OCFInterrupt;

typedef enum
{
    NO_PWM = 0,
    NON_INVERTING_PWM = 2,
    NON_INVERTING_PWM
}_enuTimer0PWMMode;

typedef struct 
{
    _enuTimer0Mode enuTimer0Mode;
    _enuTimer0PWMMode enuTimer0PWMMode;
    _enuTimer0TOVInterrupt enuTimer0TOVInterrupt;   
    _enuTimer0OCFInterrupt enuTimer0OCFInterrupt;   
}_strTimmer0Configuration;

void TMR0_vidInit(_strTimmer0Configuration *ptr_strtimerConfig);
void TMR0_vidStart(_enuTimer0CLKSource enuClkSource);
void TMR0_vidSetOCR0Value(u8 u8Value);
void TMR0_vidSetTOV0CB(void (*ptr_TOV0CB)(void));
void TMR0_vidSetOCF0CB(void (*ptr_OCF0CB)(void));


#endif