#include "key.h"
void Key_Init(void)
{

    CKCU_PeripClockConfig_TypeDef CCLOCK = {{0}}; // 不开启外设时钟相应功能无法使用
    CCLOCK.Bit.PA = 1;                            // 开启PA时钟
    CCLOCK.Bit.PB = 1;                            // 开启PB时钟
    CCLOCK.Bit.PC = 1;                            // 开启PC时钟
    CCLOCK.Bit.PD = 1;                            // 开启PD时钟
    CCLOCK.Bit.AFIO = 1;                          // 开启复用功能时钟
    CKCU_PeripClockConfig(CCLOCK, ENABLE);        // 使能时钟
	
	HT_GPIO_TypeDef *GPIO_GROUP;
    // PB15
    
    GPIO_GROUP = HT_GPIOB;
    AFIO_GPxConfig(GPIO_PB, GPIO_PIN_15, AFIO_MODE_DEFAULT);
    GPIO_InputConfig(GPIO_GROUP, GPIO_PIN_15, ENABLE);
    // PB 0 10 11 12
    //HT_GPIO_TypeDef *GPIO_GROUP;
    GPIO_GROUP = HT_GPIOC;
    AFIO_GPxConfig(GPIO_PC, GPIO_PIN_0 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12, AFIO_MODE_DEFAULT);
    GPIO_InputConfig(GPIO_GROUP, GPIO_PIN_0 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12, ENABLE);
}


