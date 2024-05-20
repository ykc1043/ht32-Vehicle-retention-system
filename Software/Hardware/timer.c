#include "timer.h"

//uint16_t count = 0;  //计时
//uint8_t led_flag = 0; //led进行翻转标志

void Timer_Init(void)
{	
	CKCU_PeripClockConfig_TypeDef CKCUClock= {{0}};
	CKCUClock.Bit.BFTM1 = 1;  //开启中断时钟
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
	NVIC_EnableIRQ(BFTM1_IRQn);
	
	BFTM_SetCounter(HT_BFTM1, 0);
	//BFTM_SetCompare(HT_BFTM1, SystemCoreClock);//定时1s产生中断
	BFTM_SetCompare(HT_BFTM1, SystemCoreClock/1000);//定时1ms产生中断
	BFTM_IntConfig(HT_BFTM1, ENABLE);//使能中断
	BFTM_EnaCmd(HT_BFTM1, ENABLE);//使能BFTM
}

