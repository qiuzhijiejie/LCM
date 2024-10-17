#include <REGX52.H>
#include "lcd240128.h"
#include "picture.h"


sbit LCD_FS=P3^5;
sbit LCD_EN=P3^3;
unsigned char mode;
extern  unsigned char  Y; 
extern  unsigned char  X; 



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
	
    DisplayBMP(0,0,240,128,BMP1);   
    Delay(500);    
    ReverseDisplayBMP(0,0,240,128,BMP1);   
    Delay(500);
	
	LcmClear();
	Display_FullCGROM();
	Delay(500);
	
	LcmClear();
	picture();
	

 while(1)  
{

  		if(ReadKey1())
		mode++;
		mode%=9;
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
		case 5:picture();	
		break;
		case 6:Display_FullCGROM();	
		break;
		case 7:DisplayBMP(0,0,240,128,BMP1); ;	
		break;
		case 8:ReverseDisplayBMP(0,0,240,128,BMP1); 	
		break;

}
 }   
}
