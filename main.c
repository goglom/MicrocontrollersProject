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
void DMA1_Channel1_IRQHandler(void);

int main(void)
{
	init();
	while(1) loop();
}

extern void delay(uint32_t ticks)
{
	while(ticks--);
}


#define TR 1
#if TR

#define DELTA_T 0.02f
#define SCALE_CONST 0.0007f  // For 2000 dps

void init(void)
{
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

void GyroUpdate()
{
	uint8_t stat = GyroReadReg(STATUS_REG);
	uint8_t zl = GyroReadReg(OUT_Z_L);
	uint8_t zh = GyroReadReg(OUT_Z_H);
	uint16_t z_raw = zl | ((uint16_t)zh << 8);
	int16_t zVelocity = *(int16_t*)&z_raw;
	
	if (abs(zVelocity) > 50)
		zAngle += SCALE_CONST * zVelocity * DELTA_T;
	
	timerFlag = false;
}

void loop(void)
{   
	if (timerFlag)
		GyroUpdate();
	
	int16_t angle	= zAngle * 10;
	float angleF = zAngle * 10;
	
	angle %= 360;
	
	if (angle < 0)
		angle = 360 + angle;
	
	uint8_t sector = (uint16_t)angle * 8 / 360;

	
	UsartTransmit(sector);
}

#else

void init(void)
{
	MatrixInit();
	UsartInitReciever();
}

void loop(void)
{    
	uint8_t sector = UsartReceive();
	
	MatrixClear();
	
	DrawNum(sector, 0);
	
	if (sector > 7)
		DrawNum(0, 4); // ERROR
	
	MatrixSwapBuffers();

	delay(1024);
}

#endif
