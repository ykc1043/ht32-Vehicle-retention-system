#include "sys.h"

void NVIC_Configuration(void){ //�жϿ��Ƶ�����
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����Ϊ	2��2	2λ��Ӧ������
}

void RCC_Configuration(void){ //RCCʱ������
	
	CKCU_PeripClockConfig_TypeDef CCLOCK = {{0}};//����������ʱ����Ӧ�����޷�ʹ��
	CCLOCK.Bit.PA    = 1;//����PAʱ��
	CCLOCK.Bit.PB    = 0;//����PBʱ��
	CCLOCK.Bit.PC    = 1;//����PCʱ��
	CCLOCK.Bit.PD    = 0;//����PDʱ��
	CCLOCK.Bit.AFIO  = 1;//�������ù���ʱ��
	CKCU_PeripClockConfig(CCLOCK, ENABLE);//ʹ��ʱ��
	
	//���������ڿ�����HT����ʱ��
}

