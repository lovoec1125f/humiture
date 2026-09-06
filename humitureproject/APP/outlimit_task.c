/*
 * outlimit_task.c
 *
 *  Created on: 2026年9月6日
 *      Author: ZhuanZ（无密码）
 */

#include "outlimit_task.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "semphr.h"


void led_task(void* arg)
{
	uint8_t i;
	while(1)
	{

		if(xSemaphoreTake(erzhi_t,portMAX_DELAY)==pdTRUE)
		{
			// 收到报警信号，快速闪烁5次作为报警（每次翻转间隔100ms）
				for(i = 0; i < 5; i++)
				{
					HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
					vTaskDelay(pdMS_TO_TICKS(100)); // 加延时，100ms翻转一次
				}

				// 闪烁结束后，确保把灯彻底关掉
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

