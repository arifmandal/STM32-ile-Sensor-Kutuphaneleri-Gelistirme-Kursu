/*
 * utility.h
 *
 *  Created on: Mar 7, 2024
 *      Author: Arif Mandal
 */

#ifndef INC_UTILITY_H_
#define INC_UTILITY_H_

#include "main.h"
#include "stdlib.h"

#define MIN(A, B)   (((A) < (B)) ? (A) : (B))
#define MAX(A, B)   (((A) > (B)) ? (A) : (B))
#define ABS(A)      ((A) < 0 ? (-A) : (A))
#define SQUARE(A)   ((A) * (A))
#define CUBE(A)     ((A) * (A) * (A))
#define POW2(A)     (1<<(A))

int32_t constrain(int32_t value, int32_t min, int32_t max);
int32_t getRandomNumber(int32_t min, int32_t max);
int32_t map(int32_t value, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);

#endif /* INC_UTILITY_H_ */
