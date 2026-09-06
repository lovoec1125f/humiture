/*
 * DHT22.c
 */
#include "main.h"
#include "DHT22.h"
#include "DWT.h"
#include <stdio.h>

// 定义超时宏（72MHz下，1us=72个周期。100us=7200周期）
#define DWT_100US  (uint32_t)(100 * (SystemCoreClock / 1000000))


static void dht22_start(void)
{
    HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_RESET);
    dwt_delay_us(2000); // 拉低2ms

    HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_SET);
    dwt_delay_us(40);   // 释放40us
}

// 获取某一位是1还是0，以高电平持续时间区分
static uint8_t dht22_getbit(void)
{
    uint32_t start = 0;
    uint32_t hightime = 0;

    // 1. 等待低电平过去(50us)，加超时
    start = DWT->CYCCNT;
    while(HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) == GPIO_PIN_RESET) {
        if ((DWT->CYCCNT - start) > DWT_100US) return 0; // 超时返回0
    }

    // 2. 获取高电平时间
    start = DWT->CYCCNT;
    while(HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) == GPIO_PIN_SET) {
        if ((DWT->CYCCNT - start) > DWT_100US) return 0; // 超时返回0
    }
    hightime = (DWT->CYCCNT - start) / 72;

    // 3. 数据0高电平约26-28us，数据1高电平约70us。超过50us判为1
    if (hightime >= 50) {
        return 1;
    } else {
        return 0;
    }
}

// 获取一个字节数据，DHT22先发高位
static uint8_t dht22_getbyte(void)
{
    uint8_t data = 0;
    for(int i = 0; i < 8; i++) {
        data <<= 1;
        data |= dht22_getbit();
    }
    return data;
}

// 主机读取整个数据
//8位整数相对湿度数据+8位小数相对湿度数据+8位整数温度数据+8位小数温度数据+8位校验和
uint8_t dht22_get(ht_data *shuju)
{
    uint8_t data[5] = {0};
    uint32_t start = 0;

    // 1. 主机发送起始信号D
    dht22_start();

    // 2. 检测外部响应信号（全部加超时！）
    // 等待低电平（传感器响应）
    start = DWT->CYCCNT;
    while(HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) == GPIO_PIN_SET) {
        if ((DWT->CYCCNT - start) > DWT_100US) return 0;
    }

    // 等待高电平（响应结束，准备发数据）
    start = DWT->CYCCNT;
    while(HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) == GPIO_PIN_RESET) {
        if ((DWT->CYCCNT - start) > DWT_100US) return 0;
    }

    // 等待低电平（第一位数开始）
    start = DWT->CYCCNT;
    while(HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) == GPIO_PIN_SET) {
        if ((DWT->CYCCNT - start) > DWT_100US) return 0;
    }

    // 3. 读取5个字节
    for(int i = 0; i < 5; i++) {
        data[i] = dht22_getbyte();
    }

    // 4. 校验数据
    if ((uint8_t)(data[0] + data[1] + data[2] + data[3]) == data[4]) {


        uint16_t rel_hum = (data[0] << 8) | data[1];
        uint16_t rel_tem = (data[2] << 8) | data[3];

        //只记录整数了，不转换成浮点数
	    // 湿度拆分
		shuju->humi_zheng = rel_hum / 10;
		shuju->humi_xiao = rel_hum % 10;

		// 温度拆分（处理负数）
		if (rel_tem & 0x8000) {
			rel_tem = (~rel_tem) + 1; // 取反加一得到绝对值
			shuju->temp_zheng = - (rel_tem / 10); // 整数部分带负号
			shuju->temp_xiao = rel_tem % 10;
		} else {
			shuju->temp_zheng = rel_tem / 10;
			shuju->temp_xiao = rel_tem % 10;
		}


		//这部分转换成浮点数，不好用
		/* *humidity = rel_hum / 10.0;
		if (rel_tem & 0x8000) { // 负温度处理
			rel_tem = (~rel_tem) + 1;
		 	 *temperature = -((float)rel_tem) / 10.0;
		} else {
		 	 *temperature = (float)rel_tem / 10.0;
		}*/
        return 1; // 成功
    }

    return 0; // 失败
}
