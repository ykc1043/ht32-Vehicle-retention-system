#include "infrared.h"
#include "usart.h"

void Infrared_Init(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock_infrared= {{0}};
	CKCUClock_infrared.Bit.PB = 1;  //开启GPIOB时钟
	CKCU_PeripClockConfig(CKCUClock_infrared, ENABLE);
	GPIO_DirectionConfig(HT_GPIO_PORT_infrared,infrared_GPIO_PIN,GPIO_DIR_IN);
	GPIO_InputConfig(HT_GPIO_PORT_infrared, infrared_GPIO_PIN, ENABLE);
  	GPIO_PullResistorConfig(HT_GPIO_PORT_infrared, infrared_GPIO_PIN, GPIO_PR_DOWN);
}


//红外人体感应功能测试
//感应到人体输出高电平，离开感应区域输出低电平
uint8_t  infrared_test(void)
{
	if( Read_STATE() == RESET)
	{
		
		//USART_Printf(HT_USART1,"NO  People\r\n");	
		return 0;
	}
	else
	{
		//USART_Printf(HT_USART1,"People\r\n");
		return 1;
	}
	
}
