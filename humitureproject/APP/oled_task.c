/*
 * oled_task.c
 *
 *  Created on: 2026年9月2日
 *      Author: ZhuanZ（无密码）
 */
#include "FreeRTOS.h"
#include "task.h"
#include "oled_task.h"
#include "semphr.h"
#include "queue.h"
#include "main.h"

#include "OLED.h"


//显示温湿度数据(消息队列)
void oled_display_task(void *arg)
{

	uint8_t rebuf[4]={0};


	while(1)
	{
		xQueueReceive(Queue_humiture_handle,rebuf,portMAX_DELAY);



		OLED_ShowNum(1,1,rebuf[0],2);
		OLED_ShowChar(1, 3, '.');
		OLED_ShowNum(1,4,rebuf[1],2);

		OLED_ShowNum(2,1,rebuf[2],2);
		OLED_ShowChar(2, 3, '.');
		OLED_ShowNum(2,4,rebuf[3],2);

		vTaskDelay(pdMS_TO_TICKS(10));
	}

}


