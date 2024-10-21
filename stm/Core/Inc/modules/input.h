/*
 * input.h
 *
 *  Created on: Jul 5, 2023
 *      Author: mfact
 */

#pragma once

#include <inttypes.h>

typedef enum {
	INPUT_OFF = 0u,
	INPUT_ON = 1u
}Input_State;

typedef enum {
	INPUT_BTN_EDIT = 0u,
	INPUT_BTN_STOP = 1u,
	INPUT_BTN_SW = 2u,

	INPUT_BTN_PRG1 = 3u,
	INPUT_BTN_PRG2 = 4u,
	INPUT_BTN_PRG3 = 5u,

	INPUT_RE_LEFT = 6u,
	INPUT_RE_RIGHT = 7u
}Input_Latch;

void Input_Init(void);

void Input_Update(void);

//reading latch clears it
Input_State Input_Read_Latch(Input_Latch button);

uint8_t Input_Ready(void);

void Input_Handler(void);
