/* include library needed */
#include "Types.h"
#include "Bit_Math.h"

#include "DIO_interface.h"

#include "UART_interface.h"
#include "UART_config.h"
#include "UART_private.h"
/* define function needed */
void (*CB_TX)(void);
void (*CB_RX)(void);
void (*CB_UDR)(void);
/* function that initialize usart protocol */
void USART_vidInit(u32 copy_u8BaudRate)
{
    u16 Local_UBRR;
    /* Adjust Mode */
    CLR_BIT(USCRC,URSEL);
    #if   USART_MODE == 0
        // SET_BIT(USCRC,UMSEL);
        CLR_BIT(USCRC,UMSEL);
        Local_UBRR = (8000000/(16*copy_u8BaudRate))-1;
    #elif USART_MODE == 2
        CLR_BIT(USCRC,UMSEL);
        SET_BIT(USCRA,U2X);
        Local_UBRR = (8000000/(8*copy_u8BaudRate))-1;
    #else
        SET_BIT(USCRC,UMSEL);
        Local_UBRR = (8000000/(2*copy_u8BaudRate))-1;
    #endif

    /* Adjust Word Size */
    #if   USART_WORDSIZE == 0
        CLR_BIT(USCRC,UCSZ0);
        CLR_BIT(USCRC,UCSZ1);
        CLR_BIT(USCRB,UCSZ2);
    #elif USART_WORDSIZE == 1
        SET_BIT(USCRC,UCSZ0);
        CLR_BIT(USCRC,UCSZ1);
        CLR_BIT(USCRB,UCSZ2);
    #elif USART_WORDSIZE == 2
        CLR_BIT(USCRC,UCSZ0);
        SET_BIT(USCRC,UCSZ1);
        CLR_BIT(USCRB,UCSZ2);
    #elif USART_WORDSIZE == 3
        SET_BIT(USCRC,UCSZ0);
        SET_BIT(USCRC,UCSZ1);
        CLR_BIT(USCRB,UCSZ2);
    #else
        SET_BIT(USCRC,UCSZ0);
        SET_BIT(USCRC,UCSZ1);
        SET_BIT(USCRB,UCSZ2);
    #endif

    /* Adjust Parity */
    #if   USART_PARITY == 0
        CLR_BIT(USCRC,UPM0);
        CLR_BIT(USCRC,UPM1);
    #elif USART_PARITY == 1
        CLR_BIT(USCRC,UPM0);
        SET_BIT(USCRC,UPM1);
    #else
        SET_BIT(USCRC,UPM0);
        SET_BIT(USCRC,UPM1);
    #endif

    /* Adjust Stop Bit */
    #if USART_STOPBIT == 0
        CLR_BIT(USCRC,USBS);
    #else
        SET_BIT(USCRC,USBS);
    #endif

    /* Enable TX and RX */
    SET_BIT(USCRB,TXEN);
    SET_BIT(USCRB,RXEN);

    /* Adjust UBRR */
    SET_BIT(USCRC,URSEL);
    UBRRH = (u8)(Local_UBRR >> 8);
    UBRRL = (u8)(Local_UBRR);
}
/* function that send one byte using usart protocol*/
void USART_vidSendByte(u8 copy_u8DataSend)
{
    while(GET_BIT(USCRA,UDRE)==0);
    UDR = copy_u8DataSend;
}
/* function that recieve one byte using usart protocol*/
u8 USART_u8RecieveByte(void)
{
    while(GET_BIT(USCRA,RXC)==0);
    return UDR;   
}
/* function that enable tx interrupt after finish transmiting */
void USART_vidEnableTXInterrupt(void)
{
    SET_BIT(USCRB,TXCIE);   
}
/* function that enable rx interrupt after finish recievieng */
void USART_vidEnableRXInterrupt(void)
{
    SET_BIT(USCRB,RXCIE);
}
/* function that enable udre interrupt */
void USART_vidEnableUDREInterrupt(void)
{
    SET_BIT(USCRB,UDRIE);
}
/* function that set call back when transimiting */
void USART_vidSetCBTX(void(*copy_CB)(void))
{
    CB_TX = copy_CB;
}
/* function that set call back when recieving */
void USART_vidSetCBRX(void(*copy_CB)(void))
{
    CB_RX = copy_CB;
}
/* function that set call back when using udre */
void USART_vidSetCBUDRE(void(*copy_CB)(void))
{
    CB_UDR = copy_CB;
}
/* function that send string using usart protocol */
void USART_vidSendString(u8 *copy_ptrString)
{
    while((*copy_ptrString) != '\0')
    {
        USART_vidSendByte(*copy_ptrString);
        copy_ptrString++;
    }
}
/* function that send block of data using usart protocol */
void USART_vidSendu32Block(u32 copy_u32DataSend)
{
    u8 Data;
    u8 counter;
    for(counter = 0; counter <= 3; counter++)
    {
        Data = (u8)(copy_u32DataSend >> (8 * counter));
        USART_vidSendByte(Data);
    }
}
/* function that recieve block of data using usart protocol */
u8 USART_u32Recieveu32Block(void)
{
    u32 Data;
    u8 counter;
    for(counter = 0; counter <=3; counter++)
    {
       Data |= (USART_u8RecieveByte() << (8 * counter));     
    }   
    return Data;
}

