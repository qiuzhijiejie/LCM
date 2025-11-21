#ifndef __LDD_H
#define __LCD_H	 

#include "reg51.h"
#include "main.h"

sbit   DS  = P1^2; //SDI-serial data input
sbit SH_CP = P1^0; //SCK-shift register clock input
sbit ST_CP = P1^5; //RCK- storage register clock input

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
