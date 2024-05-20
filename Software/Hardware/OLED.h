#ifndef __OLED_H
#define __OLED_H

#include "ht32.h"
#include "ht32_board.h"

#define _OLED_DIR 0 //0为正 1为反


void OLED_Init(void);
void OLED_Clear(void);
void OLED_turn(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_Show_Prcture(u8 x,u8 y,u8 sizex, u8 sizey,u8 BMP[]);
//void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_Print(u8 x, u8 y, char *s);
//void OLED_Display_16x16(u8 x,u8 y,u8 *dp);

#endif
