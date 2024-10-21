/*
 * control.h
 *
 *  Created on: Jul 5, 2023
 *      Author: mfact
 */

#pragma once

void Control_Init(void);
void Control_Loop(void);

void Control_AutoLoop(void);
void Control_EditLoop(void);

void Control_StopProgram(void);
void Control_RunProgram(uint8_t program);

void Control_DrawManual(void);
void Control_DrawProgram(void);

void Control_InitPage(uint8_t page);
void Control_InitValue(uint8_t address);
void Control_DrawSelect(void);
void Control_DrawOptions(void);

void Control_Error(const char *text);
