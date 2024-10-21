/*
 * movement.h
 *
 *  Created on: Jul 5, 2023
 *      Author: mfact
 */

#pragma once

typedef enum {
	MOVEMENT_UP = 0u,
	MOVEMENT_DOWN = 1u,
	MOVEMENT_VERTICAL_STOP = 2u,
} Movement_Vertical;

typedef enum {
	MOVEMENT_BACKWARD = 0u,
	MOVEMENT_FORWARD = 1u,
	MOVEMENT_HORIZONTAL_STOP = 2u,
} Movement_Horizontal;

void Movement_Init(void);

const char* Movement_GetVertical(void);
const char* Movement_GetHorizontal(void);

void Movement_MoveVertical(Movement_Vertical direction);
void Movement_MoveHorizontal(Movement_Horizontal direction);
void Movement_StopAll(void);
