#include "jw01.h"

// 定义用于暂存接收到的字节的数组和索引变量
static u8 data_buffer[6];
static u8 data_index = 0;
static u8 syncing = 1; // 添加一个同步状态标志

u16 co2_concentration;
extern u16 co2_concentration;

void USART1_IRQHandler(void)
{
  u8 data;
  if(USART_GetFlagStatus(HT_USART1, USART_FLAG_RXDR)) // 接收器 FIFO 就绪标志位
  {
    data = USART_ReceiveData(HT_USART1); // 接收数据
    
    // 如果我们正在同步或者检测到数据包的起始地址
    if(syncing || data == 0x2C)
    {
      data_buffer[0] = data; // 存储起始字节
      data_index = 1; // 重置索引
      syncing = 0; // 标记为已同步
    }
    else if(data_index < 6) // 如果不在同步状态，正常接收数据
    {
      data_buffer[data_index++] = data;
    }

    // 当收集完一个完整的数据包
    if(data_index == 6)
    {
      // 计算校验和
      u8 checksum = data_buffer[0] + data_buffer[1] + data_buffer[2] + data_buffer[3] + data_buffer[4];
      
      // 检查校验和是否正确
      if(checksum == data_buffer[5])
      {
        // 计算二氧化碳浓度
        co2_concentration = (data_buffer[1] << 8) + data_buffer[2];
        
        // 使用USART_Printf打印二氧化碳浓度
        //USART_Printf(HT_USART1,"CO2 Concentration: %d PPM\r\n", co2_concentration);
      }
      else
      {
        // 校验和错误，可以发送接收到的数据以帮助调试
		  USART_Printf(HT_USART1, "Checksum error. Checksum:%02X. Data: %02X %02X %02X %02X %02X %02X\r\n", 
                     checksum,data_buffer[0], data_buffer[1], data_buffer[2], 
                     data_buffer[3], data_buffer[4], data_buffer[5]);
      }
      
      // 重置索引，准备接收下一个数据包
      data_index = 0;
      syncing = 1; // 重置同步状态，以便重新同步
    }
  }
}
