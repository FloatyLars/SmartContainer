#include "liquidlevel.h"
#include "I2C.h"
#include <stdio.h>

int getLevel(void)
{
	uint8_t data[20];
	int i;
	
	// Set all the values to 0.
	for (i = 0; i < 20; i++)
		data[i] = 0;
	
	// read the lowest 8 values from the sensor.
	for (i = 0; i < 8; i++)
		data[i] = readSensor(LOWAD);
	
	// read the highest 12 values from the sensor.
	for (i = 8; i < 20; i++)
		data[i] = readSensor(HIGHAD);
	
	// only go to the next value if the current one is above 100.
	// this way we get the values from the bottom of the sensor going up.
	i = 0;
	while (i < 20 && data[i] > 100)
		i++;
	
	// There are 20 points of measurement so each is 5% (5x20 = 100)
	return i * 5;
}

uint8_t readSensor(uint8_t addr)
{
	uint8_t data;
	WaitForI2CFlag(I2C_ISR_BUSY);

  // Repeated start I2C read transfer for 1 byte
  I2C_TransferHandling(I2C1, addr, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
  WaitForI2CFlag(I2C_ISR_RXNE);
	
  // Read data
	data = I2C_ReceiveData(I2C1);
    
  // Wait for- and clear stop condition
	WaitForI2CFlag(I2C_ISR_STOPF);
  I2C1->ICR = I2C_ICR_STOPCF;
	
	return data;
}
