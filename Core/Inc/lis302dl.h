/*
 * lis302dl.h
 *
 *  Created on: Mar 9, 2024
 *      Author: Arif Mandal
 */

#ifndef INC_LIS302DL_H_
#define INC_LIS302DL_H_

#include "main.h"

#define LIS302DL_WHO_AM_I               0x0F
#define LIS302DL_CTRL_REG1              0x20
#define LIS302DL_CTRL_REG2              0x21
#define LIS302DL_CTRL_REG3              0x22
#define LIS302DL_HP_FILTER_RESET        0x23
#define LIS302DL_STATUS_REG             0x27
#define LIS302DL_OUT_X              	0x29
#define LIS302DL_OUT_Y              	0x2B
#define LIS302DL_OUT_Z              	0x2D

#define LIS302DL_READ                   0x80
#define LIS302DL_WRITE                  0x00

#define LIS302DL_SENSITIVITY_2G            0.0018
#define LIS302DL_SENSITIVITY_8G            0.0072

#define LIS302DL_CS_LOW					HAL_GPIO_WritePin(GPIOE, CS_Pin, GPIO_PIN_RESET);
#define LIS302DL_CS_HIGH		    	HAL_GPIO_WritePin(GPIOE, CS_Pin, GPIO_PIN_SET);

#define LIS302DL_100HZ                  0X00
#define LIS302DL_400HZ                  0X01

#define LIS302DL_POWER_DOWN                0X00
#define LIS302DL_ACTIVE_MODE               0X01

#define LIS302DL_2G                0X00
#define LIS302DL_8G                0X01

#define LIS302DL_SELF_TEST_DISABLE               0X00
#define LIS302DL_SELF_TEST_ENABLE                0X01

typedef enum {
	INIT_FAIL = 0, INIT_SUCCESS = 1,
} LIS302DLInitStatus;

typedef struct {
	uint8_t Xen :1;
	uint8_t Yen :1;
	uint8_t Zen :1;
	uint8_t STM :1;
	uint8_t STP :1;
	uint8_t FS :1;
	uint8_t PD :1;
	uint8_t DR :1;

} CTRL_REG1_Register_t;

uint8_t LIS302DL_ReadWhoAmI(SPI_HandleTypeDef *hpsi);
LIS302DLInitStatus LIS302DL_Init(SPI_HandleTypeDef *hpsi);
void LIS302DL_ReadAccel(SPI_HandleTypeDef *hpsi, int8_t *accelData);
void LIS302DL_ReadgData(SPI_HandleTypeDef *hpsi, int8_t *accelData, float *gData);

#endif /* INC_LIS302DL_H_ */
