/*
 * DHT22.c
 *
 *  Created on: 2026年9月2日
 *      Author: ZhuanZ（无密码）
 */

#include "main.h"
#include "DHT22.h"
#include "DWT.h"


static  void dht22_start(void)
{
	HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_RESET);
	dwt_delay_us(2000);

	HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_SET);
	dwt_delay_us(40);

}

//获取某一位是1还是0，以高电平持续时间区分
static uint8_t dht22_getbit(void)
{
	uint32_t  hightime=0;
	uint32_t start=0;


	while(HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin)==GPIO_PIN_RESET){

	}//等待低电平过去(50us)

	start=DWT->CYCCNT;
	//获取高电平时间
	while(HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin)==GPIO_PIN_SET){

	}
	hightime=(DWT->CYCCNT-start)/72;
	if(hightime>=50){
		return 1;
	}else{
		return 0;
	}

}

//获取一个字节数据 dht22先发高位
static uint8_t dht22_getbyte(void)
{
	uint8_t data=0;
	for(int i=0;i<8;i++){
		data<<=1;
		data|=dht22_getbit();
	}
	return data;
}


//主机读取整个数据，湿度整数+湿度小数+温度整数+温度小数+校验位
uint8_t  dht22_get(float *humidity, float *temperature){

	uint8_t data[5]={0};

	//主机发送信号

	dht22_start();

	//检测外部信号
	while(HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin)==GPIO_PIN_SET); //主机发送了高电平，等待响应，直到有响应，否则高电平就一直循环
	while(HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin)==GPIO_PIN_RESET);//(传感器自己拉低的，作为响应)
	while(HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin)==GPIO_PIN_SET); //(传感器准备发送数据的信号)

	//传感器开始发送数据，主机接收数据
	for(int i=0;i<5;i++){
		data[i]=dht22_getbyte();
	}

	//获取数据
	if(data[4]==data[0]+data[1]+data[2]+data[3])  //校验数据对不对
	{

		   //拼接实际的温湿度数据
			uint16_t  rel_hum=(data[0]<<8)|data[1];
			uint16_t  rel_tem=(data[2]<<8)|data[3];

			*humidity=rel_hum/10.0;

			//处理负温度（首位为1） DHT22 在传输负温度时，直接发送的就是该负温度的二进制补码
			if(rel_tem&0x8000){
				rel_tem=(~rel_tem)+1;//对一个负数的补码进行“取反加一”操作，得到的结果恰好是它的绝对值（正数）
				*temperature=-(rel_tem)/10.0;
			}else{
				*temperature=-(rel_tem)/10.0;
			}

			return 1;//读取成功


	}
	else
		return 0;




}














