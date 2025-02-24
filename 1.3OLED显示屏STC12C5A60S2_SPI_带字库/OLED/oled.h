#ifndef __OLED_H
#define __OLED_H

#include "REG52.h"

sbit OLED_SCL		=P3^0;
sbit OLED_SDA		=P3^1;
sbit OLED_DC		=P3^2;//Data/Command
sbit OLED_CS		=P1^0;//Chip Select
sbit OLED_READ_FS0	=P1^1;
sbit OLED_ROM_CS	=P1^2;
sbit KEY    		=P2^0;

#define u8  unsigned char
#define u32 unsigned long
#define OLED_CMD  0   //Ð´ÃüÁî
#define OLED_DATA 1   //Ð´Êý¾Ý

#define OLED_SCL_Set()       OLED_SCL=1;
#define OLED_SCL_Clr()       OLED_SCL=0;

#define OLED_SDA_Set()       OLED_SDA=1;
#define OLED_SDA_Clr()       OLED_SDA=0;

#define OLED_DC_Set()        OLED_DC=1;
#define OLED_DC_Clr()        OLED_DC=0;

#define OLED_CS_Set()        OLED_CS=1;
#define OLED_CS_Clr()        OLED_CS=0;

#define OLED_ROM_CS_Set()    OLED_ROM_CS=1;
#define OLED_ROM_CS_Clr()    OLED_ROM_CS=0;

void delay_ms(unsigned int ms);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_Clear(void);
void OLED_address(u8 x,u8 y);
void OLED_Display_128x64(u8 *dp);
void OLED_Display_16x16(u8 x,u8 y,u8 *dp);
void OLED_Display_8x16(u8 x,u8 y,u8 *dp);
void OLED_Display_5x7(u8 x,u8 y,u8 *dp);
void Send_Command_to_ROM(u8 dat);
u8 Get_data_from_ROM(void);
void OLED_get_data_from_ROM(u8 addrHigh,u8 addrMid,u8 addrLow,u8 *pbuff,u8 DataLen);
void OLED_Display_GB2312_string(u8 x,u8 y,u8 *text);
void OLED_Display_string_5x7(u8 x,u8 y,u8 *text);
void OLED_ShowNum(u8 x,u8 y,float num,u8 len);
void OLED_Init(void);


void Delay(unsigned int time);
void Ins_trans(unsigned char command);
void Data_trans(unsigned char command);
void Column_set(unsigned char column);
void Page_set(unsigned char page);
void Screen_FillClear(unsigned char FC);
void Picture_display(unsigned char *ptr_pic);
void Initial();
void display_zifu_16x16(char page,char column,char *dp);
void display_zifu_16x8(char page,char column,char *dp);
void Screen_FillCol(unsigned char FC) ;
void OLED_fullCOL(unsigned char oddData, unsigned char evenData);
void Display_Sequence();
unsigned int ReadKey() ;
void Display_Sequence2();
void PerformScreenFillSequence();



#endif