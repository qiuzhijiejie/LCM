#include <REGX52.H>
#include "picture.h"
#include "lcd12232.h"

unsigned char mode;
void main(void)
{
//    Initial();
//	Delay(50);
//	Display_dot(0x00,0x00);
//	Delay(2000);
//	Display_dot(0x55,0xAA);
//	Delay(2000);
//	Display_dot(0xaa,0x55);
//	Delay(2000);
//	Display_dot(0x00,0xff);
//	Delay(2000);
//	Display_dot(0xff,0x00);
//	Delay(2000);
//	Display_dot(0x55,0x55);
//	Delay(2000);
//	Display_dot(0xaa,0xaa);
//	Delay(2000);
//	Display_dot(0x00,0x00);
//	show_chinese();
	while(1)
	{			
	Initial();
	Delay(50);
	Display_dot(0x00,0x00);

//	Delay(8000);
//	show_chinese();
	DisplayLineEnglishEx(0, 1, 0, 13);
	Delay(8000);
	DisplayLineEnglishEx(2, 3, 13, 8);
	Delay(8000);
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

		
		
//	  	if(ReadKey1())
//		mode++;
//		mode%=7;
//		switch(mode){	
//		case 0:Display_dot(0xAA,0x55);
//		break;
//		case 1:Display_dot(0x55,0xaa);
//		break;
//		case 2:Display_dot(0x55,0x55);
//		break;
//		case 3:Display_dot(0x00,0xff);
//		break;
//		case 4:Display_dot(0xaa,0xaa);
//		break;
//		case 5:Display_dot(0xff,0xff);
//		break;
//		case 6:show_chinese();
//		break;
//}
	}
}


//void main(void)   
//{

//	Initial(); 
//	Delay(50);
//	Display_dot(0x00,0x00);
//	Delay(2000);

//    while (1)  
//    {
//      
//        if (ReadKey1()) 
//        {
//            HandleKey(); 
//        } 
//        else 
//        {
//          
//            DisplayPatterns(); 
//        }
//    }   
//}
