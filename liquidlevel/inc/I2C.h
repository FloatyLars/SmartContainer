#ifndef _I2C_H_
#define _I2C_H_

#include "stm32f0xx.h"

#define I2C_COMM_OK            (0x0)
#define I2C_COMM_ERROR         (0x1)
#define I2C_TIMEOUT            ((uint32_t)(0x00001000))

void I2C1_Setup(void);
void WaitForI2CFlag(uint32_t flag);

#endif // _I2C_H_
