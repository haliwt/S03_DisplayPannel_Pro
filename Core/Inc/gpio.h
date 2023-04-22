/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
//下面的方式是通过直接操作HAL库函数方式读取IO
#define POWER_KEY()        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)  //KEY0按键PE4
#define MODE_KEY()        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)  //KEY1按键PE3
#define ADD_KEY()        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) 	//KEY2按键PE2
#define DEC_KEY()       HAL_GPIO_ReadPin(GPIOB ,GPIO_PIN_12)  //WKUP按键PA0

#define INTERRUPT_KEY()    HAL_GPIO_ReadPin(GPIOB ,GPIO_PIN_13)  //WKUP按键PA0
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

