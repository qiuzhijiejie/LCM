#include <reg51.h>
#include <intrins.h>
#include "16032.h"
#include "picture.h"
unsigned char mode;

void main(void)
{ 
	
	lcdreset();                   
    Clean(' '); 
	charlcdfill(0x10);
	exdelay(); 
    Clean(' '); 
	fillLCD_OddColumn(0xFF, 0x00);
	exdelay(); 
	fillLCD_OddColumn(0x00, 0xff);
	exdelay(); 
	fillLCD_OddColumn(0xaa, 0x55);
	exdelay(); 
	fillLCD_OddColumn(0x55, 0xaa);
	exdelay(); 
	fillLCD_OddColumn(0xaa, 0xaa);
	exdelay(); 
	fillLCD_OddColumn(0x55, 0x55);
	exdelay();
	fillLCD_OddColumn(0xff, 0x55);
	exdelay();
	fillLCD_OddColumn(0x55, 0xff);
	exdelay();
	fillLCD_OddColumn(0x00, 0x00);
	putstrxy(0,0,"广东烨辉科技有限公司"); 	
    putstrxy(3,1," LCM16032_03 ");   

while(1)
    { 
			
	if (ReadKey()) 
{
    mode++;
    mode %= 10;
    Clean(' ');
	fillLCD_OddColumn(0x00, 0x00);
	lcdreset(); 
    switch (mode) 
	
	{
        case 0:
            fillLCD_OddColumn(0xff, 0x00);
            break;
        case 1:
            fillLCD_OddColumn(0x00, 0xFF);
            break;
        case 2:
            fillLCD_OddColumn(0xAA, 0x55);
            break;
        case 3:
            fillLCD_OddColumn(0x55, 0xAA);
            break;
		case 4:
            fillLCD_OddColumn(0xAA, 0xAA);
		   break;
	    case 5:
            fillLCD_OddColumn(0x55, 0x55);
            break;
        case 6:
            fillLCD_OddColumn(0xFF, 0x55);
            break;
        case 7:
            fillLCD_OddColumn(0x55, 0xFF);
            break;
		case 8:
            fillLCD_OddColumn(0xFF, 0xFF);
            break;
        case 9:
            putstrxy(0,0,"广东烨辉科技有限公司"); 
			putstrxy(3,1," LCM16032_03 ");  
            break;

	}
}		
    }
}

