/*************************************************************************  
Driver IC    :  ST7565R / UC1701x 
interface    :  8080 for 8 bit mcu interface 
************************************************************************/   
#include <REG52.H>    
#include "int/photo.c"
#include "12864.h"
 
void main()   
{ 

  while(1)
  {
	Init_IC();   
    lcd_test();  
//	Yehui_UI();
	delayMs(300);
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
	Init_IC();
	
	  
  }    
}   