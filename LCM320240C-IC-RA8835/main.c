#include "320240.h"
#include "PIC.h"
unsigned char mode;
void main(void)
{
	 P0=P1=P2=P3=0xff;
	 delay(10);
	 reset=0;
	 delay(10);
	 reset=1;
	 delay(500);
	 lcd_init();
	 delay(500);
	
		lcd_init();

 while(1)
	{
//		if(ReadKey1())
//		mode++;
//		mode%=7;
//		DisplayDots(0x00,0x00);
//		switch(mode)
//		{	
//		case 0:DisplayDots(0xff,0x00);
//		break;
//		case 1:DisplayDots(0x00,0xFF);
//		break;
//		case 2:DisplayDots(0x55,0x55);
//		break;
//		case 3:DisplayDots(0xAA,0xAA);
//		break;
//		case 4:DisplayDots(0xaa,0x55);
//		break;
//		case 5:DisplayDots(0x55,0xAA);
//		break;
//		case 6:DisplayDots(0xFF,0xFF);
//		break;
//		case 7:demo_picture();	
//		break;
//		}
		DisplayDots(0xAA, 0xAA);
		delay(5000);
		DisplayDots(0x55, 0x55);
		delay(5000);
		DisplayDots(0xAA, 0x55);
		delay(5000);
		DisplayDots(0x55, 0xAA);
		delay(5000);
		DisplayDots(0xff, 0x00);
		delay(5000);
		DisplayDots(0x00, 0xff);
		delay(5000);
		demo_picture();
		delay(5000);
		lcd_cls(0);
		DisplayFullCGROM();
		delay(5000);
		lcd_init();
	 demo_picture();   //show picture
	 demo_pixel();     //drawing pixels
	 demo_line();      //drawing line
//	 demo_rectangle(); //painted square demo function
//	 demo_circle();    //drawing circle
//	 demo_sine();      //sine presentation function 
//	 demo_hz();        //show chinese 
//	 demo_roll();      //display scroll 
//	 demo_game();     //show moving circle    
	 DisplayDots(0x00, 0x00);
	}
}