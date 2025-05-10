 #include <REG52.H>       
#include "lcd12864.h"   
#include "picture.h" 
unsigned char mode;
sbit int0=P2^0; 
   
void main( void )   
{      
 Delay(20);  
 IE=0x81;
 IP=0x01;
 TCON=0x01;
 int0=1;  
 LcmInit();   
 LcmClear(); 
LcmPutDots(0x55,0xaa);
 Delay(600); 
  LcmPutDots(0xaa,0x55);
  Delay(600);
  
 LcmPutDots(0x55,0x55);
 Delay(600); 
  LcmPutDots(0xaa,0xaa);
  Delay(600); 

  LcmPutDots(0xff,0x00);
  Delay(600); 
  LcmPutDots(0x00,0xff);
  Delay(600); 

Display_Chinese();
  Delay(500); 
 // Display_character();	
 while(1)   
 {
	 if(key())
	 {
		 mode++;
		 if(mode>9)mode=1;
		 switch(mode)
		 {
			 case 1:LcmPutDots(0x55,0xaa);
			 break;
			 case 2:LcmPutDots(0xaa,0x55);
			 break;
			 case 3:LcmPutDots(0xaa,0xaa);
			 break;
			 case 4:LcmPutDots(0x55,0x55);
			 break;
			 case 5:LcmPutDots(0x00,0xff);
			 break;	
			 case 6:LcmPutDots(0xff,0x00);
			 break;	
			 case 7:LcmPutDots(0xff,0xff);
			 break;	
			 case 8:Display_character();
			 break;
			 case 9:Display_Chinese();
			 break;				 
		 }
			 
	 }

 }   
}   
  
