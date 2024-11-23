#include "picture.h"
#include "242oled.h"
unsigned int mode;
const unsigned char  *point1;
const unsigned char  *point2;
const unsigned char  *point3;
const unsigned char  *point4;
const unsigned char  *point5;
int main()
{ 
	  point1= &picture_tab1[0];
	  point2= &picture_tab2[0];
	  point3= &picture_tab3[0];
	  point4= &picture_tab4[0];
	  point5= &picture_tab5[0];
	  IO_init();
	  OLED_init();	
	  OLED_dotted_pattern();
	  delay_ms(200);
	  OLED_dotted_Reversepattern();
	  display1();
	  display2();
	  display3();
	  display4();
	  display5();	  
	  OLED_full();
 while(1)
{
	
	if (ReadKey1()) {
    mode++;
    mode %= 8;
    switch (mode) {
        case 0:
            display1();
            break;
        case 1:
            display2();
            break;
        case 2:
            display3();
            break;
        case 3:
            display4();
            break;
		case 4:
            OLED_full();
            break;
		case 5:
            OLED_dotted_pattern();
            break;
		case 6:
            OLED_dotted_Reversepattern();
            break;
		case 7:
            display5();
            break;
    }
}

	}
}