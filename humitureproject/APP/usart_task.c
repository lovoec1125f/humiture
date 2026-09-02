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
		//互斥信号量确保printf不能被多个任务同时调用
		if(xSemaphoreTake(usart1_mute_handle,portMAX_DELAY)==pdPASS)
		{
			// 打印系统运行时间（Tick）和剩余的堆内存
			            printf("[LOG] System alive, Tick: %lu, Free Heap: %u bytes\r\n",
			                   (unsigned long)xTaskGetTickCount(),   //获取系统从启动（调用 vTaskStartScheduler()）到当前时刻，总共经过了多少个时钟节拍
			                   (unsigned int)xPortGetFreeHeapSize());  //获取 FreeRTOS 堆管理器中当前剩余的空闲内存大小（单位：字节）
			xSemaphoreGive(usart1_mute_handle);
		}
		vTaskDelay(pdMS_TO_TICKS(5000));
	}
}
