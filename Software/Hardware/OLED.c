#include "OLED.h"
#include "OLED_Font.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*引脚配置*/
#define OLED_W_SCL_SET()		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_7, SET);
#define OLED_W_SCL_RESET()		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_7, RESET);

#define OLED_W_SDA_SET()		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_8, SET);
#define OLED_W_SDA_RESET()		GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_8, RESET);

/*引脚初始化*/
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
  * @brief  I2C开始
  * @param  无
  * @retval 无
  */
void OLED_I2C_Start(void)
{
	OLED_W_SDA_SET();
	OLED_W_SCL_SET();
	OLED_W_SDA_RESET();
	OLED_W_SCL_RESET();
}

/**
  * @brief  I2C停止
  * @param  无
  * @retval 无
  */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA_RESET();
	OLED_W_SCL_SET();
	OLED_W_SDA_SET();
}

/**
  * @brief  I2C发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
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
	OLED_W_SCL_SET();	//额外的一个时钟，不处理应答信号
	OLED_W_SCL_RESET();
}

/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x00);		//写命令
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x40);		//写数据
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
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
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
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
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
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
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
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
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
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
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
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
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
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
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//端口初始化
	
	OLED_WriteCommand(0xAE);	//关闭显示
	
	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//设置显示开始行
	
	if(_OLED_DIR==1) 
	{
		OLED_WriteCommand(0xA0);	//设置左右方向，0xA1正常 0xA0左右反置
		OLED_WriteCommand(0xC0);	//设置上下方向，0xC8正常 0xC0上下反置
	}
	else if(_OLED_DIR==0)
	{
		OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
		OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置
	}




	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//设置对比度控制
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

	OLED_WriteCommand(0xA6);	//设置正常/倒转显示

	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//开启显示
		
	OLED_Clear();				//OLED清屏
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
		OLED_WriteCommand(0xA0);	//鐠佸墽鐤嗗锕�褰搁弬鐟版倻閿涳拷0xA1濮濓絽鐖? 0xA0瀹革箑褰搁崣宥囩枂
		OLED_WriteCommand(0xC0);	//鐠佸墽鐤嗘稉濠佺瑓閺傜懓鎮滈敍锟?0xC8濮濓絽鐖? 0xC0娑撳﹣绗呴崣宥囩枂
	}
	else if(odir%2==0)
	{
		OLED_WriteCommand(0xA1);	//鐠佸墽鐤嗗锕�褰搁弬鐟版倻閿涳拷0xA1濮濓絽鐖? 0xA0瀹革箑褰搁崣宥囩枂
		OLED_WriteCommand(0xC8);	//鐠佸墽鐤嗘稉濠佺瑓閺傜懓鎮滈敍锟?0xC8濮濓絽鐖? 0xC0娑撳﹣绗呴崣宥囩枂
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
	length = strlen(s);//取字符串总长
	//printf("length = %d\r\n",length);
	for(k=0; k<length; k++)
	{
		//printf("k = %d\r\n",k);
		if(*(s+k) <= 127){//小于128是ASCII符号
			//printf("ascii\r\n");
			OLED_Print_char(x,y,*(s+k));
			x += 8;//x坐标右移8
		}else if(*(s+k) > 127){//大于127，为汉字，前后两个组成汉字内码
			//printf("汉字\r\n");
			Index = (*(s+k) << 8) | (*(s+k+1));//取汉字的内码
			for(i=0;i<sizeof(CN16_Msk)/34;i++){//查数组
				if(Index == CN16_Msk[i].Index){
					//查询到这个字
					// OLED_Set_Pos(x,y);	
					OLED_SetCursor(y,x);
					for(t=0;t<16;t++)
						OLED_WriteData(CN16_Msk[i].Msk[t]);//写入字模

					// OLED_Set_Pos(x,y+1);
					OLED_SetCursor(y+1,x);	
					for(t=16;t<32;t++)
						OLED_WriteData(CN16_Msk[i].Msk[t]);

					x += 16;
					k += 1; //汉字占2B,跳过一个	
				}
			}
		}
	}
}

