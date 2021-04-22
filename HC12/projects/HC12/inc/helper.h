/******************************************************************************
 * Project        : HAN ESE PRJ2, PRJ1V & PRJ1D
 * File           : Helper funcions used throughout all targets
 * Copyright      : 2013 HAN Embedded Systems Engineering
 ******************************************************************************
  Change History:

    Version 1.0 - May 2013
    > Initial revision

******************************************************************************/
#ifndef _HELPER_H_
#define _HELPER_H_

#include "stm32f0xx.h"

// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------
void Delay(const int d);
void USART_PC_Setup(uint32_t USART_BaudRate);
void USART_HC12_Setup(uint32_t USART_BaudRate);
void USART_Putstr(USART_TypeDef* USARTx, char *str);
void USART_Clearscreen(USART_TypeDef* USARTx);
char *USART_itoa(uint16_t i, char *p);
void USART_putc(USART_TypeDef* USARTx, char c);
void init_GPIOA(void);

#endif /* _HELPER_H_ */
