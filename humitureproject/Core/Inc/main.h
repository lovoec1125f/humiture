/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define key_Pin GPIO_PIN_0
#define key_GPIO_Port GPIOA
#define beek_Pin GPIO_PIN_0
#define beek_GPIO_Port GPIOB
#define DHT22_Pin GPIO_PIN_12
#define DHT22_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

//温湿度的整数小数
typedef struct humiture_data{
	uint8_t humi_zheng;  //湿度
	uint8_t humi_xiao;
	uint8_t temp_zheng;  //温度
	uint8_t temp_xiao;
}ht_data;


//温湿度数据
 //float humi, temp;
extern ht_data data;

//任务句柄
extern TaskHandle_t keyhandle;
extern TaskHandle_t usart1_1handle;
extern TaskHandle_t usart1_log_handle;
extern TaskHandle_t oled_handle;
extern TaskHandle_t DHT22_handle;

//互斥锁句柄
extern SemaphoreHandle_t usart1_mute_handle;
//二值信号量
extern SemaphoreHandle_t erzhi_t;
//消息队列句柄
extern QueueHandle_t  Queue_humiture_handle;


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
