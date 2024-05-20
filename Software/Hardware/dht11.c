#include "ht32.h"
#include "ht32_board.h"
#include "sys.h"
#include "dht11.h"
#include "delay.h"
//����GPIOΪ���״̬
//void DHT11_GPIO_OUT(void)
//{	 
//	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
// 	
// 	CKCUClock.Bit.PC  = 1;	   //ʹ��PC�˿�ʱ��
//	CKCUClock.Bit.AFIO = 1;    //ʹ��AFIO����
//    CKCU_PeripClockConfig(CKCUClock, ENABLE);	//ʹ��ʱ��
//	
//	AFIO_GPxConfig(GPIO_PC,AFIO_PIN_5, AFIO_FUN_GPIO);      //����PC5��AFIO����ΪGPIO
//    GPIO_DirectionConfig(HT_GPIOC,GPIO_PIN_5,GPIO_DIR_OUT);	//����PC5Ϊ���	
//	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_5, GPIO_PR_UP);	//������������
//	GPIO_SetOutBits(HT_GPIOC, GPIO_PIN_5);	                    //PC10�����
//			    
//} 
// 
////����GPIOΪ����״̬
//void DHT11_GPIO_IN(void)
//{	 
//	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
// 	
// 	CKCUClock.Bit.PC  = 1;	 //ʹ��PC�˿�ʱ��
//	CKCUClock.Bit.AFIO = 1;  //ʹ��AFIO����
//    CKCU_PeripClockConfig(CKCUClock, ENABLE);	//ʹ��ʱ��
//	
//	AFIO_GPxConfig(GPIO_PC,AFIO_PIN_5, AFIO_FUN_GPIO);      //����PC5��AFIO����ΪGPIO
//    GPIO_DirectionConfig(HT_GPIOC,GPIO_PIN_5,GPIO_DIR_IN);	//����PC5Ϊ����
//    //���²�����STM32��ͬ��Ҫ�ر�ע��
//	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_5,ENABLE);          //��������ʹ��
//	
//} 
////��DHT11��ȡһ���ֽ�
////����ֵ������������
//u8 DHT11_Read_Byte(void)    
//{        
//    //���·ֱ�Ϊѭ�����������ݴ洢������λ�����ݴ��������ʱ��������    
//    u8 i,ReadData,temp,retry;    
//    ReadData=0;
//	retry=0;
//	
//		for(i=0;i<8;i++)    //ѭ��8�Σ�1Byte=8bit
//		{
//			//�ȴ������źŵĵ͵�ƽ��ԼΪ50us�����ߵ�ƽ���ٺ�����ѭ��
//			while(DHT11_DQ_IN==0 && retry<100)
//			{
//				delay_us(1);    //��ʱ1us
//				retry++;
//			}
//			retry = 0;    //��������
//			delay_us(30);    //��ʱ30us
//            //�����ź�Ϊ0ʱ��temp=0����֮��temp=1
//			temp=1;
//			//����0�źŸߵ�ƽ����28us������1�źŸߵ�ƽ70us����ʱ30us��ȷ������0��1
//			if(DHT11_DQ_IN == 0 ){temp=0;}
//			while(DHT11_DQ_IN==1 && retry<100)
//			{
//				delay_us(1);
//				retry++;
//			}
//			retry=0;
//			ReadData<<=1;    //����1λ
//			ReadData|=temp;	 //��temp������ֵ����ReadData
//		}
//		return ReadData;
//}
////��DHT11��ȡһ������
////����ֵ��1,����;0,��ȡʧ��
//u8 DHT11_Read_Data(void)    
//{        
//		u8 retry;
//	   unsigned char i,sum;
//		//��������Ϊ��������Ϳ�ʼ�źŵ͵�ƽ18ms���ߵ�ƽ40us
//		DHT11_GPIO_OUT(); 
//		DHT11_DQ_OUT0;	//����
//		delay_ms(18);	//��������18ms
//        //����Ϊ�������û������ֶ�����20~40us����DHT11���յ���ʼ�źź��������ʱ20~40us��        
//        //���������������ϵ��������裬�����߻����Ϊ�ߵ�ƽ
////		DHT11_DQ_OUT1;	//����
////		Systick_Delay_us(30);		//��������20~40us
//		
//		//��������Ϊ���룬��鲢������Ӧ�źŵ͵�ƽ80us���ߵ�ƽ80us  
//		DHT11_GPIO_IN();
//		delay_us(20);
//		//��ʱ20us���͵�ƽ80us����ʣ60us������Ƿ��ǵ͵�ƽ��ȷ���Ƿ�����Ӧ�ź�
//		if(DHT11_DQ_IN == 0)
//		{
//			while(DHT11_DQ_IN==0 && retry<100)//������Ӧ�źŵ͵�ƽʣ��60us
//			{
//				delay_us(1);
//				retry++;
//			}
//			retry=0;//����100us�Զ��������У����⿨��
//			while(DHT11_DQ_IN==1 && retry<100)//������Ӧ�źŸߵ�ƽ80us
//			{
//				delay_us(1);
//				retry++;				
//			}
//			retry=0;
//			//����8�ֽ�����
//			for(i=0;i<5;i++)
//			{
//				dat[i]=DHT11_Read_Byte();
//			}	
// 
//			delay_us(1);//DHT11��������50us��Ϊ�����ź�	
//		}
//		sum=dat[0]+dat[1]+dat[2]+dat[3];
//		if(dat[4]==sum)		
//		{
//			return 1;
//		}
//		else
//			return 0;
//}


uint8_t T_H,T_L,H_H,H_L;  //�ֱ�Ϊ�¶Ⱥ�ʪ�ȵĸ�8λ �Ͱ�λ 
uint8_t temp_H,temp_L,humi_H,humi_L,checkdata;

//����ͨ������Ϊ���
static void dou_dht11_pinset_out(void)
{
		GPIO_DirectionConfig(dou_dht11_HT_GPIOx, dou_dht11_GPIO_PIN, GPIO_DIR_OUT);          //���÷���Ϊ���
		GPIO_OpenDrainConfig(dou_dht11_HT_GPIOx, dou_dht11_GPIO_PIN, DISABLE); 							//���ģʽ����Ϊ�������
		//GPIO_DriveConfig(dou_dht11_HT_GPIOx,dou_dht11_GPIO_PIN,GPIO_DIR_OUT,GPIO_DV_16MA); //�������������������
}

//����ͨ������Ϊ����
static void dou_dht11_pinset_in(void)
{
		GPIO_DirectionConfig(dou_dht11_HT_GPIOx, dou_dht11_GPIO_PIN, GPIO_DIR_IN);        //���÷���Ϊ����
		//GPIO_PullResistorConfig(dou_dht11_HT_GPIOx, dou_dht11_GPIO_PIN, GPIO_PR_DISABLE); //��������������
		GPIO_PullResistorConfig(dou_dht11_HT_GPIOx, dou_dht11_GPIO_PIN, GPIO_PR_UP);
		GPIO_InputConfig(dou_dht11_HT_GPIOx,dou_dht11_GPIO_PIN,ENABLE);
}

//��ʼ������
void dou_dht11_init(void)
{
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    if (dou_dht11_GPIOx == GPIO_PA)
    {
        CKCUClock.Bit.PA = 1;
    }
    else if (dou_dht11_GPIOx == GPIO_PB)
    {
        CKCUClock.Bit.PB = 1;
    }
    else if (dou_dht11_GPIOx == GPIO_PC)
    {
        CKCUClock.Bit.PC = 1;
    }
    else if (dou_dht11_GPIOx == GPIO_PD)
    {
        CKCUClock.Bit.PD = 1;
    }
    CKCUClock.Bit.AFIO = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
    AFIO_GPxConfig(dou_dht11_GPIOx, dou_dht11_GPIO_PIN, AFIO_MODE_1); //�������Ź���ģʽ
    dou_dht11_pinset_out();
    GPIO_WriteOutBits(dou_dht11_HT_GPIOx, dou_dht11_GPIO_PIN, SET);
}

//��ȡ����״̬
bool dou_dht11_get_databit(void)
{
    if (GPIO_ReadInBit(dou_dht11_HT_GPIOx, dou_dht11_GPIO_PIN) != RESET)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//�����������
void dou_dht11_set_databit(bool level)
{
		if (level != FALSE)
    {
        GPIO_WriteOutBits(dou_dht11_HT_GPIOx, dou_dht11_GPIO_PIN, SET);
    }
    else
    {
        GPIO_WriteOutBits(dou_dht11_HT_GPIOx, dou_dht11_GPIO_PIN, RESET);
    }
}


//��ȡһ�ֽ�����
static uint8_t dou_dht11_read_byte(void)
{
    uint8_t i;
    uint8_t data = 0;
    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        while ((!dou_dht11_get_databit()));
        delay_us(30);
        if (dou_dht11_get_databit())
        {
            data |= 0x1;
            while(dou_dht11_get_databit());
        }
        else
        {
            
        }
    }
    return data;
}

static bool dou_dht11_start_sampling(void)
{
    dou_dht11_pinset_out();
    //��������18ms? ?
    dou_dht11_set_databit(FALSE);
    delay_ms(18);
    dou_dht11_set_databit(TRUE);
    //������������������ ������ʱ10us
    delay_us(20);
    //������Ϊ���� �жϴӻ���Ӧ�ź� 
    dou_dht11_pinset_in();
    //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������?? ? ?
    if (dou_dht11_get_databit() == FALSE)		//T !?? ? ?
    {
        //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����?? ? 
        while (dou_dht11_get_databit() == FALSE);

        //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
        while (dou_dht11_get_databit() != FALSE);
        return TRUE;
    }
    return FALSE;
}

void dou_dht11_get_data(void)
{
	uint8_t temp;
    if (dou_dht11_start_sampling())
    {
        //printf("DHT11 is ready to transmit data\r\n");
        //���ݽ���״̬ 
        humi_H = dou_dht11_read_byte();	 	//����ʪ�ȸ߰�λ
        humi_L = dou_dht11_read_byte(); 	//����ʪ�ȵͰ�λ
        temp_H = dou_dht11_read_byte();//�����¶ȸ߰�λ
        temp_L = dou_dht11_read_byte();//�����¶ȵͰ�λ
        checkdata = dou_dht11_read_byte();					//����Ч��λ
        /* Data transmission finishes, pull the bus high */
        dou_dht11_pinset_out();
        dou_dht11_set_databit(TRUE);
        //����У�� 
        temp = (temp_H + temp_L + humi_H + humi_L);
        if (temp == checkdata)
        {
            H_H = humi_H; 				// ʪ����������
            H_L = humi_L;					//ʪ��С������
            T_H = temp_H;					//�¶���������
            T_L = temp_L;					//�¶�С������
        }
				       
    }  
    
}
