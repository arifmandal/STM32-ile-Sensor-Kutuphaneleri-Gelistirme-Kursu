/*
 * bme280.c
 *
 *  Created on: Feb 24, 2024
 *      Author: Arif Mandal
 */

#include "bme280.h"

int BME280_ScanDeviceID(I2C_HandleTypeDef *hi2cx) {

	for (uint8_t address = 0; address < 255; address++) {
		if (HAL_I2C_IsDeviceReady(hi2cx, address, 1, TIMEOUT) == HAL_OK) {
			return address;
		}
	}

	return -1; // error

}

BME280ReadStatus BME280_ReadRegisterData(I2C_HandleTypeDef *hi2cx,
		uint16_t registerAddress, uint16_t sizeofData, uint8_t *dataBuffer) {

	if (HAL_I2C_Mem_Read(hi2cx, BME280_DEVICE_ADDRESS, registerAddress, 1,
			dataBuffer, sizeofData, TIMEOUT) == HAL_OK) {
		return READ_SUCCESS;
	}
	return READ_FAIL;
}

BME280ReadStatus BME280_ReadRegister16Data(I2C_HandleTypeDef *hi2cx,
		uint16_t registerAddress, uint16_t sizeofData, uint16_t *dataBuffer) {

	uint8_t tempData[2] = { 0 };
	if (HAL_I2C_Mem_Read(hi2cx, BME280_DEVICE_ADDRESS, registerAddress, 1,
			tempData, sizeofData, TIMEOUT) == HAL_OK) {
		*dataBuffer = (uint16_t)((tempData[1]<<8) | tempData[0]);
		return READ_SUCCESS;
	}
	return READ_FAIL;
}

BME280WriteStatus BME280_WriteRegisterData(I2C_HandleTypeDef *hi2cx,
		uint16_t registerAddress, uint16_t value) {

	uint8_t data[2] = { 0 };
	data[0] = registerAddress;
	data[1] = value;

	if (HAL_I2C_Master_Transmit(hi2cx, BME280_DEVICE_ADDRESS, data,
			sizeof(data), TIMEOUT) == HAL_OK) {
		return Write_SUCCESS;
	}

	return Write_FAIL;

}

BME280InitStatus BME280_Init(I2C_HandleTypeDef *hi2cx) {

	uint8_t dataBuffer = 0;
	uint8_t tempReg = 0xB6; //reset value

	BME280_WriteRegisterData(hi2cx, BME280_RESET, tempReg); //device reset
	BME280_ReadRegisterData(hi2cx, BME280_ID, 1, &dataBuffer); //read id

	if (dataBuffer != 0x60) {
		return INIT_FAIL;
	}

	CTRL_HUM_Register_t CTRL_HUM_Control = { 0 };

	CTRL_HUM_Control.OSRS_H = BME280_OVERSAMPLING_4;
	CTRL_HUM_Control.Reserved = 0x00;

	tempReg = *((uint8_t*) &CTRL_HUM_Control);

	BME280_WriteRegisterData(hi2cx, BME280_CTRL_HUM, tempReg);

	CTRL_MEAS_Register_t CTRL_MEAS_Control = { 0 };

	CTRL_MEAS_Control.Mode = BME280_NORMAL_MODE;
	CTRL_MEAS_Control.OSRS_P = BME280_OVERSAMPLING_4;
	CTRL_MEAS_Control.OSRS_T = BME280_OVERSAMPLING_4;

	tempReg = *((uint8_t*) &CTRL_MEAS_Control);

	BME280_WriteRegisterData(hi2cx, BME280_CTRL_MEAS, tempReg);

	Config_Register_t Config_Control = { 0 };
	Config_Control.SPI_EN = 0x00;
	Config_Control.Reserved = 0x00;
	Config_Control.Filter = BME280_FILTER_OFF;
	Config_Control.Standby_t = BME280_STANDBY_TIME_250;

	tempReg = *((uint8_t*) &Config_Control);

	BME280_WriteRegisterData(hi2cx, BME280_CONFIG, tempReg);

	return INIT_SUCCESS;
}

