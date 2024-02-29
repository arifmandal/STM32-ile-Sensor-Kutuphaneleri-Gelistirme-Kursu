/*
 * bme280.c
 *
 *  Created on: Feb 24, 2024
 *      Author: Arif Mandal
 */

#include "bme280.h"

uint16_t dig_T1;
int16_t dig_T2;
int16_t dig_T3;
uint16_t dig_P1;
int16_t dig_P2;
int16_t dig_P3;
int16_t dig_P4;
int16_t dig_P5;
int16_t dig_P6;
int16_t dig_P7;
int16_t dig_P8;
int16_t dig_P9;

uint8_t dig_H1;
int16_t dig_H2;
uint8_t dig_H3;
int16_t dig_H4;
int16_t dig_H5;
int8_t dig_H6;
uint16_t h4, h5;

#pragma GCC diagnostic ignored "-Wunused-variable"

static BME280_Calibration_Parameters_t BME280_Calibration_Parameters;

int BME280_ScanDeviceID(I2C_HandleTypeDef *hi2cx) {

	for (uint8_t address = 0; address < 255; address++) {
		if (HAL_I2C_IsDeviceReady(hi2cx, address, 1, TIMEOUT) == HAL_OK) {
			return address;
		}
	}

	return -1; // error

}

static BME280ReadStatus BME280_ReadRegisterData(I2C_HandleTypeDef *hi2cx,
		uint16_t registerAddress, uint16_t sizeofData, uint8_t *dataBuffer) {

	if (HAL_I2C_Mem_Read(hi2cx, BME280_DEVICE_ADDRESS, registerAddress, 1,
			dataBuffer, sizeofData, TIMEOUT) == HAL_OK) {
		return READ_SUCCESS;
	}
	return READ_FAIL;
}

static BME280ReadStatus BME280_ReadRegister16Data(I2C_HandleTypeDef *hi2cx,
		uint16_t registerAddress, uint16_t sizeofData, uint16_t *dataBuffer) {

	uint8_t tempData[2] = { 0 };
	if (HAL_I2C_Mem_Read(hi2cx, BME280_DEVICE_ADDRESS, registerAddress, 1,
			tempData, sizeofData, TIMEOUT) == HAL_OK) {
		*dataBuffer = (uint16_t) ((tempData[1] << 8) | tempData[0]);
		return READ_SUCCESS;
	}
	return READ_FAIL;
}

static BME280WriteStatus BME280_WriteRegisterData(I2C_HandleTypeDef *hi2cx,
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

static void BME280_ReadCalibrationData(I2C_HandleTypeDef *hi2cx) {

	BME280_ReadRegister16Data(hi2cx, BME280_CALIB00, 2, &dig_T1);
	BME280_ReadRegister16Data(hi2cx, BME280_CALIB00 + 0x02, 2,
			(uint16_t*) &dig_T2);
	BME280_ReadRegister16Data(hi2cx, BME280_CALIB00 + 0x04, 2,
			(uint16_t*) &dig_T3);
	BME280_ReadRegister16Data(hi2cx, BME280_CALIB00 + 0x06, 2, &dig_P1);
	BME280_ReadRegister16Data(hi2cx, BME280_CALIB00 + 0x08, 2,
			(uint16_t*) &dig_P2);
	BME280_ReadRegister16Data(hi2cx, BME280_CALIB00 + 0x0A, 2,
			(uint16_t*) &dig_P3);
	BME280_ReadRegister16Data(hi2cx, BME280_CALIB00 + 0x0C, 2,
			(uint16_t*) &dig_P4);
	BME280_ReadRegister16Data(hi2cx, BME280_CALIB00 + 0x0E, 2,
			(uint16_t*) &dig_P5);
	BME280_ReadRegister16Data(hi2cx, BME280_CALIB00 + 0x10, 2,
			(uint16_t*) &dig_P6);
	BME280_ReadRegister16Data(hi2cx, BME280_CALIB00 + 0x12, 2,
			(uint16_t*) &dig_P7);
	BME280_ReadRegister16Data(hi2cx, BME280_CALIB00 + 0x14, 2,
			(uint16_t*) &dig_P8);
	BME280_ReadRegister16Data(hi2cx, BME280_CALIB00 + 0x16, 2,
			(uint16_t*) &dig_P9);

	BME280_ReadRegisterData(hi2cx, BME280_CALIB00 + 0x19, 1, &dig_H1);
	BME280_ReadRegister16Data(hi2cx, BME280_CALIB26, 2, (uint16_t*) &dig_H2);
	BME280_ReadRegisterData(hi2cx, BME280_CALIB26 + 0x02, 1, &dig_H3);
	BME280_ReadRegister16Data(hi2cx, BME280_CALIB26 + 0x03, 2, &h4);
	BME280_ReadRegister16Data(hi2cx, BME280_CALIB26 + 0x04, 2, &h5);
	BME280_ReadRegisterData(hi2cx, BME280_CALIB26 + 0x06, 1,
			(uint8_t*) &dig_H6);

	dig_H4 = (h4 & 0x00ff) << 4 | (h4 & 0x0f00) >> 8;
	dig_H5 = h5 >> 4;

	/* other option
	 BME280_Calibration_Parameters.dig_T1 = dig_T1;
	 BME280_Calibration_Parameters.dig_T2 = dig_T2;
	 ...
	 */

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

	BME280_ReadCalibrationData(hi2cx);

	BME280_ReadRegisterData(hi2cx, BME280_ID, 1, &dataBuffer); //read id

	if (dataBuffer != 0x60) {
		return INIT_FAIL;
	}

	return INIT_SUCCESS;
}

static int32_t BME280_ReadCompensateTemperature(int32_t adc_temp, int32_t *fine_temp) {

	int32_t var1, var2;
	int32_t T = 0;

	var1 = ((((adc_temp >> 3) - ((int32_t) dig_T1 << 1))) * (int32_t) dig_T2)
			>> 11;
	var2 = (((((adc_temp >> 4) - (int32_t) dig_T1)
			* ((adc_temp >> 4) - (int32_t) dig_T1)) >> 12) * (int32_t) dig_T3)
			>> 14;

	*fine_temp = var1 + var2;
	T = (*fine_temp * 5 + 128) >> 8;
	return T;
}

static int32_t BME280_ReadCompensatePressure(int32_t adc_press, int32_t fine_temp) {

	int64_t var1, var2, p;

	var1 = (int64_t) fine_temp - 128000;
	var2 = var1 * var1 * (int64_t) dig_P6;
	var2 = var2 + ((var1 * (int64_t) dig_P5) << 17);
	var2 = var2 + (((int64_t) dig_P4) << 35);
	var1 = ((var1 * var1 * (int64_t) dig_P3) >> 8)
			+ ((var1 * (int64_t) dig_P2) << 12);
	var1 = (((int64_t) 1 << 47) + var1) * ((int64_t) dig_P1) >> 33;

	if (var1 == 0) {
		return 0;  // avoid exception caused by division by zero
	}

	p = 1048576 - adc_press;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = ((int64_t) dig_P9 * (p >> 13) * (p >> 13)) >> 25;
	var2 = ((int64_t) dig_P8 * p) >> 19;

	p = ((p + var1 + var2) >> 8) + ((int64_t) dig_P7 << 4);
	return p;

}

static int32_t BME280_ReadCompensateHumidity(int32_t adc_hum, int32_t fine_temp) {

	int32_t v_x1_u32r;

	v_x1_u32r = fine_temp - (int32_t) 76800;
	v_x1_u32r =
			((((adc_hum << 14) - ((int32_t) dig_H4 << 20)
					- ((int32_t) dig_H5 * v_x1_u32r)) + (int32_t) 16384) >> 15)
					* (((((((v_x1_u32r * (int32_t) dig_H6) >> 10)
							* (((v_x1_u32r * (int32_t) dig_H3) >> 11)
									+ (int32_t) 32768)) >> 10)
							+ (int32_t) 2097152) * (int32_t) dig_H2 + 8192)
							>> 14);
	v_x1_u32r = v_x1_u32r
			- (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7)
					* (int32_t) dig_H1) >> 4);
	v_x1_u32r = v_x1_u32r < 0 ? 0 : v_x1_u32r;
	v_x1_u32r = v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r;
	return v_x1_u32r >> 12;

}

static void BME280_ReadSensorRegister(I2C_HandleTypeDef *hi2cx, int32_t *temperature,
		uint32_t *pressure, uint32_t *humidity) {

	int32_t adcTemperature = 0;
	int32_t adcPressure = 0;
	int32_t adcHumidity = 0;

	uint8_t data[8] = { 0 };
	int32_t fine_temp;

	BME280_ReadRegisterData(hi2cx, BME280_PRESS_MSB, 8, data);

	adcPressure = data[0] << 12 | data[1] << 4 | data[2] >> 4;
	adcTemperature = data[3] << 12 | data[4] << 4 | data[5] >> 4;
	adcHumidity = data[6] << 8 | data[7];

	*temperature = BME280_ReadCompensateTemperature(adcTemperature, &fine_temp);
	*pressure = BME280_ReadCompensatePressure(adcPressure, fine_temp);
	*humidity = BME280_ReadCompensateHumidity(adcHumidity, fine_temp);
}

void BME280_ReadSensorData(I2C_HandleTypeDef *hi2cx, float *temperature,
		float *pressure, float *humidity) {

	int32_t fixed_temperature;
	uint32_t fixed_pressure;
	uint32_t fixed_humidity;

	BME280_ReadSensorRegister(hi2cx, &fixed_temperature, &fixed_pressure,
			&fixed_humidity);

	*temperature = (float) fixed_temperature / 100;
	*pressure = (float) (fixed_pressure / 256) / 100;
	*humidity = (float) fixed_humidity / 1024;
}





