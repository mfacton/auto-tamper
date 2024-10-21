/*
 * memory.c
 *
 *  Created on: Jul 2, 2023
 *      Author: mfact
 */

#include "modules/memory.h"
#include "lib/i2c_eeprom.h"

static i2c_eeprom_HandleTypeDef i2c_eeprom;

void Memory_Init(I2C_HandleTypeDef *hi2c) {
	eeprom_create_handle(&i2c_eeprom, hi2c, 0xA0, 100);
}

//since always keeping pairs, endianness is irrelevant
//loadProgram, GetVariable, SetVariable
void Memory_LoadProgram(uint8_t program, uint16_t *program_variables) {
	eeprom_random_read(&i2c_eeprom, program<<4, (uint8_t*)program_variables, 16);
	HAL_Delay(1);
}

uint16_t Memory_GetVariable(uint8_t address) {
	uint8_t buf[2];
	eeprom_random_read(&i2c_eeprom, address<<1, buf, 2);
	HAL_Delay(1);
	return *((uint16_t*)buf);
}
//void Memory_SetVariable(uint8_t address, uint16_t value) {
//	eeprom_write(&i2c_eeprom, address<<1, (uint8_t*)(&value), 2);
//}
//uint16_t Memory_GetVariable(uint8_t address) {
//	uint8_t buf[2];
//	eeprom_random_read(&i2c_eeprom, address<<1, buf, 2);
//	return (buf[1] << 8) & buf[0];
//}
void Memory_SetVariable(uint8_t address, uint16_t value) {
	uint8_t equal = Memory_GetVariable(address) == value;
	HAL_Delay(1);
	if (equal) {
		return;
	}
	uint8_t buf[2] = {value & 0xFF, value >> 8};
	eeprom_write(&i2c_eeprom, address<<1, buf, 2);
	HAL_Delay(1);
}

void Memory_LoadBackup(void) {
	uint8_t buf[128];
	eeprom_random_read(&i2c_eeprom, 128, buf, 128);
	HAL_Delay(1);
	for (uint8_t page = 0; page < 16; page++) {
		const uint8_t address = page << 3;
		eeprom_write(&i2c_eeprom, address, buf+address, 8);
		HAL_Delay(1);
	}
}
void Memory_WriteBackup(void) {
	uint8_t buf[128];
	eeprom_random_read(&i2c_eeprom, 0, buf, 128);
	HAL_Delay(1);
	for (uint8_t page = 0; page < 16; page++) {
		const uint8_t address = page << 3;
		eeprom_write(&i2c_eeprom, address+128, buf+address, 8);
		HAL_Delay(1);
	}
}
