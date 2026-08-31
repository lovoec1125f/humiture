/* key.c */
#include "key.h"
#include "main.h"

void Key_Init(void)
{

}

/* 读原始电平（不消抖）：按下返回 0，松开返回 1 */
uint8_t Key_Read(void)
{
    return HAL_GPIO_ReadPin(key_GPIO_Port, key_Pin);
}

/*
 * 带消抖的「按下边沿」检测：需要周期调用（比如在 KeyTask 里每 10ms 调一次）
 * 返回 1 = 检测到一次「按下」；返回 0 = 没按下（或一直按着不动）
 */
uint8_t Key_Scan(void)
{
    static uint8_t stable     = 1;   /* 当前稳定电平（1=松开，上拉默认高） */
    static uint8_t last_stable= 1;   /* 上一次稳定电平 */
    static uint8_t cnt        = 0;

    uint8_t now = HAL_GPIO_ReadPin(key_GPIO_Port, key_Pin);

    if (now != stable)            /* 电平变了，开始计数消抖 */
    {
        cnt++;
        if (cnt >= 3)             /* 连续 3 次(约30ms)相同才确认，防抖动 */
        {
            last_stable = stable;
            stable = now;
            cnt = 0;
            if (last_stable == 1 && stable == 0)   /* 松开→按下 的边沿 */
                return 1;
        }
    }
    else
        cnt = 0;

    return 0;
}
