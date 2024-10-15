#include <REGX52.H>
#include "lcd12864.h"
#include "picture.h"

unsigned char mode;

void main(void)
{
	

	Lcd12864spi_init();            // ≥ı ºªØLCD
	Display_Clear();
	charlcdfill(' ');
	
	FillScreenWithPattern(0xff); 
	FillCheckerboardPattern();
	FillVerticalStripePattern();
	FillHorizontalStripePattern();
	FillDiagonalLinePattern();

	FillScreenWithPattern(0x00);

    drawimge(Img_SUNMAN_128x64);




while (1)
{
		if(ReadKey1())
		mode++;
		mode%=6;
		switch(mode){
			case 0:FillCheckerboardPattern();
			break;
			case 1:FillVerticalStripePattern();
			break;
			case 2:FillHorizontalStripePattern();
			break;
			case 3:FillDiagonalLinePattern();
			break;
			case 4:FillScreenWithPattern(0xff);
			break;
			case 5:drawimge(Img_SUNMAN_128x64);;
}		
	
	}


} 

