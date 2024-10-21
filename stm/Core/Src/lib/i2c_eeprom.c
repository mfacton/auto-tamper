/*
 * i2c_eeprom.c
 *
 *  Created on: Jul 8, 2023
 *      Author: mfact
 */

#include "lib/i2c_eeprom.h"

void eeprom_create_handle(i2c_eeprom_HandleTypeDef *i2c_eeprom, I2C_HandleTypeDef *h_i2c, uint8_t address, uint16_t timeout) {
	i2c_eeprom->hi2c = h_i2c;
	i2c_eeprom->i2c_addr = address;
	i2c_eeprom->timeout = timeout;
}

void eeprom_write(i2c_eeprom_HandleTypeDef *i2c_eeprom, uint8_t address, uint8_t *buf, uint8_t len) {
	HAL_I2C_Mem_Write(i2c_eeprom->hi2c, i2c_eeprom->i2c_addr, address, I2C_MEMADD_SIZE_8BIT, buf, len, i2c_eeprom->timeout);
}

void eeprom_address_read(i2c_eeprom_HandleTypeDef *i2c_eeprom, uint8_t *buf, uint8_t len) {
	HAL_I2C_Master_Receive(i2c_eeprom->hi2c, i2c_eeprom->i2c_addr, buf, len, i2c_eeprom->timeout);
}

void eeprom_random_read(i2c_eeprom_HandleTypeDef *i2c_eeprom, uint8_t address, uint8_t *buf, uint8_t len) {
	HAL_I2C_Mem_Read(i2c_eeprom->hi2c, i2c_eeprom->i2c_addr, address, I2C_MEMADD_SIZE_8BIT, buf, len, i2c_eeprom->timeout);
}
