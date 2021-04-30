// ----------------------------------------------------------------------------
// Include other files
// ----------------------------------------------------------------------------
#include "nfc.h"
// ----------------------------------------------------------------------------
// Function declarations:
// ----------------------------------------------------------------------------
void ST25DV16_Open_Security_Session(void);
void ST25DV16_Disable_FTM(void);
void ST25DV16_Enable_FTM(void);
uint16_t ST25DV16_NDEF_Header(uint16_t eeprom_addr);
void ST25DV16_NDEF_Record(uint16_t eeprom_addr,uint16_t container_id,uint8_t vloeistoflvl);
void ST25DV16_Senddata2(uint16_t eeprom_addr,uint8_t data);
void ST25DV16_Close_Security_Session(void);
void ST25DV16_WaitForI2CFlag(uint32_t flag);


extern void delay(const int d);

// ----------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------
#define ST25DV16_ADDR				(0x2D)				//I2C standaard adres ST25DV16
#define ST25DV16_USER_ADDR	((0x53)<<1)		//0xA6
#define ST25DV16_SYS_ADDR		((0x57)<<1)		//0xAE

#define ST25DV16_COMM_OK (0x0)		//	ST25DV16 communication status information
#define ST25DV16_COMM_ERROR (0x1)		//	ST25DV16 communication status information
#define ST25DV16_TIMEOUT            ((uint32_t)(0x00001000))
/**		Maximum timeout value for waiting loops
	*		This timeout is not based on an accurate timing value. It just guarantess
	*		that the program will not get stuck in an endless loop whenever there is an
	*		error in the I2C communication
	*/

#define NDEF_TNF							0xD2 //0b 11010010 ||dec 210	||letter: Ò
#define NDEF_TypeLength				0x09 //0b 00001001 ||dec 9		||		||pas deze aan bij aanpassing NDEF_PayloadType
#define NDEF_PayloadLength		0x1E//0x15 //0b 00010101 ||dec 21		||
#define NDEF_PayloadType			'R','T','D','_','T','E','X','T' //8 bytes totaal		||		||pas deze aan bij aanpassing NDEF_TypeLength
/*PAYLOAD TYPE = "text/plain", typelength = 10, TNF = 0xD1 - 0b11010001*/
#define headerlength 11	//AANPASSEN BIJ AANPASSING PAYLOAD TYPE

#define NDEF_Payload1 'I','D',':',' '
#define NDEF_Payload2 0x0D,'L','e','v','e','l',':',' '
#define recordlength 16
// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------
uint32_t ST25DV16_CommStatus = ST25DV16_COMM_OK;


void ST25DV16_Open_Security_Session(void){
	int temp;
	
  // Wait while I2C peripheral is not ready
  ST25DV16_WaitForI2CFlag( I2C_ISR_BUSY );

  // Start I2C write transfer for 3 bytes
  I2C_TransferHandling( I2C1, ST25DV16_SYS_ADDR, 19, I2C_AutoEnd_Mode, I2C_Generate_Start_Write );
	ST25DV16_WaitForI2CFlag( I2C_ISR_TXIS );
	
	
  // Write eeprom address
  I2C_SendData(I2C1, 0x09);		// Address High byte
  ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
  I2C_SendData(I2C1, 0x00);		// Address Low byte
  ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);

  // Stuur wachtwoord voor validatie
	for(temp =0;temp<8;temp++){
		I2C_SendData(I2C1, 0x00);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);		//delay(SystemCoreClock / 8 / 200);
	}
	
	//Stuur validatie code
	I2C_SendData(I2C1, 0x09);	// validation code
  ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
	
	// Stuur wachtwoord opnieuw
	for(temp =0;temp<8;temp++){
		I2C_SendData(I2C1, 0x00);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
	}
    
  // Wait for- and clear stop condition
  ST25DV16_WaitForI2CFlag(I2C_ISR_STOPF);
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
}


void ST25DV16_Disable_FTM(void){
	
	// Wait while I2C peripheral is not ready
  ST25DV16_WaitForI2CFlag( I2C_ISR_BUSY );

  // Start I2C write transfer for 3 bytes
  I2C_TransferHandling( I2C1, ST25DV16_SYS_ADDR, 3, I2C_AutoEnd_Mode, I2C_Generate_Start_Write );
	ST25DV16_WaitForI2CFlag( I2C_ISR_TXIS );	//delay(SystemCoreClock / 8 / 200);

	
  // Write eeprom address
  I2C_SendData(I2C1, 0x00);		// Address High byte
  ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
  I2C_SendData(I2C1, 0x0D);		// Address Low byte
  ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
	
	// Schakel Fast Transfer Mode uit
	I2C_SendData(I2C1, 0x00);	// validation code
  ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
	
  // Wait for- and clear stop condition
  ST25DV16_WaitForI2CFlag(I2C_ISR_STOPF);
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
}

void ST25DV16_Enable_FTM(void){
	
	// Wait while I2C peripheral is not ready
  ST25DV16_WaitForI2CFlag( I2C_ISR_BUSY );

  // Start I2C write transfer for 3 bytes
  I2C_TransferHandling( I2C1, ST25DV16_SYS_ADDR, 3, I2C_AutoEnd_Mode, I2C_Generate_Start_Write );
	ST25DV16_WaitForI2CFlag( I2C_ISR_TXIS );	//delay(SystemCoreClock / 8 / 200);

	
  // Write eeprom address
  I2C_SendData(I2C1, 0x00);		// Address High byte
  ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
  I2C_SendData(I2C1, 0x0D);		// Address Low byte
  ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
	
	// Schakel Fast Transfer Mode uit
	I2C_SendData(I2C1, 0x01);	// validation code
  ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
	
  // Wait for- and clear stop condition
  ST25DV16_WaitForI2CFlag(I2C_ISR_STOPF);
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
}

//ALLEEN DEZE NODIG
void ST25DV16_NDEF_Record(uint16_t eeprom_addr,uint16_t container_id,uint8_t vloeistoflvl){
	
	
	char header[headerlength] =	{	
		NDEF_TNF,
		NDEF_TypeLength,
		NDEF_PayloadLength,
		NDEF_PayloadType
	};

//	uint8_t id_highbyte = (container_id>>8);
//	const uint8_t id_lowbyte = (container_id & 0x00FF);
//	const uint8_t level = vloeistoflvl;
	
	char writestring[25];
	sprintf(writestring,"ID: %d | Level: %d%%",container_id,vloeistoflvl);
	
	stuurdata(eeprom_addr,header,headerlength);
	eeprom_addr += headerlength;
	stuurdata(eeprom_addr,writestring,(strlen(writestring)));
	
}

void ST25DV16_Senddata2(uint16_t eeprom_addr,uint8_t data){
	
	char payloadtype_letter;
	char *writestring = "RTD_TEXT"; // 10 characters, tel deze bij aantal_bytes op!!
	int i;
	uint8_t char_amount = strlen(writestring);
	
	// Wait while I2C peripheral is not ready
  ST25DV16_WaitForI2CFlag( I2C_ISR_BUSY );

  // Start I2C write transfer for 3 bytes
  I2C_TransferHandling( I2C1, ST25DV16_USER_ADDR, (26+8), I2C_AutoEnd_Mode, I2C_Generate_Start_Write );
	ST25DV16_WaitForI2CFlag( I2C_ISR_TXIS );	//delay(SystemCoreClock / 8 / 200);

	
	
  // Write eeprom address
  I2C_SendData(I2C1, eeprom_addr >>8 );		// Address High byte
  ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
  I2C_SendData(I2C1, eeprom_addr & 0x00FF);		// Address Low byte
  ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
	
	// Stuur NDEF header
	// Stuur TNF + flags
		I2C_SendData(I2C1, 0xD3);//0b 11010011
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
	// Stuur Type length
		I2C_SendData(I2C1, 0x08);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
	// Stuur Payload length
		I2C_SendData(I2C1, 0x15);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
	// Stuur Payload Type
		for(i=0; i < 8;i++){
			payloadtype_letter = writestring[i];
			I2C_SendData(I2C1, payloadtype_letter);
			ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		}
		
	
	// Stuur NDEF data
	// Stuur "ID: "
		I2C_SendData(I2C1, 0x49);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		I2C_SendData(I2C1, 0x44);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		I2C_SendData(I2C1, 0x3A);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		I2C_SendData(I2C1, 0x20);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);

	// Stuur ID in getallen, 5 cijfers breed
		I2C_SendData(I2C1, 0x00);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		I2C_SendData(I2C1, 0x00);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		I2C_SendData(I2C1, 0x00);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		I2C_SendData(I2C1, 0x00);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		I2C_SendData(I2C1, 0x0F);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);

	// Stuur een enter (0x0D)
		I2C_SendData(I2C1, 0x0D);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);

	// Stuur "Level: "
		I2C_SendData(I2C1, 0x4C); //L
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		I2C_SendData(I2C1, 0x65); //e
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		I2C_SendData(I2C1, 0x76); //v
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		I2C_SendData(I2C1, 0x65); //e
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		I2C_SendData(I2C1, 0x6C); //l
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		I2C_SendData(I2C1, 0x3A); //:
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		I2C_SendData(I2C1, 0x20); //" "
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);

	// Stuur vloeistof niveau in getallen, 3 cijfers breed
		I2C_SendData(I2C1, 0x01);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		I2C_SendData(I2C1, 0x00);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		I2C_SendData(I2C1, 0x00);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);

	// Stuur "%"
		I2C_SendData(I2C1, 0xD1);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
	
	
	// Wait for- and clear stop condition
  ST25DV16_WaitForI2CFlag(I2C_ISR_STOPF);
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
}
/**/
void ST25DV16_Close_Security_Session(void){
	int temp;
	
  // Wait while I2C peripheral is not ready
  ST25DV16_WaitForI2CFlag( I2C_ISR_BUSY );

  // Start I2C write transfer for 3 bytes
  I2C_TransferHandling( I2C1, ST25DV16_SYS_ADDR, 19, I2C_AutoEnd_Mode, I2C_Generate_Start_Write );
	ST25DV16_WaitForI2CFlag( I2C_ISR_TXIS );
	
  // Write eeprom address
  I2C_SendData(I2C1, 0x09);		// Address High byte
  ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
  I2C_SendData(I2C1, 0x00);		// Address Low byte
  ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);

	
  // Stuur wachtwoord voor validatie
	for(temp =0;temp<8;temp++){
		if(temp<7){
			I2C_SendData(I2C1, 0x01);
		}else{
			I2C_SendData(I2C1, 0x01);
		}		
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
	}
	
	//Stuur validatie code
	I2C_SendData(I2C1, 0x09);	// validation code
  ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
	
	// Stuur wachtwoord opnieuw
	for(temp =0;temp<8;temp++){
		if(temp<7){
			I2C_SendData(I2C1, 0x01);
		}else{
			I2C_SendData(I2C1, 0x01);
		}
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
	}
	
  // Wait for- and clear stop condition
  ST25DV16_WaitForI2CFlag(I2C_ISR_STOPF);
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
}
/**/

void stuurdata(uint16_t eeprom_addr,char data[],int datalength){
	int temp;
	char letter;
	for(temp =0;temp < datalength;temp++){
		letter = data[temp];
		
		// Wait while I2C peripheral is not ready
		ST25DV16_WaitForI2CFlag( I2C_ISR_BUSY );

		// Start I2C write transfer for 3 bytes
		I2C_TransferHandling( I2C1, ST25DV16_USER_ADDR, 3, I2C_AutoEnd_Mode, I2C_Generate_Start_Write );
		ST25DV16_WaitForI2CFlag( I2C_ISR_TXIS );
		
		// Write eeprom address
		I2C_SendData(I2C1, eeprom_addr >>8 );
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		I2C_SendData(I2C1, eeprom_addr & 0x00FF);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		
		// Stuur data naar ST25DV16
		I2C_SendData(I2C1, letter);
		ST25DV16_WaitForI2CFlag(I2C_ISR_TXIS);
		
		// Wait for- and clear stop condition
		ST25DV16_WaitForI2CFlag(I2C_ISR_STOPF);
		I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
		
		// delay 5ms and increment the address
		delay(SystemCoreClock / 8 / 200);
		eeprom_addr++;
	}
}
/**/
void ST25DV16_WaitForI2CFlag(uint32_t flag){
  uint32_t timeout = ST25DV16_TIMEOUT;
  
  if(flag == I2C_ISR_BUSY){
    while(I2C_GetFlagStatus(I2C1, flag) != RESET){
      if(timeout-- == 0){
        ST25DV16_CommStatus = ST25DV16_COMM_ERROR;
        return;
      }
    }
  }else{
    while(I2C_GetFlagStatus(I2C1, flag) == RESET){
      if(timeout-- == 0){
        ST25DV16_CommStatus = ST25DV16_COMM_ERROR;
        return;
      }
    }
  }
}
