/*
 * sensors.c
 *
 *  Created on: Jul 1, 2023
 *      Author: mfact
 */

#include <modules/sensor.h>
#include "main.h"

static Sensor_State pressure;
static Sensor_State feelers[2];
static Sensor_State oil;

void Sensor_Init(void) {
	pressure = SENSOR_OFF;
	feelers[SENSOR_FEELER_FRONT] = SENSOR_OFF;
	feelers[SENSOR_FEELER_BACK] = SENSOR_OFF;
	oil = SENSOR_OFF;
}

Sensor_State Sensor_State_Pressure(void) {
	return pressure;
}
Sensor_State Sensor_State_Feeler(Sensor_Feeler feeler) {
	return feelers[feeler];
}
Sensor_State Sensor_State_Oil(void) {
	return oil;
}

void Sensor_Feeler_Clear(Sensor_Feeler feeler) {
	feelers[feeler] = SENSOR_OFF;
}

void Sensor_Handler_irq(uint16_t GPIO_Pin) {
	switch (GPIO_Pin) {
	case FL1_Pin:
		feelers[SENSOR_FEELER_FRONT] = SENSOR_ON;
		break;
	case FL2_Pin:
		feelers[SENSOR_FEELER_BACK] = SENSOR_ON;
		break;
	case OIL_Pin:
		oil = SENSOR_ON;
		break;
	}
}

void Sensor_Handler_tim() {
	pressure = !HAL_GPIO_ReadPin(PS_GPIO_Port, PS_Pin);
}
