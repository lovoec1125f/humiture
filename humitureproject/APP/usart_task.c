/*
 * usart_task.c
 *
 *  Created on: 2026年9月2日
 *      Author: ZhuanZ（无密码）
 */
#include "FreeRTOS.h"
#include "task.h"
#include "usart_task.h"
#include "usart.h"
#include <stdio.h>
#include "semphr.h"

extern SemaphoreHandle_t usart1_mute_handle;



//用于打印输出温湿度数据(消息队列实现)
void usart1_humitur_task(void* arg)
{

	while(1)
	{
		printf("1\r\n");
		vTaskDelay(pdMS_TO_TICKS(1000));

	}
}


//串口日志（互斥锁）
void usart1_log_task(void* arg)
{

	while(1)
	{
		if(xSemaphoreTake(usart1_mute_handle,portMAX_DELAY)==pdPASS)
		{
			printf("2\r\n");
			xSemaphoreGive(usart1_mute_handle);
		}
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}
