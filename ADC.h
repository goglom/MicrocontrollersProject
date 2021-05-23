#ifndef __ADC_H__
#define __ADC_H__

#include <stm32f0xx.h>
#include <stdbool.h>


#define ADC_BF_SIZE 32
extern uint16_t ADC_Val;
extern bool ADC_DMA_Ready;

void ADCInit(void);
void ADC_Update(void);

#endif