#ifndef __LDD_H
#define __LCD_H	 

#include "reg51.h"
#include "main.h"

sbit SDI = P3^0;//DS    5serial data input
sbit SCK = P3^2;//SH_CP 6shift register clock input
sbit RCK = P3^1;//ST_CP 7storage register clock input
#define LCD_Max	22


extern u8 LCD_POLARITY;
extern code u8 leddata1[];
extern code u8 leddata2[];
extern code u8 leddata3[];
extern code u8 leddata4[];
extern code u8 leddata10[];
extern code u8 leddata20[];

extern u8 CGRAM[LCD_Max];
extern u8 LCD_POLARITY;
void LCD_Delay(u16 nops);
void LCD_Port_Init(void);
void LCD_Write_Byte(u8 dat);
void LCD_Set_All(u8 start,u8 stop,u8 dat);
void LCD_Display_All(void);
void LCD_Clear_All(void);
void LCD_Update_CGRAM(u8 bit_state);
u8 LCD_Get_LEDDATA(u8 pos,u8 dat);

#endif