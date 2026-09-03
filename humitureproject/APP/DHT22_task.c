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

extern float humi, temp;

void dht22_read_task(void *arg)
{

	TickType_t xlastwaketime=xTaskGetTickCount();
    uint8_t  getstate=0;

	taskENTER_CRITICAL();
	getstate=dht22_get(&humi,&temp);
	taskEXIT_CRITICAL();

	while(1){
		printf("start\r\n");

		//读取温湿度数据测试
		if(getstate==1)
		{
			printf("shidu:%.1f%%, wendu:%.1f°C\r\n", humi, temp);
		}else{
			printf("error\r\n");
		}
		vTaskDelayUntil(&xlastwaketime,pdMS_TO_TICKS(2000));
//		vTaskDelay(pdMS_TO_TICKS(30));

	}
}


