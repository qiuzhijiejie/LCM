#include <REGX52.H>
#include "lcd12864.h"
#include "picture.h"

unsigned char mode;

void main(void)
{
	Lcd12864spi_init();            // 初始化LCD
	charlcdfill(' ');	
	drawimge(Img_SUNMAN_128x64);
	
	DisplaystringAtposition(2,0,"广东烨辉科");
	DisplaystringAtposition(2,1,"技有限公司");
	DisplaystringAtposition(3,2,"LCM-12864");
	Delay(5);
    charlcdfill(' ');
	ST7920_Fill_Row(0x55, 0xAA);   
	ST7920_Fill_Row(0xAA, 0x55);   
	ST7920_Fill_Row(0x00, 0xFF);   
	ST7920_Fill_Row(0xFF, 0x00);   
	ST7920_Fill_Row(0x55, 0x55);
	ST7920_Fill_Row(0xAA, 0xAA);   
	ST7920_Fill_Row(0xFF, 0xFF);   
while (1)
{
		if(ReadKey1())
		mode++;
		mode%=9;
	    Display_Clear();
	    charlcdfill(' ');
		switch(mode){
			case 1:ST7920_Fill_Row(0x55, 0xAA); 
			break;
			case 2:ST7920_Fill_Row(0xAA, 0x55); 
			break;
			case 3:ST7920_Fill_Row(0x55, 0x55);
			break;
			case 4:ST7920_Fill_Row(0xAA, 0xAA); 
			break;
			case 5:ST7920_Fill_Row(0xFF, 0x00); 
			break;
			case 6:ST7920_Fill_Row(0x00, 0xFF); 
			break;
			case 7:ST7920_Fill_Row(0xFF, 0xFF); 
			break;
			case 8:	drawimge(Img_SUNMAN_128x64);
				    DisplaystringAtposition(2,0,"广东烨辉科");
	                DisplaystringAtposition(2,1,"技有限公司");
	                DisplaystringAtposition(3,2,"LCM-12864");; 
}		
	
	}
} 
