/*
 * joystick.c
 *
 *  Created on: Mar 2, 2024
 *      Author: Arif Mandal
 */

#include "joystick.h"

uint32_t adcValues[NUMBEROFAXIS];


void joystickInit(ADC_HandleTypeDef hadcx){

	HAL_ADC_Start_DMA(&hadcx, adcValues, NUMBEROFAXIS);
}

void getADCValues(uint16_t *XAxisValue, uint16_t *YAxisValue, uint32_t timeInterval){

	*XAxisValue = adcValues[0];
	*YAxisValue = adcValues[1];

	for (int i = 0; i < 100 *timeInterval; i++);
}

bool getButtonState(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){

	bool switchValue = false;

	switchValue = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);

	return switchValue;

}


