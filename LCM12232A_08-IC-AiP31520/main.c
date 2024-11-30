#include <REGX52.H>
#include <AT89X52.H>
#include <intrins.h>
#include "picture.h"
#include "lcd12232.h"


unsigned char mode;
void main(void)
{
//  IE=0x81; // Turn on  external interrupt
//	IP=0x01;
//	TCON=0x01;
//	int0=1;


//	P1=0xff;
//	P2=0xff;
//	P3=0xff;
    Initial();
	Delay(50);

	Display_dot(0x00,0x00);
	Delay(2000);
	Display_dot(0x55,0xAA);
	Delay(2000);
	Display_dot(0xaa,0x55);
	Delay(2000);
	Display_dot(0x00,0xff);
	Delay(2000);
	Display_dot(0xff,0x00);
	Delay(2000);
	Display_dot(0x55,0x55);
	Delay(2000);
	Display_dot(0xaa,0xaa);
	Delay(2000);
	show_english();
	Delay(8000);
	Display_dot(0x00,0x00);
	show_chinese();
	while(1)
	{

	  	if(ReadKey1())
		mode++;
		mode%=8;
		switch(mode){	
		case 0:Display_dot(0xAA,0x55);
		break;
		case 1:Display_dot(0x55,0xaa);
		break;
		case 2:Display_dot(0x55,0x55);
		break;
		case 3:Display_dot(0x00,0xff);
		break;
		case 4:Display_dot(0xaa,0xaa);
		break;
		case 5:Display_dot(0xff,0xff);
		break;
		case 6:show_english();
		break;
		case 7:show_chinese();
		break;


}

	}
}
