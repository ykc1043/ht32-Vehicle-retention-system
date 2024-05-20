#include "buzzer.h"

void Buzzer_Init(void)
{
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.PD = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
    AFIO_GPxConfig(GPIO_PD, AFIO_PIN_2, AFIO_FUN_GPIO);
    GPIO_PullResistorConfig(HT_GPIOD, GPIO_PIN_2, GPIO_PR_DOWN);
    GPIO_WriteOutBits(HT_GPIOD, GPIO_PIN_2, RESET);
    GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_2, GPIO_DIR_OUT);
}

void Buzzer_on(uint16_t time_ms)//,uint16_t freq)
{
	for(int i=0;i<time_ms;i++)
	{
		GPIO_SetOutBits(HT_GPIOD, GPIO_PIN_2); // GPIO = HIGH
		delay_us(500);
		GPIO_ClearOutBits(HT_GPIOD, GPIO_PIN_2); // GPIO = LOW
		delay_us(500);
	}
}

