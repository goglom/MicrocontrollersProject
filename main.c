#include <stm32f0xx.h>
#include <stdbool.h>
#include <math.h>

#include "Matrix.h"
#include "ADC.h"
#include "usart.h"
#include "L3GD20.h"
#include "led.h"

void init(void);
void loop(void);

int main(void)
{
	init();
	while(1) loop();
}

static void delay(uint32_t ticks)
{
	while(ticks--);
}

void setRightArrow(void)
{
	MatrixSetBit(4, 3);
	MatrixSetBit(5, 3);
	MatrixSetBit(6, 3);
	MatrixSetBit(7, 3);
	MatrixSetBit(6, 2);
	MatrixSetBit(6, 4);
}

void setUpRightArrow(void)
{
	MatrixSetBit(4, 3);
	MatrixSetBit(5, 4);
	MatrixSetBit(6, 5);
	MatrixSetBit(7, 6);
	MatrixSetBit(6, 6);
	MatrixSetBit(7, 5);
}

void setUpArrow(void)
{
	MatrixSetBit(4, 3);
	MatrixSetBit(4, 4);
	MatrixSetBit(4, 5);
	MatrixSetBit(4, 6);
	MatrixSetBit(3, 5);
	MatrixSetBit(5, 5);
}

void setUpLeftArrow(void)
{
	MatrixSetBit(4, 3);
	MatrixSetBit(3, 4);
	MatrixSetBit(2, 5);
	MatrixSetBit(1, 6);
	MatrixSetBit(1, 5);
	MatrixSetBit(2, 6);
}

void setLeftArrow(void)
{
	MatrixSetBit(4, 3);
	MatrixSetBit(3, 3);
	MatrixSetBit(2, 3);
	MatrixSetBit(1, 3);
	MatrixSetBit(2, 2);
	MatrixSetBit(2, 4);
}

void setDownLeftArrow(void)
{
	MatrixSetBit(4, 3);
	MatrixSetBit(3, 2);
	MatrixSetBit(2, 1);
	MatrixSetBit(1, 0);
	MatrixSetBit(2, 0);
	MatrixSetBit(1, 1);
}

void setDownArrow(void)
{
	MatrixSetBit(4, 3);
	MatrixSetBit(4, 2);
	MatrixSetBit(4, 1);
	MatrixSetBit(4, 0);
	MatrixSetBit(3, 1);
	MatrixSetBit(5, 1);
}

void setDownRightArrow(void)
{
	MatrixSetBit(4, 3);
	MatrixSetBit(5, 2);
	MatrixSetBit(6, 1);
	MatrixSetBit(7, 0);
	MatrixSetBit(6, 0);
	MatrixSetBit(7, 1);
}

static void (*arrows[8])(void) = {
	setLeftArrow,
	setDownLeftArrow,
	setDownArrow,
	setDownRightArrow,
	setRightArrow,
	setUpRightArrow,
	setUpArrow,
	setUpLeftArrow
};

void setSector(int sector)
{
	arrows[sector]();
}

#define TR 0
#if TR

#define DELTA_T 0.02f
#define SCALE_CONST 0.07f  // For 2000 dps
#define FORM_FACTOR 0.068f

void init(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER8_0;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR8_1;
	GPIOA->BSRR = GPIO_BSRR_BR_8;
	
	GyroInit();
	UsartInitTransmiter();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 160000);
}

float zAngle = 0.0;

bool timerFlag = false;

void SysTick_Handler(void)
{
	timerFlag = true;
}

static void GyroUpdate(void)
{
	uint8_t zl = GyroReadReg(OUT_Z_L);
	uint8_t zh = GyroReadReg(OUT_Z_H);
	uint16_t z_raw = zl | ((uint16_t)zh << 8);
	int16_t zVelocity = *(int16_t*)&z_raw;
	
	if (abs(zVelocity) > 50)
		zAngle += FORM_FACTOR * SCALE_CONST * zVelocity * DELTA_T;
	
	timerFlag = false;
}

void loop(void)
{   
	if (timerFlag)
	{
		GyroUpdate();
	
		int16_t angle = (zAngle + 22.5f);
		angle %= 360;
		
		if (angle < 0)
			angle = 360 + angle;
		
		uint8_t sector = (uint16_t)angle * 8 / 360;
		UsartTransmit(sector);
	}
}

#else

void init(void)
{
	MatrixInit();
	UsartInitReciever();
}

void loop(void)
{    
	uint8_t sector = 0;
	
	// Blocking whine not reciece data
	while (!UsartReceive(&sector));
	
	MatrixClear();
	setSector(sector);
	MatrixSwapBuffers();
}

#endif
