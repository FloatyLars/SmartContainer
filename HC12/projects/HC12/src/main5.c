/******************************************************************************
 * File           : Main program - Output Compare
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdbool.h>
#include "helper.h"
#include "stdlib.h"
#include <string.h>



// ----------------------------------------------------------------------------
// Global variables READ README FIRST
// ----------------------------------------------------------------------------

#define OWN_ID 12345



#define BUFFER_SIZE 100
#define COMMANDTYPE_DELIMITER '|'
#define CONTAINERID_DELIMITER '^'
#define COMMAND_REQUEST_TO_SEND "RTS"
#define COMMAND_CLEAR_TO_SEND "CTS"
#define COMMAND_DATA "DAT"
#define COMMAND_ACKNOWLEDGE "ACK"
#define COMMAND_GETID "GID"
#define COMMAND_SETID "SID"

USART_TypeDef* USART_HC12 = USART1;
USART_TypeDef* USART_PC = USART2;

extern volatile char *rx_buffer;
extern volatile uint8_t writePos, readPos;
extern volatile bool overflow;
extern volatile bool newMessage;


char commandBuffer[5];

typedef struct packet {
	char commandType[4];
	uint16_t containerId;
	char payload[100];
} packet;


// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------
void HC12_Set_baud(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t baud);
void SetPins(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, FunctionalState NewState);
void readBuffer(char character);


// ----------------------------------------------------------------------------
// Init functions (SEE README FOR CALCULATIONS)
// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
// Other functions (SEE README FOR CALCULATIONS)
// ----------------------------------------------------------------------------

void SetPins(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, FunctionalState NewState) {
	if (NewState) {
		GPIOx->BSRR = (1<<GPIO_Pin);
	} else {
		GPIOx->BSRR = (1<<GPIO_Pin)*0x10000;
	}
}

uint8_t getStrSize(char arr[]) {
	uint8_t i = 0;
	while (arr[i]) {
		i++;
	}
	return i;
}

void HC12_Set_baud(GPIO_TypeDef* Set_Pin_GPIOx, uint16_t Set_Pin_PinNr, uint32_t baud) {
	char fullCommand[] = "AT+B";
	char baudString[7];
	
	SetPins(Set_Pin_GPIOx, Set_Pin_PinNr, DISABLE);
	
	Delay(SystemCoreClock/80);
	USART_itoa(baud, baudString);
	strcat(fullCommand, baudString);
	USART_Putstr(USART_HC12, fullCommand);
	
	USART_Putstr(USART_PC, "Command sent\n");
	
	//USART_DeInit(USART1);
	//USART_HC12_Setup(baud);
	Delay(SystemCoreClock/80);
	
	SetPins(Set_Pin_GPIOx, Set_Pin_PinNr, ENABLE);
}

void HC12_SendMessage(char commandType[], uint16_t containerId, char payload[]) {
	char fullMessage[111];
	char containerIdString[5];
	int i;
	USART_itoa(containerId, containerIdString);
	for (i = 0; i < 3; i++) {
		fullMessage[i] = commandType[i];
	}
	fullMessage[3] = COMMANDTYPE_DELIMITER;
	for (i = 4; i < 9; i++) {
		fullMessage[i] = containerIdString[i-4];
	}
	fullMessage[9] = CONTAINERID_DELIMITER;
	
	if (commandType == COMMAND_DATA && payload != NULL) {
		for (i = 0; i < getStrSize(payload); i++) {
			fullMessage[i+10] = payload[i];
		}
		fullMessage[i+10] = '#';
	} else {
		fullMessage[10] = '#';
	}
	
	USART_Putstr(USART_HC12, fullMessage);
}


	

void addToBuffer(char character) {
	int i;
	for (i = 0; i < 4; i++) {
		commandBuffer[i] = commandBuffer[i+1];
	}
	commandBuffer[4] = character;
}

packet getPacket() {
	int16_t i;
	char containerIdString[6] = {0};
	packet newPacket;
	newPacket.commandType[3] = 0;
	
	while ((readPos != writePos || overflow)) {
		if (rx_buffer[readPos] == COMMANDTYPE_DELIMITER) {
			for (i = 4; i >= 2; i--) {
				newPacket.commandType[i-2] = commandBuffer[i];
			}
		} else if (rx_buffer[readPos] == CONTAINERID_DELIMITER) {
			for (i = 4; i >= 0; i--) {
				containerIdString[i] = commandBuffer[i];
			}
			newPacket.containerId = atoi(containerIdString);
		} else {
			addToBuffer(rx_buffer[readPos]);
		}
		rx_buffer[readPos] = 0;
		readPos++;
		
		if (readPos == BUFFER_SIZE) {
			readPos = 0;
		}
		overflow = false;
	}
	newMessage = false;
	return newPacket;
}


// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(void)
{
	writePos = 0; 
	readPos = 0;
	
	rx_buffer = (char *)malloc(sizeof(char)*BUFFER_SIZE);
	
	USART_HC12_Setup(9600);
	USART_PC_Setup(115200);
	init_GPIOA();
	
	SetPins(GPIOA, 4, ENABLE);
	USART1->CR1 |= USART_CR1_RXNEIE;
	//USART1->CR1 &= ~(USART_CR1_RXNEIE);
	
	while(1)
	{
		packet newPacket;
		USART1->CR1 |= USART_CR1_RXNEIE; // Enable RX USART interrupts
		SCB->SCR &= ~NVIC_LP_SLEEPONEXIT; // Disable sleep on ISR exit
		
		if (newMessage) {
			char test[100] = {0};
			newPacket = getPacket();
			USART_itoa(newPacket.containerId, test);
			//HC12_SendMessage(COMMAND_REQUEST_TO_SEND, OWN_ID, NULL);
			
			HC12_SendMessage(COMMAND_DATA, 65498, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
			//HC12_SendMessage(COMMAND_DATA, 65498, "Hier passen 100 karakters");
			
			SCB->SCR &= ~NVIC_LP_SLEEPONEXIT;
			
//			if (strcmp(newPacket.commandType, COMMAND_REQUEST_TO_SEND) == 0) {
//				if (newPacket.containerId == OWN_ID) {
//					USART_Putstr(USART_PC, "\nRequest to send was for me. ID: ");
//					USART_Putstr(USART_PC, test);
//					// Ga naar ander kanaal
//				} else {
//					USART_Putstr(USART_PC, "\nRequest to send was not for me. ID: ");
//					USART_Putstr(USART_PC, test);
//				}
//			} else if (strcmp(newPacket.commandType, COMMAND_CLEAR_TO_SEND) == 0) {
//				if (newPacket.containerId == OWN_ID) {
//					USART_Putstr(USART_PC, "\nClear to send was for me. ID: ");
//					USART_Putstr(USART_PC, test);
//					// Ga naar ander kanaal
//				} else {
//					USART_Putstr(USART_PC, "\nClear to send was not for me. ID: ");
//					USART_Putstr(USART_PC, test);
//				}
//			} else if (strcmp(newPacket.commandType, COMMAND_ACKNOWLEDGE) == 0) {
//				if (newPacket.containerId == OWN_ID) {
//					USART_Putstr(USART_PC, "\nAcknowledge was for me. ID: ");
//					USART_Putstr(USART_PC, test);
//					// Ga naar ander kanaal
//				} else {
//					USART_Putstr(USART_PC, "\nAcknowledge was not for me. ID: ");
//					USART_Putstr(USART_PC, test);
//				}
//			}
		}
		//PWR_EnterSleepMode(PWR_SLEEPEntry_WFI);
	}
}

