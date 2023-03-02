#ifndef  _MCAL_ADC_INTERFACE_H_
#define _MCAL_ADC_INTERFACE_H_
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
typedef enum
{
    ADC_CH0 = 0,
    ADC_CH1,
    ADC_CH2,
    ADC_CH3,
    ADC_CH4,
    ADC_CH5,
    ADC_CH6,
    ADC_CH7 
}_enuADCChannels;
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
void ADC_vidInit(_enuADCChannels enuADCChannel);
u16 ADC_u16Read(void);

#endif /* end if */