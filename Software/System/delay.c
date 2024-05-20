#include "ht32_cm0plus_misc.h"
#include "delay.h"
//mS???????
void delay_us(u32 us)
{
	u32 i;
	SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK);          //??????????SysTick????8MHZ
	SYSTICK_SetReloadValue(SystemCoreClock / 8 / 1000000); // ??????
	SYSTICK_IntConfig(DISABLE);                            // ??????
	SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);             //?????
	SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);            //??
	for( i = 0;i < us;i++ )
	{
		while( !( (SysTick->CTRL) & (1<<16) ) ); 
	}
 
	SYSTICK_CounterCmd(SYSTICK_COUNTER_DISABLE); //??
	SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);	 //????
}

void delay_ms(u16 ms){ //mS??????? 	  
	while( ms-- != 0){
		delay_us(1000);	//??1000?????
	}
}
 
void delay_s(u16 s){ //S??????	 		  	  
	while( s-- != 0){
		delay_ms(1000);	//??1000?????
	}
} 


