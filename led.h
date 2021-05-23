#ifndef __LED_H__
#define __LED_H__

#include <stm32f0xx.h>

extern const uint32_t RED_L;
extern const uint32_t BLUE_L;
extern const uint32_t GREEN_L;
extern const uint32_t YELLOW_L;
extern const uint32_t ALL_L;

void InitLED(void);
void LEDswitchLight(uint32_t color, uint8_t mode);

#endif
