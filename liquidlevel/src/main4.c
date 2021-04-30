/******************************************************************************
 * File           : Main program - I2C 2x16 LCD BTHQ21605V
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "usart.h"
#include "nfc.h"
#include "helper.h"
#include "i2c.h"
#include "liquidlevel.h"
#include <stdio.h>


// ----------------------------------------------------------------------------
// PINS:
// USART: PA9 / PA10
// I2C: PB6 / PB7
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(void)
{
	int temp;
	char output[50];
	
  // Configure LED3 and LED4 on STM32F0-Discovery
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  // Initialize Usaddrer Button on STM32F0-Discovery
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);

  I2C1_Setup();
	
	USART_init();
	USART_clearscreen();
	
  while(1)
  {
		USART_clearscreen();
		USART_putstr("Smart container!\n");
		
		temp = getLevel();
		sprintf(output, "\nWater level: %i\n", temp);
		
		USART_putstr(output);
		
		delay(TIME_DELAY);
  }
}
