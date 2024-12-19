#include <REGX52.H>
#include "lcd240128.h"
#include "picture.h"
#include <stdio.h>


//void main( void )   
//{
//	LCD_EN=0;  
//	LCD_FS=0; 
//	LcmInit(); 
//	LcmClear();

//	if(ReadKey1())
//	mode++;
//	mode%=10;
//	LcmClear();
//	switch(mode){	
//	case 0:DisplayDots(0xff,0x00);
//	Delay(2000);	
//	break;
//	case 1:DisplayDots(0x55,0xaa);
//	Delay(2000);	
//	break;
//	case 2:DisplayDots(0x55,0x55);
//	Delay(2000);	
//	break;
//	case 3:DisplayDots(0x00,0xff);
//	Delay(2000);	
//	break;
//	case 4:DisplayDots(0xaa,0xaa);
//	Delay(2000);	
//	break;
//	case 5:picture();
//	Delay(2000);		
//	break;
//	case 6:Display_FullCGROM();
//	Delay(2000);		
//	break;
//	case 7:DisplayBMP(0,0,240,128,BMP1); 
//	Delay(2000);
//	break;
//	case 8:ReverseDisplayBMP(0,0,240,128,BMP1);
//	Delay(2000);
//	break;
//	case 9:ReverseDisplayBMP(0,0,240,128,BMP0); 
//	Delay(2000);
//	break;

//}

// while(1)  
//{
//	DisplayPatterns(); 
// }   
//}




void main(void)   
{
    LCD_EN = 0;  
    LCD_FS = 0; 
    LcmInit(); 
    LcmClear();

    while (1)  
    {
      
        if (ReadKey1()) 
        {
            HandleKey(); 
        } 
        else 
        {
          
            DisplayPatterns(); 
        }
    }   
}
