#include "shake.h"
#include "usart.h"

int shake_flag=0;

void shake_init(void)
{
  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.PB = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  {                                                                       
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_4, AFIO_FUN_GPIO);                                                                    
    GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_4, GPIO_PR_DOWN);                                                                
    GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_4, GPIO_DIR_IN);                                                                 
    GPIO_InputConfig(HT_GPIOB, GPIO_PIN_4, ENABLE);
  }
}


uint8_t shake_ReadBit(void)//∂¡»°µÕµÁ∆Ω
{
  FlagStatus InputStatus = SET;
	
  InputStatus = GPIO_ReadInBit(HT_GPIOB, GPIO_PIN_4);

  if (InputStatus == RESET)
  {
//    USART_Printf(HT_USART0,"have chake\r\n");
//	 shake_flag=1;
	  return 1;
  }else
  {
	return 0;
	}

}
