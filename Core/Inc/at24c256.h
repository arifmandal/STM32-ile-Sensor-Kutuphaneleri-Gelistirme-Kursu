/*
 * at24c256.h
 *
 *  Created on: Mar 28, 2024
 *      Author: Arif Mandal
 */

#ifndef INC_AT24C256_H_
#define INC_AT24C256_H_

#include "main.h"
#include "string.h"

#define AT24C256_DEVICE_ADDRESS  0xA0

/*The 256K is internally organized as 512 pages of 64-bytes each*/

#define AT24C256_PAGE_NUMBER     512
#define AT24C256_PAGE_SIZE       64

#define TIMEOUT                  1000

typedef enum{
	INIT_FAIL = 0,
	INIT_SUCCESS = 1,

}AT24C256InitStatus;

typedef enum{
	WRITE_FAIL = 0,
	WRITE_SUCCESS = 1,

}AT24C256WriteStatus;


typedef enum{
	READ_FAIL = 0,
	READ_SUCCESS = 1,

}AT24C256ReadStatus;


typedef enum{
	ERASE_FAIL = 0,
	ERASE_SUCCESS = 1,

}AT24C256EraseStatus;

AT24C256InitStatus AT24C256_Init(I2C_HandleTypeDef *hi2cx);
AT24C256WriteStatus AT24C256_Write(I2C_HandleTypeDef *hi2cx, uint16_t MemAddress, uint8_t *pData, uint16_t size);
AT24C256ReadStatus AT24C256_Read(I2C_HandleTypeDef *hi2cx, uint16_t MemAddress, uint8_t *pData, uint16_t size);
AT24C256EraseStatus AT24C256_Erase(I2C_HandleTypeDef *hi2cx);


#endif /* INC_AT24C256_H_ */
