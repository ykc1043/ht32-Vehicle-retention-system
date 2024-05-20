#include "led.h"

/*********************************************************************************************************//**
  * @brief  Configure the GPIO as output mode.
  * @retval None
  ***********************************************************************************************************/
void GPIO_Configuration(void)
{
//  { /* Enable peripheral clock                                                                              */
//    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
//    CKCUClock.Bit.AFIO = 1;
//    CKCUClock.Bit.PC = 1;
//    CKCU_PeripClockConfig(CKCUClock, ENABLE);
//  }

//  { /* Configure GPIO as output mode                                                                        */

//    /* Configure AFIO mode as GPIO                                                                          */
//    AFIO_GPxConfig(GPIO_PC, AFIO_PIN_15, AFIO_FUN_GPIO);

//    /* Configure GPIO pull resistor                                                                         */
//    GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_15, GPIO_PR_DOWN);

//    /* Default value RESET/SET                                                                              */
//    GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_15, RESET);

//    /* Configure GPIO direction as output                                                                   */
//    GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_15, GPIO_DIR_OUT);
//  }


  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.PB = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  { /* Configure GPIO as output mode                                                                        */

    /* Configure AFIO mode as GPIO                                                                          */
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_6, AFIO_FUN_GPIO);

    /* Configure GPIO pull resistor                                                                         */
    GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_6, GPIO_PR_DOWN);

    /* Default value RESET/SET                                                                              */
    GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_6, RESET);

    /* Configure GPIO direction as output                                                                   */
    GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_6, GPIO_DIR_OUT);
  }
}

void LED_Sharp(void)
{
	GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_6); // GPIO = HIGH
	delay_ms(100);

	GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_6); // GPIO = LOW
	delay_ms(100);
}


