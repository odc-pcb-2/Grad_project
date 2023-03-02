#ifndef _MCAL_UART_INTERFACE_H
#define _MCAL_UART_INTERFACE_H
		/************************************************************************/
		/*                       declare functions needed	                    */
		/************************************************************************/
void USART_vidInit(u32 copy_u8BaudRate);

void USART_vidSendByte(u8 copy_u8DataSend);

u8 USART_u8RecieveByte(void);

void USART_vidEnableTXInterrupt(void);

void USART_vidEnableRXInterrupt(void);

void USART_vidEnableUDREInterrupt(void);

void USART_vidSetCBTX(void(*copy_CB)(void));

void USART_vidSetCBRX(void(*copy_CB)(void));

void USART_vidSetCBUDRE(void(*copy_CB)(void));

void USART_vidSendString(u8 *copy_ptrString);

void USART_vidSendu32Block(u32 copy_u32DataSend);

u8 USART_u32Recieveu32Block(void);

#endif