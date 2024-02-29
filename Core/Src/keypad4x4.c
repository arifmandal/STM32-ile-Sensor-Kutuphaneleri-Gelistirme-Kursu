/*
 * keypad4x4.c
 *
 *  Created on: Feb 29, 2024
 *      Author: Arif Mandal
 */

#include "keypad4x4.h"

static uint16_t rowArray[ROWSIZE];
static uint16_t columnArray[COLUMNSIZE];

uint8_t buttonArray[ROWSIZE][COLUMNSIZE] = { { '1', '2', '3', 'A' }, { '4', '5',
		'6', 'B' }, { '7', '8', '9', 'C' }, { '*', '0', '#', 'D' } };

void keypadInit(GPIO_TypeDef *GPIOx, uint16_t ROW1_PIN, uint16_t ROW2_PIN,
		uint16_t ROW3_PIN, uint16_t ROW4_PIN, uint16_t COLUMN1_PIN,
		uint16_t COLUMN2_PIN, uint16_t COLUMN3_PIN, uint16_t COLUMN4_PIN) {

	rowArray[0] = ROW1_PIN;
	rowArray[1] = ROW2_PIN;
	rowArray[2] = ROW3_PIN;
	rowArray[3] = ROW4_PIN;

	columnArray[0] = COLUMN1_PIN;
	columnArray[1] = COLUMN2_PIN;
	columnArray[2] = COLUMN3_PIN;
	columnArray[3] = COLUMN4_PIN;

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/*Configure GPIO pins : ROW1_Pin ROW2_Pin ROW3_Pin ROW4_Pin */
	GPIO_InitStruct.Pin = ROW1_PIN | ROW2_PIN | ROW3_PIN | ROW4_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : COLUMN1_Pin COLUMN2_Pin COLUMN3_Pin COLUMN4_Pin */
	GPIO_InitStruct.Pin = COLUMN1_PIN | COLUMN2_PIN | COLUMN3_PIN | COLUMN4_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

static uint8_t getChar(uint8_t row, uint8_t col) {

	return buttonArray[row][col];

}

void readKeypad(GPIO_TypeDef *GPIOx, uint8_t *keyPressed) {

	uint8_t row, col;

	for (row = 0; row < ROWSIZE; row++) {
		HAL_GPIO_WritePin(GPIOx, rowArray[row], GPIO_PIN_RESET);

		KEYPAD_DELAY(2);
		for (col = 0; col < COLUMNSIZE; col++) {
			if (HAL_GPIO_ReadPin(GPIOx, columnArray[col]) == GPIO_PIN_RESET) {
				*keyPressed = getChar(row, col);
				KEYPAD_DELAY(2);
			}
		}

		HAL_GPIO_WritePin(GPIOx, rowArray[row], GPIO_PIN_SET);
	}

}



