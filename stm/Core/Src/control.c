/*
 * control.c
 *
 *  Created on: Jul 5, 2023
 *      Author: mfact
 */

#include <modules/receiver.h>
#include "main.h"
#include "control.h"
#include "modules/sensor.h"
#include "modules/movement.h"
#include "modules/display.h"
#include "modules/input.h"
#include "modules/LED.h"
#include "modules/memory.h"
#include "modules/menu_config.h"

#include <stdio.h>

//mode variables
static uint8_t mode_edit;
static uint8_t mode_auto;

//buffer for variables in current program
static uint16_t running_variables[8];

//auto variables
static uint8_t pass;
static uint8_t direction;
static uint8_t process;
static uint16_t pass_time;
static uint32_t event_time;
static uint8_t pressure;//for double pressure test

//edit variables
static uint32_t last_press;
static uint8_t edit_value; //bool
static uint8_t page_num;
static uint8_t address;
static uint16_t value;
static uint8_t scroll; //0,1,2
static uint16_t increment;
static uint8_t max_select;
static uint8_t select_option;
static uint8_t view;

//manual variables
static uint8_t last_data;
static uint32_t data_time;
static uint8_t receive_timeout;

//direction literals
static const char *direction_literals[2] = { "back", "fwd " };

//min and max functions
static uint8_t min(int8_t a, int8_t b) {
	if (a < b) {
		return a;
	}
	return b;
}

static uint8_t max(int8_t a, int8_t b) {
	if (a > b) {
		return a;
	}
	return b;
}

void Control_Init(void) {
	mode_edit = 0;
	mode_auto = 0;

	last_data = 0;
	data_time = HAL_GetTick();
	receive_timeout = 1;

	Display_Clear();
	Control_DrawManual();

//	Memory_SetVariable(8, 2000);
//	Memory_SetVariable(9, 2000);
//	Memory_SetVariable(10, 400);
//	Memory_SetVariable(11, 2);
//	Memory_SetVariable(12, 15000);
//
//	//prog2
//	Memory_SetVariable(16, 1600);
//	Memory_SetVariable(17, 1200);
//	Memory_SetVariable(18, 400);
//	Memory_SetVariable(19, 4);
//	Memory_SetVariable(20, 15000);
//
//	//prog3
//	Memory_SetVariable(24, 1300);
//	Memory_SetVariable(25, 500);
//	Memory_SetVariable(26, 500);
//	Memory_SetVariable(27, 6);
//	Memory_SetVariable(28, 15000);
//
//	Memory_WriteBackup();
}
void Control_Loop(void) {
	if (Sensor_State_Oil()) {
		Control_Error("Detected Low Oil");
	}
	if (Input_Ready()) {
		Input_Update();
	}
	if (Input_Read_Latch(INPUT_BTN_EDIT)) {
		mode_edit = !mode_edit;
		LED_Write(LED_EDIT, mode_edit);
		if (mode_edit) {
			Control_StopProgram();
			Control_InitPage(0);
		} else {
			Control_DrawManual();
		}
	}
	if (Input_Read_Latch(INPUT_BTN_STOP)) {
		Control_StopProgram();
	}else if (!mode_edit) {
		if (Input_Read_Latch(INPUT_BTN_PRG1)) {
			Control_RunProgram(1);
		}else if (Input_Read_Latch(INPUT_BTN_PRG2)) {
			Control_RunProgram(2);
		}else if (Input_Read_Latch(INPUT_BTN_PRG3)) {
			Control_RunProgram(3);
		}
	}
	if (mode_edit) {
		Control_EditLoop();
	}
	if (!mode_auto && !receive_timeout && HAL_GetTick() - data_time > 1000) {
		Movement_StopAll();
		Display_PrintLinePosition(2, 12, "Idle");
		Display_PrintLinePosition(3, 12, "Idle");
		receive_timeout = 1;
	}
	if (Receiver_Available()) {
		uint8_t data = Receiver_Read();

		//timeout
		receive_timeout = 0;
		data_time = HAL_GetTick();
		if (data == last_data) {
			goto parse_end;
		}
		//123sfbud

		if (data & 0x1F) {
			if (mode_auto) {
				Control_StopProgram();
				Control_DrawManual();
			}
			if (data & 0x10) {
				Movement_StopAll();
			} else {
				//direction is chosen
				if (data & 0x01) {		// down
					Movement_MoveVertical(MOVEMENT_DOWN);
					if (!mode_edit) {
						Display_PrintLinePosition(2, 12, "Down");
					}
				} else if (data & 0x02) {
					Movement_MoveVertical(MOVEMENT_UP);
					if (!mode_edit) {
						Display_PrintLinePosition(2, 12, "Up");
					}
				} else {
					Movement_MoveVertical(MOVEMENT_VERTICAL_STOP);
					if (!mode_edit) {
						Display_PrintLinePosition(2, 12, "Idle");
					}
				}
				if (data & 0x08) {
					Movement_MoveHorizontal(MOVEMENT_FORWARD);
					if (!mode_edit) {
						Display_PrintLinePosition(3, 12, "Forward");
					}
				} else if (data & 0x04) {
					Movement_MoveHorizontal(MOVEMENT_BACKWARD);
					if (!mode_edit) {
						Display_PrintLinePosition(3, 12, "Backward");
					}
				} else {
					Movement_MoveHorizontal(MOVEMENT_HORIZONTAL_STOP);
					if (!mode_edit) {
						Display_PrintLinePosition(3, 12, "Idle");
					}
				}
			}
		} else if (data & 0xE0) {
			if (!mode_edit) {
				if (data & 0x20) {
					Control_RunProgram(3);
				} else if (data & 0x40) {
					Control_RunProgram(2);
				} else if (data & 0x80) {
					Control_RunProgram(1);
				}
			}
		} else if (!mode_auto) {
			Movement_StopAll();
			Display_PrintLinePosition(2, 12, "Idle");
			Display_PrintLinePosition(3, 12, "Idle");
		}
		parse_end: last_data = data;
	}
	if (mode_auto) {
		Control_AutoLoop();
	}
}

void Control_AutoLoop(void) {
	uint32_t current_time = HAL_GetTick();

	if (process == 0) {
		if (event_time == -1) {
			if (Sensor_State_Pressure()) {
				if (pressure) {
					event_time = current_time;
				}else{
					pressure = 1;
				}
			}else if (pressure){
				pressure = 0;
			}
			HAL_Delay(1);//for the double check
		} else if (current_time - event_time
				>= running_variables[MEMORY_DELAY]) {
			process++;
			event_time = current_time;
			Movement_MoveVertical(MOVEMENT_UP);
		}
	} else if (process == 1) {
		if (current_time - event_time >= running_variables[MEMORY_HEIGHT]) {
			Movement_MoveVertical(MOVEMENT_VERTICAL_STOP);
			Movement_MoveHorizontal(direction);
			event_time = current_time;
			process++;
		}
	} else {
		uint8_t next_process = 0;//bool

		uint32_t delta_time = current_time - event_time;

		if (Sensor_State_Feeler(direction)) {
			Sensor_Feeler_Clear(!direction);
			direction = !direction;
			pass_time = 0;
			next_process = 1;
			pass++;

			//draw direction
			Display_PrintTextPosition(2, 16, direction_literals[direction]);

			//draw pauses
			char str[6];
			uint8_t len = sprintf(str, "%u", pass);
			Display_PrintTextPosition(1, 11 - len, str);

			if (pass >= running_variables[MEMORY_PASSES]) {
				if (mode_auto == 3) {
					Control_StopProgram();
					Control_DrawManual();
				} else {
					Control_RunProgram(mode_auto + 1);
				}
				return;
			}
		} else if (pass_time + delta_time >= running_variables[MEMORY_SAFETY]) {
			Control_Error("Safety Timeout");
		} else if (delta_time >= running_variables[MEMORY_DISTANCE]) {
			pass_time += delta_time;
			next_process = 1;
		}

		if (next_process) {
			process = 0;
			Movement_MoveHorizontal(MOVEMENT_HORIZONTAL_STOP);
			Movement_MoveVertical(MOVEMENT_DOWN);
			pressure = 0;
			event_time = -1;
		}
	}
}

void Control_EditLoop() {
	if (Input_Read_Latch(INPUT_BTN_SW)) {
		if (edit_value) {
			uint32_t current_time = HAL_GetTick();
			if (last_press != 0 && current_time - last_press < double_press) {
				Memory_SetVariable(address, value);
				Control_InitPage(page_num);
			} else {
				scroll = (scroll + 1) % 3;
				//update increment
				Display_PrintLinePosition(3, 11, scroll_literals[scroll]);
				increment = scroll_increments[scroll];
			}
			last_press = current_time;
		} else {
			//special case
			if (select_option == 1) {
				if (page_num == 5) {
					Memory_LoadBackup();
				} else if (page_num == 6) {
					Memory_WriteBackup();
				}
			}
			uint8_t redirect = *(pages[page_num].redirects + select_option);
			if (redirect < 8) {
				Control_InitPage(redirect);
			} else {
				Control_InitValue(redirect);
			}
		}
	}
	uint8_t left = Input_Read_Latch(INPUT_RE_LEFT);
	uint8_t right = Input_Read_Latch(INPUT_RE_RIGHT);
	if (left || right) {
		int8_t direction = 2 * right;		//signed
		direction--;
		if (edit_value) {
			value += direction * increment;
			Display_PrintValuePosition(2, 8, value);
		} else {
			select_option += max_select + direction;
			select_option %= max_select;
			if (max_select > 3) {
				uint8_t new_view = max(min(select_option, view),
						select_option - 2);
				if (new_view != view) {
					view = new_view;
					Control_DrawOptions();
				}
			}
			Control_DrawSelect();
		}
	}
}

void Control_StopProgram(void) {
	mode_auto = 0;
	LED_ResetPRG();
	Movement_StopAll();
	Control_DrawManual();
}
void Control_RunProgram(uint8_t program) {
	if (mode_auto == program) {
		return;
	}
	mode_auto = program;

	Sensor_Feeler_Clear(SENSOR_FEELER_FRONT);
	Sensor_Feeler_Clear(SENSOR_FEELER_BACK);

	pass = 0;
	direction = 1;
	process = 0;
	pass_time = 0;
	event_time = -1;
	pressure = 0;

	Memory_LoadProgram(mode_auto, running_variables);

	Control_DrawProgram();

	LED_ResetPRG();
	LED_Write(program, GPIO_PIN_SET);

	Movement_MoveHorizontal(MOVEMENT_HORIZONTAL_STOP);
	Movement_MoveVertical(MOVEMENT_DOWN);
}

void Control_DrawManual(void) {
	Display_Clear();
	Display_PrintTitle("Manual Mode");

	Display_PrintTextPosition(2, 0, "Vertical:");
	Display_PrintLinePosition(2, 12, Movement_GetVertical());

	Display_PrintTextPosition(3, 0, "Horizontal:");
	Display_PrintLinePosition(3, 12, Movement_GetHorizontal());
}

void Control_DrawProgram(void) {
	Display_Clear();
	Display_PrintTextPosition(0, 1, "Running Program ");
	const char str[2] = { '0' + mode_auto, '\0' };
	Display_PrintText(str);
	Display_PrintTextPosition(1, 0, "pass:     0/");
	Display_PrintValue(running_variables[MEMORY_PASSES]);
	Display_PrintTextPosition(2, 0, "dst: ");
	Display_PrintValue(running_variables[MEMORY_DISTANCE]);
	Display_PrintTextPosition(2, 11, "dir: fwd");
	Display_PrintTextPosition(3, 0, "ht: ");
	Display_PrintValue(running_variables[MEMORY_HEIGHT]);
	Display_PrintTextPosition(3, 10, "del: ");
	Display_PrintValue(running_variables[MEMORY_DELAY]);
}

void Control_InitPage(uint8_t page) {
	page_num = page;
	select_option = 0;
	max_select = pages[page_num].optionLength;
	view = 0;
	edit_value = 0;

	Display_Clear();
	Display_PrintTitle(pages[page_num].title);
	Control_DrawSelect();
	Control_DrawOptions();
}
void Control_InitValue(uint8_t value_address) {
	edit_value = 1;
	address = value_address;
	value = Memory_GetVariable(value_address);
	scroll = 0;
	increment = 1;
	last_press = 0;

	Display_Clear();
	Display_PrintTitle(*(pages[page_num].options + scroll));
	Display_PrintTextPosition(1, 0, "Original:");
	Display_PrintValuePosition(1, 10, value);
	Display_PrintTextPosition(2, 0, "Edited:");
	Display_PrintValuePosition(2, 8, value);
	Display_PrintTextPosition(3, 0, "Increment:");
	Display_PrintValuePosition(3, 11, scroll_increments[0]);
}
void Control_DrawSelect(void) {
	uint8_t row = select_option - view;
	for (uint8_t i = 0; i < 3; i++) {
		Display_PrintTextPosition(i + 1, 0, i == row ? ">" : " ");
	}
}
void Control_DrawOptions(void) {
	uint8_t max_options = min(3, max_select);
	for (uint8_t i = 0; i < max_options; i++) {
		Display_PrintLinePosition(i + 1, 1,
				*(pages[page_num].options + i + view));
	}
}

void Control_Error(const char *text) {
	Movement_StopAll();
	Display_Clear();
	Display_PrintTitle(text);
	Display_PrintTextPosition(1, 1, "Check and Restart");
	Error_Handler();
}
