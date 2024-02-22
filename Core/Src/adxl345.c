/*
 * adxl345.c
 *
 *  Created on: Feb 22, 2024
 *      Author: Arif Mandal
 */

#include "adxl345.h"

extern I2C_HandleTypeDef hi2c1;


int ADXL345_ScanDeviceID(){

	for (uint8_t address= 0; address < 255; address++) {
		if (HAL_I2C_IsDeviceReady(&hi2c1, address, 1, TIMEOUT) == HAL_OK) {
			return address;
		}
	}
	return -1;
}


