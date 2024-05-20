#include "usart.h"

////
char rxdatabufer;
u16 point1 = 0;

_SaveData Save_Data;
char USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
////

void USART_Configuration(u8 USARTx,int BaudRate)
{
    //u8 USARTx=0;

    if (USARTx == 0) {
        USART_InitTypeDef USART_InitStruct; // 声明结构体

        CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
        CKCUClock.Bit.USART0   = 1;
        CKCUClock.Bit.AFIO     = 1;
        CKCUClock.Bit.PA      = 1;
        CKCU_PeripClockConfig(CKCUClock, ENABLE);
        // PA2--Tx  PA3--Rx
        AFIO_GPxConfig(GPIO_PA, AFIO_PIN_2, AFIO_MODE_6);  // 开启复用功能  AFIO_FUN_USART_UART
        AFIO_GPxConfig(GPIO_PA, AFIO_PIN_3, AFIO_MODE_6);  

        GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_3, GPIO_PR_UP);  // 打开UxART Rx内部上拉电阻以防止未知状态 

        USART_InitStruct.USART_BaudRate = BaudRate;  // 波特率
        USART_InitStruct.USART_WordLength = USART_WORDLENGTH_8B; // 字节长度
        USART_InitStruct.USART_StopBits = USART_STOPBITS_1; // 停止位
        USART_InitStruct.USART_Parity = USART_PARITY_NO; // 校验位
        USART_InitStruct.USART_Mode = USART_MODE_NORMAL; // 模式
        USART_Init(HT_USART0, &USART_InitStruct); 



        USART_IntConfig(HT_USART0, USART_INT_RXDR ,ENABLE); // 接收数据就绪中断使能
        //USART_IntConfig(HT_USART0, USART_INT_TXDE ,ENABLE); // 发送数据空中断使能   
        NVIC_EnableIRQ(USART0_IRQn); // 初始化中断
        
        /* 设置FIFO接收发送等级 */                                                                                   
        USART_RXTLConfig(HT_USART0, USART_RXTL_01);
        USART_TXTLConfig(HT_USART0, USART_TXTL_02);


        USART_RxCmd(HT_USART0, ENABLE); // 使能USART接收、发送 
        USART_TxCmd(HT_USART0, ENABLE);


    } else if (USARTx == 1) {
        USART_InitTypeDef USART_InitStruct; // 声明结构体

        CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
        CKCUClock.Bit.USART1   = 1;
        CKCUClock.Bit.AFIO     = 1;
        CKCUClock.Bit.PA      = 1;
        CKCU_PeripClockConfig(CKCUClock, ENABLE);
        // PA2--Tx  PA3--Rx
        AFIO_GPxConfig(GPIO_PA, AFIO_PIN_4, AFIO_MODE_6);  // 开启复用功能  AFIO_FUN_USART_UART
        AFIO_GPxConfig(GPIO_PA, AFIO_PIN_5, AFIO_MODE_6);  

        GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_4, GPIO_PR_UP);  // 打开UxART Rx内部上拉电阻以防止未知状态 

        USART_InitStruct.USART_BaudRate = BaudRate;  // 波特率
        USART_InitStruct.USART_WordLength = USART_WORDLENGTH_8B; // 字节长度
        USART_InitStruct.USART_StopBits = USART_STOPBITS_1; // 停止位
        USART_InitStruct.USART_Parity = USART_PARITY_NO; // 校验位
        USART_InitStruct.USART_Mode = USART_MODE_NORMAL; // 模式
        USART_Init(HT_USART1, &USART_InitStruct); 



        USART_IntConfig(HT_USART1, USART_INT_RXDR ,ENABLE); // 接收数据就绪中断使能
        ///*不知道为啥不能开*/USART_IntConfig(HT_USART1, USART_INT_TXDE ,ENABLE); // 发送数据空中断使能   
        NVIC_EnableIRQ(USART1_IRQn); // 初始化中断
        
        /* 设置FIFO接收发送等级 */                                                                                   
        USART_RXTLConfig(HT_USART1, USART_RXTL_01);
        USART_TXTLConfig(HT_USART1, USART_TXTL_02);


        USART_RxCmd(HT_USART1, ENABLE); // 使能USART接收、发送 
        USART_TxCmd(HT_USART1, ENABLE);
    }
    CLR_Buf();
}


void USART_Printf(HT_USART_TypeDef* USARTx,char *format, ...)
{
	char String[255];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	//Serial_SendString(String);
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		//Serial_SendByte(String[i]);
        USART_SendData(USARTx, String[i]);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXDE) == RESET);

	}
}

/**************************实现函数********************************************
函数说明：发送数组
*******************************************************************************/ 
void USART0_Tx(const char* TxBuffer, u32 length)
{
  int i;
 
  for (i = 0; i < length; i++)
  {
    // while (USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET); // 判断是否 发送完成
    while (USART_GetFlagStatus(HT_USART0, USART_FLAG_TXDE) == RESET); // 判断是否 发送完成
    USART_SendData(HT_USART0, TxBuffer[i]);
  }
}

/**************************实现函数********************************************
函数说明：接收中断服务函数
*******************************************************************************/ 
// void USART0_IRQHandler(void)
// {
//   u8 data;
//   if( USART_GetFlagStatus(HT_USART0, USART_FLAG_RXDR) ) // 接收器 FIFO 就绪标志位
//     {                                                                                                                                                                                    
//       data = USART_ReceiveData(HT_USART0); // 接收数据时已经自动清除中断标志位了，不用手动清除。      
//       USART_Printf(HT_USART0,"Usr0Rec:%c\r\n",data);
//     }
                
// }

void USART0_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;

	if(USART_GetFlagStatus(HT_USART0, USART_FLAG_RXDR)) 
	{
		Res =USART_ReceiveData(HT_USART0);//读取接收到的数据
	
	if(Res == '$')
	{
		point1 = 0;	
	}
		

	  USART_RX_BUF[point1++] = Res;

	if(USART_RX_BUF[0] == '$' && USART_RX_BUF[4] == 'M' && USART_RX_BUF[5] == 'C')			//确定是否收到"GPRMC/GNRMC"这一帧数据
	{
		if(Res == '\n')									   
		{
			memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
			memcpy(Save_Data.GPS_Buffer, USART_RX_BUF, point1); 	//保存数据
			Save_Data.isGetData = true;
			point1 = 0;
			memset(USART_RX_BUF, 0, USART_REC_LEN);      //清空				
		}	
				
	}
	
	if(point1 >= USART_REC_LEN)
	{
		point1 = USART_REC_LEN;
	}	


}
}

u8 Hand(char *a)                   // 串口命令识别函数
{ 
    if(strstr(USART_RX_BUF,a)!=NULL)
	    return 1;
	else
		return 0;
}

void CLR_Buf(void)                           // 串口缓存清理
{
	memset(USART_RX_BUF, 0, USART_REC_LEN);      //清空
  point1 = 0;                    
}

void clrStruct()
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
	memset(Save_Data.UTCTime, 0, UTCTime_Length);
	memset(Save_Data.latitude, 0, latitude_Length);
	memset(Save_Data.N_S, 0, N_S_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
	memset(Save_Data.E_W, 0, E_W_Length);
	
}


/**************************实现函数********************************************
函数说明：发送数组
*******************************************************************************/ 
void USART1_Tx(const char* TxBuffer, u32 length)
{
  int i;
 
  for (i = 0; i < length; i++)
  {
    // while (USART_GetFlagStatus(HT_USART1, USART_FLAG_TXC) == RESET); // 判断是否 发送完成
    while (USART_GetFlagStatus(HT_USART1, USART_FLAG_TXDE) == RESET); // 判断是否 发送完成
    USART_SendData(HT_USART1, TxBuffer[i]);
  }
}

/**************************实现函数********************************************
函数说明：接收中断服务函数
*******************************************************************************/ 
//void USART1_IRQHandler(void)
//{
//  u8 data;
//  if( USART_GetFlagStatus(HT_USART1, USART_FLAG_RXDR) ) // 接收器 FIFO 就绪标志位
//    {                                                                                                                                                                                    
//      data = USART_ReceiveData(HT_USART1); // 接收数据时已经自动清除中断标志位了，不用手动清除。      
//    //   printf("ReceiveData = %c\n",data);
//     //USART1_Tx(&data, 1);
//		USART_Printf(HT_USART1,"Usr1Rec:%c\r\n",data);
//    }
//                
//}


//USART1_IRQHandler在jw01

void Usart_Sendbyte(HT_USART_TypeDef* USARTx, u8 Data)
{
	USART_SendData(USARTx, Data);
	// 等待发送数据寄存器清空
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXDE) == RESET);		
}
// 发送字符串
void Usart_SendStr(HT_USART_TypeDef* USARTx, uint8_t *str)
{
	uint8_t i;
	for(i = 0;str[i] != '\0';i++)
	{
		Usart_Sendbyte(USARTx,str[i]);
	}
}
