#ifndef __DHT11_H__
#define __DHT11_H__

#include "main.h"

#define u8                  unsigned char
#define u16                 unsigned short
#define u32                 unsigned int
        
#define DATA_GPIO_Port      GPIOA
#define DATA_GPIO_Pin       GPIO_PIN_7

#define DATA_SET()          HAL_GPIO_WritePin(DATA_GPIO_Port, DATA_GPIO_Pin, GPIO_PIN_SET)
#define DATA_RESET()        HAL_GPIO_WritePin(DATA_GPIO_Port, DATA_GPIO_Pin, GPIO_PIN_RESET)

#define DATA_READ()         HAL_GPIO_ReadPin(DATA_GPIO_Port, DATA_GPIO_Pin)

typedef struct
{
    u8 DATA[5];
    u8 index;
    u8 temperature;
    u8 humidity;
}DHT11_DATA;

extern DHT11_DATA DHT11_Data;

void DHT11_Task(void);

#endif
