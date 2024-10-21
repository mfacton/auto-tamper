/*
 * movement.c
 *
 *  Created on: Jul 1, 2023
 *      Author: mfact
 */

#include "modules/movement.h"
#include "main.h"

static Movement_Vertical vertical;
static Movement_Horizontal horizontal;

static const char* v_literals[3] = {"Up", "Down", "Idle"};
static const char* h_literals[3] = {"Backward", "Forward", "Idle"};

void Movement_Init(void) {
	vertical = MOVEMENT_VERTICAL_STOP;
	horizontal = MOVEMENT_HORIZONTAL_STOP;
}

const char* Movement_GetVertical(void) {
	return v_literals[vertical];
}
const char* Movement_GetHorizontal(void) {
	return h_literals[horizontal];
}

void Movement_MoveVertical(Movement_Vertical direction) {
	vertical = direction;
	switch (direction) {
	case MOVEMENT_UP:
		HAL_GPIO_WritePin(SIG1_GPIO_Port, SIG1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SIG2_GPIO_Port, SIG2_Pin, GPIO_PIN_RESET);
		break;
	case MOVEMENT_DOWN:
		HAL_GPIO_WritePin(SIG2_GPIO_Port, SIG2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SIG1_GPIO_Port, SIG1_Pin, GPIO_PIN_RESET);
		break;
	case MOVEMENT_VERTICAL_STOP:
	default:
		HAL_GPIO_WritePin(SIG1_GPIO_Port, SIG1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SIG2_GPIO_Port, SIG2_Pin, GPIO_PIN_RESET);
		break;
	}
}

void Movement_MoveHorizontal(Movement_Horizontal direction) {
	horizontal = direction;
	switch (direction) {
	case MOVEMENT_FORWARD:
		HAL_GPIO_WritePin(SIG3_GPIO_Port, SIG3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SIG4_GPIO_Port, SIG4_Pin, GPIO_PIN_RESET);
		break;
	case MOVEMENT_BACKWARD:
		HAL_GPIO_WritePin(SIG4_GPIO_Port, SIG4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SIG3_GPIO_Port, SIG3_Pin, GPIO_PIN_RESET);
		break;
	case MOVEMENT_HORIZONTAL_STOP:
	default:
		HAL_GPIO_WritePin(SIG3_GPIO_Port, SIG3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SIG4_GPIO_Port, SIG4_Pin, GPIO_PIN_RESET);
		break;
	}
}

void Movement_StopAll(void) {
	Movement_MoveVertical(MOVEMENT_VERTICAL_STOP);
	Movement_MoveHorizontal(MOVEMENT_HORIZONTAL_STOP);
}
