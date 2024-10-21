/*
 * i2c_eeprom.h
 *
 *  Created on: Jul 8, 2023
 *      Author: mfact
 */

#pragma once

#include "main.h"
//32 pages, 8 bytes each

typedef struct i2c_eeprom{
	I2C_HandleTypeDef *hi2c;
	uint8_t i2c_addr;
	uint16_t timeout;
}i2c_eeprom_HandleTypeDef;

void eeprom_create_handle(i2c_eeprom_HandleTypeDef *i2c_eeprom, I2C_HandleTypeDef *h_i2c, uint8_t address, uint16_t timeout);

//will roll over onto same page
void eeprom_write(i2c_eeprom_HandleTypeDef *i2c_eeprom, uint8_t address, uint8_t *buf, uint8_t len);

//current address looped through entire memory
void eeprom_address_read(i2c_eeprom_HandleTypeDef *i2c_eeprom, uint8_t *buf, uint8_t len);

void eeprom_random_read(i2c_eeprom_HandleTypeDef *i2c_eeprom, uint8_t address, uint8_t *buf, uint8_t len);
