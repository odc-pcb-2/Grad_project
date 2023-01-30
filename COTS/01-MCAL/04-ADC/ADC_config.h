#ifdef  _MCAL_ADC_CONFIG_H_
#define _MCAL_ADC_CONFIG_H_

/*options: 1 for clk_2
           2 for clk_4
           3 for clk_8
           4 for clk_16
           5 for clk_32
           6 for clk_64
           7 for clk_128
*/
#define ADC_PSC     3        //pre scaler

/*options:  0 for External Aref and Internal Vref Off
            1 for Avcc and External Capacitor at Aref
            3 for Internal Aref 2.56V with External Capacitor at Aref
*/
#define ADC_VREF    2

/*Options:  0 for 8-bits in ADCL and 2-bit bits in ADCH
            1 for 2-bits in ADCL and 8-bit bits in ADCH
*/
#define ADC_STORE   0

#endif