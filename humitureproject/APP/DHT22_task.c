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
	while(1){
		printf("start\r\n");
		//读取温湿度数据测试
		if(dht22_get(&humi,&temp)==1)
		{
		  printf("shidu:%f,wendu:%f\r\n",humi,temp);
		}else{
			printf("error\r\n");
		}
		vTaskDelay(2000);
	}
}


