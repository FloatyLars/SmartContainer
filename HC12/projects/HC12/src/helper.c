/******************************************************************************
 * File           : Helper funcions used throughout all targets
******************************************************************************/
#include "helper.h"



void USART_HC12_Setup(uint32_t BaudRate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  
  // --------------------------------------------------------------------------
  // Initialize USART1
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  // Setup Tx- and Rx pin
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_Init(USART1, &USART_InitStructure);
   
   // USART1 interrupts enable in NVIC
  NVIC_EnableIRQ(USART1_IRQn);
  NVIC_SetPriority(USART1_IRQn, 0);
  NVIC_ClearPendingIRQ(USART1_IRQn);

  USART_Cmd(USART1, ENABLE);
}

void USART_PC_Setup(uint32_t BaudRate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  
  // --------------------------------------------------------------------------
  // Initialize USART1
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  
  // Setup Tx- and Rx pin
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource14, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_1);
  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_Init(USART2, &USART_InitStructure);
	
  USART_Cmd(USART2, ENABLE);
}

void USART_Putstr(USART_TypeDef* USARTx, char *str)
{
  while(*str)
  {
    if(*str == '\n')
    {
      while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){;}
      USART_SendData(USARTx, '\r');
    }
    
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){;}
    USART_SendData(USARTx, *str++);
  }
}

void USART_putc(USART_TypeDef* USARTx, char c)
{
  // Wait for Transmit data register empty
  while((USARTx->ISR & USART_ISR_TXE) == 0) ;

  // Transmit data by writing to TDR, clears TXE flag  
  USARTx->TDR = c;
}


void USART_Clearscreen(USART_TypeDef* USARTx)
{
  char cmd1[5] = {0x1B, '[', '2', 'J', '\0'}; // Clear screen
  char cmd2[4] = {0x1B, '[', 'f', '\0'}; // Cursor home
  
  USART_Putstr(USARTx, cmd1);
  USART_Putstr(USARTx, cmd2);
}


char *USART_itoa(uint16_t i, char *p)
{
  int16_t t1, t2;
  char h[10];

  t1 = t2 = 0;

  if (i < 0)
  {
    p[t2] = '-';
    t2++;
    i = -i;
  }

  //if(i<1000){ p[t2++]='0'; }
  //if(i< 100){ p[t2++]='0'; }
  //if(i<  10){ p[t2++]='0'; }
  
  do
  {
    h[t1] = i % 10 + 48;
    t1++;
    i = i / 10;

  }while (i > 0);

  while (t1 > 0)
  {
    p[t2++] = h[--t1];
  }

  p[t2] = '\0';

  return(p);
}

void init_GPIOA(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin			= GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType		= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}



#pragma push
#pragma O3
void Delay(const int d)
{
  volatile int i;

  for(i=d; i>0; i--){ ; }

  return;
}
#pragma pop
