#include "picture.h"
#include "242oled.h"
unsigned int mode;

int main()
{ 
	IO_init();
	OLED_init();
	
	OLED_fullROW(0xaa);
	OLED_fullROW(0x55);	
	OLED_full();
	OLED_fullCOL(0xff,0x00);
	OLED_fullCOL(0x00,0xff);
	OLED_fullCOL(0x55,0xaa);
	OLED_fullCOL(0xaa,0x55);
	display2();
	display3();	
	display1(); 
 while(1)
{
	
	if (ReadKey1()) {
    mode++;
    mode %= 10;
    switch (mode) {
        case 0:
            display1();
            break;
        case 1:
            display2();
            break;
        case 2:
			OLED_fullROW(0x55);
            break;
        case 3:
			OLED_fullROW(0xaa);
            break;
		case 4:
            OLED_full();
            break;
		case 5:
            OLED_fullCOL(0xff,0x00);
            break;
		case 6:
            OLED_fullCOL(0x00,0xFF);
            break;
		case 7:
            OLED_fullCOL(0x55,0xAA);
            break;
		case 8:
            OLED_fullCOL(0xAA,0x55);
            break;
		case 9:
			display3();
            break;

    }
}
	}
}