#include <REGX52.H>
#include "LCD2004.h"


unsigned char mode=0;

int main() 
{
			LCD_Init(); 
			LCD_Clear();
			Write_DDRAM(0xd0); 
			Delay(30); 
            Write_DDRAM(0xd2);  
			Delay(30);
            Write_DDRAM(0xc2);  
			Delay(30);
            Write_DDRAM(0xcf);  
			Delay(30);
            Write_DDRAM(0xf2);  
			Delay(30);
            Write_DDRAM(0x8f);  
			Delay(30);
		    Write_DDRAM(0x2a); 
			Delay(30);
			write_CGROM(0x10);//ÄÚÖÃ
			Delay(30);
			Write_DDRAM(0xfa);
			
while (1) 
{

		if (ReadKey1()) {
		mode++;
		mode %= 8;
		switch (mode) {
        case 1:
            
			displayCustomCharacter1(); 
            break;
        case 2:
            Write_DDRAM(0xd2);  
            break;
        case 3:
            Write_DDRAM(0xc2);  
            break;
        case 4:
            Write_DDRAM(0xcf);  
            break;
        case 5:
            Write_DDRAM(0xf2);  
            break;
        case 6:
            Write_DDRAM(0x8f);  
            break;
		case 7:
		    Write_DDRAM(0x2a);
//			write_CGROM(0x10);
			  break;
        default:
            Write_DDRAM(0xfa);  
            break;
    
		}
	}			
}

	}

