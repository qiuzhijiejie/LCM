#include "lcm19264.h"
#include "pic.h"
void main( void )   
{   
 unsigned char mode;	
 Delay(20);  
 IE=0x81;
 IP=0x01;
 TCON=0x01;
 int0=1;  

    LcmInit();   
    LcmClear(); 
	 
   LcmPutDots(0x55,0xaa);
   Delay(5000);
	 
   LcmPutDots(0xaa,0x55);
   Delay(5000);
	 
   LcmPutDots(0x55,0x55);
   Delay(5000); 
	 
   LcmPutDots(0xaa,0xaa);
   Delay(5000); 
	 
   LcmPutDots(0xff,0x00);
   Delay(5000); 
	 
   LcmPutDots(0x00,0xff);
   Delay(5000); 

   LcmPutDots(0xff,0xff);
   Delay(5000); 	

   LcmPutBMP(BMP1);
   Display_Chinese();
   Delay(5000);
   LcmClear(); 	
 while(1)   
 {

  
  		if(ReadKey())
		mode++;
		mode%=8;
//		LcmClear();
		switch(mode){	
		case 0:LcmPutDots(0xff,0x00);
		break;
		case 1:LcmPutDots(0x00,0xff);
		break;
		case 2:LcmPutDots(0x55,0x55);
		break;
		case 3:LcmPutDots(0xaa,0xaa);
		break;
		case 4:LcmPutDots(0xaa,0x55);
		break;
		case 5:LcmPutDots(0x55,0xaa);	
		break;
		case 6:LcmPutDots(0xff,0xff);	
		break;
		case 7:   LcmPutBMP(BMP1);Display_Chinese();	
		break;
   
//   Display_Chinese();
//   Delay(3000);  
//   Display_character();

 }   
}   
 }