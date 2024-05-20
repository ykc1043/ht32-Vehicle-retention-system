#ifndef __FAN_H__
#define __FAN_H__

#include "ht32.h"
#include "delay.h"


#define 	Fan_port 	HT_GPIOD
#define 	Fan_pin		GPIO_PIN_1

#define 	Fan_on()	GPIO_SetOutBits(Fan_port, Fan_pin)
#define 	Fan_off()	GPIO_ClearOutBits(Fan_port, Fan_pin)

void Fan_Init(void);

#endif
