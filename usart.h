#ifndef __USART_H__
#define __USART_H__

#include <stm32f0xx.h>

void UsartInitTransmiter(void);
void UsartInitReciever(void);
void UsartTransmit(uint8_t data);
uint8_t UsartReceive(void);

#endif //__USART_H__
