#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "key.h"
#include "key_task.h"
#include "queue.h"

void KeyTask(void *argument)
{

	uint8_t key_num;
    while(1)
    {

        key_num=Key_Scan();
        if(key_num==1)
        {
            HAL_GPIO_TogglePin (LED_GPIO_Port, LED_Pin);
        }
        vTaskDelay(10);
        //vTaskDelay(500);
    }
}
