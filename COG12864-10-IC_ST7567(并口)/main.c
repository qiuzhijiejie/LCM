#include <REGX52.H>
#include "12864.h"
#include "photo.h"
 
void main()   
{ 
	
//	unsigned char mode;
	Init_IC();   
    lcd_test();  
	Yehui_UI();
	delayMs(3000);
    Clear();
	  
//	whatsapp();
//	delayMs(5000); 
//	Clear();	  
	lcd_describe();
	delayMs(3000);  
    Clear();
	  
//    display_photo(tupian,0,0);	  
// 	delayMs(3000);
//    Clear();
//	  
//	trans_display_photo(tupian1,0,0);	  
//	delayMs(3000);

  while(1)
  {
	 	  
	unsigned char mode;  	  
	if (HOLD()) 
	{  
    mode++;           // 先增加 mode
    mode %= 8;        // 让 mode 保持在 0~7 之间
	Clear();
    switch (mode) {
        case 0:  
            display_test(0xaa, 0xaa);
            break;
        case 1:            
            Yehui_UI();		
            break;
        case 2:
            display_test(0x55, 0x55);  
            break;
        case 3:
            display_test(0x55, 0xaa);  
            break;
        case 4:
            display_test(0xaa, 0x55);		
            break;
        case 5:
            display_test(0xff, 0x00);	 
            break;
        case 6:
            display_test(0x00, 0xff);	
            break;
        case 7:
            display_test(0xff, 0xff);	
            break;

    }
}
	  
  }    
}   