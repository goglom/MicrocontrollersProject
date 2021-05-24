#include "L3GD20.h"

static void __SPIInit(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; 
	
	GPIOB->MODER |= GPIO_MODER_MODER13_1 | GPIO_MODER_MODER14_1 | GPIO_MODER_MODER15_1;
	GPIOC->MODER |= GPIO_MODER_MODER0_0;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR13_1 | GPIO_PUPDR_PUPDR14_1 | GPIO_PUPDR_PUPDR15_1;
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR0_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR8_1;
	
	GPIOB->OSPEEDR = 0xffffffff;
	
	GPIOB->AFR[1] = 0 << (15 - 8) * 4; // Clocking 
	GPIOB->AFR[1] = 0 << (14 - 8) * 4; // (SDO) Input
	GPIOB->AFR[1] = 0 << (13 - 8) * 4; // (SDI) Output
	
	SPI2->CR1 = 
		  SPI_CR1_SSM 
		| SPI_CR1_SSI
		| SPI_CR1_BR
		| SPI_CR1_MSTR
		| SPI_CR1_CPOL
		| SPI_CR1_CPHA
		;
	
	//SPI2->CR2 = SPI_CR2_DS;
	//SPI2->CR2 |= SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0 ;//FIFO на 8 бит
	SPI2->CR2 = 
		  SPI_CR2_DS_2 
		| SPI_CR2_DS_1 
		| SPI_CR2_DS_0 
		| SPI_CR2_FRXTH
		; 
	
	GPIOC->BSRR = GPIO_BSRR_BS_0;
	SPI2->CR1 |= SPI_CR1_SPE;
}

void GyroWriteReg(uint8_t addr, uint8_t data)
{
	uint8_t rightAddr = /*0b00111111 &*/ addr;
	
	GPIOC->BSRR = GPIO_BSRR_BR_0;
	SendData(rightAddr);
	SendData(data);
	GPIOC->BSRR = GPIO_BSRR_BS_0;
}

uint8_t GyroReadReg(uint8_t addr)
{
	uint8_t rightAddr = 0b10000000;
	rightAddr |= 0b00111111 & addr;

	GPIOC->BSRR = GPIO_BSRR_BR_0;
	SendData(rightAddr);
	uint8_t res = SendData(0xff);
	GPIOC->BSRR = GPIO_BSRR_BS_0;
	
	return res;
}

uint8_t SendData(uint8_t data)
{
	while (!(SPI2->SR & SPI_SR_TXE));
	*(volatile uint8_t *)&SPI2->DR = data;
	while (!(SPI2->SR & SPI_SR_RXNE));
	return *(volatile uint8_t *)&(SPI2->DR);
}

void GyroInit(void)
{
	__SPIInit();
	GyroWriteReg(CTRL_REG4, CTRL_REG4_FS0 | CTRL_REG4_FS1);
	GyroWriteReg(CTRL_REG1, CTRL_REG1_PD | CTRL_REG1_Zen);
}