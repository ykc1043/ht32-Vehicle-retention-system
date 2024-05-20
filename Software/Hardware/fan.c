#include "fan.h"

void Fan_Init(void)
{
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.PD = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
    AFIO_GPxConfig(GPIO_PD, AFIO_PIN_1, AFIO_FUN_GPIO);
    GPIO_PullResistorConfig(HT_GPIOD, GPIO_PIN_1, GPIO_PR_DOWN);
    GPIO_WriteOutBits(HT_GPIOD, GPIO_PIN_1, RESET);
    GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_1, GPIO_DIR_OUT);
}

