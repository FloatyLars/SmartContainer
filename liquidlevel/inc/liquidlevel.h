#ifndef _liquidlevel_H_
#define _liquidlevel_H_

#include "stm32f0xx.h"

// Both adrasses are bitshifted 1 to left to make room for the r/w bit
#define HIGHAD 0x78 << 1
#define LOWAD 0x77 << 1


int getLevel(void);
uint8_t readSensor(uint8_t addr);

#endif
