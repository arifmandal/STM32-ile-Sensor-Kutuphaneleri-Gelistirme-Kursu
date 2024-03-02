/*
 * joystick.h
 *
 *  Created on: Mar 2, 2024
 *      Author: Arif Mandal
 */

#ifndef INC_JOYSTICK_H_
#define INC_JOYSTICK_H_

#include "main.h"
#include "stdbool.h"
#define NUMBEROFAXIS 2

void joystickInit(ADC_HandleTypeDef hadcx);

void getADCValues(uint16_t *XAxisValue, uint16_t *YAxisValue, uint32_t timeInterval);

bool getButtonState(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);


#endif /* INC_JOYSTICK_H_ */
