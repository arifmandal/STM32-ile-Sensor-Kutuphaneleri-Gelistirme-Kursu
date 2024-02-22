/*
 * adxl345.c
 *
 *  Created on: Feb 22, 2024
 *      Author: Arif Mandal
 */

#include "adxl345.h"


/**
  * @brief  To find 7 bits address value
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval int
  */

int ADXL345_ScanDeviceID(I2C_HandleTypeDef *hi2cx){

	for (uint8_t address= 0; address < 255; address++) {
		if (HAL_I2C_IsDeviceReady(hi2cx, address, 1, TIMEOUT) == HAL_OK) {
			return address;
		}
	}
	return -1;
}

static ADXL345ReadStatus ADXL345_ReadRegisterData(I2C_HandleTypeDef *hi2cx, uint16_t registerAddress, uint16_t sizeofData, uint8_t *dataBuffer){

	if (HAL_I2C_Mem_Read(hi2cx, ADXL345_DEVICE_ADDRESS, registerAddress, 1, dataBuffer, sizeofData, TIMEOUT) == HAL_OK) {
		return READ_SUCCESS;

	}
	return READ_FAIL;
}

static ADXL345WriteStatus ADXL345_WriteRegisterData(I2C_HandleTypeDef *hi2cx, uint16_t registerAddress, uint16_t value){

	uint8_t data[2] = {0};
	data[0] = registerAddress;
	data[1] = value;

	if (HAL_I2C_Master_Transmit(hi2cx, ADXL345_DEVICE_ADDRESS, data, sizeof(data), TIMEOUT) == HAL_OK) {
		return Write_SUCCESS;
	}

	return Write_FAIL;
}

ADXL345InitStatus ADXL345_Init(I2C_HandleTypeDef *hi2cx){

	uint8_t dataBuffer = 0;
	ADXL345_ReadRegisterData(hi2cx, DEVID, 1, &dataBuffer);

	if (dataBuffer != 0xE5) {
		return INIT_FAIL;
	}

	uint8_t tempReg = 0;

	PowerControlRegister_t powerControl = {0};

	powerControl.Wakeup = WAKEUP_8HZ;
	powerControl.Sleep = 0x00;
	powerControl.Measure = 0x01;
	powerControl.AUTO_SLEEP = 0x00;
	powerControl.Link = 0x00;
	powerControl.Reserved = 0x00;

	tempReg = *((uint8_t*)&powerControl);

	ADXL345_WriteRegisterData(hi2cx, POWER_CTL, tempReg);

	DataFormatRegister_t  dataFormatControl = {0};
	dataFormatControl.Range = RANGE_4G;
	dataFormatControl.Justify = 0x00;
	dataFormatControl.FULL_RES = 0x00;
	dataFormatControl.Reserved = 0x00;
	dataFormatControl.INT_INVERT = 0x00;
	dataFormatControl.SPI = 0x00;
	dataFormatControl.SELF_TEST = 0x00;

	tempReg = *((uint8_t*)&dataFormatControl);

	ADXL345_WriteRegisterData(hi2cx, DATA_FORMAT, tempReg);


	BWRATERegister_t BWRATEControl = {0};

	BWRATEControl.Range = BANDWIDTH_RATE_800;
	BWRATEControl.LOW_POWER = 0x00;
	BWRATEControl.Reserved = 0x00;

	tempReg = *((uint8_t*)&BWRATEControl);

	ADXL345_WriteRegisterData(hi2cx, BW_RATE, tempReg);

	return INIT_SUCCESS;

}

int16_t ADXL345_getAxisValue(I2C_HandleTypeDef *hi2cx, uint8_t axis){

	uint8_t data[2] = {0};
	int16_t outputData = 0;

	ADXL345_ReadRegisterData(hi2cx, axis, 2, data);

	outputData = ((data[1]<<8) | data[0]);

	return outputData;
}


float ADXL345_getGValue(I2C_HandleTypeDef *hi2cx, uint8_t axis, float scaleFactor){

	int16_t outputData = 0;
	float gData = 0;

	outputData = ADXL345_getAxisValue(hi2cx, axis);
	gData = (float) (outputData * scaleFactor);

	return gData;
}
