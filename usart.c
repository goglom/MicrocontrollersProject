#include "usart.h"

uint8_t USART_toTransmit = 0;

static void GeneralInit()
{
	RCC->APB1ENR|= RCC_APB1ENR_USART3EN;
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIODEN;
	GPIOC->MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1;
	GPIOD->MODER |= GPIO_MODER_MODER2_1;
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR10_1 | GPIO_PUPDR_PUPDR11_1;
	GPIOD->PUPDR |= GPIO_PUPDR_PUPDR2_1;
	
	// Enable RS-485 driver
	USART3->CR3 |= USART_CR3_DEM;
	
	// Set Alternate function 1 to PC10
	GPIOC->AFR[1] |= 1 << (10 - 8) * 4;
	// Set alternate function 1 to PC11
	GPIOC->AFR[1] |= 1 << (11 - 8) * 4;
	// Set Alternate function 1 to PD2
	GPIOD->AFR[0] |= 1 << (2  - 0) * 4;
}

void UsartInitTransmiter(void)
{
	GeneralInit();
	USART3->BRR = 480000 / 96; /* (1) */
	USART3->CR1 = USART_CR1_TE | USART_CR1_UE;/* (2) */
}

void UsartInitReciever(void)
{
	GeneralInit();
	/* (1) oversampling by 16, 9600 baud */
	/* (2) 8 data bit, 1 start bit, 1 stop bit, no parity, reception mode */
	USART3->BRR = 480000 / 96; /* (1) */
	USART3->CR1 = USART_CR1_RE | USART_CR1_UE; /* (2) */
}

bool UsartTransmit(uint8_t data)
{
	if (!(USART3->ISR & USART_ISR_TXE))
		return false;
	
	*(volatile uint8_t*)&(USART3->TDR) = data;
	
	return true;
				
}
bool UsartReceive(uint8_t* data)
{
	if (!(USART3->ISR & USART_ISR_RXNE))
		return false;
	
	*data = *(volatile uint8_t*)&(USART3->RDR);
	return true;
}
