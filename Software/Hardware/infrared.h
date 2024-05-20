#ifndef _INFRARED_H_
#define _INFRARED_H_

#include "ht32f5xxxx_ckcu.h"
#include "ht32f5xxxx_gpio.h"
#include "ht32f5xxxx_01.h"

//端口号定义
#define HT_GPIO_PORT_infrared HT_GPIOB

//引脚定义
#define infrared_GPIO_PIN  GPIO_PIN_3
#define Read_STATE() GPIO_ReadInBit(HT_GPIO_PORT_infrared,infrared_GPIO_PIN)   
void Infrared_Init(void);
uint8_t  infrared_test(void);
#endif
