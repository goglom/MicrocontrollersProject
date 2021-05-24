#include "Matrix.h"

void SPI2_IRQHandler(void);

static uint8_t matrixBuffer1[8] = {0};
static uint8_t matrixBuffer2[8] = {0};
static uint8_t* drawBuffer = matrixBuffer1;
static uint8_t* editBuffer = matrixBuffer2;

void MatrixInit(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; 
	
	// Set Alternate function 1 to pin 
	GPIOB->AFR[1] = 0 << (15 - 8) * 4;
	// Set alternate func 1 (AF0) to pin 13
	GPIOB->AFR[1] = 0 << (13 - 8) * 4;	
	SPI2->CR1 = 
		  SPI_CR1_SSM 
		| SPI_CR1_SSI
		| SPI_CR1_BR
		| SPI_CR1_MSTR
		| SPI_CR1_CPOL
		| SPI_CR1_CPHA;
	
	SPI2->CR2 = SPI_CR2_DS | SPI_CR2_RXNEIE;
	NVIC_EnableIRQ(SPI2_IRQn);
	//NVIC_SetPriority(SPI2_IRQn, 2);
	GPIOB->MODER |= GPIO_MODER_MODER13_1 | GPIO_MODER_MODER15_1;
	GPIOA->MODER |= GPIO_MODER_MODER8_0;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR13_1 | GPIO_PUPDR_PUPDR15_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR8_1;
	SPI2->CR1 |= SPI_CR1_SPE;
	
	SPI2->DR = 0;
}

void SPI2_IRQHandler(void)
{
	static volatile int curRow = 0;
	
	GPIOA->BSRR = GPIO_BSRR_BS_8;
	SPI2->DR = (0x1U << curRow) << 8 | drawBuffer[curRow];
	
	++curRow;
	curRow %= 8;
	volatile uint32_t garbage = SPI2->DR;
	
	GPIOA->BSRR = GPIO_BSRR_BR_8;
}

void DrawNum(uint8_t num, uint8_t shift)
{
	switch (num) {
	case 1:
		editBuffer[4] |= 0b00000100 << shift;
		editBuffer[3] |= 0b00000100 << shift;
		editBuffer[2] |= 0b00000100 << shift;
		editBuffer[1] |= 0b00000100 << shift;
		editBuffer[0] |= 0b00000100 << shift;
		break;
	case 2:
		editBuffer[4] |= 0b00000111 << shift;
		editBuffer[3] |= 0b00000100 << shift;
		editBuffer[2] |= 0b00000111 << shift;
		editBuffer[1] |= 0b00000001 << shift;
		editBuffer[0] |= 0b00000111 << shift;
		break;
	case 3:
		editBuffer[4] |= 0b00000111 << shift;
		editBuffer[3] |= 0b00000100 << shift;
		editBuffer[2] |= 0b00000111 << shift;
		editBuffer[1] |= 0b00000100 << shift;
		editBuffer[0] |= 0b00000111 << shift;
		break;
	case 4:
		editBuffer[4] |= 0b00000101 << shift;
		editBuffer[3] |= 0b00000101 << shift;
		editBuffer[2] |= 0b00000111 << shift;
		editBuffer[1] |= 0b00000100 << shift;
		editBuffer[0] |= 0b00000100 << shift;
		break;
	case 5:
		editBuffer[4] |= 0b00000111 << shift;
		editBuffer[3] |= 0b00000001 << shift;
		editBuffer[2] |= 0b00000111 << shift;
		editBuffer[1] |= 0b00000100 << shift;
		editBuffer[0] |= 0b00000111 << shift;
		break;
	case 6:
		editBuffer[4] |= 0b00000111 << shift;
		editBuffer[3] |= 0b00000001 << shift;
		editBuffer[2] |= 0b00000111 << shift;
		editBuffer[1] |= 0b00000101 << shift;
		editBuffer[0] |= 0b00000111 << shift;
		break;
	case 7:
		editBuffer[4] |= 0b00000111 << shift;
		editBuffer[3] |= 0b00000100 << shift;
		editBuffer[2] |= 0b00000100 << shift;
		editBuffer[1] |= 0b00000100 << shift;
		editBuffer[0] |= 0b00000100 << shift;
		break;
	case 8:
		editBuffer[4] |= 0b00000111 << shift;
		editBuffer[3] |= 0b00000101 << shift;
		editBuffer[2] |= 0b00000111 << shift;
		editBuffer[1] |= 0b00000101 << shift;
		editBuffer[0] |= 0b00000111 << shift;
		break;
	case 9:
		editBuffer[4] |= 0b00000111 << shift;
		editBuffer[3] |= 0b00000101 << shift;
		editBuffer[2] |= 0b00000111 << shift;
		editBuffer[1] |= 0b00000100 << shift;
		editBuffer[0] |= 0b00000111 << shift;
		break;
	case 0:
		editBuffer[4] |= 0b000000111 << shift;
		editBuffer[3] |= 0b000000101 << shift;
		editBuffer[2] |= 0b000000101 << shift;
		editBuffer[1] |= 0b000000101 << shift;
		editBuffer[0] |= 0b000000111 << shift;
		break;
	}
}

void MatrixSwapBuffers(void)
{
	void* tmp = editBuffer;
	editBuffer = drawBuffer;
	drawBuffer = tmp;
}

void MatrixSetRow(int row, uint8_t bits)
{
	editBuffer[row] = bits;
}

void MatrixSetBit(int x, int y)
{
	editBuffer[y] |= 1 << x;
}
void MatrixClear(void)
{
	for (int i = 0; i < 8; ++i) {
		editBuffer[i] = 0;
	}
}

