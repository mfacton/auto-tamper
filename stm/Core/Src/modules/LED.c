/*
 * LEDs.c
 *
 *  Created on: Jul 1, 2023
 *      Author: mfact
 */

#include "modules/LED.h"
#include "main.h"

void LED_Write(LED_Name led, GPIO_PinState pin_state) {
	switch (led) {
	case LED_PRG1:
		HAL_GPIO_WritePin(LED_PRG1_GPIO_Port, LED_PRG1_Pin, pin_state);
		break;
	case LED_PRG2:
		HAL_GPIO_WritePin(LED_PRG2_GPIO_Port, LED_PRG2_Pin, pin_state);
		break;
	case LED_PRG3:
		HAL_GPIO_WritePin(LED_PRG3_GPIO_Port, LED_PRG3_Pin, pin_state);
		break;
	case LED_EDIT:
		HAL_GPIO_WritePin(LED_EDIT_GPIO_Port, LED_EDIT_Pin, pin_state);
		break;
	}
}

void LED_ResetPRG(void) {
	LED_Write(LED_PRG1, GPIO_PIN_RESET);
	LED_Write(LED_PRG2, GPIO_PIN_RESET);
	LED_Write(LED_PRG3, GPIO_PIN_RESET);
}
