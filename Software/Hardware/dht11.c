#include "ht32.h"
#include "ht32_board.h"
#include "sys.h"
#include "dht11.h"
#include "delay.h"
//设置GPIO为输出状态
//void DHT11_GPIO_OUT(void)
//{	 
//	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
// 	
// 	CKCUClock.Bit.PC  = 1;	   //使能PC端口时钟
//	CKCUClock.Bit.AFIO = 1;    //使能AFIO功能
//    CKCU_PeripClockConfig(CKCUClock, ENABLE);	//使能时钟
//	
//	AFIO_GPxConfig(GPIO_PC,AFIO_PIN_5, AFIO_FUN_GPIO);      //设置PC5的AFIO功能为GPIO
//    GPIO_DirectionConfig(HT_GPIOC,GPIO_PIN_5,GPIO_DIR_OUT);	//设置PC5为输出	
//	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_5, GPIO_PR_UP);	//开启上拉电阻
//	GPIO_SetOutBits(HT_GPIOC, GPIO_PIN_5);	                    //PC10输出高
//			    
//} 
// 
////设置GPIO为输入状态
//void DHT11_GPIO_IN(void)
//{	 
//	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
// 	
// 	CKCUClock.Bit.PC  = 1;	 //使能PC端口时钟
//	CKCUClock.Bit.AFIO = 1;  //使能AFIO功能
//    CKCU_PeripClockConfig(CKCUClock, ENABLE);	//使能时钟
//	
//	AFIO_GPxConfig(GPIO_PC,AFIO_PIN_5, AFIO_FUN_GPIO);      //设置PC5的AFIO功能为GPIO
//    GPIO_DirectionConfig(HT_GPIOC,GPIO_PIN_5,GPIO_DIR_IN);	//设置PC5为输入
//    //以下步骤与STM32不同需要特别注意
//	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_5,ENABLE);          //开启输入使能
//	
//} 
////从DHT11读取一个字节
////返回值：读到的数据
//u8 DHT11_Read_Byte(void)    
//{        
//    //以下分别为循环变量、数据存储变量、位数据暂存变量、超时计数变量    
//    u8 i,ReadData,temp,retry;    
//    ReadData=0;
//	retry=0;
//	
//		for(i=0;i<8;i++)    //循环8次，1Byte=8bit
//		{
//			//等待数据信号的低电平，约为50us，当高电平来临后跳出循环
//			while(DHT11_DQ_IN==0 && retry<100)
//			{
//				delay_us(1);    //延时1us
//				retry++;
//			}
//			retry = 0;    //计数清零
//			delay_us(30);    //延时30us
//            //数字信号为0时，temp=0；反之，temp=1
//			temp=1;
//			//数字0信号高电平持续28us，数字1信号高电平70us，延时30us以确认数字0或1
//			if(DHT11_DQ_IN == 0 ){temp=0;}
//			while(DHT11_DQ_IN==1 && retry<100)
//			{
//				delay_us(1);
//				retry++;
//			}
//			retry=0;
//			ReadData<<=1;    //左移1位
//			ReadData|=temp;	 //将temp读到的值赋予ReadData
//		}
//		return ReadData;
//}
////从DHT11读取一次数据
////返回值：1,正常;0,读取失败
//u8 DHT11_Read_Data(void)    
//{        
//		u8 retry;
//	   unsigned char i,sum;
//		//主机设置为输出，发送开始信号低电平18ms，高电平40us
//		DHT11_GPIO_OUT(); 
//		DHT11_DQ_OUT0;	//拉低
//		delay_ms(18);	//拉低至少18ms
//        //以下为误区，用户无需手动拉高20~40us，当DHT11接收到开始信号后会自行延时20~40us，        
//        //而由于数据总线上的上拉电阻，数据线会呈现为高电平
////		DHT11_DQ_OUT1;	//拉高
////		Systick_Delay_us(30);		//主机拉高20~40us
//		
//		//主机设置为输入，检查并接收响应信号低电平80us，高电平80us  
//		DHT11_GPIO_IN();
//		delay_us(20);
//		//延时20us，低电平80us，还剩60us，检查是否是低电平以确定是否有响应信号
//		if(DHT11_DQ_IN == 0)
//		{
//			while(DHT11_DQ_IN==0 && retry<100)//接收响应信号低电平剩余60us
//			{
//				delay_us(1);
//				retry++;
//			}
//			retry=0;//超过100us自动向下运行，以免卡死
//			while(DHT11_DQ_IN==1 && retry<100)//接收响应信号高电平80us
//			{
//				delay_us(1);
//				retry++;				
//			}
//			retry=0;
//			//接收8字节数据
//			for(i=0;i<5;i++)
//			{
//				dat[i]=DHT11_Read_Byte();
//			}	
// 
//			delay_us(1);//DHT11拉低总线50us作为结束信号	
//		}
//		sum=dat[0]+dat[1]+dat[2]+dat[3];
//		if(dat[4]==sum)		
//		{
//			return 1;
//		}
//		else
//			return 0;
//}


uint8_t T_H,T_L,H_H,H_L;  //分别为温度和湿度的高8位 低八位 
uint8_t temp_H,temp_L,humi_H,humi_L,checkdata;

//设置通信引脚为输出
static void dou_dht11_pinset_out(void)
{
		GPIO_DirectionConfig(dou_dht11_HT_GPIOx, dou_dht11_GPIO_PIN, GPIO_DIR_OUT);          //设置方向为输出
		GPIO_OpenDrainConfig(dou_dht11_HT_GPIOx, dou_dht11_GPIO_PIN, DISABLE); 							//输出模式设置为推挽输出
		//GPIO_DriveConfig(dou_dht11_HT_GPIOx,dou_dht11_GPIO_PIN,GPIO_DIR_OUT,GPIO_DV_16MA); //设置输出驱动电流能力
}

//设置通信引脚为输入
static void dou_dht11_pinset_in(void)
{
		GPIO_DirectionConfig(dou_dht11_HT_GPIOx, dou_dht11_GPIO_PIN, GPIO_DIR_IN);        //设置方向为输入
		//GPIO_PullResistorConfig(dou_dht11_HT_GPIOx, dou_dht11_GPIO_PIN, GPIO_PR_DISABLE); //除能上下拉电阻
		GPIO_PullResistorConfig(dou_dht11_HT_GPIOx, dou_dht11_GPIO_PIN, GPIO_PR_UP);
		GPIO_InputConfig(dou_dht11_HT_GPIOx,dou_dht11_GPIO_PIN,ENABLE);
}

//初始化函数
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
    AFIO_GPxConfig(dou_dht11_GPIOx, dou_dht11_GPIO_PIN, AFIO_MODE_1); //配置引脚工作模式
    dou_dht11_pinset_out();
    GPIO_WriteOutBits(dou_dht11_HT_GPIOx, dou_dht11_GPIO_PIN, SET);
}

//获取引脚状态
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

//设置引脚输出
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


//读取一字节数据
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
    //主机拉低18ms? ?
    dou_dht11_set_databit(FALSE);
    delay_ms(18);
    dou_dht11_set_databit(TRUE);
    //总线由上拉电阻拉高 主机延时10us
    delay_us(20);
    //主机设为输入 判断从机响应信号 
    dou_dht11_pinset_in();
    //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行?? ? ?
    if (dou_dht11_get_databit() == FALSE)		//T !?? ? ?
    {
        //判断从机是否发出 80us 的低电平响应信号是否结束?? ? 
        while (dou_dht11_get_databit() == FALSE);

        //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
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
        //数据接收状态 
        humi_H = dou_dht11_read_byte();	 	//接收湿度高八位
        humi_L = dou_dht11_read_byte(); 	//接收湿度低八位
        temp_H = dou_dht11_read_byte();//接收温度高八位
        temp_L = dou_dht11_read_byte();//接收温度低八位
        checkdata = dou_dht11_read_byte();					//接收效验位
        /* Data transmission finishes, pull the bus high */
        dou_dht11_pinset_out();
        dou_dht11_set_databit(TRUE);
        //数据校验 
        temp = (temp_H + temp_L + humi_H + humi_L);
        if (temp == checkdata)
        {
            H_H = humi_H; 				// 湿度整数部分
            H_L = humi_L;					//湿度小数部分
            T_H = temp_H;					//温度整数部分
            T_L = temp_L;					//温度小数部分
        }
				       
    }  
    
}
