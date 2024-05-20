#include "OLED.h"
#include "OLED_Font.h"
#include "OLED_BMP.h"

/*寮曡剼閰嶇疆*/
#define OLED_W_SCL_SET()		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_7, SET);
#define OLED_W_SCL_RESET()		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_7, RESET);

#define OLED_W_SDA_SET()		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_8, SET);
#define OLED_W_SDA_RESET()		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_8, RESET);

/*寮曡剼鍒濆鍖�*/
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
  * @brief  I2C寮€濮�
  * @param  鏃�
  * @retval 鏃�
  */
void OLED_I2C_Start(void)
{
	OLED_W_SDA_SET();
	OLED_W_SCL_SET();
	OLED_W_SDA_RESET();
	OLED_W_SCL_RESET();
}

/**
  * @brief  I2C鍋滄�?
  * @param  鏃�
  * @retval 鏃�
  */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA_RESET();
	OLED_W_SCL_SET();
	OLED_W_SDA_SET();
}

/**
  * @brief  I2C鍙戦€佷竴涓瓧鑺�
  * @param  Byte 瑕佸彂閫佺殑涓€涓瓧鑺�?
  * @retval 鏃�
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
	OLED_W_SCL_SET();	//棰濆鐨勪竴涓椂閽燂紝涓嶅鐞嗗簲绛斾俊鍙�?
	OLED_W_SCL_RESET();
}

/**
  * @brief  OLED鍐欏懡浠�?
  * @param  Command 瑕佸啓鍏ョ殑鍛戒�?
  * @retval 鏃�
  */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//浠庢満鍦板�?
	OLED_I2C_SendByte(0x00);		//鍐欏懡浠�?
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/**
  * @brief  OLED鍐欐暟鎹�?
  * @param  Data 瑕佸啓鍏ョ殑鏁版�?
  * @retval 鏃�
  */
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//浠庢満鍦板�?
	OLED_I2C_SendByte(0x40);		//鍐欐暟鎹�?
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED璁剧疆鍏夋爣浣嶇�?
  * @param  Y 浠ュ乏涓婅涓哄師鐐癸紝鍚戜笅鏂瑰悜鐨勫潗鏍囷紝鑼冨洿锛�?0~7
  * @param  X 浠ュ乏涓婅涓哄師鐐癸紝鍚戝彸鏂瑰悜鐨勫潗鏍囷紝鑼冨洿锛�?0~127
  * @retval 鏃�
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//璁剧疆Y浣嶇�?
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//璁剧疆X浣嶇疆楂�?4浣�
	OLED_WriteCommand(0x00 | (X & 0x0F));			//璁剧疆X浣嶇疆浣�?4浣�
}

/**
  * @brief  OLED娓呭�?
  * @param  鏃�
  * @retval 鏃�
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
  * @brief  OLED鏄剧ず涓€涓瓧绗�
  * @param  Line 琛屼綅缃紝鑼冨洿锛�?1~4
  * @param  Column 鍒椾綅缃紝鑼冨洿锛�?1~16
  * @param  Char 瑕佹樉绀虹殑涓€涓瓧绗︼紝鑼冨洿锛欰SCII鍙瀛楃�?
  * @retval 鏃�
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//璁剧疆鍏夋爣浣嶇疆鍦ㄤ笂鍗婇儴鍒�?
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//鏄剧ず涓婂崐閮ㄥ垎鍐呭
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//璁剧疆鍏夋爣浣嶇疆鍦ㄤ笅鍗婇儴鍒�?
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//鏄剧ず涓嬪崐閮ㄥ垎鍐呭
	}
}

/**
  * @brief  OLED鏄剧ず瀛楃涓�?
  * @param  Line 璧峰琛屼綅缃紝鑼冨洿锛�1~4
  * @param  Column 璧峰鍒椾綅缃紝鑼冨洿锛�1~16
  * @param  String 瑕佹樉绀虹殑瀛楃涓诧紝鑼冨洿锛欰SCII鍙瀛楃�?
  * @retval 鏃�
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
  * @brief  OLED娆℃柟鍑芥暟
  * @retval 杩斿洖鍊肩瓑浜嶺鐨刌娆℃�?
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
  * @brief  OLED鏄剧ず鏁板瓧锛堝崄杩涘埗锛屾鏁帮級
  * @param  Line 璧峰琛屼綅缃紝鑼冨洿锛�1~4
  * @param  Column 璧峰鍒椾綅缃紝鑼冨洿锛�1~16
  * @param  Number 瑕佹樉绀虹殑鏁板瓧锛岃寖鍥达�?0~4294967295
  * @param  Length 瑕佹樉绀烘暟瀛楃殑闀垮害锛岃寖鍥达細1~10
  * @retval 鏃�
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
  * @brief  OLED鏄剧ず鏁板瓧锛堝崄杩涘埗锛屽甫绗﹀彿鏁帮級
  * @param  Line 璧峰琛屼綅缃紝鑼冨洿锛�1~4
  * @param  Column 璧峰鍒椾綅缃紝鑼冨洿锛�1~16
  * @param  Number 瑕佹樉绀虹殑鏁板瓧锛岃寖鍥达�?-2147483648~2147483647
  * @param  Length 瑕佹樉绀烘暟瀛楃殑闀垮害锛岃寖鍥达細1~10
  * @retval 鏃�
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
  * @brief  OLED鏄剧ず鏁板瓧锛堝崄鍏繘鍒讹紝姝ｆ暟锛�
  * @param  Line 璧峰琛屼綅缃紝鑼冨洿锛�1~4
  * @param  Column 璧峰鍒椾綅缃紝鑼冨洿锛�1~16
  * @param  Number 瑕佹樉绀虹殑鏁板瓧锛岃寖鍥达�?0~0xFFFFFFFF
  * @param  Length 瑕佹樉绀烘暟瀛楃殑闀垮害锛岃寖鍥达細1~8
  * @retval 鏃�
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
  * @brief  OLED鏄剧ず鏁板瓧锛堜簩杩涘埗锛屾鏁帮級
  * @param  Line 璧峰琛屼綅缃紝鑼冨洿锛�1~4
  * @param  Column 璧峰鍒椾綅缃紝鑼冨洿锛�1~16
  * @param  Number 瑕佹樉绀虹殑鏁板瓧锛岃寖鍥达�?0~1111 1111 1111 1111
  * @param  Length 瑕佹樉绀烘暟瀛楃殑闀垮害锛岃寖鍥达細1~16
  * @retval 鏃�
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
  * @brief  OLED鍒濆鍖�?
  * @param  鏃�
  * @retval 鏃�
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//涓婄數寤舵椂
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//绔彛鍒濆鍖�
	
	OLED_WriteCommand(0xAE);	//鍏抽棴鏄剧ず
	
	OLED_WriteCommand(0xD5);	//璁剧疆鏄剧ず鏃堕挓鍒嗛姣�/鎸崱鍣ㄩ鐜�
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//璁剧疆澶氳矾澶嶇敤鐜�?
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//璁剧疆鏄剧ず鍋忕Щ
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//璁剧疆鏄剧ず寮€濮嬭�?
	
	if(_OLED_DIR==1) 
	{
		OLED_WriteCommand(0xA0);	//璁剧疆宸﹀彸鏂瑰悜锛�0xA1姝ｅ�? 0xA0宸﹀彸鍙嶇疆
		OLED_WriteCommand(0xC0);	//璁剧疆涓婁笅鏂瑰悜锛�?0xC8姝ｅ�? 0xC0涓婁笅鍙嶇疆
	}
	else if(_OLED_DIR==0)
	{
		OLED_WriteCommand(0xA1);	//璁剧疆宸﹀彸鏂瑰悜锛�0xA1姝ｅ�? 0xA0宸﹀彸鍙嶇疆
		OLED_WriteCommand(0xC8);	//璁剧疆涓婁笅鏂瑰悜锛�?0xC8姝ｅ�? 0xC0涓婁笅鍙嶇疆
	}




	OLED_WriteCommand(0xDA);	//璁剧疆COM寮曡剼纭欢閰嶇�?
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//璁剧疆瀵规瘮搴︽帶鍒�
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//璁剧疆棰勫厖鐢靛懆鏈�?
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//璁剧疆VCOMH鍙栨秷閫夋嫨绾у埆
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//璁剧疆鏁翠釜鏄剧ず鎵撳紑/鍏抽�?

	OLED_WriteCommand(0xA6);	//璁剧疆姝ｅ父/鍊掕浆鏄剧ず

	OLED_WriteCommand(0x8D);	//璁剧疆鍏呯數娉�
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//寮€鍚樉绀�?
		
	OLED_Clear();				//OLED娓呭�?
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

////鏄剧�?16x16鐐归樀鍥惧儚銆佹眽瀛椼€佺敓鍍诲瓧鎴�16x16鐐归樀鐨勫叾浠栧浘鏍�
//void OLED_Display_16x16(u8 x,u8 y,u8 *dp)
//{
//	u8 i,j;
//	for(j=0;j<2;j++)
//	{
//		OLED_address(x,y);
//		for(i=0;i<16;i++)
//		{
//			OLED_WriteData(*dp);  //鍐欐暟鎹埌OLED,姣忓啓瀹屼竴涓�?8浣嶇殑鏁版嵁鍚庡垪鍦板潃鑷姩鍔�?1
//			dp++;
//    }
//		y++;
//  }
//}

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
/*
	x:�����꣬0-127
	y:0-7(�о�԰OLED)
    *s:Ҫ��ʾ���ַ�����Ӣ�ľ��ɣ�ǰ���������а����ĺ��֣�
*/
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
			OLED_ShowChar(x,y,*(s+k));
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
