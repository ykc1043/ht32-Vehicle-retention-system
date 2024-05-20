#include "sys.h"

void NVIC_Configuration(void){ //中断控制的设置
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置为	2：2	2位响应级优先
}

void RCC_Configuration(void){ //RCC时钟配置
	
	CKCU_PeripClockConfig_TypeDef CCLOCK = {{0}};//不开启外设时钟相应功能无法使用
	CCLOCK.Bit.PA    = 1;//开启PA时钟
	CCLOCK.Bit.PB    = 0;//开启PB时钟
	CCLOCK.Bit.PC    = 1;//开启PC时钟
	CCLOCK.Bit.PD    = 0;//开启PD时钟
	CCLOCK.Bit.AFIO  = 1;//开启复用功能时钟
	CKCU_PeripClockConfig(CCLOCK, ENABLE);//使能时钟
	
	//这里类似于开启了HT所有时钟
}

