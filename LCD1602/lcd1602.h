#ifndef __LCD1602_H__
#define __LCD1602_H__
#define LCD_DataPort P1  // 定义P0引脚为数据端口


void Delay(xms);
void LCD_WriteCommand(unsigned char Command);
void LCD_WriteData(unsigned char Data);
void LCD_Init();
void LCD_Clear();
void LCD_SetCursor(unsigned char Line, unsigned char Column);
void LCD_ShowString(unsigned char Line, unsigned char Column, char* String);
void LCD_USER_ShowString(unsigned char Line, unsigned char Column, unsigned char i);
void LCD_USER_SetCustomChar(const char data  *table, unsigned char num) ;
void Busy();
unsigned int ReadKey1();
unsigned int ReadKey2() ;
unsigned char LCD_Readdata(void) ;
void displayChessPattern1();
void HD44780_DisplayChar(unsigned char pattern);
void Write_CGRAM(unsigned char  a[]);
void shaw_character_string(unsigned char a[]);

#endif


