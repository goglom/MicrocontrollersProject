#include "led.h"

const uint32_t RED_L = 0x1U;
const uint32_t GREEN_L = RED_L << 1;
const uint32_t BLUE_L = GREEN_L << 1;
const uint32_t YELLOW_L = BLUE_L << 1;
const uint32_t ALL_L = RED_L | GREEN_L | BLUE_L | YELLOW_L;

void InitLED(void)
{
	// PC<6 - 9> - LED
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOAEN;
	// Set output mode for Port C12
	GPIOC->MODER |=	  GPIO_MODER_MODER12_0
					| GPIO_MODER_MODER6_0
					| GPIO_MODER_MODER7_0
					| GPIO_MODER_MODER8_0
					| GPIO_MODER_MODER9_0;
	
	GPIOA->MODER |= GPIO_MODER_MODER15_0;
	GPIOA->MODER &= ~GPIO_MODER_MODER4;
	GPIOA->MODER &= ~GPIO_MODER_MODER5;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_1 | GPIO_PUPDR_PUPDR5_1;
}

void LEDswitchLight(uint32_t color, uint8_t mode)
{
	uint32_t mask = 0x0;

	if (color & RED_L)
		mask |= GPIO_BSRR_BS_6;
	if (color & GREEN_L)
		mask |= GPIO_BSRR_BS_9;
	if (color & BLUE_L)
		mask |= GPIO_BSRR_BS_7;
	if (color & YELLOW_L)
		mask |= GPIO_BSRR_BS_8;
	
	if (!mode)
		mask <<= 16;
	
	GPIOC->BSRR = mask;
}
