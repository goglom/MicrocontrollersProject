#include <stm32f0xx.h>
#include <stdbool.h>
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
	InitLED();
	while(1) loop();
}

static void delay(uint32_t ticks)
{
	while(ticks--);
}


#define TR 1
#if TR

void init(void)
{
	//MatrixInit();
	//ADCInit();
	GyroInit();
}

void loop(void)
{   
	uint8_t b = GyroReadReg(CTRL_REG1);
	uint8_t a = GyroReadReg(WHO_AM_I);
	if (a == 0b11010100)
		LEDswitchLight(RED_L, 1);
	else
		LEDswitchLight(ALL_L, 0);
	
	delay(1024);
}

#else

void init(void)
{
	MatrixInit();
	UsartInitReciever();
}

void loop(void)
{    
	uint8_t data = UsartReceive();
	MatrixClear();
	MatrixSetRow(0, data);
	
	MatrixSwapBuffers();
}

#endif
