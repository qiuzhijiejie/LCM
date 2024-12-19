#include "lcd19264.h"
#include "picture.h"    
#include <REGX52.H>
sbit int0=P3^1;   
//unsigned char mode;
//   
//void main( void )   
//{      
//	Delay(20); 
//	IE=0x81;
//	IP=0x01;
//	TCON=0x01;
//	int0=1;  

//	LcmInit();   
//	LcmClear();  

//	LcmPutDots(0x55,0xaa);
//	Delay(10); 
//	LcmPutDots(0xaa,0x55);
//	Delay(10);
//	LcmPutDots(0x55,0x55);
//	Delay(10); 
//	LcmPutDots(0xaa,0xaa);
//	Delay(10); 
//	LcmPutDots(0xff,0x00);
//	Delay(10); 
//	LcmPutDots(0x00,0xff);
//	Delay(10);
//	LcmPutDots(0xff,0x55);
//	Delay(10);
//	LcmPutDots(0x55,0xff);
//	Delay(10);
//	LcmPutDots(0xff,0xff);
//	Delay(10);
//	Display_character();  
//	Delay(10);
//	LcmPutBMP(BMP0); 
//	Display_Chinese();
//	Delay(10);
//	
//	while(1)   
//	{
//		if(key())
//		{
//			mode++;
//			mode%=9;		    
//			switch(mode)
//			{
//			case 0:
//			LcmPutDots(0x55,0xaa);
//			break;
//			case 1:
//			LcmPutDots(0xaa,0x55);
//			break;	
//			case 2:
//			LcmPutDots(0x55,0x55);
//			break;	
//			case 3:
//			LcmPutDots(0xaa,0xaa);
//			break;	
//			case 4:
//			LcmPutDots(0xff,0x00);
//			break;	
//			case 5:
//			LcmPutDots(0x00,0xff);
//			break;	
//			case 6:
//			LcmPutDots(0x55,0xff);
//			break;	
//			case 7:
//			LcmPutDots(0xff,0x55);
//			break;	
//			case 8:
//			LcmPutBMP(BMP0); 
//			Display_Chinese();
//			break;	
//		}			
//					
//			
//		}

//	}   
//}   
  
  
void main(void)   
{

	Delay(20); 
	IE=0x81;
	IP=0x01;
	TCON=0x01;
	int0=1;  
	LcmInit();   
	LcmClear(); 
    while (1)  
    {   
        if (key()) 
        {
            HandleKey(); 
        } 
        else 
        {
          
            DisplayPatterns(); 
        }
    }   
}
  
  
