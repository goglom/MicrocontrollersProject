#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stm32f0xx.h>
#include <stdbool.h>

void MatrixInit(void);
void SwichReady(bool state);
void MatrixSetBit(int x, int y);
void MatrixSetRow(int row, uint8_t bits);
void MatrixClear(void);
void MatrixSwapBuffers(void);
void DrawNum(uint8_t num, uint8_t shift);

#endif
