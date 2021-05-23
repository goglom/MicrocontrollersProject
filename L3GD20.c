#include "L3GD20.h"

static void __SPIInit(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; 
	
	GPIOB->AFR[1] = 0 << (15 - 8) * 4; // Clocking 
	GPIOB->AFR[1] = 0 << (14 - 8) * 4; // (SDO) Input
	GPIOB->AFR[1] = 0 << (13 - 8) * 4; // (SDI) Output
	GPIOB->AFR[1] = 0 << (12 - 8) * 4; // Ебать жмыхнуло, братан
	
	//GPIOB->OSPEEDR = 0xffffffff;
	
	SPI2->CR1 = 
		  SPI_CR1_SSM 
		| SPI_CR1_SSI
		| SPI_CR1_BR_1
		| SPI_CR1_MSTR
		| SPI_CR1_CPOL
		| SPI_CR1_CPHA;
	
	//SPI2->CR2 = SPI_CR2_DS;
	SPI2->CR2 |= SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0;
	//SPI2->CR2 |=SPI_CR2_FRXTH;//FIFO на 8 бит
	
	GPIOB->MODER |= GPIO_MODER_MODER13_1 | GPIO_MODER_MODER14_1 | GPIO_MODER_MODER15_1;
	GPIOC->MODER |= GPIO_MODER_MODER0_0;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR13_1 | GPIO_PUPDR_PUPDR14_1 | GPIO_PUPDR_PUPDR15_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_1;
	
	GPIOC->BSRR = GPIO_BSRR_BS_0;
	SPI2->CR1 |= SPI_CR1_SPE;
}

void GyroWriteReg(uint8_t addr, uint8_t data)
{
	uint8_t address = 0b00111111 | addr;
	
	
	GPIOC->BSRR = GPIO_BSRR_BR_0;

	GPIOC->BSRR = GPIO_BSRR_BS_0;
}

uint16_t GyroReadReg(uint8_t addr)
{
	uint16_t packet = 0;
	packet |= addr | 0x1U << 7;
	
	while (!(SPI2->SR & SPI_SR_TXE));
	GPIOC->BSRR = GPIO_BSRR_BR_0;
	SPI2->DR = packet;
	
	while (!(SPI2->SR & SPI_SR_RXNE));
	GPIOC->BSRR = GPIO_BSRR_BS_0;
	
	uint16_t result =0;
	result = (uint16_t)SPI2->DR;
	return result;
}

uint8_t SendData(uint8_t data)
{
	while ((SPI2->SR & SPI_SR_BSY));
	SPI1->DR = data;
	while ((SPI2->SR & SPI_SR_BSY));
	return (uint8_t)SPI2->DR;
}

void GyroInit(void)
{
	__SPIInit();
	CTRL_REG1;
	GPIOC->BSRR|=GPIO_BSRR_BR_0;//устанавливаем низкий уровень на CS, разрешаем работу гироскопа
	SendData(0x20);//обращаемся к регистру CTRL_REG1
	SendData(0x0F);//Отправляем 0b00001111=0x0F, то есть включаем гироскоп
	SendData(0x23);
	SendData(0x30);
	GPIOC->BSRR|=GPIO_BSRR_BS_0;//отключаем CS
	
	//uint8_t ctrl1 = 0;
	//ctrl1 |= CTRL_REG1_PD;
	//uint8_t ctrl5 = 0;
	//ctrl5 |= CTRL_REG5_FIFO_EN;
	
	//GyroWriteReg(CTRL_REG5, ctrl5);
	//GyroWriteReg(CTRL_REG1, ctrl1);
}