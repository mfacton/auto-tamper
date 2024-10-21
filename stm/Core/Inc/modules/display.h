/*
 * display.h
 *
 *  Created on: Jul 2, 2023
 *      Author: mfact
 */

#pragma once

#include <inttypes.h>
#include "main.h"

void Display_Init(I2C_HandleTypeDef *h_i2c);
void Display_PrintText(const char *text);
void Display_PrintTextPosition(uint8_t row, uint8_t col, const char *text);
void Display_PrintValue(uint16_t value);
void Display_PrintValuePosition(uint8_t row, uint8_t col, uint16_t value);
void Display_PrintTitle(const char *text);
void Display_PrintLinePosition(uint8_t row, uint8_t col, const char *text);
void Display_Clear(void);
