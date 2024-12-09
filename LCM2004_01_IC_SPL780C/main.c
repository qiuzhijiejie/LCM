#include <REGX52.H>
#include "LCD2004.h"
	unsigned char mode;
	unsigned char data pattern1[8] = {0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x15}; // 棋盘
	unsigned char data pattern2[8] = {0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A};	
	unsigned char data pattern3[8] = {0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15}; //竖线
    unsigned char data pattern4[8] = {0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A}; 
    unsigned char data pattern5[8] = {0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00};//横线 
	unsigned char data pattern6[8] = {0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F}; 	
	unsigned char data pattern7[8] = {0x00, 0x15, 0x00, 0x15, 0x00, 0x15, 0x00, 0x15}; //麻点
int main() 
{
	LCD_Init(); 
	LCD_Clear();
	displayCustomCharacter1(pattern1); // 显示棋盘
	Delay(80); 
	displayCustomCharacter1(pattern2); 
	Delay(80); 
	displayCustomCharacter1(pattern3); // 显示竖线
	Delay(80); 
	displayCustomCharacter1(pattern4); 
	Delay(80); 
	displayCustomCharacter1(pattern5); // 显示横线
	Delay(80); 
	displayCustomCharacter1(pattern6); 
	Delay(80); 
	displayCustomCharacter1(pattern7); // 显示麻点
	Delay(80);
	write_CGROM(0x30);//内置
	Delay(80);
	Write_DDRAM(0xCE);//内置字符
	Delay(80);
	LCD_Clear();
	LCD_ShowString(2,5,"LCM2004_01");
	LCD_ShowString(1,2,"YeHuiDisplay.com");
			
while (1) 
{

		if (ReadKey1()) 
	  {  
		LCD_Clear();
		mode++;
		mode %= 8;			
		switch (mode) 
	  {
        case 0:            
			displayCustomCharacter1(pattern1); 
            break;
        case 1:
            displayCustomCharacter1(pattern2);  
            break;
        case 2:
            displayCustomCharacter1(pattern3); 
            break;
        case 3:
           displayCustomCharacter1(pattern4);  
            break;
        case 4:
            displayCustomCharacter1(pattern5);  
            break;
        case 5:
            displayCustomCharacter1(pattern6);  
            break;
		case 6:
			displayCustomCharacter1(pattern7);
			break;
		case 7:
			write_CGROM(0xb0); 
		   break;
    
		}
	}			
}

	}

