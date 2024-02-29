/*
 * keypad4x4.h
 *
 *  Created on: Feb 29, 2024
 *      Author: Arif Mandal
 */

#ifndef INC_KEYPAD4X4_H_
#define INC_KEYPAD4X4_H_

#include "main.h"

#define ROWSIZE 4
#define COLUMNSIZE 4

#define KEYPAD_DELAY(T)   HAL_Delay(T);

void keypadInit(GPIO_TypeDef *GPIOx, uint16_t ROW1_PIN, uint16_t ROW2_PIN,
		uint16_t ROW3_PIN, uint16_t ROW4_PIN, uint16_t COLUMN1_PIN,
		uint16_t COLUMN2_PIN, uint16_t COLUMN3_PIN, uint16_t COLUMN4_PIN);

void readKeypad(GPIO_TypeDef *GPIOx, uint8_t *keyPressed);

//uint8_t getChar(uint8_t row, uint8_t col);

#endif /* INC_KEYPAD4X4_H_ */
