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

	float rebuf[2]={0};
	uint32_t humi_zheng,humi_xiao;  //湿度的整数小数
	uint32_t temp_zheng,temp_xiao;  //温度的整数小数

	while(1)
	{
		xQueueReceive(Queue_humiture_handle,rebuf,portMAX_DELAY);

		humi_zheng=(uint32_t)rebuf[0];
		humi_xiao=(rebuf[0]-humi_zheng)*10;

		temp_zheng=(uint32_t)rebuf[1];
		temp_xiao=(rebuf[0]-temp_zheng)*10;

		OLED_ShowNum(1,1,humi_zheng,2);
		OLED_ShowChar(1, 3, '.');
		OLED_ShowNum(1,4,humi_xiao,2);

		OLED_ShowNum(2,1,temp_zheng,2);
		OLED_ShowChar(2, 3, '.');
		OLED_ShowNum(2,4,temp_xiao,2);

		vTaskDelay(pdMS_TO_TICKS(10));
	}

}


