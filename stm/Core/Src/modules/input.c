/*
 * input.c
 *
 *  Created on: Jul 2, 2023
 *      Author: mfact
 */

#include "modules/input.h"
#include "main.h"

//falling edge latch (on press)
static uint8_t data_ready;
static uint8_t btn_prev[8];
static uint8_t btn_latch[8];

void Input_Init(void) {
	for (uint8_t i = 0; i < 8; i++) {
		btn_prev[i] = 1;
		btn_latch[i] = 0;
	}

	data_ready = 0;
}

void Input_Update() {
	uint8_t btn_state[8];

	btn_state[0] = HAL_GPIO_ReadPin(BTN_EDIT_GPIO_Port, BTN_EDIT_Pin);
	btn_state[1] = HAL_GPIO_ReadPin(BTN_STOP_GPIO_Port, BTN_STOP_Pin);
	btn_state[2] = HAL_GPIO_ReadPin(RE_SW_GPIO_Port, RE_SW_Pin);

	btn_state[3] = HAL_GPIO_ReadPin(BTN_PRG1_GPIO_Port, BTN_PRG1_Pin);
	btn_state[4] = HAL_GPIO_ReadPin(BTN_PRG2_GPIO_Port, BTN_PRG2_Pin);
	btn_state[5] = HAL_GPIO_ReadPin(BTN_PRG3_GPIO_Port, BTN_PRG3_Pin);

	for (uint8_t i = 0; i < 6; i++) {
		if (!btn_state[i] && btn_prev[i]) {
			btn_latch[i] = 1;
		}
		btn_prev[i] = btn_state[i];
	}

	btn_state[6] = HAL_GPIO_ReadPin(RE_CLK_GPIO_Port, RE_CLK_Pin);
	btn_state[7] = HAL_GPIO_ReadPin(RE_DT_GPIO_Port, RE_DT_Pin);

	if (btn_prev[6] && btn_prev[7]) {
		if (!btn_state[6]) {
			btn_latch[6] = 1;
		}else if (!btn_state[7]) {
			btn_latch[7] = 1;
		}
	}
	btn_prev[6] = btn_state[6];
	btn_prev[7] = btn_state[7];

	data_ready = 0;
}

Input_State Input_Read_Latch(Input_Latch button) {
	uint8_t val = btn_latch[button];
	btn_latch[button] = 0;
	return val;
}

uint8_t Input_Ready(void) {
	return data_ready;
}

void Input_Handler(void) {
	data_ready = 1;
}
