/******************************************************************************
 * File           : I2C eeprom ST25DV16 implementation
 *****************************************************************************
   Hardware:
                ST25DV16K
	+---------+-------------+                STM
	|+--------|							|          +--------------
	||+-------|		 		VIN	 1|----------| 3V of 5V
	|||+------|				GND	 2|----------| GND
	||||+-----|				SCL  3|----------+ I2C1_SCL / PB6
	||||+-----|				SDA  4|----------+ I2C1_SDA / PB7
	|||+------|				GPO  5|			X		 |
	||+-------|				VEH  6|			X		 |
	|+--------|             |      	   |
	+---------+-------------+      	   |


Pasword protection password: 0000000000000000h



******************************************************************************/
#ifndef _NFC_H_
#define _NFC_H_

/*	Include standard libraries	*/
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/*	Include other files	*/
#include "stm32f0xx.h"
#include "helper.h"
#include "se24lc512.h"


//void handmatige_verbinding(uint8_t byte_amount);
//void ST25DV16_Senddata(uint16_t eeprom_addr, bool user_memory, uint8_t byte_amount, uint8_t data);
//uint8_t calculate_byte_amount(char* string);
//void reset_byte_amount(void);

void ST25DV16_Open_Security_Session(void);
void ST25DV16_Disable_FTM(void);
void ST25DV16_Enable_FTM(void);
uint16_t ST25DV16_NDEF_Header(uint16_t eeprom_addr);
void ST25DV16_NDEF_Record(uint16_t eeprom_addr,uint16_t container_id,uint8_t vloeistoflvl);
void ST25DV16_Senddata2(uint16_t eeprom_addr,uint8_t data);
void ST25DV16_Close_Security_Session(void);

void stuurdata(uint16_t eeprom_addr,char data[],int datalength);

void ST25DV16_WaitForI2CFlag(uint32_t flag);

#endif /* _NFC_H_ */
