#include "gps.h"

uint8_t f_USR1RXSTATUS=1;//默认使能
extern uint8_t f_USR1RXSTATUS;//串口中断使能标志


void errorLog(int num)
{
	
	while (1)
	{
	  	USART_Printf(HT_USART0,"ERROR%d\r\n",num);
		clrStruct();
		CLR_Buf();
		break;
	}
}

void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		USART_Printf(HT_USART0,"**************\r\n");
		USART_Printf(HT_USART0,Save_Data.GPS_Buffer);

		
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//解析错误
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//解析错误
				}
			}


		}
	}
}

void printGpsBuffer()
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		
		USART_Printf(HT_USART0,"Save_Data.UTCTime = ");
		USART_Printf(HT_USART0,Save_Data.UTCTime);
		USART_Printf(HT_USART0,"\r\n");

		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			USART_Printf(HT_USART0,"Save_Data.latitude = ");
			USART_Printf(HT_USART0,Save_Data.latitude);
			USART_Printf(HT_USART0,"\r\n");


			USART_Printf(HT_USART0,"Save_Data.N_S = ");
			USART_Printf(HT_USART0,Save_Data.N_S);
			USART_Printf(HT_USART0,"\r\n");

			USART_Printf(HT_USART0,"Save_Data.longitude = ");
			USART_Printf(HT_USART0,Save_Data.longitude);
			USART_Printf(HT_USART0,"\r\n");

			USART_Printf(HT_USART0,"Save_Data.E_W = ");
			USART_Printf(HT_USART0,Save_Data.E_W);
			USART_Printf(HT_USART0,"\r\n");
			
			
		}
		else
		{
			USART_Printf(HT_USART0,"GPS DATA is not usefull!\r\n");
		}
		
	}
}

float convertToDecimalDegrees(const char* ddmm, int isLatitude) {
    float degrees, minutes;
    if (isLatitude) {
        // Latitude: ddmm.mmmm
        sscanf(ddmm, "%2f%f", &degrees, &minutes);
    } else {
        // Longitude: dddmm.mmmm
        sscanf(ddmm, "%3f%f", &degrees, &minutes);
    }
    return degrees + (minutes / 60);
}

float G_latDecimal;
float G_longDecimal;

void printConvertedCoordinates(const char* latitude, const char* N_S, const char* longitude, const char* E_W) {
    float latDecimal = convertToDecimalDegrees(latitude, 1);
    float longDecimal = convertToDecimalDegrees(longitude, 0);

    // Adjust for hemisphere
    if (N_S[0] == 'S') latDecimal = -latDecimal;
    if (E_W[0] == 'W') longDecimal = -longDecimal;

    if(latDecimal>1&&longDecimal>1)//如果接收到了，就关闭串口1接收使能
    {
        USART_IntConfig(HT_USART0, USART_INT_RXDR ,DISABLE);
        f_USR1RXSTATUS=0;
    }
	G_latDecimal=latDecimal;
	G_longDecimal=longDecimal;
    USART_Printf(HT_USART0,"Latitude: %f, Longitude: %f\n", latDecimal, longDecimal);
}

void GPS_GetData(float *data)
{
	data[0]=G_latDecimal;
	data[1]=G_longDecimal;
}
