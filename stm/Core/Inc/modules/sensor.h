/*
 * sensors.h
 *
 *  Created on: Jul 5, 2023
 *      Author: mfact
 */

#pragma once

#include <inttypes.h>

typedef enum {
	SENSOR_OFF = 0u,
	SENSOR_ON = 1u
} Sensor_State;

typedef enum {
	SENSOR_FEELER_FRONT = 0u,
	SENSOR_FEELER_BACK = 1u
} Sensor_Feeler;

void Sensor_Init(void);

Sensor_State Sensor_State_Pressure(void);
Sensor_State Sensor_State_Feeler(Sensor_Feeler feeler);
Sensor_State Sensor_State_Oil(void);

void Sensor_Pressure_Clear(void);
void Sensor_Feeler_Clear(Sensor_Feeler feeler);

void Sensor_Handler_irq(uint16_t GPIO_Pin);
void Sensor_Handler_tim();
