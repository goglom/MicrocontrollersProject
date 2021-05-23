#include <stm32f0xx.h>
#include <stdbool.h>
#include "Matrix.h"
#include "ADC.h"
#include "usart.h"
#include "L3GD20.h"

void init(void);
void loop(void);
void DMA1_Channel1_IRQHandler(void);

int main(void)
{
	init();
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
	//if (ADC_DMA_Ready)
	//	ADC_Update();
	
	//uint8_t data = ADC_Val * 256 / 1024;
	//UsartTransmit(data);
	
	//MatrixClear();
	//MatrixSetRow(0, data);
	//MatrixSwapBuffers();
	
	GPIOE->BSRR|=GPIO_BSRR_BR_3;//включаем передачу (CS)
	SendData(WHO_AM_I); //отправляем адрес регистра OUT_X с запросом на чтение и инкремент адреса
	uint8_t iamwho = SendData(0x0f);//считываем данные регистра OUT_X
	
	GPIOE->BSRR|=GPIO_BSRR_BS_3;//выключаем передачу (CS)
	
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
