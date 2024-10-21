/*
 * receiver.h
 *
 *  Created on: Jul 4, 2023
 *      Author: mfact
 */

#pragma once

#include "main.h"

void Receiver_Init(SPI_HandleTypeDef *hspi);

void Receiver_Handler();

uint8_t Receiver_Available();

uint8_t Receiver_Read();
