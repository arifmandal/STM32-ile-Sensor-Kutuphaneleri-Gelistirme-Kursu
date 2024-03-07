/*
 * utility.c
 *
 *  Created on: Mar 7, 2024
 *      Author: Arif Mandal
 */

#include "utility.h"

int32_t constrain(int32_t value, int32_t min, int32_t max) {
	int limit;

	if (value < min) {
		limit = min;
	} else if (value > max) {
		limit = max;
	} else {
		limit = value;
	}

	return limit;
}

int32_t getRandomNumber(int32_t min, int32_t max){

	int32_t randomNumber = 0;

	randomNumber = (rand() % (max +1 -min)) + min;

	return randomNumber;

}

int32_t map(int32_t value, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max){

	int32_t mapValue = 0;

	mapValue = (value-in_min)*(out_max-out_min)/(in_max-in_min) + out_min;

    return mapValue;
}

