/*
 * lis302dl.c
 *
 *  Created on: Mar 9, 2024
 *      Author: Arif Mandal
 */
#include "lis302dl.h"

uint8_t LIS302DL_ReadWhoAmI(SPI_HandleTypeDef *hpsi) {

	uint8_t whoAmI = 0;
	uint8_t regAddress = LIS302DL_WHO_AM_I + LIS302DL_READ;

	LIS302DL_CS_LOW
	;
	HAL_SPI_Transmit(hpsi, &regAddress, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(hpsi, &whoAmI, 1, HAL_MAX_DELAY);
	LIS302DL_CS_HIGH
	;

	return whoAmI;
}

LIS302DLInitStatus LIS302DL_Init(SPI_HandleTypeDef *hpsi) {

	uint8_t tempReg = 0;
	CTRL_REG1_Register_t CTRL_REG1_Register = { 0 };

	CTRL_REG1_Register.DR = LIS302DL_400HZ;
	CTRL_REG1_Register.PD = LIS302DL_ACTIVE_MODE;
	CTRL_REG1_Register.FS = LIS302DL_2G;
	CTRL_REG1_Register.STP = LIS302DL_SELF_TEST_DISABLE;
	CTRL_REG1_Register.STM = LIS302DL_SELF_TEST_DISABLE;
	CTRL_REG1_Register.Zen = 0x01;
	CTRL_REG1_Register.Yen = 0x01;
	CTRL_REG1_Register.Xen = 0x01;

	tempReg = *((uint8_t*) &CTRL_REG1_Register);
	uint8_t regAddress = LIS302DL_CTRL_REG1 + LIS302DL_WRITE;

	LIS302DL_CS_LOW;
	HAL_SPI_Transmit(hpsi, &regAddress, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(hpsi, &tempReg, 1, HAL_MAX_DELAY);
	LIS302DL_CS_HIGH;

	return INIT_SUCCESS;
}

void LIS302DL_ReadAccel(SPI_HandleTypeDef *hpsi, int8_t *accelData) {

	LIS302DL_CS_LOW;

	uint8_t regAddress_X = LIS302DL_OUT_X + LIS302DL_READ;
	HAL_SPI_Transmit(hpsi, &regAddress_X, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(hpsi, (uint8_t*)&accelData[0], 1, HAL_MAX_DELAY);

	uint8_t regAddress_Y = LIS302DL_OUT_Y + LIS302DL_READ;
	HAL_SPI_Transmit(hpsi, &regAddress_Y, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(hpsi, (uint8_t*)&accelData[1], 1, HAL_MAX_DELAY);

	uint8_t regAddress_Z = LIS302DL_OUT_Z + LIS302DL_READ;
	HAL_SPI_Transmit(hpsi, &regAddress_Z, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(hpsi, (uint8_t*)&accelData[2], 1, HAL_MAX_DELAY);

	LIS302DL_CS_HIGH;
}

void LIS302DL_ReadgData(SPI_HandleTypeDef *hpsi, int8_t *accelData, float *gData) {

		for (int i = 0;i < 3; i++) {
			gData[i] = accelData[i] * LIS302DL_SENSITIVITY_2G;
		}
}
