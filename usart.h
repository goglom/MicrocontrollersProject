#ifndef __USART_H__
#define __USART_H__

#include <stm32f0xx.h>
#include <stdbool.h>

extern uint8_t USART_toTransmit;

void UsartInitTransmiter(void);
void UsartInitReciever(void);
bool UsartTransmit(uint8_t data);
bool UsartReceive(uint8_t* data);

#endif //__USART_H__
