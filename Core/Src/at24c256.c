/*
 * at24c256.c
 *
 *  Created on: Mar 28, 2024
 *      Author: Arif Mandal
 */

#include "at24c256.h"


AT24C256InitStatus AT24C256_Init(I2C_HandleTypeDef *hi2cx){

	if (HAL_I2C_IsDeviceReady(hi2cx, AT24C256_DEVICE_ADDRESS, 5, TIMEOUT) == HAL_OK) {
		return INIT_SUCCESS;
	}

	return INIT_FAIL;
}

AT24C256WriteStatus AT24C256_Write(I2C_HandleTypeDef *hi2cx, uint16_t MemAddress, uint8_t *pData, uint16_t size){

	if (HAL_I2C_Mem_Write(hi2cx, AT24C256_DEVICE_ADDRESS, MemAddress, 2, pData, size, TIMEOUT) == HAL_OK) {

		return WRITE_SUCCESS;

	}
	return WRITE_FAIL;
}

AT24C256ReadStatus AT24C256_Read(I2C_HandleTypeDef *hi2cx, uint16_t MemAddress, uint8_t *pData, uint16_t size){

	if (HAL_I2C_Mem_Read(hi2cx, AT24C256_DEVICE_ADDRESS, MemAddress, 2, pData, size, TIMEOUT) == HAL_OK) {

		return READ_SUCCESS;

	}

	return READ_FAIL;
}


AT24C256EraseStatus AT24C256_Erase(I2C_HandleTypeDef *hi2cx){

	uint16_t MemAddress = 0;

	uint8_t buffer[AT24C256_PAGE_SIZE];

	/* other option to fill buffer
	for (int i = 0; i < AT24C256_PAGE_SIZE; i++) {
		buffer[i] = 0xff;
	}
	*/
	memset(buffer, 0xff, AT24C256_PAGE_SIZE);

	while(MemAddress < (AT24C256_PAGE_SIZE * AT24C256_PAGE_NUMBER)){

		if (HAL_I2C_Mem_Write(hi2cx, AT24C256_DEVICE_ADDRESS, MemAddress, 2, buffer, AT24C256_PAGE_SIZE, TIMEOUT) == HAL_OK) {
			return ERASE_SUCCESS;
		}

	}

	return ERASE_FAIL;

}
