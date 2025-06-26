/*************************************************************************  
Driver IC    :  ST7565R / UC1701x 
interface    :  8080 for 8 bit mcu interface 
************************************************************************/   
#include <REG52.H>    
#include "int/photo.c"
#include "12864.h"
 
void main()   
{ 
	
	unsigned char mode;
//	Init_IC();   
//lcd_test();  
//delayMs(3000);
//Yehui_UI();
//delayMs(3000);
//Clear();

//whatsapp();
//delayMs(5000); 
//Clear();	  
//lcd_describe();
//delayMs(3000);  
//Clear();

//display_photo(tupian,0,0);	  
//delayMs(3000);
//Clear();

//trans_display_photo(tupian1,0,0);	  
//delayMs(3000);

  while(1)
  {
	Init_IC();
	lcd_test();  
	delayMs(1000);
	Yehui_UI();
	delayMs(1000);
	Clear();

	//	whatsapp();
	//	delayMs(5000); 
	//	Clear();	  
	lcd_describe();
	delayMs(1000);  
	Clear();

	display_photo(tupian,0,0);	  
	delayMs(1000);
	Clear();

	trans_display_photo(tupian1,0,0);	  
	delayMs(1000);
	  
	  
	  
//if (HOLD()) {  
//    mode++;           // 先增加 mode
//    mode %= 8;        // 让 mode 保持在 0~7 之间
//	Clear();
//    switch (mode) {
//        case 0:  
//            display_test(0xaa, 0xaa);
//            break;
//        case 1:            
//            Yehui_UI();		
//            break;
//        case 2:
//            display_test(0x55, 0x55);  
//            break;
//        case 3:
//            display_test(0x55, 0xaa);  
//            break;
//        case 4:
//            display_test(0xaa, 0x55);		
//            break;
//        case 5:
//            display_test(0xff, 0x00);	 
//            break;
//        case 6:
//            display_test(0x00, 0xff);	
//            break;
//        case 7:
//            display_test(0xff, 0xff);	
//            break;

//    }
//}
	  
  }    
}   