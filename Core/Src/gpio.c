/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
#include "delay.h"
#include "run.h"
#include "single_mode.h"

#include "display.h"
#include "cmd_link.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
 

 

  /*Configure GPIO pins Output : PA2 PA3 PA4 PA5 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins output :  PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//  /*Configure GPIO pins input : PB2 PB10 PB11 PB12 */
//  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = GPIO_PULLDOWN ;//GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);



	/*Configure GPIO pins : PBPin PBPin PBPin PBPin */
	 GPIO_InitStruct.Pin = TOUCH_KEY_ADD_Pin|TOUCH_KEY_POWER_Pin|TOUCH_KEY_MODE_Pin|TOUCH_KEY_DEC_Pin;
	 GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
	 HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	 /* EXTI interrupt init*/
	 HAL_NVIC_SetPriority(EXTI2_3_IRQn, 2, 0);
	 HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
	
	 HAL_NVIC_SetPriority(EXTI4_15_IRQn, 1, 0);
	 HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);


}

/* USER CODE BEGIN 2 */
//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号
//void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin)
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    static uint8_t power_on_flag_times,key_has_read_value=0,read_key_value;
    delay_ms(1);      //消抖
    switch(GPIO_Pin)
    {
        case TOUCH_KEY_POWER_Pin:

		read_key_value = power_key;
		key_has_read_value =1;
            break;
        case TOUCH_KEY_MODE_Pin:
             read_key_value = model_key;
		key_has_read_value =1;
             
            
            break;
        case TOUCH_KEY_ADD_Pin:
           read_key_value = add_key;
		key_has_read_value =1;

		   
            
            break;
        case TOUCH_KEY_DEC_Pin:
         
		read_key_value = dec_key;
		key_has_read_value =1;
            
            break;
    }

	if(key_has_read_value==1){
         key_has_read_value=0;


	     switch(read_key_value){

		 case power_key:
		    run_t.key_value= power_key;
		    read_key_value =0xff;
		 break;

		 case model_key:
			run_t.key_value= model_key;
			read_key_value =0xff;
		 break;

		 case add_key:
		     run_t.key_value= add_key;
			 read_key_value =0xff;

		 break;

		 case dec_key:
		    run_t.key_value= dec_key;
			read_key_value =0xff;

		 break;

		 
		

	     }

	}
}
/* USER CODE END 2 */
