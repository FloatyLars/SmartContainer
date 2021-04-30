/******************************************************************************
 * File           : Main program - Low power modes sleep example
******************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------
extern volatile uint32_t bounce_count;

// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------
void Delay(const int d);

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  // Configure LED3 and LED4 on STM32F0-Discovery
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  // Connect USER button to EXTI line with interrupt generation capability
  // Enable the BUTTON clock
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // Configure button pin as input
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  // Connect Button EXTI line to Button GPIO Pin
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
  
  // Configure Button EXTI line
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  // Configure interrupt
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 

  while(1)
  {
    // Enter sleep mode and Wait For Interrupt
    PWR_EnterSleepMode(PWR_SLEEPEntry_WFI);
    
    
    
    // Toggle LED3
    STM_EVAL_LEDToggle(LED3);
    
    // Delay ~100 ms for switch to debounce
    Delay(SystemCoreClock/8/10);

    // Use the debugger to see the value of this counter
    bounce_count = 0;
  }
}
