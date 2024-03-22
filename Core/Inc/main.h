/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CS_Pin GPIO_PIN_3
#define CS_GPIO_Port GPIOE
#define MENU_Pin GPIO_PIN_0
#define MENU_GPIO_Port GPIOA
#define SELECT_Pin GPIO_PIN_1
#define SELECT_GPIO_Port GPIOA
#define ESCAPE_Pin GPIO_PIN_2
#define ESCAPE_GPIO_Port GPIOA
#define encoderSwitch_Pin GPIO_PIN_10
#define encoderSwitch_GPIO_Port GPIOE
#define green_Pin GPIO_PIN_12
#define green_GPIO_Port GPIOD
#define orange_Pin GPIO_PIN_13
#define orange_GPIO_Port GPIOD
#define red_Pin GPIO_PIN_14
#define red_GPIO_Port GPIOD
#define blue_Pin GPIO_PIN_15
#define blue_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
