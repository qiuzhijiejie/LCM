#include <REGX52.H>
#include "lcd16080.h"
#include "picture.h"



unsigned char mode;
void Main( void )   
{
	LCD_EN=0;  
	LCD_FS=0; 
	LcmInit(); 
	LcmClear();
	Delay(100);
    DisplayDots(0x55,0xaa);
    Delay(500); 
	
    DisplayDots(0xaa,0x55);
    Delay(500);  
    DisplayDots(0x55,0x55);
    Delay(500); 	
    DisplayDots(0xaa,0xaa);
    Delay(500); 
    DisplayDots(0xff,0x00);
    Delay(500); 
    DisplayDots(0x00,0xff);
    Delay(500); 
	  DisplayDots(0xff,0xff);
    Delay(500); 	
	  LcmClear();
	LcmClear();
	Display_FullCGROM();
	Delay(500);	
	LcmClear();
	Show_Eng();
	

 while(1)  
{

  		if(ReadKey1())
		mode++;
		mode%=8;
		LcmClear();
		switch(mode){	
		case 0:DisplayDots(0xff,0x00);
		break;
		case 1:DisplayDots(0x55,0xaa);
		break;
		case 2:DisplayDots(0x55,0x55);
		break;
		case 3:DisplayDots(0x00,0xff);
		break;
		case 4:DisplayDots(0xaa,0xaa);
		break;
		case 5:Show_Eng();	
		break;
		case 6:Display_FullCGROM();	
		break;
		case 7:Show_Eng(); 	


}
 }   
}
