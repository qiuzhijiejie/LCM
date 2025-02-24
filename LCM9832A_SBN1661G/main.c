#include <REGX52.H>
#include "lcm9832a.h"
#include "pic.h"

void main(void)
{
//	TMOD=0x01;	//16 bit Timer 
// 	TH0=0xff;   //(65536-250)/256;	Timer high byte   1s=1000000us  1000000/4000=250
// 	TL0=0xE4;   //(65536-250)%256;	Timer low  byte
//	IE=0x83;  	// Turn on timer interrupt and external interrupt
//	IP=0x02; 	//Timer interrupt priority response  PT0=1;
//	IT0=1;		//External interrupt  trigger
// 	TR0=1;		//Turn on Timer

    Initial();
//	MAPPING(1);
	while(1)
	{		
		Display_pic();
		Show_chinese();
		Delay(800);
		Display_dot(0x00,0x00); 
		Display_One_Emglish();
		Delay(800);
	}	
}