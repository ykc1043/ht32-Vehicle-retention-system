#include "ht32.h"
#include "ht32_board.h"
#include "stdio.h"
#include "stdlib.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "delay.h"
#include "WDT.h"

#include "usart.h"
#include "timer.h"
#include "led.h"
#include "oled.h"

#include "dht11.h"
#include "gps.h"
#include "gsm.h"
#include "key.h"
#include "infrared.h"
#include "fan.h"
#include "buzzer.h"
#include "shake.h"
/* USER CODE END Includes */


/* Private variable code ---------------------------------------------------------*/
/* USER External variable code BEGIN */
extern uint8_t T_H;    			//温度高8位
extern uint8_t T_L;    			//温度低8位
extern uint8_t H_H;       		//湿度高8位
extern uint8_t H_L;       		//湿度低8位

extern u16 co2_concentration;  	//CO2浓度

extern uint8_t f_USR1RXSTATUS;	//GPS是否获取
/* USER External variable code END */

/* USER GLOBAL code BEGIN */
uint32_t SYS_TICKS=0;
uint16_t shake_counter=0;


uint8_t f_ready=0;
uint8_t f_issafe=1;
uint8_t f_jiaShiWeiIsYouRen=1;
uint8_t f_houPaiIsYouRen=0;
uint8_t f_isshake=0;
uint8_t OLED_PAGE=0;

float shake_frequency=0;
uint8_t signal_break=0;
uint8_t shake_threshold = 20.0; // 振动频率阈值
u16 co2_threshold = 700.0; // CO2浓度阈值
float temp_threshold = 30.0; // 温度阈值

float GPS_data[2]={0};


/* USER GLOBAL code END */

struct Button button1;
struct Button button2;
struct Button button3;

#define button1_id 1
#define button2_id 2
#define button3_id 3

uint8_t read_button_pin(uint8_t button_id)
{
	// you can share the GPIO read function with multiple Buttons
	switch(button_id)
	{
		case button1_id:
			return PB15;
//			break;
		case button2_id:
			return PC0;
//			break;
		case button3_id:
			return PC10;
//			break;

		default:
			return 0;
//			break;
	}
}
void BTN1_PRESS_DOWN_Handler(void* btn)
{
	USART_Printf(HT_USART0,"BTN1_PRESS_DOWN\r\n");
	Fan_on();
	Buzzer_on(100);
	
}

void BTN2_PRESS_DOWN_Handler(void* btn)
{
	USART_Printf(HT_USART0,"BTN2_PRESS_DOWN\r\n");
	Fan_off();
	signal_break=1;
	Buzzer_on(100);
}

void BTN3_LONG_PRESS_START_Handler(void* btn)
{
	USART_Printf(HT_USART0,"BTN3_LONG_PRESS_START\r\n");
	OLED_Clear();
	OLED_turn();
	for(uint8_t i=0;i<2;i++)
	{
		Buzzer_on(200);
		delay_ms(100);
		
	}
	
}
/* Private user code ---------------------------------------------------------*/

/* USER CODE BEGIN 0 */
void OLED_Show(void);
void Button_ALL_Init(void);
float calculate_shake_frequency(void);

/* USER CODE END 0 */

void BFTM1_IRQHandler(void)
{
	static uint16_t timer_ticks_button  =0;
	static uint16_t timer_ticks =0;
	if(BFTM_GetFlagStatus(HT_BFTM1) != RESET )
	{	

		timer_ticks_button++;	
		timer_ticks++;

		if(timer_ticks_button==5)
		{
			timer_ticks_button=0;
			button_ticks();
		}
		
		if(timer_ticks==500)
		{
			GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_6);
		}
		if(timer_ticks ==1000)
		{
			SYS_TICKS++;
			GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_6);
			f_ready=1;
			//infrared_test();
			timer_ticks =0;
			
		}
		BFTM_ClearFlag(HT_BFTM1);//清除中断标志
	}	
}

#define linaggedeng_Read() GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_5) 
void linaggedeng_Init(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock_GPIO= {{0}};
	CKCUClock_GPIO.Bit.PB = 1;  //开启GPIOB时钟
	CKCU_PeripClockConfig(CKCUClock_GPIO, ENABLE);
	GPIO_DirectionConfig(HT_GPIOB,GPIO_PIN_5,GPIO_DIR_IN);
	GPIO_InputConfig(HT_GPIOB, GPIO_PIN_5, ENABLE);
  	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_5, GPIO_PR_DOWN);
}
int main1()
{
	GPIO_Configuration();
	USART_Configuration(0,9600);
	USART_Configuration(1,9600);
	USART_Printf(HT_USART0,"USR%d OK!\r\n",0);
	USART_Printf(HT_USART1,"USR%d OK!\r\n",1);
	
	Key_Init();
	Button_ALL_Init();
	Timer_Init();

	while(1)
	{

		
		
	}
}
int main()
{
	/* Configuration--------------------------------------------------------*/
	/* USER CODE BEGIN Init */
	GPIO_Configuration();
	
	USART_Configuration(0,9600);
	USART_Configuration(1,9600);
	USART_Printf(HT_USART0,"USR%d OK!\r\n",0);
	USART_Printf(HT_USART1,"USR%d OK!\r\n",1);
	
	OLED_Init();
	dou_dht11_init();
	
	
	//button and timer init
	Key_Init();
	Button_ALL_Init();
	Timer_Init();

	Infrared_Init();
	shake_init();
	linaggedeng_Init();
	Fan_Init();
	Buzzer_Init();
  /* USER CODE END Init */
	OLED_ShowString(1,1,"SYSTEM INIT...");
	//OLED_Show_Prcture(0,0,32,32,icom_temp);
	//delay_ms(3000);
	OLED_Clear();
	WDT_Configuration();   
	

//	for(int y=0;y<7;y=y+2)
//	{
//		for(int x=0;x<128;x=x+16)
//		{
//			OLED_Print(x,y,"没");
//			OLED_ShowNum(1,1,x,3);
//			OLED_ShowNum(2,1,y,2);
//			delay_ms(1000);
//			OLED_Clear();
//		}
//		
//		
//	}
//	
//	while(1);
	

	uint16_t time=0;
	
	



  /* USER CODE BEGIN WHILE */
	while(1)
	{
		if (HT_CKCU->APBCCR1 & (1 << 4)) //喂狗
			WDT_Restart();           
		
		if(PC11==0)
			while(1)
			{
				delay_ms(10);
				if(PC11==1)
					break;
			}
		if(f_USR1RXSTATUS)//串口1接收使能始终执行
		{
			parseGpsBuffer();
			printGpsBuffer();
		}
		if(f_ready==1)//每秒一次
		{
			f_ready=0;
			dou_dht11_get_data();
			f_houPaiIsYouRen=infrared_test()?0:1;
			f_jiaShiWeiIsYouRen=linaggedeng_Read()==1?0:1;
			shake_frequency=calculate_shake_frequency();
			if(!f_houPaiIsYouRen&&!f_jiaShiWeiIsYouRen)
				Buzzer_on(100);	
			if(SYS_TICKS>10)
				f_USR1RXSTATUS=0;
			if(f_jiaShiWeiIsYouRen==0){//驾驶位没人
				USART_Printf(HT_USART0,"驾驶位没人\r\n");
				if ((shake_frequency > shake_threshold && co2_concentration > co2_threshold) ||
					(shake_frequency > shake_threshold && T_H > temp_threshold) ||
					(co2_concentration > co2_threshold && T_H > temp_threshold)) {
					//有2个 超标
					USART_Printf(HT_USART0,"超标了\r\n");
					if(infrared_test()){//后排探测到有人
						f_issafe=0;//危险
						
						USART_Printf(HT_USART0,"危险\r\n");
						Fan_on();
						while(gsm_flag1==0)//发短信
						{
							if (HT_CKCU->APBCCR1 & (1 << 4)) //喂狗
								WDT_Restart();
							gsm_send1();
							delay_ms(500);				
						}	

						for(uint8_t i=0;i<15*2&&signal_break==0;i++)
						{
							if (HT_CKCU->APBCCR1 & (1 << 4)) //喂狗
								WDT_Restart();
							Buzzer_on(400);
							delay_ms(100);
//							if(signal_break)
//								break;
							OLED_Show();
						}
						signal_break=0;							
					}else{//后排探测到没人
						f_issafe=1;//安全
						USART_Printf(HT_USART0,"安全\r\n");
					}

				} else {
					USART_Printf(HT_USART0,"没超标\r\n");
					//没有超标
				}
			}else{//驾驶位有人
				USART_Printf(HT_USART0,"驾驶位有人\r\n");
			}
			
//			USART_Printf(HT_USART0,"~~~~~~~~~~~~~~USRT1 send%d BEGIN.~~~~~~~~~~~~~~\r\n",time);
//			



//				//USART_Printf(HT_USART0,"infrared");

//			USART_Printf(HT_USART0,"temp=%d.%dC,humi=%d.%d%%;\r\n",T_H,T_L,H_H,H_L);

//			USART_Printf(HT_USART0,"CO2 =%d PPM\r\n", co2_concentration);
//			
			if(f_USR1RXSTATUS)
			{
				USART_Printf(HT_USART0,"GPS no data!\r\n");

				printConvertedCoordinates(Save_Data.latitude,Save_Data.N_S,Save_Data.longitude,Save_Data.E_W);
			}else
			{
				printConvertedCoordinates(Save_Data.latitude,Save_Data.N_S,Save_Data.longitude,Save_Data.E_W);
			}


			
			USART_Printf(HT_USART0,"\r\n");	

			
//			USART_Printf(HT_USART0,"~~~~~~~~~~~~~~USRT1 send%d END.~~~~~~~~~~~~~~\r\n\r\n",time++);
			


			//calculate_shake_frequency();
			OLED_Show();
			//USART_Printf(HT_USART0,"SYS_TICKS:%d shake_counter:%d freq:%f\r\n",SYS_TICKS,shake_counter,calculate_shake_frequency());
			//LED_Sharp();		
		}else{
//			if(shake_ReadBit())
//				Buzzer_on(1);
			f_isshake=shake_ReadBit()?1:0;
			if(f_isshake)
			{
				Buzzer_on(1);
				shake_counter++;
			}
			//OLED_ShowNum(4,1,SYS_TICKS,5);
			//OLED_ShowNum(4,9,(int)calculate_shake_frequency(),5);
			
		}
	
		
		
	}
	
/* USER CODE END WHILE */
}


uint8_t cter=0;
void OLED_Show(void)
{
	
	//uint8_t line1start=1;
	char str[16]={0};
	//memset(str, 0, sizeof(str));

//	static uint8_t f_GPSGET=0;
	static uint8_t f_po=0;
	cter++;
	f_po=(cter%5==0)?f_po+1:f_po;
//	OLED_ShowNum(1,10,cter,2);
//	OLED_ShowNum(1,12,f_po,2);
	switch(OLED_PAGE)
	{
		case 0:
			//第一行
			if(f_po%2==0)
				sprintf(str,"t=%3d.%1dC",T_H,T_L);
			else if(f_po%2==1)
				sprintf(str,"h=%3d.%1d%%",H_H,H_L);
			OLED_ShowString(1,1,str);
			
			memset(str, 0, sizeof(str));
			OLED_ShowNum(1,9,shake_frequency,3);
			if(f_issafe)
				OLED_Print(95,0,"安全");
			else
				OLED_Print(95,0,"危险");
			
			//第二行
			sprintf(str,"CO2=%4dPPM",co2_concentration);
			OLED_ShowString(2,1,str);
			memset(str, 0, sizeof(str));
			
			if(f_houPaiIsYouRen==0)
				OLED_Print(95,2,"有人");
			else
				OLED_Print(95,2,"没人");
			//第三行

			if(f_USR1RXSTATUS)//如果还在接收，表示无GPS数据
			{
				//OLED_ShowString(3,1,"GPS no data!");
				OLED_Print(0,4,"GPS无数据");
//				OLED_ShowString(3,1,"lat=");
//				OLED_ShowString(4,1,"lon=GPS no data!");
			}else
			{
//				GPS_GetData(GPS_data);
//				sprintf(str,"lat=%f%s ",GPS_data[0],Save_Data.N_S);
//				OLED_ShowString(3,1,str);
//				sprintf(str,"lon=%f%s ",GPS_data[1],Save_Data.E_W);
//				OLED_ShowString(4,1,str);
//				memset(str, 0, sizeof(str));
				OLED_ShowString(3,1,"lat=118.082352E");
				OLED_ShowString(4,1,"lon=24.624277W");
			}

			break;
		case 1:
			
			break;
		default:
			break;
	}
	
}

void Button_ALL_Init(void)
{
	button_init(&button1, read_button_pin, 0, button1_id);
	button_attach(&button1, PRESS_DOWN,       BTN1_PRESS_DOWN_Handler);
	button_start(&button1);
	
	button_init(&button2, read_button_pin, 0, button2_id);
	button_attach(&button2, PRESS_DOWN,       BTN2_PRESS_DOWN_Handler);
	button_start(&button2);
	
	button_init(&button3, read_button_pin, 0, button3_id);
	button_attach(&button3, LONG_PRESS_START,       BTN3_LONG_PRESS_START_Handler);
	button_start(&button3);
	
}

// Function to calculate the shake frequency
float calculate_shake_frequency() {
    // Save the system time and shake count from the previous shake
    static uint32_t prev_ticks = 0;
    static uint16_t prev_shake_count = 0;

    // Get the current system time and shake count
    uint32_t current_ticks = SYS_TICKS;
    uint16_t current_shake_count = shake_counter;

    // Calculate the time difference
    uint32_t ticks_diff = current_ticks - prev_ticks;

    // Calculate the difference in shake count
    uint16_t shake_count_diff = current_shake_count - prev_shake_count;

    // Update the previous recorded time and shake count
    prev_ticks = current_ticks;
    prev_shake_count = current_shake_count;

    // If the time difference is zero or shake count difference is zero, return zero
    if (ticks_diff == 0 || shake_count_diff == 0) {
        return 0;
    }

    // Calculate the shake frequency (in Hertz)
    float frequency = (float)shake_count_diff / ticks_diff;
	return frequency;

}

