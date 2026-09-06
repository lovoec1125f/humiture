/*
 * DHT22_task.c
 *
 *  Created on: 2026年9月2日
 *      Author: ZhuanZ（无密码）
 */
#include "DHT22_task.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "DHT22.h"
#include <stdio.h>
#include "queue.h"
#include "semphr.h"


extern ht_data data;

void dht22_read_task(void *arg)
{

	TickType_t xlastwaketime=xTaskGetTickCount();
    uint8_t  getstate=0;

	while(1){

		taskENTER_CRITICAL();
		getstate=dht22_get(&data);  //读取数据，返回值1表示读取成功
		taskEXIT_CRITICAL();

		//读取温湿度数据测试
		if(getstate==1) //读取数据成功
		{

			if(xQueueSend(Queue_humiture_handle,&data,0)!=pdPASS)  //消息队列传给串口和oled
			{
				printf("队列满了，数据丢失\r\n");
			}
			if((data.humi_zheng>=65)||(data.temp_zheng>=40)||(data.temp_zheng<=10))  //超限任务，二值信号量传
			{
				if(xSemaphoreGive(erzhi_t)!=pdTRUE)
				{
					printf("二值信号量give失败\r\n");
				}
			}
		}else{
			printf("error\r\n");
		}
		vTaskDelayUntil(&xlastwaketime,pdMS_TO_TICKS(2000));
//		vTaskDelay(pdMS_TO_TICKS(30));

	}
}


