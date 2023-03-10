		/************************************************************************/
		/*                       include library needed		                    */
		/************************************************************************/
#include "Types.h"
#include "Bit_Math.h"
#include "DIO_interface.h"
#include "EXTI_private.h"
#include "EXTI_config.h"
#include "EXTI_interface.h"
#include "avr/interrupt.h"
		/************************************************************************/
		/*                       declare functions needed	                    */
		/************************************************************************/
void (*LOC_PtrFunCBINT0)(void);
void (*LOC_PtrFunCBINT1)(void);
void (*LOC_PtrFunCBINT2)(void);
		/************************************************************************/
		/*Function Name: EXTI_vidInit											*/
		/*Function Input: two input (interrupt pin, firing source)				*/
		/*Function Description: void function take two input and return void
								initialize external interrupt					*/
		/************************************************************************/
void EXTI_vidInit(_enuSOURCE_OF_INTERUPT enuInterruptNum, _enuFIRING_SOURCE enuFiringSource)
{
    //1-Enable Global Interrupt
    SET_BIT(SREG, GI);
    switch (enuInterruptNum)
    {
    case INT0_PIN:
            //2-set pin mode as i/p
            DIO_vidSetPinMode(GPIOD, PIN2, INPUT);
            //3- clear bit
            CLR_BIT(MCUCR, ISC00);
            CLR_BIT(MCUCR, ISC01);
            //4- select firing source
            if(enuFiringSource == LOGICAL_CHANGE)
            {
                SET_BIT(MCUCR, ISC00);
            }
            else if (enuFiringSource == FALLING_EDGE)
            {
                SET_BIT(MCUCR, ISC01);
            }
            else if (enuFiringSource == RISING_EDGE)
            {
                SET_BIT(MCUCR, ISC00);
                SET_BIT(MCUCR, ISC01);
            }
            else
            {
            }
            //5-enable interrupt
            SET_BIT(GICR, INT0);
        break;
    
    case INT1_PIN:
            //2-set pin mode as i/p
            DIO_vidSetPinMode(GPIOD, PIN3, INPUT);
            //3- clear bit
            CLR_BIT(MCUCR, ISC10);
            CLR_BIT(MCUCR, ISC11);
            //4- select firing source
            if(enuFiringSource == LOGICAL_CHANGE)
            {
                SET_BIT(MCUCR, ISC10);
            }
            else if (enuFiringSource == FALLING_EDGE)
            {
                SET_BIT(MCUCR, ISC11);
            }
            else if (enuFiringSource == RISING_EDGE)
            {
                SET_BIT(MCUCR, ISC10);
                SET_BIT(MCUCR, ISC11);
            }
            else
            {
            }
            //5-enable interrupt
            SET_BIT(GICR, INT1);
        break;
        case INT2_PIN:
            //2-set pin mode as i/p
            DIO_vidSetPinMode(GPIOB, PIN2, INPUT);
            //4- select firing source
            if (enuFiringSource == FALLING_EDGE)
            {
                CLR_BIT(MCUCSR, ISC2);
            }
            else if (enuFiringSource == RISING_EDGE)
            {
                SET_BIT(MCUCSR, ISC2);
            }
            else
            {
            }
            //5-enable interrupt
            SET_BIT(GICR, INT2);
        break;
    default:
        break;
    }
}
		/************************************************************************/
		/*Function Name: EXTI_vidSetCB											*/
		/*Function Input: two input (interrupt pin, function)					*/
		/*Function Description: void function take two input and return void
								call back function when interrupt				*/
		/************************************************************************/
void EXTI_vidSetCB(_enuSOURCE_OF_INTERUPT enuInterruptNum, void (*PtrFunCB)(void))
{
    if(enuInterruptNum == INT0_PIN)
    {
        LOC_PtrFunCBINT0 = PtrFunCB;
    }
    else if(enuInterruptNum == INT1_PIN)
    {
        LOC_PtrFunCBINT1 = PtrFunCB;
    }
    else
    {
        LOC_PtrFunCBINT2 = PtrFunCB;
    }
}
		/************************************************************************/
		/*Function Name: ISR													*/
		/*Function Input: one input (function)									*/
		/*Function Description: function that call when using interrupt pin zero*/
		/************************************************************************/
ISR(INT0_vect)
{
    (*LOC_PtrFunCBINT0)();
}
		/************************************************************************/
		/*Function Name: ISR													*/
		/*Function Input: one input (function)									*/
		/*Function Description: function that call when using interrupt pin one	*/
		/************************************************************************/
ISR(INT1_vect)
{
    (*LOC_PtrFunCBINT1)();
}
		/************************************************************************/
		/*Function Name: ISR													*/
		/*Function Input: one input (function)									*/
		/*Function Description: function that call when using interrupt pin two	*/
		/************************************************************************/
ISR(INT2_vect)
{
    (*LOC_PtrFunCBINT2)(); 
}

