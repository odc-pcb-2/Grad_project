#ifndef _MCAL_TIMER2_INTERFACE_H_ 
#define _MCAL_TIMER2_INTERFACE_H_

typedef enum
{
    NO_CLK = 0,
    CLK_MC,
    CLK_8,
    CLK_64,
    CLK_128,
    CLK_256,
    CLK_1024
}_enuTimer2CLKSource;

typedef enum
{
    NORMALL_MODE = 0,
    PHASE_CORRECT_MODE,
    CTC_MODE,
    FAST_PWM
}_enuTimer2Mode;

typedef enum
{
    TOV_INTERRUPT_ENABLE = 0,
    TOV_INTERRUPT_DISABLE,
}_enuTimer2TOVInterrupt;

typedef enum
{
    OCF_INTERRUPT_ENABLE = 0,
    OCF_INTERRUPT_DISABLE,
}_enuTimer2OCFInterrupt;

typedef enum
{
    NO_PWM = 0,
    NON_INVERTING_PWM = 2,
    INVERTING_PWM
}_enuTimer2PWMMode;

typedef enum
{
    NO_RTC = 0,
    RTC_ENABLE
}_enuRTC;

typedef struct 
{
    _enuTimer2Mode enuTimer2Mode;
    _enuTimer2PWMMode enuTimer2PWMMode;
    _enuRTC enuRTC;
    _enuTimer2TOVInterrupt enuTimer2TOVInterrupt;   
    _enuTimer2OCFInterrupt enuTimer2OCFInterrupt;   
}_strTimmer2Configuration;

void TMR2_vidInit(_strTimmer2Configuration *ptr_strtimerConfig);
void TMR2_vidStart(_enuTimer2CLKSource enuClkSource);
void TMR2_vidSetOCR0Value(u8 u8Value);
void TMR2_vidSetTOV2CB(void (*ptr_TOV2CB)(void));
void TMR2_vidSetOCF2CB(void (*ptr_OCF2CB)(void));

#endif