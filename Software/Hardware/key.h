#ifndef __KEY_H__
#define __KEY_H__

#include "ht32.h"
#include "multi_button/multi_button.h"


void Key_Init(void);


#define PB15 GPIO_ReadInBit(HT_GPIOB, GPIO_PIN_15)
#define PC0  GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_0)
#define PC10  GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_10)
#define PC12  GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_12)
#define PC11  GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_11)



#endif
