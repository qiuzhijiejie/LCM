#include <REGX52.H>
#include "LCD1602.h"

sbit LCD_RS = P3^0; 
sbit LCD_RW = P3^1;
sbit LCD_EN = P3^2;
sbit   KEY  = P2^0;	



unsigned char mode=0;


unsigned char row, col;
void displayCustomCharacter()
{
    // 定义自定义字符，每个字符占8字节，16进制表示 (以简单的笑脸为例)
//unsigned char customChar[8] = {0x00, 0x0A, 0x1F, 0x11, 0x0A, 0x0E, 0x00, 0x00};
unsigned char customChar7[8] = {0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F, 0x00}; // 空心矩形
unsigned char pattern2[8] = {0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A}; // 相反的棋盘格样式

    // 将自定义字符数组写入CGRAM
    Write_CGRAM(pattern2);
		
    // 设置光标位置到LCD上的某个地方，并显示自定义字符
        for (  row = 0; row < 2; row++) {
			for (  col = 0; col < 16; col++) {
     // 设置光标位置，每行地址不同
    unsigned char position = 0x80 + (row * 0x40) + col;  // 0x80 是第一行起始地址, 0xC0 是第二行起始地址
    LCD_WriteCommand(position);
    LCD_WriteData(0x00);      // 显示存储在CGRAM中的第一个自定义字符
	}	
		}	
}





unsigned char row, col;
void displayCustomCharacter1()
{
    // 定义自定义字符，每个字符占8字节，16进制表示 (以简单的笑脸为例)

unsigned char pattern1[8] = {0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x15}; // 棋盘格样式


    // 将自定义字符数组写入CGRAM
    Write_CGRAM(pattern1);
		
    // 设置光标位置到LCD上的某个地方，并显示自定义字符
        for (  row = 0; row < 2; row++) {
			for (  col = 0; col < 16; col++) {
     // 设置光标位置，每行地址不同
    unsigned char position = 0x80 + (row * 0x40) + col;  // 0x80 是第一行起始地址, 0xC0 是第二行起始地址
    LCD_WriteCommand(position);
    LCD_WriteData(0x00);      // 显示存储在CGRAM中的第一个自定义字符
	}	
		}	
}


int main() 
{

	LCD_Init(); // 初始化LCD1602
	LCD_Clear();
	     
			displayCustomCharacter();
			Delay(50);
            displayCustomCharacter1();  
            Delay(50);
            HD44780_DisplayChar(0x48);  
            Delay(50);
            HD44780_DisplayChar(0x4b);  
			Delay(50);
            HD44780_DisplayChar(0x5d);  
			Delay(50);
            HD44780_DisplayChar(0x23);  
			Delay(50);
		    HD44780_DisplayChar(0xff); 
			Delay(50);
            HD44780_DisplayChar(0x24); 
	
		while (1) 
	{

			//按键1
			if (ReadKey1()) {
			mode++;
			mode %= 8; // 确保 mode 的值在 0 到  之间循环
			//LCD_Clear(); 
				
				
		// 直接根据 mode 的值显示图案，不再等待图案完成	
		switch (mode) {
        case 1:
            
			displayCustomCharacter();
            break;
        case 2:
             displayCustomCharacter1();  
            break;
        case 3:
            HD44780_DisplayChar(0x48);  
            break;
        case 4:
            HD44780_DisplayChar(0x4b);  
            break;
        case 5:
            HD44780_DisplayChar(0x5d);  
            break;
        case 6:
            HD44780_DisplayChar(0x23);  
            break;
		case 7:
		    HD44780_DisplayChar(0xff); 
			break;
        default:
            HD44780_DisplayChar(0x24);  
            break;
    }


	}
			

		
	



}

	}

