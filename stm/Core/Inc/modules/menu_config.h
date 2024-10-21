/*
 * menu_config.h
 *
 *  Created on: Jul 5, 2023
 *      Author: mfact
 */

#pragma once

typedef struct {
	const uint8_t optionLength;
	const uint8_t *redirects;
	const char *title;
	const char **options;
} menu_page;

const uint16_t double_press = 500;
const uint16_t scroll_increments[3] = { 1, 50, 1000 };
const char* scroll_literals[3] = {"x1", "x50", "x1000"};

const char *menuLiterals[4] =
		{ "Program 1", "Program 2", "Program 3", "General" };
const char *programLiterals[8] = { "Height", "Distance Time", "Bottom Delay",
		"Total Passes", "Safety Timeout", "BACK" };
const char *generalLiterals[3] = { "Load Backup", "Write Backup", "BACK" };
const char *confirmLiterals[2] = { "BACK", "Confirm" };

const uint8_t confirmRedirects[2] = { 0, 0 };

const menu_page menu =
		{ 4, (const uint8_t[4] ) { 1, 2, 3, 4 }, "Menu", menuLiterals };
const menu_page prog1 = { 6, (const uint8_t[6] ) { 8, 9, 10, 11, 12, 0 },
		"Program 1",
		programLiterals };
const menu_page prog2 = { 6, (const uint8_t[6] ) { 16, 17, 18, 19, 20, 0 },
		"Program 2", programLiterals };
const menu_page prog3 = { 6, (const uint8_t[6] ) { 24, 25, 26, 27, 28, 0 },
		"Program 3", programLiterals };
const menu_page general = { 3, (const uint8_t[3] ) { 5, 6, 0 }, "General",
		generalLiterals };
const menu_page load_backup = { 2, confirmRedirects, "Load Backup", confirmLiterals };
const menu_page write_backup = { 2, confirmRedirects, "Write Backup",
		confirmLiterals };

const menu_page pages[7] = { menu, prog1, prog2, prog3, general, load_backup, write_backup };
