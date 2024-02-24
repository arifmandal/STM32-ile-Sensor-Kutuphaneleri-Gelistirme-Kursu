/*
 * bme280.h
 *
 *  Created on: Feb 24, 2024
 *      Author: Arif Mandal
 */

#ifndef INC_BME280_H_
#define INC_BME280_H_

#include "main.h"


#define BME280_DEVICE_ADDRESS  0xEC

#define BME280_ID              0xD0
#define BME280_RESET           0xE0
#define BME280_CTRL_HUM        0xF2
#define BME280_STATUS          0xF3
#define BME280_CTRL_MEAS  	   0xF4
#define BME280_CONFIG     	   0xF5
#define BME280_PRESS_MSB  	   0xF7
#define BME280_PRESS_LSB  	   0xF8
#define BME280_PRESS_XLSB 	   0xF9
#define BME280_TEMP_MSB   	   0xFA
#define BME280_TEMP_LSB   	   0xFB
#define BME280_TEMP_XLSB  	   0xFC
#define BME280_HUM_MSB    	   0xFD
#define BME280_HUM_LSB    	   0xFE
#define BME280_CALIB00         0x88
#define BME280_CALIB26         0xE1


#define TIMEOUT                1000

/* @def_group oversampling */
#define BME280_OVERSAMPLING_SKIPPED  0x00
#define BME280_OVERSAMPLING_1  		 0x01
#define BME280_OVERSAMPLING_2  		 0x02
#define BME280_OVERSAMPLING_4  		 0x03
#define BME280_OVERSAMPLING_8        0x04
#define BME280_OVERSAMPLING_16       0x05

/* @def_group mode */
#define BME280_SLEEP_MODE    0x00
#define BME280_FORCED_MODE   0x01
#define BME280_NORMAL_MODE   0x03

/* @def_group Filter */
#define BME280_FILTER_OFF    0x00
#define BME280_FILTER_2      0x01
#define BME280_FILTER_4      0x02
#define BME280_FILTER_8      0x03
#define BME280_FILTER_16     0x04

/* @def_group Standby_time */
#define BME280_STANDBY_TIME_05       0x00
#define BME280_STANDBY_TIME_62_5     0x01
#define BME280_STANDBY_TIME_125      0x02
#define BME280_STANDBY_TIME_250      0x03
#define BME280_STANDBY_TIME_500      0x04
#define BME280_STANDBY_TIME_1000     0x05
#define BME280_STANDBY_TIME_2000     0x06
#define BME280_STANDBY_TIME_4000     0x07

typedef enum{
	READ_FAIL = 0,
	READ_SUCCESS = 1,
}BME280ReadStatus;

typedef enum{
	Write_FAIL = 0,
	Write_SUCCESS = 1,
}BME280WriteStatus;

typedef enum{
	INIT_FAIL = 0,
	INIT_SUCCESS = 1,
}BME280InitStatus;

typedef struct{

	uint8_t OSRS_H: 3; /* Bits 0-2 @def_group oversampling */
	uint8_t Reserved: 5; /* Bits 3-7  @def_group oversampling */

}CTRL_HUM_Register_t;

typedef struct{

	uint8_t Mode: 2; /* Bits 0-1 @def_group mode */
	uint8_t OSRS_P: 3; /* Bits 2-4 @def_group oversampling */
	uint8_t OSRS_T: 3; /* Bits 5-7 @def_group oversampling */

}CTRL_MEAS_Register_t;

typedef struct{

	uint8_t SPI_EN: 1; /* Bits 0*/
	uint8_t Reserved: 1; /* Bits 1*/
	uint8_t Filter: 3; /* Bits 2-4 @def_group Filter */
	uint8_t Standby_t: 3; /* Bits 5-7 @def_group Standby_time */

}Config_Register_t;


int BME280_ScanDeviceID(I2C_HandleTypeDef *hi2cx);

BME280ReadStatus BME280_ReadRegisterData(I2C_HandleTypeDef *hi2cx, uint16_t registerAddress, uint16_t sizeofData, uint8_t *dataBuffer);

BME280ReadStatus BME280_ReadRegister16Data(I2C_HandleTypeDef *hi2cx, uint16_t registerAddress, uint16_t sizeofData, uint16_t *dataBuffer);

BME280WriteStatus BME280_WriteRegisterData(I2C_HandleTypeDef *hi2cx, uint16_t registerAddress, uint16_t value);

BME280InitStatus BME280_Init(I2C_HandleTypeDef *hi2cx);
#endif /* INC_BME280_H_ */
