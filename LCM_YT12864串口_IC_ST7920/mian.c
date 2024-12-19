#include <REGX52.H>
#include "lcd12864.h"
#include "picture.h"

//unsigned char mode;

//void main(void)
//{
//	

//	Lcd12864spi_init();            // 初始化LCD
//	Display_Clear();
//	charlcdfill(' ');
//	
//	FillScreenWithPattern(0xff); 	
//	FillCheckerboardPattern();
//	ReverseFillCheckerboardPattern();	
//	FillVerticalStripePattern();
//	ReverseFillVerticalStripePattern();	
//	ReverseFillHorizontalStripePattern();
//	FillHorizontalStripePattern();	
//	FillDiagonalLinePattern();		
//	FillScreenWithPattern(0x00);
//	drawimge(Img_SUNMAN_128x64);
//	
//	
//	DisplaystringAtposition(0,1,"烨辉科技有限公司");
//	DisplaystringAtposition(2,2,"YT-LCD12864");


//while (1)
//{
//		if(ReadKey1())
//		mode++;
//		mode%=8;
//	    Display_Clear();
//	    charlcdfill(' ');
//		switch(mode){
//			case 0:FillCheckerboardPattern();
//			break;
//			case 1:ReverseFillCheckerboardPattern();
//			break;
//			case 2:ReverseFillVerticalStripePattern();
//			break;
//			case 3:FillVerticalStripePattern();
//			break;
//			case 4:ReverseFillHorizontalStripePattern();
//			break;
//			case 5:FillHorizontalStripePattern();
//			break;
//			case 6:FillDiagonalLinePattern();
//			break;
//			case 7:FillScreenWithPattern(0xff);
//}		
//	
//	}
//} 


void main(void)   
{

	Lcd12864spi_init();           
	Display_Clear();
	charlcdfill(' ');
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