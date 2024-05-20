#ifndef __gsm_H__
#define __gsm_H__

#include "ht32.h"


extern int k,j,gsm_flag1,gsm_flag2;
extern unsigned char str1[256];//发送短信内容
void gsm_send1(void);//发送消息
void gsm_send2(void);//打电话


#endif
