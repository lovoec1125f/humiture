/*
 * oled_task.c
 *
 *  Created on: 2026年9月2日
 *      Author: ZhuanZ（无密码）
 */
#include "FreeRTOS.h"
#include "task.h"
#include "oled_task.h"

#include "OLED.h"


//显示温湿度数据(消息队列)
void oled_display_task(void *arg)
{

	while(1)
	{
		OLED_ShowString(1,1,"a");
	}

}


