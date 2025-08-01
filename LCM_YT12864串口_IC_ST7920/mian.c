#include <REGX52.H>
#include "lcd12864.h"
#include "picture.h"

unsigned char mode;

void main(void)
{
	Lcd12864spi_init();            // 初始化LCD
	charlcdfill(' ');	
	drawimge(Img_SUNMAN_128x64);	
	DisplaystringAtposition(2,1,"广东烨辉科");
	DisplaystringAtposition(2,2,"技有限公司");
	DisplaystringAtposition(3,3,"LCM-12864");
	Delay(500);
    charlcdfill(' ');
	ST7920_Fill_Row(0x55, 0xAA);  
	Delay(500);	
	ST7920_Fill_Row(0xAA, 0x55);  
	Delay(500);	
	ST7920_Fill_Row(0x00, 0xFF);  
	Delay(500);	
	ST7920_Fill_Row(0xFF, 0x00);  
	Delay(500);	
	ST7920_Fill_Row(0x55, 0x55);
	Delay(500);
	ST7920_Fill_Row(0xAA, 0xAA);   
	Delay(500);
	ST7920_Fill_Row(0xFF, 0xFF);   
	Delay(500);
 
while (1)
{	
		if (ReadKey1()) 
	  {  
		charlcdfill(' ');
		mode++;
		mode %= 7;			
		switch (mode) 
	  {
        case 0:            
			ST7920_Fill_Row(0x55, 0xaa);  
            break;
        case 1:
            ST7920_Fill_Row(0xaa, 0x55);   
            break;
        case 2:
            ST7920_Fill_Row(0x55, 0x55);   
            break;
        case 3:
           ST7920_Fill_Row(0xaa, 0xaa);   
            break;
        case 4:
            ST7920_Fill_Row(0xFF, 0x00);   
            break;
        case 5:
            ST7920_Fill_Row(0x00, 0xFF);   
            break;
		case 6:
			ST7920_Fill_Row(0xFF, 0xFF);  
			break;   
		}
	}			
		}	
}

