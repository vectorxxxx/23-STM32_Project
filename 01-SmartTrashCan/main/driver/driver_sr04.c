#include "driver_sr04.h"                  // Device header

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

uint32_t distance_cm = 0;

void Delay_us(uint16_t us)
{
    uint16_t differ = 0xFFFF - us - 5;
    __HAL_TIM_SET_COUNTER(&htim3, differ);
    HAL_TIM_Base_Start(&htim3);
    
    if (differ < 0xFFFF - 5)
    {
        differ = __HAL_TIM_GET_COUNTER(&htim3);
    }
    
    HAL_TIM_Base_Stop(&htim3);
}  

void SR04_Trigger(void)
{
    TRIG_ON;
    Delay_us(10);           // 10us的TTL触发信号
    TRIG_OFF;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static int count = 0;
    if (GPIO_Pin == GPIO_PIN_4)
    {
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_SET)
        {
            __HAL_TIM_SET_COUNTER(&htim4, 0);
            HAL_TIM_Base_Start(&htim4);
        }
        else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_RESET)
        {
            HAL_TIM_Base_Stop(&htim4);
            count = __HAL_TIM_GET_COUNTER(&htim4);
            distance_cm = count * 340 / 2 * 0.000001 * 100;
            count = 0;
        }
    }
}

uint32_t SR04_GetDistance(void)
{
    return distance_cm;
}

