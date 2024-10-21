/*
 * memory.h
 *
 *  Created on: Jul 5, 2023
 *      Author: mfact
 */

#pragma once

#include "main.h"

typedef enum {
	MEMORY_HEIGHT = 0u,
	MEMORY_DISTANCE = 1u,
	MEMORY_DELAY = 2u,
	MEMORY_PASSES = 3u,
	MEMORY_SAFETY = 4u,
} Memory_Variables;

//prog 0* = page 0&1 not used
//prog 1 = page 2&3
//prog 2 = page 4&5
//prog 3 = page 6&7
//other vars = page 8-15

//memory backup page 16-31

void Memory_Init(I2C_HandleTypeDef *hi2c);

void Memory_LoadProgram(uint8_t program, uint16_t *program_variables);

uint16_t Memory_GetVariable(uint8_t address);
void Memory_SetVariable(uint8_t address, uint16_t value);

void Memory_LoadBackup(void);
void Memory_WriteBackup(void);
