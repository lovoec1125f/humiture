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

//用于打印输出温湿度数据
void usart1_humitur_task(void* arg)
{

	while(1)
	{
		printf("hello world\r\n");
		vTaskDelay(pdMS_TO_TICKS(1000));

	}
}
