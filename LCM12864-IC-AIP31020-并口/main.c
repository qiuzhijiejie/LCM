#include <REGX52.H>
#include "12864.h"
#include "pic.h"
extern unsigned char GXPOS;           
extern unsigned char GYPOS;      
void main(void)
{  
	lcdreset(); 
    charlcdfill(' ');                   //显示清屏
    putimage(Img_SUNMAN_128x64);
    putstrxy(2,1,"广东烨辉科");   //在(0,3)位置开始显示字符串
	putstrxy(2,2,"技有限公司");   //在(0,3)位置开始显示字符串
    Delay(500);
	charlcdfill(' ');
	while(1)
    {  
	DisplayDots(0x55, 0xAA);  
	Delay(500);	
	DisplayDots(0xAA, 0x55);  
	Delay(500);	
	DisplayDots(0x00, 0xFF);  
	Delay(500);	
	DisplayDots(0xFF, 0x00);  
	Delay(500);	
	DisplayDots(0x55, 0x55);  
	Delay(500);	
	DisplayDots(0xaa, 0xaa);  
	Delay(500);	
	DisplayDots(0xFF, 0xFF);   
	Delay(500);
				

    }
}