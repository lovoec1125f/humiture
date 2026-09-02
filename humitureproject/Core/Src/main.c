/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "key.h"
#include  <stdio.h>
#include "semphr.h"
#include "OLED.h"

#include "key_task.h"
#include "usart_task.h"
#include "oled_task.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

	//任务句柄
	TaskHandle_t keyhandle;
	TaskHandle_t usart1_1handle;
	TaskHandle_t usart1_log_handle;
	TaskHandle_t oled_handle;

	//互斥锁句柄
	SemaphoreHandle_t usart1_mute_handle;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  Key_Init();

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
 
  //可获取从机地址（仅限一个）
  //for (uint8_t addr = 1; addr < 127; addr++) {
  //    if (HAL_I2C_IsDeviceReady(&hi2c1, addr, 1, 100) == HAL_OK) {
  //        printf("I2C设备在地址: 0x%02X\r\n", addr);
  //    }
  // }

  OLED_Init();         // 要在MX_I2C1_Init();之后才行

  //创建互斥锁
  usart1_mute_handle=xSemaphoreCreateMutex();

  //创建任务
  //按键任务
  if (xTaskCreate(KeyTask,"key",128,NULL,1,&keyhandle)!= pdPASS) {
      // 如果创建失败，说明堆内存不够，直接停在这里
	  printf("keytask任务创建失败");
      while(1);
  }
  //串口打印温湿度数据
  if ( xTaskCreate(usart1_humitur_task,"usart1_1",128,NULL,1,&usart1_1handle)!= pdPASS) {
       // 如果创建失败，说明堆内存不够，直接停在这里
	   printf("usart1_1任务创建失败");
       while(1);
   }
  //串口日志
  if ( xTaskCreate(usart1_log_task,"usart1_log",128,NULL,1,&usart1_log_handle)!= pdPASS) {
         // 如果创建失败，说明堆内存不够，直接停在这里
  	   printf("usart1_log_task任务创建失败");
         while(1);
   }
  //oled任务
  if ( xTaskCreate(oled_display_task,"oled",128,NULL,1,&oled_handle)!= pdPASS) {
           // 如果创建失败，说明堆内存不够，直接停在这里
    	   printf("oled_display_task任务创建失败");
           while(1);
     }



  vTaskStartScheduler();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//串口重定向
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

//内核断言
/**
 * @brief FreeRTOS 内核断言 (configASSERT) 说明
 *
 * 【什么时候触发】（通常是操作系统级的使用错误，而非普通业务逻辑错误）：
 * 1. 传参错误：传入无效句柄（如向 xSemaphoreTake 传 NULL）。
 * 2. 违规调用：在中断服务函数 (ISR) 里调用了阻塞型API（如 xSemaphoreTake / vTaskDelay）。
 * 3. 优先级越界：外设中断优先级高于系统允许的上限（configMAX_SYSCALL_INTERRUPT_PRIORITY）。
 * 4. 内核状态异常：内存被踩踏，导致内核底层队列/信号量指针损坏。
 *
 * 【断言内部一般干什么】：
 * 1. 打印/记录现场：自定义实现里通过 printf 输出触发断言的 __FILE__（文件）和 __LINE__（行号）。
 * 2. 冻结系统：调用 taskDISABLE_INTERRUPTS() 关闭中断，并进入 for(;;); 死循环。
 * 3. 目的：强制暂停运行，防止系统在严重错误状态下继续执行导致更多数据损坏，方便开发者定位问题。
 */
void vAssertCalled( const char *pcFile, int line ){
	taskDISABLE_INTERRUPTS();  // 关掉中断避免死循环时继续进中断
	printf("\r\n[CRITICAL ERROR] FreeRTOS Assert Failed!\r\n");
	printf("File: %s\r\n", pcFile);
	printf("Line: %d\r\n", line);
    printf("System Halted.\r\n");

    for( ;; ); // 死循环

}



/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
