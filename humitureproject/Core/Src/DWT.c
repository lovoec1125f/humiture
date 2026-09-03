/*
 * DWT.c
 *
 *  Created on: 2026年9月3日
 *      Author: ZhuanZ（无密码）
 */

#include "main.h"

//DWT初始化
void dwt_init(void)
{
	CoreDebug->DEMCR|=CoreDebug_DEMCR_TRCENA_Msk;  //使能DWT外设
	DWT->CYCCNT=0;//清零CYCCNT
	DWT->CTRL|=DWT_CTRL_CYCCNTENA_Msk; //使能cyccnt
}

//使用DWT实现微秒级延时
void dwt_delay_us(uint32_t us)
{
	uint32_t  start,ticks;
	start=DWT->CYCCNT;
	ticks=us*(72000000/100000);  //(72000000/100000)72MHz的话，这个数是1us，ticks为需要的时钟周期数
    while((DWT->CYCCNT-start)<ticks);  //小于就一直等DWT_CYCCNT自增直到达到要求延时


}
