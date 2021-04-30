#include "I2C.h"

// Sends data to arduino I2C slave
// USED ONLY FOR TESTING.
void WriteData(uint16_t addr, uint8_t data)
{
  // Wait while I2C peripheral is not ready
  WaitForI2CFlag( I2C_ISR_BUSY );

  // Start I2C write transfer for 1 byte
  I2C_TransferHandling( I2C1, addr, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Write );
  WaitForI2CFlag( I2C_ISR_TXIS );

  // Write data
  I2C_SendData(I2C1, data);
    
  // Wait for- and clear stop condition
  WaitForI2CFlag(I2C_ISR_STOPF);
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
}

void I2C1_Setup(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;

  RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_1);

  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;

  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  I2C_StructInit(&I2C_InitStructure);
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0;
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_OwnAddress1 = 0;
  I2C_InitStructure.I2C_Timing = 0xB0420F13; // =100 kHz. @ 48 MHz (SYSCLK) See Table 93

  I2C_Init(I2C1, &I2C_InitStructure);
  I2C_Cmd(I2C1, ENABLE);
}


void WaitForI2CFlag(uint32_t flag)
{
	//uint32_t I2C_CommStatus = I2C_COMM_OK;
  uint32_t timeout = I2C_TIMEOUT;
  
  if(flag == I2C_ISR_BUSY)
  {
    while(I2C_GetFlagStatus(I2C1, flag) != RESET)
    {
      if(timeout-- == 0)
      {
        //I2C_CommStatus = I2C_COMM_ERROR;
        return;
      }
    }
  }
  else
  {
    while(I2C_GetFlagStatus(I2C1, flag) == RESET)
    {
      if(timeout-- == 0)
      {
        //I2C_CommStatus = I2C_COMM_ERROR;
        return;
      }
    }
  }
}
