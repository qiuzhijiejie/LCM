     
#include <REG52.H>       
#include "lcd24064.h"
#include "picture.h"

unsigned char mode;
void Main( void )   
{
 
  FS=0;
  LcmInit();
  LcmClear();	
  DisplayDots(0xaa,0xaa);
  Delay(1000); 
  DisplayDots(0x55,0x55);
  Delay(1000); 
  DisplayDots(0xaa,0x55);
  Delay(1000); 
  DisplayDots(0x55,0xaa);
  Delay(1000); 
  DisplayDots(0xff,0x00);
  Delay(1000); 
  DisplayDots(0x00,0xff);
  Delay(1000);
  LcmClear();	
  Display_FullCGROM();
  Delay(1000);
  LcmClear();
  wordageEng();
 while(1)  
{  
	
	  	if(ReadKey())
		mode++;
		mode%=9;
		LcmClear();
		switch(mode){	
		case 0:DisplayDots(0xff,0x00);
		break;
		case 1:DisplayDots(0x00,0xff);
		break;
		case 2:DisplayDots(0xaa,0x55);
		break;
		case 3:DisplayDots(0x55,0xaa);
		break;
		case 4:DisplayDots(0x55,0x55);
		break;
		case 5:DisplayDots(0xaa,0xaa);
		break;			
		case 6:DisplayDots(0xff,0xff);
		break;
		case 7:Display_FullCGROM();	
		break;
		case 8:wordageEng();	
		break;
 }   
}   
}
