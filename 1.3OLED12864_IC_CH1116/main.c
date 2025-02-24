#include <reg51.h>
#include "oled12864.h"
#include "picture.h"

void main()
{
	Delay(10);
	Initial();
	
	PerformScreenFillSequence();
	Delay(05);
	Display_Sequence();
	Delay(05);
	Display_Sequence2();

  while(1)
 {
	
	if (ReadKey()) 
   {
	unsigned char mode;
    mode++;
    mode %= 7;
    switch (mode) 
	{
        case 0:Screen_FillClear(0xAA);break;
        case 1:Screen_FillClear(0x55);break;
        case 2:OLED_fullCOL(0x55,0xaa);break;
        case 3:OLED_fullCOL(0xaa,0x55);break;
		case 4:OLED_fullCOL(0xff,0x00);break;
		case 5:OLED_fullCOL(0x00,0xff);break;
		case 6:Display_Sequence();break;

    }
}     
		 }
	 }


