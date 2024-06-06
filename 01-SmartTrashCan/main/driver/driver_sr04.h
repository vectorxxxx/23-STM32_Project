#ifndef __DRIVER_SR04_H__
#define __DRIVER_SR04_H__

#include "main.h"                  // Device header

#define TRIG_ON     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)
#define TRIG_OFF    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET)

void SR04_Trigger(void);

uint32_t SR04_GetDistance(void);

#endif
