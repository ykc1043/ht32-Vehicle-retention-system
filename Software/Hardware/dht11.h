#ifndef __dht11_H
#define __dht11_H 			   
#include "ht32.h"
#include "ht32_board.h"
////���ݶ˿�	PC10���1 										   
//#define	DHT11_DQ_OUT1	 	 GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_5,SET) 	
////���ݶ˿�	PC10���0	
//#define	DHT11_DQ_OUT0	 	 GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_5,RESET) 	
////���ݶ˿�	PC10 
//#define	DHT11_DQ_IN 		 GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_5) 					
// extern unsigned char dat[5];
// //����������
//void DHT11_GPIO_OUT(void);	//����GPIOΪ���
//void DHT11_GPIO_IN(void);		//����GPIOΪ����
//u8 DHT11_Read_Data(void);//��ȡһ����ʪ��
//u8 DHT11_Read_Byte(void);//����һ���ֽ�
//������DHT11ͨ��ʹ�õ�����
#define dou_dht11_GPIOx			GPIO_PA
#define dou_dht11_HT_GPIOx	HT_GPIOA
#define dou_dht11_GPIO_PIN	GPIO_PIN_0 


// dou_dht11 API **************************************************************
//���ݶ���:

extern uint8_t T_H;    //�¶ȸ�8λ
extern uint8_t T_L;    //�¶ȵ�8λ
extern uint8_t H_H;       //ʪ�ȸ�8λ
extern uint8_t H_L;       //ʪ�ȵ�8λ

void dou_dht11_init(void);      //��ʼ������
void dou_dht11_get_data(void);  //�������ݺ���
#endif
