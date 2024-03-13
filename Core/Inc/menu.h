/*
 * menu.h
 *
 *  Created on: Mar 13, 2024
 *      Author: Arif Mandal
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_

#include "main.h"
#include "ssd1306_conf.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"

#define MAIN_MENU_ITEM_COUNT    3
#define MENU_CLICK                (HAL_GPIO_ReadPin(GPIOA, MENU_Pin))
#define SELECT_CLICK              (HAL_GPIO_ReadPin(GPIOA, SELECT_Pin))
#define ESCAPE_CLICK              (HAL_GPIO_ReadPin(GPIOA, ESCAPE_Pin))

void printMenuItems(uint8_t menuCount);
void handleMenuNavigation(void);
void printTemperature(void);
void printHumidity(void);
void printAltitude(void);

#endif /* INC_MENU_H_ */
