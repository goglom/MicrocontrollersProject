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
#define SCALE_CONST 0.07f  // For 2000 dps

void init(void)
{
	GyroInit();
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
	
	if (abs(zVelocity) < 20)
		zVelocity = 0;
	
	zAngle += SCALE_CONST * zVelocity * DELTA_T;
	
	timerFlag = false;
}

void loop(void)
{   
	if (timerFlag)
		GyroUpdate();
	
	float angle	= zAngle;
}

#else

void init(void)
{
	MatrixInit();
	UsartInitReciever();
}

void loop(void)
{    
	uint8_t zh = UsartReceive();
	uint8_t zl = UsartReceive();
	
	uint16_t z = zl | (uint16_t)((uint16_t)zh << 8);
	int z_num = z * 99 / 0xffff;
	
	MatrixClear();
	
	DrawNum(z_num / 10, 0);
	DrawNum(z_num % 10, 4);
	
	
	MatrixSwapBuffers();
	
}

#endif
