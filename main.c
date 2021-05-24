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
	while(1) loop();
}

extern void delay(uint32_t ticks)
{
	while(ticks--);
}


#define TR 1
#if TR

void init(void)
{
	//MatrixInit();
	//ADCInit();
	//UsartInitTransmiter();
	
	//InitLED();
	GyroInit();
}

void loop(void)
{   
	uint8_t stat = GyroReadReg(STATUS_REG);
	uint8_t r1 = GyroReadReg(CTRL_REG1);
	uint8_t r2 = GyroReadReg(CTRL_REG2);
	uint8_t r3 = GyroReadReg(CTRL_REG3);
	uint8_t r4 = GyroReadReg(CTRL_REG4);
	uint8_t r5 = GyroReadReg(CTRL_REG5);
	uint8_t xl = GyroReadReg(OUT_X_L);
	uint8_t xh = GyroReadReg(OUT_X_H);
	uint8_t yl = GyroReadReg(OUT_Y_L);
	uint8_t yh = GyroReadReg(OUT_Y_H);
	uint8_t zl = GyroReadReg(OUT_Z_L);
	uint8_t zh = GyroReadReg(OUT_Z_H);
	uint8_t temp = GyroReadReg(OUT_TEMP);
	uint8_t wham = GyroReadReg(WHO_AM_I);
	
	UsartTransmit(zl);
	UsartTransmit(zh);
	
	uint8_t a = wham;
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
