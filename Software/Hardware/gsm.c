#include "gsm.h"
#include "usart.h"

int k=0,j=0,gsm_flag1=0,gsm_flag2=0;
unsigned char str1[256]="30105B8862A47AE5884C30118F668F865F025E38FF0C4F4D7F6EFF1A003100310038002E00300038003200330035003200200045002C00320034002E0036003200340032003700370057002C8BF7786E8BA48F665185662F542667094EBA90577559";//发送短信内容

void gsm_send1(void)
{
	switch(k)
	{
		case 0 :USART_Printf(HT_USART1,"AT\r\n");k++;break;//建立连接
		case 1 :USART_Printf(HT_USART1,"AT+CMGF=1\r\n");k++;break;//设置短信消息格式为文本模式
		case 2 :USART_Printf(HT_USART1,"AT+CSCS=\"UCS2\"\r\n");k++;break;//设置短信消息格式为文本模式
		case 3 :USART_Printf(HT_USART1,"AT+CSCS?\r\n");k++;break;//设置短信消息格式为文本模式
		case 4 :USART_Printf(HT_USART1,"AT+CSMP=17,167,0,25\r\n");k++;break;//数据编码格式，24发送英文，25发送中文
		case 5: USART_Printf(HT_USART1,"AT+CMGS=\"00310037003300390037003300390037003400380034\"\r\n");k++;break;
		case 6: Usart_SendStr(HT_USART1,str1);k++;break;
		case 7: Usart_Sendbyte(HT_USART1,0x1a);k++;break;
		case 8: gsm_flag1=1;break;
	}
}
void gsm_send2(void)
{
	switch(j)
	{
		case 0 :USART_Printf(HT_USART1,"AT+CPIN?\r\n");j++;break;//建立连接
		case 1 :USART_Printf(HT_USART1,"AT+CREG?\r\n");j++;break;//设置短信消息格式为文本模式
		case 2 :USART_Printf(HT_USART1,"AT+CMGF=1\r\n");j++;break;//设置短信消息格式为文本模式
		case 3 :USART_Printf(HT_USART1,"AT+CSCS=\"GSM\"\r\n");j++;break;//设置短信消息格式为文本模式
		case 4 :USART_Printf(HT_USART1,"ATD17397397484;\r\n");j++;break;//数据编码格式，24发送英文，25发送中文
		case 5: gsm_flag2=1;break;
	}
}

//使用函数模板
//while(gsm_flag2==0)//拨打电话
//			{
//				gsm_send2();
//            delay_ms(500);				
//			}	
