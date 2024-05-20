#include "key.h"
void Key_Init(void)
{

    CKCU_PeripClockConfig_TypeDef CCLOCK = {{0}}; // ����������ʱ����Ӧ�����޷�ʹ��
    CCLOCK.Bit.PA = 1;                            // ����PAʱ��
    CCLOCK.Bit.PB = 1;                            // ����PBʱ��
    CCLOCK.Bit.PC = 1;                            // ����PCʱ��
    CCLOCK.Bit.PD = 1;                            // ����PDʱ��
    CCLOCK.Bit.AFIO = 1;                          // �������ù���ʱ��
    CKCU_PeripClockConfig(CCLOCK, ENABLE);        // ʹ��ʱ��
	
	HT_GPIO_TypeDef *GPIO_GROUP;
    // PB15
    
    GPIO_GROUP = HT_GPIOB;
    AFIO_GPxConfig(GPIO_PB, GPIO_PIN_15, AFIO_MODE_DEFAULT);
    GPIO_InputConfig(GPIO_GROUP, GPIO_PIN_15, ENABLE);
    // PB 0 10 11 12
    //HT_GPIO_TypeDef *GPIO_GROUP;
    GPIO_GROUP = HT_GPIOC;
    AFIO_GPxConfig(GPIO_PC, GPIO_PIN_0 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12, AFIO_MODE_DEFAULT);
    GPIO_InputConfig(GPIO_GROUP, GPIO_PIN_0 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12, ENABLE);
}


