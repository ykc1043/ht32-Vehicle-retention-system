#include "OLED.h"
#include "OLED_Font.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*��������*/
#define OLED_W_SCL_SET()		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_7, SET);
#define OLED_W_SCL_RESET()		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_7, RESET);

#define OLED_W_SDA_SET()		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_8, SET);
#define OLED_W_SDA_RESET()		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_8, RESET);

/*���ų�ʼ��*/
void OLED_I2C_Init(void)
{

  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.PB = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  { /* Configure GPIO as output mode                                                                        */

    /* Configure AFIO mode as GPIO                                                                          */
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_7, AFIO_FUN_GPIO);

    /* Configure GPIO pull resistor                                                                         */
    GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_7, GPIO_PR_DOWN);

    /* Default value RESET/SET                                                                              */
    GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_7, RESET);

    /* Configure GPIO direction as output                                                                   */
    GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_7, GPIO_DIR_OUT);
  }
  
  { /* Configure GPIO as output mode                                                                        */

    /* Configure AFIO mode as GPIO                                                                          */
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_8, AFIO_FUN_GPIO);

    /* Configure GPIO pull resistor                                                                         */
    GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_8, GPIO_PR_DOWN);

    /* Default value RESET/SET                                                                              */
    GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_8, RESET);

    /* Configure GPIO direction as output                                                                   */
    GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_8, GPIO_DIR_OUT);
  }
	OLED_W_SCL_SET();
	OLED_W_SDA_SET();
}

/**
  * @brief  I2C��ʼ
  * @param  ��
  * @retval ��
  */
void OLED_I2C_Start(void)
{
	OLED_W_SDA_SET();
	OLED_W_SCL_SET();
	OLED_W_SDA_RESET();
	OLED_W_SCL_RESET();
}

/**
  * @brief  I2Cֹͣ
  * @param  ��
  * @retval ��
  */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA_RESET();
	OLED_W_SCL_SET();
	OLED_W_SDA_SET();
}

/**
  * @brief  I2C����һ���ֽ�
  * @param  Byte Ҫ���͵�һ���ֽ�
  * @retval ��
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		if(Byte & (0x80 >> i))
		{OLED_W_SDA_SET();}
		else
		{OLED_W_SDA_RESET();}
		//OLED_W_SDA(Byte & (0x80 >> i));
		OLED_W_SCL_SET();
		OLED_W_SCL_RESET();
	}
	OLED_W_SCL_SET();	//�����һ��ʱ�ӣ�������Ӧ���ź�
	OLED_W_SCL_RESET();
}

/**
  * @brief  OLEDд����
  * @param  Command Ҫд�������
  * @retval ��
  */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//�ӻ���ַ
	OLED_I2C_SendByte(0x00);		//д����
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/**
  * @brief  OLEDд����
  * @param  Data Ҫд�������
  * @retval ��
  */
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//�ӻ���ַ
	OLED_I2C_SendByte(0x40);		//д����
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED���ù��λ��
  * @param  Y �����Ͻ�Ϊԭ�㣬���·�������꣬��Χ��0~7
  * @param  X �����Ͻ�Ϊԭ�㣬���ҷ�������꣬��Χ��0~127
  * @retval ��
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//����Yλ��
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//����Xλ�ø�4λ
	OLED_WriteCommand(0x00 | (X & 0x0F));			//����Xλ�õ�4λ
}

/**
  * @brief  OLED����
  * @param  ��
  * @retval ��
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}

/**
  * @brief  OLED��ʾһ���ַ�
  * @param  Line ��λ�ã���Χ��1~4
  * @param  Column ��λ�ã���Χ��1~16
  * @param  Char Ҫ��ʾ��һ���ַ�����Χ��ASCII�ɼ��ַ�
  * @retval ��
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//���ù��λ�����ϰ벿��
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//��ʾ�ϰ벿������
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//���ù��λ�����°벿��
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//��ʾ�°벿������
	}
}

/**
  * @brief  OLED��ʾ�ַ���
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  String Ҫ��ʾ���ַ�������Χ��ASCII�ɼ��ַ�
  * @retval ��
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED�η�����
  * @retval ����ֵ����X��Y�η�
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED��ʾ���֣�ʮ���ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~4294967295
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
  * @retval ��
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED��ʾ���֣�ʮ���ƣ�����������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��-2147483648~2147483647
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
  * @retval ��
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED��ʾ���֣�ʮ�����ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~0xFFFFFFFF
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~8
  * @retval ��
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED��ʾ���֣������ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~1111 1111 1111 1111
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~16
  * @retval ��
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}
/**
  * @brief  OLED��ʼ��
  * @param  ��
  * @retval ��
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//�ϵ���ʱ
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//�˿ڳ�ʼ��
	
	OLED_WriteCommand(0xAE);	//�ر���ʾ
	
	OLED_WriteCommand(0xD5);	//������ʾʱ�ӷ�Ƶ��/����Ƶ��
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//���ö�·������
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//������ʾƫ��
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//������ʾ��ʼ��
	
	if(_OLED_DIR==1) 
	{
		OLED_WriteCommand(0xA0);	//�������ҷ���0xA1���� 0xA0���ҷ���
		OLED_WriteCommand(0xC0);	//�������·���0xC8���� 0xC0���·���
	}
	else if(_OLED_DIR==0)
	{
		OLED_WriteCommand(0xA1);	//�������ҷ���0xA1���� 0xA0���ҷ���
		OLED_WriteCommand(0xC8);	//�������·���0xC8���� 0xC0���·���
	}




	OLED_WriteCommand(0xDA);	//����COM����Ӳ������
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//���öԱȶȿ���
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//����Ԥ�������
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//����VCOMHȡ��ѡ�񼶱�
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//����������ʾ��/�ر�

	OLED_WriteCommand(0xA6);	//��������/��ת��ʾ

	OLED_WriteCommand(0x8D);	//���ó���
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//������ʾ
		
	OLED_Clear();				//OLED����
}

void OLED_Show_Prcture(u8 x,u8 y,u8 sizex, u8 sizey,u8 BMP[])
{ 	
  u16 j=0;
  u8 i,m;
  sizey=sizey/8+((sizey%8)?1:0);
  for(i=0;i<sizey;i++)
  {
    OLED_SetCursor(i+y,x);
    for(m=0;m<sizex;m++)
    {      
		OLED_WriteData(BMP[j++]);	 

    }
  }
} 
void OLED_turn(void)
{
	static uint8_t odir=0;
	odir++;
	if(odir%2==1) 
	{
		OLED_WriteCommand(0xA0);	//璁剧疆宸﹀彸鏂瑰悜锛�0xA1姝ｅ�? 0xA0宸﹀彸鍙嶇疆
		OLED_WriteCommand(0xC0);	//璁剧疆涓婁笅鏂瑰悜锛�?0xC8姝ｅ�? 0xC0涓婁笅鍙嶇疆
	}
	else if(odir%2==0)
	{
		OLED_WriteCommand(0xA1);	//璁剧疆宸﹀彸鏂瑰悜锛�0xA1姝ｅ�? 0xA0宸﹀彸鍙嶇疆
		OLED_WriteCommand(0xC8);	//璁剧疆涓婁笅鏂瑰悜锛�?0xC8姝ｅ�? 0xC0涓婁笅鍙嶇疆
	}
}


void OLED_Print_char(unsigned char x,unsigned char y,unsigned char Char)
{
	unsigned char i;
	if(x>128){
		x = 0;
		y += 2;
	}
	OLED_SetCursor(y,x);
	for(i=0;i<8;i++){
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);
	}
	OLED_SetCursor(y+1,x);
	for(i=0;i<8;i++){
		OLED_WriteData(OLED_F8x16[Char - ' '][i+8]);
	}
}
void OLED_Print(u8 x, u8 y, char *s)
{
	unsigned char i,k,t,length;
	unsigned short Index = 0;
	length = strlen(s);//ȡ�ַ����ܳ�
	//printf("length = %d\r\n",length);
	for(k=0; k<length; k++)
	{
		//printf("k = %d\r\n",k);
		if(*(s+k) <= 127){//С��128��ASCII����
			//printf("ascii\r\n");
			OLED_Print_char(x,y,*(s+k));
			x += 8;//x��������8
		}else if(*(s+k) > 127){//����127��Ϊ���֣�ǰ��������ɺ�������
			//printf("����\r\n");
			Index = (*(s+k) << 8) | (*(s+k+1));//ȡ���ֵ�����
			for(i=0;i<sizeof(CN16_Msk)/34;i++){//������
				if(Index == CN16_Msk[i].Index){
					//��ѯ�������
					// OLED_Set_Pos(x,y);	
					OLED_SetCursor(y,x);
					for(t=0;t<16;t++)
						OLED_WriteData(CN16_Msk[i].Msk[t]);//д����ģ

					// OLED_Set_Pos(x,y+1);
					OLED_SetCursor(y+1,x);	
					for(t=16;t<32;t++)
						OLED_WriteData(CN16_Msk[i].Msk[t]);

					x += 16;
					k += 1; //����ռ2B,����һ��	
				}
			}
		}
	}
}

