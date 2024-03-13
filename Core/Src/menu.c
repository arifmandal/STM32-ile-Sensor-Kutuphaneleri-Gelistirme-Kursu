/*
 * menu.c
 *
 *  Created on: Mar 13, 2024
 *      Author: Arif Mandal
 */

#include "menu.h"

char *main_menu_items[] = { "-> Temperature", "-> Humidity", "-> Altitude" };
uint8_t selectedMenuItem = 0;

void printMenuItems(uint8_t menuCount) {

	ssd1306_Fill(Black);

	int i = 0;

	for (i = 0; i < menuCount; i++) {
		ssd1306_SetCursor(0, i * 20);

		if (i == selectedMenuItem) {
			ssd1306_WriteString(main_menu_items[i], Font_7x10, Black);
		} else {
			ssd1306_WriteString(main_menu_items[i], Font_7x10, White);
		}

	}

	ssd1306_UpdateScreen();
}

void handleMenuNavigation() {

	printMenuItems(MAIN_MENU_ITEM_COUNT);
	HAL_Delay(150);

	if (MENU_CLICK) {
		selectedMenuItem++;
		ssd1306_WriteString(main_menu_items[selectedMenuItem], Font_7x10,
				White);

		if (selectedMenuItem == MAIN_MENU_ITEM_COUNT) {
			selectedMenuItem = 0;
		}
	}

	if (selectedMenuItem == 0 && SELECT_CLICK) {
		selectedMenuItem = 0;
		while (1) {
			printTemperature();
			if (ESCAPE_CLICK) {
				break;
			}
		}

		return;
	}

	if (selectedMenuItem == 1 && SELECT_CLICK) {
		selectedMenuItem = 0;
		while (1) {
			printHumidity();
			if (ESCAPE_CLICK) {
				break;
			}
		}

		return;
	}

	if (selectedMenuItem == 2 && SELECT_CLICK) {
		selectedMenuItem = 0;
		while (1) {
			printAltitude();
			if (ESCAPE_CLICK) {
				break;
			}
		}

		return;
	}

}

void printTemperature(){
	ssd1306_Fill(Black);
	ssd1306_SetCursor(10, 10);
	ssd1306_WriteString("Temp: 24", Font_7x10, White);
	ssd1306_UpdateScreen();

}

void printHumidity(){
	ssd1306_Fill(Black);
	ssd1306_SetCursor(10, 10);
	ssd1306_WriteString("Hum: 45", Font_7x10, White);
	ssd1306_UpdateScreen();

}

void printAltitude(){
	ssd1306_Fill(Black);
	ssd1306_SetCursor(10, 10);
	ssd1306_WriteString("Alt: 890", Font_7x10, White);
	ssd1306_UpdateScreen();

}

