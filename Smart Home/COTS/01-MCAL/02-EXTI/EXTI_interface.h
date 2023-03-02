#ifndef _MCAL_EXTI_INTERFACE_H_
#define _MCAL_EXTI_INTERFACE_H_
/* define interrupt pins */
typedef enum
{
    INT0_PIN = 0,
    INT1_PIN,
    INT2_PIN
}_enuSOURCE_OF_INTERUPT;

typedef enum
{
    LOW_LEVEL = 0,
    LOGICAL_CHANGE,
    FALLING_EDGE,
    RISING_EDGE
}_enuFIRING_SOURCE;

void EXTI_vidInit(_enuSOURCE_OF_INTERUPT enuInterruptNum, _enuFIRING_SOURCE enuFiringSource);

void EXTI_vidSetCB(_enuSOURCE_OF_INTERUPT enuInterruptNum, void (*PtrFunCB)(void));       //CB--> callBack

#endif