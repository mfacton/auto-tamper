/*
 * LED.h
 *
 *  Created on: Jul 5, 2023
 *      Author: mfact
 */

#pragma once

#include "main.h"

typedef enum {
	LED_EDIT = 0u,
	LED_PRG1 = 1u,
	LED_PRG2 = 2u,
	LED_PRG3 = 3u
} LED_Name;

void LED_Write(LED_Name led, GPIO_PinState pin_state);

void LED_ResetPRG(void);
