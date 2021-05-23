#ifndef __L3GD20_H__
#define __L3GD20_H__

#include <stm32f0xx.h>

#define WHO_AM_I 0x0FU
#define CTRL_REG1 0x20U
#define CTRL_REG2 0x21U
#define CTRL_REG3 0x22U
#define CTRL_REG4 0x23U
#define CTRL_REG5 0x24U
#define REFERENCE 0x25U
#define STATUS_REG 0x27U
#define OUT_Z_L 0x2CU
#define OUT_Z_H 0x2DU

#define CTRL_REG1_PD 0b00001000
#define CTRL_REG1_Xen 0b00000010
#define CTRL_REG1_Yen 0b00000001
#define CTRL_REG1_Zen 0b00000100

#define CTRL_REG5_FIFO_EN 0b01000000



void GyroInit(void);
void GyroWriteReg(uint8_t addr, uint8_t data);
uint16_t GyroReadReg(uint8_t addr);
uint8_t SendData(uint8_t data);

#endif