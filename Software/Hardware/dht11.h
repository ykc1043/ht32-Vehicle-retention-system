#ifndef __dht11_H
#define __dht11_H 			   
#include "ht32.h"
#include "ht32_board.h"
////数据端口	PC10输出1 										   
//#define	DHT11_DQ_OUT1	 	 GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_5,SET) 	
////数据端口	PC10输出0	
//#define	DHT11_DQ_OUT0	 	 GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_5,RESET) 	
////数据端口	PC10 
//#define	DHT11_DQ_IN 		 GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_5) 					
// extern unsigned char dat[5];
// //函数的申明
//void DHT11_GPIO_OUT(void);	//设置GPIO为输出
//void DHT11_GPIO_IN(void);		//设置GPIO为输入
//u8 DHT11_Read_Data(void);//读取一次温湿度
//u8 DHT11_Read_Byte(void);//读出一个字节
//定义与DHT11通信使用的引脚
#define dou_dht11_GPIOx			GPIO_PA
#define dou_dht11_HT_GPIOx	HT_GPIOA
#define dou_dht11_GPIO_PIN	GPIO_PIN_0 


// dou_dht11 API **************************************************************
//数据定义:

extern uint8_t T_H;    //温度高8位
extern uint8_t T_L;    //温度低8位
extern uint8_t H_H;       //湿度高8位
extern uint8_t H_L;       //湿度低8位

void dou_dht11_init(void);      //初始化函数
void dou_dht11_get_data(void);  //更新数据函数
#endif
