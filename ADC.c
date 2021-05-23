#include "ADC.h"

void DMA1_Channel1_IRQHandler(void);
static uint16_t ADC_Buffer[ADC_BF_SIZE] = {0};

uint16_t ADC_Val = 0;
bool ADC_DMA_Ready = false;

void ADCInit(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->AHBENR |= RCC_AHBENR_DMA1EN; 
	
	ADC1->CFGR1 |= ADC_CFGR1_CONT | ADC_CFGR1_OVRMOD |  ADC_CFGR1_DMAEN 
			| ADC_CFGR1_DMACFG | ADC_CFGR1_RES_0; 
	ADC1->CHSELR |= ADC_CHSELR_CHSEL1;
	
	ADC1->CR |= ADC_CR_ADCAL;
	while (ADC1->CR & ADC_CR_ADCAL);
	ADC1->CR |= ADC_CR_ADEN;
	while (!(ADC1->ISR & ADC_ISR_ADRDY));
		
	DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR)); 
	DMA1_Channel1->CMAR = (uint32_t)(ADC_Buffer);
	DMA1_Channel1->CNDTR = ADC_BF_SIZE;
	DMA1_Channel1->CCR |= DMA_CCR_MINC | DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0
		 | DMA_CCR_CIRC | DMA_CCR_TCIE;
	DMA1_Channel1->CCR |= DMA_CCR_EN;
	
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	NVIC_SetPriority(DMA1_Channel1_IRQn,0);

	ADC1->CR |= ADC_CR_ADSTART;
}

void DMA1_Channel1_IRQHandler(void)
{
	ADC_DMA_Ready = true;
	DMA1->IFCR |= DMA_IFCR_CTCIF1;
}
	
void ADC_Update()
{
	uint32_t sum = 0;
	for (int i = 0; i < ADC_BF_SIZE; ++i)
		sum += ADC_Buffer[i];
		
	ADC_Val = sum / ADC_BF_SIZE;
	ADC_DMA_Ready = false;
}
