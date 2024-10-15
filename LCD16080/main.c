#include <REGX52.H>
#include <INTRINS.H>
#include "lcd16080.h"
#include "picture.h"
extern unsigned char data Y; 
extern unsigned char data X; 
unsigned char mode;

void main(void)
{  
	lcdInitialize();		//初始化液晶显示控制器
	lcdFill(0xff);
	lcdFill(0xaa);
	FillHorizontalStripePattern();
	FillHorizontalStripePattern1();
	FillHorizontalStripePattern2();
	lcdFill(0x55);
	
	
//	lcdFill(0x00);
//	draw_more_nested_rectangles();
//	lcdFill(0x00);
//	drawNestedCircles(80, 40, 39, 1); 

	
	while(1)
    {   
		if(ReadKey1())
		mode++;
		mode%=5;
		switch(mode){
		case 0:lcdFill(0xaa);
		break;
		case 1:FillHorizontalStripePattern();
		break;
		case 2:FillHorizontalStripePattern1();
		break;
		case 3:FillHorizontalStripePattern2();
		break;
		case 4:lcdFill(0xff);
		break;
//		case 5:draw_more_nested_rectangles();
//		break;

}

    }
}